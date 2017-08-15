/**********************************************************************************
** Copyright (c) 2007-2012 Broadcom Corporation
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
**      This file implements the CCTK Framework Class.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkFramework.h>
#include <cctkInc.h>

MX_NAMESPACE_START( MXD_GNS )

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
/* Timer storage block */
typedef struct TKMTMRBLK
{
   BOS_BOOL bRecurring;
   void     *pData;
   TKMMSGCB tmrCb;
} TKMTMRBLK;

/* ---- Private Variables ------------------------------------------------------- */

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */

/* ---- Functions --------------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   CctkFramework::CctkFramework
**
**  PURPOSE:    Constructor of CCTK Framework object.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    Nothing
**
**  NOTES:      Order of initializations is significant. The status of the CCTK
**              Framework initialization can be retrieved through getInitStatus().
***********************************************************************************/
CctkFramework::CctkFramework()
   : mpReleaseSem( NULL )
{
   /* No trace is added here because M5T trace utility has not been enabled yet.
   ** Trace can be added once CCTK custom tracing support is implemented.
   */

   /* -- Internal validity check -- */
   /* Check for status code mis-alignment */
   BOS_SINT32 sCodeSize =
      ( eTKM_MSGID_ASYNCFUNC_END - eTKM_MSGID_ASYNCFUNC_START + 1 ) +
      ( eTKM_MSGID_SYNCFUNC_END - eTKM_MSGID_SYNCFUNC_START + 1 );
   CCTK_ASSERT( sCodeSize == eTKM_MSGID_MAX );
   /* -- Internal validity check (END) -- */

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   /* Start up configuration framework.
   ** Config framework MUST be the first to start up for loading the default
   ** extended system config.
   */
   CFGSTATUS eCfgStatus = cfgInit();
   if ( CFGSTATUS_IS_FAIL( eCfgStatus ) )
   {
      CCTK_TRACEI1(
         "CctkFramework: Error initializing config framework (%d)", eCfgStatus );
      eStatus = eCCTKSTATUS_NORSC;
   }

   /* Activate Utility module */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      UTILSTATUS eUtilStatus = utilInit();
      if ( eUtilStatus != eUTILSTATUS_SUCCESS )
      {
         CCTK_TRACEI1(
            "CctkFramework: Error initializing Utility module (%d)", eUtilStatus );
         eStatus = eCCTKSTATUS_NORSC;
      }
   }

   /* Activate other CCTK modules */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* Initialize Socket Control */
      nsScInit();

      /* Initialize Payload Modifier */
      pmInit();

#if CCTKCFG_ICE_LITE_SUPPORT
      /* Start ICE before NAT, as NAT doesn't have Crc32 and HMAC functions for STUN module */
      nsIceInit();
#endif

      /* Initialize NAT */
      nsNatInit();

      /* Initialize Backup and Recovery */
      nsBrInit();

      /* Initialize Call Engine */
      ceInit();

      /* Initialize MSE */
      mseInit();
   }

   CCTK_TRACED2( "CctkFramework(%p): Initialization status = %d.", this, eStatus );

   meInitStatus = eStatus;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::~CctkFramework
**
**  PURPOSE:    Destructor of the CCTK Framework object.
**
**  PARAMETERS:
**              None
**
**  RETURNS:    Nothing
**
**  NOTES:      Order of de-initializations is significant. It should follow the
**              reverse order of the initializations performed in the constructor.
***********************************************************************************/
CctkFramework::~CctkFramework()
{
   /* No trace is added here because M5T trace utility may not be enabled at this
   ** point. Trace can be added once CCTK custom tracing support is implemented.
   */

   /* De-initialize MSE */
   mseDeInit();

   /* De-initialize Call Engine */
   ceDeInit();

   /* De-initialize Backup and Recovery */
   nsBrDeInit();

   /* De-initialize NAT */
   nsNatDeInit();

#if CCTKCFG_ICE_LITE_SUPPORT
   /* De-initialize ICE */
   nsIceDeInit();
#endif

   /* De-initialize Payload Modifier */
   pmDeInit();

   /* De-initialize Utility */
   UTILSTATUS eUtilStatus = utilDeInit();
   CCTK_ASSERT( eUtilStatus == eUTILSTATUS_SUCCESS );

   /* De-initialize Config Framework */
   CFGSTATUS eCfgStatus = cfgDeInit();
   CCTK_ASSERT( CFGSTATUS_IS_SUCCESS( eCfgStatus ) );

   /* Notify the application when CCTK Framework object is completely destroyed */
   if ( mpReleaseSem != NULL )
   {
      mpReleaseSem->Signal();
   }
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::reset
**
**  PURPOSE:    Cleans up CCTK resources.
**
**  PARAMETERS:
**              eRstMode - Reset mode to be applied for this particular action.
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::reset( CCTKRSTMODE eRstMode )
{
   CCTK_TRACED2( "CctkFramework(%p) - reset(%d)", this, eRstMode );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << eRstMode;

   mxt_result res = PostMessage( false, eTKM_MSGID_RESET, pParms );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - reset: Unable to asynchronously post "
                    "eTKM_MSGID_RESET message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      pParms = NULL;

      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalReset
**
**  PURPOSE:    Handles the CCTK reset event.
**
**              Cleans up CCTK resources.
**
**  PARAMETERS:
**              pParms - parameter block.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalReset( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalReset(%p)", this, pParms );

   int sRstMode;
   CCTKRSTMODE eRstMode;

   *pParms >> sRstMode;
   eRstMode = static_cast< CCTKRSTMODE >( sRstMode );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_RESET ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalReset: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   /* Release all Call Engine / MSE resources */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( guCeInvalidGuid,
                          eCEEVT_RESET,
                          static_cast<void *> (&eRstMode) );
   }

   /* Release all Config Framework resources if necessary, or wait for
   ** further action triggered by the Call Engine.
   */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) &&
        ( eRstMode == eCCTKRSTMODE_IMMEDIATE ) )
   {
      internalResetCont();
   }
   else if ( !CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* Report failure event to the application */
      tkmEvent ( guCeInvalidGuid,
                 guCeInvalidGuid,
                 eCCTKEVT_FN_RESET_FAIL,
                 reinterpret_cast< void* >( &eStatus ) );
   }
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::internalResetCont
**
**  PURPOSE:    Handles the continuation of the CCTK reset event.
**
**              Cleans up CCTK resources.
**
**  PARAMETERS:
**              pParms - parameter block.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalResetCont( void )
{
   CCTK_TRACED1( "CctkFramework(%p) - internalResetCont", this );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   CFGSTATUS eCfgStatus = cfgReset();
   if ( CFGSTATUS_IS_FAIL( eCfgStatus ) )
   {
      CCTK_TRACEI1( "CctkFramework - internalResetCont: "
         "Error resetting Config Framework resources (%d).",
         eCfgStatus );
      eStatus = eCCTKSTATUS_INTERR;
   }

   /* Report final event to the application */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_RESET_SUCCESS :
                    eCCTKEVT_FN_RESET_FAIL;
   tkmEvent ( guCeInvalidGuid,
              guCeInvalidGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::config
**
**  PURPOSE:    Configure CCTK after startup.
**
**  PARAMETERS:
**              pCfg - Configuration block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::config( const CCTKCFG *pCfg )
{
   CCTK_TRACED2( "CctkFramework(%p) - config(%p)", this, pCfg );

   /* Assert only because it should have been checked in cctkApi.cpp */
   CCTK_ASSERT( pCfg != NULL );

   CCTKCFG *pCfgBlk = cfgDupSystem( pCfg );
   if ( pCfgBlk == NULL )
   {
      CCTK_TRACEI0( "CctkFramework - config: Error copying configuration." );
      return eCCTKSTATUS_BADPARM;
   }

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << pCfgBlk;

   mxt_result res = PostMessage( false, eTKM_MSGID_CONFIG, pParms );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - config: Unable to asynchronously post "
                    "eTKM_MSGID_CONFIG message (%d)",
                    res );

      cfgDelSystem( pCfgBlk );
      CPool<CMarshaler>::Delete( pParms );
      pParms = NULL;

      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalConfig
**
**  PURPOSE:    Handles the CCTK config event.
**
**              Configures CCTK after startup.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalConfig( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalConfig(%p)", this, pParms );

   CCTKCFG *pCfgBlk;
   *pParms >> pCfgBlk;

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CONFIG ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalConfig: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   CCTK_ASSERT( pCfgBlk != NULL );
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) && pCfgBlk == NULL )
   {
      CCTK_TRACEI0( "CctkFramework - internalConfig: NULL config block pointer." );
      eStatus = eCCTKSTATUS_BADPARM;
   }

   /* Load input config block to config framework */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      CFGSTATUS eCfgStatus = cfgSetupSystem( pCfgBlk );
      if ( CFGSTATUS_IS_FAIL( eCfgStatus ) )
      {
         CCTK_TRACEI1( "CctkFramework - internalConfig: "
            "Error setting up system config and configuring CCTK (%d).",
            eCfgStatus );
         eStatus = eCCTKSTATUS_INTERR;
      }
   }

   /* Report event to the app on failure. Success event is deferred until
   ** stack is configured (transitioned to configured state).
   */
   if ( CCTKSTATUS_IS_FAIL( eStatus ) )
   {
      tkmEvent ( guCeInvalidGuid,
                 guCeInvalidGuid,
                 eCCTKEVT_FN_CONFIG_FAIL,
                 reinterpret_cast< void* >( &eStatus ) );
   }

   /* Clean-ups */
   cfgDelSystem( pCfgBlk );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::setParmHelper
