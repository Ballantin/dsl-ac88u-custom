/***************************************************************************
*
*  Copyright 2000  Broadcom
*  All Rights Reserved
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*  All information contained in this document is Broadcom
*  company private, proprietary, and trade secret.
****************************************************************************/

#ifndef LHAPI_H
#define LHAPI_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>
#endif

#include <stddef.h>  /* Needed for size_t */

#include <hapi_dpr.h>
#include <hapi_rm.h>
#include <hapi_halshim.h>
#include <hapi_iocap.h>

typedef struct
{
   HALSHIM_MODE             halShimMode;
   HALSHIM_CONFIG_FLAG      halShimType;
   HALSHIM_SAMPLE_SIZE      halShimSampleSize;
   HALSHIM_PCM_COMP_MODE    halShimPcmCompMode;
   HALSHIM_PCM_CHANNELS     halShimPcmChannels;
   HALSHIM_PCM_CHANNEL_MUTE halShimPcmChannelMute;    
   HALSHIM_BYTE_ORDER       halShimByteOrder;
    
} LHAPI_HALSHIM_CONFIG;

/* Private data per halShim device */
typedef struct
{
   HALSHIM_CONFIG_FLAG      halShimType;           /* HAL device type */
   HALSHIM_SAMPLE_SIZE      halShimSampleSize;     /* Sample size on bits */
   HALSHIM_PCM_COMP_MODE    halShimPcmCompMode;    /* PCM compression mode (ALAW/ULAW) */
   HALSHIM_PCM_CHANNELS     halShimPcmChannels;    /* PCM channels per HAL device */
   HALSHIM_PCM_CHANNEL_MUTE halShimPcmChannelMute; /* Flag to determine whether to mute the PCM channel */   
   HALSHIM_BYTE_ORDER       halShimByteOrder;      /* Byte order of the device (little/big endian) */ 
   HAPI_UINT16              offset;                /* Offset in the DMA buffer */
 
} LHAPI_HALSHIM_PRIVATE;

int lhapiHalShimCfgDevices( LHAPI_HALSHIM_CONFIG *cfg, unsigned int numDevices );

void *lhapiIpcOpenChannel( HAPI_UINT16 vhdType, HAPI_UINT16 dspNum, HAPI_RM_OPEN_VHD_INFO *pVhdInfo );
void  lhapiIpcCloseChannel( void *ipcHandle );

unsigned lhapiIpcReadNoFree( void *ipcHandle, HAPI_UINT16 **ppBuf );
void     lhapiIpcFreeRead( void *ipcHandle );
unsigned lhapiIpcWrite( void *ipcHandle,
                        HAPI_UINT16 *pMsg1, unsigned msgLength1,
                        HAPI_UINT16 *pMsg2, unsigned msgLength2 );


void lhapiDprStatsInit( void );
void lhapiDprStatsGet( STATS_SET *statsp );
void lhapiDprStatsDiff( STATS_SET *seta, STATS_SET *setb );
void lhapiDprStatsPause( void );
void lhapiDprStatsUnpause( void );
void lhapiDprStatsSetPrescaler( HAPI_UINT16 prescale );
int  lhapiDprExclusiveExecStart( void );
void lhapiDprExclusiveExecEnd( int );


HAPI_UINT16 lhapiDcmGetProgramOverlayAddrHigh( void );
HAPI_UINT16 lhapiDcmGetProgramOverlayAddrLow( void );
HAPI_UINT16 lhapiDcmGetInstanceOverlayAddrHigh( void );
HAPI_UINT16 lhapiDcmGetInstanceOverlayAddrLow( void );


/*
**
*/
int lhapiDcmTransferFromInternalData
(
   void *pSrcBuf,
   unsigned destAddrHigh,
   unsigned destAddrLow,
   unsigned numWords
);

int lhapiDcmTransferFromInternalProgram
(
   void *pSrcBuf,
   unsigned destAddrHigh,
   unsigned destAddrLow,
   unsigned numWords
);

int lhapiDcmTransferToInternalData
(
   void *pDestBuf,
   unsigned srcAddrHigh,
   unsigned srcAddrLow,
   unsigned numWords
);

