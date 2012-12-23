#include "wifly.h"

char commChar = '$';
char *commCloseString = "*CLOS*";
char *commOpenString = "*OPEN*";
char *commRemoteString = "*HELLO*";


int8 initializeWiFly(void)
{
    initializeUart1(9600);  // 9600 Baud, 1 Stop bit, No parity, No Hardware flow control
}

void setWiFlyAdhocBeacon(uint32 ms)
{
    printfUart1("set a b %u\r", ms);
}

void setWiFlyAdhocProbe(uint32 num)
{
    printfUart1("set a p %u\r", num);
}

void setWiFlyBroadcastAddress(char *address)
{
    printfUart1("set b a %s\r", address);
}

void setWiFlyBroadcastInterval(uint32 value)
{
    printfUart1("set b i %u\r", value);
}

void setWiFlyBroadcastPort(uint16 port)
{
    printfUart1("set b p %u\r", port);
}

void setWiFlyComm(char c)
{
    printfUart1("set c %c\r", c);
    commChar = c;
}

void setWiFlyCommClose(char *string)
{
    printfUart1("set c c %s\r", string);
    commCloseString = string;
}

void setWiFlyCommOpen(char *string)
{
    printfUart1("set c o %s\r", string);
    commOpenString = string;
}

void setWiFlyCommRemote(char *string)
{
    printfUart1("set c r %s\r", string);
    commRemoteString = string;
}

void setWiFlyCommIdle(uint32 secs)
{
    printfUart1("set c i %u\r", secs);
}

void setWiFlyCommMatch(uint32 value)
{
    printfUart1("set c m %u\r", value);
}

void setWiFlyCommSize(uint32 value)
{
    printfUart1("set c s %u\r", value);
}
