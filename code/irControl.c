#include "irControl.h"

uint8   frameReceived = 0;
uint8   firstCapture = 1;
uint8   currentPosition = 0;
CircularBuffer buffer0;
//char stringBuffer[1000];

IrCommand *tmpCommand;

void initializeIrControl(void)
{
    initializeCb(&buffer0,100,sizeof(uint16));
        
    initializePWM(38000,0.5,1);
        
    setGpioDirection(IR_CAPTURE_PORT, IR_CAPTURE_PIN, GpioDirectionInput);  // TSOP input pin
    
    initializeTimer3(1000,1E9);
    //connectFunctionTimer3(&myFunc);
    //startTimer3();
    
}

void outputCommand(IrCommand *command)
{
    //static uint16 item;
    uint8 i;
        
    if (frameReceived == 1)
    {
        frameReceived = 0;
        
        printfUart0("Rec: ");
        /*while (getCb(&buffer0,&item) == 0)
        {
            sprintf(stringBuffer,"%s %u",stringBuffer,item);
        }
        sprintf(stringBuffer,"%s\n",stringBuffer);*/
        for (i = 0; i < command->length; i++)
        {
            printfUart0(" %u",command->data[i]);
            delayMs(10);
        }
        printfUart0("\n");
    }
}

IrCommand* getIrCommand(void)
{
    if (frameReceived == 1)
        return tmpCommand;
    else
        return NULL;
}

void startIrCapture(void)
{
    tmpCommand = createIrCommand();
    firstCapture = 1;
        
    enableGpioInterrupt(IR_CAPTURE_PORT, IR_CAPTURE_PIN, GpioInterruptFallingAndRisingEdge, &captureFunction);
    //startTimer3();
}

void stopIrCapture(void)
{
    stopTimer3();
    disableGpioInterrupt(IR_CAPTURE_PORT, IR_CAPTURE_PIN);
}

void captureFunction(void)
{
    uint16 timeDiff;
    
    if (firstCapture != 1)                     // Check for first run, ignore value
    {
        timeDiff = (uint16)getCounterValueTimer3();

        if (timeDiff >= IR_TIMEOUT)     // Detected a timeout => frameReceived
        {
            firstCapture = 1;
            saveIrFrame(&buffer0, tmpCommand);
            stopIrCapture();
        }
        else
        {
            putCb(&buffer0, &timeDiff);
        }
    }
    else 
    {
        firstCapture = 0;
    }
    
    resetTimer3();                      // Reset the timer
}

void saveIrFrame(CircularBuffer *buffer, IrCommand *command)
{
    uint16 item;
    uint8 i = 0;
        
    while (getCb(buffer,&item) == 0)
    {
        if (item == 0)  //remove the 0 from starting
            continue;
        command->data[i] = item;
        i++;
    }
    
    if (i > 0)
    {
        command->length = i;
        frameReceived = 1;
    }
}

void runIrCommand(IrCommand* command)
{
    tmpCommand = command;
    
    connectFunctionTimer3(&runFunction);
    setIntervalUsTimer3(10);
}

void runFunction(void )
{
    togglePwm(1);
    
}
