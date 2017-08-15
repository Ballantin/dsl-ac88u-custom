/****************************************************************************
*
*  HAPI_FAXR.H - HAUSWARE Definitions for a FAXR Facsimile Service
*
*  PURPOSE:
*     This file contains the T.30 API definitions.
*
*     (c) Copyright HotHaus Technologies 1996, 1997
*         All rights reserved
*
****************************************************************************/

#ifndef HAPI_FAXR_H                           /* support nested includes */
#define HAPI_FAXR_H
#define MERGER 1

#ifndef HAPI_GFAX
#include <hapi_gfax.h>
#endif

#if !defined( HAPIIDS_H )
#  include <hapiids.h>
#endif

#if !MERGER
#include <hapi_vfax.h>
#include <hapi_v17.h>
#else
#include <hapi_vfx.h>
#endif
/* define faxr socket frame rate to 10ms */
#define FAXRSOCKETFRMRATE        80

#define HAPIFAXRSRV              HAPI_FAXR_SRV

                                           /* FAXR FACSIMILER SERVICE COMMANDS */
#define  HAPI_FAXR_ENABLE_CMD        (HSF_MKHDR(HAPIFAXRSRV, HAPI_OPC_ENABLE_CMD))
#define  HAPI_FAXR_SETREGS_CMD       (HSF_MKHDR(HAPIFAXRSRV, HAPI_OPC_SETREGS_CMD))
#define  HAPI_FAXR_RESETREGS_CMD     (HSF_MKHDR(HAPIFAXRSRV, HAPI_OPC_RESETREGS_CMD))
#define  HAPI_FAXR_SETREG1_CMD       (HSF_MKHDR(HAPIFAXRSRV, HAPI_OPC_SETREG1_CMD))
#define  HAPI_FAXR_GETREGS_CMD       (HSF_MKHDR(HAPIFAXRSRV, HAPI_OPC_GETREGS_CMD))
#define  HAPI_FAXR_GETSTATE_CMD      (HSF_MKHDR(HAPIFAXRSRV, HAPI_OPC_GETSTATE_CMD))
#define  HAPI_FAXR_GETSTATS_CMD      (HSF_MKHDR(HAPIFAXRSRV, HAPI_OPC_GETSTATS_CMD))
#define  HAPI_FAXR_REG_UPDATE_CMD    (HSF_MKHDR(HAPIFAXRSRV, HAPI_OPC_REG_UPDATE_CMD))
#define  HAPI_FAXR_RESET_STATS_CMD   (HSF_MKHDR(HAPIFAXRSRV, HAPI_OPC_RESET_STATS_CMD))


/*
 * FAXR service specific commands
 */
#define  HAPI_FAXR_GETSTATUS_CMD        (HSF_MKCMD_STD(HAPIFAXRSRV, 0))
   /* op1: see HAPI_FAXR_RXENABLE below
      op2: see HAPI_FAXR_RXCFG below */
#define  HAPI_FAXR_SETMAXMODEM_CMD   (HSF_MKCMD_STD(HAPIFAXRSRV, 1))
   /* op1: see HAPI_FAXR_RXENABLE below
      op2: see HAPI_FAXR_RXCFG below */

#define  HAPI_FAXR_UPDATE_BLKTASK_TIMERS_CMD   (HSF_MKCMD_STD(HAPIFAXRSRV, 2))
   /* op1: time interval in msec
      op2: reserved   */


/* FAXR events */

/*
 * FAXR service events
 */
#define  HAPI_FAXR_CALLEND_EVT               (HSF_MKEVT_STD(HAPIFAXRSRV, 0))
   /* op1: reserved = 0
      op2: reserved = 0 */

#define  HAPI_FAXR_RATE_EVT                  (HSF_MKEVT_STD(HAPIFAXRSRV, 1))
   /* op1: data rate
      op2: reserved = 0 */

#define  HAPI_FAXR_REGERR_EVT                (HSF_MKEVT_STD(HAPIFAXRSRV, 2))
   /* op1: OFFSETOF register in error
      op2: value register has been set to */

