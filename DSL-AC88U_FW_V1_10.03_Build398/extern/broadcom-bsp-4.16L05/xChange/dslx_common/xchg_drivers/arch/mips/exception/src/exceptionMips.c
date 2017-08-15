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
*****************************************************************************
*
*    Filename: exceptionMips.c
*
****************************************************************************
*    Description:
*
*     Interface for MIPS specific exception module. It implements to the
*     architecture-independent API - xdrvException.h.
*
*     This module allows clients to register custom exception handlers that
*     allow the events to be handled in an application specific manner.
*
*     User registered exception handlers are high level (C language)
*     functions. Low-level, architecture specific, processor context saves
*     will be performed prior to invoking registered handlers. Like-wise,
*     a context restore will be performed after the registered handler returns.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvException.h>
#include <exceptionMips.h>
#include <stdio.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */

/*
** Set to '1' to enable support for a single, global exception handler for all
** types of exceptions. This is useful when all exceptions need to be handled
** in the same manner. Set to '0' to enable a unique handler for each type
** of exception.
*/
#define EXCEPTION_MIPS_SINGLE_HANDLER     0


#ifndef EXCEPTION_MIPS_LOG
/* UNFINISHED: What do I want the default to be? Can I rely upon the fact that
               that we have support for printf, and that it's safe to call it
               in exception context? For now I will; if this proves to be untrue
               then we can change this to be a build configuration option. */
#define EXCEPTION_MIPS_LOG( a )     printf a
#endif

/* ---- Private Variables ------------------------------------------------ */


#if EXCEPTION_MIPS_SINGLE_HANDLER
   /* Declare as a array for easier code-sharing with #else case. */
   static XDRV_EXCEPTION_HANDLER gExceptionHandler[1] = {NULL};
   static XDRV_EXCEPTION_PARM    gExceptionData[1]    = {NULL};
#else
   static XDRV_EXCEPTION_HANDLER gExceptionHandler[ EXCEPTION_MIPS_NUM_IDS ] = {NULL};
   static XDRV_EXCEPTION_PARM    gExceptionData[ EXCEPTION_MIPS_NUM_IDS ]    = {NULL};
#endif


/* ---- Private Function Prototypes -------------------------------------- */

void hal_init_exceptions( void );

static void DefaultExceptionHandler
(
   XDRV_EXCEPTION_ID       exceptionId,
   XDRV_EXCEPTION_PARM     data,
   XDRV_EXCEPTION_INFO     exceptionInfo
);


/* ---- Functions -------------------------------------------------------- */


/*
*****************************************************************************
** FUNCTION:   exceptionInitMips
**
** PURPOSE:    Architecture specific init function. Setup default, high-level
**             exception handlers.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:       It is assumed that the low-level exception vector handlers
**             have already been installed. This is performed by calling
**             exceptionMipsInstallVectors. The vectors are not installed
**             by this function because this should be done very early
**             in the chip initialization sequence, likely prior to setting up
**             the C-environment (stack pointer, bss, data-sections etc).
*****************************************************************************
*/
void exceptionInitMips( void )
{
   unsigned int excId, excIdMax;

   /*
   ** Prime the user registered handlers with default values.
   */
   #if EXCEPTION_MIPS_SINGLE_HANDLER
   {
      /* There's only a single exception handler for all exception types. */
      excIdMax = 1;
   }
   #else
   {
      /* There one exception handler per exception type. */
      excIdMax = EXCEPTION_MIPS_NUM_IDS;
   }
   #endif


   for ( excId = 0; excId < excIdMax; excId++ )
   {
      gExceptionHandler[ excId ] = DefaultExceptionHandler;
      gExceptionData   [ excId ] = NULL;
   }
}


/*
*****************************************************************************
** FUNCTION:   exceptionDeinitMips
**
** PURPOSE:    Architecture specific deinit function. Re-install default MIPS
**             exception handlers.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void exceptionDeinitMips( void )
{
   exceptionInitMips();
}


/*
*****************************************************************************
** FUNCTION:   exceptionNotifyMips
**
** PURPOSE:    This function should be called by the low-level assembly
**             exception code after saving the necessary processor state.
**             This function will invoke the high level user registered
**             exception handler.
**
** PARAMETERS: exceptionInfo  (in)  Architecture specific exception information.
**
** RETURNS:    None.
**
** NOTE:
*****************************************************************************
*/
void exceptionNotifyMips
(
   EXCEPTION_MIPS_STATE   *exceptionInfo
)
{
   XDRV_EXCEPTION_HANDLER  exceptionHandler;
   XDRV_EXCEPTION_PARM     exceptionData;
   EXCEPTION_MIPS_ID       exceptionId;
   unsigned int            exceptionIndex;


   /* The exception id is embedded in the info struct, pull it out. */
   exceptionId = ( exceptionInfo->vector >> 2 );


   /* Find the exception handler. */
   #if EXCEPTION_MIPS_SINGLE_HANDLER
   {
      /* There's only a single exception handler for all exception types. */
      exceptionIndex = 0;
   }
   #else
   {
      /* There one exception handler per exception type. It's indexed by
      ** the exception id, which is 0-indexed. */
      exceptionIndex = exceptionId;
   }
   #endif

   exceptionHandler = gExceptionHandler[ exceptionIndex ];
   exceptionData    = gExceptionData[ exceptionIndex ];


   /* Now invoke it! */
   if ( exceptionHandler != NULL )
   {
      exceptionHandler( exceptionId, exceptionData, exceptionInfo );
   }
}


