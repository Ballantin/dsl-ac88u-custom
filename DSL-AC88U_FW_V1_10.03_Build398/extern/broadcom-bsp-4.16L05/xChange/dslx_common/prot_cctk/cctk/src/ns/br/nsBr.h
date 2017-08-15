/**********************************************************************************
** Copyright (c) 2009-2012 Broadcom
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
**      This file defines the Backup and Recovery module.
**
***********************************************************************************/
#ifndef __NS_BR__H__INCLUDED__
#define __NS_BR__H__INCLUDED__


/* ---- Include Files ----------------------------------------------------------- */
#include <cctkInc.h>

/* ---- Constants and Types ----------------------------------------------------- */

#define CCTKCFG_MAX_BR_USR  2 * ((CCTKCFG_MAX_USR) + (CCTKCFG_MAX_VIRTUAL_USR))

/* Define the maximum of mapped resource we may need to keep track at any one
** time.  There cannot be more mapped resources than there is CCTK resources being
** managed.
*/
#define NS_BR_MAX_MAP             \
    ( CCTKCFG_MAX_BR_USR + CCTKCFG_MAX_CALL + CCTKCFG_MAX_EVT + CCTKCFG_MAX_NOT )

/* Define the mapping structure to keep track of the mapped resources.
*/
typedef struct
{
   BOS_UINT32 uGuid;
   mxt_opaque opq;

} NSBRMAPBLK;

/* Define the backup and recovery item state.
*/
typedef enum
{
   eNSBRSTATE_IDLE,    /* Initial state, target not used or used but no longer
                       ** active. */
   eNSBRSTATE_ACTIVE,  /* Currently active target, a single element from the list
                       ** can be in this state. */
   eNSBRSTATE_FAILED,  /* Target has been used and a failure against it was
                       ** dectected. It cannot be used any longer until the 
                       ** backup and recovery module decides it is fine to use. */
} NSBRSTATE;

/* Define the item information associated with a specific target destination for
** the backup and recovery feature.
**
** Note: we do not keep the 'transport' information type for now, so it does not
**       come into account during the backup and recovery decision at this time.
*/
typedef struct
{
   NSBRSTATE eState;
   BOS_BOOL bPrimary;
   CCTKHOSTPORT host;

} NSBRITEM;

/* Define the backup and recovery target selection restrictions allowed.
*/
typedef enum
{
   eNSBRRESTRICTIONTYPE_NONE,          /* Initial state, there are no restrictions to 
                                       ** be applied. */
   eNSBRRESTRICTIONTYPE_PRIMONLY,      /* Restricted to choose the primary when selecting
                                       ** destination target. */
   eNSBRRESTRICTIONTYPE_ADDRSPECIFIED, /* Restricted to chose the address specfied in  
                                       ** restrictSockAddr when selecting the destination 
                                       ** target. */
} NSBRRESTRICTIONTYPE;

/* Define the restriction information that needs to be considered when choosing a
** target destination.
*/
typedef struct
{
   NSBRRESTRICTIONTYPE  eType;
   MXD_GNS::CSocketAddr restrictSockAddr;
   
} NSBRRESTRICT;

/* Define the information structure which contains the actual backup and recovery
** data set which is being tracked for each user resource.
*/
typedef struct
{
   BOS_UINT32 uGuid;
   NSBRSTATUS eStatus;
   BOS_UINT32 uBackOffTmr;
   BOS_UINT32 uConsecFail;
   BOS_BOOL   bPrimOnly;
   NSBRRESTRICT nsBrRestrict;    
   MXD_GNS::CSocketAddr prefPrimSockAddr;

   BOS_UINT32 uItemCnt;
   NSBRITEM   nsBr[ CCTKCFG_MAX_BACKUP_RECOVERY_ITEMS ];

} NSBRBLK;

/* ---- Variable Externs -------------------------------------------------------- */

/* ---- Class Definition -------------------------------------------------------- */
class NsDlm;

class NsBr
{
public:

   friend class NsDlm;

   /* Constructor */
   NsBr();

   /* Destructor */
   virtual     ~NsBr();

   void        nsBrClean ( BOS_UINT32 uGuid );

   BOS_BOOL    nsBrFreeResBlk( void );

   BOS_BOOL    nsBrExists ( BOS_UINT32 uGuid );

