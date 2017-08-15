###########################################################################
#
# This makefile is used to build the BOS module.
#
###########################################################################

#
# This should match the target name.
#
XCHG_LIB_BASE  := bos

BOS_ROOT       := $(XCHG_DIR_$(XCHG_LIB_BASE))


#
# List of all source files. Do not specify directories, we use vpath below.
#
# 'None' is a valid operating system. If this is chosen, add a single generic 
# source file to the list of files to compile to keep the build environment
# happy (it doesn't allow modules with no source files). However, if 'None'
# is selected, then no BOS modules should actually be enabled in bosCfg.h, and 
# therefore the contents of this file should be empty, so no harm done.
ifeq ($(XCHG_OS),None)
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := bosInitGeneric.c
else
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(notdir $(wildcard $(BOS_ROOT)/generic/*.c $(BOS_ROOT)/$(XCHG_OS)/*.c))
endif

XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))


#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
vpath %.c $(BOS_ROOT)/generic
vpath %.c $(BOS_ROOT)/$(XCHG_OS)


#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) := $(BOS_ROOT)/$(XCHG_OS) $(BOS_ROOT)/privateInc


#
# Add in private defs that are only needed when compiling this library.
#
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) := BUILDING_BOS


#
# To set a module specific compiler option, create a target specific make
# variable which sets the option and depends on the module target name.
# For example, to enable compiler optimizations for this module:
#
#     $(XCHG_LIB_BASE): XCHG_C_OPTIMIZE_OPT = $(XCHG_C_OPTIMIZE_OPT_ENABLE)
#
# or, to disable optimizations:
#
#     $(XCHG_LIB_BASE): XCHG_C_OPTIMIZE_OPT = $(XCHG_C_OPTIMIZE_OPT_DISABLE)
