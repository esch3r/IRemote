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
    
    for (;;) 
    {
        delayMs(10);

     }

    return 0 ;
}
