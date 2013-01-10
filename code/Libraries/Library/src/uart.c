#include <uart.h>
#include <stdlib.h>

CircularBuffer uart0ReadBuffer;
CircularBuffer uart0WriteBuffer;

CircularBuffer uart1ReadBuffer;
CircularBuffer uart1WriteBuffer;

CircularBuffer uart2ReadBuffer;
CircularBuffer uart2WriteBuffer;

CircularBuffer uart3ReadBuffer;
CircularBuffer uart3WriteBuffer;

char *taskBuffer0;
char *taskBuffer1;
char *taskBuffer2;
char *taskBuffer3;

uint16 taskBufferPos0 = 0;
uint16 taskBufferPos1 = 0;
uint16 taskBufferPos2 = 0;
uint16 taskBufferPos3 = 0;

void (* taskFunctionPointer0)(char *);
void (* taskFunctionPointer1)(char *);
void (* taskFunctionPointer2)(char *);
void (* taskFunctionPointer3)(char *);

void (* errorFunctionPointer0)(void);
void (* errorFunctionPointer1)(void);
void (* errorFunctionPointer2)(void);
void (* errorFunctionPointer3)(void);

int8 initializeUart0(uint32 baudrate)
{
#if (USE_UART_TASK == 1)
    taskBuffer0 = calloc(UART_TASK_BUFFER_SIZE, sizeof(char));
    if (taskBuffer0 == NULL)
        return -1;
    
    taskFunctionPointer0 = NULL;
    errorFunctionPointer0 = NULL;
#endif
    
    UART0_POWER_ON();               // Turn on power to UART0
    UART0_ENABLE_CLK();             // Turn on UART0 peripheral clock
    
    UART0_CONFIGURE();              // 8 bits, no Parity, 1 Stop bit
    UART0_SET_DLAB();               // set Divisor Latch Accress Bit
    UART0_SET_BAUDRATE(baudrate);   // Set baud rate
    UART0_CLEAR_DLAB();
   
    UART0_ENABLE_PINS();            // Enable UART TXD and RXD Pins
    UART0_ENABLE_AND_RESET_FIFO();  // Enable and reset TX and RX FIFO
    
#if (USE_UART_BUFFER == 1)
    if (initializeCb(&uart0ReadBuffer, UART_READ_BUFFER_SIZE, sizeof(char)) == -1)      // Initialize circular read buffer
        return -1;
    
    if (initializeCb(&uart0WriteBuffer, UART_WRITE_BUFFER_SIZE, sizeof(char)) == -1)    //Initialize circular write buffer
        return -1;
    
    UART0_SET_RBR_AND_THRE_INTERRUPT();      // Set the Receiver Data Ready interrupt
    UART0_ENABLE_IRQ();             // Enable Uarts Interrupt
#endif
    
    return 0;
}

int8 initializeUart1(uint32 baudrate)
{
#if (USE_UART_TASK == 1)
    taskBuffer1 = calloc(UART_TASK_BUFFER_SIZE, sizeof(char));
    if (taskBuffer1 == NULL)
        return -1;
    
    taskFunctionPointer1 = NULL;
    errorFunctionPointer1 = NULL;
#endif
    
    UART1_POWER_ON();               // Turn on power to UART1
    UART1_ENABLE_CLK();             // Turn on UART1 peripheral clock
    
    UART1_CONFIGURE();              // 8 bits, no Parity, 1 Stop bit
    UART1_SET_DLAB();               // set Divisor Latch Accress Bit
    UART1_SET_BAUDRATE(baudrate);   // Set baud rate
    UART1_CLEAR_DLAB();
   
    UART1_ENABLE_PINS();            // Enable UART TXD and RXD Pins
    UART1_ENABLE_AND_RESET_FIFO();  // Enable and reset TX and RX FIFO
    
#if (USE_UART_BUFFER == 1)
    if (initializeCb(&uart1ReadBuffer, UART_READ_BUFFER_SIZE, sizeof(char)) == -1)      // Initialize circular read buffer
        return -1;
    
    if (initializeCb(&uart1WriteBuffer, UART_WRITE_BUFFER_SIZE, sizeof(char)) == -1)    //Initialize circular write buffer
        return -1;
    
    UART1_SET_RBR_AND_THRE_INTERRUPT();      // Set the Receiver Data Ready interrupt
    UART1_ENABLE_IRQ();             // Enable Uarts Interrupt
#endif
    
    return 0;
}