/* debug events (only generated if hsxfaxrsendevents register is non-zero) */

#define  HAPI_FAXR_DEBUGRXTONE_EVT           (HSF_MKEVT_STD(HAPIFAXRSRV, 3))
   /* op1: 0 for CED, 1 for CNG
      op2: reserved = 0 */

#define  HAPI_FAXR_DEBUGRXV21PACKET_EVT      (HSF_MKEVT_STD(HAPIFAXRSRV, 4))
   /* op1: see HSEFAXRFCF definition in file hsfaxri.h
      op2: reserved = 0 */

#define  HAPI_FAXR_DEBUGTXTONE_EVT           (HSF_MKEVT_STD(HAPIFAXRSRV, 5))
   /* op1: 0 for CED, 1 for CNG
      op2: reserved = 0 */

#define  HAPI_FAXR_DEBUGTXV21PACKET_EVT      (HSF_MKEVT_STD(HAPIFAXRSRV, 6))
   /* op1: see HSEFAXRFCF definition in file hsfaxri.h
      op2: reserved = 0 */

/* debug events (conditionally compiled) */

#define  HAPI_FAXR_DATADEBUG_EVT             (HSF_MKEVT_STD(HAPIFAXRSRV, 7))
   /* op1: see source code
      op2: see source code */

#define  HAPI_FAXR_DEBUG_EVT                 (HSF_MKEVT_STD(HAPIFAXRSRV, 8))
   /* op1: see source code
      op2: see source code */

#define  HAPI_FAXR_DEBUGEVENT_EVT            (HSF_MKEVT_STD(HAPIFAXRSRV, 9))
   /* op1: see source code
      op2: see source code */

#define  HAPI_FAXR_DEBUGRXDATA_EVT           (HSF_MKEVT_STD(HAPIFAXRSRV, 10))
   /* op1: see source code
      op2: see source code */

#define  HAPI_FAXR_DEBUGRXV21DATA_EVT        (HSF_MKEVT_STD(HAPIFAXRSRV, 11))
   /* op1: see source code
      op2: see source code */

#define  HAPI_FAXR_DEBUGRXECMBDPACKET_EVT    (HSF_MKEVT_STD(HAPIFAXRSRV, 12))
   /* op1: see source code
      op2: see source code */

#define  HAPI_FAXR_DEBUGRXECMGDPACKET_EVT    (HSF_MKEVT_STD(HAPIFAXRSRV, 13))
   /* op1: see source code
      op2: see source code */

#define  HAPI_FAXR_DEBUGTXECMBDPACKET_EVT    (HSF_MKEVT_STD(HAPIFAXRSRV, 14))
   /* op1: see source code
      op2: see source code */

#define  HAPI_FAXR_DEBUGTXECMGDPACKET_EVT    (HSF_MKEVT_STD(HAPIFAXRSRV, 15))
   /* op1: see source code
      op2: see source code */

#define  HAPI_FAXR_BM_DEBUG_EVT               (HSF_MKEVT_STD(HAPIFAXRSRV, 16))
   /* op1: see source code
      op2: see source code */

#define  HAPI_FAXR_GETREGS_EVT                (HSF_MKEVT_EXT(HAPIFAXRSRV, 17)) /* not used */

#define  HAPI_FAXR_GETSTATE_EVT               (HSF_MKEVT_EXT(HAPIFAXRSRV, 18)) /* not used */
#define  HAPI_FAXR_GETSTATS_EVT               (HSF_MKEVT_EXT(HAPIFAXRSRV, 19))

#define  HAPI_FAXR_V21_PREAMBLE_EVT           (HSF_MKEVT_EXT(HAPIFAXRSRV, 20))
   /* op1: see source code
      op2: see source code */

