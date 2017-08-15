/***************************************************************************
*    Copyright © 2000-2009 Broadcom Corporation                                        
*                                                                           
*    This program is the proprietary software of Broadcom Corporation and/or
*    its licensors, and may only be used, duplicated, modified or           
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized   
*    License").  Except as set forth in an Authorized License, Broadcom    
*    grants no license (express or implied), right to use, or waiver of any 
*    kind with respect to the Software, and Broadcom expressly reserves all 
*    rights in and to the Software and all intellectual property rights     
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO 
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM   
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.                               
*                                                                           
*                                                                           
*    Except as expressly set forth in the Authorized License,               
*                                                                           
*    1.     This program, including its structure, sequence and             
*    organization, constitutes the valuable trade secrets of Broadcom, and  
*    you shall use all reasonable efforts to protect the confidentiality    
*    thereof, and to use this information only in connection with your use  
*    of Broadcom integrated circuit products.                               
*                                                                           
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,          
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR  
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY        
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,    
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,    
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR         
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT  
*    OF USE OR PERFORMANCE OF THE SOFTWARE.                                 
*                                                                           
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL       
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, 
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN  
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR    
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE 
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: casCtlStateMachine.c
*
****************************************************************************
*    Description:
*
*      This module contains the functions of the HAUSWARE Channel
*      Associated Signaling service.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */

#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <bosCritSect.h>
#include <casCtlCfg.h>
#include <casCtl.h>
#include <xdrvSlic.h>
#include <xdrvDaa.h>
#include <xdrvApm.h>
#include <stdlib.h>
#include "casCtlStateMachine.h"             /* CAS internal header */
#include "casCtlStateMachineCfg.h"

/* ---- Private Constants and Types -------------------------- */

/* Debounce 80ms after a flash */
#define FXO_POSTFLASH   80

/* Used in FXS channel debouncing */
#define FXS_DEBOUNCE    10

#define CAS_DAADEGLITCH 0

/* ---- Private Function Prototypes -------------------------- */
static void casInitJitterBuf( CASCHANNELINFO *channelinfop );
static void casFlushJitterBuf( CASCHANNELINFO *channelinfop );
static void casProcess( VRG_UINT16 cas, CASCHANNELINFO *channelinfop, CASCALLBKP callbackp );
static void casMonitor( CASCHANNELINFO *channelinfop, CASCALLBKP callbackp );
static void casMonitorFXO( CASCHANNELINFO *channelinfop, CASCALLBKP callbackp );
#if CAS_CTL_PSTN
static void casMonitorFXS( CASCHANNELINFO *channelinfop, CASCALLBKP callbackp );
static VRG_UINT16 casUpdateFXS( CASCHANNELINFO *casstatep );
#endif
static void casChangeControlState( CASCHANNELINFO *channelinfop, CASCALLBKP callbackp, VRG_UINT32 state );
static void casGenEvent( CASCALLBKP callbackp, VRG_UINT16 evtcode, VRG_UINT16 op1, VRG_UINT16 op2 );
static void casSetSlicState( CASCHANNELINFO *channelinfop, XDRV_SLIC_MODE slicMode );
static VRG_UINT16 casUpdate( CASCHANNELINFO *casstatep );
static VRG_UINT16 casUpdateFXO( CASCHANNELINFO *casstatep );
static void casProcessSlicCommands( CASCMD *cmdp );
static void casProcessDaaCommands( CASCMD *cmdp );
static void casProcessApmCommands( CASCMD *cmdp );

/* ---- Private Variables ------------------------------------ */
/* ---- Functions -------------------------------------------- */

