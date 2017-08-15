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
*    Filename: timerMips.h
*
****************************************************************************
*    Description:
*
*     Interface for architecture specific intialization of MIPS timer module.
*     Use the architecture independent API defined by xdrvTimer.h to control
*     the actual timers.
*
****************************************************************************/

#ifndef TIMER_MIPS_H
#define TIMER_MIPS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTimer.h>


/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   timerInitMips
**
** PURPOSE:    Initialize the MIPS timer module.
**
** PARAMETERS: cpuClockFreqHz (in)  MIPS clock frequency in hertz.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void timerInitMips( unsigned int cpuClockFreqHz );


/*
*****************************************************************************
** FUNCTION:   timerDeinitMips
**
** PURPOSE:    De-initializes the MIPS timer module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void timerDeinitMips( void );



#ifdef __cplusplus
    }
#endif

#endif  /* TIMER_MIPS_H  */
