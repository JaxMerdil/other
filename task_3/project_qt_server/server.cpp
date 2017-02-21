#include "server.h"
#include <QTcpSocket>

Server::Server(QObject *parent)
    : QObject(parent)
    , m_server(new QTcpServer(this))
{
}


Server::~Server()
{
    delete m_server;
}

void Server::Start()
{
    connect(m_server, SIGNAL(newConnection()), this, SLOT(OnNewConnection()));

    if(!m_server->listen(QHostAddress::Any, 9999))
    {
        qDebug() << "Server could not start!";
    }
    else
    {
        qDebug() << "Server started!";
    }
}

void Server::OnNewConnection()
{
    qDebug() << "Connected (server)!";

    QTcpSocket *socket = m_server->nextPendingConnection();

    connect(socket, SIGNAL(readyRead()), this, SLOT(OnReadyRead()));
}

void Server::OnBytesWritten(qint64 bytes)
{
    qDebug() << "We wrote: " << bytes << " bytes";

    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());
    socket->close();
}


void Server::OnReadyRead()
{
    QTcpSocket* socket = static_cast<QTcpSocket*>(sender());

    QByteArray data = socket->readAll();
    //    QString s_data = QString::fromUtf8(data.data());

    qDebug() << "Read: " << data.size() << endl;
    //    qDebug() << "Data: " << s_data << endl;
    //    connect(socket, SIGNAL(bytesWritten(qint64)), this, SLOT(onBytesWritten(qint64)));

    socket->write(data);
//    emit DataRead(data.size());
}