/* Temp   */
#define HAPIFAXRGETSTATUS         HAPI_FAXR_GETSTATUS_CMD
#define HAPIFAXRENABLE            HAPI_FAXR_ENABLE_CMD
#define HAPIFAXRSETREGS           HAPI_FAXR_SETREGS_CMD   /* set registers          */
#define HAPIFAXRRESETREGS         HAPI_FAXR_RESETREGS_CMD
#define HAPIFAXRSETMAXMODEM       HAPI_FAXR_SETMAXMODEM_CMD   /* set the maximum using modem */
#define HAPIFAXRGETSTATS          HAPI_FAXR_GETSTATS_CMD   /* Get the fax relay statistics */

/* FAXR events */

#define HAPIFAXRCALLEND           HAPI_FAXR_CALLEND_EVT   /* FAXR call end */
#define HAPIFAXRATE               HAPI_FAXR_RATE_EVT      /* FAXR call rate */
#define HAPI_FAXR_GETREGS         HAPI_FAXR_GETREGS_EVT   /* get registers */
#define HAPI_FAXR_GETSTATE        HAPI_FAXR_GETSTATE_EVT  /* Get faxr state */
#define HAPI_FAXR_GETSTATS        HAPI_FAXR_GETSTATS_EVT  /* Get faxr stats */

/* debug events */

#define HAPIFAXRDEBUGRXDATA       HAPI_FAXR_DEBUGRXDATA_EVT   /* FAXR receive fax data from remote fax */
#define HAPIFAXRDEBUGRXV21DATA    HAPI_FAXR_DEBUGRXV21DATA_EVT   /* FAXR V.21 data detection */
#define HAPIFAXRDEBUGRXV21PACKET  HAPI_FAXR_DEBUGRXV21PACKET_EVT
#define HAPIFAXRDEBUGTXV21PACKET  HAPI_FAXR_DEBUGTXV21PACKET_EVT   /* FAXR packet transmitted to remote fax */
#define HAPIFAXRDEBUGRXTONE       HAPI_FAXR_DEBUGRXTONE_EVT   /* FAXR a CED / CNG tone has been detected */
#define HAPIFAXRDEBUGTXTONE       HAPI_FAXR_DEBUGTXTONE_EVT   /* FAXR a CED / CNG tone has been tramsitted */

/* the following debug events are temporary and for internal use only */

#define HAPIFAXRDEBUGTXECMGDPACKET HAPI_FAXR_DEBUGTXECMGDPACKET_EVT  /* TX ECM good frame numeber and size */
#define HAPIFAXRDEBUGTXECMBDPACKET HAPI_FAXR_DEBUGTXECMBDPACKET_EVT /* TX ECM bad frame numeber and size */
#define HAPIFAXRDEBUGRXECMGDPACKET HAPI_FAXR_DEBUGRXECMGDPACKET_EVT  /* RX ECM good frame numeber and size */
#define HAPIFAXRDEBUGRXECMBDPACKET HAPI_FAXR_DEBUGRXECMBDPACKET_EVT  /* RX ECM bad frame numeber and size */

#define HAPIFAXRDEBUGEVENT        HAPI_FAXR_DEBUGEVENT_EVT   /* General Fax Relay debug event */
#define HAPIFAXRDEBUG             HAPI_FAXR_DEBUG_EVT   /* general debug event */
#define HAPIFAXRBMDEBUGEVT        HAPI_FAXR_BM_DEBUG_EVT   /* general debug event */
#define HAPIFAXRDATADEBUG         HAPI_FAXR_DATADEBUG_EVT   /* # of packets held by faxr */

#define HAPIFAXEBUGV21PREAMBLE    HAPI_FAXR_V21_PREAMBLE_EVT
/* FAXR status word */

#define  HSFAXRENABLE             0x0001   /* enable bit for Fax Relay Service */
//#define  HSFAXRV17ENABLE          0x0002   /* enable bit for V.17 Service */
#define  HSFAXRVFAXENABLE         0x0004   /* enable bit for V.Fax Service	*/

