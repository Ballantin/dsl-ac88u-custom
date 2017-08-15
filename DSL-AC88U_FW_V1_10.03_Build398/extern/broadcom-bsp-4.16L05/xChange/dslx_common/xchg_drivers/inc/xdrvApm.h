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
*    Filename: xdrvApm.h
*
****************************************************************************
*    Description:
*
*     Public API for hardware-independent interface to the Audio
*     Processor Module (APM).
*
****************************************************************************/

#ifndef XDRV_APM_H
#define XDRV_APM_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>

/* ---- Constants and Types ---------------------------------------------- */

typedef enum XDRV_APM_HVG_VOLTAGE
{
   XDRV_APM_HVG_VOLTAGE_OFFHOOK,
   XDRV_APM_HVG_VOLTAGE_ONHOOK,
   XDRV_APM_HVG_VOLTAGE_ONHOOK_LP,
   XDRV_APM_HVG_VOLTAGE_RING,
   XDRV_APM_HVG_VOLTAGE_UHV_RING_13,
   XDRV_APM_HVG_VOLTAGE_UHV_RING_15,
   XDRV_APM_HVG_VOLTAGE_TEST1,
   XDRV_APM_HVG_VOLTAGE_TEST2,
   XDRV_APM_HVG_VOLTAGE_NONE

} XDRV_APM_HVG_VOLTAGE;

typedef enum XDRV_APM_HVG_DESIGN
{
   XDRV_APM_HVG_DESIGN_BUCKBOOST,
   XDRV_APM_HVG_DESIGN_BUCKBOOST_ALT,
   XDRV_APM_HVG_DESIGN_FLYBACK,
   XDRV_APM_HVG_DESIGN_FLYBACK_LPV,
   XDRV_APM_HVG_DESIGN_FLYBACK_TRACKING,
   XDRV_APM_HVG_DESIGN_NUM_TYPES
} XDRV_APM_HVG_DESIGN;

typedef enum XDRV_APM_DEBUG_TYPE
{
   XDRV_APM_APM_DEBUG_OFF,
   XDRV_APM_APM_DEBUG_EGRESS,
   XDRV_APM_APM_DEBUG_INGRESS,
   XDRV_APM_APM_DEBUG_LOOPBACK
} XDRV_APM_DEBUG_TYPE;

#define XDRV_APM_BATTERY_ENABLED            0x01
#define XDRV_APM_ENHANCED_CONTROL_ENABLED   0x10

/* --------------------------------------------------------------------------
** 'XDRV_APM' is the base, generic APM structure; it specifies a set of function
** pointers that define the hardware-independent API that must be provided by
** all chip specific APM drivers.
**
** 'XDRV_APM_FUNCS' must be the only structure member of 'XDRV_APM'.
**
** Chip specific APM drivers should provide a "derived" version of XDRV_APM:
**
**    typedef struct APM_CHIP_DRV
**    {
**       const XDRV_APM_FUNCS  *drvFuncs;
**
**       int   chipSpecificData1;
**       int   chipSpecificData2;
**       ...
**
**    } APM_CHIP_DRV;
**
** Clients, which have access to XDRV_APM structs, can use the hardware
** independent macros provided below to transparently invoke hardware specific
** functions. The hardware dependent functions will receive a pointer to the
** XDRV_APM struct as the first parameter. This struct can be cast to the chip
** specific version of the APM structure to gain access to chip specific
** instance memory.
*/
typedef struct
{
   /* This must be the only structure member. */
   const struct XDRV_APM_FUNCS *pDrvFuncs;

} XDRV_APM;


/* --------------------------------------------------------------------------
** APM API function pointers.
*/

