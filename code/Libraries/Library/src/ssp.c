#include <ssp.h>

CircularBuffer ssp1ReadBuffer;
CircularBuffer ssp1WriteBuffer;

int8 initializeSSP0(uint32 baudrate,SspDataSize dataSize,SspFrameFormat frameFormat)
{
    SSP_ENABLE_POWER(0);
    SSP0_SET_CORE_CLK();    
   SSP0_SET_PR(baudrate);                      // set Prescaler
 
   SSP0_SET_DATA_SIZE(dataSize);
   SSP0_SET_FRAME_FORMAT(frameFormat);

   
   SSP0_SET_SSEL;     
   SSP0_SET_SCK;
   SSP0_SET_MISO;
   SSP0_SET_MOSI;

   SSP0_ENABLE_IRQ();
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
   
    SSP1_SET_SSEL;
    SSP1_SET_SCK;
    SSP1_SET_MISO;
    SSP1_SET_MOSI;
    
    if (initializeCb(&ssp1ReadBuffer, SSP1_READ_BUFFER_SIZE, sizeof(uint16)) == -1)      // Initialize circular read buffer
        return -1;
    
    if (initializeCb(&ssp1WriteBuffer, SSP1_WRITE_BUFFER_SIZE, sizeof(uint16)) == -1)    //Initialize circular write buffer
        return -1;
    

   SSP1_ENABLE_IRQ();
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
        putCb(&ssp1ReadBuffer,(void*)(&c));
    }
    else                                        // Else interrupt comes from THRE or put function
    {
        if (getCb(&ssp1WriteBuffer,&c) == 0)   // If data is available
            SSP1_SET_CHAR(c);                  // Put data to the uart, disables interrupt
    }
}
#endif

int8 putcharSsp1(uint16 c)
{
#if (USE_SSP_BUFFER == 1)
    if (putCb(&ssp1WriteBuffer, &c) == 0)
    {
        if (SSP1_FIFO_RECEIVE_EMPTY)         // If THR is empty, trigger an interrupt to write data to the UART
            SSP1_SET_IRQS();
        
        return 0;
    }
    else 
        return -1;
#else
    while(!SSP1_FIFO_TRANSMIT_EMPTY);         // Block until THR empty
    SSP1_SET_CHAR(c);
    return 0;
#endif
}

int8 getcharSsp1(uint16 *c)
{
#if (USE_SSP_BUFFER == 1)
    return getCb(&ssp1ReadBuffer,c);   // Read char from circular buffer
#else
    while(SSPI_FIFO_RECEIVE_EMPTY);         // Nothing received so just block 
    *c = SSP1_READ_CHAR();             // Read Receiver buffer register
    return 0;
#endif
}
