#!/bin/bash
#kill previous openocd session
PID=`ps aux | grep '[o]penocd' | awk '{print $2}'`
echo $PID
if [ "$PID" != "" ]; then
  kill $PID
fi
#run openocd
cd /home/alexander/Dropbox/C_Embedded_Systems/Uartprog/
openocd -f DbgCfg/LPC1758_OOCD_CMARM.cfg
sleep 1