#include "iremotewindow.h"
#include "ui_iremotewindow.h"
#include <QDebug>
#include <QInputDialog>

IRemoteWindow::IRemoteWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::IRemoteWindow)
{
    ui->setupUi(this);

    initializeRemoteCommandTable();
    initializeCommandTable();

    currentProfile = NULL;
    scenePixmap = NULL;
    signalMapper = NULL;
    commandComboMapper = NULL;

    int width = 300;
    int height = 600;

    scene = new QGraphicsScene(0,0,width,height);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(0,0,width,height);

    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
    ui->graphicsView->setBackgroundBrush(QColor(230, 200, 167));

    iremote = new IRemote();
    connect(iremote, SIGNAL(irCommandReceived(RemoteCommand)),
            this, SLOT(irCommandReceived(RemoteCommand)));
    connect(iremote, SIGNAL(networkConnected()),
            this, SLOT(networkConnected()));
    connect(iremote, SIGNAL(networkDisconnected()),
            this, SLOT(networkDisconnected()));
#ifdef SERIALPORT
    connect(iremote, SIGNAL(serialPortConnected()),
            this, SLOT(serialPortConnected()));
    connect(iremote, SIGNAL(serialPortDisconnected()),
            this, SLOT(serialPortDisconnected()));
#endif

    connect(iremote, SIGNAL(queueFinished()),
            this, SLOT(queueFinished()));
    connect(iremote, SIGNAL(queueStarted()),
            this, SLOT(queueStarted()));

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

void IRemoteWindow::applicationStarted(QString message)
{
    qDebug() << message;
    RemoteCommand command = getIrCommand(message);
    iremote->actionRun(command);
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
        RemoteCommand command;

        settings.setArrayIndex(i);
        name = settings.value("name").toString();
        bytes = settings.value("command").toByteArray();
        memcpy(&command, bytes.data(), sizeof(RemoteCommand));
        addRemoteCommand(name, command);
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
        ui->irCountSpin->setValue(settings.value("repeat", 5).toInt());
        ui->irReceiveTimeoutSpin->setValue(settings.value("receiveTimeout", 30).toInt());
        ui->irSendTimeoutSpin->setValue(settings.value("sendTimeout", 100).toInt());
    settings.endGroup();

    ui->profileCombo->setCurrentIndex(settings.value("currentProfile", 0).toInt());
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
        QMapIterator<QString, RemoteCommand> iterator(irCommandMap);
         while (iterator.hasNext()) {
             QByteArray bytes;
             iterator.next();
             bytes.append((char*)(&(iterator.value())), sizeof(RemoteCommand));

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
         settings.setValue("repeat", ui->irCountSpin->value());
         settings.setValue("receiveTimeout", ui->irReceiveTimeoutSpin->value());
         settings.setValue("sendTimeout", ui->irSendTimeoutSpin->value());
     settings.endGroup();

     settings.setValue("currentProfile", ui->profileCombo->currentIndex());

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

void IRemoteWindow::irCommandReceived(RemoteCommand irCommand)
{
    QString name = QInputDialog::getText(this, tr("New Command"), tr("Insert the name of the new command"));
    if (!name.isEmpty())
    {
        addRemoteCommand(name, irCommand);
        refreshProfiles();
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

        commandComboMapper = new QSignalMapper(this);
        connect(commandComboMapper, SIGNAL(mapped(int)),
                this, SLOT(commandComboClicked(int)));

    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    for (int i = 0; i < currentProfile->commandList.size(); i++)
    {
        GraphicButton *button = createButton(currentProfile->commandList.at(i).buttonData.name,
                                             currentProfile->commandList.at(i).buttonData.rect,
                                             currentProfile->commandList.at(i).buttonData.pos,
                                             i);
        currentProfile->commandList[i].button = button;
        addCommandTableRow(button->buttonName(), currentProfile->commandList.at(i).commandName);
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

void IRemoteWindow::addCommandTableRow(QString buttonName, QString commandName)
{
    QTableWidgetItem *newItem;
    QComboBox *box;
    int row;
    int column;

    row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row + 1);

    // Column 0 Name
    column = 0;
    newItem = new QTableWidgetItem(buttonName);
    ui->tableWidget->setItem(row, column, newItem);

    // Column 1 combo box
    column = 1;
    box = new QComboBox(this);
    for (int i = 0; i < ui->remoteCommandTable->rowCount(); i++)
    {
        box->addItem(ui->remoteCommandTable->item(i, 0)->text());
    }
    box->setCurrentIndex(box->findText(commandName));
    ui->tableWidget->setCellWidget(row, column, box);

    commandComboMapper->setMapping(box, row);
    connect(box, SIGNAL(currentIndexChanged(int)),
            commandComboMapper, SLOT(map()));

    ui->tableWidget->resizeColumnToContents(1);

}

void IRemoteWindow::initializeCommandTable()
{
    QStringList labels;

    labels << tr("Command") << tr("Button");
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setHorizontalHeaderLabels(labels);
    ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
}

void IRemoteWindow::addRemoteCommand(const QString name, RemoteCommand command)
{
    irCommandMap.insert(name, command);

    if (ui->remoteCommandTable->findItems(name, Qt::MatchCaseSensitive).isEmpty())
    {
        QTableWidgetItem *newItem;
        QString mediumName;
        int row;
        int column;

        row = ui->remoteCommandTable->rowCount();
        ui->remoteCommandTable->setRowCount(row + 1);

        // Column 0 Name
        column = 0;
        newItem = new QTableWidgetItem(name);
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->remoteCommandTable->setItem(row, column, newItem);

        // Column 1 Medium
        if (command.medium == RemoteMedium_Ir) {
            mediumName = tr("IR");
        }
        else if (command.medium == RemoteMedium_433MHz) {
            mediumName = tr("433MHz");
        }
        else if (command.medium == RemoteMedium_868MHz) {
            mediumName = tr("868MHz");
        }
        column = 1;
        newItem = new QTableWidgetItem(mediumName);
        newItem->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        ui->remoteCommandTable->setItem(row, column, newItem);

    }
}

void IRemoteWindow::removeRemoteCommand(const QString name)
{
    irCommandMap.remove(name);

    for (int i = 0; i < ui->remoteCommandTable->rowCount(); i++)
    {
        if (ui->remoteCommandTable->item(i, 0)->text() == name)
        {
            ui->remoteCommandTable->removeRow(i);
            return;
        }
    }
}

void IRemoteWindow::copyRemoteCommand(const QString name)
{
    RemoteCommand command = irCommandMap.value(name);

    addRemoteCommand(name + "_copy", command);
}

void IRemoteWindow::renameRemoteCommand(const QString name, const QString newName)
{
    RemoteCommand command = irCommandMap.value(name);

    removeRemoteCommand(name);
    addRemoteCommand(newName, command);
}

RemoteCommand IRemoteWindow::getIrCommand(const QString name)
{
    return irCommandMap.value(name);
}

void IRemoteWindow::initializeRemoteCommandTable()
{
    QStringList labels;

    labels << tr("Name") << tr("Medium");
    ui->remoteCommandTable->setColumnCount(2);
    ui->remoteCommandTable->setHorizontalHeaderLabels(labels);
    ui->remoteCommandTable->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    ui->remoteCommandTable->horizontalHeader()->setResizeMode(1, QHeaderView::Stretch);
}

void IRemoteWindow::on_tableWidget_cellChanged(int row, int column)
{
    QString cellContent = ui->tableWidget->item(row,column)->text();

    if ((column == 0) && (currentProfile->commandList.size() > row))
    {
        currentProfile->commandList.at(row).button->setButtonName(cellContent);
    }
}

void IRemoteWindow::commandComboClicked(int row)
{
    currentProfile->commandList[row].commandName =  ((QComboBox*)(ui->tableWidget->cellWidget(row,1)))->currentText();
}

void IRemoteWindow::queueStarted()
{
    ui->statusBar->showMessage(tr("Running command"));
}

void IRemoteWindow::queueFinished()
{
    ui->statusBar->showMessage(tr("Commands finished"));
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

#ifdef SERIALPORT
void IRemoteWindow::on_serialConnectButton_clicked()
{
    if (!iremote->isSerialPortConnected())
        iremote->connectSerialPort(ui->serialDeviceCombo->currentText());
    else
        iremote->closeSerialPort();
}
#endif

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
    iremote->actionCaptureIr();
}

void IRemoteWindow::on_capture433Button_clicked()
{
    iremote->actionCaptureRadio433MHz();
}

void IRemoteWindow::on_capture868Button_clicked()
{
    iremote->actionCaptureRadio868MHz();
}

void IRemoteWindow::on_runButton_clicked()
{
    int row = ui->remoteCommandTable->currentRow();
    if (row < 0)
        return;

    RemoteCommand command = getIrCommand(ui->remoteCommandTable->item(row, 0)->text());
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

void IRemoteWindow::on_wlanInfrastructureButton_clicked()
{
    iremote->startWlanInfrastructure();
}

void IRemoteWindow::on_removeCommandButton_clicked()
{
    int row = ui->remoteCommandTable->currentRow();
    if (row != -1)
    {
        removeRemoteCommand(ui->remoteCommandTable->item(row, 0)->text());
        refreshProfiles();
    }
}

void IRemoteWindow::on_copyCommandButton_clicked()
{
    int row = ui->remoteCommandTable->currentRow();
    if (row != -1)
    {
        copyRemoteCommand(ui->remoteCommandTable->item(row, 0)->text());
        refreshProfiles();
    }
}

void IRemoteWindow::on_renameCommandButton_clicked()
{
    int row = ui->remoteCommandTable->currentRow();
    if (row != -1)
    {
        QString oldName = ui->remoteCommandTable->item(row, 0)->text();

        QString name = QInputDialog::getText(this, tr("Rename Command"), tr("Insert the new name of the command"), QLineEdit::Normal, oldName);
        if (!name.isEmpty())
        {
            renameRemoteCommand(oldName, name);
            refreshProfiles();
        }
    }
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

void IRemoteWindow::on_setIrButton_clicked()
{
    iremote->setIrCount(ui->irCountSpin->value());
    iremote->setIrReceiveTimeout(ui->irReceiveTimeoutSpin->value());
    iremote->setIrSendTimeout(ui->irSendTimeoutSpin->value());
}

void IRemoteWindow::on_remoteCommandTable_doubleClicked(const QModelIndex &index)
{
    Q_UNUSED(index);

    int row = ui->remoteCommandTable->currentRow();
    QString commandName = ui->remoteCommandTable->item(row, 0)->text();

    RemoteCommand command = getIrCommand(commandName);
    ShowCommandDialog *dialog = new ShowCommandDialog(this);
    dialog->setRemoteCommand(command);
    dialog->exec();
    addRemoteCommand(commandName, dialog->remoteCommand());
}

void IRemoteWindow::on_hideCommandTableButton_clicked()
{
    if (ui->tableWidget->isVisible())
    {
        ui->tableWidget->setVisible(false);
        ui->profileWidget->setVisible(false);
        ui->buttonWidget->setVisible(false);
        ui->hideCommandTableButton->setIcon(QIcon::fromTheme("arrow-left"));
    }
    else
    {
        ui->tableWidget->setVisible(true);
        ui->profileWidget->setVisible(true);
        ui->buttonWidget->setVisible(true);
        ui->hideCommandTableButton->setIcon(QIcon::fromTheme("arrow-right"));
    }
}
