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
**      This file contains the Telephony Client Application Manager Communication
**      layer.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <tcamInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */

/* ---- Private Variables ------------------------------------------------------- */

/* The currently handled event from the TCAM communication module.
*/
TCAMEVT geTcamCurEvt = eTCAM_INVALID;
/* The currently handled parameter set from the TCAM communication module.
*/
BOS_UINT8 guTcamCurParm [ TCAM_COM_PARM_NUM ][ TCAM_COM_PARM_LEN + 1 ];
/* The number of parameters associated with the command.
*/
BOS_UINT32 guTcamCurParmNum;

/* The holder for sending out an action (with parameter) to the outside
** world.  Format is "ACTION<comma>PARAM*(<comma>PARAM)".
*/
BOS_UINT8 guTcamMsg[ TCAM_COM_ACTION_LEN + 1 +  \
                     TCAM_COM_PARM_NUM * (TCAM_COM_PARM_LEN + 1) ];

/* User and P-CSCF identifier holder.
*/
BOS_UINT8 guUsrIdentity[ TCAM_USR_IDENTIFIER_LEN + 1 ];
BOS_UINT8 guPcscfIdentity[ TCAM_PCSCF_IDENTIFIER_LEN + 1 ];

#define TCAM_COM__CHK_ARGS( uNum, uMin )  ( (uNum) <= (uMin) ) 

/* The following defines the mapping of the parameter index to the
** intended value as part of the command.
*/
/* 'ANY COMMAND', <Local User>, <Call Identifier>, [<Param>] */
#define TCAM_COM__ALL__LCL_USR     0
#define TCAM_COM__ALL__CALL_ID     1
/* DIAL, <Local User>, <Call Identifier>, <Destination> */
#define TCAM_COM__DIAL__DEST       2
/* CONF, <Local User>, <Call Identifier>, <Conference Call Identifier> */
#define TCAM_COM__CONF__CALL_ID    2
/* BXFER, <Local User>, <Call Identifier>, <Destination> */
#define TCAM_COM__BXFER__DEST      2
/* CXFER, <Local User>, <Call Identifier>, <Transfer Call Identifier> */
#define TCAM_COM__CXFER__DEST      2
/* MESSAGE, <Local User>, <Call Identifier (unused)>, <Destination>, <Message> */
#define TCAM_COM__MSG__DEST        2
#define TCAM_COM__MSG__TXT         3
/* SETHANDLE, <Local User>, <Call Identifier>, <Internal Call Identifier> */
#define TCAM_COM__HDL__INT_ID      2
/* DTMF, <Local User>, <Call Identifier>, <DTMF Data> */
#define TCAM_COM__DTMF__VAL        2

/* Misc.
*/
#define TCAM_COM__DTMF__STAR_CHAR  '*'
#define TCAM_COM__DTMF__STAR_VAL   42  /* ASCII equivalent */
#define TCAM_COM__DTMF__PND_CHAR   '#'
#define TCAM_COM__DTMF__PND_VAL    35  /* ASCII equivalent */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
** FUNCTION:   tcamCom
**
** PURPOSE:    Entry point into the communication layer for receiving and processing
**             commands from the outside application.
**
** PARAMETERS: pCmdStr - the command string received.
**
** RETURNS:    None.
**
** NOTE:
***********************************************************************************/
void tcamCom ( BOS_UINT8 *pCmdStr )
{
   /* Parse the command received.
   */
   if ( pCmdStr && tcamComParse ( pCmdStr ) )
   {
      /* If parsing was successful, process the command accordingly.
      */
      switch ( geTcamCurEvt )
      {
         case eTCAM_CFG:
         {
            TCAMLOG_INFORMATION (( "tcamCom: configuration command." ));
            /* Configuration command is to query the local configuration
            ** information and send it back to the GUI layer.
            */
            tcamComCfg ( eTCAM_SVCRDY );
         }
         break;

         default:
         {
            /* Invoke the Call Manager corresponding command.
            */
            TCAMLOG_INFORMATION (( "tcamCom: relay %s to CMGR",
                                   tcamMapGetStr ( tcamMapOutEvtStr,
                                                   geTcamCurEvt ) ));
            tcamComCmgr ();
         }
         break;
      }
   }
}


