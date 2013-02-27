/**
 * This is the main file
 */

#include <timer.h>
#include "iremote.h"

int main(void)
{   
    initializeHardware();
    initializeVariables();
    
    uint8 ledTiming = 0;
    
    for (;;) 
    {
            
        mainTask();
        buttonTask();
        
        if (ledTiming == 200)   // led have to be changed only every 1000ms
        {
            ledTask();
            ledTiming = 0;
        }
        ledTiming++;
        
        Timer_delayMs(5);
     }

    return 0 ;
}
