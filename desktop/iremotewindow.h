#ifndef IREMOTEWINDOW_H
#define IREMOTEWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QSettings>
#include <QFileDialog>
#include "graphicbutton.h"
#include <QSignalMapper>
#include <iremote.h>
#include "showcommanddialog.h"

typedef struct {
    GraphicButton *button;
    QString commandName;
} CommandCombination;

namespace Ui {
class IRemoteWindow;
}

class IRemoteWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit IRemoteWindow(QWidget *parent = 0);
    ~IRemoteWindow();
    
private slots:
    void buttonClicked(int id);
    void irCommandReceived(IrCommand irCommand);

    void serialPortConnected();
    void serialPortDisconnected();
    void networkConnected();
    void networkDisconnected();

    void on_addButton_clicked();

    void on_editableCheck_clicked();

    void on_imageButton_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_tableWidget_cellClicked(int row, int column);

    void on_removeButton_clicked();

    void on_networkConnectButton_clicked();

    void on_serialConnectButton_clicked();

    void on_captureButton_clicked();

    void on_runButton_clicked();

    void on_settingsSubmitButton_clicked();

    void on_commandList_doubleClicked(const QModelIndex &index);

private:
    Ui::IRemoteWindow *ui;

    QGraphicsScene *scene;
    //QList<GraphicButton*> graphicButtonList;
    QList<CommandCombination> commandList;
    QString pictureFileName;

    IRemote *iremote;
    QMap<QString,IrCommand>    irCommandMap;

    QSignalMapper *signalMapper;

    GraphicButton *createButton(QString name, QRectF rect, QPointF pos, int id);
    void addCommand(QString commandName, GraphicButton *button);
    void removeCommand(int id);
    void loadPicture(QString fileName);
    void addTableRow(QString buttonName, QString commandName);

    void addIrCommand(const QString name, IrCommand command);
    void removeIrCommand(const QString name);
    IrCommand getIrCommand(const QString name);

    void loadSettings();
    void saveSettings();
};

#endif // IREMOTEWINDOW_H