/***********************************************************************************
** FUNCTION:   tcamComParse
**
** PURPOSE:    Parse a command received.
**
** PARAMETERS: pCmdStr - the command string to be parsed.
**
** RETURNS:    BOS_TRUE if successful, BOS_FALSE otherwise.
**
** NOTE:       The result of the parsed command is saved in local variable(s)
**             to be used immediately following.  Since the process here is
**             synchronous, that works just fine.  Note that if the function
**             return an error (BOS_FALSE), the content of the local variable(s)
**             should be ignored.
**      
**             Command format to TCAM is:
**
**                'COMMAND', <Local User>, <Call Identification>, [<Param>]
**
**             <Call Identification> *always* represents the application call
**             identifier, this identifier is mapped to an internal (to TCAM)
**             identifier within TCAM.
**
**             <Param> represents a variable list of parameters, the size of the
**             list and the meaning of each parameter is defined per each command.
**
***********************************************************************************/
BOS_BOOL tcamComParse ( BOS_UINT8 *pCmdStr )
{
   BOS_BOOL bRet = BOS_FALSE;
   BOS_UINT32 uIx;

   if ( pCmdStr )
   {
      BOS_UINT8 *pParm = NULL;
      TCAMEVT eEvt = eTCAM_INVALID;

#ifdef TCAM_COM__MAGIC_SIGNALING
      if ( *pCmdStr == TCAM_COM__MAGIC_SIGNALING )
      {
         TCAMLOG_DEBUG (( "tcamComParse: skip over magic" ));
         ++pCmdStr;
      }
      /* Message does not contain the magic cookie first when it
      ** should, ignore it.
      */
      else
      {
         return bRet;
      }
#endif /* TCAM_COM__MAGIC_SIGNALING */

      if ( (pParm = (BOS_UINT8*) strTok( (char **) &pCmdStr, "," )) != NULL )
      {
         eEvt = (TCAMEVT) tcamMapGetIdFromStr ( tcamMapOutEvtStr,
                                                pParm );
      }

      if ( eEvt != TCAM_INVALID_HANDLE )
      {
         guTcamCurParmNum = 0;

         /* Valid action, save the current action and associated parameter such
         ** that the command can be processed by the CMGR.
         */
         geTcamCurEvt = eEvt;

         pParm = (BOS_UINT8*) strTok( (char **) &pCmdStr, ",\n\r\0" );
         while ( pParm != NULL )
         {
            memset ( (void *) &(guTcamCurParm[ guTcamCurParmNum ]),
                     0,
                     TCAM_COM_PARM_LEN + 1 );

            strMaxCpy ( (char *) guTcamCurParm[ guTcamCurParmNum ],
                        (const char *) pParm,
                        TCAM_COM_PARM_LEN );

            ++guTcamCurParmNum;

            pParm = (BOS_UINT8*) strTok( (char **) &pCmdStr, ",\n\r\0" );
         };

         TCAMLOG_INFORMATION (( "tcamComParse: action = 0x%lX, params = %d",
                                geTcamCurEvt, guTcamCurParmNum ));
         for ( uIx = 0 ; uIx < guTcamCurParmNum ; uIx++ )
         {
            TCAMLOG_INFORMATION (( "tcamComParse: %d - value = \'%s\'",
                                   uIx, guTcamCurParm[ uIx ] ));
         }

         bRet = BOS_TRUE;
      }
   }

   /* Apply some simple sanity check to verify the message content vs.
   ** the number of expected mandatory arguments to run the command
   ** properly.
   */
   bRet = tcamComValid ( geTcamCurEvt, guTcamCurParmNum );

   TCAMLOG_INFORMATION (( "tcamComParse: %s",
                          bRet ? "success" : "failure" ));

   return bRet;
}


