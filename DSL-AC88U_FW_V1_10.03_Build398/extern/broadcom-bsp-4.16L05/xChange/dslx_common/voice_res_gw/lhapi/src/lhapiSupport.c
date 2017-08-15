/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: lhapiSupport.c
*
****************************************************************************
*    Description:
*
*     Implements the "Low-level HAPI" API that is required to provide
*     common application supports (such as semas, error logging) to LDX
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <lhapi.h>
#include <lhapiExtension.h>

#if FEAT_HAL_CAPTURE_TO_FILE
#include <generated/autoconf.h>
#include <bosFile.h>
#include <bosMutex.h>
#include <linux/slab.h>

#ifndef BRCM_NONDIST_BUILD
#include <xdrvItpc.h>
#include <itpcSharedCmds.h>
#endif /* BRCM_NONDIST_BUILD */
#endif /* FEAT_HAL_CAPTURE_TO_FILE */

#include <vrgLog.h>
#include <vrgTypes.h>
#include <stdio.h>
#include <bosSem.h>
#include <xchgAssert.h>
#include <bosCritSect.h>
#include <xdrvGlobalInterrupt.h>
#include <xdrvCircBuf.h>
#include "lhapiPrivate.h"
#include <str.h>

#include <bosSpinlock.h>
#include <bosMisc.h>
#include <bosSleep.h>

#include <boardparms_voice.h>
#include <boardHalApm.h>
#include <boardHalProv.h>

/* ---- Private Constants and Types -------------------------------------- */
#define LHAPI_ERROR_BUFFER_SIZE         256

#if FEAT_HAL_CAPTURE_TO_FILE

#define LHAPI_HALCAP_TASK_NAME            "HalCap"
#define LHAPI_HALCAP_TASK_STACKSIZE       ( 1024 * 8 )

#ifndef LHAPI_HALCAP_TASK_PRIORITY
#define LHAPI_HALCAP_TASK_PRIORITY        BOS_TASK_CLASS_MED_HIGH
#endif

#define EG_FILE_PREFIX     "/mnt/usb1_1/egCap"
#define ING_FILE_PREFIX    "/mnt/usb1_1/ingCap"

typedef struct {
   BOS_FILE * filepEg;
   BOS_FILE * filepIng;
   HAL_CAP_INFO halCapInfo;
} HAL_CAP_INFO_BLK;

#endif /* FEAT_HAL_CAPTURE_TO_FILE */

/* ---- Public Variables ------------------------------------------------- */
#if FEAT_HAL_CAPTURE_TO_FILE

HAL_CAP_INFO_BLK * halCapInfoBlkp[HAL_CAP_MAX_CHAN] = { NULL };

#endif /* FEAT_HAL_CAPTURE_TO_FILE */

/* ---- Private Variables ------------------------------------------------- */
#if FEAT_HAL_CAPTURE_TO_FILE

/* Capture task and timer objects. */
static BOS_TASK_ID      gHalCapTaskId = 0;
static BOS_SEM          gThreadHalCapSem;

static BOS_MUTEX  gHalCapBosMutex;
static BOS_BOOL   gHalCapInitialized = BOS_FALSE;
char tempBuff[ (HAL_CAP_NUM_SAMPLES * sizeof( unsigned short)) ];

#ifndef BRCM_NONDIST_BUILD
extern XDRV_ITPC gHalItpcDrv;
#endif /* BRCM_NONDIST_BUILD */

#endif /* FEAT_HAL_CAPTURE_TO_FILE */

/* ---- Private Function Prototypes -------------------------------------- */
#if FEAT_HAL_CAPTURE_TO_FILE

#ifdef BRCM_NONDIST_BUILD
extern void halCaptureSetup( HAL_CAP_INFO * info );
extern void halCaptureDeinit( void );
#endif /* BRCM_NONDIST_BUILD */
static void writeCapDataToFile( int chan );
static void lhapiHalCaptureThread( BOS_TASK_ARG taskArg );
static void lhapiHalCaptureThreadDeinitCB( BOS_TASK_ARG taskArg );
static void lhapiHalCaptureThreadInitCB( BOS_TASK_ARG taskArg );
static void lhapiHalCaptureThreadInit( void );
static void lhapiFlushtoFile( void );
#endif /* FEAT_HAL_CAPTURE_TO_FILE */

