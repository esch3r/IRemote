#include "iremote.h"

ActiveConnection activeConnections = 0;

RemoteCommand *currentCommand;
ApplicationState applicationState = ApplicationState_Idle;
ApplicationSettings applicationSettings;

int8 initializeHardware(void)
{
    Crc_initialize();                    // init crc function for firmware flashing
    Timeout_initialize(Timer1);    // initialize Timer1 for general timeout functions
        
    Led_initialize(1,29, Led_LowActive_Yes);     // led 0 - onboard
    Led_initialize(0,0, Led_LowActive_No);     // led 1 - green
    Led_initialize(0,1, Led_LowActive_No);     // led 2 - yellow
    Led_initialize(0,10, Led_LowActive_No);    // led 3 - red
    Led_clearAll();
    
    Button_initialize(1000, 1E4, 1E5);  //100kHz timer, 10ms timeout
    Button_initializeButton(0, 2, 4, ButtonTypeLowActive);
    Button_initializeButton(1, 2, 5, ButtonTypeLowActive);
    Button_initializeButton(2, 2, 10, ButtonTypeLowActive);
   
    //Program started notifier
    Led_set(Led3);    
    Led_blink(Led2);
    Led_set(Led1);
    Timer_delayMs(200);
    
    RemoteControl_initialize();
    
    // initialize Network Connections
    initializeSerialConnection();
    if (initializeNetworkConnection() == -1)
    {
        printfData("ERR: Network initalization failed");
    }
     
    // Welcome message
    printfData("Welcome to IRemote!\r");    // Send a welcome message
    printfData("Id: %i, Version: %i, Serial: %i\r",Iap_readId(),Iap_readVersion(),Iap_readSerial());
   
    Timer_delayMs(500);
    
    Led_clear(3);
    Led_blink2(0);   //onboard we came through the initialization
    
    return 0;
}

int8 initializeVariables(void )
{
    // init variables
    currentCommand = RemoteCommand_create();
    
    //load settings....
    Iap_loadApplicationSettings(&applicationSettings, sizeof(ApplicationSettings));
    
    if (applicationSettings.firstStartIdentificator != 42)
    {
        applicationSettings.irReceiveTimeout = 25000;
        applicationSettings.irSendTimeout = 50000;
        applicationSettings.irRepeatCount = 2;
        applicationSettings.radio433ReceiveTimeout = 9000;
        applicationSettings.radio433SendTimeout = 10000;
        applicationSettings.radio433RepeatCount = 10;
        applicationSettings.radio868ReceiveTimeout = 9000;
        applicationSettings.radio868SendTimeout = 10000;
        applicationSettings.radio868RepeatCount = 10;
        strcpy(applicationSettings.wlanSsid, "");
        strcpy(applicationSettings.wlanPhrase, "");
        strcpy(applicationSettings.wlanKey, "");
        strcpy(applicationSettings.wlanHostname, "IRemoteBox");
        applicationSettings.wlanAuth = 0;
        applicationSettings.wlanDhcp = 1;
        strcpy(applicationSettings.wlanIp, "169.254.1.1");
        strcpy(applicationSettings.wlanMask, "255.255.0.0");
        strcpy(applicationSettings.wlanGateway, "169.254.1.2");
        applicationSettings.networkMode = NetworkMode_Adhoc;
        
        applicationSettings.firstStartIdentificator = 42;   // remove the first start indicator
    
        startWlanAdhocMode();   // Start the adhoc mode
        Iap_saveApplicationSettings(&applicationSettings, sizeof(ApplicationSettings));
    }
    
    
    RemoteControl_setReceiveTimeout(RemoteControl_Medium_Ir, applicationSettings.irReceiveTimeout);
    RemoteControl_setSendTimeout(RemoteControl_Medium_Ir, applicationSettings.irSendTimeout);
    RemoteControl_setRepeatCount(RemoteControl_Medium_Ir, applicationSettings.irRepeatCount);
    RemoteControl_setReceiveTimeout(RemoteControl_Medium_433Mhz, applicationSettings.radio433ReceiveTimeout);
    RemoteControl_setSendTimeout(RemoteControl_Medium_433Mhz, applicationSettings.radio433SendTimeout);
    RemoteControl_setRepeatCount(RemoteControl_Medium_433Mhz, applicationSettings.radio433RepeatCount);
    RemoteControl_setReceiveTimeout(RemoteControl_Medium_868Mhz, applicationSettings.radio868ReceiveTimeout);
    RemoteControl_setSendTimeout(RemoteControl_Medium_868Mhz, applicationSettings.radio868SendTimeout);
    RemoteControl_setRepeatCount(RemoteControl_Medium_868Mhz, applicationSettings.radio868RepeatCount);
    
    return 0;
}

