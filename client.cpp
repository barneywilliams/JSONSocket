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
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readFortune()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(displayError(QAbstractSocket::SocketError)));
}

void Client::requestNewFortune()
{
    blockSize = 0;
    tcpSocket->abort();
    tcpSocket->connectToHost("localhost", 19024);
}

void Client::readFortune()
{
    QDataStream in(tcpSocket);

    // Start by reading the block size from the message
    if (blockSize == 0)
    {
        if (tcpSocket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> blockSize;
    }

    // Return if the complete message is not yet available
    if (tcpSocket->bytesAvailable() < blockSize)
        return;

    // Read in the complete fortune
    QString nextFortune;
    in >> nextFortune;

    // Request the next fortune to start the transimission
    if (nextFortune == currentFortune)
    {
        QTimer::singleShot(0, this, SLOT(requestNewFortune()));
        return;
    }

    currentFortune = nextFortune;
    qout << currentFortune << endl;
}

void Client::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
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
