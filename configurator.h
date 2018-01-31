#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QObject>
#include <QCoreApplication>
#include <QSettings>
#include <signal.h>
#include <QFile>
#include "tcpserver.h"
#include "kays.h"

class Configurator : public QObject
{
    Q_OBJECT
public:
    explicit Configurator(QObject *parent = 0);
    ~Configurator();
    void off(void);
private:
    QString ip;
    void getStendConfig();

    QString fileName;

    int tcpPort;
    int timeLife;
    int timeTrap;

    MyTcpServer *tcpServer;
    KAUS *kaus;



    QThread *workerThreadTcp;
    QThread *worker;

};
#endif // CONFIGURATOR_H
