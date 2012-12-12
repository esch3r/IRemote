#include "irControl.h"

uint8   frameReceived = 0;

void initializeIrControl()
{
    initializeCb(&buffer0,100,sizeof(uint16));
        
    initializePWM(38000,0.5,1);
        
    setGpioDirection(2,6,GpioDirectionInput);  // TSOP input pin
    enableGpioInterrupt(2,6,GpioInterruptFallingAndRisingEdge, &captureFunction);
    
    initializeTimer3(1000,1E9);
    //connectFunctionTimer3(&myFunc);
    startTimer3();
    
}

void processData()
{
    static uint16 item;
        
    if (frameReceived == 1)
    {
        strcpy(stringBuffer,"Received. ");
        while (getCb(&buffer0,&item) == 0)
        {
            sprintf(stringBuffer,"%s %u",stringBuffer,item);
        }
        sprintf(stringBuffer,"%s\n",stringBuffer);
        printfUart0(stringBuffer);
        frameReceived = 0;
    }
}

void captureFunction()
{
//    static uint32 state;
    static uint8 first = 1;
    const uint16 timeout = 20000; //20ms
    static uint32 lastTime = 0;
    static uint32 currentTime;
    static uint16 timeDiff;
    
    currentTime = getCounterValueTimer3();
    timeDiff = currentTime - lastTime;
    
//    currentTime++;
//    state = readGpio(2,6);
//    if (state != lastState)
//    {
        if (first != 1)
        {
            putCb(&buffer0, &timeDiff);
            if (timeDiff >= timeout)     //detected a timeout => frameReceived
            {
                frameReceived = 1;
                first = 1;
            }
        }
        first = 0;
//        currentTime = 0;
//    }
//    lastState = state;
    lastTime = currentTime;
}
