#include "ssp.h"

volatile SspMasterSlave ssp0MasterSlave;
volatile SspLoopbackMode ssp0LoopbackMode;
volatile SspMasterSlave ssp1MasterSlave;
volatile SspLoopbackMode ssp1LoopbackMode;

volatile GpioPair ssp0SelPins[SSP_MAX_SEL_PINS];
volatile GpioPair ssp1SelPins[SSP_MAX_SEL_PINS];

volatile uint32 ssp0InterruptOverRunStat = 0;
volatile uint32 ssp0InterruptRxTimeoutStat = 0;
volatile uint32 ssp0InterruptRxStat = 0;

volatile uint32 ssp1InterruptOverRunStat = 0;
volatile uint32 ssp1InterruptRxTimeoutStat = 0;
volatile uint32 ssp1InterruptRxStat = 0;

void initializeSsp0(uint32 baudrate,
                    SspDataSize dataSize,
                    SspFrameFormat frameFormat, 
                    SspMasterSlave masterSlave, 
                    SspLoopbackMode loopbackMode,
                    SspSlaveOutput slaveOutput,
                    SspClockOutPolarity clockOutPolarity,
                    SspClockOutPhase clockOutPhase
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
   
   if (masterSlave == SspMasterMode)
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
   else if (masterSlave == SspSlaveMode)
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
   ssp1MasterSlave = masterSlave;
   
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
   
   if (loopbackMode == SspLoopbackEnabled)
   {
       SSP0_SET_LOOPBACK();
   }
   ssp1LoopbackMode = loopbackMode;
   
   for (i = 0; i < SSP_FIFOSIZE; i++)   // CLEAR THE FIFO
   {
       dummy = SSP0_READ_CHAR();
   }
   
   SSP0_ENABLE_SSP();
}

void initializeSsp1(uint32 baudrate,
                    SspDataSize dataSize,
                    SspFrameFormat frameFormat, 
                    SspMasterSlave masterSlave, 
                    SspLoopbackMode loopbackMode,
                    SspSlaveOutput slaveOutput,
                    SspClockOutPolarity clockOutPolarity,
                    SspClockOutPhase clockOutPhase
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
   
   if (masterSlave == SspMasterMode)
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
   else if (masterSlave == SspSlaveMode)
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
   ssp1MasterSlave = masterSlave;
   
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
   
   if (loopbackMode == SspLoopbackEnabled)
   {
       SSP1_SET_LOOPBACK();
   }
   ssp1LoopbackMode = loopbackMode;
   
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
            ssp0InterruptOverRunStat++;
            SSP0_RESET_RORIC_INTERRUPT();
    }
    if ( regValue & SSPMIS_RTMIS )        /* Receive timeout interrupt */
    {
            ssp0InterruptRxTimeoutStat++;
            SSP0_RESET_RTIC_INTERRUPT();
    }

    if ( regValue & SSPMIS_RXMIS )        /* Rx at least half full */
    {
        ssp0InterruptRxStat++;             /* receive until it's empty */          
    }
}

void SSP1_IRQHANDLER()
{
    uint32_t regValue;
    
    regValue = SSP1_GET_INTERRUPT_STATUS();
    if ( regValue & SSPMIS_RORMIS )       /* Receive overrun interrupt */
    {
            ssp1InterruptOverRunStat++;
            SSP1_RESET_RORIC_INTERRUPT();
    }
    if ( regValue & SSPMIS_RTMIS )        /* Receive timeout interrupt */
    {
            ssp1InterruptRxTimeoutStat++;
            SSP1_RESET_RTIC_INTERRUPT();
    }

    if ( regValue & SSPMIS_RXMIS )        /* Rx at least half full */
    {
        ssp1InterruptRxStat++;             /* receive until it's empty */          
    }
}

void initializeSelSsp1(uint8 id, uint8 port, uint8 pin)
{
    ssp1SelPins[id].pin = pin;
    ssp1SelPins[id].port = port;
    
    setGpioDirection(port, pin, GpioDirectionOutput);
    setPinMode(port, pin, PinModePullUp);
    setGpio(port, pin);
}

