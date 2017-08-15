/****************************************************************************
*
* Copyright © 2006-2008 Broadcom Corporation
*
* This program is the proprietary software of Broadcom Corporation and/or its
* licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
* AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
* SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1.     This program, including its structure, sequence and organization,
*        constitutes the valuable trade secrets of Broadcom, and you shall
*        use all reasonable efforts to protect the confidentiality thereof,
*        and to use this information only in connection with your use of
*        Broadcom integrated circuit products.
*
* 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
*        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
*        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*        OR ITS LICENSORS BE LIABLE FOR
*        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
*             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
*             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
*             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Description:
*
*      ICE SDP encoding decoding module.
*      This module is responsible for generating:
*        1. a=ice-pwd and a=ice-ufrag lines at session level
*        2. ip address for the c= line at session level
*        3. port number for the m= line
*        4. a=candidate lines at media level
*      
****************************************************************************/
/* ---- Include Files ---------------------------------------- */
#include <bosTypes.h>
#include <stdio.h>
#include <bosIpAddr.h>
#include "icei.h"
#include "icesdp.h"
#include "icecndt.h"
#include "iceagnt.h"
#include "icedbg.h"
/* ---- Public Variables ------------------------------------- */

/* ---- Private Constants and Types -------------------------- */
/* candidate a-line
 * candidate-attribute   = "candidate" ":" foundation SP component-id SP
 *                         transport SP
 *                         priority SP
 *                         connection-address SP     ;from RFC 4566
 *                         port         ;port from RFC 4566
 *                         [SP cand-type]
 *                         [SP rel-addr]
 *                         [SP rel-port]
 *                         *(SP extension-att-name SP
 *                              extension-att-value)
 *
 * foundation            = 1*ice-char
 * component-id          = 1*DIGIT
 * transport             = "UDP" / transport-extension
 * transport-extension   = token              ; from RFC 3261
 * priority              = 1*DIGIT
 * cand-type             = "typ" SP candidate-types
 * candidate-types       = "host" / "srflx" / "prflx" / "relay" / token
 * rel-addr              = "raddr" SP connection-address
 * rel-port              = "rport" SP port
 * extension-att-name    = byte-string    ;from RFC 4566
 * extension-att-value   = byte-string
 * ice-char              = ALPHA / `DIGIT / "+" / "/"
 * 
 *
 */
/*
 * Example:
 * a=candidate:2 1 UDP 1694498562 192.0.2.3 45664 typ srflx raddr
 * 10.0.1.1 rport 8998
 */
#define CNDTSDP_FMT_STRING             "candidate:%s %u %s %u %s %u typ %s"
#define CNDTSDP_FMT_SUBSTRING          " raddr %s rport %u"
#define CNDTSDP_FMT_STRING_IN          "candidate:%%%ds %%u %%%ds %%u %%%ds %%u typ %%%ds raddr %%%ds rport %%u %%%ds"
#define IP_ADDR_LEN_MAX                40
#define TYP_LEN_MAX                    5
#define TRANSPORT_TYPE_LEN_MAX         3
#define EXTENSION_ATTR_LEN_MAX         64
#define ICE_LITE_STR                   "ice-lite"



/* ---- Private Variables ------------------------------------ */
typedef struct PARM_MAP
{
   int   val;
   char  *pToken;
} PARM_MAP;

static const PARM_MAP transport_map[] = { {ICE_TRANS_UDP,    "UDP" },
                                          {ICE_TRANS_TCP,    "TCP" },
                                          {-1,               NULL  } };
   
static const PARM_MAP typ_map[] = { {ICECNDT_HOST,          "host"   },  
                                    {ICECNDT_SRVRFLX,       "srflx"  },
                                    {ICECNDT_PEERRFLX,      "prflx"  },
                                    {ICECNDT_RELAYED,       "relay"  } };

/* ---- Private Function Prototypes -------------------------- */
static char *getToken( const PARM_MAP *pMap, const int val );
static int getValue( const PARM_MAP *pMap, const char *pToken );
static BOS_BOOL atoCndtAddr( const char *pAddr, const int port, ICECNDT_ADDR *pCndtAddr );
static void randIceChar( char *pBuf, const int len );
/* ---- Functions -------------------------------------------- */

