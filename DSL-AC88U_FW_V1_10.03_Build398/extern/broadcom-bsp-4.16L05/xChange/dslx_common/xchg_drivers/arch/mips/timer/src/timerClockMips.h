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
*    Filename: timerClockMips.h
*
****************************************************************************
*    Description:
*
*     Interface for MIPS timer control functions using the CP0 count
*     and compare registers.
*
****************************************************************************/

#ifndef TIMER_CLOCK_MIPS_H
#define TIMER_CLOCK_MIPS_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   timerResetMips
**
** PURPOSE:    To re-initialize the MIPS core timers to generate the next
**             interrupt.
**
** PARAMETERS: period   (in)  Number of cycles after which a timer interrupt
**                            will be generated. The period must be calculated
**                            based upon the fact that the count register
**                            increments at half the CPU clock frequency.
**
** RETURNS:    Nothing.
**
** NOTE:       void timerMipsReset( unsigned int period );
*****************************************************************************
*/
void timerResetMips( unsigned int period );



/*
*****************************************************************************
** FUNCTION:   timerGetCountMips
**             timerSetCountMips
**             timerGetCompareMips
**             timerSetCountMips
**
** PURPOSE:    Accessor functions for MIPS CP0 count and compare registers.
**
** PARAMETERS:
**
** RETURNS:
**
** NOTE:
*****************************************************************************
*/
unsigned int timerGetCountMips( void );
void         timerSetCountMips( unsigned int );
unsigned int timerGetCompareMips( void );
void         timerSetCompareMips( unsigned int );


#ifdef __cplusplus
    }
#endif

#endif  /* TIMER_CLOCK_MIPS_H  */