/***********************************************************************************
** FUNCTION:   tcamComCmgr
**
** PURPOSE:    Invoke the proper CMGR command based on the last known received
**             and parsed command.
**
** PARAMETERS: None.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void tcamComCmgr ( void )
{
   switch ( geTcamCurEvt )
   {
      /* DIAL, <Local User>, <Call Identifier>, <Destination> */
      case eTCAM_DIAL:
      {
         CMGRPLACECALL cmdInfo;

         memset ( &cmdInfo,
                  0,
                  sizeof ( CMGRPLACECALL ) );

         memcpy ( cmdInfo.uUid,
                  (const void *) guTcamCurParm[ TCAM_COM__ALL__LCL_USR ],
                  sizeof ( cmdInfo.uUid ) );

         cmdInfo.uAppIx =
            tcamResSetAppIx ( guTcamCurParm[ TCAM_COM__ALL__CALL_ID ],
                              TCAM_INVALID_HANDLE );

         /* Always assume this is a SIP URI to format.  One can add SIPS support
         ** later on if needed.
         */
         sprintf( (char *) cmdInfo.uDest,
                  "sip:" );
         
         /* If the dialled string contains some '*' characters, assume an attempt
         ** to dial by IP.
         */
         if ( strchr ( (const char *)
                           guTcamCurParm[ TCAM_COM__DIAL__DEST ], '*' ) != NULL )
         {
            BOS_UINT32 uIx;
            
            for ( uIx = 0 ;
                  uIx < strlen ( (const char *)
                                    guTcamCurParm[ TCAM_COM__DIAL__DEST ] ) ; uIx++ )
            {
               if ( guTcamCurParm [ TCAM_COM__DIAL__DEST ][ uIx ] == '*' )
               {
                  guTcamCurParm [ TCAM_COM__DIAL__DEST ][ uIx ] = '.';
               }
            }

            memcpy ( cmdInfo.uDest + strlen ( (const char *) cmdInfo.uDest ),
                     (const void *) guTcamCurParm[ TCAM_COM__DIAL__DEST ],
                     strlen ( (const char *) guTcamCurParm[ TCAM_COM__DIAL__DEST ] ) );
         }
         else
         {
            memcpy ( cmdInfo.uDest + strlen ( (const char *) cmdInfo.uDest ),
                     (const void *) guTcamCurParm[ TCAM_COM__DIAL__DEST ],
                     strlen ( (const char *) guTcamCurParm[ TCAM_COM__DIAL__DEST ] ) );
            /* Make sure we append the P-CSCF information to the destination URI.
            */
            if ( strchr ( (const char *) cmdInfo.uDest, '@' ) == NULL )
            {
               BOS_UINT8 *pPcscf = tcamComPcscf ();

               /* We have a proper P-CSCF information, append it now.
               */
               if ( pPcscf && strlen ( (const char *) pPcscf ) )
               {
                  sprintf ( (char *) cmdInfo.uDest +
                                       strlen ( (const char *) cmdInfo.uDest ),
                            "@%s",
                            pPcscf );
               }
               /* We have no P-CSCF information, assume a 'dial-by-subnet' scenario.
               **
               ** In 'dial-by-subnet' mode, the remote end is assumed to be on the
               ** same IP subnet than the caller side and the caller side will use
               ** that assumption to format the target SIP URI.
               */
               else if ( pPcscf && !strlen ( (const char *) pPcscf ) )
               {
                  CFGPROVDEVICECFG *pDevCfg = NULL;
                  PROV_CFG_IDX cfgIdx;
                  BOS_UINT8 *pDelim = NULL;

                  /* Get the device IP address.
                  */
                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  if ( tcamCbCmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                          &cfgIdx,
                                          (void *) &pDevCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     pPcscf = guPcscfIdentity;
                     memset ( (void *) guPcscfIdentity,
                              0,
                              sizeof ( guPcscfIdentity ) ); 

                     bosIpAddrGetStr ( &pDevCfg->IpAddress,
                                       (char *) pPcscf,
                                       TCAM_IP_ADDRESS_STRING_LEN );

                     tcamCbCmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                        &cfgIdx,
                                        (void *) pDevCfg,
                                        CFG_ITEM_DEL );
                     pDevCfg = NULL;
                  }

                  /* Note: for the time being, assume the device IP address is an
                  **       IPv4 address.
                  **
                  ** Replace the last 'octets' of the IP address with the one from
                  ** the destination.
                  **
                  **      - P-CSCF is assumed to be "A1.A2.A3.A4"
                  **      - Current dialed parameter (guTcamCurParm) is assumed to
                  **        be 'xB1' with:
                  **          -> x in [1;n] identifying the n-th user on the
                  **             remote device
                  **          -> B1 the last octet of the remote IP that should be
                  **             of the format " A1.A2.A3.B1"
                  **
                  ** Following the manipulation, the dialed URI will therefore be
                  ** "sip:xB1@A1.A2.A3.B1", originated from "sip:xA4@A1.A2.A3.A4"
                  **        
                  */
                  if ( (pDelim =
                           (BOS_UINT8 *) strrchr ( (const char *) pPcscf, '.' ))
                                                                           != NULL )
                  {
                     sprintf ( /* Start overriding from the last 'octet'. */
                               (char *) (pDelim + 1),
                               "%d",
                               /* Skip the leading 'user' index. */
                               atoi ( (const char *)
                                          &(guTcamCurParm [
                                                TCAM_COM__DIAL__DEST ])[ 1 ] ) );
                  }

                  sprintf ( (char *) cmdInfo.uDest +
                                       strlen ( (const char *) cmdInfo.uDest ),
                            "@%s",
                            pPcscf );
               }
            }
         }

         TCAMLOG_INFORMATION (( "tcamComCmgr: calling \'%s\' from \'%s\' (0x%lX)",
                                cmdInfo.uDest, cmdInfo.uUid, cmdInfo.uAppIx ));

         /* Send the command to the Call Manager for call creation and processing.
         */
         cmgrCmd( eCMCMD_PLACECALL, (void *) &cmdInfo );

         /* Inform the upper layer that this call is an outgoing call.
         */
         tcamComWorld ( eTCAM_DIROUT, cmdInfo.uAppIx, NULL );
      }
      break;

      /* HANGUP, <Local User>, <Call Identifier> */
      case eTCAM_HANGUP:
      {
         BOS_UINT32 uAppIx =
            tcamResFindAppIx ( guTcamCurParm[ TCAM_COM__ALL__CALL_ID ] );

         TCAMLOG_INFORMATION (( "tcamComCmgr: ending 0x%lX",
                                uAppIx ));

         /* Send the command to the Call Manager for call termination.
         */
         cmgrCmd( eCMCMD_ENDCALL, (void *) &uAppIx );

         tcamResFreeAppIx ( uAppIx );
      }
      break;

      /* ANSWER, <Local User>, <Call Identifier> */
      case eTCAM_ANSWER:
      {
         BOS_UINT32 uAppIx =
            tcamResFindAppIx ( guTcamCurParm[ TCAM_COM__ALL__CALL_ID ] );

         TCAMLOG_INFORMATION (( "tcamComCmgr: answering 0x%lX",
                                uAppIx ));

         /* Send the command to the Call Manager for call answer.
         */
         cmgrCmd( eCMCMD_ANSCALL, (void *) &uAppIx );

         /* Inform the upper layer that this call has been answered by the local
         ** user.
         */
         tcamComWorld ( eTCAM_ANSWERED, uAppIx, NULL );
      }
      break;

      /* 'HOLD-SERVICE', <Local User>, <Call Identifier> */
      case eTCAM_HOLD:
      case eTCAM_UNHOLD:
      {
         CMGRHOLDSVC holdSvc;
         
         holdSvc.bHold =
            (geTcamCurEvt == eTCAM_HOLD) ? BOS_TRUE : BOS_FALSE;
         holdSvc.uAppIx =
            tcamResFindAppIx ( guTcamCurParm[ TCAM_COM__ALL__CALL_ID ] );

         TCAMLOG_INFORMATION (( "tcamComCmgr: %s-service 0x%lX",
                                (holdSvc.bHold ? "hold" : "unhold"),
                                holdSvc.uAppIx ));

         /* Send the command to the Call Manager for service application.
         */
         cmgrCmd( eCMCMD_HOLDSVC, (void *) &holdSvc );
      }
      break;

      /* CONF, <Local User>, <Call Identifier>, <Conference Call Identifier> */
      case eTCAM_CONF:
      {
         CMGRAPPCALLHDL confSvc;
         
         confSvc.uAppIx =
            tcamResFindAppIx ( guTcamCurParm[ TCAM_COM__ALL__CALL_ID ] );
         confSvc.uSesHdl =
            tcamResFindAppIx ( guTcamCurParm[ TCAM_COM__CONF__CALL_ID ] );

         TCAMLOG_INFORMATION (( "tcamComCmgr: conf-service 0x%lX and 0x%lX",
                                confSvc.uAppIx, 
                                confSvc.uSesHdl ));

         /* Send the command to the Call Manager for service application.
         */
         cmgrCmd( eCMCMD_CONFSVC, (void *) &confSvc );
      }
      break;

      /* CXFER, <Local User>, <Call Identifier>, <Transfer Call Identifier> */
      case eTCAM_CXFER:
      {
         CMGRAPPCALLHDL xferSvc;

         xferSvc.uAppIx =
            tcamResFindAppIx ( guTcamCurParm[ TCAM_COM__CXFER__DEST ] );
         xferSvc.uSesHdl =
            tcamResFindAppIx ( guTcamCurParm[ TCAM_COM__ALL__CALL_ID ] );

         TCAMLOG_INFORMATION (( "tcamComCmgr: cxfer-service 0x%lX to 0x%lX",
                                xferSvc.uAppIx, 
                                xferSvc.uSesHdl ));

         /* Send the command to the Call Manager for service application.
         */
         cmgrCmd( eCMCMD_CXFERSVC, (void *) &xferSvc );
      }
      break;

      /* XFER_BLIND, <Local User>, <Call Identifier>, <Destination> */
      case eTCAM_BXFER:
      {
         CMGRPLACECALL cmdInfo;

         memset ( &cmdInfo,
                  0,
                  sizeof ( CMGRPLACECALL ) );

         memcpy ( cmdInfo.uUid,
                  (const void *) tcamComUser (),
                  sizeof ( cmdInfo.uUid ) );
         cmdInfo.uAppIx =
            tcamResFindAppIx ( guTcamCurParm[ TCAM_COM__ALL__CALL_ID ] );

         sprintf ( (char *) cmdInfo.uDest,
                  "sip:" );
         /* If the dialled string contains some '*' characters, assume an attempt
         ** to dial by IP.
         */
         if ( strchr (
               (const char *)
                  guTcamCurParm[ TCAM_COM__BXFER__DEST ], '*' ) != NULL )
         {
            BOS_UINT32 uIx;
            
            for ( uIx = 0 ;
                  uIx < strlen ( (const char *)
                                    guTcamCurParm[ TCAM_COM__BXFER__DEST ] ) ;
                  uIx++ )
            {
               if ( guTcamCurParm [ TCAM_COM__BXFER__DEST ][ uIx ] == '*' )
               {
                  guTcamCurParm [ TCAM_COM__BXFER__DEST ][ uIx ] = '.';
               }
            }

            memcpy ( cmdInfo.uDest +
                        strlen ( (const char *) cmdInfo.uDest ),
                     (const void *) guTcamCurParm[ TCAM_COM__BXFER__DEST ],
                     strlen ( (const char *)
                                 guTcamCurParm[ TCAM_COM__BXFER__DEST ] ) );
         }
         else
         {
            memcpy ( cmdInfo.uDest + 
                        strlen ( (const char *) cmdInfo.uDest ),
                     (const void *) guTcamCurParm[ TCAM_COM__BXFER__DEST ],
                     strlen ( (const char *)
                                 guTcamCurParm[ TCAM_COM__BXFER__DEST ] ) );
            /* Make sure we append the P-CSCF information to the destination URI.
            */
            if ( strchr ( (const char *) cmdInfo.uDest, '@' ) == NULL )
            {
               BOS_UINT8 *pPcscf = tcamComPcscf ();

               /* We have a proper P-CSCF information, append it now.
               */
               if ( pPcscf && strlen ( (const char *) pPcscf ) )
               {
                  sprintf ( (char *) cmdInfo.uDest +
                                       strlen ( (const char *) cmdInfo.uDest ),
                            "@%s",
                            pPcscf );
               }
               /* We have no P-CSCF information, assume a 'dial-by-subnet' scenario.
               **
               ** In 'dial-by-subnet' mode, the remote end is assumed to be on the
               ** same IP subnet than the caller side and the caller side will use
               ** that assumption to format the target SIP URI.
               */
               else if ( pPcscf && !strlen ( (const char *) pPcscf ) )
               {
                  CFGPROVDEVICECFG *pDevCfg = NULL;
                  PROV_CFG_IDX cfgIdx;
                  BOS_UINT8 *pDelim = NULL;

                  /* Get the device IP address.
                  */
                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  if ( tcamCbCmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                          &cfgIdx,
                                          (void *) &pDevCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     pPcscf = guPcscfIdentity;
                     memset ( (void *) guPcscfIdentity,
                              0,
                              sizeof ( guPcscfIdentity ) ); 

                     bosIpAddrGetStr ( &pDevCfg->IpAddress,
                                       (char *) pPcscf,
                                       TCAM_IP_ADDRESS_STRING_LEN );

                     tcamCbCmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                        &cfgIdx,
                                        (void *) pDevCfg,
                                        CFG_ITEM_DEL );
                     pDevCfg = NULL;
                  }

                  /* Note: for the time being, assume the device IP address is an
                  **       IPv4 address.
                  **
                  ** Replace the last 'octets' of the IP address with the one from
                  ** the destination.
                  **
                  **      - P-CSCF is assumed to be "A1.A2.A3.A4"
                  **      - Current dialed parameter (guTcamCurParm) is assumed to
                  **        be 'xB1' with:
                  **          -> x in [1;n] identifying the n-th user on the
                  **             remote device
                  **          -> B1 the last octet of the remote IP that should be
                  **             of the format " A1.A2.A3.B1"
                  **
                  ** Following the manipulation, the dialed URI will therefore be
                  ** "sip:xB1@A1.A2.A3.B1", originated from "sip:xA4@A1.A2.A3.A4"
                  **        
                  */
                  if ( (pDelim =
                           (BOS_UINT8 *) strrchr ( (const char *) pPcscf, '.' ))
                                                                           != NULL )
                  {
                     sprintf ( /* Start overriding from the last 'octet'. */
                               (char *) (pDelim + 1),
                               "%d",
                               /* Skip the leading 'user' index. */
                               atoi ( (const char *)
                                 &(guTcamCurParm [ TCAM_COM__BXFER__DEST ])[ 1 ] ) );
                  }

                  sprintf ( (char *) cmdInfo.uDest +
                                       strlen ( (const char *) cmdInfo.uDest ),
                            "@%s",
                            pPcscf );
               }
            }
         }

         TCAMLOG_INFORMATION (( "tcamComCmgr: bxfer-service 0x%lX to \'%s\'",
                                cmdInfo.uAppIx, cmdInfo.uDest ));

         /* Send the command to the Call Manager for blind call transfer.
         */
         cmgrCmd( eCMCMD_BXFERSVC, (void *) &cmdInfo );
      }
      break;

      /* This triggers a device restart (fresh boot up).
      */
      case eTCAM_RESTART:
      {
         CMGRRECONFIG cmdInfo;

         memset ( &cmdInfo, 0, sizeof ( CMGRRECONFIG ) );
         cmdInfo.eCfgAction = eCMCFGACT_CFGDEV;

         cmgrCmd( eCMCMD_NEWCONFIG, (void *) &cmdInfo );

         /* Since the action will trigger a full flush of the CMGR (and below)
         ** resources, we might as well clean up the TCAM resources too to avoid
         ** any loss of synch between the different modules.
         */
         tcamResInit ();
      }
      break;

      /* This triggers a user reconfiguration including network credentials.
      */
      case eTCAM_USRCFG:
      {
         CMGRRECONFIG cmdInfo;

         memset ( &cmdInfo, 0, sizeof ( CMGRRECONFIG ) );
         cmdInfo.eCfgAction = eCMCFGACT_CFGUSRWITHNET;
         memcpy ( cmdInfo.uUid,
                  tcamComUser (),
                  sizeof ( cmdInfo.uUid ) );

         cmgrCmd( eCMCMD_NEWCONFIG, (void *) &cmdInfo );
      }
      break;

      /* This triggers a user deletion (termination).
      */
      case eTCAM_USRDEL:
      {
         CMGRRECONFIG cmdInfo;

         memset ( &cmdInfo, 0, sizeof ( CMGRRECONFIG ) );
         cmdInfo.eCfgAction = eCMCFGACT_CFGUSRTERM;
         memcpy ( cmdInfo.uUid,
                  tcamComUser (),
                  sizeof ( cmdInfo.uUid ) );

         cmgrCmd( eCMCMD_NEWCONFIG, (void *) &cmdInfo );
      }
      break;

