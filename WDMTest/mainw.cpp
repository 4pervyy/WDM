/*

Learn more about the IcmpSendEcho function: https://docs.microsoft.com/en-us/windows/desktop/api/icmpapi/nf-icmpapi-icmpsendecho

  */
#include "mainw.h"
//#include <client.h>
//#include <server.h>
#include "ui_mainw.h"

MainW::MainW(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainW)
{
    ui->setupUi(this);
    //Button
    QAction* screen_bt = menuBar()->addAction(tr("Настройки"));
    connect(screen_bt, SIGNAL(triggered(bool)), this, SLOT(screeng()));
    QAction* about_bt = menuBar()->addAction(tr("Справка"));
    connect(about_bt, SIGNAL(triggered(bool)), this, SLOT(aboutf()));
    QAction* exi_bt = menuBar()->addAction(tr("Выход"));
    connect(exi_bt, SIGNAL(triggered(bool)), this, SLOT(closedw()));
    //Spell checking
    QString ipRange = "(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])";
    QRegExp ipRegex ("^" + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange
                     + "\\." + ipRange + "$");
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->ippco->setValidator(ipValidator);
    //
  //  ui->ippco->()


}

MainW::~MainW()
{
    delete ui;
}

void MainW::on_pushButton_clicked()
{
    ui->myip->clear();
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
     for(int nIter=0; nIter<list.count(); nIter++)
      {
          if(!list[nIter].isLoopback())
              if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol )
              ui->myip->append(list[nIter].toString());
      }
}

void MainW::on_Button_ping_clicked()
{
    //chart
    chrt = new QChart;
    ui->vchrt->setChart(chrt); // graphics link to display element
    chrt->setTitle("График ping");
    // Axis Х
    QValueAxis *axisX = new QValueAxis;
    axisX->setRange(1, ui->SB_pn->value()); // диапазон значений на оси
    axisX->setTickCount(ui->SB_pn->value()); // число линий сетки
    axisX->setLabelFormat("%.2f"); // формат отображения чисел на оси
     // Axis Y
    QValueAxis *axisY = new QValueAxis;
    axisY->setRange(0, 50);
    axisY->setTickCount(5);
    axisY->setLabelFormat("%g");
    axisX->setTitleText("Номер пакета");
    axisY->setTitleText("Задержка, мс");
    QLineSeries* series1 = new QLineSeries();
    series1->setColor(Qt::darkBlue);
    //series1->setName(tr(" "));





    // Declare variables
    HANDLE hIcmpFile;                       // Handler
    unsigned long ipaddr = INADDR_NONE;     // Destination Address
    DWORD dwRetVal = 0;                     // Number of responses
    char SendData[ui->SB_ps->value()];      // buffer отсылаемых данных
    LPVOID ReplyBuffer = nullptr;           // buffer answer
    DWORD ReplySize = 0;                    // size buffer answer
    unsigned long timeoutp = ui->SB_pn->value();
    int aresult[ui->SB_pn->value()];
    int packetinfo[3] = {ui->SB_pn->value(),0,0};
    int max,min;


    // Set the IP address from the lineEdit field
    ipaddr = inet_addr(ui->ippco->text().toStdString().c_str());
    hIcmpFile = IcmpCreateFile();   // Create Handler

    // Allocate memory for the response buffer
    ReplySize = sizeof(ICMP_ECHO_REPLY) + sizeof(SendData);
    ReplyBuffer = (VOID*) malloc(ReplySize);

        QString strMessage = "";
        QString resMessage = "";
         strMessage += "Отправка пакетов на " + ui->ippco->text() + "\n";
          strMessage += "Количество пакетов: " + QString::number( ui->SB_pn->value())+ "\n";
          strMessage += "Ответы: \n" ;

    for(int i=1;i<=ui->SB_pn->value();i++) {
        // Call the ICMP echo request function
        dwRetVal = IcmpSendEcho(hIcmpFile, ipaddr, SendData, sizeof(SendData),
                    nullptr, ReplyBuffer, ReplySize, timeoutp);
        if (dwRetVal != 0) {
            // Echo response structure
            PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY)ReplyBuffer;
            struct in_addr ReplyAddr;
            ReplyAddr.S_un.S_addr = pEchoReply->Address;
            // create a line to write response messages

                if (dwRetVal > 1) {
                    strMessage += "Больше ICMP > 1 ";
                }
                    strMessage += "Номер пакета: "+QString::number(i) + "\n";
                    strMessage += "Время = " + QString::number(pEchoReply->RoundTripTime) + " мс | Размер пакета:"+ QString::number(ui->SB_ps->value())+" Байт \n";
                    aresult[i-1]=pEchoReply->RoundTripTime;
                    series1->append(i,pEchoReply->RoundTripTime);
                    ++packetinfo[1];
                    //qDebug()<< aresult[i-1];

        } else {
                strMessage += "Номер пакета: "+QString::number(i) + "\n";
                strMessage += "Функция IcmpSendEcho не работает. Пакет потерян.  \n";
                strMessage += "IcmpSendEcho код ошибки: ";
                strMessage += QString::number(GetLastError()) +"\n";
                ++packetinfo[2];
            }

    }
    chrt->addSeries(series1);
    chrt->setAxisX(axisX, series1);
    chrt->setAxisY(axisY, series1);
     max=aresult[0];
     min=aresult[0];
     for(int i=1;i<=ui->SB_pn->value();i++)
     {
         if(max<aresult[i-1]) max=aresult[i-1];
         if(min>aresult[i-1]) min=aresult[i-1];
     }

    resMessage += "Всего пакетов:" + QString::number(packetinfo[0]) + "| Получено пакетов: " + QString::number(packetinfo[1]) +  "| Потеряно пакетов: "  + QString::number(packetinfo[2]) + "\n";
    resMessage += "Максимальное значения: " + QString::number(max) + "| Минимальное значения: " + QString::number(min);

    ui->statusping->setText(strMessage);
    ui->resultping->setText(resMessage);// Show information about received data
    free(ReplyBuffer); // Memory release

}
//server::server(){}

//server::~server(){}

void MainW::on_Button_Server_clicked()
{

}

void MainW::on_Button_Client_clicked()
{

}

/*void server::startServer(){
    if(this->listen())
}*/
