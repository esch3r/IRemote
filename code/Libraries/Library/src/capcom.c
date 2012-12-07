#include "capcom.h"

#include <lpc17xx_timer.h>
#include <lpc_types.h>
//#include "lpc17xx_libcfg.h"
#include <lpc17xx_pinsel.h>
#include <lpc17xx_gpio.h>
#include <uart.h>

//UART_CFG_Type UARTConfigStruct;
uint32_t MatchCount;

//timer init
TIM_TIMERCFG_Type TIM_ConfigStruct;
TIM_MATCHCFG_Type TIM_MatchConfigStruct ;
TIM_CAPTURECFG_Type TIM_CaptureConfigStruct;
uint8_t volatile timer0_flag = FALSE, timer1_flag = FALSE;

/************************** PRIVATE FUNCTIONS *************************/
/* Interrupt service routines */
void TIMER3_IRQHandler(void);

int c_entry(void)
{
    PINSEL_CFG_Type PinCfg;


    //Config P1.26 as CAP0.0
    PinCfg.Funcnum = 3;
    PinCfg.OpenDrain = 0;
    PinCfg.Pinmode = 0;
    PinCfg.Portnum = 1;
    PinCfg.Pinnum = 26;
    PINSEL_ConfigPin(&PinCfg);

    // Initialize timer 0, prescale count time of 1000000uS = 1S
    TIM_ConfigStruct.PrescaleOption = TIM_PRESCALE_USVAL;
    TIM_ConfigStruct.PrescaleValue  = 1000000;

    // use channel 0, CAPn.0
    TIM_CaptureConfigStruct.CaptureChannel = 0;
    // Enable capture on CAPn.0 rising edge
    TIM_CaptureConfigStruct.RisingEdge = ENABLE;
    // Enable capture on CAPn.0 falling edge
    TIM_CaptureConfigStruct.FallingEdge = ENABLE;
    // Generate capture interrupt
    TIM_CaptureConfigStruct.IntOnCaption = DISABLE;


    // Set configuration for Tim_config and Tim_MatchConfig
    TIM_Init(LPC_TIM3, TIM_TIMER_MODE,&TIM_ConfigStruct);
    TIM_ConfigCapture(LPC_TIM3, &TIM_CaptureConfigStruct);
    TIM_ResetCounter(LPC_TIM3);


    /* preemption = 1, sub-priority = 1 */
    NVIC_SetPriority(TIMER3_IRQn, ((0x01<<3)|0x01));
    /* Enable interrupt for timer 0 */
    NVIC_EnableIRQ(TIMER3_IRQn);
    // To start timer 0
    TIM_Cmd(LPC_TIM3,ENABLE);

    return 1;
}

void TIMER3_IRQHandler(void)
{
    if (TIM_GetIntCaptureStatus(LPC_TIM3,0))
    {
        TIM_ClearIntCapturePending(LPC_TIM3,0);
        printfUart0("%u\n",TIM_GetCaptureValue(LPC_TIM3,0));
    }
}
