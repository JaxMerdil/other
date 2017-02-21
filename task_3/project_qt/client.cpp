#include "client.h"
#include <iostream>
#include <QDateTime>
#include <cstring>
#include <iomanip>

Client::Client(QObject *parent)
    : QObject(parent)
    , m_socket(new QTcpSocket(this))
    , m_latency(0)
    , m_nByte(0)
    , m_nPackage(0)
{
    connect(m_socket, SIGNAL(connected()),          this, SLOT(OnConnected()));
    connect(m_socket, SIGNAL(disconnected()),       this, SLOT(OnDisconnected()));
    connect(m_socket, SIGNAL(readyRead()),          this, SLOT(OnReadyRead()));
    connect(m_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(OnBytesWritten(qint64)));
}

Client::~Client()
{
    delete m_socket;
}

void Client::Connect()
{
    m_socket->connectToHost("127.0.01", 9999);
}

void Client::SetValue(int newValue)
{
    m_value = newValue;
    emit ValueSet(m_value);
}

void Client::WriteSocketPackage()
{
    const int size = 20;
    char m_package[size]="PACKAGE";
    std::string m_stime = std::to_string(QDateTime::currentMSecsSinceEpoch());
    char m_strToChar[m_stime.length()];
    for(unsigned int i=0; i<m_stime.length(); ++i)
    {
        m_strToChar[i] = m_stime[i];
    }
    const int sizeInf = strlen(m_package)+m_stime.length();
    std::strcat(m_package, m_strToChar);
    const int m_size = 4096;
    char str[m_size];
    memset(str,'f',m_size);
    for(int i=0; i<sizeInf; ++i)
    {
        str[i]=m_package[i];
    }
    str[m_size]='\0';
    m_socket->write(str);
}

void Client::ParameterThroughputAndLtency(const QByteArray &byte)
{
    if(byte.toStdString().find("PACKAGE") == -1) return ;
    std::string DataOld = "";
    for(int i = byte.toStdString().find("PACKAGE") + 7; i < byte.toStdString().find("PACKAGE") + 20; ++i)
    {
        //if(std::isalpha(byte[i])) break;
        //std::cout << byte[i];
        if(!std::isalpha(byte[i]))
        DataOld += byte[i];
    }
    m_nPackage ++;
   // std::cout<<DataOld<<std::endl;
    m_latency += QDateTime::currentMSecsSinceEpoch() - std::atoll(DataOld.c_str());
    m_nByte += byte.size();
    if(m_latency > 1000)
    {
        m_throughput = m_nByte / m_latency;
        qDebug() << "Throughput: " << /*(m_throughput*8)/1000*/m_nByte*8*1000/(m_latency*1024*1024) << "Mbit/s" << " | " << "Latency "<< m_latency/*/m_nPackage*/ << "m";
        m_latency = 0;
        m_nByte = 0;
    }
}

void Client::OnConnected()
{
    //    qDebug() << "Connected (client)!";
    WriteSocketPackage();
       //    qDebug() << "Written!";
}

void Client::OnDisconnected()
{
//    qDebug() << "Disconnected!";
}

void Client::OnBytesWritten(qint64 bytes)
{
//    qDebug() << "We wrote: " << bytes << " bytes";
}

void Client::OnReadyRead()
{
    QByteArray byte = m_socket->readAll();
  // qDebug() <<"Read: " <<byte ;
    ParameterThroughputAndLtency(byte);
    OnConnected();
}
