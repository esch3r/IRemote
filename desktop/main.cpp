#include "iremotewindow.h"
#include <QApplication>
#include <QtSingleApplication>

int main(int argc, char *argv[])
{
    QtSingleApplication instance(argc, argv);
    if (instance.sendMessage(argv[1]))
    {
        qWarning("IRemote is running, sent message to iremote");
        return 0;
    }

    IRemoteWindow w;
    //QApplication::setQuitOnLastWindowClosed(false);

    instance.setActivationWindow(&w);

    QObject::connect(&instance, SIGNAL(messageReceived(const QString&)),
                     &w, SLOT(applicationStarted(QString)));

    w.show();

    return instance.exec();
}