static HSZBIOSSTATE CritSectEnter( void );
static void CritSectLeave( HSZBIOSSTATE state );
static HSZBIOSSTATE CritSectEnterSMP( void );
static void CritSectLeaveSMP( HSZBIOSSTATE state );
static HSZBIOSSTATE SpinlockAcquire( void );
static void SpinlockRelease( HSZBIOSSTATE flags );

/* ---- Functions -------------------------------------------------------- */

/* Map hausware functions to BOS wrapper functions */
LHAPI_FNCP_LOCK      LHAPI_gpFncLock;
LHAPI_FNCP_UNLOCK    LHAPI_gpFncUnlock;
LHAPI_FNCP_LOCK      LHAPI_gpFncLockSMP;
LHAPI_FNCP_UNLOCK    LHAPI_gpFncUnlockSMP;


static BOS_CRITSECT  gCritSect;
static char gHapiErrorBuffer[LHAPI_ERROR_BUFFER_SIZE + 1];

static BOS_SPINLOCK gSpinLock;

/*
*****************************************************************************
** FUNCTION:   lhapiCritSecInit
**
** PURPOSE:    Init lhapi critical section primitive.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiCritSecInit( void )
{

   /*
   ** The semantics of the lhapi "critical section" depend on the hausware
   ** configuration.
   **
   ** Host-only (non-distributed) configurations require interrupts to be
   ** disabled/restored. This is necessary because digital samples are moved
   ** to/from hausware ingress/egress HAL buffers in ISR context. Therefore,
   ** interrupts need to be disabled when hausware threads read/write to
   ** the buffers/queues.
   **
   ** Distributed configurations don't necessarily need to use interrupt
   ** disables/enables, since hausware is run in task context on the host,
   ** and digital samples are moved to/from hausware on the DSP.
   */
   if ( lhapiIsHauswareDistributed() )
   {
      /* Create the BOS critical section primitive. */
      bosCritSectCreate( &gCritSect );

      /* Map the lhapi lock functions. */
      LHAPI_gpFncLock   = (LHAPI_FNCP_LOCK)     CritSectEnter;
      LHAPI_gpFncUnlock = (LHAPI_FNCP_UNLOCK)   CritSectLeave;
      LHAPI_gpFncLockSMP   = (LHAPI_FNCP_LOCK)    CritSectEnterSMP;
      LHAPI_gpFncUnlockSMP = (LHAPI_FNCP_UNLOCK)  CritSectLeaveSMP;

      /* We have to do some nasty casting when locking and unlocking lhapi.
      ** Make sure that we are casting between types of the same size, otherwise
      ** we might be asking for trouble. */
      XCHG_ASSERT_COND( sizeof( BOS_CRITSECT ), ==, sizeof( HSZBIOSSTATE ) );
   }
   else
   {
      bosSpinlockCreate( &gSpinLock );

      /* Map the lhapi lock functions. */
      LHAPI_gpFncLock   = (LHAPI_FNCP_LOCK)     SpinlockAcquire;
      LHAPI_gpFncUnlock = (LHAPI_FNCP_UNLOCK)   SpinlockRelease;
      LHAPI_gpFncLockSMP   = (LHAPI_FNCP_LOCK)    CritSectEnterSMP;
      LHAPI_gpFncUnlockSMP = (LHAPI_FNCP_UNLOCK)  CritSectLeaveSMP;

      /* We have to do some nasty casting when locking and unlocking lhapi.
      ** Make sure that we are casting between types of the same size, otherwise
      ** we might be asking for trouble. */
      XCHG_ASSERT_COND( sizeof( unsigned long ), ==, sizeof( HSZBIOSSTATE ) );
   }
}


