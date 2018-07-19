#include "mainwindow.h"
#include <QApplication>
#include <logindialog.h>
#include <QSettings>
#include <QSplitter>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QSettings load("login.ini",QSettings::IniFormat);
    MainWindow w;
    LoginDialog l;
    if(load.value("rememberPasswd",0).toInt()&&load.value("autoLogin",0).toInt()){
        //w.initTimetable();
        w.show();
        return a.exec();
    }else{
        if(l.exec()==QDialog::Accepted)
        {
            QFile file("timetable.txt");
            file.open(QIODevice::ReadOnly);
            if(file.size()>=10000){
                w.initTimetable();
                w.show();
                return a.exec();
            }else{
                exit(0);
            }

        }
    }
    return a.exec();
}
