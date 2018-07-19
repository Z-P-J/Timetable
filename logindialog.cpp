#include "logindialog.h"
#include "ui_logindialog.h"
#include "mainwindow.h"
#include <QMessageBox>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkCookie>
#include <QtNetwork/QNetworkCookieJar>
#include <QEventLoop>
#include <QDebug>
#include <QTextCodec>
#include <QByteArray>
#include <QtCore>
#include <QBitArray>
#include <QList>
#include <ctype.h>
#include <QTextCodec>
#include <QDomDocument>
#include <QXmlStreamWriter>
#include "timetablesql.h"
#include <QSettings>
#include <QPalette>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setFixedSize(500,380);

    this->setWindowOpacity(0.9);

    QTextCodec *codec = QTextCodec::codecForName("GBK");
    ui->edt_username->setPlaceholderText(codec->toUnicode("请输入您的学号"));
    //ui->edt_username->setPlaceholderText("请输入您的学号");
    ui->edt_username->setStyleSheet("border:1px groove grey;border-radius:8px;padding:2px 4px;background:transparent");
    ui->edt_password->setPlaceholderText(codec->toUnicode("请输入您的密码"));
    //ui->edt_password->setPlaceholderText("请输入您的密码");
    ui->edt_password->setStyleSheet("border:1px groove grey;border-radius:10px;padding:2px 4px;background:transparent");

    //this->autoFillBackground(true);

    QPalette palette;
    palette.setColor(QPalette::Background,Qt::white);
    //palette.setBrush(QPalette::Background,QBrush(QPixmap(":/timg.jpg")));
    this->setPalette(palette);
    ui->btn_login->setStyleSheet("background-color: rgb(232,232,232);border:1px solid grey;border-radius:8px;font:bold 14px;background:transparent");
    //去掉qt自带的窗口标题栏
    //this->setWindowFlags(Qt::FramelessWindowHint);

    ui->edt_password->setEchoMode(QLineEdit::Password);
    loadcfg();
    connect(this->ui->btn_login,SIGNAL(clicked(bool)),this,SLOT(login()));
}

LoginDialog::~LoginDialog()
{
    manager->deleteLater();
    delete ui;
}

void LoginDialog::login()
{
    //QMessageBox::information(tr("提示"),tr((char*)s),QMessageBox::Ok);
    ui->btn_login->setText("正在登录中,请稍后...");
    QSettings load("login.ini",QSettings::IniFormat);
    if(load.value("autoLogin",0).toInt()&&load.value("rememberPasswd",0).toInt()){
        exit(0);
        return;
    }
    getCookies();
    getTimetable();

    QFile file("timetable.txt");
    file.open(QIODevice::ReadOnly);
    if(file.size()>=10000){
        setTimetable();
        savecfg();
        loginSuccess = true;
    }
    else
    {
        loginSuccess = false;
    }


    /*
    QFile file("timetable.html");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug()<<"打开文件失败";
        return -1;
    }

    QByteArray html = file.readAll();
    */

}

void LoginDialog::getCookies()
{
    //QString str=s+"?zjh="+this->ui->lineEdit->text()+"&mm="+this->ui->lineEdit_2->text();
    //qDebug()<<str;


    //设置post的参数
    QByteArray postData;
    postData.append("zjh="+this->ui->edt_username->text()+"&mm="+this->ui->edt_password->text());
    qDebug()<<postData;

    QNetworkRequest *request = new QNetworkRequest();
    request->setUrl(loginUrl);
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request->setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");

    QEventLoop loop;
    this->manager = new QNetworkAccessManager(this);

    //NetworkCookieJar *netCookies = manager->cookieJar();


    //QNetworkCookie* cookies=new QNetworkCookie(this);
    //manager->setCookieJar(cookies);

    //发出请求
    QNetworkReply *reply = manager->post(*request,postData);
    //请求结束并下载完成后，退出子事件循环
    connect(reply,SIGNAL(finished()),&loop,SLOT(quit()),Qt::UniqueConnection);
    //开启子事件循环
    loop.exec();

    if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()!= 200)
    {
        QMessageBox::information(this,tr("错误"),tr("登陆失败!"),QMessageBox::Ok);
        exit(0);
        return;
    }

    //获取川大本科教务系统模拟登陆后的cookies
    qDebug()<<manager->cookieJar();
    list = manager->cookieJar()->cookiesForUrl(request->url());
    for(int i=0;i<list.length();i++){
        qDebug()<<list[i].name()<<":"<<list[i].value();
    }

    //解决中文乱码
    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QString ss = codec->toUnicode(reply->readAll());
    //QString string = QString::fromUtf8(bytes);
    qDebug()<<ss;
    reply->deleteLater();
}

