/**
 * This is a file 
 */
#include <timer.h>
#include <led.h>
#include <uart.h>
#include <gpio.h>
#include <pincon.h>
#include <pwm.h>
#include <stdio.h>
#include <string.h>

void myFunc();
void testFunc();

//uint16  currentTime = 0;
uint32  lastState = (1 << 26);
uint8   frameReceived = 0;
uint8   first = 1;

//uint32 lastTime = 0;

CircularBuffer buffer0;
char stringBuffer[1000];

int main(void)
{   
    initializeLeds();
    clearAllLeds();
    
    //Program started notifier
    delayMs(500);
    blinkLed3(1);
    delayMs(500);
    
    initializeUart0(115200);                    // Init the UART
    printfUart0("Welcome to my Program!\n");    // Send a welcome message
    
    initializePWM(38000,0.5,1);
    
    initializeCb(&buffer0,100,sizeof(uint16));
    
    initializeTimer3(1000,1E9);
    //connectFunctionTimer3(&myFunc);
    startTimer3();
    
    setGpioDirection(2,6,GpioDirectionInput);  // TSOP input pin
    enableGpioInterrupt(2,6,GpioInterruptFallingAndRisingEdge, &myFunc);
    
    setPinMode(2,10,PinModeNoPullUpDown);       // button3
    setGpioDirection(2,10,GpioDirectionInput);
    enableGpioInterrupt(2,10,GpioInterruptRisingEdge,&testFunc);
    
    setGpioDirection(0,9,GpioDirectionOutput);   // Output pin for testing purposes
    
    uint16 item;
    for (;;) 
    {
        delayMs(10);
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

    return 0 ;
}

void myFunc()
{
//    static uint32 state;
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

void testFunc()
{
    startPWM(1);
}
