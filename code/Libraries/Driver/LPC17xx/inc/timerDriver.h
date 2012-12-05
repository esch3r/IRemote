#ifndef _TIMERDRIVER_H_
#define _TIMERDRIVER_H_

#define SYSTEMCLK SystemCoreClock
#define OPTIMIZE_PRESCALER_US()         (SYSTEMCLK/1E6)-1
#define OPTIMIZE_PRESCALER_MS()         (SYSTEMCLK/1E3)-1

#define PROZESSOR_SLEEP()               LPC_SC->PCON = 0x00;
#define PROZESSOR_WAIT_FOR_IRQ()        __WFI()

#define LOW_IRQ_PRIORITY                1
#define HIGH_IRQ_PRIORITY               0

#define TIMER0_ENABLE_CLK()             LPC_SC->PCONP |= (1 << 1); LPC_SC->PCLKSEL0 &= ~(1 << 3); LPC_SC->PCLKSEL0 |= (1 << 2)
#define TIMER0_DISABLE_CLK()            //LPC_SC->PCONP &= ~(1 << 1)
#define TIMER0_RESET()                  LPC_TIM0->TCR = 0x02
#define TIMER0_START()                  LPC_TIM0->TCR = 0x01
#define TIMER0_STOP()                   LPC_TIM0->TCR = 0x00
#define TIMER0_RUNNING()                (LPC_TIM0->TCR & 0x01)
#define TIMER0_SET_PRESCALER(x)         LPC_TIM0->PR = (uint32)(x)
#define TIMER0_SET_KHZ(x)               TIMER0_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER0_SET_MHZ(x)               TIMER0_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER0_RESET_IRQS()             LPC_TIM0->IR  = 0xff
#define TIMER0_SET_INTERVAL_MS(x)       LPC_TIM0->MR0 = (uint32)x*(SYSTEMCLK/(LPC_TIM0->PR+1)) / 1000
#define TIMER0_SET_INTERVAL_US(x)       LPC_TIM0->MR0 = (uint32)x*(SYSTEMCLK/(LPC_TIM0->PR+1)) / 1000000
#define TIMER0_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER0_IRQn)
#define TIMER0_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER0_IRQn)
#define TIMER0_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER0_IRQn,x)
#define TIMER0_RESET_AND_IRQ_ON_MATCH()           LPC_TIM0->MCR = (1 << 0) | (1 << 1)
#define TIMER0_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TIM0->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER0_STOP_AND_IRQ_ON_MATCH()            LPC_TIM0->MCR = (1 << 0) | (1 << 2)
#define TIMER0_IRQHANDLER()             TIMER0_IRQHandler(void)

#define TIMER1_ENABLE_CLK()             LPC_SC->PCONP |= (1 << 2); LPC_SC->PCLKSEL0 &= ~(1 << 5); LPC_SC->PCLKSEL0 |= (1 << 4)
#define TIMER1_DISABLE_CLK()            //LPC_SC->PCONP &= ~(1 << 2)
#define TIMER1_RESET()                  LPC_TIM1->TCR = 0x02
#define TIMER1_START()                  LPC_TIM1->TCR = 0x01
#define TIMER1_STOP()                   LPC_TIM1->TCR = 0x00
#define TIMER1_RUNNING()                (LPC_SC->PCONP & (1 << 2)) && (LPC_TIM1->TCR & 0x01)
#define TIMER1_SET_PRESCALER(x)         LPC_TIM1->PR = (uint32)(x)
#define TIMER1_SET_KHZ(x)               TIMER1_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER1_SET_MHZ(x)               TIMER1_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER1_RESET_IRQS()             LPC_TIM1->IR  = 0xff
#define TIMER1_SET_INTERVAL_MS(x)       LPC_TIM1->MR0 = (uint32)x*(SYSTEMCLK/(LPC_TIM1->PR+1)) / 1000
#define TIMER1_SET_INTERVAL_US(x)       LPC_TIM1->MR0 = (uint32)x*(SYSTEMCLK/(LPC_TIM1->PR+1)) / 1000000
#define TIMER1_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER1_IRQn)
#define TIMER1_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER1_IRQn)
#define TIMER1_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER1_IRQn,x)
#define TIMER1_RESET_AND_IRQ_ON_MATCH()           LPC_TIM1->MCR = (1 << 0) | (1 << 1)
#define TIMER1_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TIM1->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER1_STOP_AND_IRQ_ON_MATCH()            LPC_TIM1->MCR = (1 << 0) | (1 << 2)
#define TIMER1_IRQHANDLER()             TIMER1_IRQHandler(void)

