#include "iremotewindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    IRemoteWindow w;
    w.show();
    
    return a.exec();
}