#define  HSFAXRV21ENABLE          0x0008   /* enable bit for V.21 Service */
#define  HAPI_FAXR_EOM            0x0010   /* EOM, PPS-EOM detected */
#define  HAPI_FAXR_EOP            0x0020   /* EOP, PPS-EOP detected */ 
#define  HAPI_FAXR_MCF            0x0040   /* MCF detected */ 

#define  HAPI_FAXR_CALL_SUCCESS   0x8000   /* Call successful */ 

/* FAXR enable status word */

#define  HSFAXRNORMAL             0        /* Normal enable state */
#define  HSFAXRTXHDLC             1        /* Start in Tx HDLC state */
#define  HSFAXRRXHDLC             2        /* Start in Rx HDLC state	*/
#define  HSFAXRCNGDET             3        /* CNG detected 	*/
/*
 * op1 for CALLEND event
 */
#define  HAPI_FAXR_CALLEND_OTHER      0        /* call ended due to internal timeout */
#define  HAPI_FAXR_CALLEND_DCN        1        /* call ended due to DCN */
#define  HAPI_FAXR_CALLEND_SILENCE    2        /* call ended due to bi-directional silence */ 

/* Ctrl Types */

#define  HSFAXRFINAL               0xc8
#define  HSFAXRNONFINAL            0xc0


/* FAXR Facsimile registers */

#define  HSFAXRCEDONDELAY      2000
#define  HSFAXRCEDONDURATION   3000

#define  HSFAXRCNGONDELAY      3000
#define  HSFAXRCNGONDURATION   500

/* tone types */

#define  HSFAXRCED          0
#define  HSFAXRCNG          1

/* Modem types (number assigned according to DIS table) */

#define  HSFAXRV17          13
#define  HSFAXRV29          12
#define  HSFAXRV27          4

/* Packet size for different tx rate */

#define  HSFAXR_14400_PACKETSIZE   18  /* number of words for 14400 in 20 ms */
#define  HSFAXR_12000_PACKETSIZE   15  /* number of words for 12000 in 20 ms */
#define  HSFAXR_9600_PACKETSIZE    12  /* number of words for 9600 in 20 ms */
#define  HSFAXR_7200_PACKETSIZE    9   /* number of words for 7200 in 20 ms */
#define  HSFAXR_4800_PACKETSIZE    6   /* number of words for 4800 in 20 ms */
#define  HSFAXR_2400_PACKETSIZE    3   /* number of words for 2400 in 20 ms */

/* TCF bit err threshold for different bit rates */

#define  HSFAX_14400_MAXERRBITS   170  /* 1% of 1.2 sec of bits at 14400 */
#define  HSFAX_12000_MAXERRBITS   144  /* 1% of 1.2 sec of bits at 12000 */
#define  HSFAX_9600_MAXERRBITS    115  /* 1% of 1.2 sec of bits at 9600  */
#define  HSFAX_7200_MAXERRBITS    86   /*  1% of 1.2 sec of bits at 7200 */
#define  HSFAX_4800_MAXERRBITS    57   /*  1% of 1.2 sec of bits at 4800 */
#define  HSFAX_2400_MAXERRBITS    28   /*  1% of 1.2 sec of bits at 2400 */

#define  HSFAXRMAXJITTER          150     /* expected max jitter time on network */
#define  HSFAXRMAXDELAY           2000    /* expected max delay time on network */
#define  HSFAXRMAXREORDERDELAY    0     /* expected max delay time on network */

#define  HSFAXRV21SENDNETMSG      0       /* number of redundant V.21 messages to network */
#define  HSFAXRTIMESTAMPMODULUS   0       /* number of redundant V.21 messages to network */

/* Default value for custom features register */
#define HAPI_FAXR_CUSTOM_DEFAULT  0

/* Faxr custom feature list */
#define HAPI_FAXR_TCF_MASK        (1 << 0)

/*****************************************************************
** Fax Relay FCF Enumerated Types
*****************************************************************/

