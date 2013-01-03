#include "iremote.h"
#include <stdarg.h>
#include <stdio.h>

ActiveConnection activeConnections = 0;

int8 initializeSerialConnection(void)
{
    char tmpChar;
        
    if (initializeUart0(115200) == 0)
    {
        while (getcharUart0(&tmpChar) == 0)     // Trash all unusefull characters
            ;
        activeConnections |= SerialConnection;
        return 0;
    }
    
    return -1;
}

int8 printfData(char* format, ... )
{
    char buffer[PRINTF_BUFFER_SIZE];
    
    va_list arg_ptr;
    uint8 i = 0;
    
    va_start(arg_ptr,format);
    vsnprintf(buffer, PRINTF_BUFFER_SIZE, format, arg_ptr);
    va_end(arg_ptr);
    
    if (activeConnections & NetworkConnection)
    {
        while (buffer[i] != 0)      // Loop through until reach string's zero terminator
        {
            if (putcharWiFly(buffer[i]) == -1)
                return -1;
            i++;
        }
        return 0;
    }
    else if (activeConnections & SerialConnection)
    {
        while (buffer[i] != 0)      // Loop through until reach string's zero terminator
        {
            if (putcharUart0(buffer[i]) == -1)
                return -1;
            i++;
        }
        return 0;
    }
    
    return -1;
}

int8 writeData(void *data, uint32 length)
{
    if (activeConnections & NetworkConnection)
    {
        return writeDataWiFly(data, length);
    }
    else if (activeConnections & SerialConnection)
    {
        return writeDataUart0(data, length);
    }
    
    return -1;
}
