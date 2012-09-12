#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

QT_BEGIN_NAMESPACE
class QTextStream;
class QObject;
class QTcpSocket;
QT_END_NAMESPACE

class Client : public QObject
{
    Q_OBJECT

public:
    Client();
    void requestNewFortune();

private slots:
    void readFortune();
    void displayError(QAbstractSocket::SocketError socketError);

private:

    QTextStream qout;
    QTcpSocket *tcpSocket;
    QString currentFortune;
    quint16 blockSize;
};

#endif
