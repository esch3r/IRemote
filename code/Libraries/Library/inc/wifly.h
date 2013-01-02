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
/** Sets the flush timer. An IP packet will be sent if no additional bytes are
received for “num” milliseconds. “num” is one milliseconds interval.
Default is 10 (10 milliseconds). Setting this value to 0 will disable
forwarding based on the flush timer.
Flush timer is one of three ways to control TCP/IP packet forwarding.
The others are match character and size. For more information refer to
section on UART Receiver.*/
void setWiFlyCommTime(uint32 num);

/** Sets the IP address of the DNS sever. This is auto-set when using DHCP,
and needs to be set in STATIC IP or Auto-IP modes.*/
void setWiFlyDnsAddr(char *addr);
/** Sets the name of the host for TCP/IP connections.*/
void setWiFlyDnsName(char *string);
/** Sets the name of the backup host for TCP/IP connections.*/
void setWiFlyDnsBackup(char *string);
/** Set the IP address of the FTP server. By default, the IP address is set to
Roving Networks’ FTP server (208.109.78.34) */
void setWiFlyFtpAddr(char *addr);
/** Sets the directory to use on the FTP server. Default is “public”. To
read/write to sub-folders, use the “\” for your directory structure. For
example, to read/write to the “test” sub-folder in “demo” directory, the
command is set ftp dir demo\test. To root directory is set by using the
period “.” To se the root directory, the command if set ftp dir .*/
void setWiFlyFtpDir(char *string);
/** Sets the name of the file transferred when issuing the ftp u command. The
file here refers to the firmware image. If any file other than the firmware
image is set, the WiFly module will download the file and then issue the
UPDATE FAIL=3 error since it is not the firmware image.*/
void setWiFlyFtpFilename(char *file);
/** Sets the ftp mode. Default is passive mode (mode =0x0). Setting
mode=0x1 enables active mode.*/
void setWiFlyFtpMode(uint32 mask);
/** Sets the ftp server remote port number (default is 21).*/
void setWiFlyFtpRemote(uint32 port);
/** Sets the ftp timeout value. This timer is used to automatically close the
FTP connection. The timer is 1/8th of the actual value. For example, to set
a timer of 5 seconds, the command is set ftp timer 40. To set the timer to
10 seconds is set ftp timer 80*/
void setWiFlyFtpTime(uint32 value);
/**  sets the ftp user name for accessing the FTP server. Default is roving */
void setWiFlyFtpUser(char *name);
/**  sets the ftp password for accessing the FTP server. Default is Pass123 */
void setWiFlyFtpPass(char *pass);

/** Sets the IP address of the WiFly GSX module. If DHCP is turned on, the
IP address is assigned and overwritten during association with the access
point. IP addresses are “.” delimited.
Example: set ip a 10.20.20.1 */
void setWiFlyIpAddress(char *addr);
/** Sets a secondary host IP address. If the primary host IP is not reachable
the module will try the secondary IP address if set.*/
void setWiFlyIpBackup(char *addr);
/** Enable/disable DHCP mode. If enabled, the IP address, gateway, netmask,
and DNS server are requested and set upon association with access point.
Any current IP values are overwritten.
DHCP Cache mode can reduce the time it takes the module to wake from
deep sleep thus saving power. In cache mode, the lease time is checked
and if not expired, the module uses the previous IP settings. If the lease
has expired the module will attempt to associated and use DHCP to get the
IP settings. DHCP cached IP address does not survive a power cycle or
reset. */
void setWiFlyIpDhcp(uint32 value);
/** Set TCP/IP functions. Value is a bit mapped register. Default = 0x7.*/
void setWiFlyIpFlags(uint32 value);
/** Sets the gateway IP address, If DHCP is turned on, the gateway IP address
is assign and overwritten during association with the access point.*/
void setWiFlyIpGateway(char *addr);
/** Sets the remote host IP address. This command is used for making
connections from the WiFly module to a TCP/IP server at the IP address
<addr>.*/
void setWiFlyIpHost(char *addr);
/** Sets the local port number.*/
void setWiFlyIpLocalport(uint32 num);
/** Sets the network mask. If DHCP is turned on, the net mask is assign and
overwritten during association with the access point.*/
void setWiFlyIpNetmask(char *value);
/** Sets the IP protocol. Value is a bit mapped setting. To connect to the
WiFly GSX module over TCP/IP such as Telnet the device must have the 
use the TCP Server protocol / bit 2 set. To accept both TCP and UDP use
value = 3 (bit 1 and bit 2 set) */
void setWiFlyIpProtocol(uint32 value);
/** Sets the remote host port number.*/
void setWiFlyIpRemote(uint32 value);
/** A new register called TCPMODE is added to the IP parameters in version
2.27 and later. This is a bit masked register and controls the TCP connect
timers, DNS preferences and remote configuration options. Default is 0
with all these parameters not used.
Example: To force DNS, use set ip tcp-mode 0x4
To disable remote configuration, use set ip tcp-mode 0x10 */
void setWiFlyIpTcpMode(uint32 mask);