/****************************************************************************
*
*  casIO - Channel Associated Signaling IO command parser
*
*  PURPOSE:
*
*     This function supports IO commands for this service.
*
*  PARAMETERS:
*     localp => the instance memory of this channel
*     cmdp   => the received command
*
*  RETURNS:
*     CAS_CTL_SUCCESS or CAS_CTL_FAILURE
*
*  NOTE:
*
****************************************************************************/
CAS_CTL_STATUS casIO ( CASLOCLMEM *localp, CASCMD *cmdp )
{
   CAS_CTL_STATUS     result;
   CAS_CTL_FXO_INTERFACE_STATE state;
   CAS_CTL_FXO_INTERFACE_STATE nextState;

   /* Assume result is successful until proven otherwise */
   result = CAS_CTL_SUCCESS;

   /*
   ** Action dependent upon specific command
   */
   switch ( cmdp->cmd )
   {
      case CAS_ENABLE:
      {
         if ( cmdp->op1 == VRG_TRUE )
         {
            /* Enable the CAS Service */

            if ( (localp->status & CAS_ENABLED) != CAS_ENABLED )
            {
               /* reset jitter buffer queue */
               casInitJitterBuf( &localp->channelinfo );

               /*
               ** Configure the pte status reg and algorithms based on the
               ** configuration register
               */
               /* init the status reg to 0 */
               localp->status = 0;

               /* set the CAS task flag */
               localp->status |= CAS_ENABLED;
            }
         }
         else if (localp->status & CAS_ENABLED)
         {
            /* if destroy fails, return CAS_CTL_FAILURE */
            /* disable CAS task flag */
            localp->status &= ~CAS_ENABLED;

            /* flush jitter buffer by freeing all packets */
            casFlushJitterBuf( &localp->channelinfo );
         }
      }
      break;

      case CAS_STATECTL:
      {
         /*
         ** Note: only put in jitter buffer if the CAS service
         ** has been enabled.
         */

         if (localp->status & CAS_ENABLED)
         {
            /***** Critical section begins *****/

            bosCritSectEnter( &localp->critSect );

            state = (CAS_CTL_FXO_INTERFACE_STATE) (((localp->channelinfo.state) & CAS_STATEBITS_MASK) >> CAS_STATEBITS_SHIFT);
            nextState = (CAS_CTL_FXO_INTERFACE_STATE)((((CASPROFILE *)cmdp->op2.ref)->casstm.state[0].stateinfo & CAS_STATEBITS_MASK) >> CAS_STATEBITS_SHIFT);
            if ( (localp->channelinfo.castype == CAS_TYPE_FXO) 
                 && (state == CAS_CTL_FXO_ADDRESSING || state == CAS_CTL_FXO_ACTIVE) 
                 && (nextState == CAS_CTL_FXO_IDLE))
            {
               /*  Disallow switching from ACTIVE/ADDRESSING to IDLE state for FXO interface as this doesn't make much sense */
               free(cmdp->op2.ref);
               bosCritSectLeave( &localp->critSect );
               //VRG_LOG(("CAS_STATECTL ignored: line %d curr %X, next %X\n", localp->channelinfo.channelnum, state, nextState));
               return(CAS_CTL_SUCCESS);
            }

            if (localp->channelinfo.profilep != NULL)
            {
               /* Free the previous packet */
               free(localp->channelinfo.profilep);
            }

            /* Store the new pointer for thread usage */

            localp->channelinfo.profilep = cmdp->op2.ref;

            /* Reset the state machine */
            if ( localp->channelinfo.castype == CAS_TYPE_FXO && xdrvCasHistoryEnabled( localp->channelinfo.casDriver ) )
            {
               localp->channelinfo.counter = xdrvCasHistoryRateMsec( localp->channelinfo.casDriver );
            }
            else
            {
               localp->channelinfo.counter = CAS_CTL_CFG_TASK_RATE_MS;
            }

            if ( (localp->channelinfo.state & CAS_STATEBITS_MASK) !=
                 ( ((CASPROFILE *)cmdp->op2.ref)->casstm.state[0].stateinfo & CAS_STATEBITS_MASK) )
            {
               /* Update the state in use */
               localp->channelinfo.substate = 0;
               localp->channelinfo.currentCount = 0;
               if( (localp->channelinfo.state & CAS_STATEBITS_MASK) == FXOBITS_IDLE )
               {
                  /* Remove glitches caused by L9500 SLIC offhook to onhook transition. */
                  localp->channelinfo.deglitchCount = CAS_CTL_DEFAULT_FXO_DEGLITCH_MSEC;
               }
            }

            localp->channelinfo.state = ((localp->channelinfo.state
                                        & (CAS_STATEBITS_MASK + CAS_ABCD_MASK))
                                        | CAS_NEXTSTATE_0);

            bosCritSectLeave( &localp->critSect );

            /***** Critical section ends *****/
         }
         else
         {
            return ( CAS_CTL_FAILURE );
         }
      }
      break;

      case CAS_CONFIG:
      {
         BOS_STATUS  status;

         status = bosCritSectCreate( &localp->critSect );
         XCHG_ASSERT( status == BOS_STATUS_OK );

         bosCritSectEnter( &localp->critSect );

         /* OP1 contains the type, OP2 contains the handle */
         localp->channelinfo.castype = cmdp->op1;
         localp->channelinfo.channelnum = cmdp->op2.val;

         /*
         ** Let the CAS startup in IDLE state with STARTUP_IDLE substate
         ** and don't let it blindly change the channel state into IDLE mode.
         ** Instead, let it run through the state machine, debounce the initial
         ** channel state, and set it to whatever state corresponds to the current
         ** mode. This removes the unncessary assuption that the channel is in IDLE
         ** upon CAS startup, and removes transient conditions on the line.
         */
         
         /*
         ** Let the CAS startup in IDLE state with STARTUP_IDLE substate
         ** and don't let it blindly change the channel state into IDLE mode.
         ** Instead, let it run through the state machine, debounce the initial
         ** channel state, and set it to whatever state corresponds to the current 
         ** mode. This removes the unncessary assuption that the channel is in IDLE
         ** upon CAS startup, and removes transient conditions on the line.
         */          
         localp->channelinfo.state = 
            ((cmdp->op1 == CAS_TYPE_FXS) ? FXSBITS_RELEASED : FXOBITS_IDLE);
         localp->channelinfo.substate = FXO_STARTUP_IDLE;
         localp->channelinfo.statechange = VRG_FALSE;

         bosCritSectLeave( &localp->critSect );
      }
      break;

      case CAS_CONFIG_FXO:
      {
         BOS_STATUS          status;
         CAS_CTL_FXO_PARMS   *fxoParms;

         status = bosCritSectCreate( &localp->critSect );
         XCHG_ASSERT( status == BOS_STATUS_OK );

         bosCritSectEnter( &localp->critSect );

         /* Set Fxo Threshholds from pointer in OP2 */
         fxoParms = (CAS_CTL_FXO_PARMS*)cmdp->op2.ref;
         localp->channelinfo.fxoParms = *fxoParms;

         bosCritSectLeave( &localp->critSect );

         /* Free the FXO parms structure */
         free(cmdp->op2.ref);
      }
      break;

      case CAS_EXIT:
      {
         bosCritSectDestroy( &localp->critSect );
      }
      break;

      case CAS_GET_LINESTATUS:
      {
         CASSTATUS *statusp = cmdp->op2.ref;

         /* Fill in line status information */
         statusp->state = localp->channelinfo.state;
         statusp->currentCAS = localp->channelinfo.currentCAS;
         statusp->currentCount = localp->channelinfo.currentCount;
         statusp->deglitchCount = localp->channelinfo.deglitchCount;
         statusp->substate = localp->channelinfo.substate;
      }
      break;

      case CAS_SLIC_CMD:
      {
         casProcessSlicCommands(cmdp);
      }
      break;

      case CAS_DAA_CMD:
      {
         casProcessDaaCommands(cmdp);
      }
      break;

      case CAS_APM_CMD:
      {
         casProcessApmCommands(cmdp);
      }
      break;

      default:
      {
         /* unknown command */
         return (CAS_CTL_FAILURE);
      }
      break;
   }
   return ( result );
}

/****************************************************************************
*
*  casBlk - CAS Block Processing Routine
*
*  PURPOSE:
*     This function performs block signal processing for the CAS Service.
*
*  PARAMETERS:
*     srvp     => the instance of the service
*
*  RETURNS:
*
*  NOTE:
*     This routine is invoked automatically from thread created during enable.
*     Make sure all initialization is done BEFORE the thread create!
*
****************************************************************************/
void casBlk( CASSRV *srvp )
{
   VRG_UINT16 casbits_rx;     /* Received signalling bits */

   /* get temporary pointer to the instance memory */
   CASLOCLMEM *localp = &(srvp->localmem);

   /* Verify enabled */
   if( ! ( localp->status & CAS_ENABLED ))
   {
      return;
   }

   /************************************************************************
   *                          UPDATE STAGE                                *
   ************************************************************************/

   /*
   ** Call the HAL to control the external hardware and return the
   ** monitored status of all the channels.
   */
   casbits_rx = casUpdate( &localp->channelinfo );

   /************************************************************************
   *                          PROCESS STAGE                                *
   ************************************************************************/

   casProcess( casbits_rx, &localp->channelinfo, srvp->callbackp );
}


