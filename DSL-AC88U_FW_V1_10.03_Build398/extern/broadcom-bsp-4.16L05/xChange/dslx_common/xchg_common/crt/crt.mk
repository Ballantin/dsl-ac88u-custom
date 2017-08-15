###########################################################################
#
# This makefile is used to build the CRT module.
#
###########################################################################

XCHG_LIB_BASE  = crt

CRT_ROOT    := $(XCHG_DIR_$(XCHG_LIB_BASE))

# If the target OS is Linux kernel, then use the version of CRT specific
# to that OS. Otherwise, use the generic CRT module.
#
ifeq ($(XCHG_OS),LinuxKernel)
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(notdir $(wildcard $(CRT_ROOT)/src/LinuxKernel/*.c))
else
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(notdir $(wildcard $(CRT_ROOT)/src/generic/*.c))
endif

XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))

#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
ifeq ($(XCHG_OS),LinuxKernel)
vpath %.c $(CRT_ROOT)/src/LinuxKernel
else
vpath %.c $(CRT_ROOT)/src/generic
endif
include $(XCHG_ROOT)/crt/rules/crtEnv.mk
