#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QDebug>
#include <QDomDocument>
#include <QPalette>
#include <QTableWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSql>
#include <QSettings>
#include <QHeaderView>
#include <QSplitter>
#include <QVBoxLayout>
#include <QDateTime>
#include <QTextCodec>
#include <QList>
#include <QDialogButtonBox>
#include "logindialog.h"
#include <QProcess>
#include <QDir>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //窗口透明
    this->setWindowOpacity(0.8);

    //设置窗口背景颜色
    QPalette palette;
    palette.setColor(QPalette::Background,Qt::white);
    this->setPalette(palette);
    //去掉qt自带的窗口标题栏
    //this->setWindowFlags(Qt::FramelessWindowHint);


    QSettings load("login.ini",QSettings::IniFormat);
    if(load.value("autoLogin",0).toInt()&&load.value("rememberPasswd",0).toInt())
    {
        initTimetable();
    }
    else
    {
        QFile file("timetable.db");
        QFile file1("timetableDetail.db");
        if(file.exists())
        {
            file.remove();
        }
        if(file1.exists())
        {
            file1.remove();
        }
        qDebug()<<file.exists();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::getTimetable()
{
    QString select_all_sql = "select * from timetable";
    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
    }
    database.setDatabaseName("timetable.db");
    QSqlQuery sql_query(database);

    if(!database.open())
    {
        qDebug()<<database.lastError();
    }
    else
    {
        sql_query.prepare(select_all_sql);
        if(!sql_query.exec())
        {
            qDebug()<<"gdfhj";
            qDebug()<<sql_query.lastError();
        }
        else
        {

            int row = 0;
            while (sql_query.next()) {
                for(int i = 1; i < 8; i++)
                {
                    QTableWidgetItem *item = new QTableWidgetItem();

                    QString course = sql_query.value(i).toString();
                    qDebug()<<course;
                    item->setText(course);
                    table->setItem(row,i-1,item);
                }
                row++;
            }

            /*
            for(int i = 1; i < 8; i++)
            {
                while (sql_query.next()) {
                    QString course = sql_query.value(i).toString();
                    qDebug()<<course;
                }
                sql_query.clear();
            }
            */
        }



    }
}

void MainWindow::initTimetable()
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    table = new QTableWidget(13,7);

    //设置表格列标题
    QStringList headerLabels;
    headerLabels<<codec->toUnicode("星期一")<<codec->toUnicode("星期二")<<codec->toUnicode("星期三")<<codec->toUnicode("星期四")<<
                  codec->toUnicode("星期五")<<codec->toUnicode("星期六")<<codec->toUnicode("星期日");
    table->setHorizontalHeaderLabels(headerLabels);

    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //设置表格行标题
    QStringList rowLabels;
    rowLabels<<codec->toUnicode("第一节\n上午8:15")<<codec->toUnicode("第二节\n上午9:10")<<codec->toUnicode("第三节\n上午10:15")<<codec->toUnicode("第四节\n上午11:10")<<
               codec->toUnicode("第五节\n下午13:50")<<codec->toUnicode("第六节\n下午14:45")<<codec->toUnicode("第七节\n下午15:40")<<codec->toUnicode("第八节\n下午16:45")<<
               codec->toUnicode("第九节\n下午17:40")<<codec->toUnicode("第十节\n晚上19:20")<<codec->toUnicode("第十一节\n晚上20:15")<<
               codec->toUnicode("第十二节\n晚上21:10")<<codec->toUnicode("第十三节\n晚上22:05");
    table->setVerticalHeaderLabels(rowLabels);

    //设置每一行的高度
    for(int i = 0; i < 13; i++)
    {
        table->setRowHeight(i,100);

    }
    for(int i = 0; i < 7; i++)
    {
        table->setColumnWidth(i,120);
    }

    //设置表格的选择方式
    table->setSelectionBehavior(QAbstractItemView::SelectItems);

    //设置表格的编辑方式
    table->setEditTriggers(QAbstractItemView::DoubleClicked);//双击编辑

    //this->setCentralWidget(table);

    getTimetable();
    initTimetableDetail();

}

