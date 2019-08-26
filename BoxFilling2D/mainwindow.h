#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "meshcutting.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    // give a dialog, let user choose a insole file
    void dialog_open_insole_file();
    bool open_insole_file(const QString& filepath);

    void dialog_set_cutting_plane();

private:
    Ui::MainWindow *ui;
    MeshCutting m_meshcutter;
};

#endif // MAINWINDOW_H
