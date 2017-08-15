/****************************************************************************
*
*  Copyright (c) 2005-2014 Broadcom

    <:label-BRCM:2014:DUAL/GPL:standard
    
    Unless you and Broadcom execute a separate written software license
    agreement governing use of this software, this software is licensed
    to you under the terms of the GNU General Public License version 2
    (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
    with the following added to such license:
    
       As a special exception, the copyright holders of this software give
       you permission to link this software with independent modules, and
       to copy and distribute the resulting executable under terms of your
       choice, provided that you also meet, for each linked independent
       module, the terms and conditions of the license of that module.
       An independent module is a module which is not derived from this
       software.  The special exception does not apply to any modifications
       of the software.
    
    Not withstanding the above, under no circumstances may you combine
    this software in any way with any other Broadcom software provided
    under a license other than the GPL, without Broadcom's express prior
    written consent.
    
:>
*/

/****************************************************************************
*    Description:
*		This is the Legerity LE88506 - Single Channel Digital SLIC Device
*		driver definition.
*
****************************************************************************/
#ifndef SLIC_Le9672_H    /* support nested includes */
#define SLIC_Le9672_H

#if defined(__cplusplus)
extern "C" {
#endif


#include <bcmChip.h>
#include <board.h>
#include <vrgCountry.h>
#include "zarlinkCommonSlic.h"

/* ---- Constants and Types ---------------------------------- */
#define Le9672_FXS_LINES    2  

/* ---- Variable Externs ------------------------------------- */

/* ---- Function Prototypes ---------------------------------- */

XDRV_SLIC_STATUS slicLe9672Init( SLIC_DEVICE_DRV *pDev, XDRV_UINT16 resetGpioPin );
XDRV_SLIC_STATUS slicLe9672Deinit( SLIC_DEVICE_DRV *pDev );
#ifdef DYING_GASP_API
XDRV_SLIC_STATUS slicLe9672Shutdown( XDRV_UINT16 rstGpioPin );
#endif

#ifdef __cplusplus
    }
#endif

#endif   /* SLIC_Le9672_H */
