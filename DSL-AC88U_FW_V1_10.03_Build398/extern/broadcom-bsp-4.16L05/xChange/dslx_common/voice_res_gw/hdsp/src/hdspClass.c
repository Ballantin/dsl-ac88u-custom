/****************************************************************************
*
*  Copyright (c) 2000-2008 Broadcom Corporation
*
*  This program is the proprietary software of Broadcom Corporation and/or
*  its licensors, and may only be used, duplicated, modified or distributed
*  pursuant to the terms and conditions of a separate, written license
*  agreement executed between you and Broadcom (an "Authorized License").
*  Except as set forth in an Authorized License, Broadcom grants no license
*  (express or implied), right to use, or waiver of any kind with respect to
*  the Software, and Broadcom expressly reserves all rights in and to the
*  Software and all intellectual property rights therein.  IF YOU HAVE NO
*  AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
*  AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
*  SOFTWARE.
*
*  Except as expressly set forth in the Authorized License,
*
*  1.     This program, including its structure, sequence and organization,
*  constitutes the valuable trade secrets of Broadcom, and you shall use all
*  reasonable efforts to protect the confidentiality thereof, and to use this
*  information only in connection with your use of Broadcom integrated circuit
*  products.
*
*  2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*  "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
*  OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*  RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*  IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
*  A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*  ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*  THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*  3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
*  OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
*  INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
*  RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
*  HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
*  EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
*  WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
*  FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*    Filename: hdspclass.c
*
****************************************************************************
*    Description:
*
*      This file contains the interface for the class (caller id) transmitter
*
****************************************************************************/

#include <string.h>
#include <ctype.h>

#include <vrgEndptCfg.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <hapi_clidtx.h>               /* CallerID Transmitter */
#include <hdspCfg.h>
#include <hdspTone.h>
#ifdef HDSP_CFG_CLIDRX
#include <hapi_clidrx.h>               /* CallerID Receiver */
#endif
#include "hdspClass.h"
#include "hdspCmd.h"

#define CLASS_DEBUG 0                  /* Set to 1 for debug prints */

/*
** CLASS Message Formats
*/
#define CLASS_SDMF            0x04
#define CLASS_MDMF            0x80

/*
** Class Type Fields - defined in TR-NWT-001188
*/
#define CLASS_TIME            0x01
#define CLASS_ID              0x02
#define CLASS_DDN             0x03  /* Canadian CLID format http://www.ainslie.org.uk/callerid/cli_faq.htm#Q_7 */
#define CLASS_NOID            0x04
#define CLASS_NAME            0x07
#define CLASS_NONAME          0x08

#define CLASS_START           0x00
#define CLASS_CHECKSUM        0x09
#define CLASS_CALLTYPE        0x11

/* Visual Message Waiting Indicator Formats TR-NWT-001401 */
#define VMWI_MDMF             0x82
#define VMWI_SDMF             0x06

/* VMWI Type fields */
#define VMWI_HEADER           0x0B
#define VMWI_ACTIVATE         0xFF
#define VMWI_DEACTIVATE       0x00

#define VMWI_SDMF_ACTIVATE    0x42
#define VMWI_SDMF_DEACTIVATE  0x6F


/* frequency and amplitude definitions for the V.23 modem */
#define CLIDTX_AMP_DEFAULT           CLIDTX_AMP         /* Mark Amplitude (-14 dBm)  (22656 = 0dBm) */
#define CLIDTX_MARK_FREQ_DEFAULT     CLIDTX_MARK_FREQ   /* Mark Frequency 1300  Hz                  */
#define CLIDTX_SPACE_FREQ_DEFAULT    CLIDTX_SPACE_FREQ  /* Space Frequency 2100 Hz                  */

/*
** Japan CLID Fields - defined in NTT - Telephone Service Interfaces, Edition 5
*/
#define CLASS_SOH       0x01     /* start of heading */
#define CLASS_STX       0x02     /* start of text */
#define CLASS_ETX       0x03     /* end of text */
#define CLASS_SO        0x0e     /* shift out */
#define CLASS_SI        0x0f     /* shift in */
#define CLASS_DLE       0x10     /* data link escape */
#define CLASS_HDR_JP    0x07     /* header byte */
#define CLASS_SRVTYP_JP 0x40     /* clid service type */
#define CLASS_JAPAN_HEADER_SIZE_BYTES  7 /* Japan CLID RX header size in bytes */
#define CLASS_DEFAULT_DATE "01010000"

/* frequency and amplitude definitions for the V.23 modem */
#define  CLIDTX_AMP_JAPAN        4520  /* Mark Amplitude (-14 dBm)  (22656 = 0dBm) */
#define  CLIDTX_MARK_FREQ_JAPAN  1300  /* Mark Frequency 1300  Hz                  */
#define  CLIDTX_SPACE_FREQ_JAPAN 2100  /* Space Frequency 2100 Hz                  */

/* polynomial definition for Japan message format */
#define  CLASS_JCLIPCRCPOLY   0x1021   /* CCITT CRC polynomial */


/* caller ID packet string definitions */
#define  CLASS_MAX_PARAM_SIZE    22

/*
** Private Macros
*/

/* Check Japan blockcodes */
#define IS_BLOCKED_JAPAN( blockCode )        \
   ( ( toupper( blockCode ) == 'P' )   ||    \
     ( toupper( blockCode ) == 'O' )   ||    \
     ( toupper( blockCode ) == 'C' )   ||    \
     ( toupper( blockCode ) == 'S' ) )

/* Check Taiwan blockcodes */
#define IS_BLOCKED_TAIWAN( blockCode )       \
   ( ( toupper( blockCode ) == 'P' )   ||    \
     ( toupper( blockCode ) == 'O' )   ||    \
     ( toupper( blockCode ) == 'I' )   ||    \
     ( toupper( blockCode ) == 'C' ) )

/* Check Denmark blockcodes. Note 'E' is for DTMF CLID Error code "D3#" */
#define IS_BLOCKED_DENMARK( blockCode )      \
   ( ( toupper( blockCode ) == 'P' )   ||    \
     ( toupper( blockCode ) == 'O' )   ||    \
     ( toupper( blockCode ) == 'E' ) )

/* Check Default blockcodes */
#define IS_BLOCKED_DEFAULT( blockCode )      \
   ( ( toupper( blockCode ) == 'P' )   ||    \
     ( toupper( blockCode ) == 'O' ) )

/* DTMF digit to freq pair map */
typedef struct DBM_Q15_MAP
{
   VRG_UINT16 dBm;      /* power level in dBm */
   VRG_UINT16 q15;      /* power level as Q.15 value */
} DBM_Q15_MAP;

/* Mapping from dBm power levels (in pos. tens of dBm) to Q.15 values */
static const DBM_Q15_MAP gdBm2Q15Map[] =
{
   {0, 22656},
   {5, 21389},
   {10, 20192},
   {15, 19063},
   {20, 17996},
   {25, 16990},
   {30, 16039},
   {35, 15142},
   {40, 14295},
   {45, 13495},
   {50, 12740},
   {55, 12028},
   {60, 11355},
   {65, 10720},
   {70, 10120},
   {75, 9554},
   {80, 9020},
   {85, 8515},
   {90, 8039},
   {95, 7589},
   {100, 7164},
   {105, 6764},
   {110, 6385},
   {115, 6028},
   {120, 5691},
   {125, 5373},
   {130, 5072},
   {135, 4788},
   {140, 4520},
   {145, 4268},
   {150, 4029},
   {155, 3803},
   {160, 3591},
   {165, 3390},
   {170, 3200},
   {175, 3021},
   {180, 2852},
   {185, 2693},
   {190, 2542},
   {195, 2400},
   {200, 2266},
   {0xffff, 0xffff}
};


/*
** Local function prototypes
*/

#ifdef HDSP_CFG_CLIDRX
static void dispCallerID( HDSP_CLASS_CLIDRX_STATE *clidRxStatep );
static void fillClidRxString(VRG_COUNTRY country, unsigned char key, char * pClidRxString);
static VRG_BOOL disAssembleCLASS( VRG_COUNTRY country, HDSP_CLASS_CLIDRX_STATE *clidRxStatep );
#if VRG_COUNTRY_CFG_JAPAN
static VRG_BOOL disAssembleJapanClidRx( HDSP_CLASS_CLIDRX_STATE *clidRxStatep );
#endif
#endif

static VRG_UINT16 copyMessage( VRG_UINT16* dst, const unsigned char* src, int bytes );
static VRG_UINT16 dBmToQ15 ( VRG_UINT16 dBmVal );

static VRG_SINT16 classTxConfigure
(
   VRG_UINT16     vhdHdl,
   int            lineId,
   CLASS_MSG_TYPE classtype,
   CLASS_MSG_FSK_PARMS *fskParmp,
   VRG_UINT16     clidTxDelay
);

static VRG_SINT16 classCreatePacketMDMF
(
   VRG_COUNTRY       country,
   CLASS_CLID_STATE *packetInfo,
   VRG_UINT16        vhdHdl,
   CLASS_MSG_TYPE    classtype,
   CLASS_MSG_FSK_PARMS *fskParmp
);

#if VRG_COUNTRY_CFG_JAPAN
static VRG_SINT16 classCreatePacketJapan
(
   CLASS_CLID_STATE *packetInfo,
   VRG_UINT16        vhdHdl
);

static void classJapanFormatBytes
(
   VRG_UINT16 *checksum,
   char       *message,
   int         numbytes,
   VRG_UINT16 *dest
);

static VRG_UINT16 classJCLIDChecksum ( VRG_UINT16 data, VRG_UINT16 accum );
#endif
static VRG_BOOL isClidBlocked( const char *clidStr, VRG_COUNTRY country, char *blockReason );
static VRG_BOOL isValidClidChar( char c );
static VRG_SINT16 SendMdmfVmwiMessage( VRG_UINT16 vhdHdl, VRG_UINT16 type, VRG_COUNTRY country );
static VRG_SINT16 SendSdmfVmwiMessage( VRG_UINT16 vhdHdl, VRG_UINT16 type, VRG_COUNTRY country );


#ifdef HDSP_CFG_CLIDRX
static VRG_BOOL classRxDepacketize( HSZCLIDRXPACKET *clidRxPktp, HDSP_CLASS_CLIDRX_STATE *clidRxStatep );
#if VRG_COUNTRY_CFG_JAPAN
static VRG_BOOL checkParityBit( VRG_UINT16 numByte, VRG_UINT16 *checksum, VRG_UINT8 *msgp );
#endif
static void  classByteOrder( unsigned char *bufp, VRG_UINT16 length );
#endif

