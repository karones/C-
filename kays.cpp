#include "kays.h"
#include <QDebug>
#include <unistd.h>
char packetNewConnection[] = {0xC0, 0x01,0x00,0x00,0x00};
char packetStopConnection[] = {0xc0, 0x02,0x00,0x00,0x00};
char packetStreamConnection[] ={0xc0, 0x00, 0x00, 0x00, 0x00};
char packetKvit[] ={0xc0, 0x04, 0x00, 0x01, 0x01, 0x01};




KAUS::KAUS( int timeLife_, int timeTrap_, QString fileName, QObject *parent) : QObject(parent),
    timeLife(timeLife_),
    timeTrap(timeTrap_)
{

    parcer = new CSV_PARCER(fileName);
    pthread_create(&tranciever,NULL,runT,this);
    QObject:: connect(this, SIGNAL(sendPack()), this, SLOT(slotSendPacket()));

}

void KAUS::trancieve()
{
    sleep(1);
    qDebug()<<timeTrap;

    QList<QString> data = parcer->getData();
    QList<QString> old_file = data;
    while(1)
    {
        data = parcer->getData();
        if (old_file != data)
        {
            old_file =data;
            emit sendPack();
        }
        sleep(timeTrap);
    }
}

void KAUS::checkMessage(QByteArray data)

{
    qDebug()<<data.toHex();
    data.reserve(6);
    structMessage * message= reinterpret_cast<structMessage*>(data.data());

    printf("0x%02x, \n",message->preambula);
    printf("0x%02x, \n",message->typeMessage);
    printf("0x%02x, \n",message->size1);
    printf("0x%02x, \n",message->size2);
    printf("0x%02x, \n",message->code);
    printf("0x%02x, \n",message->param);

    if (message->preambula != 0xc0)
    {
        stopConnectionAll();
    }

    switch (message->typeMessage){

    case 0:
        break; //хз что делать
    case 1:
        newConnection();
        KAUS::createPacket(0x03);
        break;
    case 2:

        stopConnectionAll();
        break;
    case 3:
        if (message->code ==2)
        {
            sendPacketKvitation();
            createPacket(0x04);
        }
        break;
    case 4:
        //квитанции, задел на будущее
        break;
    default:
        qDebug()<<"unknown type message";
        sendPacketStopConnection();
        stopConnectionAll();
        break;
    }
}

void KAUS::createPacket(u_int8_t typePacket)
{
    QByteArray packetMessage;
    u_int8_t zero=0;
    u_int8_t countBoard= 0;  //количество плат
    u_int8_t stateAllBoard =0; //обобщенное состояние оборудования
    QList<QString> data = parcer->getData();
    // qDebug()<<data;
    QList<QString> boards =parcer->getCountBoard(data, countBoard, stateAllBoard);
    if (boards.isEmpty())
    {
        qDebug()<<"Неверный формат файла";
        return;
    }
    qDebug()<<"count board"<<countBoard<<" state "<<stateAllBoard;
 //   qDebug()<<stateAllBoard;
    packetMessage.push_back(stateAllBoard); //обобщенное состояние оборудования
    packetMessage.push_back(countBoard);


    //dev_id, port_type, port_id, state
    QList<QString> temp=data;
    for (u_int8_t i=0; i<countBoard; i++)
    {
        u_int8_t stateAllStyk = 0;
        u_int8_t countStyk =0;
        packetMessage.push_back(0x01);//Тип еденицы оборудования
        packetMessage.push_back(i+1); //порядковый номер переделать в hex
        //   qDebug()<<"i"<<i;
        countStyk=parcer->getCountStyk(data, boards[i], stateAllStyk);
        //  qDebug()<<"Styk ="<<countStyk;
        packetMessage.push_back(stateAllStyk);//состояние еденицы оборудования
        packetMessage.push_back(countStyk); //количество стыков


        for (u_int8_t k=1; k<=countStyk; k++)
        {
            u_int8_t typeStik=0;

            QStringList alarms = parcer->getAlarm(temp, boards[i],  typeStik);
            //  qDebug()<<temp;
            temp.pop_front();
            packetMessage.push_back(typeStik);

            packetMessage.push_back(k);
            qDebug()<<"boards"<<boards[i]<<"stuk: "<<k<<" alarms"<<alarms.length()<<"type: "<<typeStik<<"alarm"<<alarms.at(0);
            if (alarms.at(0).toInt()>0)
            {
                //qDebug()<<"alarm";
                packetMessage.push_back(0x01);//авария на стыке
                packetMessage.push_back(alarms.length());
                foreach (QString alarm, alarms) {

                    packetMessage.push_back(alarm.toInt());  //toInt(0,16)
               //   qDebug()<<alarm.toInt();
                }
            }
            else
            {
                packetMessage.push_back(zero);//авария на стыке
                packetMessage.push_back(zero);
            }
        }

    }
    u_int16_t sizeMessage = packetMessage.size();
    qDebug()<<sizeMessage;
    char byte0=(char) (sizeMessage & 0xFF);
    char byte1=(char) ((sizeMessage >> 8) & 0xFF);

    packetMessage.push_front(0x02);//код сообщения
    packetMessage.push_front(byte0);
    packetMessage.push_front(byte1);


    packetMessage.push_front(typePacket);
    packetMessage.push_front(0xc0);



    qDebug()<<"FULLpacket:"<<packetMessage.toHex();


    emit writeToSocket(packetMessage);
}

void KAUS::newConnection()
{
    QByteArray message(packetNewConnection, sizeof(packetNewConnection));
    emit writeToSocket(message);
    tmr = new QTimer(this); // Создаем объект класса QTimer и передаем адрес переменной
    tmr->setInterval(timeLife); // Задаем интервал таймера
    connect(tmr, SIGNAL(timeout()), this, SLOT(streamConnection())); // Подключаем сигнал таймера к нашему слоту
    tmr->start(); // Запускаем таймер
}

void KAUS::stopConnectionAll()
{
    if (tmr !=NULL)
    {
        delete tmr;
    }
    emit  stopConnection();
}


void KAUS::streamConnection()
{
    qDebug()<<"ping";
    QByteArray message(packetStreamConnection, sizeof(packetStreamConnection));
    emit writeToSocket(message);
}

void KAUS::sendPacketKvitation()
{
    QByteArray message(packetKvit, sizeof(packetKvit));
    emit writeToSocket(message);

    restartTimer();
}

void KAUS::sendPacketStopConnection()
{
    QByteArray message(packetStopConnection, sizeof(packetStopConnection));
    emit writeToSocket(message);
}

void KAUS::slotSendPacket()
{
    createPacket(0x03);
    restartTimer();

}

void KAUS::restartTimer()
{
    tmr->stop();
    if (tmr !=NULL)
    {
        delete tmr;
    }

    tmr = new QTimer(this); // Создаем объект класса QTimer и передаем адрес переменной
    tmr->setInterval(timeLife); // Задаем интервал таймера
    connect(tmr, SIGNAL(timeout()), this, SLOT(streamConnection())); // Подключаем сигнал таймера к нашему слоту
    tmr->start(); // Запускаем таймер

}