#if CMGRCFG_PASS_THRU_MSG
      /* MESSAGE, <Local User>, <Call Identifier>, <Destination>, <Message> */
      case eTCAM_MESSAGE:
      {
         CMGRPASSTHRUMSG cmdInfo;

         memset ( (void *) &cmdInfo, 0, sizeof ( cmdInfo ) );
         memcpy ( cmdInfo.toDest.uUid,
                  (const void *) guTcamCurParm[ TCAM_COM__ALL__LCL_USR ],
                  sizeof ( cmdInfo.toDest.uUid ) );
         cmdInfo.toDest.uAppIx = TCAM_INVALID_HANDLE;

         sprintf( (char *) cmdInfo.toDest.uDest,
                  "sip:" );
         /* If the dialled string contains some '*' characters, assume an attempt
         ** to dial by IP.
         */
         if ( strchr (
               (const char *) guTcamCurParm[ TCAM_COM__MSG__DEST ], '*' ) != NULL )
         {
            BOS_UINT32 uIx;
            
            for ( uIx = 0 ;
                  uIx < strlen ( (const char *)
                                    guTcamCurParm[ TCAM_COM__MSG__DEST ] ) ; uIx++ )
            {
               if ( guTcamCurParm [ TCAM_COM__MSG__DEST ][ uIx ] == '*' )
               {
                  guTcamCurParm [ TCAM_COM__MSG__DEST ][ uIx ] = '.';
               }
            }

            memcpy ( cmdInfo.toDest.uDest +
                        strlen ( (const char *) cmdInfo.toDest.uDest ),
                     (const void *) guTcamCurParm[ TCAM_COM__MSG__DEST ],
                     strlen ( (const char *)
                                 guTcamCurParm[ TCAM_COM__MSG__DEST ] ) );
         }
         else
         {
            memcpy ( cmdInfo.toDest.uDest + 
                        strlen ( (const char *) cmdInfo.toDest.uDest ),
                     (const void *) guTcamCurParm[ TCAM_COM__MSG__DEST ],
                     strlen ( (const char *)
                                 guTcamCurParm[ TCAM_COM__MSG__DEST ] ) );
            /* Make sure we append the P-CSCF information to the destination URI.
            */
            if ( strchr ( (const char *) cmdInfo.toDest.uDest, '@' ) == NULL )
            {
               BOS_UINT8 *pPcscf = tcamComPcscf ();

               /* We have a proper P-CSCF information, append it now.
               */
               if ( pPcscf && strlen ( (const char *) pPcscf ) )
               {
                  sprintf ( (char *) cmdInfo.toDest.uDest +
                                       strlen ( (const char *) cmdInfo.toDest.uDest ),
                            "@%s",
                            pPcscf );
               }
               /* We have no P-CSCF information, assume a 'dial-by-subnet' scenario.
               **
               ** In 'dial-by-subnet' mode, the remote end is assumed to be on the
               ** same IP subnet than the caller side and the caller side will use
               ** that assumption to format the target SIP URI.
               */
               else if ( pPcscf && !strlen ( (const char *) pPcscf ) )
               {
                  CFGPROVDEVICECFG *pDevCfg = NULL;
                  PROV_CFG_IDX cfgIdx;
                  BOS_UINT8 *pDelim = NULL;

                  /* Get the device IP address.
                  */
                  memset ( &cfgIdx, 0, sizeof ( PROV_CFG_IDX ) );
                  if ( tcamCbCmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                          &cfgIdx,
                                          (void *) &pDevCfg,
                                          CFG_ITEM_GET ) == BOS_STATUS_OK )
                  {
                     pPcscf = guPcscfIdentity;
                     memset ( (void *) guPcscfIdentity,
                              0,
                              sizeof ( guPcscfIdentity ) ); 

                     bosIpAddrGetStr ( &pDevCfg->IpAddress,
                                       (char *) pPcscf,
                                       TCAM_IP_ADDRESS_STRING_LEN );

                     tcamCbCmgrProvCb ( CFGITEM( PRV, DevConcept ),
                                        &cfgIdx,
                                        (void *) pDevCfg,
                                        CFG_ITEM_DEL );
                     pDevCfg = NULL;
                  }

                  /* Note: for the time being, assume the device IP address is an
                  **       IPv4 address.
                  **
                  ** Replace the last 'octets' of the IP address with the one from
                  ** the destination.
                  **
                  **      - P-CSCF is assumed to be "A1.A2.A3.A4"
                  **      - Current dialed parameter (guTcamCurParm) is assumed to
                  **        be 'xB1' with:
                  **          -> x in [1;n] identifying the n-th user on the
                  **             remote device
                  **          -> B1 the last octet of the remote IP that should be
                  **             of the format " A1.A2.A3.B1"
                  **
                  ** Following the manipulation, the dialed URI will therefore be
                  ** "sip:xB1@A1.A2.A3.B1", originated from "sip:xA4@A1.A2.A3.A4"
                  **        
                  */
                  if ( (pDelim =
                           (BOS_UINT8 *) strrchr ( (const char *) pPcscf, '.' ))
                                                                           != NULL )
                  {
                     sprintf ( /* Start overriding from the last 'octet'. */
                               (char *) (pDelim + 1),
                               "%d",
                               /* Skip the leading 'user' index. */
                               atoi ( (const char *)
                                 &(guTcamCurParm [ TCAM_COM__MSG__DEST ])[ 1 ] ) );
                  }

                  sprintf ( (char *) cmdInfo.toDest.uDest +
                                       strlen ( (const char *) cmdInfo.toDest.uDest ),
                            "@%s",
                            pPcscf );
               }
            }
         }

         /* Assign the message.
         */
         cmdInfo.puMsg = guTcamCurParm [ TCAM_COM__MSG__TXT ];   
         cmdInfo.uSize =
            strlen ( (const char *) guTcamCurParm [ TCAM_COM__MSG__TXT ] );

         TCAMLOG_INFORMATION ((
            "tcamComCmgr: send canned msg (%s, %u) to \'%s\' from \'%s\' (0x%lX)",
            cmdInfo.puMsg, cmdInfo.uSize, cmdInfo.toDest.uDest,
            cmdInfo.toDest.uUid, cmdInfo.toDest.uAppIx ));

         cmgrCmd( eCMCMD_MESSAGE, (void *) &cmdInfo );
      }
      break;
