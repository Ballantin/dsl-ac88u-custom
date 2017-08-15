/****************************************************************************
*
* Copyright (c) 2008-2012 Broadcom Corporation
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
*    Filename: boardHalSlic6818L9530.c
*
****************************************************************************
*    Description:
*
*     This file implements the hardware-independent SLIC interface for
*     the 6818.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <boardparms_voice.h>
#include <xchgAssert.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <boardHalSlic.h>
#include <boardHalInit.h>
#include <boardHalApm.h>
#include <bcm6818.h>
#include "boardHal6818.h"
#include <xdrvDaa.h>
#include <boardHalSlic6818uni.h>

#include <bosSleep.h>
#include <bosTask.h>
#include <bosMutex.h>

/* Include driver-specific defines */
#include <slic6818L9530.h>
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

extern void slic6818L9530UpdateState( SLIC_6818_L9530_DRV *pDev );

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#define LINE_OFFHOOK0           0x00000001
#define LINE_OFFHOOK1           0x00000020

/* SLIC information for each endpoint */
static SLIC_6818_L9530_DRV gSlicDriver[ BOARD_HAL_6818_NUM_ENDPTS ];
static VRG_BOOL slicEnabled[ BOARD_HAL_6818_NUM_ENDPTS ];
static VRG_BOOL slicInit[ BOARD_HAL_6818_NUM_ENDPTS ] = { VRG_FALSE, VRG_FALSE };
static VRG_BOOL hvgEnabled[ BOARD_HAL_6818_NUM_ENDPTS ];

/* ---- Private Variables ------------------------------------------------ */
/* SLIC "meta-driver" interface functions */
int boardHalSlicEnable9530( int lineId, VRG_BOOL enableSlic );
int boardHalSlicReadWriteMemory9530( int endpt, int addr, int data, int write );
int boardHalSlicForceHvgShutdown9530( void );
int boardHalSlicHvgRefresh9530( void );
XDRV_SLIC* boardHalSlicGetDriver9530( int chan );
void boardHalSetAnalogLoopback9530( int lineId, VRG_BOOL enabled );
void boardHalSlicSetRingParms9530( int chan, int ringVoltage, int ringOffset );
static void boardHalSlicL9530TimerTaskMain( BOS_TASK_ARG taskArg );
static void boardHalSlicL9530TimerTaskInitCB( BOS_TASK_ARG taskArg );
static void boardHalSlicL9530TimerTaskDeinitCB( BOS_TASK_ARG taskArg );
/*static int boardHalSlicL9530UpdateLineStatus( int lineId, VRG_BOOL enableSlic, VRG_BOOL enableHvg );*/

/* SLIC "meta-driver" interface */
static const BOARD_HAL_SLIC_FUNCS boardHal6818L9530DrvFuncs =
{
   boardHalSlicEnable9530,
   boardHalSlicReadWriteMemory9530,
   boardHalSlicForceHvgShutdown9530,
   boardHalSlicHvgRefresh9530,
   boardHalSlicGetDriver9530,
   boardHalSetAnalogLoopback9530,
   boardHalSlicSetRingParms9530
};
/*
** 6818 <-> L9530 SLIC connectivity configuration structure.
*/
static const SLICCFG_6818_L9530_PIN_INFO gSlicPinInfo6818RG[BOARD_HAL_6818_NUM_ENDPTS] =
{
   /* Chan 0 */
   {
      26,   //B0 pin
      28,   //B1
      29,   //B2
      27,   //NSTAT
      1,   //TLDEN
   },
   /* Chan 1 */
   {
      16,   //B0 pin
      17,   //B1
      18,   //B2
      20,   //NSTAT
      21,   //TLDEN
   }
};

static const SLICCFG_6818_L9530_PIN_INFO gSlicPinInfo6818EGG[BOARD_HAL_6818_NUM_ENDPTS] =
{
   /* Chan 0 */
   {
      36,   //B0 pin
      17,   //B1
      19,   //B2
      29,   //NSTAT
      20,   //TLDEN
   },
   /* Chan 1 */
   {
      38,   //B0 pin
      9,   //B1
      18,   //B2
      4,   //NSTAT
      39,   //TLDEN
   }
};