/*
*****************************************************************************
** FUNCTION:   lhapiCritSecDeinit
**
** PURPOSE:    De-initialize lhapi critical section primitive.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiCritSecDeinit( void )
{
   if ( lhapiIsHauswareDistributed() )
   {
      /* Create the BOS critical section primitive. */
      bosCritSectDestroy( &gCritSect );
   }
   else
   {
      bosSpinlockDestroy( &gSpinLock );
   }
}


/*
*****************************************************************************
** FUNCTION:   CritSectEnter
**
** PURPOSE:    Enter critical section
**
** PARAMETERS: none
**
** RETURNS:    HSZBIOSTATE (not used - returned for consistency)
**
*****************************************************************************
*/
static HSZBIOSSTATE CritSectEnter( void )
{
   bosCritSectEnter( &gCritSect );

   return 0;
}


/*
*****************************************************************************
** FUNCTION:   CritSectLeave
**
** PURPOSE:    Enter critical section
**
** PARAMETERS: state - BIOS state
**
** RETURNS:    none
**
*****************************************************************************
*/
static void CritSectLeave( HSZBIOSSTATE state )
{
   (void)state;

   bosCritSectLeave( &gCritSect );
}

/*
*****************************************************************************
** FUNCTION:   CritSectEnterSMP
**
** PURPOSE:    Enter critical section. This function locks only in 
**             non-distributed configurations with SMP enabled.
**
** PARAMETERS: none
**
** RETURNS:    HSZBIOSTATE (not used - returned for consistency)
**
*****************************************************************************
*/
static HSZBIOSSTATE CritSectEnterSMP( void )
{
#ifdef CONFIG_SMP
   return SpinlockAcquire();
#endif /* CONFIG_SMP */

   return 0;
}


/*
*****************************************************************************
** FUNCTION:   CritSectLeaveSMP
**
** PURPOSE:    Enter critical section. This function unlocks only in 
**             non-distributed configurations with SMP enabled.
**
** PARAMETERS: state - BIOS state
**
** RETURNS:    none
**
*****************************************************************************
*/
static void CritSectLeaveSMP( HSZBIOSSTATE state )
{
#ifdef CONFIG_SMP
   SpinlockRelease(state);
#endif /* CONFIG_SMP */
}

/*
*****************************************************************************
** FUNCTION:   SpinlockAcquire
**
** PURPOSE:    Acquire the lhapi spinlock.
**
** PARAMETERS: None.
**
** RETURNS:    HSZBIOSTATE (not used - returned for consistency)
**
*****************************************************************************
*/
static HSZBIOSSTATE SpinlockAcquire( void )
{
   unsigned long flags;

   bosSpinlockAcquire( &gSpinLock, &flags );

   return ( (HSZBIOSSTATE) flags );
}


/*
*****************************************************************************
** FUNCTION:   SpinlockRelease
**
** PURPOSE:    Release the lhapi spinlock.
**
** PARAMETERS: flags - CPU flags
**
** RETURNS:    none
**
*****************************************************************************
*/
static void SpinlockRelease( HSZBIOSSTATE flags )
{
   bosSpinlockRelease( &gSpinLock, (unsigned long)flags );
}


/*
*****************************************************************************
** FUNCTION:   lhapiErrorPrint
**
** PURPOSE:    Print error
**
** PARAMETERS: string to print
**
** RETURNS:    0 on success
**
*****************************************************************************
*/
void lhapiErrorPrint(const char *format, ...)
{
   va_list argList;
   va_start( argList, format );
   memset( gHapiErrorBuffer, 0, LHAPI_ERROR_BUFFER_SIZE + 1 );
   vStrPrintf( gHapiErrorBuffer, LHAPI_ERROR_BUFFER_SIZE,
               format, argList );
   va_end( argList );
   VRG_LOG_ERROR((VRG_LOG_MOD_LHAPI, "%s", gHapiErrorBuffer ));
}


/*
*****************************************************************************
** FUNCTION:   lhapiKnlOverrunCondition
**
** PURPOSE:    Determines if thread-processor load balancing is required, and
**             if so notifies the primary thread processor.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void lhapiKnlOverrunCondition( unsigned int isOverrun )
{
   /*
   ** Nothing to do on the host processor.
   */
}


