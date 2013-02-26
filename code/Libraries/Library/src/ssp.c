#include "ssp.h"

volatile Ssp_Mode ssp0_masterSlave;
volatile Ssp_Loopback ssp0_loopbackMode;
volatile Ssp_Mode ssp1_masterSlave;
volatile Ssp_Loopback ssp1_loopbackMode;

volatile GpioPair ssp0_selPins[SSP_MAX_SEL_PINS];
volatile GpioPair ssp1_selPins[SSP_MAX_SEL_PINS];

volatile uint32 ssp0_interruptOverRunStat = 0;
volatile uint32 ssp0_interruptRxTimeoutStat = 0;
volatile uint32 ssp0_interruptRxStat = 0;

volatile uint32 ssp1_interruptOverRunStat = 0;
volatile uint32 ssp1_interruptRxTimeoutStat = 0;
volatile uint32 ssp1_interruptRxStat = 0;

void Ssp0_initialize(uint32 baudrate,
                    Ssp_DataSize dataSize,
                    Ssp_FrameFormat frameFormat, 
                    Ssp_Mode masterSlave, 
                    Ssp_Loopback loopbackMode,
                    Ssp_SlaveOutput slaveOutput,
                    Ssp_ClockOutPolarity clockOutPolarity,
                    Ssp_ClockOutPhase clockOutPhase
                    )
{
   uint8 i;
   uint16 dummy = dummy;
   
   uint8 cpsdvsr;
   uint8 scr;
   uint32 pclk;
   uint32 divisor;
    
   SSP0_ENABLE_POWER();
   
   cpsdvsr = 2;
   scr = 0;
   
   if (masterSlave == Ssp_Mode_Master)
   { 
        cpsdvsr = 126;
        scr = 127;
        pclk = baudrate*(cpsdvsr*(scr+1));
        divisor = SystemCoreClock/pclk;
        
        // calculate core clock divisor
        if (divisor < 8)
        {
            if (divisor < 4)
            {
                if (divisor < 2)
                {
                    SSP0_SET_CORE_CLK_DIVISOR_1();
                    pclk = SystemCoreClock;
                }
                else
                {
                    SSP0_SET_CORE_CLK_DIVISOR_2();
                    pclk = SystemCoreClock/2;
                }
            }
            else
            {
                SSP0_SET_CORE_CLK_DIVISOR_4();
                pclk = SystemCoreClock/4;
            }
        }
        else
        {
            SSP0_SET_CORE_CLK_DIVISOR_8();
            pclk = SystemCoreClock/8;
        }
        
        // calculate cpsdvsr
        scr = 255;
        cpsdvsr = (uint8)(pclk/baudrate/(scr+1))+1;  // alternative to ceil  
        cpsdvsr = cpsdvsr + (cpsdvsr % 2);   // only even numbers
        
        // calculate scr
        scr = (uint8)(pclk/(baudrate*cpsdvsr)-1)+1;
        
        SSP0_SET_MASTER();
   }
   else if (masterSlave == Ssp_Mode_Slave)
   {
       pclk = baudrate * 12;            // the ssp core clock needs to be 12 time as fast as the ssp clock 
       divisor = SystemCoreClock/pclk;
       
       if (divisor < 8)
        {
            if (divisor < 4)
            {
                if (divisor < 2)
                {
                    SSP0_SET_CORE_CLK_DIVISOR_1();
                }
                else
                {
                    SSP0_SET_CORE_CLK_DIVISOR_2();
                }
            }
            else
            {
                SSP0_SET_CORE_CLK_DIVISOR_4();
            }
        }
        else
        {
            SSP0_SET_CORE_CLK_DIVISOR_8();
        }
       
       SSP0_CLEAR_MASTER();
   }
   ssp1_masterSlave = masterSlave;
   
   SSP0_INIT_SCK();
   SSP0_INIT_MISO();
   SSP0_INIT_MOSI();
   
   SSP0_SET_TIM_AND_RORIM_INTERRUPT();
   SSP0_ENABLE_IRQ();
   
   SSP0_SET_CPSR(cpsdvsr);
   
   SSP0_SET_DATA_SIZE(dataSize);
   SSP0_SET_FRAME_FORMAT(frameFormat);
   SSP0_SET_CPOL(clockOutPolarity);
   SSP0_SET_CPHA(clockOutPhase);
   SSP0_SET_SCR(scr);
   SSP0_SET_SOD(slaveOutput);
   
   SSP0_SET_SCR(scr);
   
   if (loopbackMode == Ssp_Loopback_Enabled)
   {
       SSP0_SET_LOOPBACK();
   }
   ssp1_loopbackMode = loopbackMode;
   
   for (i = 0; i < SSP_FIFOSIZE; i++)   // CLEAR THE FIFO
   {
       dummy = SSP0_READ_CHAR();
   }
   
   SSP0_ENABLE_SSP();
}