/** Join timer is the time in milliseconds the join function will wait for the an
access point to complete the association process. This timer is also the
timeout for the WPA handshaking process. Default is 1000.*/
void setWiFlyOptJointmr(uint32 msecs);
/** Settings for HTTP client/web server value is a bitmapped register. See
section on web server modes for more details.*/
void setWiFlyOptFormat(uint32 value);
/** Replacement character for spaces. The replacement character is used when
entering SSID and pass phrases that include space. This is used by the
WiFly command parser only. Each occurrence of the replacement
character is changed into a space. The default is “$” (0x24)*/
void setWiFlyOptReplace(char character);
/** Configurable Device ID - can be used for storing serial numbers, product
name or other device information. This information is sent as part of the
broadcast hello packet that is sent as a UDP. The current value can be
shown with the “get option” or “show deviceid” commands. Max string
size is 32 bytes. The default is “WiFly-GSX”.*/
void setWiFlyOptDeviceid(char *string);
/** TCP connection password. Provides minimal authentication by requiring
any remote device that connects to send and match a challenge <string>.
When set, all newly opened connections must first send the exact
characters that match the stored password otherwise the WiFly module
will close the connection. When the password is set the WiFly module
sends the string “PASS?” to the remote host. All characters in the string
must be sent in one TCP packet. Max string size is 32 bytes. To disable
the password feature use string=0 which is the default.*/
void setWiFlyOptPassword(char *string);

/** TCP mode: sets the auto connect timer. This command causes the module
periodically connect to the stored remote host. The timer <value>
determines how often to connect to the stored remote host. Default=0*/
void setWiFlySysAutoconn(uint32 value);
/** Sets the auto-sleep timer in UDP mode. If the protocol is set to UDP
ONLY, this timer is used as a quick sleep function. Device will sleep
<value> milliseconds after transmission of the first UDP packet. Setting
value to 0 disables the autosleep timer.*/
void setWiFlySysAutosleep(uint32 value);
/** Sets the IO port alternate functions. Bit-mapped value. For more details
see section 10.5 */
void setWiFlySysIofunc(uint32 value);
/** Sets the IO port direction mask. Bit-mapped value. For more information
see section 10.5 */
void setWiFlySysMask(uint32 mask);
/** Controls debug print messages. This configures the debug messages
printed by the WiFly module on the UART. Default=1. Please refer
section 10.6 on Setting Debug Print levels.*/
void setWiFlySysPrintlvl(uint32 value);
/** Sets output GPIO pins to HIGH or LOW. This is a bit-mapped value.
Optional mask only sets a subset of pins.*/
void setWiFlySysOutput(uint32 value, uint32 mask);
/** sets the sleep timer. The sleep timer is the time in seconds after which the
module goes to sleep. This timer is disabled during an open TCP
connection. When the TCP connection is closed, the module counts down
and puts the module in sleep state based on the value (in seconds). Setting
the value= 0 disables the sleep counter and the module will not go into
sleep based on this counter.
NOTE: Be sure to set the wake timer if not using an external wake up
signal before issuing the sleep timer or the module will never wake up.
See section 10.1 for more details on using system timers */
void setWiFlySysSleep(uint32 value);
/** The WiFly modules can wake up from sleep state using the sensor input 0,
1, 2 and 3. This command sets the sensor input(s) to wake on (0-3). This
is a bit-mapped value. Setting the value=0 disables wake on sensor
inputs.*/
void setWiFlySysTrigger(uint32 value);
/** Sets the default value of the GPIO outputs upon power up. The GPIOs that
are configured as outputs can either be driven HIGH or LOW upon power
up or when the module wakes from sleep. The default power up states can
be set ONLY for the GPIOs set as outputs.
Setting the GPIO value to 1 sets the default power-up state of that GPIO
HIGH and setting the GPIO value to 0 sets the default power-up state to
be LOW.*/
void setWiFlySysValue(uint32 value);
/** Sets the auto wake timer. The wake timer is the number of seconds after
which the WiFly module will wake from sleep state. Setting value=0
disables. See section on using system timers for more details.*/
void setWiFlySysWake(uint32 secs);

