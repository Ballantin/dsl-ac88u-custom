//==SDOC========================================================================
//==============================================================================
//
//     Copyright(c) 2003-2005 M5T Centre d'Excellence en Telecom Inc. ("M5T")
//
//  NOTICE:
//   This document contains information that is confidential and proprietary
//   to M5T.
//
//   M5T reserves all rights to this document as well as to the Intellectual
//   Property of the document and the technology and know-how that it includes
//   and represents.
//
//   This publication cannot be reproduced, neither in whole nor in part, in
//   any form whatsoever without written prior approval by M5T.
//
//   M5T reserves the right to revise this publication and make changes at any
//   time and without the obligation to notify any person and/or entity of such
//   revisions and/or changes.
//
//==============================================================================
//==EDOC========================================================================
#ifndef MXG_TIME_H
//M5T_INTERNAL_USE_BEGIN
#define MXG_TIME_H

//===========================================================================
//====  INCLUDES + FORWARD DECLARATIONS  ====================================
//===========================================================================

//-- M5T Global definitions
//---------------------------
#ifndef MXG_MXCONFIG_H
#include "Config/MxConfig.h"
#endif

#include <time.h>

// BRCM_CUSTOM - [begin] - Add ECOS support.
#if defined(MXD_OS_BOS_ECOS)
#   include <bosCfg.h>
#   include <bosTime.h>
#endif
// BRCM_CUSTOM - [end] - Add ECOS support.

MX_NAMESPACE_START(MXD_GNS)

//===========================================================================
//====  INLINE FUNCTIONS  ===================================================
//===========================================================================
inline time_t MxTime(OUT time_t* pTime)
{
#if defined(MXD_OS_WINDOWS_CE)
    if(pTime != NULL)
    {
        *pTime = 0;
    }
    return 0;
#else
    return time(pTime);
#endif
}

inline time_t MxMkTime(IN tm* pTime)
{
#if defined(MXD_OS_WINDOWS_CE)
    return 0;
// BRCM_CUSTOM - [begin] - Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
    time_t calendarTime = (time_t)0;
    bosTimeMkTime(pTime, &calendarTime);
    return calendarTime;
// BRCM_CUSTOM - [end] - Add ECOS support.
#else
    return mktime(pTime);
#endif
}

inline size_t MxStrfTime(OUT char* szBuffer,
                         IN size_t nMax,
                         IN const char* szFormat,
                         IN const tm *pTime)
{
#if defined(MXD_OS_WINDOWS_CE)
    memset(szBuffer, 0, nMax);
    return 0;
#else
    return strftime(szBuffer, nMax, szFormat, pTime);
#endif
}

inline bool MxGetGmtTime(OUT tm& rTime)
{
#if defined(MXD_OS_WINDOWS_CE)
    memset(&rTime, 0, sizeof(struct tm));
    return true;
// BRCM_CUSTOM - [begin] - Add ECOS support.
#elif defined(MXD_OS_BOS_ECOS)
    time_t timeValue = time(NULL);
    if (bosTimeGetGmTime(&timeValue, &rTime) == BOS_STATUS_OK)
    {
       return true;
    }
    else
    {
       return false;
    }
// BRCM_CUSTOM - [end] - Add ECOS support.
#else
    time_t timeValue = time(NULL);
    struct tm *pTm = gmtime(&timeValue);
    if(pTm != NULL) {
        rTime = *pTm;
    }
    return (pTm != NULL);
#endif
}

MX_NAMESPACE_END(MXD_GNS)

//M5T_INTERNAL_USE_END

#endif // MXG_TIME_H
