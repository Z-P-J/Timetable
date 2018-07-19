#ifndef TIMETABLESQL_H
#define TIMETABLESQL_H
#include <QString>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSql>


class TimetableSql
{
public:
    TimetableSql();
    void getTimetableSql(QString timetableSource);
    void getTimetableDetailSql(QString timetableSource);
private:
    //QString timetableSource;

    QSqlDatabase database;

};

#endif // TIMETABLESQL_H
