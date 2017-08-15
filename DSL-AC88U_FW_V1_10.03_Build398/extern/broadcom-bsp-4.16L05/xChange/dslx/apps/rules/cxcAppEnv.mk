###################################################################################
#
#    Copyright 2000-2002  Broadcom Corporation
#    All Rights Reserved
#    No portions of this material may be reproduced in any form without the
#    written permission of:
#             Broadcom Corporation
#             16215 Alton Parkway
#             P.O. Box 57013
#             Irvine, California 92619-7013
#    All information contained in this document is Broadcom Corporation
#    company private, proprietary, and trade secret.
#
###################################################################################
#
#    Filename: cxcAppEnv.mk
#
###################################################################################
#
#    CablexChange application "environment" makefile 
#        - This file should only define variables and not have any rules in it.
#
#
#    This makefile requires the following to be set as environment variables,
#    or to be exported as part of the containing makefile
# 
#        CXC_PROJ_DIR         - Full path of root project directory
#        CXC_OS_ROOT_DIR      - Full path of operating system directory (Tornado, isimip)
#        CXC_APP_NAME         - Build configuation name
#        CXC_OS_TYPE          - Operating system name
#
###################################################################################


#----------------------------------------------------------------------------
# Environment/make variable check.
#     - Verify that required environment variables are set
#     - Convert any back-slashes to forward-slashes
#

ifeq ($(CXC_PROJ_DIR),)
$(error "CXC_PROJ_DIR variable hasn't been set")
else
export CXC_PROJ_DIR := $(subst \,/,$(CXC_PROJ_DIR))
endif

ifeq ($(CXC_APP_NAME),)
$(error "CXC_APP_NAME variable hasn't been set")
endif

ifeq ($(CXC_OS_TYPE),)
$(error "CXC_OS_TYPE variable hasn't been set")
endif

ifeq ($(CXC_OS_TYPE),VxWorks)
ifeq ($(CXC_OS_ROOT_DIR),)
$(error "CXC_OS_ROOT_DIR variable hasn't been set")
else
export CXC_OS_ROOT_DIR := $(subst \,/,$(CXC_OS_ROOT_DIR))
endif
endif

ifeq ($(CXC_LDX_HAUSWARE_HOST_LIB_DIR),)
$(error "CXC_LDX_HAUSWARE_HOST_LIB_DIR variable hasn't been set")
endif



#----------------------------------------------------------------------------
# Setup commonly used directory paths.
#

ifeq ($(CXC_CABLEX_DIR),)
export CXC_CABLEX_DIR := $(CXC_PROJ_DIR)/dslx
else
export CXC_CABLEX_DIR := $(subst \,/,$(CXC_CABLEX_DIR))
endif

ifeq ($(CXC_RULES_DIR),)
export CXC_RULES_DIR := $(CXC_CABLEX_DIR)/apps/rules
else
export CXC_RULES_DIR := $(subst \,/,$(CXC_RULES_DIR))
endif

ifeq ($(CXC_APPS_DIR),)
export CXC_APPS_DIR := $(CXC_CABLEX_DIR)/apps
else
export CXC_APPS_DIR := $(subst \,/,$(CXC_APPS_DIR))
endif

ifeq ($(CXC_MTA_DIR),)
export CXC_MTA_DIR := $(CXC_CABLEX_DIR)/src/mta
else
export CXC_MTA_DIR := $(subst \,/,$(CXC_MTA_DIR))
endif

ifeq ($(CXC_SMAPI_DIR),)
export CXC_SMAPI_DIR := $(CXC_PROJ_DIR)/dslx_common/sec_SRTP
else
export CXC_SMAPI_DIR := $(subst \,/,$(CXC_SMAPI_DIR))
endif

ifeq ($(CXC_XCHG_COMMON_DIR),)
export CXC_XCHG_COMMON_DIR := $(CXC_PROJ_DIR)/dslx_common/xchg_common
else
export CXC_XCHG_COMMON_DIR := $(subst \,/,$(CXC_XCHG_COMMON_DIR))
endif

ifeq ($(CXC_VRG_DIR),)
export CXC_VRG_DIR := $(CXC_PROJ_DIR)/dslx_common/voice_res_gw
else
export CXC_VRG_DIR := $(subst \,/,$(CXC_VRG_DIR))
endif

ifeq ($(CXC_XDRV_DIR),)
export CXC_XDRV_DIR := $(CXC_PROJ_DIR)/dslx_common/xchg_drivers
else
export CXC_XDRV_DIR := $(subst \,/,$(CXC_XDRV_DIR))
endif

ifeq ($(CXC_LDX_HAUSWARE_INC_DIR),)
export CXC_LDX_HAUSWARE_INC_DIR := $(CXC_PROJ_DIR)/dslx_common/ldx_hausware/hausware/inc
else
export CXC_LDX_HAUSWARE_INC_DIR := $(subst \,/,$(CXC_LDX_HAUSWARE_INC_DIR))
endif

ifeq ($(CXC_LDX_HAUSWARE_SYSINC_DIR),)
export CXC_LDX_HAUSWARE_SYSINC_DIR := $(CXC_PROJ_DIR)/dslx_common/ldx_hausware/hausware/sysinc
else
export CXC_LDX_HAUSWARE_SYSINC_DIR := $(subst \,/,$(CXC_LDX_HAUSWARE_SYSINC_DIR))
endif

ifeq ($(CXC_LDX_APPS_DIR),)
export CXC_LDX_APPS_DIR := $(CXC_PROJ_DIR)/dslx_common/ldx_apps/dsl/apps
else
export CXC_LDX_APPS_DIR := $(subst \,/,$(CXC_LDX_APPS_DIR))
endif

