#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();

signals:
    void DataRead(int);

public slots:
    void Start();
    void OnNewConnection();
    void OnBytesWritten(qint64 bytes);
    void OnReadyRead();

private:
    QTcpServer* m_server;
};

#endif // SERVER_H
