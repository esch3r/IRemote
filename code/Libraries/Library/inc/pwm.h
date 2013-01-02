/**
 * @file pwm.h
 * @author Johannes Wimmer & Alexander Rössler
 * @brief PWM Library
 * @date 16-10-2012
 */

#ifndef _PWM_DRIVER_H_
#define _PWM_DRIVER_H_

#include <types.h>
#include <LPC17xx.h>

#define PCONP_PWM     (1 << 6) 		//set pwm1/clock control bit
#define PWM_PDN        (1 << 21)     // 1 ADC on, 0 ADC off
#define PCLK_PWM       (1 << 24)     // 0b00 CCLK/4, 0b01 CCLK, 0b10 CCLK/2, 0b11 CCLK/8
#define PWM_SET_CORE_CLK()  LPC_SC->PCLKSEL0 |= (0b00<< 12); //set 12:13 for PWM1
#define SYSTEM_CLK      SystemCoreClock
#define PWM_CLK         SYSTEM_CLK/4

#define PWM_ENABLE_POWER()  LPC_SC->PCONP |= PCONP_PWM
#define PWM_DISABLE_POWER() LPC_SC->PCONP &= ~PCONP_PWM

#define PWM_ENABLE_PIN0()   LPC_PINCON->PINSEL4  |= (0b01 << 0); LPC_PINCON->PINMODE4 |= (0b00 << 0)  // PWM 1.1 func 01 with pull-up Resistor
#define PWM_ENABLE_PIN1()   LPC_PINCON->PINSEL4  |= (0b01 << 2); LPC_PINCON->PINMODE4 |= (0b00 << 2)
#define PWM_ENABLE_PIN2()   LPC_PINCON->PINSEL4  |= (0b01 << 4); LPC_PINCON->PINMODE4 |= (0b00 << 4)
#define PWM_ENABLE_PIN3()   LPC_PINCON->PINSEL4  |= (0b01 << 6); LPC_PINCON->PINMODE4 |= (0b00 << 6)
#define PWM_ENABLE_PIN4()   LPC_PINCON->PINSEL4  |= (0b01 << 8); LPC_PINCON->PINMODE4 |= (0b00 << 8)
#define PWM_ENABLE_PIN5()   LPC_PINCON->PINSEL4  |= (0b01 << 10); LPC_PINCON->PINMODE4 |= (0b00 << 10)

int8 initializePWM(uint32 freq,float duty, uint8 pin);
inline void startPWM(uint8 pin);
inline void stopPwm(uint8 pin);
inline void togglePwm(uint8 pin);
#endif
