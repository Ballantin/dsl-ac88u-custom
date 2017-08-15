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
*    Filename: xdrvSlic.h
*
****************************************************************************
*    Description:
*
*     Public API for hardware-independent interface to the SLICs.
*
****************************************************************************/

#ifndef XDRV_SLIC_H
#define XDRV_SLIC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>


/* ---- Constants and Types ---------------------------------------------- */

/* Public SLIC modes */
typedef enum
{
   XDRV_SLIC_MODE_LCFO,
   XDRV_SLIC_MODE_STANDBY,
   XDRV_SLIC_MODE_OHT,
   XDRV_SLIC_MODE_OHTR,
   XDRV_SLIC_MODE_TIPOPEN,
   XDRV_SLIC_MODE_RING,
   XDRV_SLIC_MODE_LCF,
   XDRV_SLIC_MODE_RLCF,
   XDRV_SLIC_MODE_RINGOPEN,
   XDRV_SLIC_MODE_WINK,
   XDRV_SLIC_MODE_NULL

} XDRV_SLIC_MODE;

typedef enum XDRV_SLIC_TYPE
{
   XDRV_SLIC_L9500,
   XDRV_SLIC_L9520,
   XDRV_SLIC_L9530,
   XDRV_SLIC_L9540,
   XDRV_SLIC_L9541,
   XDRV_SLIC_LE89116,
   XDRV_SLIC_SI3226,
   XDRV_SLIC_SI3226X,
   XDRV_SLIC_SI3230,   
   XDRV_SLIC_SI3239,
   XDRV_SLIC_SI32392,
   XDRV_SLIC_EXTAUDIO,
   XDRV_SLIC_NUM_TYPES

} XDRV_SLIC_TYPE;

#define XDRV_SLIC_STATUS_SUCESS XDRV_SLIC_STATUS_SUCCESS 
#define XDRV_SLIC_STATUS_ERROR XDRV_SLIC_STATUS_FAILURE

typedef enum XDRV_SLIC_STATUS
{
   XDRV_SLIC_STATUS_SUCCESS,
   XDRV_SLIC_STATUS_FAILURE

} XDRV_SLIC_STATUS;

/* --------------------------------------------------------------------------
** 'XDRV_SLIC' is the base, generic SLIC structure; it specifies a set of function
** pointers that define the hardware-independent API that must be provided by
** all chip specific SLIC drivers.
**
** 'XDRV_SLIC_FUNCS' must be the only structure member of 'XDRV_SLIC'.
**
** Chip specific SLIC drivers should provide a "derived" version of XDRV_SLIC:
**
**    typedef struct SLIC_CHIP_DRV
**    {
**       const XDRV_SLIC_FUNCS  *drvFuncs;
**
**       int   chipSpecificData1;
**       int   chipSpecificData2;
**       ...
**
**    } SLIC_CHIP_DRV;
**
** Clients, which have access to XDRV_SLIC structs, can use the hardware
** independent macros provided below to transparently invoke hardware specific
** functions. The hardware dependent functions will receive a pointer to the
** XDRV_SLIC struct as the first parameter. This struct can be cast to the chip
** specific version of the SLIC structure to gain access to chip specific
** instance memory.
*/
typedef struct
{
   /* This must be the only structure member. */
   const struct XDRV_SLIC_FUNCS *pDrvFuncs;

} XDRV_SLIC;


/* --------------------------------------------------------------------------
** SLIC API function pointers.
*/

