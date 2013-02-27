#include "irControl.h"

uint8   frameReceived = 0;
uint8   firstCapture = 1;
uint8   currentPosition = 0;
uint8   commandRunning = 0;
CircularBuffer buffer0;

uint8   repeatCount = 4;
uint8   currentRepeatCount = 0;

uint32 receiveTimeout = 20000;
uint32 sendTimeout = 100000;

IrCommand *tmpCommand = NULL;

int8 initializeIrControl(void)
{
    if (initializeCb(&buffer0,IR_MAX_TRANSITIONS,sizeof(uint16)) == -1)
        return -1;
    
    setGpioDirection(1, 18, GpioDirectionOutput);
    setPinMode(1,18, PinModePullDown);
    
    if (initializePWM(38000,0.5,IR_PWM_PIN) == -1)
        return -1;
    
    //startPWM(0);
    
        
    setGpioDirection(IR_CAPTURE_PORT, IR_CAPTURE_PIN, GpioDirectionInput); // TSOP input pin

    if (Timer_initialize(Timer3, 1000,1E9) == -1)
        return -1;
    
    return 0;
}

void outputCommand(IrCommand *command)
{
    if (frameReceived == 1)
    {
        uint16 commandSize = sizeof(IrCommand);
        uint16 i;
        
        printfData("*DATA");
        for (i = 0; i < commandSize; i++)
        {
            printfData("%02x",(uint8)(((char*)command)[i]));
        }
        printfData("\r");
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
    if (tmpCommand != NULL)
        freeIrCommand(tmpCommand);
    
    tmpCommand = createIrCommand();
    frameReceived = 0;
    firstCapture = 1;
        
    Timer_setIntervalMs(Timer3, 1E6);
    Timer_connectFunction(Timer3, NULL);
    enableGpioInterrupt(IR_CAPTURE_PORT, IR_CAPTURE_PIN, GpioInterruptFallingAndRisingEdge, &captureFunction);
    //startTimer3();
    Timer_reset(Timer3);
}

void stopIrCapture(void)
{
    Timer_start(Timer3);
    disableGpioInterrupt(IR_CAPTURE_PORT, IR_CAPTURE_PIN);
}

void captureFunction(void)
{
    static uint16 timeDiff;
    
    //if (firstCapture != 1)                     // Check for first run, ignore value
    //{
        timeDiff = (uint16) Timer_counterValue(Timer3);

        if (timeDiff >= receiveTimeout)     // Detected a timeout => frameReceived
        {
            if ((saveIrFrame(&buffer0, tmpCommand) == -1) || (frameReceived != 1))
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
    /*}
    else 
    {
        firstCapture = 0;
    }*/
    
    Timer_reset(Timer3);                      // Reset the timer
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
    
    Timer_connectFunction(Timer3, &runFunction);
    Timer_setIntervalUs(Timer3, 10);
    Timer_start(Timer3);
}

void stopIrCommand(void )
{
    Timer_start(Timer3);
    stopPwm(IR_PWM_PIN);
    currentRepeatCount++;
    
    if (currentRepeatCount == repeatCount)  // Repeat the command a few times
    {
        commandRunning = 0;
        currentRepeatCount = 0;
    }
    else
    {
        currentPosition = 0;
        Timer_setIntervalUs(Timer3, sendTimeout);    // Timeout between commands
        Timer_start(Timer3);
    }
}

void runFunction(void )
{
    togglePwm(IR_PWM_PIN);
    if (currentPosition < tmpCommand->length)
    {
        Timer_setIntervalUs(Timer3, tmpCommand->data[currentPosition]);

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

void setIrReceiveTimeout(uint32 timeout)
{
    receiveTimeout = timeout;
}

void setIrSendTimeout(uint32 timeout)
{
    sendTimeout = timeout;
}

void setIrRepeatCount(uint8 count)
{
    repeatCount = count;
}
