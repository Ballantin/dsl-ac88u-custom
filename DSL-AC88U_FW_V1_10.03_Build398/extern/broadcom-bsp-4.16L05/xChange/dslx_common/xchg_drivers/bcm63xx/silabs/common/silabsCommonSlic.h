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
*      This is common code for all SiLabs SLIC devices, that uses ProSLIC API.
*
***************************************************************************/
#ifndef SILABS_COMMON_SLIC_H
#define SILABS_COMMON_SLIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <xdrvTypes.h>
#include <xdrvSlic.h>
#include <vrgCountry.h>
#include <xdrvDaa.h>
#include <bosMutex.h>
#include <linux/timer.h>
#include "proslic.h"
#include "proslic_rtp.h"

#define SI_MAX_NUM_DEVICES   2
#define SI_MAX_NUM_LINES     4

/* Common CodecTypes */
#define SILABS_LINEAR       1
#define SILABS_ULAW         2
#define SILABS_ALAW         3
#define SILABS_WIDEBAND     4

#define LF_MASK             0x07
#define LFS_BIT_OFFSET      4

typedef enum
{
   SILABS_FLYBACK,
   SILABS_BUCKBOOST,
   SILABS_QCUK,
   SILABS_FIXEDRAIL,
   SILABS_FB_TSS,
   SILABS_FB_TSS_ISO,
   SILABS_PMOS_BUCK_BOOST,
   SILABS_LCQCUK,
   SILABS_UNDEFINED
} SILABS_DEVICE_PROFILE;

/* Macro used when multiple devices of the same type are required - Note: physChanId is physical device channel */
#define SI_SLICSLAC_LINE(deviceId,chansPerDev,physChanId) (deviceId*chansPerDev+physChanId)

typedef struct SI_SLIC_CHANNEL_INFO
{
   /* Must be first element of this struct */
   const XDRV_SLIC_FUNCS*  pDrvFuncs;
   int                     pcmMode;       /* 16 bit linear, 8 bit u/A law, or wideband */
   int                     chan;          /* Physical SLIC channel, NOT endpt #, NOT line # */
   XDRV_BOOL               bDrvEnabled;   /* SLIC driver state (enabled/disabled) */
   int                     txTimeSlot;    /* pcm highway transmit timeslot */
   int                     rxTimeSlot;    /* pcm highway receive timeslot */
   void*                   pParent;       /* Pointer to parent device structure */
   SiVoiceChanType_ptr     pProSlicChan;  /* Store ProSlic Channel Pointer address */
   XDRV_UINT8              phaseReversalActive;   /* Phase Reversal Flag */
   int                     ringBurstBlockFlag; /* while block ring mode flag */
   struct timer_list       ringtripTimer;       /* the timer to block ring mode */ 
   int                     lastOffhookStatus;   /* the flag to change mode to lcf once when offhook */
   int                     previousSlicMode;/* last time the slic mode set */
   BOS_MUTEX               silabsEquCriMutex;
   BOS_MUTEX               silabsPolRevMutex; 
} SI_SLIC_CHANNEL_INFO;

typedef struct SI_DAA_CHANNEL_INFO
{
   /* Must be first element of this struct */
   const XDRV_DAA_FUNCS*   pDrvFuncs;
   int                     pcmMode;          /* 16 bit linear, 8 bit u/A law, or wideband */
   int                     chan;             /* Physical SLIC channel, NOT endpt #, NOT line # */
   XDRV_BOOL               bDrvEnabled;      /* SLIC driver state (enabled/disabled) */
   int                     txTimeSlot;       /* pcm highway transmit timeslot */
   int                     rxTimeSlot;       /* pcm highway receive timeslot */ 
   void*                   pParent;          /* Pointer to parent device structure */
   SiVoiceChanType_ptr     pDaaProSlicChan;  /* Store ProSlic Channel Pointer address */
} SI_DAA_CHANNEL_INFO;

/* 
** Device Driver information
*/
typedef struct SI_SLIC_DEVICE_DRV
{
   SI_SLIC_CHANNEL_INFO       slicChannel[2];  /* At most, 2 for now */
   SI_DAA_CHANNEL_INFO        daaChannel[1];   /* At most, 1 for now */

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
   
} SI_SLIC_DEVICE_DRV;

