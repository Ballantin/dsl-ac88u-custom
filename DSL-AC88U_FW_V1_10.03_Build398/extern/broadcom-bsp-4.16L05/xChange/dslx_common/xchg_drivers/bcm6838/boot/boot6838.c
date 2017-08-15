/***************************************************************************
*    Copyright © 2008 Broadcom Corporation                                        
*                                                                           
*    This program is the proprietary software of Broadcom Corporation and/or
*    its licensors, and may only be used, duplicated, modified or           
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized   
*    License").  Except as set forth in an Authorized License, Broadcom    
*    grants no license (express or implied), right to use, or waiver of any 
*    kind with respect to the Software, and Broadcom expressly reserves all 
*    rights in and to the Software and all intellectual property rights     
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO 
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM   
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.                               
*                                                                           
*                                                                           
*    Except as expressly set forth in the Authorized License,               
*                                                                           
*    1.     This program, including its structure, sequence and             
*    organization, constitutes the valuable trade secrets of Broadcom, and  
*    you shall use all reasonable efforts to protect the confidentiality    
*    thereof, and to use this information only in connection with your use  
*    of Broadcom integrated circuit products.                               
*                                                                           
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,          
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR  
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY        
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,    
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,    
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR         
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT  
*    OF USE OR PERFORMANCE OF THE SOFTWARE.                                 
*                                                                           
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL       
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, 
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN  
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR    
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE 
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: boot6838.c
*
****************************************************************************
*    Description:
*
*     This file contains the implementation for bootloading the "secondary"
*     MIPS thread-processor on the 6838. The bootload is performed by the
*     "primary" thread-processor. In a typical application, the "secondary"
*     thread-processor would be running voice-processing (DSP) algorithms.
*     However, this module should be agnostic to the type of application
*     that is being loaded.
*
****************************************************************************/

/* ---- Include Files ----------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <boot6838.h>
#include <handshake.h>
#include <itpcSharedCmds.h>
#include <mipsDefines.h>
#include <xchgAssert.h>
#include <bosSleep.h>
#include <bcm6838.h>

#include <asm/mipsregs.h>

#define read_c0_status_sel(sel)	__read_32bit_c0_register($12, sel)
#define read_c0_cause_sel(sel)	__read_32bit_c0_register($13, sel)

/* ---- External Variables ------------------------------------------------- */
   
#ifndef BRCM_NONDIST_BUILD   
/* DSP application entry point on a cold start */
extern void TP0halResetEntry(void); 
#else
/* itpcMailbox and TP0halResetEntry are symbols defined on the DSP side
** (for distributed load). When non-distribued noise is used these 
** two variables are defined in this file only as stubs/placeholders */
unsigned int itpcMailbox = 0;
void TP0halResetEntry(void) {}
#endif


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

static XDRV_ITPC *gItpcDrv;
static XDRV_BOOL firstTime = XDRV_TRUE;


/* ---- Private Function Prototypes -------------------------------------- */

static void ResetThreadProcessor( unsigned int *loadAddress );


/* ---- Functions -------------------------------------------------------- */
extern void InstallExcpnHandler(void);

/*****************************************************************************
*
*  FUNCTION:   boot6838Init
*
*  PURPOSE:    Initializes the 6838, MIPS thread-processor bootloader.
*
*
*  PARAMETERS: itpcDrv  (in)  Inter-thread processor communication driver.
*                             Used to signal other TP during boot sequence.
*
*  RETURNS:    0 on success
*
*  NOTES:
*
*****************************************************************************/
int boot6838Init( XDRV_ITPC *itpcDrv )
{

   gItpcDrv = itpcDrv;

   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "regStatus (reg 12 sel 0)    = 0x%08x ", read_c0_status_sel(0) ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "regStatus (reg 12 sel 7)    = 0x%08x ", read_c0_status_sel(7) ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "regCause  (reg 13 sel 0)    = 0x%08x ", read_c0_cause_sel(0)  ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "regCause  (reg 13 sel 7)    = 0x%08x ", read_c0_cause_sel(7)  ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "regCMT    (reg 22 sel 0)    = 0x%08x ", read_c0_diag()        ));
   XDRV_LOG_DBG((XDRV_LOG_MOD_XDRV, "regCMT    (reg 22 sel 1)    = 0x%08x ", read_c0_diag1()       ));

   if ( firstTime )
   {
      /*
      ** Install special exception handler used to bootload the secondary
      ** thread-processor.
      */
      InstallExcpnHandler();
   }

   return ( 0 );
}


