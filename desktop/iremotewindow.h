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
    QString name;
    QRectF rect;
    QPointF pos;
} Button;

typedef struct {
    GraphicButton *button;
    Button buttonData;
    QString commandName;
} CommandCombination;

typedef struct {
    QList<CommandCombination> commandList;
    QString profileName;
    QString pictureFileName;
} Profile;

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

    void commandComboClicked(int row);

    void queueStarted();
    void queueFinished();

    void on_addButton_clicked();

    void on_editableCheck_clicked();

    void on_imageButton_clicked();

    void on_tableWidget_cellChanged(int row, int column);

    void on_tableWidget_cellClicked(int row, int column);

    void on_removeButton_clicked();

    void on_networkConnectButton_clicked();

#ifdef SERIALPORT
    void on_serialConnectButton_clicked();
#endif

    void on_captureButton_clicked();

    void on_runButton_clicked();

    void on_settingsSubmitButton_clicked();

    void on_commandList_doubleClicked(const QModelIndex &index);

    void on_ipMethodCombo_currentIndexChanged(int index);

    void on_profileAddButton_clicked();

    void on_profileRemoveButton_clicked();

    void on_profileCombo_currentIndexChanged(int index);

    void on_wlanSecurityCombo_currentIndexChanged(int index);

    void on_wlanAdhocButton_clicked();

    void on_removeCommandButton_clicked();

    void on_openFlashfileButton_clicked();

    void on_flashButton_clicked();

private:
    Ui::IRemoteWindow *ui;

    QGraphicsScene *scene;
    QGraphicsPixmapItem *scenePixmap;
    IRemote *iremote;

    QList<Profile> profiles;

    Profile *currentProfile;

    QMap<QString,IrCommand>    irCommandMap;

    QSignalMapper *signalMapper;
    QSignalMapper *commandComboMapper;

    QString settingsDir;

    GraphicButton *createButton(QString name, QRectF rect, QPointF pos, int id);
    void addCommand(QString commandName);
    void removeCommand(int id);
    void loadPicture(QString fileName);
    void unloadPicture();
    void addTableRow(QString buttonName, QString commandName);

    void addIrCommand(const QString name, IrCommand command);
    void removeIrCommand(const QString name);
    IrCommand getIrCommand(const QString name);

    void loadSettings();
    void saveSettings();

    void loadProfile();
    void unloadProfile();

    void addProfile(QString name);
    void removeProfile(int id);
    void refreshProfiles();
};

#endif // IREMOTEWINDOW_H