**
**  PURPOSE:    Set parameter for id synchronously.
**
**  PARAMETERS:
**              uId     - Object id to set parameter.
**              eParm   - Parameter id.
**              pVal    - Parameter value.
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is synchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::setParmHelper( BOS_UINT32 uId, 
                                         CCTKPARM eParm, 
                                         const void *pVal )
{
   CCTK_TRACED4(
      "CctkFramework(%p) - setParmHelper(0x%lX, %d, %p)", this, uId, eParm, pVal );

   /* Assert only because it should have been checked in cctkApi.cpp */
   CCTK_ASSERT( CCTK_IS_CFGPARM( eParm ) );
   CCTK_ASSERT( pVal != NULL );

   void *pDupVal = cfgDupXtlCfg( eParm, pVal );
   if ( pDupVal == NULL )
   {
      CCTK_TRACEI0( "CctkFramework - setParmHelper: Error copying configuration." );
      return eCCTKSTATUS_BADPARM;
   }

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uId
           << eParm
           << pDupVal
           << BOS_FALSE;

   internalSetParm( pParms );

   /* Clean-ups */
   CPool<CMarshaler>::Delete( pParms );
   pParms = NULL;

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::setParm
**
**  PURPOSE:    Set parameter for id.
**
**  PARAMETERS:
**              uId     - Object id to set parameter.
**              eParm   - Parameter id.
**              pVal    - Parameter value.
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::setParm( BOS_UINT32 uId,
                                   CCTKPARM eParm,
                                   const void *pVal )
{
   CCTK_TRACED4(
      "CctkFramework(%p) - setParm(0x%lX, %d, %p)", this, uId, eParm, pVal );

   /* Assert only because it should have been checked in cctkApi.cpp */
   CCTK_ASSERT( CCTK_IS_CFGPARM( eParm ) );
   CCTK_ASSERT( pVal != NULL );

   void *pDupVal = cfgDupXtlCfg( eParm, pVal );
   if ( pDupVal == NULL )
   {
      CCTK_TRACEI0( "CctkFramework - setParm: Error copying configuration." );
      return eCCTKSTATUS_BADPARM;
   }

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uId
           << eParm
           << pDupVal
           << BOS_TRUE;

   mxt_result res = PostMessage( false, eTKM_MSGID_SETPARM, pParms );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - setParm: unable to asynchronously post "
                    "eTKM_MSGID_SETPARM message (%d)",
                    res );

      cfgDelXtlCfg( eParm, pDupVal );
      CPool<CMarshaler>::Delete( pParms );
      pParms = NULL;

      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalSetParm
**
**  PURPOSE:    Handles the SetParm event.
**
**              Set parameter for id.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalSetParm( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalSetParm(%p)", this, pParms );

   BOS_UINT32 uId;
   CCTKPARM eParm;
   int sParm;
   void *pVal;
   BOS_BOOL bNotifyApp;

   *pParms >> uId
           >> sParm
           >> pVal
           >> bNotifyApp;
   eParm = static_cast< CCTKPARM >( sParm );

   BOS_UINT32 uUserGuid, uGuid;
   getId( uId, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_SETPARM ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalSetParm: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   CCTK_ASSERT( CCTK_IS_CFGPARM( eParm ) );
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) && !CCTK_IS_CFGPARM( eParm ) )
   {
      CCTK_TRACEI1( "CctkFramework - internalSetParm: Invalid parameter (%d).",
                    eParm );
      eStatus = eCCTKSTATUS_BADPARM;
   }

   CCTK_ASSERT( pVal != NULL );
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) && pVal == NULL )
   {
      CCTK_TRACEI0( "CctkFramework - internalSetParm: NULL value pointer." );
      eStatus = eCCTKSTATUS_BADPARM;
   }

   /* Set config parameter value */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      switch ( eParm )
      {
         /* This information is managed by the Call Engine
         ** directly (call statistics information.
         */
         case eCCTKPARM_EC_CALLSTATS:
         {
            uUserGuid = (static_cast<CCTKCALLMETRICS *> (pVal))->uUserGuid;
            
            if ( !ceSetCallInfo( uId,
                                 static_cast<CCTKCALLMETRICS *> (pVal) ) )
            {
               eStatus = eCCTKSTATUS_INTERR;
            }
         }
         break;

         /* This information is also managed by the Call Engine
         ** for reference.
         */
         case eCCTKPARM_EC_CALLPRIV:
         {
             ceSetPrivInfo( uId, static_cast<CCTKPRIVCFG *> (pVal) );
         }
         /* Note: Intentional fall through */
         default:
         {
            CFGSTATUS eCfgStatus = cfgSetXtlCfg( uId, eParm, pVal );
            if ( CFGSTATUS_IS_FAIL( eCfgStatus ) )
            {
               CCTK_TRACEI2( "CctkFramework - internalSetParm: "
                  "Error setting external config parameter %d (%d).",
                  eParm,
                  eCfgStatus );
               eStatus = eCCTKSTATUS_INTERR;
            }
         }
      }
   }

   /* Report event to the app */
   if ( bNotifyApp )
   {
      CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                       eCCTKEVT_FN_SETPARM_SUCCESS :
                       eCCTKEVT_FN_SETPARM_FAIL;
      tkmEvent ( uUserGuid,
                 uGuid,
                 eFnEvt,
                 reinterpret_cast< void* >( &eStatus ) );
   }

   /* Clean-ups */
   cfgDelXtlCfg( eParm, pVal );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::getParm
