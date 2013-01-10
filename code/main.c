/**
 * This is a file 
 */

#define COMMAND_BUFFER_SIZE 250
#define DATA_BUFFER_SIZE    250

#include <led.h>
#include <iap.h>
#include <wifly.h>
#include <timeout.h>
#include <button.h>
#include <pwm.h>
#include "irControl.h"
#include "iremote.h"


typedef enum {
    ApplicationStateIdle = 0,
    ApplicationStateCaptureCommand = 1,
    ApplicationStateRunCommand = 2,
    ApplicationStateWiFlyTest = 100
} ApplicationState;

typedef enum {
    NetworkStateConnected = 0,
    NetworkStateDisconnected = 1
} NetworkState;

void testFunc();
void startState(ApplicationState state);
void processCommand(char* buffer);
void processWiFly(char* buffer);
void errorCommand();
void errorWiFly();

IrCommand *currentCommand;
ApplicationState applicationState = ApplicationStateIdle;
NetworkState networkState = NetworkStateDisconnected;

char dataBuffer[DATA_BUFFER_SIZE];

int main(void)
{   
    initializeTimeout(TIMER1);
        
    initializeLeds();
    clearAllLeds();
    
   initializeButton(10,1,2,10);
                              
    
    //Program started notifier
    delayMs(500);
    blinkLed3(1);
    delayMs(500);
    
    initializeSerialConnection();
    
    if (initializeNetworkConnection() == -1)
    {
        printfData("ERR: Network initalization failed");
    }
    initializeIrControl();
    
    setProcessFunctionUart0(&processCommand);
    setErrorFunctionUart0(&errorCommand);
    setProcessFunctionWiFly(&processCommand);
    setErrorFunctionWiFly(&errorWiFly);
     
    printfData("Welcome to IRemote!\r");    // Send a welcome message
    printfData("Id: %i, Version: %i, Serial: %i\r",readIdIap(),readVersionIap(),readSerialIap());
   
    blinkLed2(1);
    
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
        delayMs(50);
     }

    return 0 ;
}

void errorCommand()
{
    printfData("ERR: Command too long\r");
}

void errorWiFly()
{
    printfData("ERR: WiFly command too long\r");
}

void startState(ApplicationState state)
{
    if (applicationState == state)              // If we are already in this state => ignore
        return;
    
    if (state == ApplicationStateIdle)
    {
        applicationState = ApplicationStateIdle;
        
        printfData("Going into idle\r");
    }
    else if (state == ApplicationStateCaptureCommand)
    {
        applicationState = ApplicationStateCaptureCommand;
        
        printfData("Start capturing data\r");
        blinkLed2(1);
        startIrCapture();
    }
    else if (state == ApplicationStateRunCommand)
    {
        applicationState = ApplicationStateRunCommand;
                
        printfData("Start running command\r");
        blinkLed(1);
        runIrCommand(currentCommand);
    }
    else if (state == ApplicationStateWiFlyTest)
    {
        applicationState = ApplicationStateWiFlyTest;
        
        printfData("Going into WiFly Test state, all uart in and outputs will redirected.\n");
    }
    
    return;
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

void printUnknownCommand(void)
{
    printfData("CMD?\r");
}

void printParameterMissing(void)
{
    printfData("Missing parameter.\r");
}

void printAcknowledgement(void)
{
    printfData("ACK\r");
}

void printError(char *message)
{
    printfData("ERR: %s\r", message);
}

void processWiFly(char *buffer)
{
    
}
void processCommand(char *buffer)
{
    char *dataPointer;
    
    dataPointer = strtok(buffer," ");
    
    if (compareBaseCommand("run", dataPointer))
    {
        // We have a run command
        dataPointer = strtok(NULL," ");
        if (dataPointer != NULL)
        {
            memcpy(currentCommand, dataPointer, sizeof(IrCommand));
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
                    //strncpy(dataBuffer, dataPointer, DATA_BUFFER_SIZE);
                    //printfData("%s %s\r", dataBuffer, dataPointer);
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
                    strncpy(dataBuffer, dataPointer, DATA_BUFFER_SIZE);
                    if (setWiFlyWlanPhrase(dataBuffer) == 0)
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
                    strncpy(dataBuffer, dataPointer, DATA_BUFFER_SIZE);
                    if (setWiFlyWlanKey(dataBuffer) == 0)
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
                // set ssid
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    strncpy(dataBuffer, dataPointer, DATA_BUFFER_SIZE);
                    if (setWiFlyDnsName(dataBuffer) == 0)
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
                // set ssid
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    strncpy(dataBuffer, dataPointer, DATA_BUFFER_SIZE);
                    if (setWiFlyWlanAuth(atoi(dataBuffer)) == 0)
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
                    printAcknowledgement();
                else
                    printError("entering command mode failed");
                    
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
