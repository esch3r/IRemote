/**
 * This is the main file
 */

#include <led.h>
#include <iap.h>
#include <wifly.h>
#include <timeout.h>
#include <button.h>
#include <pwm.h>
#include <crc.h>
#include "irControl.h"
#include "iremote.h"


typedef enum {
    ApplicationStateIdle = 0,
    ApplicationStateCaptureCommand = 1,
    ApplicationStateRunCommand = 2,
    ApplicationStateFlashFirmware = 3,
    ApplicationStateWiFlyTest = 100
} ApplicationState;

typedef enum {
    NetworkStateConnected = 0,
    NetworkStateDisconnected = 1
} NetworkState;

typedef struct {
    uint32 irReceiveTimeout;
    uint32 irSendTimeout;
    uint32 irRepeatCount;
    char   wlanSsid[100];
    char   wlanPhrase[100];
    char   wlanKey[100];
    char   wlanHostname[100];
    uint8  wlanAuth;
    uint8  wlanDhcp;
    char   wlanIp[20];
    char   wlanMask[20];
    char   wlanGateway[20];
    
} ApplicationSettings;

void startState(ApplicationState state);
void processCommand(char* buffer);

IrCommand *currentCommand;
ApplicationState applicationState = ApplicationStateIdle;
NetworkState networkState = NetworkStateDisconnected;
ApplicationSettings applicationSettings;

int main(void)
{   
    crcInit();                    // init crc function for firmware flashing
    
    initializeTimeout(TIMER1);
        
    initializeLed(1,29, TRUE);     // led 0 - onboard
    initializeLed(0,0, FALSE);     // led 1 - green
    initializeLed(0,1, FALSE);     // led 2 - yellow
    initializeLed(0,10, FALSE);    // led 3 - red
    clearAllLeds();
   
    // initializeButton(10,1,2,10);
    initializeButton(10,1,2,0,1);
   
    //Program started notifier
    delayMs(500);
    setLed(3);    
    blinkLed(2);
    setLed(1);
    delayMs(500);
    
    initializeIrControl();
    initializeSerialConnection();
    
    if (initializeNetworkConnection() == -1)
    {
        printfData("ERR: Network initalization failed");
    }
    
    setProcessFunctionUart0(&processCommand);
    setErrorFunctionUart0(&errorCommand);
    setProcessFunctionWiFly(&processCommand);
    setErrorFunctionWiFly(&errorWiFly);
     
    printfData("Welcome to IRemote!\r");    // Send a welcome message
    printfData("Id: %i, Version: %i, Serial: %i\r",readIdIap(),readVersionIap(),readSerialIap());
   
    clearLed(3);
    blinkLed2(0);   //onboard we came through the initialization
    
    // init variables
    currentCommand = createIrCommand();
    
    
    // Testing IAP functions
    //    uint32 testVar;
    //printfUart0("Sector 26: %i, Sector 27: %i\n",checkBlankIap(26),checkBlankIap(27));
    //testVar = 0;
    //__disable_irq();
    //eraseIap(27); 
    //writeIap(27,256*64,(void*)(&testVar),sizeof(testVar));
    //readIap(27,256*64,(void*)(&testVar),sizeof(testVar));
    //printfUart0("Test: %u\n",testVar);
    //__enable_irq();
    
    //setPinMode(2,10,PinModeNoPullUpDown);       // button3
    //setGpioDirection(2,10,GpioDirectionInput);
    //enableGpioInterrupt(2,10,GpioInterruptRisingEdge,&testFunc);
    
    //setGpioDirection(0,9,GpioDirectionOutput);   // Output pin for testing purposes
    
    uint8 ledTiming = 0;
    
    for (;;) 
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
        
        if (ledTiming == 200)
        {
            ledTask();
            ledTiming = 0;
        }
        ledTiming++;
        
        delayMs(5);
     }

    return 0 ;
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
                    if (setWiFlyWlanSsid(dataPointer) == 0)
                        printAcknowledgement();
                    else
                        printError("setting SSID failed");
                    
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
                    if (setWiFlyWlanPhrase(dataPointer) == 0)
                        printAcknowledgement();
                    else
                        printError("setting passphrase failed");
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
                    if (setWiFlyWlanKey(dataPointer) == 0)
                        printAcknowledgement();
                    else
                        printError("setting key failed");
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
                    if (setWiFlyDnsName(dataPointer) == 0)
                        printAcknowledgement();
                    else
                        printError("setting hostname failed");
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
                    if (setWiFlyWlanAuth(atoi(dataPointer)) == 0)
                        printAcknowledgement();
                    else
                        printError("setting auth mode failed");
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
                    if (setWiFlyIpDhcp(atoi(dataPointer)) == 0)
                        printAcknowledgement();
                    else
                        printError("setting dhcp mode failed");
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
                    if (setWiFlyIpAddress(dataPointer) == 0)
                        printAcknowledgement();
                    else
                        printError("setting ip address failed");
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
                    if (setWiFlyIpNetmask(dataPointer) == 0)
                        printAcknowledgement();
                    else
                        printError("setting mask failed");
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
                    if (setWiFlyIpGateway(dataPointer) == 0)
                        printAcknowledgement();
                    else
                        printError("setting gateway failed");
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
            else if (compareExtendedCommand("config",dataPointer))
            {
                if (actionWiFlyEnterCommandMode(FALSE) == 0)
                {
                    if (setWiflyInfrastructureParams() == 0)
                        printAcknowledgement();
                    else
                        printError("setting infrastructure settings failed");
                }
                else
                    printError("entering command mode failed");
                    
                return;
            }
            else if (compareExtendedCommand("adhoc",dataPointer))
            {
                if (createWiFlyAdhocNetwork("IRemoteBox") == 0)
                    printAcknowledgement();
                else
                    printError("entering adhoc mode failed");
                    
                return;
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
        else if (compareExtendedCommand("wlan",dataPointer))
        {
            // set wlan
            dataPointer = strtok(NULL," ");
            if (dataPointer == NULL)
            {
                printUnknownCommand();
                return;
            }
            else if (compareExtendedCommand("config",dataPointer))
            {
                if (fileIoWiFlySaveDefault() == 0)
                {
                    if (actionWiFlyReboot() == 0)
                        printAcknowledgement();
                    else
                        printError("reboot failed");
                }
                else
                    printError("saving config failed");
                    
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