/*
*****************************************************************************
** FUNCTION:   hdspClassMessage
**
** PURPOSE:    This function prepares the packets for the caller ID
**             transmission and sends them down to the CLIDTX Service.
**
** PARAMETERS: clidInfo    - pointer to caller ID message information
**             vhdHdl      - the vhd handle for VHD with CLIDTX Service
**             country     - country code
**             classtype   - type I (on-hook) or type II (off-hook)
**
** RETURNS:    HAPISUCCESS on success, else error code
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspClassMessage
(
   CLASS_CLID_STATE *clidInfo,
   VRG_UINT16        vhdHdl,
   int               lineId,
   VRG_COUNTRY       country,
   CLASS_MSG_TYPE    classtype,
   CLASS_MSG_FSK_PARMS *fskParmp,
   VRG_UINT16        clidTxDelay
)
{

   VRG_SINT16 rc;

   /* Configure the CLIDTX registers based on country */
   rc = classTxConfigure( vhdHdl, lineId, classtype, fskParmp, clidTxDelay );
   if (rc != HAPISUCCESS)
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "HDSP: Configure Class %d Failed on vhdHdl %x", classtype, vhdHdl ));
      return ( rc );
   }

   /*
   ** Pick packet format based on country
   */
   XCHG_ASSERT( country < VRG_COUNTRY_MAX );

#if VRG_COUNTRY_CFG_JAPAN
   if ( country == VRG_COUNTRY_JAPAN )
   {
      /*
      ** Japan
      */
      rc = classCreatePacketJapan( clidInfo, vhdHdl );
   }
   else
#endif
   {
      /*
      ** North American and Europe use the same caller-id message format.
      */
      rc = classCreatePacketMDMF( country, clidInfo, vhdHdl, classtype, fskParmp );
      if (rc != HAPISUCCESS)
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "HDSP: Assembly Class %d Failed on vhdHdl %x", classtype, vhdHdl ));
         return ( rc );
      }
   }

#if CLASS_DEBUG
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "CLASS %d - CALLER ID %s %s %s",
         classtype, clidInfo->date, clidInfo->number, clidInfo->name ));
#endif /* CLASS_DEBUG */

   return ( rc );
}


/*
*****************************************************************************
** FUNCTION:   dBmToQ15
**
** PURPOSE:    Converts a dBm value to a Q.15 number via table lookup.
**
** PARAMETERS: dBmVal      - power level in dBm (pos. value in tens of dBm)
**
** RETURNS:    power level represented as a Q.15 number
**
** NOTE:
**
*****************************************************************************
*/
static VRG_UINT16 dBmToQ15 ( VRG_UINT16 dBmVal )
{
   unsigned int i;

   for(i = 0; dBmVal > gdBm2Q15Map[i].dBm; i++);

   return ( gdBm2Q15Map[i].q15 );
}


/*
*****************************************************************************
** FUNCTION:   classTxConfigure
**
** PURPOSE:    This function sets the CLIDTX Service registers based on
**             the country code.
**
** PARAMETERS: vhdHdl      - the vhd handle for VHD with CLIDTX Service
**             classtype   - type of class service
**             fskParmp    - pointer to FSK parameters to use for CLASS TX
**             clidTxDeley - CLID TX start delay
**
** RETURNS:    HAPISUCCESS on success
**
** NOTE:
**
*****************************************************************************
*/
static VRG_SINT16 classTxConfigure
(
   VRG_UINT16        vhdHdl,
   int               lineId,
   CLASS_MSG_TYPE    classtype,
   CLASS_MSG_FSK_PARMS *fskParmp,
   VRG_UINT16        clidTxDelay
)
{
   HSZCLIDTXREGS regs;
   VRG_UINT16 adjustedAmp;

   /* Adjust power level to take DLP into account. */
   adjustedAmp = hdspToneAdjustEgressPowerLevel(fskParmp->amplitude, lineId);

   /* Convert the power level to a Q.15 number. */
   regs.Amplitude      = dBmToQ15(adjustedAmp);

   regs.MarkFrequency  = fskParmp->markFrequency;
   regs.SpaceFrequency = fskParmp->spaceFrequency;

   regs.delay = clidTxDelay;

#if CLASS_DEBUG
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "HDSP Class config: amp=0x%x; markFreq=%dHz; spaceFreq=%dHz; delay=%d; numSeize=%d; numMark=%d",
             regs.Amplitude, regs.MarkFrequency, regs.SpaceFrequency, regs.delay,
             fskParmp->startNumSeizes, fskParmp->startNumMarks ));
#endif /* CLASS_DEBUG */

   return ( hdspSendCmdData( vhdHdl,
                             HAPI_CLIDTX_SETREGS,
                             HSCMDEXTDATA_SYNC,
                             sizeof( HSZCLIDTXREGS ),
                             &regs ));
}


/*
*****************************************************************************
** FUNCTION:   classCreatePacketMDMF
**
** PURPOSE:    This function Generates a fake class signal (multiple data
**             message format) as per GR-30-CORE and TR-NWT-000031
**
** PARAMETERS: packetInfo  - caller id info (name, number, date)
**             vhdHdl      - the vhd handle
**             classtype   - CLASSI or CLASS2
**             fskParmp    - FSK modulation parameters
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
static VRG_SINT16 classCreatePacketMDMF
(
   VRG_COUNTRY       country,
   CLASS_CLID_STATE *packetInfo,
   VRG_UINT16        vhdHdl,
   CLASS_MSG_TYPE    classtype,
   CLASS_MSG_FSK_PARMS *fskParmp
)
{
   HSZCLASSRELAYPACKET packet;

   VRG_BOOL   multiplePacket = VRG_FALSE;
   VRG_UINT16 checksum = 0;
   VRG_UINT16 msglen;
   VRG_UINT16 numberLen;
   VRG_UINT16 nameLen, tempLen;

   char  date[CLASS_DATE_SIZE + 1];
   char  number[CLASS_MAX_NUMBER_SIZE + 1];
   char  name[CLASS_MAX_NAME_SIZE + 1];
   char  numberPType;
   char  namePType;
   char  defaultDate[CLASS_DATE_SIZE + 1] = CLASS_DEFAULT_DATE;

   /* Prepare the class start packet */
   /* Payload ID */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_START );
   packet.hsxPacketHdr.payloadLength = 4;                      /* Number of Payload bytes   */
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;                /* Timestamp of packet       */

   /* Seizure and Mark Bit lengths differ for Class I &II */
   if ( classtype == CLASS_MSG_ONHOOK_CLID )
   {
      packet.hsxWord0 = fskParmp->startNumSeizes;                 /* # Seizes to send          */
      packet.hsxWord1 = fskParmp->startNumMarks;                  /* # Marks to follow Seize   */
   }
   else
   {
      /* No seizure bits in Class II */
      packet.hsxWord0 = 0;                                        /* # Seizes to send          */
      packet.hsxWord1 = 80;                                       /* # Marks to follow Seize   */
   }

   /* send the class start packet */
   hdspSendDataPacket( vhdHdl, &packet );

   /* -------------Prepare the body Packets --------------------------------------- */

   /* There will be 3 parameters sent each with two bytes in the parameter header. */
   msglen = 3 * 2;

   /* process the date field */
   if ( strlen( packetInfo->date ) != CLASS_DATE_SIZE )
   {
      /* date is invalid, so make a fake date of January 1 0:00 */
      strcpy(date, defaultDate );
   }
   else
   {
      /* date is fine so just copy it */
      strcpy(date, packetInfo->date);
   }
   msglen += CLASS_DATE_SIZE;

   /* process the number field and get the length */
   if (  (( toupper(packetInfo->reasonNoNumber) == 'P' ) ||
          ( toupper(packetInfo->reasonNoNumber) == 'O' )) ||
#if VRG_COUNTRY_CFG_TAIWAN
        ( ( country == VRG_COUNTRY_TAIWAN ) &&               /* Add I and C for Taiwan. */
          (( toupper(packetInfo->reasonNoNumber) == 'I' ) ||
           ( toupper(packetInfo->reasonNoNumber) == 'C' )) ))
#else
         VRG_FALSE)
#endif
   {
      /* Number will be blocked */
      number[0] = (char) toupper(packetInfo->reasonNoNumber);
      number[1] = '\0';
      numberLen = 1;
      numberPType = CLASS_NOID;
   }
   else
   {
      /* Number will not be blocked */
      numberLen = (VRG_UINT16) strlen( packetInfo->number );
      strcpy( number, packetInfo->number );
      numberPType = CLASS_ID;
   }
   msglen += numberLen;

   /* process the name field and get the length */
   if (  (( toupper(packetInfo->reasonNoName) == 'P' ) ||
          ( toupper(packetInfo->reasonNoName) == 'O' )) ||
#if VRG_COUNTRY_CFG_TAIWAN
        ( ( country == VRG_COUNTRY_TAIWAN ) &&               /* Add I and C for Taiwan. */
          (( toupper(packetInfo->reasonNoName) == 'I' ) ||
           ( toupper(packetInfo->reasonNoName) == 'C' )) ))
#else
         VRG_FALSE)
#endif
   {
      /* Name will be blocked */
      name[0] = (char) toupper(packetInfo->reasonNoName);
      name[1] = '\0';
      nameLen = 1;
      namePType = CLASS_NONAME;
   }
   else
   {
      /* Name will not be blocked */
      nameLen = (VRG_UINT16) strlen( packetInfo->name );
      strcpy( name, packetInfo->name );
      namePType = CLASS_NAME;
   }
   msglen += nameLen;

   /* Set up the first body packet which contains the message header and the date parameter */
   /* Payload ID */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_BODY );
   packet.hsxPacketHdr.payloadLength = 4 + (4+CLASS_DATE_SIZE);       /* Number of bytes = Word0 + Word1 + headers(4 bytes) + #bytes in time field */
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;                       /* Timestamp of Packet */

   packet.hsxWord0 = 2 + 2 + CLASS_DATE_SIZE;                         /* Number of class bytes, 2 message header + 2 parameter header + date */
   packet.hsxWord1 = 0;                                               /* Number of marks to generate after class packet */

   /* fill in the message header for MDMF */
   packet.hsxClassMsg[0] = (VRG_UINT16) ((CLASS_MDMF) + (msglen <<8));              /* msgtype, msglen */
   checksum += CLASS_MDMF;
   checksum += msglen;

   /* fill in the parameter header for the date */
   packet.hsxClassMsg[1] = ( CLASS_TIME ) + (CLASS_DATE_SIZE << 8);  /* time param, msglen */
   checksum += CLASS_TIME;
   checksum += CLASS_DATE_SIZE;

   /* fill in the date */
   checksum += copyMessage( &packet.hsxClassMsg[2], (unsigned char*)date, CLASS_DATE_SIZE );

   /* Send Packet */
   hdspSendDataPacket( vhdHdl, &packet );

   /* Prepare 2nd body packet which contains Calling Line ID or Reason for Absence of DN parameter */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_BODY );
   packet.hsxPacketHdr.payloadLength = 4 + 2 + numberLen + 1;
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;

   packet.hsxWord0 = 2 + numberLen;  /*#bytes*/
   packet.hsxWord1 = 0;              /*mark*/

   /* fill in the Calling Line ID or Reason for Absence of DN parameter header */
   packet.hsxClassMsg[0] = (VRG_UINT16) (( numberPType ) + (numberLen << 8)); /* id param, msglen */
   checksum += numberPType;
   checksum += numberLen;

   /* copy message */
   checksum += copyMessage( &packet.hsxClassMsg[1], (unsigned char*)number, numberLen );

   /* Send 2nd Packet */
   hdspSendDataPacket( vhdHdl, &packet );

   /* Prepare 3rd body packet which contains Name or Reason for Absence of Name parameter */

   /* FIX by Alliu: ETSI300 659-3 requires maximum 50 Bytes of calling party name
    * however the length of data which hdspSendDataPacket() can send for each of
    * packet is only 21 words(40 bytes), so we have to split "calling party name"
    * to two hdspSendDataPacket() calls
    */
   if( nameLen > 30 )
   {
#if CLASS_DEBUG
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, " DEBUG ************ multiple packet per name nameLen = %d", nameLen));
#endif /* CLASS_DEBUG */
      multiplePacket = VRG_TRUE;
      tempLen = 30 ;
   }
   else
   {
#if CLASS_DEBUG
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, " DEBUG ************ single packet per name nameLen = %d", nameLen));
#endif /* CLASS_DEBUG */
      multiplePacket = VRG_FALSE;
      tempLen = nameLen;
   }

   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_BODY );
   packet.hsxPacketHdr.payloadLength = 4 + 2 + tempLen + 1;
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;

   packet.hsxWord0 = 2 + tempLen;      /* #Bytes */
   packet.hsxWord1 = 0;                /* Mark */

   /* fill in parameter header for Name or Reason for Absence of Name */
   packet.hsxClassMsg[0] = (VRG_UINT16) ((namePType) + ( nameLen << 8 ));  /* name param, msglen */
   checksum += namePType;
   checksum += nameLen;

   /* copy message */
   checksum += copyMessage( &packet.hsxClassMsg[1], (unsigned char*)name, tempLen );

   /* Send 3rd body packet*/
