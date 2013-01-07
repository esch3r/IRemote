#include "wifly.h"

#define WIFLY_PRINTF                printfUart1
#define WIFLY_WRITEDATA             writeDataUart1
#define WIFLY_PUTCHAR               putcharUart1
#define WIFLY_GETCHAR               getcharUart1
#define WIFLY_INIT                  initializeUart1
#define WIFLY_SET_PROCESS_FUNCTION  setProcessFunctionUart1
#define WIFLY_SET_ERROR_FUNCTION    setErrorFunctionUart1
#define WIFLY_PROCESS_TASK          processTaskUart1
#define WIFLY_SET_BAUDRATE          setBaudrateUart1
#define WIFLY_FLUSH                 flushUart1

#define WIFLY_STANDARD_BAUD 9600

#define WIFLY_COMMAND_MODE_ENTER_RETRY_ATTEMPTS 5
#define WIFLY_COMMAND_MODE_GUARD_TIME           250     // ms
#define WIFLY_COMMAND_SETTLE_TIME               20
#define WIFLY_VERSION_LENGTH                    4
#define WIFLY_SET_OK                            "AOK"
#define WIFLY_COMMAND_BUFFER_SIZE               256
#define WIFLY_RESPONSE_BUFFER_SIZE              256
#define WIFLY_RESPONSE_TIMEOUT                  1000

char commChar = '$';
char commCloseString[10];
char commOpenString[10];
char commRemoteString[10];
char wiFlyVersion[WIFLY_VERSION_LENGTH + 3];    // <,>, and \0

char commandBuffer[WIFLY_COMMAND_BUFFER_SIZE];
char responseBuffer[WIFLY_RESPONSE_BUFFER_SIZE];
uint16 responseBufferPos;

void (* taskFunctionPointer)(char *);

WiFlyState wiFlyState = WiFlyStateDisconnected;

int8 findInResponse(const char* toMatch, uint32 timeout);
int8 responseMatched(const char* toMatch);
int8 findWiFlyVersion(uint32 timeout);
int8 setCommand(char *command);
int8 getCommand(char* command);
int8 otherCommand(char *command, char* awaitedResponse);

void internalProcessTask(char* command);

int8 initializeWiFly(uint32 baudrate)
{
    if (WIFLY_INIT(baudrate) == -1)     // 9600 Baud, 1 Stop bit, No parity, No Hardware flow control
        return -1;
    
    WIFLY_FLUSH();                      // Trash unusefull chars
    
    if (actionWiFlyEnterCommandMode(FALSE) == -1)      // Configure the wlan module
        return -1;
    
    setWiFlyCommOpen("*OPEN*\a");        // Set the open command
    setWiFlyCommClose("*CLOS*\a");       // Set the close command
    setWiFlyCommRemote("*HELLO*\a");     // Set the remote command
    
    actionWiFlyExitCommandMode();
    
    WIFLY_SET_PROCESS_FUNCTION(&internalProcessTask);
    
    /*actionWiFlyEnterCommandMode();           // Enter command mode
    setWiFlyUartBaud(baudrate);              // Set baudrate instantly to target rate
    fileIoWiFlySaveDefault();
    actionWiFlyReboot();
    WIFLY_SET_BAUDRATE(baudrate);*/
    
    wiFlyState = WiFlyStateDisconnected;
    
    return 0;
}

int8 setWiFlyAdhocBeacon(uint32 ms)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set a b %u\r", ms);
    return setCommand(commandBuffer);
}

int8 setWiFlyAdhocProbe(uint32 num)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set a p %u\r", num);
    return setCommand(commandBuffer);
}

int8 setWiFlyBroadcastAddress(char *address)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set b a %s\r", address);
    return setCommand(commandBuffer);
}

int8 setWiFlyBroadcastInterval(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set b i %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyBroadcastPort(uint16 port)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set b p %u\r", port);
    return setCommand(commandBuffer);
}

int8 setWiFlyComm(char c)
{
    commChar = c;
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c %c\r", c);
    return setCommand(commandBuffer);
}

int8 setWiFlyCommClose(char *string)
{
    strncpy(commCloseString, string, 10);
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c c %s\r", string);
    return setCommand(commandBuffer);
}

int8 setWiFlyCommOpen(char *string)
{
    strncpy(commOpenString, string, 10);
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c o %s\r", string);
    return setCommand(commandBuffer);
}

int8 setWiFlyCommRemote(char *string)
{
    strncpy(commRemoteString, string, 10);
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c r %s\r", string);
    return setCommand(commandBuffer);
}

