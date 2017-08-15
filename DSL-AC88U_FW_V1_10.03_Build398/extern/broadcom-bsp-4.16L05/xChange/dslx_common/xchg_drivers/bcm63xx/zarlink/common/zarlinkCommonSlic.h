/***************************************************************************
*
*    Copyright 2008  Broadcom
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
*    Description: 
*      This is common code for all Zarlink (Legerity) SLIC devices, that uses API-2
*
***************************************************************************/
#ifndef ZARLINK_COMMON_SLIC_H
#define ZARLINK_COMMON_SLIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <vp_api.h>
#include <vp_profile.h>
#include <xdrvTypes.h>

#include <xdrvSlic.h>
#include <vrgCountry.h>
#include <xdrvDaa.h>




/* Constants declarations */
#define SLIC_INIT_TIMEOUT 500
#define MAX_NUM_DEVICES   2
#define MAX_NUM_LINES     4

/* Common CodecTypes */
#define ZARLINK_LINEAR       1
#define ZARLINK_ULAW         2
#define ZARLINK_ALAW         3
#define ZARLINK_WIDEBAND     4

#define ZARLINK_THALASSA           0x8000
#define ZARLINK_THALASSA_SUBSLAC1  0x0100
#define ZARLINK_THALASSA_SUBSLAC2  0x0200
#define ZARLINK_ZSI                0x4000

typedef enum
{
   ZARLINK_FLYBACK,
   ZARLINK_BUCKBOOST,
   ZARLINK_INVBOOST,
   ZARLINK_MASTERSLAVE_FB,
   ZARLINK_INVBOOST_THALASSA,
   ZARLINK_FLYBACK_THALASSA,
   ZARLINK_UNDEFINED
} ZARLINK_DEVICE_PROFILE;

/* Macro used when multiple devices of the same type are required - Note: physChanId is physical device channel */
#define SLICSLAC_LINE(deviceId,chansPerDev,physChanId) (deviceId*chansPerDev+physChanId)

typedef struct SLIC_CHANNEL_INFO
{
   /* Must be first element of this struct */
   const XDRV_SLIC_FUNCS*  pDrvFuncs;
   int                     pcmMode;       /* 16 bit linear, 8 bit u/A law, or wideband */
   int                     chan;          /* Physical SLIC channel, NOT endpt #, NOT line # */
   int                     globalId;      /* Global SLIC channel ID. Zero to seven on eight line product. */
   XDRV_BOOL               bDrvEnabled;   /* SLIC driver state (enabled/disabled) */
   int                     txTimeSlot;    /* pcm highway transmit timeslot */
   int                     rxTimeSlot;    /* pcm highway receive timeslot */
   void*                   pParent;       /* Pointer to parent device structure */
   VpLineCtxType*          pChanLineCtx;  /* Store Line context address */
   XDRV_BOOL               phaseReversalActive;   /* Phase Reversal Flag */
   XDRV_SLIC_MODE          slicMode;              /* Current Slic Mode */
   XDRV_BOOL               bOnBattery;            /* battery mode (true=on battery; false=on AC power) */
   XDRV_BOOL               bBoostedLoopCurrent;   /* Boosted Loop Current Flag */
   XDRV_BOOL               bTestLoad;             /* Test Load Flag */
   XDRV_BOOL               bEnhancedControl;      /* Enhanced control supported */
   int                     ringVoltage;           /* Current Ring Voltage */
   int                     ringOffset;            /* Current Ring DC Offset */
   int                     ringOffsetCal;         /* Current Ring DC Offset Calibration */
   int                     ringFrequency;         /* Current Ring Frequency */
   int                     ringWaveshape;         /* Current Ring Waveshape */

} SLIC_CHANNEL_INFO;

typedef struct DAA_CHANNEL_INFO
{
   /* Must be first element of this struct */
   const XDRV_DAA_FUNCS*   pDrvFuncs;
   int                     pcmMode;          /* 16 bit linear, 8 bit u/A law, or wideband */
   int                     chan;             /* Physical SLIC channel, NOT endpt #, NOT line # */
   XDRV_BOOL               bDrvEnabled;      /* SLIC driver state (enabled/disabled) */
   int                     txTimeSlot;       /* pcm highway transmit timeslot */
   int                     rxTimeSlot;       /* pcm highway receive timeslot */ 
   void*                   pParent;          /* Pointer to parent device structure */
   VpLineCtxType*          pDaaChanLineCtx;  /* Store Line context address */
} DAA_CHANNEL_INFO;

