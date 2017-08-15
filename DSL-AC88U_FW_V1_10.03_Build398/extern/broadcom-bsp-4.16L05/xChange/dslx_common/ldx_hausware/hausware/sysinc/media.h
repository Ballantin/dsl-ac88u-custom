/****************************************************************************
*
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    media.h
*
*  @brief   Global definitions for media definitions.
*
****************************************************************************/
/**
*  @defgroup media
*
*  @brief   Defines generic media definitions
*
****************************************************************************/

#if !defined( MEDIA_H )
#define MEDIA_H

/* ---- Include Files ---------------------------------------------------- */

#if !defined( HAPI_TYPES_H )
#  include <hapi_types.h>
#endif

#if !defined( XCFG_HSF_H )
#  include <xcfg_hsf.h>
#endif

/**
 * @addtogroup media
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */

/**
* Defines the sampling rate left shift constant or the sampling rate
* log base 2 numbers.
*/
typedef enum
{
   MEDIA_8KHZ_RATE_SHIFT = 0,
   MEDIA_16KHZ_RATE_SHIFT = 1,
   MEDIA_MAX_RATE_SHIFTS

} MEDIA_RATE_SHIFT;

/**
* Macros to calculated number of samples based on the rate shift.
*/

#define MediaGetNumSamples( x, shift) ((x) << (shift))

/* MEDIA_IBD_xxKHZ_MASK specifies the in-band packet receive
 * mask of the HAL device.
 */
#define MEDIA_IBD_8KHZ_MASK   HSF_IBD_LOWBAND_MASK 
#define MEDIA_IBD_16KHZ_MASK  HSF_IBD_HIGHBAND_MASK

/* MEDIA_SYSTEM_RATE_SHIFT specifies the left shift constant
 * for the max sampling rate of the system.
 * MEDIA_SYSTEM_IBD_MASK specifies the in-band packet receive
 * mask for the max sampling rate of the system.
 */
#if XCFG_HSF_WIDEBAND_SUPPORT

#  define MEDIA_SYSTEM_RATE_SHIFT   MEDIA_16KHZ_RATE_SHIFT
#  define MEDIA_SYSTEM_IBD_MASK     MEDIA_IBD_16KHZ_MASK

#else

#  define MEDIA_SYSTEM_RATE_SHIFT   MEDIA_8KHZ_RATE_SHIFT
#  define MEDIA_SYSTEM_IBD_MASK     MEDIA_IBD_8KHZ_MASK

#endif /* HSF_WIDEBAND_SUPPORT */

/* ---- Variable Externs ------------------------------------------------- */

/* ---- Function Prototypes ---------------------------------------------- */

/** @} */

#endif /* MEDIA_H */

