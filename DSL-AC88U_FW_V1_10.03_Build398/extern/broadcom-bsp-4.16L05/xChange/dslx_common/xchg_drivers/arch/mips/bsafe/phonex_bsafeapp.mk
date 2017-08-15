###########################################################################
#
# This makefile is used to build the modules provided by Broadcom Security
# Group for Broadsafe uHSM.
#
###########################################################################

#
# This should match the target name.
#
XCHG_LIB_BASE     := phonex_bsafeapp

BSAFE_APP_ROOT    := $(XDRV_DIR_$(XCHG_LIB_BASE))

 

#
# We use notdir and vpath so that the object files don't wind up in
# nested subdirectories
#
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(notdir $(wildcard $(BSAFE_APP_ROOT)/lib/*.c))

XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))

vpath %.c $(BSAFE_APP_ROOT)/lib



#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) := $(BSAFE_APP_ROOT)/shared $(BSAFE_APP_ROOT)/sys $(BSAFE_APP_ROOT)/lib

#
# Add in private defs that are only needed when compiling this library.
#
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) :=


#
# To set a module specific compiler option, create a target specific make
# variable which sets the option and depends on the module target name.
# For example, to enable compiler optimizations for this module:
#
#     $(XCHG_LIB_BASE): XCHG_C_OPTIMIZE_OPT := $(XCHG_C_OPTIMIZE_OPT_ENABLE)
#
# or, to disable optimizations:
#
#     $(XCHG_LIB_BASE): XCHG_C_OPTIMIZE_OPT := $(XCHG_C_OPTIMIZE_OPT_DISABLE)
