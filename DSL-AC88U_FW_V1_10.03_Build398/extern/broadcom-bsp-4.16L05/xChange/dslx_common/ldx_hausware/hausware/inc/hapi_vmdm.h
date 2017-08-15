/****************************************************************************
*
*  HAPI_VMDM.H - HAUSWARE common modem commands and events.
*
*  PURPOSE:
*     This file contains definitions common to V8 V22 V32 and V34
*
**********************************************************************/

#ifndef HAPI_VMDM                           /* support nested includes */
#define HAPI_VMDM

#if !defined( HAPI_TYPES_H )
#include <hapi_types.h>                /* Hausware environment definitions */
#endif
#if !defined( HAPIGDEF_H )
#include <hapigdef.h>                  /* Hausware API Global definitions  */
#endif
#if !defined( HAPI_OPCODE_H )
#include <hapi_opcode.h>                  /* Hausware generic cmds/events     */
#endif
#if !defined( HAPIIDS_H )
#  include <hapiids.h>
#endif

/* MODEM service types */
#define    HAPIV8SRV   HAPI_V8_SRV        /* V8 autobaud */
#define    HAPIV21SRV  HAPI_V21_SRV       /* V.21 300bps */
#define    HAPIV22BSRV HAPI_V22B_SRV      /* V.22bis 1200-2400 bps */
#define    HAPIV23SRV  HAPI_V23_SRV       /* V.23 1200/75 bps, asymmetric */
#define    HAPIV26TSRV HAPI_V26T_SRV      /* V.26ter */
#define    HAPIV32SRV  HAPI_V32B_SRV      /* V.32bis 4800 - 14,400 bps */
#define    HAPIV34SRV  HAPI_V34_SRV       /* V.34 2400 - 33,600  */



/* Modem commands - maximum of 0x1f unique codes allowed */
typedef enum
{
    HAPI_VMDM_DISCONNECTCMD,    /* Disconnection           */
    HAPI_VMDM_REMOTEEVT,        /* data relay only - event from
                                   remote modem */
    HAPI_VMDM_REQNEWRATE,       /* request new rate        */
    HAPI_VMDM_RETRAIN,          /* initiate retraining     */
    HAPI_VMDM_RXCFG,            /* configure receiver      */
    HAPI_VMDM_SETIDLEPTN,       /* set tx idle pattern     */
    HAPI_VMDM_START,            /* start modem session     */
    HAPI_VMDM_TXCFG,            /* configure transmitter   */
    HAPI_VMDM_TXDATA,           /* transmit data           */
    HAPI_VMDM_TXREADYCMD,       /* transmitter ready       */
    HAPI_VMDM_TXNOTREADYCMD,    /* transmitter not ready   */
    HAPI_VMDM_SENDBRK           /* send break signal       */

} HAPI_VMDM_COMMAND;

