/**
 * This is a file 
 */

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
IrCommand *testCommand;
uint8 applicationState = ApplicationStateIdle;
char commandBuffer[100];
uint8 commandBufferPos = 0;

int main(void)
{   
    initializeLeds();
    clearAllLeds();
    
    //Program started notifier
    delayMs(500);
    blinkLed3(1);
    delayMs(500);
    
    initializeSerialConnection();
     
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
    
    initializeIrControl();
    initializeWiFly();
    
    setPinMode(2,10,PinModeNoPullUpDown);       // button3
    setGpioDirection(2,10,GpioDirectionInput);
    enableGpioInterrupt(2,10,GpioInterruptRisingEdge,&testFunc);
    
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
                    commandBufferPos++;
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
            testCommand = getIrCommand();
            if (testCommand != NULL)    // We finally received something
            {
                outputCommand(testCommand);
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



void testFunc()
{
    if (testMode == 0)
    {
        startState(ApplicationStateCaptureCommand);
        testMode = 1;
    }
    else
    {
        startState(ApplicationStateRunCommand);
        testMode = 0;
    }
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
        runIrCommand(testCommand);
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
    printfData("CMD?\n");
}

void printParameterMissing(void)
{
    printfData("Missing parameter.\n");
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
            memcpy(testCommand, dataPointer, sizeof(IrCommand));
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
            }
            else if (compareExtendedCommand("ssid",dataPointer))
            {
                // set ssid
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
