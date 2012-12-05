#ifndef _LEDDRIVER_H_
#define _LEDDRIVER_H_

#define LEDCOUNT 1
#define LEDOFFSET 28
#define MAXLED LEDCOUNT+LEDOFFSET
#define INIT_OK 0

#define LATCH_EN (1<<2)
#define LED1	 (1<<29)  /* PIO1_29  */

#define LED_GREEN_ENB   (1<<0)
#define LED_RED_ENB     (1<<2)

#define SET_LED(x)    LPC_GPIO1->FIOCLR = (1<<(x+LEDOFFSET))
#define CLEAR_LED(x)      LPC_GPIO1->FIOSET = (1<<(x+LEDOFFSET))
#define READ_LED(x)     !(LPC_GPIO1->FIOPIN & (1<<(x+LEDOFFSET)))
#define ENABLE_GREEN()  //LPC_GPIO2->DATA &= ~(LED_GREEN_ENB)
#define DISABLE_GREEN() //LPC_GPIO2->DATA |= LED_GREEN_ENB
#define ENABLE_RED()    //LPC_GPIO1->DATA |= LED_RED_ENB
#define DISABLE_RED()   //LPC_GPIO1->DATA &= ~(LED_RED_ENB)

#include <LPC17xx.h>
#include <types.h>

inline uint8 initLeds(void);

#endif
