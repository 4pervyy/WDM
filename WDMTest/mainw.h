#ifndef MAINW_H
#define MAINW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QRegExpValidator>
#include "winsock2.h"
#include "iphlpapi.h"
#include "icmpapi.h"
#include <QtCharts>

namespace Ui {
class MainW;
}

class MainW : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainW(QWidget *parent = nullptr);
    ~MainW();

private slots:
    void on_pushButton_clicked();

    void on_Button_ping_clicked();

    void on_Button_Server_clicked();

    void on_Button_Client_clicked();

private:
    Ui::MainW *ui;
    QChart *chrt;
    QChartView *chrtv;
};

#endif // MAINW_H
