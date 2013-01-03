#include "wifly.h"
#include <stdarg.h>
#include <stdio.h>

#define WIFLY_PRINTF    printfUart1
#define WIFLY_WRITEDATA writeDataUart1
#define WIFLY_PUTCHAR   putcharUart1
#define WIFLY_GETCHAR   getcharUart1
#define WIFLY_INIT      initializeUart1

char commChar = '$';
char *commCloseString = "*CLOS*";
char *commOpenString = "*OPEN*";
char *commRemoteString = "*HELLO*";


int8 initializeWiFly(void)
{
    return WIFLY_INIT(9600);  // 9600 Baud, 1 Stop bit, No parity, No Hardware flow control
}

void setWiFlyAdhocBeacon(uint32 ms)
{
    WIFLY_PRINTF("set a b %u\r", ms);
}

void setWiFlyAdhocProbe(uint32 num)
{
    WIFLY_PRINTF("set a p %u\r", num);
}

void setWiFlyBroadcastAddress(char *address)
{
    WIFLY_PRINTF("set b a %s\r", address);
}

void setWiFlyBroadcastInterval(uint32 value)
{
    WIFLY_PRINTF("set b i %u\r", value);
}

void setWiFlyBroadcastPort(uint16 port)
{
    WIFLY_PRINTF("set b p %u\r", port);
}

void setWiFlyComm(char c)
{
    WIFLY_PRINTF("set c %c\r", c);
    commChar = c;
}

void setWiFlyCommClose(char *string)
{
    WIFLY_PRINTF("set c c %s\r", string);
    commCloseString = string;
}

void setWiFlyCommOpen(char *string)
{
    WIFLY_PRINTF("set c o %s\r", string);
    commOpenString = string;
}

void setWiFlyCommRemote(char *string)
{
    WIFLY_PRINTF("set c r %s\r", string);
    commRemoteString = string;
}

void setWiFlyCommIdle(uint32 secs)
{
    WIFLY_PRINTF("set c i %u\r", secs);
}

void setWiFlyCommMatch(uint32 value)
{
    WIFLY_PRINTF("set c m %u\r", value);
}

void setWiFlyCommSize(uint32 value)
{
    WIFLY_PRINTF("set c s %u\r", value);
}

void setWiFlyCommTime(uint32 num)
{
    WIFLY_PRINTF("set c t %u\r", num);
}

void setWiFlyDnsAddr(char *addr)
{
    WIFLY_PRINTF("set d a %s\r", addr);
}

void setWiFlyDnsName(char *string)
{
    WIFLY_PRINTF("set d n %s\r", string);
}

void setWiFlyDnsBackup(char *string)
{
    WIFLY_PRINTF("set d b %s\r", string);
}

void setWiFlyFtpAddr(char *addr)
{
    WIFLY_PRINTF("set f a %s\r", addr);
}

void setWiFlyFtpDir(char *string)
{
    WIFLY_PRINTF("set f d %s\r", string);
}

void setWiFlyFtpFilename(char *file)
{
    WIFLY_PRINTF("set f f %s\r", file);
}

void setWiFlyFtpMode(uint32 mask)
{
    WIFLY_PRINTF("set f m %x\r", mask);
}

void setWiFlyFtpRemote(uint32 port)
{
    WIFLY_PRINTF("set f r %u\r", port);
}

void setWiFlyFtpTime(uint32 value)
{
    WIFLY_PRINTF("set f t %u\r", value);
}

void setWiFlyFtpUser(char *name)
{
    WIFLY_PRINTF("set f u %s\r", name);
}

void setWiFlyFtpPass(char *pass)
{
    WIFLY_PRINTF("set f p %s\r", pass);
}

void setWiFlyIpAddress(char *addr)
{
    WIFLY_PRINTF("set i a %s\r", addr);
}

void setWiFlyIpBackup(char *addr)
{
    WIFLY_PRINTF("set i b %s\r", addr);
}

void setWiFlyIpDhcp(uint32 value)
{
    WIFLY_PRINTF("set i d %u\r", value);
}

void setWiFlyIpFlags(uint32 value)
{
    WIFLY_PRINTF("set i f %x\r", value);
}

void setWiFlyIpGateway(char *addr)
{
    WIFLY_PRINTF("set i g %s\r", addr);
}

void setWiFlyIpHost(char *addr)
{
    WIFLY_PRINTF("set i h %s\r", addr);
}

void setWiFlyIpLocalport(uint32 num)
{
    WIFLY_PRINTF("set i l %u\r", num);
}

void setWiFlyIpNetmask(char *value)
{
    WIFLY_PRINTF("set i n %s\r", value);
}

void setWiFlyIpProtocol(uint32 value)
{
    WIFLY_PRINTF("set i p %u\r", value);
}

void setWiFlyIpRemote(uint32 value)
{
    WIFLY_PRINTF("set i r %u\r", value);
}

void setWiFlyIpTcpMode(uint32 mask)
{
    WIFLY_PRINTF("set i t %x\r", mask);
}

void setWiFlyOptJointmr(uint32 msecs)
{
    WIFLY_PRINTF("set o j %u\r", msecs);
}

void setWiFlyOptFormat(uint32 value)
{
    WIFLY_PRINTF("set o f %u\r", value);
}

