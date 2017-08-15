###########################################################################
#
# This makefile is used to build the SIP Call Manager module.
#
###########################################################################

#
# This should match the target name.
#
XCHG_LIB_BASE  := lib_sip_cmgr

XCHG_LIB_DIR = $(CXC_CABLEX_DIR)/lib/$(CXC_OS_TYPE)
XCHG_OBJ_DIR = $(CXC_CABLEX_DIR)/obj/$(CXC_OS_TYPE)/sip_cmgr

#
# Define necessary variable used in the core makefile(s).
#
CCTK_ROOT := $(CXC_CCTK_ROOT)
CMGR_ROOT := $(CCTK_ROOT)/cmgr

#
# The following variables are checked for emptyness but are not needed
# in the CxC build environment, so we just define them to some 'random'
# value.
#
CMGR_BUILD_CONFIG_INCLUDE := $(CMGR_ROOT)
CMGR_BUILD_IMPORTED_INCLUDE := $(CMGR_ROOT)
#
# Import the CCTK paths necessary for CMGR compilation.
#
CMGR_BUILD_CCTK_INCLUDE :=    $(CCTK_ROOT)/cctk/cfginc              \
                              $(CCTK_ROOT)/cctk/inc

#
# Reference CMGR makefile.
#
include $(CCTK_ROOT)/cmgr/build/xchg/cmgr.mk

#
# List of all source files. Do not specify directories, we use vpath below.
#
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(CMGR_BUILD_SRC_FILES)

XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))

XCHG_RECURSIVE_BUILD = 1


#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
# CMGR -- C only.
#
vpath %.c   $(CMGR_BUILD_SRC_DIRS)

# include our env make files
include $(XCHG_ROOT)/rules/xChgEnv.mk

#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) :=  $(CXC_APPS_DIR)/sip_cmgr/cfginc                          \
                                          $(CXC_APPS_DIR)/sip_cctk/cfginc                          \
                                          $(CXC_APPS_DIR)/$(CXC_APP_NAME)/cfginc/mta               \
                                          $(CXC_APPS_DIR)/cfginc/mta                               \
                                          $(CXC_APPS_DIR)/cfginc/xchg_common                       \
                                          $(CXC_MTA_DIR)/inc/cfg                                   \
                                          $(CXC_LIB_SUPPORT_DIR)/sec_SRTP/srtp                     \
                                          $(CXC_LIB_SUPPORT_DIR)/xchg_common/str                   \
                                          $(CXC_LIB_SUPPORT_DIR)/xchg_common/bos/publicInc         \
                                          $(CXC_LIB_SUPPORT_DIR)/xchg_common/assert/cfginc         \
                                          $(CXC_LIB_SUPPORT_DIR)/xchg_common/assert/inc            \
                                          $(CXC_VIEW_DIR)/userspace/private/apps/vodsl/voip/inc    \
										  $(BCM_FSBUILD_DIR)/public/include                        \
                                          $(CCTK_ROOT)/apps/inc
                                      
#                                           $(CXC_LIB_SUPPORT_DIR)/voice_res_gw/codec                \
#                                           $(CXC_LIB_SUPPORT_DIR)/voice_res_gw/endpt/inc            \
#                                           $(CXC_LIB_SUPPORT_DIR)/voice_res_gw/inc                  \
#                                           $(CXC_LIB_SUPPORT_DIR)/voice_res_gw/tpdCtl/inc           \
#                                           $(CXC_LIB_SUPPORT_DIR)/voice_res_gw/dectCtl              \
#                                           $(CXC_LIB_SUPPORT_DIR)/voice_res_gw/hdsp/inc             \
#                                           $(CXC_LIB_SUPPORT_DIR)/xchg_drivers/inc                  \
#                                           $(CXC_LIB_SUPPORT_DIR)/ldx_hausware/hausware/inc         \
#                                           $(CXC_LIB_SUPPORT_DIR)/sec_SRTP/srtp                     \                                        
                                              
                                         
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(CMGR_BUILD_COMPOSITE_INCLUDE)
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(CMGR_BUILD_CCTK_INCLUDE)