/* 
** Device Driver information
*/
typedef struct SLIC_DEVICE_DRV
{
   SLIC_CHANNEL_INFO       slicChannel[2];  /* At most, 2 for now */
   DAA_CHANNEL_INFO        daaChannel[1];   /* At most, 1 for now */

   /* Normally we reset the device and then start initialization. 
   ** However sometimes few devices might be connected to a same reset pin.
   ** In this case the board hal will need to reset only the first device.
   ** Set this value to TRUE if the device requires reset, FALSE otherwise */
   XDRV_BOOL               requiresReset; 

   /* Device id (0-based). Set to 0 if only one device exists on the board */
   int                     deviceId;

   int                     mspiDevId;             /* Device ID on MSPI bus  */    
   VRG_COUNTRY             locale;                /* locale */
   int                     deviceProfile;         /* Device Profile */
   VpDevCtxType*           pDeviceCtx;            /* Store Device Context address */

} SLIC_DEVICE_DRV;

typedef enum
{
   ZARLINK_RF50,
   ZARLINK_RF14
} ZARLINK_RFPROT;

typedef struct ZARLINK_CONFIG
{
   SLIC_DEVICE_DRV*              pDev;
   XDRV_UINT8                    numFxsPerDevice;
   XDRV_UINT8                    numFxoPerDevice;
   VpDeviceType                  deviceType;
   VpTermType                    fxsTermType;
   XDRV_UINT8                    fxsTermTypeSecondValid;  /* Set to '1' when valid */
   VpTermType                    fxsTermTypeSecond;  /* Optional: Used only when 2nd line term is different than 1st line */
   VpTermType                    fxoTermType; 
   VpDevCtxType*                 pDevCtx;
   void*                         pDevObj;
   VpLineCtxType*                pLineCtx;
   void*                         pLineObj;
   VpProfilePtrType              pDevProfile;
   VpProfilePtrType              pDcProfile;
   VpProfilePtrType              pRingProfile;   
   XDRV_UINT8                    rProt;
} ZARLINK_CONFIG;

/*
 *  Function declarations
 */
XDRV_BOOL zarlinkFxsLineIsOffhook( VpLineCtxType *pLineCtx );
void zarlinkFxsLineModeControl( VpLineCtxType *pLineCtx, XDRV_SLIC_MODE mode );
XDRV_BOOL zarlinkWaitForEvent( VpDevCtxType* pDevCtx, VpEventCategoryType category, XDRV_UINT16 event );
void zarlinkProcessFxsEvents( VpDevCtxType* pDevCtx, XDRV_UINT32 deviceChan );

XDRV_SLIC_STATUS initZarlinkDevice( ZARLINK_CONFIG* zarlinkConfig );
XDRV_SLIC_STATUS initZarlinkLines( ZARLINK_CONFIG* zarlinkConfig );
XDRV_SLIC_STATUS deinitZarlink( ZARLINK_CONFIG* zarlinkConfig );
                                  
void deviceReset( XDRV_UINT16 rstGpioPin );

XDRV_SINT16 slicZarlinkGetDlp( XDRV_SLIC *pDrv );
void slicZarlinkProcessEvents( XDRV_SLIC *pDrv );
XDRV_BOOL slicZarlinkIsOffhook( XDRV_SLIC *pDrv );
void slicZarlinkModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode );

void slicZarlinkLedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
void slicZarlinkPhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
int slicZarlinkSetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal );
int slicZarlinkGetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal );
int slicZarlinkSetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value );
int slicZarlinkSetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value );
XDRV_BOOL slicZarlinkGetOverCurrentStatus( XDRV_SLIC *pDrv );
XDRV_UINT32 slicZarlinkProbe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg, XDRV_UINT32 regSize,
                              void *dataPtr, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, XDRV_UINT8 set );

XDRV_BOOL daaZarlinkIsOffhook( XDRV_DAA *pDrv );
void daaZarlinkProcessEvents( XDRV_DAA *pDrv);
XDRV_BOOL daaZarlinkIsRingActive( XDRV_DAA *pDrv );
XDRV_DAA_LOOP daaZarlinkGetLoopCurrent( XDRV_DAA *pDrv );
void daaZarlinkModeControl( XDRV_DAA *pDrv, XDRV_DAA_MODE mode );
void daaZarlinkLedControl( XDRV_DAA *pDrv, int value );
XDRV_UINT32 daaZarlinkProbe( XDRV_DAA *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg, XDRV_UINT32 regSize,
                             void *dataPtr, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, XDRV_UINT8 set );
int slicZarlinkGetSlicParms( XDRV_SLIC *pDrv, int *phaseReversal, int *loopCurrent, int *powerSource, int *slicMode );
#ifdef __cplusplus
    }
#endif

#endif  /* ZARLINK_COMMON_SLIC_H  */
