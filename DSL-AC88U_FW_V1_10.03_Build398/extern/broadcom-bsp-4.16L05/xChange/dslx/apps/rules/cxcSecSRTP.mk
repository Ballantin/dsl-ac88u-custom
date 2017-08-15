#############################################################################
#
#    Copyright 2000-2005  Broadcom Corporation
#    All Rights Reserved
#    No portions of this material may be reproduced in any form without the
#    written permission of:
#        Broadcom Corporation
#        16215 Alton Parkway
#        P.O. Box 57013
#        Irvine, California 92619-7013
#    All information contained in this document is Broadcom Corporation
#    company private, proprietary, and trade secret.
#
#############################################################################
#
#    Filename: cxcSecSRTP.mk
#
#############################################################################
#
#  This makefile is used to build Sec_SRTP targets. It is intended to be 
#  included from a project specific makefile.
#
#############################################################################


# ---------------------------------------------------------------------------
#  Environment setup.
#


#
# These are the default xchg_common targets to be built.
#
CXC_SMAPI_DEFAULT_TARGETS := srtp


#
# There is no default list of Sec_SRTP targets. This must be specified by the
# containing makefile.
#
ifeq ($(CXC_SMAPI_TARGET_LIST),)
CXC_SMAPI_TARGET_LIST = $(CXC_SMAPI_DEFAULT_TARGETS)
endif

CXC_SMAPI_OUTPUT_DIR := smapi
#
# Setup the xchg_common build enviroment. Note that the following will apply to
# all modules that use the xchg_common build environment, and is not strictly 
# limited to the modules contained within the xchg_common VOB.
#

export SMAPI_OS          := $(CXC_OS_TYPE)
export SMAPI_HOST_TYPE   := $(CXC_HOST_TYPE)
export SMAPI_ROOT        := $(CXC_SMAPI_DIR)
ifneq ($(findstring Linux,$(SMAPI_OS)),)
export SMAPI_LINUX_ARCH  := $(CXC_LINUX_ARCHITECTURE)
endif

##############################################
#	SMAPI-SRTP SETTING
#	- Linking requires BOS, STR, ASSERT of XCHG_COMMON
#	- Compiling requires BOS
##############################################

#Only have one target 'srtp'
export SMAPI_MAKE_TARGETS := $(CXC_SMAPI_TARGET_LIST)
export SMAPI_C_GLOBAL_DEFS = $(CXC_APPS_GLOBAL_DEFS)
export SMAPI_APPS_GLOBAL_CFG_INCLUDE = $(CXC_APPS_GLOBAL_CFG_INCLUDE)
export SMAPI_C_GLOBAL_INCLUDE = $(CXC_APPS_GLOBAL_INCLUDE)
export SMAPI_LIB_DIR:=$(CXC_BASE_LIB_DIR)/$(CXC_SMAPI_OUTPUT_DIR)
export SMAPI_OBJ_DIR:=$(CXC_BASE_OBJ_DIR)/$(CXC_SMAPI_OUTPUT_DIR)

export SMAPI_OS_ROOT_DIR := $(CXC_OS_ROOT_DIR)
export SMAPI_COMPILER_ROOT := $(CXC_COMPILER_ROOT_DIR)

include $(SMAPI_ROOT)/SMAPI_env.mk

#
# Rules to build/clean xchg_common modules all at once.
#
.PHONY: smapi
smapi :
	$(MAKE) -C $(SMAPI_ROOT) -f SMAPI.mk 
   
.PHONY: clean_smapi 
clean_smapi:
	$(MAKE) -C $(SMAPI_ROOT) -f SMAPI.mk clean

#
# Rules to build/clean xchg_common modules independantly.
#
.PHONY: $(CXC_SMAPI_TARGET_LIST)

$(CXC_SMAPI_TARGET_LIST):
	@$(ECHO) 'Making $@ ... '
	$(MAKE) -C $(SMAPI_ROOT) -f SMAPI.mk $@ 
    
PHONY: $(addprefix clean_,$(CXC_SMAPI_TARGET_LIST))
$(addprefix clean_,$(CXC_SMAPI_TARGET_LIST)):
	$(MAKE) -C $(SMAPI_ROOT) -f SMAPI.mk $@    
   
.PHONY: print-smapi-targets
print-smapi-targets:
	@$(ECHO) -e 'CXC_SMAPI_TARGET_LIST =\n  $(subst $(space),\n  ,$(strip $(CXC_SMAPI_TARGET_LIST)))'