#endif /* CMGRCFG_PASS_THRU_MSG */

      /* SETHANDLE, <Local User>, <Call Identifier>, <Internal Call Identifier> */
      case eTCAM_HANDLE:
      {
         /* Find the temporary handle TCAM assigned.
         */
         BOS_UINT32 uAppIx =
            atoi ( (const char *) guTcamCurParm[ TCAM_COM__HDL__INT_ID ] );
         
         /* Replace it with the handle from the application.
         */
         tcamResReplaceAppIx ( uAppIx,
                               guTcamCurParm[ TCAM_COM__ALL__CALL_ID ] );
      }
      break;

      /* DTMF, <Local User>, <Call Identifier>, <DTMF digit> */
      case eTCAM_DTMF:
      {
         CMGRAPPCALLHDL dtmfPt;

         dtmfPt.uAppIx =
            tcamResFindAppIx ( guTcamCurParm[ TCAM_COM__ALL__CALL_ID ] );
         /* 
         * Handle special cases that would not be leading to a valid
         ** outcome from the 'atoi' conversion.
         */
         if ( strchr ( (const char *) guTcamCurParm[ TCAM_COM__DTMF__VAL ],
                       TCAM_COM__DTMF__PND_CHAR ) )
         {
            dtmfPt.uSesHdl = TCAM_COM__DTMF__PND_VAL;
         }
         else if ( strchr ( (const char *) guTcamCurParm[ TCAM_COM__DTMF__VAL ],
                            TCAM_COM__DTMF__STAR_CHAR ) )
         {
            dtmfPt.uSesHdl = TCAM_COM__DTMF__STAR_VAL;
         }
         else
         {
            dtmfPt.uSesHdl =
               atoi ( (const char *) guTcamCurParm[ TCAM_COM__DTMF__VAL ] );
         }

         TCAMLOG_INFORMATION (( "tcamComCmgr: dtmf-%d on 0x%lX",
                                dtmfPt.uSesHdl, dtmfPt.uAppIx ));

         /* Send the command to the Call Manager for call answer.
         */
         cmgrCmd( eCMCMD_DTMFPT, (void *) &dtmfPt );

         /* Inform the upper layer that this call has been answered by the local
         ** user.
         */
         tcamComWorld ( eTCAM_DTMFPLAY,
                        dtmfPt.uAppIx,
                        (void *) &dtmfPt.uSesHdl );
      }
      break;

      /* Not handled, or ignored on purpose (example ALERT).
      */
      default:
      break;
   }
}


