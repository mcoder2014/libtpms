#include <QCoreApplication>
#include <iostream>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>


void setOptions(QList<QCommandLineOption>& options);

/// TODO: 命令行工具，将网格模型转换为 TPMS 多孔结构
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

    QList<QCommandLineOption> options;

    // model file
    QCommandLineOption modelPathOption(
                QStringList() << "m" << "model",
                QCoreApplication::translate("main",
                                            "<filepath> filepath of 3D model to convert, e.g. type_p.ply" ),
                QCoreApplication::translate("model", "model"));
    std::cout << "Add option porous " << parser.addOption(modelPathOption);  // 添加选项到程序

    // save porosus path
    QCommandLineOption savePorosusPathOption(
                QStringList() << "p" << "porosus",
                QCoreApplication::translate("main", "<filepath> filepath to save the converting result e.g. tpms.ply"),
                QCoreApplication::translate("porosus","porosus"));
    parser.addOption(savePorosusPathOption);


    // type

    parser.process(a);

}