/* Modem events - maximum of 0x1f unique codes allowed */
typedef enum
{
   /* General events */
   HAPI_VMDM_TXDATASENT,         /* number of Tx data bits sent */
   HAPI_VMDM_DATASENT,         /* number of Tx data bits sent */
   HAPI_VMDM_RXDATA,           /* one receive buffer is filled and
                                       available for reading */
   HAPI_VMDM_TXCMDQLOW,        /* command queue length below low
                                       threshold */
   HAPI_VMDM_TXCMDQHIGH,       /* command queue length above high
                                       threshold */
   HAPI_VMDM_TXCMDQUFLOW,      /* command queue under flow
                                       (length = 0) */
   HAPI_VMDM_TXREADY,          /* transmitter is ready        */
   HAPI_VMDM_TXNOTREADY,       /* transmitter is not ready    */
   HAPI_VMDM_DISCONNECT,       /* disconnect complete */
   HAPI_VMDM_NOCARRIER,        /* no carrier detected */
   HAPI_VMDM_SCR1SDETD,        /* 1200bps scrambled detected */

   /* Diagnostic events */
   HAPI_VMDM_DIAG_RTD,         /* round trip dly too long for bulk dly */
   HAPI_VMDM_ERROR,            /* unexpected code error */
   HAPI_VMDM_DIAG,             /* generic diagnostic event.  Extended message
                                  with the following format within the message :
                                  word 0 : service ID
                                  word 1 : diagnostic code
                                  remaining words : dependent on diagnostic code */

   /* Specific event base reset */
   FIRST_SPECIFIC_EVT,

   /* V.32 specific events */
   HAPI_VMDM_UNSUPPRATE = FIRST_SPECIFIC_EVT,   /* our desired tx rate is not supported
                                                   by the other modem */
   HAPI_VMDM_GSTNCLR,          /* GSTN clear down; requested by the
                                  other modem */
   HAPI_VMDM_ANSDET,           /* Call mode, ANS tone detected */
   HAPI_VMDM_ACDET,            /* Call mode AC pattern det */
   HAPI_VMDM_AADET,            /* Answermode, AA Detected */

   /* V.8 specific events */
   HAPI_VMDM_CNCT_TYPE = FIRST_SPECIFIC_EVT,    /* connect type */
   HAPI_VMDM_JM,              /* Joint menu */
   HAPI_VMDM_FAX_CNG,         /* calling station is fax */
   HAPI_VMDM_RES_MENU_FLD,    /* reserved menu field */
   HAPI_VMDM_AM,              /* AM detected on answer tone */
   HAPI_VMDM_LOCAL_EVT,       /* data relay only - event from local
                                 modem */
   HAPI_VMDM_RATE_SYNCED,     /* data relay only - local and remote
                                 modem rates are synchronized */
   HAPI_VMDM_FAX,             /* data relay only
                                 - if call modem, then HDLC
                                   detected from local modem
                                 - if answer modem, then HDLC
                                   detected at remote modem
                                 - in either case, host should stop
                                   V.8 and start fax. */
   HAPI_VMDM_RTD,             /* reports round trip delay  */

   /* Link Layer events */
   HAPI_VMDM_BREAK_RCVD,      /* received break signal from remote side */
   HAPI_VMDM_LL_STATUS,       /* link layer status */

   /* V.21 specific events */
   HAPI_VMDM_RXCED = FIRST_SPECIFIC_EVT,        /* called station ID signal */
   HAPI_VMDM_RXCNG,           /* calling tone detected */
   HAPI_VMDM_RXHDLC,          /* HDLC detected */

   /* Fax specific events */
   HAPI_VMDM_MODEMRDY = FIRST_SPECIFIC_EVT,     /* segment 4 has been transmitted or received */
   HAPI_VMDM_RXDATAEND,       /* incoming data signal has ended
                                 or the modem has lost synchronization */
   HAPI_VMDM_RXTCFERR,        /* number of bit errors and maximum
                                 error measured during TCF */
   HAPI_VMDM_RXTURNON,        /* fax turn on sequence detected */
   HAPI_VMDM_TRAINRX,         /* receiver detected train */
   HAPI_VMDM_TXDATAEND,        /* transmitter has completed sending data */
   HAPI_VMDM_BMDEBUG        /* transmitter has completed sending data */


} HAPI_VMDM_EVENT;


/* MODEM SERVICE COMMANDS */
#define  HAPIVMDMENABLE            HAPI_OPC_ENABLE_CMD        /* enable and reset modem  */
#define  HAPIVMDMGETSTATUS         HAPI_OPC_GETSTATE_CMD        /* get modem status        */
#define  HAPIVMDMSETREGS           HAPI_OPC_SETREGS_CMD        /* set registers           */
#define  HAPIVMDMSETREG1           HAPI_OPC_SETREG1_CMD        /* set registers           */
#define  HAPIVMDMGETREGS           HAPI_OPC_GETREGS_CMD        /* get registers           */
#define  HAPIVMDMRESETREGS         HAPI_OPC_RESETREGS_CMD        /* reset registers         */
#define  HAPIVMDMTXCFG             HAPI_VMDM_TXCFG        /* configure transmitter   */
#define  HAPIVMDMSTART             HAPI_VMDM_START        /* start modem session     */
#define  HAPIVMDMTXDATA            HAPI_VMDM_TXDATA        /* transmit data           */
#define  HAPIVMDMRETRAIN           HAPI_VMDM_RETRAIN        /* initiate retraining     */
#define  HAPIVMDMDISCONNECT        HAPI_VMDM_DISCONNECT        /* Disconnection           */
#define  HAPIVMDMRXCFG             HAPI_VMDM_RXCFG        /* configure receiver      */
#define  HAPIVMDMSETIDLEPTN        HAPI_VMDM_SETIDLEPTN        /* set tx idle pattern     */
#define  HAPIVMDMREQNEWRATE        HAPI_VMDM_REQNEWRATE        /* request new rate        */
// #define  HAPIVMDMGETRXQLTY         HAPI_VMDM_GETRXQLTY        /* get receive quality     */
// #define  HAPIVMDMSTOPMDM           HAPI_VMDM_STOPMDM        /* V.8 stop modem */
// #define  HAPIVMDMSTARTMDM          HAPI_VMDM_STARTMDM        /* V.8 start another modem */
#define  HAPIVMDMREMOTEEVT         HAPI_VMDM_REMOTEEVT       /* data relay only - event from
                                                 remote modem */

