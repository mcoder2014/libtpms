#include <QCoreApplication>
#include <iostream>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>

#include <SurfaceMeshModel.h>

#include <marchbox.h>
#include <smoothtool.h>
#include <IO/importer.h>
#include <IO/exporter.h>
#include <generalporositycalculator.h>

///
/// 将鞋垫模型转换成 TPMS 结构
///

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

}
