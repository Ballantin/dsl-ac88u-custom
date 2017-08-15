###########################################################################
#
# This makefile is used to build the endpoint module.
#
###########################################################################


#
# This should match the target name.
#
XCHG_LIB_BASE  := endpt

ENDPT_ROOT    := $(VRG_DIR_$(XCHG_LIB_BASE))


#
# List of all source files. Do not specify directories, we use vpath below.
#
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(notdir $(wildcard $(ENDPT_ROOT)/src/*.c $(ENDPT_ROOT)/src/*.s))

# Don't build cache test for ARM
ifeq ($(CXC_LINUX_ARCHITECTURE),ARM)
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(filter-out vrgCacheTest.s, $(XCHG_SRC_FILES_$(XCHG_LIB_BASE)))
endif

XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))

#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
vpath %.c $(ENDPT_ROOT)/src
vpath %.s $(ENDPT_ROOT)/src


#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) := $(VRG_LDX_HAUSWARE_INC_DIR) $(VRG_LDX_HAUSWARE_SYSINC_DIR)


#
# Add in private defs that are only needed when compiling this library.
#
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) :=


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
