#ifndef KAYS_H
#define KAYS_H
#include <qbytearray.h>
#include <QObject>
#include <QDebug>
#include <QTimer>
#include "csv_parcer.h"


class KAUS: public QObject
{
    Q_OBJECT
public:
    explicit KAUS( int timeLife, int timeTrap, QString fileName, QObject *parent = 0);
    CSV_PARCER *parcer;

public slots:
    void checkMessage(QByteArray);
    void streamConnection();
    void sendPacketKvitation();
    void sendPacketStopConnection();

private slots:
    void slotSendPacket();
private:
    void newConnection();
    void stopConnectionAll();
    void createPacket(u_int8_t typePacket);
    pthread_t tranciever;
    void trancieve();

    void restartTimer();


    QTimer *tmr;
    int timeLife;
    int timeTrap;

    struct structMessage
    {
        u_char preambula;
        u_char typeMessage;
        char size1;
        char size2;
        u_char code;
        u_char param;

        //        u_int8_t * message;
        //        structMessage()
        //        {

        //                message = new u_int8_t (100);
        //        }
    };
    static void * runT(void * arg)
    {
        ((KAUS *)arg)->trancieve();
        return NULL;
    }

signals:
    void writeToSocket(QByteArray);
    void stopConnection();
    void sendPack();


};

#endif // KAYS_H
