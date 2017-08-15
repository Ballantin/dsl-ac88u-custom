/***************************************************************************
*    Copyright 2000  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: ledSim.c
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      6358VW/VW2 LEDs.
*
****************************************************************************/


/* ---- Include Files ---------------------------------------------------- */

#include "ledSim.h"


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */

/* ---- Functions -------------------------------------------------------- */

/*
**********************************************************************************
**
**  FUNCTION: xdrvLedCtrl
**
**  PURPOSE:
**      A wrapper to the kerSysLedCtrl API call.
**
**  PARAMETERS: line_id = Line ID corresponding to the LED state being modified
**              state   = Desired LED state	(0 => Off, 1 => On)
**				
**  RETURNS:
**
**  NOTES:
**
**********************************************************************************
*/
void xdrvLedCtrl(unsigned int lineId, unsigned int state)
{                                 
    (void)lineId;
	(void)state;
}
