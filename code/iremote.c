#include "iremote.h"
#include <timer.h>
#include <stdarg.h>
#include <stdio.h>

ActiveConnection activeConnections = 0;

int8 initializeSerialConnection(void)
{
    if (initializeUart0(115200) == 0)
    {
        flushUart0();                           // Trash all unusefull characters
        activeConnections |= SerialConnection;
        return 0;
    }
    
    return -1;
}

int8 initializeNetworkConnection(void)
{
    if (initializeWiFly(115200) == 0)
    {
        if (actionWiFlyEnterCommandMode(FALSE) == 0)      // Configure the wlan module
        {
            setWiFlySysPrintlvl(0);             // Turn off unessesary messages
            setWiFlyWlanJoin(2);                // Auto join
            setWiFlyDnsName("IRemoteBox");      // Sets the default hostname
            setWiFlyDnsBackup("IRemote");       // Sets the backup hostname
            actionWiFlyExitCommandMode();
            return 0;
        }
        else
            return -1;
    }
    
    return -1;
}

int8 startWlanInfrastructureMode(ApplicationSettings *settings)
{
    if (actionWiFlyEnterCommandMode(FALSE) == -1)
        return -1;
    if (setWiflyInfrastructureParams() == -1)
        return -1;
    if (setWiFlyWlanSsid(settings->wlanSsid) == -1)
        return -1;
    if (setWiFlyWlanPhrase(settings->wlanPhrase) == -1)
        return -1;
    if (setWiFlyWlanKey(settings->wlanKey) == -1)
        return -1;
    if (setWiFlyDnsName(settings->wlanHostname) == -1)
        return -1;
    if (setWiFlyWlanAuth(settings->wlanAuth) == -1)
        return -1;
    if (setWiFlyIpDhcp(settings->wlanDhcp) == -1)
        return -1;
    if (setWiFlyIpAddress(settings->wlanIp) == -1)
        return -1;
    if (setWiFlyIpNetmask(settings->wlanMask) == -1)
        return -1;
    if (setWiFlyIpGateway(settings->wlanGateway) == -1)
        return -1;
    if (fileIoWiFlySaveDefault() == -1)
        return -1;
    return actionWiFlyReboot();
}

int8 startWlanAdhocMode(void)
{
    return createWiFlyAdhocNetwork("IRemoteBox");
}

int8 printfData(char* format, ... )
{
    static char buffer[PRINTF_BUFFER_SIZE];
    
    va_list arg_ptr;
    uint16 i = 0;
    
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

int8 putcharData(char c)
{  
    if (activeConnections & NetworkConnection)
    {
        return putcharWiFly(c);
    }
    else if (activeConnections & SerialConnection)
    {
        return putcharUart0(c);
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

void errorCommand()
{
    printfData("ERR: Command too long\r");
}

void errorWiFly()
{
    printfData("ERR: WiFly command too long\r");
}

void printUnknownCommand(void)
{
    printfData("CMD?\r");
    clearLed(2);
}

void printParameterMissing(void)
{
    printfData("Missing parameter.\r");
    clearLed(2);
}

void printAcknowledgement(void)
{
    printfData("ACK\r");
    clearLed(2);
}

void printError(char *message)
{
    printfData("ERR: %s\r", message);
}

void printAliveMessage(void)
{
    printfData("yes\r");
    clearLed(2);
}

bool compareBaseCommand(char *original, char *received)
{
    return (strcmp(original,received) == 0);
}

bool compareExtendedCommand(char *original, char *received)
{
    return (((strlen(received) == 1) && (strncmp(original,received,1) == 0)) ||
                (strcmp(original,received) == 0));
}

void ledTask(void )
{
    if (isWiFlyConnected()) // keeps also track of the connectedness
    {
        activeConnections |= NetworkConnection;
    }
    else
    {
        activeConnections &= ~NetworkConnection;
    }
    
    if ((activeConnections & NetworkConnection))
    {
        setLed(1);  // Green LED
    }
    else
    {
        toggleLed(1);   // Green LED
    }
    
    clearLed(2);    // clear the yellow led in case it is still running
}

uint32 hex2int(char *a, unsigned int len)
{
    uint32 i;
    uint32 val = 0;

    for(i=0;i<len;i++)
       if(a[i] <= 57)
        val += (a[i]-48)*(1<<(4*(len-1-i)));
       else
        val += (a[i]-87)*(1<<(4*(len-1-i)));
    return val;
}
