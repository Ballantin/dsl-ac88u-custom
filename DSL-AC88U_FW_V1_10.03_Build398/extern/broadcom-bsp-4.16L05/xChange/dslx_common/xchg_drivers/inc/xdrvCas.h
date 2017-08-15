/***************************************************************************
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
****************************************************************************
*
*    Filename: xdrvCas.h
*
****************************************************************************
*    Description:
*
*     Public API for hardware-independent interface to the CAS driver.
*
****************************************************************************/

#ifndef XDRV_CAS_H
#define XDRV_CAS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvSlic.h>
#include <xdrvApm.h>

/* ---- Constants and Types ---------------------------------------------- */

/* Hook state */
typedef enum XDRV_CAS_HOOKSTATE
{
   XDRV_CAS_UNKNOWN = -1,
   XDRV_CAS_ONHOOK  = 0,
   XDRV_CAS_OFFHOOK = 1
} XDRV_CAS_HOOKSTATE;

#define XDRV_CAS_HIST_BUFFER_SIZE   20

typedef struct XDRV_CAS_HISTORY
{
   XDRV_CAS_HOOKSTATE            currentState;
   int                           currentIntervalMsec;
   int                           debounceIntervalMsec;
   XDRV_BOOL                     bHistorySupported;
   XDRV_BOOL                     historyEnabled;
   int                           historyRateMsec;
   XDRV_CAS_HOOKSTATE            hookStateBuf[XDRV_CAS_HIST_BUFFER_SIZE];
   volatile XDRV_CAS_HOOKSTATE  *writep;  /* Write Pointer location in buffer */
   volatile XDRV_CAS_HOOKSTATE  *readp;   /* Read Pointer Location in buffer */
   volatile XDRV_CAS_HOOKSTATE  *prevp;   /* Pointer to previous hook state position in buffer */
} XDRV_CAS_HISTORY;


typedef struct XDRV_CAS
{
   XDRV_SLIC        *slicDriver;
   XDRV_APM         *apmDriver;
   XDRV_CAS_HISTORY  casHistory;

   /* Fast slic state mode; lets the casDriver manipulate the SLIC state
   ** as soon as it detects a hook switch change (2 ms), rather then leaving
   ** it up to the CAS service to debounce the full connect time (20 ms) before
   ** detecting a hook switch and changing the SLIC state.
   ** This is needed for some CWCLID devices that perfom a fake offhook
   ** switch and expect the SLIC mode to change instantly.*/
   XDRV_BOOL         bFastSlicStateModeEnabled;
   XDRV_BOOL         bFastSlicStateModeSupported;
   
   XDRV_BOOL         bDrvEnabled;   /* Boolean indicating enable/disable status of the channel driver */
   int               chan;          /* Channel number for this instance */

   struct XDRV_CAS  *nextDrv;
} XDRV_CAS;



/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

#define xdrvCasSetSlicMode( drv, mode ) xdrvSlicModeControl( (drv)->slicDriver, mode )
#define xdrvCasHistoryEnabled( drv )    ( (drv)->casHistory.historyEnabled )
#define xdrvCasHistoryRateMsec( drv )   ( (drv)->casHistory.historyRateMsec )

int                  xdrvCasNumHookSamples( XDRV_CAS *casDriver );
XDRV_CAS_HOOKSTATE   xdrvCasGetHookState  ( XDRV_CAS *casDriver );
int xdrvCasEnableFastSlicStateMode( XDRV_CAS *casDriver, XDRV_BOOL enable );

#ifdef __cplusplus
}
#endif

#endif  /* XDRV_CAS_H  */