/****************************************************************************
*
*  casProcess - Process the CAS bits.
*
*  PURPOSE:
*     This function processes the CAS and performs the resulting action(s).
*
*  PARAMETERS:
*     casbits        => new CAS bits
*     channelinfop   => pointer to channel information
*
*  RETURNS:
*     Updated channel info/state information, plus possible events.
*
*  NOTE:
*
****************************************************************************/
static void casProcess( VRG_UINT16 casbits, CASCHANNELINFO *channelinfop, CASCALLBKP callbackp )
{
   CASPROFILE *profilep;
   CASCHANNELINFO *channelp;
   XDRV_CAS *casDriver;
   VRG_UINT16 statenum;
   VRG_UINT16 blockrate;

   /* Get a pointer to this channel's info */
   channelp = channelinfop;
   casDriver = channelp->casDriver;

   /* Get the history rate */
   if ( channelp->castype == CAS_TYPE_FXO 
        && xdrvCasHistoryEnabled( casDriver ) )
   {
      blockrate = xdrvCasHistoryRateMsec( casDriver );
   }
   else
   {
      blockrate = CAS_CTL_CFG_TASK_RATE_MS;
   }

   /************************************************************************
   *                         MONITOR STAGE                                 *
   ************************************************************************/

   /* Monitor ports, except FXO ports where service is denied */
   if (( channelp->currentCount < CAS_STANBDY_THRESH ) &&
       ( channelp->ignoreHookState == CAS_IGNOREHOOKSTATE_OFF ) &&
       !((channelp->castype == CAS_TYPE_FXO) &&
         (( channelp->state & CAS_STATEBITS_MASK ) 
          == FXOBITS_SERVICEDENIED )))
   {
      /* Increment the current Count*/
      channelp->currentCount += blockrate;

      /* Remove glitches caused by L9500 SLIC offhook to onhook transition. */
      if(channelp->deglitchCount > blockrate)
      {
         channelp->deglitchCount -= blockrate;
      }
      else
      {
         channelp->deglitchCount = 0;
      }

      /* Every 64 ms run the monitor state machine */
      if (( channelp->currentCount >= CAS_MONITOR_RATE ) &&
          ( channelp->currentCAS == casbits ))
      {
         casMonitor( channelp, callbackp );
      }
   }

   /************************************************************************
   *                         DEBOUNCE STAGE                                *
   ************************************************************************/

   /* Debounce the CAS bits, except for FXO ports where service is denied */
   if ((channelp->castype == CAS_TYPE_FXO) &&
       ( channelp->currentCAS != casbits ) &&
       ( channelp->ignoreHookState == CAS_IGNOREHOOKSTATE_OFF ) &&
       ( ( channelp->state & CAS_STATEBITS_MASK ) 
          != FXOBITS_SERVICEDENIED ))
   {
      /* CAS bits are different than current CAS state */
      channelp->newCount += blockrate;

      /* Debounce debounceCount ms worth of data */
#if CAS_CTL_DEFAULT_FXO_DEBOUNCE_MSEC > 0
      if (channelp->newCount >= CAS_CTL_DEFAULT_FXO_DEBOUNCE_MSEC)
#endif
      {
         /* Debounce period ended */
         channelp->currentCount = CAS_CTL_DEFAULT_FXO_DEBOUNCE_MSEC;
         channelp->currentCAS = casbits;

         /* Run Monitor State machine */
         casMonitor( channelp, callbackp );

         channelp->newCount = 0;
      }
   }
   else if ( (channelp->castype == CAS_TYPE_FXS) &&
        (channelp->currentCAS != casbits) )
   {
      /* CAS bits are different than current CAS state */
      channelp->newCountFxs += blockrate;

      /* Debounce FXS_DEBOUNCE ms worth of data */
      if (channelp->newCountFxs >= FXS_DEBOUNCE)
      {
         /* Debounce period ended */
         channelp->currentCount = FXS_DEBOUNCE;
         channelp->currentCAS = casbits;

         /* Run Monitor State machine */
         casMonitor( channelp, callbackp );

         channelp->newCountFxs = 0;
      }
   }
   else
   {
      channelp->newCount = 0;
      channelp->newCountFxs = 0;
   }
   
   if ( channelp->ignoreHookState > blockrate )
   {
      channelp->ignoreHookState -= blockrate;
   }
   else
   {
      channelp->ignoreHookState = CAS_IGNOREHOOKSTATE_OFF;
   }

   /************************************************************************
   *                          ACTION STAGE                                 *
   ************************************************************************/

   /* Get the profile for this channel */
   profilep = channelp->profilep;

   /************************************************************************
   *                     CONTROL STATE MACHINE                             *
   ************************************************************************/

   if (profilep != NULL)
   {
      /* There is a state machine to run for this channel */

      channelp->counter -= blockrate;

      if (channelp->counter <= 0)
      {
         /* State complete, change states */

         if ((channelp->state & CAS_ENDING_STATE) == CAS_ENDING_STATE)
         {
            /* Profile complete */

            if( profilep != NULL )
            {
               /* Free the packet */
               free(profilep);
            }

            /* NULL the pointer */
            channelp->profilep = NULL;

            /* Notify brief ring completion if we've just generated a brief ring */
            if ( ( channelp->state & CASEVENT_BRIEF_RING ) == CASEVENT_BRIEF_RING )
            {
               /* Generate an event to client */
               casGenEvent( callbackp,
                            CAS_CTL_BRIEF_RING_COMPLETE_EVENT,
                            0,
                            0 );
            }
         }
         else
         {
            VRG_UINT32 state;

            /* Goto next state in profile */
            statenum = (VRG_UINT16)((channelp->state & CAS_NEXTBITS_MASK) >> CAS_NEXTBITS_SHIFT);

            if (channelp->state & CAS_LOOP_FLAG)
            {
               profilep->casstm.loopcount--;

               if (profilep->casstm.loopcount == 0)
               {
                  statenum = profilep->casstm.afterloop;
               }
            }

            /* Flag that a state change has occured */
            channelp->statechange = VRG_TRUE;

            /* Update the counter for next state */
            channelp->counter = profilep->casstm.state[statenum].duration;

            state = profilep->casstm.state[statenum].stateinfo;

            if ((( channelp->state & CASBITS_MASK ) != CASBITS_RINGING ) &&
                (( channelp->state & CASBITS_MASK ) != CASBITS_LCFO )    && 
                (( channelp->state & CAS_STATEBITS_MASK ) != FXOBITS_SERVICEDENIED ))
            {
               /* Store the previous state */
               channelp->previousActiveState = channelp->state;
            }

            /* Update the state in use */
            channelp->state = state;

            if (channelp->state & CAS_EVENT_MASK)
            {
               VRG_UINT16   stateEvent;

               /* Generate an event to client */
               stateEvent = (VRG_UINT16)((channelp->state & CAS_EVENT_MASK) >> CAS_EVENT_SHIFT);
               casGenEvent( callbackp, CAS_CTL_STATECTL_EVENT, stateEvent, channelp->channelnum );
            }
         }
      }
   }
}


/****************************************************************************
*
*  casInitJitterBuf - Initialize the CAS Jitter buffer
*
*  PURPOSE:
*     This function initializes the CAS Jitter Buffer (Queue).
*
*  PARAMETERS:
*     channelinfop => the instance of the service
*
*  RETURNS:
*
*  NOTE:
*
****************************************************************************/
static void casInitJitterBuf( CASCHANNELINFO *channelinfop )
{
   /* Init the Q */
   channelinfop->profilep = NULL;
}

