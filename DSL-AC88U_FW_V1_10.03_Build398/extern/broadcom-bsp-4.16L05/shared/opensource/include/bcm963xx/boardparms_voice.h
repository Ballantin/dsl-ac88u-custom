/*
    Copyright 2000-2010 Broadcom Corporation
    
    <:label-BRCM:2011:DUAL/GPL:standard
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License, version 2, as published by
    the Free Software Foundation (the "GPL").
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    
    A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
    writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
    
    :>
*/                       

/**************************************************************************
 * File Name  : boardparms_voice.h
 *
 * Description: This file contains definitions and function prototypes for
 *              the BCM63xx voice board parameter access functions.
 *
 ***************************************************************************/

#if !defined(_BOARDPARMS_VOICE_H)
#define _BOARDPARMS_VOICE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <boardparms.h>

#define VOICE_BOARD_ID_DEFAULT         "ID_NOT_SET"

#define SI32261ENABLE    /* Temporary #def until fully supported */
#define SI32267ENABLE   /* Temporary #def until fully supported */

/* Daughtercard defines */
#define VOICECFG_NOSLIC_STR            "NOSLIC"

#define VOICECFG_LE9530_STR            "LE9530"
#define VOICECFG_LE9530_WB_STR         "LE9530_WB"
#define VOICECFG_LE9540_STR            "LE9540"
#define VOICECFG_LE9540_WB_STR         "LE9540_WB"
#define VOICECFG_LE9541_STR            "LE9541"
#define VOICECFG_LE9541_WB_STR         "LE9541_WB"
#define VOICECFG_LE9530_LE88276_STR    "LE9530_LE88276"
#define VOICECFG_LE9530_LE88506_STR    "LE9530_LE88506"
#define VOICECFG_LE9530_SI3226_STR     "LE9530_SI3226"

#define VOICECFG_SI3239_STR            "SI3239"
#define VOICECFG_SI32392_STR            "SI32392"


#define VOICECFG_LE88276_NTR_STR       "LE88276_NTR"

#define VOICECFG_LE88506_STR           "LE88506"
#define VOICECFG_LE88536_TH_STR        "LE88536_THAL"
#define VOICECFG_LE88536_ZSI_STR       "LE88536_ZSI"
#define VOICECFG_LE88264_TH_STR        "LE88264_THAL"

#define VOICECFG_VE890_INVBOOST_STR    "VE890_INVBOOST"
#define VOICECFG_LE89116_STR           "LE89116"
#define VOICECFG_LE89316_STR           "LE89316"

#define VOICECFG_VE890HV_PARTIAL_STR   "VE890HV_Partial"
#define VOICECFG_VE890HV_STR           "VE890HV"
#define VOICECFG_LE89136_STR           "LE89136"
#define VOICECFG_LE89336_STR           "LE89336"

#define VOICECFG_LE88266x2_STR         "LE88266x2"
#define VOICECFG_LE88266_STR           "LE88266"
#define VOICECFG_ZL88601_STR           "ZL88601"
#define VOICECFG_ZL88701_STR           "ZL88701"
#define VOICECFG_ZL88702_ZSI_STR       "ZL88702_ZSI"
#define VOICECFG_LE9672_ZSI_STR        "LE9672_ZSI"
#define VOICECFG_LE9662_ZSI_STR        "LE9662_ZSI"
#define VOICECFG_LE9662_ZSI_BB_STR     "LE9662_ZSI_BB"
#define VOICECFG_LE9642_ZSI_BB_STR     "LE9642_ZSI_BB"
#define VOICECFG_LE9641_ZSI_BB_STR     "LE9641_ZSI_BB"
#define VOICECFG_LE9652_ZSI_IB_STR     "LE9652_ZSI_IB"
#define VOICECFG_LE9661_ZSI_STR        "LE9661_ZSI"
#define VOICECFG_LE9661_ZSI_BB_STR     "LE9661_ZSI_BB"
#define VOICECFG_ZL88801_89010_BB_STR  "ZL88801_89010BB"

#define VOICECFG_SI3217X_STR           "SI3217X"
#define VOICECFG_SI32176_STR           "SI32176"
#define VOICECFG_SI32176_LCQC_STR      "SI32176_LCQC"
#define VOICECFG_SI32178_STR           "SI32178"
#define VOICECFG_SI3217X_NOFXO_STR     "SI3217X_NOFXO"