/*****************************************************************************
*  FUNCTION:   icesdp_Encode
*
*  PURPOSE:    Encode from candidate list into SDP parameters.
*
*  PARAMETERS:
*              pAgnt       [MOD] - Agent
*              pSdpParms   [OUT] - SDP Parameters
*
*  RETURNS:    BOS_TRUE if SDP is encoded successfully. BOS_FALSE
*              otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL icesdp_Encode( ICEAGNT *pAgnt, ICE_SDP_PARMS *pSdpParms )
{
   ICECNDT_LST *pCndtLst;
   ICECNDT *pCndt;
   int i;
   int j = 0;
   BOS_BOOL bTransportSet = BOS_FALSE;
   
   BOS_ASSERT( NULL != pSdpParms );

   memset( pSdpParms, 0, sizeof(ICE_SDP_PARMS) );

   /* For ICE_LITE mode operation, insert a=ice-lite */
   if ( ICE_LITE == pAgnt->mode )
   {
      strncpy(pSdpParms->iceMode, ICE_LITE_STR, ICE_MODE_LEN_MAX);
   }
   else
   {
      pSdpParms->iceMode[0] = '\0';
   }

   /* Generate short term credential */
   randIceChar( pSdpParms->credential.ufrag, ICE_SDP_UFRAG_LEN_MAX );
   randIceChar( pSdpParms->credential.pwd, ICE_SDP_PWD_LEN_MAX );
#if ICEDBG_CANNED_CREDENTIAL
   strncpy( pSdpParms->credential.ufrag, ICEDBG_UFRAG, ICE_SDP_UFRAG_LEN_MAX );
   strncpy( pSdpParms->credential.pwd, ICEDBG_PWD, ICE_SDP_PWD_LEN_MAX );
#endif

   /* Generate a=candidate lines for each component */
   for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
   {
      /* Get candidate list */
      if ( (NULL == (pCndtLst = pAgnt->pLclCndtLst[i]) ) ||
           (NULL == (pCndt = pCndtLst->pCndtHead )) )
      {
         continue;
      }

      /* Go through candidate list and generate candidate a-lines for each
       * media component 
       */
      while ( (NULL != pCndt) && (j < ICE_CNDT_ALINE_NUM_MAX) )
      {
         /* Note: see example on top */
         icesdp_GenerateAline( pSdpParms->aline[j++], pCndt, pCndtLst->compId, pCndtLst->transType );

         /* set the conection ip and port to the last transport address
          * of the RTP candidate list
          */
         if ( !bTransportSet && 
               pCndt->transport.bValid && 
               (ICE_COMP_RTP == pCndtLst->compId) )
         {
            memcpy( &pSdpParms->addr, &pCndt->transport.socAddr, sizeof(STUN_TRANSPORT));
            bTransportSet = BOS_TRUE;
         }

         pCndt = pCndt->pNext;
      }
   }

   return( BOS_TRUE );
}


