##################################################################################
#
# Copyright (c) 2008-2012 Broadcom Corporation
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
#                  This makefile is used to build the CCTK module.
#
##################################################################################


#
# List all the directories in which CCTK modules are implemented.
#
CCTK_ALL_DIRS			:=	src				\
								src/ce			\
								src/ce/cdr		\
								src/ce/dlg		\
								src/ce/fsm		\
								src/ce/loc		\
								src/ce/res		\
								src/ce/uap		\
								src/cfg			\
								src/main			\
								src/mse			\
								src/ns/br		\
								src/ns/dlm		\
								src/ns/nat		\
								src/ns/sc		\
								src/ns/ice		\
								src/util			\
								src/pm			\
								cfgsrc


#
# List all the sources is usage by CCTK.
#
# Note that is applicable, modules can be selected in/out depending on build
# configuration option.
#

CCTK_SRC				:=

CCTK_CE					:=	ceMain.cpp

CCTK_CE_CDR			:=	ceCdr.cpp			\
								ceCdrMgr.cpp

CCTK_CE_DLG			:=	ceDlg.cpp

CCTK_CE_FSM			:=	ceFsmBase.cpp		\
								ceFsmCall.cpp		\
								ceFsmCallIn.cpp	\
								ceFsmCallOut.cpp	\
								ceFsmCallProxyIn.cpp	\
								ceFsmCallProxyOut.cpp	\
								ceFsmHold.cpp		\
								ceFsmPub.cpp		\
								ceFsmReg.cpp		\
								ceFsmSub.cpp		\
								ceFsmXfer.cpp

CCTK_CE_LOC			:=	ceLoc.cpp

CCTK_CE_RES			:=	ceResBase.cpp		\
								ceResCall.cpp		\
								ceResNot.cpp		\
								ceResSub.cpp		\
								ceResUser.cpp

CCTK_CE_UAP			:=	ceUap.cpp

CCTK_CFG				:=	cfgConvert.cpp		\
								cfgEngine.cpp		\
								cfgMain.cpp

CCTK_CFGSRC			:=	cctkExtCfg.cpp

CCTK_MAIN				:=	cctkApi.cpp			\
								cctkFramework.cpp

CCTK_MSE				:=	mseMain.cpp

CCTK_NS					:=

CCTK_NS_BR				:=	nsBrApi.cpp			\
								nsBrCore.cpp

CCTK_NS_DLM			:=	nsDlmCore.cpp

CCTK_NS_NAT			:=	nsNatApi.cpp		\
								nsNatKa.cpp			\
								nsNatStun.cpp

CCTK_NS_SC				:=	nsScCore.cpp

CCTK_NS_ICE				:=	nsIceCore.cpp

CCTK_UTIL				:=	utilData.cpp		\
								utilMain.cpp		\
								utilMem.cpp			\
								utilStr.cpp			\
								utilTel.cpp

CCTK_PM					:=	pmObserver.cpp


#
# This is the information passed to the module interested in building CCTK.
#
CCTK_ALL_SRCS			:=	$(CCTK_SRC)			\
								$(CCTK_CE)			\
								$(CCTK_CE_CDR)		\
								$(CCTK_CE_DLG)		\
								$(CCTK_CE_FSM)		\
								$(CCTK_CE_LOC)		\
								$(CCTK_CE_RES)		\
								$(CCTK_CE_UAP)		\
								$(CCTK_CFG)			\
								$(CCTK_CFGSRC)		\
								$(CCTK_MAIN)		\
								$(CCTK_MSE)			\
								$(CCTK_NS)			\
								$(CCTK_NS_BR)		\
								$(CCTK_NS_DLM)		\
								$(CCTK_NS_NAT)		\
								$(CCTK_NS_SC)		\
								$(CCTK_NS_ICE)		\
								$(CCTK_UTIL)		\
								$(CCTK_PM)