#define VOICECFG_SI32267_STR           "SI32267"
#define VOICECFG_SI32267_LCQC_STR      "SI32267_LCQC"
#define VOICECFG_SI32267_NTR_STR       "SI32267_NTR"

#define VOICECFG_SI32260x2_SI3050_STR  "SI32260x2_3050"
#define VOICECFG_SI32260x2_STR         "SI32260x2"
#define VOICECFG_SI32260_STR           "SI32260"
#define VOICECFG_SI32260_LCQC_STR      "Si32260_LCQC"
#define VOICECFG_SI32260_SI3050_STR    "SI32260_3050"
#define VOICECFG_SI32260_SI3050_QC_STR "SI32260_3050_QC"

/* Non-daughtercard defines */
#define VOICECFG_6368MVWG_STR                     "MVWG"

#define VOICE_OPTION_DECT_PROMPT_STR        "DECT Type Installed (0-#       :"
#define VOICE_OPTION_NO_DECT_STR            "No DECT"
#define VOICE_OPTION_INT_DECT_STR           "Internal DECT"
#define VOICE_OPTION_EXT_DECT_STR           "External DECT"

#define VOICE_OPTION_DECT_ERROR_STR         "Invalid data"
#define VOICE_OPTION_DECT_MASK              0x0000000f

#define BP_DECT_POPULATED 1 
#define BP_DECT_NOT_POPULATED 0 
/* Maximum number of devices in the system (on the board).
** Devices can refer to DECT, SLAC/SLIC, or SLAC/DAA combo. */
#define BP_MAX_VOICE_DEVICES           5 

/* Maximum numbers of channels per SLAC. */
#define BP_MAX_CHANNELS_PER_DEVICE     2

/* Maximum number of voice channels in the system.
** This represents the sum of all channels available on the devices in the system */
#define BP_MAX_VOICE_CHAN              (BP_MAX_VOICE_DEVICES * BP_MAX_CHANNELS_PER_DEVICE)

/* APM SLIC channel swap flag */
#define BP_APMCH_SWAP_OFF              0
#define BP_APMCH_SWAP_ON               1

/* Max number of GPIO pins used for controling PSTN failover relays
** Note: the number of PSTN failover relays can be larger if multiple
** relays are controlled by single GPIO */
#define BP_MAX_RELAY_PINS              2

#define BP_TIMESLOT_INVALID            0xFF

/* General-purpose flag definitions (rename as appropriate) */
#define BP_FLAG_DSP_APMHAL_ENABLE            (1 << 0)
#define BP_FLAG_DSP_PCMHAL_ENABLE            (1 << 1)
#define BP_FLAG_ISI_SUPPORT                  (1 << 2)
#define BP_FLAG_ZSI_SUPPORT                  (1 << 3)
#define BP_FLAG_THALASSA_SUPPORT             (1 << 4)
#define BP_FLAG_MODNAME_TESTNAME4            (1 << 5)
#define BP_FLAG_MODNAME_TESTNAME5            (1 << 6)
#define BP_FLAG_MODNAME_TESTNAME6            (1 << 7)
#define BP_FLAG_MODNAME_TESTNAME7            (1 << 8)
#define BP_FLAG_MODNAME_TESTNAME8            (1 << 9)

#define BP_NULL_DEVICE_MACRO_NEW     \
{                                \
   BP_VD_NONE,                   \
   BP_SPI_SS_NOT_REQUIRED,       \
   BP_RESET_NOT_REQUIRED,        \
   {                             \
      { BP_VOICE_CHANNEL_INACTIVE, BP_VCTYPE_NONE, BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE, BP_VOICE_CHANNEL_NARROWBAND, BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS, BP_VOICE_CHANNEL_ENDIAN_BIG, BP_TIMESLOT_INVALID, BP_TIMESLOT_INVALID }, \
      { BP_VOICE_CHANNEL_INACTIVE, BP_VCTYPE_NONE, BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE, BP_VOICE_CHANNEL_NARROWBAND, BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS, BP_VOICE_CHANNEL_ENDIAN_BIG, BP_TIMESLOT_INVALID, BP_TIMESLOT_INVALID }, \
   }                             \
}

/* 
 * Use this macro for boards which do not support DECT in place of the 
 * channel list in the voice parameters struct. 
 */
#define BP_DECT_NOT_SUPPORTED { BP_NULL_CHANNEL_DESCRIPTION_MACRO }


/*
 * Use this macro for boards which support internal DECT.
 */
