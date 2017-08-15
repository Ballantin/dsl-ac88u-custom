##################################################################################
#
# Copyright © 2008 - 2010 Broadcom Corporation
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
#                  This makefile is used to build the XML library module.
#
##################################################################################

#
# NOTE: The following variable MUST be defined prior to calling this makefile
#       in order for it to work properly:
#
#     EXPAT_LIBDIR: The absolute path to the directory where the final library
#                   is located.
#
#     EXPAT_ROOT:   The absolute path to the directory containing the 'expat'
#                   module
#                   Example: 'export EXPAT_ROOT := lib_xml_expat'
#
#     XCHG_OS:      The OS this module is being built for.
#                   Example: 'export XCHG_OS := eCos'
#


#
# Define the library name and location for the objects.
#
XCHG_LIB_BASE := libExpat1958
XCHG_OBJ_DIR  := $(XCHG_OBJ_DIR)/expat
XCHG_LIB_DIR  := $(EXPAT_LIBDIR)

XCHG_RECURSIVE_BUILD = 1


#
# Reference the actual sources which are kept in a different makefile for ease of
# usage and readability.
#
include $(EXPAT_ROOT)/expat_srcs.mk


#
# Setup source to build for the library.
#
XCHG_SRC_FILES := $(EXPAT_ALL_SRCS)


#
# We use vpath so that the object files don't wind up in nested subdirectories.
# These must be absolute paths in order to support non-recursive makefiles.
#
vpath %.cpp $(EXPAT_ALL_DIRS)
vpath %.cxx $(EXPAT_ALL_DIRS)
vpath %.c   $(EXPAT_ALL_DIRS)


#
# Include common rules.
#
include $(XCHG_ROOT)/rules/xChgEnv.mk


#
# Add in private directories that are only needed when compiling this library.
# These must be absolute paths in order to support non-recursive makefiles.
#
XCHG_C_LOCAL_INCLUDE := $(EXPAT_ROOT)/expat/xmlwf               \
                        $(EXPAT_ROOT)/expat/vms                 \
                        $(EXPAT_ROOT)/expat/lib

#
# Add xchg_common public includes.
#
include $(CXC_XCHG_COMMON_DIR)/xchg_common_env.mk

export XCHG_C_GLOBAL_INCLUDE += $(XCHG_COMPOSITE_PUBLIC_INCLUDE)
export XCHG_C_GLOBAL_DEFS    += $(XCHG_COMPOSITE_PUBLIC_DEFS)

#
# Add in application specific defines.
#
XCHG_C_GLOBAL_DEFS += HAVE_MEMMOVE


#
# Add OS specific defines.
#
ifeq ($(XCHG_OS),eCos)
   XCHG_C_GLOBAL_DEFS += BOS_OS_ECOS
endif

ifeq ($(XCHG_OS),LinuxUser)
   XCHG_C_GLOBAL_DEFS += BOS_OS_LINUXUSER
endif

#
# Disable warnings (third party code, no control over it).
#
ifeq ($(XCHG_OS),LinuxUser)
#  Dont support this in BOS for linux user   
else
   XCHG_SUPPRESS_WARNINGS := 1
endif

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
   @$(ECHO) 'Removing Expat library objects'
   $(RM) -f $(XCHG_OBJ_DIR)/*.*


