#ifndef _GPIODIVER_H_
#define _GPIODIVER_H_

#define GPIO0_CLEAR_DIR(pin)            LPC_GPIO0->FIODIR &= ~(1 << pin)
#define GPIO0_SET_DIR(direction,pin)    LPC_GPIO0->FIODIR |= (direction << pin)    
#define GPIO1_CLEAR_DIR(pin)            LPC_GPIO1->FIODIR &= ~(1 << pin)
#define GPIO1_SET_DIR(direction,pin)    LPC_GPIO1->FIODIR |= (direction << pin)    
#define GPIO2_CLEAR_DIR(pin)            LPC_GPIO2->FIODIR &= ~(1 << pin)
#define GPIO2_SET_DIR(direction,pin)    LPC_GPIO2->FIODIR |= (direction << pin)    
#define GPIO3_CLEAR_DIR(pin)            LPC_GPIO3->FIODIR &= ~(1 << pin)
#define GPIO3_SET_DIR(direction,pin)    LPC_GPIO3->FIODIR |= (direction << pin)    

#define GPIO0_READ(pin)                 ((LPC_GPIO0->FIOPIN & (1 << pin)) > 0)
#define GPIO1_READ(pin)                 ((LPC_GPIO1->FIOPIN & (1 << pin)) > 0)
#define GPIO2_READ(pin)                 ((LPC_GPIO2->FIOPIN & (1 << pin)) > 0)
#define GPIO3_READ(pin)                 ((LPC_GPIO3->FIOPIN & (1 << pin)) > 0)

#define GPIO0_SET(pin)                  LPC_GPIO0->FIOSET |= (1 << pin)
#define GPIO1_SET(pin)                  LPC_GPIO1->FIOSET |= (1 << pin)
#define GPIO2_SET(pin)                  LPC_GPIO2->FIOSET |= (1 << pin)
#define GPIO3_SET(pin)                  LPC_GPIO3->FIOSET |= (1 << pin)

#define GPIO0_CLEAR(pin)                LPC_GPIO0->FIOCLR |= (1 << pin)
#define GPIO1_CLEAR(pin)                LPC_GPIO1->FIOCLR |= (1 << pin)
#define GPIO2_CLEAR(pin)                LPC_GPIO2->FIOCLR |= (1 << pin)
#define GPIO3_CLEAR(pin)                LPC_GPIO3->FIOCLR |= (1 << pin)

#define GPIO0_INT_PIN_COUNT     21
#define GPIO2_INT_PIN_COUNT     14

#include <LPC17xx.h>

#endif