#define BP_DECT_INTERNAL  {							\
   { 														\
      BP_VOICE_CHANNEL_ACTIVE,					\
      BP_VCTYPE_DECT,								\
      BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,		\
      BP_VOICE_CHANNEL_WIDEBAND,					\
      BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,	\
      BP_VOICE_CHANNEL_ENDIAN_BIG,				\
      BP_TIMESLOT_INVALID,							\
      BP_TIMESLOT_INVALID							\
   },														\
   {  													\
      BP_VOICE_CHANNEL_ACTIVE,					\
      BP_VCTYPE_DECT,								\
      BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,		\
      BP_VOICE_CHANNEL_WIDEBAND,					\
      BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,	\
      BP_VOICE_CHANNEL_ENDIAN_BIG,				\
      BP_TIMESLOT_INVALID,							\
      BP_TIMESLOT_INVALID							\
   },														\
   { 														\
      BP_VOICE_CHANNEL_ACTIVE,					\
      BP_VCTYPE_DECT,								\
      BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,		\
      BP_VOICE_CHANNEL_WIDEBAND,					\
      BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,	\
      BP_VOICE_CHANNEL_ENDIAN_BIG,				\
      BP_TIMESLOT_INVALID,							\
      BP_TIMESLOT_INVALID							\
   },														\
   {  													\
      BP_VOICE_CHANNEL_ACTIVE,					\
      BP_VCTYPE_DECT,								\
      BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,		\
      BP_VOICE_CHANNEL_WIDEBAND,					\
      BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,	\
      BP_VOICE_CHANNEL_ENDIAN_BIG,				\
      BP_TIMESLOT_INVALID,							\
      BP_TIMESLOT_INVALID							\
   },														\
			   											\
   BP_NULL_CHANNEL_DESCRIPTION_MACRO			\
}
	
#define BP_MAX_SUPPORTED_DC 20
#define BP_MAX_DECT_DEVICE 2
#define BP_MAX_DC_SPI_DEVICE 3
#define BP_NULL_CHANNEL_DESCRIPTION_MACRO     \
      { BP_VOICE_CHANNEL_NONE, BP_VCTYPE_NONE, BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE, BP_VOICE_CHANNEL_NARROWBAND, BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS, BP_VOICE_CHANNEL_ENDIAN_BIG, BP_TIMESLOT_INVALID, BP_TIMESLOT_INVALID } 

/* To avoid conflict with GPIO pin defines */
#define BP_NOT_CONNECTED BP_NOT_DEFINED
#define BP_DEDICATED_PIN BP_GPIO_NONE

typedef enum
{
   BP_VOICE_NO_DECT,
   BP_VOICE_INT_DECT,
   BP_VOICE_EXT_DECT
}BP_VOICE_DECT_TYPE;

/* 
** Device-specific definitions 
*/
typedef enum
{
   BP_VD_NONE = -1,
   BP_VD_IDECT1,  /* Do not move this around, otherwise rebuild dect_driver.bin */
   BP_VD_EDECT1,  
   BP_VD_SILABS_3050, /* SILAB IDs till BP_VD_SILABS_MAXID. Do NOT mess up Zarlink and Silab IDs. Put SILAB IDs before Zarlink's always*/
   BP_VD_SILABS_3215,
   BP_VD_SILABS_3216,
   BP_VD_SILABS_3217,
   BP_VD_SILABS_32176,
   BP_VD_SILABS_32178,
   BP_VD_SILABS_3226,
   BP_VD_SILABS_32260,
   BP_VD_SILABS_32261,
   BP_VD_SILABS_32267,
   BP_VD_SILABS_3239,
   BP_VD_SILABS_32392,
   BP_VD_SILABS_MAXID,
   
   BP_VD_ZARLINK_88010,/* ZARLINK IDs till BP_VD_MAX */
   BP_VD_ZARLINK_88221,
   BP_VD_ZARLINK_88266,
   BP_VD_ZARLINK_88276,
   BP_VD_ZARLINK_88506,
   BP_VD_ZARLINK_88536,
   BP_VD_ZARLINK_88264,
   BP_VD_ZARLINK_89010,
   BP_VD_ZARLINK_89116,
   BP_VD_ZARLINK_89316,
   BP_VD_ZARLINK_9530,
   BP_VD_ZARLINK_9540,
   BP_VD_ZARLINK_9541,
   BP_VD_ZARLINK_89136,
   BP_VD_ZARLINK_89336,
   BP_VD_ZARLINK_88601,
   BP_VD_ZARLINK_88701,
   BP_VD_ZARLINK_88702_ZSI,
   BP_VD_ZARLINK_9662,
   BP_VD_ZARLINK_9661,
   BP_VD_ZARLINK_88801,
   BP_VD_ZARLINK_9672_ZSI,
   BP_VD_ZARLINK_9642_ZSI,
   BP_VD_ZARLINK_9641_ZSI,
   BP_VD_ZARLINK_9652_ZSI_IB,
   BP_VD_MAX,
} BP_VOICE_DEVICE_TYPE;