void putcharSsp1(uint8 selId, uint16 data)
{
   uint16 dummy = dummy;
   
   clearGpio(ssp1SelPins[selId].port, ssp1SelPins[selId].pin);
   
    /* Move on only if NOT busy and TX FIFO not full. */
    while ( SSP1_TRANSMIT_BUFFER_NOT_EMPTY_OR_BUSY() )
        ;
    
    SSP1_WRITE_DATA_REGISTER(data);
   
    if (ssp1LoopbackMode == SspLoopbackDisabled)
    {
        while (SSP1_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
                ; 
        dummy = SSP1_READ_CHAR();
    }
    else if (ssp1LoopbackMode == SspLoopbackEnabled)
    {
        /* Wait until the Busy bit is cleared. */
        while ( SSP1_BUSY() )
            ;
    }
    
    setGpio(ssp1SelPins[selId].port, ssp1SelPins[selId].pin);
}

void getcharSsp1(uint8 selId, uint16 *data)
{
    clearGpio(ssp1SelPins[selId].port, ssp1SelPins[selId].pin);
    
    if (ssp1LoopbackMode == SspLoopbackDisabled)
    {
        if (ssp1MasterSlave == SspMasterMode)
        {
            SSP1_WRITE_DATA_REGISTER(0xFF); // write anything to the bus
        
            while (SSP0_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
                ;
        }
        else if (ssp1MasterSlave == SspSlaveMode)
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
    
    setGpio(ssp1SelPins[selId].port, ssp1SelPins[selId].pin);
}

void readWriteSsp1(uint8 selId, uint16 writeData, uint16 *readData)
{
    clearGpio(ssp1SelPins[selId].port, ssp1SelPins[selId].pin);
    
    SSP1_WRITE_DATA_REGISTER(writeData); // write something to the bus
        
    while (SSP0_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
        ;
    
    *readData = SSP0_READ_DATA_REGISTER();
    
    setGpio(ssp1SelPins[selId].port, ssp1SelPins[selId].pin);
}

void initializeSelSsp0(uint8 id, uint8 port, uint8 pin)
{
    ssp0SelPins[id].pin = pin;
    ssp0SelPins[id].port = port;
    
    setGpioDirection(port, pin, GpioDirectionOutput);
    setPinMode(port, pin, PinModePullUp);
    setGpio(port, pin);
}

void putcharSsp0(uint8 selId, uint16 data)
{
    clearGpio(ssp0SelPins[selId].port, ssp0SelPins[selId].pin);
    
    uint16 dummy = dummy;
   
    /* Move on only if NOT busy and TX FIFO not full. */
    while ( SSP0_TRANSMIT_BUFFER_NOT_EMPTY_OR_BUSY() )
        ;
    
    SSP0_WRITE_DATA_REGISTER(data);
   
    if (ssp0LoopbackMode == SspLoopbackDisabled)
    {
        while (SSP0_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
                ; 
        dummy = SSP0_READ_CHAR();
    }
    else if (ssp0LoopbackMode == SspLoopbackEnabled)
    {
        /* Wait until the Busy bit is cleared. */
        while ( SSP0_BUSY() )
            ;
    }
    
     setGpio(ssp0SelPins[selId].port, ssp0SelPins[selId].pin);
}

void getcharSsp0(uint8 selId, uint16 *data)
{
    clearGpio(ssp0SelPins[selId].port, ssp0SelPins[selId].pin);
    
    if (ssp0MasterSlave == SspMasterMode)
    {
        SSP0_WRITE_DATA_REGISTER(0xFF); // write anything to the bus
        
        while (SSP0_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
            ;
    }
    else if (ssp0MasterSlave == SspSlaveMode)
    {
        while (!SSP0_RECEIVE_BUFFER_NOT_EMPTY())
            ;
    }
    
    *data = SSP0_READ_DATA_REGISTER();
    
    setGpio(ssp0SelPins[selId].port, ssp0SelPins[selId].pin);
}

void readWriteSsp0(uint8 selId, uint16 writeData, uint16 *readData)
{
   clearGpio(ssp0SelPins[selId].port, ssp0SelPins[selId].pin);

    SSP0_WRITE_DATA_REGISTER(writeData); // write anything to the bus
        
    while (SSP0_RECEIVE_BUFFER_EMPTY_OR_BUSY()) // wait for incoming
        ;
        
    *readData = SSP0_READ_DATA_REGISTER();
    
    setGpio(ssp0SelPins[selId].port, ssp0SelPins[selId].pin);
}