/* MODEM SERVICE EVENTS */

#define  HAPIVMDMTXDATASENT     HAPI_VMDM_TXDATASENT    /* the current transmit data buffer is
                                                sent */ /* Not in XME */
#define  HAPIVMDMRXDATA         HAPI_VMDM_RXDATA    /* one receive buffer is filled and
                                                available for reading */
#define  HAPIVMDMTXCMDQLOW      HAPI_VMDM_TXCMDQLOW    /* command queue length below low
                                          threshold */
#define  HAPIVMDMTXCMDQHIGH     HAPI_VMDM_TXCMDQHIGH    /* command queue length above high
                                          threshold */
#define  HAPIVMDMTXCMDQUFLOW    HAPI_VMDM_TXCMDQUFLOW    /* command queue under flow
                                           (length = 0) */
// #define  HAPIVMDMTXINVALIDCMD   HAPI_VMDM_TXINVALIDCMD    /* invalid command during training */
#define  HAPIVMDMRXDATAEND      HAPI_VMDM_RXDATAEND    /* incomming data signal has ended */
                                        /* or the modem has lost
                                           synchronization */
#define  HAPIVMDMERROR          HAPI_VMDM_ERROR    /* error message in op1 */
// #define  HAPIVMDMRXGAIN         HAPI_VMDM_RXGAIN    /* Receiver Gain */
// #define  HAPIVMDMRESETRXGAIN    HAPI_VMDM_RESETRXGAIN    /* Signal the client app. to reset */
                                        /* the receiver pxd and hardware gain */
#define  HAPIVMDMTXREADY        HAPI_VMDM_TXREADY    /* transmitter is ready        */
// #define  HAPIVMDMTXDATACLR      HAPI_VMDM_TXDATACLR    /* clear the transmit buffer */
                                        /* host has to free the buffer memory
                                           point to */
                                        /* by op2 */
// #define  HAPIVMDMINVALIDSETREGS HAPI_VMDM_INVALIDSETREGS    /* set register when modem enabled.
//                                            Not allowed */
// #define  HAPIVMDMINVALIDTXRATE  HAPI_VMDM_INVALIDTXRATE    /* rate not supported */
// #define  HAPIVMDMSWITCHRATE     HAPI_VMDM_SWITCHRATE    /* rate switched to one that is
//                                            supported by the remote modem */
// #define  HAPIVMDMFALLBACK       HAPI_VMDM_FALLBACK    /* retrain due to excessive error */
#define  HAPIVMDMNOCARRIER      HAPI_VMDM_NOCARRIER    /* no carrier detected */
#define  HAPIVMDMSCR1SDETD      HAPI_VMDM_SCR1SDETD    /* 1200bps scrambled detected */
//#define  HAPIVMDMINVALIDREGSETTING  HAPI_VMDM_INVALIDREGSETTING    /* Particular reg setting invalid */

//#define  HAPIVMDMRETRAINREQ     HAPI_VMDM_RETRAINREQ    /* retrain/rate negotation request from
//                                           commercial modem detected */

#define  HAPIVMDMDATASENT     HAPI_VMDM_DATASENT    /* the current transmit data buffer is
                                                sent */

/* Modem specific events start here */