/*
*****************************************************************************
** FUNCTION:   lhapiGetCpu
**
** PURPOSE:    Gets the CPU on which this code is currently executing.
**
** PARAMETERS: None.
**
** RETURNS:    Current CPU.
**
** NOTE:
*****************************************************************************
*/
unsigned int lhapiGetCpu()
{
   unsigned int cpu;

   bosGetCpu(&cpu);
   
   return cpu;
}

/*
*****************************************************************************
** FUNCTION:   lhapiPutCpu
**
** PURPOSE:    Cleanup function for lhapiGetCpu.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void lhapiPutCpu()
{
   bosPutCpu();
}

#if FEAT_HAL_CAPTURE_TO_FILE

static char * lhapiHalCaptureAlloc ( int size )
{
   return( (char *)kmalloc( size, GFP_KERNEL ) );
}

static void lhapiHalCaptureFree ( char * addr )
{
   kfree( addr );
}

/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureInit
**
** PURPOSE:    Sets up the HAL capture block for a specific channel
**
** PARAMETERS: chan - Hal index to capture from
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiHalCaptureInit( int chan )
{   
   /* CREATE MUTEX + LAUNCH THREADS */
   if( !gHalCapInitialized )
   {
      bosMutexCreate( "halCap-mutex", &gHalCapBosMutex );
      
      /* Create capture thread if it has not been created before */
      if( !gHalCapTaskId )
      {
         lhapiHalCaptureThreadInit();
      }
      
      VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureInit: Created halCap-mutex\n"));
   }   
   
   /* TAKE MUTEX */   
   if ( bosMutexAcquire( &gHalCapBosMutex ) != BOS_STATUS_OK )
   {
      VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureInit: Error aquiring halCap-mutex\n"));
   }
   
   /* Setup capture info block if not already initialized */
   if( !halCapInfoBlkp[chan] )
   {         
      /* Allocate CAPTURE_INFO struct + eg/ing buff*/
      halCapInfoBlkp[chan] = (HAL_CAP_INFO_BLK *)kmalloc( sizeof(HAL_CAP_INFO_BLK) + 2 * ( sizeof( unsigned short) * HAL_CAP_NUM_SAMPLES )
                                                          , GFP_KERNEL );
                                                          
      memset( (char*)halCapInfoBlkp[chan], 0, (sizeof(HAL_CAP_INFO_BLK) + 2*( sizeof( unsigned short) * HAL_CAP_NUM_SAMPLES )) );
      halCapInfoBlkp[chan]->halCapInfo.chan = chan;
      
      /* Allocate file descriptor */
      halCapInfoBlkp[chan]->filepEg = (BOS_FILE*)kmalloc( sizeof(BOS_FILE), GFP_KERNEL );
      halCapInfoBlkp[chan]->filepIng = (BOS_FILE*)kmalloc( sizeof(BOS_FILE), GFP_KERNEL );
      memset( (char*)halCapInfoBlkp[chan]->filepEg, 0, sizeof(BOS_FILE) );
      memset( (char*)halCapInfoBlkp[chan]->filepIng, 0, sizeof(BOS_FILE) );
      
      /* Create Circbuffs over here */
      circBufCreate( (XDRV_CIRCBUF_HDL*)&halCapInfoBlkp[chan]->halCapInfo.egressCircBufHdl,  
                     (sizeof( unsigned short) * HAL_CAP_NUM_SAMPLES),
                     XDRV_TRUE,
                     (XDRV_CIRCBUF_ALLOCMEM_FP)&lhapiHalCaptureAlloc,
                     (XDRV_CIRCBUF_FREEMEM_FP)&lhapiHalCaptureFree );
                     
      circBufCreate( (XDRV_CIRCBUF_HDL*)&halCapInfoBlkp[chan]->halCapInfo.ingressCircBufHdl,
                     (sizeof( unsigned short) * HAL_CAP_NUM_SAMPLES),
                     XDRV_TRUE,
                     (XDRV_CIRCBUF_ALLOCMEM_FP)&lhapiHalCaptureAlloc,
                     (XDRV_CIRCBUF_FREEMEM_FP)&lhapiHalCaptureFree );
            
      XCHG_ASSERT_COND( (int)halCapInfoBlkp[chan]->halCapInfo.egressCircBufHdl, !=, (int)NULL );
      XCHG_ASSERT_COND( (int)halCapInfoBlkp[chan]->halCapInfo.ingressCircBufHdl, !=, (int)NULL );
   
      /* Send pointer to CAPTURE_INFO struct to DSP/LIB */
   #ifdef BRCM_NONDIST_BUILD
      halCaptureSetup( &halCapInfoBlkp[chan]->halCapInfo );
   #else
      /* Send ITPC */
      xdrvItpcSendSyncCommand( &gHalItpcDrv, ITPC_SET_HALCAP_INFO_CMD_ID,(XDRV_ITPC_DATA)&(halCapInfoBlkp[chan]->halCapInfo), 0 );     
   #endif /* BRCM_NONDIST_BUILD  */
   
      VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureInit: chan %d %d capifno 0x%08x ingbuf 0x%08x egbuf 0x%08x \n"  
                                                                        , chan 
                                                                        , sizeof(HAL_CAP_INFO_BLK) 
                                                                        , (unsigned int)halCapInfoBlkp[chan]  
                                                                        , (unsigned int)halCapInfoBlkp[chan]->halCapInfo.ingressCircBufHdl
                                                                        , (unsigned int)halCapInfoBlkp[chan]->halCapInfo.egressCircBufHdl));
                                                                        
      gHalCapInitialized = BOS_TRUE;
   }
   
   /* RELEASE MUTEX */
   if ( bosMutexRelease( &gHalCapBosMutex ) != BOS_STATUS_OK )
   {
       VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureInit: Error releasing halCap-mutex\n"));
   }
}

