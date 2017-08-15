###########################################################################
#
# /xchg_common/xchg_common_targets.mk
#
# This makefile is provides a list of xchg_common targets.
#
###########################################################################


#
# This is needed to get access to the tools that are used in the public defs
# below, e.g. $(TR).
#
include $(XCHG_ROOT)/rules/xChgEnv.mk


# ---------------------------------------------------------------------------
# Target list.
#

XCHG_ALL_TARGETS := \
   assert           \
   blog             \
   bos              \
   containers       \
   crt              \
   mem              \
   rfs              \
   rpc              \
   rpcStubs         \
   sme              \
   str              \
   switch           \
   trace
   
   
# ---------------------------------------------------------------------------
# Target directories. For each target listed above, this provides the 
# directory location of the target, relative to the XCHG_ROOT directory.
#
   
XCHG_DIR_assert            := $(XCHG_ROOT)/assert
XCHG_DIR_blog              := $(XCHG_ROOT)/blog
XCHG_DIR_bos               := $(XCHG_ROOT)/bos
XCHG_DIR_containers        := $(XCHG_ROOT)/containers
XCHG_DIR_crt               := $(XCHG_ROOT)/crt
XCHG_DIR_mem               := $(XCHG_ROOT)/mem
XCHG_DIR_rfs               := $(XCHG_ROOT)/rfs
XCHG_DIR_rpc               := $(XCHG_ROOT)/rpc
XCHG_DIR_rpcStubs          := $(XCHG_ROOT)/rpcStubs
XCHG_DIR_sme               := $(XCHG_ROOT)/sme
XCHG_DIR_str               := $(XCHG_ROOT)/str
XCHG_DIR_switch            := $(XCHG_ROOT)/switch
XCHG_DIR_trace             := $(XCHG_ROOT)/trace


# ---------------------------------------------------------------------------
# Target public includes. For each target listed above, this provides the
# public include directory for the module. By default, these will be added
# to the global search path for this VOB.
#

XCHG_PUBLIC_INC_assert           := $(XCHG_DIR_assert)/inc $(XCHG_DIR_assert)/cfginc
XCHG_PUBLIC_INC_blog             := $(XCHG_DIR_blog)/inc $(XCHG_DIR_blog)/cfginc
XCHG_PUBLIC_INC_bos              := $(XCHG_DIR_bos)/publicInc
XCHG_PUBLIC_INC_containers       := $(XCHG_DIR_containers)
XCHG_PUBLIC_INC_mem              := $(XCHG_DIR_mem)/inc
XCHG_PUBLIC_INC_rfs              := $(XCHG_DIR_rfs)/inc
XCHG_PUBLIC_INC_rpc              := $(XCHG_DIR_rpc)/inc $(XCHG_DIR_rpc)/cfginc
XCHG_PUBLIC_INC_rpcStubs         := $(XCHG_DIR_rpcStubs)/inc
XCHG_PUBLIC_INC_sme              := $(XCHG_DIR_sme)
XCHG_PUBLIC_INC_str              := $(XCHG_DIR_str)
XCHG_PUBLIC_INC_switch           := $(XCHG_DIR_switch)/inc $(XCHG_DIR_switch)/cfginc
XCHG_PUBLIC_INC_trace            := $(XCHG_DIR_trace)


ifeq ($(XCHG_OS),LinuxKernel)
XCHG_PUBLIC_INC_crt              := $(XCHG_DIR_crt)/inc/LinuxKernel
else
XCHG_PUBLIC_INC_crt              := $(XCHG_DIR_crt)/inc/generic
endif


# ---------------------------------------------------------------------------
# Target public defines. For each target listed above, this provides the
# public defines for the module. By default, these will be added
# to the global compiler options for this VOB.
#

XCHG_PUBLIC_DEFS_assert          :=
XCHG_PUBLIC_DEFS_blog            :=
XCHG_PUBLIC_DEFS_bos             := BOS_OS_$(XCHG_OS)
XCHG_PUBLIC_DEFS_containers      := 
XCHG_PUBLIC_DEFS_crt             := 
XCHG_PUBLIC_DEFS_mem             := 
XCHG_PUBLIC_DEFS_rfs             := 
XCHG_PUBLIC_DEFS_rpc             := 
XCHG_PUBLIC_DEFS_rpcStubs        := 
XCHG_PUBLIC_DEFS_sme             := 
XCHG_PUBLIC_DEFS_str             :=
XCHG_PUBLIC_DEFS_switch          :=
XCHG_PUBLIC_DEFS_trace           :=
