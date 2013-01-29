#include "iremote.h"

ActiveConnection activeConnections = 0;

IrCommand *currentCommand;
ApplicationState applicationState = ApplicationStateIdle;
ApplicationSettings applicationSettings;

int8 initializeHardware(void)
{
    crcInit();                    // init crc function for firmware flashing
    initializeTimeout(TIMER1);    // initialize Timer1 for general timeout functions
        
    initializeLed(1,29, TRUE);     // led 0 - onboard
    initializeLed(0,0, FALSE);     // led 1 - green
    initializeLed(0,1, FALSE);     // led 2 - yellow
    initializeLed(0,10, FALSE);    // led 3 - red
    clearAllLeds();
    
    initializeButtons(1000, 1E4, 1E5);  //100kHz timer, 10ms timeout
    initializeButton(0, 2, 4, ButtonTypeLowActive);
    initializeButton(1, 2, 5, ButtonTypeLowActive);
    initializeButton(2, 2, 10, ButtonTypeLowActive);
   
    //Program started notifier
    setLed(3);    
    blinkLed(2);
    setLed(1);
    delayMs(200);
    
    initializeIrControl();
    
    // initialize Network Connections
    initializeSerialConnection();
    if (initializeNetworkConnection() == -1)
    {
        printfData("ERR: Network initalization failed");
    }
     
    // Welcome message
    printfData("Welcome to IRemote!\r");    // Send a welcome message
    printfData("Id: %i, Version: %i, Serial: %i\r",readIdIap(),readVersionIap(),readSerialIap());
   
    clearLed(3);
    blinkLed2(0);   //onboard we came through the initialization
    
    initializeRfm12();
    
    return 0;
}

int8 initializeVariables(void )
{
    // init variables
    currentCommand = createIrCommand();
    
    //load settings....
    loadSettings(&applicationSettings, sizeof(ApplicationSettings));
    
    if (applicationSettings.firstStartIdentificator != 40)
    {
        applicationSettings.irReceiveTimeout = 25000;
        applicationSettings.irSendTimeout = 50000;
        applicationSettings.irRepeatCount = 2;
        strcpy(applicationSettings.wlanSsid, "");
        strcpy(applicationSettings.wlanPhrase, "");
        strcpy(applicationSettings.wlanKey, "");
        strcpy(applicationSettings.wlanHostname, "");
        applicationSettings.wlanAuth = 0;
        applicationSettings.wlanDhcp = 0;
        strcpy(applicationSettings.wlanIp, "192.168.1.2");
        strcpy(applicationSettings.wlanMask, "255.255.255.0");
        strcpy(applicationSettings.wlanHostname, "192.168.1.1");
        applicationSettings.networkMode = AdhocNetworkMode;
        
        applicationSettings.firstStartIdentificator = 40;   // remove the first start indicator
    
        startWlanAdhocMode();   // Start the adhoc mode
        saveSettings(&applicationSettings, sizeof(ApplicationSettings));
    }
    
    
    setIrReceiveTimeout(applicationSettings.irReceiveTimeout);
    setIrSendTimeout(applicationSettings.irSendTimeout);
    setIrRepeatCount(applicationSettings.irRepeatCount);
    
    return 0;
}

int8 initializeSerialConnection(void)
{
    if (initializeUart0(115200) == 0)
    {
        flushUart0();                           // Trash all unusefull characters
        activeConnections |= SerialConnection;
        
        setProcessFunctionUart0(&processCommand);
        setErrorFunctionUart0(&errorCommand);
    
        return 0;
    }
    
    return -1;
}

int8 initializeNetworkConnection(void)
{
    if (initializeWiFly(115200) == 0)
    {
        if (actionWiFlyEnterCommandMode(FALSE) == 0)      // Configure the wlan module
        {
            setWiFlySysPrintlvl(0);             // Turn off unessesary messages
            setWiFlyWlanJoin(2);                // Auto join
            setWiFlyDnsName("IRemoteBox");      // Sets the default hostname
            setWiFlyDnsBackup("IRemote");       // Sets the backup hostname
            actionWiFlyExitCommandMode();
            
            setProcessFunctionWiFly(&processCommand);
            setErrorFunctionWiFly(&errorWiFly);
            
            return 0;
        }
        else
            return -1;
    }
    
    return -1;
}

