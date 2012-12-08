/**
 * This is a file 
 */
#include <timer.h>
#include <led.h>
#include <uart.h>
#include <adc.h>
#include <dac.h>
#include <gpio.h>
#include <pincon.h>
#include <math.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>

void myFunc();
void testFunc();

uint16  currentTime = 0;
uint32  lastState = (1 << 26);
uint8   frameReceived = 0;
uint8   first = 1;

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
    
    initializeCb(&buffer0,100,sizeof(uint16));
    
    initializeTimer3(1000,5);
    connectFunctionTimer3(&myFunc);
    startTimer3();
    
    //LPC_GPIO1->FIODIR &= ~(1 << 26);
    setGpioDirection(1,26,GpioDirectionInput);
    setPinMode(2,10,PinModeNoPullUpDown);
    setGpioDirection(2,10,GpioDirectionInput);
    enableGpioInterrupt(2,10,GpioInterruptRisingEdge,&testFunc);
    //LPC_PINCON->PINMODE3 |= (0b10 << 20);
    
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
    static uint32 state;
    const uint16 timeout = 2000; //10ms
    
    currentTime++;
    state = readGpio(1,26);//(LPC_GPIO1->FIOPIN & (1 << 26));
    if (state != lastState)
    {
        if (first != 1)
        {
            putCb(&buffer0, &currentTime);
            if (currentTime >= timeout)     //detected a timeout => frameReceived
            {
                frameReceived = 1;
                first = 1;
            }
        }
        first = 0;
        currentTime = 0;
    }
    lastState = state;
}

void testFunc()
{
    toggleLed(1);
}
