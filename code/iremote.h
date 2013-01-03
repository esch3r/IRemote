/**
 * @file iremote.h
 * @author Alexander Rössler
 * @brief IRemote specific functions
 * @date 4-01-2012
 */

#pragma once

#define PRINTF_BUFFER_SIZE  256

#include <wifly.h>
#include <uart.h>

typedef enum {
        SerialConnection = 0x01,
        NetworkConnection = 0x02
    } ActiveConnection;
    
int8 initializeSerialConnection(void);
int8 initializeNetworkConnection(void);
    
int8 printfData(char* format, ...);
int8 writeData(void *data, uint32 length);
