#include "pwm.h"
int8 initializePWM(uint32 freq,float duty, uint8 pin)
{
    PWM_ENABLE_POWER();
    PWM_SET_CORE_CLK();
    
    LPC_PWM1->PR = 0;
    //PWM_CLEAR_PINSEL();        !!!   // clear pin selection bits 
    switch (pin)
    {
        case 1: PWM_ENABLE_PIN1();break;  //Enable pin 1
    //LPC_PWM->PCR |= (1<<2)		//select double edge for PWM2
    
        default: break;
    }

    //LPC_PWM->MCR |= (1<<6)		//set for PWMR2 enable interrupt
    //LPC_PWM->MCR |= (1<<7)		//reset by match for PWMR2
    LPC_PWM1->LER |= (1<<0);		//Latch enable for Match 0	
    LPC_PWM1->LER |= (1<<1);		//Latch enable for Match 1
    LPC_PWM1->MR0 = (uint32)(PWM_CLK/freq);
    LPC_PWM1->MR1 = (uint32)(duty*(PWM_CLK/freq));	
    
    LPC_PWM1->TCR |= (1<<3) | (1 << 0);     //start PWM mode and counter

    return 0;
}

inline void startPWM(uint8 pin)
{
    LPC_PWM1->PCR |= (1<<9);        //Output enabled for PWM1

    return;
}

inline void stopPwm(uint8 pin)
{
    LPC_PWM1->TC = LPC_PWM1->MR1-1; // Nasty but awesome workaround to set the output to 0
    LPC_PWM1->PCR &= ~(1<<9);       // Output disabled for PWM1
    
    
    return;
}

inline void togglePwm(uint8 pin)
{
    (LPC_PWM1->PCR & (1<<9)) ? stopPwm(1) : startPWM(1);  // Toggles output for PWM1
    
    return;
}
