#include "timetablesql.h"
#include <QTextCodec>
#include <QDebug>
#include <QVariantList>
#include <QFileInfo>
#include <QFile>


TimetableSql::TimetableSql()
{
}

void TimetableSql::getTimetableSql(QString timetableSource)
{
    //qDebug()<<QSqlDatabase::drivers();
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
    }
    database.setDatabaseName("timetable.db");
    //database.setUserName("20171414");
    //database.setPassword("123456");
    QString create_sql = "create table timetable (id int primary key,one varchar(30),two varchar(30),three varchar(30),four varchar(30),five varchar(30),six varchar(30),seven varchar(30))";
    QString insert_sql = "insert into timetable values(?,?,?,?,?,?,?,?)";
    if(!database.open())
    {
        qDebug()<<database.lastError();
    }
    else
    {
        QSqlQuery sql_query(database);
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            qDebug()<<"create succuss!";
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            int i = timetableSource.indexOf(codec->toUnicode("第一节"));
            //int i = timetableSource.indexOf("第一节");
            timetableSource.remove(0,i);
            i = timetableSource.indexOf(codec->toUnicode("</tr></table></td></tr>"));
            timetableSource.remove(i+1,timetableSource.size());
            //qDebug()<<timetableSource;
            QVariantList Ids;
            QVariantList one;
            QVariantList two;
            QVariantList three;
            QVariantList four;
            QVariantList five;
            QVariantList six;
            QVariantList seven;
            sql_query.prepare(insert_sql);

            for(int j=0;j<13;j++)
            {
                Ids.append(j);
                if(j == 4)
                {
                    i = timetableSource.indexOf(codec->toUnicode("第五节"));
                    //i = timetableSource.indexOf("第五节");
                    timetableSource.remove(0,i);
                    //qDebug()<<i+timetableSource;
                }else if(j==9)
                {
                    i = timetableSource.indexOf(codec->toUnicode("第十节"));
                    //i = timetableSource.indexOf("第十节");
                    timetableSource.remove(0,i);

                }
                for(int k=0;k<7;k++)
                {
                    i=timetableSource.indexOf(";");
                    timetableSource.remove(0,i);
                    i = timetableSource.indexOf("<");
                    //qDebug()<<timetableSource.mid(1,i-1);
                    QString str = timetableSource.mid(1,i-1);
                    if(!str.indexOf("&nbsp;")){
                        str = "";
                    }
                    switch (k) {
                    case 0:
                        one.append(str);
                        break;
                    case 1:
                        two.append(str);
                        break;
                    case 2:
                        three.append(str);
                        break;
                    case 3:
                        four.append(str);
                        break;
                    case 4:
                        five.append(str);
                        break;
                    case 5:
                        six.append(str);
                        break;
                    case 6:
                        seven.append(str);
                        break;
                    default:
                        break;
                    }
                    timetableSource.remove(0,i);
                }
            }
            sql_query.addBindValue(Ids);
            sql_query.addBindValue(one);
            sql_query.addBindValue(two);
            sql_query.addBindValue(three);
            sql_query.addBindValue(four);
            sql_query.addBindValue(five);
            sql_query.addBindValue(six);
            sql_query.addBindValue(seven);
            //qDebug()<<Ids;
            if(!sql_query.execBatch()){
                qDebug()<<sql_query.lastError();
            }else{
                qDebug()<<"insert succuss";
            }
        }

    }
    database.close();
}

