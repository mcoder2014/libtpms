#pragma once

#include <qgl.h>
#include "SurfaceMeshModel.h"
#include "RenderObjectExt.h"
using namespace SurfaceMesh;

#define glVector3( v ) glVertex3d( v.x(), v.y(), v.z() )
#define glNormal3( v ) glNormal3d( v.x(), v.y(), v.z() )
#define glColorQt(c) glColor4d(c.redF(), c.greenF(), c.blueF(), c.alphaF())

struct QuickMeshDraw{

	static void drawMeshSolid( SurfaceMeshModel * mesh, QColor c = QColor(255,255,255,255), bool lighting = true )
	{
		if(!mesh) return;

		if(!mesh->has_face_property<Vector3>("f:normal") ||  !mesh->has_face_property<Vector3>("v:normal") )
		{
			mesh->update_face_normals();
			mesh->update_vertex_normals();
		}

		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if (lighting)
		{
			glEnable(GL_LIGHTING);
		}
		else
		{
			glDisable(GL_LIGHTING);
		}
	
		glColorQt(c);

		Surface_mesh::Vertex_property<Vector3> points = mesh->vertex_property<Vector3>("v:point");
		Surface_mesh::Face_property<Vector3> fnormals = mesh->face_property<Vector3>("f:normal");

		Surface_mesh::Face_iterator fit, fend = mesh->faces_end();
		Surface_mesh::Vertex_around_face_circulator fvit, fvend;
		
		glBegin(GL_TRIANGLES);
		for (fit=mesh->faces_begin(); fit!=fend; ++fit){
			glNormal3( fnormals[fit] );
			fvit = fvend = mesh->vertices(fit);
			do{ glVector3( points[fvit] ); } while (++fvit != fvend);
		}
		glEnd();

		glEnable(GL_LIGHTING);
	}

    static void drawMeshWireFrame(SurfaceMeshModel * mesh, QColor c = QColor(255, 255, 255, 255), bool lighting = true)
    {
        if (!mesh) return;

        Surface_mesh::Face_iterator fit, fend = mesh->faces_end();
        Surface_mesh::Vertex_around_face_circulator fvit, fvend;
        Surface_mesh::Vertex_property<Vector3> points = mesh->vertex_property<Vector3>("v:point");
        Surface_mesh::Face_property<Vector3> fnormals = mesh->face_property<Vector3>("f:normal");

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glColor4d(0, 0, 0, 0.25);
        glLineWidth(1.0f);
        //glEnable(GL_CULL_FACE);
        //glCullFace(GL_BACK);
        glDisable(GL_CULL_FACE);

        if (lighting)
        {
            glEnable(GL_LIGHTING);
        }
        else
        {
            glDisable(GL_LIGHTING);
        }

        glColorQt(c);

        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        for (fit = mesh->faces_begin(); fit != fend; ++fit) {
            glBegin(GL_POLYGON);
            glNormal3(fnormals[fit]);
            fvit = fvend = mesh->vertices(fit);
            do { glVector3(points[fvit]); } while (++fvit != fvend);
            glEnd();
        }
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        glDisable(GL_CULL_FACE);
    }

	//static void drawMeshWireFrame( SurfaceMeshModel * mesh )
	//{
	//	if(!mesh) return;

	//	Surface_mesh::Face_iterator fit, fend = mesh->faces_end();
	//	Surface_mesh::Vertex_around_face_circulator fvit, fvend;
	//	Surface_mesh::Vertex_property<Vector3> points = mesh->vertex_property<Vector3>("v:point");
	//	Surface_mesh::Face_property<Vector3> fnormals = mesh->face_property<Vector3>("f:normal");

	//	glEnable (GL_BLEND);
	//	glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	glColor4d(0,1,1, 0.25);
	//	glLineWidth(1.0f);
	//	//glEnable(GL_CULL_FACE);
	//	//glCullFace(GL_BACK);
	//	glDisable(GL_CULL_FACE);

	//	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	//	for (fit=mesh->faces_begin(); fit!=fend; ++fit){
	//		glBegin(GL_POLYGON);
	//		glNormal3( fnormals[fit] );
	//		fvit = fvend = mesh->vertices(fit);
	//		do{ glVector3( points[fvit] ); } while (++fvit != fvend);
	//		glEnd();
	//	}
	//	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	//	glDisable(GL_CULL_FACE);
	//}

	static void drawMeshName( SurfaceMeshModel * mesh, int name = 0 )
	{
        glPushName( static_cast<GLuint>(name) );

		Surface_mesh::Vertex_property<Vector3> points = mesh->vertex_property<Vector3>("v:point");
		Surface_mesh::Face_iterator fit, fend = mesh->faces_end();
		Surface_mesh::Vertex_around_face_circulator fvit, fvend;

		glBegin(GL_TRIANGLES);
		for (fit=mesh->faces_begin(); fit!=fend; ++fit){
			fvit = fvend = mesh->vertices(fit);
			do{ glVector3( points[fvit] ); } while (++fvit != fvend);
		}
		glEnd();

		glPopName();
	}

	static Eigen::Matrix4d getRotationMatrix(Vec3d newZ)
	{
		Eigen::Quaternion<double> q;
		q.setFromTwoVectors(newZ, Vec3d(0,0,1));
		Eigen::Matrix3d M = q.toRotationMatrix();
		Eigen::Matrix4d R;
		for (int i=0; i<3; i++)
		{
			for (int j=0; j<3; j++)
			{
				R(i, j) = M(i, j);
			}
			R(i, 3) = 0.0;
			R(3, i) = 0.0;
		}
		R(3, 3) = 1.0;
			
		return R;
	}

