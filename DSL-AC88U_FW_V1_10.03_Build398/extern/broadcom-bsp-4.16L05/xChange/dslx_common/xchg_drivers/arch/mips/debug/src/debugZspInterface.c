/****************************************************************************
*
* Copyright © 2000-2008 Broadcom Corporation
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
****************************************************************************
*
*    Filename: debugZspInterface.c
*
****************************************************************************
*    Description:
*
*     This file contains functions related to the Debug Module.
*     Debug module is used to retrieve info of interest from the DSP,
*     IPC, and potentially other peripherals.
*
*     This instance of the debug module will interface to a ZSP processor to
*     obtain DSP debug information.
*
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <string.h>
#include <dbgCom.h>
#include <zspContext.h>
#include <debugZspInterface.h>
#include "debugIpcPrivate.h"
#include <xdrvDebug.h>

/* Redefine XDRV_LOG_INFO to use stdio printf so it
** doesn't add the timestamp and garble the output */
#include <stdio.h>
#undef XDRV_LOG_INFO
#undef XDRV_LOG_INTERNAL_PRINTF
#define XDRV_LOG_INFO(x)                   XDRV_LOG_INTERNAL_PRINTF x
#define XDRV_LOG_INTERNAL_PRINTF(x,y,...)  printf(y "\n", ##__VA_ARGS__)

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

#define  MAX_BUFFER_SIZE   0x200

/* ---- Private Variables ------------------------------------------------ */

/* Pointer to DSP debug communication driver */
static DBGCOM *gDbgCom;

/* ---- Private Function Prototypes -------------------------------------- */

static void PrintRegs( const ZSP_CONTEXT_REG *regp, XDRV_UINT16 r12 );
static void PrintBuffer( XDRV_UINT16 dspAddr, const XDRV_UINT16 *pData, unsigned int numToPrint );

static int debugDspReadCnt( void );
static int debugDspForceAssert( void );
static int debugDspLogAssertContext( void  );
static unsigned int debugDspGetAssertStackPtr( void );
static int debugDspGetRegs( void );
static void debugDspDumpDataMem( unsigned int dspAddr, unsigned int amountToRead );
static void debugDspStackShow( void );

/* Debug "meta-driver" interface */
static const XDRV_DEBUG_FUNCS  gDebugCmtDrvFuncs =
{
   debugDspReadCnt,
   debugDspForceAssert,
   debugDspLogAssertContext,
   debugDspGetAssertStackPtr,
   debugDspGetRegs,
   debugDspDumpDataMem,
   debugDspStackShow
};


/* ---- Functions -------------------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   debugZspInterfaceInit
**
** PURPOSE:    Initialize the Debug module. This instance of the debug
**             module will interface to a ZSP processor to obtain DSP
**             debug information.
**
** PARAMETERS: pDbgCom - pointer to the DSP debug communication driver
**             pDrv    - pointer to Debug driver object
**
** RETURNS:    none
**
*****************************************************************************
*/
void debugZspInterfaceInit( DBGCOM *pDbgCom, XDRV_DEBUG *pDrv )
{
   /* Init sub-modules. */
   debugIpcInit();

   /* Link in driver modules */
   gDbgCom = pDbgCom;

   /* Set the "meta-driver" interface */
   memset( pDrv, 0, sizeof( XDRV_DEBUG ) );
   pDrv->pDrvFuncs = &gDebugCmtDrvFuncs;
}

/*
*****************************************************************************
** FUNCTION:   debugZspInterfaceDeinit
**
** PURPOSE:    Shutdown the Debug module
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void debugZspInterfaceDeinit( void )
{
   debugIpcDeinit();
}

/*
*****************************************************************************
** FUNCTION:   debugDspReadCnt
**
** PURPOSE:    To read the value of the DSP's background & interrupt counters.
**
** PARAMETERS: none
**
** RETURNS:    0  - DSP is moving
**             -1 - DSP counters are static
**
** NOTE:
*****************************************************************************
*/
static int debugDspReadCnt( void )
{
   int rc;

   rc = dbgComDspReadCnt( gDbgCom );

   return ( rc );
}