#define TIMER2_ENABLE_CLK()             LPC_SC->PCONP |= (1 << 22); LPC_SC->PCLKSEL1 &= ~(1 << 13); LPC_SC->PCLKSEL1 |= (1 << 12)
#define TIMER2_DISABLE_CLK()            //LPC_SC->PCONP &= ~(1 << 22)
#define TIMER2_RESET()                  LPC_TIM2->TCR = 0x02
#define TIMER2_START()                  LPC_TIM2->TCR = 0x01
#define TIMER2_STOP()                   LPC_TIM2->TCR = 0x00
#define TIMER2_RUNNING()                (LPC_SC->PCONP & (1 << 22)) && (LPC_TIM2->TCR & 0x01)
#define TIMER2_SET_PRESCALER(x)         LPC_TIM2->PR = (uint32)(x)
#define TIMER2_SET_KHZ(x)               TIMER2_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER2_SET_MHZ(x)               TIMER2_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER2_RESET_IRQS()             LPC_TIM2->IR  = 0xff
#define TIMER2_SET_INTERVAL_MS(x)       LPC_TIM2->MR0 = (uint32)x*(SYSTEMCLK/(LPC_TIM2->PR+1)) / 1000
#define TIMER2_SET_INTERVAL_US(x)       LPC_TIM2->MR0 = (uint32)x*(SYSTEMCLK/(LPC_TIM2->PR+1)) / 1000000
#define TIMER2_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER2_IRQn)
#define TIMER2_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER2_IRQn)
#define TIMER2_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER2_IRQn,x)
#define TIMER2_RESET_AND_IRQ_ON_MATCH()           LPC_TIM2->MCR = (1 << 0) | (1 << 1)
#define TIMER2_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TIM2->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER2_STOP_AND_IRQ_ON_MATCH()            LPC_TIM2->MCR = (1 << 0) | (1 << 2)
#define TIMER2_IRQHANDLER()             TIMER2_IRQHandler(void)

#define TIMER3_ENABLE_CLK()             LPC_SC->PCONP |= (1 << 23); LPC_SC->PCLKSEL0 &= ~(1 << 15); LPC_SC->PCLKSEL1 |= (1 << 14)
#define TIMER3_DISABLE_CLK()            //LPC_SC->PCONP &= ~(1 << 23)
#define TIMER3_RESET()                  LPC_TIM3->TCR = 0x02
#define TIMER3_START()                  LPC_TIM3->TCR = 0x01
#define TIMER3_STOP()                   LPC_TIM3->TCR = 0x00
#define TIMER3_RUNNING()                (LPC_SC->PCONP & (1 << 23)) && (LPC_TIM3->TCR & 0x01)
#define TIMER3_SET_PRESCALER(x)         LPC_TIM3->PR = (uint32)(x)
#define TIMER3_SET_KHZ(x)               TIMER3_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E3))-1)
#define TIMER3_SET_MHZ(x)               TIMER3_SET_PRESCALER((uint32)((SYSTEMCLK)/(x*1E6))-1)
#define TIMER3_RESET_IRQS()             LPC_TIM3->IR  = 0xff
#define TIMER3_SET_INTERVAL_MS(x)       LPC_TIM3->MR0 = (uint32)x*(SYSTEMCLK/(LPC_TIM3->PR+1)) / 1000
#define TIMER3_SET_INTERVAL_US(x)       LPC_TIM3->MR0 = (uint32)x*(SYSTEMCLK/(LPC_TIM3->PR+1)) / 1000000
#define TIMER3_ENABLE_IRQ()             NVIC_EnableIRQ(TIMER3_IRQn)
#define TIMER3_DISABLE_IRQ()            NVIC_DisableIRQ(TIMER3_IRQn)
#define TIMER3_SET_IRQ_PRIORITY(x)                NVIC_SetPriority(TIMER3_IRQn,x)
#define TIMER3_RESET_AND_IRQ_ON_MATCH()           LPC_TIM3->MCR = (1 << 0) | (1 << 1)
#define TIMER3_RESET_STOP_AND_IRQ_ON_MATCH()      LPC_TIM3->MCR = (1 << 0) | (1 << 1) | (1 << 2)
#define TIMER3_STOP_AND_IRQ_ON_MATCH()            LPC_TIM3->MCR = (1 << 0) | (1 << 2)
#define TIMER3_IRQHANDLER()             TIMER3_IRQHandler(void)

#include <LPC17xx.h>
#include <types.h>

#endif
