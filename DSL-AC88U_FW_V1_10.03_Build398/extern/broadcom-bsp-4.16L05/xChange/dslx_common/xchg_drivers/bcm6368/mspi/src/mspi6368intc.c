/***************************************************************************
*    Copyright 2001  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16251 Alton Parkway
*             Irvine, California  92618
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */
#include <bcm6368.h>
#include <mspi6368.h>
#include <mspi6368defs.h>
#include <xdrvIntCtrl.h>

/* ---- Public Variables ------------------------------------ */
/* ---- Private Constants and Types -------------------------- */
/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */
/* ---- Functions -------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   mspi6368intc_IrqDisable
**
** PURPOSE:    Disable the MSPI interrupt
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void mspi6368intc_IrqDisable( void )
{
   xdrvIntCtrlDisableInterrupt( INTERRUPT_ID_SPI );
}      


/*
*****************************************************************************
** FUNCTION:   mspi6368intc_IrqEnable
**
** PURPOSE:    Enable the MSPI interrupt
**
** PARAMETERS: none
**
** RETURNS:    none
**
*****************************************************************************
*/
void mspi6368intc_IrqEnable( void )
{
   xdrvIntCtrlEnableInterrupt( INTERRUPT_ID_SPI );
}      


/*
*****************************************************************************
** FUNCTION:   mspi6368intc_intVectIrqIsrInstall
**
** PURPOSE:    Install the MSPI interrupt vector
**
** PARAMETERS: isr - ISR to install
**
** RETURNS:    none
**
*****************************************************************************
*/
void mspi6368intc_intVectIrqIsrInstall( XDRV_INT_CTRL_ISR isr )
{
   xdrvIntCtrlInstallInterrupt( INTERRUPT_ID_SPI, isr, (void *)NULL );
}