static const SLICCFG_6818_L9530_PIN_INFO gSlicPinInfo6818GRONT[BOARD_HAL_6818_NUM_ENDPTS] =
{
   /* Chan 0 */
   {
      17,   //B0 pin
      19,   //B1
      20,   //B2
      39,   //NSTAT
      11,   //TLDEN
   },
   /* Chan 1 */
   {
      9,    //B0 pin
      18,   //B1
      4,    //B2
      38,   //NSTAT
      13,   //TLDEN
   }
};

/* SLIC information for each endpoint */
static SLIC_6818_L9530_DRV   gSlicDriver[ BOARD_HAL_6818_NUM_ENDPTS ];

/* ---- Private Function Prototypes -------------------------------------- */
//static void boardHalApmGpioInit ( char* baseBoardIdStr, SLICCFG_6818_L9530_PIN_INFO * pPinInfo );
/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalSlicInit9530
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
int boardHalSlicInit9530( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg, VRG_BOOL arg )
{
   SLIC_6818_L9530_DRV *pDev;
   int                  endpt;
   int                  result;
   XDRV_APM            *pApmDrv;
   BOS_STATUS           status;
   static int            firstTime = VRG_TRUE;

   char baseBoardIdStr[BP_BOARD_ID_LEN];  

   if( firstTime == VRG_TRUE )
   {
      /* Create SLIC mutex */
      bosMutexCreate( "SLIC-mutex", &gSlicBosMutex );

      /* Set control flag */
      firstTime = VRG_FALSE;
   }

   /* Acquire mutex to protect interruption during resets */
   bosMutexAcquire( &gSlicBosMutex );

   if ( BpGetBoardId(baseBoardIdStr) == BP_SUCCESS )
   {
      if( 0 == strcmp(baseBoardIdStr, "96818G_RG") )
      {

#define GPIO_MODE_PCI_Req1_GPIO_16           (1<<16)
#define GPIO_MODE_PCI_Gnt1_GPIO_17           (1<<17)
#define GPIO_MODE_PCI_Intb_GPIO_18           (1<<18)
#define GPIO_MODE_EBI_CS2_GPIO_26            (1<<26)
#define GPIO_MODE_SPI_SSN2_GPIO_28           (1<<28)
#define GPIO_MODE_SPI_SSN3_GPIO_29           (1<<29)

         GPIO->GPIOMode &= ~( GPIO_MODE_PCI_Req1_GPIO_16 |
                            GPIO_MODE_PCI_Gnt1_GPIO_17 |
                            GPIO_MODE_PCI_Intb_GPIO_18 |
                            GPIO_MODE_EBI_CS2_GPIO_26  |
                            GPIO_MODE_SPI_SSN2_GPIO_28 |
                            GPIO_MODE_SPI_SSN3_GPIO_29 );
      }
      else if( 0 == strcmp(baseBoardIdStr, "96818EGG") )
      {

#define GPIO_MODE_6818GR_GPIO_36           (1<<(36-32)) //B0
#define GPIO_MODE_6818GR_GPIO_17           (1<<17)      //B1
#define GPIO_MODE_6818GR_GPIO_19           (1<<19)      //B2
#define GPIO_MODE_6818GR_GPIO_29           (1<<29)      //NSTAT
#define GPIO_MODE_6818GR_GPIO_20           (1<<20)      //TLDEN

#define GPIO_MODE_6818GR_GPIO_38           (1<<(38-32)) //B0
#define GPIO_MODE_6818GR_GPIO_9            (1<<9)       //B1
#define GPIO_MODE_6818GR_GPIO_18           (1<<18)      //B2
#define GPIO_MODE_6818GR_GPIO_4            (1<<4)       //NSTAT
#define GPIO_MODE_6818GR_GPIO_39           (1<<(39-32)) //TLDEN

            GPIO->GPIOMode &= ~( GPIO_MODE_6818GR_GPIO_17 |
                                 GPIO_MODE_6818GR_GPIO_19 |
                                 GPIO_MODE_6818GR_GPIO_29 |
                                 GPIO_MODE_6818GR_GPIO_20 |
                                 GPIO_MODE_6818GR_GPIO_9  |
                                 GPIO_MODE_6818GR_GPIO_18 |
                                 GPIO_MODE_6818GR_GPIO_4 );

            MISC->miscGpioModeCtrlHi &= ~( GPIO_MODE_6818GR_GPIO_36 |
                                           GPIO_MODE_6818GR_GPIO_38 |
                                           GPIO_MODE_6818GR_GPIO_39);
      }
      else if( 0 == strcmp(baseBoardIdStr, "96818GR_ONT") )
      {

#define GPIO_MODE_6818GR_ONT_GPIO_17       (1<<17)      //B0
#define GPIO_MODE_6818GR_ONT_GPIO_19       (1<<19)      //B1
#define GPIO_MODE_6818GR_ONT_GPIO_20       (1<<20)      //B2
#define GPIO_MODE_6818GR_ONT_GPIO_39       (1<<(39-32)) //NSTAT
#define GPIO_MODE_6818GR_ONT_GPIO_11       (1<<20)      //TLDEN

#define GPIO_MODE_6818GR_ONT_GPIO_9        (1<<9)       //B0
#define GPIO_MODE_6818GR_ONT_GPIO_8        (1<<8)       //B1
#define GPIO_MODE_6818GR_ONT_GPIO_14       (1<<14)      //B2
#define GPIO_MODE_6818GR_ONT_GPIO_38       (1<<(38-32)) //NSTAT
#define GPIO_MODE_6818GR_ONT_GPIO_13       (1<<13)      //TLDEN

            GPIO->GPIOMode &= ~( GPIO_MODE_6818GR_ONT_GPIO_17 |
                                 GPIO_MODE_6818GR_ONT_GPIO_19 |
                                 GPIO_MODE_6818GR_ONT_GPIO_20 |
                                 GPIO_MODE_6818GR_ONT_GPIO_11 |
                                 GPIO_MODE_6818GR_ONT_GPIO_9  |
                                 GPIO_MODE_6818GR_ONT_GPIO_8  |
                                 GPIO_MODE_6818GR_ONT_GPIO_14 |
                                 GPIO_MODE_6818GR_ONT_GPIO_13 );

            MISC->miscGpioModeCtrlHi &= ~( GPIO_MODE_6818GR_GPIO_39 |
                                           GPIO_MODE_6818GR_GPIO_38);
      }      
   }
   else
   {
       VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Failed to obtain Base board id string !!!" ));
   }
    
   /* Set the "meta-driver" interface */
   boardHalInitCfg->pDrvSlicFuncs = &boardHal6818L9530DrvFuncs;
   
   /* Need the APM driver to query the HVG module */
   pApmDrv = boardHalApmGetDriver( 0 );

   for ( endpt = 0; endpt < BOARD_HAL_6818_NUM_ENDPTS; endpt++ )
   {
      pDev = &(gSlicDriver[endpt]);
      memset( pDev, 0, sizeof( SLIC_6818_L9530_DRV ) );
      pDev->chan = -1;

      if ( BpGetBoardId(baseBoardIdStr) == BP_SUCCESS )
      {
         if( 0 == strcmp(baseBoardIdStr, "96818G_RG") )
         {
            slic6818L9530Init( pDev, endpt, &gSlicPinInfo6818RG[endpt], pApmDrv);
         }
         else if( 0 == strcmp(baseBoardIdStr, "96818EGG") )
         {
            slic6818L9530Init( pDev, endpt, &gSlicPinInfo6818EGG[endpt], pApmDrv);
         }
         else if( 0 == strcmp(baseBoardIdStr, "96818GR_ONT") )
         {
            slic6818L9530Init( pDev, endpt, &gSlicPinInfo6818GRONT[endpt], pApmDrv);
         }
         else
         {
            VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Board not supported !!!" ));
            XCHG_ASSERT( 0 );
         } 
      }
      else
      {
         VRG_LOG_NOTICE(( VRG_LOG_MOD_BOARDHAL, "Failed to obtain Base board id string !!!" ));
      }

      /* Start the High voltage Generator */
      if ((result = hvg6818Start(endpt)))
      {
         VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "hvg6818Start failed, err code (%d)", result));
         bosMutexRelease( &gSlicBosMutex );
         return ( result );
      }

      slicInit[endpt] = VRG_TRUE;
      slicEnabled[endpt] = VRG_TRUE;
      hvgEnabled[endpt] = VRG_TRUE;

      /* Disable external test loads by default */
      boardHalSetAnalogLoopback9530( endpt, VRG_FALSE );
   }

   /* Initialize SLIC timer task if required */
   gSlicTaskHalted = XDRV_FALSE;
   if( boardHalInitCfg->driverRefresh != XDRV_TRUE )
   {
      gSlicTaskId = 0;
      status = bosTaskCreateEx( SLIC_TIMER_TASK_NAME,
                                SLIC_TIMER_TASK_STACKSIZE,
                                SLIC_TIMER_TASK_PRIORITY,
                                boardHalSlicL9530TimerTaskInitCB,
                                boardHalSlicL9530TimerTaskMain,
                                boardHalSlicL9530TimerTaskDeinitCB,
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
** FUNCTION:   boardHalSlicDeinit9530
**
** PURPOSE:    Shutdown the SLIC
**
** PARAMETERS:
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalSlicDeinit9530( VRG_BOARDHAL_INIT_CFG *boardHalInitCfg )
{
   SLIC_6818_L9530_DRV *pDev;
   int                  endpt;

   /* Acquire mutex to protect interruption during resets */
   bosMutexAcquire( &gSlicBosMutex );
   for ( endpt = 0; endpt < BOARD_HAL_6818_NUM_ENDPTS; endpt++ )
   {
      pDev = &(gSlicDriver[endpt]);

      slicInit[endpt] = VRG_FALSE;

      /* Disable the L9530 SLIC */
      slic6818L9530Deinit( pDev );

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
** FUNCTION:   boardHalSlicEnable9530
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
int boardHalSlicEnable9530( int lineId, VRG_BOOL enableSlic )
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

      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "%s: Line %d status change (%d)", __FUNCTION__, lineId, enableSlic));
      return ( 0 );
   }
   else
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicEnable: line out of range %d/%d", lineId, BOARD_HAL_6818_NUM_ENDPTS));
      return ( 1 );
   }
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicReadWriteMemory9530
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
int boardHalSlicReadWriteMemory9530( int endpt, int addr, int data, int write )
{
   /* L9530 SLICs do not have memory */
   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "%s: L9530 SLICs do not have memory", __FUNCTION__ ));

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicForceHvgShutdown9530
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
int boardHalSlicForceHvgShutdown9530( void )
{
   int endpt;

   for ( endpt = 0; endpt < BOARD_HAL_6818_NUM_ENDPTS; endpt++ )
   {
      if ( hvg6818IsEnabled( endpt ) )
      {
         if (hvg6818Stop(endpt))
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicForceHvgShutdown: chan %d hvg6818Stop failed !!!", endpt));
            return ( -1 );
         }
         else
         {
            VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicForceHvgShutdown: forced HVG shutdown completed successfully on endpt %d!", endpt));
         }
      }
   }

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicHvgRefresh9530
**
** PURPOSE:    Refreshes the HVG based on the current operating state
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
int boardHalSlicHvgRefresh9530( void )
{
   int endpt;

   for ( endpt = 0; endpt < BOARD_HAL_6818_NUM_ENDPTS; endpt++ )
   {
      if ( !hvg6818IsEnabled( endpt ) )
      {
         VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "%s: chan %d HVG state refresh.", __FUNCTION__, endpt));

         /* HVG is disabled - need to restart it */
         hvg6818Start( endpt );
      }
   }

   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "%s: HVG refresh completed successfully!", __FUNCTION__ ));

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boardHalSlicGetDriver9530
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
XDRV_SLIC* boardHalSlicGetDriver9530( int chan )
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
** FUNCTION:   boardHalSetAnalogLoopback9530
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
void boardHalSetAnalogLoopback9530( int lineId, VRG_BOOL enabled )
{

   if (lineId >= BOARD_HAL_6818_NUM_ENDPTS)
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "Incorrect chan id %d !!!", lineId));
      return;
   }

   SLIC_6818_L9530_DRV *pDev = &(gSlicDriver[lineId]);
   if(pDev->bDrvEnabled != XDRV_TRUE)
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "Device chan %d not ready!!!", lineId));
      return;
   }

   volatile XDRV_UINT32 *pTldenCtrl = pDev->slicCfgInfo.slicIoTldCtrlp;
   volatile XDRV_UINT32 *pTldenIo = pDev->slicCfgInfo.slicIoTldDatap;
   XDRV_UINT32 tldenMask = pDev->slicCfgInfo.slicIoTldMask;

   if (enabled)
   {
      *pTldenCtrl |= tldenMask;
      *pTldenIo |= tldenMask;
   }
   else
   {
      *pTldenCtrl |= tldenMask;
      *pTldenIo &= ~(tldenMask);
   }

}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicSetRingParms9530
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
void boardHalSlicSetRingParms9530( int chan, int ringVoltage, int ringOffset )
{
   /* Nothing to do here */
   return;
}


