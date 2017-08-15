/****************************************************************************
*
* Copyright © 2012 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
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
*    Filename: boardHalSlic6818Si32392.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent SLIC interface for
*     the 6818.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xchgAssert.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <boardHalSlic.h>
#include <boardHalInit.h>
#include <bcm6818.h>
#include <boardHalApm.h>
#include "boardHal6818.h"
#include <boardHalSlic6818uni.h>
#include <bosSleep.h>
#include <bosTask.h>
#include <bosMutex.h>

/* Include driver-specific defines */
#include <slic6818Si32392.h>
#include <slic6818Si32392Defs.h>
#include <hvg6818.h>

/* Mutex used to control SLIC access */
static BOS_MUTEX gSlicBosMutex;

#if BOS_OS_ECOS
   #include <cyg/infra/diag.h>   /* For diag_printf */
#endif

/* Task properities */
#define SLIC_TIMER_TASK_PRIORITY        MTA_TASK_HARDWARE
#define SLIC_TIMER_TASK_NAME            "SLIC"
#define SLIC_TIMER_TASK_STACKSIZE       ( 1024 * 8 )
#ifndef SLIC_TIMER_TASK_PRIORITY
#define SLIC_TIMER_TASK_PRIORITY        BOS_TASK_CLASS_HIGH
#endif

/* Task objects */
static BOS_TASK_ID gSlicTaskId;
static VRG_BOOL    gSlicTaskHalted;

extern void slic6818Si32392UpdateState( SLIC_6818_SI32392_DRV *pDev );

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/* SLIC information for each endpoint */
static SLIC_6818_SI32392_DRV gSlicDriver[ BOARD_HAL_6818_NUM_ENDPTS ];
static int ringDcOffset[ BOARD_HAL_6818_NUM_ENDPTS ];
static VRG_BOOL slicEnabled[ BOARD_HAL_6818_NUM_ENDPTS ];
static VRG_BOOL slicInit[ BOARD_HAL_6818_NUM_ENDPTS ] = { VRG_FALSE, VRG_FALSE };
static VRG_BOOL hvgEnabled[ BOARD_HAL_6818_NUM_ENDPTS ];
/*
** 6818 <-> SI32392 SLIC configuration structure.
*/
static SLICCFG_6818_SI32392_INFO gSlicCfgInfo[ BOARD_HAL_6818_NUM_ENDPTS ];
/* SLIC "meta-driver" interface functions */
int boardHalSlicEnable32392( int lineId, VRG_BOOL enableSlic );
int boardHalSlicReadWriteMemory32392( int endpt, int addr, int data, int write );
int boardHalSlicForceHvgShutdown32392( void );
int boardHalSlicHvgRefresh32392( void );
XDRV_SLIC* boardHalSlicGetDriver32392( int chan );
void boardHalSetAnalogLoopback32392( int lineId, VRG_BOOL enabled );
void boardHalSlicSetRingParms32392( int chan, int ringVoltage, int ringOffset );
static void boardHalSlicSi32392TimerTaskMain( BOS_TASK_ARG taskArg );
static void boardHalSlicSi32392TimerTaskInitCB( BOS_TASK_ARG taskArg );
static void boardHalSlicSi32392TimerTaskDeinitCB( BOS_TASK_ARG taskArg );

