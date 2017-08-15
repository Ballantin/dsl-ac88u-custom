/***************************************************************************
*
*    Copyright 2008  Broadcom Corporation
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
*    Description: 
*      This is common code for all Zarlink (Legerity) SLIC devices, that uses API-2
*
***************************************************************************/


/* ---- Include Files ---------------------------------------- */
#include <zarlinkCommonDaa.h>
#include <zarlinkCommon.h>

/*
*****************************************************************************
** FUNCTION:   zarlinkFxoLineModeControl
**
** PURPOSE:    Sets the DAA into one of the defined modes
**
** PARAMETERS: mode  - the mode to set the SLAC into.
**
** RETURNS:    none
**
*****************************************************************************
*/
void zarlinkFxoLineModeControl( VpLineCtxType *pLineCtx, XDRV_DAA_MODE mode )
{
   VpLineStateType lineState = VP_LINE_FXO_LOOP_OPEN;

   switch( mode )
   {
      /* Put device into on-hook - release line */
      case XDRV_DAA_MODE_ONHOOK:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA: Going OnHook"));
         lineState = VP_LINE_FXO_LOOP_OPEN;
      }
      /*Pass through so that it will be ready to detect CLID RX for next incoming call */
   
      /*  Caller ID receive mode, enable onhook data reception  */
      case XDRV_DAA_MODE_ONHOOK_DATA:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA: Enable on-hook Caller ID receive. "));
         lineState = VP_LINE_FXO_OHT;     
      }
      break;

      /* Put device into off-hook - seize line */
      case XDRV_DAA_MODE_OFFHOOK:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA: Going OffHook"));
         lineState = VP_LINE_FXO_TALK;
      }
      break;
  
      /* Enable ring validation mode */
      case XDRV_DAA_MODE_RING_VALID_ON:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA: Enable ring validation"));
         lineState = VP_LINE_FXO_LOOP_CLOSE;
      }
      break;
      
      /* Disables ring validation mode */
      case XDRV_DAA_MODE_RING_VALID_OFF:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA: Disables ring validation"));
         lineState = VP_LINE_FXO_LOOP_OPEN;
      }
      break;

      default:
      {
         XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA: unrecognized DAA mode"));
         return;
      }
      break;
   }

   VpSetLineState( pLineCtx, lineState );
}

/*
*****************************************************************************
** FUNCTION:   zarlinkFxoLineIsOffhook
**
** PURPOSE:    Determine if a channel is on or off hook
**
** PARAMETERS: 
**
** RETURNS:    TRUE if offhook, FALSE if onhook
**
*****************************************************************************
*/
XDRV_BOOL zarlinkFxoLineIsOffhook( VpLineCtxType *pLineCtx )
{
   VpLineInfoType lineInfo;
   VpLineStateType lineState = VP_LINE_FXO_LOOP_OPEN;
   bool liuStatus = FALSE;
   bool feedDisableStatus = FALSE;
   bool disconnectStatus = FALSE;
   static bool liuStatusOld = FALSE;
   static bool feedDisableStatusOld = FALSE;
   static bool disconnectStatusOld = FALSE;

   lineInfo.pLineCtx = pLineCtx;
   lineInfo.pDevCtx = VP_NULL;

   VpGetLineInfo( &lineInfo );

   /* LIU status does not return success at this point due to Zarlink bug */
   if ( VpGetLineStatus( pLineCtx, VP_INPUT_LIU, &liuStatus ) != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA ERROR %s %d", __FUNCTION__, __LINE__));
   }   

   if ( VpGetLineStatus( pLineCtx, VP_INPUT_DISCONNECT, &disconnectStatus ) != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA ERROR %s %d", __FUNCTION__, __LINE__));
   }   

   if ( VpGetLineStatus( pLineCtx, VP_INPUT_FEED_DIS, &feedDisableStatus ) != VP_STATUS_SUCCESS )
   {
      XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "DAA ERROR %s %d", __FUNCTION__, __LINE__));
   }   

   if ( (liuStatus != liuStatusOld) || (feedDisableStatus != feedDisableStatusOld) || (disconnectStatus != disconnectStatusOld)  )
   {
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "pLineCtx = 0x%08X, pDevCtx = 0x%08X, liu = %d, disconnect = %d, feedDisable = %d ", 
            (unsigned int)pLineCtx, (unsigned int)lineInfo.pDevCtx, liuStatus, disconnectStatus, feedDisableStatus));
   }

   if( VpGetLineState( pLineCtx, &lineState) != VP_STATUS_SUCCESS ) 
   {
      XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "DAA ERROR %s %d", __FUNCTION__, __LINE__));
   }

   liuStatusOld = liuStatus;
   feedDisableStatusOld = feedDisableStatus;
   disconnectStatusOld = disconnectStatus;

   /* LIU, feedDisconnect, disconnect and line state table.
   ** ----------------------------------------------------------------------------------------------
   **                                       liu    feedDisconnect    disconnect     lineState
   ** Idle no FXO call, FXO line connected:  0           0              0           VP_LINE_FXO_OHT
   ** Active FXO call , FXO line connected:  0           0              0           VP_LINE_FXO_TALK
   ** FXO line disconnected               :  0           1              1           VP_LINE_FXO_OHT
   **
   */
   //XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "liu = %d, disconnect = %d, feedDisable = %d lineState = %d ", 
   //      liuStatus, disconnectStatus, feedDisableStatus, lineState));

   if ( (liuStatus == TRUE) && (feedDisableStatus == FALSE) && (disconnectStatus == FALSE) )
   {
      return (XDRV_TRUE);
   }

   if ( ((lineState == VP_LINE_FXO_TALK) || (lineState == VP_LINE_FXO_LOOP_CLOSE)) )
   {
      return ( XDRV_TRUE);
   }
   else
   {
      return ( XDRV_FALSE );
   }
}

/*
*****************************************************************************
** FUNCTION:   zarlinkProcessFxoEvents
**
** PURPOSE:    Call VpApiTick and process events.
**
** PARAMETERS: pDevCtx    - Device context
**             deviceChan - Channel number within device itself
**
** RETURNS:    none
**
*****************************************************************************
    */
void zarlinkProcessFxoEvents( VpDevCtxType* pDevCtx, XDRV_UINT32 deviceChan )
      {
   VpEventCategoryType category_save=0;
   XDRV_UINT16 eventid_save=0;
            
   zarlinkProcessEvents( pDevCtx, &category_save, &eventid_save, deviceChan );
   }
