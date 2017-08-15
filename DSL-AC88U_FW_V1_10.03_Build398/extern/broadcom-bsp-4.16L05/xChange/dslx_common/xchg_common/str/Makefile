###########################################################################
#
# This makefile is used to build the STR library, and optionally, the STR
# test code.
#
# To build the STR test code, execute:
#
#   make XCHG_OS=Win32 TestPrintf
#
###########################################################################

XCHG_ROOT = ..
XCHG_LIB_BASE  = str

include $(XCHG_ROOT)/rules/xChgEnv.mk
include $(XCHG_ROOT)/str/rules/strEnv.mk

XCHG_SRC_FILES = \
    strMaxCat.c     \
    strMaxCpy.c     \
    strCatSize.c    \
    strCmpNoCaseSize.c \
    strPrintf.c

#
# Now define stuff for testing
#

XCHG_EXE_LIBS = $(call XCHG_MAKE_LIB_NAME,str)

include $(XCHG_ROOT)/rules/xChgRules.mk

.PHONY: TestPrintf

TestPrintf: $(call XCHG_MAKE_EXE_NAME,TestPrintf)

test: TestPrintf
    $(call XCHG_MAKE_EXE_NAME,$<)
    
