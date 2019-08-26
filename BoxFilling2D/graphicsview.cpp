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

void GraphicsView::updateCuttingPoints(std::vector<std::vector<OpenMesh::Vec2f> > *cutting_points_2d)
{
    // clean old
    this->m_cutting_points.clear();

    // TODO: Delete the lines from the scene
    this->m_cutting_lines.clear();

    // Add new
    for(auto vec_points: *cutting_points_2d)
    {
        this->m_cutting_points.push_back(std::vector<QPointF>());
        auto& cutting_points =
                this->m_cutting_points[this->m_cutting_points.size()-1];

        for(auto vec_point:vec_points)
        {

            cutting_points.push_back(
                QPointF(static_cast<qreal>(vec_point[0]),
                        static_cast<qreal>(vec_point[1])));
        }
    }

    // Draw it on the graphicsview
    for(auto vec_qpoints: this->m_cutting_points)
    {
        this->m_cutting_lines.push_back(std::vector<QGraphicsLineItem*>());
        if(vec_qpoints.size() <2)
                break;
        std::vector<QGraphicsLineItem*>& lines = this->m_cutting_lines[this->m_cutting_lines.size()-1];

        for(unsigned long i = 0; i < vec_qpoints.size()-1; i++)
        {
            // Two points draw one line
            QGraphicsLineItem *line = this->scene()->addLine(
                        QLineF(vec_qpoints[i], vec_qpoints[i+1]));
            lines.push_back(line);
        }
    }
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