/* V32 MODEM specific EVENTS 0x40 to 0x6f */

#define  HAPIVMDMUNSUPPRATE     HAPI_VMDM_UNSUPPRATE    /* our desired tx rate is not supported
                                           by the other modem */
// #define  HAPIVMDMMISMATCHRATE   HAPI_VMDM_MISMATCHRATE    /* final received rate not matched
//                                            with the desired tx rate  */
#define  HAPIVMDMGSTNCLR        HAPI_VMDM_GSTNCLR    /* GSTN clear down; requested by the
                                           other modem */
// #define  HAPIVMDMRCVGAIN        HAPI_VMDM_RCVGAIN    /* Gain sent back to client */
#define  HAPIVMDMANSDET         HAPI_VMDM_ANSDET    /* Call mode, ANS tone detected */
#define  HAPIVMDMACDET          HAPI_VMDM_ACDET    /* Call mode AC pattern det */
#define  HAPIVMDMAADET          HAPI_VMDM_AADET    /* Answermode, AA Detected */

/* V32 MODEM SERVICE DIAGNOSTIC EVENTS */

//#define  HAPIVMDMACCA           HAPI_VMDM_ACCA    /* ACCA Detected (rxLevel, threshold) */
//#define  HAPIVMDMAACC           HAPI_VMDM_AACC    /* AACC Detected (rxLevel, threshold) */
//#define  HAPIVMDMCAAC           HAPI_VMDM_CAAC    /* CAAC Detected (rxLevel, threshold) */
//#define  HAPIVMDMS              HAPI_VMDM_S    /* S Detected (rxLevel) */
//#define  HAPIVMDMTRN            HAPI_VMDM_TRN    /* Training sequence received
//                                           (average Error) */
//#define  HAPIVMDMR1             HAPI_VMDM_R1    /* Rate 1 Detected (Rate) */
//#define  HAPIVMDMR2             HAPI_VMDM_R2    /* Rate 2 Detected (Rate) */
//#define  HAPIVMDMR3             HAPI_VMDM_R3    /* Rate 3 Detected (Rate) */
//#define  HAPIVMDME              HAPI_VMDM_E    /* E Detected (E Value) */
//#define  HAPIVMDMHSTHRESH       HAPI_VMDM_HSTHRESH    /* theshold for phase reversal det */


/* Diagnostic EVENTS 0xd0 to 0xdf */
#define  HAPIVMDM_DIAG_RTD      HAPI_VMDM_DIAG_RTD    /* round trip dly too long for bulk dly */


/* V8 specific EVENTS 0xe0 to 0xef */
#define  HAPIVMDM_CNCT_TYPE     HAPI_VMDM_CNCT_TYPE    /* connect type */
#define  HAPIVMDM_JM            HAPI_VMDM_JM    /* Joint menu */
#define  HAPIVMDM_FAX_CNG       HAPI_VMDM_FAX_CNG    /* calling station is fax */
#define  HAPIVMDM_RES_MENU_FLD  HAPI_VMDM_RES_MENU_FLD    /* reserved menu field */
#define  HAPIVMDM_AM            HAPI_VMDM_AM    /* AM detected on answer tone */
#define  HAPIVMDM_LOCAL_EVT     HAPI_VMDM_LOCAL_EVT    /* data relay only - event from local
                                           modem */
#define  HAPIVMDM_RATE_SYNCED   HAPI_VMDM_RATE_SYNCED    /* data relay only - local and remote
                                           modem rates are synchronized */
#define  HAPIVMDM_FAX           HAPI_VMDM_FAX    /* data relay only
                                            - if call modem, then HDLC
                                            detected from local modem
                                            - if answer modem, then HDLC
                                            detected at remote modem
                                            - in either case, host should stop
                                            V.8 and start fax. */
/*
 * CNCT_TYPE
 *    HSVMDMTYPE op1;          indicate modem type
 *    op2 = 0
 * JM (extended data)
 *    op1 = sizeof(MENU)       sizeof extended data
 *    MENU *op2                joint menu pointer
 *
 * AM
 *    HAPI_SINT16 op1;              modulation power
 *    HAPI_SINT16 op2;              carrier power
 */

