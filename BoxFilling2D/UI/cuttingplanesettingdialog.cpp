#include "cuttingplanesettingdialog.h"
#include "ui_cuttingplanesettingdialog.h"

CuttingPlaneSettingDialog::CuttingPlaneSettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CuttingPlaneSettingDialog)
{
    ui->setupUi(this);
}

CuttingPlaneSettingDialog::~CuttingPlaneSettingDialog()
{
    delete ui;
}

std::vector<OpenMesh::Vec3f> CuttingPlaneSettingDialog::getSettingFromThreePoints()
{
    std::vector<OpenMesh::Vec3f> points;

    if(ui->radioButton->isChecked())
    {
        // XOY
        double zValue = ui->doubleSpinBox->value();
        points.push_back(OpenMesh::Vec3f(0,0,zValue));
        points.push_back(OpenMesh::Vec3f(0,1,zValue));
        points.push_back(OpenMesh::Vec3f(1,1,zValue));
    }
    else if (ui->radioButton_2->isChecked())
    {
        // YOZ
        double xValue = ui->doubleSpinBox->value();
        points.push_back(OpenMesh::Vec3f(xValue, 0, 0));
        points.push_back(OpenMesh::Vec3f(xValue, 0, 1));
        points.push_back(OpenMesh::Vec3f(xValue, 1, 1));
    }
    else if (ui->radioButton_3->isChecked())
    {
        // XOZ
        double yValue = ui->doubleSpinBox->value();
        points.push_back(OpenMesh::Vec3f(0, yValue, 0));
        points.push_back(OpenMesh::Vec3f(0, yValue, 1));
        points.push_back(OpenMesh::Vec3f(1, yValue, 1));
    }

    return points;
}
