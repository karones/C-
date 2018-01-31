#pragma once
#ifndef MYTCPSERVER_H
#define MYTCPSERVER_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QtNetwork/QNetworkRequest>
#include <QHostAddress>
#include <QDebug>


class MyTcpServer : public QObject
{
    Q_OBJECT
public:
    explicit MyTcpServer(QString ip, int port, QObject *parent = 0);
signals:
    void message(QByteArray);
public slots:
    void newConnection();
    void readMessage();
    void writeS(QByteArray string);
    void stopConnection();
private:
    QTcpServer *server;
    QTcpSocket *socket;
};

#endif // MYTCPSERVER_H
