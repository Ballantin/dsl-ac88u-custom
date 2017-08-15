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
*    Filename: interface_driver_si_common.h
*
****************************************************************************
*    Description:
*
*      This file contains functions related to the timer interface for
*      proslic api
*
****************************************************************************/
#ifndef INTERFACE_DRIVER_SI_COMMON_H
#define INTERFACE_DRIVER_SI_COMMON_H

#include <bosMutex.h>

extern BOS_MUTEX spiSiMutex; 

/*
******************************************************************************
** FUNCTION:  TimerInit
**
** PURPOSE:   Initializes the timer interface
**
** PARAMETERS: None
**
** RETURNS: None
**
*****************************************************************************
*/
void spiSiInit(void);


void spiSiDeinit(void);



#endif
