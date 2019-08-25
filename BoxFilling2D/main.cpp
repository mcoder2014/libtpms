#include "mainwindow.h"
#include <QApplication>
#include <QScreen>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    // Resize the init size of the application
    QScreen *primaryScreen = QApplication::primaryScreen();
    float width_factor = 0.7f;
    float height_factor = width_factor;
    w.resize(static_cast<int>(primaryScreen->size().width() * width_factor),
             static_cast<int>(primaryScreen->size().height() * height_factor));

    // Change icon
    w.setWindowIcon(QIcon(":/icons/feet.png"));
    w.setWindowTitle("Bounding Fill 2D (Demo by Chaoqun Jiang)");

    return a.exec();
}
