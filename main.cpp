#include <QCoreApplication>
#include "main.h"
#include <QTextStream>
#include <iostream>
#include <QString>
#include <QtGlobal>

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    configClass = new Configurator();
    return a.exec();
}