void setWiFlyOptReplace(char character)
{
    WIFLY_PRINTF("set o r %c\r", character);
}

void setWiFlyOptDeviceid(char *string)
{
    WIFLY_PRINTF("set o d %s\r", string);
}

void setWiFlyOptPassword(char *string)
{
    WIFLY_PRINTF("set o p %s\r", string);
}

void setWiFlySysAutoconn(uint32 value)
{
    WIFLY_PRINTF("set s a %u\r", value);
}

void setWiFlySysAutosleep(uint32 value)
{
    WIFLY_PRINTF("set s a %u\r", value);
}

void setWiFlySysIofunc(uint32 value)
{
    WIFLY_PRINTF("set s i %u\r", value);
}

void setWiFlySysMask(uint32 mask)
{
    WIFLY_PRINTF("set s m %x\r", mask);
}

void setWiFlySysPrintlvl(uint32 value)
{
    WIFLY_PRINTF("set s p %u\r", value);
}

void setWiFlySysOutput(uint32 value, uint32 mask)
{
    WIFLY_PRINTF("set s o %x %x\r", value, mask);
}

void setWiFlySysSleep(uint32 value)
{
    WIFLY_PRINTF("set s s %u\r", value);
}

void setWiFlySysTrigger(uint32 value)
{
    WIFLY_PRINTF("set s t %u\r", value);
}

void setWiFlySysValue(uint32 value)
{
    WIFLY_PRINTF("set s v %u\r", value);
}

void setWiFlySysWake(uint32 secs)
{
    WIFLY_PRINTF("set s w %u\r", secs);
}

void setWiFlyTimeAddress(char *addr)
{
    WIFLY_PRINTF("set t a %s\r", addr);
}

void setWiFlyTimePort(uint32 num)
{
    WIFLY_PRINTF("set t p %u\r", num);
}

void setWiFlyTimeEnable(uint32 value)
{
    WIFLY_PRINTF("set t e %u\r", value);
}

void setWiFlyTimeRaw(uint32 value)
{
    WIFLY_PRINTF("set t r %u\r", value);
}

void setWiFlyUartBaud(uint32 rate)
{
    WIFLY_PRINTF("set u b %u\r", rate);
}

void setWiFlyUartInstant(uint32 rate)
{
    WIFLY_PRINTF("set u i %u\r", rate);
}

void setWiFlyUartRaw(uint32 rate)
{
    WIFLY_PRINTF("set u r %u\r", rate);
}

void setWiFlyUartFlow(uint8 flow)
{
    WIFLY_PRINTF("set u f %u\r", flow);
}

void setWiFlyUartMode(uint16 mode)
{
    WIFLY_PRINTF("set u m %x\r", mode);
}

void setWiFlyUartTx(uint8 enabled)
{
    WIFLY_PRINTF("set u t %u\r", enabled);
}

void setWiFlyWlanAuth(uint8 value)
{
    WIFLY_PRINTF("set w a %u\r", value);
}

void setWiFlyWlanChannel(uint8 value)
{
    WIFLY_PRINTF("set w c %u\r", value);
}

void setWiFlyWlanExtAntenna(uint8 enabled)
{
    WIFLY_PRINTF("set w e %u\r", enabled);
}

void setWiFlyWlanJoin(uint8 value)
{
    WIFLY_PRINTF("set w j %u\r", value);
}

void setWiFlyWlanHide(uint8 enabled)
{
    WIFLY_PRINTF("set w h %u\r", enabled);
}

void setWiFlyWlanKey(char *value)
{
    WIFLY_PRINTF("set w k %s\r", value);
}

void setWiFlyWlanLinkmon(uint32 value)
{
    WIFLY_PRINTF("set w l %u\r", value);
}

void setWiFlyWlanMask(uint32 mask)
{
    WIFLY_PRINTF("set w m %x\r", mask);
}

void setWiFlyWlanNum(uint8 value)
{
    WIFLY_PRINTF("set w n %u\r", value);
}

void setWiFlyWlanPhrase(char *string)
{
    WIFLY_PRINTF("set w p %s\r", string);
}

void setWiFlyWlanRate(uint8 value)
{
    WIFLY_PRINTF("set w r %u\r", value);
}

void setWiFlyWlanSsid(char *string)
{
    WIFLY_PRINTF("set w s %s\r", string);
}

void setWiFlyWlanTx(uint8 value)
{
    WIFLY_PRINTF("set w t %u\r", value);
}

void setWiFlyWlanWindow(uint32 value)
{
    WIFLY_PRINTF("set w w %u\r", value);
}

void setWiFlyQSensor(uint32 mask)
{
    WIFLY_PRINTF("set q s %x\r", mask);
}

void setWiFlyQPower(uint8 value)
{
    WIFLY_PRINTF("set q p %u\r", value);
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

void actionWiFlyEnterCommandMode()
{
    WIFLY_PRINTF("%c%c%c",commChar,commChar,commChar);
}

void actionWiFlyExitCommandMode()
{
    WIFLY_PRINTF("exit\r");
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

void actionWiFlyReboot()
{
    WIFLY_PRINTF("reboot\r");
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

void fileIoWiFlySaveDefault()
{
    WIFLY_PRINTF("save\r");
}

void fileIoWiFlySave(char *name)
{
    WIFLY_PRINTF("save %s\r", name);
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