int8 startWlanInfrastructureMode(ApplicationSettings *settings)
{
    if (actionWiFlyEnterCommandMode(FALSE) == -1)
        return -1;
    if (setWiflyInfrastructureParams() == -1)
        return -1;
    if (setWiFlyWlanSsid(settings->wlanSsid) == -1)
        return -1;
    if (setWiFlyWlanPhrase(settings->wlanPhrase) == -1)
        return -1;
    if (setWiFlyWlanKey(settings->wlanKey) == -1)
        return -1;
    if (setWiFlyDnsName(settings->wlanHostname) == -1)
        return -1;
    if (setWiFlyWlanAuth(settings->wlanAuth) == -1)
        return -1;
    if (setWiFlyIpDhcp(settings->wlanDhcp) == -1)
        return -1;
    if (setWiFlyIpAddress(settings->wlanIp) == -1)
        return -1;
    if (setWiFlyIpNetmask(settings->wlanMask) == -1)
        return -1;
    if (setWiFlyIpGateway(settings->wlanGateway) == -1)
        return -1;
    if (fileIoWiFlySaveDefault() == -1)
        return -1;
    return actionWiFlyReboot();
}

int8 startWlanAdhocMode(void)
{
    char buffer[100];
    snprintf(buffer, 100, "IRemoteBox_%u", readSerialIap());
    return createWiFlyAdhocNetwork(buffer);
}

int8 printfData(char* format, ... )
{
    static char buffer[PRINTF_BUFFER_SIZE];
    
    va_list arg_ptr;
    uint16 i = 0;
    
    va_start(arg_ptr,format);
    vsnprintf(buffer, PRINTF_BUFFER_SIZE, format, arg_ptr);
    va_end(arg_ptr);
    
    if (activeConnections & NetworkConnection)
    {
        while (buffer[i] != 0)      // Loop through until reach string's zero terminator
        {
            if (putcharWiFly(buffer[i]) == -1)
                return -1;
            i++;
        }
        return 0;
    }
    else if (activeConnections & SerialConnection)
    {
        while (buffer[i] != 0)      // Loop through until reach string's zero terminator
        {
            if (putcharUart0(buffer[i]) == -1)
                return -1;
            i++;
        }
        return 0;
    }
    
    return -1;
}

int8 putcharData(char c)
{  
    if (activeConnections & NetworkConnection)
    {
        return putcharWiFly(c);
    }
    else if (activeConnections & SerialConnection)
    {
        return putcharUart0(c);
    }
    
    return -1;
}

int8 writeData(void *data, uint32 length)
{   
    if (activeConnections & NetworkConnection)
    {
        return writeDataWiFly(data, length);
    }
    else if (activeConnections & SerialConnection)
    {
        return writeDataUart0(data, length);
    }
    
    return -1;
}

void errorCommand()
{
    printfData("ERR: Command too long\r");
}

void errorWiFly()
{
    printfData("ERR: WiFly command too long\r");
}

void printUnknownCommand(void)
{
    printfData("CMD?\r");
    clearLed(2);
}

void printParameterMissing(void)
{
    printfData("Missing parameter.\r");
    clearLed(2);
}

void printAcknowledgement(void)
{
    printfData("ACK\r");
    clearLed(2);
}

void printError(char *message)
{
    printfData("ERR: %s\r", message);
}

void printAliveMessage(void)
{
    printfData("yes\r");
    clearLed(2);
}

bool compareBaseCommand(char *original, char *received)
{
    return (strcmp(original,received) == 0);
}

bool compareExtendedCommand(char *original, char *received)
{
    return (((strlen(received) == 1) && (strncmp(original,received,1) == 0)) ||
                (strcmp(original,received) == 0));
}