int8 initializeUart2(uint32 baudrate)
{
#if (USE_UART_TASK == 1)
    taskBuffer2 = calloc(UART_TASK_BUFFER_SIZE, sizeof(char));
    if (taskBuffer2 == NULL)
        return -1;
    
    taskFunctionPointer2 = NULL;
    errorFunctionPointer2 = NULL;
#endif
    
    UART2_POWER_ON();               // Turn on power to UART2
    UART2_ENABLE_CLK();             // Turn on UART2 peripheral clock
    
    UART2_CONFIGURE();              // 8 bits, no Parity, 1 Stop bit
    UART2_SET_DLAB();               // set Divisor Latch Accress Bit
    UART2_SET_BAUDRATE(baudrate);   // Set baud rate
    UART2_CLEAR_DLAB();
   
    UART2_ENABLE_PINS();            // Enable UART TXD and RXD Pins
    UART2_ENABLE_AND_RESET_FIFO();  // Enable and reset TX and RX FIFO
    
#if (USE_UART_BUFFER == 1)
    if (initializeCb(&uart2ReadBuffer, UART_READ_BUFFER_SIZE, sizeof(char)) == -1)      // Initialize circular read buffer
        return -1;
    
    if (initializeCb(&uart2WriteBuffer, UART_WRITE_BUFFER_SIZE, sizeof(char)) == -1)    //Initialize circular write buffer
        return -1;
    
    UART2_SET_RBR_AND_THRE_INTERRUPT();      // Set the Receiver Data Ready interrupt
    UART2_ENABLE_IRQ();             // Enable Uarts Interrupt
#endif
    
    return 0;
}

int8 initializeUart3(uint32 baudrate)
{
#if (USE_UART_TASK == 1)
    taskBuffer3 = calloc(UART_TASK_BUFFER_SIZE, sizeof(char));
    if (taskBuffer3 == NULL)
        return -1;
    
    taskFunctionPointer3 = NULL;
    errorFunctionPointer3 = NULL;
#endif
    
    UART3_POWER_ON();               // Turn on power to UART3
    UART3_ENABLE_CLK();             // Turn on UART3 peripheral clock
    
    UART3_CONFIGURE();              // 8 bits, no Parity, 1 Stop bit
    UART3_SET_DLAB();               // set Divisor Latch Accress Bit
    UART3_SET_BAUDRATE(baudrate);   // Set baud rate
    UART3_CLEAR_DLAB();
   
    UART3_ENABLE_PINS();            // Enable UART TXD and RXD Pins
    UART3_ENABLE_AND_RESET_FIFO();  // Enable and reset TX and RX FIFO
    
#if (USE_UART_BUFFER == 1)
    if (initializeCb(&uart3ReadBuffer, UART_READ_BUFFER_SIZE, sizeof(char)) == -1)      // Initialize circular read buffer
        return -1;
    
    if (initializeCb(&uart3WriteBuffer, UART_WRITE_BUFFER_SIZE, sizeof(char)) == -1)    //Initialize circular write buffer
        return -1;
    
    UART3_SET_RBR_AND_THRE_INTERRUPT();      // Set the Receiver Data Ready interrupt
    UART3_ENABLE_IRQ();             // Enable Uarts Interrupt
#endif
    
    return 0;
}

#if (USE_UART_BUFFER == 1)
void UART0_IRQHANDLER()
{
    char c;
    
    if (UART0_RBR_INTERRUPT_OCCURED())          // If the interrupt comes from RBR
    {
        c = UART0_READ_CHAR();                  // Read character and put it to the buffer, also disables interrupt
        putCb(&uart0ReadBuffer,(void*)(&c));
    }
    else                                        // Else interrupt comes from THRE or put function
    {
        if (getCb(&uart0WriteBuffer,&c) == 0)   // If data is available
            UART0_SET_CHAR(c);                  // Put data to the uart, disables interrupt
    }
}
#endif

#if (USE_UART_BUFFER == 1)
void UART1_IRQHANDLER()
{
    char c;
    
    if (UART1_RBR_INTERRUPT_OCCURED())          // If the interrupt comes from RBR
    {
        c = UART1_READ_CHAR();                  // Read character and put it to the buffer, also disables interrupt
        putCb(&uart1ReadBuffer,(void*)(&c));
    }
    else                                        // Else interrupt comes from THRE or put function
    {
        if (getCb(&uart1WriteBuffer,&c) == 0)   // If data is available
            UART1_SET_CHAR(c);                  // Put data to the uart, disables interrupt
    }
}
#endif