/*
*****************************************************************************
** FUNCTION:   xdrvApmSetRingParms
**
** PURPOSE:    Sets the APM ringing waveform parameters
**
** PARAMETERS: pDrv - pointer to the APM driver device structure
**             ringFrequency  - ringing frequency
**             ringWaveshape  - ringing waveshape
**             ringVoltage    - ringing voltage
**             ringOffset     - ringing DC offset
**             ringOffsetCal  - ringing DC offset calibration
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvApmSetRingParms)( XDRV_APM *pDrv, int ringFrequency, int ringWaveshape,
                                     int ringVoltage, int ringOffset, int ringOffsetCal );

/*
*****************************************************************************
** FUNCTION:   xdrvApmGetRingParms
**
** PURPOSE:    Gets the APM ringing waveform parameters
**
** PARAMETERS: pDrv           - pointer to the APM driver device structure
**             ringFrequency  - ringing frequency
**             ringWaveshape  - ringing waveshape
**             ringVoltage    - ringing voltage
**             ringOffset     - ringing DC offset
**             ringOffsetCal  - ringing DC offset calibration
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvApmGetRingParms)( XDRV_APM *pDrv, int *ringFrequency, int *ringWaveshape,
                                     int *ringVoltage, int *ringOffset, int *ringOffsetCal );

/*
*****************************************************************************
** FUNCTION:   xdrvApmGetDlp
**
** PURPOSE:    Retreive the APM Decode Level Point (receive loss) in dB
**
** PARAMETERS: pDrv - pointer to the APM driver interface
**
** RETURNS:    DLP in dB
**
*****************************************************************************
*/
typedef XDRV_SINT16 (*xdrvApmGetDlp)( XDRV_APM *pDrv );

/*
*****************************************************************************
** FUNCTION:   xdrvApmGetElp
**
** PURPOSE:    Retreive the APM Encode Level Point (ingress loss) in dB
**
** PARAMETERS: pDrv - pointer to the APM driver interface
**
** RETURNS:    ELP in dB
**
*****************************************************************************
*/
typedef XDRV_SINT16 (*xdrvApmGetElp)( XDRV_APM *pDrv );

/*
*****************************************************************************
** FUNCTION:   xdrvApmDebugStatusDump
**
** PURPOSE:    Dump APM registers for debug purpose
**
** PARAMETERS: pDrv - pointer to the APM driver interface
**
** RETURNS:    0 on success
**
** NOTES:      none
**
*****************************************************************************
*/
typedef XDRV_SINT32 (*xdrvApmDebugStatusDump)( XDRV_APM *pDrv );

/*
*****************************************************************************
** FUNCTION:   xdrvApmGetOverCurrentStatus
**
** PURPOSE:    Check to see if the HVG over current protection has kicked in
**
** PARAMETERS: pDrv  - pointer to the APM driver device structure
**             chan  - channel for which the over current status should be
**                     checked
**
** RETURNS:    XDRV_TRUE  - If the over current protection has kicked in on
**                          the HVG registers
**             XDRV_FALSE - Otherwise
**
*****************************************************************************
*/
typedef XDRV_BOOL (*xdrvApmGetOverCurrentStatus)( XDRV_APM *pDrv, int chan );