int8 initializeSerialConnection(void)
{
    if (Uart_initialize(Uart0, 115200) == 0)
    {
        Uart_flush(Uart0);                           // Trash all unusefull characters
        activeConnections |= SerialConnection;
        
        Uart_setProcessFunction(Uart0, &processCommand);
        Uart_setErrorFunction(Uart0, &errorCommand);
    
        return 0;
    }
    
    return -1;
}

int8 initializeNetworkConnection(void)
{
    if (WiFly_initialize(Uart1, 115200) == 0)
    {
        if (WiFly_actionEnterCommandMode(FALSE) == 0)      // Configure the wlan module
        {
            WiFly_setSysPrintlvl(0);             // Turn off unessesary messages
            WiFly_setWlanJoin(2);                // Auto join
            WiFly_setDnsName("IRemoteBox");      // Sets the default hostname
            WiFly_setDnsBackup("IRemote");       // Sets the backup hostname
            WiFly_actionExitCommandMode();
            
            WiFly_setProcessFunction(&processCommand);
            WiFly_setErrorFunction(&errorWiFly);
            
            return 0;
        }
        else
            return -1;
    }
    
    return -1;
}

int8 startWlanInfrastructureMode(ApplicationSettings *settings)
{
    applicationSettings.networkMode = NetworkMode_Infrastructure;
    
    if (WiFly_actionEnterCommandMode(FALSE) == -1)
        return -1;
    if (WiFly_setInfrastructureParams() == -1)
        return -1;
    if (WiFly_setWlanSsid(settings->wlanSsid) == -1)
        return -1;
    if (WiFly_setWlanPhrase(settings->wlanPhrase) == -1)
        return -1;
    if (WiFly_setWlanKey(settings->wlanKey) == -1)
        return -1;
    if (WiFly_setWlanAuth(settings->wlanAuth) == -1)
        return -1;
    if (WiFly_setWlanJoin(1) == -1)  // Turn on Auto Join mode
        return -1;
    if (WiFly_setDnsName(settings->wlanHostname) == -1)
        return -1;
    if (WiFly_setIpDhcp(settings->wlanDhcp) == -1)
        return -1;
    if (WiFly_setIpAddress(settings->wlanIp) == -1)
        return -1;
    if (WiFly_setIpNetmask(settings->wlanMask) == -1)
        return -1;
    if (WiFly_setIpGateway(settings->wlanGateway) == -1)
        return -1;
    if (WiFly_fileIoSaveDefault() == -1)
        return -1;
    
    if (WiFly_actionReboot() == -1)
    {
        applicationSettings.networkMode = NetworkMode_None;
        return -1;
    }
    else
    {
        applicationSettings.networkMode = NetworkMode_Infrastructure;
        Iap_saveApplicationSettings(&applicationSettings, sizeof(ApplicationSettings));
        return 0;
    }
}

