#include "csv_parcer.h"
#include <QFile>

#include <QDebug>
CSV_PARCER::CSV_PARCER(QString file):
    fileName(file)
{

}

QList<QString> CSV_PARCER::getData()
{
    QList<QString>data;


    QFile file(fileName);
    if ( !file.open(QFile::ReadOnly | QFile::Text) ) {
        qDebug() << "File not exists";
    } else {
        // Создаём поток для извлечения данных из файла
        QTextStream in(&file);
        // Считываем данные до конца файла

        while (!in.atEnd())
        {
            // ... построчно
            QString line = in.readLine();
            // Добавляем в модель по строке с элементами

            data.push_back(line);
        }

        file.close();
        return data;
    }
}

QList<QString> CSV_PARCER::getCountBoard(QList<QString>csvData, u_int8_t &count, u_int8_t &stateAllBoard)
{
    QList<QString> board;
    foreach (QString line, csvData) {
        QStringList list =line.split(";");
        if (list.length()!=4)
        {
            QList<QString>empty;
            return empty;
        }
        if (!board.contains(list[0]))
        {
            board.push_back(list[0]);
            count++;

        }

        if (list[3].toInt()!=0)
        {
            stateAllBoard =0x02;
        }
    }
    return board;
}
u_int8_t CSV_PARCER::getCountStyk(QList<QString>csvData, QString board, u_int8_t &statePlata)
{
    u_int8_t count =0;
    foreach (QString line, csvData) {
        QStringList list =line.split(";");

        if (list[0].contains(board))
        {
            count++;
            if (list[3].toInt()!=0)
            {
                if (statePlata != 0x01)
                    statePlata =0x02;
            }
            if (list[3].toInt()==78)
            {
                statePlata = 0x01;
            }
        }

    }
    return count;
}


QStringList CSV_PARCER::getAlarm(QList<QString>csvData, QString board, u_int8_t &styk)
{
    QStringList Alarm;
    //qDebug()<<csvData;
    foreach (QString line, csvData) {
        QStringList list =line.split(";");

        if (list[0].contains(board))
            //  if(list[2].contains(styk))
        {
            qDebug()<<"check"<<list;
            styk=list[1].toInt();
            Alarm =list[3].split(",");
            //  qDebug()<<Alarm;
            break;
        }
    }
    return Alarm;
}
