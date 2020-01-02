#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>

#include "porousdesignplugin.h"

namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(PorousDesignPlugin *m_plugin, QWidget *parent = nullptr);
    ~ControlWidget();

private:
    Ui::ControlWidget *ui;
    PorousDesignPlugin *m_plugin;
};

#endif // CONTROLWIDGET_H
