/**********************************************************************************
** Copyright © 2008 Broadcom
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/**
*
*  @file    bosCfgCustom.h
*
*  @brief   BOS configuration file
*
*  BOS custom application configuration file
*
****************************************************************************/

#ifndef BOSCFGCUSTOM_H
#define BOSCFGCUSTOM_H
#if defined(XCBUILD_OS_VXWORKS)
#define BOS_OS_VXWORKS        1
#elif defined(XCBUILD_OS_ECOS)
#define BOS_OS_ECOS           1
#else
#error "XCBUILD_OS_XXX not defined"
#endif

#define BOS_CFG_INIT          1
#define BOS_CFG_MUTEX         1
#define BOS_CFG_SLEEP         1
#define BOS_CFG_SEM           1
#define BOS_CFG_SOCKET        1
#define BOS_CFG_SOCKET_INTERFACE    1
#define BOS_CFG_MSGQ          1
#define BOS_CFG_EVENT         1
#define BOS_CFG_TIMER         1
#define BOS_CFG_IP_ADDRESS    1
#define BOS_CFG_IP_ADDR_V6_SUPPORT  0
#define BOS_CFG_TASK          1
#define BOS_CFG_CRITSECT      1
#define BOS_CFG_LOG           0
#define BOS_CFG_TIME          1

#define BOS_CFG_ERROR_NOASSERT   1

#if BOS_OS_ECOS
#define BOS_CFG_RESET_SUPPORT             1
#define BOS_CFG_RESET_GRANULARITY         500

#define BOS_CFG_CRITSECT_USE_GLOBAL       1

#if BOS_CFG_CRITSECT_USE_GLOBAL
#define BOS_CFG_GLOBAL_CRITSECT mtaCritSect
#endif

/* MTA's task classes */
#define MTA_TASK_RTP_DATA     BOS_TASK_CLASS_EXT1
#define MTA_TASK_DATA         BOS_TASK_CLASS_EXT2
#define MTA_TASK_MANAGEMENT   BOS_TASK_CLASS_EXT3
#define MTA_TASK_HARDWARE     BOS_TASK_CLASS_EXT4
#define MTA_TASK_BACKGROUND   BOS_TASK_CLASS_EXT5
#define MTA_TASK_ROOT         BOS_TASK_CLASS_EXT6
#define MTA_TASK_DEFAULT      BOS_TASK_CLASS_EXT7

/* Number of external task classes used */
#define BOS_CFG_TASK_NUM_EXT_CLASSES   7

/* Task priority for MTA root task */
#define BOS_CFG_ROOT_TASK_PRIORITY           MTA_TASK_ROOT

/* Task priority for MTA application task */
#define BOS_CFG_APP_TASK_PRIORITY            MTA_TASK_DEFAULT

/* Task priority definitions
 * NOTE: In CablexChange, we get the absolute priority values from the CM.
 *       Therefore, the abstract BOS priority types (high, med-high..etc.)
 *       will not be used.  To be safe, we'll need to set the BOS abstract
 *       priority types to invalid values.  When the application
 *       tries to create a task using these abstract BOS priority types, an
 *       error will be returned */
#define BOS_CFG_TASK_HIGH_VAL       0xFFFF     /* corresponds to BOS_TASK_HIGH */
#define BOS_CFG_TASK_MED_HIGH_VAL   0xFFFF     /* corresponds to BOS_TASK_MED_HIGH */
#define BOS_CFG_TASK_MED_VAL        0xFFFF     /* corresponds to BOS_TASK_MED */
#define BOS_CFG_TASK_MED_LOW_VAL    0xFFFF     /* corresponds to BOS_TASK_MED_LOW */
#define BOS_CFG_TASK_LOW_VAL        0xFFFF     /* corresponds to BOS_TASK_LOW */

/* General task module definitions */
#define  BOS_CFG_TASK_MAX_TASKS     30

/* maximum number of messages per message queue */
#define BOS_CFG_MSGQ_MAX_MSG_PER_MSGQ 100

/* maximum number of message queues */
#define BOS_CFG_MSGQ_MAX_MSGQS        16

#endif

#endif   /* BOSCFGCUSTOM_H */
