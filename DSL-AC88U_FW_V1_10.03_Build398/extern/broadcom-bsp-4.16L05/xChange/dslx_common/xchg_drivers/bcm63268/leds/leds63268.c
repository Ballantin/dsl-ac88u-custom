/***************************************************************************
*
*  Copyright (c) 2001 - 2002 Broadcom Corporation, All Rights Reserved.
*  Contains proprietary and confidential information.
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*
*        Broadcom Corporation
*        16215 Alton Parkway
*        P.O. Box 57013
*        Irvine, California 92619-7013
*
*  All information contained in this document is Broadcom Corporation
*  company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: leds63268.c
*
****************************************************************************
*
*    Description:  Wrapper functions for the API calls controlling LEDs
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */

#include <leds.h>
#include <bcmChip.h>  /* For controling Serial LED GPIO pins */
#include <board.h>    /* For controlling VoIP and POTS LEDs */

/* ---- Constants ---------------------------------------- */

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
**  RETURNS: 0 on SUCCESS
**
**  NOTES:
**
**********************************************************************************
*/
int xdrvLedCtrl(unsigned int lineId, unsigned int state)
{                                 
    /* Set corresponding LED state */
    kerSysLedCtrl((BOARD_LED_NAME)(lineId + kLedVoip1), (state == 1) ?kLedStateOn :kLedStateOff);

   /* Always successful */
   return ( 0 );
}
