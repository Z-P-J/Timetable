#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QList>
#include <QNetworkCookie>
#include <QNetworkAccessManager>

namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public slots:
    void login();

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    bool loginSuccess;

private slots:

    void on_reme_passwd_stateChanged(int arg1);

    void on_auto_login_stateChanged(int arg1);

protected:
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);
    virtual void mouseReleaseEvent(QMouseEvent* e);

private:
    Ui::LoginDialog *ui;
    bool remeberPasswd;
    bool autoLogin;
    const  QString loginUrl="http://202.115.47.141/loginAction.do";
    const QString timetableUrl="http://202.115.47.141/xkAction.do?actionType=6";
    QList<QNetworkCookie> list;
    QNetworkAccessManager *manager;
    void getCookies();
    void getTimetable();
    void setTimetable();
    void savecfg();
    void loadcfg();


    bool isWindowMoveable;
};

#endif // LOGINDIALOG_H