/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureStart
**
** PURPOSE:    Starts hal capture on all captureblocks
**
** PARAMETERS: arg is ignored.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiHalCaptureStart( void )
{      
   /* Set enabled flag for all allocated CAPTURE_INFO blocks */
   int i;
   char tempEgFileName[ BOS_FILE_NAME_STR_LEN ];
   char tempIngFileName[ BOS_FILE_NAME_STR_LEN ];   
   
   if( gHalCapInitialized )
   {
      for( i = 0; i < HAL_CAP_MAX_CHAN; i++ )
      {
         /* TAKE MUTEX */
         if ( bosMutexAcquire( &gHalCapBosMutex ) != BOS_STATUS_OK )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureStart: Error aquiring halCap-mutex\n"));
         }
               
         if( halCapInfoBlkp[i] && halCapInfoBlkp[i]->filepEg && halCapInfoBlkp[i]->filepIng && !halCapInfoBlkp[i]->halCapInfo.enabled )
         {         
            /* Open Files */   
            VOICE_BOARD_PARMS* voiceParams = boardHalProvGetVoiceParms(); 
            if( ( boardHalApmGetChanSwapStatus() == XDRV_TRUE ) && 
                ( voiceParams->flags == BP_FLAG_DSP_APMHAL_ENABLE ) && 
                ( i < boardHalGetNumFxsEndpoints() ) )
            {
               snprintf(tempEgFileName,  BOS_FILE_NAME_STR_LEN, "%s%d.raw", EG_FILE_PREFIX,  boardHalGetNumFxsEndpoints()-1 - i );
               snprintf(tempIngFileName, BOS_FILE_NAME_STR_LEN, "%s%d.raw", ING_FILE_PREFIX, boardHalGetNumFxsEndpoints()-1 - i );
            }
            else
            {
               snprintf(tempEgFileName,  BOS_FILE_NAME_STR_LEN, "%s%d.raw", EG_FILE_PREFIX,  i );
               snprintf(tempIngFileName, BOS_FILE_NAME_STR_LEN, "%s%d.raw", ING_FILE_PREFIX, i );
            }
            
            if( bosFileOpen( halCapInfoBlkp[i]->filepEg, tempEgFileName, BOS_FILE_FLAG_RDWR | BOS_FILE_FLAG_CREAT ) == BOS_STATUS_ERR )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureStart: Error opening file %s \n", tempEgFileName));            
               return;
            }
            
            if( bosFileOpen( halCapInfoBlkp[i]->filepIng, tempIngFileName, BOS_FILE_FLAG_RDWR | BOS_FILE_FLAG_CREAT ) == BOS_STATUS_ERR )
            {
               VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureStart: Error opening file %s \n", tempIngFileName));            
               return;
            }
            
                     
            VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureStart: chan %d file %s %s \n", i, tempEgFileName, tempIngFileName));
   
            /* Set enable flag */
            halCapInfoBlkp[i]->halCapInfo.enabled = 1;
         }  
         
         /* RELEASE MUTEX */
         if ( bosMutexRelease( &gHalCapBosMutex ) != BOS_STATUS_OK )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureStart: Error releasing halCap-mutex\n"));
         }          
      }
   }

}


