#include "circularbuffer.h"
#include <stdlib.h>
#include <string.h>
#include <uart.h>

int8 initializeCb(CircularBuffer *buffer, uint16 bufferSize, uint16 dataSize)
{
    buffer->dataSize = dataSize;
    buffer->bufferSize = bufferSize*dataSize;
    buffer->startPointer = calloc(bufferSize,dataSize);
    
    if (buffer->startPointer == NULL)    //memory full
        return -1;
    
    buffer->inPointer = buffer->startPointer + buffer->dataSize;
    buffer->outPointer = buffer->startPointer;   //empty buffer
    
    return 0;
}

int8 putCb(CircularBuffer *buffer, void *item)
{
    if ((buffer->inPointer == (buffer->outPointer - buffer->dataSize)) || ((buffer->inPointer == (buffer->startPointer+buffer->bufferSize)) && (buffer->outPointer == buffer->startPointer)))  //buffer full
        return -1;
    
    memcpy(buffer->inPointer,item,buffer->dataSize);
    
    //increase the pointer
    if (buffer->inPointer == (buffer->startPointer+buffer->bufferSize))
    {
        buffer->inPointer = buffer->startPointer;
    }
    else
    {
        buffer->inPointer += buffer->dataSize;
    }
    
    return 0;
}

int8 getCb(CircularBuffer *buffer, void *item)
{
    if (buffer->outPointer == buffer->inPointer)   //buffer empty
        return -1;
    
    memcpy(item, buffer->outPointer, buffer->dataSize);
     
    //increase the pointer
    if (buffer->outPointer == (buffer->startPointer+buffer->bufferSize))
    {
        buffer->outPointer = buffer->startPointer;
    }
    else
    {
        buffer->outPointer += buffer->dataSize;
    }
    
    return 0;
}
