/****************************************************************************
*
* Copyright © 2000-2012 Broadcom
*
* This program is the proprietary software of Broadcom and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*****************************************************************************
*
*    Filename: itpcSharedCmds.h
*
****************************************************************************
*    Description:
*
*     Command-id's and associated data structures that are shared between the
*     thread processors in a CMT (concurrent multi-threaded) processor. The
*     commands/data are sent via the inter-thread processor communication (ITPC)
*     module.
*
*     This header file does not need to be logically grouped with the ITPC
*     module. The ITPC module is simply a transport layer. It does not
*     understand the commands types, or contents of the data structures that
*     are sent between the thread-processors.  This header file defines the
*     application specific commands and data structures that are transmitted.
*     The ITPC module should not have any dependencies on this header file.
*     Different applications can define similar header files to send
*     alternate commands/data between thread-processors.
*
****************************************************************************/

#ifndef ITPC_SHARED_CMDS_H
#define ITPC_SHARED_CMDS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvItpc.h>
#include <mipsContext.h>


/* ---- Constants and Types ---------------------------------------------- */

/* --------------------------------------------------------------------------
** List of commands that can be sent from one thread-processor to another.
*/
enum
{
   /* Commands that should be sent to the "DSP" thread processor. */
   ITPC_GET_CONTEXT_REGS_CMD_ID           = XDRV_ITCP_START_CMD_ID,
   ITPC_GET_COUNTERS_CMD_ID,
   ITPC_FORCE_ASSERT_CMD_ID,
   ITPC_GET_ASSERT_CONTEXT_CMD_ID,
   ITPC_GET_EXCEPTION_CONTEXT_CMD_ID,
   ITPC_GET_STACK_STATS_CMD_ID,
   ITPC_STANDY_CMD_ID,
   ITPC_RESET_CMD_ID,
   ITPC_POWERSAVE_CMD_ID,
   ITPC_POWERSAVE_VRX_SHUTDOWN_CMD_ID,
   ITPC_GET_LOAD_BALANCE_CONFIG_CMD_ID,
   ITPC_SET_LOAD_BALANCE_CONFIG_CMD_ID,
   ITPC_SET_GENERIC_PARM_CONFIG_CMD_ID,
   ITPC_SET_START_DECT_BUFFER_PROCESSING_CMD_ID,
   ITPC_SET_HALCAP_INFO_CMD_ID,
   ITPC_GET_DSP_VERSION_CMD_ID,
   
   /* Commands that should be sent by the "DSP" thread processor. */
   ITPC_ASSERT_NOTIFY_CMD_ID,
   ITPC_EXCEPTION_NOTIFY_CMD_ID,
   ITPC_HAUSWARE_OFFLOAD_CMD_ID,
   ITPC_LOAD_BALANCE_TRIGGER_CMD_ID,
   ITPC_HAUSWARE_FRAMEASYNC_CMD_ID,
   ITPC_HAUSWARE_SYNC_CMD_ID,

   /* This must be last. */
   ITPC_NUM_CMDS,
};



/* --------------------------------------------------------------------------
** Data structures that represent the command and response data structures
** associated with the command-types listed above.
*/

/*
** Counters.
*/
typedef struct ITPC_SHARED_COUNTERS
{
   /*
   ** Debug "I-am-alive" counters.
   */
   unsigned int   backgroundCounter;
   unsigned int   interruptCounter;

   /* Exception overflow count. */
   unsigned int   overflowExceptionCount;
} ITPC_SHARED_COUNTERS;


/*
** This is the state saved by the low level exception handler when an exception
** occurs.
*/
typedef MIPS_CONTEXT_REGS  ITPC_SHARED_CONTEXT_REGS;


/*
** Debug stack usage statistics.
*/
typedef struct ITPC_SHARED_STACK_STATS
{
   unsigned int   stackSizeBytes;
   unsigned int   stackUsedBytes;
} ITPC_SHARED_STACK_STATS;


/*
** Dynamic load balancing configuration.
*/
typedef struct ITPC_SHARED_LOAD_BALANCE_CONFIG
{
   XDRV_BOOL   isEnabled;
   XDRV_BOOL   isStaticModeEnabled;

} ITPC_SHARED_LOAD_BALANCE_CONFIG;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* ITPC_SHARED_CMDS_H  */
