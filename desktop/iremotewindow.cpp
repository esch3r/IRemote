#include "iremotewindow.h"
#include "ui_iremotewindow.h"
#include <QDebug>
#include <QInputDialog>

IRemoteWindow::IRemoteWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IRemoteWindow)
{
    ui->setupUi(this);

    int width = 300;
    int height = 600;

    scene = new QGraphicsScene(0,0,width,height);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(0,0,width,height);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setBackgroundBrush(QColor(230, 200, 167));

    signalMapper = new QSignalMapper(this);
    connect(signalMapper, SIGNAL(mapped(int)),
            this, SLOT(buttonClicked(int)));

    loadSettings();

    iremote = new IRemote();
    connect(iremote, SIGNAL(irCommandReceived(IrCommand)),
            this, SLOT(irCommandReceived(IrCommand)));
    connect(iremote, SIGNAL(networkConnected()),
            this, SLOT(networkConnected()));
    connect(iremote, SIGNAL(networkDisconnected()),
            this, SLOT(networkDisconnected()));
    connect(iremote, SIGNAL(serialPortConnected()),
            this, SLOT(serialPortConnected()));
    connect(iremote, SIGNAL(serialPortDisconnected()),
            this, SLOT(serialPortDisconnected()));
}

IRemoteWindow::~IRemoteWindow()
{
    saveSettings();

    delete ui;
}

void IRemoteWindow::loadSettings()
{
    QSettings settings;
    int size = settings.beginReadArray("command");
    for (int i = 0; i < size; i++)
    {
        settings.setArrayIndex(i);
        GraphicButton *button = createButton(settings.value("buttonName").toString(),
                                      settings.value("buttonRect").toRectF(),
                                      settings.value("buttonPos").toPointF(),i);
        addCommand(settings.value("commandName").toString(), button);
    }
    settings.endArray();

    size = settings.beginReadArray("irCommand");
    for (int i = 0; i < size; i++)
    {
        QByteArray bytes;
        QString name;
        IrCommand command;

        settings.setArrayIndex(i);
        name = settings.value("name").toString();
        bytes = settings.value("command").toByteArray();
        memcpy(&command, bytes.data(), sizeof(IrCommand));
        addIrCommand(name, command);
    }

    pictureFileName = settings.value("pictureFileName","").toString();
    if (!pictureFileName.isEmpty())
        loadPicture(pictureFileName);
}

void IRemoteWindow::saveSettings()
{
    QSettings settings;
    settings.beginWriteArray("command");
    for (int i = 0; i < commandList.size(); i++)
    {
        settings.setArrayIndex(i);
        settings.setValue("commandName", commandList.at(i).commandName);
        settings.setValue("buttonName", commandList.at(i).button->buttonName());
        settings.setValue("buttonRect", commandList.at(i).button->rect());
        settings.setValue("buttonPos", commandList.at(i).button->pos());
    }
    settings.endArray();

    int i = 0;
    settings.beginWriteArray("irCommand");
        QMapIterator<QString, IrCommand> iterator(irCommandMap);
         while (iterator.hasNext()) {
             QByteArray bytes;
             iterator.next();
             bytes.append((char*)(&(iterator.value())), sizeof(IrCommand));

             settings.setArrayIndex(i);
             settings.setValue("name", iterator.key());
             settings.setValue("command", bytes);

             i++;
         }
     settings.endArray();

    settings.setValue("pictureFileName",pictureFileName);
}

void IRemoteWindow::buttonClicked(int id)
{
    QString commandName = commandList.at(id).commandName;

    if (irCommandMap.find(commandName) != irCommandMap.end())
    {
        iremote->actionRun(irCommandMap.value(commandName));
    }
}

void IRemoteWindow::irCommandReceived(IrCommand irCommand)
{
    QString name = QInputDialog::getText(this, tr("New Command"), tr("Insert the name of the new command"));
    if (!name.isEmpty())
    {
        addIrCommand(name, irCommand);
    }
}

GraphicButton *IRemoteWindow::createButton(QString name, QRectF rect, QPointF pos, int id)
{
    GraphicButton *item = new GraphicButton;
    item->setPos(pos);
    item->setRect(rect);
    item->setButtonName(name);
    item->setEditable(ui->editableCheck->isChecked());
    scene->addItem(item);

    signalMapper->setMapping(item,id);
    connect(item, SIGNAL(clicked()),
            signalMapper, SLOT(map()));

    return item;
}

void IRemoteWindow::addCommand(QString commandName, GraphicButton *button)
{
    CommandCombination command;
    command.button = button;
    command.commandName = commandName;
    commandList.append(command);

    addTableRow(button->buttonName(), command.commandName);
}

void IRemoteWindow::removeCommand(int id)
{
    ui->tableWidget->removeRow(id);
    scene->removeItem(commandList.at(id).button);
    commandList.at(id).button->deleteLater();
    commandList.removeAt(id);
}

void IRemoteWindow::loadPicture(QString fileName)
{
    QPixmap pixmap;
    pixmap.load(fileName);
    pixmap = pixmap.scaled(scene->width(),
                           scene->height(),
                           Qt::KeepAspectRatio,
                           Qt::SmoothTransformation);
    QGraphicsPixmapItem *pixmapItem = scene->addPixmap(pixmap);
    pixmapItem->setZValue(-1);
}

