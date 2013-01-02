#include "irControl.h"

uint8   frameReceived = 0;
uint8   firstCapture = 1;
uint8   currentPosition = 0;
uint8   commandRunning = 0;
CircularBuffer buffer0;

uint8   repeatCount = 5;
uint8   currentRepeatCount = 0;

IrCommand *tmpCommand;

void initializeIrControl(void)
{
    initializeCb(&buffer0,100,sizeof(uint16));
        
    initializePWM(38000,0.5,1);
        
    setGpioDirection(IR_CAPTURE_PORT, IR_CAPTURE_PIN, GpioDirectionInput);  // TSOP input pin
    
    initializeTimer3(1000,1E9);
    
}

void outputCommand(IrCommand *command)
{
    if (frameReceived == 1)
    {
        frameReceived = 0;
        
        printfUart0("*DATA");
        writeDataUart0((void*)command, sizeof(IrCommand));
        printfUart0("\r");
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
    frameReceived = 0;
    firstCapture = 1;
        
    setIntervalMsTimer3(1E6);
    connectFunctionTimer3(NULL);
    enableGpioInterrupt(IR_CAPTURE_PORT, IR_CAPTURE_PIN, GpioInterruptFallingAndRisingEdge, &captureFunction);
    //startTimer3();
    resetTimer3();
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
            if (saveIrFrame(&buffer0, tmpCommand) == -1)
            {
                firstCapture = 1;
            }
            else
            {
                stopIrCapture();
                return;
            }
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

int8 saveIrFrame(CircularBuffer *buffer, IrCommand *command)
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
        return 0;
    }
    
    return -1;
}

void runIrCommand(IrCommand* command)
{
    tmpCommand = command;
    currentPosition = 0;
    commandRunning = 1;     // Set the variable which indicates that a command is running
    
    connectFunctionTimer3(&runFunction);
    setIntervalUsTimer3(10);
    startTimer3();
}

void stopIrCommand(void )
{
    stopTimer3();
    stopPwm(1);
    currentRepeatCount++;
    
    if (currentRepeatCount == repeatCount)  // Repeat the command a few times
    {
        commandRunning = 0;
        currentRepeatCount = 0;
    }
    else
    {
        currentPosition = 0;
        setIntervalUsTimer3(IR_TIMEOUT);    // Timeout between commands
        startTimer3();
    }
}

void runFunction(void )
{
    togglePwm(1);
    if (currentPosition < tmpCommand->length)
    {
        setIntervalUsTimer3(tmpCommand->data[currentPosition]);

        currentPosition++;
    }
    else
    {
        stopIrCommand();
    }
}

uint8 isCommandRunning(void )
{
    return commandRunning;
}

void setIrRepeatCount(uint8 count)
{
    repeatCount = count;
}
