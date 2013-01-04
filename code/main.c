/**
 * This is a file 
 */

#define COMMAND_BUFFER_SIZE 200
#define DATA_BUFFER_SIZE    200

#include <led.h>
#include <iap.h>
#include <wifly.h>
#include "irControl.h"
#include "iremote.h"

typedef enum {
    ApplicationStateIdle = 0,
    ApplicationStateCaptureCommand = 1,
    ApplicationStateRunCommand = 2,
    ApplicationStateWiFlyTest = 100
} ApplicationState;

void testFunc();
void startState(ApplicationState state);
void processCommand(void);

uint8 testMode = 0;
IrCommand *currentCommand;
uint8 applicationState = ApplicationStateIdle;

char commandBuffer[COMMAND_BUFFER_SIZE];
uint8 commandBufferPos = 0;

char dataBuffer[DATA_BUFFER_SIZE];

int main(void)
{   
    initializeLeds();
    clearAllLeds();
    
    //Program started notifier
    delayMs(500);
    blinkLed3(1);
    delayMs(500);
    
    initializeSerialConnection();
    initializeNetworkConnection();
    initializeIrControl();
     
    printfData("Welcome to IRemote!\r");    // Send a welcome message
    printfData("Id: %i, Version: %i, Serial: %i\r",readIdIap(),readVersionIap(),readSerialIap());
   
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
    
    setGpioDirection(0,9,GpioDirectionOutput);   // Output pin for testing purposes
    
    for (;;) 
    {
        if (applicationState == ApplicationStateIdle)
        {
            static char receivedData;
            while (getcharUart0(&receivedData) == 0)
            {
                if (receivedData != 13) // Carriage Return (CR)
                {
                    commandBuffer[commandBufferPos] = receivedData;
                    if (commandBufferPos < COMMAND_BUFFER_SIZE-1)
                    {
                        commandBufferPos++;
                    }
                    else
                    {
                        printfData("ERR: command to long");
                        commandBufferPos = 0;
                    }
                }
                else
                {
                    commandBuffer[commandBufferPos] = '\0';
                    processCommand();
                    commandBufferPos = 0;
                }
            }
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
        delayMs(100);
     }

    return 0 ;
}

void startState(ApplicationState state)
{
    if (applicationState == state)              // If we are already in this state => ignore
        return;
    
    if (state == ApplicationStateIdle)
    {
        applicationState = ApplicationStateIdle;
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

void processCommand(void )
{
    char *dataPointer;
    
    dataPointer = strtok(commandBuffer," ");
    
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
                    strncpy(dataBuffer, dataPointer, DATA_BUFFER_SIZE);
                    actionWiFlyEnterCommandMode();
                    setWiFlyWlanSsid(dataBuffer);
                    actionWiFlyExitCommandMode();
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
                    strncpy(dataBuffer, dataPointer, DATA_BUFFER_SIZE);
                    actionWiFlyEnterCommandMode();
                    setWiFlyWlanPhrase(dataBuffer);
                    actionWiFlyExitCommandMode();
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
                    strncpy(dataBuffer, dataPointer, DATA_BUFFER_SIZE);
                    actionWiFlyEnterCommandMode();
                    setWiFlyWlanKey(dataBuffer);
                    actionWiFlyExitCommandMode();
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
                // set ssid
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    strncpy(dataBuffer, dataPointer, DATA_BUFFER_SIZE);
                    actionWiFlyEnterCommandMode();
                    setWiFlyDnsName(dataBuffer);
                    actionWiFlyExitCommandMode();
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
                // set ssid
                dataPointer = strtok(NULL," ");
                if (dataPointer != NULL)
                {
                    strncpy(dataBuffer, dataPointer, DATA_BUFFER_SIZE);
                    actionWiFlyEnterCommandMode();
                    setWiFlyWlanAuth(atoi(dataBuffer));
                    actionWiFlyExitCommandMode();
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
