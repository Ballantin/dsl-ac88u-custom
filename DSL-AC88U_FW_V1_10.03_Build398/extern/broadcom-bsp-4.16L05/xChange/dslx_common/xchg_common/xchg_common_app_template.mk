###########################################################################
#
# This makefile is an example project makefile that illustrates how to
# build xchg_common targets.
#
###########################################################################

XCHG_ROOT = .
include $(XCHG_ROOT)/rules/xChgEnv.mk

#
# Libraries and executables to build.
#
libTargets = str assert bos
exeTarget  = Test
allTargets = libTargets $(exeTarget)


#
# Initialize these to NULL here using immediate variable evaluation. This 
# guarantees that immediate evaluation (which is more efficient than defered
# evaluation) will continue to be used when the variables are added to below.
# 
XCHG_C_GLOBAL_DEFS      :=
XCHG_C_GLOBAL_INCLUDE   :=
 
 
#
# Create the public include path list.
#
export XCHG_MAKE_TARGETS := $(libTargets)
include $(XCHG_ROOT)/xchg_common_env.mk
XCHG_C_GLOBAL_DEFS      += $(XCHG_COMPOSITE_PUBLIC_DEFS)
XCHG_C_GLOBAL_INCLUDE   += $(XCHG_COMPOSITE_PUBLIC_INCLUDE)
   
   
   
include $(XCHG_ROOT)/rules/xChgRules.mk
   
   
#
# Default target.
#
.PHONY: all $(allTargets)
all: $(allTargets)

#
# Build libraries.
#
.PHONY: libTargets
libTargets:
   $(MAKE) -C $(XCHG_ROOT) -f xchg_common.mk 


#
# Clean libraries.
#
.PHONY: clean_lib
clean_lib:
    $(MAKE) -C $(XCHG_ROOT) -f xchg_common.mk clean
 
    
#
# Build executable.
#
$(exeTarget) : $(call XCHG_MAKE_EXE_NAME,$(exeTarget))
$(call XCHG_MAKE_EXE_NAME,$(exeTarget)): $(call XCHG_MAKE_LIB_NAME,$(libTargets))
   $(ECHO) 'Linking $@'
