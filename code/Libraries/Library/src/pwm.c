#include "pwm.h"
#include <LPC17xx.h>
int8 initializePWM(uint32 freq,float duty, uint8 pin){

	PWM_ENABLE_POWER();
	PWM_SET_CORE_CLK();
	
	LPC_PWM->PR = 0;
	//PWM_CLEAR_PINSEL();        !!!   // clear pin selection bits 
    switch (pin)
    {
        case 1: PWM_ENABLE_PIN1();break;  //Enable pin 1
//LPC_PWM->PCR |= (1<<2)		//select double edge for PWM2
    
        default: break;
    }

	//LPC_PWM->MCR |= (1<<6)		//set for PWMR2 enable interrupt
	//LPC_PWM->MCR |= (1<<7)		//reset by match for PWMR2
	LPC_PWM->LER |= (1<<0);		//Latch enable for Match 0	
	LPC_PWM->LER |= (1<<1);		//Latch enable for Match 1
	LPC_PWM->MCR0 =(PWM_CLK/freq);
	LPC_PWM->MCR1 = (duty*PWM_CLK/freq);	
	

return 0;
}
int8 startPWM(uint8 pin){
	
	LPC_PWM->PCR |= (1<<9);		//output enabled for PWM1
	LPC_PWM->PCR &= ~(1<<9);		//output disabled for PWM1

	LPC_PWM->TCR |= (1<<3);		//start PWM mode counter
return 0;
}