**
**  PURPOSE:    Get parameter for id.
**
**  PARAMETERS:
**              uId     - Object id to set parameter.
**              eParm   - Parameter id.
**              pVal    - Parameter value.
**
**  RETURNS:    Return status.
**
**  NOTES:      - This function is synchronous.
**              - Currently intended for getting user configuration only.
***********************************************************************************/
CCTKSTATUS CctkFramework::getParm( BOS_UINT32 uId, CCTKPARM eParm, void *pVal )
{
   CCTK_TRACED4(
      "CctkFramework(%p) - getParm(0x%lX, %d, %p)", this, uId, eParm, pVal );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;
   CCTKSTATUS *pStatus = (CCTKSTATUS *) &eStatus;

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uId
           << eParm
           << pVal
           << pStatus;

   /* The message would be processed synchronously */
   mxt_result res = PostMessage( true, eTKM_MSGID_GETPARM, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - getParm: unable to synchronously post "
                    "eTKM_MSGID_GETPARM message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return *pStatus;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalGetParm
**
**  PURPOSE:    Handles the GetParm event.
**
**              Get parameter for id.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalGetParm( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalGetParm(%p)", this, pParms );

   BOS_UINT32 uId;
   CCTKPARM eParm;
   int sParm;
   void *pVal;
   void *pStatus;
   CCTKSTATUS *peStatus;

   *pParms >> uId
           >> sParm
           >> pVal
           >> pStatus;
   eParm = static_cast< CCTKPARM >( sParm );
   peStatus = reinterpret_cast<CCTKSTATUS *>( pStatus );

   if ( !cfgIsFuncAllowed( eTKM_MSGID_GETPARM ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalGetParm: "
         "Function not allowed to be called in current state." );
      *peStatus = eCCTKSTATUS_BADSTATE;
   }

   /* Get/copy parameter value here */
   switch ( eParm )
   {
      case eCCTKPARM_EXPINFO:
      {
         CCTKEXPIREINFO *pExpInfo, *pExpInfoCur;
         pExpInfo = reinterpret_cast<CCTKEXPIREINFO *>( pVal );

         pExpInfoCur = ceGetExpInfo( uId );
         if ( pExpInfoCur )
         {
            *pExpInfo = *pExpInfoCur;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_MEDINFO:
      {
         CCTKMEDINFO *pMedInfo, *pMedInfoCur;
         pMedInfo = reinterpret_cast<CCTKMEDINFO *>( pVal );
         pMedInfoCur = ceGetMediaInfo( uId );
         if ( pMedInfoCur )
         {
            *pMedInfo = *pMedInfoCur;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_LASTCALLIN:
      {
         CCTKCDR *pCdr, *pCdrIn;
         pCdr = reinterpret_cast<CCTKCDR *>( pVal );
         pCdrIn = ceGetLastCallIn( uId );
         if ( pCdrIn )
         {
            *pCdr = *pCdrIn;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_LASTCALLOUT:
      {
         CCTKCDR *pCdr, *pCdrOut;
         pCdr = reinterpret_cast<CCTKCDR *>( pVal );
         pCdrOut = ceGetLastCallOut( uId );
         if ( pCdrOut )
         {
            *pCdr = *pCdrOut;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_FEATINFO:
      {
         CCTKFEATINFO *pFeatInfo;
         pFeatInfo = reinterpret_cast<CCTKFEATINFO *>( pVal );
         ceGetFeatInfo( uId, pFeatInfo );
         break;
      }
      case eCCTKPARM_EC_MEDNEGCFG:
      {
         CCTKMEDNEGCFG *pMedNegCfg;
         pMedNegCfg = reinterpret_cast<CCTKMEDNEGCFG *>( pVal );

         const CFGCALL *pCfgCall = CFG_CALL( uId );          
         if ( pCfgCall )
         {
            *pMedNegCfg = pCfgCall->xtl.media.neg;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_MC_MEDIACFG:
      {
         CCTKMEDCFG *pMediaCfg; 
         pMediaCfg = reinterpret_cast<CCTKMEDCFG *>( pVal );
         
         const CFGCALL *pCfgCall = CFG_CALL( uId );
         if ( pCfgCall )
         {
            *pMediaCfg = *reinterpret_cast< const CCTKMEDCFG *> (&(pCfgCall->xtl.media));
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;         
      }
      case eCCTKPARM_EC_CALLHOLD:
      {
         CCTKHOLDCFG *pCallHoldCfg;
         pCallHoldCfg = reinterpret_cast<CCTKHOLDCFG *>( pVal );

         const CFGCALL *pCfgCall = CFG_CALL( uId );          
         if ( pCfgCall )
         {
            *pCallHoldCfg = pCfgCall->xtl.supp.hold;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_EC_CALLXFER:
      {
         CCTKXFERCFG *pXferCfg;
         pXferCfg = reinterpret_cast<CCTKXFERCFG *>( pVal );

         const CFGUSR *pCfgUsr = CFG_USR( uId );          
         if ( pCfgUsr )
         {
            *pXferCfg = pCfgUsr->xtl.dft.call.supp.xfer;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_EC_USRBRCFG:
      {
         CCTKUSRBRCFG *pUsrBrCfg;
         pUsrBrCfg = reinterpret_cast<CCTKUSRBRCFG *>( pVal );

         const CFGUSR *pCfgUsr = CFG_USR( uId );          
         if ( pCfgUsr )
         {
            *pUsrBrCfg = pCfgUsr->xtl.usr.net.brCfg;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_EC_REGTMR:
      {
         CCTKREGTMR *pRegTmrCfg;
         pRegTmrCfg = reinterpret_cast<CCTKREGTMR *>( pVal );

         const CFGUSR *pCfgUsr = CFG_USR( uId );          
         if ( pCfgUsr )
         {
            *pRegTmrCfg = pCfgUsr->xtl.reg.gen.tmr;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_EC_UAIDHDR:
      {
         CCTKUAIDHDR *pUaIdHdr;
         pUaIdHdr = reinterpret_cast<CCTKUAIDHDR *>( pVal );

         const CFGSYS *pCfgSys = CFG_SYS;          
         if ( pCfgSys )
         {
            *pUaIdHdr = pCfgSys->xtl.net.prot.gen.uaIdHdr;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_EC_EVTTMR:
      {
         CCTKEVTTMR *pEvtTmrCfg;
         pEvtTmrCfg = reinterpret_cast<CCTKEVTTMR *>( pVal );

         const CFGUSR *pCfgUsr = CFG_USR( uId );          
         if ( pCfgUsr )
         {
            *pEvtTmrCfg = pCfgUsr->xtl.dft.evt.gen.tmr;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_EC_SESTMR:
      {
         CCTKSESTMR *pSesTmrCfg;
         pSesTmrCfg = reinterpret_cast<CCTKSESTMR *>( pVal );

         const CFGUSR *pCfgUsr = CFG_USR( uId );          
         if ( pCfgUsr )
         {
            *pSesTmrCfg = pCfgUsr->xtl.dft.call.basic.adv.sesTmr;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      case eCCTKPARM_EC_UAPEVTCFG:
      {
         CCTKUAPEVTCFG *pUapCfg;
         pUapCfg = reinterpret_cast<CCTKUAPEVTCFG *>( pVal );

         const CFGUSR *pCfgUsr = CFG_USR( uId );          
         if ( pCfgUsr )
         {
            *pUapCfg = pCfgUsr->xtl.usr.net.uapEvt;
         }
         else
         {
            *peStatus = eCCTKSTATUS_BADPARM;
         }
         break;
      }
      default:
      {
         *peStatus = eCCTKSTATUS_BADPARM;
         CCTK_TRACED1( "internalGetParm - Invalid parameter id %d", eParm );
         break;
      }
   }
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::userAdd
**
**  PURPOSE:    Add a user to the system.
**
**  PARAMETERS:
**              uUid - User id (id created on return).
**              pCfg - User config block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::userAdd( BOS_UINT32 uUid, const CCTKUSRCFG *pCfg )
{
   CCTK_TRACED3( "CctkFramework(%p) - userAdd(0x%lX, %p)", this, uUid, pCfg );

   /* Assert only because it should have been checked in cctkApi.cpp */
   CCTK_ASSERT( pCfg != NULL );

   CCTKUSRCFG *pUsrCfg = cfgDupUsrCfg( pCfg );
   if ( pUsrCfg == NULL )
   {
      CCTK_TRACEI0( "CctkFramework - userAdd: Error copying configuration." );
      return eCCTKSTATUS_BADPARM;
   }

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uUid
           << pUsrCfg;

   mxt_result res = PostMessage( false, eTKM_MSGID_USERADD, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - userAdd: unable to asynchronously post "
                    "eTKM_MSGID_USERADD message (%d)",
                    res );

      cfgDelUsrCfg( pUsrCfg );
      CPool<CMarshaler>::Delete( pParms );

      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalUserAdd
**
**  PURPOSE:    Handles the UserAdd event.
**
**              Add a user to the system.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalUserAdd( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalUserAdd(%p)", this, pParms );

   BOS_UINT32 uUid;
   CCTKUSRCFG* pUsrCfg;

   *pParms >> uUid
           >> pUsrCfg;

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_USERADD ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalUserAdd: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   CCTK_ASSERT( pUsrCfg != NULL );
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) && pUsrCfg == NULL )
   {
      CCTK_TRACEI0( "CctkFramework - internalUserAdd: NULL config block pointer." );
      eStatus = eCCTKSTATUS_BADPARM;
   }

   /* Create the Config Framework resource to handle this user */
   CFGSTATUS eCfgStatus;
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eCfgStatus = cfgAddUsr( uUid, pUsrCfg );
      if ( eCfgStatus == eCFGSTATUS_NOTCMPL )
      {
         /* User remove operation has not been completed yet due to persistent
         ** connection termination. Fail user add operation.
         */
         CCTK_TRACEI1( "CctkFramework - internalUserAdd: "
            "User remove operation in progress (0x%1X).",
            uUid );
         eStatus = eCCTKSTATUS_CNXNOTREADY;
      }
      else if ( CFGSTATUS_IS_FAIL( eCfgStatus ) )
      {
         CCTK_TRACEI2( "CctkFramework - internalUserAdd: "
            "Error creating user config resource (0x%1X, %d).",
            uUid,
            eCfgStatus );
         eStatus = eCCTKSTATUS_INTERR;
      }
   }

   /* Create the Call Engine resource to handle this user */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uUid, eCEEVT_USR_ADD, NULL );
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* In the case when keep-alive is configured, defer success function event
      ** until the persistent connections are established. Otherwise, report
      ** success function event immediately.
      */
      if ( !CFG_USR( uUid )->in.bDeferEvtAdd )
      {
         tkmEvent ( uUid,
                    guCeInvalidGuid,
                    eCCTKEVT_FN_USERADD_SUCCESS,
                    reinterpret_cast< void* >( &eStatus ) );
      }
   }
   else
   {
      /* Report failure function event immediately to the app */
      tkmEvent ( uUid,
                 guCeInvalidGuid,
                 eCCTKEVT_FN_USERADD_FAIL,
                 reinterpret_cast< void* >( &eStatus ) );

      /* Clean up on failure.
      ** NOTE: Return status is not important here.
      */
      cfgRemoveUsr( uUid );
   }

   /* Clean up */
   cfgDelUsrCfg( pUsrCfg );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::userRegister
**
**  PURPOSE:    Sends a REGISTER to register the configured user.
**
**  PARAMETERS:
**              uUid - User id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::userRegister( BOS_UINT32 uUid )
{
   CCTK_TRACED2( "CctkFramework(%p) - userRegister(0x%lX)", this, uUid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uUid;

   mxt_result res = PostMessage( false, eTKM_MSGID_USERREGISTER, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - userRegister: unable to asynchronously post "
                    "eTKM_MSGID_USERREGISTER message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalUserRegister
**
**  PURPOSE:    Handles the UserRegister event.
**
**              Sends a REGISTER to register the configured user.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalUserRegister( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalUserRegister(%p)", this, pParms );

   BOS_UINT32 uUid;

   *pParms >> uUid;

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_USERREGISTER ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalUserRegister: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uUid, eCEEVT_REG_USR_REG, NULL );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_USERREGISTER_SUCCESS :
                    eCCTKEVT_FN_USERREGISTER_FAIL;
   tkmEvent ( uUid,
              guCeInvalidGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::userUnregister
**
**  PURPOSE:    Send a REGISTER with expires=0 to unregister the contact.
**
**  PARAMETERS:
**              uUid    - User id.
**              bClear  - Set clear to TRUE to unregister all registrar-cached
**                        contacts associated with the Address of Record (AoR).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::userUnregister( BOS_UINT32 uUid, BOS_BOOL bClear )
{
   CCTK_TRACED3( "CctkFramework(%p) - userUnregister(0x%lX, %d)", this, uUid, bClear );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uUid
           << bClear;

   mxt_result res = PostMessage( false, eTKM_MSGID_USERUNREGISTER, pParms );

   if (MX_RIS_F( res ))
   {
      CCTK_TRACEI1( "CctkFramework - userUnregister: unable to asynchronously post "
                    "eTKM_MSGID_USERUNREGISTER message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalUserUnregister
**
**  PURPOSE:    Handles the UserUnregister event.
**
**              Send a REGISTER with expires=0 to unregister the contact.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalUserUnregister( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalUserUnregister(%p)", this, pParms );

   BOS_UINT32 uUid;
   BOS_BOOL bClear;

   *pParms >> uUid
           >> bClear;

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_USERUNREGISTER ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalUserUnregister: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uUid, eCEEVT_REG_USR_UNREG, NULL );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_USERUNREGISTER_SUCCESS :
                    eCCTKEVT_FN_USERUNREGISTER_FAIL;
   tkmEvent ( uUid,
              guCeInvalidGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::userRemove
**
**  PURPOSE:    Remove the user from the system.
**
**  PARAMETERS:
**              uUid - User id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::userRemove( BOS_UINT32 uUid )
{
   CCTK_TRACED2( "CctkFramework(%p) - userRemove(0x%lX)", this, uUid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uUid;

   mxt_result res = PostMessage( false, eTKM_MSGID_USERREMOVE, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - userRemove: unable to asynchronously post "
                    "eTKM_MSGID_USERREMOVE message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalUserRemove
**
**  PURPOSE:    Handles the UserRemove event.
**
**              Remove the user from the system.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalUserRemove( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalUserRemove(%p)", this, pParms );

   BOS_UINT32 uUid;

   *pParms >> uUid;

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_USERREMOVE ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalUserRemove: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   /* Remove the Config Framework resource which handles this user */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      CFGSTATUS eCfgStatus = cfgRemoveUsr( uUid );
      if ( eCfgStatus == eCFGSTATUS_NOTCMPL )
      {
         /* User remove operation has not been completed yet. Nullify this command
         ** by returning immediately without sending any function event.
         */
         CCTK_TRACEI1( "CctkFramework - internalUserRemove: "
            "User removing in progress (0x%1X).",
            uUid );
         return;
      }
      else if ( CFGSTATUS_IS_FAIL( eCfgStatus ) )
      {
         CCTK_TRACEI1( "CctkFramework - internalUserRemove: "
            "Error removing user config resource (%d).",
            eCfgStatus );
         eStatus = eCCTKSTATUS_INTERR;
      }
   }

   /* Remove the Call Engine resource which handles this user */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uUid, eCEEVT_USR_REM, NULL );
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* In the case when keep-alive is configured, defer success function event
      ** until the persistent connections are terminated completely.
      ** Otherwise, report success function event immediately.
      */
      if ( CFG_USR( uUid ) == NULL || !CFG_USR( uUid )->in.bDeferEvtRemove )
      {
         tkmEvent ( uUid,
                    guCeInvalidGuid,
                    eCCTKEVT_FN_USERREMOVE_SUCCESS,
                    reinterpret_cast< void* >( &eStatus ) );
      }
   }
   else
   {
      /* Report failure function event immediately to the app */
      tkmEvent ( uUid,
                 guCeInvalidGuid,
                 eCCTKEVT_FN_USERREMOVE_FAIL,
                 reinterpret_cast< void* >( &eStatus ) );
   }
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::eventCreate
**
**  PURPOSE:    Create an event object for subscription.
**
**  PARAMETERS:
**              uEid       - Subscription event id.
**              uUid       - User id for the subscription.
**              eSubtype   - Subscription event type (e.g. reg-event).
**              uExpTime   - Subscription expiration time (must be non-zero).
**              pcTo       - Subscription destination address (URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::eventCreate( BOS_UINT32 uEid,
                                       BOS_UINT32 uUid,
                                       CCTKSUBEVT eSubtype,
                                       BOS_UINT32 uExpTime,
                                       const BOS_UINT8 *pcTo )
{
   CCTK_TRACED4(
      "CctkFramework(%p) - eventCreate(0x%lX, %d, %lu)",
      this,
      uEid,
      eSubtype,
      uExpTime );

   CMarshaler *pParms = CPool<CMarshaler>::New();
   BOS_UINT8 *pcToAddr = utilStrDup( pcTo );

   *pParms << uEid
           << uUid
           << eSubtype
           << uExpTime
           << pcToAddr;

   mxt_result res = PostMessage( false, eTKM_MSGID_EVENTCREATE, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - eventCreate: unable to asynchronously post "
                    "eTKM_MSGID_EVENTCREATE message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      BRCM_DELETE_ARRAY( pcToAddr );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalEventCreate
**
**  PURPOSE:    Handles the EventCreate event.
**
**              Create an event object for subscription.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalEventCreate( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalEventCreate(%p)", this, pParms );

   BOS_UINT32 uEid, uUid, uExpTime;
   CCTKSUBEVT eSubtype;
   int sSubtype;
   BOS_UINT8 *pcToAddr;

   *pParms >> uEid
           >> uUid
           >> sSubtype
           >> uExpTime
           >> pcToAddr;
   eSubtype = static_cast< CCTKSUBEVT >( sSubtype );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_EVENTCREATE ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalEventCreate: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   CCTK_ASSERT( uExpTime > 0 && pcToAddr != NULL );
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) && ( uExpTime == 0 || pcToAddr == NULL ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalEventCreate: Bad parameters." );
      eStatus = eCCTKSTATUS_BADPARM;
   }

   /* Create the Config Framework resource to handle this event subscription */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      CFGSTATUS eCfgStatus = cfgAddEvt( uEid, uUid );
      if ( CFGSTATUS_IS_FAIL( eCfgStatus ) )
      {
         CCTK_TRACEI1( "CctkFramework - internalEventCreate: "
            "Error creating event config resource (%d).",
            eCfgStatus );
         eStatus = eCCTKSTATUS_INTERR;
      }
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      TKMSUBINFO subInfo;

      subInfo.uGuid = uUid;
      subInfo.uSubTimeout = uExpTime;
      subInfo.eSubType = eSubtype;
      subInfo.pcSubAddr = pcToAddr;

      eStatus = ceNotify( uEid,
                          eCEEVT_SUB_ADD,
                          (static_cast<void *> ( &subInfo )) );

      if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
      {
         /* Second, setup the allocated subscription resource properly to be able to
         ** manage this particular call.
         */
         eStatus = ceNotify( uEid,
                             eCEEVT_SUB_USR_CREATE,
                             (static_cast<void *> ( &subInfo )) );
      }

      if ( !CCTKSTATUS_IS_SUCCESS( eStatus ) )
      {
         cfgRemoveEvt ( uEid );
      }
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_EVENTCREATE_SUCCESS :
                    eCCTKEVT_FN_EVENTCREATE_FAIL;
   tkmEvent ( uUid,
              uEid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );

   /* Clean up */
   BRCM_DELETE_ARRAY( pcToAddr );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::eventSubscribe
**
**  PURPOSE:    Send a SUBSCRIBE to the event service.
**
**  PARAMETERS:
**              uEid - Event id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::eventSubscribe( BOS_UINT32 uEid )
{
   CCTK_TRACED2( "CctkFramework(%p) - eventSubscribe(0x%lX)", this, uEid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uEid;

   mxt_result res = PostMessage( false, eTKM_MSGID_EVENTSUBSCRIBE, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - eventSubscribe: unable to asynchronously post "
                    "eTKM_MSGID_EVENTSUBSCRIBE message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalEventSubscribe
**
**  PURPOSE:    Handles the EventSubscribe event.
**
**              Send a SUBSCRIBE to the event service.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalEventSubscribe( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalEventSubscribe(%p)", this, pParms );

   BOS_UINT32 uEid;

   *pParms >> uEid;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uEid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_EVENTSUBSCRIBE ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalEventSubscribe: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uEid, eCEEVT_SUB_USR_SUBSCRIBE, NULL );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_EVENTSUBSCRIBE_SUCCESS :
                    eCCTKEVT_FN_EVENTSUBSCRIBE_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::eventUnsubscribe
**
**  PURPOSE:    Send a SUBSCRIBE with Expires equal to 0 to unscribe from
**              the event service.
**
**  PARAMETERS:
**              uEid - Event id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::eventUnsubscribe( BOS_UINT32 uEid )
{
   CCTK_TRACED2( "CctkFramework(%p) - eventUnsubscribe(0x%lX)", this, uEid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uEid;

   mxt_result res = PostMessage( false, eTKM_MSGID_EVENTUNSUBSCRIBE, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - eventSubscribe: unable to asynchronously post "
                    "eTKM_MSGID_EVENTUNSUBSCRIBE message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalEventUnsubscribe
**
**  PURPOSE:    Handles the EventUnsubscribe event.
**
**              Send a SUBSCRIBE with Expires equal to 0 to unscribe from
**              the event service.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalEventUnsubscribe( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalEventUnsubscribe(%p)", this, pParms );

   BOS_UINT32 uEid;

   *pParms >> uEid;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uEid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_EVENTUNSUBSCRIBE ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalEventUnsubscribe: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uEid, eCEEVT_SUB_USR_UNSUBSCRIBE, NULL );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_EVENTUNSUBSCRIBE_SUCCESS :
                    eCCTKEVT_FN_EVENTUNSUBSCRIBE_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::eventDelete
**
**  PURPOSE:    Unsubscribe and delete the (subscribed) event.
**
**  PARAMETERS:
**              uEid - Event id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::eventDelete( BOS_UINT32 uEid )
{
   CCTK_TRACED2( "CctkFramework(%p) - eventDelete(0x%lX)", this, uEid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uEid;

   mxt_result res = PostMessage( false, eTKM_MSGID_EVENTDELETE, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - eventDelete: unable to asynchronously post "
                    "eTKM_MSGID_EVENTDELETE message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalEventDelete
**
**  PURPOSE:    Handles the EventDelete event.
**
**              Unsubscribe and delete the (subscribed) event.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalEventDelete( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalEventDelete(%p)", this, pParms );

   BOS_UINT32 uEid;

   *pParms >> uEid;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uEid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_EVENTDELETE ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalEventDelete: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   /* Remove the Config Framework resource which handles this event subscription */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      CFGSTATUS eCfgStatus = cfgRemoveEvt( uEid );
      if ( CFGSTATUS_IS_FAIL( eCfgStatus ) )
      {
         CCTK_TRACEI1( "CctkFramework - internalEventDelete: "
            "Error removing event config resource (%d).",
            eCfgStatus );
         eStatus = eCCTKSTATUS_INTERR;
      }
   }

   /* Remove the Call Engine resource which handles this event */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* First we unsubscribe the user.
      */
      eStatus = ceNotify( uEid, eCEEVT_SUB_USR_UNSUBSCRIBE, NULL );

      if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
      {
         /* Then we terminate the support for this subscription.
         */
         eStatus = ceNotify( uEid, eCEEVT_SUB_REM, NULL );
      }
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_EVENTDELETE_SUCCESS :
                    eCCTKEVT_FN_EVENTDELETE_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callCreate
**
**  PURPOSE:    Creates a handle for call operation.
**
**  PARAMETERS:
**              uCid - Call id.
**              uUid - User id for the call.
**              pcTo - Call destination address (dial-string or URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callCreate( BOS_UINT32 uCid,
                                      BOS_UINT32 uUid,
                                      const BOS_UINT8 *pcTo )
{
   CCTK_TRACED4(
      "CctkFramework(%p) - callCreate(0x%lX, 0x%lX, %p)", this, uCid, uUid, pcTo );

   CMarshaler *pParms = CPool<CMarshaler>::New();
   BOS_UINT8 *pcToAddr = utilStrDup( pcTo );

   *pParms << uCid
           << uUid
           << pcToAddr;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLCREATE, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - callCreate: unable to asynchronously post "
                    "eTKM_MSGID_CALLCREATE message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      BRCM_DELETE_ARRAY( pcToAddr );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallCreate
**
**  PURPOSE:    Handles the CallCreate event.
**
**              Creates a handle for call operation.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallCreate( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalCallCreate(%p)", this, pParms );

   BOS_UINT32 uCid, uUid;
   BOS_UINT8 *pcToAddr;

   *pParms >> uCid
           >> uUid
           >> pcToAddr;

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLCREATE ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallCreate: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   CCTK_ASSERT( pcToAddr != NULL );
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) && pcToAddr == NULL )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallCreate: NULL address pointer." );
      eStatus = eCCTKSTATUS_BADPARM;
   }

   /* Create the Config Framework resource to handle this call */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      CFGSTATUS eCfgStatus = cfgAddCall( uCid, uUid );
      if ( CFGSTATUS_IS_FAIL( eCfgStatus ) )
      {
         CCTK_TRACEI1( "CctkFramework - internalCallCreate: "
            "Error creating call config resource (%d).",
            eCfgStatus );
         eStatus = eCCTKSTATUS_INTERR;
      }
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      /* First, create the call resource and properly associated it with
      ** the user.
      */
      eStatus = ceNotify( uCid, eCEEVT_CALL_ADD, (static_cast<void *> ( &uUid )) );

      if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
      {
         /* Second, setup the allocated call resource properly to be able to
         ** manage this particular call.
         */
         eStatus = ceNotify( uCid, eCEEVT_CALL_USR_CREATE,
                             (static_cast<void *> ( pcToAddr )) );
      }

      if ( !CCTKSTATUS_IS_SUCCESS( eStatus ) )
      {
         cfgRemoveCall ( uCid );
      }
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLCREATE_SUCCESS :
                    eCCTKEVT_FN_CALLCREATE_FAIL;
   tkmEvent ( uUid,
              uCid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );

   /* Clean up */
   BRCM_DELETE_ARRAY( pcToAddr );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callMake
**
**  PURPOSE:    Initiate a call (Send an INVITE).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callMake( BOS_UINT32 uCid )
{
   CCTK_TRACED2( "CctkFramework(%p) - callMake(0x%lX)", this, uCid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uCid;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLMAKE, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - callMake: unable to asynchronously post "
                    "eTKM_MSGID_CALLMAKE message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallMake
**
**  PURPOSE:    Handles the CallMake event.
**
**              Initiate a call (Send an INVITE).
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallMake( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalCallMake(%p)", this, pParms );

   BOS_UINT32 uCid;

   *pParms >> uCid;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLMAKE ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallMake: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uCid, eCEEVT_CALL_USR_MAKE, NULL );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLMAKE_SUCCESS :
                    eCCTKEVT_FN_CALLMAKE_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callAnswer
**
**  PURPOSE:    Answer the call (Send a 200 OK for the INVITE).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callAnswer( BOS_UINT32 uCid )
{
   CCTK_TRACED2( "CctkFramework(%p) - callAnswer(0x%lX)", this, uCid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uCid;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLANSWER, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - callAnswer: unable to asynchronously post "
                    "eTKM_MSGID_CALLANSWER message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallAnswer
**
**  PURPOSE:    Handles the CallAnswer event.
**
**              Answer the call (Send a 200 OK for the INVITE).
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallAnswer( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalCallAnswer(%p)", this, pParms );

   BOS_UINT32 uCid;

   *pParms >> uCid;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLANSWER ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallAnswer: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uCid, eCEEVT_CALL_USR_ANSWER, NULL );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLANSWER_SUCCESS :
                    eCCTKEVT_FN_CALLANSWER_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callForward
**
**  PURPOSE:    Forward the call (Send a 3xx response for the INVITE).
**
**  PARAMETERS:
**              uCid    - Call id.
**              pcFwdTo - Forward-to address (dial-string or URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callForward( BOS_UINT32 uCid, const BOS_UINT8 *pcFwdTo )
{
   CCTK_TRACED3( "CctkFramework(%p) - callForward(0x%lX, %p)",
                 this, uCid, pcFwdTo );

   CMarshaler *pParms = CPool<CMarshaler>::New();
   BOS_UINT8 *pcAddr = utilStrDup( pcFwdTo );

   *pParms << uCid
           << pcAddr;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLFORWARD, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - callForward: unable to asynchronously post "
                    "eTKM_MSGID_CALLFORWARD message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      BRCM_DELETE_ARRAY( pcAddr );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallForward
**
**  PURPOSE:    Handles the CallForward event.
**
**              Forward the call (Send a 3xx response for the INVITE).
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallForward( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalCallForward(%p)", this, pParms );

   BOS_UINT32 uCid;
   BOS_UINT8 *pcFwdTo;

   *pParms >> uCid
           >> pcFwdTo;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLFORWARD ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallForward: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uCid, eCEEVT_CALL_USR_FORWARD,
                          (static_cast<void *> ( pcFwdTo )) );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLFORWARD_SUCCESS :
                    eCCTKEVT_FN_CALLFORWARD_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );

   /* Clean up */
   BRCM_DELETE_ARRAY( pcFwdTo );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callHold
**
**  PURPOSE:    Hold the current call (Send a re-INVITE with SDP/a=sendonly).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callHold( BOS_UINT32 uCid )
{
   CCTK_TRACED2( "CctkFramework(%p) - callHold(0x%lX)", this, uCid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uCid;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLHOLD, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - callHold: unable to asynchronously post "
                    "eTKM_MSGID_CALLHOLD message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallHold
**
**  PURPOSE:    Handles the CallHold event.
**
**              Hold the current call (Send a re-INVITE with SDP/a=sendonly).
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallHold( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalCallHold(%p)", this, pParms );

   BOS_UINT32 uCid;

   *pParms >> uCid;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLHOLD ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallHold: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uCid, eCEEVT_CALL_USR_HOLD, NULL );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLHOLD_SUCCESS :
                    eCCTKEVT_FN_CALLHOLD_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callUnhold
**
**  PURPOSE:    Unhold the current call (Send a re-INVITE with SDP/a=sendonly).
**
**  PARAMETERS:
**              uCid - Call id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callUnhold( BOS_UINT32 uCid )
{
   CCTK_TRACED2( "CctkFramework(%p) - callUnhold(0x%lX)", this, uCid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uCid;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLUNHOLD, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - callUnhold: unable to asynchronously post "
                    "eTKM_MSGID_CALLUNHOLD message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallUnhold
**
**  PURPOSE:    Handles the CallUnhold event.
**
**              Unhold the current call (Send a re-INVITE with SDP/a=sendonly).
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallUnhold( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalCallUnhold(%p)", this, pParms );

   BOS_UINT32 uCid;

   *pParms >> uCid;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLUNHOLD ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallUnhold: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uCid, eCEEVT_HOLD_USR_UNHOLD, NULL );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLUNHOLD_SUCCESS :
                    eCCTKEVT_FN_CALLUNHOLD_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callTransfer
**
**  PURPOSE:    Attended call transfer (REFER with replaces).
**
**  PARAMETERS:
**              uCid    - Call id for the call to the transferer.
**              uXcid   - Call id for the call to transfer to.
**              bNoCallTermination - Boolean to indicate whether to terminate the calls
**                                   upon successful transfer. Needed for server-based 
**                                   transfer scenario
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callTransfer( BOS_UINT32 uCid, BOS_UINT32 uXcid, BOS_BOOL bNoCallTermination )
{
   CCTK_TRACED3( "CctkFramework(%p) - callTransfer(0x%lX, 0x%lX)", this, uCid, uXcid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uCid
           << uXcid
           << bNoCallTermination;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLTRANSFER, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - callTransfer: unable to asynchronously post "
                    "eTKM_MSGID_CALLTRANSFER message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallTransfer
**
**  PURPOSE:    Handles the CallTransfer event.
**
**              Attended call transfer (REFER with replaces).
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallTransfer( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalCallTransfer(%p)", this, pParms );

   BOS_UINT32 uCid, uXcid;
   BOS_BOOL bNoCallTermination;

   *pParms >> uCid
           >> uXcid
           >> bNoCallTermination;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLTRANSFER ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallTransfer: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      if (bNoCallTermination)
      {
         eStatus = ceNotify( uCid, eCEEVT_CALL_USR_XFER_NOTERM,
                          (static_cast<void *> ( &uXcid )) );
      }
      else
      {
         eStatus = ceNotify( uCid, eCEEVT_CALL_USR_XFER,
                          (static_cast<void *> ( &uXcid )) );
      }
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLTRANSFER_SUCCESS :
                    eCCTKEVT_FN_CALLTRANSFER_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callTransferBlind
**
**  PURPOSE:    Unattended call transfer (REFER no replaces).
**
**  PARAMETERS:
**              uCid   - Call id for the call to the transferer.
**              pcAddr - Transfer-to address (dial-string or URI).
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callTransferBlind( BOS_UINT32 uCid,
                                             const BOS_UINT8 *pcAddr )
{
   CCTK_TRACED3(
      "CctkFramework(%p) - callTransferBlind(0x%lX, %p)", this, uCid, pcAddr );

   CMarshaler *pParms = CPool<CMarshaler>::New();
   BOS_UINT8 *pcXferAddr = utilStrDup( pcAddr );

   *pParms << uCid
           << pcXferAddr;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLTRANSFERBLIND, pParms );

   if (MX_RIS_F( res ))
   {
      CCTK_TRACEI1( "CctkFramework - callTransferBlind: unable to asynchronously "
                    "post eTKM_MSGID_CALLTRANSFERBLIND message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      BRCM_DELETE_ARRAY( pcXferAddr );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallTransferBlind
**
**  PURPOSE:    Handles the CallTransferBlind event.
**
**              Unattended call transfer (REFER no replaces).
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallTransferBlind( CMarshaler *pParms )
{
   CCTK_TRACED2(
      "CctkFramework(%p) - internalCallTransferBlind(%p)", this, pParms );

   BOS_UINT32 uCid;
   BOS_UINT8* pcXferAddr;

   *pParms >> uCid
           >> pcXferAddr;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLTRANSFERBLIND ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallTransferBlind: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uCid,
                          eCEEVT_CALL_USR_BXFER,
                          (static_cast<void *> ( pcXferAddr )) );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLTRANSFERBLIND_SUCCESS :
                    eCCTKEVT_FN_CALLTRANSFERBLIND_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );

   /* Clean up */
   BRCM_DELETE_ARRAY( pcXferAddr );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callMediaNotify
**
**  PURPOSE:    Update remote party that a change in media has taken place.
**
**  PARAMETERS:
**              uCid - Call id.
**              eReason - Reason for the update.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callMediaNotify( BOS_UINT32 uCid, CCTKMUPDRSN eReason )
{
   CCTK_TRACED3(
      "CctkFramework(%p) - callMediaNotify(0x%lX, %d)", this, uCid, eReason );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uCid
           << eReason;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLMEDIANOTIFY, pParms );

   if (MX_RIS_F( res ))
   {
      CCTK_TRACEI1( "CctkFramework - callMediaNotify: unable to asynchronously "
                    "post eTKM_MSGID_CALLMEDIANOTIFY message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallMediaNotify
**
**  PURPOSE:    Handles the CallMediaNotify event.
**
**              Update remote party that a change in media has taken place.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallMediaNotify( CMarshaler *pParms )
{
   CCTK_TRACED2(
      "CctkFramework(%p) - internalCallMediaNotify(%p)", this, pParms );

   BOS_UINT32 uCid;
   CCTKMUPDRSN eReason;
   int sReason;

   *pParms >> uCid
           >> sReason;
   eReason = static_cast< CCTKMUPDRSN >( sReason );

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLMEDIANOTIFY ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallMediaNotify: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uCid, eCEEVT_CALL_USR_MEDIA, static_cast< void *> ( &eReason ) );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLMEDIANOTIFY_SUCCESS :
                    eCCTKEVT_FN_CALLMEDIANOTIFY_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callDelete
**
**  PURPOSE:    Delete allocated call resources.
**
**  PARAMETERS:
**              uCid - Call id.
**              eReason - Reason for the deletion
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callDelete( BOS_UINT32 uCid, CCTKCALLDELRSN eReason )
{
   CCTK_TRACED3( "CctkFramework(%p) - callDelete(0x%lX, %d)", this, uCid, eReason );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uCid
           << eReason;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLDELETE, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - callDelete: unable to asynchronously post "
                    "eTKM_MSGID_CALLDELETE message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallDelete
**
**  PURPOSE:    Handles the CallDelete event.
**
**              Delete allocated call resources.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallDelete( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalCallDelete(%p)", this, pParms );

   BOS_UINT32 uCid;
   CCTKCALLDELRSN eReason;
   int sReason;

   *pParms >> uCid
           >> sReason;
   eReason = static_cast< CCTKCALLDELRSN >( sReason );

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLDELETE ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallDelete: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   /* Remove the Call Engine resource which handles this call */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uCid, eCEEVT_CALL_USR_ENDED,
                          static_cast< void *> ( &eReason ) );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLDELETE_SUCCESS :
                    eCCTKEVT_FN_CALLDELETE_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callDtmfInfo
**
**  PURPOSE:    Pass a DTMF information event to the call resource.
**
**  PARAMETERS:
**              uCid - Call id.
**              uDtmf - The DTMF identifier.
**              uDur - The duration of the DTMF.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callDtmfInfo( BOS_UINT32 uCid,
                                        BOS_UINT32 uDtmf, BOS_UINT32 uDur )
{
   CCTK_TRACED4( "CctkFramework(%p) - callDtmfInfo(0x%lX, %d, %d)",
                 this, uCid, uDtmf, uDur );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uCid
           << uDtmf
           << uDur;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLDTMFINFO, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - callDtmfInfo: unable to asynchronously post "
                    "eTKM_MSGID_CALLDTMFINFO message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallDtmfInfo
**
**  PURPOSE:    Handles the CallDtmfInfo event.
**
**              Pass a DTMF information to the call resources.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallDtmfInfo( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalCallDtmfInfo(%p)", this, pParms );

   BOS_UINT32 uCid;
   BOS_UINT32 uDtmf;
   BOS_UINT32 uDur;

   *pParms >> uCid
           >> uDtmf
           >> uDur;
   
   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLDTMFINFO ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallDtmfInfo: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   /* Remove the Call Engine resource which handles this call */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      TKMDTMFINFO dtmfInfo;

      dtmfInfo.uDtmf = uDtmf;
      dtmfInfo.uDuration = uDur;

      eStatus = ceNotify( uCid, eCEEVT_CALL_USR_DTMF,
                          static_cast< void *> ( &dtmfInfo ) );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLDTMFINFO_SUCCESS :
                    eCCTKEVT_FN_CALLDTMFINFO_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callFlashInfo
**
**  PURPOSE:    Pass a Hook Flash information event to the call resource.
**
**  PARAMETERS:
**              uCid         - Call id.
**              pcHfMimeType - Content-Type of the INFO message
**              pcHfContent  - Content of the INFO message
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callFlashInfo( BOS_UINT32 uCid,
                                         const BOS_UINT8 *pcHfMimeType,
                                         const BOS_UINT8 *pcHfContent )
{
   CCTK_TRACED4( "CctkFramework(%p) - callFlashInfo(0x%lX, %s, %s)",
                 this, uCid, pcHfMimeType, pcHfContent );

   CMarshaler *pParms = CPool<CMarshaler>::New();
   BOS_UINT8 *pcHfMimeTypeData = utilStrDup( pcHfMimeType );
   BOS_UINT8 *pcHfContentData = utilStrDup( pcHfContent );

   *pParms << uCid << pcHfMimeTypeData << pcHfContentData;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLFLASHINFO, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - callFlashInfo: unable to asynchronously post "
                    "eTKM_MSGID_CALLFLASHINFO message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      BRCM_DELETE_ARRAY( pcHfMimeTypeData );
      BRCM_DELETE_ARRAY( pcHfContentData );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallFlashInfo
**
**  PURPOSE:    Handles the CallFlashInfo event.
**
**              Pass a Hook Flash information to the call resources.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallFlashInfo( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalCallFlashInfo(%p)", this, pParms );

   BOS_UINT32 uCid;
   BOS_UINT8 *pcHfMimeType;
   BOS_UINT8 *pcHfContent;

   *pParms >> uCid >> pcHfMimeType >> pcHfContent;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLFLASHINFO ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallFlashInfo: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) && ( pcHfMimeType == NULL || pcHfContent == NULL ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallFlashInfo: Bad parameters." );
      eStatus = eCCTKSTATUS_BADPARM;
   }

   /* Remove the Call Engine resource which handles this call */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      TKMFLASHINFO flashInfo;
      
      flashInfo.pcHfMimeType = pcHfMimeType;
      flashInfo.pcHfContent = pcHfContent;

      eStatus = ceNotify( uCid, eCEEVT_CALL_USR_FLASH,
                          static_cast< void *> ( &flashInfo ) );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLFLASHINFO_SUCCESS :
                    eCCTKEVT_FN_CALLFLASHINFO_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );

   /* Clean up */
   BRCM_DELETE_ARRAY( pcHfMimeType );
   BRCM_DELETE_ARRAY( pcHfContent );

}

/***********************************************************************************
**  FUNCTION:   CctkFramework::callPrivInfo
**
**  PURPOSE:    Pass on the privacy configuration for the given call.
**
**  PARAMETERS:
**              uCid - Call id.
**              pPrivCfg - Privacy configuration.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callPrivInfo( BOS_UINT32 uCid, const CCTKPRIVCFG *pPrivCfg )
{
   CCTK_TRACED2(
      "CctkFramework(%p) - callPrivInfo(0x%lX, %p)", this, pPrivCfg );
      
   /* Assert only because it should have been checked in cctkApi.cpp */
   CCTK_ASSERT( pPrivCfg != NULL );

   CCTKPRIVCFG *pPrivCfgDup = BRCM_NEW( CCTKPRIVCFG );
   if ( pPrivCfgDup == NULL )
   {
      CCTK_TRACEI0( "CctkFramework - callPrivInfo: Error copying configuration." );
      return eCCTKSTATUS_BADPARM;
   }
   
   /* Copy over the privacy info */
   *pPrivCfgDup = *( pPrivCfg );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uCid
           << pPrivCfgDup;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLPRIVINFO, pParms );

   if (MX_RIS_F( res ))
   {
      CCTK_TRACEI1( "CctkFramework - callPrivInfo: unable to asynchronously "
                    "post eTKM_MSGID_CALLPRIVINFO message (%d)",
                    res );

      /* Clean up */
      BRCM_DELETE( pPrivCfgDup );
      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallPrivInfo
**
**  PURPOSE:    Handles the CallPrivInfo event.
**
**              Updates the call's privacy setting info.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallPrivInfo( CMarshaler *pParms )
{
   CCTK_TRACED2(
      "CctkFramework(%p) - internalCallPrivInfo(%p)", this, pParms );

   BOS_UINT32 uCid;
   CCTKPRIVCFG *pPrivCfg;
   int sReason;

   *pParms >> uCid
           >> pPrivCfg;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLPRIVINFO ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallPrivInfo: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      if ( !ceSetPrivInfo( uCid, pPrivCfg ) )
      {
         eStatus = eCCTKSTATUS_INTERR;
      }
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLPRIVINFO_SUCCESS :
                    eCCTKEVT_FN_CALLPRIVINFO_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
   
   BRCM_DELETE( pPrivCfg );
}

#if CCTKCFG_REMOTE_USER_CONTROL
/***********************************************************************************
**  FUNCTION:   CctkFramework::callProxy
**
**  PURPOSE:    Proxy call referenced.
**
**  PARAMETERS:
**              uCid - Call id.
**              uPid - Call id of the proxy resource
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::callProxy( BOS_UINT32 uCid, BOS_UINT32 uPid )
{
   CCTK_TRACED3( "CctkFramework(%p) - callProxy(0x%lX, 0x%lX)", this, uCid, uPid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uCid
           << uPid;

   mxt_result res = PostMessage( false, eTKM_MSGID_CALLPROXY, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - callProxy: unable to asynchronously post "
                    "eTKM_MSGID_CALLPROXY message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalCallProxy
**
**  PURPOSE:    Handles the CallProxy event.
**
**              Proxy call referenced.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalCallProxy( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalCallProxy(%p)", this, pParms );

   BOS_UINT32 uCid, uPid;

   *pParms >> uCid
           >> uPid;

   BOS_UINT32 uUserGuid, uGuid;
   getId( uCid, uUserGuid, uGuid );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_CALLPROXY ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalCallProxy: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   /* Pass the information to Call Engine to apply the action. */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uCid, eCEEVT_CALL_USR_PROXY,
                          static_cast< void *> ( &uPid ) );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_CALLPROXY_SUCCESS :
                    eCCTKEVT_FN_CALLPROXY_FAIL;
   tkmEvent ( uUserGuid,
              uGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}
#endif /* CCTKCFG_REMOTE_USER_CONTROL */


#if CCTKCFG_MESSAGE_PASS_THROUGH
/***********************************************************************************
**  FUNCTION:   CctkFramework::passThruMsg
**
**  PURPOSE:    Pass thru a message from the application.
**
**  PARAMETERS:
**              uResGuid - Resource identifier.
**              uUsrGuid - User identifier associated with the resource
**              pcTo     - Where to send the message
**              pcBody   - Body to be happended to the message
**              uSize    - Size of the happended body
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::passThruMsg( BOS_UINT32 uResGuid,
                                       BOS_UINT32 uUsrGuid,
                                       const BOS_UINT8 *pcTo,
                                       const BOS_UINT8 *pcBody,
                                       const BOS_UINT32 uSize )
{
   CCTK_TRACED4( "CctkFramework(%p) - passThruMsg(0x%lX, 0x%lX, %s)",
                 this, uResGuid, uUsrGuid, pcTo );

   CMarshaler *pParms = CPool<CMarshaler>::New();
   BOS_UINT8 *pcToAddr = utilStrDup( pcTo );
   BOS_UINT8 *pcBodyData = utilDataDup( pcBody, uSize );

   *pParms << uUsrGuid
           << uResGuid
           << pcToAddr
           << pcBodyData
           << uSize;

   mxt_result res = PostMessage( false, eTKM_MSGID_MSGPASSTHRU, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - passThruMsg: unable to asynchronously post "
                    "eTKM_MSGID_MSGPASSTHRU message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      BRCM_DELETE_ARRAY( pcToAddr );
      BRCM_DELETE_ARRAY( pcBodyData );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::internalPassThruMsg
**
**  PURPOSE:    Handles the pass through message event.
**
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalPassThruMsg( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalPassThruMsg(%p)", this, pParms );

   BOS_UINT32 uUsrGuid;
   BOS_UINT32 uResGuid;
   BOS_UINT8 *pcToAddr;
   BOS_UINT8 *pcBody;
   BOS_UINT32 uSize;

   *pParms >> uUsrGuid
           >> uResGuid
           >> pcToAddr
           >> pcBody
           >> uSize;

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_MSGPASSTHRU ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalPassThruMsg: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   /* Validate the input parameters */
   if ( cfgGetType( uUsrGuid ) != eCFGTYPE_USR )
   {
      CCTK_TRACEI0( "CctkFramework - internalPassThruMsg: "
         "Invalid input parameter type for non user resource." );
      eStatus = eCCTKSTATUS_BADPARM;
   }

   /* Pass thru the message through the Call Engine resource */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      TKMMSGINFO msgInfo;

      msgInfo.uGuid = uResGuid;
      msgInfo.pcMsgAddr = pcToAddr;
      msgInfo.pcMsgBody = pcBody;
      msgInfo.uSize = uSize;

      eStatus = ceNotify( uUsrGuid,
                          eCEEVT_REG_USR_MSG,
                          static_cast< void *> ( &msgInfo ) );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_PUBLISH_SUCCESS :
                    eCCTKEVT_FN_PUBLISH_FAIL;
   tkmEvent ( uUsrGuid,
              uResGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );

   /* Clean up */
   BRCM_DELETE_ARRAY( pcToAddr );
   BRCM_DELETE_ARRAY( pcBody );
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::passThruRsp
**
**  PURPOSE:    Pass thru a response to a message received earlier on.
**
**  PARAMETERS:
**              uResGuid  - Resource identifier.
**              uFinalRsp - Final response identifier to be issued.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::passThruRsp( BOS_UINT32 uResGuid, BOS_UINT32 uFinalRsp )
{
   CCTK_TRACED3( "CctkFramework(%p) - passThruRsp(0x%lX, %u)",
                 this, uResGuid, uFinalRsp );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uResGuid
           << uFinalRsp;

   mxt_result res = PostMessage( false, eTKM_MSGID_MSGPASSTHRURSP, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - passThruRsp: unable to asynchronously post "
                    "eTKM_MSGID_MSGPASSTHRURSP message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::internalPassThruRsp
**
**  PURPOSE:    Handles the response to pass thru message event.
**
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalPassThruRsp( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalPassThruRsp(%p)", this, pParms );

   BOS_UINT32 uResGuid;
   BOS_UINT32 uFinalRsp;

   *pParms >> uResGuid
           >> uFinalRsp;

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_MSGPASSTHRURSP ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalPassThruRsp: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   /* Validate the input parameters */
   if ( cfgGetType( uResGuid ) != eCFGTYPE_EVT )
   {
      CCTK_TRACEI0( "CctkFramework - internalPassThruRsp: "
         "Invalid input parameter type for non user resource." );
      eStatus = eCCTKSTATUS_BADPARM;
   }

   /* Pass the message final response to the handler. */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uResGuid,
                          eCEEVT_NOT_USR_RSP,
                          static_cast< void *> ( &uFinalRsp ) );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_MSGPASSTHRURSP_SUCCESS :
                    eCCTKEVT_FN_MSGPASSTHRURSP_FAIL;
   tkmEvent ( cfgGetUsrId ( uResGuid ),
              uResGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */


/***********************************************************************************
**  FUNCTION:   CctkFramework::postMsg
**
**  PURPOSE:    Post a message to the CCTK message queue using the M5T framework.
**
**  PARAMETERS:
**              bWait   - True to post a message synchronously,
**                        i.e. this function is blocked until the message has
**                        been processed.
**              uId     - Object id (created on return).
**              pData   - Optional data.
**              msgCb   - Callback function to call when message is ready.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
CCTKSTATUS CctkFramework::postMsg( BOS_BOOL bWait,
                                   BOS_UINT32 uId,
                                   void *pData,
                                   TKMMSGCB msgCb )
{
   /* Post message onto the message queue */
   CMarshaler *pParms = CPool<CMarshaler>::New();
   bool bPostWait = ( ( bWait == BOS_TRUE ) ? true : false );

   *pParms << uId
           << pData
           << (void *) ( msgCb );

   mxt_result res = PostMessage( bPostWait, eMFW_MSGID_INTMSG, pParms );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI2( "CctkFramework - postMsg: Unable to post eMFW_MSGID_INTMSG "
                    "message (%d, %d)",
                    bWait,
                    res );

      CPool<CMarshaler>::Delete( pParms );
      pParms = NULL;
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalPostMsg
**
**  PURPOSE:    Handles the internal message event.
**
**  PARAMETERS:
**              pParms - parameter block.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void CctkFramework::internalPostMsg( CMarshaler *pParms )
{
   BOS_UINT32 uId;
   void *pData, *pMsgCb;
   TKMMSGCB msgCb;
   *pParms >> uId
           >> pData
           >> pMsgCb;
   msgCb = (TKMMSGCB) ( pMsgCb );

   /* Assert here only because it should have been checked earlier */
   CCTK_ASSERT( msgCb != NULL );

   /* Call back to the internal module */
   msgCb( uId, pData );
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::startTimer
**
**  PURPOSE:    Creates and schedules a timer with the M5T framework.
**
**  PARAMETERS:
**              uId        - Object id (created on return).
**              uMsec      - Timer in msec.
**              bRecurring - Indicates whether the timer is recurring.
**              pData      - Optional data
**              tmrCb      - Callback function to call when timer is up.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
CCTKSTATUS CctkFramework::startTimer( BOS_UINT32 uId,
                                      BOS_UINT32 uMsec,
                                      BOS_BOOL bRecurring,
                                      void *pData,
                                      TKMMSGCB tmrCb )
{
   CCTK_TRACED4( "CctkFramework(%p) - startTimer(0x%lX, %lu, %d)", this, uId, uMsec,
      bRecurring );

   ITimerService::EPeriodicity ePeriodicity;
   TKMTMRBLK *pTmrBlk;

   /* Allocate timer data block and cached the data */
   pTmrBlk = BRCM_NEW( TKMTMRBLK );
   pTmrBlk->pData = pData;
   pTmrBlk->tmrCb = tmrCb;
   pTmrBlk->bRecurring = bRecurring;

   /* Timer recurrance */
   if ( bRecurring )
   {
      ePeriodicity =
         ITimerService::ePERIODICITY_REAJUST_WITH_PREVIOUS_TIME_CYCLE_LOST;
   }
   else
   {
      ePeriodicity = ITimerService::ePERIODICITY_NOT_PERIODIC;
   }

   /* Start timer */
   CEventDriven::StartTimer( uId,
                             uMsec,
                             static_cast<mxt_opaque>( pTmrBlk ),
                             ePeriodicity );

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::stopTimer
**
**  PURPOSE:    Stops the scheduled timer.
**
**  PARAMETERS:
**              id - Object id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
CCTKSTATUS CctkFramework::stopTimer( BOS_UINT32 id )
{
   CCTK_TRACED2( "CctkFramework(%p) - stopTimer(0x%lX)", this, id );

   /*
   ** Stop timer with the specified id
   ** Note: Timer resource is cleaned up within the event callback
   */
   CEventDriven::StopTimer( id );

   return eCCTKSTATUS_SUCCESS;
}

#if CCTKCFG_MEMORY_MONITORING && CCTKCFG_DEBUG
/***********************************************************************************
**  FUNCTION:   CctkFramework::showMemStatus
**
**  PURPOSE:    Display the memory allocation status.
**
**  PARAMETERS:
**              bCctkOnly     - True to retrieve the amount of memory allocated
**                              by CCTK.
**              bTraceTable   - True to trace the entire memory allocation table.
**                              If bCctkOnly is enabled, only the memory blocks
**                              allocated by CCTK are traced.
**                              Not used when CCTK_TRACELVL_INFO is disabled.
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
CCTKSTATUS CctkFramework::showMemStatus( BOS_BOOL bCctkOnly, BOS_BOOL bTraceTable )
{
   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << bCctkOnly
           << bTraceTable;

   mxt_result res = PostMessage( false, eTKM_MSGID_SHOWMEMSTATUS, pParms );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - showMemStatus: unable to asynchronously post "
                    "eTKM_MSGID_SHOWMEMSTATUS message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalShowMemStatus
**
**  PURPOSE:    Handles the showMemStatus event.
**
**              Display the memory allocation status.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalShowMemStatus( CMarshaler *pParms )
{
   BOS_BOOL bCctkOnly, bTraceTable;

   *pParms >> bCctkOnly
           >> bTraceTable;

   utilMemAllocGet( bCctkOnly, bTraceTable );
}
#endif /* #if CCTKCFG_MEMORY_MONITORING && CCTKCFG_DEBUG */

#if CCTKCFG_APPLICATION_STATUS
/***********************************************************************************
**  FUNCTION:   CctkFramework::showStatus
**
**  PURPOSE:    Display the application dynamic resource allocation status.
**
**  PARAMETERS: None
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
CCTKSTATUS CctkFramework::showStatus( void )
{
   mxt_result res = PostMessage( false, eTKM_MSGID_SHOWSTATUS, NULL );
   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - showStatus: unable to asynchronously post "
                    "eTKM_MSGID_SHOWSTATUS message (%d)",
                    res );

      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalShowStatus
**
**  PURPOSE:    Handles the showStatus event.
**
**              Display the resource allocation status.
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Nothing.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalShowStatus( CMarshaler *pParms )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pParms );

   /* Invoke the status function call for each relevant module within CCTK.
   */
   ceShow();
}
#endif /* #if CCTKCFG_APPLICATION_STATUS */


/***********************************************************************************
**  FUNCTION:   CctkFramework::startUp
**
**  PURPOSE:    Start up CCTK, SCE, and SIP stack.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is synchronous. It MUST only be called within
**              the application task context.
***********************************************************************************/
CCTKSTATUS CctkFramework::startUp( void )
{
   CCTK_TRACED1( "CctkFramework(%p) - startUp()", this );

   /* Start up Config Framework */
   CFGSTATUS eCfgStatus = cfgStartUp( this );
   if ( CFGSTATUS_IS_FAIL( eCfgStatus ) )
   {
      CCTK_TRACEI1(
         "CctkFramework - startUp: Fail to start up Config Framework (%d).",
         eCfgStatus );
      return eCCTKSTATUS_INTERR;
   }

   /* Start up Call Engine.
   */
   if ( ceNotify( guCeInvalidGuid, eCEEVT_STARTUP, NULL ) != eCCTKSTATUS_SUCCESS )
   {
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::shutDown
**
**  PURPOSE:    Shutdown CCTK, SCE, and SIP stack.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is synchronous. It MUST only be called within
**              the application task context.
***********************************************************************************/
CCTKSTATUS CctkFramework::shutDown( void )
{
   CCTK_TRACED1( "CctkFramework(%p) - shutDown()", this );

   /* Shut down Call Engine (including resources).
   */
   CCTKSTATUS eCeStatus = ceShutDown();
   if ( CCTKSTATUS_IS_FAIL( eCeStatus ) )
   {
      CCTK_TRACEI1( "CctkFramework - shutDown: "
         "ERROR - Call Engine shutdown failed (%d).",
         eCeStatus );
   }

   /* Shut down Config Framework */
   CFGSTATUS eCfgStatus = cfgShutDown();
   if ( CFGSTATUS_IS_FAIL( eCfgStatus ) )
   {
      if ( eCfgStatus == eCFGSTATUS_RSCLEAK )
      {
         CCTK_TRACEI1( "CctkFramework - shutDown: "
            "ERROR - Config Framework shut down with resource leaks (%d).",
            eCfgStatus );
         return eCCTKSTATUS_RSCLEAK;
      }
      else
      {
         CCTK_TRACEI1(
            "CctkFramework - shutDown: Fail to shut down Config Framework (%d).",
            eCfgStatus );
         return eCCTKSTATUS_INTERR;
      }
   }

   return eCCTKSTATUS_SUCCESS;
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::kaSuccess
**
**  PURPOSE:    Keep alive successful exchange reporting.
**
**  PARAMETERS:
**              uUid    - User id.
**              eType   - The type of transport used for the keep alive.
**              puAddr  - The address of the keep alive remote.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::kaSuccess( BOS_UINT32 uUid,
                                     TKMTRANSPORT eType,
                                     BOS_UINT8 *puAddr )
{
   CCTK_TRACED4( "CctkFramework(%p) - kaSuccess(0x%lX, %d, %p)", this, uUid, eType,
                 puAddr );

   CMarshaler *pParms = CPool<CMarshaler>::New();
   BOS_UINT8 *puKaAddr = utilStrDup( puAddr );

   *pParms << uUid
           << eType
           << puKaAddr;

   mxt_result res = PostMessage( false, eTKM_MSGID_KA_SUCCESS, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - kaSuccess: unable to asynchronously post "
                    "eTKM_MSGID_KA_SUCCESS message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      BRCM_DELETE_ARRAY( puKaAddr );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::internalKaSuccess
**
**  PURPOSE:    Handles the eTKM_MSGID_KA_SUCCESS event.
**
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalKaSuccess( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalKaSuccess(%p)", this, pParms );

   BOS_UINT32 uUid;
   BOS_UINT32 uLastArrayIdx;
   TKMTRANSPORT eType;
   int sType;
   BOS_UINT8 *puAddr;
   CCTKKAINFO kaInfo;

   memset( &kaInfo, 0, sizeof( kaInfo ) );

   *pParms >> uUid
           >> sType
           >> puAddr;
   eType = static_cast< TKMTRANSPORT >( sType );

   kaInfo.bDatagram =
      ( (eType == eTKMTRANSPORT_STREAM) ? BOS_FALSE : BOS_TRUE );
   uLastArrayIdx = sizeof( kaInfo.uKaPeer ) - 1;
   strncpy( reinterpret_cast< char * >( kaInfo.uKaPeer ),
            reinterpret_cast< const char * >( puAddr ),
            uLastArrayIdx );
   kaInfo.uKaPeer[uLastArrayIdx] = 0;

   ceNotify( uUid,
             eCEEVT_REG_USR_KA_OK,
             static_cast<void *> (&kaInfo) );

   /* Clean up.
   */
   BRCM_DELETE_ARRAY( puAddr );
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::kaFailure
**
**  PURPOSE:    Keep alive failure reporting.
**
**  PARAMETERS:
**              uUid - User id.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::kaFailure( BOS_UINT32 uUid )
{
   CCTK_TRACED2( "CctkFramework(%p) - kaFailure(0x%lX)", this, uUid );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uUid;

   mxt_result res = PostMessage( false, eTKM_MSGID_KA_FAILURE, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - kaFailure: unable to asynchronously post "
                    "eTKM_MSGID_KA_FAILURE message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::internalKaFailure
**
**  PURPOSE:    Handles the eTKM_MSGID_KA_FAILURE event.
**
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalKaFailure( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalKaFailure(%p)", this, pParms );

   BOS_UINT32 uUid;

   *pParms >> uUid;

   ceNotify( uUid, eCEEVT_REG_USR_KA_FAIL, NULL );
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::infoPublish
**
**  PURPOSE:    Publishes information of interest for a given resource.
**
**  PARAMETERS:
**              uUsrGuid - user GUID associated with the resource of interest.
**              uResGuid - GUID of the resource of interest.
**              eType    - type of publication to advertize.
**              pTo      - Where to publish
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::infoPublish( BOS_UINT32 uUsrGuid,
                                       BOS_UINT32 uResGuid,
                                       CCTKPUBLISH eType,
                                       const BOS_UINT8 *pcTo )
{
   CCTK_TRACED4( "CctkFramework(%p) - infoPublish(0x%lX, 0x%lX, %d)",
                 this, uUsrGuid, uResGuid, eType );

   CMarshaler *pParms = CPool<CMarshaler>::New();
   BOS_UINT8 *pcToAddr = utilStrDup( pcTo );

   *pParms << uUsrGuid
           << uResGuid
           << eType
           << pcToAddr;

   mxt_result res = PostMessage( false, eTKM_MSGID_INFOPUBLISH, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - infoPublish: unable to asynchronously post "
                    "eTKM_MSGID_INFOPUBLISH message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      BRCM_DELETE_ARRAY( pcToAddr );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::internalInfoPublish
**
**  PURPOSE:    Handles the Publish (information publication) event.
**
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalInfoPublish( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalInfoPublish(%p)", this, pParms );

   BOS_UINT32 uUsrGuid;
   BOS_UINT32 uResGuid;
   CCTKPUBLISH eType;
   int sType;
   BOS_UINT8 *pcToAddr;

   *pParms >> uUsrGuid
           >> uResGuid
           >> sType
           >> pcToAddr;
   eType = static_cast< CCTKPUBLISH >( sType );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_INFOPUBLISH ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalInfoPublish: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   /* Validate the input parameters */
   if ( ( eType == eCCTKPUBLISH_CALLMETRICS ) &&
        ( cfgGetType( uUsrGuid ) != eCFGTYPE_USR ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalInfoPublish: "
         "Invalid input parameter type for non user resource." );
      eStatus = eCCTKSTATUS_BADPARM;
   }

   /* Publish the information through the Call Engine resource */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      switch ( eType )
      {
         case eCCTKPUBLISH_CALLMETRICS:
         {
            TKMPUBINFO pubInfo;

            pubInfo.uGuid = uResGuid;
            pubInfo.ePubType = eCCTKPUBLISH_CALLMETRICS;
            pubInfo.pcSubAddr = pcToAddr;

            eStatus = ceNotify( uUsrGuid,
                                eCEEVT_REG_USR_PUBLISH,
                                static_cast< void *> ( &pubInfo ) );
         }
         break;

         default:
         break;
      }
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_PUBLISH_SUCCESS :
                    eCCTKEVT_FN_PUBLISH_FAIL;
   tkmEvent ( uUsrGuid,
              uResGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );

   /* Clean up */
   BRCM_DELETE_ARRAY( pcToAddr );
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::serviceControl
**
**  PURPOSE:    Applies outside of CCTK direct control over a user resource
**              service.
**
**  PARAMETERS:
**              uUsrGuid - user GUID associated with the service of interest.
**              eType    - type of service to act on.
**              bEnable  - to enable or disable the service.
**
**
**  RETURNS:    Return status.
**
**  NOTES:      This function is asynchronous.
***********************************************************************************/
CCTKSTATUS CctkFramework::serviceControl( BOS_UINT32 uUsrGuid,
                                          CCTKSUBEVT eType,
                                          BOS_BOOL bEnable )
{
   CCTK_TRACED4( "CctkFramework(%p) - serviceControl(0x%lX, %d, %s)",
                 this, uUsrGuid, eType, (bEnable ? "enable" : "disable") );

   CMarshaler *pParms = CPool<CMarshaler>::New();

   *pParms << uUsrGuid
           << eType
           << bEnable;

   mxt_result res = PostMessage( false, eTKM_MSGID_SVCCTRL, pParms );

   if ( MX_RIS_F( res ) )
   {
      CCTK_TRACEI1( "CctkFramework - serviceControl: unable to asynchronously post "
                    "eTKM_MSGID_SVCCTRL message (%d)",
                    res );

      CPool<CMarshaler>::Delete( pParms );
      return eCCTKSTATUS_INTERR;
   }

   return eCCTKSTATUS_SUCCESS;
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::internalServiceControl
**
**  PURPOSE:    Handles the Service Control event.
**
**
**  PARAMETERS:
**              pParams - parameter block.
**
**
**  RETURNS:    Return status.
**
**  NOTES:
***********************************************************************************/
void CctkFramework::internalServiceControl( CMarshaler *pParms )
{
   CCTK_TRACED2( "CctkFramework(%p) - internalServiceControl(%p)", this, pParms );

   BOS_UINT32 uUsrGuid;
   CCTKSUBEVT eType;
   int sType;
   BOS_BOOL bEnable;

   *pParms >> uUsrGuid
           >> sType
           >> bEnable;
   eType = static_cast< CCTKSUBEVT >( sType );

   CCTKSTATUS eStatus = eCCTKSTATUS_SUCCESS;

   if ( !cfgIsFuncAllowed( eTKM_MSGID_SVCCTRL ) )
   {
      CCTK_TRACEI0( "CctkFramework - internalServiceControl: "
         "Function not allowed to be called in current state." );
      eStatus = eCCTKSTATUS_BADSTATE;
   }

   /* Pass the service control event to Call Engine that will take care of
   ** applying the needed control.
   */
   if ( CCTKSTATUS_IS_SUCCESS( eStatus ) )
   {
      eStatus = ceNotify( uUsrGuid,
                          bEnable ?
                              eCEEVT_REG_USR_SVC_ENA :
                              eCEEVT_REG_USR_SVC_DIS,
                          static_cast< void *> ( &eType ) );
   }

   /* Report event to the app */
   CCTKEVT eFnEvt = CCTKSTATUS_IS_SUCCESS( eStatus ) ?
                    eCCTKEVT_FN_SVCCTRL_SUCCESS :
                    eCCTKEVT_FN_SVCCTRL_FAIL;
   tkmEvent ( uUsrGuid,
              guCeInvalidGuid,
              eFnEvt,
              reinterpret_cast< void* >( &eStatus ) );
}


/***********************************************************************************
**  FUNCTION:   CctkFramework::EvMessageServiceMgrAwaken
**
**  PURPOSE:    Notifies the manager that a new message must be processed.
**
**  PARAMETERS:
**              bWaitingCompletion  - True if the message is being processed
**                                    synchronously, false if processed
**                                    asynchronously.
**              uMessage            - The identifier of the message that must be
**                                    posted.
**              pParameter          - An optional CMarshaler parameter.
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void CctkFramework::EvMessageServiceMgrAwaken( bool bWaitingCompletion,
                                               unsigned int uMessage,
                                               CMarshaler *pParameter )
{
   TKM_MSGID eMsg = static_cast< TKM_MSGID >( uMessage );
   switch( eMsg )
   {
   case eTKM_MSGID_RESET:
      {
         internalReset( pParameter );
      }
      break;
   case eTKM_MSGID_CONFIG:
      {
         internalConfig( pParameter );
      }
      break;
   case eTKM_MSGID_SETPARM:
      {
         internalSetParm( pParameter );
      }
      break;
   case eTKM_MSGID_GETPARM: /* Synchronous */
      {
         internalGetParm( pParameter );
      }
      break;
   case eTKM_MSGID_USERADD:
      {
         internalUserAdd( pParameter );
      }
      break;
   case eTKM_MSGID_USERREGISTER:
      {
         internalUserRegister( pParameter );
      }
      break;
   case eTKM_MSGID_USERUNREGISTER:
      {
         internalUserUnregister( pParameter );
      }
      break;
   case eTKM_MSGID_USERREMOVE:
      {
         internalUserRemove( pParameter );
      }
      break;
   case eTKM_MSGID_EVENTCREATE:
      {
         internalEventCreate( pParameter );
      }
      break;
   case eTKM_MSGID_EVENTSUBSCRIBE:
      {
         internalEventSubscribe( pParameter );
      }
      break;
   case eTKM_MSGID_EVENTUNSUBSCRIBE:
      {
         internalEventUnsubscribe( pParameter );
      }
      break;
   case eTKM_MSGID_EVENTDELETE:
      {
         internalEventDelete( pParameter );
      }
      break;
   case eTKM_MSGID_CALLCREATE:
      {
         internalCallCreate( pParameter );
      }
      break;
   case eTKM_MSGID_CALLMAKE:
      {
         internalCallMake( pParameter );
      }
      break;
   case eTKM_MSGID_CALLANSWER:
      {
         internalCallAnswer( pParameter );
      }
      break;
   case eTKM_MSGID_CALLFORWARD:
      {
         internalCallForward( pParameter );
      }
      break;
   case eTKM_MSGID_CALLHOLD:
      {
         internalCallHold( pParameter );
      }
      break;
   case eTKM_MSGID_CALLUNHOLD:
      {
         internalCallUnhold( pParameter );
      }
      break;
   case eTKM_MSGID_CALLTRANSFER:
      {
         internalCallTransfer( pParameter );
      }
      break;
   case eTKM_MSGID_CALLTRANSFERBLIND:
      {
         internalCallTransferBlind( pParameter );
      }
      break;
   case eTKM_MSGID_CALLMEDIANOTIFY:
      {
         internalCallMediaNotify( pParameter );
      }
      break;
   case eTKM_MSGID_CALLDELETE:
      {
         internalCallDelete( pParameter );
      }
      break;
   case eTKM_MSGID_CALLDTMFINFO:
      {
         internalCallDtmfInfo( pParameter );
      }
      break;
   case eTKM_MSGID_CALLFLASHINFO:
      {
         internalCallFlashInfo( pParameter );
      }
      break;
   case eTKM_MSGID_CALLPRIVINFO:
      {
          internalCallPrivInfo( pParameter );
      }
      break;
#if CCTKCFG_REMOTE_USER_CONTROL
   case eTKM_MSGID_CALLPROXY:
      {
         internalCallProxy( pParameter );
      }
      break;
#endif /* CCTKCFG_REMOTE_USER_CONTROL */
#if CCTKCFG_MESSAGE_PASS_THROUGH
   case eTKM_MSGID_MSGPASSTHRU:
      {
         internalPassThruMsg( pParameter );
      }
      break;

   case eTKM_MSGID_MSGPASSTHRURSP:
      {
         internalPassThruRsp( pParameter );
      }
      break;
#endif /* CCTKCFG_MESSAGE_PASS_THROUGH */
#if CCTKCFG_MEMORY_MONITORING && CCTKCFG_DEBUG
   case eTKM_MSGID_SHOWMEMSTATUS:
      {
         internalShowMemStatus( pParameter );
      }
      break;
#endif
#if CCTKCFG_APPLICATION_STATUS
   case eTKM_MSGID_SHOWSTATUS:
      {
         internalShowStatus( pParameter );
      }
      break;
#endif
   case eMFW_MSGID_INTMSG:
      {
         internalPostMsg( pParameter );
      }
      break;
   case eTKM_MSGID_KA_SUCCESS:
      {
         internalKaSuccess( pParameter );
      }
      break;
   case eTKM_MSGID_KA_FAILURE:
      {
         internalKaFailure( pParameter );
      }
      break;
   case eTKM_MSGID_INFOPUBLISH:
      {
         internalInfoPublish( pParameter );
      }
      break;
   case eTKM_MSGID_SVCCTRL:
      {
         internalServiceControl( pParameter );
      }
      break;
   default:
      {
         CEventDriven::EvMessageServiceMgrAwaken( bWaitingCompletion,
                                                  uMessage,
                                                  pParameter );
      }
      break;
   }
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::EvTimerServiceMgrAwaken
**
**  PURPOSE:    Notifies the manager that a new timer elapsed or has been stopped.
**
**  PARAMETERS:
**              bStopped   - True if the timer has been explicitly stopped, false
**                           if it normally elapsed. For an exponential timer,
**                           true is also sent if the ceiling has been reached
**                           and bStopAtCeiling is true.
**              uTimer     - The identifier of the timer.
**              opq        - An optional opaque supplied by the caller.
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void CctkFramework::EvTimerServiceMgrAwaken( bool bStopped,
                                             unsigned int uTimer,
                                             mxt_opaque opq )
{
   CCTK_TRACED4( "CctkFramework(%p) - EvTimerServiceMgrAwaken(%d, %u, %p)",
                  this,
                  bStopped,
                  uTimer,
                  opq );

   TKMTMRBLK *pTmrBlk = static_cast< TKMTMRBLK* >( opq );

   if ( !bStopped )
   {
      ( pTmrBlk->tmrCb )( static_cast< BOS_UINT32 >( uTimer ), pTmrBlk->pData );
   }

   /* Timer either stopped or not expected to recur, clean up allocated resources
   ** for this timer.
   */
   if ( bStopped || (!bStopped && !pTmrBlk->bRecurring) )
   {
      BRCM_DELETE( pTmrBlk );
      pTmrBlk = NULL;
   }
}

/***********************************************************************************
**  FUNCTION:   CctkFramework::getId
**
**  PURPOSE:    Retrieve the associated user and call IDs of the input ID.
**
**  PARAMETERS:
**              uId        - Input ID.
**              ruUsrId    - Associated user ID.
**              ruCallId   - Associated call ID.
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
void CctkFramework::getId( BOS_UINT32 uId,
                           BOS_UINT32 &ruUsrId,
                           BOS_UINT32 &ruCallId )
{
   ruUsrId = cfgGetUsrId( uId );
   ruCallId = ( uId == ruUsrId ) ? guCeInvalidGuid : uId;
}

MX_NAMESPACE_END( MXD_GNS )
