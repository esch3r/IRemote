#include "iremotewindow.h"
#include "ui_iremotewindow.h"
#include <QDebug>
#include <QInputDialog>

IRemoteWindow::IRemoteWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IRemoteWindow)
{
    ui->setupUi(this);

    currentProfile = NULL;
    scenePixmap = NULL;
    signalMapper = NULL;

    int width = 300;
    int height = 600;

    scene = new QGraphicsScene(0,0,width,height);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(0,0,width,height);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setBackgroundBrush(QColor(230, 200, 167));

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

    QSettings tmpConfig(QSettings::IniFormat, QSettings::UserScope, "iremote", "settings");
    settingsDir = QFileInfo(tmpConfig.fileName()).absolutePath() + "/";

    loadSettings();
}

IRemoteWindow::~IRemoteWindow()
{
    unloadProfile();
    saveSettings();

    delete ui;
}

void IRemoteWindow::loadSettings()
{
    QSettings settings(settingsDir + "settings.ini", QSettings::IniFormat);

    int profileSize = settings.beginReadArray("profile");
    for (int num = 0; num < profileSize; num++)
    {
        settings.setArrayIndex(num);
        profiles.append(Profile());
        profiles.last().profileName = settings.value("profileName").toString();
        profiles.last().pictureFileName = settings.value("pictureFileName").toString();
        currentProfile = &(profiles.last());

        int size = settings.beginReadArray("command");
        for (int i = 0; i < size; i++)
        {
            settings.setArrayIndex(i);
            CommandCombination commandCombination;
            commandCombination.commandName = settings.value("commandName").toString();
            commandCombination.buttonData.name = settings.value("buttonName").toString();
            commandCombination.buttonData.rect = settings.value("buttonRect").toRectF();
            commandCombination.buttonData.pos = settings.value("buttonPos").toPointF();
            profiles.last().commandList.append(commandCombination);
        }
        settings.endArray();
    }
    settings.endArray();

    int size = settings.beginReadArray("irCommand");
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
    settings.endArray();

    currentProfile = NULL;
    if (profiles.size() > 0)
    {
        refreshProfiles();
    }

    // GUI settings
    settings.beginGroup("serial");
        ui->serialDeviceCombo->setCurrentIndex(settings.value("device", 0).toInt());
    settings.endGroup();

    settings.beginGroup("network");
        ui->networkAddressEdit->setText(settings.value("address", "192.168.1.5").toString());
        ui->networkPortSpin->setValue(settings.value("port", 2000).toInt());
    settings.endGroup();

    settings.beginGroup("wlan");
        ui->wlanHostnameEdit->setText(settings.value("hostname", "IRemoteBox").toString());
        ui->wlanSsidEdit->setText(settings.value("ssid", "").toString());
        ui->wlanSecurityCombo->setCurrentIndex(settings.value("auth", 0).toInt());
        ui->wlanPassphraseEdit->setText(settings.value("passphrase", "").toString());
    settings.endGroup();

    settings.beginGroup("ip");
        ui->ipAddressEdit->setText(settings.value("address", "169.254.1.1").toString());
        ui->ipMethodCombo->setCurrentIndex(settings.value("method", 0).toInt());
        ui->subnetMaskEdit->setText(settings.value("subnetMask", "255.255.0.0").toString());
        ui->gatewayEdit->setText(settings.value("gateway", "").toString());
    settings.endGroup();

    settings.beginGroup("ir");
        ui->irRepeatSpin->setValue(settings.value("repeat", 5).toInt());
        ui->irTimeoutSpin->setValue(settings.value("timeout", 50).toInt());
    settings.endGroup();
}

