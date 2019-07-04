/*#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>

class server: public QTcpServer
{
    Q_OBJECT
public:
    server();
    ~server();

    QTcpSocket* socket;
    QByteArray Data;

public slots:
    void startServer();
    void incomingConnection(int socketDescriptor);
    void sockReady();
    void sockDisc();
};

#endif // SERVER_H*/
