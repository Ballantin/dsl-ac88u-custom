/***************************************************************************
*    Copyright 2009  Broadcom
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
*    Filename: boardHalGpio.h
*
****************************************************************************
*    Description:
*
*      This file contains a hardware-independent public interface for the
*      GPIO Interface (GPIO) driver present on a board
*
****************************************************************************/

#ifndef BOARD_HAL_GPIO_H
#define BOARD_HAL_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   boardHalGpioInit
**
** PURPOSE:    Initialize the MSPI drivers.
**
** PARAMETERS: 
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalGpioInit( void );

#ifdef BRCM_6838
/*****************************************************************************
*
* FUNCTION:   boardHalGpioSetPinMux
*
* PURPOSE:    Sets pinmuxing for gpios
*
* PARAMETERS: pinMuxSel - Pinmux selection setting
*             gpioNum   - Gpio to set pinmux for
* RETURNS:    Nothing
*
*****************************************************************************/
void boardHalGpioSetPinMux( VRG_UINT32 pinMuxSel, VRG_UINT32 gpioNum );
#endif /* BRCM_6838 */

#ifdef BRCM_63381
/*****************************************************************************
*
* FUNCTION:   boardHalGpioSetPinMux
*
* PURPOSE:    Sets pinmuxing for gpios
*
* PARAMETERS: gpioNum   - Gpio to set pinmux for
* RETURNS:    pinMuxSel - Pinmux selection setting
*
*****************************************************************************/
VRG_UINT32 boardHalGpioGetPinMuxSel( VRG_UINT32 gpioNum );
#endif /* BRCM_63381 */

/*
*****************************************************************************
** FUNCTION:   boardHalGpioDeinit
**
** PURPOSE:    De-initialize the MSPI drivers.
**
** PARAMETERS: none
**
** RETURNS:    0 on success, else 1
**
*****************************************************************************
*/
int boardHalGpioDeinit( void );


#ifdef __cplusplus
}
#endif

#endif  /* BOARD_HAL_GPIO_H  */
