/**********************************************************************************
** Copyright © 2008 Broadcom Corporation
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      This module implements the Timer engine which is used by the CMGR.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* The maximum number of timer needed at any one time is calculated based on
** the actual number of resources present.  We need:
**
**     - up to two timers per user resource.
**     - one timer per session (media or service) resource.
*/
#define CMGRCFG_TIMER_MAX                                                         \
            CMGRCFG_USER_MAX * ( CMGRCFG_MEDSES_MAX + CMGRCFG_SRVSES_MAX + 2 )

/* ---- Private Variables ------------------------------------------------------- */

CMTMRCTL *gpTmrCtl[ CMGRCFG_TIMER_MAX ];

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   cmTmr
**
** PURPOSE:    Manages the timer resources.
**
** PARAMETERS: bCreate - Whether resources should be created (BOS_TRUE) or ended
**                       (BOS_FALSE) when this call is invoked.
**
** RETURNS:    None.
***********************************************************************************/
void cmTmr ( BOS_BOOL bCreate )
{
   BOS_UINT32 uIx;

   if ( bCreate )
   {
      CMGRLOG_INFORMATION (( "cmTmr: creating timer support." ));

      for ( uIx = 0 ; uIx < CMGRCFG_TIMER_MAX ; uIx++ )
      {
         gpTmrCtl[ uIx ] = (CMTMRCTL *) malloc ( sizeof(CMTMRCTL) );
         memset ( gpTmrCtl[ uIx ], 0, sizeof(CMTMRCTL) );

         gpTmrCtl[ uIx ]->uIx = uIx;
         gpTmrCtl[ uIx ]->tmr.uUsr = CMGR_INVALID_HANDLE;
         gpTmrCtl[ uIx ]->tmr.uSes = CMGR_INVALID_HANDLE;
      }
   }
   else
   {
      CMGRLOG_INFORMATION (( "cmTmr: terminating timer support." ));

      for ( uIx = 0 ; uIx < CMGRCFG_TIMER_MAX ; uIx++ )
      {
         free ( gpTmrCtl[ uIx ] );
         gpTmrCtl[ uIx ] = NULL;
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmTmrStart
**
** PURPOSE:    Starts a timer.
**
** PARAMETERS: pTmr - the timer block information.
**
** RETURNS:    None.
***********************************************************************************/
void cmTmrStart ( CMTMR *pTmr )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uTmr = CMGR_INVALID_HANDLE;

   CMGRLOG_DEBUG (( "cmTmrStart: tmr %s, usr 0x%X, ses 0x%X, to 0x%X",
                    cmMapGetStr ( cmMapCmEvent, pTmr->eType ),
                    pTmr->uUsr, pTmr->uSes, pTmr->uTimeOut ));

   /* If the timer has a NULL duration (zero timeout), do not process it.
   **
   ** This could be the case when configuration has returned a zero value
   ** for a timer, which essentially is equivalent to not having to start
   ** it.
   */
   if ( !pTmr->uTimeOut )
   {
      CMGRLOG_DEBUG (( "cmTmrStart: bypass NULL duration timer (0x%X, 0x%X)",
                       pTmr->uUsr,
                       pTmr->uSes ));
      return;
   }

   /* Locate a free block for this resource.
   **
   ** At this time, we do not enforce check on how many timers are really
   ** used by a given user, but we could also do it here if needed.
   */
   for ( uIx = 0 ; uIx < CMGRCFG_TIMER_MAX ; uIx++ )
   {
      if ( ( uTmr == CMGR_INVALID_HANDLE ) &&
           gpTmrCtl[ uIx ] &&
           ( gpTmrCtl[ uIx ]->bUsed == BOS_FALSE ) )
      {
         uTmr = uIx;
      }
   }

   if ( uTmr != CMGR_INVALID_HANDLE )
   {
      if ( gpTmrCtl[ uTmr ]->bUsed )
      {
         CMGRLOG_DEBUG (( "cmTmrStart [%d]: timer overwrite (0x%X, 0x%X)",
                          uTmr,
                          gpTmrCtl[ uTmr ]->tmr.uUsr,
                          gpTmrCtl[ uTmr ]->tmr.uSes ));

         /* Timer overwrite and restart.
         */
         gpTmrCtl[ uTmr ]->tmr.eType = pTmr->eType;
         gpTmrCtl[ uTmr ]->tmr.uTimeOut = pTmr->uTimeOut;
      }
      else
      {
         gpTmrCtl[ uTmr ]->bUsed = BOS_TRUE;

         CMGRLOG_DEBUG (( "cmTmrStart [%d]: new timer (0x%X, 0x%X)",
                          uTmr,
                          pTmr->uUsr,
                          pTmr->uSes ));

         /* New timer.
         */
         gpTmrCtl[ uTmr ]->tmr.eType = pTmr->eType;
         gpTmrCtl[ uTmr ]->tmr.uUsr = pTmr->uUsr;
         gpTmrCtl[ uTmr ]->tmr.uSes = pTmr->uSes;
         gpTmrCtl[ uTmr ]->tmr.uTimeOut = pTmr->uTimeOut;
      }
   }
   else
   {
      CMGRLOG_WARNING (( "cmTmrStart: no free timer for (%s, 0x%X, 0x%X, 0x%X )!!",
                         cmMapGetStr ( cmMapCmEvent, pTmr->eType ),
                         pTmr->uUsr, pTmr->uSes, pTmr->uTimeOut ));
   }
}


/***********************************************************************************
** FUNCTION:   cmTmrStop
**
** PURPOSE:    Stops a timer.
**
** PARAMETERS: pTmr - the timer block information.
**
** RETURNS:    None.
**
** NOTE:       If the timer type passed is 'invalid' (or not set), then this call
**             will stop ALL the timers associated with the corresponding resource
**             at once.
**
**             If the 'session' passed is invalid, then this call will also stop
**             all the timers present for each resources under the given user.
***********************************************************************************/
void cmTmrStop ( CMTMR *pTmr )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_TIMER_MAX ; uIx++ )
   {
      if ( gpTmrCtl[ uIx ] && ( gpTmrCtl[ uIx ]->tmr.uUsr == pTmr->uUsr ) )
      {
         if ( ( pTmr->uSes == CMGR_INVALID_HANDLE ) ||
              (( pTmr->uSes != CMGR_INVALID_HANDLE ) &&
                 ( gpTmrCtl[ uIx ]->tmr.uSes == pTmr->uSes )) )
         {
            if ( ( pTmr->eType == eCMEVT_INVALID ) ||
                 (( pTmr->eType != eCMEVT_INVALID ) &&
                    ( gpTmrCtl[ uIx ]->tmr.eType == pTmr->eType )) )
            {
               gpTmrCtl[ uIx ]->bUsed = BOS_FALSE;

               CMGRLOG_DEBUG ((
                    "cmTmrStop [%d]: tmr %s, usr 0x%X, ses 0x%X, to 0x%X",
                    uIx,
                    cmMapGetStr ( cmMapCmEvent,
                                  gpTmrCtl[ uIx ]->tmr.eType ),
                    gpTmrCtl[ uIx ]->tmr.uUsr,
                    gpTmrCtl[ uIx ]->tmr.uSes,
                    gpTmrCtl[ uIx ]->tmr.uTimeOut ));

               gpTmrCtl[ uIx ]->tmr.uTimeOut = 0;
               gpTmrCtl[ uIx ]->tmr.uUsr = CMGR_INVALID_HANDLE;
               gpTmrCtl[ uIx ]->tmr.uSes = CMGR_INVALID_HANDLE;
            }
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmTmrTick
**
** PURPOSE:    Provide a 'tick' for the timer module..
**
** PARAMETERS: uTickMs - the value in milliseconds of the timer tick.
**
** RETURNS:    Nothing, however following a tick, a (or several) timer(s) may be
**             expired, in which case an event is pushed on the FSM for processing.
***********************************************************************************/
void cmTmrTick ( BOS_UINT32 uTickMs )
{
   BOS_UINT32 uIx;
   CMUSR *pUsr = NULL;

   for ( uIx = 0 ; uIx < CMGRCFG_TIMER_MAX ; uIx++ )
   {
      if ( gpTmrCtl[ uIx ] && gpTmrCtl[ uIx ]->bUsed )
      {
         if ( gpTmrCtl[ uIx ]->tmr.uTimeOut <= uTickMs )
         {
            /* Time out!
            */
            CMGRLOG_DEBUG (( "cmTmrTick [%d]: time out! tmr %s, usr 0x%X, ses 0x%X",
                             uIx,
                             cmMapGetStr ( cmMapCmEvent,
                                           gpTmrCtl[ uIx ]->tmr.eType ),
                             gpTmrCtl[ uIx ]->tmr.uUsr,
                             gpTmrCtl[ uIx ]->tmr.uSes ));

            if ( (pUsr = cmUsrValid ( gpTmrCtl[ uIx ]->tmr.uUsr )) != NULL )
            {
               CMEVT evt;

               /* Construct the event.
               */
               evt.uEvt = gpTmrCtl[ uIx ]->tmr.eType;
               evt.uDat1 = gpTmrCtl[ uIx ]->tmr.uUsr;
               evt.uDat2 = gpTmrCtl[ uIx ]->tmr.uSes;
               evt.pDat3 = NULL;

               /* Push the event on top of the user block.
               */
               cmUsrNewEvent ( pUsr, gpTmrCtl[ uIx ]->tmr.eType );

               /* Copy the event block to the user information.
               */
               memcpy ( &pUsr->evt, &evt, sizeof ( CMEVT ) );
            }

            /* Clean up the timer control block since we do not use it and FSM
            ** run may require to re-use a timer block if something else needs to
            ** be kept track of.
            */
            gpTmrCtl[ uIx ]->bUsed = BOS_FALSE;
            gpTmrCtl[ uIx ]->tmr.uTimeOut = 0;
            gpTmrCtl[ uIx ]->tmr.uUsr = CMGR_INVALID_HANDLE;
            gpTmrCtl[ uIx ]->tmr.uSes = CMGR_INVALID_HANDLE;

            /* Run the correct FSM.
            */
            if ( pUsr != NULL )
            {
               cmFsmRun ( eCMFSM_TMR, (void *) pUsr );
            }
         }
         else
         {
            gpTmrCtl[ uIx ]->tmr.uTimeOut -= uTickMs;
         }
      }
   }
}


/***********************************************************************************
** FUNCTION:   cmTmrIsRunning
**
** PURPOSE:    Checks whether a timer is running.
**
** PARAMETERS: pTmr - the timer identification to lookup.
**
** RETURNS:    BOS_TRUE if the timer is running.
**             BOS_FALSE otherwise.
***********************************************************************************/
BOS_BOOL cmTmrIsRunning ( CMTMR *pTmr )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < CMGRCFG_TIMER_MAX ; uIx++ )
   {
      if ( gpTmrCtl[ uIx ] && ( gpTmrCtl[ uIx ]->tmr.uUsr == pTmr->uUsr ) )
      {
         if ( ( pTmr->uSes == CMGR_INVALID_HANDLE ) ||
              (( pTmr->uSes != CMGR_INVALID_HANDLE ) &&
                 ( gpTmrCtl[ uIx ]->tmr.uSes == pTmr->uSes )) )
         {
            if ( gpTmrCtl[ uIx ]->tmr.eType == pTmr->eType )
            {
               CMGRLOG_DEBUG ((
                    "cmTmrIsRunning [%d]: tmr %s, usr 0x%X, ses 0x%X, to 0x%X",
                    uIx,
                    cmMapGetStr ( cmMapCmEvent,
                                  gpTmrCtl[ uIx ]->tmr.eType ),
                    gpTmrCtl[ uIx ]->tmr.uUsr,
                    gpTmrCtl[ uIx ]->tmr.uSes,
                    gpTmrCtl[ uIx ]->tmr.uTimeOut ));

               return BOS_TRUE;
            }
         }
      }
   }

   CMGRLOG_DEBUG ((
        "cmTmrIsRunning [NOT]: tmr %s, usr 0x%X, ses 0x%X",
        cmMapGetStr ( cmMapCmEvent,
                      pTmr->eType ),
        pTmr->uUsr,
        pTmr->uSes ));
   return BOS_FALSE;
}
