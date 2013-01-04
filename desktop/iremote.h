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

    explicit IRemote(QObject *parent = 0);
    ~IRemote();

    bool connectSerialPort(const QString &device);
    void closeSerialPort();
    bool connectNetwork(QString hostname, int port);
    void closeNetwork();

    bool isSerialPortConnected();
    bool isNetworkConnected();

    void setWlanSsid(const QString &ssid);
    void setWlanPhrase(const QString &phrase);
    void setWlanKey(const QString &key);
    void setWlanHostname(const QString &hostname);
    void setWlanAuth(WlanAuthType mode);

    void actionRun();
    void actionRun(IrCommand irCommand);
    void actionCapture();
    
signals:
    void irCommandReceived(IrCommand irCommand);
    void serialPortConnected();
    void networkConnected();
    void serialPortDisconnected();
    void networkDisconnected();

public slots:

private slots:
    void incomingSerialData();
    void incomingNetworkData();

private:
    SerialPort *serialPort;
    QTcpSocket *tcpSocket;
    QByteArray dataBuffer;

    ActiveConnections activeConnections;

    void receivedCommand(QByteArray command);
    void sendData(const QByteArray &data);
    
};

#endif // IREMOTE_H
