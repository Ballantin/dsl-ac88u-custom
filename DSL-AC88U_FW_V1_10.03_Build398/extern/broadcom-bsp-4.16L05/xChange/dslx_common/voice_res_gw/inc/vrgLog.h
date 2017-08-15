/****************************************************************************
*
* <:copyright-BRCM:2014:DUAL/GPL:standard
* 
*    Copyright (c) 2014 Broadcom
*    All Rights Reserved
* 
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed
* to you under the terms of the GNU General Public License version 2
* (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
* with the following added to such license:
* 
*    As a special exception, the copyright holders of this software give
*    you permission to link this software with independent modules, and
*    to copy and distribute the resulting executable under terms of your
*    choice, provided that you also meet, for each linked independent
*    module, the terms and conditions of the license of that module.
*    An independent module is a module which is not derived from this
*    software.  The special exception does not apply to any modifications
*    of the software.
* 
* Not withstanding the above, under no circumstances may you combine
* this software in any way with any other Broadcom software provided
* under a license other than the GPL, without Broadcom's express prior
* written consent.
* 
* :>
*
*****************************************************************************

*
*    Filename: vrgLog.h
*
****************************************************************************
*    Description:
*
*        This file provides a set of functions for logging messages to the
*        console.
*
****************************************************************************/

#ifndef VRG_LOG_H
#define VRG_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ------------ Includes -------------------------------------------------- */

#include <stdio.h>
#include <vrgLogCfgCustom.h>
#include <bosTime.h>

/* ------------ Constants ------------------------------------------------- */


/*
** Logging module handle names
*/
#ifndef VRG_LOG_MOD_BOARDHAL
#   define VRG_LOG_MOD_BOARDHAL "BHAL"
#endif
#ifndef VRG_LOG_MOD_CAS
#   define VRG_LOG_MOD_CAS      "CAS"
#endif
#ifndef VRG_LOG_MOD_EPT
#   define VRG_LOG_MOD_EPT      "EPT"
#endif
#ifndef VRG_LOG_MOD_HBT
#   define VRG_LOG_MOD_HBT      "HBT"
#endif
#ifndef VRG_LOG_MOD_HDSP
#   define VRG_LOG_MOD_HDSP     "HDSP"
#endif
#ifndef VRG_LOG_MOD_TPD
#   define VRG_LOG_MOD_TPD      "TPD"
#endif
#ifndef VRG_LOG_MOD_PROV
#   define VRG_LOG_MOD_PROV     "PROV"
#endif
#ifndef VRG_LOG_MOD_PROBE
#   define VRG_LOG_MOD_PROBE    "PROBE"
#endif
#ifndef VRG_LOG_MOD_DECT
#   define VRG_LOG_MOD_DECT     "DECT"
#endif
#ifndef VRG_LOG_MOD_LHAPI
#   define VRG_LOG_MOD_LHAPI    "LHAPI"
#endif


/* Logging module action (registration, deregistration, playing with severities
** support and fields within the log).
*/
#ifndef VRG_LOG_REGISTER
#   define VRG_LOG_REGISTER(x)
#endif
#ifndef VRG_LOG_DEREGISTER
#   define VRG_LOG_DEREGISTER(x)
#endif
#ifndef VRG_LOG_GET_SEV
#   define VRG_LOG_GET_SEV(x)
#endif
#ifndef VRG_LOG_SET_SEV
#   define VRG_LOG_SET_SEV(x)
#endif
#ifndef VRG_LOG_GET_FLD
#   define VRG_LOG_GET_FLD(x)
#endif
#ifndef VRG_LOG_SET_FLD
#   define VRG_LOG_SET_FLD(x)
#endif


/* 
** VRG_LOG_INTERNAL_PRINTF is for mapping to printf only.
** It must NOT be used directly.  
** Always use the logging module: VRG_LOG_CRIT, VRG_LOG_WARN, VRG_LOG_INFO, VRG_LOG_DBG, VRG_LOG_FORCED
*/

#if !defined(VRG_LOG_CRIT) || !defined(VRG_LOG_WARN) || !defined(VRG_LOG_NOTICE) || !defined(VRG_LOG_INFO)|| !defined(VRG_LOG_DBG) || !defined(VRG_LOG_FORCED) || !defined(VRG_LOG_ERROR)
#   define VRG_LOG_INTERNAL_PRINTF(x,y,...)     printf(x, ":" y "", ##__VA_ARGS__)

#   define VRG_LOG_INTERNAL_PRINTF_CRIT(x,y,...)      \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   printf( x, "(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}

#   define VRG_LOG_INTERNAL_PRINTF_WARN(x,y,...)      \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   BCM_LOG_ERROR( x, "(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}

#   define VRG_LOG_INTERNAL_PRINTF_NOTICE(x,y,...)    \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   BCM_LOG_NOTICE( x ,"(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}

#   define VRG_LOG_INTERNAL_PRINTF_INFO(x,y,...)      \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   BCM_LOG_INFO( x, "(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}

#   define VRG_LOG_INTERNAL_PRINTF_DEBUG(x,y,...)     \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   BCM_LOG_DEBUG( x, "(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}

#   define VRG_LOG_INTERNAL_PRINTF_ERROR(x,y,...)     \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   BCM_LOG_ERROR( x, "(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}

#endif

/* Logging module logging available.
*/
#ifndef VRG_LOG_CRIT
#   define VRG_LOG_CRIT(x)          VRG_LOG_INTERNAL_PRINTF_ERROR x
#endif

#ifndef VRG_LOG_WARN
#   define VRG_LOG_WARN(x)          VRG_LOG_INTERNAL_PRINTF_ERROR x
#endif

#ifndef VRG_LOG_NOTICE
#   define VRG_LOG_NOTICE(x)        VRG_LOG_INTERNAL_PRINTF_NOTICE x
#endif

#ifndef VRG_LOG_INFO
#   define VRG_LOG_INFO(x)          VRG_LOG_INTERNAL_PRINTF_INFO x
#endif

#ifndef VRG_LOG_DBG
#   define VRG_LOG_DBG(x)           VRG_LOG_INTERNAL_PRINTF_DEBUG x
#endif

#ifndef VRG_LOG_FORCED
#   define VRG_LOG_FORCED(x)        VRG_LOG_INTERNAL_PRINTF_ERROR x
#endif

#ifndef VRG_LOG
#define VRG_LOG(x)                  printf x
#endif

#ifndef VRG_LOG_ERROR
#define VRG_LOG_ERROR(x)            VRG_LOG_INTERNAL_PRINTF_ERROR x
#endif

#ifdef __cplusplus
    }
#endif

#endif  /* VRG_LOG_H  */
