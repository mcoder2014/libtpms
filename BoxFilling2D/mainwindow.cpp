#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QAction>

#include <UI/cuttingplanesettingdialog.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_meshcutter()
{
    ui->setupUi(this);

    // Dialog Open and input insole model file
    connect(ui->action_open_insole_file, SIGNAL(triggered()),
            this, SLOT(dialog_open_insole_file()));

    // Help and about Qt button
    connect(ui->action_about_Qt, SIGNAL(triggered()),
            QApplication::instance(), SLOT(aboutQt()));

    // Dialog
    connect(ui->action_set_cutting_flat, SIGNAL(triggered()),
            this, SLOT(dialog_set_cutting_plane()));

    // Cutting
    connect(ui->action_cutting_mesh, SIGNAL(triggered()),
            this, SLOT(cutting_mesh()));

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::dialog_open_insole_file()
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
        QString path = fileDialog->selectedFiles()[0];      // 用户选择文件名
        qDebug() << "QFileDialog selected filepath: " <<path;

        // 调用打开文件
        if(!this->open_insole_file(path))
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
bool MainWindow::open_insole_file(const QString &filepath)
{
    qDebug() << "open_insole_file filepath:" << filepath;
    return m_meshcutter.loadMesh(filepath.toStdString());
}

void MainWindow::dialog_set_cutting_plane()
{
    // Open Dialog, getting setting

    CuttingPlaneSettingDialog settingDialog(this);

    // Test setting cutting plane function
    m_meshcutter.setCuttingPlane(OpenMesh::Vec3f(0,0,4),
                                 OpenMesh::Vec3f(1,0,4),
                                 OpenMesh::Vec3f(0,1,4));

    // After Dialog executed, apply the setting
    if(settingDialog.exec())
    {
        //
        std::cout << "Cutting plane setting dialog" << std::endl;
    }
}

void MainWindow::cutting_mesh()
{
    // Test
    this->m_meshcutter.cutting();
    this->m_meshcutter.printCuttingResult();

    // Draw
    ui->graphicsView->updateCuttingPoints(this->m_meshcutter.getCuttingPoints2d());
}
