###########################################################################
#
# This makefile is used to build the STR module.
#
###########################################################################

#
# This should match the target name.
#
XCHG_LIB_BASE  := str

STR_ROOT       := $(XCHG_DIR_$(XCHG_LIB_BASE))


#
# List of all source files. Do not specify directories, we use vpath below.
#

# UNFINISHED: The zOEMtools_gnu_mips_elf toolchain currently does not provide
# support for a C run-time library. Therefore, we can't suport
# source files that have dependencies on CRT functions.
ifeq ($(XCHG_ENV),gnu_mips_elf)
XCHG_SRC_FILES_$(XCHG_LIB_BASE) :=  strPrintf.c
else
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := strCmpNoCase.c  \
                                   strCpyLower.c   \
                                   strCpyUpper.c   \
                                   strMaxCat.c     \
                                   strMaxCpy.c     \
                                   strPrintf.c     \
                                   strCatSize.c    \
                                   strCmpNoCaseSize.c \
                                   strTok.c

ifneq ($(XCHG_OS),LinuxKernel) 
XCHG_SRC_FILES_$(XCHG_LIB_BASE) += strScanf.c
endif

endif                                   


XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))


#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
vpath %.c $(STR_ROOT)


#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) :=


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