typedef enum
{
   HSFAXRNULL     = 0x0,
   HSFAXRPPSNULL  = 0x0,
   HSFAXRDIS      = 0x01,
   HSFAXRCSI      = 0x02,
   HSFAXRNSF      = 0x04,
   HSFAXRCFR      = 0x21,
   HSFAXRFTT      = 0x22,
   HSFAXRCTR      = 0x23,
   HSFAXRMCF      = 0x31,
   HSFAXRRTN      = 0x32,
   HSFAXRRTP      = 0x33,
   HSFAXRPIN      = 0x34,
   HSFAXRPIP      = 0x35,
   HSFAXRRNR      = 0x37,
   HSFAXRERR      = 0x38,
   HSFAXRPPR      = 0x3d,
   HSFAXRDCS      = 0x41,
   HSFAXRTSI      = 0x42,
   HSFAXRSUB      = 0x43,
   HSFAXRNSS      = 0x44,
   HSFAXRPWD      = 0x45,
   HSFAXRCTC      = 0x48,
   HSFAXRCRP      = 0x58,
   HSFAXRDCN      = 0x5f,
   HSFAXREOM      = 0x71,
   HSFAXRMPS      = 0x72,
   HSFAXREOR      = 0x73,
   HSFAXREOP      = 0x74,
   HSFAXRRR       = 0x76,
   HSFAXRPRIEOM   = 0x79,
   HSFAXRPRIMPS   = 0x7a,
   HSFAXRPRIEOP   = 0x7c,
   HSFAXRPPS      = 0x7d,
   HSFAXRDTC      = 0x81,
   HSFAXRCIG      = 0x82,
   HSFAXRNSC      = 0x84,
   HSFAXRPPSEOM   = 0xf1,
   HSFAXRPPSMPS   = 0xf2,
   HSFAXRPPSEOP   = 0xf4,
   HSFAXRPPSPRIEOM= 0xf9,
   HSFAXRPPSPRIMPS= 0xfa,
   HSFAXRPPSPRIEOP= 0xfc,
   HSFAXRHDLC     = 0xff

} HSEFAXRFCF;


/* setup modes */

/* FAXR local registers */

typedef  struct
{
   HAPI_UINT16            hsxmaxjitter;
   HAPI_UINT16            hsxmaxdelay;
   HAPI_UINT16            hsxmaxreorderdelay;
   HAPI_UINT16            hsxTimeStampModulus;
   HAPI_UINT16            hsxnumv21msgsend;
   HAPI_UINT16            hsxmaxfaxmodem;
   HAPI_UINT16            hsxnumgenHDLC;
   HAPI_UINT16            hsxnumdetHDLC;
   HAPI_UINT16            hsxcedduration;
   HAPI_UINT16            hsxcngdonedelay;
   HAPI_UINT16            hsxcngduration;
   HAPI_UINT16            hsxlocalrate;
   HAPI_UINT16            hsxtcflocalgen;
   HAPI_UINT16            hsxrxpacketsize;
   HAPI_UINT16            hsxfaxrsendevents;
   HAPI_UINT16            hsxcustomfeatures;

} HSZFAXRREGS;

/*********************************
** Default Registers
*********************************/