int lhapiDcmTransferToInternalProgram
(
   void *pDestBuf,
   unsigned srcAddrHigh,
   unsigned srcAddrLow,
   unsigned numWords
);


void lhapiProcessEntrySync( void );
void lhapiProcessEntryAsync( void );

/*
** Provide this for backward compatability
*/
#define lhapiProcessEntry()  lhapiProcessEntrySync()


void lhapiProcessIngress( void );
void lhapiProcessEgress( void );
void lhapiReenableInterrupt( void );

int lhapiTraceWrite( unsigned value );

unsigned int lhapiContextGet( void );

typedef void (*LHAPI_OFFLOAD_FUNC_PTR)( void *parm );

typedef struct
{
   LHAPI_OFFLOAD_FUNC_PTR funcPtr;
   void *parm;
 
} LHAPI_ITPC_OFFLOAD_STRUCT;

void lhapiKnlRunOffloadThreads( LHAPI_ITPC_OFFLOAD_STRUCT *offloadList, unsigned offloadCount, volatile int *offloadedActive );
void lhapiKnlOverrunCondition( unsigned int isOverrun );
void lhapiKnlScheduleReadyThreads( void );

int lhapiPveTransferHostToDspData
(
   HAPI_UINT16 dstaddr,
   HAPI_UINT16 srcaddrh,
   HAPI_UINT16 srcaddrl,
   size_t bytelen
);


HAPI_UINT32 lhapiPveGetPayloadBufAddr( HAPI_UINT16 vhdHandle );

void lhapiPveWriteHostPayload( HAPI_UINT16 *payloadp,      /* (i) address of destination payload buffer,
                                                              (o) because we write to the buffer */
                               HAPI_UINT16 *superpacketp,  /* (i) payload header for debug purposes, NULL if
                                                                  debugging is disabled. */
                               HAPI_UINT16 *superpayloadp, /* (i) payload data, not half-word aligned */
                               HAPI_UINT16 payloadSize,    /* (i) size of payload data, in bytes  */
                               HAPI_UINT16 payloadIdx );   /* (i) byte index for writing into payload buffer */

void lhapiIcpCalibrate( void );
void lhapiIcpStartIdle( void );
void lhapiIcpEndIdle( void );


int lhapiOsSemCreate( void **ppSem );
int lhapiOsSemGive( void *pSem );

int lhapiOsSemTake( void *pSem, int timeoutMs );
int lhapiOsSemReset( void *pSem );


HAPI_UINT16 lhapiRtpRand16(void);
int lhapiRtpTimeGet( HAPI_UINT32 *sec, HAPI_UINT32 *uSec );
void lhapiRtpTimeSet( HAPI_UINT32 sec, HAPI_UINT32 uSec );

unsigned int lhapiGetCpu( void );
void lhapiPutCpu( void );

/*---------------------------------------------------------------------------
** Nested Support for Interrupt enable and disable
**
** Using the direct function call will produce a smaller code size.
** Calling the Lock and Unlock indirectly through the function pointers
** allows us to jump directly to the os function (e.g., intLock).  A two
** step call to intLock (i.e., lhapiBiosLock->intLock) would likely result
** in an extra cache miss.  Since these functions are called frequently
** we want to avoid cache missed.
*/
typedef  void *HSZBIOSSTATE;

HSZBIOSSTATE lhapiBiosLock( void );
void lhapiBiosUnlock( HSZBIOSSTATE state );

typedef  HSZBIOSSTATE (*LHAPI_FNCP_LOCK )( void );
typedef  void (*LHAPI_FNCP_UNLOCK)( HSZBIOSSTATE  );

extern LHAPI_FNCP_LOCK   LHAPI_gpFncLock;
extern LHAPI_FNCP_UNLOCK LHAPI_gpFncUnlock;

extern LHAPI_FNCP_LOCK   LHAPI_gpFncLockSMP;
extern LHAPI_FNCP_UNLOCK LHAPI_gpFncUnlockSMP;

#include <stdarg.h>

void lhapiErrorPrint(const char *format, ...);
void lhapiAssertHandler( void );

void lhapiSetHptTimerFreq(unsigned int cpuSpeed);

#endif /* LHAPI_H */
