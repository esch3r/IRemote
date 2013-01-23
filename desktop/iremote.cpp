#include "iremote.h"
#include <QtGui>

IRemote::IRemote(QObject *parent) :
    QObject(parent)
{
    m_responseTimeout = 2000;

    serialPort = NULL;
    tcpSocket = NULL;

    waitingForRespose = false;

    responseTimer = new QTimer(this);
    responseTimer->setInterval(1);
    connect(responseTimer, SIGNAL(timeout()),
            this, SLOT(responseTimerTick()));
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

bool IRemote::setWlanDhcpMethod(IRemote::IpDhcpMethod method)
{
    sendData(QString("set w d %1").arg((int)method).toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

bool IRemote::setWlanIpAddress(QString address)
{
    sendData(QString("set w i %1").arg(address).toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

bool IRemote::setWlanSubnetMask(QString address)
{
    sendData(QString("set w m %1").arg(address).toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

bool IRemote::setWlanGateway(QString address)
{
    sendData(QString("set w g %1").arg(address).toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

bool IRemote::setIrRepeat(int times)
{
    sendData(QString("set i r %1").arg(times).toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

bool IRemote::setIrTimeout(int ms)
{
    sendData(QString("set i t %1").arg(ms).toLocal8Bit());
    return findInResponse("ACK", m_responseTimeout);
}

bool IRemote::actionRun()
{
    sendData("run\r");
    return findInResponse("Going into idle", m_responseTimeout);
}

bool IRemote::actionRun(IrCommand irCommand)
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
    return findInResponse("Start running command", m_responseTimeout);
}

bool IRemote::actionCapture()
{
    sendData("capture\r");
    return findInResponse("Start capturing data", m_responseTimeout);
}

bool IRemote::startWlanAdhoc()
{
    sendData("start w a\r");
    return findInResponse("ACK", m_responseTimeout);
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

void IRemote::responseTimerTick()
{
    if (!((activeConnections & NetworkConnection) || (activeConnections & SerialConnection)))
    {
        responseTimer->stop();
        error("No network connection");
        return;
    }

    char byteRead;

    if (currentResponseTimeout > 0) // check time
    {
        if (responseTime.elapsed() > currentResponseTimeout)
        {
            responseTimer->stop();
            error("Response timeout");
            return;
        }
    }

    if (activeConnections & NetworkConnection)  // get data
    {
        if (tcpSocket->bytesAvailable())
        {
            tcpSocket->read(&byteRead, 1);
        }
        else
            return;
    }
    else if (activeConnections & SerialConnection)
    {
        if (serialPort->bytesAvailable())
        {
            serialPort->read(&byteRead, 1);
        }
        else
            return;
    }

    responseTime.restart();     // reset time

    if (byteRead != responseString.at(responseOffset))
    {
        responseOffset = 0;

        if (byteRead != responseString.at(0))
        {
            responseOffset = -1;
        }
        //responseOffset++;
        //return;
    }

    if (responseOffset < (responseString.length()-1))
        responseOffset++;
    else    // matched
    {
        responseTimer->stop();
        waitingForRespose = false;
        doQueue();
        qDebug() << "test";
        return;
    }

    responseTimerTick();    // next step
}

void IRemote::receivedCommand(QByteArray command)
{
    const QByteArray dataCode("*DATA");

    if (command.size() == 0)
        return;

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
    responseString = toMatch;
    responseOffset = 0;
    currentResponseTimeout = timeout;
    responseTime.restart();

    waitingForRespose = true;

    responseTimer->start();
}


void IRemote::doQueue()
{
}
