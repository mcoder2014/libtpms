#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <vector>

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsLineItem>
#include <QVector>
#include <QPointF>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/IO/MeshIO.hh>

class GraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit GraphicsView(QWidget *parent = nullptr);

signals:

public slots:
    void updateCuttingPoints(std::vector<std::vector<OpenMesh::Vec2f>>* points2d);
    void updatePointCloud(std::vector<OpenMesh::Vec2f>& points2d);

protected:

    virtual void wheelEvent(QWheelEvent *event) override;
    // 缩放视野
    void scaleView(qreal scaleFactor);
    qreal m_scale;


private:
    std::vector<std::vector<QPointF>> cuttingPoints;
    std::vector<std::vector<QGraphicsLineItem*>> cuttingLines;
};

#endif // GRAPHICSVIEW_H
