#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QTextStream;
class QObject;
class QTcpSocket;
class QDataStream;
QT_END_NAMESPACE

class Client : public QObject
{
    Q_OBJECT

public:
    Client();
    void start();

private slots:
    void onRequest();
    void displayError(QAbstractSocket::SocketError socketError);

private:

    void stop();

    QTextStream qout;
    QTcpSocket *tcpSocket;
    quint16 blockSize;
};

#endif
