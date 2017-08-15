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
*    Filename: xdrvException.h
*
****************************************************************************
*    Description:
*
*     Interface for generic, architecture-independent exception module.
*     All architecture-specific exception modules should implement to
*     this common interface.
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

#ifndef XDRV_EXCEPTION_H
#define XDRV_EXCEPTION_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/*
** Exception number.
*/
typedef unsigned int XDRV_EXCEPTION_ID;

/*
** User supplied parameter that is passed to exception handler.
*/
typedef void* XDRV_EXCEPTION_PARM;

/*
** Information about exception that was raised.
*/
typedef void* XDRV_EXCEPTION_INFO;

/*
** Exception handler prototype that may be registered for each type of exception.
*/
typedef void ( *XDRV_EXCEPTION_HANDLER )
(
   XDRV_EXCEPTION_ID,
   XDRV_EXCEPTION_PARM,
   XDRV_EXCEPTION_INFO
);


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   xdrvExceptionSetHandler
**
** PURPOSE:    To register an exception handler for a specific exception type.
**
** PARAMETERS: exceptionId          (in)  Exception to register. The value of
**                                        'exceptionId' will be chip architecture
**                                        specific.
**             newExceptionHandler  (in)  Exception handler to register.
**             newExceptionParm     (in)  Parameter to pass to exception when invoked.
**             oldExceptionHandler  (out) Previously registered exception handler.
**             oldExceptionParm     (out) Previously registered parameter.
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
);


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
);


/*
*****************************************************************************
** FUNCTION:   xdrvExceptionResetHandler
**
** PURPOSE:    To de-register the user specified exception handler, and revert
**             to the default exception handler (for the specified exception
**             type only).
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
);


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
void xdrvExceptionResetAllHandlers( void );


#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_EXCEPTION_H  */