#if (USE_UART_BUFFER == 1)
void UART2_IRQHANDLER()
{
        char c;
    
    if (UART2_RBR_INTERRUPT_OCCURED())          // If the interrupt comes from RBR
    {
        c = UART2_READ_CHAR();                  // Read character and put it to the buffer, also disables interrupt
        putCb(&uart2ReadBuffer,(void*)(&c));
    }
    else                                        // Else interrupt comes from THRE or put function
    {
        if (getCb(&uart2WriteBuffer,&c) == 0)   // If data is available
            UART2_SET_CHAR(c);                  // Put data to the uart, disables interrupt
    }
}
#endif

#if (USE_UART_BUFFER == 1)
void UART3_IRQHANDLER()
{
    char c;
    
    if (UART3_RBR_INTERRUPT_OCCURED())          // If the interrupt comes from RBR
    {
        c = UART3_READ_CHAR();                  // Read character and put it to the buffer, also disables interrupt
        putCb(&uart3ReadBuffer,(void*)(&c));
    }
    else                                        // Else interrupt comes from THRE or put function
    {
        if (getCb(&uart3WriteBuffer,&c) == 0)   // If data is available
            UART3_SET_CHAR(c);                  // Put data to the uart, disables interrupt
    }
}
#endif

int8 putcharUart0(char c)
{
#if (USE_UART_BUFFER == 1)
    if (putCb(&uart0WriteBuffer, &c) == 0)
    {
        if (UART0_THR_EMPTY())         // If THR is empty, trigger an interrupt to write data to the UART
            UART0_SET_IRQS();
        
        return 0;
    }
    else 
        return -1;
#else
    while(!UART0_THR_EMPTY());         // Block until THR empty
    UART0_SET_CHAR(c);
    return 0;
#endif
}

int8 putcharUart1(char c)
{
#if (USE_UART_BUFFER == 1)
    if (putCb(&uart1WriteBuffer, &c) == 0)
    {
        if (UART1_THR_EMPTY())         // If THR is empty, trigger an interrupt to write data to the UART
            UART1_SET_IRQS();
        
        return 0;
    }
    else 
        return -1;
#else
    while(!UART1_THR_EMPTY());         // Block until THR empty
    UART1_SET_CHAR(c);
    return 0;
#endif
}

int8 putcharUart2(char c)
{
#if (USE_UART_BUFFER == 1)
    if (putCb(&uart2WriteBuffer, &c) == 0)
    {
        if (UART2_THR_EMPTY())         // If THR is empty, trigger an interrupt to write data to the UART
            UART2_SET_IRQS();
        
        return 0;
    }
    else 
        return -1;
#else
    while(!UART2_THR_EMPTY());         // Block until THR empty
    UART2_SET_CHAR(c);
    return 0;
#endif
}

int8 putcharUart3(char c)
{
#if (USE_UART_BUFFER == 1)
    if (putCb(&uart3WriteBuffer, &c) == 0)
    {
        if (UART3_THR_EMPTY())         // If THR is empty, trigger an interrupt to write data to the UART
            UART3_SET_IRQS();
        
        return 0;
    }
    else 
        return -1;
#else
    while(!UART3_THR_EMPTY());         // Block until THR empty
    UART3_SET_CHAR(c);
    return 0;
#endif
}

int8 writeDataUart0(void *data, uint32 length)
{
    uint32 i;
    
    for (i = 0; i < length; i++)
    {
        if (putcharUart0(*(char*)(data+i)) == -1)
            return -1;
    }
    return 0;
}

int8 writeDataUart1(void *data, uint32 length)
{
    uint32 i;
    
    for (i = 0; i < length; i++)
    {
        if (putcharUart1(*(char*)(data+i)) == -1)
            return -1;
    }
    return 0;
}

int8 writeDataUart2(void *data, uint32 length)
{
    uint32 i;
    
    for (i = 0; i < length; i++)
    {
        if (putcharUart2(*(char*)(data+i)) == -1)
            return -1;
    }
    return 0;
}

