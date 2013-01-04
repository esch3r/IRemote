/**
 * @file uart.h
 * @author Alexander Rössler
 * @brief UART Library
 * @date 06-11-2012
 */

#ifndef _UART_H_
#define _UART_H_

/** Size of UARTs circular buffer for reading */
#ifndef UART_READ_BUFFER_SIZE
#define UART_READ_BUFFER_SIZE 250
#endif
/** Size of UARTs circular buffer for writing */
#ifndef UART_WRITE_BUFFER_SIZE
#define UART_WRITE_BUFFER_SIZE 250
#endif
/** Defines wheter the UART buffer should be used or not */
#ifndef USE_UART_BUFFER
#define USE_UART_BUFFER 1
#endif

#define UART_PRINTF_BUFFER_SIZE 256

#include <uartDriver.h>
#include <circularbuffer.h>
#include <types.h>

CircularBuffer uart0ReadBuffer;
CircularBuffer uart0WriteBuffer;

CircularBuffer uart1ReadBuffer;
CircularBuffer uart1WriteBuffer;

CircularBuffer uart2ReadBuffer;
CircularBuffer uart2WriteBuffer;

CircularBuffer uart3ReadBuffer;
CircularBuffer uart3WriteBuffer;

/** Initializes UART0.
 *  The UART is configured with following configuration: 8 bits, no Parity, 1 Stop bit
 *  @param baudrate The baudrate the UART should run with
 */
int8 initializeUart0(uint32 baudrate);
/** Initializes UART1.
 *  The UART is configured with following configuration: 8 bits, no Parity, 1 Stop bit
 *  @param baudrate The baudrate the UART should run with
 */
int8 initializeUart1(uint32 baudrate);
/** Initializes UART2.
 *  The UART is configured with following configuration: 8 bits, no Parity, 1 Stop bit
 *  @param baudrate The baudrate the UART should run with
 */
int8 initializeUart2(uint32 baudrate);
/** Initializes UART3.
 *  The UART is configured with following configuration: 8 bits, no Parity, 1 Stop bit
 *  @param baudrate The baudrate the UART should run with
 */
int8 initializeUart3(uint32 baudrate);

/** Function to send character over UART0.
 *  @param c The character to send.
 *  @return 0 if successful -1 if nothing to write
 */
int8 putcharUart0(char c);
/** Function to send character over UART1.
 *  @param c The character to send.
 *  @return 0 if successful -1 if nothing to write
 */
int8 putcharUart1(char c);
/** Function to send character over UART2.
 *  @param c The character to send.
 *  @return 0 if successful -1 if nothing to write
 */
int8 putcharUart2(char c);
/** Function to send character over UART3.
 *  @param c The character to send.
 *  @return 0 if successful -1 if nothing to write
 */
int8 putcharUart3(char c);

/** Function to write a defined a number of bytes from memory to UART0.
 * @param data Pointer to the data.
 * @param length Number of bytes to write.
 * @return 0 if successful -1 if nothing to write
 */
int8 writeDataUart0(void *data, uint32 length);
/** Function to write a defined a number of bytes from memory to UART1.
 * @param data Pointer to the data.
 * @param length Number of bytes to write.
 * @return 0 if successful -1 if nothing to write
 */
int8 writeDataUart1(void *data, uint32 length);
/** Function to write a defined a number of bytes from memory to UART2.
 * @param data Pointer to the data.
 * @param length Number of bytes to write.
 * @return 0 if successful -1 if nothing to write
 */
int8 writeDataUart2(void *data, uint32 length);
/** Function to write a defined a number of bytes from memory to UART3.
 * @param data Pointer to the data.
 * @param length Number of bytes to write.
 * @return 0 if successful -1 if nothing to write
 */
int8 writeDataUart3(void *data, uint32 length);

/** Function to get a character from UART0.
 *  @param c Pointer to the received character.
 *  @return 0 if successful -1 if nothing to read
 */
int8 getcharUart0(char* c);
/** Function to get a character from UART1.
 *  @param c Pointer to the received character.
 *  @return 0 if successful -1 if nothing to reading
 */
int8 getcharUart1(char* c);
/** Function to get a character from UART2.
 *  @param c Pointer to the received character.
 *  @return 0 if successful -1 if nothing to reading
 */
int8 getcharUart2(char* c);
/** Function to get a character from UART3.
  *  @param c Pointer to the received character.
  *  @return 0 if successful -1 if nothing to reading
 */
int8 getcharUart3(char* c);

/** Custom printf function for UART0.
 *  @param format Formated string.
 *  @param ... Formatting parameters.
 *  @return 0 if successful -1 if nothing to write
 */
int8 printfUart0(char* format, ...);
/** Custom printf function for UART1.void
 *  @param format Formated string.
 *  @param ... Formatting parameters.
 *  @return 0 if successful -1 if nothing to write
 */
int8 printfUart1(char* format, ...);
/** Custom printf function for UART2.
 *  @param format Formated string.
 *  @param ... Formatting parameters.
 *  @return 0 if successful -1 if nothing to write
 */
int8 printfUart2(char* format, ...);
/** Custom printf function for UART3.
 *  @param format Formated string.
 *  @param ... Formatting parameters.
 *  @return 0 if successful -1 if nothing to write
 */
int8 printfUart3(char* format, ...);

void setBaudrateUart1(uint32 baudrate);

#endif /*UART_H_*/
