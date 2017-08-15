/****************************************************************************
*
*     Copyright (c) 2003 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
*****************************************************************************
*
*  halshim.h
*
*  PURPOSE:
*
*     This file contains public definitions for the HAL Shim layer.
*
*  NOTES:
*
****************************************************************************/

#ifndef HALSHIM_H
#define HALSHIM_H

/* ---- Include Files ---------------------------------------------------- */

#if !defined( HAPI_TYPES_H )
#  include "hapi_types.h"
#endif

#if !defined( HSF_MASKS_H )
#  include "hsf_masks.h"
#endif

#if !defined( INLINE_LDX_H )
#  include "inline_ldx.h"
#endif

#if !defined( MEDIA_H )
#  include <media.h>
#endif

#include <ldx_hapi.h>
#include <lhapi.h>


/* HALSHIM_CONFIG is used to configure the specifics of each
 * HAL Shim device.
 */
typedef struct
{
   /*
   ** This store the ldxHandle of this hal data access point.
   ** That is,  ((dspNum << 8) | index).  For single dsp systems the index
   ** and the handle will be the same.
   */
   unsigned    halShimLdxHandle;

   /* Enabled/Disabled by default
    */
   int         isEnabled;

   /* HAL Device in-band receive mask
    *   MEDIA_IBD_8KHZ_MASK specifies 8 kHz HAL
    *   MEDIA_IBD_16KHZ_MASK specifies 16 kHz HAL
    *   MEDIA_SYSTEM_IBD_MASK specifies system configured max sampling rate
    */
   HSF_IBD_MASK mask;

   /* Specifies number of samples received per frame
    */
   unsigned int frameSamples;

} HALSHIM_CONFIG;

/* HALSHIM_CONFIG_EXTENDED is an extended version of HALSHIM_CONFIG. The config entry
 * is embedded within the structure along with a point to HAL DEVICE specific data.
 */
typedef struct
{
   HALSHIM_CONFIG        halShimConfigEntry;
   LHAPI_HALSHIM_PRIVATE *priv;
  
} HALSHIM_CONFIG_EXTENDED;

/* ---- Variable Externs ------------------------------------------------- */

/* Global structure containing information for each HAL device
 */
extern HALSHIM_CONFIG_EXTENDED gHalShimConfigs[];

/* ---- Function Prototypes ---------------------------------------------- */

/****************************************************************************
*  halShimIsEnabled
*
*  PURPOSE:
*     Indicates whether a HAL device is enabled.
*
*  PARAMETERS:
*     handle     (in) Handle of HAL device
*
*  RETURNS:
*     TRUE if HAL is enabled
*     FALSE if HAL is disabled
****************************************************************************/
INLINE int halShimIsEnabled( unsigned halShimIndex )
{
   return gHalShimConfigs[ LDX_HandleGetInternal((LDX_Handle)halShimIndex) ].halShimConfigEntry.isEnabled;

} /* halShimIsEnabled */

/****************************************************************************
*  halShimIsWideband
*
*  PURPOSE:
*     Indicates whether a HAL device is running in wideband mode.
*
*  PARAMETERS:
*     handle     (in) Handle of HAL device
*
*  RETURNS:
*     TRUE if HAL is wideband
*     FALSE if HAL is narrowband
****************************************************************************/
INLINE int halShimIsWideband( unsigned halShimIndex )
{
   return (gHalShimConfigs[ LDX_HandleGetInternal((LDX_Handle)halShimIndex) ].halShimConfigEntry.mask == MEDIA_IBD_16KHZ_MASK);

} /* halShimIsEnabled */

/*****************************************************************************
*  FUNCTION:   halShimGetMask
*
*  PURPOSE:    Gets the HAL Shim in-band receive mask
*
*  PARAMETERS:
*     halShimIndex (in)  Index of the HAL Device
*
*  RETURNS:    In-band receive mask
*****************************************************************************/
INLINE HSF_IBD_MASK halShimGetMask( unsigned halShimIndex )
{
   return gHalShimConfigs[ LDX_HandleGetInternal((LDX_Handle)halShimIndex) ].halShimConfigEntry.mask;

} /* halShimGetMask */

/*****************************************************************************
*  FUNCTION:   halShimGetMask
*
*  PURPOSE:    Gets the private data area of a HAL SHIM device table entry
*
*  PARAMETERS:
*     halShimIndex (in)  Index of the HAL Device
*
*  RETURNS:    Private data area
*****************************************************************************/
INLINE LHAPI_HALSHIM_PRIVATE* halShimGetPriv( unsigned halShimIndex )
{
   return gHalShimConfigs[ LDX_HandleGetInternal((LDX_Handle)halShimIndex) ].priv;

} /* halShimGetMask */

/*****************************************************************************
*  FUNCTION:   halShimCfgDevices
*
*  PURPOSE:    Configures the HAL shim devices
*
*  PARAMETERS:
*     cfg         (in) Pointer to configuration array for HAL devices
*     numDevices  (in) Number of hal shim devices
*     privs       (in) Private data table
*
*  RETURNS:    0 if successful.  Negative value if failure
*****************************************************************************/
int halShimCfgDevices( LHAPI_HALSHIM_CONFIG *cfg, unsigned int numDevices, LHAPI_HALSHIM_PRIVATE *privs );

/*****************************************************************************
*  FUNCTION:   halShimGetNumDevCfgd
*
*  PURPOSE:    Gets the number of HAL shim devices that have been configured.
*              Cannot exceed configured maximum allowable HAL shim devices.
*
*  PARAMETERS:
*
*  RETURNS:    Number of configured HAL shim devices
*****************************************************************************/
unsigned int halShimGetNumDevCfgd( void );

/****************************************************************************
* halShimIngress
*     halShimIndex (in)  Index of the HAL Device
*
*/
void halShimIngress( void* bufp, unsigned halShimIndex );

/****************************************************************************
* halShimEgress
*     halShimIndex (in)  Index of the HAL Device
*
*/
void halShimEgress( void* bufp, unsigned halShimIndex );

#endif /* HALSHIM_H */