/* V21 MODEM SERVICE EVENTS 0xf0 to 0xfe */
#define  HAPIVMDMRXCED          HAPI_VMDM_RXCED    /* called station ID signal */
#define  HAPIVMDMRXCNG          HAPI_VMDM_RXCNG    /* calling tone detected */

#define  HAPIVMDMBMDEBUG         HAPI_VMDM_BMDEBUG        /* transmitter has completed sending data */
//#define  HAPIVMDM_MAXBLKTIME    HAPI_VMDM_MAXBLKTIME
//#define  HAPIVMDM_STATECHNG     HAPI_VMDM_STATECHNG
typedef enum
{
   HSVMDM_75 = -2,
   HSVMDM_300 = -1,
   HSVMDM_1200 = 0,
   HSVMDM_2400,
   HSVMDM_4800,
   HSVMDM_7200CODED,
   HSVMDM_9600,
   HSVMDM_9600CODED,
   HSVMDM_12000CODED,
   HSVMDM_14400CODED,
   HSVMDM_16800CODED,
   HSVMDM_19200CODED,
   HSVMDM_21600CODED,
   HSVMDM_24000CODED,
   HSVMDM_26400CODED,
   HSVMDM_28800CODED,
   HSVMDM_31200CODED,
   HSVMDM_33600CODED

} HSEVMDMRATE;

/* Modem Direction */
typedef enum
{
   HSVMDMORIGINATE = 0,    /* duplex calling modem */
   HSVMDMANSWER,           /* duplex answer modem */
   HSVMDMSIMPLEX           /* one way only */

} VMDMDIRECTION;

/* Modem Rates */
typedef enum
{
   HSVMDM_V8     = HAPIV8SRV,
   HSVMDM_V21    = HAPIV21SRV,
   HSVMDM_V22    = HAPIV22BSRV,
   HSVMDM_V23    = HAPIV23SRV,
   HSVMDM_V26    = HAPIV26TSRV,
   HSVMDM_V32    = HAPIV32SRV,
   HSVMDM_V34    = HAPIV34SRV

}  HSVMDMTYPE;

/* Common register definitions for pure data modems (V22, V32, V34 and also used by V.8) */
typedef struct
{
   HAPI_UINT16             hsxrate;              /* data rate */
   HAPI_UINT16             hsxdirection;         /* direction */
   HAPI_UINT16             hsxanstxamplitude;    /* tx amplitude */
   HAPI_UINT16             hsxanstoneduration;   /* answer tone duration */
   HAPI_UINT16             hsxaveerrorthresh;    /* Error rate threshold */
   HAPI_UINT16             hsxnumbits;           /* Number of bits in packets */
   HAPI_UINT16             hsxpxd;               /* the PXD where the modem is */
                                           /*   attached to */
   HAPI_UINT16             hsxmode;              /* transmit data mode SYNC/ASYNC */

   HAPI_UINT16             hsxcmdqhighthreshold; /* command queue high threshold */
   HAPI_UINT16             hsxcmdqlowthreshold;  /* command queue low threshold */

} HSZVMDMREGS;

/* Modem register ID's for HAPIVMDMINVALIDREGSETTING event */

typedef enum
{
   /* ID's for HSZVMDMREGS registers */

   HSXRATE_ID,
   HSXDIRECTION_ID,
   HSXANSTXAMPLITUDE_ID,
   HSXANSTONEDURATION_ID,
   HSXAVEERRORTHRESH_ID,
   HSXNUMBITS_ID,
   HSXPXD_ID,
   HSXMODE_ID,
   HSXCMDQHIGHTHRESHOLD_ID,
   HSXCMDQLOWTHRESHOLD_ID,

   /* ID's for V.34 extended registers */

   HSXTXRATE_ID,
   HSXRATESYMMETRYSEL_ID,

   /* ID's for V.34 debug registers */

   HSXSELCTL_ID,
   HSXTCSEL_ID,
   HSXTXLVLSEL_ID,
   HSXNLCSEL_ID

} HSEVMDMREGID;


/*
 *  Set this bit in hsxrxpktsize register if the receive packet size is
 *  specified in bits rather than ms
 */
#define HAPI_VMDM_RXPKTSIZE_IN_BITS 0x8000

#endif
