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
**      This file implements the Memory Management main.
**
***********************************************************************************/

/* ---- Include Files ----------------------------------------------------------- */
#include <utilApi.h>
#include <utilMem.h>

#if CCTKCFG_MEMORY_MONITORING

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
/* Margin size for preventing multiple memory allocation events reported when
** memory is allocated/de-allocated around the threshold.
*/
static const BOS_UINT32 UTILMEM_THRESHOLD_MARGIN = 30000;

/* ---- Private Variables ------------------------------------------------------- */
#if CCTKCFG_DEBUG
/* Memory Management class instance */
static UtilMem  *gpUtilMem = NULL;
#endif

/* True to indicate memory allocation is overrun */
static BOS_BOOL gbMemOverrun = BOS_FALSE;

/* ---- Private Maps------------------------------------------------------------- */

/* ---- Private Function Prototypes --------------------------------------------- */
/* Stack observer callback functions */
static void eventObserver( void *pContext,
                           MX_NS EEventNotifier eEvent,
                           va_list args );
static void eventObserverCb( BOS_UINT32 uId, void *pData );

/* ---- Functions --------------------------------------------------------------- */

/* ---- "C" API (START OF) ------------------------------------------------------ */
/***********************************************************************************
**  FUNCTION:   utilMemInit
**
**  PURPOSE:    External API to initialize (and launch) Memory Management module.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    Return status.
**
**  NOTES:      This API must be invoked prior to any further Memory Management
**              work is possible.
***********************************************************************************/
UTILSTATUS utilMemInit( void )
{
   CCTK_TRACED0( "util - utilMemInit()" );

#if CCTKCFG_DEBUG
   if ( gpUtilMem != NULL )
   {
      CCTK_TRACEI0(
         "util - utilMemInit: Memory management module already initialized." );
      return eUTILSTATUS_BADSTATE;
   }

   gpUtilMem = BRCM_NEW( UtilMem );
   CCTK_ASSERT( gpUtilMem != NULL );
   if ( gpUtilMem == NULL )
   {
      CCTK_TRACEI0( "util - utilMemInit: Error initializing Memory Management." );
      return eUTILSTATUS_NORSC;
   }
#endif

   /* Private variable initialization */
   gbMemOverrun = BOS_FALSE;

   return eUTILSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   utilMemDeInit
**
**  PURPOSE:    External API to de-initialize the Memory Management module.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Return status
**
**  NOTES:      After this API has been invoked, there is no further processing
**              possible from the Memory Management module.
***********************************************************************************/
UTILSTATUS utilMemDeInit( void )
{
   CCTK_TRACED0( "util - utilMemDeInit()" );

#if CCTKCFG_DEBUG
   if ( gpUtilMem == NULL )
   {
      CCTK_TRACEI0( "util - utilMemDeInit: "
         "Memory Management module has not been initialized." );
      return eUTILSTATUS_BADSTATE;
   }

   BRCM_DELETE( gpUtilMem );
   gpUtilMem = NULL;
#endif

   return eUTILSTATUS_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   utilMemEvtObsGet
**
**  PURPOSE:    External API to retrieve the stack event observer function pointer.
**
**  PARAMETERS:
**              None.
**
**  RETURNS:    Function pointer of the stack event observer.
**
**  NOTES:      None.
***********************************************************************************/
void *utilMemEvtObsGet( void )
{
   return (void *)&eventObserver;
}

#if CCTKCFG_DEBUG
/***********************************************************************************
**  FUNCTION:   utilMemAllocGet
**
**  PURPOSE:    External API to retrieve the total amount of memory allocated.
**
**  PARAMETERS:
**              bCctkOnly     - True to retrieve the amount of memory allocated
**                              by CCTK.
**              bTraceTable   - True to trace the entire memory allocation table.
**                              If bCctkOnly is enabled, only the memory blocks
**                              allocated by CCTK are traced.
**                              Not used when CCTK_TRACELVL_INFO is disabled.
**
**  RETURNS:    The total amount of memory allocated.
**
**  NOTES:      None.
***********************************************************************************/
BOS_UINT32 utilMemAllocGet( BOS_BOOL bCctkOnly, BOS_BOOL bTraceTable )
{
   CCTK_ASSERT( gpUtilMem != NULL );
   if ( gpUtilMem == NULL )
   {
      CCTK_TRACEI0(
         "util - utilMemAllocGet: Utility module has not been initialized." );
      return 0;
   }

   UtilMem::UTILMEM_TRACEMODE eMode = UtilMem::eUTILMEM_TRACEMODE_NONE;
   if ( bTraceTable )
   {
      eMode = bCctkOnly ?
              UtilMem::eUTILMEM_TRACEMODE_CCTK :
              UtilMem::eUTILMEM_TRACEMODE_ALL;
   }

   UtilMem::UTILMEMSTAT memStat;
   gpUtilMem->getMemStatistics( eMode, memStat );

   if ( bCctkOnly )
   {
      CCTK_TRACEI2(
         "Memory allocated by CCTK: %lu bytes, %lu blks",
         memStat.uMemAllocCctk,
         memStat.uNumAllocCctk );
   }
   else
   {
      CCTK_TRACEI4(
         "Total memory allocated: %lu bytes, %lu blks, by CCTK: %lu bytes, %lu blks",
         memStat.uMemAllocAll,
         memStat.uNumAllocAll,
         memStat.uMemAllocCctk,
         memStat.uNumAllocCctk );
   }

   return bCctkOnly ? memStat.uMemAllocCctk : memStat.uMemAllocAll;
}
#endif /* #if CCTKCFG_DEBUG */

/* ---- "C" API (END OF) -------------------------------------------------------- */

#if CCTKCFG_DEBUG
/***********************************************************************************
**  FUNCTION:   UtilMem::UtilMem
**
**  PURPOSE:    Class constructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
UtilMem::UtilMem()
   : meMemTableTrace( eUTILMEM_TRACEMODE_NONE )
{
   memset( &mMemStatistics, 0, sizeof( mMemStatistics ) );
}

/***********************************************************************************
**  FUNCTION:   UtilMem::~UtilMem
**
**  PURPOSE:    Class desctructor.
**
**  PARAMETERS:
**              None
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None
***********************************************************************************/
UtilMem::~UtilMem()
{

}

/***********************************************************************************
**  FUNCTION:   UtilMem::getMemStatistics
**
**  PURPOSE:    Retrieve the memory allocation statistics.
**
**  PARAMETERS:
**              eTraceMode - Memory allocation table tracing mode.
**                           Not used when CCTKTRACELVL_INFO is disabled.
**              rMemStat   - Memory allocation statistics to be output.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      This function is synchronous.
***********************************************************************************/
void UtilMem::getMemStatistics( UtilMem::UTILMEM_TRACEMODE eTraceMode,
                                UtilMem::UTILMEMSTAT &rMemStat )
{
   /* Initialize statistics before enumeration */
   meMemTableTrace = eTraceMode;
   memset( &mMemStatistics, 0, sizeof( mMemStatistics ) );

   /* Enumerate through all allocated memory blocks.
   ** Memory statistics will be updated after this function returns.
   */
   MX_NS CMemoryAllocator::EnumMemoryBlocks( this );

   rMemStat = mMemStatistics;
}

/***********************************************************************************
**  FUNCTION:   UtilMem::Accumulate
**
**  PURPOSE:    Insert private header to outgoing SIP packet.
**
**  PARAMETERS:
**              uGuid   - GUID.
**              rPkt    - Outgoing SIP packet.
**
**
**  RETURNS:    Nothing
**
**  NOTES:      None.
***********************************************************************************/
void UtilMem::Accumulate( MX_NS CMemoryAllocator::CMemoryBlock *pMemoryBlock )
{
   size_t memSize = pMemoryBlock->GetSize();
   bool bCctkAlloc = pMemoryBlock->IsBrcmAllocated();

   if ( meMemTableTrace == eUTILMEM_TRACEMODE_ALL ||
        meMemTableTrace == eUTILMEM_TRACEMODE_CCTK && bCctkAlloc )
   {
      CCTK_TRACEI4(
         "file: %s, line: %u, size: %u, addr: %p",
         pMemoryBlock->GetFilename(),
         pMemoryBlock->GetLineNumber(),
         memSize,
         pMemoryBlock->GetPointer() );
   }

   if ( bCctkAlloc )
   {
      mMemStatistics.uMemAllocCctk += memSize;
      mMemStatistics.uNumAllocCctk++;
   }

   mMemStatistics.uMemAllocAll += memSize;
   mMemStatistics.uNumAllocAll++;
}
#endif /* CCTKCFG_DEBUG */

/* ---- Private Functions ------------------------------------------------------- */

/***********************************************************************************
**  FUNCTION:   eventObserver
**
**  PURPOSE:    Event observer for receiving stack events.
**
**  PARAMETERS:
**              pContext   - The context associated with the observer.
**              eEvent     - The event to observe.
**              args       - Arguments passed to the observer,
**                           depending on the event being observed.
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void eventObserver( void *pContext,
                           MX_NS EEventNotifier eEvent,
                           va_list args )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pContext );

   switch ( eEvent )
   {
      case MX_NS eEN_CMEMORYALLOCATOR_TRACKING:
      {
         /* Memory tracking event - triggered when memory usage changes */
         uint32_t uMemAllocated = va_arg( args, uint32_t );
         CCTKSTATUS eStatus =
            tkmMsgPost( BOS_FALSE, uMemAllocated, NULL, &eventObserverCb );
         if ( CCTKSTATUS_IS_FAIL( eStatus ) )
         {
            CCTK_TRACEI1( "utilMem - eventObserver: "
               "Unable to asynchronously post message (%d)",
               eStatus );
         }
      }
      break;
      default:
      {
         /* Ignore other stack events */
      }
      break;
   }
}