#if CLASS_DEBUG
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, " DEBUG ************ first name packet length = %d", packet.hsxWord0));
#endif /* CLASS_DEBUG */
   hdspSendDataPacket( vhdHdl, &packet );

   if( multiplePacket )
   {
      /* name length is longer than what hdspSendPacket() can hold */
      packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_BODY );
      packet.hsxPacketHdr.payloadLength = 4 + ( nameLen - tempLen ) + 1;
      packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
      packet.hsxPacketHdr.payloadTimeStampLow = 0;

      packet.hsxWord0 = (nameLen - tempLen);    /* #Bytes */
      packet.hsxWord1 = 0;                      /* Mark */

      /* copy message */
      checksum += copyMessage( &packet.hsxClassMsg[0], (unsigned char*) &name[tempLen], (nameLen - tempLen) );

      /* Send 3rd body packet*/
#if CLASS_DEBUG
      VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, " DEBUG ************ second name packet length = %d", packet.hsxWord0));
#endif /* CLASS_DEBUG */
      hdspSendDataPacket( vhdHdl, &packet );
   }
   /* Prepare the CLASS end packet */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_END );
   packet.hsxPacketHdr.payloadLength = 4 + 1 + 1;
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;

   /* Set seizure and Mark Bit lengths */
   packet.hsxWord0 = 1;  /* #Bytes */
   packet.hsxWord1 = 10; /* Mark (1-10 normaly, some countries (ie. UK) need 2-10) */

   packet.hsxClassMsg[0] = (VRG_UINT16) ((~checksum) + 1);

   /* Send the CLASS end packet*/
   hdspSendDataPacket( vhdHdl, &packet );

   /* Return success */
   return (HAPISUCCESS);
}

#if VRG_COUNTRY_CFG_JAPAN
/*
*****************************************************************************
** FUNCTION:   classCreatePacketJapan
**
** PURPOSE:    This function generates Japan CLID format packets from the
**             fields passed in by the packetInfo structure.  The packets
**             are passed to the CLIDTX Service and queued for processing.
**
** PARAMETERS: packetInfo - pointer to caller ID message information
**             vhdHdl - the vhd handle for VHD with CLIDTX Service
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:       there is no error checking done on packet send commands to DSP
**
** even parity generation on control codes and data
** start and stop bits inserted by the clidtx service
** message format
**    Mark Bit | DLE | SOH | Header | DLE | STX | Data* | DLE | ETX | CRC(bit-reversed)
**    - 72 mark bits
**    - *data format
**         Service type | Data size | Parameter 1** | ... | Parameter n
**    - **parameter format
**         Parameter type | Parameter size | Parameter content
**    - CRC done on data starting from Header
**    - CRC uses polynomial 0x1201
**    - CRC sent in reverse order, MSB sent first, LSB sent last
**
*****************************************************************************
*/
static VRG_SINT16 classCreatePacketJapan( CLASS_CLID_STATE *packetInfo, VRG_UINT16 vhdHdl )
{
   HSZCLASSRELAYPACKET packet;

   VRG_UINT16 checksum;
   VRG_UINT16 brChecksum;
   VRG_UINT16 msglen;
   VRG_UINT16 numberLen;

   char  number[CLASS_MAX_NUMBER_SIZE + 1];
   char  numberPType;
   char  message[CLASS_MAX_PARAM_SIZE + 1];

   int j;

   /* Prepare the class start packet */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_START );
   packet.hsxPacketHdr.payloadLength = 4;                      /* Number of Payload bytes   */
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;                /* Timestamp of packet       */

   packet.hsxWord0 = 0;                                        /* # Seizes to send          */
   packet.hsxWord1 = 72;                                       /* # Marks to follow Seize   */

   /* send the class start packet */
   hdspSendDataPacket( vhdHdl, &packet );

   /* -------------Prepare the body Packets --------------------------------------- */

   /* initialize total message length */
   msglen = 0;

   /* process the number field and get the length */
   if (( toupper(packetInfo->reasonNoNumber) == 'P' ) ||
       ( toupper(packetInfo->reasonNoNumber) == 'O' ) ||
       ( toupper(packetInfo->reasonNoNumber) == 'C' ) ||
       ( toupper(packetInfo->reasonNoNumber) == 'S' ))
   {
      /* Number will be blocked */
      number[0] = (char) toupper(packetInfo->reasonNoNumber);
      number[1] = '\0';

      /* only 1 byte in number */
      numberLen = 1;

      /* set the parameter type to no id */
      numberPType = CLASS_NOID;

      /* add 2 bytes for parameter type and length fields */
      msglen += 2;
   }
   else
   {
      /* Number will not be blocked, copy the input number */
      numberLen = (VRG_UINT16) strlen( packetInfo->number );
      strcpy( number, packetInfo->number );

      /* set parameter type for displaying number */
      numberPType = CLASS_ID;

      /* add 2 bytes for parameter type and length fields */
      msglen += 2;
   }

   if ( numberLen == 16 )
   {
      /* if numberLen is 16, then an extra DLE must be inserted into message */
      msglen += 1;
   }

   msglen += numberLen;

   /* Set up the first body packet which contains the message header up to the first parameter */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_BODY );
   packet.hsxPacketHdr.payloadLength = 4 + 7 + 1;                     /* 7 header bytes            */
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;                       /* Timestamp of Packet       */

   packet.hsxWord1 = 0;                                               /* Number of marks to generate after class packet */

   /* fill in the message */
   message[0] = CLASS_DLE;
   message[1] = CLASS_SOH;
   message[2] = CLASS_HDR_JP;
   message[3] = CLASS_DLE;
   message[4] = CLASS_STX;
   message[5] = CLASS_SRVTYP_JP;

   /* check for DLE needed in front of message length */
   if ( msglen == 16 )
   {
      /* 8 class bytes: DLE, SOH, HEADER, DLE, STX, TYPE, DLE, LENGTH */
      message[6] = CLASS_DLE;
      message[7] = (char)msglen;
      packet.hsxWord0 = 8;
   }
   else
   {
      /* 7 class bytes: DLE, SOH, HEADER, DLE, STX, TYPE, LENGTH */
      message[6] = (char)msglen;
      packet.hsxWord0 = 7;
   }

   /* process the first two bytes of message and copy into packet */
   checksum = 0;
   classJapanFormatBytes( &checksum, message, 2, &packet.hsxClassMsg[0] );

   /* process the rest of the message, start the checksum on the HDR byte */
   checksum = 0;
   classJapanFormatBytes( &checksum, &message[2], packet.hsxWord0 - 2, &packet.hsxClassMsg[1] );

   /* Send Packet */
   hdspSendDataPacket( vhdHdl, &packet );

   /* Prepare 2nd body packet which contains Calling Line ID or Reason for Absence of DN parameter */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_BODY );
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;

   packet.hsxWord1 = 0;

   /* fill in the message */
   message[0] = numberPType;
   if ( numberLen == 16 )
   {
      /* need to add an extra DLE */
      message[1] = CLASS_DLE;
      message[2] = 16;
      strcpy( &message[3], number );
      /* number of bytes is number length plus 1 for extra DLE plus two bytes for parameter header */
      packet.hsxWord0 = 1 + numberLen + 2;
   }
   else
   {
      /* length is not 16, no extra DLE */
      message[1] = (char) numberLen;
      strcpy( &message[2], number );
      /* number of bytes is number length plus two bytes for parameter header */
      packet.hsxWord0 = numberLen + 2;
   }

   packet.hsxPacketHdr.payloadLength = 4 + packet.hsxWord0 + 1;

   /* process message and copy into packet */
   classJapanFormatBytes( &checksum, message, packet.hsxWord0, &packet.hsxClassMsg[0] );

   /* Send 2nd Packet */
   hdspSendDataPacket( vhdHdl, &packet );

   /* Prepare the end packet */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_END );
   packet.hsxPacketHdr.payloadLength = 4 + 4;
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;

   packet.hsxWord0 = 4;    /* 4 bytes: DLE, ETX, CRChigh, CRClow */
   packet.hsxWord1 = 0;

   /* fill in the message */
   message[0] = CLASS_DLE;
   message[1] = CLASS_ETX;

   /* process message and copy into packet */
   classJapanFormatBytes( &checksum, message, 2, &packet.hsxClassMsg[0] );

   /* bit-reverse checksum */
   for ( j = 0, brChecksum = 0; j < 16; j++ )
   {
      brChecksum = brChecksum | ((VRG_UINT16) ((( checksum >> j ) & 1 ) << ( 15 - j )));
   }

   /* store checksum (2 bytes) in packet */
   packet.hsxClassMsg[1] = brChecksum;

   /* Send the CLASS end packet*/
   hdspSendDataPacket( vhdHdl, &packet );

   /* Return success */
   return (HAPISUCCESS);
}


/*
*****************************************************************************
** FUNCTION:   classJCLIDChecksum
**
** PURPOSE:    Calculates the checksum using the 0x1021 CCITT polynomial
**
** PARAMETERS: data - the message bytes
**             accum - the accumulating checksum word
**
** RETURNS:    the new checksum word
**
** NOTE:
**             This function was taken mainly from 'Asynchronous Programming in C'
**             Chapter 23
**
*****************************************************************************
*/
static VRG_UINT16 classJCLIDChecksum ( VRG_UINT16 data, VRG_UINT16 accum )
{
   int j;

   /* data to high byte order */
   data <<= 8;

   for ( j=8; j>0; j--)
   {
      /* check if msb of (data XOR accum) is TRUE */
      /* the "<< (j-1)" is in there because we want to use sequence */
      /* of b1 to bp */
      if ( ( (data << (j-1)) ^ accum ) & 0x8000 )
      {
         /* shift and subtract poly */
         accum = (VRG_UINT16) (( accum << 1 ) ^ CLASS_JCLIPCRCPOLY );
      }
      else
      {
         /* otherwise, transparent shift */
         accum <<= 1;
      }
   }

   return( accum );
}