/*
*****************************************************************************
** FUNCTION:   debugDspForceAssert
**
** PURPOSE:    Force DSP assert.
**
** PARAMETERS: none.
**
** RETURNS:    0 on success
**
** NOTE:
*****************************************************************************
*/
static int debugDspForceAssert( void )
{
   dbgComForceAssert( gDbgCom );

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   debugDspGetRegs
**
** PURPOSE:    Get a snapshot of DSP registers.
**
** PARAMETERS: none.
**
** RETURNS:    0 on success
**
** NOTE:       When the DSP receives the getreg command, it places a copy
**             of its registers into an internal memory region, and then place
**             the address of that place into the trace-register of the shared memory
**             debug-com interface.
*****************************************************************************
*/
static int debugDspGetRegs( void )
{
   DBGCOM               *pDebugCom = gDbgCom;
   XDRV_UINT16          dspRegPntr, dspStackPntr;
   ZSP_CONTEXT_REG zspRegs;

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "    ----****----" ));

   /*
   ** Get the DSP internal memory address where the
   ** DSP register snapshot gets stored.
   */
   dspRegPntr = dbgComDspGetRegs( pDebugCom );


   /* Check if the reg pointer makes sense. Hardcoded to 0x0000-0x8000 for now */
   if (( dspRegPntr == 0 ) || (dspRegPntr > 0x8000))
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DEBUG ERROR: DSP reg pointer (0x%x) out of bounds (0x0001-0x8000) !!!", dspRegPntr ));
      return ( -1 );
   }
   else
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   regp  0x%x", dspRegPntr ));
   }

   /*
   ** When the dsp receives the getreg command, it puts its stack pointer in the trace reg.
   */
   dspStackPntr = dbgComTraceRead( pDebugCom );
   dspStackPntr += (sizeof( zspRegs ) / sizeof( XDRV_UINT16 ));

   /* Check if the stack pointer makes sense. Hardcoded to 0x1000-0x8000 for now */
   if (( dspStackPntr < 0x1000 ) || (dspStackPntr > 0x8000))
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DEBUG ERROR: DSP stack pointer (0x%x) out of bounds (0x1000-0x8000) !!!", dspStackPntr ));
      return (-1);
   }
   else
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   stackp  0x%x", dspStackPntr ));
   }

   /*
   ** The dbgComDspGetRegs code on the dsp has stored all of the zsp regs on the stack
   */
   if (dbgComDspRead( pDebugCom,
                    (XDRV_UINT16 *)&zspRegs,
                     dspRegPntr,
                     sizeof( zspRegs ) / sizeof( XDRV_UINT16 ) )==0)
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DEBUG ERROR: Could not get the DSP registers."));
      return ( -1 );
   }

   /*
   ** Print the register values.
   */
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "    ----****----" ));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DSP running at address of 0x%04x.", zspRegs.tpc ));
   PrintRegs( &zspRegs, dspStackPntr );

   return ( 0 );
}

/*
*****************************************************************************
** FUNCTION:   debugDspGetAssertStackPtr
**
** PURPOSE:    Get DSP assert stack Ptr.
**
** PARAMETERS:
**
** RETURNS:    Assert stack pointer.
**
** NOTE:       When the DSP asserts, it overwrites the "doing fine" indicator
**             with the actual stack pointer. This routine can be used to
**             Check if the DSP is doing fine
*****************************************************************************
*/
static unsigned int debugDspGetAssertStackPtr( void )
{
   DBGCOM               *pDebugCom = gDbgCom;
   XDRV_UINT16          dspStackPntr;

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "    ----****----" ));

   /*
   ** When the dsp asserts it puts its stack pointer in the trace reg.
   */
   dspStackPntr = dbgComDspReadAssertStackPtr( pDebugCom );

   if (dspStackPntr == ZSP_DEBUG_DOING_FINE)
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DEBUG: DSP doing just fine -> stackp = 0x%x", dspStackPntr ));
   }
   else
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DSP ASSERTED" ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "   stackp  0x%x", dspStackPntr ));
   }

   return ( dspStackPntr );
}

