/**
 * @file circularbuffer.h
 * @author Alexander Rössler
 * @brief Circular Buffer Library
 * @date 07-11-2012
 */
#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_

#include <types.h>


/** Initialize this structure with the initializeCb function! */
typedef struct
{
    void *inPointer;        /** Pointer holding the write position */
    void *outPointer;       /** Pointer holding the read position */
    void *startPointer;     /** Pointer to the beginning of the data */
    uint16     dataSize;    /** Size of one element */
    uint16     bufferSize;  /** Size of the buffer in bytes */
    
} CircularBuffer;

/** Initializes a circular buffer.
 *  @param buffer The buffer to initialize,
 *  @param bufferSize The number of elements the buffer should store (the usable elements will be bufferSize-1)
 *  @param dataSize The size of one element in bytes.
 *  @return Returns -1 if not enough memory is available and 0 if successful.
 */
int8 initializeCb(CircularBuffer *buffer, uint16 bufferSize, uint16 dataSize);
/** Puts one element to the circular buffer
 *  @param buffer Target buffer.
 *  @param item   Pointer to the element to add, only the data of the element will be used.
 *  @return Returns -1 if the buffer is full and 0 if successful.
 */
int8 putCb(CircularBuffer *buffer, void *item);
/** Get one element from the circular buffer.
 *  @param buffer Target buffer.
 *  @param item Pointer to the element where the data should be stored.
 *  @return Returns -1 if the buffer is empty and 0 if successful.
 */
int8 getCb(CircularBuffer *buffer, void *item);


#endif
