/****************************************************************************
*
* Copyright © 2000-2008 Broadcom
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
*    Filename: xdrvLog.h
*
****************************************************************************
*    Description:
*
*       This file provides a set of functions for logging messages to the
*       console.
*
****************************************************************************/

#ifndef XDRV_LOG_H
#define XDRV_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------ Includes -------------------------------------------------- */

#include <stdio.h>
#include <xdrvLogCfgCustom.h>


/* ------------ Constants ------------------------------------------------- */


/*
** Logging module handle names
*/
#ifndef XDRV_LOG_MOD_XDRV
#   define XDRV_LOG_MOD_XDRV "XDRV"



#endif


/* Logging module action (registration, deregistration, playing with severities
** support and fields within the log).
*/
#ifndef XDRV_LOG_REGISTER
#   define XDRV_LOG_REGISTER(x)
#endif
#ifndef XDRV_LOG_DEREGISTER
#   define XDRV_LOG_DEREGISTER(x)
#endif
#ifndef XDRV_LOG_GET_SEV
#   define XDRV_LOG_GET_SEV(x)
#endif
#ifndef XDRV_LOG_SET_SEV
#   define XDRV_LOG_SET_SEV(x)
#endif
#ifndef XDRV_LOG_GET_FLD
#   define XDRV_LOG_GET_FLD(x)
#endif
#ifndef XDRV_LOG_SET_FLD
#   define XDRV_LOG_SET_FLD(x)
#endif


/* 
** XDRV_LOG_INTERNAL_PRINTF is for mapping to printf only.
** It must NOT be used directly.  
** Always use the logging module: XDRV_LOG_CRIT, XDRV_LOG_WARN, XDRV_LOG_INFO, XDRV_LOG_DBG, XDRV_LOG_FORCED
*/
#if !defined(XDRV_LOG_CRIT) || !defined(XDRV_LOG_WARN) || !defined(XDRV_LOG_INFO) || !defined(XDRV_LOG_DBG) || !defined(XDRV_LOG_FORCED)
#   define XDRV_LOG_INTERNAL_PRINTF(x,y,...)     printf(x ":" y "\n", ##__VA_ARGS__)

















































#endif

/* Logging module logging available.
*/
#ifndef XDRV_LOG_CRIT
#   define XDRV_LOG_CRIT(x)           XDRV_LOG_INTERNAL_PRINTF x
#endif

#ifndef XDRV_LOG_WARN
#   define XDRV_LOG_WARN(x)           XDRV_LOG_INTERNAL_PRINTF x
#endif

#ifndef XDRV_LOG_INFO
#   define XDRV_LOG_INFO(x)           XDRV_LOG_INTERNAL_PRINTF x




#endif

#ifndef XDRV_LOG_DBG
#   define XDRV_LOG_DBG(x)            XDRV_LOG_INTERNAL_PRINTF x
#endif

#ifndef XDRV_LOG_FORCED
#   define XDRV_LOG_FORCED(x)         XDRV_LOG_INTERNAL_PRINTF x
#endif

#ifndef XDRV_LOG
#define XDRV_LOG(x)           printf x
#endif

#ifndef XDRV_LOG_ERROR
#define XDRV_LOG_ERROR(x)     printf x
#endif

#ifndef XDRV_LOG_INFO
#define XDRV_LOG_INFO(x)      
#endif



#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_LOG_H  */