/****************************************************************************
*
*  casFlushJitterBuf - Flush the CAS Jitter buffer
*
*  PURPOSE:
*     This function flushes the CAS Jitter Buffer (Queue).
*
*  PARAMETERS:
*     channelinfop => the instance of the service
*
*  RETURNS:
*
*  NOTE:
*
****************************************************************************/
static void casFlushJitterBuf( CASCHANNELINFO *channelinfop )
{
   if (channelinfop->profilep != NULL)
   {
      /* Flush the Q */
      free( channelinfop->profilep );
   }
}


/****************************************************************************
*
*  casMonitor - Monitor the CAS bits.
*
*  PURPOSE:
*     This function monitors the CAS bits.
*
*  PARAMETERS:
*     channelinfop   => pointer to channel information
*
*  RETURNS:
*     Updated channel info/state information, plus possible events.
*
*  NOTE:
*
****************************************************************************/
static void casMonitor( CASCHANNELINFO *channelinfop, CASCALLBKP callbackp )
{
   if (channelinfop->castype & CASTYPE_FXO)
   {
      casMonitorFXO( channelinfop, callbackp );
   }
#if CAS_CTL_PSTN
   else
   {
      casMonitorFXS( channelinfop, callbackp );
   }
#endif
}

#if CAS_CTL_PSTN
/****************************************************************************
*
*  casMonitorFXS - Monitor the CAS bits.
*
*  PURPOSE:
*     This function monitors the CAS bits.
*
*  PARAMETERS:
*     channelinfop   => pointer to channel information
*
*  RETURNS:
*     Updated channel info/state information, plus possible events.
*
*  NOTE:
*
****************************************************************************/
static void casMonitorFXS( CASCHANNELINFO *channelinfop, 
                           CASCALLBKP callbackp )
{
   CAS_CTL_FXS_INTERFACE_STATE state;
   VRG_UINT16              casbits;
   VRG_UINT16              currentCount;
#if CAS_DAADEGLITCH
   VRG_UINT16              event;
#endif

   state = (CAS_CTL_FXS_INTERFACE_STATE) (((channelinfop->state) 
             & CAS_STATEBITS_MASK) >> CAS_STATEBITS_SHIFT);
   casbits = (channelinfop->currentCAS) & CAS_ABCD_MASK;
   currentCount = channelinfop->currentCount;

   switch (state)
   {
      case CAS_CTL_FXS_RELEASED:
      {
         /* 
         ** Assume that no current detection is possible in the onhook state. 
         ** This is true for the CPC562x DAA 
         */
         if ( casbits == CASBITS_RINGING )
         {
            if ( currentCount >= FXS_RINGON && channelinfop->substate != 1 )
            {
               channelinfop->substate = 1; /* Ring on detected */

               /* just report ring detection */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_RINGON,
                            channelinfop->channelnum );
            }
         }
         else if (channelinfop->substate == 1)
         {
            /* Check for ring signal off */
            if (currentCount >=  FXS_RINGOFF)
            {
               /* report ring off detection */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_RINGOFF,
                            channelinfop->channelnum );

               //if ( country == CAS_CTL_NORTH_AMERICA )
               {
                  /* Look for caller ID */
                  xdrvDaaModeControl( (XDRV_DAA *)channelinfop->casDriver, 
                                      XDRV_DAA_MODE_ONHOOK_DATA);
                  channelinfop->substate = 2;
               }
            }
         }
         else if (channelinfop->substate == 2)
         {
            if (currentCount >= FXS_CLIDRXEND)
            {
               /* Change back to onhook mode so we can receive rings */
               xdrvDaaModeControl( (XDRV_DAA *)channelinfop->casDriver, 
                                   XDRV_DAA_MODE_ONHOOK);
               /* Look for ring end */
               channelinfop->substate = 3;
            }
         }   
         else if (channelinfop->substate == 3)
         {      
            /* Check for end of ringing */
            if (currentCount >=  FXS_RINGEND)
            {
               /* report ring end detection */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_RINGEND,
                            channelinfop->channelnum );

               /* Look for rings */
               channelinfop->substate = 0;
            }
         }
      }
      break;

      case CAS_CTL_FXS_SEIZED:
      {
#if CAS_DAADEGLITCH
         if (currentCount > FXS_LOOPDEGLITCH)
         {
            if (casbits != channelinfop->substate)
            {
               /* Store current casbits */
               channelinfop->substate = casbits;

               /* Report loop current change */
               switch (casbits)
               {
                  case CASBITS_RLCF:
                  {
                     event = CAS_CTL_EVENT_RLCF;
                  }
                  break;

                  case CASBITS_LCF:
                  {
                     event = CAS_CTL_EVENT_LCF;
                  }
                  break;

                  case CASBITS_LCFO:
                  {
                     event = CAS_CTL_EVENT_LCFO;
                  }
                  break;

                  default:
                  {
                     /* Ignore */
                     event = CAS_CTL_EVENT_NONE;
                  }
                  break;
               }

               if (event != CAS_CTL_EVENT_NONE)
               {
                  /* report loop current change detection */
                  casGenEvent( callbackp,
                               CAS_CTL_DETECT_EVENT,
                               event,
                               channelinfop->channelnum );
               }
            }
         }
#endif /* CAS_DAADEGLITCH*/
      }
      break;

#if 0
      case CAS_CTL_FXS_RING_ON:
      {
         if ( casbits != CASBITS_RG )
         {
            /* ring has ended - has the ring duration matched the CAR criteria */
            if (currentCount > FXS_RING_JAPAN_CLIDRX_CAR_MAX)
            {
               /* ring period has exceeded CAR interval send ring detection event to client */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_RINGON,
                            channelinfop->channelnum );

               casChangeControlState( channelinfop,
                                      callbackp,
                                      ( FXSBITS_RELEASED ) );
            }
            else if ( (currentCount >= FXS_RING_JAPAN_CLIDRX_CAR_MIN) &&  (currentCount <= FXS_RING_JAPAN_CLIDRX_CAR_MAX) )
            {
               /* ring period matched CAR interval */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_JAPAN_CLIDRX_CAR,
                            channelinfop->channelnum );

               casChangeControlState( channelinfop,
                                      callbackp,
                                      ( FXSBITS_JAPAN_CLIDRX_DATA ) );
            }
         }
      }
      break;

      case CAS_CTL_FXS_JAPAN_CLIDRX_DATA:
      {
         /* run a timeout - generally we will be transition out by external state change request */
         if (currentCount > FXS_RING_JAPAN_CLIDRX_DATA_MAX)
         {
            casChangeControlState( channelinfop,
                                   callbackp,
                                   ( FXSBITS_RELEASED ) );
         }
      }
      break;

      case CAS_CTL_FXS_JAPAN_CLIDRX_POST_DATA:
      {
         /* run a timeout - generally we will be transition out by external state change request */
         if (currentCount > FXS_RING_JAPAN_CLIDRX_DATA_MAX)
         {
            casChangeControlState( channelinfop,
                                   callbackp,
                                   ( FXSBITS_RELEASED ) );
         }
}
      break;
