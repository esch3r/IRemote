#ifndef _PWM_DRIVER_H_
#define _PWM_DRIVER_H_

//#include "LPC17xx.h"
#include "lpc17xx_pinsel.h"
#include <types.h>
#include <LPC17xx.h>

#define PCONP_PWM     (1 << 6) 		//set pwm1/clock control bit
#define PWM_PDN        (1 << 21)     // 1 ADC on, 0 ADC off
#define PCLK_PWM       (1 << 24)     // 0b00 CCLK/4, 0b01 CCLK, 0b10 CCLK/2, 0b11 CCLK/8

#define SYSTEM_CLK      SystemCoreClock
#define PWM_CLK         SYSTEM_CLK/4


#define PWM_ENABLE_POWER()  LPC_SC->PCONP |= PCONP_PWM
#define PWM_DISABLE_POWER() LPC_SC->PCONP &= ~PCONP_PWM
#define PWM_SET_CORE_CLK()  LPC_SC->PCLKSEL0 |= (0b00<< 12); //set 12:13 for PWM1 

#define PWM_CLEAR_PINSEL()  LPC_ADC->ADCR &= ~0xFF

#define PWM_ENABLE_PIN1()   LPC_PINCON->PINSEL3  |= (0b10 << 4); LPC_PINCON->PINMODE1 |= (0b00 << 4)  // PWM 1.1 func 10


#define ADC_SET_CLK(x)      LPC_ADC->ADCR |= (((uint16)(ADC_CLK/x) - 1) << 8);
#define ADC_CONFIGURE()     LPC_ADC->ADCR |= (0 << 24); LPC_ADC->ADCR |= (0 << 27); // No start and CAP/MAT rising edge trigger
#define ADC_ENABLE()        LPC_ADC->ADCR |= ADCR_PDN
#define ADC_DISABLE()       LPC_ADC->ADCR &= ~ADCR_PDN
#define ADC_SET_BURSTMODE() LPC_ADC->ADCR |= ADCR_BURST 

#define PWM_ENABLE_IRQ()            NVIC_EnableIRQ(PWM1_IRQn)

#define ADC_DISABLE_IRQ()           NVIC_DisableIRQ(ADC_IRQn)
#define ADC_SET_ALL_IRQS_BURST()    LPC_ADC->ADINTEN = 0xFF
#define ADC_SET_ALL_IRQS()          LPC_ADC->ADINTEN = 0x1FF

#define ADC_START()         LPC_ADC->ADCR |= (1 << 24)
#define ADC_STOP()          LPC_ADC->ADCR &= ~(0x7 << 24)

// Working with the ADC output register
#define ADC_HAS_OVERRUN(x)  x & ADC_OVERRUN
#define ADC_GET_VALUE(x)    (x >> 4) & 0xFFFF;

int8 startPWM(uint8 pin);
int8 initializePWM(uint32 freq,float duty, uint8 pin);
#endif
