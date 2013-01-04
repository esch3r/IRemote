#include "iremote.h"

IRemote::IRemote(QObject *parent) :
    QObject(parent)
{
    serialPort = NULL;
    tcpSocket = NULL;
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

bool IRemote::connectNetwork(QString hostname, int port)
{
    if (tcpSocket != NULL)
        closeNetwork();

    tcpSocket = new QTcpSocket(this);
    tcpSocket->connectToHost(hostname, port);

    if (tcpSocket->waitForConnected(10000))
    {
        connect(tcpSocket, SIGNAL(readyRead()),
                this, SLOT(incomingNetworkData()));
        activeConnections |= NetworkConnection;

        emit networkConnected();

        qDebug() << "IRemote: connected to network device";
        return true;
    }
    else
    {
        qDebug() << "IRemote: not connected to network device";
        return false;
    }
}

void IRemote::closeNetwork()
{
    tcpSocket->close();
    tcpSocket->deleteLater();
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

void IRemote::setWlanSsid(const QString &ssid)
{
    sendData(QString("set w s %1\r").arg(ssid).toLocal8Bit());
}

void IRemote::setWlanPhrase(const QString &phrase)
{
    sendData(QString("set w p %1\r").arg(phrase).toLocal8Bit());
}

void IRemote::setWlanKey(const QString &key)
{
    sendData(QString("set w k %1\r").arg(key).toLocal8Bit());
}

void IRemote::setWlanHostname(const QString &hostname)
{
    sendData(QString("set w h %1\r").arg(hostname).toLocal8Bit());
}

void IRemote::setWlanAuth(IRemote::WlanAuthType mode)
{
    sendData(QString("set w a %1\r").arg((int)mode).toLocal8Bit());
}

void IRemote::actionRun()
{
    sendData("run\r");
}

void IRemote::actionRun(IrCommand irCommand)
{
    QByteArray data;
    data.append("run ");
    data.append((char*)(&irCommand), sizeof(IrCommand));
    data.append("\r");

    sendData(data);
}

void IRemote::actionCapture()
{
    sendData("capture\r");
}

void IRemote::incomingSerialData()
{
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
           qDebug() << dataBuffer;
           receivedCommand(dataBuffer);
           dataBuffer.clear();
       }
    }
}

void IRemote::incomingNetworkData()
{
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
           qDebug() << dataBuffer;
           receivedCommand(dataBuffer);
           dataBuffer.clear();
       }
    }
}

void IRemote::receivedCommand(QByteArray command)
{
    const QByteArray dataCode("*DATA");

    if (command.at(0) == '*')   // this command sends us something necessary
    {
        if (command.indexOf(dataCode) == 0)
        {
            command.remove(0,dataCode.length());
            IrCommand irCommand;
            memcpy(&irCommand,command.data(), sizeof(IrCommand));
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
        serialPort->write(data);
    }
}
