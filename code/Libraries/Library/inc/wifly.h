/**
 * @file wifly.h
 * @author Alexander Rössler
 * @brief WiFly Library
 * @date 23-12-2012
 */

#pragma once

#include <uart.h>
#include <types.h>

int8 initializeWiFly(void);

/**  Sets the adhoc beacon interval in milliseconds. Default is 100. */
void setWiFlyAdhocBeacon(uint32 ms);
/**  Sets the adhoc probe timeout in seconds. This is the number of seconds
waiting for probe responses before declaring, “ADHOC is lost” and
disabling the network interface. Default is 60. */
void setWiFlyAdhocProbe(uint32 num);
/**  Sets the address to which the UDP hello/heartbeat message is sent. The
default address is 255.255.255.255 */

void setWiFlyBroadcastAddress(char *addr);
/**  Sets the interval at which the hello/heartbeat UDP message is sent.
Interval is specified in seconds. The value is a mask that is compared to a
free running seconds counter. For example if interval = 0x7, a packet will
be sent every 8 seconds. The minimum interval value is 1 (every 2
seconds) and max value is 0xff (every 256 seconds). Setting the interval
value to zero disables sending UDP broadcast messages. Default interval
is 7.*/
void setWiFlyBroadcastInterval(uint32 value);
/** Sets the port number to which the UDP hello/heartbeat message is sent.
Default port is 55555.*/
void setWiFlyBroadcastPort(uint16 port);

/** Sets character used to enter command mode. Typically used when “$$$” is
a possible data string. Care should be taken when setting this to note the
new character as once this setting is saved every subsequent reboot will
ignore “$$$” and look for “<char><char><char>”. Default is ‘$’ */
void setWiFlyComm(char c);
/** Sets the ASCI string that is sent to the local UART when the TCP port is
closed. If no string is desired, use 0 as the <string> parameter. Max string
length is 32 characters. Default is *CLOS* */
void setWiFlyCommClose(char *string);
/** Sets the string that is sent to the local UART when the TCP port is opened.
If no string is desired, use 0 as the <string> parameter. Max string length
is 32 characters. Default is *OPEN* */
void setWiFlyCommOpen(char *string);
/** Sets the string that is sent to the remote TCP client when the TCP port is
opened. If no string is desired, use 0 as the <string> parameter. Max string
length is 32 characters. Default is *HELLO* */
void setWiFlyCommRemote(char *string);
/** Sets the Idle Timer Value. This is the number of seconds with no transmit
or receive data over TCP before the connection is closed automatically.
Default is 0, never disconnect on idle. */
void setWiFlyCommIdle(uint32 secs);
/** Sets match character. An IP packet will be sent each time the match
character appears in the data. Value is entered either as decimal (13) or
hex (0xd) of the of the ASCII character. Default is 0, disabled. The match
character is one of three ways to control TCP/IP packet forwarding. The
others are comm size and comm timer. For more information refer to
section on UART Receiver. */
void setWiFlyCommMatch(uint32 value);
/** Sets the flush size. An IP packet will be sent each time “value” bytes are
received. Default is 64 bytes. It is recommended to set this value to the
largest possible setting to maximize TCP/IP performance. Maximum
value = 1420 (at 9600) bytes.
NOTE: This value is set automatically when the baudrate is set, in an
attempt to optimize the link. It is assumed that higher baudrates equates
to more data and hence the flush size is increased.
Flush size is one of three ways to control TCP/IP packet forwarding. The
others are match character and timer. For more information refer to
section on UART Receiver. */
void setWiFlyCommSize(uint32 value);