/*****************************************************************************
** FUNCTION:   boardHalSlicL9530TimerTaskMain
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
static void boardHalSlicL9530TimerTaskMain( BOS_TASK_ARG taskArg )
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
               slic6818L9530UpdateState( &(gSlicDriver[endpt]) );
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
** FUNCTION:   boardHalSlicL9530TimerTaskInitCB
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
static void boardHalSlicL9530TimerTaskInitCB( BOS_TASK_ARG taskArg )
{
   /* Nothing to do here */
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicL9530TimerTaskDeinitCB
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
static void boardHalSlicL9530TimerTaskDeinitCB( BOS_TASK_ARG taskArg )
{
   /* Nothing to do here */
}

/*
*****************************************************************************
** FUNCTION:   boardHalSlicL9530UpdateLineStatus
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
int boardHalSlicL9530UpdateLineStatus( int lineId, VRG_BOOL enableSlic, VRG_BOOL enableHvg )
{
   XDRV_SLIC *pDev;

   /* Check endpoint number is valid */
   if (( lineId < 0 ) && ( lineId >= BOARD_HAL_6818_NUM_ENDPTS ))
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicL9530UpdateLineStatus: line out of range %d/%d", lineId, BOARD_HAL_6818_NUM_ENDPTS ));
      return ( 1 );
   }

   /* Acquire mutex to protect interruption during resets */
   if ( bosMutexAcquire( &gSlicBosMutex ) != BOS_STATUS_OK )
   {
      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicL9530UpdateLineStatus: unable to obtain mutex %d", lineId ));
      return ( 1 );
   }

   /* Check if SLIC is initialized */
   if( slicInit[lineId] == VRG_FALSE )
   {
      /* Release mutex */
      bosMutexRelease( &gSlicBosMutex );

      VRG_LOG_WARN(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicL9530UpdateLineStatus: slic currently deinitialized %d", lineId ));
      return ( 1 );
   }

   /* Check if anything changed */
   if (( slicEnabled[lineId] == enableSlic ) &&
       ( hvgEnabled[lineId] == enableHvg ))
   {
      /* Release mutex */
      bosMutexRelease( &gSlicBosMutex );

      VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicL9530UpdateLineStatus: no status change." ));
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

   VRG_LOG_DBG(( VRG_LOG_MOD_BOARDHAL, "boardHalSlicL9530UpdateLineStatus: Line %d status change (%d) (%d)", lineId, enableSlic, enableHvg ));
   return ( 0 );
}