/***********************************************************************************
** FUNCTION:   tcamComWorld
**
** PURPOSE:    Invoke the callback into the outside world to issue an event to
**             the application.
**
** PARAMETERS: eTcamEvt - the TCAM event to notify.
**             uAppIx - the TCAM application handle used to match to the outside
**                      world application handle.
**             pData - extra information data associated with the command.
**
** RETURNS:    None.
**
** NOTE:       Event format from TCAM is:
**
**                'EVENT', <Local User>, <Call Identification>, [<Param>]
**
**             <Call Identification> *always* represents the application call
**             identifier.  When such call identification is not yet known (for
**             example during NEWCALL event report from TCAM), this parameter is
**             therefore <empty>.
**
**             <Param> represents a variable list of parameters, the size of the
**             list and the meaning of each parameter is defined per each event.
**
***********************************************************************************/
void tcamComWorld ( TCAMEVT eTcamEvt, BOS_UINT32 uAppIx, void *pData )
{
   BOS_UINT8 *puAction = tcamMapGetStr ( tcamMapOutEvtStr, eTcamEvt );
   BOS_UINT8 *puCallId = NULL;
   BOS_UINT8 uCallId [ TCAM_COM_ACTION_LEN ];

   TCAMLOG_INFORMATION (( "tcamComWorld: (\'%s\' (0x%lX)):: \'%s\' (from 0x%lX)",
                          puAction ? puAction : (BOS_UINT8 *) "(null)",
                          eTcamEvt,
                          puCallId ? puCallId : (BOS_UINT8 *) "(null)",
                          uAppIx ));

   /* Transform the application index into the proper call identifier unless
   ** the events are not meant to carry this information.
   */
   if ( !((eTcamEvt == eTCAM_DIRINC) ||
          (eTcamEvt == eTCAM_DIROUT) ||
          (eTcamEvt == eTCAM_ANSWERED) ||
          (eTcamEvt == eTCAM_DISC)   ||
          (eTcamEvt == eTCAM_CONFEST) ||
          (eTcamEvt == eTCAM_NEWCALL)) )
   {
      puCallId = (uAppIx != TCAM_INVALID_HANDLE ) ?
                              tcamResFindAppId ( uAppIx ) : NULL;
   }
   else if ( eTcamEvt == eTCAM_NEWCALL )
   {
      sprintf ( (char *) uCallId, "%u", (unsigned int) uAppIx );
      puCallId = uCallId;
   }

   /* If the parameter is NULL at this point with a disconnect action, it
   ** is for confirmation of a call ending, we do not relay this information
   ** to the world.
   */
   if ( ( eTcamEvt == eTCAM_DISCONNECT) &&
        ( puCallId == NULL ) )
   {
      return;
   }

   memset ( (void *) guTcamMsg,
            0,
            sizeof ( guTcamMsg ) );

#ifdef TCAM_COM__MAGIC_SIGNALING
   strPrintf ( (char *) guTcamMsg,
               sizeof ( guTcamMsg ),
               "%c",
               TCAM_COM__MAGIC_SIGNALING );
#endif /* TCAM_COM__MAGIC_SIGNALING */

   /* Event being generated.
   */
   strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
               sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
               "%s, ",
               puAction );

   /* Local user associated with this event.
   */
   strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
               sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
               "%s, ",
               tcamComUser() );

   /* Call identifier on which this event applies.
   */
   strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
               sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
               "%s",
               /* If this value is NULL, the event is not directed to a
               ** call but rather to the user (or it is an invalid event
               ** all together, for the application to determine which).
               */
               puCallId ? puCallId : (BOS_UINT8 *)"" );

   /* Add extra parameter if any needed.
   */
   switch ( eTcamEvt )
   {
      /* For a new call reported by TCAM, we pass the local handle such that the
      ** application can use the SETHANDLE command in return to associate this call
      ** with its own handle.
      */
      case eTCAM_NEWCALL:
      {
         /* The local handle is kept in the same location as the final application
         ** handle, it will be overriden when this latter is set.
         */
         puCallId = tcamResFindAppId ( uAppIx );
         
         strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
                     sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
                     ", %s",
                     puCallId );

         /* Add the remote party number and [optional] remote party name for
         ** user application display.
         **
         ** The remote party number is actually the local handle until this
         ** later gets overwritten by the application.
         */
         strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
                     sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
                     ", %s",
                     puCallId );
      }
      break;

      case eTCAM_MWION:
      case eTCAM_MWIOFF:
      {
         /* Message waiting indicator, pass 1 for enabling, 0 for disabling.
         */
         strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
                     sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
                     ", %d",
                     (eTcamEvt == eTCAM_MWION) ? 1 : 0 );

         /* Pass extra information regarding the number of messages.
         */
      }
      break;

      case eTCAM_DIRINC:
      case eTCAM_DIROUT:
      case eTCAM_DISC:
      case eTCAM_ANSWERED:
      {
         /* Internal event.  Pass the internal application index.
         */
         strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
                     sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
                     ", %d",
                     uAppIx );
      }
      break;

      case eTCAM_CONFEST:
      {
         /* Internal conference event.  Pass the internal application index.
         */
         strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
                     sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
                     ", %d",
                     uAppIx );

         /* Pass the internal conference application index.
         */
         strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
                     sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
                     ", %d",
                     *((BOS_UINT32 *)pData) );
      }
      break;

      case eTCAM_DTMFPLAY:
      {
         /* Internal event.  Pass the internal application index.
         */
         strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
                     sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
                     ", %d",
                     uAppIx );

         /* Data associated with this event should be the DTMF to play out.
         */
         if ( pData != NULL )
         {
            strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
                        sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
                        ", %d",
                        *((int *)pData) );
         }
      }
      break;

      default:
      break;
   }

   /* Pass to the interface module to publish the message outside of TCAM.
   */
   tcamIfcMsg ( guTcamMsg );
}