/*
*****************************************************************************
** FUNCTION:   classJapanFormatBytes
**
** PURPOSE:    The input bytes have the parity bit added, and the checksum
**             is updated using the formatted bytes.  The data is then
**             copied to the destination buffer in the correct byte order.
**
** PARAMETERS: checksum (IN/OUT) - the current checksum
**             message (IN) - the message bytes to be processed
**             numbytes (IN) - the number of bytes to be processed
**             dest (OUT) - pointer to destination buffer for output
**
** RETURNS:    nothing
**
** NOTE:
**
*****************************************************************************
*/
static void classJapanFormatBytes( VRG_UINT16 *checksum, char *message, int numbytes, VRG_UINT16 *dest )
{
   int i, j;
   int parityBit;

   for ( i = 0; i < numbytes; i++ )
   {
      /* add the parity bit */
      for ( j = 0, parityBit = 0; j < 7; j++ )
      {
         parityBit = parityBit ^ (message[i] >> j);
      }
      message[i] |= (parityBit << 7);

      /* update the checksum */
      *checksum = classJCLIDChecksum( message[i], *checksum );
   }

   /* copy message to VRG_UINT16 array */
   copyMessage( dest, (const unsigned char*)message, numbytes );
}
#endif

/*
*****************************************************************************
** FUNCTION:   hdspClassParseStr
**
** PURPOSE:    This is a helper function that parses a single caller-id string
**             into it's component fields (date, number, name). The format of
**             the input string is given below.
**
** PARAMETERS: clidStr  (in) String containing caller-id info that is to be parsed
**             country  (in) country type
**             clidInfo (out) Parsed caller-id state (date, number, name)
**
** RETURNS:    HAPISUCCESS on success, else error code
**
** NOTE: The caller-id string is of the format (time, number, name). Each of the
**       three fields are optional, however each of the commas will always be
**       included.
**
**       - The time parameter is coded as "MM/DD/HH/MM", where MM is a two-digit
**       value for Month between 01 and 12, DD is a two-digit value for Day between
**       1 and 31, and Hour and Minute are two-digit values coded according to
**       military local time, e.g., 00 is midnight, 01 is 1 a.m., and 13 is 1 p.m.
**
**       - The number parameter is coded as an ASCII character string of decimal
**       digits that identify the calling line number. White spaces are permitted
**       if the string is quoted, however they will be ignored.
**
**       - The name parameter is coded as a string of ASCII characters that identify
**       the calling line name. White spaces, commas, and parenthases are permitted
**       if the string is quoted. A "P" in the number or name field is used to
**       indicate a private number or name, and an "O" is used to indicate an
**       unavailable number or name. The following example illustrates a caller-id
**       string:
**
**       "10/14/17/26, "555 1212", CableLabs"
*****************************************************************************
*/
VRG_SINT16 hdspClassParseStr
(
   const char       *clidStr,
   VRG_COUNTRY       country,
   CLASS_CLID_STATE *clidInfo
)
{
   char         message[128];
   char        *pName = NULL;
   char        *pTime = NULL;
   char        *pPhone = NULL;
   unsigned int i, j;
   char         blockReason;

   clidInfo->reasonNoName = '\0';
   clidInfo->reasonNoNumber = '\0';

   /* catch null pointer */
   if(clidStr == NULL)
   {
      clidInfo->name[0] ='\0';
      return ( HAPIFAILURE );
   }

   strncpy( message, clidStr, 128 );
   message[127] = '\0'; /* insurance */

   /* Get time */
   pTime = clidInfo->date;
   for ( i = 0, j = 0; (i < strlen(message)) && ( j < CLASS_DATE_SIZE ); i++ )
   {
      if ( ( message[i] == ',' ) || ( message[i] == '\0' ) )
      {
         break;
      }

      if ( ( message[i] != '/' ) && ( j < CLASS_DATE_SIZE ) )
      {
         pTime[j++] = message[i];
      }
   }
   pTime[j] = '\0';

   /* Get number */
   pPhone = strchr( message, ',' );
   if( pPhone != NULL )
   {
      pPhone++;

      /* Skip past any leading spaces */
      while (*pPhone == ' ')
      {
         pPhone++;
      }

      /* Check if this is a quoted string */
      if (*pPhone == '"')
      {
         /* remove the starting quote */
         pPhone++;

         /* Get the string up to the ending quote. */
         for(i = 0;
             ( i < strlen(pPhone) ) &&
             ( i < CLASS_MAX_NUMBER_SIZE ) &&
             ( pPhone[i] != '"' );
             i++ );
      }
      else
      {
         /* Get up to the first invalid char (including the comma). */
         for(i = 0;
             ( i < strlen(pPhone) ) &&
             ( i < CLASS_MAX_NUMBER_SIZE ) &&
             ( isValidClidChar( pPhone[i] ));
             i++ );
      }

      /* Find the separating comma to start the name before ending the
       * number. */
      pName = strchr( &pPhone[i], ',' );
      pPhone[i] = '\0';

      /* Get the name information */
      if( pName != NULL )
      {
         pName++;

         /* Skip past any leading spaces */
         while (*pName == ' ')
         {
            pName++;
         }

         /* Check if this is a quoted string */
         if (*pName == '"')
         {
            /* remove the starting quote */
            pName++;

            /* Caller ID names of length 17 chars or greater are truncated */
            for(i = 0;
                ( i < CLASS_MAX_NAME_SIZE ) &&
                ( i < strlen(pName) ) &&
                ( pName[i] != '"');
                i++ );

            /* Remove trailing white spaces from the parsed Caller ID name */
            for( i--; (i != 0) && (pName[i] == ' '); i-- );
            i++; /* Advance back to the first trailing space char. */
         }
         /* if the string is not quoted, the caller ID name
            contains only the first set of characters (i.e.,
            only numbers and characters - no white spaces,
            commas, or parenthases are allowed) */
         else
         {
            /* Caller ID names of length 17 chars or greater are truncated */
            for(i = 0;
                ( i < CLASS_MAX_NAME_SIZE ) &&
                ( i < strlen(pName) ) &&
                ( isValidClidChar( pName[i] ));
                i++);
         }

         /* Remove the trailing white spaces and ending quote (if available ) */
         pName[i] = '\0';
      }
   }

   /* Store the date */
   strncpy( clidInfo->date, pTime, CLASS_DATE_SIZE );

   /* Check if the number is blocked and get reason */
   if ( isClidBlocked( pPhone, country, &blockReason ))
   {
      /* Do not display caller-id number */
      clidInfo->reasonNoNumber = blockReason;
      clidInfo->number[0] = '\0';
   }
   else
   {
      /* Remove any non-digits up to CLASS_MAX_NUMBER_SIZE */
      for(i = 0, j = 0;
          ( i < strlen(pPhone) ) &&
          ( j < CLASS_MAX_NUMBER_SIZE );
          i++ )
      {
         if( isdigit( (int)pPhone[i] ) )
         {
            pPhone[j++] = pPhone[i];
         }
      }
      pPhone[j] = '\0';
      strncpy( clidInfo->number, pPhone, CLASS_MAX_NUMBER_SIZE );
   }

   /* Check if the name is blocked and get reason */
   if ( isClidBlocked( pName, country, &blockReason ))
   {
      /* Do not display caller-id name */
      clidInfo->reasonNoName = blockReason;
      clidInfo->name[0] = '\0';
   }
   else
   {
      strncpy( clidInfo->name, pName, CLASS_MAX_NAME_SIZE );
   }
#if CLASS_DEBUG
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "HDSP: Parsing String Complete:" ));
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "HDSP: clidInfo->date = %s", clidInfo->date ));
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "HDSP: clidInfo->name = %s", clidInfo->name ));
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "HDSP: clidInfo->number = %s", clidInfo->number ));
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "HDSP: clidInfo->reasonNoName = %c", clidInfo->reasonNoName ));
   VRG_LOG_DBG(( VRG_LOG_MOD_HDSP, "HDSP: clidInfo->reasonNoNumber = %c", clidInfo->reasonNoNumber ));
#endif /* CLASS_DEBUG */
   return ( HAPISUCCESS );
}


/*
*****************************************************************************
** FUNCTION:   isValidClidChar
**
** PURPOSE:    To determine if a character is a valid caller id character for
**             an un-quoted caller id 'name'.
**
** PARAMETERS: c  (in) A character within the caller-id name string
**
** RETURNS:    VRG_BOOL
**
** NOTE:       All visible ASCII characters from 0x21-0x7e are valid except for
**             double-quote ["], open/close parentheses [(] and [)], comma [,]
**             and equals-sign [=].
**             If the name is quoted, the above characters would be allowed,
**             and this function does not need to be called.
**
*****************************************************************************
*/
static VRG_BOOL isValidClidChar( char c )
{
   /* Valid characters allowed are in the visible ASCII range from 0x21-0x7e
    * except for double-quote, open/close parentheses, comma, equals-sign. */
   if (( c < 0x21 ) ||
       ( c > 0x7e ) ||
       ( c == '"' ) ||
       ( c == '(' ) ||
       ( c == ')' ) ||
       ( c == ',' ) ||
       ( c == '=' ))
   {
      return ( VRG_FALSE );
   }
   else
   {
      return ( VRG_TRUE );
   }
}


/*
*****************************************************************************
** FUNCTION:   isClidBlocked
**
** PURPOSE:    To determine if the caller id name or number should be blocked,
**             i.e. not displayed on the caller-id receiver display.
**
** PARAMETERS: clidStr     (in) Caller-id name or number string
**             country     (in) Country type (North America, Japan, etc.)
**             blockReason (out) If blocked, this indicates the reason, see below
**
** RETURNS:    VRG_BOOL
**
** NOTE:       For North America, "P" (private) indicates that the name is
**             blocked by the sender, "O" indicates out-of-area or unavailable.
**
**             For Japan, "P" (private) indicates that the name is
**             blocked by the sender, "O" indicates out-of-area or unavailable,
**             "C" indicates a call from a public (pay) phone, and "S" indicates
**             a service conflict.
**
*****************************************************************************
*/
static VRG_BOOL isClidBlocked
(
   const char    *clidStr,
   VRG_COUNTRY    country,
   char          *blockReason
)
{
   char  blockCode;

   *blockReason = '\0';

   if ( (clidStr == NULL) || (strlen( clidStr ) == 0) )
   {
      /* Caller-id string is empty, return unavailable code. */
      *blockReason = 'O';
      return ( VRG_TRUE );
   }
   else if( strlen( clidStr ) == 1 )
   {
      blockCode = *clidStr;

      /*
      ** Check for blocked codes
      */

      switch( country )
      {
#if VRG_COUNTRY_CFG_JAPAN
         case VRG_COUNTRY_JAPAN:
         {
            /*
            ** Japan.
            */
            if ( IS_BLOCKED_JAPAN( blockCode ) )
            {
               *blockReason = blockCode;
               return ( VRG_TRUE );
            }
         }
         break;
#endif /* VRG_COUNTRY_CFG_JAPAN */

#if VRG_COUNTRY_CFG_TAIWAN
         case VRG_COUNTRY_TAIWAN:
         {
            /*
            ** Taiwan.
            */
            if ( IS_BLOCKED_TAIWAN( blockCode ) )
            {
               *blockReason = blockCode;
               return ( VRG_TRUE );
            }
         }
         break;
#endif /* VRG_COUNTRY_CFG_TAIWAN */

#if VRG_COUNTRY_CFG_DENMARK
         case VRG_COUNTRY_DENMARK:
         {
            /*
            ** Denmark.
            */
            if ( IS_BLOCKED_DENMARK( blockCode ) )
            {
               *blockReason = blockCode;
               return ( VRG_TRUE );
            }
         }
         break;
#endif /* VRG_COUNTRY_CFG_DENMARK */

         default:
         {
            /*
            ** North America and Europe.
            */

            if ( IS_BLOCKED_DEFAULT( blockCode ) )
            {
               *blockReason = blockCode;
               return ( VRG_TRUE );
            }
         }
         break;
      } /* end switch */
   }

   return ( VRG_FALSE );
}


