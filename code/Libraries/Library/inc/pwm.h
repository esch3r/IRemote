/**
 * @file spi.h
 * @author Johannes Wimmer & Alexander R�ssler
 * @brief PWM Library
 * @date 16-10-2012
 */

#ifndef _PWM_DRIVER_H_
#define _PWM_DRIVER_H_

#include <types.h>
#include <LPC17xx.h>

#define PCONP_SSPI(ch)     (1 << (8+2*ch))      //set clock control bit
#define SSPI0_SET_CORE_CLK()  LPC_SC->PCLKSEL1 |= (0b00<< 10); //set 11:10 for SSPI0
#define SSPI1_SET_CORE_CLK()  LPC_SC->PCLKSEL0 |= (0b00<< 20); //set 21:20 for SSPI1
#define SYSTEM_CLK      SystemCoreClock

#define SSPI_ENABLE_POWER(ch)  LPC_SC->PCONP |= PCONP_SSPI(ch)             //enable PCONP for SSPI
#define SPI_DISABLE_POWER(ch) LPC_SC->PCONP &= ~PCONP_SSPI(ch)              //set PCLKSEL for SSPI

#define SSPI_SET_SSEL1         LPC_PINCON->PINSEL0  |= (0b10 << 12); LPC_PINCON->PINMODE0 |= (0b11 << 12)  // P0.6 pull-down Resistor
#define SSPI_SET_SCK1          LPC_PINCON->PINSEL0  |= (0b10 << 14); LPC_PINCON->PINMODE0 |= (0b11 << 14)  // P0.7 pull-down Resistor
#define SSPI_SET_MISO1         LPC_PINCON->PINSEL0  |= (0b10 << 16); LPC_PINCON->PINMODE0 |= (0b11 << 16)  // P0.8 pull-down Resistor
#define SSPI_SET_MOSI1         LPC_PINCON->PINSEL0  |= (0b10 << 18); LPC_PINCON->PINMODE0 |= (0b11 << 18)  // P0.9 pull-down Resistor

#define SPI_ENABLE_IRQ()      NVIC_EnableIRQ(SPI_IRQn);

#endifefine PWM1_TOGGLE_OUTPUT(ch)     (LPC_PWM1->PCR & (1<<(ch+9))) ? stopPwm(ch) : startPWM(ch);  // Toggles output for PWM1

/** Initializes the pulsewith generator.
 *@param freq  Is the frequency for the gernerated signal.
 *@param duty  Is the duty-cicle for the signal.
 *@param ch   Says which channel(1-6) should be taken.
 */
int8 initializePWM(uint32 freq,float duty, uint8 ch);

/** Startes the PWM mode and the counter.
 *@param ch Says which channel(1-5) should be taken.
 */
inline void startPWM(uint8 ch);
/** Stop the PWM and set signal to 0.
 *@param ch Says which channel(1-5) should be taken.
 */
