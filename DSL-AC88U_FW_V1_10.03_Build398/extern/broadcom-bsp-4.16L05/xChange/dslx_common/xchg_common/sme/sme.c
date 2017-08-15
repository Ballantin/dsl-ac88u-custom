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
*    Filename: sme.c
*
****************************************************************************
*    Description:
*
*     This file contains the implementation of all global functions
*     provided by the State Machine Engine.
*
****************************************************************************/




/* ---- Include Files ---------------------------------------- */

#include <xchgAssert.h>
#include <sme.h>  /* include local header file */


/* ---- Public Variables ------------------------------------- */
/* ---- Private Constants and Types -------------------------- */

/* Maximum number of internal events */
#define MAX_INT_EVENTS  50


/* ---- Private Variables ------------------------------------ */
/* ---- Private Function Prototypes -------------------------- */

static SME_EVENT smeActions (const SME_STATE *statep, void *datap);


/* ---- Functions -------------------------------------------- */

/***********************************************************************
*
*  smeProcess - process an event for a state machine
*
*  PARAMETERS:
*     statep      - [IN] pointer to current state's table
*     datap       - [IN] pointer to user data to be passed to action
*                        functions
*     event       - [IN] event which triggered the state machine
*     transitions - [OUT] number of state transitions
*
*  RETURNS:
*     SME_STATE * - pointer to new state's events table
*
***********************************************************************/
const SME_STATE *smeProcess
(
   const SME_STATE  *statep,
   void             *datap,
   SME_EVENT         event,
   int              *transitions
)
{
   const SME_STATE *curstatep;   /* pointer to current state */
   const SME_STATE *cureventp;   /* pointer to current event entry */

   XCHG_ASSERT (statep);

   curstatep = statep;
   cureventp = curstatep;
   *transitions = 0;

   /* keep processing until no more events */
   while (event != SME_EVENT_NULL)
   {
      /* try to find the event in the list of specified events accepted by
       * the current state
       */
      while (cureventp->event != SME_EVENT_NULL)
      {
         if (cureventp->event == event)
         {  /* match found */
            break;
         }  /* if */
         /* advance to next event */
         cureventp++;
      }  /* while */

      /* was match found? */
      if (cureventp->event != SME_EVENT_NULL)
      {  /* match found */

         /* perform actions, get local event, and advance state */
         event = smeActions (cureventp, datap);
         curstatep = cureventp->nextstatep;
         cureventp = curstatep;

         /* check for excessive number of internal events */
         (*transitions)++;
         XCHG_ASSERT ((*transitions) < MAX_INT_EVENTS);
      }  /* if */
      else
      {  /* no match was found */
         break;
      }  /* else */
   }  /* while */

   /* return the new state */
   return (curstatep);
}  /* smeProcess */


/* Private functions */

/***********************************************************************
*
*  smeActions - perform actions specified in state table
*
*  PARAMETERS:
*     statep      - [IN] pointer to current state's table
*     datap       - [IN] pointer to user data to be passed to action
*                        functions
*
*  RETURNS:
*     int - local event generated, SME_EV_NULL if no local events
*
***********************************************************************/
static SME_EVENT smeActions( const SME_STATE *statep, void *datap )
{
   int   i;          /* action counter */

   /* perform all actions */
   for (i = 0; i < SME_MAX_FUNC; i++)
   {
      int   event;      /* loacl event */

      /* check for any more action */
      if (statep->action[i] == NULL)
      {
         break;
      }  /* if */

      /* perform action and get local event */
      event = (*statep->action[i]) (datap);

      /* stop processing if there is a local event */
      if (event != SME_EVENT_NULL)
      {
         return (event);
      }  /* if */

   }  /* for */

   /* no local event */
   return SME_EVENT_NULL;

}  /* smeActions */
