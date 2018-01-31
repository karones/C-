#ifndef CSV_PARCER_H
#define CSV_PARCER_H
#include <QTextStream>
#include <QStringList>
class CSV_PARCER
{
public:
    CSV_PARCER(QString fileName);
    QList<QString> getData();
    QList<QString> getCountBoard(QList<QString>csvData, u_int8_t &countBoards, u_int8_t &stateAllBoard);
    u_int8_t getCountStyk(QList<QString>csvData, QString board, u_int8_t &stateAllStyk);
    QStringList getAlarm(QList<QString>csvData, QString board, u_int8_t &styk);

private:
    QString fileName;
};

#endif // CSV_PARCER_H
