include "sspi.h"

CircularBuffer Ssp1ReadBuffer;
CircularBuffer Ssp1WriteBuffer;

int8 initializeSSP0(uint32 baudrate,SspDataSize dataSize,SspFrameFormat frameFormat)
{
    SSP_ENABLE_POWER(0);
    SSP0_SET_CORE_CLk;    
   SSP_PR(baudrate);                      // set Prescaler
 
   SSP0_SET_DATA_SIZE(dataSize);
   SSP0_SET_FRAME_FORMAT(frameFormat);

   
   SSP0_SET_SSEL1;     
   SSP0_SET_SCK1;
   SSP0_SET_MISO1;
   SSP0_SET_MOSI1;

   SSP_ENABLE_IRQ();
   SSP0_SET_RXIM_AND_TXIM_INTERRUPT();
   SSP0_ENABLE_SSP;
   
   return 0;
}
int8 initializeSSP1(uint32 baudrate,SspDataSize dataSize,SspFrameFormat frameFormat)
{
    SSP_ENABLE_POWER(1);
    SSP1_SET_CORE_CLK();

    SSP1_SET_PR(baudrate);                      // set Prescaler
 
    SSP1_SET_DATA_SIZE(dataSize);
    SSP1_SET_FRAME_FORMAT(frameFormat);
   
    SSP1_SET_SSEL1;
    SSP1_SET_SCK1;
    SSP1_SET_MISO1;
    SSP1_SET_MOSI1;
    
    if (initializeCb(&Ssp1ReadBuffer, SSP1_READ_BUFFER_SIZE, sizeof(uint16)) == -1)      // Initialize circular read buffer
        return -1;
    
    if (initializeCb(&Ssp1WriteBuffer, SSP1_WRITE_BUFFER_SIZE, sizeof(uint16)) == -1)    //Initialize circular write buffer
        return -1;
    

   SSP_ENABLE_IRQ();
   SSP1_SET_RXIM_AND_TXIM_INTERRUPT();
   
   SSP1_ENABLE_SSP;
   return 0;
}

#if (USE_SSP_BUFFER == 1)
void SSP1_IRQHANDLER()
{
    uint16 c;
    
    if (!SSP1_FIFO_TRANSMIT_EMPTY)          // If the interrupt comes from RBR
    {
        c = SSP1_READ_CHAR();                  // Read character and put it to the buffer, also disables interrupt
        putCb(&Ssp1ReadBuffer,(void*)(&c));
    }
    else                                        // Else interrupt comes from THRE or put function
    {
        if (getCb(&Ssp1WriteBuffer,&c) == 0)   // If data is available
            SSP1_SET_CHAR(c);                  // Put data to the uart, disables interrupt
    }
}
#endif

int8 putcharSsp1(uint16 c)
{
#if (USE_SSP_BUFFER == 1)
    if (putCb(&Ssp1WriteBuffer, &c) == 0)
    {
        if (SSP_FIFO_RECEIVE_EMPTY)         // If THR is empty, trigger an interrupt to write data to the UART
            SSP_SET_IRQS();
        
        return 0;
    }
    else 
        return -1;
#else
    while(!UART0_THR_EMPTY());         // Block until THR empty
    SSP1_SET_CHAR(c);
    return 0;
#endif
}

int8 getcharUart0(char *c)
{
#if (USE_UART_BUFFER == 1)
    return getCb(&uart0ReadBuffer,c);   // Read char from circular buffer
#else
    while( UART0_RBR_EMPTY() );         // Nothing received so just block 
    *c = UART0_READ_CHAR();             // Read Receiver buffer register
    return 0;
#endif
}
