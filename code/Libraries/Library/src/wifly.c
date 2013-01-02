#include "wifly.h"

char commChar = '$';
char *commCloseString = "*CLOS*";
char *commOpenString = "*OPEN*";
char *commRemoteString = "*HELLO*";


int8 initializeWiFly(void)
{
    return initializeUart1(9600);  // 9600 Baud, 1 Stop bit, No parity, No Hardware flow control
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

void setWiFlyCommTime(uint32 num)
{
    printfUart1("set c t %u\r", num);
}

void setWiFlyDnsAddr(char *addr)
{
    printfUart1("set d a %s\r", addr);
}

void setWiFlyDnsName(char *string)
{
    printfUart1("set d n %s\r", string);
}

void setWiFlyDnsBackup(char *string)
{
    printfUart1("set d b %s\r", string);
}

void setWiFlyFtpAddr(char *addr)
{
    printfUart1("set f a %s\r", addr);
}

void setWiFlyFtpDir(char *string)
{
    printfUart1("set f d %s\r", string);
}

void setWiFlyFtpFilename(char *file)
{
    printfUart1("set f f %s\r", file);
}

void setWiFlyFtpMode(uint32 mask)
{
    printfUart1("set f m %x\r", mask);
}

void setWiFlyFtpRemote(uint32 port)
{
    printfUart1("set f r %u\r", port);
}

void setWiFlyFtpTime(uint32 value)
{
    printfUart1("set f t %u\r", value);
}

void setWiFlyFtpUser(char *name)
{
    printfUart1("set f u %s\r", name);
}

void setWiFlyFtpPass(char *pass)
{
    printfUart1("set f p %s\r", pass);
}

void setWiFlyIpAddress(char *addr)
{
    printfUart1("set i a %s\r", addr);
}

void setWiFlyIpBackup(char *addr)
{
    printfUart1("set i b %s\r", addr);
}

void setWiFlyIpDhcp(uint32 value)
{
    printfUart1("set i d %u\r", value);
}

void setWiFlyIpFlags(uint32 value)
{
    printfUart1("set i f %x\r", value);
}

void setWiFlyIpGateway(char *addr)
{
    printfUart1("set i g %s\r", addr);
}

void setWiFlyIpHost(char *addr)
{
    printfUart1("set i h %s\r", addr);
}

void setWiFlyIpLocalport(uint32 num)
{
    printfUart1("set i l %u\r", num);
}

void setWiFlyIpNetmask(char *value)
{
    printfUart1("set i n %s\r", value);
}

void setWiFlyIpProtocol(uint32 value)
{
    printfUart1("set i p %u\r", value);
}

void setWiFlyIpRemote(uint32 value)
{
    printfUart1("set i r %u\r", value);
}

void setWiFlyIpTcpMode(uint32 mask)
{
    printfUart1("set i t %x\r", mask);
}

void setWiFlyOptJointmr(uint32 msecs)
{
    printfUart1("set o j %u\r", msecs);
}

void setWiFlyOptFormat(uint32 value)
{
    printfUart1("set o f %u\r", value);
}

void setWiFlyOptReplace(char character)
{
    printfUart1("set o r %c\r", character);
}

void setWiFlyOptDeviceid(char *string)
{
    printfUart1("set o d %s\r", string);
}

void setWiFlyOptPassword(char *string)
{
    printfUart1("set o p %s\r", string);
}

void setWiFlySysAutoconn(uint32 value)
{
    printfUart1("set s a %u\r", value);
}

void setWiFlySysAutosleep(uint32 value)
{
    printfUart1("set s a %u\r", value);
}

void setWiFlySysIofunc(uint32 value)
{
    printfUart1("set s i %u\r", value);
}

void setWiFlySysMask(uint32 mask)
{
    printfUart1("set s m %x\r", mask);
}

void setWiFlySysPrintlvl(uint32 value)
{
    printfUart1("set s p %u\r", value);
}

void setWiFlySysOutput(uint32 value, uint32 mask)
{
    printfUart1("set s o %x %x\r", value, mask);
}

void setWiFlySysSleep(uint32 value)
{
    printfUart1("set s s %u\r", value);
}

void setWiFlySysTrigger(uint32 value)
{
    printfUart1("set s t %u\r", value);
}

void setWiFlySysValue(uint32 value)
{
    printfUart1("set s v %u\r", value);
}

void setWiFlySysWake(uint32 secs)
{
    printfUart1("set s w %u\r", secs);
}

void setWiFlyTimeAddress(char *addr)
{
    printfUart1("set t a %s\r", addr);
}

void setWiFlyTimePort(uint32 num)
{
    printfUart1("set t p %u\r", num);
}

void setWiFlyTimeEnable(uint32 value)
{
    printfUart1("set t e %u\r", value);
}

void setWiFlyTimeRaw(uint32 value)
{
    printfUart1("set t r %u\r", value);
}

void setWiFlyUartBaud(uint32 rate)
{
    printfUart1("set u b %u\r", rate);
}

void setWiFlyUartInstant(uint32 rate)
{
    printfUart1("set u i %u\r", rate);
}

void setWiFlyUartRaw(uint32 rate)
{
    printfUart1("set u r %u\r", rate);
}

void setWiFlyUartFlow(uint8 flow)
{
    printfUart1("set u f %u\r", flow);
}

void setWiFlyUartMode(uint16 mode)
{
    printfUart1("set u m %x\r", mode);
}

void setWiFlyUartTx(uint8 enabled)
{
    printfUart1("set u t %u\r", enabled);
}

void setWiFlyWlanAuth(uint8 value)
{
    printfUart1("set w a %u\r", value);
}

void setWiFlyWlanChannel(uint8 value)
{
    printfUart1("set w c %u\r", value);
}

void setWiFlyWlanExtAntenna(uint8 enabled)
{
    printfUart1("set w e %u\r", enabled);
}

void setWiFlyWlanJoin(uint8 value)
{
    printfUart1("set w j %u\r", value);
}

void setWiFlyWlanHide(uint8 enabled)
{
    printfUart1("set w h %u\r", enabled);
}

void setWiFlyWlanKey(char *value)
{
    printfUart1("set w k %s\r", value);
}

void setWiFlyWlanLinkmon(uint32 value)
{
    printfUart1("set w l %u\r", value);
}

void setWiFlyWlanMask(uint32 mask)
{
    printfUart1("set w m %x\r", mask);
}

void setWiFlyWlanNum(uint8 value)
{
    printfUart1("set w n %u\r", value);
}

void setWiFlyWlanPhrase(char *string)
{
    printfUart1("set w p %s\r", string);
}

void setWiFlyWlanRate(uint8 value)
{
    printfUart1("set w r %u\r", value);
}

void setWiFlyWlanSsid(char *string)
{
    printfUart1("set w s %s\r", string);
}

void setWiFlyWlanTx(uint8 value)
{
    printfUart1("set w t %u\r", value);
}

void setWiFlyWlanWindow(uint32 value)
{
    printfUart1("set w w %u\r", value);
}

void setWiFlyQSensor(uint32 mask)
{
    printfUart1("set q s %x\r", mask);
}

void setWiFlyQPower(uint8 value)
{
    printfUart1("set q p %u\r", value);
}

void getWiFlyAdhoc()
{
    printfUart1("get a\r");
}

void getWiFlyBroadcast()
{
    printfUart1("get b\r");
}

void getWiFlyCom()
{
    printfUart1("get c\r");
}

void getWiFlyDns()
{
    printfUart1("get d\r");
}

void getWiFlyEverything()
{
    printfUart1("get e\r");
}

void getWiFlyFtp()
{
    printfUart1("get f\r");
}

void getWiFlyIp(uint8 a)
{
    if (a)
        printfUart1("get i a\r");
    else
        printfUart1("get i\r");
}

void getWiFlyMac()
{
    printfUart1("get m\r");
}

void getWiFlyOption()
{
    printfUart1("get o\r");
}

void getWiFlySys()
{
    printfUart1("get s\r");
}

void getWiFlyTime()
{
    printfUart1("get t\r");
}

void getWiFlyWlan()
{
    printfUart1("get w\r");
}

void getWiFlyUart()
{
    printfUart1("get u\r");
}

void getWiFlyVer()
{
    printfUart1("ver\r");
}

void showWiFlyBattery()
{
    printfUart1("show b\r");
}

void showWiFlyConnection()
{
    printfUart1("show c\r");
}

void showWiFlyIo()
{
    printfUart1("show i\r");
}

void showWiFlyNet(uint8 n)
{
    printfUart1("show n\r");
}

void showWiFlyRssi()
{
    printfUart1("show r\r");
}

void showWiFlyStats()
{
    printfUart1("show s\r");
}

void showWiFlyTime()
{
    printfUart1("show t\r");
}

void showWiFlyQ(uint8 num)
{
    printfUart1("show q %u\r", num);
}

void showWiFlyQ0x1(uint8 mask)
{
    printfUart1("show q 0x1%u\r", mask);
}

void actionWiFlyEnterCommandMode()
{
    printfUart1("%c%c%c",commChar,commChar,commChar);
}

void actionWiFlyExitCommandMode()
{
    printfUart1("exit\r");
}

void actionWiFlycloseTcpConnection()
{
    printfUart1("close\r");
}

void actionWiFlyFactoryReset()
{
    printfUart1("factory RESET\r");
}

void actionWiFlyJoin()
{
    printfUart1("join\r");
}

void actionWiFlyJoinSsid(char *ssid)
{
    printfUart1("join %s\r", ssid);
}

void actionWiFlyJoinNum(uint8 num)
{
    printfUart1("join # %u\r", num);
}

void actionWiFlyLeaveAccesPoint()
{
    printfUart1("leave\r");
}

void actionWiFlyLites()
{
    printfUart1("lites\r");
}

void actionWiFlyLookup(char *hostname)
{
    printfUart1("lookup %s\r", hostname);
}

void actionWiFlyOpenTcpConnection()
{
    printfUart1("open\r");
}

void actionWiFlyOpenTcpConnectionAddr(char *addr, uint16 port)
{
    printfUart1("open %s %u\r", addr, port);
}

void actionWiFlyPing(char *parameters, uint16 num)
{
    printfUart1("ping  %s %u\r", parameters, num);
}

void actionWiFlyReboot()
{
    printfUart1("reboot\r");
}

void actionWiFlyScan(uint32 time, uint8 passive)
{
    if (passive)
        printfUart1("scan %u P\r", time);
    else
        printfUart0("scan %u\r", time);
}

void actionWiFlySleep()
{
    printfUart1("sleep\r");
}

void actionWiFlyTime()
{
    printfUart1("time\r");
}

void fileIoWiFlyDel(char *name)
{
    printfUart1("del %s\r", name);
}

void fileIoWiFlyLoad(char *name)
{
    printfUart1("load %s\r", name);
}

void fileIoWiFlyLs()
{
    printfUart1("ls\r");
}

void fileIoWiFlySaveDefault()
{
    printfUart1("save\r");
}

void fileIoWiFlySave(char *name)
{
    printfUart1("save %s\r", name);
}

void fileIoWiFlyBootImage(uint16 num)
{
    printfUart1("boot image %u\r", num);
}

void fileIoWiFlyFtpUpdate(char *name)
{
    printfUart1("ftp update %s\r", name);
}
