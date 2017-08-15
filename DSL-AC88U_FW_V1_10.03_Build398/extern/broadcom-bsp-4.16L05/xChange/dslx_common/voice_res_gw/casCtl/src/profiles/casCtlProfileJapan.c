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
*    Filename: casCtlProfileJapan.c
*
****************************************************************************
*    Description:
*
*      This module contains the static Japanese ring profiles
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */
#include "casCtlProfiles.h"

/* ---- Private Constants and Types -------------------------- */
#if VRG_COUNTRY_CFG_JAPAN
const CASSTM gCasCtlRingPatterns_JAPAN[] =
{
   /*************************************************
   **        Profile[0]: RING - JAPAN IR           **
   *************************************************/
   {
      /* Loop Counter */
      /* Not used for this pattern */
      0,0,

      {
         /* State 0 */
         /* Ring on for 1000 ms (1 second), generate a "ring on" event at start of state */
         {( FXOBITS_JAPAN_ALERTING | CAS_NEXTSTATE_1 | CASEVENT_RINGON | CASBITS_RINGING), 1000},

         /* State 1 */
         /* Ring off for 2 seconds, generate a "ring off" event, loopback to state 0 */
         {( FXOBITS_JAPAN_ALERTING | CAS_NEXTSTATE_0 | CASEVENT_RINGOFF | CASBITS_RLCF ), 2000},

         /* State 2 - Not Used */
         {0,0},
         /* State 3 - Not Used */
         {0,0},
         /* State 4 - Not Used */
         {0,0},
         /* State 5 - Not Used */
         {0,0},
         /* State 6 - Not Used */
         {0,0},
         /* State 7 - Not Used */
         {0,0},
         /* State 8 - Not Used */
         {0,0},
         /* State 9 - Not Used */
         {0,0},
         /* State 10 - Not Used */
         {0,0},
         /* State 11 - Not Used */
         {0,0}
      }
   },
   /***************************************************************************
   **      Profile [6]: Polarity inversion and CAR Ringing for Japanese caller-id.
   **                   The loop polarity must be inverted for 250ms prior to the
   **                   start of CAR ringing. The CAR ring pattern consists of
   **                   a 500ms on, 500ms off pattern. The CAR ring signal must
   **                   not exceed 6 seconds from the start of polarity inversion.
   **
   **                   Note that we don't support the loop-count; therefore,
   **                   we need to unroll the ring on-off loop.
   ****************************************************************************/
   {
      /* Loop Counter */
      /* Not used for this pattern */
      0,0,

      {
         /* State 0 */
         /* Reverse loop polarity for 250ms */
         {( FXOBITS_JAPAN_ALERTING | CAS_NEXTSTATE_1 | CASEVENT_RLCF | CASBITS_RLCF ), 250},

         /* State 1 */
         /* Send event to client informing of timer expiry */
         {( FXOBITS_JAPAN_ALERTING | CAS_NEXTSTATE_2 | CASEVENT_TIMER_EXPIRE | CASBITS_RLCF ), 0},

         /* State 2 */
         /* Ring on for 500 ms (0.5 second), generate a "ring on" event at start of state */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_3 | CASEVENT_RINGON | CASBITS_RINGING), 500},

         /* State 3 */
         /* Ring off for 0.5 seconds, generate a "ring off" event. Note that we don't
         ** have loop-count support, so we need to unroll the ringing loop. */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_4 | CASEVENT_RINGOFF | CASBITS_RLCF ), 500},

         /* State 4 */
         /* Ring on for 500 ms (0.5 second), generate a "ring on" event at start of state */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_5 | CASEVENT_RINGON | CASBITS_RINGING), 500},

         /* State 5 */
         /* Ring off for 0.5 seconds, generate a "ring off" event */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_6 | CASEVENT_RINGOFF | CASBITS_RLCF ), 500},

         /* State 6 */
         /* Ring on for 500 ms (0.5 second), generate a "ring on" event at start of state */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_7 | CASEVENT_RINGON | CASBITS_RINGING), 500},

         /* State 7 */
         /* Ring off for 0.5 seconds, generate a "ring off" event */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_8 | CASEVENT_RINGOFF | CASBITS_RLCF ), 500},

         /* State 8 */
         /* Ring on for 500 ms (0.5 second), generate a "ring on" event at start of state */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_9 | CASEVENT_RINGON | CASBITS_RINGING), 500},

         /* State 9 */
         /* Ring off for 0.5 seconds, generate a "ring off" event */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_10 | CASEVENT_RINGOFF | CASBITS_RLCF ), 500},

         /* State 10 */
         /* Ring on for 500 ms (0.5 second), generate a "ring on" event at start of state */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_11 | CASEVENT_RINGON | CASBITS_RINGING), 500},

         /* State 11 */
         /* Ring off for 0.5 seconds, generate a "ring off" event */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_12 | CASEVENT_RINGOFF | CASBITS_RLCF ), 500},

         /* State 12 */
         /* Ring on for 500 ms (0.5 second), generate a "ring on" event at start of state */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_13 | CASEVENT_RINGON | CASBITS_RINGING), 500},

         /* State 13 */
         /* Ring off for 0.25 seconds, generate a "ring off" event */
         {( FXOBITS_JAPAN_ALERTING_CLID | CAS_NEXTSTATE_14 | CASEVENT_RINGOFF | CASBITS_RLCF ), 250},

         /* State 14 */
         /* 6s from line-reversal expired, generate a "timer expiry" event */
         {( FXOBITS_JAPAN_ALERTING | CAS_ENDING_STATE | CASEVENT_TIMER_EXPIRE | CASBITS_RLCF ), 250},

      }
   }
};
#endif