/** Sets the time server address. (sNTP servers) */
void setWiFlyTimeAddress(char *addr);
/** Sets the time server port number. Defaults to 123, which is almost always
the sNTP server port.*/
void setWiFlyTimePort(uint32 num);
/**  Enable or disable fetching time from the specified sNTP time server.
Default=0= disabled. A value or 1 gets time only once on power up. Any
value > 1 gets time continuously every <value> minutes.*/
void setWiFlyTimeEnable(uint32 value);
/**  Enables setting the RTC raw value from the console. This command sets
the RTC in seconds which ticks at 32768 Hz.*/
void setWiFlyTimeRaw(uint32 value);

/** Set the UART baud rate. Valid settings are {2400, 4800, 9600, 19200,
38400, 57600, 115200, 230400, 460800, 921600}.
NOTE: the RS232 interface on the RN-134 does not work above 230400 */
void setWiFlyUartBaud(uint32 rate);
/** This immediately changes the baudrate. This is useful when testing
baudrate settings, or switching baudrate “on the fly” remotely while
connected over TCP (Telnet). This setting does not affect configuration.
Returns the AOK response, and then this command will exit command
mode.*/
void setWiFlyUartInstant(uint32 rate);
/** Sets a RAW UART value. Used to set non-standard rates. The lowest
possible baud rate is 2400.*/
void setWiFlyUartRaw(uint32 rate);
/** Sets the flow control mode. Default=0=Disabled, 1= hardware RTS/CTS.*/
void setWiFlyUartFlow(uint8 flow);
/** Sets the UART mode register. This is a bit-mapped value.*/
void setWiFlyUartMode(uint16 mode);
/**  Disables or enables the TX pin= PIO10 of the UART. Disable will set
PIO10 to an INPUT with weak pulldown.*/
void setWiFlyUartTx(uint8 enabled);

/** Sets the authentication mode. Not needed unless using auto join mode 2.
i.e. set wlan join 2
Note: During association the WiFly module interrogates the Access Point
and automatically selects the authentication mode.
The current release of WiFly firmware supports these security modes:
• WEP-64 and WEP-128 (open mode only, NOT shared mode)
• WPA2-PSK (AES only)
• WPA1-PSK (TKIP only)
• WPA-PSK mixed mode (some APs, not all are supported) */
void setWiFlyWlanAuth(uint8 value);
/** Sets the wlan channel, 1-13 is the valid range for a fixed channel. If 0 is
set, then scan is performed, using the ssid, for all the channels set in the
channel mask.*/
void setWiFlyWlanChannel(uint8 value);
/** Determines which antenna is active, use 0 for chip antenna, 1 for UF.L
connector. Default = 0. Only one antenna is active at a time and the
module must be power cycled after switching the antenna.
NOTE: This command applies only to RN-131. This command is not
applied to the RN-171. Issuing this command on the RN-171 will give an
error message: ERR: Bad Args */
void setWiFlyWlanExtAntenna(uint8 enabled);
/** Sets the policy for automatically joining/associating with network access
points. This policy is used when the module powers up, including wake up
from the sleep timer.*/
void setWiFlyWlanJoin(uint8 value);
/** Hides the WEP key and WPA passphrase. When set, displaying the wlan
settings shows ****** for these fields. To unhide the passphrase or
passkey, re-enter the key or passphrase using the set wlan key or set wlan
passphrase command. Default = 0, don’t hide.*/
void setWiFlyWlanHide(uint8 enabled);
/** Sets the 128 bit WEP key. If you are using WPA or WPA2 you should
enter a pass phrase with the set wlan passphase command. Key must be
EXACTLY 13 bytes (26 ASCII chars). Data is expected in HEX format,
“0x” should NOT be used here.
Example : “set w k 112233445566778899AABBCCDD”
Hex digits > 9 can be either upper or lower case.
The Wifly GSX only supports “open” key mode, 128 bit keys for WEP.
WEP-128, shared mode is not supported as it is known to be easily
compromised and has been deprecated from the WiFi standards.*/
void setWiFlyWlanKey(char *value);
/** Sets the link monitor timeout threshold. If set to 1 or more, WiFly will
scan once per second for the AP it is associated with. The value is the
threshold of failed scans before the WiFly declares “AP is Lost”, de-
authenticates. The WiFly will retry the association based on the join
policy variable. A value of 5 is recommended, as some APs will not
always respond to probes. Default is 0 (disabled). Without this feature,
there is no way to detect an AP is no longer present until it becomes
available again (if ever).*/
void setWiFlyWlanLinkmon(uint32 value);
/** Sets the wlan channel mask used for scanning channels with the auto-join
policy 1 or 2, used when the channel is set to 0. Value is a bit-map where
bit 0 = channel 1. Input for this command can be entered in decimal or
hex if prefixed with 0x. Default value is 0x1FFF (all channels)*/
void setWiFlyWlanMask(uint32 mask);
/** Sets the default WEP key to use. 1-4 is the valid range.
Example : “set w n 2” sets the default key to 2.*/
void setWiFlyWlanNum(uint8 value);
/** Sets the passphrase for WPA and WPA2 security modes. 1-64 chars. The
passphrase can be alpha and numeric, and is used along with the SSID to
generate a unique 32 byte Pre-shared key (PSK), which is then hashed into
a 256 bit number. Changing either the SSID or this value re-calculates
and stores the PSK.
If exactly 64 chars are entered, it is assumed that this entry is already an
ASCII HEX representation of the 32 byte PSK and the value is simply
stored.
For passphrases that contain spaces use the replacement character $
instead of spaces. For example “my pass word” would be entered
“my$pass$word”. The replacement character can be changed using the
optional command set opt replace <char>.*/
void setWiFlyWlanPhrase(char *string);
/** Sets the wireless data rate. Lowering the rate increases the effective range
of the WiFly-GSX module. The value entered is mapped according to the
following table:*/
void setWiFlyWlanRate(uint8 value);
/** sets the wlan ssid to associate with. 1-32 chars.
NOTE: If the passphrase or ssid contain the SPACE ( ‘ ‘)
characters, these can be entered using substitution via the “$” character.
For example, if the ssid of the AP is “yellow brick road”
You would enter “yellow$brick$road”
Using the ‘get w” command will properly display the value:
SSID=yellow brick road.*/
void setWiFlyWlanSsid(char *string);
/** Sets the Wi-Fi transmit power. Accepts value from 1-12 corresponding to
1dBm to 12dBm. Default is 0 corresponding to 12dBm (max TX power).
Setting the value to 0 or 12 sets the TX power to 12dBm
NOTE: This command applies only to RN-171 module and not the RN-
131. The transmit power on the RN-131 is fixed to 18dBm. Issuing this
command on the RN-131 will return an error message, ERR: Bad Args*/
void setWiFlyWlanTx(uint8 value);
/** Sets the IP maximum buffer window size. Default is 1460 bytes.*/
void setWiFlyWlanWindow(uint32 value);

