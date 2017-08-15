crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL.Rel1.2.tar.bz2

About build environment, the perl package must be version 5.12.3 (perl --version).
If you are using higher perl version, find /extern/broadcom-bsp/kernel/linux-3.4rt/kernel/timeconst.pl --> line 373
change "if (!defined(@val))" to "if (!@val)"