/*
*****************************************************************************
** FUNCTION:   xdrvExceptionSetHandler
**
** PURPOSE:    To register an exception handler.
**
** PARAMETERS: exceptionId          (in)  Exception to register. The value of
**                                        'exceptionId' will be chip architecture
**                                        specific.
**             newExceptionHandler  (in)  Exception handler to register.
**             newExceptionParm     (in)  Parameter to pass to exception when invoked.
**             oldExceptionHandler  (out) Previously registered exception handler.
**                                        Pass in NULL for don't care.
**             oldExceptionParm     (out) Previously registered parameter.
**                                        Pass in NULL for don't care.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvExceptionSetHandler
(
   XDRV_EXCEPTION_ID       exceptionId,
   XDRV_EXCEPTION_HANDLER  newExceptionHandler,
   XDRV_EXCEPTION_PARM     newExceptionParm,
   XDRV_EXCEPTION_HANDLER *oldExceptionHandler,
   XDRV_EXCEPTION_PARM    *oldExceptionParm
)
{
   unsigned int exceptionIndex;

   #if EXCEPTION_MIPS_SINGLE_HANDLER
   {
      /* There's only a single exception handler for all exception types. */
      exceptionIndex = 0;
   }
   #else
   {
      /* There one exception handler per exception type. It's indexed by
      ** the exception id, which is 0-indexed. */
      exceptionIndex = exceptionId;
   }
   #endif


   /* Return the old handlers. */
   if ( oldExceptionHandler != NULL )
   {
      *oldExceptionHandler = gExceptionHandler[ exceptionIndex ];
   }

   if ( oldExceptionParm != NULL )
   {
      *oldExceptionParm    = gExceptionData[ exceptionIndex ];
   }


   /* Install new handlers. */
   gExceptionHandler[ exceptionIndex ] = newExceptionHandler;
   gExceptionData[ exceptionIndex ]    = newExceptionParm;
}


/*
*****************************************************************************
** FUNCTION:   xdrvExceptionSetAllHandlers
**
** PURPOSE:    To register the same exception handler for all exception types.
**
** PARAMETERS: newExceptionHandler  (in)  Exception handler to register.
**             newExceptionParm     (in)  Parameter to pass to exception when invoked.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvExceptionSetAllHandlers
(
   XDRV_EXCEPTION_HANDLER  newExceptionHandler,
   XDRV_EXCEPTION_PARM     newExceptionParm
)
{
   unsigned int excId, excIdMax;

   #if EXCEPTION_MIPS_SINGLE_HANDLER
   {
      /* We only support a single exception handler for all exception types.
      ** Therefore, just install the user-handler for a single exception. */
      excIdMax = 1;
   }
   #else
   {
      /* Install the same exception handler for each exception type. */
      excIdMax = EXCEPTION_MIPS_NUM_IDS;
   }
   #endif


   for ( excId = 0; excId < excIdMax; excId++ )
   {
      xdrvExceptionSetHandler( excId,
                               newExceptionHandler,
                               newExceptionParm,
                               NULL,
                               NULL );
   }
}


/*
*****************************************************************************
** FUNCTION:   xdrvExceptionResetHandler
**
** PURPOSE:    To de-register the user specified exception handler, and revert
**             to the default exception handler.
**
** PARAMETERS: exceptionId (in)  Revert to default handler for this exception.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvExceptionResetHandler
(
   XDRV_EXCEPTION_ID    exceptionId
)
{
   xdrvExceptionSetHandler( exceptionId, DefaultExceptionHandler, 0, NULL, NULL );
}


/*
*****************************************************************************
** FUNCTION:   xdrvExceptionResetAllHandlers
**
** PURPOSE:    To de-register the user specified exception handler, and revert
**             to the default exception handler (for all exception types).
**
** PARAMETERS: exceptionId (in)  Revert to default handler for this exception.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvExceptionResetAllHandlers( void )
{
   unsigned int excId, excIdMax;

   #if EXCEPTION_MIPS_SINGLE_HANDLER
   {
      /* We only support a single exception handler for all exception types.
      ** Therefore, just reset single exception handler back to it's default. */
      excIdMax = 1;
   }
   #else
   {
      /* Reset exception handlers for each exception type. */
      excIdMax = EXCEPTION_MIPS_NUM_IDS;
   }
   #endif


   for ( excId = 0; excId < excIdMax; excId++ )
   {
      xdrvExceptionResetHandler( excId );
   }
}


/*
*****************************************************************************
** FUNCTION:   DefaultExceptionHandler
**
** PURPOSE:    The default exception handler used when one hasn't been registered
**             by the user.
**
** PARAMETERS: exceptionId    (in)  Exception number.
**             data           (in)  Not used for default exception handler.
**             exceptionInfo  (in)  Information about exception that was raised.
**
** RETURNS:    Nothing.
**
** NOTE:       It is expected that users will register their own exception
**             handler since this default is designed to provide minimal debug
**             information.
**
**             This function does not return.
*****************************************************************************
*/
static void DefaultExceptionHandler
(
   XDRV_EXCEPTION_ID       exceptionId,
   XDRV_EXCEPTION_PARM     data,
   XDRV_EXCEPTION_INFO     exceptionInfo
)
{
   MIPS_CONTEXT_REGS   *context = (MIPS_CONTEXT_REGS *) exceptionInfo;

   EXCEPTION_MIPS_LOG(( "\n\nERROR: Unhandled exception (%d), info: 0x%x, PC: 0x%x!!",
                        exceptionId,
                        (unsigned int) exceptionInfo,
                        context->pc ));


   /*
   ** If we simply return, the exception instruction will execute over and over
   ** again. Not good. We could step over the offending instruction and continue
   ** executing, but that might lead to unexpected/misleading behaviour. Also
   ** not good. Let's just hang out....
   */
   while ( 1 )
   {
      ;
   }
}