/* SLIC "meta-driver" interface */
static const BOARD_HAL_SLIC_FUNCS boardHal6818Si32392DrvFuncs =
{
   boardHalSlicEnable32392,
   boardHalSlicReadWriteMemory32392,
   boardHalSlicForceHvgShutdown32392,
   boardHalSlicHvgRefresh32392,
   boardHalSlicGetDriver32392,
   boardHalSetAnalogLoopback32392,
   boardHalSlicSetRingParms32392
};
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   boardHalSlicInit32392
**
** PURPOSE:    Initialize the SLIC
**
** PARAMETERS: boardHalInitCfg - current init cfg
**             arg - reserved argument.
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicInit32392( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg, VRG_BOOL arg )
{
   SLIC_6818_SI32392_DRV  *pDev;
   int                    endpt;
   int                    result;
   XDRV_APM              *pApmDrv;
   BOS_STATUS            status;
   static int            firstTime = VRG_TRUE;
   if( firstTime == VRG_TRUE )
   {
      /* Create SLIC mutex */
      bosMutexCreate( "SLIC-mutex", &gSlicBosMutex );

      /* Set control flag */
      firstTime = VRG_FALSE;
   }

   /* Acquire mutex to protect interruption during resets */
   bosMutexAcquire( &gSlicBosMutex );



   /* Set the "meta-driver" interface */
   boardHalInitCfg->pDrvSlicFuncs = &boardHal6818Si32392DrvFuncs;

   /* Need the APM driver to query the HVG module */
   pApmDrv = boardHalApmGetDriver( 0 );

   GPIO->GPIOMode |= GPIO_MODE_APM_HVG_MAX_PWM;
   
   for ( endpt = 0; endpt < BOARD_HAL_6818_NUM_ENDPTS; endpt++ )
   {
      /* Fill in the configuration parameters for this chan */
      gSlicCfgInfo[endpt].enhancedControl = boardHalInitCfg->enhancedControl;

      pDev = &( gSlicDriver[endpt] );
      memset( pDev, 0, sizeof( SLIC_6818_SI32392_DRV ) );
      pDev->chan = -1;
      ringDcOffset[ endpt ] = 0;

      /* Initialize the Si32392 SLIC */
      slic6818Si32392Init( endpt, pDev, &gSlicCfgInfo[endpt], pApmDrv );

      /* Start the High voltage Generator */
      if (( result = hvg6818Start( endpt )))
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "hvg6818Start failed, err code (%d)", result));
         /* Release mutex */
         bosMutexRelease( &gSlicBosMutex );
         return ( result );
      }

      /* Set the default reference voltage to the ring voltage for each line */
      if (( result = xdrvApmHvgSetVoltage( pApmDrv, endpt, XDRV_APM_HVG_VOLTAGE_RING )))
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "ERROR: xdrvApmHvgSetVoltage failed, err code (%d)", result ));
         /* Release mutex */
         bosMutexRelease( &gSlicBosMutex );
         return ( result );
      }
      slicInit[endpt] = VRG_TRUE;
      slicEnabled[endpt] = VRG_TRUE;
      hvgEnabled[endpt] = VRG_TRUE;
   }
   
   /* Disable external test loads by default */   
   for ( endpt = 0; endpt < BOARD_HAL_6818_NUM_ENDPTS; endpt++ )
   {
      boardHalSetAnalogLoopback32392( endpt, VRG_FALSE );
   }
   
   /* Initialize SLIC timer task if required */
   gSlicTaskHalted = XDRV_FALSE;
   if( boardHalInitCfg->driverRefresh != XDRV_TRUE )
   {
      gSlicTaskId = 0;
      status = bosTaskCreateEx( SLIC_TIMER_TASK_NAME,
                                SLIC_TIMER_TASK_STACKSIZE,
                                SLIC_TIMER_TASK_PRIORITY,
                                boardHalSlicSi32392TimerTaskInitCB,
                                boardHalSlicSi32392TimerTaskMain,
                                boardHalSlicSi32392TimerTaskDeinitCB,
                                0,
                                &gSlicTaskId );
      XCHG_ASSERT( status == BOS_STATUS_OK );
   }

   /* Release mutex */
   bosMutexRelease( &gSlicBosMutex );

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicDeinit32392
**
** PURPOSE:    Shutdown the SLIC
**
** PARAMETERS:
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicDeinit32392( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg )
{
   SLIC_6818_SI32392_DRV  *pDev;
   int                    endpt;

   /* Acquire mutex to protect interruption during resets */
   bosMutexAcquire( &gSlicBosMutex );
   for ( endpt = 0; endpt < BOARD_HAL_6818_NUM_ENDPTS; endpt++ )
   {
      pDev = &( gSlicDriver[endpt] );

      slicInit[endpt] = VRG_FALSE;
      /* Disable the Si32392 SLIC */
      slic6818Si32392Deinit( pDev );

      /* Do not stop the HVG as we need to preserve line voltage over reset */
   }

   /* Shutdown task for standard resets */
   if( boardHalInitCfg->driverRefresh != XDRV_TRUE )
   {
      if ( gSlicTaskId != 0 )
      {
         /* Currently, the task should be blocked in the reset-acknowledgement state.
          * Advance the task to the reset-active state. This will cause the task to
          * resume operation. In this state, the task should perform any operations
          * required for proper shutdown before exiting from its main task function. */
         bosTaskProceedToShutdown( &gSlicTaskId );

         /* Wait for the task to complete de-initialization */
         bosTaskWaitForDone( &gSlicTaskId );
      }
   }
   else
   {
      gSlicTaskHalted = XDRV_TRUE;
   }

   /* Release mutex */
   bosMutexRelease( &gSlicBosMutex );

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicEnable32392
**
** PURPOSE:    Disable the SLIC interface on the line
**
** PARAMETERS: lineId      - physical interface to control
**             enableSlic  - controls whether to enable the SLIC i/f
**                           (1 = enable; 0 = disable )
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicEnable32392( int lineId, VRG_BOOL enableSlic )
{
   XDRV_SLIC *pDev;

   if ((lineId >= 0)&&(lineId < BOARD_HAL_6818_NUM_ENDPTS))
   {
      pDev = (XDRV_SLIC*)&(gSlicDriver[lineId]);

      if (enableSlic)
      {
         /* Reconnect the SLIC device */
         xdrvSlicModeControl(pDev, XDRV_SLIC_MODE_STANDBY);

         if ( !hvg6818IsEnabled( lineId ) )
         {
            /* HVG is disabled - need to restart it */
            hvg6818Start( lineId );
         }
      }
      else
      {
         /* Disconnect the SLIC device */
         xdrvSlicModeControl(pDev, XDRV_SLIC_MODE_LCFO);

         /* Cut the battery voltage on this channel */
         hvg6818Stop(lineId);
      }

      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicEnable32392: Line %d status change (%d)", lineId, enableSlic));
      return ( 0 );
   }
   else
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicEnable32392: line out of range %d/%d", lineId, BOARD_HAL_6818_NUM_ENDPTS));
      return ( 1 );
   }
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicReadWriteMemory
**
** PURPOSE:    Writes/reads to/from slic memory
**
** PARAMETERS: endpt - the line number ( 0 referenced )
**             addr - the address to read or write
**             data - the data to write to the address ( unused for read operations )
**             write - 0 for read operations, 1 for write operations
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicReadWriteMemory32392( int endpt, int addr, int data, int write )
{
   VRG_UINT16 value;

   /* Perform the necessary read or write operation */
   if( write )
   {
      slic6818Si32392RegisterWrite( endpt, ( VRG_UINT8 )addr, ( VRG_UINT16 )data );
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Data value 0x%04X written to address %d.", ( VRG_UINT16 )data, ( VRG_UINT8 )addr ));
   }
   else
   {
      slic6818Si32392RegisterRead( endpt, ( VRG_UINT8 )addr, &value );
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "Data value 0x%04X read from address %d.", ( VRG_UINT16 )value, ( VRG_UINT8 )addr ));
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicForceHvgShutdown
**
** PURPOSE:    Forces the HVG shutdown regardless of the current operating state.
**
** PARAMETERS:  none
**
** RETURNS:    0 on success, else 1
**
** NOTE:       This is intended to be used by the maintanace thread
**             for interroperability with home security systems.
**
*****************************************************************************
*/
int boardHalSlicForceHvgShutdown32392( void )
{
   int endpt;

   for ( endpt = 0; endpt < BOARD_HAL_6818_NUM_ENDPTS; endpt++ )
   {
      if ( hvg6818IsEnabled( endpt ) )
      {
         if ( hvg6818Stop( endpt ) )
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicForceHvgShutdown: chan %d hvg6818Stop failed !!!", endpt));
            return ( -1 );
         }
         else
         {
            VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicForceHvgShutdown: forced HVG shutdown completed successfully on endpt %d!", endpt));
         }
      }
   }

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicHvgRefresh32392
**
** PURPOSE:    Refreshes the HVG based on the current operating state
**
** PARAMETERS:  none
**
** RETURNS:    0 on success, else 1
**
**  NOTES:     There are requirements for line voltage behaviour during different
**             states of MTA operation (board reset, voice only reset, connectivity
**             loss) to be programmable and assynchrounously controlled from a
**             higher application layer (such as Maintanance Task). Satisfying these
**             requirements may necessitate to shutdown the HVG even after the
**             boardHalInit has configured the HVG for the last time.
**             This API allows the application to check if such late shutdown has
**             occured and revive the HVG module if needed.
**
*****************************************************************************
*/
int boardHalSlicHvgRefresh32392( void )
{
   int endpt;

   for ( endpt = 0; endpt < BOARD_HAL_6818_NUM_ENDPTS; endpt++ )
   {
      if ( !hvg6818IsEnabled( endpt ) )
      {
         VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicHvgRefresh: chan %d HVG state refresh.", endpt));

         /* HVG is disabled - need to restart it */
         hvg6818Start( endpt );
      }
   }

   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicHvgRefresh: HVG refresh completed successfully!"));

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicGetDriver32392
**
** PURPOSE:    Get the SLIC driver associated with an endpoint
**
** PARAMETERS: chan - the line number ( 0 referenced )
**
** RETURNS:    pointer to the SLIC driver interface on success
**             NULL on failure (if driver disabled during asynchronous shutdown)
**
** NOTE:       The application calling into this API needs to check the 
**             the return code to ensure the driver is accessible because
**             NULL pointer returned from this API indicates a failure.
*****************************************************************************
*/
XDRV_SLIC* boardHalSlicGetDriver32392( int chan )
{
   XCHG_ASSERT( ( 0 <= chan ) && ( chan < BOARD_HAL_6818_NUM_ENDPTS ) );

   if( gSlicDriver[chan].bDrvEnabled )
   {
      return ( (XDRV_SLIC*)&(gSlicDriver[chan]) ); 
   }
   else
   {
      return (NULL);
   }
}

/*
*****************************************************************************
** FUNCTION:   boardHalSetAnalogLoopback32392
**
** PURPOSE:    Set GPIO offhook pin to enable/disable analog loopback.
**
** PARAMETERS: lineId - lineID to enable/disable analog loopback
**             enabled - 0 to disable, 1 to enable
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalSetAnalogLoopback32392( int lineId, VRG_BOOL enabled )
{

   if (lineId >= BOARD_HAL_6818_NUM_ENDPTS)
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "Incorrect chan id %d !!!", lineId));
      return;
   }

   if (enabled)
   {
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "lineId %d - enable analog loopback.", lineId));

      /* Engage the built-in test features in 32392 */
      slic6818Si32392RegisterMaskWrite( lineId, SI32392_REG_LINEFEED_CONFIG_2,
                       ( ( lineId % 2 ) == 0 ) ? SI32392_CHAN0_TEST_LOAD_ON:SI32392_CHAN1_TEST_LOAD_ON,
                       ( ( lineId % 2 ) == 0 ) ? SI32392_CHAN0_TEST_LOAD_MASK:SI32392_CHAN1_TEST_LOAD_MASK, 0 );
   
   }
   else
   {
      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "lineId %d - disable analog loopback.", lineId));

      /* Remove the built-in test features in 32392 */
      slic6818Si32392RegisterMaskWrite( lineId, SI32392_REG_LINEFEED_CONFIG_2,
                       ( ( lineId % 2 ) == 0 ) ? SI32392_CHAN0_TEST_LOAD_OFF:SI32392_CHAN1_TEST_LOAD_OFF,
                       ( ( lineId % 2 ) == 0 ) ? SI32392_CHAN0_TEST_LOAD_MASK:SI32392_CHAN1_TEST_LOAD_MASK, 0 );
      }
   }