/*****************************************************************************
*
*  FUNCTION:   boot6838Deinit
*
*  PURPOSE:    De-initializes the 6838 thread-processor bootloader.
*
*  PARAMETERS: None.
*
*  RETURNS:    0 on success
*
*  NOTES:
*
*****************************************************************************/
int boot6838Deinit( void )
{
   /*
   ** Signal the secondary thread-processor to goto sleep...
   */
   if ( gItpcDrv != NULL )
   {
      xdrvItpcSendSyncCommand( gItpcDrv, ITPC_STANDY_CMD_ID, NULL, NULL );
   }

   gItpcDrv = NULL;

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   boot6838LoadImage
**
** PURPOSE:    To load an image to the secondary thread processor.
**
** PARAMETERS: image       (in)  Image to load onto secondary thread processor.
**             loadAddress (in)  Load address of image.
**
** RETURNS:    0 on success.
**
** NOTE:
*****************************************************************************
*/
int boot6838LoadImage
(
   const BOOT6838_THREAD_PROCESSOR_IMAGE *image,
   unsigned int                          *loadAddress
)
{
   XDRV_BOOL            threadProcessorStarted;
   unsigned int         i;

   /* -----------------------------------------------------------------------
   ** Signal the secondary thread processor to jump to it's entry point and
   ** start running.
   */
#ifdef BRCM_NONDIST_BUILD
   ResetThreadProcessor( NULL );
#else
   ResetThreadProcessor( (unsigned int*)&TP0halResetEntry );
#endif /* BRCM_NONDIST_BUILD */


   /* -----------------------------------------------------------------------
   ** Wait for the secondary thread process to signal that life is good...
   */

   threadProcessorStarted = XDRV_FALSE;
   for ( i = 0; i < 50; i++ )
   {
      int   msg;

      handshakeGetMsg( &msg );

      if( msg == HANDSHAKE_THREAD_PROCESSOR_BOOTED )
      {
         threadProcessorStarted = XDRV_TRUE;
         break;
      }

      bosSleep( 10 );
   }

   if( threadProcessorStarted )
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Thread processor handshake. Secondary app initialized properly." ));
   }
   else
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "ERROR: Secondary thread processor not initialized properly" ));
      return ( -1 );
   }


   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   ResetThreadProcessor
**
** PURPOSE:    To signal the secondary thread processor to jump to it's
**             entry point, and start running.
**
** PARAMETERS: entryPoint  (in)  Thread processor entry point (start) address.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void ResetThreadProcessor( unsigned int *entryPoint )
{
   typedef struct
   {
      unsigned int   command;
      unsigned int   numParams;
      unsigned int  *pTp0Stack;
      unsigned int  *pTp0Function;
   } IpcStruct;

#if 0
   static IpcStruct ipcStruct;
#endif

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Kicking off secondary thread processor at entry point 0x%08X ..." , (unsigned int)entryPoint ));

   if ( firstTime )
   {
      firstTime = XDRV_FALSE;

      /*
      ** For the very first reset, we will send a shared memory command to the
      ** secondary thread-processor, indicating it's start address (and
      ** optional stack pointer). The secondary thread-processor is signalled
      ** via a software interrupt. The interrupt handler is installed by
      ** boot6838Init().
      */

#if 0
      ipcStruct.command       = 0;
      ipcStruct.numParams     = 2;
      ipcStruct.pTp0Stack     = 0; /* Currently, not used. */
      ipcStruct.pTp0Function  = (unsigned int *) entryPoint;
#endif

      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Secondary thread processor entry point at 0x%x", (unsigned int)entryPoint ));

      boot6838SendIpc( (void*)entryPoint );
   }
   else
   {
      /*
      ** For subsequent resets, we will use the inter-thread processor
      ** communication module to signal the secondary thread processor to reset.
      ** This method is different from the first-time reset because the
      ** first-time method requires us to install a special interrupt handler.
      ** Attempting to install a handler may lead to chaos if an interrupt
      ** happens to occur at the same time...so we'll go with this method
      ** instead.
      */

      xdrvItpcSendSyncCommand( gItpcDrv, ITPC_RESET_CMD_ID, NULL, NULL );
   }
}