#ifdef HDSP_CFG_CLIDRX
/*
*****************************************************************************
** FUNCTION:   classRxConfigure
**
** PURPOSE:    This function sets the CLIDRX Service registers based on
**             the country code.
**
** PARAMETERS: vhdHdl      - the vhd handle for VHD with CLIDRX Service
**             country     - country code
**
** RETURNS:    HAPISUCCESS on success
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 classRxConfigure
(
   VRG_UINT16     vhdHdl,
   CLASS_MSG_TYPE classtype,
   VRG_COUNTRY    country
)
{
   HSZCLIDRXREGS regs;
   VRG_SINT16 rc = HAPIFAILURE;

   XCHG_ASSERT( country < VRG_COUNTRY_MAX );

   if ( classtype == CLASS_MSG_ONHOOK_CLID )
   {
      switch ( country )
      {
#if VRG_COUNTRY_CFG_UK
         case VRG_COUNTRY_UK:
         {
            memset( &regs, 0, sizeof( HSZCLIDRXREGS ) );
            regs.hsxminseizurelen = 96;
            regs.hsxminmarklen = 55;
            regs.hsxmaxmsglen = 255;      /* default */
            regs.hsxminrxthreshold = -35; /* default (dB) */
            rc = hdspSendCmdData( vhdHdl,
                                  HAPI_CLIDRX_SETREGS_CMD,
                                  HSCMDEXTDATA_SYNC,
                                  sizeof( HSZCLIDRXREGS ),
                                  &regs );
         }
         break;
#endif /* VRG_COUNTRY_CFG_UK */

#if VRG_COUNTRY_CFG_JAPAN
         case VRG_COUNTRY_JAPAN:
         {
            memset( &regs, 0, sizeof( HSZCLIDRXREGS ) );
            regs.hsxminseizurelen = 0;    /* no seizure bits */
            regs.hsxminmarklen = 72 * 9 / 10; /* 90% of TX setting */
            regs.hsxmaxmsglen = 255;      /* default */
            regs.hsxminrxthreshold = -35; /* default (dB) */
            rc = hdspSendCmdData( vhdHdl,
                                  HAPI_CLIDRX_SETREGS_CMD,
                                  HSCMDEXTDATA_SYNC,
                                  sizeof( HSZCLIDRXREGS ),
                                  &regs );
         }
         break;
#endif /* VRG_COUNTRY_CFG_JAPAN */

         default:
         {
            memset( &regs, 0, sizeof( HSZCLIDRXREGS ) );
            regs.hsxminseizurelen = 250;  /* default */
            regs.hsxminmarklen = 150;     /* default */
            regs.hsxmaxmsglen = 255;      /* default */
            regs.hsxminrxthreshold = -35; /* default (dB) */
            rc = hdspSendCmdData( vhdHdl,
                                     HAPI_CLIDRX_SETREGS_CMD,
                                     HSCMDEXTDATA_SYNC,
                                     sizeof( HSZCLIDRXREGS ),
                                     &regs );
         }
         break;
      }
   }
   else if ( classtype == CLASS_MSG_OFFHOOK_CLID )
   {
      /* Type 2 (off-hook) caller id */
      memset( &regs, 0, sizeof( HSZCLIDRXREGS ) );
      regs.hsxminseizurelen = 0;    /* no seizure bits */
      regs.hsxminmarklen = 80;
      regs.hsxmaxmsglen = 255;      /* default */
      regs.hsxminrxthreshold = -35; /* default (dB) */
      rc = hdspSendCmdData( vhdHdl,
                            HAPI_CLIDRX_SETREGS_CMD,
                            HSCMDEXTDATA_SYNC,
                            sizeof( HSZCLIDRXREGS ),
                            &regs );
   }
   else
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, "ERROR: Incorrect class type for RX caller id "));
   }

   return( rc );
}

/*
*****************************************************************************
** FUNCTION:   hdspClidRxParse
**
** PURPOSE:    Disassembles North American CLIDRX packets.
**
** PARAMETERS: clidRxPktp (IN) - the current CLIDRX packet (may be several in a CLIDRX session)
**             clidRxParamp (OUT) - the extracted CLIDRX info
**
** RETURNS:    TRUE - complete CLIDRX transmission disassembly (END packet found)
**             FALSE - incomplete CLIDRX transmission disassembly
**
** NOTE:
**
*****************************************************************************
*/
VRG_BOOL hdspClidRxParse( VRG_COUNTRY country, HSZCLIDRXPACKET *clidRxPktp, HDSP_CLASS_CLIDRX_STATE *clidRxStatep )
{
   VRG_BOOL valid;

   if ( classRxDepacketize( clidRxPktp, clidRxStatep ) )
   {
      /* parse received message */
#if VRG_COUNTRY_CFG_JAPAN
      if ( country == VRG_COUNTRY_JAPAN )
      {
         valid = disAssembleJapanClidRx( clidRxStatep );
      }
      else
#endif
      {
         valid = disAssembleCLASS( country, clidRxStatep );
      }

      if (valid)
      {
         /* print clidrx data to console */
         dispCallerID( clidRxStatep );

         return VRG_TRUE;
      }
   }
   return VRG_FALSE;
}

/*
*****************************************************************************
** FUNCTION:   classRxDepacketize
**
** PURPOSE:    Disassembles CLIDRX packets.
**
** PARAMETERS: clidRxPktp (IN) - the current CLIDRX packet (may be several in a CLIDRX session)
**             clidRxParamp (OUT) - the extracted CLIDRX info
**
** RETURNS:    TRUE - complete CLIDRX transmission disassembly (END packet found)
**             FALSE - incomplete CLIDRX transmission disassembly
**
** NOTE:
**
*****************************************************************************
*/
VRG_BOOL classRxDepacketize( HSZCLIDRXPACKET *clidRxPktp, HDSP_CLASS_CLIDRX_STATE *clidRxStatep )
{
   VRG_UINT16 ploadLen;

   if ( ( clidRxPktp->hsxClassCode ) == CLIDRX_PKT_START )
   {
      /* Initialize global variables */
     clidRxStatep->numSeizure = clidRxPktp->hsxClassMsgWord0;
     clidRxStatep->numMark = clidRxPktp->hsxClassMsgWord1;
     clidRxStatep->msgLen = 0;
     memset( &clidRxStatep->clidRxData, 0, sizeof(clidRxStatep->clidRxData) );
     memset( clidRxStatep->parseBuf, 0, sizeof(clidRxStatep->parseBuf) );

     /* Set-up for copying */
     clidRxStatep->parseBufp = &clidRxStatep->parseBuf[0];
   }
   else if ( ( clidRxPktp->hsxClassCode ) == CLIDRX_PKT_BODY )
   {

      // Get Payload Length in bytes.
      ploadLen = clidRxPktp->hsxClassMsgWord0;

      if (clidRxStatep->parseBufp + ploadLen >= &clidRxStatep->parseBuf[CLASS_STRING_SIZE])
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  " ERROR: Caller ID receive parse buffer overflow"));
         return VRG_FALSE;
      }

      /* Copy to the Parsing buffer. Take an extra byte in case the count is odd. */
      memcpy( clidRxStatep->parseBufp, clidRxPktp->hsxClassMsg, ploadLen + 1);
      classByteOrder( clidRxStatep->parseBufp, ploadLen );
      clidRxStatep->parseBufp += (ploadLen) ;
      clidRxStatep->msgLen += ploadLen;

   }
   else if ( ( clidRxPktp->hsxClassCode ) == CLIDRX_PKT_END )
   {
      ploadLen = clidRxPktp->hsxClassMsgWord0;

      if (clidRxStatep->parseBufp + ploadLen >= &clidRxStatep->parseBuf[CLASS_STRING_SIZE])
      {
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  " ERROR: Caller ID receive parse buffer overflow"));
         return VRG_FALSE;
      }

      /* Copy to the Parsing buffer. Take an extra byte in case the count is odd */
      memcpy( clidRxStatep->parseBufp, clidRxPktp->hsxClassMsg, ploadLen + 1);
      classByteOrder( clidRxStatep->parseBufp, ploadLen );
      clidRxStatep->msgLen += ploadLen;

      return VRG_TRUE; /* indicate finished */
   }
   else
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP, " ERROR: Unknown CLASS Payload Code "));
   }

   return VRG_FALSE; /* indicate not finished */
}

#if VRG_COUNTRY_CFG_JAPAN
static VRG_BOOL checkParityBit( VRG_UINT16 numByte, VRG_UINT16 *checksump, VRG_UINT8 *msgp )
{
   int i, j;
   int parityBit;

   for ( i = 0; i < numByte; i++ )
   {
      /* update the checksum */
      *checksump = classJCLIDChecksum( msgp[ i ], *checksump );

      /* generate the parity bit */
      for ( j = 0, parityBit = 0; j < 7; j++ )
      {
         parityBit = parityBit ^ ( ( msgp[ i ] & 0x7F ) >> j);
      }

      /* compare against received parity */
      if ( ( parityBit & 0x01 ) != ( ( msgp[ i ] & 0x80 ) >> 7 ) )
      {
         /* strip polarity bit */
         msgp[ i ] &= 0x7F;
         VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "checkParityBit ERROR!!!" ));
         return VRG_FALSE;
      }

      /* strip parity bit */
      msgp[ i ] &= 0x7F;
   }
   return VRG_TRUE;
}
#endif

