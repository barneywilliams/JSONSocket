#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>
#include <QScriptValue>

QT_BEGIN_NAMESPACE
class QTextStream;
class QObject;
class QTcpSocket;
class QDataStream;
class QByteArray;
class QScriptValue;
QT_END_NAMESPACE

class Client : public QObject
{
   Q_OBJECT

public:
   Client();
   ~Client(){};
   void Start();
   void Stop();

private slots:
   void OnHostFound();
   void ProcessRequest();
   QString ProcessCommand(QScriptValue cmd);
   void DisplayError(QAbstractSocket::SocketError socketError);

private:
   QString CommandResponse(QString cmd, QString msg);
   QString CommandErrorResponse(QString cmd, QString msg);
   QString CreateResponse(QString type, QString cmd, QString msg);

   QTextStream qout;
   QTcpSocket *tcpSocket;
   bool shutdownRequested;
};

#endif