void processCommand(char *buffer)
{
    char *dataPointer;
    
    setLed(2);  // set the yellow led to indicate incoming data status
    
    dataPointer = strtok(buffer," ");
    
    if (compareBaseCommand("alive", dataPointer))
    {
        // We have a keep alive command
        printAliveMessage();
    }
    else if (compareBaseCommand("run", dataPointer))
    {
        // We have a run command
        dataPointer = strtok(NULL," ");
        if (dataPointer != NULL)
        {
            uint16 commandSize = strlen(dataPointer);
            uint16 i;
            for (i = 0; i < commandSize; i+=2)
            {
                ((char*)currentCommand)[i/2] = (char)hex2int(dataPointer+i,2);
            }
        }
        startState(ApplicationStateRunCommand);
    }
    else if (compareBaseCommand("capture", dataPointer))
    {
        // We have a capture command
        startState(ApplicationStateCaptureCommand);
    }
    else if (compareBaseCommand("stop", dataPointer))
    {
        // We have a capture command
        startState(ApplicationStateIdle);
    }
    else if (compareBaseCommand("flash", dataPointer))
    {
        // We have a flash command
        char buffer[100];
        uint16 receivedChecksum;
        uint16 calculatedChecksum;
        
        dataPointer = strtok(NULL," ");
        if (dataPointer != NULL)
        {
            uint16 commandSize = strlen(dataPointer);
            uint16 i;
            for (i = 0; i < commandSize; i+=2)
            {
                buffer[i/2] = (char)hex2int(dataPointer+i,2);
            }
            
            dataPointer = strtok(NULL," ");
            if (dataPointer != NULL)
            {
                receivedChecksum = (uint16)hex2int(dataPointer,4);;
                calculatedChecksum = crcFast(buffer, 100);
                if (receivedChecksum == calculatedChecksum)
                    printAcknowledgement();
                else
                    printfData("%u %u %u\r",commandSize,receivedChecksum,calculatedChecksum);
            }
        }
        
    }
    else if (compareBaseCommand("set", dataPointer))
    {
        // starting a set command
        dataPointer = strtok(NULL," ");
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wlan",dataPointer))
        {
            // set wlan
            dataPointer = strtok(NULL," ");
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("ssid",dataPointer))
            {
                // set ssid
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanSsid, dataPointer, 100);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("phrase",dataPointer))
            {
                // set ssid
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanPhrase, dataPointer, 100);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("key",dataPointer))
            {
                // set ssid
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanKey, dataPointer, 100);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("hostname",dataPointer))
            {
                // set hostname
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanHostname, dataPointer, 100);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("auth",dataPointer))
            {
                // set auth
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    applicationSettings.wlanAuth = atoi(dataPointer);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("dhcp",dataPointer))
            {
                // set auth
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    applicationSettings.wlanDhcp = atoi(dataPointer);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("ip",dataPointer))
            {
                // set auth
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanIp, dataPointer, 20);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("mask",dataPointer))
            {
                // set auth
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanMask, dataPointer, 20);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else if (compareExtendedCommand("gateway",dataPointer))
            {
                // set auth
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    strncpy(applicationSettings.wlanGateway, dataPointer, 20);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printUnknownCommand();
                    return;
                }
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else if (compareExtendedCommand("ir",dataPointer))
        {
            // set ir
            dataPointer = strtok(NULL," ");
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {

                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    applicationSettings.irReceiveTimeout = atoi(dataPointer)*1000;
                    setIrReceiveTimeout(applicationSettings.irReceiveTimeout);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else if (compareExtendedCommand("sendTimeout",dataPointer))
            {

                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    applicationSettings.irSendTimeout = atoi(dataPointer)*1000;
                    setIrSendTimeout(applicationSettings.irSendTimeout);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else if (compareExtendedCommand("count",dataPointer))
            {

                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    applicationSettings.irRepeatCount = atoi(dataPointer);
                    setIrRepeatCount(applicationSettings.irRepeatCount);
                    printAcknowledgement();
                    return;
                }
                else
                {
                    printParameterMissing();
                    return;
                }
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("get", dataPointer))            // starting a get command
    {
        dataPointer = strtok(NULL," ");
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wlan",dataPointer))
        {
            // get wlan
            dataPointer = strtok(NULL," ");
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("ssid",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanSsid);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("phrase",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanPhrase);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("key",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanKey);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("hostname",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanHostname);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("auth",dataPointer))
            {
                printfData("%u\r", applicationSettings.wlanAuth);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("dhcp",dataPointer))
            {
                printfData("%u\r", applicationSettings.wlanDhcp);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("ip",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanIp);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("mask",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanMask);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("gateway",dataPointer))
            {
                printfData("%s\r", applicationSettings.wlanGateway);
                printAcknowledgement();
                return;
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else if (compareExtendedCommand("ir",dataPointer))
        {
            // set ir
            dataPointer = strtok(NULL," ");
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.irReceiveTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("sendTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.irSendTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("count",dataPointer))
            {
                printfData("%u\r", applicationSettings.irRepeatCount);
                printAcknowledgement();
                return;
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("start", dataPointer))
    {
        // starting a start command
        dataPointer = strtok(NULL," ");
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wlan",dataPointer))
        {
            // set wlan
            dataPointer = strtok(NULL," ");
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("adhoc",dataPointer))
            {
                if (startWlanAdhocMode() == 0)
                    printAcknowledgement();
                else
                    printError("entering adhoc mode failed");
                    
                return;
            }
            else if (compareExtendedCommand("infrastructure",dataPointer))
            {
                if (startWlanInfrastructureMode(&applicationSettings) == 0)
                    printAcknowledgement();
                else
                    printError("entering infrastructure mode failed");
                    
                return;
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else if (compareExtendedCommand("flash", dataPointer))
        {
            startState(ApplicationStateFlashFirmware);
            printAcknowledgement();
            
            return;
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("save", dataPointer))
    {
        // starting a save command
        dataPointer = strtok(NULL," ");
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("config",dataPointer))
        {
            // save config
            if (saveSettings(&applicationSettings, sizeof(ApplicationSettings)) == 0)
            {
                printAcknowledgement();
            }
            else
            {
                printError("saving settings failed");
            }
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("test", dataPointer))
    {
        dataPointer = strtok(NULL," ");
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wifly",dataPointer))
        {
            startState(ApplicationStateWiFlyTest);
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else
    {
        printUnknownCommand();
    }
}

void startState(ApplicationState state)
{
    if (applicationState == state)              // If we are already in this state => ignore
        return;
    
    if ((state != ApplicationStateIdle) 
        && (applicationState != ApplicationStateIdle))  // only changes beetween idle and non idle are possible
        return;
    
    if (state == ApplicationStateIdle)
    {
        applicationState = ApplicationStateIdle;
        
        printfData("Idle\r");
    }
    else if (state == ApplicationStateCaptureCommand)
    {
        applicationState = ApplicationStateCaptureCommand;
        
        printfData("Capturing data\r");
        blinkLed2(2);
        startIrCapture();
    }
    else if (state == ApplicationStateRunCommand)
    {
        applicationState = ApplicationStateRunCommand;
                
        printfData("Running command\r");
        blinkLed(2);
        runIrCommand(currentCommand);
    }
    else if (state == ApplicationStateFlashFirmware)
    {
        setAllLeds();
        
    }
    else if (state == ApplicationStateWiFlyTest)
    {
        applicationState = ApplicationStateWiFlyTest;
        
        printfData("Going into WiFly Test state, all uart in and outputs will redirected.\n");
    }
    
    return;
}

void mainTask(void)
{
        if (applicationState == ApplicationStateIdle)
        {
            processTaskUart0();     // serial task
            processTaskWiFly();     // wifly task
        }
        else if (applicationState == ApplicationStateCaptureCommand)
        {
            currentCommand = getIrCommand();
            if (currentCommand != NULL)    // We finally received something
            {
                outputCommand(currentCommand);
                startState(ApplicationStateIdle);
            }
        }
        else if (applicationState == ApplicationStateRunCommand)
        {
            if (!isCommandRunning())    // Command has finished
                startState(ApplicationStateIdle);
        }
        else if (applicationState == ApplicationStateWiFlyTest)
        {
            static char receivedData;
            while (getcharUart0(&receivedData) == 0)
                putcharWiFly(receivedData);
            while (getcharWiFly(&receivedData) == 0)
                putcharUart0(receivedData);
        }
}

void ledTask(void)
{
    if (isWiFlyConnected()) // keeps also track of the connectedness
    {
        activeConnections |= NetworkConnection;
    }
    else
    {
        activeConnections &= ~NetworkConnection;
    }
    
    if ((activeConnections & NetworkConnection))
    {
        setLed(1);  // Green LED
    }
    else
    {
        if (applicationSettings.networkMode == AdhocNetworkMode)
        {
            toggleLed(1);   // Green LED
        }
        else if (applicationSettings.networkMode == InfrastructureNetworkMode)
        {
            if (readLed(1))
            {
                clearLed(1);
                setLed(3);
            }
            else
            {
                clearLed(3);
                setLed(1);
            }
            
        }
    }
    
    clearLed(2);    // clear the yellow led in case it is still running
}

void buttonTask(void )
{
    ButtonValue buttonValue;
    if (getButtonPress(&buttonValue) == 0)
    {
        printfData("pressed %u, %u\r", buttonValue.id, buttonValue.count);
        if ((buttonValue.id == 0) && (buttonValue.count == 1))
        {
            if (applicationSettings.networkMode == AdhocNetworkMode)
            {
                startWlanInfrastructureMode(&applicationSettings);
                applicationSettings.networkMode = InfrastructureNetworkMode;
                saveSettings(&applicationSettings, sizeof(ApplicationSettings));
            }
            else if (applicationSettings.networkMode == InfrastructureNetworkMode)
            {
                startWlanAdhocMode();
                applicationSettings.networkMode = AdhocNetworkMode;
                saveSettings(&applicationSettings, sizeof(ApplicationSettings));
            }
        }
    }
}

uint32 hex2int(char *a, unsigned int len)
{
    uint32 i;
    uint32 val = 0;

    for(i=0;i<len;i++)
       if(a[i] <= 57)
        val += (a[i]-48)*(1<<(4*(len-1-i)));
       else
        val += (a[i]-87)*(1<<(4*(len-1-i)));
    return val;
}
