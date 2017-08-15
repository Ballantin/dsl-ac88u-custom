##################################################################################
#
# Copyright © 2008 - 2009 Broadcom Corporation
#
# This program  is the  proprietary software  of Broadcom  Corporation and/or  its
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
# 1.      This  program,  including  its  structure,  sequence  and  organization,
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
#                  This makefile is used to build the SIP library module.
#
##################################################################################


#
# NOTE: The following variable MUST be defined prior to calling this makefile
#       in order for it to work properly:
#
#     M5T_SIPUA_ROOT:   The absolute path to the directory containing the
#                       'm5t_sipua' codebase.
#                       Example: 'export M5T_SIPUA_ROOT := prot_m5t_sipua'
#
#     M5T_SIPUA_INCLUDE: The list of private (application dependent) include
#                        paths to be considered during the build.
#
#     M5T_SIPUA_LIBDIR:  The absolute path to where the final library should
#                        be stored as the result of the build.
#
#     M5T_SIPUA_OBJDIR:  The absolute path to where the objects built for the
#                        library should be stored as the result of the build.
#
#     CCTK_ROOT:         The absolute path to the directory containing the
#                        'cctk' module.
#                        Example: 'export CCTK_ROOT := prot_cctk'
#
#     EXPAT_ROOT:        The absolute path to the directory containing the
#                        'expat' third party library module.
#                        Example: 'export EXPAT_ROOT := lib_xml_expat'
#
#     XCHG_OS:           The OS this module is being built for.
#                        Example: 'export XCHG_OS := eCos'
#


#
# Define the library name and location for the objects.
#

XCHG_LIB_BASE := lib_sip_m5tua

XCHG_LIB_DIR = $(CXC_CABLEX_DIR)/lib/$(CXC_OS_TYPE)
XCHG_OBJ_DIR = $(CXC_CABLEX_DIR)/obj/$(CXC_OS_TYPE)/sip_m5tua

#
# Define necessary variable used in the core makefile(s).
#
CCTK_ROOT      := $(CXC_CCTK_ROOT)
M5T_SIPUA_ROOT := $(CXC_M5T_SIPUA_ROOT)
EXPAT_ROOT     := $(CXC_EXPAT_ROOT)

XCHG_RECURSIVE_BUILD = 1


#
# The following variables are checked for emptyness but are not needed
# in the CxC build environment, so we just define them to some 'random'
# value.
#
M5T_SIPUA_BUILD_IMPORTED_INCLUDE := $(M5T_SIPUA_ROOT)
M5T_SIPUA_BUILD_CONFIG_INCLUDE := $(M5T_SIPUA_ROOT)


#
# Reference the actual library makefile which is kept in a different location.
#
include $(M5T_SIPUA_ROOT)/build/xchg/m5t_sipua.mk


#
# Setup source to build for the library.
#
XCHG_SRC_FILES := $(M5T_SIPUA_BUILD_SRC_FILES)


#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
vpath %.cpp $(M5T_SIPUA_BUILD_SRC_DIRS)
vpath %.c   $(M5T_SIPUA_BUILD_SRC_DIRS)


#
# Include common rules.
#
include $(XCHG_ROOT)/rules/xChgEnv.mk


#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE := $(M5T_SIPUA_INCLUDE)                                    \
                        $(CXC_APPS_DIR)/sip_cctk/cfginc                         \
                        $(CXC_APPS_DIR)/$(CXC_APP_NAME)/cfginc/mta              \
                        $(CXC_APPS_DIR)/cfginc/mta                              \
                        $(CXC_APPS_DIR)/cfginc/xchg_common                      \
                        $(CXC_MTA_DIR)/inc/cfg                                  \
                        $(CCTK_ROOT)/cctk/cfginc                                \
                        $(CCTK_ROOT)/cctk/src/cfg/extcfg                        \
                        $(M5T_SIPUA_ROOT)/M5TSce                                \
                        $(M5T_SIPUA_ROOT)/M5TSdp                                \
                        $(M5T_SIPUA_ROOT)/M5TTextMessageParser                  \
                        $(M5T_SIPUA_ROOT)/M5TSipUa                              \
                        $(M5T_SIPUA_ROOT)/M5TFramework                          \
                        $(M5T_SIPUA_ROOT)/M5TFramework/Custom/Bos/BosSingleton  \
                        $(M5T_SIPUA_ROOT)/M5TXcapClient                         \
                        $(M5T_SIPUA_ROOT)/ThirdParty/regex                      \
						$(BCM_FSBUILD_DIR)/public/include                       \
                        $(EXPAT_ROOT)/expat/lib


# Add xchg_common public includes.

include $(CXC_XCHG_COMMON_DIR)/xchg_common_env.mk
export XCHG_C_GLOBAL_INCLUDE += $(XCHG_COMPOSITE_PUBLIC_INCLUDE)
export XCHG_C_GLOBAL_DEFS    += $(XCHG_COMPOSITE_PUBLIC_DEFS)

#
# Add in application specific defines.
#
XCHG_C_GLOBAL_DEFS += $(M5T_SIPUA_DEFS)


#
# Add OS specific defines.
#
XCHG_C_GLOBAL_DEFS += BOS_OS_LINUXUSER

ifeq (${CXC_LINUX_ARCHITECTURE},MIPS)
XCHG_C_GLOBAL_DEFS += MXD_OS_BOS_LINUX_BE
else
ifeq (${CXC_LINUX_ARCHITECTURE},ARM)
XCHG_C_GLOBAL_DEFS += MXD_OS_BOS_LINUX_LE
endif
endif


#
# Disable warnings (third party code, no control over it).
#
# XCHG_SUPPRESS_WARNINGS := 1

XCHG_C_LOCAL_COMPILER_OPTS += -Wno-error -w

#
# Do not include symbol table.
#
# Note: this should be made conditional to including the debug information
#       for a particular build environment.
#
XCHG_ADD_SYMBOLS := 0


include $(XCHG_RULES_DIR)/xChgRules.mk


#
# Define 'clean' target.
#
.PHONY: clean
clean:
	@$(ECHO) 'Removing SIP M5T SIPUA library objects'
	$(RM) -f $(XCHG_OBJ_DIR)/*.*