void Ssp1_initialize(uint32 baudrate,
                    Ssp_DataSize dataSize,
                    Ssp_FrameFormat frameFormat, 
                    Ssp_Mode masterSlave, 
                    Ssp_Loopback loopbackMode,
                    Ssp_SlaveOutput slaveOutput,
                    Ssp_ClockOutPolarity clockOutPolarity,
                    Ssp_ClockOutPhase clockOutPhase
                    )
{
   uint8 i;
   uint16 dummy = dummy;
   
   uint8 cpsdvsr;
   uint8 scr;
   uint32 pclk;
   uint32 divisor;
    
   SSP1_ENABLE_POWER();
   
   cpsdvsr = 2;
   scr = 0;
   
   if (masterSlave == Ssp_Mode_Master)
   { 
        cpsdvsr = 126;
        scr = 127;
        pclk = baudrate*(cpsdvsr*(scr+1));
        divisor = SystemCoreClock/pclk;
        
        // calculate core clock divisor
        if (divisor < 8)
        {
            if (divisor < 4)
            {
                if (divisor < 2)
                {
                    SSP1_SET_CORE_CLK_DIVISOR_1();
                    pclk = SystemCoreClock;
                }
                else
                {
                    SSP1_SET_CORE_CLK_DIVISOR_2();
                    pclk = SystemCoreClock/2;
                }
            }
            else
            {
                SSP1_SET_CORE_CLK_DIVISOR_4();
                pclk = SystemCoreClock/4;
            }
        }
        else
        {
            SSP1_SET_CORE_CLK_DIVISOR_8();
            pclk = SystemCoreClock/8;
        }
        
        // calculate cpsdvsr
        scr = 255;
        cpsdvsr = (uint8)(pclk/baudrate/(scr+1))+1;  // alternative to ceil  
        cpsdvsr = cpsdvsr + (cpsdvsr % 2);   // only even numbers
        
        // calculate scr
        scr = (uint8)(pclk/(baudrate*cpsdvsr)-1)+1;
        
        SSP1_SET_MASTER();
   }
   else if (masterSlave == Ssp_Mode_Slave)
   {
       pclk = baudrate * 12;            // the ssp core clock needs to be 12 time as fast as the ssp clock 
       divisor = SystemCoreClock/pclk;
       
       if (divisor < 8)
        {
            if (divisor < 4)
            {
                if (divisor < 2)
                {
                    SSP1_SET_CORE_CLK_DIVISOR_1();
                }
                else
                {
                    SSP1_SET_CORE_CLK_DIVISOR_2();
                }
            }
            else
            {
                SSP1_SET_CORE_CLK_DIVISOR_4();
            }
        }
        else
        {
            SSP1_SET_CORE_CLK_DIVISOR_8();
        }
       
       SSP1_CLEAR_MASTER();
   }
   ssp1_masterSlave = masterSlave;
   
   SSP1_INIT_SCK();
   SSP1_INIT_MISO();
   SSP1_INIT_MOSI();
   
   SSP1_SET_TIM_AND_RORIM_INTERRUPT();
   SSP1_ENABLE_IRQ();
   
   SSP1_SET_CPSR(cpsdvsr);
   
   SSP1_SET_DATA_SIZE(dataSize);
   SSP1_SET_FRAME_FORMAT(frameFormat);
   SSP1_SET_CPOL(clockOutPolarity);
   SSP1_SET_CPHA(clockOutPhase);
   SSP1_SET_SCR(scr);
   SSP1_SET_SOD(slaveOutput);
   
   if (loopbackMode == Ssp_Loopback_Enabled)
   {
       SSP1_SET_LOOPBACK();
   }
   ssp1_loopbackMode = loopbackMode;
   
   for (i = 0; i < SSP_FIFOSIZE; i++)   // CLEAR THE FIFO
   {
       dummy = SSP1_READ_CHAR();
   }
   
   SSP1_ENABLE_SSP();
}

void SSP0_IRQHANDLER()
{
    uint32_t regValue;
    
    regValue = SSP0_GET_INTERRUPT_STATUS();
    if ( regValue & SSPMIS_RORMIS )       /* Receive overrun interrupt */
    {
            ssp0_interruptOverRunStat++;
            SSP0_RESET_RORIC_INTERRUPT();
    }
    if ( regValue & SSPMIS_RTMIS )        /* Receive timeout interrupt */
    {
            ssp0_interruptRxTimeoutStat++;
            SSP0_RESET_RTIC_INTERRUPT();
    }

    if ( regValue & SSPMIS_RXMIS )        /* Rx at least half full */
    {
        ssp0_interruptRxStat++;             /* receive until it's empty */          
    }
}

void SSP1_IRQHANDLER()
{
    uint32_t regValue;
    
    regValue = SSP1_GET_INTERRUPT_STATUS();
    if ( regValue & SSPMIS_RORMIS )       /* Receive overrun interrupt */
    {
            ssp1_interruptOverRunStat++;
            SSP1_RESET_RORIC_INTERRUPT();
    }
    if ( regValue & SSPMIS_RTMIS )        /* Receive timeout interrupt */
    {
            ssp1_interruptRxTimeoutStat++;
            SSP1_RESET_RTIC_INTERRUPT();
    }

    if ( regValue & SSPMIS_RXMIS )        /* Rx at least half full */
    {
        ssp1_interruptRxStat++;             /* receive until it's empty */          
    }
}

