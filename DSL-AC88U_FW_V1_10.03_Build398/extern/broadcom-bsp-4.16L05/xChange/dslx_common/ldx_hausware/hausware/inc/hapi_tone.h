/****************************************************************************
*
*  HAPI_TONE.H - HausWare Tone Generation and Detection Global Definitions
*
*  PURPOSE:
*     This file contains the HAUS Global definitions for tone detection
*
**********************************************************************/

#ifndef HAPI_TONE_H                       /* Prevents multiple includes */
#define HAPI_TONE_H

#ifndef HAPI_TYPES_H
#include <hapi_types.h>
#endif

/*
*****************************************************************************
** GLOBAL CONSTANTS
*****************************************************************************
*/

/*
** Edge detection type.  Defines which edge of a signal that detection should
** begin on.
*/
typedef enum
{
   HSRISINGEDGE = 0,
   HSFALLINGEDGE,
   HSBOTHEDGES

}  HSETRIGGER;

/* DTMF Digit Button */
typedef enum
{
   DTMFDDGT0 = 0,
   DTMFDDGT1 = 1,
   DTMFDDGT2 = 2,
   DTMFDDGT3 = 3,
   DTMFDDGT4 = 4,
   DTMFDDGT5 = 5,
   DTMFDDGT6 = 6,
   DTMFDDGT7 = 7,
   DTMFDDGT8 = 8,
   DTMFDDGT9 = 9,
   DTMFDDGTSTR = 10,
   DTMFDDGTPND = 11,
   DTMFDDGTA   = 12,
   DTMFDDGTB   = 13,
   DTMFDDGTC   = 14,
   DTMFDDGTD   = 15,
   HSDTMFDMAXDGT = 16

}  HSEDTMFDDGT;

/* CPR Indexes to Profiles defined in pteprofc.c  */
typedef enum
{
   CPRDIALTONE = 0,       /* Continuous dial tone */
   CPRSTUTTERDIALTONE,    /* 350&440 100 ms on, 100ms off, contionus on */
   CPRMWITONE,            /* 350&440 100 ms on, 100 ms off, continous on */
   CPRCONFIRMATION,       /* 350&440 100 ms on 100ms off 5 times  */
   CPRAUDIBLERING1,       /* 440&480 1s on 3s off */
   CPRAUDIBLERING2,       /* 440&480 2s on 4s off */
   CPRBUSY,               /* 480&620 500ms on 500ms off */
   CPRREORDER,            /* 480&620 250ms on 250ms off */
   CPRROH                /* 1400, 2060, 2450, 2600 100ms on, 100ms off*/
} HSECPRINDEX;


#endif      /* if HAPI_TONE_H not defined */