/*
*****************************************************************************
** FUNCTION:   lhapiFlushtoFile
**
** PURPOSE:    Flushes captured hal data to a file
**
** PARAMETERS: arg is ignored.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void lhapiFlushtoFile( void )
{   
   int i;
   
   if( gHalCapInitialized )
   {
      for( i = 0; i < HAL_CAP_MAX_CHAN; i++ )
      {      
         /* TAKE MUTEX */
         if ( bosMutexAcquire( &gHalCapBosMutex ) != BOS_STATUS_OK )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiFlushtoFile: Error aquiring halCap-mutex\n"));
         }
         
         if( halCapInfoBlkp[i] && halCapInfoBlkp[i]->halCapInfo.enabled )
         {
            writeCapDataToFile( i );
         }
            
         /* RELEASE MUTEX */
         if ( bosMutexRelease( &gHalCapBosMutex ) != BOS_STATUS_OK )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiFlushtoFile: Error releasing halCap-mutex\n"));
         }                           
      }
   }   
}

/*
*****************************************************************************
** FUNCTION:   writeCapDataToFile
**
** PURPOSE:    Writes captured data for particular channel to file
**
** PARAMETERS: chan - Channel to write to file.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void writeCapDataToFile( int chan )
{  
   int numWritten = 0;
   int bytesRead = 0;
         
   if (  halCapInfoBlkp[chan] && halCapInfoBlkp[chan]->filepEg && halCapInfoBlkp[chan]->filepIng )
   {  
   
      /**********************
       *   EGRESS CAPTURE   *
       **********************/
      bytesRead = 0;
      bytesRead = circBufRead( (XDRV_CIRCBUF_HDL*)&halCapInfoBlkp[chan]->halCapInfo.egressCircBufHdl, &tempBuff[0], (HAL_CAP_NUM_SAMPLES * sizeof( unsigned short)) );
            
      if( bytesRead )
      {      
         numWritten = bosFileWrite( halCapInfoBlkp[chan]->filepEg, &tempBuff[0], bytesRead );               
      }
                                         
      if( numWritten < 0 )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "writeCapDataToFile: Error writing to file %s \n", halCapInfoBlkp[chan]->filepEg->fileName));       
         return;                 
      }     
                                           
      /**********************
       *   INGRESS CAPTURE  *
       **********************/
      bytesRead = 0;
      bytesRead = circBufRead( (XDRV_CIRCBUF_HDL*)&halCapInfoBlkp[chan]->halCapInfo.ingressCircBufHdl, &tempBuff[0], (HAL_CAP_NUM_SAMPLES * sizeof( unsigned short)) );
      
      if( bytesRead )
      {
         numWritten = bosFileWrite( halCapInfoBlkp[chan]->filepIng, &tempBuff[0], bytesRead );               
      }
      
      if( numWritten < 0 )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "writeCapDataToFile: Error writing to file %s \n", halCapInfoBlkp[chan]->filepIng->fileName));       
         return;                 
      }        
   }
   else
   {
      /* Print error */
   }
      
}

