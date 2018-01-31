#include "tcpserver.h"

MyTcpServer::MyTcpServer(QString ip,int port, QObject *parent) : QObject(parent)
{
    server = new QTcpServer(this);
    // whenever a user connects, it will emit signal
    connect(server, SIGNAL(newConnection()),this, SLOT(newConnection()));
    QHostAddress ip_addr(ip);
    if(!server->listen(ip_addr, port))
    {
        QString problem = QString("TCP сервер не может быть окрыт на порту %1. Причина: %2").arg(QString::number(port)).arg(server->errorString()).toUtf8();

        qFatal(problem.toStdString().c_str());
        exit (1);
    }
    else
    {
        qDebug()<<"runt tcp:"<<port;
    }
}

void MyTcpServer::newConnection()
{
    socket = server->nextPendingConnection();
    connect(socket, SIGNAL(readyRead()), this, SLOT(readMessage()));
    qWarning() << QString("newConnection %1 port %2").arg(socket->peerAddress().toString()).arg(QString::number(socket->peerPort())).toUtf8();

}

void MyTcpServer::writeS(QByteArray data)
{

    socket->write(data);
    socket->flush();

}

void MyTcpServer::readMessage()
{

    QByteArray data=socket->readAll();
    emit message(data);

}
void MyTcpServer::stopConnection()
{
    socket->close();
}