int8 startWlanAdhocMode(void)
{
    applicationSettings.networkMode = NetworkMode_Adhoc;
    
    char buffer[100];
    snprintf(buffer, 100, "IRemoteBox_%u", Iap_readSerial());
    
    if (WiFly_createAdhocNetwork(buffer) == -1)
    {
        applicationSettings.networkMode = NetworkMode_None;
        return -1;
    }
    else
    {
        applicationSettings.networkMode = NetworkMode_Adhoc;
        Iap_saveApplicationSettings(&applicationSettings, sizeof(ApplicationSettings));
        return 0;
    }
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
            if (WiFly_putchar(buffer[i]) == -1)
                return -1;
            i++;
        }
        return 0;
    }
    else if (activeConnections & SerialConnection)
    {
        while (buffer[i] != 0)      // Loop through until reach string's zero terminator
        {
            if (Uart_putchar(Uart0, buffer[i]) == -1)
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
        return WiFly_putchar(c);
    }
    else if (activeConnections & SerialConnection)
    {
        return Uart_putchar(Uart0, c);
    }
    
    return -1;
}

int8 writeData(void *data, uint32 length)
{   
    if (activeConnections & NetworkConnection)
    {
        return WiFly_writeData(data, length);
    }
    else if (activeConnections & SerialConnection)
    {
        return Uart_writeData(Uart0, data, length);
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
    Led_clear(2);
}

void printParameterMissing(void)
{
    printfData("Missing parameter.\r");
    Led_clear(2);
}

void printAcknowledgement(void)
{
    printfData("ACK\r");
    Led_clear(2);
}

void printError(char *message)
{
    printfData("ERR: %s\r", message);
}

void printAliveMessage(void)
{
    printfData("yes\r");
    Led_clear(2);
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
    char *savePointer;
    
    Led_set(LedYellow);  // set the yellow led to indicate incoming data status
    
    dataPointer = strtok_r(buffer, " ", &savePointer);
    
    if (compareBaseCommand("alive", dataPointer))
    {
        // We have a keep alive command
        printAliveMessage();
    }
    else if (compareBaseCommand("run", dataPointer))
    {
        // We have a run command
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer != NULL)
        {
            uint16 commandSize = strlen(dataPointer);
            uint16 i;
            for (i = 0; i < commandSize; i+=2)
            {
                ((char*)currentCommand)[i/2] = (char)hex2int(dataPointer+i,2);
            }
        }
        startState(ApplicationState_RunCommand);
    }
    else if (compareBaseCommand("capture", dataPointer))
    {
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("ir",dataPointer))
        {
            startState(ApplicationState_CaptureIrCommand);
        }
        else if (compareExtendedCommand("radio433", dataPointer))
        {
            startState(ApplicationState_CaptureRadio433MhzCommand);
        }
        else if (compareExtendedCommand("radio868", dataPointer))
        {
            startState(ApplicationState_CaptureRadio868MhzCommand);
        }
        else
        {
            printUnknownCommand();
            return;
        }
    }
    else if (compareBaseCommand("stop", dataPointer))
    {
        startState(ApplicationState_Idle);
    }
    else if (compareBaseCommand("flash", dataPointer))
    {
        // We have a flash command
        char buffer[100];
        uint16 receivedChecksum;
        uint16 calculatedChecksum;
        
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer != NULL)
        {
            uint16 commandSize = strlen(dataPointer);
            uint16 i;
            for (i = 0; i < commandSize; i+=2)
            {
                buffer[i/2] = (char)hex2int(dataPointer+i,2);
            }
            
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer != NULL)
            {
                receivedChecksum = (uint16)hex2int(dataPointer,4);;
                calculatedChecksum = Crc_fast(buffer, 100);
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
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wlan",dataPointer))
        {
            // set wlan
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("ssid",dataPointer))
            {
                // set ssid
                dataPointer = strtok_r(NULL, " ", &savePointer);
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
                dataPointer = strtok_r(NULL, " ", &savePointer);
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
                dataPointer = strtok_r(NULL, " ", &savePointer);
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
                dataPointer = strtok_r(NULL, " ", &savePointer);
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
                dataPointer = strtok_r(NULL, " ", &savePointer);
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
                dataPointer = strtok_r(NULL, " ", &savePointer);
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
                dataPointer = strtok_r(NULL, " ", &savePointer);
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
                dataPointer = strtok_r(NULL, " ", &savePointer);
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
                dataPointer = strtok_r(NULL, " ", &savePointer);
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
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.irReceiveTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setReceiveTimeout(RemoteControl_Medium_Ir, applicationSettings.irReceiveTimeout);
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

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.irSendTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setSendTimeout(RemoteControl_Medium_Ir, applicationSettings.irSendTimeout);
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

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.irRepeatCount = atoi(dataPointer);
                    RemoteControl_setRepeatCount(RemoteControl_Medium_Ir, applicationSettings.irRepeatCount);
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
        else if (compareExtendedCommand("433",dataPointer))
        {
            // set ir
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio433ReceiveTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setReceiveTimeout(RemoteControl_Medium_433Mhz, applicationSettings.radio433ReceiveTimeout);
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

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio433SendTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setSendTimeout(RemoteControl_Medium_433Mhz, applicationSettings.radio433SendTimeout);
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

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio433RepeatCount = atoi(dataPointer);
                    RemoteControl_setRepeatCount(RemoteControl_Medium_433Mhz, applicationSettings.radio433RepeatCount);
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
        else if (compareExtendedCommand("868",dataPointer))
        {
            // set ir
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio868ReceiveTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setReceiveTimeout(RemoteControl_Medium_868Mhz, applicationSettings.radio868ReceiveTimeout);
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

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio868SendTimeout = atoi(dataPointer)*1000;
                    RemoteControl_setSendTimeout(RemoteControl_Medium_868Mhz, applicationSettings.radio868SendTimeout);
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

                dataPointer = strtok_r(NULL, " ", &savePointer);
                if (dataPointer != NULL)
                {
                    applicationSettings.radio868RepeatCount = atoi(dataPointer);
                    RemoteControl_setRepeatCount(RemoteControl_Medium_868Mhz, applicationSettings.radio868RepeatCount);
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
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wlan",dataPointer))
        {
            // get wlan
            dataPointer = strtok_r(NULL, " ", &savePointer);
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
            dataPointer = strtok_r(NULL, " ", &savePointer);
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
        else if (compareExtendedCommand("433",dataPointer))
        {
            // set ir
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio433ReceiveTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("sendTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio433SendTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("count",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio868RepeatCount);
                printAcknowledgement();
                return;
            }
            else
            {
                printUnknownCommand();
                return;
            }
        }
        else if (compareExtendedCommand("868",dataPointer))
        {
            // set ir
            dataPointer = strtok_r(NULL, " ", &savePointer);
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("receiveTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio868ReceiveTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("sendTimeout",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio868SendTimeout/1000);
                printAcknowledgement();
                return;
            }
            else if (compareExtendedCommand("count",dataPointer))
            {
                printfData("%u\r", applicationSettings.radio868RepeatCount);
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
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wlan",dataPointer))
        {
            // set wlan
            dataPointer = strtok_r(NULL, " ", &savePointer);
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
            startState(ApplicationState_FlashFirmware);
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
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("config",dataPointer))
        {
            // save config
            if (Iap_saveApplicationSettings(&applicationSettings, sizeof(ApplicationSettings)) == 0)
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
        dataPointer = strtok_r(NULL, " ", &savePointer);
        if (dataPointer == NULL)
        {
            printUnknownCommand();
            return;
        }
        else if (compareExtendedCommand("wifly",dataPointer))
        {
            startState(ApplicationState_WiFlyTest);
        }
        else if (compareExtendedCommand("response",dataPointer))
        {
            printfData(WiFly_getAdhoc());
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
    
    if ((state != ApplicationState_Idle) 
        && (applicationState != ApplicationState_Idle))  // only changes beetween idle and non idle are possible
        return;
    
    if (state == ApplicationState_Idle)
    {
        applicationState = ApplicationState_Idle;
        
        printfData("Idle\r");
    }
    else if (state == ApplicationState_CaptureIrCommand)
    {
        applicationState = ApplicationState_CaptureIrCommand;
        
        printfData("Capturing IR data\r");
        Led_blink2(Led2);
        RemoteControl_startCapture(RemoteControl_Medium_Ir);
    }
    else if (state == ApplicationState_CaptureRadio433MhzCommand)
    {
        applicationState = ApplicationState_CaptureIrCommand;
        
        printfData("Capturing 433MHz data\r");
        Led_blink2(Led2);
        RemoteControl_startCapture(RemoteControl_Medium_433Mhz);
    }
    else if (state == ApplicationState_CaptureRadio868MhzCommand)
    {
        applicationState = ApplicationState_CaptureIrCommand;
        
        printfData("Capturing 868MHz data\r");
        Led_blink2(Led2);
        RemoteControl_startCapture(RemoteControl_Medium_868Mhz);
    }
    else if (state == ApplicationState_RunCommand)
    {
        applicationState = ApplicationState_RunCommand;
                
        printfData("Running command\r");
        Led_blink(Led2);
        RemoteControl_runCommand(currentCommand);
    }
    else if (state == ApplicationState_FlashFirmware)
    {
        Led_setAll();
        
    }
    else if (state == ApplicationState_WiFlyTest)
    {
        applicationState = ApplicationState_WiFlyTest;
        
        printfData("Going into WiFly Test state, all uart in and outputs will redirected.\n");
    }
    
    return;
}

void mainTask(void)
{
        if (applicationState == ApplicationState_Idle)
        {
            Uart_processTask0();     // serial task
            WiFly_processTask();     // wifly task
        }
        else if ((applicationState == ApplicationState_CaptureIrCommand)
              || (applicationState == ApplicationState_CaptureRadio433MhzCommand)
              || (applicationState == ApplicationState_CaptureRadio868MhzCommand)
        )
        {
            currentCommand = RemoteControl_command();
            if (currentCommand != NULL)    // We finally received something
            {
                RemoteControl_outputCommand(currentCommand);
                startState(ApplicationState_Idle);
            }
        }
        else if (applicationState == ApplicationState_RunCommand)
        {
            if (!RemoteControl_isCommandRunning())    // Command has finished
                startState(ApplicationState_Idle);
        }
        else if (applicationState == ApplicationState_WiFlyTest)
        {
            static char receivedData;
            while (Uart_getchar(Uart0, &receivedData) == 0)
                WiFly_putchar(receivedData);
            while (WiFly_getchar(&receivedData) == 0)
                Uart_putchar(Uart0, receivedData);
        }
}

void ledTask(void)
{
    if (WiFly_isConnected()) // keeps also track of the connectedness
    {
        activeConnections |= NetworkConnection;
    }
    else
    {
        activeConnections &= ~NetworkConnection;
    }
    
    if ((activeConnections & NetworkConnection))
    {
        Led_set(LedGreen);  // Green LED
    }
    else
    {
        if (applicationSettings.networkMode == NetworkMode_Adhoc)
        {
            Led_toggle(LedGreen);   // Green LED
            Led_clear(LedRed);
        }
        else if (applicationSettings.networkMode == NetworkMode_Infrastructure)
        {
            if (Led_read(LedGreen))
            {
                Led_clear(LedGreen);
                Led_set(LedRed);
            }
            else
            {
                Led_clear(LedRed);
                Led_set(LedGreen);
            }
            
        }
        else if (applicationSettings.networkMode == NetworkMode_None)
        {
            if (Led_read(LedGreen))
            {
                Led_clear(LedGreen);
                Led_set(LedYellow);
            }
            else if (Led_read(LedYellow))
            {
                Led_clear(LedYellow);
                Led_set(LedRed);
            }
            else
            {
                Led_clear(LedRed);
                Led_set(LedGreen);
            }
        }
    }
    
    Led_clear(LedYellow);    // clear the yellow led in case it is still running
}

void buttonTask(void )
{
    ButtonValue buttonValue;
    if (Button_getPress(&buttonValue) == 0)
    {
        printfData("pressed %u, %u\r", buttonValue.id, buttonValue.count);
        if ((buttonValue.id == 0) && (buttonValue.count == 1))
        {
            if (applicationSettings.networkMode == NetworkMode_Adhoc)
            {
                startWlanInfrastructureMode(&applicationSettings);
            }
            else if (applicationSettings.networkMode == NetworkMode_Infrastructure)
            {
                startWlanAdhocMode();
            }
            else
            {
                startWlanAdhocMode();
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
