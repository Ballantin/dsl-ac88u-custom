/*****************************************************************************
*
*  Copyright (c) 2006 Broadcom, All Rights Reserved.
*  Contains proprietary and confidential information.
*
*  No portions of this material may be reproduced in any form without the
*  written permission of:
*
*        Broadcom
*        16215 Alton Parkway
*        P.O. Box 57013
*        Irvine, California 92619-7013
*
*  All information contained in this document is Broadcom
*  company private, proprietary, and trade secret.
*
*****************************************************************************/

/****************************************************************************/
/**
*  @file    leds.h   
*
*  This file contains protoypes for wrappers to the API controlling the LEDs
*/
/****************************************************************************/

#ifndef LEDS_H
#define LEDS_H

#ifdef __cplusplus
extern "C"
{
#endif

/* ---- Public Variables -------------------------------------------------- */
/* ---- Public Function Prototypes ---------------------------------------- */
int xdrvLedCtrl(unsigned int lineId, unsigned int state);

#ifdef __cplusplus
}
#endif

#endif