#endif
      /* No CAS events detected in these states */

      /* 
      ** CLIDRX state may be used to manually control reception of onhook data.
      ** No current or ring detection is available in this state.
      */
      case CAS_CTL_FXS_CLIDRX:
      default:
      break;
   }
}
#endif

/****************************************************************************
*
*  casMonitorFXO - Monitor the CAS bits.
*
*  PURPOSE:
*     This function monitors the CAS bits.
*
*  PARAMETERS:
*     channelinfop   => pointer to channel information
*
*  RETURNS:
*     Updated channel info/state information, plus possible events.
*
*  NOTE:
*
****************************************************************************/
static void casMonitorFXO( CASCHANNELINFO *channelinfop, CASCALLBKP callbackp )
{
   CAS_CTL_FXO_INTERFACE_STATE state;
   VRG_UINT16              casbits;
   VRG_UINT16              currentCount;

   state = (CAS_CTL_FXO_INTERFACE_STATE) (((channelinfop->state) & CAS_STATEBITS_MASK) >> CAS_STATEBITS_SHIFT);
   casbits = (channelinfop->currentCAS) & CAS_ABCD_MASK;
   currentCount = channelinfop->currentCount;

   switch (state)
   {
      case CAS_CTL_FXO_JAPAN_ALERTING_CLID:
      {
         if ((casbits & CASBITS_LC) == CASBITS_LC)
         {
            if ( currentCount >= channelinfop->fxoParms.minConnect )
            {
               casChangeControlState( channelinfop,
                                      callbackp,
                                      ( FXOBITS_JAPAN_ALERTING_CLID_OFFHOOK | CASEVENT_OFFHOOK | CASBITS_RLCF ));
            }
         }
         else
         {
            /* Loop is open - do nothing */
         }
      }
      break;

      case CAS_CTL_FXO_JAPAN_ALERTING_CLID_OFFHOOK:
      {
         if ((casbits & CASBITS_LC) == CASBITS_LC)
         {
            /* Loop is closed - do nothing */
         }
         else
         {
            if ( currentCount >= channelinfop->fxoParms.minDisconnect )
            {
               casChangeControlState( channelinfop,
                                      callbackp,
                                      ( FXOBITS_JAPAN_ALERTING_CLID | CASEVENT_ONHOOK | CASBITS_RLCF ));
            }
         }
      }
      break;

      case CAS_CTL_FXO_SERVICEDENIED:
      {
         /* No monitoring required - service is denied! */
      }
      break;

      case CAS_CTL_FXO_IDLE:
      {
         /* Wink/Hook detection */

         if ((casbits & CASBITS_LC) == CASBITS_LC)
         {
            if (channelinfop->earlyOffState)
            {
               /* Generate an early off hook event to client */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_EARLY_OFFHOOK,
                            channelinfop->channelnum );

               if(channelinfop->deglitchCount == 0)
               {
                  /* Generate Loop Current Feed */
                  casSetSlicState( channelinfop, XDRV_SLIC_MODE_LCF );
               }

               channelinfop->earlyOffState = 0;
               channelinfop->earlyOnState = 1;
            }

            if (channelinfop->delayOffState)
            {
               /* Generate a delayed off hook event to client */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_DELAY_OFFHOOK,
                            channelinfop->channelnum );

               channelinfop->delayOffState = 0;
               channelinfop->delayOnState = 1;
            }

            if ( currentCount >= channelinfop->fxoParms.minConnect )
            {
               /* Loop is closed - go to addressing state with LCF, issue off-hook event */
               casChangeControlState( channelinfop,
                                      callbackp,
                                      FXOBITS_ADDRESSING | CASEVENT_OFFHOOK | CASBITS_LCF );
            }
            else if ( currentCount >= channelinfop->fxoParms.minWink )
            {
               channelinfop->substate = FXO_IDLE_WINK;
            }
            else
            {
               channelinfop->substate = 0;
            }
         }
         else
         {
            /* Loop is open */

            if (channelinfop->earlyOnState)
            {
               /* Generate an early on hook event to client */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_EARLY_ONHOOK,
                            channelinfop->channelnum );

               if(channelinfop->deglitchCount == 0)
               {
                  /* Go back to steady state */
                  casSetSlicState( channelinfop, XDRV_SLIC_MODE_STANDBY );
               }

               channelinfop->earlyOnState = 0;
               channelinfop->earlyOffState = 1;
            }

            if (channelinfop->delayOnState)
            {
               /* Generate a delayed on hook event to client */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_DELAY_ONHOOK,
                            channelinfop->channelnum );

               channelinfop->delayOnState = 0;
               channelinfop->delayOffState = 1;
            }

            if (channelinfop->substate == FXO_IDLE_WINK)
            {
               if ( currentCount >= channelinfop->fxoParms.minPostWink )
               {
                  /* Generate an event to client */
                  casGenEvent( callbackp,
                               CAS_CTL_DETECT_EVENT,
                               CAS_CTL_EVENT_WINK,
                               channelinfop->channelnum );

                  channelinfop->substate = 0;
               }
            }
            else if (channelinfop->substate == FXO_STARTUP_IDLE)
            {
               /* This is a transition from startup init state to LCF
               ** so we need to change the channel state to LCF
               ** because CAS doesn't assume the initial state.
               ** Don't have to wait for full disconnect time to set the initial
               ** state because this isn't a real transition (thus no event )
               ** */
               casChangeControlState( channelinfop,
                                      callbackp,
                                      FXOBITS_IDLE | CASEVENT_NOEVT | CASBITS_LCF );
               channelinfop->substate = 0;
            }
         }
      }
      break;

      case CAS_CTL_FXO_ALERTING:
      case CAS_CTL_FXO_JAPAN_ALERTING:
      {
         /* Hook detection only */

         if ((casbits & CASBITS_LC) == CASBITS_LC)
         {
            if (channelinfop->currentCount >= CAS_CTL_CFG_RING_TRIP_REMOVE)
            {
               /* Loop is closed - remove ringing and go to active state with previous LCF or RLCF, issue off-hook event */
               casChangeControlState( channelinfop,
                                      callbackp,
                                      ( FXOBITS_ACTIVE | CASEVENT_OFFHOOK | ( channelinfop->previousActiveState & CASBITS_MASK ) ) );
            }
         }
         else
         {
            /* Loop is open - do nothing */
         }
      }
      break;

      case CAS_CTL_FXO_ACTIVE:
      case CAS_CTL_FXO_ADDRESSING:
      {
         /* Hook + flash + pulse dial detection */

         if ((casbits & CASBITS_LC) == CASBITS_LC)
         {
            /* Loop is closed - offhook */

            /* Check if we should generate an early-offhook event */
            if ((channelinfop->earlyOffState) && (currentCount >= channelinfop->fxoParms.earlyActiveHookInterval))
            {
               /* Generate an early off hook event to client */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_EARLY_OFFHOOK,
                            channelinfop->channelnum );

              channelinfop->earlyOffState = 0;
              channelinfop->earlyOnState = 1;
            }

            /* Check if we should generate an delayed-offhook event */
            if ((channelinfop->delayOffState) && (currentCount >= channelinfop->fxoParms.delayActiveHookInterval))
            {
               /* Generate a delayed off hook event to client */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_DELAY_OFFHOOK,
                            channelinfop->channelnum );

              channelinfop->delayOffState = 0;
              channelinfop->delayOnState = 1;
            }

            if (channelinfop->substate == FXO_FLASH_STARTED)
            {
               if (currentCount >= FXO_POSTFLASH)
               {
                  /* Generate an event to client */
                  casGenEvent( callbackp,
                               CAS_CTL_DETECT_EVENT,
                               CAS_CTL_EVENT_FLASH,
                               channelinfop->channelnum );

                  channelinfop->substate = 0;
               }
            }

            if (currentCount >= channelinfop->fxoParms.pulseMaxMake)
            {
               /* Interdigit spacing has been achieved */

               if ((channelinfop->substate) && (channelinfop->substate != FXO_FLASH_STARTED))
               {
                  /*
                  ** Generate an event indicating the number of pulses
                  ** detected.
                  */
                  casGenEvent( callbackp,
                               CAS_CTL_PULSEDIAL_EVENT,
                               (VRG_UINT16)(channelinfop->substate >> 1),
                               channelinfop->channelnum );

                  channelinfop->substate = 0;
               }
            }
            else
            {
               /* This could be a pulse digit */

               if (currentCount >= channelinfop->fxoParms.pulseMinMake)
               {
                  if (channelinfop->substate & 1)
                  {
                     channelinfop->substate++;
                  }
               }
            }
         }
         else
         {
            /* Loop is open - onhook */

            /* Check if we should generate an early-onhook event */
            if ((channelinfop->earlyOnState) && (currentCount >= channelinfop->fxoParms.earlyActiveHookInterval))
            {
               /* Generate an early on hook event to client */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_EARLY_ONHOOK,
                            channelinfop->channelnum );

              channelinfop->earlyOnState = 0;
              channelinfop->earlyOffState = 1;
            }

            /* Check if we should generate a delayed-onhook event */
            if ((channelinfop->delayOnState) && (currentCount >= channelinfop->fxoParms.delayActiveHookInterval))
            {
               /* Generate a delayed on hook event to client */
               casGenEvent( callbackp,
                            CAS_CTL_DETECT_EVENT,
                            CAS_CTL_EVENT_DELAY_ONHOOK,
                            channelinfop->channelnum );

              channelinfop->delayOnState = 0;
              channelinfop->delayOffState = 1;
            }

            if (currentCount >= channelinfop->fxoParms.pulseMaxBreak)
            {
               /* Exceeded maximum make interval - ignore possible digits */

               channelinfop->substate = 0;
            }
            else
            {
               /* This could be a pulse digit */
               if (currentCount >= channelinfop->fxoParms.pulseMinBreak)
               {
                  if (!(channelinfop->substate & 1))
                  {
                     channelinfop->substate++;
                  }
               }
            }

            if ( currentCount >= channelinfop->fxoParms.minDisconnect )
            {
               /* Loop is open - enter idle mode */

#if !HOOK_EVENTS_SIMULATED_THROUGH_COMMAND_LINE
               casChangeControlState( channelinfop,
                                      callbackp,
                                      FXOBITS_IDLE | CASEVENT_ONHOOK | CASBITS_LCF );
#endif                                  
               channelinfop->substate = 0;
            }
            else if (currentCount >= channelinfop->fxoParms.maxHookFlash)
            {
               channelinfop->substate = 0;
            }
            else if (currentCount >= channelinfop->fxoParms.minHookFlash)
            {
               channelinfop->substate = FXO_FLASH_STARTED;
            }
         }
      }
      break;

      default:
      {
      }
      break;

   }
}