	static void drawMeshTransparent(SurfaceMeshModel* mesh, QColor color, bool showWeight = false, double maxWeight = 0.0)
	{
		glEnable (GL_BLEND);
		glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_LIGHTING);

		ScalarFaceProperty fweight = mesh->get_face_property<Scalar>("f:weight");
		Surface_mesh::Vertex_property<Vector3> points = mesh->vertex_property<Vector3>("v:point");
		Surface_mesh::Face_property<Vector3> fnormals = mesh->face_property<Vector3>("f:normal");

		if (!fweight.is_valid())
		{
			return;
		}

		Surface_mesh::Face_iterator f_it, f_end = mesh->faces_end();
		Surface_mesh::Vertex_around_face_circulator fv_it, fv_end;

		glBegin(GL_TRIANGLES);
		for (f_it=mesh->faces_begin(); f_it!=f_end; ++f_it){

			if (showWeight)
			{
				double w = fweight[f_it]/maxWeight;
				color = starlab::qtJetColor(w);
			}
			color.setAlphaF(0.2);
			glColorQt(color);

			glNormal3( fnormals[f_it] );
			fv_it = fv_end = mesh->vertices(f_it);
			do{ glVector3( points[fv_it] ); } while (++fv_it != fv_end);
		}
		glEnd();
		glEnable(GL_LIGHTING);
	}

	static void drawBoundingBox(Eigen::AlignedBox3d bbox, QColor color = Qt::black)
	{
		Vector3 minCorner = bbox.min();
		Vector3 maxCorner = bbox.max();

		glDisable(GL_LIGHTING);

		glBegin(GL_LINES);
		color.setAlphaF(0.2);
		glColorQt(color);
		glVector3(Vector3(minCorner[0], minCorner[1], minCorner[2])); glVector3(Vector3(maxCorner[0], minCorner[1], minCorner[2]));
		glVector3(Vector3(minCorner[0], minCorner[1], minCorner[2])); glVector3(Vector3(minCorner[0], maxCorner[1], minCorner[2]));
		glVector3(Vector3(minCorner[0], minCorner[1], minCorner[2])); glVector3(Vector3(minCorner[0], minCorner[1], maxCorner[2]));

		glVector3(Vector3(minCorner[0], maxCorner[1], maxCorner[2])); glVector3(Vector3(maxCorner[0], maxCorner[1], maxCorner[2]));
		glVector3(Vector3(minCorner[0], maxCorner[1], maxCorner[2])); glVector3(Vector3(minCorner[0], minCorner[1], maxCorner[2]));
		glVector3(Vector3(minCorner[0], maxCorner[1], maxCorner[2])); glVector3(Vector3(minCorner[0], maxCorner[1], minCorner[2]));

		glVector3(Vector3(maxCorner[0], maxCorner[1], minCorner[2])); glVector3(Vector3(minCorner[0], maxCorner[1], minCorner[2]));
		glVector3(Vector3(maxCorner[0], maxCorner[1], minCorner[2])); glVector3(Vector3(maxCorner[0], minCorner[1], minCorner[2]));
		glVector3(Vector3(maxCorner[0], maxCorner[1], minCorner[2])); glVector3(Vector3(maxCorner[0], maxCorner[1], maxCorner[2]));

		glVector3(Vector3(maxCorner[0], minCorner[1], maxCorner[2])); glVector3(Vector3(minCorner[0], minCorner[1], maxCorner[2]));
		glVector3(Vector3(maxCorner[0], minCorner[1], maxCorner[2])); glVector3(Vector3(maxCorner[0], maxCorner[1], maxCorner[2]));
		glVector3(Vector3(maxCorner[0], minCorner[1], maxCorner[2])); glVector3(Vector3(maxCorner[0], minCorner[1], minCorner[2]));
		glEnd();

		glEnable(GL_LIGHTING);
	}

	static void drawBoundingBox(Eigen::Matrix3Xd bboxMat, QColor color = Qt::black)
	{
		glDisable(GL_LIGHTING);

		glBegin(GL_LINES);
		color.setAlphaF(0.2);
		glColorQt(color);

		glVector3(bboxMat.col(0)); glVector3(bboxMat.col(1));
		glVector3(bboxMat.col(1)); glVector3(bboxMat.col(2));
		glVector3(bboxMat.col(2)); glVector3(bboxMat.col(3));
		glVector3(bboxMat.col(3)); glVector3(bboxMat.col(0));

		glVector3(bboxMat.col(4)); glVector3(bboxMat.col(5));
		glVector3(bboxMat.col(5)); glVector3(bboxMat.col(6));
		glVector3(bboxMat.col(6)); glVector3(bboxMat.col(7));
		glVector3(bboxMat.col(7)); glVector3(bboxMat.col(4));

		glVector3(bboxMat.col(0)); glVector3(bboxMat.col(4));
		glVector3(bboxMat.col(1)); glVector3(bboxMat.col(5));
		glVector3(bboxMat.col(2)); glVector3(bboxMat.col(6));
		glVector3(bboxMat.col(3)); glVector3(bboxMat.col(7));

		glEnd();
		
		glEnable(GL_LIGHTING);
	}
};