   void        nsBrSelectDest ( BOS_UINT32 uUsrGuid,
                                BOS_UINT32 uGuid,
                                const MXD_GNS::CHostPort& rFqdn,
                                MXD_GNS::CList<MXD_GNS::SNaptrRecord>
                                                            &rlstNaptrRecord );

   BOS_BOOL    nsBrRegister ( BOS_UINT32 uGuid );
   void        nsBrDeRegister ( BOS_UINT32 uGuid );

   NSBRSTATUS  nsBrStatus( BOS_UINT32 uGuid );
   NSBRSTATUS  nsBrBackupOnFail( BOS_UINT32 uGuid, BOS_BOOL bBkoffTmr );
   NSBRSTATUS  nsBrRestart( BOS_UINT32 uGuid );
   BOS_BOOL    nsBrApplyRestriction( BOS_UINT32 uGuid, BOS_BOOL bRestrict,
                                       NSBRRESTRICTIONTYPE eRstrictToApply, 
                                       MXD_GNS::CSocketAddr *pRstrictAddr = NULL );
   NSBRSTATUS  nsBrCopyTgt( BOS_UINT32 uGuidFrom, BOS_UINT32 uGuidTo );
   BOS_UINT32  nsBrBackOff( BOS_UINT32 uGuid );
   BOS_BOOL    nsBrResetBackOff( BOS_UINT32 uGuid );
   BOS_BOOL    nsBrFailedPrimary( BOS_UINT32 uGuid );
   BOS_BOOL    nsBrPrimaryStillPreferred( BOS_UINT32 uGuid );
   BOS_BOOL    nsBrGetActiveTgt( BOS_UINT32 uGuid, MXD_GNS::CSocketAddr& rActSockAddr );

   NSBRSTATUS  nsBrSave( BOS_UINT32 uGuid, NSBRBLK *pSaveTo );
   NSBRSTATUS  nsBrPrime( BOS_UINT32 uGuid, NSBRBLK *pPrimeFrom );

   BOS_BOOL    nsBrGetAvailTgt( BOS_UINT32 uGuid,
                                CCTKHOSTPORT& rHostPort );

   BOS_BOOL    nsBrXtlLstSetFailedTgt( BOS_UINT32 uGuid,
                                       MXD_GNS::CSocketAddr& rActSockAddr );

protected:

   void        nsBrSetOpq ( BOS_UINT32 uGuid,
                            mxt_opaque opq );

   BOS_BOOL    nsBrGetOpq ( BOS_UINT32 uGuid,
                            mxt_opaque &opq );

   NSBRBLK     *nsBrGetBlk ( BOS_UINT32 uGuid );

   void        nsBrGetTmr ( BOS_UINT32 uGuid );

private:

   NSBRMAPBLK *mpNsBrMapBlk [ NS_BR_MAX_MAP ];

   NSBRBLK    *mpNsBrBlk [ CCTKCFG_MAX_BR_USR ];


   void       nsBrManage ( NSBRBLK *pBrBlk,
                           const MXD_GNS::CHostPort& rFqdn, 
                           MXD_GNS::CList<MXD_GNS::SNaptrRecord> &rlstNaptrRecord );
  
   void       nsBrChoose ( BOS_UINT32 uUsrGuid,
                           BOS_UINT32 uGuid,
                           NSBRBLK *pBrBlk,
                           MXD_GNS::CList<MXD_GNS::SNaptrRecord> &rlstNaptrRecord );

   BOS_BOOL   nsBrEqual ( NSBRBLK *pBrBlk,
                          BOS_UINT32 uIx,
                          const MXD_GNS::CSocketAddr& rSocketAddr,
                          MXD_GNS::ESipTransport eTransport = MXD_GNS::eINVALID );

   BOS_BOOL   nsBrEqual ( NSBRBLK *pBrBlk_1,
                          BOS_UINT32 uIx_1,
                          NSBRBLK *pBrBlk_2,
                          BOS_UINT32 uIx_2,
                          MXD_GNS::ESipTransport eTransport = MXD_GNS::eINVALID );

   BOS_BOOL    nsBrAddrEqual ( NSBRBLK *pBrBlk,
                               BOS_UINT32 uIx,
                               const MXD_GNS::CSocketAddr& rSocketAddr );

   void       nsBrXtlLstUpdate( BOS_UINT32 uGuid );
};

#endif /* __NS_BR__H__INCLUDED__ */
