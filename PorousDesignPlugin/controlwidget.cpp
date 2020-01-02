#include "controlwidget.h"
#include "ui_controlwidget.h"

ControlWidget::ControlWidget(PorousDesignPlugin *m_plugin,QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ControlWidget)
{
    ui->setupUi(this);
}

ControlWidget::~ControlWidget()
{
    delete ui;
}
