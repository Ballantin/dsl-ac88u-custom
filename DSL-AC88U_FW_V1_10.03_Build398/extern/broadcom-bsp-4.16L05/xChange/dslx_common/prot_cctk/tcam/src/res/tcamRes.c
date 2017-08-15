/**********************************************************************************
** Copyright © 2009 - 2010 Broadcom Corporation
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
**      This file defines the Telephony Client Application Manager Resources
**      management.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <tcamInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

TCAMAPPMAP gAppIxAppIdMap[ TCAM_MAX_CALL_RESOURCES ];


/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */


/***********************************************************************************
** FUNCTION:   tcamResInit
**
** PURPOSE:    Initializes the TCAM resource layer.
**
** PARAMETERS: None.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void tcamResInit ( void )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < TCAM_MAX_CALL_RESOURCES ; uIx++ )
   {
      gAppIxAppIdMap[ uIx ].uAppIx = TCAM_INVALID_HANDLE;
      gAppIxAppIdMap[ uIx ].uAppIdent = TCAM_INVALID_HANDLE;
      memset ( gAppIxAppIdMap[ uIx ].uAppId,
               0,
               sizeof ( gAppIxAppIdMap[ uIx ].uAppId ) );
   }
}


/***********************************************************************************
** FUNCTION:   tcamResSetAppIx
**
** PURPOSE:    Assign an application index based on the external (pAppId) unique
**             identifier for this resource.
**
** PARAMETERS: pAppId - the (external to TCAM) string identifier for the resource.
**             uAppId - the (external to TCAM) GUID identifier for the resource.
**
** RETURNS:    The TCAM internal application index.
**
** NOTE:       The TCAM internal application index is used for interface with the
**             CMGR actions/events.
**
**             The combination of pAppId and uAppId create a unique identifier
**             for the resource, uAppId in particular is guaranteed to be unique
**             by the CMGR.
***********************************************************************************/
BOS_UINT32 tcamResSetAppIx ( BOS_UINT8 *pAppId, BOS_UINT32 uAppId )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < TCAM_MAX_CALL_RESOURCES ; uIx++ )
   {
      if ( gAppIxAppIdMap[ uIx ].uAppIx == TCAM_INVALID_HANDLE )
      {
         /* Save and assign the application handle.
         */
         gAppIxAppIdMap[ uIx ].uAppIx = uIx;
         gAppIxAppIdMap[ uIx ].uAppIdent = uAppId;
         strMaxCpy ( (char *) gAppIxAppIdMap[ uIx ].uAppId,
                     (const char *) pAppId,
                     TCAM_COM_PARM_LEN );

         /* Report application handle for future use.
         */
         TCAMLOG_INFORMATION (( "tcamResSetAppIx: allocate \'%s\' -> 0x%lX",
                                pAppId ? pAppId : (BOS_UINT8 *) "(null)",
                                uIx ));
         return uIx;
      }
      else if ( (gAppIxAppIdMap[ uIx ].uAppIx != TCAM_INVALID_HANDLE) &&
                (uAppId != TCAM_INVALID_HANDLE) &&
                (gAppIxAppIdMap[ uIx ].uAppIdent == uAppId) )
      {
         /* Report application handle for future use.
         */
         TCAMLOG_INFORMATION (( "tcamResSetAppIx: existing 0x%lX",
                                uIx ));
         return uIx;
      }
   }

   TCAMLOG_WARNING (( "tcamResSetAppIx: failed to allocate for \'%s\'",
                      pAppId ? pAppId : (BOS_UINT8 *) "(null)" ));
   return TCAM_INVALID_HANDLE;
}


/***********************************************************************************
** FUNCTION:   tcamResFreeAppIx
**
** PURPOSE:    Release an application index identifier from usage.
**
** PARAMETERS: uAppIx - the TCAM identifier for the resource.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void tcamResFreeAppIx ( BOS_UINT32 uAppIx )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < TCAM_MAX_CALL_RESOURCES ; uIx++ )
   {
      if ( gAppIxAppIdMap[ uIx ].uAppIx == uAppIx )
      {
         TCAMLOG_INFORMATION ((
            "tcamResFreeAppIx: deallocate 0x%lX (\'%s\', 0x%lX)",
            gAppIxAppIdMap[ uIx ].uAppIx,
            gAppIxAppIdMap[ uIx ].uAppId,
            gAppIxAppIdMap[ uIx ].uAppIdent ));

         /* Free the application handle.
         */
         gAppIxAppIdMap[ uIx ].uAppIx = TCAM_INVALID_HANDLE;
         gAppIxAppIdMap[ uIx ].uAppIdent = TCAM_INVALID_HANDLE;
         memset ( gAppIxAppIdMap[ uIx ].uAppId,
                  0,
                  sizeof ( gAppIxAppIdMap[ uIx ].uAppId ) );
      }
   }
}


