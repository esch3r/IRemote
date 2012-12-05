set editing off
set confirm off
set print static-members off
set print asm-demangle on
set print thread-events off
set unwindonsignal on
define kdbg__alllocals
info locals
info args
end
define kdbg_infolinemain
list
info line
end
set prompt (kdbg)
GNU gdb (Sourcery CodeBench Lite 2012.03-56) 7.2.50.20100908-cvs
Copyright (C) 2010 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=i686-pc-linux-gnu --target=arm-none-eabi".
For bug reporting instructions, please see:
<https://support.codesourcery.com/GNUToolchain/>.
(gdb) (gdb) (gdb) (gdb) (gdb) (gdb) (gdb) >>>(gdb) >>>(gdb) (kdbg)tty /dev/pts/2
(kdbg)file "/home/horauer/workspace/LPC1758_EXTINT/firmware.elf"
(kdbg)target remote localhost:3333
0x00000000 in __cs3_interrupt_vector_cortex_m ()
(kdbg)set args 
(kdbg)bt
#0  0x00000000 in __cs3_interrupt_vector_cortex_m ()
(kdbg)info threads
warning: RMT ERROR : failed to get remote thread list.
* 1 Remote target  0x00000000 in __cs3_interrupt_vector_cortex_m ()
(kdbg)frame 0
#0  0x00000000 in __cs3_interrupt_vector_cortex_m ()
(kdbg)info breakpoints
No breakpoints or watchpoints.
(kdbg)kdbg__alllocals
No symbol table info available.
No symbol table info available.
(kdbg)info all-registers
r0             0x0	0
r1             0x0	0
r2             0x0	0
r3             0x0	0
r4             0x0	0
r5             0x0	0
r6             0x0	0
r7             0x0	0
r8             0x0	0
r9             0x0	0
r10            0x0	0
r11            0x0	0
r12            0x0	0
sp             0x0	0x0
lr             0x0	0
pc             0x1	0x1 <__cs3_interrupt_vector_cortex_m+1>
f0             0	(raw 0x000000000000000000000000)
f1             0	(raw 0x000000000000000000000000)
f2             0	(raw 0x000000000000000000000000)
f3             0	(raw 0x000000000000000000000000)
f4             0	(raw 0x000000000000000000000000)
f5             0	(raw 0x000000000000000000000000)
f6             0	(raw 0x000000000000000000000000)
f7             0	(raw 0x000000000000000000000000)
fps            0x0	0
cpsr           0x0	0
(kdbg)Ending remote debugging.
