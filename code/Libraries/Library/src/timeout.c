#include <timeout.h>

#define TIMEOUT_TIMER_KHZ       1
#define TIMEOUT_TIMER_INTERVAL  ~(uint32)0

uint8 timerId;

int8 initializeTimeout(int8 id)
{
    
    if (id == -1)
    {
        timerId = initializeTimer(TIMEOUT_TIMER_KHZ, TIMEOUT_TIMER_INTERVAL);
    }
    else
    {
        timerId = id;
        if (timerId == 0)
        {
            if (initializeTimer0(TIMEOUT_TIMER_KHZ, TIMEOUT_TIMER_INTERVAL) == -1)
            {
                return -1;
            }
        }
        else if (timerId == 1)
        {
            if (initializeTimer1(TIMEOUT_TIMER_KHZ, TIMEOUT_TIMER_INTERVAL) == -1)
            {
                return -1;
            }
        }
        else if (timerId == 2)
        {
            if (initializeTimer2(TIMEOUT_TIMER_KHZ, TIMEOUT_TIMER_INTERVAL) == -1)
            {
                return -1;
            }
        }
        else if (timerId == 3)
        {
            if (initializeTimer3(TIMEOUT_TIMER_KHZ, TIMEOUT_TIMER_INTERVAL) == -1)
            {
                return -1;
            }
        }
        else
        {
            timerId = -1;
        }
    }
    
    if (timerId == -1)
        return -1;
    
    startTimer(timerId);
    
    return 0;
}

uint32 timeoutMsecs(void)
{
    return getCounterValueTimer(timerId);
}

void resetTimeout(void)
{
    resetTimer(timerId);
}
