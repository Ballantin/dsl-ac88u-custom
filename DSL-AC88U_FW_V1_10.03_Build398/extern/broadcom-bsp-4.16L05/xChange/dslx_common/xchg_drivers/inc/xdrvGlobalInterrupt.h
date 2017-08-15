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
*    Filename: xdrvGlobalInterrupt.h
*
****************************************************************************
*    Description:
*
*     Architecture independent API for enabling and disabling global CPU interrupts.
*
****************************************************************************/

#ifndef XDRV_GLOBAL_INTERRUPT_H
#define XDRV_GLOBAL_INTERRUPT_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

typedef unsigned int XDRV_GLOBAL_INTERRUPT_LOCK;

/* 
** Structure used to save the register context. We only need to save one register
** for now. The structure can be easily extended in the future. 
*/
typedef struct
{
   unsigned int register1;
}XDRV_REGISTER_SET;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   xdrvGlobalInterruptDisable
**
** PURPOSE:    Disable global interrupts
**
** PARAMETERS: lockKey - pointer to XDRV_GLOBAL_INTERRUPT_LOCK
**
** RETURNS:    nothing
**
** NOTES:      On some OSs locking out interrupts does not prevent rescheduling.
**             Thus, if a task locks out interrupts and invokes kernel services
**             that cause the task to block (e.g., taskSuspend() or taskDelay())
**             or that cause a higher priority task to be ready (e.g., semGive()
**             or taskResume()), then rescheduling occurs and interrupts are
**             unlocked while other tasks run. Therefore, caller has to make
**             sure that no kernel services are used in between the
**             xdrvGlobalInterruptDisable/Restore pair.
**
**             The lockKey value is passed back to xdrvGlobalInterruptRestore to
**             re-enable CPU interrupts.
**
**             Calls to xdrvGlobalInterruptDisable and xdrvGlobalInterruptRestore
**             are nestable.
*****************************************************************************
*/
void xdrvGlobalInterruptDisable( XDRV_GLOBAL_INTERRUPT_LOCK *lockKey );


/*
*****************************************************************************
** FUNCTION:   xdrvGlobalInterruptRestore
**
** PURPOSE:    Restore previous global interrupt state
**
** PARAMETERS: lockKey - input - value set by xdrvGlobalInterruptDisable
**
** RETURNS:    nothing
**
** NOTES:      On some OSs locking out interrupts does not prevent rescheduling.
**             Thus, if a task locks out interrupts and invokes kernel services
**             that cause the task to block (e.g., taskSuspend() or taskDelay())
**             or that cause a higher priority task to be ready (e.g., semGive()
**             or taskResume()), then rescheduling occurs and interrupts are
**             unlocked while other tasks run. Therefore, caller has to make
**             sure that no kernel services are used in between the
**             xdrvGlobalInterruptDisable/Enable pair.
**
**             Calls to xdrvGlobalInterruptDisable and xdrvGlobalInterruptRestore
**             are nestable.
*****************************************************************************
*/
void xdrvGlobalInterruptRestore( XDRV_GLOBAL_INTERRUPT_LOCK *lockKey );


/*
*****************************************************************************
** FUNCTION:   xdrvGlobalInterruptEnable
**
** PURPOSE:    Blindly re-enables interrupts, independent of how many times
**             xdrvGlobalInterruptDisable has been called previously.
**
** PARAMETERS: none
**
** RETURNS:    nothing
**
** NOTES:      Calls to xdrvGlobalInterruptDisable and xdrvGlobalInterruptRestore
**             are nestable and should be used as a function pair to protect
**             critical sections of code.
**
**             This function should really only be used in special situations
**             where interrupts need to be enabled independent of any
**             previous state.
*****************************************************************************
*/
void xdrvGlobalInterruptEnable( void );

/*****************************************************************************
** FUNCTION:   xdrvSaveRegisterState
**
** PURPOSE:    Save register values.
**
** PARAMETERS: registerSet - Location for saving the register values.
**
** RETURNS:    nothing
**
*****************************************************************************/
void xdrvSaveRegisterState( XDRV_REGISTER_SET *registerSet );

/*****************************************************************************
** FUNCTION:   xdrvRestoreRegisterState
**
** PURPOSE:    Restore register values
**
** PARAMETERS: registerSet - Location containing the values that need to be
**                           restored.
**
** RETURNS:    nothing
**
*****************************************************************************/
void xdrvRestoreRegisterState( XDRV_REGISTER_SET *registerSet );

#ifdef __cplusplus
    }
#endif

#endif  /* XDRV_GLOBAL_INTERRUPT_H  */
