/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: xdrvLogCfgCustom.h
*
****************************************************************************
*    Description:
*
*     Overrides default build options in xdrvLog.h
*
****************************************************************************/

/* XDRV_LOG_CFG_CUSTOM_H */

#ifndef XDRV_LOG_CFG_CUSTOM_H
#define XDRV_LOG_CFG_CUSTOM_H

/* ------------ Includes -------------------------------------------------- */

#include <linux/bcm_log.h>
#include <stdio.h>
#include <bosTime.h>

/* ------------ Constants ------------------------------------------------- */

#define XDRV_LOG_MOD_XDRV	BCM_LOG_ID_VOICE_XDRV
#define XDRV_LOG_MOD_SLIC	BCM_LOG_ID_VOICE_XDRV_SLIC
#define XDRV_LOG_MOD_PCM	BCM_LOG_ID_VOICE_XDRV
#define XDRV_LOG_MOD_DECT	BCM_LOG_ID_VOICE_XDRV

#   define XDRV_LOG_INTERNAL_PRINTF(x,y,...)     printf(x ":" y "", ##__VA_ARGS__)

#   define XDRV_LOG_INTERNAL_PRINTF_CRIT(x,y,...)     \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   printf( x, "(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}

#   define XDRV_LOG_INTERNAL_PRINTF_WARN(x,y,...)     \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   BCM_LOG_ERROR( x, "(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}

#   define XDRV_LOG_INTERNAL_PRINTF_NOTICE(x,y,...)   \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   BCM_LOG_NOTICE( x ,"(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}

#   define XDRV_LOG_INTERNAL_PRINTF_INFO(x,y,...)     \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   BCM_LOG_INFO( x, "(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}

#   define XDRV_LOG_INTERNAL_PRINTF_DEBUG(x,y,...)    \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   BCM_LOG_DEBUG( x, "(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}

#   define XDRV_LOG_INTERNAL_PRINTF_ERROR(x,y,...)    \
{                                                     \
   BOS_TIME_SEC timeSec;                              \
   BOS_TIME_USEC timeMicrosec;                        \
   bosTimeGetTimeOfDay(&timeSec, &timeMicrosec);      \
   BCM_LOG_ERROR( x, "(ts: %-2us %-3ums)" ":" y "", timeSec % 60, timeMicrosec/1000, ##__VA_ARGS__); \
}


/* Logging module logging available.
*/
#define XDRV_LOG_CRIT(x)              XDRV_LOG_INTERNAL_PRINTF_ERROR x

#define XDRV_LOG_WARN(x)              XDRV_LOG_INTERNAL_PRINTF_ERROR x

#define XDRV_LOG_NOTICE(x)            XDRV_LOG_INTERNAL_PRINTF_NOTICE x

#define XDRV_LOG_INFO(x)              XDRV_LOG_INTERNAL_PRINTF_INFO x

#define XDRV_LOG_DBG(x)               XDRV_LOG_INTERNAL_PRINTF_DEBUG x

#define XDRV_LOG_FORCED(x)            XDRV_LOG_INTERNAL_PRINTF_ERROR x

#define XDRV_LOG(x)                   printf x

#define XDRV_LOG_ERROR(x)             XDRV_LOG_INTERNAL_PRINTF_ERROR x


#endif   /* XDRV_LOG_CFG_CUSTOM_H */

