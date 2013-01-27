#ifndef IREMOTE_H
#define IREMOTE_H

#define IR_MAX_TRANSITIONS 100

#include <QObject>
#include <QTcpSocket>
#include <QQueue>
#include <QTimer>
#include <QtGui>

#ifdef SERIALPORT
#include <QtAddOnSerialPort/serialport.h>
#include <QtAddOnSerialPort/serialportinfo.h>
QT_USE_NAMESPACE_SERIALPORT
#endif

typedef struct {
    quint16  data[IR_MAX_TRANSITIONS];
    quint8   length;
    quint16  id;
} IrCommand;

enum QueueCommandType {
    NormalQueueCommandType = 0,
    KeepAliveQueueCommandType = 1,
    GroupQueueCommandType = 2
};

typedef struct {
    QByteArray command;
    QByteArray response;
    int timeout;
    QueueCommandType type;//= NormalQueueCommandType;
} QueueCommand;

class IRemote : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int responseTimeout READ responseTimeout WRITE setResponseTimeout NOTIFY responseTimeoutChanged)
    Q_PROPERTY(bool queueRunning READ isQueueRunning WRITE setQueueRunning NOTIFY queueRunningChanged)
    Q_PROPERTY(int keepAliveTimeout READ keepAliveTimeout WRITE setKeepAliveTimeout)
    Q_PROPERTY(bool autoconnectEnabled READ autoconnectEnabled WRITE setAutoconnectEnabled NOTIFY autoconnectEnabledChanged)
    Q_PROPERTY(QString networkHostname READ networkHostname WRITE setNetworkHostname NOTIFY networkHostnameChanged)
    Q_PROPERTY(int networkPort READ networkPort WRITE setNetworkPort NOTIFY networkPortChanged)

    Q_FLAGS(ActiveConnection ActiveConnections)

public:

    enum ActiveConnection {
        NoConnection = 0x00,
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

#ifdef SERIALPORT
    bool connectSerialPort(const QString &device);
    void closeSerialPort();
#endif
    void connectNetwork(QString hostname, int port);
    void closeNetwork();

    bool isSerialPortConnected();
    bool isNetworkConnected();

    void startWlanConfig();
    void saveWlanConfig();
    void setWlanSsid(const QString &ssid);
    void setWlanPhrase(const QString &phrase);
    void setWlanKey(const QString &key);
    void setWlanHostname(const QString &hostname);
    void setWlanAuth(WlanAuthType mode);
    void setWlanDhcpMethod(IpDhcpMethod method);
    void setWlanIpAddress(QString address);
    void setWlanSubnetMask(QString address);
    void setWlanGateway(QString address);

    void setIrRepeat(int times);
    void setIrTimeout(int ms);

    void actionRun();
    void actionRun(IrCommand irCommand);
    void actionCapture();
    void startWlanAdhoc();

    void flashFirmware(QString filename);
    
    int responseTimeout() const
    {
        return m_responseTimeout;
    }

    bool isQueueRunning() const
    {
        return m_queueRunning;
    }

    int keepAliveTimeout() const
    {
        return m_keepAliveTimeout;
    }

    bool autoconnectEnabled() const
    {
        return m_autoconnectEnabled;
    }

    QString networkHostname() const
    {
        return m_networkHostname;
    }

    int networkPort() const
    {
        return m_networkPort;
    }

signals:
    void irCommandReceived(IrCommand irCommand);
    void networkConnected();
    void networkDisconnected();

#ifdef SERIALPORT
    void serialPortConnected();
    void serialPortDisconnected();
#endif

    void responseTimeoutChanged(int arg);

    void queueFinished();
    void queueStarted();

    void error(QString text);

    void queueRunningChanged(bool arg);

    void autoconnectEnabledChanged(bool arg);

    void networkHostnameChanged(QString arg);

    void networkPortChanged(int arg);

public slots:

void setResponseTimeout(int arg)
{
    if (m_responseTimeout != arg) {
        m_responseTimeout = arg;
        emit responseTimeoutChanged(arg);
    }
}

void setQueueRunning(bool arg)
{
    if (m_queueRunning != arg) {
        m_queueRunning = arg;
        emit queueRunningChanged(arg);
    }
}

void setKeepAliveTimeout(int arg)
{
    m_keepAliveTimeout = arg;
    keepAliveTimer->setInterval(arg);
}

void setAutoconnectEnabled(bool arg)
{
    m_autoconnectEnabled = arg;
}

void setNetworkHostname(QString arg)
{
    if (m_networkHostname != arg) {
        m_networkHostname = arg;
        emit networkHostnameChanged(arg);
    }
}

void setNetworkPort(int arg)
{
    if (m_networkPort != arg) {
        m_networkPort = arg;
        emit networkPortChanged(arg);
    }
}

private slots:
#ifdef SERIALPORT
    void incomingSerialData();
#endif
    void incomingNetworkData();
    void incomingByte(char byte);
    void tcpSocketConnected();
    void tcpSocketDisconnected();
    void tcpSocketError(QAbstractSocket::SocketError error);
    void responseTimerTick();
    void keepAliveTimerTick();

private:
#ifdef SERIALPORT
    SerialPort *serialPort;
#endif
    QTcpSocket *tcpSocket;
    QByteArray dataBuffer;
    bool waitingForRespose;

    bool wantsConnection;   // this variable indicates wheter a user wants a connection or not, necessary for autoconnect

    ActiveConnections activeConnections;

    QQueue<QueueCommand> commandQueue;
    QueueCommand currentCommand;
    QTimer *responseTimer;
    QString responseString;
    int     responseOffset;

    QTimer *keepAliveTimer;
    bool    keepAliveReceived;

    QByteArray dataToHex(const char* data, int size);

    void receivedCommand(QByteArray command);
    void sendData(const QByteArray &data);
    void findInResponse(QString toMatch, int timeout);
    void responseReceived(QueueCommandType type);
    void responseTimedOut(QueueCommandType type);

    void startQueue();
    void doQueue();
    void endQueue();

    void pauseKeepAlive(int msecs);
    
    int m_responseTimeout;
    bool m_queueRunning;
    int m_keepAliveTimeout;
    bool m_autoconnectEnabled;
    QString m_networkHostname;
    int m_networkPort;
};

#endif // IREMOTE_H
