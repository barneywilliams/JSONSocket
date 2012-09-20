#include <QTextStream>
#include <QScriptEngine>
#include <QScriptValue>
#include <QCoreApplication>
#include <iostream>

#include "client.h"

Client::Client()
    : QObject(0), qout(stdout), tcpSocket(NULL), shutdownRequested(false)
{
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
    tcpSocket->waitForBytesWritten(200);
    tcpSocket->close();
}

void Client::OnHostFound()
{
    qout << "client: Found host!" << endl;
}

void Client::ProcessRequest(void)
{
    QByteArray request = tcpSocket->readAll();
    QScriptEngine engine;
    QString response;

    qout << "client: << " << request << endl;

    // Validate that the request was parsed and non-empty
    QScriptValue data = engine.evaluate("(" + QString(request) + ")");
    if (!data.isNull() && !data.isValid())
    {
        qout << "client: Empty/malformed message received from server!" << endl;
    }
    else
    {
        // Validate request data is present
        QScriptValue req = data.property("request");
        if (!req.isValid())
        {
            qout << "client: Invalid request received from server!" << endl;
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
        qout << "client: >> " << response << endl;
    }

    if (shutdownRequested)
    {
        Stop();
        qout << "client: Shutting down per request..." << endl;
        QCoreApplication::exit(0);
    }
}

QString Client::CreateResponse(QString type, QString cmd, QString msg)
{
    return "{\"" + type + "\":{\"command\":\"" + cmd + "\",\"message\":\"" + msg + "\"}}";
}

QString Client::CommandResponse(QString cmd, QString msg)
{
    return CreateResponse("response", cmd, msg);
}

QString Client::CommandErrorResponse(QString cmd, QString msg)
{
    return CreateResponse("error", cmd, msg);
}

QString Client::ProcessCommand(QScriptValue req)
{
    QScriptValue cmd = req.property("command");
    QString response;

    // Basic sanity check
    if (!cmd.isValid())
    {
        response = CommandErrorResponse("invalid", "No command specified");
    }

    // Process greeting
    else if (cmd.toString() == "greeting")
    {
        QScriptValue message = req.property("message");

        if (!message.isValid())
        {
            response = CommandErrorResponse("greeting", "No message specified");
        }
        else
        {
            response = CommandResponse("greeting", "Client reporting for duty.");
        }
    }

    // Process shutdown request
    else if (cmd.toString() == "shutdown")
    {
        response = CommandResponse("shutdown", "Goodbye!");
        shutdownRequested = true;
    }

    // Handle unrecognized requests
    else
    {
        response = CommandErrorResponse("invalid", "Unsupported command specified");
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