/*****************************************************************************
*  FUNCTION:   icesdp_Parse
*
*  PURPOSE:    Parse SDP a=candiate lines and generate candidate 
*              list.
*
*  PARAMETERS:
*              pAgnt       [MOD] - Agent
*              pSdpParms   [IN]  - pSdpParms
*
*  RETURNS:    BOS_TRUE if parsing is successful, BOS_FALSE otherwise.
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL icesdp_Parse( ICEAGNT *pAgnt, const ICE_SDP_PARMS *pSdpParms )
{
   int i;
   int j;
   ICECNDT *pCndt;
   ICECNDT_LST *pRtpCndtLst = NULL;
   ICECNDT_LST *pRtcpCndtLst = NULL;
   ICECNDT_LST *pCndtLst = NULL;
   char fmtStr[ICE_CNDT_ALINE_LEN_MAX+1];
   char transportType[TRANSPORT_TYPE_LEN_MAX+1];
   char cnxAddr[IP_ADDR_LEN_MAX+1];
   char raddrAddr[IP_ADDR_LEN_MAX+1];
   char typ[TYP_LEN_MAX+1];
   char foundation[ICECNDT_FOUNDATION_LEN_MAX+1];
   char extensionAttr[EXTENSION_ATTR_LEN_MAX+1];
   unsigned int compIdVal;
   unsigned int cnxPortVal;
   unsigned int rportVal;
   int numArgs;
   int priority;
   BOS_BOOL rc = BOS_TRUE;

   BOS_ASSERT( (NULL != pAgnt) && (NULL != pSdpParms));

   /* Set remote agent mode */
   if ( strcmp( pSdpParms->iceMode, ICE_LITE_STR ) )
   {
      pAgnt->rmtMode = ICE_FULL;
   }
   else
   {
      pAgnt->rmtMode = ICE_LITE;
   }

   /* Generate format string */
   sprintf( fmtStr, 
            CNDTSDP_FMT_STRING_IN, 
            ICECNDT_FOUNDATION_LEN_MAX,
            TRANSPORT_TYPE_LEN_MAX,
            IP_ADDR_LEN_MAX,
            TYP_LEN_MAX,
            IP_ADDR_LEN_MAX,
            EXTENSION_ATTR_LEN_MAX);

   for ( i = 0, j = 0; 
         (BOS_TRUE == rc) && (i < ICE_CNDT_ALINE_NUM_MAX); 
         i++ )
   {
      if ( '\0' == pSdpParms->aline[i][0] )
      {
         rc = BOS_TRUE;
         break;
      }

      numArgs = sscanf( pSdpParms->aline[i], fmtStr, foundation, &compIdVal, transportType, &priority, cnxAddr, &cnxPortVal, typ, raddrAddr, &rportVal, extensionAttr );
      
      /* Minimal sanity check */
      if ( (numArgs < 6) )
      {
         rc = BOS_FALSE;
         break;
      }

      if ( (ICE_COMP_RTP == compIdVal) || (ICE_COMP_RTCP == compIdVal) )
      {
         /* Fill Cndt info  */
         pCndt = icecndt_NewCndt();
         pCndt->type = getValue( typ_map, typ );
         pCndt->priority = (BOS_UINT32)priority;
         pCndt->state = ICECNDT_INUSE;
         pCndt->bInUse = BOS_TRUE;
         (void)atoCndtAddr( cnxAddr, cnxPortVal, &pCndt->transport );
         pCndt->pRelated = NULL; 
         strncpy( pCndt->foundation, foundation, ICECNDT_FOUNDATION_LEN_MAX );
      }
      else
      {
         /* Ignore unknown component type */
         continue;
      }

      switch( compIdVal )
      {
         case ICE_COMP_RTP:
         {
            if ( NULL == pRtpCndtLst )
            {
               BOS_ASSERT(j < ICE_COMP_NUM_MAX);
               /* create new list */
               pRtpCndtLst = pAgnt->pRmtCndtLst[j] = icecndt_NewList();
               pRtpCndtLst->compId = ICE_COMP_RTP;
               pRtpCndtLst->transType = getValue( transport_map, transportType );
               j++;
            }
            pCndtLst = pRtpCndtLst;
         }
         break;
         case ICE_COMP_RTCP:
         {
            if ( NULL == pRtcpCndtLst )
            {
               BOS_ASSERT(j < ICE_COMP_NUM_MAX);
               /* create new list */
               pRtcpCndtLst = pAgnt->pRmtCndtLst[j] = icecndt_NewList();
               pRtcpCndtLst->compId = ICE_COMP_RTCP;
               pRtcpCndtLst->transType = getValue( transport_map, transportType );
               j++;
            }
            pCndtLst = pRtcpCndtLst;
         }
         break;
         default:
         {
            /* skip unknown component */
            BOS_ASSERT( BOS_FALSE );
         }
      }
      
      BOS_ASSERT( NULL != pCndtLst); 
      pCndt->cndtId = pCndtLst->cndtCount;
      rc = icecndt_AddCndtToList( pCndt, pCndtLst );
      BOS_ASSERT( BOS_TRUE == rc );
   }

   if ( BOS_TRUE == rc )
   {
#if ICEDBG_CANNED_CREDENTIAL
      strncpy( (char*)pSdpParms->credential.ufrag, ICEDBG_UFRAG, ICE_SDP_UFRAG_LEN_MAX );
      strncpy( (char*)pSdpParms->credential.pwd, ICEDBG_PWD, ICE_SDP_PWD_LEN_MAX );
#endif
      /* Set remote credential */
      pAgnt->rmtCredential = pSdpParms->credential;
   }
   else
   {
      /* Clean up on error */
      for ( i = 0; i < ICE_COMP_NUM_MAX; i++ )
      {
         if ( NULL != pAgnt->pRmtCndtLst[i] )
         {
            icecndt_DeleteList( pAgnt->pRmtCndtLst[i] );
            pAgnt->pRmtCndtLst[i] = NULL;
         }
      }
   }
   
   return( rc );
}

/*****************************************************************************
*  FUNCTION:   getToken
*
*  PURPOSE:    Return token string from value.
*
*  PARAMETERS:
*              pMap - [IN] Parameter map
*              val -  [IN] look up value
*
*  RETURNS:    Token string. 
*
*  NOTES:
*              Function asserts if token not found.
*
*****************************************************************************/
static char *getToken( const PARM_MAP *pMap, const int val )
{
   BOS_ASSERT( NULL != pMap );

   for ( ; -1 != pMap->val; pMap++ )
   {
      if ( val == pMap->val )
      {
         return( pMap->pToken );
      }
   }

   BOS_ASSERT( BOS_FALSE );
   return( NULL );
}

/*****************************************************************************
*  FUNCTION:   getValue
*
*  PURPOSE:    Return val from matching token. return -1 if not found.
*
*  PARAMETERS:
*              pMap  [IN] - parameter map
*              pToken[IN] - matching token
*
*  RETURNS:    value.
*
*  NOTES:
*
*****************************************************************************/
static int getValue( const PARM_MAP *pMap, const char *pToken )
{
   BOS_ASSERT( NULL != pMap );

   if ( NULL != pToken )
   {
      for ( ; -1 != pMap->val; pMap++ )
      {
         if ( !strcmp( pToken, pMap->pToken) )
         {
            return( pMap->val );
         }
      }
   }
   return( -1 );
}