void TimetableSql::getTimetableDetailSql(QString timetableSource)
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
    }
    database.setDatabaseName("timetableDetail.db");
    QString create_sql = "create table timetableDetail (id int primary key,s1 varchar(30),s2 varchar(30),s3 varchar(30),s4 varchar(30),s5 varchar(30),"
                         "s6 varchar(30),s7 varchar(30),s8 varchar(30),s9 varchar(30),s10 varchar(30),s11 varchar(30),s12 varchar(30),s13 varchar(30),"
                         "s14 varchar(30),s15 varchar(30),s16 varchar(30))";
    QString insert_sql = "insert into timetableDetail values(?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)";
    if(!database.open())
    {
        qDebug()<<database.lastError();
    }
    else
    {
        QSqlQuery sql_query(database);
        sql_query.prepare(create_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            qDebug()<<"create succuss!";
            QTextCodec *codec = QTextCodec::codecForName("GBK");
            int i = timetableSource.indexOf("this.className='evenfocus';");
            timetableSource.remove(0,i);

            i = timetableSource.indexOf("<Script language=");
            timetableSource.remove(i,timetableSource.size());
            //qDebug()<<timetableSource;


            QVariantList Ids;
            QVariantList s1;
            QVariantList s2;
            QVariantList s3;
            QVariantList s4;
            QVariantList s5;
            QVariantList s6;
            QVariantList s7;
            QVariantList s8;
            QVariantList s9;
            QVariantList s10;
            QVariantList s11;
            QVariantList s12;
            QVariantList s13;
            QVariantList s14;
            QVariantList s15;
            QVariantList s16;
            sql_query.prepare(insert_sql);

            int count = 1;
            while (timetableSource.indexOf("&nbsp;") != -1) {
                Ids.append(count);

                int num = 0;
                QString str,tempStr1,tempStr2;

                for(int j = 0; j < 16; j++)
                {
                    if(timetableSource.indexOf("<td rowspan=\"2\" >")<timetableSource.indexOf(codec->toUnicode("&nbsp;"))){
                        num++;
                        qDebug()<<"numnumnum";
                    }
                    qDebug()<<num;
                    if(num >= 11){
                        if(j == 10){
                            tempStr1 = timetableSource;
                            tempStr2 = timetableSource;
                            tempStr1.remove(tempStr1.indexOf("onMouseOut=this.className"),tempStr1.size());
                            tempStr2.remove(0,tempStr2.indexOf("onMouseOut=this.className"));
                            tempStr2.remove(0,tempStr2.indexOf("&nbsp;"));
                            tempStr2.remove(tempStr2.indexOf("onMouseOut=this.className"),tempStr2.size());
                            tempStr1.remove(tempStr1.indexOf("<td rowspan=\"2\" align=\"center\">"),tempStr1.indexOf(codec->toUnicode("大纲日历")));
                            qDebug()<<tempStr1;
                            tempStr2.remove(tempStr2.indexOf("<td rowspan=\"2\" align=\"center\">"),tempStr2.indexOf(codec->toUnicode("大纲日历")));
                            qDebug()<<tempStr2;
                        }


                        int i = tempStr1.indexOf(codec->toUnicode("&nbsp;"));
                        tempStr1.remove(0,i+6);
                        i = tempStr1.indexOf("<");
                        str = tempStr1.mid(0,i).trimmed();
                        qDebug()<<"tempstr1";
                        qDebug()<<str;

                        tempStr1.remove(0,i);

                        i = tempStr2.indexOf(codec->toUnicode("&nbsp;"));
                        tempStr2.remove(0,i+6);
                        i = tempStr2.indexOf("<");
                        qDebug()<<"tempstr2";
                        qDebug()<<tempStr2.mid(0,i).trimmed();
                        if(str != tempStr2.mid(0,i).trimmed())
                        {
                            qDebug()<<"cscscscscscscscscs";
                            str = str + "/" + tempStr2.mid(0,i).trimmed();
                        }
                        tempStr2.remove(0,i);
                        if(j == 15){
                            timetableSource.remove(0,timetableSource.indexOf("<td rowspan=\"1\" >"));
                            num = 0;
                        }
                        qDebug()<<str;
                    }
                    else
                    {
                        int i = timetableSource.indexOf(codec->toUnicode("&nbsp;"));
                        timetableSource.remove(0,i+6);
                        i = timetableSource.indexOf("<");
                        str = timetableSource.mid(0,i).trimmed();
                        qDebug()<<j;
                        qDebug()<<str;
                        timetableSource.remove(0,i);
                        if(j == 9)
                        {
                            num++;
                        }
                    }

                    switch (j) {
                    case 0:
                        s1.append(str);
                        break;
                    case 1:
                        s2.append(str);
                        break;
                    case 2:
                        s3.append(str);
                        break;
                    case 3:
                        s4.append(str);
                        break;
                    case 4:
                        s5.append(str);
                        break;
                    case 5:
                        s6.append(str);
                        break;
                    case 6:
                        s7.append(str);
                        break;
                    case 7:
                        s8.append(str);
                        break;
                    case 8:
                        s9.append(str);
                        break;
                    case 9:
                        s10.append(str);
                        break;
                    case 10:
                        s11.append(str);
                        break;
                    case 11:
                        s12.append(str);
                        break;
                    case 12:
                        s13.append(str);
                        break;
                    case 13:
                        s14.append(str);
                        break;
                    case 14:
                        s15.append(str);
                        break;
                    case 15:
                        s16.append(str);
                        break;
                    default:
                        break;
                    }

                }
                count++;
            }
            sql_query.addBindValue(Ids);
            sql_query.addBindValue(s1);
            sql_query.addBindValue(s2);
            sql_query.addBindValue(s3);
            sql_query.addBindValue(s4);
            sql_query.addBindValue(s5);
            sql_query.addBindValue(s6);
            sql_query.addBindValue(s7);
            sql_query.addBindValue(s8);
            sql_query.addBindValue(s9);
            sql_query.addBindValue(s10);
            sql_query.addBindValue(s11);
            sql_query.addBindValue(s12);
            sql_query.addBindValue(s13);
            sql_query.addBindValue(s14);
            sql_query.addBindValue(s15);
            sql_query.addBindValue(s16);
            //qDebug()<<Ids;
            if(!sql_query.execBatch()){
                qDebug()<<sql_query.lastError();
            }else{
                qDebug()<<"insert succuss";
            }
        }

    }
    database.close();
}
