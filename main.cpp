#include <QtCore/QCoreApplication>
#include <QDebug>
#include "client.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    Client* client = new Client();
    client->start();
    return app.exec();
}
