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
*    Filename: ledSim.h
*
****************************************************************************
*    Description:
*
*      This file contains defines and functions related to the control of the
*      6358VW/VW2 LEDs.
*
****************************************************************************/

#ifndef LED_SIM_H
#define LED_SIM_H

#if defined(__cplusplus)
extern "C" {
#endif

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
void xdrvLedCtrl(unsigned int lineId, unsigned int state);


#if defined(__cplusplus)
}
#endif

#endif   /* LED_SIM_H */