/****************************************************************************
*
*  casChangeControlState - Monitor the CAS bits.
*
*  PURPOSE:
*     This function monitors the CAS bits.
*
*  PARAMETERS:
*     channelinfop   => pointer to channel information
*
*  RETURNS:
*     Updated channel info/state information, plus possible events.
*
*  NOTE:
*
****************************************************************************/
static void casChangeControlState( CASCHANNELINFO *channelinfop, CASCALLBKP callbackp, VRG_UINT32 state )
{
   VRG_UINT16   detectedEvent;

   if (channelinfop->profilep)
   {
      free(channelinfop->profilep);

      /* NULL the pointer */
      channelinfop->profilep = NULL;
   }

   /* Flag that a state change has occured */
   channelinfop->statechange = VRG_TRUE;

   if ( (channelinfop->state & CAS_STATEBITS_MASK) !=
        (state & CAS_STATEBITS_MASK) )
   {
      /* Update the state in use */
      channelinfop->substate = 0;
      channelinfop->currentCount = 0;

      if( (state & CAS_STATEBITS_MASK) == FXOBITS_IDLE )
      {
         /* Remove glitches caused by L9500 SLIC offhook to onhook transition. */
         channelinfop->deglitchCount = CAS_CTL_DEFAULT_FXO_DEGLITCH_MSEC;
      }
   }
   channelinfop->state = state;

   /* Generate an event to client */
   detectedEvent = (VRG_UINT16)((state & CAS_EVENT_MASK) >> CAS_EVENT_SHIFT);

#if CAS_CTL_CFG_STAGGER_RING_SUPPORT
   /* Make sure we clean the stagger ring block associated with this CAS channel if we need
   ** to.  We do not need to worry about the actual transition here since all cases where
   ** this function is called is a valid case where we would want the ring to be terminated.
   */
   casStaggerRingClean( channelinfop->channelnum );
#endif

   casGenEvent( callbackp, CAS_CTL_DETECT_EVENT, detectedEvent, channelinfop->channelnum );
}


/****************************************************************************
*
*  casGenEvent - Generate an event
*
*****************************************************************************
*
*  PURPOSE:
*     Generate an event
*
*  PARAMETERS:
*     srvdrvp - service driver pointer
*     evtcode - event to be sent
*     op1     - size of the extended data to be sent
*     op2     - pointer to the extended data
*
*  RETURNS:
*     NOTHING
*
*  NOTE:
*
****************************************************************************/
static void casGenEvent( CASCALLBKP callbackp, VRG_UINT16 evtcode, VRG_UINT16 op1, VRG_UINT16 op2 )
{
   CASEVT evt;

   /* initialize the event structure */
   evt.devhdl  = 0;
   evt.evt     = evtcode;
   evt.reqid   = 0;
   evt.op1     = op1;
   evt.op2.val = op2;

   /* send event to the client */
   (callbackp)( &evt, 0 );
}