/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureStop
**
** PURPOSE:    Stops all hal captures and closes all open files
**
** PARAMETERS: arg is ignored.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiHalCaptureStop( void )
{
   int i;
   
   if( gHalCapInitialized )
   {  
      for( i = 0; i < HAL_CAP_MAX_CHAN; i++ )
      {
         /* TAKE MUTEX */
         if ( bosMutexAcquire( &gHalCapBosMutex ) != BOS_STATUS_OK )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureStop: Error aquiring halCap-mutex\n"));
         }
         
         if(  halCapInfoBlkp[i] &&  halCapInfoBlkp[i]->halCapInfo.enabled )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureStop: chan %d \n", i));
            halCapInfoBlkp[i]->halCapInfo.enabled = 0;
            bosFileClose( halCapInfoBlkp[i]->filepEg );
            bosFileClose( halCapInfoBlkp[i]->filepIng );
            VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureStop: chan %d \n", i));
         }      
         
         /* RELEASE MUTEX */
         if ( bosMutexRelease( &gHalCapBosMutex ) != BOS_STATUS_OK )
         {
            VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureStop: Error releasing halCap-mutex\n"));
         }
   
      }   
   }
}

/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureDeInit
**
** PURPOSE:    Destroys all capture blocks
**
** PARAMETERS: arg is ignored.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiHalCaptureDeInit( void )
{
   /* Loop through CAPTURE_INFO pointer array and de-allocate all memory */
   /* Call this function at endpt shut down */
   int i;
  
   if( gHalCapInitialized )
   {
      /* Set state as disabled */
      gHalCapInitialized = BOS_FALSE;
      
      /* Send disable command to DSP */
      #ifdef BRCM_NONDIST_BUILD
         halCaptureDeinit();   
      #else
         /* Send ITPC */
         xdrvItpcSendSyncCommand( &gHalItpcDrv, ITPC_SET_HALCAP_INFO_CMD_ID,(XDRV_ITPC_DATA)(NULL), 0 );     
      #endif /* BRCM_NONDIST_BUILD  */
      
      /* Wait For dsp to de-init capture */
      bosSleep( 100 );
        
      /* TAKE MUTEX */
      if ( bosMutexAcquire( &gHalCapBosMutex ) != BOS_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureDeInit: Error aquiring halCap-mutex\n"));
      }
      
      /* Free all allocated memory */
      for( i = 0; i < HAL_CAP_MAX_CHAN; i++ )
      {
         if(  halCapInfoBlkp[i] )
         {
            circBufDestroy( (XDRV_CIRCBUF_HDL*)&halCapInfoBlkp[i]->halCapInfo.egressCircBufHdl );
            circBufDestroy( (XDRV_CIRCBUF_HDL*)&halCapInfoBlkp[i]->halCapInfo.ingressCircBufHdl );
            
            kfree( halCapInfoBlkp[i]->filepEg );
            kfree( halCapInfoBlkp[i]->filepIng );
            kfree( halCapInfoBlkp[i] );         
   
            halCapInfoBlkp[i] = NULL;
            VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureDeInit: chan %d \n", i));
         }
      }
      
      /* RELEASE MUTEX */
      if ( bosMutexRelease( &gHalCapBosMutex ) != BOS_STATUS_OK )
      {
         VRG_LOG_ERROR(( VRG_LOG_MOD_LHAPI, "lhapiHalCaptureDeInit: Error releasing halCap-mutex\n"));
      }
              
      /* DESTROY MUTEX */
      bosMutexDestroy( &gHalCapBosMutex );
   }
}
         
/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureThreadInit.
**
** PURPOSE:    Init hal capture thread.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
static void lhapiHalCaptureThreadInit( void )
{
   BOS_STATUS        status;
   
   /*
   ** Create the task within who's samples will be written to flash.
   */
   gHalCapTaskId = 0;
   status = bosTaskCreateEx( LHAPI_HALCAP_TASK_NAME,
                             LHAPI_HALCAP_TASK_STACKSIZE,
                             LHAPI_HALCAP_TASK_PRIORITY,
                             lhapiHalCaptureThreadInitCB,
                             lhapiHalCaptureThread,
                             lhapiHalCaptureThreadDeinitCB,
                             0,
                             &gHalCapTaskId );

   XCHG_ASSERT( status == BOS_STATUS_OK );   
}

