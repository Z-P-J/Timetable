#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QSplitter>
#include <QPushButton>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void changeTimetableDetail();
    void refreshTimetable();
    void withdrawTimetable();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void getTimetable();
    void initTimetable();
    void initTimetableDetail();
    void getTimetableDetail(QString str);
    QPushButton *btn1;
    QPushButton *btn2;
    static int const REBOOT_CODE;

private:
    Ui::MainWindow *ui;
    QTableWidget *table;
    QTableWidget *detailTable;
    QSplitter *sp;
};

#endif // MAINWINDOW_H
