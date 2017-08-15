###########################################################################
#
# bosRules_LinuxKernel.mk
#
# This file contains Linux kernel space specific build rules for XCHG.
#
###########################################################################

# mips-specific options
ifeq ($(strip $(BRCM_CHIP)), 68500)
XCHG_C_MIPS_OPT := -mno-abicalls -mips32r2 -mlong-calls -G 0 \
				  -fno-pic -Wa,--trap
else
XCHG_C_MIPS_OPT := -mno-abicalls -mips32 -mlong-calls -G 0 \
				  -fno-pic -Wa,--trap
endif

ifneq ($(strip $(XCHG_EXTRA_C_MIPS_OPT)),)
XCHG_C_MIPS_OPT += $(XCHG_EXTRA_C_MIPS_OPT)
endif

XCHG_ASM_MIPS_OPT := -MD -Wa,-mips32 -mlong-calls -mno-abicalls -fno-pic

# arm-specific options
XCHG_C_ARM_OPT := -marm -mabi=aapcs-linux -mno-thumb-interwork -funwind-tables -D__LINUX_ARM_ARCH__=7 \
                  -march=armv7-a -Uarm -mfpu=vfp3 -mfloat-abi=softfp -fno-stack-protector -fomit-frame-pointer \
                  -Wdeclaration-after-statement -Wno-pointer-sign -Wno-format-security -fno-delete-null-pointer-checks \
                  -fno-strict-overflow

XCHG_ASM_ARM_OPT := ${XCHG_C_ARM_OPT} -MD

XCHG_C_COMMON_OPT = -c -MD -Wall -Wstrict-prototypes -Wno-trigraphs \
                    -Wtype-limits                                   \
                    -fno-strict-aliasing -fno-common -pipe -Werror  \
                    -fno-strict-overflow                            \
                    $(XCHG_C_$(XCHG_LINUX_ARCH)_OPT)

XCHG_C_GLOBAL_DEFS += __KERNEL__ $(XCHG_EXTRA_LINUX_KERNEL_OPT)

include $(XCHG_RULES_DIR)/xChgRules_LinuxCommon.mk