/*
*****************************************************************************
** FUNCTION:   xdrvSlicModeControl
**
** PURPOSE:    Sets the SLIC into one of the defined modes
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             mode  - the mode to set the SLIC into.
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvSlicModeControl)( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode );

/*
*****************************************************************************
** FUNCTION:   xdrvSlicLedControl
**
** PURPOSE:    This function controls the on board Hook State LED
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvSlicLedControl) ( XDRV_SLIC *pDrv, XDRV_UINT16 value );

/*
*****************************************************************************
** FUNCTION:   xdrvSlicPhaseReversalControl
**
** PURPOSE:    This function controls the phase reversal status
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvSlicPhaseReversalControl) ( XDRV_SLIC *pDrv, XDRV_UINT16 value );

/*
*****************************************************************************
** FUNCTION:   xdrvSlicIsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: pDrv - pointer to the SLIC driver device structure
**
** RETURNS:    XDRV_TRUE if offhook, XDRV_FALSE if onhook
**
*****************************************************************************
*/
typedef XDRV_BOOL (*xdrvSlicIsOffhook)  ( XDRV_SLIC *pDrv );

/*
*****************************************************************************
** FUNCTION:   xdrvSlicSetRingParms
**
** PURPOSE:    This function controls the ring waveform
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             ringFrequency  - ringing frequency
**             ringWaveshape  - ringing waveshape
**             ringVoltage    - ringing voltage
**             ringOffset     - ringing DC offset
**             ringOffsetCal  - ringing DC offset calibration
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
typedef int (*xdrvSlicSetRingParms) ( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal );

/*
*****************************************************************************
** FUNCTION:   xdrvSlicGetRingParms
**
** PURPOSE:    This function retrieves the current ring waveform parameters
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             ringFrequency  - ringing frequency
**             ringWaveshape  - ringing waveshape
**             ringVoltage    - ringing voltage
**             ringOffset     - ringing DC offset
**             ringOffsetCal  - ringing DC offset calibration
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
typedef int (*xdrvSlicGetRingParms) ( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal );

/*
*****************************************************************************
** FUNCTION:   xdrvSlicSetBoostedLoopCurrent
**
** PURPOSE:    This function controls the boosted loop current
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 (on) or 0 (off)
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
typedef int (*xdrvSlicSetBoostedLoopCurrent) ( XDRV_SLIC *pDrv, XDRV_UINT16 value );

/*
*****************************************************************************
** FUNCTION:   xdrvSlicSetPowerSource
**
** PURPOSE:    This function notifies the SLIC driver of any changes in 
**             power  source (on battery or AC power)
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             value - 1 = device operating on battery power
**                     0 = device operating on AC power
**
** RETURNS:    0 on success
** 
*****************************************************************************
*/
typedef int (*xdrvSlicSetPowerSource) ( XDRV_SLIC *pDrv, XDRV_UINT16 value );

/*
*****************************************************************************
** FUNCTION:   xdrvSlicGetOverCurrentStatus
**
** PURPOSE:    Check to see if the over current protection has kicked in
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**
** RETURNS:    XDRV_TRUE  - If the over current protection has kicked in
**             XDRV_FALSE - Otherwise
**
*****************************************************************************
*/
typedef XDRV_BOOL (*xdrvSlicGetOverCurrentStatus)( XDRV_SLIC *pDrv );


/*
*****************************************************************************
** FUNCTION:   xdrvSlicGetDlp
**
** PURPOSE:    To get the Slic Decode level point (exgress loss) in dB
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**
** RETURNS:    loss in dB
**
*****************************************************************************
*/
typedef XDRV_SINT16 (*xdrvSlicGetDlp)( XDRV_SLIC *pDrv );