/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureThreadInitCB
**
** PURPOSE:    Hal capture task initialization function. This function will be
**             automatically called by the OS prior to invoking the
**             task's main function. Task OS resource allocation and
**             initialization may be performed here.
**
** PARAMETERS: None
**
** RETURNS:    Nothing
**
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
**
*****************************************************************************
*/
static void lhapiHalCaptureThreadInitCB( BOS_TASK_ARG taskArg )
{
   BOS_STATUS        status;
   
   /*
   ** Create a semaphore that will be used to signal the task created below
   ** to run the hausware kernel.
   */
   status = bosSemCreate( "lhapiHalCaptureSem", 0, 1, &gThreadHalCapSem );
   XCHG_ASSERT( status == BOS_STATUS_OK );
}

/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureThreadDeinitCB
**
** PURPOSE:    Hal capture task deinitialization function. This function will be
**             automatically called by the OS after the task's main
**             function has exited.
**
** PARAMETERS: None
**
** RETURNS:    Nothing
**
*  NOTE:       This function should not be called directly. It should be
*              registered with the OS and invoked indirectly.
**
*****************************************************************************
*/
static void lhapiHalCaptureThreadDeinitCB( BOS_TASK_ARG taskArg )
{
   /* Cleanup */
   bosSemDestroy( &gThreadHalCapSem );
}


/*
****************************************************************************
** FUNCTION:   lhapiHalCaptureThread
**
** PURPOSE:    Hal capture task function for hal capture task. This task
**             simply calls the flush to file function.
**
** PARAMETERS: taskArg - not used
**
** RETURNS:    none
**
** NOTE:
******************************************************************************
*/
static void lhapiHalCaptureThread( BOS_TASK_ARG taskArg )
{
   BOS_STATUS  status;

   while ( 1 )
   {
      /* Wait to be notified to run hausware threads. */
      status = bosSemTake( &gThreadHalCapSem );

      if ( status == BOS_STATUS_RESET )
      {
         /* This task has been notified that a reset is pending.
          * Acknowledge the notification and then block until the
          * task is resumed. */
         bosTaskResetAck();

         /* The task should stop running by exiting from this main function */
         return;
      }
      else if ( status == BOS_STATUS_EXIT )
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_LHAPI, "Exiting task %s ", LHAPI_HALCAP_TASK_NAME ));
         return;
      }

      /*
      ** Call the flush function to write samples to file.
      */
      lhapiFlushtoFile(); 
   }
}

/*
*****************************************************************************
** FUNCTION:  lhapiHalCaptureThreadDeInit.
**
** PURPOSE:    De-initialize hal capture thread.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiHalCaptureThreadDeInit( void )
{
   /* Shutdown task */
   if ( gHalCapTaskId != 0 )
   {
      /* Currently, the task should be blocked in the reset-acknowledgement state.
       * Advance the task to the reset-active state. This will cause the task to
       * resume operation. In this state, the task should perform any operations
       * required for proper shutdown before exiting from its main task function. */
      bosTaskProceedToShutdown( &gHalCapTaskId );

      /* Wait for the task to complete de-initialization */
      bosTaskWaitForDone( &gHalCapTaskId );
   }

   gHalCapTaskId = 0;
}

/*
*****************************************************************************
** FUNCTION:   lhapiHalCaptureThreadDispatch
**
** PURPOSE:    This function is called by hausware when it is done to kick off 
**             the capture thread.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void lhapiHalCaptureThreadDispatch( void )
{   
   if( gHalCapInitialized )
   {   
      bosSemGive( &gThreadHalCapSem );
   }
}

#endif /* FEAT_HAL_CAPTURE_TO_FILE */