/*
*****************************************************************************
** FUNCTION:   boardHalSlicSetRingParms
**
** PURPOSE:    Inform board HAL of current ring settings
**
** PARAMETERS: chan - endpoint
**             ringVoltage - ringing voltage for this endpoint
**             ringOffset - ringing DC offset for this endpoint
**
** RETURNS:    none
**
*****************************************************************************
*/
void boardHalSlicSetRingParms32392( int chan, int ringVoltage, int ringOffset )
{
   XDRV_APM *pApmDrv;
   int endpt;
   int result;
   int hvgRefVoltage = XDRV_APM_HVG_VOLTAGE_RING;

   /* Store new ring setting */
   ringDcOffset[ chan ] = ringOffset;

   /* Check if any line is using DC offset during ringing */
   for ( endpt = 0; endpt < BOARD_HAL_6818_NUM_ENDPTS; endpt++ )
   {
      if( ringDcOffset[ endpt ] )
      {
         hvgRefVoltage = XDRV_APM_HVG_VOLTAGE_UHV_RING_13;
      }
   }

   /* Need the APM driver to query the HVG module */
   pApmDrv = boardHalApmGetDriver( 0 );

   /* Set the HVG to the appropriate reference voltage */
   if (( result = xdrvApmHvgSetVoltage( pApmDrv, 0, hvgRefVoltage )))
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "ERROR: xdrvApmHvgSetVoltage failed, err code (%d)", result ));
   }
}

