#include "configurator.h"
#include "QDateTime"
#include "main.h"


#define CONFIG_FILE "./config.ini"



Configurator::Configurator(QObject *parent): QObject(parent)
{
    qDebug("Старт конфигуратора");


    getStendConfig();
    if (!ip.isEmpty() and tcpPort and !fileName.isEmpty())
    {


        kaus = new KAUS(timeLife, timeTrap, fileName);
        tcpServer = new MyTcpServer(ip,tcpPort);


        QObject:: connect(tcpServer, SIGNAL(message(QByteArray)), kaus, SLOT(checkMessage(QByteArray))); //tcp в сокет в xml
        QObject:: connect(kaus, SIGNAL(writeToSocket(QByteArray )), tcpServer, SLOT(writeS(QByteArray ))); //отправка сообщение в tcp
        QObject:: connect(kaus, SIGNAL(stopConnection()), tcpServer, SLOT(stopConnection())); //рвем соединение
    }
    else
    {
        printf("Не хватает параметров для установки сервера\n");
        exit (1);
    }

}

Configurator::~Configurator()
{
    delete tcpServer;
}

void Configurator::off(void)
{

    delete tcpServer;

}

void Configurator::getStendConfig()
{
    if(!QFile::exists(QString(CONFIG_FILE)))
    {
        qFatal("Не найден файл конфигурации стенда %s",CONFIG_FILE);
    }
    else
    {
        qWarning("Загрузка конфигурации стенда из %s...",CONFIG_FILE);

        QSettings settings(CONFIG_FILE,QSettings::IniFormat);
        ip = settings.value(QString("ip"),QString("NULL")).toString();
        tcpPort = settings.value(QString("port"),QString("NULL")).toInt();
        timeLife = settings.value(QString("timeLife"),QString("9")).toInt() * 1000;
        timeTrap = settings.value(QString("timeTrap"),QString("2")).toInt();
        fileName = settings.value(QString("file"),QString("NULL")).toString();

    }
}