typedef enum
{
   BP_VD_FLYBACK,
   BP_VD_FLYBACK_TH,
   BP_VD_BUCKBOOST,
   BP_VD_INVBOOST,
   BP_VD_INVBOOST_TH,
   BP_VD_QCUK,
   BP_VD_FIXEDRAIL,
   BP_VD_MASTERSLAVE_FB,
   BP_VD_FB_TSS,
   BP_VD_FB_TSS_ISO,
   BP_VD_PMOS_BUCK_BOOST,
   BP_VD_LCQCUK,
} BP_VOICE_DEVICE_PROFILE;

/* 
** Channel-specific definitions 
*/

typedef enum
{
   BP_VOICE_CHANNEL_NONE=-1,
   BP_VOICE_CHANNEL_ACTIVE,
   BP_VOICE_CHANNEL_INACTIVE,
} BP_VOICE_CHANNEL_STATUS;

typedef enum
{
   BP_VCTYPE_NONE = -1,
   BP_VCTYPE_SLIC,
   BP_VCTYPE_DAA,
   BP_VCTYPE_DECT
} BP_VOICE_CHANNEL_TYPE;

typedef enum
{
   BP_VOICE_CHANNEL_SAMPLE_SIZE_16BITS,
   BP_VOICE_CHANNEL_SAMPLE_SIZE_8BITS,
} BP_VOICE_CHANNEL_SAMPLE_SIZE;

typedef enum
{
   BP_VOICE_CHANNEL_NARROWBAND,
   BP_VOICE_CHANNEL_WIDEBAND,
} BP_VOICE_CHANNEL_FREQRANGE;


typedef enum
{
   BP_VOICE_CHANNEL_ENDIAN_BIG,
   BP_VOICE_CHANNEL_ENDIAN_LITTLE,
} BP_VOICE_CHANNEL_ENDIAN_MODE;

typedef enum
{
   BP_VOICE_CHANNEL_PCMCOMP_MODE_NONE,
   BP_VOICE_CHANNEL_PCMCOMP_MODE_ALAW,
   BP_VOICE_CHANNEL_PCMCOMP_MODE_ULAW,
} BP_VOICE_CHANNEL_PCMCOMP_MODE;


typedef struct
{
   unsigned int  status;        /* active/inactive */
   unsigned int  type;          /* SLIC/DAA/DECT */
   unsigned int  pcmCompMode;   /* u-law/a-law (applicable for 8-bit samples) */
   unsigned int  freqRange;     /* narrowband/wideband */
   unsigned int  sampleSize;    /* 8-bit / 16-bit */
   unsigned int  endianMode;    /* big/little */
   unsigned int  rxTimeslot;    /* Receive timeslot for the channel */
   unsigned int  txTimeslot;    /* Sending timeslot for the channel */

} BP_VOICE_CHANNEL;

typedef struct
{
   int                  spiDevId;               /* SPI device id */
   unsigned short       spiGpio;                /* SPI GPIO (if used for SPI control) */
} BP_VOICE_SPI_CONTROL;

typedef struct
{
   unsigned short       relayGpio[BP_MAX_RELAY_PINS];
} BP_PSTN_RELAY_CONTROL;

typedef struct
{
   unsigned short       dectUartGpioTx;
   unsigned short       dectUartGpioRx;
} BP_DECT_UART_CONTROL;

typedef struct
{
   unsigned int         voiceDeviceType;        /* Specific type of device (Le88276, Si32176, etc.) */
   BP_VOICE_SPI_CONTROL spiCtrl;                /* SPI control through dedicated SPI pin or GPIO */
   int                  requiresReset;          /* Does the device requires reset (through GPIO) */
   unsigned short       resetGpio;              /* Reset GPIO */
   BP_VOICE_CHANNEL     channel[BP_MAX_CHANNELS_PER_DEVICE];   /* Device channels */

} BP_VOICE_DEVICE_MID_LAYER;

