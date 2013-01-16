#ifndef IREMOTE_H
#define IREMOTE_H

#define IR_MAX_TRANSITIONS 100

#include <QObject>
#include <QTcpSocket>
#include <QtAddOnSerialPort/serialport.h>
#include <QtAddOnSerialPort/serialportinfo.h>

QT_USE_NAMESPACE_SERIALPORT

typedef struct {
    quint16  data[IR_MAX_TRANSITIONS];
    quint8   length;
    quint16  id;
} IrCommand;

class IRemote : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int responseTimeout READ responseTimeout WRITE setResponseTimeout NOTIFY responseTimeoutChanged)

    Q_FLAGS(ActiveConnection ActiveConnections)


public:


    enum ActiveConnection {
        SerialConnection = 0x01,
        NetworkConnection = 0x02
    };
    Q_DECLARE_FLAGS(ActiveConnections, ActiveConnection)

    enum WlanAuthType {
        OpenAuthType = 0,
        WEP128AuthType = 1,
        WPA1AuthType = 2,
        MixedWPA1AndWPA2PSKAuthType = 3,
        WPA2PSKAuthType = 4,
        AdhocAuthType = 6,
        WPE64AuthType = 7
    };

    enum IpDhcpMethod {
        DhcpOffMethod = 0,
        DhcpOnMethod = 1,
        AutoIpMethod = 2,
        DhcpCacheMethod = 3
    };

    explicit IRemote(QObject *parent = 0);
    ~IRemote();

    bool connectSerialPort(const QString &device);
    void closeSerialPort();
    void connectNetwork(QString hostname, int port);
    void closeNetwork();

    bool isSerialPortConnected();
    bool isNetworkConnected();

    bool startWlanConfig();
    bool saveWlanConfig();
    bool setWlanSsid(const QString &ssid);
    bool setWlanPhrase(const QString &phrase);
    bool setWlanKey(const QString &key);
    bool setWlanHostname(const QString &hostname);
    bool setWlanAuth(WlanAuthType mode);
    bool setWlanDhcpMethod(IpDhcpMethod method);
    bool setWlanIpAddress(QString address);
    bool setWlanSubnetMask(QString address);
    bool setWlanGateway(QString address);

    bool setIrRepeat(int times);
    bool setIrTimeout(int ms);

    bool actionRun();
    bool actionRun(IrCommand irCommand);
    bool actionCapture();
    bool startWlanAdhoc();
    
    int responseTimeout() const
    {
        return m_responseTimeout;
    }

signals:
    void irCommandReceived(IrCommand irCommand);
    void serialPortConnected();
    void networkConnected();
    void serialPortDisconnected();
    void networkDisconnected();

    void responseTimeoutChanged(int arg);

public slots:

void setResponseTimeout(int arg)
{
    if (m_responseTimeout != arg) {
        m_responseTimeout = arg;
        emit responseTimeoutChanged(arg);
    }
}

private slots:
    void incomingSerialData();
    void incomingNetworkData();
    void tcpSocketConnected();
    void tcpSocketDisconnected();
    void tcpSocketError(QAbstractSocket::SocketError error);

private:
    SerialPort *serialPort;
    QTcpSocket *tcpSocket;
    QByteArray dataBuffer;
    bool waitingForRespose;

    ActiveConnections activeConnections;

    void receivedCommand(QByteArray command);
    void sendData(const QByteArray &data);
    bool findInResponse(QString toMatch, int timeout);
    
    int m_responseTimeout;
};

#endif // IREMOTE_H