/***********************************************************************************
** FUNCTION:   tcamResFindAppId
**
** PURPOSE:    Locates the external (to TCAM) application identifier to be used
**             for communication about a given call.
**
** PARAMETERS: uAppIx - the TCAM identifier for the resource.
**
** RETURNS:    The external application identifier.
**
** NOTE:       .
***********************************************************************************/
BOS_UINT8 *tcamResFindAppId ( BOS_UINT32 uAppIx )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < TCAM_MAX_CALL_RESOURCES ; uIx++ )
   {
      if ( gAppIxAppIdMap[ uIx ].uAppIx == uAppIx )
      {
         return gAppIxAppIdMap[ uIx ].uAppId;
      }
   }

   return NULL;
}


/***********************************************************************************
** FUNCTION:   tcamResFindAppIx
**
** PURPOSE:    Locates the internal (to TCAM) application identifier to be used
**             for communication with CMGR.
**
** PARAMETERS: uAppId - the external identifier of the resource.
**
** RETURNS:    The TCAM internal application identifier.
**
** NOTE:       .
***********************************************************************************/
BOS_UINT32 tcamResFindAppIx ( BOS_UINT8 *uAppId )
{
   BOS_UINT32 uIx;
   BOS_UINT32 uRet = TCAM_INVALID_HANDLE;

   for ( uIx = 0 ; uIx < TCAM_MAX_CALL_RESOURCES ; uIx++ )
   {
      TCAMLOG_INFORMATION (( "tcamResFindAppIx: \'%s\' and \'%s\' (0x%lX)",
                             gAppIxAppIdMap[ uIx ].uAppId,
                             uAppId,
                             gAppIxAppIdMap[ uIx ].uAppIx ));

      if ( !strCmpNoCase ( (const char *) gAppIxAppIdMap[ uIx ].uAppId,
                           (const char *) uAppId ) )
      {
         uRet = gAppIxAppIdMap[ uIx ].uAppIx;
         break;
      }
   }

   return uRet;
}


/***********************************************************************************
** FUNCTION:   tcamResReplaceAppIx
**
** PURPOSE:    Replace an assigned application identifier with the new one
**             provided.
**
** PARAMETERS: uAppIx - the identifier for the resource to update.
**             pAppId - the application identifier to use as replacement
**
** RETURNS:    Nothing.
**
** NOTE:       This function is used to replace a temporary (or no longer valid)
**             application identifier with a one provided by a third party.  A
**             typical usage example of this is when following an incoming call
**             the user application has been made aware of the call and it is
**             assigning its own call handle to track it.
***********************************************************************************/
void tcamResReplaceAppIx ( BOS_UINT32 uAppIx, BOS_UINT8 *pAppId )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < TCAM_MAX_CALL_RESOURCES ; uIx++ )
   {
      if ( pAppId && (gAppIxAppIdMap[ uIx ].uAppIx == uAppIx) )
      {
         strMaxCpy ( (char *) gAppIxAppIdMap[ uIx ].uAppId,
                     (const char *) pAppId,
                     TCAM_COM_PARM_LEN );

         TCAMLOG_INFORMATION (( "tcamResReplaceAppIx: assigned \'%s\' to 0x%lX",
                                pAppId, uAppIx ));

         break;
      }
   }
}


/***********************************************************************************
** FUNCTION:   tcamResRemapAppHdl
**
** PURPOSE:    Remap a selected application handle if it is not the proper one.
**
** PARAMETERS: uAppIdent - the internal handle to check for.
**             puAppId - the application identifier to use as replacement.
**
** RETURNS:    Nothing.
**
** NOTE:       .
***********************************************************************************/
void tcamResRemapAppHdl ( BOS_UINT32 uAppIdent, BOS_UINT32 *puAppId )
{
   BOS_UINT32 uIx;

   for ( uIx = 0 ; uIx < TCAM_MAX_CALL_RESOURCES ; uIx++ )
   {
      if ( (gAppIxAppIdMap[ uIx ].uAppIdent != TCAM_INVALID_HANDLE) &&
           (gAppIxAppIdMap[ uIx ].uAppIdent == uAppIdent) )
      {
         TCAMLOG_INFORMATION (( "tcamResRemapAppHdl: ident 0x%lX to 0x%lX",
                                uAppIdent,
                                gAppIxAppIdMap[ uIx ].uAppIx ));

         *puAppId = gAppIxAppIdMap[ uIx ].uAppIx;
         break;
      }
   }
}