typedef struct 
{
   int 					nDeviceType;			/* Specific type of device (Le88267, Si32176, etc.) */
   int 					nSPI_SS_Bx;				/* SPI Control */
   int 					nRstPin;				/* Reset pin */
   BP_VOICE_CHANNEL     channel[BP_MAX_CHANNELS_PER_DEVICE];	/* Device channels */
  
} BP_VOICE_DEVICE;

typedef enum
{
   SPI_DEV_0,
   SPI_DEV_1,
   SPI_DEV_2,
   SPI_DEV_3,
   SPI_DEV_4,
   SPI_DEV_5,
   SPI_DEV_6,
   SPI_DEV_7
   
} BP_SPI_PORT;

typedef enum
{
   BP_SPI_SS_NOT_REQUIRED=-1,
   BP_SPI_SS_B1,
   BP_SPI_SS_B2,
   BP_SPI_SS_B3,
   
} BP_SPI_SIGNAL;

typedef enum
{
   BP_RESET_NOT_REQUIRED=-1,
   BP_RESET_FXS1,
   BP_RESET_FXS2,
   BP_RESET_FXO
   
} BP_RESET_PIN;

typedef struct 
{
   int numSpiPort;
   int numGpio;
   
} BP_VOICE_SPI_PORT_CS;

typedef struct
{
   int nDeviceType;
   int nSPI_SS_Bx;
   BP_VOICE_CHANNEL voiceChDes;
   
} BP_VOICE_CHANNEL_ATT;

/*
** Main structure for defining the board parameters and used by boardHal
** for proper initialization of the DSP and devices (SLACs, DECT, etc.)
*/
typedef struct VOICE_BOARD_PARMS
{
   char                    szBoardId[BP_BOARD_ID_LEN];      /* daughtercard id string */
   char                    szBaseBoardId[BP_BOARD_ID_LEN];  /* motherboard id string */
   unsigned int            numFxsLines;            /* Number of FXS lines in the system */
   unsigned int            numFxoLines;            /* Number of FXO lines in the system */
   unsigned int            numDectLines;           /* Number of DECT lines in the system */
   unsigned int            numFailoverRelayPins;   /* Number of GPIO pins controling PSTN failover relays */
   BP_VOICE_DEVICE_MID_LAYER voiceDevice[BP_MAX_VOICE_DEVICES];  /* Voice devices in the system */
   BP_PSTN_RELAY_CONTROL   pstnRelayCtrl;          /* Control for PSTN failover relays */
   BP_DECT_UART_CONTROL    dectUartControl;        /* Control for external DECT UART */
   unsigned int            deviceProfile;          /* Battery configuration, if required */
   unsigned int            apmChannelSwap;         /* APM SLIC channel hardware design swapped flag */
   unsigned int            flags;                  /* General-purpose flags */
   
} VOICE_BOARD_PARMS, *PVOICE_BOARD_PARMS;

typedef struct 
{
   char                    szBoardId[BP_BOARD_ID_LEN];      /* daughtercard id string */
   unsigned int            numFxsLines;                     /* Number of FXS lines in the system */
   unsigned int            numFxoLines;                     /* Number of FXO lines in the system */
   BP_VOICE_DEVICE         voiceDevice[BP_MAX_VOICE_DEVICES];  /* Voice devices in the system */   
   unsigned int            deviceProfile;                   /* Battery configuration, if required */
   unsigned int            flags;                           /* General-purpose flags */
   
} VOICE_DAUGHTER_BOARD_PARMS, *PVOICE_DAUGHTER_BOARD_PARMS;


/* Function prototypes */


#if !defined(_CFE_)
int BpGetVoiceParms( char *pszVoiceDaughterCardId, VOICE_BOARD_PARMS* voiceParms, char* pszBaseBoardId );
int BpGetNTRExternReference( void );
#endif /* !defined(_CFE_) */
int BpDectPopulated( void ); 
void BpSetDectPopulatedData( int BpData );
int BpSetVoiceBoardId( char *pszVoiceDaughterCardId );
int BpGetVoiceBoardId( char *pszVoiceDaughterCardId );
int BpGetVoiceBoardIds( char *pszVoiceDaughterCardIds, int nBoardIdsSize, char *pszBaseBoardId );
int BpGetVoiceDectType( char *pszBaseBoardId );

#if !defined(_CFE_)
void PrintAllParms(VOICE_BOARD_PARMS *parms);
#endif

#ifdef __cplusplus
}
#endif

#endif /* _BOARDPARMS_VOICE_H */

