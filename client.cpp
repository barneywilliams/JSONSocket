#include <QTextStream>
#include <QTimer>
#include <QDebug>
#include <iostream>

#include "client.h"

Client::Client()
    : QObject(0), qout(stdout)
{
    qout << "JSON client started!" << endl;

    tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(onRequest()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void Client::start()
{
    tcpSocket->connectToHost("localhost", 19024);
}

void Client::stop()
{
    tcpSocket->close();
}

void Client::onRequest()
{
    qDebug() << "Request received!";
    QTextStream stream(tcpSocket);
    QString request = "";
    int count = tcpSocket->bytesAvailable() - 1;
    while (count)
    {
        QString chunk;
        stream >> chunk;
        request += chunk;
        count -= chunk.length();
    }
    qout << request << endl;
    QString response = "Howdy there server!";
    //stream.writeBytes(response.toAscii(), response.size());
    stream << response << endl;
    qDebug() << "Response sent!";
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        stop();
        break;
    case QAbstractSocket::HostNotFoundError:
        qout << "The host was not found. Please check the host name and port settings." << endl;
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qout << "The connection was refused by the peer. Make sure the fortune server is running,\n"
            "and check that the host name and port settings are correct." << endl;
        break;
    default:
        qout << QString("The following error occurred: %1.").arg(tcpSocket->errorString()) << endl;
    }
}