void MainWindow::initTimetableDetail()
{
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    detailTable = new QTableWidget(16,1);

    //设置表格列标题
    QStringList headerLabels;
    headerLabels<<codec->toUnicode("详细信息");
    //headerLabels<<"详细信息";
    detailTable->setHorizontalHeaderLabels(headerLabels);

    detailTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //设置表格行标题
    QStringList rowLabels;

    //注：由于编译器不同，可能会导致中文乱码，这时需要对中文转码
    rowLabels<<codec->toUnicode("培养方案")<<codec->toUnicode("课程号")<<codec->toUnicode("课程名")<<codec->toUnicode("课序号")<<
               codec->toUnicode("学分")<<codec->toUnicode("课程属性")<<codec->toUnicode(" 考试类型")<<codec->toUnicode("教师")<<
               codec->toUnicode("修读方式")<<codec->toUnicode("选课状态")<<codec->toUnicode("周次")<<codec->toUnicode("星期")<<
               codec->toUnicode("节次")<<codec->toUnicode("校区")<<codec->toUnicode("教学楼")<<codec->toUnicode("教室");
    /*
    rowLabels<<"培养方案"<<"课程号"<<"课程名"<<"课序号"<<
               "学分"<<"课程属性"<<" 考试类型"<<"教师"<<
               "修读方式"<<"选课状态"<<"周次"<<"星期"<<
               "节次"<<"校区"<<"教学楼"<<"教室";*/
    detailTable->setVerticalHeaderLabels(rowLabels);

    //设置每一行的高度
    for(int i = 0; i < 16; i++)
    {
        detailTable->setRowHeight(i,50);
    }

    sp = new QSplitter(Qt::Horizontal,this);
    sp->addWidget(table);
    //QVBoxLayout *layout = new QVBoxLayout();
    //layout->addWidget(detailTable);
    btn1 = new QPushButton();
    btn2 = new QPushButton();
    btn1->setText(codec->toUnicode("刷新"));
    btn2->setText(codec->toUnicode("注销"));
    //btn1->setText("刷新");
    //btn2->setText("注销");
    btn1->setStyleSheet("border:1px groove grey;border-radius:2px;background:transparent;height:30px");
    btn2->setStyleSheet("border:1px groove grey;border-radius:2px;background:transparent;height:30px");

    //layout->addWidget(btn);

    //sp->addWidget(detailTable);
    //设置分割的两个窗口宽度的比值8:2
    sp->setStretchFactor(0,8);
    sp->setStretchFactor(1,2);

    QSplitter *spRight = new QSplitter(Qt::Vertical,sp);
    spRight->addWidget(detailTable);
    //spRight->addWidget(btn1);
    //spRight->addWidget(btn2);
    spRight->setStretchFactor(0,3);
    spRight->setStretchFactor(1,1);
    QSplitter *sp3 = new QSplitter(Qt::Horizontal,spRight);
    sp3->addWidget(btn1);
    sp3->addWidget(btn2);
    sp3->setStretchFactor(0,1);
    sp3->setStretchFactor(1,1);
    //将设置好的分割窗口添加到主界面
    this->setCentralWidget(sp);
    //当QQTableWidget的item被点击时，刷新详情界面的数据
    connect(table,SIGNAL(itemPressed(QTableWidgetItem*)),this,SLOT(changeTimetableDetail()));
    connect(btn1,SIGNAL(clicked(bool)),this,SLOT(refreshTimetable()));
    connect(btn2,SIGNAL(clicked(bool)),this,SLOT(withdrawTimetable()));

    //获取系统星期
    QDateTime current_data_time = QDateTime::currentDateTime();
    QString current_date = current_data_time.toString("ddd");
    int current_date_num;
    qDebug()<<current_date;

    //将星期转换为对应的数字
    if(current_date == codec->toUnicode("周一"))
        current_date_num = 1;
    else if(current_date == codec->toUnicode("周二"))
        current_date_num = 2;
    else if(current_date == codec->toUnicode("周三"))
        current_date_num = 3;
    else if(current_date == codec->toUnicode("周四"))
        current_date_num = 4;
    else if(current_date == codec->toUnicode("周五"))
        current_date_num = 5;
    else if(current_date == codec->toUnicode("周六"))
        current_date_num = 6;
    else if(current_date == codec->toUnicode("周日"))
        current_date_num = 7;



    //获取系统时间
    QString current_time = current_data_time.toString("hh:mm");
    qDebug()<<current_time;
    int current_time_num;
    //将上课时间用数字来表示
    if(current_time>="8:15"&&current_time<"9:00")
    {
        current_time_num = 1;
    }
    else if(current_time>="9:10"&&current_time<"9:55")
    {
        current_time_num = 2;
    }
    else if(current_time>="10:15"&&current_time<"11:00")
    {
        current_time_num = 3;
    }
    else if(current_time>="11:10"&&current_time<"11:55")
    {
        current_time_num = 4;
    }
    else if(current_time>="13:50"&&current_time<"14:35")
    {
        current_time_num = 5;
    }
    else if(current_time>="14:45"&&current_time<"15:30")
    {
        current_time_num = 6;
    }
    else if(current_time>="15:40"&&current_time<"16:25")
    {
        current_time_num = 7;
    }
    else if(current_time>="16:45"&&current_time<"17:30")
    {
        current_time_num = 8;
    }
    else if(current_time>="17:40"&&current_time<"18:25")
    {
        current_time_num = 9;
    }
    else if(current_time>="19:20"&&current_time<"20:05")
    {
        current_time_num = 10;
    }
    else if(current_time>="20:15"&&current_time<"2!:00")
    {
        current_time_num = 11;
    }
    else if(current_time>="21:10"&&current_time<"21:55")
    {
        current_time_num = 12;
    }
    else if(current_time>="22:05"&&current_time<"22:50")
    {
        current_time_num = 13;
    }
    else
    {
        current_time_num = -1;
    }
    qDebug()<<current_date_num<<current_time_num;
    //根据对应的星期和上课时间，定位到对应的课程item
    if(current_time_num != -1)
    {
        QTableWidgetSelectionRange range(current_time_num-1,current_date_num-1,current_time_num-1,current_date_num-1);
        table->setRangeSelected(range,true);
        table->setFocus();

        QList<QTableWidgetItem*> items = table->selectedItems();

        QTableWidgetItem *item = items.at(0);

        qDebug()<<item->text();
        getTimetableDetail(item->text());
    }
}

