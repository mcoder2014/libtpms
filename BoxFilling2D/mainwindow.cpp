#include "mainwindow.h"


#include <QFileDialog>
#include <QDebug>
#include <QAction>

#include <UI/cuttingplanesettingdialog.h>
#include <UI/rangeprojectdialog.h>

#include "FootProjector.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),meshcutter()
{
    setupUi(this);

    // Dialog Open and input insole model file
    connect(action_open_insole_file, SIGNAL(triggered()),
            this, SLOT(dialogOpenModelFile()));

    // Help and about Qt button
    connect(action_about_Qt, SIGNAL(triggered()),
            QApplication::instance(), SLOT(aboutQt()));

    // Dialog
    connect(action_set_cutting_flat, SIGNAL(triggered()),
            this, SLOT(dialogSettingCuttingPlane()));

    // Cutting
    connect(action_cutting_mesh, SIGNAL(triggered()),
            this, SLOT(cuttingMesh()));

    // projector
    connect(action_range_projector, SIGNAL(triggered()),
            this, SLOT(dialogRangeProjector()));

}

MainWindow::~MainWindow()
{
}

void MainWindow::dialogOpenModelFile()
{
    QFileDialog * fileDialog = new QFileDialog(this);
    fileDialog->setWindowIcon(QIcon(":/icons/open.png"));
    fileDialog->setAcceptMode(QFileDialog::AcceptOpen);         // 设置对话框为打开文件类型
    fileDialog->setFileMode(QFileDialog::ExistingFile);         // 设置文件对话框能够存在的文件
    fileDialog->setViewMode(QFileDialog::Detail);               // 文件以细节形式显示出来
    fileDialog->setNameFilter(tr("3D model files(*.obj *.stl *.off *.ply)"));            // 设置文件过滤器
    fileDialog->setWindowTitle(tr("Choose an obj document file!"));

    if(fileDialog->exec() == QDialog::Accepted)
    {
        QString path = fileDialog->selectedFiles().first();      // 用户选择文件名
        qDebug() << "QFileDialog selected filepath: " <<path;

        // 调用打开文件
        if(!this->openModelFile(path))
        {
            // Load Mesh failed
            std::cout << "Loading Mesh failed! " << path.toStdString() << std::endl;
        }
        else
        {
            // Load Mesh success
            std::cout << "Loading Mesh succeed! " << path.toStdString() << std::endl;

        }
    }

    delete fileDialog;
}

///
/// \brief MainWindow::open_insole_file
/// \param filepath
/// \return
///
bool MainWindow::openModelFile(const QString &filepath)
{
    qDebug() << "open_insole_file filepath:" << filepath;
    return meshcutter.loadMesh(filepath.toStdString());
}

void MainWindow::dialogSettingCuttingPlane()
{
    // Open Dialog, getting setting

    CuttingPlaneSettingDialog settingDialog(this);

    // Test setting cutting plane function
//    m_meshcutter.setCuttingPlane(OpenMesh::Vec3f(0,0,4),
//                                 OpenMesh::Vec3f(1,0,4),
//                                 OpenMesh::Vec3f(0,1,4));

    // After Dialog executed, apply the setting
    if(settingDialog.exec())
    {
        std::cout << "Cutting plane setting dialog" << std::endl;
        std::vector<OpenMesh::Vec3f> plane = settingDialog.getSettingFromThreePoints();

        // setting the cutting plane
        meshcutter.setCuttingPlane(plane[0],plane[1],plane[2]);
    }
}

void MainWindow::cuttingMesh()
{
    // Test
    this->meshcutter.cutting();
    this->meshcutter.printCuttingResult();

    // Draw
    graphicsView->updateCuttingPoints(this->meshcutter.getCuttingPoints2d());
}

void MainWindow::dialogRangeProjector()
{
    RangeProjectDialog rangeSettingDialog(this);
    if(rangeSettingDialog.exec()) {
        assert(meshcutter.mesh() != nullptr);

        double top = rangeSettingDialog.rangeTop->value();
        double bottom = rangeSettingDialog.rangeBottom->value();

        FootProjector footProjector;
        footProjector.setModel(meshcutter.mesh());
        footProjector.setRange(top, bottom);
        std::vector<OpenMesh::Vec2f> points2d = footProjector.getPoints2d();
        graphicsView->updatePointCloud(points2d);
    }
}
