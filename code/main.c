/**
 * This is a file 
 */
#include <timer.h>
#include <led.h>
#include <uart.h>
#include <adc.h>
#include <dac.h>
#include <math.h>
#include <sys/types.h>
#include <stdio.h>

void myFunc();

uint16 currentTime = 0;
uint32  lastState = 0;

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
    
    initializeTimer3(200,10);
    connectFunctionTimer3(&myFunc);
    startTimer3();
    
    LPC_GPIO1->FIODIR &= ~(1 << 26);
    
    uint16 item;
    uint16 n;
    for (;;) 
    {
        delayMs(1000);
        n = 0;
        while (getCb(&buffer0,&item) == 0)
        {
            printfUart0("%u\n",item);
            //n += snprintf(&stringBuffer[n],1000-n,"%u ",item);
        }
        //printfUart0(stringBuffer);
     }

    return 0 ;
}

void myFunc()
{
    currentTime++;
    static uint32 state;
    state = (LPC_GPIO1->FIOPIN & (1 << 26));
    if (state != lastState)
    {
        //printfUart0("%u %u\n", currentTime, state);
        putCb(&buffer0, &currentTime);
        currentTime = 0;
    }
    lastState = state;
}

