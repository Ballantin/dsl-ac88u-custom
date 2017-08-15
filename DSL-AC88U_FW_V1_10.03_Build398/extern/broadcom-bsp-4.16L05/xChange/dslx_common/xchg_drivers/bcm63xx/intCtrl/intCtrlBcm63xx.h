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
*    Filename: intCtrlBcm63xx.h
*
****************************************************************************
*    Description:
*
*     Defines an interface to the Cable Modem Board Support Package (BSP)
*     for registering interrupts.
*
*     All functions declared in this file must be implemented by the CM BSP.
*
****************************************************************************/

#ifndef INT_CTRL_BCM_63XX_H
#define INT_CTRL_BCM_63XX_H

#if __cplusplus
extern "C" {
#endif

#include <63xx_intr_wrap.h>

//#define  INTERRUPT_ID_DSP_RX_CHAN            3
//#define  INTERRUPT_ID_DSP_TX_CHAN            4
//#define  INTERRUPT_ID_VPM_BIT_CHANGED_IRQ    112
//#define  INTERRUPT_ID_VPM_MPI_DATA_RDY_IRQ   113
//#define  INTERRUPT_ID_EXTERNAL_3             203


/*
*****************************************************************************
** FUNCTION:   BcmHalMapInterruptWithHandle
**
** PURPOSE:    Maps a handler to specific CPU interrupt
**
** PARAMETERS: pfunc       - pointer to handler routine
**             pUserHandle - user handle that will passed as a parameter to the
**                           ISR handler.
**             interruptId - interrupt id
**
** RETURNS:    0 on success, otherwise -1
**
** NOTES:      This routine is implemented within Board Support Package
**
*****************************************************************************
*/
extern unsigned long BcmHalMapInterruptWithHandle
(
   unsigned long   (*pfunc)(XDRV_INT_CTRL_ISR_PARM),
   void             *pUserHandle,
   unsigned long     interruptId
);


/*
*****************************************************************************
** FUNCTION:   BcmHalSetInterruptParm
**
** PURPOSE:
**
** PARAMETERS:
**
** RETURNS:
**
** NOTE:       Temporary work-around until this added into standard CM release.
*****************************************************************************
*/
extern void BcmHalSetInterruptParm( void *interruptParm, unsigned long interruptId );

/*
*****************************************************************************
** FUNCTION:   BcmHalInterruptEnable
**
** PURPOSE:    Enables specific CPU interrupt
**
** PARAMETERS: interruptId - interrupt id
**
** RETURNS:    nothing
**
** NOTES:      This routine is implemented within Board Support Package
**
*****************************************************************************
*/
//extern void BcmHalInterruptEnable(unsigned long interruptId);

/*
*****************************************************************************
** FUNCTION:   BcmHalInterruptDisable
**
** PURPOSE:    Disables specific CPU interrupt
**
** PARAMETERS: interruptId - interrupt id
**
** RETURNS:    nothing
**
** NOTES:      This routine is implemented within Board Support Package
**
*****************************************************************************
*/
//extern void BcmHalInterruptDisable(unsigned long  interruptId);

/*
*****************************************************************************
** FUNCTION:   BcmHalGenerateSoftInterrupt
**
** PURPOSE:    Trigger the specified software interrupt.
**
** PARAMETERS: interruptId - interrupt id
**
** RETURNS:    nothing
**
** NOTES:      This routine is implemented within Board Support Package
**
*****************************************************************************
*/
extern void BcmHalGenerateSoftInterrupt( unsigned long interruptId );


#if __cplusplus
}
#endif

#endif   /* INT_CTRL_BCM_33XX_H */
