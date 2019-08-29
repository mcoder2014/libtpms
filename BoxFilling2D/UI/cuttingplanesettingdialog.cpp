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
    std::vector<OpenMesh::Vec3f> points(3);

    if(ui->radioButton->isChecked())
    {
        // XOY
    }
    else if (ui->radioButton_2->isChecked())
    {
        // YOZ
    }
    else if (ui->radioButton_3->isChecked())
    {
        // XOZ
    }

    return points;
}