#if VRG_COUNTRY_CFG_JAPAN
/*
*****************************************************************************
** FUNCTION:   disAssembleJapanClidRx
**
** PURPOSE:    Disassembles Japan CLIDRX packets.
**
** PARAMETERS: gClassInfop (IN/OUT)
**
** RETURNS:    TRUE - complete CLIDRX transmission disassembly
**             FALSE - incomplete/errored CLIDRX transmission disassembly
**
** NOTE:
**
*****************************************************************************
*/
static VRG_BOOL disAssembleJapanClidRx( HDSP_CLASS_CLIDRX_STATE *clidRxStatep )
{
   unsigned char *msgWordp;
   VRG_UINT16 msgLen, numLen;
   VRG_UINT16 checksum = 0;

   /* strip polarity bit from header */
   msgWordp = &clidRxStatep->parseBuf[0];
   if ( !checkParityBit( CLASS_JAPAN_HEADER_SIZE_BYTES, &checksum, ( VRG_UINT8 * )msgWordp ) )
   {
      return VRG_FALSE;
   }

   /* find service type byte */
   msgWordp = (unsigned char*) strchr( (char*)msgWordp, CLASS_SRVTYP_JP );
   if ( msgWordp == NULL )
   {
      /* if not found, abandon */
      return VRG_FALSE;
   }

   /* extract message length, strip polarity bit from remainder of message */
   msgWordp++;
   checksum = 0; /* reset checksum - begin checksum at CLASS_SRVTYP_JP */
   if ( !checkParityBit( 1,  &checksum, ( VRG_UINT8 * )msgWordp ) )
   {
      return VRG_FALSE;
   }

   if ( *msgWordp == CLASS_DLE )
   {
      /* if the message length is 16 there is a DLE prepending message length...skip over */
      msgWordp++;
      if ( !checkParityBit( 1, &checksum, ( VRG_UINT8 * )msgWordp ) )
      {
         return VRG_FALSE;
      }
   }
   msgLen = *msgWordp;
   if ( !checkParityBit( msgLen, &checksum, ( VRG_UINT8 * )msgWordp ) )
   {
      return VRG_FALSE;
   }

   /* handle the calling number extraction */
   msgWordp++;
   if ( *msgWordp == CLASS_NOID )
   {
      /* handle "no-number" reason field */
      msgWordp++;
      clidRxStatep->clidRxData.reasonNoNumber = *msgWordp++; /* step over NULL character */
   }
   else
   {
      msgWordp++;

      /* if the number length is 16 there is a DLE prepending number length...skip over */
      if ( *msgWordp == CLASS_DLE )
      {
         msgWordp++;
      }
      numLen = *msgWordp;
      msgWordp++;

      /* extract calling number info now that the length is known */
      memcpy( clidRxStatep->clidRxData.number, msgWordp, numLen );
      clidRxStatep->clidRxData.number[ numLen ] = '\0';
   }

   msgWordp += numLen + 2; /* skip over number + DLE + ETX */

#if 0
   /* bit-reverse checksum */
   for ( i = 0, brChecksum = 0; i < 16; i++ )
   {
      brChecksum = brChecksum | ((VRG_UINT16) ((( checksum >> i ) & 1 ) << ( 15 - i )));
   }

   VRG_LOG_DBG((VRG_LOD_MOD_HDSP,  "brChecksum=%x, rcvChecksum=%x", brChecksum, ( ( *msgWordp << 8 ) + ( *( msgWordp++ ) ) ) ) );

   return ( brChecksum == ( ( *msgWordp << 8 ) + ( *( msgWordp++ ) ) ) );
#else
   return VRG_TRUE;
#endif
}
#endif

/* Reverse the byte order prior to parsing */
static void classByteOrder( unsigned char *bufp, VRG_UINT16 length )
{
#if BOS_CFG_BIG_ENDIAN
   unsigned char temp;
   VRG_UINT16 i;

   for ( i = 0; i <= length; i++ )
   {
      temp = bufp[i];
      bufp[i] = bufp[i+1];
      bufp[i+1] = temp;
      i++;
   }
#endif
}

/* Takes the CLIDRX message, extracted from the body and end packets, and extracts
   the date, number and name string */
static VRG_BOOL disAssembleCLASS( VRG_COUNTRY country, HDSP_CLASS_CLIDRX_STATE *clidRxStatep )
{
   unsigned char *msgWordp;
   VRG_UINT16 mlen, plen, offset;
   VRG_UINT16 mtype, ptype;
   VRG_UINT16 checksum = 0;

   msgWordp = &clidRxStatep->parseBuf[0];

   /* Get the message header */
   mtype =  msgWordp[0];
   mlen =  msgWordp[1];

   if ( mlen >= CLASS_STRING_SIZE )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "Invalid Caller ID message length byte: %d", mlen ));
      return VRG_FALSE;
   }

   /* Validate the checksum */
   for (offset = 0; offset < mlen + 3; offset++)
   {
      VRG_LOG_DBG((VRG_LOG_MOD_HDSP, " DEBUG CLID[%d] = 0x%x",offset,msgWordp[offset]));
      checksum += msgWordp[offset];
   }
   checksum = checksum % 256;

   if (checksum != 0)
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_HDSP,  "Invalid Caller ID checksum." ));
      return VRG_FALSE;
   }

   switch (msgWordp[0])
   {
      case CLASS_SDMF:
      {
         /* Single Data Message Format includes timestamp and number.
         ** Number may be blocked or out-of-area/unavailable  */
         /* Get the date/time */
         memcpy( clidRxStatep->clidRxData.date, &msgWordp[2], 8 );
         clidRxStatep->clidRxData.date[8] = 0;

         /* Get the  Number */
         plen = mlen - 8;
         if (plen > CLASS_MAX_NUMBER_SIZE)
         {
            plen = CLASS_MAX_NUMBER_SIZE;
         }

         switch (msgWordp[10])
         {
            case 'P':
            {
               strcpy(clidRxStatep->clidRxData.number, "Private");
               clidRxStatep->clidRxData.reasonNoNumber = msgWordp[10];
            }
            break;

            case 'O':
            {
#if VRG_COUNTRY_CFG_TAIWAN
               if( country == VRG_COUNTRY_TAIWAN )
               {
                  strcpy(clidRxStatep->clidRxData.number, "Out Of Area");
               }
               else
#else
               {
                  strcpy(clidRxStatep->clidRxData.number, "Unavailable");
               }
#endif
               clidRxStatep->clidRxData.reasonNoNumber = msgWordp[10];
            }
            break;

#if VRG_COUNTRY_CFG_TAIWAN
            case 'I':
            {
               if( country == VRG_COUNTRY_TAIWAN )
               {
                  strcpy(clidRxStatep->clidRxData.number, "International");
                  clidRxStatep->clidRxData.reasonNoNumber = msgWordp[10];
               }
            }
            break;

            case 'C':
            {
               if( country == VRG_COUNTRY_TAIWAN )
               {
                  strcpy(clidRxStatep->clidRxData.number, "Coin/Public");
                  clidRxStatep->clidRxData.reasonNoNumber = msgWordp[10];
               }
            }
            break;
#endif

            default:
            {
               memcpy( clidRxStatep->clidRxData.number, &msgWordp[10], plen );
               clidRxStatep->clidRxData.number[plen] = 0;
            }
            break;
         }
      }
      break;

      case VMWI_MDMF:
      case CLASS_MDMF:
      {
         /* Multiple data message format includes various parameters, typically
         ** datestamp, number, name, and call type. Number and/or name may be
         ** blocked or out of area/unavailable. VMWI MDMF is similar, so can be
         ** parsed in the same block, with just a few more parms to look for
         ** if needed.
         */
         offset = 2;

         while (offset - 1 < mlen)
         {
            ptype  =  msgWordp[offset ];
            plen =  msgWordp[offset + 1];

            if (offset + plen > mlen)
            {
               break;
            }

            switch ( ptype )
            {
               case CLASS_TIME:
               {
                  memcpy( clidRxStatep->clidRxData.date, &msgWordp[offset + 2], 8 );
                  clidRxStatep->clidRxData.date[8] = 0;
               }
               break;

               case CLASS_ID:
               {
                  if (plen > CLASS_MAX_NUMBER_SIZE)
                  {
                     plen = CLASS_MAX_NUMBER_SIZE;
                  }
                  memcpy( clidRxStatep->clidRxData.number, &msgWordp[offset + 2], plen );
                  clidRxStatep->clidRxData.number[plen] = 0;
               }
               break;

               case CLASS_DDN:
               {
#if VRG_COUNTRY_CFG_NORTH_AMERICA
                  /* In Canada, this will be the calling number to use to return a call,
                     as it is defined as 0x03 instead of 0x02, as is used in the USA. This
                     needs to be seperated from other countries as 0x03 is used for other
                     parameters outside of Canada, and may conflict with 0x02. An example of
                     this is in ETSI 300-659 03, where 0x02 is the Calling Line Identity and
                     0x03 is the Called Line Identity, thus we will store the wrong number if
                     we assume 0x02 and 0x03 are the same thing. */
                  if( country == VRG_COUNTRY_NORTH_AMERICA )
                  {
                     if (plen > CLASS_MAX_NUMBER_SIZE)
                     {
                        plen = CLASS_MAX_NUMBER_SIZE;
                     }
                     memcpy( clidRxStatep->clidRxData.number, &msgWordp[offset + 2], plen );
                     clidRxStatep->clidRxData.number[plen] = 0;
                  }
#endif
               }
               break;

               case CLASS_NOID:
               {
                  fillClidRxString(country, msgWordp[offset + 2], clidRxStatep->clidRxData.number);
                  clidRxStatep->clidRxData.reasonNoNumber = msgWordp[offset + 2];
               }
               break;

               case CLASS_NAME:
               {
                  if (plen > CLASS_MAX_NAME_SIZE)
                  {
                     plen = CLASS_MAX_NAME_SIZE;
                  }
                  memcpy( clidRxStatep->clidRxData.name, &msgWordp[offset + 2], plen );
                  clidRxStatep->clidRxData.name[plen] = 0;
               }
               break;

               case CLASS_NONAME:
               {
                  fillClidRxString(country, msgWordp[offset + 2], clidRxStatep->clidRxData.name);
                  clidRxStatep->clidRxData.reasonNoName = msgWordp[offset + 2];
               }
               break;

               case CLASS_CALLTYPE:
               {
                  /* Do nothing */
               }
               break;

               case VMWI_HEADER:
               {
                  /* Do nothing */
               }
               break;

               default:
               {
                  VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "Unrecognized Caller ID parameter received: %d", ptype ));
               }
               break;
            }

            /* Advance to next parameter */
            offset += msgWordp[offset + 1] + 2;
         }
      }
      break;

      default:
      {
         VRG_LOG_DBG((VRG_LOG_MOD_HDSP,  "Unknown Caller ID message format received: %d", mtype ));
         return VRG_FALSE;
      }
      break;
   }
   return VRG_TRUE;
}


static void dispCallerID( HDSP_CLASS_CLIDRX_STATE *clidRxStatep )
{

    /* Print out the received class message */
    VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "-----------------------------------------------------"));
    VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "INCOMING CALL:"));
    VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "Mark = %d; Seizure = %d ", clidRxStatep->numMark, clidRxStatep->numSeizure ));

    VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "Time Field (MONTH, DAY, HOUR, MIN): %s", clidRxStatep->clidRxData.date));
    VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "Number Field (up to 20 digits): %s", clidRxStatep->clidRxData.number));
    VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "Name Field (up to 15 characters): %s", clidRxStatep->clidRxData.name));
    VRG_LOG_DBG((VRG_LOG_MOD_HDSP, "-----------------------------------------------------"));
}