/** Bitmask value that determines which sensor pins to sample when sending
data using the UDP broadcast packet, or the HTTP auto sample function.*/
void setWiFlyQSensor(uint32 mask);
/** This register is used to automatically turn on the sensor power. This is a 8
bit register with two 4 bit nibbles. If the top nibble is set, then power is
applied upon power up and removed upon power down or sleep.*/
void setWiFlyQPower(uint8 value);

/** Display all adhoc settings.*/
void getWiFlyAdhoc();
/** Will display the broadcast UPD address, port and interval */
void getWiFlyBroadcast();
/** Display comm. settings.*/
void getWiFlyCom();
/** Display DNS settings.*/
void getWiFlyDns();
/** Displays all configuration settings, useful for debug.*/
void getWiFlyEverything();
/** Display FTP settings.*/
void getWiFlyFtp();
/** Display IP address and port number settings. Optional parameter just returns the
current IP address value.*/
void getWiFlyIp(uint8 a);
/** Display the device MAC address.*/
void getWiFlyMac();
/** Display the option settings like device ID */
void getWiFlyOption();
/** Display system settings, sleep, wake timers, etc.*/
void getWiFlySys();
/** Display the time server UDP address and port number.*/
void getWiFlyTime();
/** Display the ssid, chan, and other wlan settings.*/
void getWiFlyWlan();
/** Display the UART settings.*/
void getWiFlyUart();
/** Return the software release version */
void getWiFlyVer();

/** Displays current battery voltage, (only valid for Roving battery powered product like the
RN-370 and temperature sensors and the ISENSOR-CB) */
void showWiFlyBattery();
/** Displays connection status in this HEX format: 8XYZ */
void showWiFlyConnection();
/** Displays GPIO pin levels status in this HEX format: 8ABC
Example: show i returns 8103 indicates GPIO 0, 1 and 8 are HIGH.*/
void showWiFlyIo();
/** Displays current network status, association, authentication, etc. Optional parameter
displays only the MAC address of the AP currently associated.*/
void showWiFlyNet(uint8 n);
/** Displays current last received signal strength.*/
void showWiFlyRssi();
/** Displays current statistics, packet rx/tx counters, etc.*/
void showWiFlyStats();
/** Displays number of seconds since last powerup or reboot*/
void showWiFlyTime();
/** Display the value of the an analog interface pin from 0 to 7. The value returned will be in
the format 8xxxxx where xxxxx is voltage in microvolts sampled on the channel you
request with the 8 in front as a start marker.*/
void showWiFlyQ(uint8 num);
/** Displays multiple analog interface values at once. The channels displayed are
controlled by a bit mask, which is proceeded by a 0x1xx where xx mask is the bit mask of
the channels.*/
void showWiFlyQ0x1(uint8 mask);