void IRemoteWindow::saveSettings()
{
    QSettings settings(settingsDir + "settings.ini", QSettings::IniFormat);

    settings.beginWriteArray("profile");
    for (int num = 0; num < profiles.size(); num++)
    {
        settings.setArrayIndex(num);
        settings.setValue("profileName", profiles.at(num).profileName);
        settings.setValue("pictureFileName", profiles.at(num).pictureFileName);

        settings.beginWriteArray("command");
        for (int i = 0; i < profiles.at(num).commandList.size(); i++)
        {
            settings.setArrayIndex(i);
            settings.setValue("commandName", profiles.at(num).commandList.at(i).commandName);
            settings.setValue("buttonName", profiles.at(num).commandList.at(i).buttonData.name);
            settings.setValue("buttonRect", profiles.at(num).commandList.at(i).buttonData.rect);
            settings.setValue("buttonPos", profiles.at(num).commandList.at(i).buttonData.pos);
        }
        settings.endArray();
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

     // GUI settings
     settings.beginGroup("serial");
        settings.setValue("device", ui->serialDeviceCombo->currentIndex());
     settings.endGroup();

     settings.beginGroup("network");
         settings.setValue("address", ui->networkAddressEdit->text());
         settings.setValue("port", ui->networkPortSpin->value());
     settings.endGroup();

     settings.beginGroup("wlan");
         settings.setValue("hostname", ui->wlanHostnameEdit->text());
         settings.setValue("ssid", ui->wlanSsidEdit->text());
         settings.setValue("auth", ui->wlanSecurityCombo->currentIndex());
         settings.setValue("passphrase", ui->wlanPassphraseEdit->text());
     settings.endGroup();

     settings.beginGroup("ip");
         settings.setValue("address", ui->ipAddressEdit->text());
         settings.setValue("method", ui->ipMethodCombo->currentIndex());
         settings.setValue("subnetMask", ui->subnetMaskEdit->text());
         settings.setValue("gateway", ui->gatewayEdit->text());
     settings.endGroup();

     settings.beginGroup("ir");
         settings.setValue("repeat", ui->irRepeatSpin->value());
         settings.setValue("timeout", ui->irTimeoutSpin->value());
     settings.endGroup();

    settings.sync();
}

void IRemoteWindow::buttonClicked(int id)
{
    QString commandName = currentProfile->commandList.at(id).commandName;

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

void IRemoteWindow::loadProfile()
{
    if (signalMapper != NULL)
    {
        signalMapper->deleteLater();
    }
        signalMapper = new QSignalMapper(this);
        connect(signalMapper, SIGNAL(mapped(int)),
                this, SLOT(buttonClicked(int)));

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    for (int i = 0; i < currentProfile->commandList.size(); i++)
    {
        GraphicButton *button = createButton(currentProfile->commandList.at(i).buttonData.name,
                                             currentProfile->commandList.at(i).buttonData.rect,
                                             currentProfile->commandList.at(i).buttonData.pos,
                                             i);
        currentProfile->commandList[i].button = button;
        addTableRow(button->buttonName(), currentProfile->commandList.at(i).commandName);
    }

    if (!currentProfile->pictureFileName.isEmpty())
        loadPicture(currentProfile->pictureFileName);
}

void IRemoteWindow::unloadProfile()
{
    for (int i = 0; i < currentProfile->commandList.size(); i++)
    {
        GraphicButton *button = currentProfile->commandList.at(i).button;
        currentProfile->commandList[i].buttonData.name = button->buttonName();
        currentProfile->commandList[i].buttonData.rect = button->rect();
        currentProfile->commandList[i].buttonData.pos = button->pos();
        scene->removeItem(currentProfile->commandList.at(i).button);
        currentProfile->commandList[i].button = NULL;
    }

    unloadPicture();
}

void IRemoteWindow::addProfile(QString name)
{
    Profile profile;
    profile.profileName = name;

    profiles.append(profile);
}

void IRemoteWindow::removeProfile(int id)
{
    if (profiles.size() > id)
    {
        unloadProfile();
        currentProfile = NULL;
        profiles.removeAt(id);
    }
}

void IRemoteWindow::refreshProfiles()
{
    int previousIndex = ui->profileCombo->currentIndex();

    ui->profileCombo->clear();
    for (int i = 0; i < profiles.size(); i++)
    {
        ui->profileCombo->addItem(profiles.at(i).profileName);
    }

    if ((previousIndex > -1) && (profiles.size() > previousIndex))
    {
        ui->profileCombo->setCurrentIndex(previousIndex);
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

void IRemoteWindow::addCommand(QString commandName)
{
    CommandCombination command;
    command.buttonData.name = "";
    command.buttonData.rect = QRect(0,0,50,50);
    command.buttonData.pos = QPointF(10,10);
    command.commandName = commandName;

    unloadProfile();
    currentProfile->commandList.append(command);
    loadProfile();
}

void IRemoteWindow::removeCommand(int id)
{
    ui->tableWidget->removeRow(id);
    scene->removeItem(currentProfile->commandList.at(id).button);
    currentProfile->commandList.at(id).button->deleteLater();
    currentProfile->commandList.removeAt(id);
}

void IRemoteWindow::loadPicture(QString fileName)
{
    QPixmap pixmap;
    pixmap.load(fileName);
    pixmap = pixmap.scaled(scene->width(),
                           scene->height(),
                           Qt::KeepAspectRatio,
                           Qt::SmoothTransformation);

    if (scenePixmap == NULL)
        scenePixmap = scene->addPixmap(pixmap);
    else
        scenePixmap->setPixmap(pixmap);

    scenePixmap->setZValue(-1);
}

void IRemoteWindow::unloadPicture()
{
    if (scenePixmap != NULL)
        scenePixmap->setPixmap(QPixmap());
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
        ui->commandList->takeItem(ui->commandList->row(item));
    }
}

IrCommand IRemoteWindow::getIrCommand(const QString name)
{
    return irCommandMap.value(name);
}

void IRemoteWindow::on_tableWidget_cellChanged(int row, int column)
{
    QString cellContent = ui->tableWidget->item(row,column)->text();
    if ((column == 0) && (currentProfile->commandList.size() > row))
        currentProfile->commandList.at(row).button->setButtonName(cellContent);
    else if ((column == 1) && (currentProfile->commandList.size() > row))
        currentProfile->commandList[row].commandName = cellContent;
}

void IRemoteWindow::on_tableWidget_cellClicked(int row, int column)
{
    Q_UNUSED(column);
    for (int i = 0; i < currentProfile->commandList.size(); i++)
    {
        currentProfile->commandList.at(i).button->setFocused((row == i));
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
    if (currentProfile == NULL)
        return;

    if (!ui->editableCheck->isChecked())
        ui->editableCheck->click();

    int id = currentProfile->commandList.size();
    addCommand(QString("%1").arg(id));
}

void IRemoteWindow::on_editableCheck_clicked()
{
    if (currentProfile == NULL)
        return;

    foreach(CommandCombination command, currentProfile->commandList)
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
        currentProfile->pictureFileName = fileName;
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
    if (ui->commandList->currentRow() < 0)
        return;

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

    IRemote::IpDhcpMethod dhcpMethod = IRemote::DhcpOnMethod;

    switch (ui->ipMethodCombo->currentIndex())
    {
    case 0: dhcpMethod = IRemote::DhcpOnMethod;
        break;
    case 1: dhcpMethod = IRemote::DhcpOffMethod;
        break;
    case 2: dhcpMethod = IRemote::AutoIpMethod;
        break;
    }
    iremote->setWlanDhcpMethod(dhcpMethod);

    if (dhcpMethod == IRemote::DhcpOffMethod)
    {
        iremote->setWlanIpAddress(ui->ipAddressEdit->text());
        iremote->setWlanSubnetMask(ui->subnetMaskEdit->text());
        iremote->setWlanGateway(ui->gatewayEdit->text());
    }

    //iremote->setIrRepeat(ui->irRepeatSpin->value());
    //iremote->setIrTimeout(ui->irTimeoutSpin->value());

    qDebug() << "test";
    iremote->saveWlanConfig();
}

void IRemoteWindow::on_commandList_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);

    IrCommand command = getIrCommand(ui->commandList->currentItem()->text());
    ShowCommandDialog *dialog = new ShowCommandDialog(this);
    dialog->loadIrCommand(command);
    dialog->show();
}

void IRemoteWindow::on_ipMethodCombo_currentIndexChanged(int index)
{
    bool enabled = !((index == 0) || (index == 2));    //DHCP or Auto-Ip

    ui->ipAddressEdit->setEnabled(enabled);
    ui->subnetMaskEdit->setEnabled(enabled);
    ui->gatewayEdit->setEnabled(enabled);
}

void IRemoteWindow::on_profileAddButton_clicked()
{
    QString name = QInputDialog::getText(this, tr("New Profile"), tr("Insert the name of the new profile"));
    if (!name.isEmpty())
    {
        addProfile(name);
        refreshProfiles();
    }
}

void IRemoteWindow::on_profileRemoveButton_clicked()
{
    if (ui->profileCombo->currentIndex() > -1)
    {
        removeProfile(ui->profileCombo->currentIndex());
        refreshProfiles();
    }
}

void IRemoteWindow::on_profileCombo_currentIndexChanged(int index)
{
    if (index == -1)
        return;

    if (currentProfile != NULL)
        unloadProfile();

    currentProfile = &(profiles[index]);
    loadProfile();
}

void IRemoteWindow::on_wlanSecurityCombo_currentIndexChanged(int index)
{
    if (index == -1)
        return;

    bool enabled = (!((index == 0) || (index == 5)));   //Open mode or Adhoc

    ui->wlanPassphraseEdit->setEnabled(enabled);
}

void IRemoteWindow::on_wlanAdhocButton_clicked()
{
    iremote->startWlanAdhoc();
    ui->networkAddressEdit->setText("169.254.1.1");
}

void IRemoteWindow::on_removeCommandButton_clicked()
{
    removeIrCommand(ui->commandList->currentItem()->text());
}

void IRemoteWindow::on_openFlashfileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open flash file"),
                                                    QDir::homePath(),
                                                    tr("Bin file (*.bin) (*.bin)"));

    if (!fileName.isNull())
    {
        ui->flashfileNameEdit->setText(fileName);
    }
}

void IRemoteWindow::on_flashButton_clicked()
{
    iremote->flashFirmware(ui->flashfileNameEdit->text());
}