/***********************************************************************************
** FUNCTION:   tcamComPcscf
**
** PURPOSE:    Retrieve the P-CSCF information for the default user account.
**
** PARAMETERS: Nothing.
**
** RETURNS:    Pointer to the P-CSCF information retrieved.
**
** NOTE:
***********************************************************************************/
BOS_UINT8 * tcamComPcscf ( void )
{
   CFGSIPPCSCF *pPcscfUsr = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_UINT8 *pPcscf = guPcscfIdentity;

   memset ( (void *) guPcscfIdentity,
            0,
            sizeof ( guPcscfIdentity ) ); 

   /* Do not pass any specific handle, this is how we get to the 'default'
   ** P-CSCF configuration information.
   */
   cfgIdx.pcCharIdx = NULL;
   cfgIdx.uIndex = TCAM_INVALID_HANDLE;

   if ( tcamCbCmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                           &cfgIdx,
                           (void *) &pPcscfUsr,
                           CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pPcscfUsr )
      {
         if ( pPcscfUsr->pcPCSCFDomain )
         {
            memcpy ( (void *) pPcscf,
                     (const void *) pPcscfUsr->pcPCSCFDomain,
                     strlen ( (const char *) pPcscfUsr->pcPCSCFDomain ) );
         }
         else
         {
            /* If the P-CSCF address is 'null' (zero address), it means that
            ** the address was either not set or was explicitely null'ed for
            ** a given configuration purpose.  In such case, we do not want to
            ** offer any address, the caller will need to act accordingly.
            */
            if ( !bosIpAddrIsZero ( &pPcscfUsr->PCSCFAddr ) )
            {
               bosIpAddrGetStr ( &pPcscfUsr->PCSCFAddr,
                                 (char *) pPcscf,
                                 TCAM_IP_ADDRESS_STRING_LEN );
            }
         }
      }

      tcamCbCmgrProvCb ( CFGITEM( SIP, PCSCFConcept ),
                         &cfgIdx,
                         (void *) pPcscfUsr,
                         CFG_ITEM_DEL );
      pPcscfUsr = NULL;
   }

   TCAMLOG_DEBUG (( "tcamComPcscf: \'%s\'", pPcscf ));
   return pPcscf;
}


