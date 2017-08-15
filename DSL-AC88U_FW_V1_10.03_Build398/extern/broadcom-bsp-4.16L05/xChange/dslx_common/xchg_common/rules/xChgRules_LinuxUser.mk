###########################################################################
#
# bosRules_LinuxUser.mk
#
# This file contains Linux user space specific build rules for XCHG.
#
###########################################################################

XCHG_C_COMMON_OPT := -c -Wall -MD -s -Os -fomit-frame-pointer -fexceptions -fno-strict-aliasing -fPIC -Werror -Wtype-limits

# XCHG_C_COMMON_OPT := -c -MD -Wall -s -Os -fomit-frame-pointer -fno-exceptions

# XCHG_CPP_COMMON_OPT := -c -Wall -s -Os -fomit-frame-pointer -fno-exceptions -fno-rtti -fno-dollars-in-identifiers

XCHG_CPP_COMMON_OPT := -c -Wall -s -Os -fomit-frame-pointer -fexceptions -fno-rtti -fno-dollars-in-identifiers -fno-strict-aliasing -Wtype-limits

XCHG_C_GLOBAL_DEFS += LINUX

include $(XCHG_RULES_DIR)/xChgRules_LinuxCommon.mk
