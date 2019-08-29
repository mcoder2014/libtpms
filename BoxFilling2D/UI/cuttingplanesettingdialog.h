#ifndef CUTTINGPLANESETTINGDIALOG_H
#define CUTTINGPLANESETTINGDIALOG_H

#include <vector>

#include <QDialog>

#include <OpenMesh/Core/Mesh/PolyMesh_ArrayKernelT.hh>

namespace Ui {
class CuttingPlaneSettingDialog;
}

class CuttingPlaneSettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CuttingPlaneSettingDialog(QWidget *parent = nullptr);
    ~CuttingPlaneSettingDialog();
    std::vector<OpenMesh::Vec3f> getSettingFromThreePoints();

private:
    Ui::CuttingPlaneSettingDialog *ui;
};

#endif // CUTTINGPLANESETTINGDIALOG_H