void LoginDialog::getTimetable()
{
    QNetworkRequest *request=new QNetworkRequest();
    request->setUrl(timetableUrl);
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    request->setRawHeader("User-Agent","Mozilla/5.0 (compatible; MSIE 9.0; Windows NT 6.1; WOW64; Trident/5.0)");

    //QByteArray postData;
    //postData.append("jsessionid="+list.at(0).value());

    QVariant var;
    var.setValue(list);
    qDebug()<<list[0].value();
    request->setHeader(QNetworkRequest::CookieHeader,var);
    //request.setHeader(QNetworkRequest::CookieHeader,"sid=abc7F1b0bYuOm6uc9AYrw");
    //request->setRawHeader("Jsessionid","sid="+list[0].value());

    QEventLoop loop;
    this->manager = new QNetworkAccessManager(this);

    //NetworkCookieJar *netCookies;

    //manager->setCookieJar();

    //manager->cookieJar()->setCookiesFromUrl(list,QUrl(timetableUrl));

    QNetworkReply *reply = manager->get(*request);
    //QNetworkReply *reply = manager->post(*request,postData);
    connect(reply,SIGNAL(finished()),&loop,SLOT(quit()),Qt::UniqueConnection);
    loop.exec();

    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QString ss = codec->toUnicode(reply->readAll());
    qDebug()<<ss;
    qDebug()<<"网页源码";

    //去除空格
    //ss.remove(QRegExp("\\s"));
    //ss.remove(QRegExp("<(?<HtmlTag>title)[^>]*?>((?<Nested><\k<HtmlTag>[^>]*>)|</\k<HtmlTag>>(?<-Nested>)|.*?)*</\k<HtmlTag>>"));

    //将得到的网页源码写入本地文件
    QFile file("timetable.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream out(&file);
    out<<ss<<endl;
    qDebug()<<"filesize";
    qDebug()<<file.size();
    if(file.size()>=10000)
    {
        ui->btn_login->setText("登陆成功!");
        //QMessageBox::information(this,tr("提示"),tr("登陆成功"),QMessageBox::Ok);
    }
    else
    {
        QMessageBox::information(this,codec->toUnicode("提示"),codec->toUnicode("登录失败"),QMessageBox::Ok);
    }
    file.close();
    if(file.size()<10)
    {
        file.remove();
    }
}

void LoginDialog::setTimetable()
{
    QFile file("timetable.txt");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug()<<"打开文件失败";
        return;
    }


    QTextCodec *codec = QTextCodec::codecForName("GBK");
    QString html = codec->toUnicode(file.readAll());

    TimetableSql timetableSql;
    timetableSql.getTimetableDetailSql(html);
    html.remove(QRegExp("\\s"));
    //qDebug()<<html;
    timetableSql.getTimetableSql(html);

    //html.remove(QRegExp("<(?<HtmlTag>div)[^>]*?>((?<Nested><\k<HtmlTag>[^>]*>)|</\k<HtmlTag>>(?<-Nested>)|.*?)*</\k<HtmlTag>>"));
    //QTextCodec::setCodecForLocale(QTextCodec::codecForName("GBK"));

    /*
    int i = html.indexOf(codec->toUnicode("第一节"));
    html.remove(0,i);
    i = html.indexOf(codec->toUnicode("第十三节"));
    html.remove(i,html.size());
    //int i = html.indexOf("</tr></table></td></tr>");
    //qDebug()<<html;


    //char timetable[7][13];
    i = html.indexOf(";");
    //qDebug()<<i;
    html.remove(0,i);
    i=html.indexOf("<");
    //qDebug()<<html.mid(1,i-1);

    QDomDocument doc;
    if(!doc.setContent(&file,false))
    {
        qDebug()<<"11111";
        return;
    }
    QDomElement root = doc.documentElement();
    QDomNodeList nodeList = root.elementsByTagName("title");
    if(nodeList.isEmpty())
    {
        qDebug()<<"222222";
        return;
    }
    qDebug()<<"33333";
    for(int i=0;i<nodeList.size();i++)
    {

        qDebug()<<nodeList.at(i).toElement().text();
    }
    qDebug()<<"4444444";
    */
}

void LoginDialog::savecfg()
{
    QSettings save("login.ini",QSettings::IniFormat);
    if(ui->reme_passwd->isChecked())
    {
        save.setValue("rememberPasswd",1);
        save.setValue("username",ui->edt_username->text());
        //对记住的密码简单加密
        QString str = ui->edt_password->text();
        QByteArray password = str.toLatin1();
        //password.toBase64();
        save.setValue("password",password.toBase64());
    }
    else
    {
        save.setValue("rememberPasswd",0);
    }

    if(ui->auto_login->isChecked())
    {
        save.setValue("autoLogin",1);
    }
    else
    {
        save.setValue("autoLogin",0);
    }
}

void LoginDialog::loadcfg()
{
    QSettings load("login.ini",QSettings::IniFormat);
    ui->reme_passwd->setChecked(load.value("rememberPasswd",1).toInt());
    ui->auto_login->setChecked(load.value("autoLogin",0).toInt());
    if(load.value("rememberPasswd",1).toInt())
    {
        ui->edt_username->setText(load.value("username","").toString());
        //将加密保存的密码解密
        //QByteArray password;
        ui->edt_password->setText(QByteArray::fromBase64(load.value("password","").toString().toLatin1()));
    }

}

void LoginDialog::on_reme_passwd_stateChanged(int arg1)
{
    if(arg1)
    {
        remeberPasswd = true;
    }
    else
    {
        //ui->edt_password->clear();
        remeberPasswd = false;
    }
}

void LoginDialog::on_auto_login_stateChanged(int arg1)
{
    if(arg1&&ui->reme_passwd->isChecked())
    {

        autoLogin = true;
    }
    else if(arg1&&!ui->reme_passwd->isChecked())
    {
        ui->auto_login->click();
        autoLogin = false;
    }
    else
    {
        autoLogin = false;
    }
}

void LoginDialog::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        isWindowMoveable = true;
    }
}

void LoginDialog::mouseMoveEvent(QMouseEvent *e)
{
    //e->windowPos() = e->pos();
    if(isWindowMoveable)
    {
        move(e->globalPos());
    }
}

void LoginDialog::mouseReleaseEvent(QMouseEvent *e)
{
    isWindowMoveable = false;
}
