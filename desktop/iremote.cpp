#include "iremote.h"
#include "crc.h"
#include <QtGui>

IRemote::IRemote(QObject *parent) :
    QObject(parent)
{
    crcInit();

    m_responseTimeout = 2000;
    m_keepAliveTimeout = 5000;
    m_queueRunning = false;
    m_autoconnectEnabled = true;

#ifdef SERIALPORT
    serialPort = NULL;
#endif
    tcpSocket = NULL;

    waitingForRespose = false;
    wantsConnection = false;

    responseTimer = new QTimer(this);
    responseTimer->setInterval(1);
    connect(responseTimer, SIGNAL(timeout()),
            this, SLOT(responseTimerTick()));

    keepAliveTimer = new QTimer(this);
    keepAliveTimer->setInterval(m_keepAliveTimeout);
    connect(keepAliveTimer, SIGNAL(timeout()),
            this, SLOT(keepAliveTimerTick()));
    keepAliveTimer->start();
}

IRemote::~IRemote()
{
#ifdef SERIALPORT
    if (serialPort != NULL)
        serialPort->close();
#endif

    if (tcpSocket != NULL)
        tcpSocket->close();
}

#ifdef SERIALPORT
bool IRemote::connectSerialPort(const QString &device)
{
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
    endQueue();
}
#endif

void IRemote::connectNetwork(QString hostname, int port)
{
    if (tcpSocket != NULL)
        closeNetwork();

    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(hostname, port);

    setNetworkHostname(hostname);
    setNetworkPort(port);
    wantsConnection = true;

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
    if ((tcpSocket != NULL) && (tcpSocket->state() == QAbstractSocket::ConnectedState))
        tcpSocket->disconnectFromHost();
    tcpSocket = NULL;
    activeConnections &= ~NetworkConnection;
    wantsConnection = false;
    emit networkDisconnected();
    endQueue();
}

#ifdef SERIALPORT
bool IRemote::isSerialPortConnected()
{
    return (serialPort != NULL);
}
#endif

bool IRemote::isNetworkConnected()
{
    return (tcpSocket != NULL);
}