/*
*****************************************************************************
** FUNCTION:   debugDspLogAssertContext
**
** PURPOSE:    To log the state of the DSP register set and stack context. This
**             function is intented to be called after the DSP asserts.
**
** PARAMETERS: pDebugCom         (mod) Debug-com interface used to read DSP
**                                     regitser set and stack context.
**
** RETURNS:    0 on success.
**
** NOTE:       When the DSP asserts, it is assumed that it will push the contents
**             of it's register set onto the stack, and then place the value
**             of the stack pointer into the trace-register of the shared memory
**             debug-com interface.
*****************************************************************************
*/
static int debugDspLogAssertContext( void )
{
   DBGCOM               *pDebugCom = gDbgCom;
   XDRV_UINT16          dspAddr;
   unsigned int         dspStackPntr;
   XDRV_UINT16          actualr12;
   unsigned int         amountToRead;
   ZSP_CONTEXT_REG zspRegs;
   unsigned int         dspAddrOfStackEnd = 0x8000;   /* HACK: hardcode DSP stack End address */


   /* Get the assert stack pointer - make sure the DSP asserted */
   if ((dspStackPntr = debugDspGetAssertStackPtr()) == ZSP_DEBUG_DOING_FINE)
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "ERROR: Why did you hit debugDspLogAssertContext() if DSP's fine ?!?!?! "));
      return ( -1 );
   }

   /* Check if the stack pointer makes sense. Hardcoded to 0x1000-0x8000 for now */
   if (( dspStackPntr < 0x1000 ) || (dspStackPntr > dspAddrOfStackEnd))
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "ERROR: DSP stack pointer (0x%x) out of bounds (0x1000-0x%x) !!!", dspStackPntr, dspAddrOfStackEnd ));
      return ( -1 );
   }

   /*
   ** The assert code on the dsp has stored all of the zsp regs on the stack
   */
   if ( ( amountToRead = dbgComDspRead( pDebugCom,
                                       (XDRV_UINT16 *)&zspRegs,
                                       (dspStackPntr + 1),
                                        sizeof( zspRegs ) / sizeof( XDRV_UINT16 ) )) == 0)
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DEBUG ERROR:    ----****----" ));
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DEBUG ERROR: DSP asserted but could not get the context."));
      return ( -1 );
   }

   /*
   ** Print the register values, use the actual r12 (sp) value without
   ** the saved context.
   */
   actualr12 = dspStackPntr + (sizeof( zspRegs ) / sizeof( XDRV_UINT16 ));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "    ----****----" ));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "DSP asserted at address of 0x%04x.", zspRegs.tpc ));
   PrintRegs( &zspRegs, actualr12 );

   /*
   ** Read the block of memory, starting at the stack pointer.
   */
   dspAddr = actualr12 + 1;
   amountToRead = dspAddrOfStackEnd - dspAddr;
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Stack dump:" ));
   debugDspDumpDataMem( dspAddr, amountToRead );

   return ( 0 );
}


