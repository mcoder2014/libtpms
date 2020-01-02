#include "porousdesignplugin.h"

#include <QFileDialog>
#include <QDebug>

#include "controlwidget.h"
//#include "scene.h"

PorousDesignPlugin::PorousDesignPlugin()
    :SurfaceMeshModePlugin ()
{
//    this->m_curScene = nullptr;
    this->m_controlWidget = nullptr;
    this->m_controlDockWidget = nullptr;
}

PorousDesignPlugin::~PorousDesignPlugin()
{
//    if(m_curScene)
//    {
//        delete m_curScene;
//    }
}

void PorousDesignPlugin::create()
{
    if(m_controlWidget && m_controlDockWidget)
    {
        m_controlDockWidget->setVisible(true);
        return;
    }

//    // Check ptr
    if(m_controlWidget) delete m_controlWidget;
    if(m_controlDockWidget) delete m_controlDockWidget;

//    // Create new
    m_controlWidget = new ControlWidget(this);
    m_controlDockWidget = new ModePluginDockWidget(
                tr("Porous Design"), mainWindow());
    m_controlDockWidget->setWidget(m_controlWidget);

//    // stop some options
    m_controlDockWidget->setFeatures(QDockWidget::NoDockWidgetFeatures);
    mainWindow()->addDockWidget(Qt::RightDockWidgetArea, m_controlDockWidget);

//    // Create Scene
//    if(!m_curScene)
//        m_curScene = new Scene();

//    // qDebug()
    qDebug() << "Porous design plugin dock widget's size :\t"
             << m_controlWidget->width() << " " << m_controlWidget->height();
}

void PorousDesignPlugin::destroy()
{
    // Not real destroy
    std::cout<< "Porous design plugin destroy";

    if(m_controlDockWidget)
        m_controlDockWidget->setVisible(false);
}

void PorousDesignPlugin::decorate()
{
    // 装饰\渲染

//    if(m_curScene)
//    {
//        //
//    }

}

bool PorousDesignPlugin::mousePressEvent(QMouseEvent *mouseEvent)
{

}

bool PorousDesignPlugin::mouseMoveEvent(QMouseEvent *mouseEvent)
{

}

bool PorousDesignPlugin::mouseReleaseEvent(QMouseEvent *mouseEvent)
{

}