/** Enter command mode. X characters are PASSED until this exact sequence
is seen. If any bytes are seen before these chars, or after these chars, in a
250ms window, command mode will not be entered and these bytes will
be passed on to other side.*/
void actionWiFlyEnterCommandMode();
/** Exit command mode. Exit command mode. “EXIT” will be displayed.*/
void actionWiFlyExitCommandMode();
/** Disconnect a TCP connection.*/
void actionWiFlycloseTcpConnection();
/** Loads factory defaults into the RAM configuration. This command also writes the settings out
to the standard config file. After this command the module then needs to
be rebooted for settings to take effect.*/
void actionWiFlyFactoryReset();
void actionWiFlyJoin();
/** Joins the network <ssid>. If network is security enabled you must set the
pass phrase with the set wlan phrase command prior to issuing the join
command.
NOTE : This command will not work if the ssid has "space » character */
void actionWiFlyJoinSsid(char *ssid);
/** Join a network from the scan list. <num> is the entry number in the scan
list that is returned from the scan command. If network is security
enabled you must set the pass phrase with the set wlan phrase command
prior to issuing the join command */
void actionWiFlyJoinNum(uint8 num);
/** Disconnects the module from the currently associated Access Point.*/
void actionWiFlyLeaveAccesPoint();
/** Blinks LEDs on RN-134 Surf board and RN-370 WiFly serial adapter.
Issue the same command to stop blinking LEDs.*/
void actionWiFlyLites();
/** Performs a DNS query on the supplied hostname.*/
void actionWiFlyLookup(char *hostname);
/** Opens a TCP connection to the given IP port and address. The device will attempt to connect to the stored remote host
IP address and remote port number.*/
void actionWiFlyOpenTcpConnection();
/** Opens a TCP connection to the given IP port and address. <addr> can also be a DNS hostname
and will be resolved if entered.*/
void actionWiFlyOpenTcpConnectionAddr(char *addr, uint16 port);
/** Ping remote host. Default sends 1 packet. Optional <num> sends <num>
pings at 10 per second.
ping g
 pings the gateway, the gateway IP address is loaded if DHCP
is turned on, otherwise it should be set with the set ip gateway <addr>
command
ping h
 pings the stored host IP address, the host IP address can be set
with the set ip host <addr> command
ping i
 pings a known Internet server at www.neelum.com by first
resolving the URL (proves that DNS is working and proves the device has
internet connectivity).
ping 0
 terminates a ping command*/
void actionWiFlyPing(char *parameters, uint16 num);
/** Forces a reboot of the device (similar to power cycle)*/
void actionWiFlyReboot();
/** Performs an active probe scan of access points on all 13 channels. Returns
MAC address, signal strength, SSID name, security mode.
Default scan time is 200ms / channel = about 3 seconds.
time is an optional parameter, this is the time in ms per channel.
For example, “scan 30” reduces the total scan time down to about 1
second. This command also works in Adhoc mode. If the optional P
parameter is entered, the module will perform a passive scan, and list all
APs that are seen in passive mode.*/
void actionWiFlyScan(uint32 time, uint8 passive);
/** Puts the module to sleep mode. The module can come out of sleep mode
by either sending characters over the uart or by using the wake timer.*/
void actionWiFlySleep();
/** Sets the Real time clock by synchronizing with the time server specified
with the time server parameters (see section 5.9) This command sends a
UDP time server request packet.*/
void actionWiFlyTime();

/** Deletes a file. Optional <num> will override the name and use the sector
number shown in the “ls” command.*/
void fileIoWiFlyDel(char *name);
/** Reads in a new config file.*/
void fileIoWiFlyLoad(char *name);
/** Displays the files in the system */
void fileIoWiFlyLs();
/** Saves the configuration to “config” (the default file).*/
void fileIoWiFlySaveDefault();
/** Saves the configuration data to a new file name */
void fileIoWiFlySave(char *name);
/** Makes file <num> the new boot image.*/
void fileIoWiFlyBootImage(uint16 num);
/** Deletes the backup image, retrieves new image and updates the boot
pointer to the new image.*/
void fileIoWiFlyFtpUpdate(char *name);