ifeq ($(CXC_LIB_SUPPORT_DIR),)
export CXC_LIB_SUPPORT_DIR := $(CXC_PROJ_DIR)/dslx_common
else
export CXC_LIB_SUPPORT_DIR := $(subst \,/,$(CXC_LIB_SUPPORT_DIR))
endif

ifeq ($(CXC_RESOLVER_DIR),)
export CXC_RESOLVER_DIR := $(CXC_LIB_SUPPORT_DIR)/resolv
else
export CXC_RESOLVER_DIR := $(subst \,/,$(CXC_RESOLVER_DIR))
endif

ifeq ($(CXC_SIP_MX_DIR),)
export CXC_SIP_MX_DIR := $(subst \,/,$(CXC_SIP_MX_DIR))
endif

ifeq ($(CXC_SIP_CC_DIR),)
export CXC_SIP_CC_DIR := $(subst \,/,$(CXC_SIP_CC_DIR))
endif

ifeq ($(CXC_SIP_CMGR_DIR),)
export CXC_SIP_CMGR_DIR := $(subst \,/,$(CXC_SIP_CMGR_DIR))
endif

ifeq ($(CXC_CELL_ROOT),)
export CXC_CELL_ROOT := $(CXC_LIB_SUPPORT_DIR)/prot_cell
else
export CXC_CELL_ROOT := $(subst \,/,$(CXC_CELL_ROOT))
endif

ifeq ($(CXC_CCTK_ROOT),)
export CXC_CCTK_ROOT := $(CXC_LIB_SUPPORT_DIR)/prot_cctk
else
export CXC_CCTK_ROOT := $(subst \,/,$(CXC_CCTK_ROOT))
endif

ifeq ($(CXC_SIP_CMGR_CCTK_DIR),)
export CXC_SIP_CMGR_CCTK_DIR := $(CXC_CCTK_ROOT)/cmgr
else
export CXC_SIP_CMGR_CCTK_DIR := $(subst \,/,$(CXC_SIP_CMGR_CCTK_DIR))
endif

ifeq ($(CXC_SIP_CCTK_DIR),)
export CXC_SIP_CCTK_DIR := $(CXC_CCTK_ROOT)/cctk
else
export CXC_SIP_CCTK_DIR := $(subst \,/,$(CXC_SIP_CCTK_DIR))
endif

ifeq ($(CXC_M5T_SIPUA_ROOT),)
export CXC_M5T_SIPUA_ROOT := $(CXC_LIB_SUPPORT_DIR)/prot_m5t_sipua
else
export CXC_M5T_SIPUA_ROOT := $(subst \,/,$(CXC_M5T_SIPUA_ROOT))
endif

ifeq ($(CXC_CELL_CMGR_DIR),)
export CXC_CELL_CMGR_DIR := $(CXC_CELL_ROOT)/cmgr
else
export CXC_CELL_CMGR_DIR := $(subst \,/,$(CXC_CELL_CMGR_DIR))
endif

ifeq ($(CXC_EXPAT_ROOT),)
export CXC_EXPAT_ROOT := $(CXC_LIB_SUPPORT_DIR)/lib_xml_expat
else
export CXC_EXPAT_ROOT := $(subst \,/,$(CXC_EXPAT_ROOT))
endif

ifeq ($(CXC_NAT_ROOT),)
export CXC_NAT_ROOT := $(CXC_LIB_SUPPORT_DIR)/prot_NATtrvrsl
else
export CXC_NAT_ROOT := $(subst \,/,$(CXC_NAT_ROOT))
endif

ifeq ($(CXC_CM_ROOT_DIR),)
export CXC_CM_ROOT_DIR := $(CXC_CABLEX_DIR)/src/cm_v3
else
export CXC_CM_ROOT_DIR := $(subst \,/,$(CXC_CM_ROOT_DIR))
endif

ifeq ($(CXC_STUB_DIR),)
export CXC_STUB_DIR := $(CXC_CABLEX_DIR)/src/stubs
else
export CXC_STUB_DIR := $(subst \,/,$(CXC_STUB_DIR))
endif

ifeq ($(CXC_OS_TYPE),eCos)
ifeq ($(CXC_OS_ROOT_DIR),)
export CXC_OS_ROOT_DIR := $(CXC_PROJ_DIR)/zOEMtools_eCos/eCos20
else
export CXC_OS_ROOT_DIR := $(subst \,/,$(CXC_OS_ROOT_DIR))
endif

ifeq ($(CXC_GNU_TOOLCHAIN_CYGWIN_DIR),)
export CXC_GNU_TOOLCHAIN_CYGWIN_DIR := $(CXC_PROJ_DIR)/zOEMtools_cygwin/cygwin-1.5.7
else
export CXC_GNU_TOOLCHAIN_CYGWIN_DIR := $(subst \,/,$(CXC_GNU_TOOLCHAIN_CYGWIN_DIR))
endif
endif

ifeq ($(CXC_OS_TYPE),Win32)
ifeq ($(CXC_OS_ROOT_DIR),)
export CXC_OS_ROOT_DIR := $(CXC_PROJ_DIR)/zOEMtools_vc
else
export CXC_OS_ROOT_DIR := $(subst \,/,$(CXC_OS_ROOT_DIR))
endif
endif

ifeq ($(CXC_EPILOGUE_DIR),)
export CXC_EPILOGUE_DIR := $(subst \,/,$(CXC_OS_ROOT_DIR))
else
export CXC_EPILOGUE_DIR := $(subst \,/,$(CXC_EPILOGUE_DIR))
endif