void MainWindow::getTimetableDetail(QString str)
{
    //QTextCodec *codec = QTextCodec::codecForName("GBK");
    if(str == "")
    {
        for(int i = 0; i < 16; i++)
        {
            QTableWidgetItem *item = new QTableWidgetItem();
            item->setText("");
            detailTable->setItem(i,0,item);
        }
        return;
    }
    QString select_all_sql = "select * from timetableDetail";
    QSqlDatabase database;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
    {
        database = QSqlDatabase::database("qt_sql_default_connection");
    }
    else
    {
        database = QSqlDatabase::addDatabase("QSQLITE");
    }
    database.setDatabaseName("timetableDetail.db");
    QSqlQuery sql_query(database);

    if(!database.open())
    {
        qDebug()<<database.lastError();
    }
    else
    {
        sql_query.prepare(select_all_sql);
        if(!sql_query.exec())
        {
            qDebug()<<sql_query.lastError();
        }
        else
        {
            while (sql_query.next()) {
                QString detail = sql_query.value(3).toString();
                qDebug()<<detail;
                if(str.startsWith(detail))
                {
                    for(int i = 0; i < 16; i++)
                    {
                        QTableWidgetItem *item = new QTableWidgetItem();
                        QString timetable_detail = sql_query.value(i+1).toString();
                        qDebug()<<timetable_detail;
                        item->setText(timetable_detail);
                        detailTable->setItem(i,0,item);
                    }
                }
            }
        }
    }
}

void MainWindow::changeTimetableDetail()
{
    //获取被选中的所有item
    QList<QTableWidgetItem*> items = table->selectedItems();
    QTableWidgetItem *item = items.at(0);

    qDebug()<<item->text();
    getTimetableDetail(item->text());
}

void MainWindow::refreshTimetable()
{
    initTimetable();
}

void MainWindow::withdrawTimetable()
{
    //MainWindow::REBOOT_CODE = -1;
    QSettings save("login.ini",QSettings::IniFormat);
    save.setValue("autoLogin",0);
    //qApp->exit(MainWindow::REBOOT_CODE);
    QProcess::startDetached(QApplication::applicationFilePath(),QApplication::arguments(),QDir::currentPath());
    QApplication::closeAllWindows();
}
