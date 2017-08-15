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

#include <stdio.h>
#include <xdrvLog.h>
#include <zarlinkCommon.h>
#include <zarlinkCommonTpdIntApi.h>

/*
*****************************************************************************
** FUNCTION:   zarlinkProcessEvents
**
** PURPOSE:    Call VpApiTick and process events.
**
** PARAMETERS: 
**
** RETURNS:    none
**
*****************************************************************************
*/
void zarlinkProcessEvents( VpDevCtxType* pDevCtx, VpEventCategoryType *category_save, XDRV_UINT16 *eventid_save, XDRV_UINT32 deviceChan )
{
   VpStatusType status;
   bool vpApiEventPending = FALSE;
   VpDeviceInfoType  deviceInfo;
   
   deviceInfo.pLineCtx = VP_NULL;
   deviceInfo.pDevCtx = pDevCtx;

   VpGetDeviceInfo( &deviceInfo );
   
   /* Note: Make exception for LE89010, because physical channel is 1, but it is single channel. */
   if( (deviceChan == 0) || (deviceInfo.productCode == 0x2) ) /* VP890_DEV_PCN_89010 */
   {
      status = VpApiTick( pDevCtx, &vpApiEventPending );
      if ( (status == VP_STATUS_SUCCESS) && (TRUE == vpApiEventPending) )
      {
         VpEventType pEvent;
         VpOptionEventMaskType pOptions;
         while( VpGetEvent( pDevCtx, &pEvent ) )
         {           
            /* Expand this as needed, checking other parameters of VpEventType as required. */
            switch (pEvent.eventCategory) 
            {
               /* Add more categories as required */
               case VP_EVCAT_RESPONSE:
                  switch(pEvent.eventId)
                  {
                     /* Add more events as needed */
                     case VP_DEV_EVID_DEV_INIT_CMP:
                        XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Received VP_DEV_EVID_DEV_INIT_CMP event"));
                        break;
                     case VP_LINE_EVID_LINE_INIT_CMP:
                        XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Received VP_LINE_EVID_LINE_INIT_CMP event"));
                        break;
                     case VP_EVID_CAL_CMP:
                        XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Received VP_EVID_CAL_CMP event"));
                        /* Can retrieve result of calibration here using VpGetResults, if required */
                        break;
                     case VP_LINE_EVID_RD_OPTION:
                        XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Received VP_LINE_EVID_RD_OPTION"));
                        VpGetResults( &pEvent, &pOptions);
                        XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Zarlink Event Mask: faults: 0x%04x", pOptions.faults));
                        XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Zarlink Event Mask: signaling: 0x%04x", pOptions.signaling));
                        XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Zarlink Event Mask: response: 0x%04x", pOptions.response));
                        XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Zarlink Event Mask: test: 0x%04x", pOptions.test));
                        XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Zarlink Event Mask: process: 0x%04x", pOptions.process));
                        XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC, "SLIC: Zarlink Event Mask: fxo: 0x%04x", pOptions.fxo));  

                        break;
                     default: 
                        /* Do nothing */
                        XDRV_LOG_ERROR((XDRV_LOG_MOD_SLIC, "SLIC: ERROR Unexpected Response Event %d came from the SLIC.", pEvent.eventId));
                        break;
                  }                 
                  /* Save RESPONSE event category and event id retrieved.  Used for Device Init purposes. */
                  *category_save = pEvent.eventCategory;
                  *eventid_save  = pEvent.eventId;
                  break;
   
               case VP_EVCAT_TEST:
                  switch(pEvent.eventId)
                  {
                     case VP_LINE_EVID_TEST_CMP:
                     case VP_LINE_EVID_ABORT:
                     {
#if XDRV_ZARLINK_TPD_SUPPORT
                        VpTestResultType testResults; 
                        LtEventType eventType;
                        LtTestStatusType status;

                        if( pEvent.hasResults == TRUE )
                        {
                           VpGetResults( &pEvent, &testResults );
                           eventType.pResult = &testResults;
                        }
                        else
                        {
                           eventType.pResult = NULL;
                        }

                        eventType.pVpEvent = &pEvent;
                        status = LtEventHandler( tpdGetTestCtx(pEvent.lineId), &eventType );

                        if( status != LT_STATUS_RUNNING )
                        {
                           tpdUpdateTestStatus( status, pEvent.lineId, pEvent.eventId ); 
                        }
#endif
                     }
                     break;
                  }
                  break;   
               default:
                  XDRV_LOG_DBG((XDRV_LOG_MOD_SLIC,"SLIC: Unexpected Event- Category: %d EventId: %d, came from the SLIC.", pEvent.eventCategory, pEvent.eventId));
                  break;
            }
         } /* while */
      } /* if */
   } /* if */
   return;
}