static void fillClidRxString(VRG_COUNTRY country, unsigned char key, char * pClidRxString)
{
   switch (key)
   {
      case 'P':
      {
         strcpy(pClidRxString, "Private");
      }
      break;

#if VRG_COUNTRY_CFG_TAIWAN
      case '0':
      {
         if ( country == VRG_COUNTRY_TAIWAN )
         {
            strcpy(pClidRxString, "Out Of Area");
         }
      }
      break;

      case 'I':
      {
         if ( country == VRG_COUNTRY_TAIWAN )
         {
            strcpy(pClidRxString, "International");
         }
      }
      break;

      case 'C':
      {
         if ( country == VRG_COUNTRY_TAIWAN )
         {
            strcpy(pClidRxString, "Coin/Public");
         }
      }
      break;
#endif

      default:
      {
         strcpy(pClidRxString, "Unavailable");
      }
      break;
   }
}

#endif /* HDSP_CFG_CLIDRX */

/*
*****************************************************************************
** FUNCTION:   hdspClassTransmit
**
** PURPOSE:    This function starts the class transmitter, transmitting a
**             message previously queued with hdspClassMessage
**
** PARAMETERS: vhdHdl - the vhd handle
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspClassTransmit( VRG_UINT16 vhdHdl )
{
   /* Send command, ignoring result because it will block the endpoint callback otherwise */
   return( hdspSendCmdAsync( vhdHdl, HAPI_CLIDTX_START, HSCMDDATA, VRG_TRUE, 0 ));
}

/*
*****************************************************************************
** FUNCTION:   hdspClassStopTransmit
**
** PURPOSE:    This function stops the class transmitter which flushes
**             its tx queue of any previous CLASS packets
**
** PARAMETERS: vhdHdl - the vhd handle
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspClassStopTransmit( VRG_UINT16 vhdHdl )
{
   VRG_SINT16 status;

   /* Call HDSP function to stop the DTMF transmitter */
   status = hdspClassDtmfStopTransmit( vhdHdl );
   if ( status )
   {
      return( status );
   }

   /* Call HDSP function to stop the CLASS transmitter */
   return( hdspSendCmd( vhdHdl, HAPI_CLIDTX_START, HSCMDDATA, VRG_FALSE, 0 ));
}

/*
*****************************************************************************
** FUNCTION:   hdspClassDtmfTransmit
**
** PURPOSE:    This function starts the DTMF transmitter, transmitting a
**             message contained in the class state
**
** PARAMETERS: vhdHdl    - the vhd handle
**             clidInfo  - pointer to caller ID message information
**             dtmfParmp - pointer to international DTMF parameters
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspClassDtmfTransmit( VRG_UINT16 vhdHdl, CLASS_CLID_STATE *clidInfo, CLASS_MSG_DTMF_PARMS *dtmfParmp )
{
   HAPI_PTE_DIALSTRING dialstring;
   char digit = '\0';
   char *pNumber;
   unsigned int i,len;

   memset(&dialstring, 0, sizeof(HAPI_PTE_DIALSTRING));

   dialstring.dBmLevel = dtmfParmp->dbLevel;             /* dBm power */
   dialstring.encoderType = 0;                           /* 0=DTMF, 1=MFR1, 2=MFR2F, 3=MFR2B */
   dialstring.pauseTime = dtmfParmp->pulseTime * 8;      /* Generate pauses with 70 ms */
   dialstring.pulseOffTime = dtmfParmp->pulseTime * 8;   /* Off times of 70 ms */
   dialstring.pulseOnTime = dtmfParmp->pulseTime * 8;    /* On times of 70 ms */

   /* Process the number field and get the length */
   switch( toupper(clidInfo->reasonNoNumber) )
   {
      case 'P':
      {
         /* Number will be blocked - private */

         pNumber = (char*)&dialstring.digitArray[0];

         /* Copy in the private code */
         i=0;
         while (dtmfParmp->privateCode[i] != HAPI_PTE_DTMFDDGTERROR)
         {
            pNumber[i] = dtmfParmp->privateCode[i];
            if ( i> HAPI_PTE_GEN_MAX_DIGITS)
            {
               VRG_LOG_WARN((VRG_LOG_MOD_HDSP, "HDSP Class WARNING: Incorrect PRIVATE dtmf code !!! "));
               return ( HAPIFAILURE );
            }
            i++;
         }

         /* Number of digits to transmit */
         dialstring.numCharacters = i;
         len = i;
      }
      break;

      case 'O':
      {
         /* Number will be blocked - unavailable */

         pNumber = (char*)&dialstring.digitArray[0];

         /* Copy in the unavailable code */
         i=0;
         while (dtmfParmp->unavailCode[i] != HAPI_PTE_DTMFDDGTERROR)
         {
            pNumber[i] = dtmfParmp->unavailCode[i];
            if ( i> HAPI_PTE_GEN_MAX_DIGITS)
            {
               VRG_LOG_WARN((VRG_LOG_MOD_HDSP, "HDSP Class WARNING: Incorrect UNAVAILABLE dtmf code !!! "));
               break;;
            }
            i++;
         }

         /* Number of digits to transmit */
         dialstring.numCharacters = i;
         len = i;
      }
      break;

      case 'E':
      {
         /* Used by Denmark. Should only be in the reasonNoNumber field if the country
            is set to Denmark and we recieved a DTMF CLID of "D3#" */
         /* Number will be blocked - error */

         pNumber = (char*)&dialstring.digitArray[0];

         /* Copy in the error code */
         i=0;
         while (dtmfParmp->errorCode[i] != HAPI_PTE_DTMFDDGTERROR)
         {
            pNumber[i] = dtmfParmp->errorCode[i];
            if ( i> HAPI_PTE_GEN_MAX_DIGITS)
            {
               VRG_LOG_WARN((VRG_LOG_MOD_HDSP, "HDSP Class WARNING: Incorrect ERROR dtmf code !!! "));
               break;;
            }
            i++;
         }

         /* Number of digits to transmit */
         dialstring.numCharacters = i;
         len = i;
      }
      break;

      default:
      {
         /* Number will not be blocked */
         pNumber = (char*)&dialstring.digitArray[0];
         pNumber[0] = dtmfParmp->startCode;            /* Fill in DTMF start code */

         /* Get the length of the number */
         len = strlen(clidInfo->number);

         if ( len > HAPI_PTE_GEN_MAX_DIGITS)
         {
            VRG_LOG_WARN((VRG_LOG_MOD_HDSP, "HDSP Class WARNING: Too many digits (%d) in clid number !!!", len));
            len = HAPI_PTE_GEN_MAX_DIGITS;
         }

         /* Copy telephone number and convert it to integer format */
         for ( i = 0; i < len; i++ )
         {
            digit = clidInfo->number[i];
            switch(digit)
            {
               case 'A': pNumber[i+1] = HAPI_PTE_DTMFDDGTA;   break; /* A */
               case 'B': pNumber[i+1] = HAPI_PTE_DTMFDDGTB;   break; /* B */
               case 'C': pNumber[i+1] = HAPI_PTE_DTMFDDGTC;   break; /* C */
               case 'D': pNumber[i+1] = HAPI_PTE_DTMFDDGTD;   break; /* D */
               case '*': pNumber[i+1] = HAPI_PTE_DTMFDDGTSTR; break; /* * */
               case '#': pNumber[i+1] = HAPI_PTE_DTMFDDGTPND; break; /* # */
               default:  pNumber[i+1] = digit - '0';                 /* Digit */
            }
         }
         /* Stop code */
         pNumber[i+1] = dtmfParmp->endCode;          /* Fill in DTMF end code */

         /* Number of digits to transmit: number+start+stop */
         len = len + 2;
         dialstring.numCharacters = len;
      }
      break;
   } /* end switch */

   /* Send command, ignoring result because it will block the endpoint callback otherwise */
   return ( hdspSendCmdDataAsync( vhdHdl,
                                  HAPI_PTE_DIALSTRING_CMD,
                                  HSCMDEXTDATA_ASYNC,
                                  sizeof(HAPI_PTE_DIALSTRING),
                                  &dialstring ));
}

/*
*****************************************************************************
** FUNCTION:   hdspClassDtmfStopTransmit
**
** PURPOSE:    This function stops the DTMF transmitter
**
** PARAMETERS: vhdHdl - the vhd handle
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspClassDtmfStopTransmit( VRG_UINT16 vhdHdl )
{
   return ( hdspSendCmd( vhdHdl, HAPI_PTE_TONE_END_CMD, HSCMDDATA, 0, 0 ));
}

/*
*****************************************************************************
** FUNCTION:   hdspClassDtmfTransmitVmwi
**
** PURPOSE:    This function starts the DTMF transmitter for VMWI
**
** PARAMETERS: vhdHdl    - the vhd handle
**             vmwiType  - vmwi type to transmit (on or off)
**             dtmfParmp - pointer to international DTMF parameters
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspClassDtmfTransmitVmwi( VRG_UINT16 vhdHdl, CLASS_VMWI_TYPE vmwiType, CLASS_MSG_DTMF_PARMS *dtmfParmp )
{
   HAPI_PTE_DIALSTRING dialstring;
   char *pNumber;

   dialstring.dBmLevel = dtmfParmp->dbLevel;             /* dBm power */
   dialstring.encoderType = 0;                           /* 0=DTMF, 1=MFR1, 2=MFR2F, 3=MFR2B */
   dialstring.pauseTime = dtmfParmp->pulseTime * 8;      /* Generate pauses with 70 ms */
   dialstring.pulseOffTime = dtmfParmp->pulseTime * 8;   /* Off times of 70 ms */
   dialstring.pulseOnTime = dtmfParmp->pulseTime * 8;    /* On times of 70 ms */

   /* Create char pointer to 16 bit digit array */
   pNumber = (char*)&dialstring.digitArray[0];

   if( vmwiType == CLASS_VMWI_ACTIVATE )
   {
      /* Fill in VMWI Activate code */
      pNumber[0] = HAPI_PTE_DTMFDDGTC;
      pNumber[1] = HAPI_PTE_DTMFDDGT0;
      pNumber[2] = HAPI_PTE_DTMFDDGT1;
      pNumber[3] = HAPI_PTE_DTMFDDGTPND;

      /* Get the length of the number */
      dialstring.numCharacters = 4;
   }
   else
   {
      /* Fill in VMWI Activate code */
      pNumber[0] = HAPI_PTE_DTMFDDGTC;
      pNumber[1] = HAPI_PTE_DTMFDDGT0;
      pNumber[2] = HAPI_PTE_DTMFDDGT0;
      pNumber[3] = HAPI_PTE_DTMFDDGTPND;

      /* Get the length of the number */
      dialstring.numCharacters = 4;
   }

   /* Send command, ignoring result because it will block the endpoint callback otherwise */
   return ( hdspSendCmdDataAsync( vhdHdl,
                                  HAPI_PTE_DIALSTRING_CMD,
                                  HSCMDEXTDATA_ASYNC,
                                  sizeof(HAPI_PTE_DIALSTRING),
                                  &dialstring ));
}

