cmd_arch/arm/boot/compressed/misc.o := /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/bin/arm-unknown-linux-uclibcgnueabi-gcc -Wp,-MD,arch/arm/boot/compressed/.misc.o.d  -nostdinc -isystem /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/lib/gcc/arm-unknown-linux-uclibcgnueabi/4.6.2/include -I/home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/include/linux/kconfig.h -I/extern/broadcom-bsp-4.16L05/shared/opensource/include/bcm963xx -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-bcm963xx/include -Iarch/arm/plat-bcm63xx/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -marm -fno-dwarf2-cfi-asm -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=2048 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DARCADYAN -fpic -fno-builtin -Iarch/arm/boot/compressed -I/home/users/popeye_shen/ac88u-gpl/bcm963xx/bcmdrivers/opensource/include/bcm963xx -I/home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/include/bcm963xx    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(misc)"  -D"KBUILD_MODNAME=KBUILD_STR(misc)" -c -o arch/arm/boot/compressed/misc.o arch/arm/boot/compressed/misc.c

source_arch/arm/boot/compressed/misc.o := arch/arm/boot/compressed/misc.c

deps_arch/arm/boot/compressed/misc.o := \
    $(wildcard include/config/debug/icedcc.h) \
    $(wildcard include/config/cpu/v6.h) \
    $(wildcard include/config/cpu/v6k.h) \
    $(wildcard include/config/cpu/v7.h) \
    $(wildcard include/config/cpu/xscale.h) \
  include/linux/compiler.h \
    $(wildcard include/config/sparse/rcu/pointer.h) \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/bcm/kf/bounce.h) \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
    $(wildcard include/config/brcm/bounce.h) \
  include/linux/compiler-gcc4.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/arch/dma/addr/t/64bit.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/bcm/kf/unaligned/exception.h) \
    $(wildcard include/config/mips/bcm963xx.h) \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/types.h \
  include/asm-generic/int-ll64.h \
  arch/arm/include/generated/asm/bitsperlong.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  include/linux/stddef.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/posix_types.h \
  include/asm-generic/posix_types.h \
  include/linux/linkage.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/linkage.h \
  arch/arm/include/generated/mach/uncompress.h \
    $(wildcard include/config/bcm/kf/arm/bcm963xx.h) \
    $(wildcard include/config/debug/uart/addr.h) \
  arch/arm/include/generated/mach/hardware.h \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/plat/bcm63xx/acp.h) \
  arch/arm/include/generated/asm/sizes.h \
  include/asm-generic/sizes.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/bcmdrivers/opensource/include/bcm963xx/bcm_map_part.h \
    $(wildcard include/config/bcm96318.h) \
    $(wildcard include/config/bcm963268.h) \
    $(wildcard include/config/bcm96328.h) \
    $(wildcard include/config/bcm96362.h) \
    $(wildcard include/config/bcm96838.h) \
    $(wildcard include/config/bcm963138.h) \
    $(wildcard include/config/bcm960333.h) \
    $(wildcard include/config/bcm963381.h) \
    $(wildcard include/config/bcm963148.h) \
    $(wildcard include/config/bcm96848.h) \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/include/bcm963xx/63138_map_part.h \
    $(wildcard include/config/lock.h) \
    $(wildcard include/config/arm.h) \
    $(wildcard include/config/2/bar1/size/mask.h) \
    $(wildcard include/config/2/bar1/disable.h) \
    $(wildcard include/config/lo/match/address/mask.h) \
    $(wildcard include/config/lo/size/256mb.h) \
    $(wildcard include/config/lo/size/max.h) \
    $(wildcard include/config/lo/match/addr/mask.h) \
    $(wildcard include/config/lo/enable/mask.h) \
    $(wildcard include/config/match/mask.h) \
    $(wildcard include/config/match/shift.h) \
    $(wildcard include/config/data/mask.h) \
    $(wildcard include/config/offset/mask.h) \
    $(wildcard include/config/access/en.h) \
    $(wildcard include/config/bcm/gmac.h) \
    $(wildcard include/config/5x3/crossbar/support.h) \
    $(wildcard include/config/4x2/crossbar/support.h) \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/bcmdrivers/opensource/include/bcm963xx/bcmtypes.h \

arch/arm/boot/compressed/misc.o: $(deps_arch/arm/boot/compressed/misc.o)

$(deps_arch/arm/boot/compressed/misc.o):
