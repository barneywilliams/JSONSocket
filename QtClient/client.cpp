#include <QTextStream>
#include <QScriptEngine>
#include <QScriptValue>
#include <iostream>

#include "client.h"

Client::Client()
    : QObject(0), qout(stdout), tcpSocket(NULL), m_NumRequestsProcessed(0)
{
    qout << "client: JSON client created!" << endl;

    tcpSocket = new QTcpSocket(this);
    Q_ASSERT(connect(tcpSocket, SIGNAL(hostFound()), this, SLOT(OnHostFound())));
    Q_ASSERT(connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(ProcessRequest())));
    Q_ASSERT(connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(DisplayError(QAbstractSocket::SocketError))));
}

void Client::Start()
{
    tcpSocket->connectToHost("127.0.0.1", 19024);
    qout << "client: Connection initiated." << endl;
}

void Client::Stop()
{
    tcpSocket->close();
}

void Client::OnHostFound()
{
    qout << "client: Found host!";
}

int Client::NumRequestsProcessed()
{
    return m_NumRequestsProcessed;
}

void Client::ProcessRequest(void)
{
    QByteArray request = tcpSocket->readAll();
    QScriptEngine engine;
    QString response;

    // Validate that the request was parsed and non-empty
    QScriptValue data = engine.evaluate("(" + QString(request) + ")");
    if (!data.isNull() && !data.isValid())
    {
        qout << "client: Empty/malformed message received from server!";
    }
    else
    {
        // Validate request data is present
        QScriptValue req = data.property("request");
        if (!req.isValid())
        {
            qout << "client: Invalid request received from server!";
        }

        // Process command
        else
        {
            response = ProcessCommand(req);
        }
    }

    // Send the response
    if (response.length() > 0)
    {
        tcpSocket->write(response.toAscii());
        qout << response << endl;
    }

    m_NumRequestsProcessed += 1;
}

QString Client::ProcessCommand(QScriptValue req)
{
    QScriptValue cmd = req.property("command");
    QString response;

    if (!cmd.isValid())
    {
        response = "{\"error\": {\"command\": \"invalid\", \"message\": \"No command specified\"}}";
        return response;
    }

    // Process greeting
    if (cmd.toString() == "greeting")
    {
        QScriptValue message = req.property("message");

        if (!message.isValid())
        {
            response = "{\"error\": {\"command\": \"greeting\", \"message\": \"No message specified\"}}";
        }
        else
        {
            response = "{\"response\": {\"command\": \"greeting\", \"message\": \"Client reporting for duty!\"}}";
        }
    }

    // Handle unrecognized requests
    else
    {
        response = "{\"error\": {\"command\": \"invalid\", \"message\": \"Unknown request specified\"}}";
    }

    return response;
}

void Client::DisplayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError)
    {
    case QAbstractSocket::RemoteHostClosedError:
        Stop();
        break;
    case QAbstractSocket::HostNotFoundError:
        qout << "client: The host was not found. Please check the host name and port settings." << endl;
        break;
    case QAbstractSocket::ConnectionRefusedError:
        qout << "client: The connection was refused by the peer. Make sure the fortune server is running,\n"
                "client: and check that the host name and port settings are correct." << endl;
        break;
    default:
        qout << QString("client: The following error occurred: %1.").arg(tcpSocket->errorString()) << endl;
    }
}