/*
*****************************************************************************
** FUNCTION:   debugDspDumpDataMem
**
** PURPOSE:    To log DSP data memory to the console.
**
** PARAMETERS: dspAddr        (in)  DSP data address to start log from.
**             amountToRead   (in)  Amount of data to dump.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void debugDspDumpDataMem( unsigned int addr, unsigned int amountToRead )
{
   DBGCOM         *pDebugCom = gDbgCom;
   unsigned int   numBlocks, numRemain;
   XDRV_UINT16    readBuf[MAX_BUFFER_SIZE];
   unsigned int   i, numRead;
   XDRV_UINT16    dspAddr = (XDRV_UINT16) addr;


   if ( (  addr                 > 0x8000 )   ||
        (  amountToRead         > 0x8000 )   ||
        ( (addr + amountToRead) > 0x8000 ) )
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Input parameters addr=0x%x; len=0x%x out of valid range (0-0x8000).", addr, amountToRead ));
      return;
   }


   numBlocks = amountToRead / MAX_BUFFER_SIZE;
   numRemain = amountToRead % MAX_BUFFER_SIZE;

   for( i = 0; i < numBlocks; i++ )
   {
      if ((numRead = dbgComDspRead( pDebugCom, readBuf, dspAddr, MAX_BUFFER_SIZE )) == 0)
      {
         return;
      }

      PrintBuffer( dspAddr, readBuf, numRead );
      dspAddr = dspAddr + MAX_BUFFER_SIZE;
   }

   if( numRemain != 0 )
   {
      if ((numRead = dbgComDspRead( pDebugCom, readBuf, dspAddr, numRemain ))== 0)
      {
         return;
      }

      PrintBuffer( dspAddr, readBuf, numRead );
   }
}


/*
*****************************************************************************
** FUNCTION:   PrintRegs
**
** PURPOSE:    To log the ZSP register set.
**
** PARAMETERS: regp  (in)  Register set to log.
**             r12   (in)
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void PrintRegs( const ZSP_CONTEXT_REG *regp, XDRV_UINT16 r12 )
{

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "    ----****----" ));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "ZSP Register Dump:" ));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "      r0:%04x      r4:%04x      r8:%04x     r12:%04x",
              regp->r0,
              regp->r4,
              regp->r8,
              r12       ));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "      r1:%04x      r5:%04x      r9:%04x     r13:%04x",
              regp->r1,
              regp->r5,
              regp->r9,
              regp->r13 ));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "      r2:%04x      r6:%04x     r10:%04x     r14:%04x",
               regp->r2,
               regp->r6,
               regp->r10,
               regp->r14 ));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "      r3:%04x      r7:%04x     r11:%04x     r15:%04x",
              regp->r3,
              regp->r7,
              regp->r11,
              regp->r15 ));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "" ));
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "     rpc:%04x   loop0:%04x cb0_beg:%04x   fmode:%04x",
              regp->rpc,
              regp->loop0,
              regp->cb0_beg,
              regp->fmode ));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "     tpc:%04x   loop1:%04x cb0_end:%04x   smode:%04x",
              regp->tpc,
              regp->loop1,
              regp->cb0_end,
              regp->smode ));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "    vitr:%04x   loop2:%04x cb1_beg:%04x   guard:%04x",
              regp->vitr,
              regp->loop2,
              regp->cb1_beg,
              regp->guard ));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "  hwflag:%04x   loop3:%04x cb1_end:%04x   imask:%04x",
              regp->hwflag,
              regp->loop3,
              regp->cb1_end, regp->imask ));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "    ireq:%04x", regp->ireq ));

   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "    ----****----" ));
}


/*
*****************************************************************************
** FUNCTION:   PrintBuffer
**
** PURPOSE:    Utility logging function.
**
** PARAMETERS: dspAddr     (in)  Start DSP address.
**             pData       (in)  Buffer to log.
**             numToPrint  (in)  Number words to log.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void PrintBuffer
(
   XDRV_UINT16          dspAddr,
   const XDRV_UINT16   *pData,
   unsigned int         numToPrint
)
{
   unsigned int i,j,count;

   if( numToPrint != 0 )
   {
      unsigned int numFullRows;
      unsigned int numRemain;

      count = 0;
      numFullRows = numToPrint / 16;
      numRemain   = numToPrint % 16;

      for( i = 0; i < numFullRows; i++ )
      {
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%04x: ", dspAddr + count ));
         for( j=0; j<16; j++ )
         {
            XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%04x ", *(pData + count) ));
            count++;
         }
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "" ));
      }

      if( numRemain != 0 )
      {
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%04x: ", dspAddr + count ));
         for( j=0; j<numRemain; j++ )
         {
            XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "%04x ", *(pData + count) ));
            count++;
         }
         XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "" ));
      }
   }
   else
   {
      XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "dspAssertPrint.c  Error:  Requested to print 0 words." ));
   }
}


/*
*****************************************************************************
** FUNCTION:   debugDspGetStackStats
**
** PURPOSE:    To display stack usage statistics for the DSP.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
static void debugDspStackShow( void )
{
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "Sorry, stack-show command not supported!" ));
}