/*
*****************************************************************************
** FUNCTION:   xdrvApmGetOverVoltageStatus
**
** PURPOSE:    Check to see if an HVG over voltage condition exists
**
** PARAMETERS: pDrv  - pointer to the APM driver device structure
**             chan  - channel for which the over voltage status should be
**                     checked
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvApmGetOverVoltageStatus)( XDRV_APM *pDrv, int chan );

/*
*****************************************************************************
** FUNCTION:   xdrvApmPulseSignal
**
** PURPOSE:    Generate Pulse Metering Signal
**
** PARAMETERS: pDrv       - pointer to the APM driver device structure
**             endpt      - endpoint to control
**             duration   - duration of signal
**             period     - period of signal
**             repetition - number of repetitions of signal
**             amplitude  - amplitude of signal
**             frequency  - frequency of signal
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void (*xdrvApmPulseSignal)( XDRV_APM *pDrv, int endpt, int duration, int period,
                                    int repetition, int amplitude, int frequency );

/*
*****************************************************************************
** FUNCTION:   xdrvApmSetHPF
**
** PURPOSE:    Set HPF in APM module
**
** PARAMETERS: pDrv  - pointer to the APM driver interface
**             endpt  - endpoint to control
**             enable - enable or disable HPF
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef void      (*xdrvApmSetHPF)( XDRV_APM *pDrv, int endpt, XDRV_BOOL enable );

/*
*****************************************************************************
** FUNCTION:   xdrvApmHvgSetVoltage
**
** PURPOSE:    Change the HVG Voltage
**
** PARAMETERS: pDrv       - pointer to the APM driver device structure
**             endpt      - endpoint to control
**             voltage    - voltage level
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef int (*xdrvApmHvgSetVoltage)( XDRV_APM *pDrv, int endpt, int voltage );

/*
*****************************************************************************
** FUNCTION:   xdrvApmHvgUpdateSlicStatus
**
** PURPOSE:    Update HVG parameters based on latest operating state
**
** PARAMETERS: pDrv        - pointer to the APM driver device structure
**             endpt       - endpoint to control
**             slicState   - current SLIC mode
**             dcOffset    - dc offset enabled
**             battery     - on battery power
**             loopCurrent - boosted loop current status
**             ringStop    - set if ringing is ending
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef int (*xdrvApmHvgUpdateSlicStatus)( XDRV_APM *pDrv, int endpt, int slicState,
                                           int dcOffset, int battery, int loopCurrent,
                                           XDRV_BOOL ringStop );

/*
*****************************************************************************
** FUNCTION:   xdrvApmHvgUpdateHookStatus
**
** PURPOSE:    Update HVG parameters based on latest hook state
**
** PARAMETERS: pDrv       - pointer to the APM driver device structure
**             endpt      - endpoint to control
**             hookState  - current hook state
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef int (*xdrvApmHvgUpdateHookStatus)( XDRV_APM *pDrv, int endpt, int hookState );

/*
*****************************************************************************
** FUNCTION:   xdrvApmHvgUpdateHookStatusEnable
**
** PURPOSE:    Disable HVG changes on hook state updates
**
** PARAMETERS: pApmDrv     - pointer to the APM driver interface
**             chan        - the line number
**             update      - enable or disable updates
**
** RETURNS: 0 on SUCCESS
**
*****************************************************************************
*/
typedef int (*xdrvApmHvgUpdateHookStatusEnable)( XDRV_APM *pDrv, int endpt, XDRV_BOOL update );

/*
*****************************************************************************
** FUNCTION:   xdrvApmSetTxRxGains
**
** PURPOSE:    Change the ingress and egress gains in the APM block
**
** PARAMETERS: pDrv       - pointer to the APM driver device structure
**             chan       - channel to modify
**             txGain     - desired ingress gain
**             rxGain     - desired egress gain
**             apmTxGain  - realized ingress gain
**             apmRxGain  - realized egress gain
**
** RETURNS:    none
**
*****************************************************************************
*/
typedef int (*xdrvApmSetTxRxGains)( XDRV_APM *pDrv, int chan, int txGain, int rxGain, int *apmTxGain, int *apmRxGain );
											 

/*
** Structure that contains all function pointers that define the API.
*/
typedef struct XDRV_APM_FUNCS
{
   xdrvApmSetRingParms                 apmSetRingParms;
   xdrvApmGetRingParms                 apmGetRingParms;
   xdrvApmGetDlp                       apmGetDlp;
   xdrvApmDebugStatusDump              apmDebugStatusDump;
   xdrvApmGetElp                       apmGetElp;
   xdrvApmGetOverCurrentStatus         apmGetOverCurrentStatus;
   xdrvApmGetOverVoltageStatus         apmGetOverVoltageStatus;
   xdrvApmPulseSignal                  apmPulseSignal;
   xdrvApmHvgSetVoltage                apmHvgSetVoltage;
   xdrvApmSetHPF                       apmSetHPF;
   xdrvApmHvgUpdateSlicStatus          apmHvgUpdateSlicStatus;
   xdrvApmHvgUpdateHookStatus          apmHvgUpdateHookStatus;
   xdrvApmHvgUpdateHookStatusEnable    apmHvgUpdateHookStatusEnable;
   xdrvApmSetTxRxGains                 apmSetTxRxGains;
      

} XDRV_APM_FUNCS;