#
# This is required so that the CCTK/CMGR/STUN/ICE module know what OS it is dealing with.
#
XCHG_C_GLOBAL_DEFS_$(XCHG_LIB_BASE) += CXC_OS_$(shell $(ECHO) $(CXC_OS_TYPE) | $(TR) 'a-z' 'A-Z')=1

#
# Add in private defs that are only needed when compiling those modules.
#
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) := MXD_RELEASE MXD_OS_LINUX_BE BOS_OS_LINUXUSER
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) += $(CMGR_BUILD_DEFS)

# Add xchg_common public includes.
include $(CXC_XCHG_COMMON_DIR)/xchg_common_env.mk
export XCHG_C_GLOBAL_INCLUDE += $(XCHG_COMPOSITE_PUBLIC_INCLUDE)
export XCHG_C_GLOBAL_DEFS    += $(XCHG_COMPOSITE_PUBLIC_DEFS)

# Assign lib specific variable to global and local variables
XCHG_C_LOCAL_INCLUDE += $(XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE))
XCHG_C_LOCAL_DEFS    += $(XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE))
XCHG_C_GLOBAL_DEFS   += $(XCHG_C_GLOBAL_DEFS_$(XCHG_LIB_BASE))

#
# To set a module specific compiler option, create a target specific make
# variable which sets the option and depends on the module target name.
# For example, to enable compiler optimizations for this module:
#
#     XCHG_C_OPTIMIZE_OPT = $(XCHG_C_OPTIMIZE_OPT_ENABLE)
#
# or, to disable optimizations:
#
#     XCHG_C_OPTIMIZE_OPT = $(XCHG_C_OPTIMIZE_OPT_DISABLE)

XCHG_C_OPT_NO_REDUNDANT_DECLS = $(XCHG_C_OPT_NO_REDUNDANT_DECLS_DISABLE)
XCHG_C_OPT_SUPPRESS_WARNINGS = $(XCHG_C_OPT_SUPPRESS_WARNINGS_ENABLE)
XCHG_CPP_EXCEPTIONS_OPT = $(XCHG_CPP_EXCEPTIONS_ENABLE)

ifneq ($(strip $(BUILD_GDBSERVER)),)
XCHG_C_LOCAL_COMPILER_OPTS += -g
endif

XCHG_C_LOCAL_COMPILER_OPTS += -Wno-error

###########################################################################

# gcc -c -Os -fomit-frame-pointer -I../stacks/sip_mx  -fno-exceptions -fno-rtti -fno-dollars-in-identifiers -Wall -Wstrict-prototypes 
# XCHG_C_LOCAL_COMPILER_OPTS += -fomit-frame-pointer -fno-exceptions -fno-rtti -fno-dollars-in-identifiers -Wall -Wstrict-prototypes 

# -fomit-frame-pointer
#     Don't keep the frame pointer in a register for functions that don't need
#     one. This avoids the instructions to save, set up and restore frame
#     pointers; it also makes an extra register available in many functions. It
#     also makes debugging impossible on some machines. On some machines, such as
#     the Vax, this flag has no effect, because the standard calling sequence
#     automatically handles the frame pointer and nothing is saved by pretending
#     it doesn't exist. The machine-description macro FRAME_POINTER_REQUIRED
#     controls whether a target machine supports this flag. See section `Register
#     Usage' in Using and Porting GCC.

# -fno-exceptions
# -fno-rtti

# -fno-dollars-in-identifiers
# -fdollars-in-identifiers
#     Accept `$' in identifiers. You can also explicitly prohibit use of `$' with
#     the option `-fno-dollars-in-identifiers'. (GNU C++ allows `$' by default on
#     some target systems but not others.) Traditional C allowed the character `$'
#     to form part of identifiers. However, ANSI C and C++ forbid `$' in
#     identifiers.

# -Wall
# -Wstrict-prototypes 


include $(XCHG_RULES_DIR)/xChgRules.mk
