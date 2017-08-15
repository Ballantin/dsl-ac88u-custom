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
*    Filename: xdrvIntCtrl.h
*
****************************************************************************
*    Description:
*
*     Interface for generic, chip-independent interrupt control module.
*     All chip-specific interrupt controller modules should implement to
*     this common interface.
*
*     This module allows clients to register an interrupt service routine for
*     each interrupt source supported by a specific chip.
*
****************************************************************************/

#ifndef XDRV_INT_CTRL
#define XDRV_INT_CTRL

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/*
** Interrupt id.
*/
typedef unsigned int XDRV_INT_CTRL_INTERRUPT;

/*
** Parameter that is passed to ISR.
*/
typedef void* XDRV_INT_CTRL_ISR_PARM;

/*
** ISR prototype that may be registered for each peripheral interrupt.
*/
typedef unsigned int ( *XDRV_INT_CTRL_ISR )( XDRV_INT_CTRL_ISR_PARM );

/* 
** Structure containing the interrupt ISR and the corresponding parameter 
*/
typedef struct 
{
   XDRV_INT_CTRL_ISR      isrFunction;
   XDRV_INT_CTRL_ISR_PARM isrParm;
} XDRV_INT_CTRL_ISR_INFO;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   xdrvIntCtrlInstallInterrupt
**
** PURPOSE:    To register an interupt service routine for a specific interrupt.
**
** PARAMETERS: interruptId (in)  Interrupt to register. The value of 'interruptId'
**                               will be chip-specific, since each chip supports
**                               a different set of interrupt sources.
**             isrFunc     (in)  ISR that will be invoked when interrupt triggers.
**             isrParm     (in)  Parameter to pass to ISR when invoked.
**
** RETURNS:    Nothing.
**
** NOTE:       This function simply registers the interrupt service routine.
**             It does not enable it. xdrvIntCtrlEnableInterrupt() must be
**             called to enable the ISR.
*****************************************************************************
*/
void xdrvIntCtrlInstallInterrupt
(
   XDRV_INT_CTRL_INTERRUPT interruptId,
   XDRV_INT_CTRL_ISR       isrFunc,
   XDRV_INT_CTRL_ISR_PARM  isrParm
);


/*
*****************************************************************************
** FUNCTION:   xdrvIntCtrlUninstallInterrupt
**
** PURPOSE:    To de-register an interupt service routine for a specific interrupt.
**
** PARAMETERS: interruptId (in)  Interrupt to register. The value of 'interruptId'
**                               will be chip-specific, since each chip supports
**                               a different set of interrupt sources.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvIntCtrlUninstallInterrupt
(
   XDRV_INT_CTRL_INTERRUPT interruptId
);


/*
*****************************************************************************
** FUNCTION:   xdrvIntCtrlEnableInterrupt
**
** PURPOSE:    Enable the specified interrupt.
**
** PARAMETERS: interruptId (in)  Interrupt to enable.
**
** RETURNS:    Nothing.
**
** NOTE:       xdrvIntCtrlInstallInterrupt() must be called to register the
**             actual interrupt service routine.
*****************************************************************************
*/
void xdrvIntCtrlEnableInterrupt( XDRV_INT_CTRL_INTERRUPT interruptId );


/*
*****************************************************************************
** FUNCTION:   xdrvIntCtrlDisableInterrupt
**
** PURPOSE:    Disable the specified interrupt.
**
** PARAMETERS: interruptId (in)  Interrupt to enable.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvIntCtrlDisableInterrupt( XDRV_INT_CTRL_INTERRUPT interruptId );


/*
*****************************************************************************
** FUNCTION:   xdrvIntCtrlGenerateSoftInterrupt
**
** PURPOSE:    Generate the specified software interrupt.
**
** PARAMETERS: interruptId (in)  Software interrupt to generate.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void xdrvIntCtrlGenerateSoftInterrupt( XDRV_INT_CTRL_INTERRUPT interruptId );



#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_INT_CTRL  */