/***********************************************************************************
** FUNCTION:   tcamComUser
**
** PURPOSE:    Retrieve the user information for the default user account.
**
** PARAMETERS: Nothing.
**
** RETURNS:    Pointer to the user information retrieved.
**
** NOTE:       TCAM only supports one user at the time being, so the user
**             retrieved through this call is always the initial user configured.
***********************************************************************************/
BOS_UINT8 * tcamComUser ( void )
{
   CFGSIPUSERCFG *pUsrCfg = NULL;
   PROV_CFG_IDX cfgIdx;
   BOS_UINT8 *pUser = guUsrIdentity;

   memset ( (void *) guUsrIdentity,
            0,
            sizeof ( guUsrIdentity ) ); 

   /* Do not pass any specific handle, this will always return the list of
   ** all users configured, out of which we currently only use the first one.
   */
   cfgIdx.pcCharIdx = NULL;
   cfgIdx.uIndex = TCAM_INVALID_HANDLE;

   if ( tcamCbCmgrProvCb ( CFGITEM( SIP, UserConcept ),
                           &cfgIdx,
                           (void *) &pUsrCfg,
                           CFG_ITEM_GET ) == BOS_STATUS_OK )
   {
      if ( pUsrCfg )
      {
         memcpy ( (void *) pUser,
                  (const void *) pUsrCfg->pcUserId,
                  strlen ( (const char *) pUsrCfg->pcUserId ) ); 
      }

      tcamCbCmgrProvCb ( CFGITEM( SIP, UserConcept ),
                         &cfgIdx,
                         (void *) pUsrCfg,
                         CFG_ITEM_DEL );
      pUsrCfg = NULL;
   }

   TCAMLOG_DEBUG (( "tcamComUser: \'%s\'", pUser ));
   return pUser;
}


/***********************************************************************************
** FUNCTION:   tcamComCfg
**
** PURPOSE:    Invoke the handler for configuration update to the GUI application
**             when such request is received.
**
** PARAMETERS: eEvt - the event to publish.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
void tcamComCfg ( TCAMEVT eEvt )
{
   /* Currently this function is only used to pass the user (extension)
   ** information up to the outside world, for allowing the GUI to show
   ** that piece of data.
   */
   BOS_UINT8 *puAction = tcamMapGetStr ( tcamMapOutEvtStr, eEvt );
   BOS_UINT8 *puParam = tcamComUser ();

   memset ( (void *) guTcamMsg,
            0,
            sizeof ( guTcamMsg ) );

#ifdef TCAM_COM__MAGIC_SIGNALING
   strPrintf ( (char *) guTcamMsg,
               sizeof ( guTcamMsg ),
               "%c",
               TCAM_COM__MAGIC_SIGNALING );
#endif /* TCAM_COM__MAGIC_SIGNALING */

   /* Action is guaranteed to be present.
   */
   strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
               sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
               "%s,",
               puAction );

   if ( puParam )
   {
      strPrintf ( (char *) (guTcamMsg + strlen ( (const char *) guTcamMsg )),
                  sizeof ( guTcamMsg ) - strlen ( (const char *) guTcamMsg ),
                  "%s",
                  puParam );
   }

   /* Pass to the interface module to publish the message outside of TCAM.
   */
   tcamIfcMsg ( guTcamMsg );
}


/***********************************************************************************
** FUNCTION:   tcamComValid
**
** PURPOSE:    Applies some validation on a parsed message content based on the
**             event type and the arguments given.
**
** PARAMETERS: eTcamEvt - the TCAM event.
**             uArgs - the number of arguments processed.
**
** RETURNS:    None.
**
** NOTE:       .
***********************************************************************************/
BOS_BOOL tcamComValid ( TCAMEVT eTcamEvt, BOS_UINT32 uArgs )
{
   BOS_BOOL bRet = BOS_TRUE;

   /* Basic validation.  Validate the number of arguments per command.
   */
   switch ( eTcamEvt )
   {
      case eTCAM_DIAL:
      case eTCAM_CONF:
      case eTCAM_BXFER:
      case eTCAM_CXFER:
      case eTCAM_HANDLE:
      case eTCAM_DTMF:
      {
         bRet = !TCAM_COM__CHK_ARGS( uArgs, TCAM_COM__DIAL__DEST );
      }
      break;

      case eTCAM_MESSAGE:
      {
         bRet = !TCAM_COM__CHK_ARGS( uArgs, TCAM_COM__MSG__TXT );
      }
      break;

      case eTCAM_HANGUP:
      case eTCAM_ANSWER:
      case eTCAM_ALERT:
      case eTCAM_HOLD:
      case eTCAM_UNHOLD:
      {
         bRet = !TCAM_COM__CHK_ARGS( uArgs, TCAM_COM__ALL__CALL_ID );
      }
      break;

      default:
      break;
   }

   /* Advanced validation.  Validate the type and value of the arguments
   ** per command.
   */

   return bRet;
}
