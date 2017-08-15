cmd_/home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/flash/flash_api.o := /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/bin/arm-unknown-linux-uclibcgnueabi-gcc -Wp,-MD,/home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/flash/.flash_api.o.d  -nostdinc -isystem /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/lib/gcc/arm-unknown-linux-uclibcgnueabi/4.6.2/include -I/home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include -Iarch/arm/include/generated -Iinclude  -include /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/include/linux/kconfig.h -I/extern/broadcom-bsp-4.16L05/shared/opensource/include/bcm963xx -D__KERNEL__ -mlittle-endian -Iarch/arm/mach-bcm963xx/include -Iarch/arm/plat-bcm63xx/include -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -marm -fno-dwarf2-cfi-asm -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables -D__LINUX_ARM_ARCH__=7 -march=armv7-a -msoft-float -Uarm -Wframe-larger-than=2048 -fno-stack-protector -Wno-unused-but-set-variable -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -fconserve-stack -DARCADYAN -DCONFIG_BCM963138 -I/home/users/popeye_shen/ac88u-gpl/bcm963xx/bcmdrivers/opensource/include/bcm963xx -I/home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/include/bcm963xx -DINC_CFI_FLASH_DRIVER=0 -DINC_SPI_FLASH_DRIVER=1    -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(flash_api)"  -D"KBUILD_MODNAME=KBUILD_STR(flash_api)" -c -o /home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/flash/flash_api.o /home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/flash/flash_api.c

source_/home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/flash/flash_api.o := /home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/flash/flash_api.c

deps_/home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/flash/flash_api.o := \
    $(wildcard include/config/mtd/bcm/spi/nand.h) \
    $(wildcard include/config/bcm96838.h) \
    $(wildcard include/config/bcm963381.h) \
    $(wildcard include/config/bcm963138.h) \
    $(wildcard include/config/bcm96848.h) \
    $(wildcard include/config/bcm96362.h) \
    $(wildcard include/config/bcm96328.h) \
    $(wildcard include/config/bcm963268.h) \
    $(wildcard include/config/bcm96828.h) \
    $(wildcard include/config/bcm963148.h) \
  include/linux/kernel.h \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/bcm/kf/printk/int/enabled.h) \
    $(wildcard include/config/bcm/printk/int/enabled.h) \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/atomic/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/numa.h) \
    $(wildcard include/config/compaction.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/bcm/kf/android.h) \
    $(wildcard include/config/bcm/android.h) \
  include/linux/sysinfo.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
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
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/posix_types.h \
  include/asm-generic/posix_types.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/lib/gcc/arm-unknown-linux-uclibcgnueabi/4.6.2/include/stdarg.h \
  include/linux/linkage.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/linkage.h \
  include/linux/bitops.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/bitops.h \
    $(wildcard include/config/smp.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/preempt/rt/full.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
  include/linux/typecheck.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/irqflags.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/ptrace.h \
    $(wildcard include/config/cpu/endian/be8.h) \
    $(wildcard include/config/arm/thumb.h) \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/hwcap.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/arch_hweight.h \
  include/asm-generic/bitops/const_hweight.h \
  include/asm-generic/bitops/lock.h \
  include/asm-generic/bitops/le.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  include/asm-generic/bitops/ext2-atomic-setbit.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/printk.h \
    $(wildcard include/config/early/printk.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/bcm/kf/extra/debug.h) \
    $(wildcard include/config/dynamic/debug.h) \
  include/linux/init.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/hotplug.h) \
  include/linux/dynamic_debug.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/div64.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/compiler.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel/linux-3.4rt/arch/arm/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
    $(wildcard include/config/thumb2/kernel.h) \
    $(wildcard include/config/debug/bugverbose.h) \
    $(wildcard include/config/arm/lpae.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/preempt/rt/base.h) \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/bcmdrivers/opensource/include/bcm963xx/bcm_map_part.h \
    $(wildcard include/config/bcm96318.h) \
    $(wildcard include/config/bcm960333.h) \
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
  arch/arm/include/generated/mach/hardware.h \
    $(wildcard include/config/bcm/kf/arm/bcm963xx.h) \
    $(wildcard include/config/mmu.h) \
    $(wildcard include/config/plat/bcm63xx/acp.h) \
  arch/arm/include/generated/asm/sizes.h \
  include/asm-generic/sizes.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/include/bcm963xx/bcm_hwdefs.h \
    $(wildcard include/config/brcm/ikos.h) \
    $(wildcard include/config/bcm63138/sim.h) \
    $(wildcard include/config/bcm63148/sim.h) \
    $(wildcard include/config/webjsmin/proj/ui/product/name.h) \
    $(wildcard include/config/arcadyan/model/name.h) \
    $(wildcard include/config/arcadyan/hw/ver.h) \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/include/bcm963xx/arc_hwdefs.h \
  /home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/include/bcm963xx/flash_api.h \

/home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/flash/flash_api.o: $(deps_/home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/flash/flash_api.o)

$(deps_/home/users/popeye_shen/ac88u-gpl/bcm963xx/shared/opensource/flash/flash_api.o):