#define  V17            1     /* enable V.17 rates */
#if V17
#define HAPI_FAXR_DEF_REGS                                        \
{                                                                 \
   HSFAXRMAXJITTER,         /* max expected jitter on network */  \
   HSFAXRMAXDELAY,          /* max expected delay on network */  \
   HSFAXRMAXREORDERDELAY,   /* Maximum reodering delay */  \
   HSFAXRTIMESTAMPMODULUS,  /* time stamp modulus  */  \
   HSFAXRV21SENDNETMSG,     /* number of resend V.21 message */  \
   HSFAXRV17,               /* max fax modem we could use */  \
   33,                      /* HDLC flags - generation for 1 sec */  \
   15,                      /* HDLC flags - detection  */  \
   HSFAXRCEDONDURATION,       \
   HSFAXRCNGONDELAY,          \
   HSFAXRCNGONDURATION,       \
   HSGFAX_14400_V17,        /* V.17 default local rate */  \
   0,                       /* TCF generate locally */  \
   (HAPI_VFX_RXPKTSIZE_IN_BITS | 16*18),    /* Max packet size in bits  */  \
   0,                        /* FAXR Events */  \
   HAPI_FAXR_CUSTOM_DEFAULT, \
}
#else
#define HAPI_FAXR_DEF_REGS                                        \
{                                                                 \
   HSFAXRMAXJITTER,         /* max expected jitter on network */  \
   HSFAXRMAXDELAY,          /* max expected delay on network */  \
   HSFAXRMAXREORDERDELAY,   /* Maximum reodering delay */  \
   HSFAXRTIMESTAMPMODULUS,  /* time stamp modulus  */  \
   HSFAXRV21SENDNETMSG,     /* number of resend V.21 message */  \
   HSFAXRV29,               /* max fax modem we could use */  \
   33,                      /* HDLC flags - generation for 1 sec */  \
   15,                      /* HDLC flags - detection  */  \
   HSFAXRCEDONDURATION,       \
   HSFAXRCNGONDELAY,          \
   HSFAXRCNGONDURATION,       \
   HSGFAX_9600,             /* V.29 default local rate */  \
   0,                       /* TCF generate locally */  \
   (HAPI_VFX_RXPKTSIZE_IN_BITS | 16*18),  /* Max packet size in bits  */  \
   0,                        /* FAXR Events */  \
   HAPI_FAXR_CUSTOM_DEFAULT \
}
#endif

typedef struct
{
   HAPI_UINT16         hsxctrl;                   /* control field */
   HAPI_UINT16         hsxfcf;                    /* facsimile control field */
   HAPI_UINT16         hsxfcf2;                   /* faxsimile control field 2 (for ECM)*/

} HSZFAXRMSG;

/* service archive */


typedef struct
{
  HAPI_UINT16    hsxFrameCount;        /* Rx Frame Count */
  HAPI_UINT16    hsxOverrunCount;       /* Egress overrun counter,currently unused */
  HAPI_UINT16    hsxSpoofCount;          /* Spoofing counter */
  HAPI_UINT16    hsxReorderCount;         /* Lost frames resulting in frame repeat */
  HAPI_UINT16    hsxfailtoReorderCount;         /* Lost frames resulting in frame repeat */
  HAPI_UINT16    hsxretrainCount;          /* Lost frames resulting in frame repeat */
  HAPI_UINT16    hsxPPRCount;          /* PPR resulting in frame retransmission */
  HAPI_UINT16    hsxbadframeCount;          /* Bad FCS frame count */
  HAPI_UINT16    hsxSpoofTimeoutCount;  /* Spoofing timeout counter */
  HAPI_UINT16    hsxv21TxUflowCount;       /* V.21 Tx. underflow counter. Implies
                                         that there was excessive network
                                         jitter than "hsxmaxjitter"*/
  HAPI_UINT16    hsxNumRxPages;        /* Number of fax pages received */


} HSZFAXREGRESS_STATS;

typedef struct
{
  HAPI_UINT16    hsxFrameCount;    /* Tx Frame Count */
  HAPI_UINT16    hsxOverrunCount;  /* Encoder overrun count */
  HAPI_UINT16    hsxretrainCount;  /* Lost frames resulting in frame repeat */
  HAPI_UINT16    hsxPPRCount;      /* Lost frames resulting in frame repeat */
  HAPI_UINT16    hsxbadframeCount;          /* Bad FCS frame count */
  HAPI_UINT16    hsxNumTxPages;        /* Number of fax pages transmitted */
} HSZFAXRINGRESS_STATS;

/* PVE encoder decoder states structure */
typedef struct
{
   HSZFAXREGRESS_STATS   hsxfaxrEgressStats;
   HSZFAXRINGRESS_STATS   hsxfaxrIngressStats;

} HSZFAXRSTATS;

#endif                                    /* HAPI_FAXR_H */
