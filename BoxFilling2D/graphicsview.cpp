#include "graphicsview.h"

#include <iostream>

#include <QApplication>
#include <QGraphicsScene>
#include <QMouseEvent>

GraphicsView::GraphicsView(QWidget *parent)
    : QGraphicsView (parent), m_scale(1)
{
    this->setScene(new QGraphicsScene);
}

void GraphicsView::updateCuttingPoints(std::vector<std::vector<OpenMesh::Vec2f> > *points2d)
{
    // clean old
    this->cuttingPoints.clear();

    // TODO: Delete the lines from the scene
    this->cuttingLines.clear();

    this->scene()->clear();

    // Add new
    for(auto vec_points: *points2d)
    {
        this->cuttingPoints.push_back(std::vector<QPointF>());
        auto& cutting_points =
                this->cuttingPoints[this->cuttingPoints.size()-1];

        for(auto vec_point:vec_points)
        {

            cutting_points.push_back(
                QPointF(static_cast<qreal>(vec_point[0]),
                        static_cast<qreal>(vec_point[1])));
        }
    }

    // Draw it on the graphicsview
    for(auto vec_qpoints: this->cuttingPoints)
    {
        this->cuttingLines.push_back(std::vector<QGraphicsLineItem*>());
        if(vec_qpoints.size() <2)
                break;
        std::vector<QGraphicsLineItem*>& lines = this->cuttingLines[this->cuttingLines.size()-1];

        for(unsigned long i = 0; i < vec_qpoints.size()-1; i++)
        {
            // Two points draw one line
            QGraphicsLineItem *line = this->scene()->addLine(
                        QLineF(vec_qpoints[i], vec_qpoints[i+1]));
            lines.push_back(line);
        }
    }

    this->updateGeometry();
}

void GraphicsView::updatePointCloud(std::vector<OpenMesh::Vec2f> &points2d)
{
    this->scene()->clear();

    for(OpenMesh::Vec2f point:points2d) {
        QPointF qpoint(point[0], point[1]);
        this->scene()->addLine(QLineF(qpoint, qpoint));
    }

    this->updateGeometry();
}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if ( QApplication::keyboardModifiers () == Qt::ControlModifier)
    {
        QMatrix matrix;

        if(event->delta() > 0)
        {
            // TODO: zoom out / smaller
            std::cout << "ctrl + wheel up" << std::endl;
            m_scale *= 0.8;
        }
        else
        {
            // TODO: zoom in / bigger
            std::cout << "ctrl + wheel down" << std::endl;
            m_scale *= 1.25;
        }
        matrix.scale(m_scale, m_scale);
        this->setMatrix(matrix);
    }
    QGraphicsView::wheelEvent(event);
}
