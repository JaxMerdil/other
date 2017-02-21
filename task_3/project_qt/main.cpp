#include <iostream>
#include <QCoreApplication>
#include <QTimer>
#include <QThread>
#include <QDateTime>
#include "client.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

        Client c;
        c.Connect();

        return app.exec();
}
