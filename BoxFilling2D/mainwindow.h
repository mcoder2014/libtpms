#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "ui_mainwindow.h"

#include "meshcutting.h"

class MainWindow : public QMainWindow, public Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    // give a dialog, let user choose a insole file
    void dialogOpenModelFile();
    bool openModelFile(const QString& filepath);

    void dialogSettingCuttingPlane();
    void cuttingMesh();
    void dialogRangeProjector();

private:
    MeshCutting meshcutter;
};

#endif // MAINWINDOW_H
