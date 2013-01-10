#include "iremote.h"
#include <QtGui>

IRemote::IRemote(QObject *parent) :
    QObject(parent)
{
    serialPort = NULL;
    tcpSocket = NULL;

    waitingForRespose = false;
}

IRemote::~IRemote()
{
    if (serialPort != NULL)
        serialPort->close();

    if (tcpSocket != NULL)
        tcpSocket->close();
}

bool IRemote::connectSerialPort(const QString &device)
{
    /*foreach (const SerialPortInfo &info, SerialPortInfo::availablePorts()) {
            qDebug() << "Name        : " << info.portName();
            qDebug() << "Description : " << info.description();
            qDebug() << "Manufacturer: " << info.manufacturer();

            // Example use SerialPort
            SerialPort serial;
            serial.setPort(info);
            if (serial.open(QIODevice::ReadWrite))
                serial.close();
        }*/
    if (serialPort != NULL)
        closeSerialPort();

    serialPort = new SerialPort(this);
    serialPort->setPort(device);
    serialPort->setDataBits(SerialPort::Data8);
    serialPort->setParity(SerialPort::NoParity);
    serialPort->setStopBits(SerialPort::OneStop);
    serialPort->setRate(115200);
    if (serialPort->open(QIODevice::ReadWrite))
    {
        connect(serialPort, SIGNAL(readyRead()),
                this, SLOT(incomingSerialData()));
        activeConnections |= SerialConnection;

        emit serialPortConnected();
        serialPort->write("\r");

        qDebug() << "IRemote: connected to serial device";
        return true;
    }
    else
    {
        qDebug() << "IRemote: not connected to serial device";
        return false;
    }

}

void IRemote::closeSerialPort()
{
    serialPort->close();
    serialPort->deleteLater();
    serialPort = NULL;
    activeConnections &= ~SerialConnection;
    emit serialPortDisconnected();
}

void IRemote::connectNetwork(QString hostname, int port)
{
    if (tcpSocket != NULL)
        closeNetwork();

    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(hostname, port);

    connect(tcpSocket, SIGNAL(connected()),
            this, SLOT(tcpSocketConnected()));
    connect(tcpSocket, SIGNAL(disconnected()),
            this, SLOT(tcpSocketDisconnected()));
    connect(tcpSocket, SIGNAL(readyRead()),
            this, SLOT(incomingNetworkData()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpSocketError(QAbstractSocket::SocketError)));
}

void IRemote::closeNetwork()
{
    if (tcpSocket->state() == QAbstractSocket::ConnectedState)
        tcpSocket->disconnectFromHost();
    tcpSocket = NULL;
    activeConnections &= ~NetworkConnection;
    emit networkDisconnected();
}

bool IRemote::isSerialPortConnected()
{
    return (serialPort != NULL);
}

bool IRemote::isNetworkConnected()
{
    return (tcpSocket != NULL);
}

bool IRemote::startWlanConfig()
{
    sendData(QString("start w c\r").toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

bool IRemote::saveWlanConfig()
{
    sendData(QString("save w c\r").toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout*2);
}

bool IRemote::setWlanSsid(const QString &ssid)
{
    sendData(QString("set w s %1\r").arg(ssid).toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

bool IRemote::setWlanPhrase(const QString &phrase)
{
    sendData(QString("set w p %1\r").arg(phrase).toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

bool IRemote::setWlanKey(const QString &key)
{
    sendData(QString("set w k %1\r").arg(key).toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

bool IRemote::setWlanHostname(const QString &hostname)
{
    sendData(QString("set w h %1\r").arg(hostname).toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

bool IRemote::setWlanAuth(IRemote::WlanAuthType mode)
{
    sendData(QString("set w a %1\r").arg((int)mode).toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

void IRemote::actionRun()
{
    sendData("run\r");
}

void IRemote::actionRun(IrCommand irCommand)
{
    int commandSize = sizeof(IrCommand);
    QByteArray data;

    data.append("run ");
    for (int i = 0; i < commandSize; i++)
    {
        data.append(QString("%1").arg((quint8)(((char*)(&irCommand))[i]),
                                      2,
                                      16,
                                      QLatin1Char('0')));
    }
    data.append("\r");

    sendData(data);
}

void IRemote::actionCapture()
{
    sendData("capture\r");
}

void IRemote::incomingSerialData()
{
    if (waitingForRespose)
        return;

    QByteArray data;
    while (serialPort->bytesAvailable() != 0)
    {
       data = serialPort->read(1);
       if (data != "\r")
       {
           dataBuffer.append(data);
       }
       else
       {
           receivedCommand(dataBuffer);
           dataBuffer.clear();
       }
    }
}

void IRemote::incomingNetworkData()
{
    if (waitingForRespose)
        return;

    QByteArray data;
    while (tcpSocket->bytesAvailable() != 0)
    {
       data = tcpSocket->read(1);
       if (data != "\r")
       {
           dataBuffer.append(data);
       }
       else
       {
           receivedCommand(dataBuffer);
           dataBuffer.clear();
       }
    }
}

void IRemote::tcpSocketConnected()
{
    activeConnections |= NetworkConnection;
    emit networkConnected();

    qDebug() << "IRemote: connected to network device";
}

void IRemote::tcpSocketDisconnected()
{
    qDebug() << "IRemote: disconnected network device";
}

void IRemote::tcpSocketError(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::NetworkError)
        qDebug() << "IRemote: network error";

    qDebug() << error;
}

void IRemote::receivedCommand(QByteArray command)
{
    const QByteArray dataCode("*DATA");

    if (command.at(0) == '*')   // this command sends us something necessary
    {
        if (command.indexOf(dataCode) == 0)
        {
            QByteArray resultingCommand;
            bool ok;

            command.remove(0,dataCode.length());
            for (int i = 0; i < command.size(); i+=2)   // mash 2 bytes to 1
            {
                resultingCommand.append(command.mid(i,2).toUInt(&ok, 16));
            }
            IrCommand irCommand;
            memcpy(&irCommand,resultingCommand.data(), sizeof(IrCommand));
            irCommandReceived(irCommand);
        }
    }
    else
        qDebug() << "IRemote received:" << command;
}

void IRemote::sendData(const QByteArray &data)
{
    if (activeConnections & NetworkConnection)
    {
        tcpSocket->write(data);
    }
    else if (activeConnections & SerialConnection)
    {
        serialPort->flush();
        serialPort->write(data);
    }
}

bool IRemote::findInResponse(QString toMatch, int timeout)
{
    QTime timeoutTime;
    int offset;
    char byteRead;

    waitingForRespose = true;

    for (offset = 0; offset < toMatch.length(); offset++)
    {
        timeoutTime.restart();

        while (!serialPort->bytesAvailable())
        {
            if (timeout > 0)
            {
                if (timeoutTime.elapsed() > timeout)
                {
                    waitingForRespose = false;
                    return false;
                }
            }
            QTime time;
            time.start();
            while (time.elapsed() < 1)
                ;
        }

        serialPort->read(&byteRead, 1);

        if (byteRead != toMatch.at(offset))
        {
            offset = 0;

            if (byteRead != toMatch.at(0))
            {
                offset = -1;
            }
            continue;
        }
    }

    waitingForRespose = false;
    return true;
}
