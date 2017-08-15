###########################################################################
#
# /xchg_common/rules/xChgCheckVars.mk
#
# This is a common makefile which is used to check that a bunch of required
# variables have been set.
#
###########################################################################

ifeq ($(XCHG_ROOT),)
$(error "XCHG_ROOT variable hasn't been set")
endif

ifeq ($(XCHG_OS),)
$(error "XCHG_OS variable hasn't been set")
endif

ifeq ($(XCHG_TOOLS_BIN_DIR),)
$(error "XCHG_TOOLS_BIN_DIR variable hasn't been set")
endif

ifeq ($(XCHG_OBJ_DIR),)
$(error "XCHG_OBJ_DIR variable hasn't been set")
endif

ifeq ($(XCHG_LIB_DIR),)
$(error "XCHG_LIB_DIR variable hasn't been set")
endif

ifeq ($(XCHG_EXE_DIR),)
$(error "XCHG_EXE_DIR variable hasn't been set")
endif

ifeq ($(XCHG_RULES_DIR),)
$(error "XCHG_RULES_DIR variable hasn't been set")
endif