typedef struct{
   int portID;
} ctrl_SiLabs;

typedef struct SILABS_CONFIG
{
   SI_SLIC_DEVICE_DRV*           pDev;
   XDRV_UINT8                    numFxsPerDevice;
   XDRV_UINT8                    numFxoPerDevice;
   ctrl_SiLabs                   *spiObj;
   int*                          timerObj;
   SiVoiceChanType_ptr           *voiceChans;
   SiVoiceDeviceType*            *voiceDevices;
   SiVoiceControlInterfaceType*  *voiceHWIntf;
   XDRV_UINT32                   chipType;
   ctrl_WriteRegister_fptr       writeRegister_fptr;
   ctrl_ReadRegister_fptr        readRegister_fptr;
   ctrl_WriteRAM_fptr            writeRAM_fptr;
   ctrl_ReadRAM_fptr             readRAM_fptr;
   XDRV_UINT8                    bomType;   
} SILABS_CONFIG;

/*
 *  Function declarations
 */

XDRV_SLIC_STATUS initSiLabsDevice( SILABS_CONFIG* silabsConfig );
XDRV_SLIC_STATUS initSiLabsLines( SILABS_CONFIG* silabsConfig );
XDRV_SLIC_STATUS deinitSiLabs( SILABS_CONFIG* silabsConfig );
                                  
void deviceSiLabsReset( XDRV_UINT16 rstGpioPin );

XDRV_SINT16 slicSiLabsGetDlp( XDRV_SLIC *pDrv );
void slicSiLabsProcessEvents( XDRV_SLIC *pDrv );
XDRV_BOOL slicSiLabsIsOffhook( XDRV_SLIC *pDrv );
void slicSiLabsModeControl( XDRV_SLIC *pDrv, XDRV_SLIC_MODE mode );

void slicSiLabsLedControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
void slicSiLabsPhaseReversalControl( XDRV_SLIC *pDrv, XDRV_UINT16 value );
int slicSiLabsSetRingParms( XDRV_SLIC *pDrv, int ringFrequency, int ringWaveshape,
                                      int ringVoltage, int ringOffset, int ringOffsetCal );
int slicSiLabsGetRingParms( XDRV_SLIC *pDrv, int *ringFrequency, int *ringWaveshape,
                                      int *ringVoltage, int *ringOffset, int *ringOffsetCal );
int slicSiLabsSetBoostedLoopCurrent( XDRV_SLIC *pDrv, XDRV_UINT16 value );
int slicSiLabsSetPowerSource( XDRV_SLIC *pDrv, XDRV_UINT16 value );
XDRV_BOOL slicSiLabsGetOverCurrentStatus( XDRV_SLIC *pDrv );
XDRV_UINT32 slicSiLabsProbe( XDRV_SLIC *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg, XDRV_UINT32 regSize,
                             void *dataPtr, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, XDRV_UINT8 set );

XDRV_BOOL daaSiLabsIsOffhook( XDRV_DAA *pDrv );
void daaSiLabsProcessEvents( XDRV_DAA *pDrv);
XDRV_BOOL daaSiLabsIsRingActive( XDRV_DAA *pDrv );
XDRV_DAA_LOOP daaSiLabsGetLoopCurrent( XDRV_DAA *pDrv );
void daaSiLabsModeControl( XDRV_DAA *pDrv, XDRV_DAA_MODE mode );
void daaSiLabsLedControl( XDRV_DAA *pDrv, int value );
XDRV_UINT32 daaSiLabsProbe( XDRV_DAA *pDrv, XDRV_UINT32 deviceId, XDRV_UINT32 chan, XDRV_UINT32 reg, XDRV_UINT32 regSize,
                            void *dataPtr, XDRV_UINT32 valueSize, XDRV_UINT32 indirect, XDRV_UINT8 set );
int slicSiLabsGetSlicParms( XDRV_SLIC *pDrv, int *phaseReversal, int *loopCurrent, int *powerSource, int *slicMode );
#ifdef __cplusplus
    }
#endif

#endif  /* SILABS_COMMON_SLIC_H  */
