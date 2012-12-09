#include <timer.h>

int8 initializeTimer0(uint32 khz, uint32 intervalUs)
{
    if (TIMER0_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER0_ENABLE_CLK();                        /* Enable clock */
    
    TIMER0_RESET();                             /* Reset Timer Counter and Prescale Counter */
    TIMER0_SET_KHZ(khz);                        /* Configure the timer to run with the given frequency */
    TIMER0_SET_INTERVAL_US(intervalUs);         /* Set interval */
    TIMER0_RESET_IRQS();                        /* Reset all interrupts */
    TIMER0_RESET_AND_IRQ_ON_MATCH();            /* Reset the TC and generate an interrupt */
    TIMER0_ENABLE_IRQ();                        /* Enable IRQ for Timer_32_0) */
    TIMER0_SET_IRQ_PRIORITY(LOW_IRQ_PRIORITY);  /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    
    functionPointer0 = NULL;

    return 0;
}

int8 deinitializeTimer0(void)
{
    if (TIMER0_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER0_DISABLE_CLK();                               /* Disable the timer */
    TIMER0_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

int8 initializeCapCom0(uint32 khz, uint8 pin, void (* func)(void))
{
    if (TIMER0_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
        
    TIMER0_ENABLE_CLK();                        /* Enable clock */
    
    TIMER0_RESET();                             /* Reset Timer Counter and Prescale Counter */
    TIMER0_SET_KHZ(khz);                        /* Configure the timer to run with the given frequency */
    TIMER0_SET_INTERVAL_MS(1000);
    TIMER0_RESET_IRQS();                        /* Reset all interrupts */
    TIMER0_ENABLE_IRQ();                        /* Enable IRQ for Timer_32_0) */
    TIMER0_SET_IRQ_PRIORITY(LOW_IRQ_PRIORITY);  /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    
    
    switch (pin)
    {
        case 0: CAPCOM_ENABLE_PIN0();  // Enable pin 0
                CAPCOM0_ENABLE_CAP0(); // set CAP0 events
                break;
        case 1: CAPCOM_ENABLE_PIN1();  //Enable pin 1
                CAPCOM0_ENABLE_CAP0(); // set CAP0 events
                break;
        case 2: CAPCOM_ENABLE_PIN2();  //Enable pin 2
                CAPCOM0_ENABLE_CAP0(); // set CAP0 events
                break;
        case 3: CAPCOM_ENABLE_PIN3();  //Enable pin 3
                CAPCOM0_ENABLE_CAP0(); // set CAP0 events
                break;
        case 4: CAPCOM_ENABLE_PIN4();  //Enable pin 4
                CAPCOM0_ENABLE_CAP1(); // set CAP1 events
                break;
        case 5: CAPCOM_ENABLE_PIN5();  //Enable pin 5
                CAPCOM0_ENABLE_CAP1(); // set CAP1 events
                break;
        case 6: CAPCOM_ENABLE_PIN6();  //Enable pin 6
                CAPCOM0_ENABLE_CAP1(); // set CAP1 events
                break;
        case 7: CAPCOM_ENABLE_PIN7();  //Enable pin 7
                CAPCOM0_ENABLE_CAP1(); // set CAP1 events
                break;
        default: break;
    }
    
    functionPointer0 = func;  
    
    return 0;
}

int8 initializeCapCom3(uint32 khz, uint8 pin, void (* func)(void))
{
    //if (TIMER3_RUNNING())                       /* if timer is already in use return -1 */
    //    return -1;
        
    TIMER3_ENABLE_CLK();                        /* Enable clock */
    
    CAPCOM_ENABLE_PIN5();  // Enable pin 0
    
    TIMER3_SET_KHZ(khz);                        /* Configure the timer to run with the given frequency */
    TIMER3_RESET();                             /* Reset Timer Counter and Prescale Counter */
    //TIMER3_SET_INTERVAL_MS(1000);
    //TIMER3_RESET_AND_IRQ_ON_MATCH();
    //TIMER3_RESET_ON_MATCH();            /* Reset the TC and generate an interrupt */
    //TIMER3_RESET_IRQS();                        /* Reset all interrupts */
    //TIMER3_SET_IRQ_PRIORITY(LOW_IRQ_PRIORITY);  /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    CAPCOM3_ENABLE_CAP0();
    CAPCOM3_ENABLE_CAP1(); // set CAP0 events
    TIMER3_START(); /* start timer */
    
    
   /* switch (pin)
    {
        case 0: CAPCOM_ENABLE_PIN0();  // Enable pin 0
                CAPCOM3_ENABLE_CAP0(); // set CAP0 events
                break;
        case 1: CAPCOM_ENABLE_PIN1();  //Enable pin 1
                CAPCOM3_ENABLE_CAP0(); // set CAP0 events
                break;
        case 2: CAPCOM_ENABLE_PIN2();  //Enable pin 2
                CAPCOM3_ENABLE_CAP0(); // set CAP0 events
                break;
        case 3: CAPCOM_ENABLE_PIN3();  //Enable pin 3
                CAPCOM3_ENABLE_CAP0(); // set CAP0 events
                break;
        case 4: CAPCOM_ENABLE_PIN4();  //Enable pin 4
                CAPCOM3_ENABLE_CAP1(); // set CAP1 events
                break;
        case 5: CAPCOM_ENABLE_PIN5();  //Enable pin 5
                CAPCOM3_ENABLE_CAP1(); // set CAP1 events
                break;
        case 6: CAPCOM_ENABLE_PIN6();  //Enable pin 6
                CAPCOM3_ENABLE_CAP1(); // set CAP1 events
                break;
        case 7: CAPCOM_ENABLE_PIN7();  //Enable pin 7
                CAPCOM3_ENABLE_CAP1(); // set CAP1 events
                break;
        default: break;
    }*/
    
    NVIC_SetPriority(TIMER3_IRQn, ((0x01<<3)|0x01));
    TIMER3_ENABLE_IRQ();                        /* Enable IRQ for Timer_32_0) */
    
    
    functionPointer3 = func;  
    
    return 0;
}

void startTimer0(void)
{
    TIMER0_START(); /* start timer */
    
    return;
}

void stopTimer0(void)
{
    TIMER0_STOP(); /* stop timer */
    
    return;
}

void connectFunctionTimer0(void (* func)(void))
{
    functionPointer0 = func;
    
    return;
}

void setIntervalUsTimer0(uint32 us)
{
    uint8 wasRunning = TIMER0_RUNNING();
    
    TIMER0_RESET();
    TIMER0_SET_INTERVAL_US(us);
    
    if (wasRunning)
        TIMER0_START();
    
    return;
}

void setIntervalMsTimer0(uint32 ms)
{
    uint8 wasRunning = TIMER0_RUNNING();
    
    TIMER0_RESET();
    TIMER0_SET_INTERVAL_MS(ms);
    
    if (wasRunning)
        TIMER0_START();
    
    return;
}

void setPriorityTimer0(uint8 priority)
{
    TIMER0_SET_IRQ_PRIORITY(priority);
    
    return;
}

int8 delayUsTimer0(uint32 us)
{
    if (TIMER0_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER0_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER0_RESET();                                     /* Reset the timer */
    TIMER0_SET_PRESCALER(OPTIMIZE_PRESCALER_US());      /* Set prescaler to 2 */
    TIMER0_SET_INTERVAL_US(us);                         /* Set timer interval */
    TIMER0_RESET_IRQS();                                /* Reset all interrupts */
    TIMER0_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER0_SET_IRQ_PRIORITY(HIGH_IRQ_PRIORITY);         /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER0_STOP_AND_IRQ_ON_MATCH();                     /* Stop timer on match */
    
    functionPointer0 = NULL;
    
    TIMER0_START();                                     /* Start timer */
    
    do {
        PROZESSOR_SLEEP();                              /* set prozessor into sleep mode */
        PROZESSOR_WAIT_FOR_IRQ();                       /* Wait for interrupt */
    } 
    while(TIMER0_RUNNING());                            /* Wait until the timer has finished */ 
    
    TIMER0_DISABLE_CLK();                               /* Disable the timer again */
    TIMER0_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

int8 delayMsTimer0(uint32 ms)
{
    if (TIMER0_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER0_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER0_RESET();                                     /* Reset the timer */
    TIMER0_SET_PRESCALER(OPTIMIZE_PRESCALER_MS());      /* Set prescaler to 2 */
    TIMER0_SET_INTERVAL_MS(ms);                         /* Set timer interval */
    TIMER0_RESET_IRQS();                                /* Reset all interrupts */
    TIMER0_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER0_SET_IRQ_PRIORITY(HIGH_IRQ_PRIORITY);         /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER0_STOP_AND_IRQ_ON_MATCH();                     /* Stop timer on match */
    
    functionPointer0 = NULL;
    
    TIMER0_START();                                     /* Start timer */
    
    do {
        PROZESSOR_SLEEP();                              /* set prozessor into sleep mode */
        PROZESSOR_WAIT_FOR_IRQ();                       /* Wait for interrupt */
    } 
    while(TIMER0_RUNNING());                            /* Wait until the timer has finished */ 
    
    TIMER0_DISABLE_CLK();                               /* Disable the timer again */
    TIMER0_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

int8 singleShotTimer0(uint32 ms, void (* func)(void))
{
    if (TIMER0_RUNNING())                       /* if timer is already in use return 1 */
        return -1;
    
    TIMER0_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER0_RESET();                                     /* Reset the timer */
    TIMER0_SET_PRESCALER(OPTIMIZE_PRESCALER_MS());      /* Set prescaler to 0 */
    TIMER0_SET_INTERVAL_MS(ms);                         /* Set timer interval */
    TIMER0_RESET_IRQS();                                /* Reset all interrupts */
    TIMER0_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER0_SET_IRQ_PRIORITY(LOW_IRQ_PRIORITY);          /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER0_RESET_STOP_AND_IRQ_ON_MATCH();               /* Reset the TC and generate an interrupt */
    
    functionPointer0 = func;
        
    TIMER0_START();                                     /* Start timer */
    
    return 0;
}

inline uint32 getCounterValueTimer0()
{
    return TIMER0_COUNTER_VALUE();
}

#if (TIMER0_IRQ_ENABLED == 1)
void TIMER0_IRQHANDLER()
{
    TIMER0_RESET_IRQS();                    /* clear interrupt flag */

    if (functionPointer0 != NULL)
        (*functionPointer0)();
    
    return;
}
#endif

int8 initializeTimer1(uint32 khz, uint32 intervalUs)
{
    if (TIMER1_RUNNING())                       /* if timer is already in use return 1 */
        return -1;
    
    TIMER1_ENABLE_CLK();                        /* Enable clock */
    
    TIMER1_RESET();                             /* Reset Timer Counter and Prescale Counter */
    TIMER1_SET_KHZ(khz);                        /* Configure the timer to run with the given frequency */
    TIMER1_SET_INTERVAL_US(intervalUs);         /* Set interval */
    TIMER1_RESET_IRQS();                        /* Reset all interrupts */
    TIMER1_RESET_AND_IRQ_ON_MATCH();            /* Reset the TC and generate an interrupt */
    TIMER1_ENABLE_IRQ();                        /* Enable IRQ for Timer_32_0) */
    TIMER1_SET_IRQ_PRIORITY(LOW_IRQ_PRIORITY);  /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    
    functionPointer1 = NULL;

    return 0;
}

int8 deinitializeTimer1(void)
{
    if (TIMER1_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER1_DISABLE_CLK();                               /* Disable the timer */
    TIMER1_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

void startTimer1(void)
{
    TIMER1_START(); /* start timer */
    
    return;
}

void stopTimer1(void)
{
    TIMER1_STOP(); /* stop timer */
    
    return;
}

void connectFunctionTimer1(void (* func)(void))
{
    functionPointer1 = func;
    
    return;
}

void setIntervalUsTimer1(uint32 us)
{
    uint8 wasRunning = TIMER1_RUNNING();
    
    TIMER1_RESET();
    TIMER1_SET_INTERVAL_US(us);
    
    if (wasRunning)
        TIMER1_START();
    
    return;
}

void setIntervalMsTimer1(uint32 ms)
{
    uint8 wasRunning = TIMER1_RUNNING();
    
    TIMER1_RESET();
    TIMER1_SET_INTERVAL_MS(ms);
    
    if (wasRunning)
        TIMER1_START();
    
    return;
}

void setPriorityTimer1(uint8 priority)
{
    TIMER1_SET_IRQ_PRIORITY(priority);
    
    return;
}

int8 delayUsTimer1(uint32 us)
{
    if (TIMER1_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER1_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER1_RESET();                                     /* Reset the timer */
    TIMER1_SET_PRESCALER(OPTIMIZE_PRESCALER_US());      /* Set prescaler to 2 */
    TIMER1_SET_INTERVAL_US(us);                         /* Set timer interval */
    TIMER1_RESET_IRQS();                                /* Reset all interrupts */
    TIMER1_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER1_SET_IRQ_PRIORITY(HIGH_IRQ_PRIORITY);         /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER1_STOP_AND_IRQ_ON_MATCH();                     /* Stop timer on match */
    
    functionPointer1 = NULL;
    
    TIMER1_START();                                     /* Start timer */
    
    do {
        PROZESSOR_SLEEP();                              /* set prozessor into sleep mode */
        PROZESSOR_WAIT_FOR_IRQ();                       /* Wait for interrupt */
    } 
    while(TIMER1_RUNNING());                            /* Wait until the timer has finished */ 
    
    TIMER1_DISABLE_CLK();                               /* Disable the timer again */
    TIMER1_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

int8 delayMsTimer1(uint32 ms)
{
    if (TIMER1_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER1_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER1_RESET();                                     /* Reset the timer */
    TIMER1_SET_PRESCALER(OPTIMIZE_PRESCALER_MS());      /* Set prescaler to 2 */
    TIMER1_SET_INTERVAL_MS(ms);                         /* Set timer interval */
    TIMER1_RESET_IRQS();                                /* Reset all interrupts */
    TIMER1_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER1_SET_IRQ_PRIORITY(HIGH_IRQ_PRIORITY);         /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER1_STOP_AND_IRQ_ON_MATCH();                     /* Stop timer on match */
    
    functionPointer1 = NULL;
    
    TIMER1_START();                                     /* Start timer */
    
    do {
        PROZESSOR_SLEEP();                              /* set prozessor into sleep mode */
        PROZESSOR_WAIT_FOR_IRQ();                       /* Wait for interrupt */
    } 
    while(TIMER1_RUNNING());                            /* Wait until the timer has finished */ 
    
    TIMER1_DISABLE_CLK();                               /* Disable the timer again */
    TIMER1_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

int8 singleShotTimer1(uint32 ms, void (* func)(void))
{
    if (TIMER1_RUNNING())                       /* if timer is already in use return 1 */
        return -1;
    
    TIMER1_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER1_RESET();                                     /* Reset the timer */
    TIMER1_SET_PRESCALER(OPTIMIZE_PRESCALER_MS());      /* Set prescaler to 0 */
    TIMER1_SET_INTERVAL_MS(ms);                         /* Set timer interval */
    TIMER1_RESET_IRQS();                                /* Reset all interrupts */
    TIMER1_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER1_SET_IRQ_PRIORITY(LOW_IRQ_PRIORITY);          /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER1_RESET_STOP_AND_IRQ_ON_MATCH();               /* Reset the TC and generate an interrupt */
    
    functionPointer1 = func;
        
    TIMER1_START();                                     /* Start timer */
    
    return 0;
}

inline uint32 getCounterValueTimer1()
{
    return TIMER1_COUNTER_VALUE();
}

#if (TIMER1_IRQ_ENABLED == 1)
void TIMER1_IRQHANDLER()
{
    TIMER1_RESET_IRQS();                    /* clear interrupt flag */

    if (functionPointer1 != NULL)
        (*functionPointer1)();
    
    return;
}
#endif

int8 initializeTimer2(uint32 khz, uint32 intervalUs)
{
    if (TIMER2_RUNNING())                       /* if timer is already in use return 1 */
        return -1;
    
    TIMER2_ENABLE_CLK();                        /* Enable clock */
    
    TIMER2_RESET();                             /* Reset Timer Counter and Prescale Counter */
    TIMER2_SET_KHZ(khz);                        /* Configure the timer to run with the given frequency */
    TIMER2_SET_INTERVAL_US(intervalUs);         /* Set interval */
    TIMER2_RESET_IRQS();                        /* Reset all interrupts */
    TIMER2_RESET_AND_IRQ_ON_MATCH();            /* Reset the TC and generate an interrupt */
    TIMER2_ENABLE_IRQ();                        /* Enable IRQ for Timer_32_0) */
    TIMER2_SET_IRQ_PRIORITY(LOW_IRQ_PRIORITY);  /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    
    functionPointer2 = NULL;

    return 0;
}

int8 deinitializeTimer2(void)
{
    if (TIMER2_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER2_DISABLE_CLK();                               /* Disable the timer */
    TIMER2_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

void startTimer2(void)
{
    TIMER2_START(); /* start timer */
    
    return;
}

void stopTimer2(void)
{
    TIMER2_STOP(); /* stop timer */
    
    return;
}

void connectFunctionTimer2(void (* func)(void))
{
    functionPointer2 = func;
    
    return;
}

void setIntervalUsTimer2(uint32 us)
{
    uint8 wasRunning = TIMER2_RUNNING();
    
    TIMER2_RESET();
    TIMER2_SET_INTERVAL_US(us);
    
    if (wasRunning)
        TIMER2_START();
    
    return;
}

void setIntervalMsTimer2(uint32 ms)
{
    uint8 wasRunning = TIMER2_RUNNING();
    
    TIMER2_RESET();
    TIMER2_SET_INTERVAL_MS(ms);
    
    if (wasRunning)
        TIMER2_START();
    
    return;
}

void setPriorityTimer2(uint8 priority)
{
    TIMER2_SET_IRQ_PRIORITY(priority);
    
    return;
}

int8 delayUsTimer2(uint32 us)
{
    if (TIMER2_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER2_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER2_RESET();                                     /* Reset the timer */
    TIMER2_SET_PRESCALER(OPTIMIZE_PRESCALER_US());      /* Set prescaler to 2 */
    TIMER2_SET_INTERVAL_US(us);                         /* Set timer interval */
    TIMER2_RESET_IRQS();                                /* Reset all interrupts */
    TIMER2_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER2_SET_IRQ_PRIORITY(HIGH_IRQ_PRIORITY);         /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER2_STOP_AND_IRQ_ON_MATCH();                     /* Stop timer on match */
    
    functionPointer2 = NULL;
    
    TIMER2_START();                                     /* Start timer */
    
    do {
        PROZESSOR_SLEEP();                              /* set prozessor into sleep mode */
        PROZESSOR_WAIT_FOR_IRQ();                       /* Wait for interrupt */
    } 
    while(TIMER2_RUNNING());                            /* Wait until the timer has finished */ 
    
    TIMER2_DISABLE_CLK();                               /* Disable the timer again */
    TIMER2_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

int8 delayMsTimer2(uint32 ms)
{
    if (TIMER2_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER2_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER2_RESET();                                     /* Reset the timer */
    TIMER2_SET_PRESCALER(OPTIMIZE_PRESCALER_MS());      /* Set prescaler to 2 */
    TIMER2_SET_INTERVAL_MS(ms);                         /* Set timer interval */
    TIMER2_RESET_IRQS();                                /* Reset all interrupts */
    TIMER2_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER2_SET_IRQ_PRIORITY(HIGH_IRQ_PRIORITY);         /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER2_STOP_AND_IRQ_ON_MATCH();                     /* Stop timer on match */
    
    functionPointer2 = NULL;
    
    TIMER2_START();                                     /* Start timer */
    
    do {
        PROZESSOR_SLEEP();                              /* set prozessor into sleep mode */
        PROZESSOR_WAIT_FOR_IRQ();                       /* Wait for interrupt */
    } 
    while(TIMER2_RUNNING());                            /* Wait until the timer has finished */ 
    
    TIMER2_DISABLE_CLK();                               /* Disable the timer again */
    TIMER2_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

int8 singleShotTimer2(uint32 ms, void (* func)(void))
{
    if (TIMER2_RUNNING())                       /* if timer is already in use return 1 */
        return -1;
    
    TIMER2_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER2_RESET();                                     /* Reset the timer */
    TIMER2_SET_PRESCALER(OPTIMIZE_PRESCALER_MS());      /* Set prescaler to 0 */
    TIMER2_SET_INTERVAL_MS(ms);                         /* Set timer interval */
    TIMER2_RESET_IRQS();                                /* Reset all interrupts */
    TIMER2_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER2_SET_IRQ_PRIORITY(LOW_IRQ_PRIORITY);          /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER2_RESET_STOP_AND_IRQ_ON_MATCH();               /* Reset the TC and generate an interrupt */
    
    functionPointer2 = func;
        
    TIMER2_START();                                     /* Start timer */
    
    return 0;
}

inline uint32 getCounterValueTimer2()
{
    return TIMER2_COUNTER_VALUE();
}

#if (TIMER2_IRQ_ENABLED == 1)
void TIMER2_IRQHANDLER()
{
    TIMER2_RESET_IRQS();                    /* clear interrupt flag */

    if (functionPointer2 != NULL)
        (*functionPointer2)();
    
    return;
}
#endif

int8 initializeTimer3(uint32 khz, uint32 intervalUs)
{
    if (TIMER3_RUNNING())                       /* if timer is already in use return 1 */
        return -1;
    
    TIMER3_ENABLE_CLK();                        /* Enable clock */
    
    TIMER3_RESET();                             /* Reset Timer Counter and Prescale Counter */
    TIMER3_SET_KHZ(khz);                        /* Configure the timer to run with the given frequency */
    TIMER3_SET_INTERVAL_US(intervalUs);         /* Set interval */
    TIMER3_RESET_IRQS();                        /* Reset all interrupts */
    TIMER3_RESET_AND_IRQ_ON_MATCH();            /* Reset the TC and generate an interrupt */
    TIMER3_ENABLE_IRQ();                        /* Enable IRQ for Timer_32_0) */
    TIMER3_SET_IRQ_PRIORITY(LOW_IRQ_PRIORITY);  /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    
    functionPointer3 = NULL;

    return 0;
}

int8 deinitializeTimer3(void)
{
    if (TIMER3_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER3_DISABLE_CLK();                               /* Disable the timer */
    TIMER3_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

void startTimer3(void)
{
    TIMER3_START(); /* start timer */
    
    return;
}

void stopTimer3(void)
{
    TIMER3_STOP(); /* stop timer */
    
    return;
}

void connectFunctionTimer3(void (* func)(void))
{
    functionPointer3 = func;
    
    return;
}

void setIntervalUsTimer3(uint32 us)
{
    uint8 wasRunning = TIMER3_RUNNING();
    
    TIMER3_RESET();
    TIMER3_SET_INTERVAL_US(us);
    
    if (wasRunning)
        TIMER3_START();
    
    return;
}

void setIntervalMsTimer3(uint32 ms)
{
    uint8 wasRunning = TIMER3_RUNNING();
    
    TIMER3_RESET();
    TIMER3_SET_INTERVAL_MS(ms);
    
    if (wasRunning)
        TIMER3_START();
    
    return;
}

void setPriorityTimer3(uint8 priority)
{
    TIMER3_SET_IRQ_PRIORITY(priority);
    
    return;
}

int8 delayUsTimer3(uint32 us)
{
    if (TIMER3_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER3_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER3_RESET();                                     /* Reset the timer */
    TIMER3_SET_PRESCALER(OPTIMIZE_PRESCALER_US());      /* Set prescaler to 2 */
    TIMER3_SET_INTERVAL_US(us);                         /* Set timer interval */
    TIMER3_RESET_IRQS();                                /* Reset all interrupts */
    TIMER3_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER3_SET_IRQ_PRIORITY(HIGH_IRQ_PRIORITY);         /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER3_STOP_AND_IRQ_ON_MATCH();                     /* Stop timer on match */
    
    functionPointer3 = NULL;
    
    TIMER3_START();                                     /* Start timer */
    
    do {
        PROZESSOR_SLEEP();                              /* set prozessor into sleep mode */
        PROZESSOR_WAIT_FOR_IRQ();                       /* Wait for interrupt */
    } 
    while(TIMER3_RUNNING());                            /* Wait until the timer has finished */ 
    
    TIMER3_DISABLE_CLK();                               /* Disable the timer again */
    TIMER3_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

int8 delayMsTimer3(uint32 ms)
{
    if (TIMER3_RUNNING())                       /* if timer is already in use return -1 */
        return -1;
    
    TIMER3_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER3_RESET();                                     /* Reset the timer */
    TIMER3_SET_PRESCALER(OPTIMIZE_PRESCALER_MS());      /* Set prescaler to 2 */
    TIMER3_SET_INTERVAL_MS(ms);                         /* Set timer interval */
    TIMER3_RESET_IRQS();                                /* Reset all interrupts */
    TIMER3_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER3_SET_IRQ_PRIORITY(HIGH_IRQ_PRIORITY);         /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER3_STOP_AND_IRQ_ON_MATCH();                     /* Stop timer on match */
    
    functionPointer3 = NULL;
    
    TIMER3_START();                                     /* Start timer */
    
    do {
        PROZESSOR_SLEEP();                              /* set prozessor into sleep mode */
        PROZESSOR_WAIT_FOR_IRQ();                       /* Wait for interrupt */
    } 
    while(TIMER3_RUNNING());                            /* Wait until the timer has finished */ 
    
    TIMER3_DISABLE_CLK();                               /* Disable the timer again */
    TIMER3_DISABLE_IRQ();                               /* Disable timer interrupt */
    
    return 0;
}

int8 singleShotTimer3(uint32 ms, void (* func)(void))
{
    if (TIMER3_RUNNING())                       /* if timer is already in use return 1 */
        return -1;
    
    TIMER3_ENABLE_CLK();                                /* Enable clock for timer */
    
    TIMER3_RESET();                                     /* Reset the timer */
    TIMER3_SET_PRESCALER(OPTIMIZE_PRESCALER_MS());      /* Set prescaler to 0 */
    TIMER3_SET_INTERVAL_MS(ms);                         /* Set timer interval */
    TIMER3_RESET_IRQS();                                /* Reset all interrupts */
    TIMER3_ENABLE_IRQ();                                /* Enable timer interrupt */
    TIMER3_SET_IRQ_PRIORITY(LOW_IRQ_PRIORITY);          /* Set interrupt priority to allow delays in interrupt routine (not recommended) */
    TIMER3_RESET_STOP_AND_IRQ_ON_MATCH();               /* Reset the TC and generate an interrupt */
    
    functionPointer3 = func;
        
    TIMER3_START();                                     /* Start timer */
    
    return 0;
}

inline uint32 getCounterValueTimer3()
{
    return TIMER3_COUNTER_VALUE();
}

#if (TIMER3_IRQ_ENABLED == 1)
void TIMER3_IRQHANDLER()
{
    TIMER3_RESET_IRQS();                    // clear interrupt flag 
    
    if (functionPointer3 != NULL)
        (*functionPointer3)();
    
    return;
}
#endif

int8 initializeTimer(uint32 khz, uint32 intervalUs)
{
   if (!TIMER0_RUNNING())
   {
      initializeTimer0(khz, intervalUs);
      return 0;
   }
   else if (!TIMER1_RUNNING())
   {
       initializeTimer1(khz, intervalUs);
       return 1;
   }
   else if (!TIMER2_RUNNING())
   {
       initializeTimer2(khz, intervalUs);
       return 2;
   }
   else if (!TIMER3_RUNNING())
   {
       initializeTimer3(khz, intervalUs);
       return 3;
   }
   else
       return -1;
}

int8 deinitializeTimer(uint8 id)
{
    if (id == TIMER0)
    {
        return deinitializeTimer0();
    }
    else if (id == TIMER1)
    {
        return deinitializeTimer1();
    }
    else if (id == TIMER2)
    {
        return deinitializeTimer2();
    }
    else if (id == TIMER3)
    {
        return deinitializeTimer3();
    }
    return 0;
}

void startTimer(uint8 id)
{
    if (id == TIMER0)
    {
        return startTimer0();
    }
    else if (id == TIMER1)
    {
        return startTimer1();
    }
    else if (id == TIMER2)
    {
        return startTimer2();
    }
    else if (id == TIMER3)
    {
        return startTimer3();
    }
    return;
}

void stopTimer(uint8 id)
{
    if (id == TIMER0)
    {
        return stopTimer0();
    }
    else if (id == TIMER1)
    {
        return stopTimer1();
    }
    else if (id == TIMER2)
    {
        return stopTimer2();
    }
    else if (id == TIMER3)
    {
        return stopTimer3();
    }
    return;
}

void setTimerIntervalUs(uint8 id, uint32 us)
{
    if (id == TIMER0)
    {
        return setIntervalUsTimer0(us);
    }
    else if (id == TIMER1)
    {
        return setIntervalUsTimer1(us);
    }
    else if (id == TIMER2)
    {
        return setIntervalUsTimer2(us);
    }
    else if (id == TIMER3)
    {
        return setIntervalUsTimer3(us);
    }
    return;
}

void setTimerIntervalMs(uint8 id, uint32 ms)
{
    if (id == TIMER0)
    {
        return setIntervalMsTimer0(ms);
    }
    else if (id == TIMER1)
    {
        return setIntervalMsTimer1(ms);
    }
    else if (id == TIMER2)
    {
        return setIntervalMsTimer2(ms);
    }
    else if (id == TIMER3)
    {
        return setIntervalMsTimer3(ms);
    }
    return;
}

void connectTimerFunction(uint8 id, void (* func)(void))
{
    if (id == TIMER0)
    {
        return connectFunctionTimer0(func);
    }
    else if (id == TIMER1)
    {
        return connectFunctionTimer1(func);
    }
    else if (id == TIMER2)
    {
        return connectFunctionTimer2(func);
    }
    else if (id == TIMER3)
    {
        return connectFunctionTimer3(func);
    }
    return;
}

void setTimerPriority(uint8 id, uint8 priority)
{
    if (id == TIMER0)
    {
        setPriorityTimer0(priority);
    }
    else if (id == TIMER1)
    {
        setPriorityTimer1(priority);
    }
    else if (id == TIMER2)
    {
        setPriorityTimer2(priority);
    }
    else if (id == TIMER3)
    {
        setPriorityTimer3(priority);
    }
    return;
}

int8 delayUs(uint32 us)
{
   if (delayUsTimer0(us) == 0)
   {
      return 0;
   }
   else if (delayUsTimer1(us) == 0)
   {
       return 1;
   }
   else if (delayUsTimer2(us) == 0)
   {
       return 2;
   }
   else if (delayUsTimer3(us) == 0)
   {
       return 3;
   }
   else
       return -1;
}

int8 delayMs(uint32 ms)
{
    if (!TIMER0_RUNNING())
   {
      delayMsTimer0(ms);
      return 0;
   }
   else if (!TIMER1_RUNNING())
   {
       delayMsTimer1(ms);
       return 1;
   }
   else if (!TIMER2_RUNNING())
   {
       delayMsTimer2(ms);
       return 2;
   }
   else if (!TIMER3_RUNNING())
   {
       delayMsTimer3(ms);
       return 3;
   }
   else
       return -1;
}

int8 singleShotTimer(uint32 ms, void (* func)(void))
{
    if (!TIMER0_RUNNING())
   {
      singleShotTimer0(ms, func);
      return 0;
   }
   else if (!TIMER1_RUNNING())
   {
       singleShotTimer1(ms, func);
       return 1;
   }
   else if (!TIMER2_RUNNING())
   {
       singleShotTimer2(ms, func);
       return 2;
   }
   else if (!TIMER3_RUNNING())
   {
       singleShotTimer3(ms, func);
       return 3;
   }
   else
       return -1;
}
