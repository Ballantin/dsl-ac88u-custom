###########################################################################
#
# bosEnv_LinuxKernel.mk
#
# This file contains Linux Kernel specific defines for the xchg_common
# build environment.
#
# This file should only define variables and not have any rules in it.
#
###########################################################################


# ---------------------------------------------------------------------------
# OS-specific output file extentions and prefixes.
#
 
XCHG_LIB_PREFIX :=
XCHG_LIB_EXT    := .a
XCHG_OBJ_EXT    := .o
XCHG_EXE_EXT    := .hex


# ---------------------------------------------------------------------------
# XCHG_RELEASE defined to 1 appends "Release" to lib dirs, obj dirs and
# exe dirs.  The default is a debug release, which has no string appended
# (Debug was NOT appended to maintain compatibility with the previous versions)
#

export XCHG_RELEASE
ifeq ($(XCHG_RELEASE),1)
XCHG_RELEASE_EXT:=Release
else
XCHG_RELEASE_EXT:=
endif