/*****************************************************************************
*  FUNCTION:   icesdp_GenerateAline
*
*  PURPOSE:    Generate the a=candidate line
*
*  PARAMETERS:
*              pLineBuf [OUT] - buffer to hold the a-line
*              compId   [IN]  - Component ID
*              transType[IN]  - Transport type
*
*  RETURNS:    none
*
*  NOTES:
*
*****************************************************************************/
void icesdp_GenerateAline( char *pLineBuf, const ICECNDT *pCndt, const BOS_UINT8 compId, const ICE_TRANS_TYPE transType )
{
   char buf[BOS_IP_ADDRESS_MAX_STR_LEN]={0};
   BOS_ASSERT( (NULL != pLineBuf) && (NULL != pCndt) );
   (void)bosIpAddrGetStr(&pCndt->transport.socAddr.ipAddr, buf, BOS_IP_ADDRESS_MAX_STR_LEN);
   sprintf( pLineBuf,
            CNDTSDP_FMT_STRING,
            pCndt->foundation,
            compId,
            getToken( transport_map, transType ),
            (unsigned int)pCndt->priority,
            buf,
            pCndt->transport.socAddr.port,
            getToken( typ_map, pCndt->type ) );
   if ( (ICECNDT_SRVRFLX == pCndt->type) ||
        (ICECNDT_RELAYED == pCndt->type ))
   {
      BOS_ASSERT( NULL != pCndt->pRelated );
      (void)bosIpAddrGetStr(&pCndt->pRelated->socAddr.ipAddr, buf, BOS_IP_ADDRESS_MAX_STR_LEN);
      /* Add substring for non 'local' type */
      sprintf( pLineBuf + strlen( pLineBuf ),
               CNDTSDP_FMT_SUBSTRING,
               buf,
               pCndt->pRelated->socAddr.port );
   }
}

/*****************************************************************************
*  FUNCTION:   atoCndtAddr
*
*  PURPOSE:    Convert from ascii network address and port format 
*              (e.g. x.x.x.x) into CndtAddr address format.
*
*  PARAMETERS:
*              pAddr - [IN] network addres
*              port -  [IN] network port
*              pCndtAddr - [OUT] CndtAddr
*
*  RETURNS:    none
*
*  NOTES:
*              TODO: Support IPV4 only.
*
*****************************************************************************/
static BOS_BOOL atoCndtAddr( const char *pAddr, const int port, ICECNDT_ADDR *pCndtAddr )
{
   BOS_STATUS status = bosIpAddrCreateFromStr( pAddr, &pCndtAddr->socAddr.ipAddr );

   if ( BOS_STATUS_OK != status )
   {
      return( BOS_FALSE );
   }

   pCndtAddr->bValid = BOS_TRUE;
   pCndtAddr->socAddr.port = (BOS_SOCKET_IP_PORT)port;

   return( BOS_TRUE );

}

/*****************************************************************************
*  FUNCTION:   randIceChar
*
*  PURPOSE:    Generate random string of printable characters 
*              [A-Z][a-z][0-9][+][/]of <len> into
*              <pBuf>. 
*              
*              <pBuf> must have sufficient buffer space for holding <len+1> 
*              characters.
*  
*  PARAMETERS:
*              pBuf  - [OUT] buffer to hold the output
*              len   - [IN] number of bytes of randomness
*
*  RETURNS:    none
*
*  NOTES:
*              Algorithm used:
*              1. There are a total of 64 allowable characters, which means
*                 6 bits of randomization is needed.
*              2. Generate a 16 bit random number, exclusive-or the upper
*                 and lower byte
*              3. use the value of the lower 6 bits to index to one of the
*                 64 allowed characters.
*
*****************************************************************************/
static void randIceChar( char *pBuf, const int len )
{
   BOS_UINT16 rand16;
   BOS_UINT8 val;
   int i; 
   char c;

   memset( pBuf, 0, len+1 );

   for ( i = 0; i < len; i++ )
   {
      rand16 = (BOS_UINT16)rand();
      val = (BOS_UINT8)(rand16>>8); 
      val = (val ^ ((BOS_UINT8)(rand16 & 0x00ff))) & 0x3f; 

      if ( val < 26 )
      {
         c = 'A' + val;
      }
      else if ( val < 52 )
      {
         c = 'a' + (val - 26);
      }
      else if ( val < 62 )
      {
         c = '0' + (val - 52);
      }
      else if ( val == 62 )
      {
         c = '+';
      }
      else
      {
         c = '/';
      }

      *pBuf++ = c;
   }
}
