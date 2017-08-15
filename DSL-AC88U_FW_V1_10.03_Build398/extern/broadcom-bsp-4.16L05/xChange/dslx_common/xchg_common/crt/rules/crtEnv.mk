###########################################################################
#
# crtEnv.mk
#
# This file contains common definitions that should be included by all
# makefiles which use CRT.
#
###########################################################################

ifeq ($(CRT_ROOT),)
export CRT_ROOT = $(XCHG_ROOT)/crt
endif

# The following takes XCHG_OS which is mixed case, and creates
# CRT_OS_XXX=1 where XXX is $(XCHG_OS) in upper case.
#

export XCHG_C_GLOBAL_DEFS += CRT_OS_$(shell $(ECHO) $(XCHG_OS) | $(TR) 'a-z' 'A-Z')=1

#
# Add the public include directory to the search path
#

export XCHG_C_GLOBAL_INCLUDE += $(CRT_ROOT)/inc/$(XCHG_OS)