void IRemote::setWlanSsid(const QString &ssid)
{
    QueueCommand command;
    command.command = QString("set w s %1\r").arg(ssid).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::setWlanPhrase(const QString &phrase)
{
    QueueCommand command;
    command.command = QString("set w p %1\r").arg(phrase).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::setWlanKey(const QString &key)
{
    QueueCommand command;
    command.command = QString("set w k %1\r").arg(key).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::setWlanHostname(const QString &hostname)
{
    QueueCommand command;
    command.command = QString("set w h %1\r").arg(hostname).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::setWlanAuth(IRemote::WlanAuthType mode)
{
    QueueCommand command;
    command.command = QString("set w a %1\r").arg((int)mode).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::setWlanDhcpMethod(IRemote::IpDhcpMethod method)
{
    QueueCommand command;
    command.command = QString("set w d %1\r").arg((int)method).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::setWlanIpAddress(QString address)
{
    QueueCommand command;
    command.command = QString("set w i %1\r").arg(address).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::setWlanSubnetMask(QString address)
{
    QueueCommand command;
    command.command = QString("set w m %1\r").arg(address).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::setWlanGateway(QString address)
{
    QueueCommand command;
    command.command = QString("set w g %1\r").arg(address).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::setIrCount(int times)
{
    QueueCommand command;
    command.command = QString("set i c %1\r").arg(times).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::setIrReceiveTimeout(int ms)
{
    QueueCommand command;
    command.command = QString("set i r %1\r").arg(ms).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::setIrSendTimeout(int ms)
{
    QueueCommand command;
    command.command = QString("set i s %1\r").arg(ms).toLocal8Bit();
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::actionRun()
{
    QueueCommand command;
    command.command = "run\r";
    command.response = "Idle";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::actionRun(IrCommand irCommand)
{
    QByteArray data;

    data.append("run ");
    data.append(dataToHex((char*)&irCommand, sizeof(irCommand)));
    data.append("\r");

    QueueCommand command;
    command.command = data;
    command.response = "Idle";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::actionCapture()
{
    QueueCommand command;
    command.command = "capture\r";
    command.response = "Capturing data";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();

    pauseKeepAlive(m_responseTimeout*20);   // let there be enough time to capture signals
}

void IRemote::startWlanAdhoc()
{
    QueueCommand command;
    command.command = "start w a\r";
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::startWlanInfrastructure()
{
    QueueCommand command;
    command.command = "start w i\r";
    command.response = "ACK";
    command.timeout = m_responseTimeout;
    command.type = NormalQueueCommandType;

    commandQueue.enqueue(command);
    startQueue();
}

void IRemote::flashFirmware(QString filename)
{
    // here the flashing is happening
    QFile file(filename);
    QByteArray data;

    if (file.open(QIODevice::ReadOnly))
    {
        int size;
        quint16 checksum;
        QByteArray commandData;

        data = file.readAll();

        // split the file in 100 bytes parts
        for (int i = 0; i < data.size(); i+=100)
        {
            if (i+100 < data.size())
                size = 100;
            else
                size = data.size() - i;

            checksum = crcFast(&(data.data()[i]), size);//qChecksum(&(data.data()[i]), size);

            commandData.clear();
            commandData.append("flash ");
            commandData.append(dataToHex(&(data.data()[i]),size));
            commandData.append(" ");
            commandData.append(QString("%1").arg(checksum,4,16,QLatin1Char('0')));
            commandData.append("\r");

            qDebug() << checksum;

            QueueCommand command;
            command.command = commandData;
            command.response = "ACK";
            command.timeout = m_responseTimeout;
            command.type = NormalQueueCommandType;

            commandQueue.enqueue(command);
            startQueue();
        }

    }
}

#ifdef SERIALPORT
void IRemote::incomingSerialData()
{
    QByteArray data;
    while (serialPort->bytesAvailable() != 0)
    {
       data = serialPort->read(1);
       incomingByte(data.at(0));
    }
}
#endif

void IRemote::incomingNetworkData()
{
    QByteArray data;
    while (tcpSocket->bytesAvailable() != 0)
    {
       data = tcpSocket->read(1);
       incomingByte(data.at(0));
    }
}

void IRemote::incomingByte(char byte)
{
    if (waitingForRespose)
    {
        responseTimer->stop();
        responseTimer->start();

        if (byte != responseString.at(responseOffset))
        {
            responseOffset = 0;

            if (byte != responseString.at(0))
            {
                responseOffset = -1;
            }
        }

        if (responseOffset < (responseString.length()-1))
            responseOffset++;
        else    // matched
        {
            responseTimer->stop();
            waitingForRespose = false;
            responseReceived(currentCommand.type);
            return;
        }

        return;
    }

    if (byte != '\r')
    {
        dataBuffer.append(byte);
    }
    else
    {
        receivedCommand(dataBuffer);
        dataBuffer.clear();
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
    closeNetwork();
}

void IRemote::tcpSocketError(QAbstractSocket::SocketError error)
{
    if (error == QAbstractSocket::NetworkError)
        qDebug() << "IRemote: network error";

    qDebug() << error;
}

void IRemote::responseTimerTick()
{
    responseTimer->stop();
    waitingForRespose = false;
    responseTimedOut(currentCommand.type);
}

void IRemote::keepAliveTimerTick()
{
    if (!keepAliveTimer->isActive())
        keepAliveTimer->start();

    if ((activeConnections == NoConnection) || isQueueRunning())    // if no connection or queue running no timeout is needed
    {
        if ((activeConnections == NoConnection) && autoconnectEnabled() && wantsConnection)
        {
            connectNetwork(networkHostname(), networkPort());
        }
    }
    else
    {
        QueueCommand command;
        command.command = "alive\r";
        command.response = "yes";
        command.timeout = m_responseTimeout;
        command.type = KeepAliveQueueCommandType;

        commandQueue.enqueue(command);
        startQueue();
    }
}

QByteArray IRemote::dataToHex(const char *data, int size)
{
    QByteArray bytes;
    for (int i = 0; i < size; i++)
    {
        bytes.append(QString("%1").arg((quint8)(data[i]),
                                      2,
                                      16,
                                      QLatin1Char('0')));
    }

    return bytes;
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
#ifdef SERIALPORT
    else if (activeConnections & SerialConnection)
    {
        serialPort->flush();
        serialPort->write(data);
    }
#endif
}

void IRemote::findInResponse(QString toMatch, int timeout)
{
    responseString = toMatch;
    responseOffset = 0;

    waitingForRespose = true;

    responseTimer->setInterval(timeout);
    responseTimer->start();
}

void IRemote::responseReceived(QueueCommandType type)
{
    if (type == NormalQueueCommandType)
    {
        doQueue();
    }
    else if (type == KeepAliveQueueCommandType)
    {
        doQueue();
    }
}

void IRemote::responseTimedOut(QueueCommandType type)
{
    if (type == NormalQueueCommandType)
    {
        error("Response timeout");
        doQueue();
    }
    else if (type == KeepAliveQueueCommandType)
    {
        error("Keep alive timeout");
        endQueue();

        if (activeConnections & NetworkConnection)
        {
            activeConnections &= ~NetworkConnection;
            closeNetwork();
            wantsConnection = true; // we lost the connection but we want it
        }
#ifdef SERIALPORT
        if (activeConnections & SerialConnection)
        {
            activeConnections &= ~SerialConnection;
            closeSerialPort();
        }
#endif

    }
}

void IRemote::startQueue()
{
    if (isQueueRunning())
        return;

    doQueue();

    queueStarted();
    setQueueRunning(true);
}


void IRemote::doQueue()
{

    if (commandQueue.isEmpty())
    {
        emit queueFinished();
        setQueueRunning(false);

        return;
    }

    currentCommand = commandQueue.dequeue();

    sendData(currentCommand.command);
    findInResponse(currentCommand.response, currentCommand.timeout);
}

void IRemote::endQueue()
{
    setQueueRunning(false);
}

void IRemote::pauseKeepAlive(int msecs)
{
    keepAliveTimer->stop();
    QTimer::singleShot(msecs, this, SLOT(keepAliveTimerTick()));
}
