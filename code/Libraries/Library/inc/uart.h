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

/** Defines wheter a UART task to process incoming commands should be used or not. */
#ifndef USE_UART_TASK
#define USE_UART_TASK 1
#endif

#define UART_PRINTF_BUFFER_SIZE 256
#define UART_TASK_BUFFER_SIZE   256

#define UART0_COMMAND_CHAR  13  // Carriage Return (CR)
#define UART1_COMMAND_CHAR  13
#define UART2_COMMAND_CHAR  13
#define UART3_COMMAND_CHAR  13

#define UART1_SECONDARY_COMMAND_CHAR    '\a'

#include <uartDriver.h>
#include <circularbuffer.h>
#include <types.h>
#include <stdarg.h>
#include <stdio.h>

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

/** Cleans in and output buffers of UART0. */
void flushUart0(void);  
/** Cleans in and output buffers of UART1. */
void flushUart1(void);  
/** Cleans in and output buffers of UART2. */
void flushUart2(void);  
/** Cleans in and output buffers of UART3. */
void flushUart3(void);  


/** Changes the baudrate of UART0
 *  @param baudrate The target baudrate.
 */
void setBaudrateUart0(uint32 baudrate);
/** Changes the baudrate of UART1
 *  @param baudrate The target baudrate.
 */
void setBaudrateUart1(uint32 baudrate);
/** Changes the baudrate of UART2
 *  @param baudrate The target baudrate.
 */
void setBaudrateUart2(uint32 baudrate);
/** Changes the baudrate of UART3
 *  @param baudrate The target baudrate.
 */
void setBaudrateUart3(uint32 baudrate);

#if (USE_UART_TASK == 1)
/** Sets the command processing function for UART0, 
 *  only needed if process tasks are used.
 *  @param func Pointer to the function that should be called when a command arives
 */ 
void setProcessFunctionUart0(void (* func)(char *));
/** Sets the command processing function for UART1, 
 *  only needed if process tasks are used.
 *  @param func Pointer to the function that should be called when a command arives
 */ 
void setProcessFunctionUart1(void (* func)(char *));
/** Sets the command processing function for UART2, 
 *  only needed if process tasks are used.
 *  @param func Pointer to the function that should be called when a command arives
 */ 
void setProcessFunctionUart2(void (* func)(char *));
/** Sets the command processing function for UART3, 
 *  only needed if process tasks are used.
 *  @param func Pointer to the function that should be called when a command arives
 */ 
void setProcessFunctionUart3(void (* func)(char *));

/** Sets the command error function for UART0, 
 *  only needed if process tasks are used.
 *  @param func Pointer to the function that should be called when a error occures
 */ 
void setErrorFunctionUart0(void (* func)());
/** Sets the command error function for UART1, 
 *  only needed if process tasks are used.
 *  @param func Pointer to the function that should be called when a error occures
 */ 
void setErrorFunctionUart1(void (* func)());
/** Sets the command error function for UART2, 
 *  only needed if process tasks are used.
 *  @param func Pointer to the function that should be called when a error occures
 */ 
void setErrorFunctionUart2(void (* func)());
/** Sets the command error function for UART3, 
 *  only needed if process tasks are used.
 *  @param func Pointer to the function that should be called when a error occures
 */ 
void setErrorFunctionUart3(void (* func)());

/** Task function for UART0. This function processes incoming characters
 *  to find commands that end with a command char (see defines).
 */
void processTaskUart0();
/** Task function for UART1. This function processes incoming characters
 *  to find commands that end with a command char (see defines).
 */
void processTaskUart1();
/** Task function for UART2. This function processes incoming characters
 *  to find commands that end with a command char (see defines).
 */
void processTaskUart2();
/** Task function for UART3. This function processes incoming characters
 *  to find commands that end with a command char (see defines).
 */
void processTaskUart3();
#endif

#endif /*UART_H_*/