void Ssp1_initializeSel(uint8 id, uint8 port, uint8 pin)
{
    ssp1_selPins[id].pin = pin;
    ssp1_selPins[id].port = port;
    
    setGpioDirection(port, pin, GpioDirectionOutput);
    setPinMode(port, pin, PinModePullUp);
    setGpio(port, pin);
}

void Ssp1_putchar(uint8 selId, uint16 data)
{
   uint16 dummy = dummy;
   
   clearGpio(ssp1_selPins[selId].port, ssp1_selPins[selId].pin);
   
    /* Move on only if NOT busy and TX FIFO not full. */
    while ( SSP1_TRANSMIT_BUFFER_NOT_EMPTY_OR_BUSY() )
        ;
    
    SSP1_WRITE_DATA_REGISTER(data);
   
    if (ssp1_loopbackMode == Ssp_Loopback_Disabled)
    {
        while (SSP1_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
                ; 
        dummy = SSP1_READ_CHAR();
    }
    else if (ssp1_loopbackMode == Ssp_Loopback_Enabled)
    {
        /* Wait until the Busy bit is cleared. */
        while ( SSP1_BUSY() )
            ;
    }
    
    setGpio(ssp1_selPins[selId].port, ssp1_selPins[selId].pin);
}

void Ssp1_getchar(uint8 selId, uint16 *data)
{
    clearGpio(ssp1_selPins[selId].port, ssp1_selPins[selId].pin);
    
    if (ssp1_loopbackMode == Ssp_Loopback_Disabled)
    {
        if (ssp1_masterSlave == Ssp_Mode_Master)
        {
            SSP1_WRITE_DATA_REGISTER(0xFF); // write anything to the bus
        
            while (SSP0_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
                ;
        }
        else if (ssp1_masterSlave == Ssp_Mode_Slave)
        {
            while (!SSP0_RECEIVE_BUFFER_NOT_EMPTY())
            ;
        }
    }
    else
    {
        while (!SSP0_RECEIVE_BUFFER_NOT_EMPTY())
            ;
    }

    *data = SSP0_READ_DATA_REGISTER();
    
    setGpio(ssp1_selPins[selId].port, ssp1_selPins[selId].pin);
}

void Ssp1_readWrite(uint8 selId, uint16 writeData, uint16 *readData)
{
    clearGpio(ssp1_selPins[selId].port, ssp1_selPins[selId].pin);
    
    SSP1_WRITE_DATA_REGISTER(writeData); // write something to the bus
        
    while (SSP0_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
        ;
    
    *readData = SSP0_READ_DATA_REGISTER();
    
    setGpio(ssp1_selPins[selId].port, ssp1_selPins[selId].pin);
}

void Ssp0_initializeSel(uint8 id, uint8 port, uint8 pin)
{
    ssp0_selPins[id].pin = pin;
    ssp0_selPins[id].port = port;
    
    setGpioDirection(port, pin, GpioDirectionOutput);
    setPinMode(port, pin, PinModePullUp);
    setGpio(port, pin);
}

void Ssp0_putchar(uint8 selId, uint16 data)
{
    clearGpio(ssp0_selPins[selId].port, ssp0_selPins[selId].pin);
    
    uint16 dummy = dummy;
   
    /* Move on only if NOT busy and TX FIFO not full. */
    while ( SSP0_TRANSMIT_BUFFER_NOT_EMPTY_OR_BUSY() )
        ;
    
    SSP0_WRITE_DATA_REGISTER(data);
   
    if (ssp0_loopbackMode == Ssp_Loopback_Disabled)
    {
        while (SSP0_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
                ; 
        dummy = SSP0_READ_CHAR();
    }
    else if (ssp0_loopbackMode == Ssp_Loopback_Enabled)
    {
        /* Wait until the Busy bit is cleared. */
        while ( SSP0_BUSY() )
            ;
    }
    
     setGpio(ssp0_selPins[selId].port, ssp0_selPins[selId].pin);
}

void Ssp0_getchar(uint8 selId, uint16 *data)
{
    clearGpio(ssp0_selPins[selId].port, ssp0_selPins[selId].pin);
    
    if (ssp0_masterSlave == Ssp_Mode_Master)
    {
        SSP0_WRITE_DATA_REGISTER(0xFF); // write anything to the bus
        
        while (SSP0_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
            ;
    }
    else if (ssp0_masterSlave == Ssp_Mode_Slave)
    {
        while (!SSP0_RECEIVE_BUFFER_NOT_EMPTY())
            ;
    }
    
    *data = SSP0_READ_DATA_REGISTER();
    
    setGpio(ssp0_selPins[selId].port, ssp0_selPins[selId].pin);
}

void Ssp0_readWrite(uint8 selId, uint16 writeData, uint16 *readData)
{
   clearGpio(ssp0_selPins[selId].port, ssp0_selPins[selId].pin);

    SSP0_WRITE_DATA_REGISTER(writeData); // write anything to the bus
        
    while (SSP0_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
        ;
        
    *readData = SSP0_READ_DATA_REGISTER();
    
    setGpio(ssp0_selPins[selId].port, ssp0_selPins[selId].pin);
}
