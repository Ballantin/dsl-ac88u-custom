#############################################################################
#
# Copyright © 2008 - 2009 Broadcom Corporation
#
# This program is the proprietary software of Broadcom Corporation and/or its
# licensors, and may only be used, duplicated, modified or distributed pursuant to
# the  terms and  conditions of  a separate,  written license  agreement executed
# between you and Broadcom (an "Authorized  License").  Except as set forth in  an
# Authorized License, Broadcom  grants no license  (express or implied),  right to
# use, or waiver of any kind with respect to the Software, and Broadcom  expressly
# reserves all rights in and to the Software and all intellectual property  rights
# therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
# SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
# USE OF THE SOFTWARE.
#
# Except as expressly set forth in the Authorized License,
#
# 1.     This program, including its structure, sequence and organization,
# constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
# reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
# information only  in connection  with your  use of  Broadcom integrated  circuit
# products.
#
# 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
# AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
# WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
# THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
# TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
# OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
# CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
# PERFORMANCE OF THE SOFTWARE.
#
# 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
# LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
# EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
# OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
# PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
# SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
# REMEDY.
#
##################################################################################
#
#               This makefile is used to build the CCTK application.
#
##################################################################################

#
# This should match the target name.
#
XCHG_LIB_BASE  := lib_sip_cctk

XCHG_LIB_DIR = $(CXC_CABLEX_DIR)/lib/$(CXC_OS_TYPE)
XCHG_OBJ_DIR = $(CXC_CABLEX_DIR)/obj/$(CXC_OS_TYPE)/sip_cctk

#
# Define necessary variable used in the core makefile(s).
#
CCTK_ROOT      := $(CXC_CCTK_ROOT)
M5T_SIPUA_ROOT := $(CXC_M5T_SIPUA_ROOT)
EXPAT_ROOT     := $(CXC_EXPAT_ROOT)
NAT_ROOT       := $(CXC_NAT_ROOT)
STK_ROOT       := $(M5T_SIPUA_ROOT)

#
# The following variables are checked for emptyness but are not needed
# in the CxC build environment, so we just define them to some 'random'
# value.
#
CCTK_BUILD_CONFIG_INCLUDE := $(CCTK_ROOT)
CCTK_BUILD_IMPORTED_INCLUDE := $(CCTK_ROOT)
#
# Import the SIP stack paths necessary for CCTK compilation.
#
CCTK_BUILD_STACK_INCLUDE :=    $(STK_ROOT)/CustomConfig                          \
                               $(STK_ROOT)/M5TSce                                \
                               $(STK_ROOT)/M5TSdp                                \
                               $(STK_ROOT)/M5TSipUa                              \
                               $(STK_ROOT)/M5TFramework                          \
                               $(STK_ROOT)/M5TFramework/Custom/Bos/BosSingleton  \
                               $(STK_ROOT)/M5TXcapClient                         \
                               $(STK_ROOT)/ThirdParty/regex                      \
                               $(EXPAT_ROOT)/expat/lib							 \
                               $(BCM_FSBUILD_DIR)/public/include
#
# Import the NAT paths necessary for CCTK compilation.
#
CCTK_BUILD_NAT_INCLUDE :=      $(NAT_ROOT)/stacks/cfginc                         \
                               $(NAT_ROOT)/stacks/ice/inc                        \
                               $(NAT_ROOT)/stacks/stunc/inc                      \
                               $(NAT_ROOT)/stacks/stunc/stun/inc

#
# Reference CCTK makefile.
#
include $(CCTK_ROOT)/cctk/build/xchg/cctk.mk
#


#
# List of all source files. Do not specify directories, we use vpath below.
#
XCHG_SRC_FILES_$(XCHG_LIB_BASE) := $(CCTK_BUILD_SRC_FILES)

XCHG_SRC_FILES += $(XCHG_SRC_FILES_$(XCHG_LIB_BASE))

XCHG_RECURSIVE_BUILD = 1

#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
# CCTK -- C++ design.
#
vpath %.c   $(CCTK_BUILD_SRC_DIRS)
vpath %.cpp $(CCTK_BUILD_SRC_DIRS)

# include our env make files
include $(XCHG_ROOT)/rules/xChgEnv.mk

#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) :=  $(CXC_APPS_DIR)/sip_cctk/cfginc                          \
                                          $(CXC_APPS_DIR)/$(CXC_APP_NAME)/cfginc/mta               \
                                          $(CXC_APPS_DIR)/cfginc/mta                               \
                                          $(CXC_APPS_DIR)/cfginc/xchg_common                       \
                                          $(CXC_MTA_DIR)/inc/cfg                                   \
                                          $(CXC_LIB_SUPPORT_DIR)/xchg_common/str                   \
                                          $(CXC_LIB_SUPPORT_DIR)/xchg_common/bos/publicInc         \
                                          $(CXC_LIB_SUPPORT_DIR)/xchg_common/assert/cfginc         \
                                          $(CXC_LIB_SUPPORT_DIR)/xchg_common/assert/inc            \
										  $(BCM_FSBUILD_DIR)/public/include

XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(CCTK_BUILD_COMPOSITE_INCLUDE)
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(CCTK_BUILD_STACK_INCLUDE)
XCHG_C_LOCAL_INCLUDE_$(XCHG_LIB_BASE) += $(CCTK_BUILD_NAT_INCLUDE)


#
# This is required so that the CCTK module know what OS it is dealing with.
#
XCHG_C_GLOBAL_DEFS_$(XCHG_LIB_BASE) += CXC_OS_$(shell $(ECHO) $(CXC_OS_TYPE) | $(TR) 'a-z' 'A-Z')=1


#
# Add in private defs that are only needed when compiling those modules.
#
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) := MXD_RELEASE MXD_OS_BOS_LINUX_BE BOS_OS_LINUXUSER
XCHG_C_LOCAL_DEFS_$(XCHG_LIB_BASE) += $(CCTK_BUILD_DEFS)

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
#     $(XCHG_LIB_BASE): XCHG_C_OPTIMIZE_OPT = $(XCHG_C_OPTIMIZE_OPT_ENABLE)
#
# or, to disable optimizations:
#
#     $(XCHG_LIB_BASE): XCHG_C_OPTIMIZE_OPT = $(XCHG_C_OPTIMIZE_OPT_DISABLE)

XCHG_C_OPT_NO_REDUNDANT_DECLS = $(XCHG_C_OPT_NO_REDUNDANT_DECLS_DISABLE)
XCHG_C_OPT_SUPPRESS_WARNINGS = $(XCHG_C_OPT_SUPPRESS_WARNINGS_ENABLE)
XCHG_CPP_EXCEPTIONS_OPT = $(XCHG_CPP_EXCEPTIONS_ENABLE)

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

