/****************************************************************************
*
* Copyright (c) 2009-2012 Broadcom
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
*  hvg6816.h - BCM6816 High Voltage Generator Definitions
*
*  PURPOSE:
*
*  NOTES:
*
****************************************************************************/

#ifndef HVG6816_H
#define HVG6816_H

#if defined(__cplusplus)
  extern "C" {
#endif

#include <xdrvTypes.h>
#include <xdrvApm.h>
#include <bcm6816.h>

/* Typedef to local type */
typedef HvgControlRegisters BCM6816_HVG;

typedef struct BCM6816_HVG_CUSTOM
{
   /* HVG Shared Settings */
   XDRV_UINT32 reg_hvg_track_max;          /* Reg 7 */ 
   XDRV_UINT32 reg_hvg_ring;               /* Reg 9 */ 

   /* HVG Channel Settings */
   XDRV_UINT16 reg_hvg_max_hvg_slic;       /* Reg 14,18 - Upper 16 Bits */
   XDRV_UINT16 reg_hvg_const_volt_goal;    /* Reg 14,18 - Lower 16 Bits */

} BCM6816_HVG_CUSTOM;

#define BCM6816_HVG_PTR           ((volatile BCM6816_HVG *)APM_HVG_BASE)

int hvg6816Init( XDRV_APM *pApmDrv );
int hvg6816Start( int chan );
int hvg6816Stop( int chan );
int hvg6816Reset( int chan );
int hvg6816IsEnabled( int chan );
XDRV_BOOL hvg6816GetOverCurrentStatus( XDRV_APM *pDrv, int chan );
void hvg6816GetOverVoltageStatus( XDRV_APM *pDrv, int chan );
int hvg6816SetVoltage( XDRV_APM *pApmDrv, int chan, int voltage );
int hvg6816UpdateSlicStatus( XDRV_APM *pApmDrv, int chan, int slicState,
                             int dcOffset, int battery, int loopCurrent,
                             XDRV_BOOL ringStop );
int hvg6816UpdateHookStatus( XDRV_APM *pApmDrv, int chan, int hookState );
int hvg6816UpdateHookStatusEnable( XDRV_APM *pApmDrv, int chan, XDRV_BOOL update );

#if defined(__cplusplus)
}
#endif

#endif /* HVG6816_H */