/****************************************************************************
*
*  casUpdate - Update the CAS Channel States
*
*****************************************************************************
*
*  PURPOSE:
*     This function Updates the CAS Channel States
*
*  PARAMETERS:
*     channelinfop - pointer to structure that contains all of the information
*                    associated with this channel
*
*  RETURNS:
*     onhook/offhook status
*
*  NOTE:
*
****************************************************************************/
VRG_UINT16 casUpdate( CASCHANNELINFO *channelinfop )
{
#if CAS_CTL_PSTN
    if ( channelinfop->castype == CAS_TYPE_FXO )
    {
        return casUpdateFXO( channelinfop );
    }
    else
    {
        return casUpdateFXS( channelinfop );
    }
#else
   return casUpdateFXO( channelinfop );
#endif
}

#if CAS_CTL_PSTN
/****************************************************************************
*
*  casUpdateFXS - Update the CAS Channel States
*
*  PURPOSE:
*     This function Updates the CAS Channel States
*
*  PARAMETERS:
*     channelinfop - pointer to structure that contains all of the information
*                    associated with this channel
*
*  RETURNS:
*     currently detected signalling bit status
*
*  NOTE:
*
****************************************************************************/
VRG_UINT16 casUpdateFXS( CASCHANNELINFO *channelinfop )
{
   CAS_CTL_FXS_INTERFACE_STATE interfaceState;  /* Current channel state */
   VRG_UINT16 casbits;                          /* Signalling bits */
   XDRV_DAA  *daaDriver;                        /* DAA driver pointer */
   XDRV_DAA_LOOP loopCurrent;                   /* Loop current state */

   /*
   ** Update CAS Outputs
   */
   interfaceState = (CAS_CTL_FXS_INTERFACE_STATE)
      ((channelinfop->state & CAS_STATEBITS_MASK) >> CAS_STATEBITS_SHIFT);
   casbits = (VRG_UINT16)(channelinfop->state & CASBITS_MASK);

   (void)casbits;

   /*
   ** Get DAA Driver Pointer
   */
   daaDriver = (XDRV_DAA*)channelinfop->casDriver;

   /*
   ** Handle state change requests
   */
   if ( channelinfop->statechange )
   {
      /* Clear the State Change Flag */
      channelinfop->statechange = VRG_FALSE;

      switch ( interfaceState )
      {
         case CAS_CTL_FXS_SEIZED:
         {
            /* Seized mode, set DAA to offhook */
            xdrvDaaModeControl( daaDriver, XDRV_DAA_MODE_OFFHOOK );
         }
         break;


         case CAS_CTL_FXS_CLIDRX:
         {
            /* Caller ID receive mode, enable onhook data reception */
            xdrvDaaModeControl( daaDriver, XDRV_DAA_MODE_ONHOOK_DATA );
         }
         break;

         case CAS_CTL_FXS_RELEASED:
#if 0
         case CAS_CTL_FXS_RING_ON:
         case CAS_CTL_FXS_JAPAN_CLIDRX_DATA:
         case CAS_CTL_FXS_JAPAN_CLIDRX_POST_DATA:
#endif
         default:
         {
            /* Released mode, set to onhook */
            xdrvDaaModeControl( daaDriver, XDRV_DAA_MODE_ONHOOK );
         }
         break;            
      }
   }

   /* Process any events prior to retrieving hookstate */
   xdrvDaaProcessEvents( daaDriver );
   /*
   ** Update CAS Inputs
   */
   if( xdrvDaaIsOffhook( daaDriver ) )
   {
      /* In offhook mode, we monitor loop current */
      loopCurrent = xdrvDaaGetLoopCurrent( daaDriver );
      switch ( loopCurrent )
      {
         case XDRV_DAA_LOOP_CLOSED:
         {
            /* Off-hook, loop current feed */
            return CASBITS_LCF;
         } 
         break;

         case XDRV_DAA_LOOP_CLOSED_REVERSE:
         {
            /* Off-hook, reverse loop current feed */
            return CASBITS_RLCF;
         } 
         break;

         case XDRV_DAA_LOOP_OPEN:
         default:
         {
            /* Off-hook, loop open */
            return CASBITS_LCFO;
         }
         break;
      }
   }
   else if ( xdrvDaaIsRingActive ( daaDriver ) )
   {
      /* On-hook, ring detected */
      return( CASBITS_RINGING );
   }
   else
   {
      /* On-hook, idle */
      return CASBITS_LCFO;
   }
}
#endif

/****************************************************************************
*
*  casUpdateFXO - Update the CAS Channel States
*
*****************************************************************************
*
*  PURPOSE:
*     This function Updates the CAS Channel States
*
*  PARAMETERS:
*     channelinfop - pointer to structure that contains all of the information
*                    associated with this channel
*
*  RETURNS:
*     onhook/offhook status
*
*  NOTE:
*
****************************************************************************/
VRG_UINT16 casUpdateFXO( CASCHANNELINFO *channelinfop )
{
   VRG_UINT16 interfaceState;
   VRG_UINT16 casbits;
   XDRV_CAS  *casDriver;

   /*
   ** Update CAS Outputs
   */
   interfaceState = (VRG_UINT16)((channelinfop->state & CAS_STATEBITS_MASK) >> CAS_STATEBITS_SHIFT);
   casbits = (VRG_UINT16)(channelinfop->state & CASBITS_MASK);

   /*
   ** Get CAS Driver Pointer
   */
   casDriver = channelinfop->casDriver;

   /*
   ** Handle state change requests
   */
   if (channelinfop->statechange)
   {
      switch (interfaceState)
      {
         case CAS_CTL_FXO_SERVICEDENIED:
         {
            if ( casbits == CASBITS_LCF )
            {
               /* Restore loop current to interface - used for OSI */
               casSetSlicState( channelinfop, XDRV_SLIC_MODE_LCF );
            }
            else
            {
               /* Remove loop current from interface */
               casSetSlicState( channelinfop, XDRV_SLIC_MODE_LCFO );
            }
         }
         break;

         case CAS_CTL_FXO_IDLE:
         {
            /* Use standby current on interface */
            casSetSlicState( channelinfop, XDRV_SLIC_MODE_STANDBY );
         }
         break;

         case CAS_CTL_FXO_ALERTING:
         case CAS_CTL_FXO_JAPAN_ALERTING:
         case CAS_CTL_FXO_JAPAN_ALERTING_CLID:
         case CAS_CTL_FXO_JAPAN_ALERTING_CLID_OFFHOOK:
         {
            if ( casbits == CASBITS_LCF )
            {
               casSetSlicState( channelinfop, XDRV_SLIC_MODE_OHT );
            }
            else if ( casbits == CASBITS_RLCF )
            {
               casSetSlicState( channelinfop, XDRV_SLIC_MODE_OHTR );
            }
            else
            {
               casSetSlicState( channelinfop, XDRV_SLIC_MODE_RING );
            }
         }
         break;

         default:
         {
            if ( casbits == CASBITS_LCF )
            {
               /* Generate Loop Current Feed */
               casSetSlicState( channelinfop, XDRV_SLIC_MODE_LCF );
            }
            else
            {
               /* Generate Reverse Loop Current Feed */
               casSetSlicState( channelinfop, XDRV_SLIC_MODE_RLCF );
            }
         }
         break;
      }

      /* Clear the State Change Flag */
      channelinfop->statechange = VRG_FALSE;
   }


   /* Process any events prior to retrieving hookstate */
   xdrvSlicProcessEvents( casDriver->slicDriver );
   /*
   ** Update CAS Inputs
   */
   if( xdrvCasGetHookState( casDriver ) == XDRV_CAS_ONHOOK )
   {
      /* On-hook, loop open */
      return( CASBITS_LO );
   }
   else
   {
      /* Off-hook, loop closed */
      return( CASBITS_LC );
   }
}