void IRemoteWindow::addTableRow(QString buttonName, QString commandName)
{
    QTableWidgetItem *newItem;
    int row;
    int column;

    row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row + 1);

    column = 0;
    newItem = new QTableWidgetItem(buttonName);
    ui->tableWidget->setItem(row, column, newItem);

    column = 1;
    newItem = new QTableWidgetItem(commandName);
    ui->tableWidget->setItem(row, column, newItem);

}

void IRemoteWindow::addIrCommand(const QString name, IrCommand command)
{
    irCommandMap.insert(name, command);
    if (ui->commandList->findItems(name, Qt::MatchCaseSensitive).isEmpty())
    {
        ui->commandList->addItem(name);
    }
}

void IRemoteWindow::removeIrCommand(const QString name)
{
    irCommandMap.remove(name);
    foreach (QListWidgetItem* item, ui->commandList->findItems(name, Qt::MatchCaseSensitive))
    {
        ui->commandList->removeItemWidget(item);
    }
}

IrCommand IRemoteWindow::getIrCommand(const QString name)
{
    return irCommandMap.value(name);
}

void IRemoteWindow::on_tableWidget_cellChanged(int row, int column)
{
    QString cellContent = ui->tableWidget->item(row,column)->text();
    if (column == 0)
        commandList.at(row).button->setButtonName(cellContent);
    else if (column == 1)
        commandList[row].commandName = cellContent;
}

void IRemoteWindow::on_tableWidget_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    for (int i = 0; i < commandList.size(); i++)
    {
        commandList.at(i).button->setFocused((row == i));
    }
}

void IRemoteWindow::on_removeButton_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0)
        return;

    removeCommand(row);
}

void IRemoteWindow::on_addButton_clicked()
{
    if (!ui->editableCheck->isChecked())
        ui->editableCheck->click();

    int id = commandList.size();
    GraphicButton *button = createButton(QString("Button %1").arg(commandList.size()),
                                       QRect(0,0,40,20),QPointF(0,0),
                                       id);
    addCommand(QString("%1").arg(id), button);
}

void IRemoteWindow::on_editableCheck_clicked()
{
    foreach(CommandCombination command, commandList)
    {
        command.button->setEditable(ui->editableCheck->isChecked());
    }
}

void IRemoteWindow::on_imageButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
         tr("Open Image"), QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isNull())
    {
        loadPicture(fileName);
        pictureFileName = fileName;
    }
}

void IRemoteWindow::on_networkConnectButton_clicked()
{
    if (!iremote->isNetworkConnected())
        iremote->connectNetwork(ui->networkAddressEdit->text(), ui->networkPortSpin->value());
    else
        iremote->closeNetwork();
}

void IRemoteWindow::on_serialConnectButton_clicked()
{
    if (!iremote->isSerialPortConnected())
        iremote->connectSerialPort(ui->serialDeviceCombo->currentText());
    else
        iremote->closeSerialPort();
}

void IRemoteWindow::serialPortConnected()
{
    ui->serialStatusFrame->setStyleSheet("background-color: green;");
    ui->serialConnectButton->setText(tr("Disconnect"));
}

void IRemoteWindow::serialPortDisconnected()
{
    ui->serialStatusFrame->setStyleSheet("background-color: red;");
    ui->serialConnectButton->setText(tr("Connect"));
}

void IRemoteWindow::networkConnected()
{
    ui->networkStatusFrame->setStyleSheet("background-color: green;");
    ui->networkConnectButton->setText(tr("Disconnect"));
}

void IRemoteWindow::networkDisconnected()
{
    ui->networkStatusFrame->setStyleSheet("background-color: red;");
    ui->networkConnectButton->setText(tr("Connect"));
}

void IRemoteWindow::on_captureButton_clicked()
{
    iremote->actionCapture();
}

void IRemoteWindow::on_runButton_clicked()
{
    IrCommand command = getIrCommand(ui->commandList->currentItem()->text());
    iremote->actionRun(command);
}

void IRemoteWindow::on_settingsSubmitButton_clicked()
{
    IRemote::WlanAuthType authType = IRemote::OpenAuthType;

    switch (ui->wlanSecurityCombo->currentIndex())
    {
    case 0: authType = IRemote::OpenAuthType;
        break;
    case 1: authType = IRemote::WEP128AuthType;
        break;
    case 2: authType = IRemote::WPA1AuthType;
        break;
    case 3: authType = IRemote::MixedWPA1AndWPA2PSKAuthType;
        break;
    case 4: authType = IRemote::WPA2PSKAuthType;
        break;
    case 5: authType = IRemote::AdhocAuthType;
        break;
    case 6: authType = IRemote::WPE64AuthType;
        break;
    }
    iremote->startWlanConfig();
    iremote->setWlanSsid(ui->wlanSsidEdit->text());
    iremote->setWlanAuth(authType);
    iremote->setWlanHostname(ui->wlanHostnameEdit->text());
    if ((authType == IRemote::WEP128AuthType) ||
            (authType == IRemote::WPE64AuthType))
    {
        iremote->setWlanKey(ui->wlanPassphraseEdit->text());
    }
    else if ((authType == IRemote::WPA1AuthType) ||
             (authType == IRemote::MixedWPA1AndWPA2PSKAuthType) ||
             (authType == IRemote::WPA2PSKAuthType))
    {
        iremote->setWlanPhrase(ui->wlanPassphraseEdit->text());
    }
    iremote->saveWlanConfig();
}

void IRemoteWindow::on_commandList_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);

    IrCommand command = getIrCommand(ui->commandList->currentItem()->text());
    ShowCommandDialog dialog;
    dialog.loadIrCommand(command);
    dialog.exec();
}
