/*****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
******************************************************************************
*
*  zsptimer.h - ZSP on-chip timer definitions
*
*  PURPOSE:
*
*     This file contains functions for utilizing the ZSP core on-chip timers.
*
*  NOTES:
*
*     1. There are two countdown timers, timer0 and timer1.
*
*     2.  tc register
*
*     fields of the timer control register are shown below.
*
*       15       14       13       12       11       10        9        8
*   +--------+--------+-----------------------------------------------------+
*   |   et1  |   cm1  |                      tmrdiv1                        |
*   +--------+--------+-----------------------------------------------------+
*
*        7        6        5        4        3        2        1        0
*   +--------+--------+-----------------------------------------------------+
*   |   et0  |   cm0  |                      tmrdiv0                        |
*   +--------+--------+-----------------------------------------------------+
*
*     etX      - Enable timerX
*                 etX = 0; Disable timerX
*                 etX = 1; Enable timerX
*
*              On reset, et0 = 0
*
*     cmX      - Continuous mode, timer X
*                 cmX = 0; single shot mode for timerX
*                 cmX = 1; continuous mode for timerX
*
*              On reset, cm0 = 0
*
*     tmrdivX  - timerX prescale value. The 6-bit field holds the divide down
*                 value used to prescale the timer.  For a value N, the
*                 divisor is N + 1. For example, if N = 3, timerX decrements
*                 every N + 1 = 4 clock cycles
*
*****************************************************************************/

#ifndef ZSPTIMER_H
#define ZSPTIMER_H

/* ---- Include Files ----------------------------------------------------- */

#include <xdrvTypes.h>

/* ---- Constants and Types ----------------------------------------------- */
/* ---- Variable Externs -------------------------------------------------- */
/* ---- Function Prototypes ----------------------------------------------- */

extern void zspTimer0SetTimerPrescaler( XDRV_UINT16 prescaleValue );
extern XDRV_UINT16 zspTimer0GetTimerPrescaler( void );
extern void zspTimer0SetTimer( XDRV_UINT16 timerValue );
extern XDRV_UINT16 zspTimer0GetTimer( void );
extern XDRV_UINT16 zspTimerGetTCReg( void );
extern void zspTimer1SetTimerPrescaler( XDRV_UINT16 prescaleValue );
extern XDRV_UINT16 zspTimer1GetTimerPrescaler( void );
extern void zspTimer1SetTimer( XDRV_UINT16 timerValue );
extern XDRV_UINT16 zspTimer1GetTimer( void );

/* ---- Macros ------------------------------------------------------------ */

#define zspTimer0IntEnable()        asm("\n   bits    %imask, 5\n");
#define zspTimer0IntDisable()       asm("\n   bitc    %imask, 5\n");
#define zspTimer0Enable()           asm("\n   bits    %tc, 7\n");
#define zspTimer0Disable()          asm("\n   bitc    %tc, 7\n");
#define zspTimer0ContinuousMode()   asm("\n   bits    %tc, 6\n");
#define zspTimer0SingleShotMode()   asm("\n   bitc    %tc, 6\n");

#define zspTimer1IntEnable()        asm("\n   bits    %imask, 6\n");
#define zspTimer1IntDisable()       asm("\n   bitc    %imask, 6\n");
#define zspTimer1Enable()           asm("\n   bits    %tc, 15\n");
#define zspTimer1Disable()          asm("\n   bitc    %tc, 15\n");
#define zspTimer1ContinuousMode()   asm("\n   bits    %tc, 14\n");
#define zspTimer1SingleShotMode()   asm("\n   bitc    %tc, 14\n");

#endif /* ZSPTIMER_H */

