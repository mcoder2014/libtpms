#include <memory>
#include <iostream>

#include <QCoreApplication>

#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>

#include <Mesh/Mesh>
#include <IO/Importer.h>

using namespace std;

int main(int argc, char *argv[])
{
    // 设置程序相关信息
    QCoreApplication a(argc, argv);
    QCoreApplication::setApplicationName(QObject::tr("calc-porosity"));
    QCoreApplication::setApplicationVersion(QObject::tr("0.1"));

    // 命令行工具
    QCommandLineParser parser;
    parser.setApplicationDescription( QObject::tr("Face Reconstruction"));
    parser.addHelpOption();             // --help
    //    parser.addVersionOption();          // --Version

    // image file
    QCommandLineOption porousPathOption(
                QStringList() << "p" << "porous",
                QCoreApplication::translate("main",
                                            "<filepath> filepath of porous model, e.g. type_p.ply" ),
                QCoreApplication::translate("porous", "porous"));
    std::cout << "Add option porous " << parser.addOption(porousPathOption);  // 添加选项到程序

    QCommandLineOption boundaryPathOption(
                QStringList() << "b" << "boundary",
                QCoreApplication::translate("main",
                                            "<filepath> filepaht of boundary model, e.g. insole.ply"));
    // 告诉它后面跟随参数
    boundaryPathOption.setValueName(QCoreApplication::translate("boundary", "boundary"));
    boundaryPathOption.setDefaultValue("");
    std::cout << "Add option boundary " << parser.addOption(boundaryPathOption);

    // Process the actual command line arguments given by the user
    parser.process(a);
    std::cout << "boundary isset: " << parser.isSet(boundaryPathOption)
              << " porous isset: " <<parser.isSet(porousPathOption)
              << std::endl;

    QString porousPath = parser.value(porousPathOption);
    QString boundaryPath = parser.value(boundaryPathOption);

    for(int i=0; i < argc; i++)
    {
        std::cout << argv[i] << " ";
    }
    std::cout << std::endl;

    cout << "Porous Path: " << porousPath.toStdString()
         << "\nBoundary Path: " << boundaryPath.toStdString()
         << std::endl;

    Importer importer;
    PorosityCalculator porosityCalculator;

    if(boundaryPath.size() > 0)
    {
        std::cout << "Boundary Mode." << std::endl;

        Mesh porousMesh = importer.loadMesh(porousPath.toStdString());
        Mesh boundaryMesh = importer.loadMesh(boundaryPath.toStdString());

        float porosity = porosityCalculator.getPorsity(porousMesh, boundaryMesh);
        std::cout << "Porosity: " << porosity << std::endl;
    }
    else if(porousPath.size() > 0)
    {
        std::cout << "BoundingBox Mode." << std::endl;

        Mesh porousMesh = importer.loadMesh(porousPath.toStdString());
        float porosity = porosityCalculator.getVolume(porousMesh);
        std::cout << "Porosity: " << porosity << std::endl;

    }
    else
    {
        std::cout << "Error! Whithout enough parameter" << std::endl;
    }

    return 0;
}