/* --------------------------------------------------------------------------
** This is the generic API that should be used by clients to access the APM.
*/

#define xdrvApmSetRingParms( pDrv, freq, waveform, voltage, offset, offsetcal ) \
         ( pDrv->pDrvFuncs->apmSetRingParms( pDrv, freq, waveform, voltage, offset, offsetcal ) )

#define xdrvApmGetRingParms( pDrv, freq, waveform, voltage, offset, offsetcal ) \
         ( pDrv->pDrvFuncs->apmGetRingParms( pDrv, freq, waveform, voltage, offset, offsetcal ) )

#define xdrvApmGetDlp( pDrv ) \
         ( pDrv->pDrvFuncs->apmGetDlp( pDrv ) )

#define xdrvApmGetElp( pDrv ) \
         ( pDrv->pDrvFuncs->apmGetElp( pDrv ) )

#define xdrvApmDebugStatusDump( pDrv ) \
         ( pDrv->pDrvFuncs->apmDebugStatusDump( pDrv ) )

#define xdrvApmSetRingDCoffset( pDrv, dcoffset, waveform ) \
         ( pDrv->pDrvFuncs->apmSetRingDCoffset( pDrv, dcoffset, waveform ) )

#define xdrvApmGetOverCurrentStatus( pDrv, chan ) \
         ( pDrv->pDrvFuncs->apmGetOverCurrentStatus( pDrv, chan ) )

#define xdrvApmGetOverVoltageStatus( pDrv, chan ) \
         ( pDrv->pDrvFuncs->apmGetOverVoltageStatus( pDrv, chan ) )

#define xdrvApmPulseSignal( pDrv, endpt, duration, period, repetition, amplitude, frequency ) \
         ( pDrv->pDrvFuncs->apmPulseSignal( pDrv, endpt, duration, period, repetition, amplitude, frequency ) )

#define xdrvApmHvgSetVoltage( pDrv, endpt, voltage ) \
         ( pDrv->pDrvFuncs->apmHvgSetVoltage( pDrv, endpt, voltage ) )

#define xdrvApmSetHPF( pDrv, endpt, enable ) \
         ( pDrv->pDrvFuncs->apmSetHPF( pDrv, endpt, enable ) )
		 
#define xdrvApmHvgUpdateSlicStatus( pDrv, endpt, slicState, dcOffset, battery, loopCurrent, ringStop ) \
         ( pDrv->pDrvFuncs->apmHvgUpdateSlicStatus( pDrv, endpt, slicState, dcOffset, battery, loopCurrent, ringStop ) )

#define xdrvApmHvgUpdateHookStatus( pDrv, endpt, hookState ) \
         ( pDrv->pDrvFuncs->apmHvgUpdateHookStatus( pDrv, endpt, hookState ) )

#define xdrvApmHvgUpdateHookStatusEnable( pDrv, endpt, update ) \
         ( pDrv->pDrvFuncs->apmHvgUpdateHookStatusEnable( pDrv, endpt, update ) )

#define xdrvApmSetTxRxGains( pDrv, chan, txGain, rxGain, apmTxGain, apmRxGain ) \
         ( pDrv->pDrvFuncs->apmSetTxRxGains( pDrv, chan, txGain, rxGain, apmTxGain, apmRxGain ) )
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_APM_H  */