/*****************************************************************************
** FUNCTION:   boardHalSlicSi32392TimerTaskMain
**
** PURPOSE:    Main task function for SLIC processing timer task. This task
**             simply calls the callback registered by the client.
**
** PARAMETERS: taskArg - not used
**
** RETURNS:    none
**
** NOTE:
*****************************************************************************/
static void boardHalSlicSi32392TimerTaskMain( BOS_TASK_ARG taskArg )
{
   BOS_STATUS status;
   int endpt;

   while ( 1 )
   {
      /* Update SLIC state information if task active */
      if( gSlicTaskHalted == XDRV_FALSE )
      {
         for ( endpt = 0; endpt < BOARD_HAL_6818_NUM_ENDPTS; endpt++ )
         {
            if ( gSlicDriver[endpt].bDrvEnabled )
            {
               /* Run periodic SLIC control functionality */
               slic6818Si32392UpdateState( &(gSlicDriver[endpt]) );
            }
         }
      }

      /* Sleep before next execution */
      status = bosSleep( 10 );

      if ( status == BOS_STATUS_RESET )
      {
         /*
         ** This task has been notified that a reset is pending.
         ** Acknowledge the notification and then block until the
         ** task is resumed.
         */
         bosTaskResetAck();

         /* The task should stop running by exiting from this main function */
         return;
      }
   }
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicSi32392TimerTaskInitCB
**
** PURPOSE:    SLIC task initialization function. This function will be
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
static void boardHalSlicSi32392TimerTaskInitCB( BOS_TASK_ARG taskArg )
{
   /* Nothing to do here */
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicSi32392TimerTaskDeinitCB
**
** PURPOSE:    SLIC task deinitialization function. This function will be
**             automatically called by the OS after the task's main
**             function has exited. Any task OS resources allocated by the
**             task initialization function should be de-allocated here.
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
static void boardHalSlicSi32392TimerTaskDeinitCB( BOS_TASK_ARG taskArg )
{
   /* Nothing to do here */
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicSi32392UpdateLineStatus
**
** PURPOSE:    Update the SLIC/HVG based on the current statuses
**
** PARAMETERS: lineId      - physical interface to control
**             enableSlic  - controls whether to enable the SLIC i/f
**             enableHvg   - controls whether to enable the HVG i/f
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicSi32392UpdateLineStatus( int lineId, VRG_BOOL enableSlic, VRG_BOOL enableHvg )
{
   XDRV_SLIC *pDev;

   /* Check endpoint number is valid */
   if (( lineId < 0 ) && ( lineId >= BOARD_HAL_6818_NUM_ENDPTS ))
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicSi32392UpdateLineStatus: line out of range %d/%d", lineId, BOARD_HAL_6818_NUM_ENDPTS ));
      return ( 1 );
   }

   /* Acquire mutex to protect interruption during resets */
   if ( bosMutexAcquire( &gSlicBosMutex ) != BOS_STATUS_OK )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicSi32392UpdateLineStatus: unable to obtain mutex %d", lineId ));
      return ( 1 );
   }

   /* Check if SLIC is initialized */
   if( slicInit[lineId] == VRG_FALSE )
   {
      /* Release mutex */
      bosMutexRelease( &gSlicBosMutex );

      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicSi32392UpdateLineStatus: slic currently deinitialized %d", lineId ));
      return ( 1 );
   }

   /* Check if anything changed */
   if (( slicEnabled[lineId] == enableSlic ) &&
       ( hvgEnabled[lineId] == enableHvg ))
   {
      /* Release mutex */
      bosMutexRelease( &gSlicBosMutex );

      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicSi32392UpdateLineStatus: no status change." ));
      return ( 0 );
   }

   /* Load drivers */
   pDev = (XDRV_SLIC*)&(gSlicDriver[lineId]);

   /* Update drivers accordingly */
   if( enableSlic && enableHvg )
   {
      if( !hvg6818IsEnabled( lineId ) )
      {
         /* HVG is disabled - need to restart it */
         hvg6818Start( lineId );
      }

      /* Reconnect the SLIC device */
      xdrvSlicModeControl( pDev, XDRV_SLIC_MODE_STANDBY );
   }
   else
   {
      /* Disconnect the SLIC device */
      xdrvSlicModeControl( pDev, XDRV_SLIC_MODE_LCFO );

      if( hvg6818IsEnabled( lineId ) )
      {
         /* Cut the battery voltage on this channel */
         hvg6818Stop( lineId );
      }
   }

   /* Record new status */
   slicEnabled[lineId] = enableSlic;
   hvgEnabled[lineId] = enableHvg;

   /* Release mutex */
   bosMutexRelease( &gSlicBosMutex );

   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicSi32392UpdateLineStatus: Line %d status change (%d) (%d)", lineId, enableSlic, enableHvg ));
   return ( 0 );
}