int8 setWiFlyCommIdle(uint32 secs)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c i %u\r", secs);
    return setCommand(commandBuffer);
}

int8 setWiFlyCommMatch(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c m %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyCommSize(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c s %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyCommTime(uint32 num)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set c t %u\r", num);
    return setCommand(commandBuffer);
}

int8 setWiFlyDnsAddr(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set d a %s\r", addr);
    return setCommand(commandBuffer);
}

int8 setWiFlyDnsName(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set d n %s\r", string);
    return setCommand(commandBuffer);
}

int8 setWiFlyDnsBackup(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set d b %s\r", string);
    return setCommand(commandBuffer);
}

int8 setWiFlyFtpAddr(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f a %s\r", addr);
    return setCommand(commandBuffer);
}

int8 setWiFlyFtpDir(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f d %s\r", string);
    return setCommand(commandBuffer);
}

int8 setWiFlyFtpFilename(char *file)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f f %s\r", file);
    return setCommand(commandBuffer);
}

int8 setWiFlyFtpMode(uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f m %x\r", mask);
    return setCommand(commandBuffer);
}

int8 setWiFlyFtpRemote(uint32 port)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f r %u\r", port);
    return setCommand(commandBuffer);
}

int8 setWiFlyFtpTime(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f t %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyFtpUser(char *name)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f u %s\r", name);
    return setCommand(commandBuffer);
}

int8 setWiFlyFtpPass(char *pass)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set f p %s\r", pass);
    return setCommand(commandBuffer);
}

int8 setWiFlyIpAddress(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i a %s\r", addr);
    return setCommand(commandBuffer);
}

int8 setWiFlyIpBackup(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i b %s\r", addr);
    return setCommand(commandBuffer);
}

