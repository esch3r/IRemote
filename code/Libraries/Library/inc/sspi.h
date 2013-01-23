/**
 * @file spi.h
 * @author Johannes Wimmer & Alexander Rössler
 * @brief PWM Library
 * @date 16-10-2012
 */

#ifndef _PWM_DRIVER_H_
#define _PWM_DRIVER_H_

#include <types.h>
#include <LPC17xx.h>

#define PCONP_SSPI(ch)     (1 << (8+2*ch)) 		//set clock control bit
#define SSPI0_SET_CORE_CLK()  LPC_SC->PCLKSEL0 |= (0b00<< 10); //set 11:10 for SSPI
#define SSPI1_SET_CORE_CLK()  LPC_SC->PCLKSEL1 |= (0b00<< 20); //set 16:17 for SSPI
#define SYSTEM_CLK      SystemCoreClock

#define SSPI_ENABLE_POWER(ch)  LPC_SC->PCONP |= PCONP_SSPI(ch)             //enable PCONP for SSPI
#define SPI_DISABLE_POWER(ch) LPC_SC->PCONP &= ~PCONP_SSPI(ch)              //set PCLKSEL for SSPI

#define SPI_SET_SSEL1         LPC_PINCON->PINSEL0  |= (0b10 << 12); LPC_PINCON->PINMODE0 |= (0b11 << 12)  // P0.6 pull-down Resistor
#define SPI_SET_SCK1          LPC_PINCON->PINSEL0  |= (0b10 << 14); LPC_PINCON->PINMODE0 |= (0b11 << 14)  // P0.7 pull-down Resistor
#define SPI_SET_MISO1         LPC_PINCON->PINSEL0  |= (0b10 << 16); LPC_PINCON->PINMODE0 |= (0b11 << 16)  // P0.8 pull-down Resistor
#define SPI_SET_MOSI1         LPC_PINCON->PINSEL0  |= (0b10 << 18); LPC_PINCON->PINMODE0 |= (0b11 << 18)  // P0.9 pull-down Resistor

#define SPI_ENABLE_IRQ()      NVIC_EnableIRQ(SPI_IRQn);

#endif
