/**
 * This is a file 
 */

#include <led.h>
#include <iap.h>
#include "irControl.h"

typedef enum {
    ApplicationStateIdle = 0,
    ApplicationStateCaptureCommand = 1,
    ApplicationStateRunCommand = 2
} ApplicationState;

void testFunc();
void startState(ApplicationState state);

uint8 testMode = 0;
IrCommand *testCommand;
uint8 applicationState = ApplicationStateIdle;

int main(void)
{   
    uint32 testVar;
    char   testChar;
    
    initializeLeds();
    clearAllLeds();
    
    //Program started notifier
    delayMs(500);
    blinkLed3(1);
    delayMs(500);
    
    initializeUart0(115200);                    // Init the UART
    printfUart0("Welcome to IRemote!\n");    // Send a welcome message
    printfUart0("Id: %i, Version: %i, Serial: %i\n",readIdIap(),readVersionIap(),readSerialIap());
    //printfUart0("Sector 26: %i, Sector 27: %i\n",checkBlankIap(26),checkBlankIap(27));
    testVar = 0;
    __disable_irq();
    //eraseIap(27);
    //writeIap(27,256*64,(void*)(&testVar),sizeof(testVar));
    readIap(27,256*64,(void*)(&testVar),sizeof(testVar));
    printfUart0("Test: %u\n",testVar);
    __enable_irq();
    
    initializeIrControl();
    
    setPinMode(2,10,PinModeNoPullUpDown);       // button3
    setGpioDirection(2,10,GpioDirectionInput);
    enableGpioInterrupt(2,10,GpioInterruptRisingEdge,&testFunc);
    
    setGpioDirection(0,9,GpioDirectionOutput);   // Output pin for testing purposes
    
    for (;;) 
    {
        if (applicationState == ApplicationStateIdle)
        {
            while (getcharUart0(&testChar) == 0)
            {
                if (testChar == 'r')
                    startState(ApplicationStateRunCommand);
                else if (testChar == 'c')
                    startState(ApplicationStateCaptureCommand);
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
        
        printfUart0("Start capturing data\n");
        blinkLed2(1);
        startIrCapture();
    }
    else if (state = ApplicationStateRunCommand)
    {
        applicationState = ApplicationStateRunCommand;
                
        printfUart0("Start running command\n");
        blinkLed(1);
        runIrCommand(testCommand);
    }
    
    return;
}
