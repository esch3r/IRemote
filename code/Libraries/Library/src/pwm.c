#include "pwm.h"
int8 initializePWM(uint32 freq,float duty, uint8 pin)
{
    PWM_ENABLE_POWER();
    PWM_SET_CORE_CLK();
    
    LPC_PWM1->PR = 0;

    switch (pin)
    {
        case 0: PWM_ENABLE_PIN0();break;  //Enable PWM1.1 on P2.0
        case 1: PWM_ENABLE_PIN1();break;  //Enable PWM1.2 on P2.1
        case 2: PWM_ENABLE_PIN2();break;  //Enable PWM1.3 on P2.2
        case 3: PWM_ENABLE_PIN3();break;  //Enable PWM1.4 on P2.3
        case 4: PWM_ENABLE_PIN4();break;  //Enable PWM1.5 on P2.4
        case 5: PWM_ENABLE_PIN5();break;  //Enable PWM1.6 on P2.5
    
        default: break;
    }

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
