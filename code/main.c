/**
 * This is a file 
 */
#include <timer.h>
#include <led.h>
#include <uart.h>
#include <adc.h>
#include <dac.h>
#include <math.h>

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
    
    initializeDac();
    initializeAdc(100000,2,0);
    //setDacVolt(2200);

    uint16 f = 1;
    double omega = 2* M_PI*f;
    uint32 t = 0;
    uint32 time = 0;
    
    uint16 adcValue = 50;
    
    for (;;) 
    {
        delayMs(10);
        t++;
        time++;
        if (t > 100)
            t=1;
        setDacValue((uint16)(511.5*sin(omega*((double)t/100.0))+511.5));
        readAdc(&adcValue);
        printfUart0("%i %i\n", time, adcValue);
     }

    return 0 ;
}
