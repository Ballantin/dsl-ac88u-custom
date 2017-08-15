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
*    Filename: hdspCfg.h
*
****************************************************************************
*    Description:
*
*     HDSP default build options.  To override defaults in
*     hdspCfg.h make a file called hdspCfgCustom.h in your build
*     directory.
*
****************************************************************************/

#ifndef HDSP_CFG_H
#define HDSP_CFG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <hdspCfgCustom.h>


/*
*****************************************************************************
** CONSTANT DECLARATIONS
*****************************************************************************
*/


/* Which tone pattern type should be used */
#ifndef HDSP_CFG_TONEPROV_IETF_SIG_MIB
#define HDSP_CFG_TONEPROV_IETF_SIG_MIB   0
#endif

/* Call Monitoring Tone Detection Service - disabled by default. */
#ifndef HDSP_CFG_CMTD_ENABLED
#define HDSP_CFG_CMTD_ENABLED          0
#endif

/* Debug support - enabled by default
** When enabled, HDSP will utilize the platform specific Debug Module
** to retrieve debug information from the DSP upon failure conditions:
** command failures/timeouts, etc. */
#ifndef HDSP_CFG_DEBUG_SUPPORT
#define HDSP_CFG_DEBUG_SUPPORT         1
#endif

/* Software HPF - disabled by default */
#ifndef HDSP_CFG_USE_SF_HPF
#define HDSP_CFG_USE_SF_HPF            0
#endif

/* Use float numbers for ICP calculation */
#ifndef HDSP_CFG_ICP_USE_FLOATS
#define HDSP_CFG_ICP_USE_FLOATS        1
#endif

/* DSP Error recovery mechanism */
#ifndef HDSP_ERR_RECOVERY_MECHANISM
#define HDSP_ERR_RECOVERY_MECHANISM    1
#endif

/* Maximum number of running errors per VHD.  Running errors are decremented
** upon subsequent successful commands */
#ifndef HDSP_ERR_RECOVERY_MAX_VHD_ERR_COUNT
#define HDSP_ERR_RECOVERY_MAX_VHD_ERR_COUNT           5
#endif

/* Maximum number of total errors per VHD */
#ifndef HDSP_ERR_RECOVERY_MAX_VHD_TOTAL_ERR_COUNT
#define HDSP_ERR_RECOVERY_MAX_VHD_TOTAL_ERR_COUNT     10
#endif

/* Maximum number of running errors for the overall device.  Running errors
** are decremented upon subsequent successful commands */
#ifndef HDSP_ERR_RECOVERY_MAX_DEVICE_ERR_COUNT
#define HDSP_ERR_RECOVERY_MAX_DEVICE_ERR_COUNT        10
#endif

/* Maximum number of total errors for the overall device */
#ifndef HDSP_ERR_RECOVERY_MAX_DEVICE_TOTAL_ERR_COUNT
#define HDSP_ERR_RECOVERY_MAX_DEVICE_TOTAL_ERR_COUNT  20
#endif

#ifdef __cplusplus
    }
#endif

#endif  /* HDSP_CFG_H  */