/***********************************************************************************
**  FUNCTION:   eventObserverCb
**
**  PURPOSE:    Callback function that the Main Framework calls when the processing
**              of stack event observer is ready.
**
**  PARAMETERS:
**              uId     - Message ID.  Used to store total memory allocated here.
**              pData   - Optional data.  Should be NULL here.
**
**  RETURNS:    Nothing.
**
**  NOTES:      None.
***********************************************************************************/
static void eventObserverCb( BOS_UINT32 uId, void *pData )
{
   MX_REMOVE_UNUSED_FUNCTION_PARAM_WARNING( pData );

   BOS_UINT32 uMemAllocated = uId;
   const CCTKMEMPROTCFG *pMemCfg = &( CFG_SYS->xtl.sec.dos.memProt );

   CCTK_ASSERT( pMemCfg->uThreshold > 0 );
   if ( pMemCfg->uThreshold > 0 )
   {
      CCTKEVT eEvt = eCCTKEVT_MAX;
      CCTKSTATUS eStatus;

      if ( !gbMemOverrun && uMemAllocated > pMemCfg->uThreshold )
      {
         /* Memory allocated above the threshold */
         CCTK_TRACED0( "utilMem - eventObserverCb: Exceed memory threshold." );
         gbMemOverrun = BOS_TRUE;
         eEvt = eCCTKEVT_MEM_OVERTHRESHOLD;

         /* Check if the app wishes to deny all incoming requests */
         if ( pMemCfg->ePolicy != eCCTKMEMPROT_NOTIFY )
         {
            eStatus = ceNotify( guCeInvalidGuid, eCEEVT_SUSPEND_NEW, NULL );
            if ( CCTKSTATUS_IS_FAIL( eStatus ) )
            {
               CCTK_TRACEI1( "utilMem - eventObserverCb: "
                  "Error suspending new incoming requests (%d)",
                  eStatus );
            }
         }
      }
      else if ( gbMemOverrun &&
                uMemAllocated <=
                  MX_MAX( pMemCfg->uThreshold - UTILMEM_THRESHOLD_MARGIN, 0 ) )
      {
         /* Memory allocated below the threshold */
         CCTK_TRACED0( "utilMem - eventObserverCb: Below memory threshold." );
         gbMemOverrun = BOS_FALSE;
         eEvt = eCCTKEVT_MEM_UNDERTHRESHOLD;

         /* Check if the app was denying incoming requests */
         if ( pMemCfg->ePolicy != eCCTKMEMPROT_NOTIFY )
         {
            eStatus = ceNotify( guCeInvalidGuid, eCEEVT_RESUME_ALL, NULL );
            if ( CCTKSTATUS_IS_FAIL( eStatus ) )
            {
               CCTK_TRACEI1( "utilMem - eventObserverCb: "
                  "Error resuming incoming requests (%d)",
                  eStatus );
            }
         }
      }

      /* Check if the app wishes to get notified on the memory overrun/underrun */
      if ( eEvt != eCCTKEVT_MAX && pMemCfg->ePolicy != eCCTKMEMPROT_DENY_REQ )
      {
         tkmEvent( guCeInvalidGuid,
                   guCeInvalidGuid,
                   eEvt,
                   reinterpret_cast< void* >( uMemAllocated ) );
      }
   }
}

#endif /* CCTKCFG_MEMORY_MONITORING */