/*
*****************************************************************************
** FUNCTION:   copyMessage
**
** PURPOSE:    This function copies a class message, byte swapping and
**             returns a checksum value
**
** PARAMETERS: dst   - the copy destination, VRG_UINT16 addressed,
**             src   - the source string
**             bytes - the string length
**
** RETURNS:    checksum - string checksum value
**
** NOTE:
**
*****************************************************************************
*/
static VRG_UINT16 copyMessage( VRG_UINT16* dst, const unsigned char* src, int bytes )
{
   VRG_UINT16 checksum = 0;
   int i;

   for( i=0; i<bytes; i++ )
   {
      /* pack bytes to words (byte swap) */
      if( i & 0x1 )
      {
         *dst |= (*src << 8 );
         dst++;
      }
      else
      {
         *dst = *src;
      }

      checksum += *src++;
   }

   return checksum;
}

/*
*****************************************************************************
** FUNCTION:   hdspClassVmwi
**
** PURPOSE:    This function assembles a Visual Message Waiting Indicator
**             message and sends it to the host.
**
** PARAMETERS: vhdHdl - the vhd handle
**             type   - VMWI type (see VMWITYPES enum)
**             format - VMWI format (SDMF/MDMF)
**             fskParmp - FSK modulation parameters
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspClassVmwi
(
   VRG_UINT16  vhdHdl,
   int         lineId,
   VRG_UINT16  type,
   VRG_UINT16  format,
   CLASS_MSG_FSK_PARMS *fskParmp,
   VRG_COUNTRY country
)
{
   /*
   ** Configure the CLIDTX registers based on country, and class-type.
   */
   classTxConfigure( vhdHdl, lineId, CLASS_MSG_VMWI, fskParmp, 0 );


   if ( format == VMWI_MODE_MDMF )
   {
      /*
      ** Send VMWI using Multiple Data Message Format (MDMF).
      */
      return ( SendMdmfVmwiMessage( vhdHdl, type, country ) );
   }
   else
   {
      /*
      ** Send VMWI using Simple Data Message Format (SDMF).
      */
      return ( SendSdmfVmwiMessage( vhdHdl, type, country ) );
   }
}


/*
*****************************************************************************
** FUNCTION:   SendSdmfVmwiMessage.
**
** PURPOSE:    This function assembles a Visual Message Waiting Indicator
**             message in Simple Data Message Format (MDMF) and sends it to
**             the DSP.
**
** PARAMETERS: vhdHdl - the vhd handle
**             type - VMWI type (see VMWITYPES enum)
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
static VRG_SINT16 SendSdmfVmwiMessage( VRG_UINT16 vhdHdl, VRG_UINT16 type, VRG_COUNTRY country )
{
   HSZCLASSRELAYPACKET packet;

   VRG_UINT16 checksum = 0;
   VRG_UINT16 msglen = 3 ;

   /* Prepare the class start packet */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_START );
   packet.hsxPacketHdr.payloadLength = 4;                      /* Number of Payload bytes =4 */
   packet.hsxPacketHdr.payloadOffset = sizeof(HAPI_PKT_EGRESS_HDR);
   packet.hsxPacketHdr.payloadTimeStampLow = 0;                /* Timestamp of packet        */

   packet.hsxWord0 = 300;                                      /* # Seizes to send           */
   packet.hsxWord1 = 180;                                      /* # Marks to follow Seize    */

   /* send the class start packet */
   /* send the class start packet */
   hdspSendDataPacket( vhdHdl, &packet );

   /* -------------Prepare the body Packets --------------------------------------- */

   /* Each packet can contain up to CLASS_MESSAGESIZE words */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_BODY );
   packet.hsxPacketHdr.payloadLength = 6 + (msglen+1);         /* Number of bytes = word0 + word1 + ClassMsg[0] + time + id */
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;                /* Timestamp of Packet       */

   packet.hsxWord0 = 2 + msglen;        /* Number of class bytes (Message Type Byte) + (Message Length Byte) + Data Bytes */
   packet.hsxWord1 = 0;                 /* Number of marks to generate after class packet */

   /* Setup Multi Data Message Format (SDMF=0x04) */
   packet.hsxClassMsg[0] = (VRG_UINT16) ((VMWI_SDMF) + ((msglen) <<8));       /* msgtype, msglen */
   checksum += VMWI_SDMF;
   checksum += msglen ;

   if ( type == CLASS_VMWI_ACTIVATE )
   {
      /* copy pointer of message to pointer buffer */
      packet.hsxClassMsg[1] = ( VMWI_SDMF_ACTIVATE ) + ( (VMWI_SDMF_ACTIVATE)<<8 ) ;
      packet.hsxClassMsg[2] = ( VMWI_SDMF_ACTIVATE ) ;
      checksum += (3 * VMWI_SDMF_ACTIVATE );
   }
   else
   {
      /* copy pointer of message to pointer buffer */
      packet.hsxClassMsg[1] = ( VMWI_SDMF_DEACTIVATE ) + ( (VMWI_SDMF_DEACTIVATE)<<8 ) ;
      packet.hsxClassMsg[2] = ( VMWI_SDMF_DEACTIVATE ) ;
      checksum += ( 3 * VMWI_SDMF_DEACTIVATE );
   }

   /* send the class start packet */
   hdspSendDataPacket( vhdHdl, &packet );

   /* Prepare the CLASS end packet */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_END );
   packet.hsxPacketHdr.payloadLength = 6;  /* 6 bytes */
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;

   packet.hsxWord0 = 1;   /* #bytes */
   packet.hsxWord1 = 10; /* mark */
   packet.hsxClassMsg[0] = (VRG_UINT16)((~checksum) + 1);

   /* Send the CLASS end packet*/
   hdspSendDataPacket( vhdHdl, &packet );

   /* Return success */
   return (HAPISUCCESS);
}


/*
*****************************************************************************
** FUNCTION:   SendMdmfVmwiMessage.
**
** PURPOSE:    This function assembles a Visual Message Waiting Indicator
**             message in Multiple Data Message Format (MDMF) and sends it to
**             the DSP.
**
** PARAMETERS: vhdHdl - the vhd handle
**             type - VMWI type (see VMWITYPES enum)
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
static VRG_SINT16 SendMdmfVmwiMessage( VRG_UINT16 vhdHdl, VRG_UINT16 type, VRG_COUNTRY country )
{
   HSZCLASSRELAYPACKET packet;
   VRG_UINT16 checksum = 0;
   VRG_UINT16 msglen = 1;

   /* -------------Prepare the start packet ------------------------------- */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_START );
   packet.hsxPacketHdr.payloadLength = 4;                   /* Number of Payload bytes =4 */
   packet.hsxPacketHdr.payloadOffset = sizeof(HAPI_PKT_EGRESS_HDR);
   packet.hsxPacketHdr.payloadTimeStampLow = 0;                /* Timestamp of packet        */

   XCHG_ASSERT( country < VRG_COUNTRY_MAX );
   switch (country)
   {
#if VRG_COUNTRY_CFG_UK
      case VRG_COUNTRY_UK:
      {
         packet.hsxWord0 = 240;                                       /* # Seizes to send           */
         packet.hsxWord1 = 72;                                       /* # Marks to follow Seize    */
         break;
      }
#endif
      default:
      {
   packet.hsxWord0 = 300;                                      /* # Seizes to send           */
   packet.hsxWord1 = 180;                                      /* # Marks to follow Seize    */
         break;
      }
   }

   /* send the class start packet */
   /* send the class start packet */
   hdspSendDataPacket( vhdHdl, &packet );

   /* -------------Prepare the body packet -------------------------------- */

   /* Each packet can contain up to CLASS_MESSAGESIZE words */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_BODY );
   packet.hsxPacketHdr.payloadLength = 4 + 4 + (msglen+1);     /* Number of bytes = word0 + word1 + class bytes. */
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;                /* Timestamp of Packet       */

   packet.hsxWord0 = 2 + 2 + msglen;    /* Number of class bytes (Message Type Byte) + (Message Length Byte) + Data Bytes */
   packet.hsxWord1 = 0;                 /* Number of marks to generate after class packet */

   /* Setup Multi Data Message Format */
   packet.hsxClassMsg[0] = (VRG_UINT16) ((VMWI_MDMF) + ((msglen + 2) <<8));       /* msgtype, msglen */
   checksum += VMWI_MDMF;
   checksum += ( msglen + 2 );

   /* fill in the parameter header for the date */
   packet.hsxClassMsg[1] = ( VMWI_HEADER ) + (msglen << 8);  /* time param, msglen */
   checksum += VMWI_HEADER;
   checksum += msglen;


   if ( type == CLASS_VMWI_ACTIVATE )
   {
      /* copy pointer of message to pointer buffer */
      packet.hsxClassMsg[2] = VMWI_ACTIVATE;
      checksum += VMWI_ACTIVATE;
   }
   else
   {
      /* copy pointer of message to pointer buffer */
      packet.hsxClassMsg[2] = VMWI_DEACTIVATE;
      checksum += VMWI_DEACTIVATE;
   }

   /* send the class start packet */
   hdspSendDataPacket( vhdHdl, &packet );


   /* -------------Prepare the end packet --------------------------------- */
   packet.hsxPacketHdr.payloadID = HAPI_PKT_MKPID( HAPI_PKT_CLASSRELAY, HAPI_PKT_CLASS_END );
   packet.hsxPacketHdr.payloadLength = 6;  /* 6 bytes */
   packet.hsxPacketHdr.payloadOffset = sizeof( HAPI_PKT_EGRESS_HDR ); /* offset to payload  */
   packet.hsxPacketHdr.payloadTimeStampLow = 0;

   packet.hsxWord0 = 1;   /* #bytes */

   switch (country)
   {
#if VRG_COUNTRY_CFG_UK
      case VRG_COUNTRY_UK:
      {
         packet.hsxWord1 = 0;   /* mark */
         break;
      }
#endif
      default:
      {
         packet.hsxWord1 = 10; /* mark */
         break;
      }
   }
   packet.hsxClassMsg[0] = (VRG_UINT16)((~checksum) + 1);


   /* Send the CLASS end packet*/
   hdspSendDataPacket( vhdHdl, &packet );

   /* Return success */
   return (HAPISUCCESS);
}


/*
*****************************************************************************
** FUNCTION:   hdspClassEnable
**
** PURPOSE:    This function enables the class transmitter
**
** PARAMETERS: vhdHdl - the vhd handle
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspClassEnable( VRG_UINT16 vhdHdl )
{
   return( hdspSendCmd( vhdHdl, HAPI_CLIDTX_ENABLE, HSCMDDATA, VRG_TRUE, 0 ));
}

/*
*****************************************************************************
** FUNCTION:   hdspClassDisable
**
** PURPOSE:    This function disables the class transmitter
**
** PARAMETERS: vhdHdl - the vhd handle
**
** RETURNS:    status - HAPISUCCESS == 0
**
** NOTE:
**
*****************************************************************************
*/
VRG_SINT16 hdspClassDisable( VRG_UINT16 vhdHdl )
{
   return( hdspSendCmdAsync( vhdHdl, HAPI_CLIDTX_ENABLE, HSCMDDATA, VRG_FALSE, 0 ));
}
