/***************************************************************************
*    Copyright 2000  Broadcom
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
*    Filename: exceptionHdlrCmtMips.h
*
****************************************************************************
*    Description:
*
*     Interface for a generic exception handler task for a concurrent multi-threaded
*     (CMT) processor. The module will register with the inter-thread processor
*     communication module to be notified when an exception occurs on the
*     other thread processor. This notification will trigger an ISR,
*     which will signal the exception handler task. The task will invoke
*     a user registered callback to perform application specific exception
*     handling.
*
****************************************************************************/

#ifndef EXCEPTION_HDLR_CMT_MIPS_H
#define EXCEPTION_HDLR_CMT_MIPS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvItpc.h>


/* ---- Constants and Types ---------------------------------------------- */

/*
** User register callback, invoked when an remote exception occurs.
*/
typedef void* EXCEPTION_HDLR_CMT_CALLBACK_PARM;
typedef int (*EXCEPTION_HDLR_CMT_CALLBACK)( EXCEPTION_HDLR_CMT_CALLBACK_PARM );


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   exceptionHdlrCmtInit
**
** PURPOSE:    To initialize the CMT exception handler module.
**
** PARAMETERS: callbackFnc    (in)  Callback invoked when exception occurs on
**                                  other thread processor.
**             callbackParam  (in)  Parameter passed to callback function.
**             itpcDrv        (in)  Inter-thread processor communication
**                                  driver used to communication with other
**                                  thread processor.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void exceptionHdlrCmtInit
(
   EXCEPTION_HDLR_CMT_CALLBACK         callbackFnc,
   EXCEPTION_HDLR_CMT_CALLBACK_PARM    callbackParam,
   XDRV_ITPC                          *itpcDrv
);


/*
*****************************************************************************
** FUNCTION:   exceptionHdlrCmtDeinit
**
** PURPOSE:    To de-initialize the CMT exception handler module.
**
** PARAMETERS: None.
**
** RETURNS:    void
**
** NOTE:
*****************************************************************************
*/
void exceptionHdlrCmtDeinit( void );


#ifdef __cplusplus
    }
#endif

#endif  /* EXCEPTION_HDLR_CMT_MIPS_H  */