int8 setWiFlyIpDhcp(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i d %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyIpFlags(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i f %x\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyIpGateway(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i g %s\r", addr);
    return setCommand(commandBuffer);
}

int8 setWiFlyIpHost(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i h %s\r", addr);
    return setCommand(commandBuffer);
}

int8 setWiFlyIpLocalport(uint32 num)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i l %u\r", num);
    return setCommand(commandBuffer);
}

int8 setWiFlyIpNetmask(char *value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i n %s\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyIpProtocol(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i p %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyIpRemote(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i r %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyIpTcpMode(uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set i t %x\r", mask);
    return setCommand(commandBuffer);
}

int8 setWiFlyOptJointmr(uint32 msecs)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set o j %u\r", msecs);
    return setCommand(commandBuffer);
}

int8 setWiFlyOptFormat(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set o f %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyOptReplace(char character)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set o r %c\r", character);
    return setCommand(commandBuffer);
}

int8 setWiFlyOptDeviceid(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set o d %s\r", string);
    return setCommand(commandBuffer);
}

int8 setWiFlyOptPassword(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set o p %s\r", string);
    return setCommand(commandBuffer);
}

int8 setWiFlySysAutoconn(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s a %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlySysAutosleep(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s a %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlySysIofunc(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s i %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlySysMask(uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s m %x\r", mask);
    return setCommand(commandBuffer);
}

int8 setWiFlySysPrintlvl(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s p %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlySysOutput(uint32 value, uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s o %x %x\r", value, mask);
    return setCommand(commandBuffer);
}

int8 setWiFlySysSleep(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s s %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlySysTrigger(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s t %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlySysValue(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s v %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlySysWake(uint32 secs)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set s w %u\r", secs);
    return setCommand(commandBuffer);
}

int8 setWiFlyTimeAddress(char *addr)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set t a %s\r", addr);
    return setCommand(commandBuffer);
}

int8 setWiFlyTimePort(uint32 num)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set t p %u\r", num);
    return setCommand(commandBuffer);
}

int8 setWiFlyTimeEnable(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set t e %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyTimeRaw(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set t r %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyUartBaud(uint32 rate)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u b %u\r", rate);
    return setCommand(commandBuffer);
}

int8 setWiFlyUartInstant(uint32 rate)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u i %u\r", rate);
    return setCommand(commandBuffer);
}

int8 setWiFlyUartRaw(uint32 rate)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u r %u\r", rate);
    return setCommand(commandBuffer);
}

int8 setWiFlyUartFlow(uint8 flow)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u f %u\r", flow);
    return setCommand(commandBuffer);
}

int8 setWiFlyUartMode(uint16 mode)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u m %x\r", mode);
    return setCommand(commandBuffer);
}

int8 setWiFlyUartTx(uint8 enabled)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set u t %u\r", enabled);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanAuth(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w a %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanChannel(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w c %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanExtAntenna(uint8 enabled)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w e %u\r", enabled);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanJoin(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w j %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanHide(uint8 enabled)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w h %u\r", enabled);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanKey(char *value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w k %s\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanLinkmon(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w l %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanMask(uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w m %x\r", mask);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanNum(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w n %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanPhrase(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w p %s\r", string);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanRate(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w r %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanSsid(char *string)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w s %s\r", string);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanTx(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w t %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyWlanWindow(uint32 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set w w %u\r", value);
    return setCommand(commandBuffer);
}

int8 setWiFlyQSensor(uint32 mask)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set q s %x\r", mask);
    return setCommand(commandBuffer);
}

int8 setWiFlyQPower(uint8 value)
{
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "set q p %u\r", value);
    return setCommand(commandBuffer);
}

void getWiFlyAdhoc()
{
    WIFLY_PRINTF("get a\r");
}

void getWiFlyBroadcast()
{
    WIFLY_PRINTF("get b\r");
}

void getWiFlyCom()
{
    WIFLY_PRINTF("get c\r");
}

void getWiFlyDns()
{
    WIFLY_PRINTF("get d\r");
}

void getWiFlyEverything()
{
    WIFLY_PRINTF("get e\r");
}

void getWiFlyFtp()
{
    WIFLY_PRINTF("get f\r");
}

void getWiFlyIp(uint8 a)
{
    if (a)
        WIFLY_PRINTF("get i a\r");
    else
        WIFLY_PRINTF("get i\r");
}

void getWiFlyMac()
{
    WIFLY_PRINTF("get m\r");
}

void getWiFlyOption()
{
    WIFLY_PRINTF("get o\r");
}

void getWiFlySys()
{
    WIFLY_PRINTF("get s\r");
}

void getWiFlyTime()
{
    WIFLY_PRINTF("get t\r");
}

void getWiFlyWlan()
{
    WIFLY_PRINTF("get w\r");
}

void getWiFlyUart()
{
    WIFLY_PRINTF("get u\r");
}

void getWiFlyVer()
{
    WIFLY_PRINTF("ver\r");
}

void showWiFlyBattery()
{
    WIFLY_PRINTF("show b\r");
}

void showWiFlyConnection()
{
    WIFLY_PRINTF("show c\r");
}

void showWiFlyIo()
{
    WIFLY_PRINTF("show i\r");
}

void showWiFlyNet(uint8 n)
{
    WIFLY_PRINTF("show n\r");
}

void showWiFlyRssi()
{
    WIFLY_PRINTF("show r\r");
}

void showWiFlyStats()
{
    WIFLY_PRINTF("show s\r");
}

void showWiFlyTime()
{
    WIFLY_PRINTF("show t\r");
}

void showWiFlyQ(uint8 num)
{
    WIFLY_PRINTF("show q %u\r", num);
}

void showWiFlyQ0x1(uint8 mask)
{
    WIFLY_PRINTF("show q 0x1%u\r", mask);
}

int8 actionWiFlyEnterCommandMode(uint8 isAfterReboot)
{
    uint8 retryCount;
    
    for (retryCount = 0; 
         retryCount < WIFLY_COMMAND_MODE_ENTER_RETRY_ATTEMPTS; 
         retryCount++)
    {
        if (isAfterReboot == 1)
        {
            delayMs(1000);  // This delay is so characters aren't missed after a reboot.
        }
        
        delayMs(WIFLY_COMMAND_MODE_GUARD_TIME);
    
        WIFLY_FLUSH();
        WIFLY_PRINTF("%c%c%c",commChar,commChar,commChar);  // Print the command chars
        
        delayMs(WIFLY_COMMAND_MODE_GUARD_TIME);
        
        WIFLY_PRINTF("\r\r");                               // Print 2 carriage return to make shure it has entered command mode

        // This is used to determine whether command mode has been entered
        // successfully.
        WIFLY_PRINTF("ver\r");
        
        if (findInResponse("WiFly Ver ", 1000) == 0)
        {
            return findWiFlyVersion(1000);
        }
    }
    
    return -1;
}

int8 actionWiFlyExitCommandMode()
{
    WIFLY_FLUSH();
    WIFLY_PRINTF("exit\r");
    delayMs(WIFLY_COMMAND_SETTLE_TIME);
    
    if (findInResponse("EXIT", 1000) == 0)
    { 
        return 0;
    }

    return -1;
}

void actionWiFlycloseTcpConnection()
{
    WIFLY_PRINTF("close\r");
}

void actionWiFlyFactoryReset()
{
    WIFLY_PRINTF("factory RESET\r");
}

void actionWiFlyJoin()
{
    WIFLY_PRINTF("join\r");
}

void actionWiFlyJoinSsid(char *ssid)
{
    WIFLY_PRINTF("join %s\r", ssid);
}

void actionWiFlyJoinNum(uint8 num)
{
    WIFLY_PRINTF("join # %u\r", num);
}

void actionWiFlyLeaveAccesPoint()
{
    WIFLY_PRINTF("leave\r");
}

void actionWiFlyLites()
{
    WIFLY_PRINTF("lites\r");
}

void actionWiFlyLookup(char *hostname)
{
    WIFLY_PRINTF("lookup %s\r", hostname);
}

void actionWiFlyOpenTcpConnection()
{
    WIFLY_PRINTF("open\r");
}

void actionWiFlyOpenTcpConnectionAddr(char *addr, uint16 port)
{
    WIFLY_PRINTF("open %s %u\r", addr, port);
}

void actionWiFlyPing(char *parameters, uint16 num)
{
    WIFLY_PRINTF("ping  %s %u\r", parameters, num);
}

#define WIFLY_SOFTWARE_REBOOT_RETRY_ATTEMPTS 5
int8 actionWiFlyReboot()
{
    uint8 retryCount;
    
    for (retryCount = 0; retryCount < WIFLY_SOFTWARE_REBOOT_RETRY_ATTEMPTS; retryCount++)
    {
        if (actionWiFlyEnterCommandMode(1) == -1)
        {
            return -1;  // If the included retries have failed we give up
        }
        
        WIFLY_PRINTF("reboot\r");
            
        // For some reason the full "*Reboot*" message doesn't always
        // seem to be received so we look for the later "*READY*" message instead.
    
        if (findInResponse("*READY*", 2000))
        {
            wiFlyState = WiFlyStateDisconnected;    // Now it also should be disconnected
            return 0;
        }
    }
    
    return -1;
}

void actionWiFlyScan(uint32 time, uint8 passive)
{
    if (passive)
        WIFLY_PRINTF("scan %u P\r", time);
    else
        printfUart0("scan %u\r", time);
}

void actionWiFlySleep()
{
    WIFLY_PRINTF("sleep\r");
}

void actionWiFlyTime()
{
    WIFLY_PRINTF("time\r");
}

void fileIoWiFlyDel(char *name)
{
    WIFLY_PRINTF("del %s\r", name);
}

void fileIoWiFlyLoad(char *name)
{
    WIFLY_PRINTF("load %s\r", name);
}

void fileIoWiFlyLs()
{
    WIFLY_PRINTF("ls\r");
}

int8 fileIoWiFlySaveDefault()
{
    return otherCommand("save config\r", "Storing in config");
}

int8 fileIoWiFlySave(char *name)
{
    char buffer[100];
    snprintf(commandBuffer, WIFLY_COMMAND_BUFFER_SIZE, "save %s\r", name);
    snprintf(buffer, 100, "Storing in %s", name);
    return otherCommand(commandBuffer, buffer);
}

void fileIoWiFlyBootImage(uint16 num)
{
    WIFLY_PRINTF("boot image %u\r", num);
}

void fileIoWiFlyFtpUpdate(char *name)
{
    WIFLY_PRINTF("ftp update %s\r", name);
}

inline int8 putcharWiFly(char c)
{
    return WIFLY_PUTCHAR(c);
}

inline int8 writeDataWiFly(void *data, uint32 length)
{
    return WIFLY_WRITEDATA(data, length);
}

inline int8 getcharWiFly(char* c)
{
    return WIFLY_GETCHAR(c);
}

int8 printfWiFly(char *format, ...)
{
    char buffer[WIFLY_PRINTF_BUFFER_SIZE];
    
    va_list arg_ptr;
    uint8 i = 0;
    
    va_start(arg_ptr,format);
    vsnprintf(buffer, WIFLY_PRINTF_BUFFER_SIZE, format, arg_ptr);
    va_end(arg_ptr);
    
    while (buffer[i] != 0)      // Loop through until reach string's zero terminator
    {
        if (putcharWiFly(buffer[i]) == -1)
            return -1;
        i++;
    }
    
    return 0;
}

void setProcessFunctionWiFly(void (* func)(char *))
{
    taskFunctionPointer = func;
    //WIFLY_SET_PROCESS_FUNCTION(func);
}

void setErrorFunctionWiFly(void (* func)())
{
    WIFLY_SET_ERROR_FUNCTION(func);
}

inline void processTaskWiFly()
{
    WIFLY_PROCESS_TASK();
}

int8 findInResponse(const char* toMatch, uint32 timeout)
{
    uint32 timeoutTarget;
    uint16 offset;
    char   byteRead;
    
    responseBufferPos = 0;                                  //clear the responseBuffer
    
    for (offset = 0; offset < strlen(toMatch); offset++)
    {
        timeoutTarget = timeoutMsecs() + timeout;
        
        while (getcharWiFly(&byteRead) != 0)
        {
            // Wait with timeout
            if (timeout > 0)
            {
                if (timeoutMsecs() > timeoutTarget)
                {
                    return -1;
                }
            }
            delayMs(2); // Improves reliability, 1ms just makes this thing freak out
        }
        
        delayMs(10);    // Seems to be necessary, don't know why
        
        if (byteRead != toMatch[offset]) {
            offset = 0;
            
            responseBuffer[responseBufferPos] = byteRead;   // Store unmachtech bytes
            if (responseBufferPos < WIFLY_RESPONSE_BUFFER_SIZE-1)
                responseBufferPos++;
            else
                responseBufferPos = 0;
            
            // Ignore character read if it's not a match for the start of the string
            if (byteRead != toMatch[offset])
            {
                offset = -1;
            }
            continue;
        }
    }
    
    responseBuffer[responseBufferPos] = '\0';   // Add the string terminator
    return 0;
}


int8 findWiFlyVersion(uint32 timeout)
{
    uint32 timeoutTarget;
    char   byteRead;    
    uint8 i;
    
    for (i = 0; i < WIFLY_VERSION_LENGTH; i++)
    {
        timeoutTarget = timeoutMsecs() + timeout;
        
        while (getcharWiFly(&byteRead) != 0)
        {
            // Wait with timeout
            if (timeout > 0)
            {
                if (timeoutMsecs() > timeoutTarget)
                {
                    return -1;
                }
            }
            delayMs(1); // Improves reliability
        }
        
        wiFlyVersion[1 + i] = byteRead;
    }
    wiFlyVersion[0] = '<';
    wiFlyVersion[WIFLY_VERSION_LENGTH + 1] = '>';
    wiFlyVersion[WIFLY_VERSION_LENGTH + 2] = '\0';
    
    return 0;
}

int8 responseMatched(const char* toMatch)
{
    int8 matchFound = 0;
    uint32 timeout;
    uint16 offset;
    char byteRead;
    
    for (offset = 0; offset < strlen(toMatch); offset++)
    {
        timeout = timeoutMsecs();
        while (getcharWiFly(&byteRead) != 0)
        {
            // Wait, with optional timeout
            if ((timeoutMsecs() - timeout) > 5000)
            {
                return -1;
            }
            delayMs(1);
        }
        
        if (byteRead != toMatch[offset])
        {
            matchFound = -1;
            break;
        }
    }
    
    return matchFound;
}

int8 setCommand(char* command)
{
    WIFLY_FLUSH();          // Empty buffers
    WIFLY_PRINTF(command);  // Send command
    delayMs(WIFLY_COMMAND_SETTLE_TIME);
    return findInResponse(WIFLY_SET_OK, WIFLY_RESPONSE_TIMEOUT);
}

int8 getCommand(char* command)
{
    WIFLY_FLUSH();          // Empty buffers
    WIFLY_PRINTF(command);  // Send command
    return findInResponse(wiFlyVersion, WIFLY_RESPONSE_TIMEOUT);
}

int8 otherCommand(char* command, char* awaitedResponse)
{
    WIFLY_FLUSH();          // Empty buffers
    WIFLY_PRINTF(command);  // Send command
    return findInResponse(awaitedResponse, WIFLY_RESPONSE_TIMEOUT);
}

char* getWiFlyResponse()
{
    return responseBuffer;
}

void internalProcessTask(char *command)
{
    if (command[0] == '*')
    {
        if (command[1] == commOpenString[1])
        {
            wiFlyState = WiFlyStateConnected;
        }
        else if (command[1] == commCloseString[1])
        {
            wiFlyState = WiFlyStateDisconnected;
        }
    }
    else if (wiFlyState == WiFlyStateConnected)
    {
        (*taskFunctionPointer)(command);
    }
    // else it is unnecessary
}

uint8 isWiFlyConnected()
{
    return (wiFlyState == WiFlyStateConnected);
}