int8 writeDataUart3(void *data, uint32 length)
{
    uint32 i;
    
    for (i = 0; i < length; i++)
    {
        if (putcharUart3(*(char*)(data+i)) == -1)
            return -1;
    }
    return 0;
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

int8 getcharUart1(char *c)
{
#if (USE_UART_BUFFER == 1)
    return getCb(&uart1ReadBuffer,c);   // Read char from circular buffer
#else
    while( UART1_RBR_EMPTY() );         // Nothing received so just block 
    *c = UART1_READ_CHAR();             // Read Receiver buffer register
    return 0;
#endif
}

int8 getcharUart2(char *c)
{
#if (USE_UART_BUFFER == 1)
    return getCb(&uart2ReadBuffer,c);   // Read char from circular buffer
#else
    while( UART2_RBR_EMPTY() );         // Nothing received so just block 
    *c = UART2_READ_CHAR();             // Read Receiver buffer register
    return 0;
#endif
}

int8 getcharUart3(char *c)
{
#if (USE_UART_BUFFER == 1)
    return getCb(&uart3ReadBuffer,c);   // Read char from circular buffer
#else
    while( UART3_RBR_EMPTY() );         // Nothing received so just block 
    *c = UART3_READ_CHAR();             // Read Receiver buffer register
    return 0;
#endif
}

int8 printfUart0(char *format, ...)
{
    static char buffer[UART_PRINTF_BUFFER_SIZE];
    
    va_list arg_ptr;
    uint16 i = 0;
    
    va_start(arg_ptr,format);
    vsnprintf(buffer,UART_PRINTF_BUFFER_SIZE, format, arg_ptr);
    va_end(arg_ptr);
    
    while (buffer[i] != 0)      // Loop through until reach string's zero terminator
    {
        if (putcharUart0(buffer[i]) == -1)
            return -1;
        i++;
    }
    
    return 0;
}

int8 printfUart1(char *format, ...)
{
    static char buffer[UART_PRINTF_BUFFER_SIZE];
    
    va_list arg_ptr;
    uint16 i = 0;
    
    va_start(arg_ptr,format);
    vsnprintf(buffer, UART_PRINTF_BUFFER_SIZE, format, arg_ptr);
    va_end(arg_ptr);
    
    while (buffer[i] != 0)      // Loop through until reach string's zero terminator
    {
        if (putcharUart1(buffer[i]) == -1)
            return -1;
        i++;
    }
    
    return 0;
}

int8 printfUart2(char *format, ...)
{
    static char buffer[UART_PRINTF_BUFFER_SIZE];
    
    va_list arg_ptr;
    uint16 i = 0;
    
    va_start(arg_ptr,format);
    vsnprintf(buffer, UART_PRINTF_BUFFER_SIZE, format, arg_ptr);
    va_end(arg_ptr);
    
    while (buffer[i] != 0)      // Loop through until reach string's zero terminator
    {
        if (putcharUart2(buffer[i]) == -1)
            return -1;
        i++;
    }
    
    return 0;
}

int8 printfUart3(char *format, ...)
{
    static char buffer[UART_PRINTF_BUFFER_SIZE];
    
    va_list arg_ptr;
    uint16 i = 0;
    
    va_start(arg_ptr,format);
    vsnprintf(buffer, UART_PRINTF_BUFFER_SIZE, format, arg_ptr);
    va_end(arg_ptr);
    
    while (buffer[i] != 0)      // Loop through until reach string's zero terminator
    {
        if (putcharUart3(buffer[i]) == -1)
            return -1;
        i++;
    }
    
    return 0;
}

void flushUart0(void)
{
    char byteRead;
    while (getCb(&uart0ReadBuffer, (void*)&byteRead) == 0)
        ;
    while (getCb(&uart0WriteBuffer, (void*)&byteRead) == 0)
        ;
}

void flushUart1(void)
{
    char byteRead;
    while (getCb(&uart1ReadBuffer, (void*)&byteRead) == 0)
        ;
    while (getCb(&uart1WriteBuffer, (void*)&byteRead) == 0)
        ;
}

void flushUart2(void)
{
    char byteRead;
    while (getCb(&uart2ReadBuffer, (void*)&byteRead) == 0)
        ;
    while (getCb(&uart2WriteBuffer, (void*)&byteRead) == 0)
        ;
}

void flushUart3(void)
{
    char byteRead;
    while (getCb(&uart3ReadBuffer, (void*)&byteRead) == 0)
        ;
    while (getCb(&uart3WriteBuffer, (void*)&byteRead) == 0)
        ;
}

void setBaudrateUart0(uint32 baudrate)
{
    UART0_SET_DLAB();               // set Divisor Latch Accress Bit
    UART0_SET_BAUDRATE(baudrate);   // Set baud rate
    UART0_CLEAR_DLAB();
    
    UART0_ENABLE_AND_RESET_FIFO();  // Enable and reset TX and RX FIFO
}

void setBaudrateUart1(uint32 baudrate)
{
    UART1_SET_DLAB();               // set Divisor Latch Accress Bit
    UART1_SET_BAUDRATE(baudrate);   // Set baud rate
    UART1_CLEAR_DLAB();
    
    UART1_ENABLE_AND_RESET_FIFO();  // Enable and reset TX and RX FIFO
}

void setBaudrateUart2(uint32 baudrate)
{
    UART2_SET_DLAB();               // set Divisor Latch Accress Bit
    UART2_SET_BAUDRATE(baudrate);   // Set baud rate
    UART2_CLEAR_DLAB();
    
    UART2_ENABLE_AND_RESET_FIFO();  // Enable and reset TX and RX FIFO
}

void setBaudrateUart3(uint32 baudrate)
{
    UART3_SET_DLAB();               // set Divisor Latch Accress Bit
    UART3_SET_BAUDRATE(baudrate);   // Set baud rate
    UART3_CLEAR_DLAB();
    
    UART3_ENABLE_AND_RESET_FIFO();  // Enable and reset TX and RX FIFO
}

#if (USE_UART_TASK == 1)

void setProcessFunctionUart0(void (* func)(char *))
{
    taskFunctionPointer0 = func;
}

void setProcessFunctionUart1(void (* func)(char *))
{
    taskFunctionPointer1 = func;
}

void setProcessFunctionUart2(void (* func)(char *))
{
    taskFunctionPointer2 = func;
}

void setProcessFunctionUart3(void (* func)(char *))
{
    taskFunctionPointer3 = func;
}

void setErrorFunctionUart0(void (* func)())
{
    errorFunctionPointer0 = func;
}

void setErrorFunctionUart1(void (* func)())
{
    errorFunctionPointer1 = func;
}

void setErrorFunctionUart2(void (* func)())
{
    errorFunctionPointer2 = func;
}

void setErrorFunctionUart3(void (* func)())
{
    errorFunctionPointer3 = func;
}

void processTaskUart0()
{
    static char receivedData;
    while (getcharUart0(&receivedData) == 0)
    {
        if ((receivedData != UART0_COMMAND_CHAR) && 
            (receivedData != UART0_SECONDARY_COMMAND_CHAR))
        {
            taskBuffer0[taskBufferPos0] = receivedData;
            if (taskBufferPos0 < UART_TASK_BUFFER_SIZE-1)
            {
                taskBufferPos0++;
            }
            else
            {
                (*errorFunctionPointer0)();
                taskBufferPos0 = 0;
            }
        }
        else
        {
            taskBuffer0[taskBufferPos0] = '\0';
            (*taskFunctionPointer0)(taskBuffer0);
            taskBufferPos0 = 0;
        }
    }
}

void processTaskUart1()
{
    static char receivedData;
    while (getcharUart1(&receivedData) == 0)
    {
        if ((receivedData != UART1_COMMAND_CHAR) && 
            (receivedData != UART1_SECONDARY_COMMAND_CHAR))
        {
            taskBuffer1[taskBufferPos1] = receivedData;
            if (taskBufferPos1 < UART_TASK_BUFFER_SIZE-1)
            {
                taskBufferPos1++;
            }
            else
            {
                (*errorFunctionPointer1)();
                taskBufferPos1 = 0;
            }
        }
        else
        {
            taskBuffer1[taskBufferPos1] = '\0';
            (*taskFunctionPointer1)(taskBuffer1);
            taskBufferPos1 = 0;
        }
    }
}

void processTaskUart2()
{
    static char receivedData;
    while (getcharUart2(&receivedData) == 0)
    {
        if ((receivedData != UART2_COMMAND_CHAR) && 
            (receivedData != UART2_SECONDARY_COMMAND_CHAR))
        {
            taskBuffer2[taskBufferPos2] = receivedData;
            if (taskBufferPos2 < UART_TASK_BUFFER_SIZE-1)
            {
                taskBufferPos2++;
            }
            else
            {
                (*errorFunctionPointer2)();
                taskBufferPos2 = 0;
            }
        }
        else
        {
            taskBuffer2[taskBufferPos2] = '\0';
            (*taskFunctionPointer2)(taskBuffer2);
            taskBufferPos2 = 0;
        }
    }
}

void processTaskUart3()
{
    static char receivedData;
    while (getcharUart3(&receivedData) == 0)
    {
        if ((receivedData != UART3_COMMAND_CHAR) && 
            (receivedData != UART3_SECONDARY_COMMAND_CHAR))
        {
            taskBuffer3[taskBufferPos3] = receivedData;
            if (taskBufferPos3 < UART_TASK_BUFFER_SIZE-1)
            {
                taskBufferPos3++;
            }
            else
            {
                (*errorFunctionPointer3)();
                taskBufferPos3 = 0;
            }
        }
        else
        {
            taskBuffer3[taskBufferPos3] = '\0';
            (*taskFunctionPointer3)(taskBuffer3);
            taskBufferPos3 = 0;
        }
    }
}
#endif
