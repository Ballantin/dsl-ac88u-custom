
/****************************************************************************
*
*  HAPI_GFAX.H - HAUSWARE generic FAX modem commands and events.
*
*  PURPOSE:
*     This file contains definitions common to the V.FAX and V.17 Services
*     
**********************************************************************/

#ifndef HAPI_GFAX                            /* support nested includes */
#define HAPI_GFAX

/* FAX MODEM SERVICE COMMANDS */

#define  HAPIGFAXENABLE          0x0      /* enable and reset modem  */
#define  HAPIGFAXGETSTATUS       0x1      /* get modem status        */
#define  HAPIGFAXSETREGS         0x2      /* set registers           */
#define  HAPIGFAXGETREGS         0x3      /* get registers           */
#define  HAPIGFAXRESETREGS       0x4      /* reset registers         */
#define  HAPIGFAXTXCFG           0x5      /* configure transmitter   */
#define  HAPIGFAXTXDATA          0x6      /* transmit data           */
#define  HAPIGFAXRXCFG           0x9      /* configure receiver      */
#define  HAPIGFAXSETIDLEPTN      0xA      /* Set idle pattern        */
#define  HAPIGFAXSETREG1         0xB      /* set one register           */


/* FAX MODEM SERVICE EVENTS */

#define  HAPIGFAXTXDATASENT      0x10     /* the current transmit data buffer is
                                             sent */
#define  HAPIGFAXRXDATA          0x11     /* one receive buffer is filled and
                                             available for reading */
#define  HAPIGFAXRXTURNON        0x12     /* fax turn on sequence detected */
#define  HAPIGFAXRXTCFERR        0x14     /* number of bit errors and maximum
                                             error measured during TCF */
#define  HAPIGFAXRXDATAEND       0x15     /* incoming data signal has ended
                                             or the modem has lost
                                             synchronization */
#define  HAPIGFAXTXCMDQLOW       0x16     /* command queue length below low
                                             threshold */
#define  HAPIGFAXTXCMDQHIGH      0x17     /* command queue length above high
                                             threshold */
#define  HAPIGFAXTXDATAEND       0x18     /* transmitter has completed 
                                             sending data */
#define  HAPIGFAXMODEMRDY        0x19     /* segment 4 has been transmitted
                                             or received */
#define  HAPIGFAXTXDATACLR       0x1A     /* transmitter has completed sending
                                             data but there are commands in 
                                             transmit command queue which
                                             need to be cleared      */
#define  HAPIGFAXNOCARRIER       0x1B     /* receiver has lost carrier */
#define  HAPIGFAXTRAINRX         0x1C     /* receiver detected train */
#define  HAPIGFAXDATASENT        0x1D     /* number of bits sent */
#define  HAPIGFAXBMDEBUG         0x1E     /* number of bits sent */
#define  HAPIGFAXRXAVEERROR      0x20     /* returns average error for debugging */

typedef enum
{
   HSGFAX_0 = 0,                          /* NULL rate */
   HSGFAX_2400,                           /* V.27ter 2.4 kbps */
   HSGFAX_4800,                           /* V.27ter 4.8 kbps */
   HSGFAX_7200,                           /* V.29 7.2 kbps */
   HSGFAX_9600,                           /* V.29 9.6 kbps */
   HSGFAX_12000_V33,                      /* V.33 12 kbps */
   HSGFAX_14400_V33,                      /* V.33 14.4 kbps */
   HSGFAX_7200_V17,                       /* V.17 7.2 kbps */
   HSGFAX_9600_V17,                       /* V.17 9.6 kbps */
   HSGFAX_12000_V17,                      /* V.17 12 kbps */
   HSGFAX_14400_V17,                      /* V.17 14.4 kbps */
   HSGFAXNUMRATES                         /* number of enumerated rates */

} HSEGFAXRATE;

typedef enum                              /* TX CONFIGURATION DEFINITIONS */
{
   HSGFAXTXCFGIDLE = 0,                   /* put transmitter in idle mode */
   HSGFAXTXCFGTURNON,                     /* transmit turn on and data */
   HSGFAXTXCFGTRAIN,                      /* transmit training check */
   HSGFAXTXCFGEXTTURNON                   /* transmit extended turnon and 
                                             data (for V.17) */
} HSEGFAXTXCFG;

typedef enum                              /* RX ENABLE DEFINITIONS */
{
   HSGFAXRXCFGOFF = 0,                    /* put receiver in idle mode */
   HSGFAXRXCFGON                          /* put receiver in active mode */

} HSEGFAXRXENABLE;

typedef enum                              /* RX CONFIGURATION DEFINITIONS */
{
   HSGFAXRXCFGTRAIN = 0,                  /* receive training check */
   HSGFAXRXCFGTURNON,                     /* receive turnon and data */
   HSGFAXRXCFGEXTTURNON,                  /* receive extended turnon and 
                                             data (for V.17) */
   HSGFAXRXCFGTCFDATA,																					/* receive TCF in data mode  */
   HSGFAXRXCFGECMDATA,                     /* receive ECM data with short training         */
			HSGFAXRXCFGEXTECMDATA																			/* receive ECM data with long train */

} HSEGFAXRXCFG;

/*
 *  Set this bit in hsxrxpktsize register if the receive packet size is
 *  specified in bits rather than ms
 */
#define HAPI_GFAX_RXPKTSIZE_IN_BITS 0x8000


#endif