/*
*****************************************************************************
** FUNCTION:   xdrvSlicProcessEvents
**
** PURPOSE:    To process events.
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvSlicProcessEvents)( XDRV_SLIC *pDrv );

/*
*****************************************************************************
** FUNCTION:   xdrvSlicProbe
**
** PURPOSE:    To probe registers. (DEBUG ONLY)
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef XDRV_UINT32 (*xdrvSlicProbe)( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg,
                                      XDRV_UINT32 regSize, void* value, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, 
                                      XDRV_UINT8 set );
									  
/*
*****************************************************************************
** FUNCTION:   xdrvSlicGetSlicParms
**
** PURPOSE:    This function gets the current slic configuration parameters
**
** PARAMETERS: pDrv  - pointer to the SLIC driver device structure
**             phaseReversal  - current phase reversal setting
**             loopCurrent    - current loop current setting
**             powerSource    - current power source config setting
**             slicMode       - current slic mode
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
typedef int (*xdrvSlicGetSlicParms)( XDRV_SLIC *pDrv, int *phaseReversal, int *loopCurrent, int *powerSource, int *slicMode );									  
/*
** Structure that contains all function pointers that define the API.
*/
typedef struct XDRV_SLIC_FUNCS
{
   xdrvSlicModeControl           slicModeControl;
   xdrvSlicLedControl            slicLedControl;
   xdrvSlicPhaseReversalControl  slicPhaseReversalControl;
   xdrvSlicIsOffhook             slicIsOffhook;
   xdrvSlicSetRingParms          slicSetRingParms;
   xdrvSlicGetRingParms          slicGetRingParms;
   xdrvSlicSetBoostedLoopCurrent slicSetBoostedLoopCurrent;
   xdrvSlicSetPowerSource        slicSetPowerSource;
   xdrvSlicGetOverCurrentStatus  slicGetOverCurrentStatus;
   xdrvSlicGetDlp                slicGetDlp;
   xdrvSlicProcessEvents         slicProcessEvents;
   xdrvSlicProbe                 slicProbe;
   xdrvSlicGetSlicParms          slicGetSlicParms;   
} XDRV_SLIC_FUNCS;


/* --------------------------------------------------------------------------
** This is the generic API that should be used by clients to access the SLIC.
*/

#define xdrvSlicModeControl( pDrv, mode ) \
         ( pDrv->pDrvFuncs->slicModeControl( pDrv, mode ) )

#define xdrvSlicLedControl( pDrv, value ) \
         ( pDrv->pDrvFuncs->slicLedControl( pDrv, value ) )

#define xdrvSlicPhaseReversalControl( pDrv, value ) \
         ( pDrv->pDrvFuncs->slicPhaseReversalControl( pDrv, value ) )

#define xdrvSlicIsOffhook( pDrv ) \
         ( pDrv->pDrvFuncs->slicIsOffhook( pDrv ) )

#define xdrvSlicSetRingParms( pDrv, freq, waveform, voltage, offset, offsetcal ) \
         ( pDrv->pDrvFuncs->slicSetRingParms( pDrv, freq, waveform, voltage, offset, offsetcal ) )

#define xdrvSlicGetRingParms( pDrv, freq, waveform, voltage, offset, offsetcal ) \
         ( pDrv->pDrvFuncs->slicGetRingParms( pDrv, freq, waveform, voltage, offset, offsetcal ) )

#define xdrvSlicSetBoostedLoopCurrent( pDrv, value ) \
         ( pDrv->pDrvFuncs->slicSetBoostedLoopCurrent( pDrv, value ) )

#define xdrvSlicSetPowerSource( pDrv, value ) \
         ( pDrv->pDrvFuncs->slicSetPowerSource( pDrv, value ) )

#define xdrvSlicGetOverCurrentStatus( pDrv ) \
         ( pDrv->pDrvFuncs->slicGetOverCurrentStatus( pDrv ) )

#define xdrvSlicGetDlp( pDrv ) \
         ( pDrv->pDrvFuncs->slicGetDlp( pDrv ) )
         
#define xdrvSlicProcessEvents( pDrv )\
         ( pDrv->pDrvFuncs->slicProcessEvents( pDrv ) )
         
#define xdrvSlicProbe( pDrv, deviceId, chan, reg, regSize, value, valueSize, indirect, set )\
         ( pDrv->pDrvFuncs->slicProbe( pDrv, deviceId, chan, reg, regSize, value, valueSize, indirect, set ) )

#define xdrvSlicGetSlicParms( pDrv, phaseReversal, loopCurrent, powerSource, slicMode ) \
         ( pDrv->pDrvFuncs->slicGetSlicParms( pDrv, phaseReversal, loopCurrent, powerSource, slicMode ) )

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_SLIC_H  */
