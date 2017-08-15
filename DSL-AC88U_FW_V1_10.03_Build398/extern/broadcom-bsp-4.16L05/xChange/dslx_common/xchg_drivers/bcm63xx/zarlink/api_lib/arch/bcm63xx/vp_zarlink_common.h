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
#ifndef VP_ZARLINK_COMMON_H
#define VP_ZARLINK_COMMON_H

#include <bosMutex.h>

extern BOS_MUTEX spiZarMutex; 

/*
******************************************************************************
** FUNCTION:  spiInit
**
** PURPOSE:   Initializes the timer interface
**
** PARAMETERS: None
**
** RETURNS: None
**
*****************************************************************************
*/
void spiZarInit(void);


void spiZarDeinit(void);



#endif