/****************************************************************************
*
*  casSetSlicState - Change SLIC state
*
*****************************************************************************
*
*  PURPOSE:
*     Change SLIC state
*
*  PARAMETERS:
*     channelinfop - cas channel info
*     slicMode     - new slic mode
*
*  RETURNS:
*     NOTHING
*
*  NOTE:
*
****************************************************************************/
static void casSetSlicState( CASCHANNELINFO *channelinfop, XDRV_SLIC_MODE slicMode )
{
   if ( slicMode == XDRV_SLIC_MODE_RING )
   {
      /* Notify callback of state change */
      if( channelinfop->casCallback != NULL )
      {
         channelinfop->casCallback( channelinfop->channelnum, VRG_TRUE );
      }

      /* Change SLIC mode */
      xdrvCasSetSlicMode( (XDRV_CAS*)channelinfop->casDriver, slicMode );
   }
   else
   {
      /* Change SLIC mode */
      xdrvCasSetSlicMode( (XDRV_CAS*)channelinfop->casDriver, slicMode );

      /* Notify callback of state change */
      if( channelinfop->casCallback != NULL )
      {
         channelinfop->casCallback( channelinfop->channelnum, VRG_FALSE );
      }
   }
}

/****************************************************************************
*
*  casProcessSlicCommands - Process SLIC commands
*
*****************************************************************************
*
*  PURPOSE:
*     Process SLIC commands
*
*  PARAMETERS:
*     cmdp -  Pointer to CAS command structure
*
*  RETURNS:
*     NOTHING
*
*  NOTE:
*
****************************************************************************/
static void casProcessSlicCommands( CASCMD *cmdp )
{
   XDRV_SLIC *slicDrv = (XDRV_SLIC*)cmdp->op1;
   
   switch( cmdp->reqid )
   {
      case CAS_REQ_SLIC_BOOTED_LOOP_CURRENT:
      {
         xdrvSlicSetBoostedLoopCurrent( slicDrv, (int)cmdp->op2.val );
      }
      break;
            
      case CAS_REQ_SLIC_SET_RING_PARMS:
      {
         CAS_CTL_EXT_CMD_PARM *parms = (CAS_CTL_EXT_CMD_PARM *)cmdp->op2.ref;
         XDRV_SLIC *slicDrv = (XDRV_SLIC*)parms->drvHdl;

         xdrvSlicSetRingParms( slicDrv, (int)parms->arguments[0],
                               (int)parms->arguments[1], (int)parms->arguments[2],   
                                (int)parms->arguments[3], (int)parms->arguments[4] );

         *cmdp->flags |= CAS_CMD_FLAG_DROP;
      }
      break;

      case CAS_REQ_SLIC_PHASE_REVERSAL_CONTROL:
      {
         xdrvSlicPhaseReversalControl( slicDrv, (int)cmdp->op2.val );
      }
      break;

      case CAS_REQ_SLIC_IS_OFFHOOK:
      {
         /* TODO */
      }
      break;

      case CAS_REQ_SLIC_SET_POWER_SOURCE:
      {
         xdrvSlicSetPowerSource( slicDrv, (int)cmdp->op2.val );
      }
      break;
      
      default:
      {
         XCHG_ASSERT(0);
      }
      break;
   }
}

/****************************************************************************
*
*  casProcessDaaCommands - Process DAA commands
*
*****************************************************************************
*
*  PURPOSE:
*     Process DAA commands
*
*  PARAMETERS:
*     cmdp -  Pointer to CAS command structure
*
*  RETURNS:
*     NOTHING
*
*  NOTE:
*
****************************************************************************/
static void casProcessDaaCommands( CASCMD *cmdp )
{  
   switch( cmdp->reqid )
   {
      case CAS_REQ_DAA_MODE_CONTROL:
      {
         XDRV_DAA *daaDrv = (XDRV_DAA*)cmdp->op1;

         xdrvDaaModeControl( daaDrv, (XDRV_DAA_MODE)cmdp->op2.val );
      }
      break;

      case CAS_REQ_DAA_GET_HOOK_STATUS:
      {
         CAS_CTL_EXT_CMD_PARM *parms = (CAS_CTL_EXT_CMD_PARM *)cmdp->op2.ref;
         XDRV_DAA *daaDrv = (XDRV_DAA*)parms->drvHdl;

         parms->arguments[0] = (int)xdrvDaaIsOffhook( daaDrv );
         *cmdp->flags |= (CAS_CMD_FLAG_DROP | CAS_CMD_FLAG_MODIFIED);
      }
      break; 
                 
      default:
      {
         XCHG_ASSERT(0);
      }
      break;
   }
}

/****************************************************************************
*
*  casProcessApmCommands - Process APM commands
*
*****************************************************************************
*
*  PURPOSE:
*     Process APM commands
*
*  PARAMETERS:
*     cmdp -  Pointer to CAS command structure
*
*  RETURNS:
*     NOTHING
*
*  NOTE:
*
****************************************************************************/
static void casProcessApmCommands( CASCMD *cmdp )
{
   CAS_CTL_EXT_CMD_PARM *parms = (CAS_CTL_EXT_CMD_PARM *)cmdp->op2.ref;
   XDRV_APM *apmDrv = (XDRV_APM*)parms->drvHdl;

   switch( cmdp->reqid )
   {
      case CAS_REQ_APM_SET_RING_PARMS:
      {
         xdrvApmSetRingParms( apmDrv, (int)parms->arguments[0],
                              (int)parms->arguments[1], (int)parms->arguments[2],   
                              (int)parms->arguments[3], (int)parms->arguments[4] );
         *cmdp->flags |= CAS_CMD_FLAG_DROP;
      }
      break;
                  
      case CAS_REQ_APM_PULSE_SIGNAL:
      {         
         xdrvApmPulseSignal( apmDrv, (int)parms->arguments[0],
                             (int)parms->arguments[1], (int)parms->arguments[2],   
                             (int)parms->arguments[3], (int)parms->arguments[4],
                             (int)parms->arguments[5] );
         *cmdp->flags |= CAS_CMD_FLAG_DROP;
      }
      break;
                  
      default:
      {
         XCHG_ASSERT(0);
      }
      break;
   }
}

