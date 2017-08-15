/****************************************************************************
*
*  Copyright (c) 2000-2009 Broadcom Corporation
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
*    Filename: vrgEndptModem.c
*
****************************************************************************
*    Description:
*
*      This file contains the implementation of 2G/3G voice processing.
*
****************************************************************************/

#ifdef VRG_ENDPT_2G3G_SUPPORT

/*
*****************************************************************************
** INCLUDE FILES
*****************************************************************************
*/
#include <string.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <vrgEndptCfg.h>
#include <hapi_net.h>                  /* Net VHD API */
#include <hapi_pte.h>                  /* PTE Service */
#include <hapi_t38.h>                  /* FXR Service */
#include <hapi_faxr.h>                 /* FXR Service */
#include <hapi_cdis.h>                 /* CDIS Service */
#include <hapi_clidtx.h>               /* CLIDTX Service */
#include <hapi_rtp.h>                  /* RTP Service */
#include <endpt.h>                     /* Endpoint include file */
#include <codec.h>                     /* codec map */
#include <ctype.h>
#include <bosSem.h>
#include <bosMisc.h>
#include <bosTask.h>
#include <bosSleep.h>
#include <bosSocket.h>
#if VRG_ENDPT_CFG_USE_HFE
#include <bosTimer.h>
#endif

#if BRCM_DUAL_3341
#include <hapi.h>
#endif

#include <hapi_rm.h>
#include <hapi_tone.h>
#include <hapi_gvad_defs.h>
#include <hapi_wrap_defs.h>
#include <hapi_gain.h>
#include <haus_hapi.h>
#include <hapi_swb.h>
#include <hapi_hec.h>

#include <hdspCmd.h>                   /* DSP interface */
#include <hdspTone.h>                  /* DSP tone interface */
#include <vrgTone.h>
#include <vrgClass.h>
#include <hapi_cmtd.h>                 /* CMTD Service */
#include <hapi_dtas.h>                 /* DTAS Service */
#include <hdspInit.h>
#include <hdspVhd.h>
#include <hdspRm.h>
#include <hdspHal.h>
#include <hdspIcp.h>

#include <boardHalInit.h>
#include <boardHalApm.h>
#include <boardHalCas.h>
#include <boardHalDsp.h>
#include <boardHalDaa.h>
#include <boardHalSlic.h>
#include <boardHalPower.h>
#include <boardHalDect.h>
#include <boardHalProv.h>
#include <vrgEndpt.h>                  /* VRG local header file */
#include <vrgProv.h>                   /* Endpoint provisioning */
#include "vrgEndptGlobe.h"             /* Endpoint around the "Globe" interface */
#include <classStm.h>                  /* CLASS state machine */
#include <casCtl.h>                    /* CAS signalling support */
#include <vrgRing.h>                   /* Provisioned Ring Cadences */
#include <cmtdCtl.h>
#include <pstnCtl.h>                   /* PSTN call setup support */
#include <xdrvDaa.h>                   /* DAA driver interface */
#if VRG_ENDPT_CFG_USE_TPD
#include <tpdCtl.h>                    /* TPD testing support */
#endif
#if VRG_ENDPT_CFG_USE_MLT
#include <mltApi.h>
#endif

#include <debug.h>                     /* Debug module interface */
#include <heartbeat.h>                 /* Heartbeat monitoring module */

#include <boardHalIpc.h>
#include <lhapiExtension.h>

#include <str.h>
#include <memCircBuf.h>
#include <bosTime.h>

#include <leds.h>                      /* LED control wappers */

/* These are needed for the randomization functions  */
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/types.h>
#include <linux/spinlock.h>

#include <asm/uaccess.h>

#include "vrgEndptPrivate.h"
#include "vrgEndptModem.h"

#include <linux/types.h>
#include <linux/delay.h>
#include <linux/timer.h>

/*
*****************************************************************************
** CONSTANTS
*****************************************************************************
*/
/*
 ***************************************************************************
 * EXPORTED VARIABLES
 ***************************************************************************
*/
BOS_SEM   vrgCSEncodeNotifySem;

BOS_TASK_ID vrgCSMediaProcessTaskId = 0;
void VrgCSMediaTxTaskMain( BOS_TASK_ARG taskArg );


/* CS (Circuit Switched 2G/3G ) media stream info */
volatile unsigned int gAudioULReady = 0;
volatile unsigned int gAudioULStart = 0;
volatile MODEM_STREAM_INFO gCSMedia[MODEM_MAX_AUDIO_STREAM];

/*
*****************************************************************************
** LOCAL VARIABLES
*****************************************************************************
*/
static struct timer_list syncTimer;


static   int rx_handle;
static   unsigned char resp_packet[AUDIO_BUFF_SIZE] = {0};
static   unsigned char rx_media[AUDIO_BUFF_SIZE] = {0};
static   unsigned char csPacket[AUDIO_BUFF_SIZE] = {0};
static   unsigned char audioPayload[AUDIO_BUFF_SIZE] = {0};
static   unsigned char gsm_tx_audio_buffer[MODEM_MAX_AUDIO_STREAM][AUDIO_BUFF_SIZE];
static   unsigned int  gNextSyncTimeStamp;
static   unsigned int  localTs = 100;

/* Cherry pick bits for EFR CRC calculations */
static unsigned char efr_lookup_table[] = {
    38,  39,  40,  41,  42,  43,  47,  86,  44,   1,
     2,   7,   9,  17,  18,  23,  45,  46, 141, 142,
   143, 144, 145, 146,  91,  92, 194, 195,  97, 136,
   147,  93, 196, 148, 149,  94, 197,   3,   4,  10,
    11,  15,   8,   5,   6,  12,  16,  19,  95, 198,
     0,  13,  14,  20,  24,  25,  27, 150, 200, 189,
   239,  87, 137, 190, 240
};

static unsigned short efr_scramble_table[] = {
   39,  40,  41,  42,  43,  44, 146, 147, 148, 149,
  150, 151,  94,  95, 201, 202,  48,  89, 100, 141,
   45, 152,  96, 203,   2,   3,   8,  10,  18,  19,
   24,  46,  47, 153, 154,  97, 204,   4,   5,  11,
   12,  16,   9,   6,   7,  13,  17,  20,  98, 205,
    1,  14,  15,  21,  25,  26,  28, 155, 207, 196,
  248,  90, 142, 197, 249, 253, 254, 255, 256, 257,
  258, 259, 260,  49, 101, 156, 208,  22,  23,  27,
   29,  52,  56,  60,  64,  68, 104, 108, 112, 116,
  120, 159, 163, 167, 171, 175, 211, 215, 219, 223,
  227,  91, 143, 198, 250,  50, 102, 157, 209,  30,
   31,  32,  33,  34,  35,  36,  99, 206,  53,  57,
   61,  65,  69, 105, 109, 113, 117, 121, 160, 164,
  168, 172, 176, 212, 216, 220, 224, 228,  54,  58,
   62,  66, 106, 110, 114, 118, 161, 165, 169, 173,
  213, 221, 225,  92, 144, 199, 251,  51, 103, 158,
  210,  93, 145, 200, 252,  55,  59,  63,  67, 107,
  111, 115, 119, 162, 166, 170, 174, 214, 222, 226,
   37,  38,  70,  72,  73, 122, 124, 125, 177, 179,
  180, 229, 231, 232, 217, 218,  71, 123, 178, 230,
   74,  77,  80,  83,  86, 126, 129, 132, 135, 138,
  181, 184, 187, 190, 193, 233, 236, 239, 242, 245,
   75,  78,  81,  84,  87, 127, 130, 133, 136, 139,
  182, 185, 188, 191, 194, 234, 237, 240, 243, 246,
   76,  79,  82,  85,  88, 128, 131, 134, 137, 140,
  183, 186, 189, 192, 195, 235, 238, 241, 244, 247
};


static unsigned short fr_scramble_table[] = {
    6,  53, 109, 165, 221,   5,  12,  17,   4,  11,
   16,  22,  43,  99, 155, 211,  52, 108, 164, 220,
   10,  26,  30,  42,  98, 154, 210,  41,  97, 153,
  209,  40,  96, 152, 208,  39,  95, 151, 207,  51,
  107, 163, 219,   3,  21,  33,  38,  94, 150, 206,
   25,  29,  45, 101, 157, 213,  37,  93, 149, 205,
   47, 103, 159, 215,   2,   9,  15,  36,  20,  24,
   32,  44, 100, 156, 212,  50, 106, 162, 218,  56,
   59,  62,  65,  68,  71,  74,  77,  80,  83,  86,
   89,  92, 112, 115, 118, 122, 124, 127, 130, 133,
  136, 139, 142, 145, 148, 168, 171, 174, 177, 180,
  183, 186, 189, 192, 195, 198, 201, 204, 224, 227,
  230, 233, 236, 239, 242, 245, 248, 251, 254, 257,
  260,  46, 102, 158, 214,  49, 105, 161, 217,  55,
   58,  61,  64,  67,  70,  73,  76,  79,  82,  85,
   88,  91, 111, 114, 117, 120, 123, 126, 129, 132,
  135, 138, 141, 144, 147, 167, 170, 173, 176, 179,
  182, 185, 188, 191, 194, 197, 200, 203, 223, 226,
  229, 232, 235, 238, 241, 244, 247, 250, 253, 256,
  259,   1,   8,  14,  28,  31,  35,  34,  13,  19,
   18,  23,  48, 104, 160, 216,  54,  57,  60,  63,
   66,  69,  72,  75,  78,  81,  84,  87,  90, 110,
  113, 116, 119, 122, 125, 128, 131, 134, 137, 140,
  143, 146, 166, 169, 172, 175, 178, 181, 184, 187,
  190, 193, 196, 199, 202, 222, 225, 228, 231, 234,
  237, 240, 243, 246, 249, 252, 255, 258,   7,  27
};


static unsigned short hr_voice_scramble_table[] = {
   14,  15,  19,  20,  21,  54,  72,  90, 108,  55,
   73,  91, 109,  56,  74,  92, 110,  45,  46,  47,
   48,  49,  50,  51,  52,  53,  63,  64,  65,  66,
   69,  70,  71,  81,  67,  68,  57,  75,  93, 111,
   58,  76,  94, 112,  34,  25,  33,  98,  32,  24,
   97,  80,  62,  44,  96,  79,  61,  43,  31,  30,
   29,  23,  28,  27,  22,   5,  26,  16,  95,  78,
   60,  42,   4,  77,  59,  41,  40,  18,  17,  13,
   12,  11,  10,   3,  39,  38,  37,   9,   8,   7,
    6,   2,   1,  36,  35, 107, 106, 105, 104, 103,
  102, 101, 100,  99,  89,  88,  87,  86,  85,  84,
   83,   82
};

static unsigned short hr_unvoice_scramble_table[] = {
    4,  26,  53,  72,  91, 110,  16,  20,  21,  22,
   23,  24,  27,  28,  29,  30,  31,  32,  62,  63,
   64,  65,  66,  67,  68,  69,  75,  76,  77,  78,
   79,  80,  81,  82,  83,  84,  85,  33,   5,  34,
   61,  60,  59,  58,  57,  56,  50,  49,  48,  47,
   46,  45,  44,  43,  42,  41,  40,  39,  38,  37,
  112,  93,  74,  55,  25, 111,  92,  73,  54,  15,
   14,  13,  12,  11, 109,  90,  71,  52,  19,  18,
   17, 108,  89,  70,  51,  10,   9,   8,   7,   3,
    6,   2,   1,  36,  35, 107, 106, 105, 104, 103,
  102, 101, 100,  99,  98,  97,  96,  95,  94,  88,
   87,  86
};


static unsigned char efr_silence[] = {
  0x55, 0xF5, 0x35, 0xE9, 0xDA, 0xA5, 0x6B, 0x07, 0xD6,
  0x61, 0x00, 0x00, 0x00, 0x1E, 0x05, 0x00, 0x00, 0x00,
  0x00, 0xE0, 0x01, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0xFF, 0x00, 0x00 };


static unsigned char hr_silence[] = {
  0x64, 0x01, 0xE5, 0xB8, 0x00, 0x80, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };


static unsigned char fr_silence[] = {
  0x0A, 0x82, 0x15, 0x2E, 0x00, 0xA5, 0x92, 0x04, 0x24,
  0x49, 0x45, 0x92, 0x04, 0x00, 0x49, 0x92, 0x92, 0x24,
  0x00, 0x45, 0x92, 0x04, 0x24, 0x49, 0x45, 0x92, 0x04,
  0x00, 0x49, 0x92, 0x92, 0x24, 0x00, 0x40};


unsigned int get_rx_time_stamp (unsigned char* buffer, tISI_ModemAudioSpcDecoderDataReq *pDataReq)
{

  unsigned int timeStamp = 0;
  unsigned short frameLength;
  int streamId = 0;

  if (gCSMedia[streamId].codecUse == SPC_USE_GSM_MODEM) {

    frameLength = GETB16( &pDataReq->length ) + 6 - 4;

    switch (gCSMedia[streamId].dataFormatRequest){
    case SPC_UNI_GSM_EFR:
    {
      char *ptr, tmp;

      ptr = (char*)(buffer + frameLength - 2);
      /* This is hack to make EFR happy */
      tmp = ptr[1];
      ptr[1] = ptr[0];
      ptr[0] = tmp;
      timeStamp = *(unsigned int*) (ptr);
    }
    break;
    default:
      timeStamp = GETB32(buffer + frameLength - sizeof (unsigned int ));
      localTs += 160;
      timeStamp = localTs;
    }
  }
  else {
    frameLength = GETB16( &pDataReq->length ) + 6;
    timeStamp = *(unsigned int*) (buffer + frameLength - sizeof (unsigned int ));
  }
  return timeStamp;
}

/***************************************************************************
* FUNCTION:    scramble_audio
* PURPOSE:     scrambled bits before transmission
* PARAMETERS:  in: un scrambled buffer
*              out: scrambled buffer
*   scramble_table: Table describing the bits to scramble
*       total_bits: total number of bits
* RETURNS:
* NOTE:
****************************************************************************/
void scramble_audio( char* in, char* out, unsigned short* scramble_table, int total_bits)
{
  char bit;
  int i;
  int bitAddr = 0;

  memset((void*)out, 0, total_bits/8);

  for (i = 0; i < total_bits; i++ ){
    bitAddr = scramble_table[i] - 1;
    bit = (in[bitAddr >> 3] & (1 << (7 - (bitAddr & 0x7)))) ? 1 : 0;
    out[i >> 3] |= bit << (7 - (i & 0x7));
  }
}


/***************************************************************************
* FUNCTION:    descramble_audio
* PURPOSE:     de-scrambled bits for efr codec
* PARAMETERS:  in: scrambled buffer
*              out: de-scrambled buffer
*   scramble_table: Table describing the bits to scramble
*       total_bits: total number of bits
* RETURNS:
* NOTE:
****************************************************************************/
void descramble_audio ( char* in, char* out, unsigned short* scramble_table, int total_bits)
{
  char bit;
  int i;
  int bit_addr = 0;

  memset((void*)out, 0, total_bits/8);

  for (i = 0; i < total_bits; i++ ){
    bit = (in[i >> 3] & (1 << (7 - (i & 0x7)))) ? 1 : 0;
    bit_addr = scramble_table[i] - 1;
    out[bit_addr >> 3] |= bit << (7 - (bit_addr & 0x7));
  }
}

/***************************************************************************
* FUNCTION:    getCrc
* PURPOSE:     calculate CRC
* PARAMETERS:  message: selected bits to use for crc
*              numBits: Number of bits
* RETURNS:     computed CRC
* NOTE:
****************************************************************************/
unsigned char getCrc(unsigned char  message[], int numBits)
{
    unsigned int  remainder = 0;
    unsigned char bits, one_or_zero;

#define POLYNOMIAL  0x11D // 1 0 0 0 1  1 1 0 1
#define NORM_POLYNOMIAL (0x11D << 23)

    /* collect first 9 bits */
    remainder = message[0] << 24;
    remainder |= message[1] << 16;
    remainder &= 0xFF800000;

    /*
     * Perform modulo-2 division over 9 bits
     */
    for (bits = 9; bits <= numBits + 8; ++bits)
    {
         if (remainder & 0x80000000) {
          remainder = remainder ^ NORM_POLYNOMIAL;
        }
        remainder <<= 1;
        one_or_zero = (message[bits >> 3] & (1 << (7 - (bits & 0x7)))) ? 1 : 0;
        remainder |=  one_or_zero << 23;
    }

    /*
     * The final remainder is the CRC result.
     */
    return ((remainder & 0xFF000000) >> 24);
}   /* getCrc */

/***************************************************************************
* FUNCTION:    efr_crc_calculate
* PURPOSE:     Calculate 8 bit CRC from 65 cherry picked bits
* PARAMETERS:  buffer: audio frame from EFR encoder
* RETURNS:     computed CRC
* NOTE:
****************************************************************************/
unsigned char efr_crc_calculate ( char* buffer )
{
  /* Need space for 65 bits */
  char crcBuffer[9];
  int i = 0;
  int bitAddr = 0;
  int totalBits = sizeof (efr_lookup_table);
  char bit;
  unsigned char crc = 0;

  memset((void*)crcBuffer, 0, 9);

  /* Extarct bits using look-up table */
  for ( i = 0; i < totalBits; i++ ){
    bitAddr = efr_lookup_table[i];
    bit = (buffer[bitAddr >> 3] & (1 << (7 - (bitAddr & 0x7)))) ? 1 : 0;
    crcBuffer[i >> 3] |= bit << (7 - (i & 0x7));
  }

  crc = getCrc (crcBuffer, totalBits);

  return crc;
}

/***************************************************************************
* FUNCTION:    efr_encode_audio
* PURPOSE:     Encode efr audio for 2G transmission
* PARAMETERS:  inBuffer: audio frame from EFR encoder
* RETURNS:
* NOTE:
****************************************************************************/
void efr_encode_audio ( char* inBuffer, char* outBuffer, char crc )
{
  char bit;
  int k = 0;

  /* Clear all output buffer */
  memset(outBuffer, 0, 33);
  /* copy first 71 (0-70) bits */
  memcpy ((void*)outBuffer, (void*)inBuffer, 9);
  /* clear 72th bit (bit 71) */
  outBuffer[8] &= 0xFE;

  /* Process output bits 73 - 122 */
  for ( k = 73; k <= 122; k++ ){
    bit = (inBuffer[(k - 2) >> 3] & (1 << ( 7 - (( k - 2) & 0x7)))) ? 1 : 0;
    outBuffer[ k >> 3] |= bit << (7 - ( k & 0x7 ));
  }
  /* Process output bits 125 - 177 */
  for ( k = 125; k <= 177; k++ ){
    bit = (inBuffer[(k - 4) >> 3] & (1 << ( 7 - (( k - 4) & 0x7)))) ? 1 : 0;
    outBuffer[ k >> 3] |= bit << (7 - ( k & 0x7 ));
  }
  /* Process output bits 180 - 229 */
  for ( k = 180; k <= 229; k++ ){
    bit = (inBuffer[(k - 6) >> 3] & (1 << ( 7 - (( k - 6) & 0x7)))) ? 1 : 0;
    outBuffer[ k >> 3] |= bit << (7 - ( k & 0x7 ));
  }
  /* Process output bits 232 - 251 */
  for ( k = 232; k <= 251; k++ ){
    bit = (inBuffer[(k - 8) >> 3] & (1 << ( 7 - (( k - 8) & 0x7)))) ? 1 : 0;
    outBuffer[ k >> 3] |= bit << (7 - ( k & 0x7 ));
  }

  /* Process the repitition bits */
  bit = inBuffer[69 >> 3] & (1 << ( 7 - (69 & 0x7))) ? 1 : 0;
  outBuffer[ 71 >> 3] |= bit << (7 - ( 71 & 0x7 ));
  outBuffer[ 72 >> 3] |= bit << (7 - ( 72 & 0x7 ));

  bit = (inBuffer[119 >> 3] & (1 << ( 7 - (119 & 0x7)))) ? 1 : 0;
  outBuffer[ 123 >> 3] |= bit << (7 - ( 123 & 0x7 ));
  outBuffer[ 124 >> 3] |= bit << (7 - ( 124 & 0x7 ));

  bit = (inBuffer[172 >> 3] & (1 << ( 7 - (172 & 0x7)))) ? 1 : 0;
  outBuffer[ 178 >> 3] |= bit << (7 - ( 178 & 0x7 ));
  outBuffer[ 179 >> 3] |= bit << (7 - ( 179 & 0x7 ));

  bit = (inBuffer[222 >> 3] & (1 << ( 7 - (222 & 0x7)))) ? 1 : 0;
  outBuffer[ 230 >> 3] |= bit << (7 - ( 230 & 0x7 ));
  outBuffer[ 231 >> 3] |= bit << (7 - ( 231 & 0x7 ));

  /* embed 8 bit crc */
  for ( k = 0; k < 8; k++ ) {
    bit = (crc & (1 << ( 7 - k))) ? 1 : 0;
    outBuffer[ (252 + k) >> 3] |= bit << (7 - ((252 + k) & 0x7 ));
  }
}



/***************************************************************************
* FUNCTION:    efrDecodeAudio
* PURPOSE:     Decode efr audio for egress processing
* PARAMETERS:  inBuffer: audio frame received from the network
* RETURNS:
* NOTE:
****************************************************************************/
void efrDecodeAudio ( char* inBuffer, char* outBuffer)
{
  char bit;
  int i = 0;

  /* copy first 71 (0-70) bits */
  memcpy ((void*)outBuffer, (void*)inBuffer, 9);
  /* clear 72th bit (bit 71) */
  outBuffer[8] &= 0xFFFE;
  /* and the remaining bits */
  memset(outBuffer + 9, 0, 22);

  /* Process output bits 71 - 120 */
  for ( i = 71; i <= 120; i++ ){
    bit = (inBuffer[(i + 2) >> 3] & (1 << ( 7 - (( i + 2) & 0x7)))) ? 1 : 0;
    outBuffer[ i >> 3] |= bit << (7 - ( i & 0x7 ));
  }
  /* Process output bits 121 - 173 */
  for ( i = 121; i <= 173; i++ ){
    bit = (inBuffer[(i + 4) >> 3] & (1 << ( 7 - (( i + 4) & 0x7)))) ? 1 : 0;
    outBuffer[ i >> 3] |= bit << (7 - ( i & 0x7 ));
  }
  /* Process output bits 174 - 223 */
  for ( i = 174; i <= 223; i++ ){
    bit = (inBuffer[(i + 6) >> 3] & (1 << ( 7 - (( i + 6) & 0x7)))) ? 1 : 0;
    outBuffer[ i >> 3] |= bit << (7 - ( i & 0x7 ));
  }
  /* Process output bits 224 - 243 */
  for ( i = 224; i <= 243; i++ ){
    bit = (inBuffer[(i + 8) >> 3] & (1 << ( 7 - (( i + 8) & 0x7)))) ? 1 : 0;
    outBuffer[ i >> 3] |= bit << (7 - ( i & 0x7 ));
  }
}


/***************************************************************************
* FUNCTION:    getRTPAudioPayloadType
* PURPOSE:     obtain payload type from 2G/3G audio format
* PARAMETERS:  cnx: endpoint connection object
*              data_format: 2G/3G audio format
* RETURNS:     HAPI payload type
* NOTE:
****************************************************************************/
unsigned char getRTPAudioPayloadType (CNXSTATE*  cnx, unsigned short data_format)
{
  int i = 0;
  unsigned char hapiCodecType = 0;

  switch (data_format) {
  case SPC_UNI_WB_AMR:
    hapiCodecType = HAPI_PKT_VC_WB_AMR_OA_2385;
    break;
  case SPC_UNI_AMR:
    hapiCodecType = HAPI_PKT_VC_GSM_AMR_OA_122;
    break;
  case SPC_UNI_GSM_FR:
    hapiCodecType = HAPI_PKT_VC_GSM_FR_13;
    break;
  case SPC_UNI_GSM_HR:
    hapiCodecType = HAPI_PKT_VC_GSM_HR_56;
    break;
  case SPC_UNI_GSM_EFR:
    hapiCodecType = HAPI_PKT_VC_GSM_EFR_122;
    break;
  default:
    VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "Payload not supported for 2G/3G\n"));
    return -1;
  }

  /* Find the payload type */
  for ( i = 0; i < HAPI_RTP_MAX_N_CODECS; i++ ) {
    if ((cnx->rtpRegs.mediaEgressPayloadTypeMap[i] & 0xFF) == hapiCodecType) {
      return cnx->rtpRegs.mediaEgressPayloadTypeMap[i] >> 8;
    }
  }
  VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "Did not find the payload for 2G/3G call\n"));
  return -1;
}

/***************************************************************************
* FUNCTION:     swap_short_word
* PURPOSE: 
* PARAMETERS: length: buffer length in bytes
*             ptr   : Pointer to the buffer 
* RETURNS:
* NOTE:
****************************************************************************/
void swap_short_word (unsigned char* ptr, unsigned short length) {
  unsigned short value;
  unsigned short i = 0;

  /* Find the number of short words */
  length = (length + 1) / 2;

  for (i = 0; i < length; i++ ) {
    value = ((unsigned short*)ptr)[i];
    PUTB16( ptr + (i * 2), value);
  }
}

/***************************************************************************
* FUNCTION:     swap_bits_in_bytes
* PURPOSE:
* PARAMETERS: length: buffer length in bytes
*             ptr   : Pointer to the buffer
* RETURNS:
* NOTE:
****************************************************************************/
void swap_bits_in_bytes (unsigned char* ptr, unsigned short length)
{
  unsigned char temp, mask, bit;
  int i; 

  for ( i = 0; i < length; i++ ){
      temp = ptr[i];
      ptr[i] = 0;
      mask = 0x80;
      for (bit = 0; bit < 8; bit++){
        ptr[i] |= ( temp & (1 << bit) ) ? mask : 0;
        mask >>= 1;
      }
  }
}

/***************************************************************************
* FUNCTION:    getAMRhapiCodec
* PURPOSE:     Returns the vaild AMR-NB HAPI codec type
* PARAMETERS:
*
* RETURNS:
* NOTE:
****************************************************************************/
unsigned short getAMRhapiCodec ( unsigned short codecCfg )
{
  switch (codecCfg) {
  case MODEM_AUDIO_AMR_475:
    return HAPI_PKT_VC_GSM_AMR_OA_475;
  case MODEM_AUDIO_AMR_515:
    return HAPI_PKT_VC_GSM_AMR_OA_515;
  case MODEM_AUDIO_AMR_590:
    return HAPI_PKT_VC_GSM_AMR_OA_590;
  case MODEM_AUDIO_AMR_670:
    return HAPI_PKT_VC_GSM_AMR_OA_670;
  case MODEM_AUDIO_AMR_740:
    return HAPI_PKT_VC_GSM_AMR_OA_740;
  case MODEM_AUDIO_AMR_795:
    return HAPI_PKT_VC_GSM_AMR_OA_795;
  case MODEM_AUDIO_AMR_102:
    return HAPI_PKT_VC_GSM_AMR_OA_102;
  case MODEM_AUDIO_AMR_122:
    return HAPI_PKT_VC_GSM_AMR_OA_122;
  default:
    VRG_LOG_ERROR ((VRG_LOG_MOD_EPT, "Invalid AMR-NB Codec Rate\n"));
  }
  return -1;
}

/***************************************************************************
* FUNCTION:    getAMRWBhapiCodec
* PURPOSE:     Returns the vaild AMR-WB HAPI codec type
* PARAMETERS:
*
* RETURNS:
* NOTE:
****************************************************************************/
unsigned short getAMRWBhapiCodec ( unsigned short codecCfg )
{
  switch (codecCfg) {
  case MODEM_AUDIO_WB_AMR_660:
    return HAPI_PKT_VC_WB_AMR_OA_660;
  case MODEM_AUDIO_WB_AMR_885:
    return HAPI_PKT_VC_WB_AMR_OA_885;
  case MODEM_AUDIO_WB_AMR_1265:
    return HAPI_PKT_VC_WB_AMR_OA_1265;
  case MODEM_AUDIO_WB_AMR_1425:
    return HAPI_PKT_VC_WB_AMR_OA_1425;
  case MODEM_AUDIO_WB_AMR_1585:
    return HAPI_PKT_VC_WB_AMR_OA_1585;
  case MODEM_AUDIO_WB_AMR_1825:
    return HAPI_PKT_VC_WB_AMR_OA_1825;
  case MODEM_AUDIO_WB_AMR_1985:
    return HAPI_PKT_VC_WB_AMR_OA_1985;
  case MODEM_AUDIO_WB_AMR_2305:
    return HAPI_PKT_VC_WB_AMR_OA_2305;
  case MODEM_AUDIO_WB_AMR_2385:
    return HAPI_PKT_VC_WB_AMR_OA_2385;

  default:
    VRG_LOG_ERROR ((VRG_LOG_MOD_EPT, "Invalid AMR-WB Codec Rate\n"));
  }
  return -1;
}


/***************************************************************************
* FUNCTION:    audioSyncTimer
* PURPOSE:     Timer to sync the audio
* PARAMETERS:
*
* RETURNS:
* NOTE:
****************************************************************************/
static void audioSyncTimer(unsigned long arg)
{
  /* Notify to transmit the frame */
  bosSemGive( &vrgCSEncodeNotifySem );
}

/***************************************************************************
* FUNCTION:    send_audio_encode_notification
* PURPOSE:     send enocode notification message
* PARAMETERS:
*
* RETURNS:
* NOTE:
****************************************************************************/
void send_audio_encode_notification (void)
{
  ModemAudioSpcEncoderDataNtf *pEncodeAudioHdr;
  MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR *pAudioSubBlockHdr;
  MODEM_AUDIO_HEADER *pAudioHdr;
  unsigned short audioOffset, frameLength, audioLength, streamId = 0;
  unsigned short subBlockLength = 0;
  unsigned short header = 0;
  /* Handle NO audio packet for early media */
  switch (gCSMedia[streamId].dataFormatRequest){
  case SPC_UNI_AMR:
    if ( gCSMedia[streamId].codecUse == SPC_USE_GSM_MODEM ) {
      /* Encode no audio for 2G call */
      header  = gCSMedia[streamId].encodeConfig << MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_OFFSET;
      header |= MODEM_AUDIO_TX_TYPE_NO_DATA << MODEM_AUDIO_SPC_DATA_UNI_AMR_tx_type_OFFSET;
    }
    else if (gCSMedia[streamId].codecUse == SPC_USE_WCDMA_MODEM){
      /* Encode no audio for 3G call */
      header = MODEM_AUDIO_NO_RX_TX << MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_OFFSET;
    }
    subBlockLength = SPC_UNI_AMR_LEN;
    break;
  case SPC_UNI_WB_AMR:
    if ( gCSMedia[streamId].codecUse == SPC_USE_GSM_MODEM ) {
      /* Encode no audio for 2G call */
      header  = gCSMedia[streamId].encodeConfig << MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_OFFSET;
      header |= MODEM_AUDIO_TX_TYPE_NO_DATA << MODEM_AUDIO_SPC_DATA_UNI_AMR_tx_type_OFFSET;
    }
    else if (gCSMedia[streamId].codecUse == SPC_USE_WCDMA_MODEM){
      /* Encode no audio for 3G call */
      header = MODEM_AUDIO_WB_AMR_NO_RX_TX << MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_OFFSET;
    }
    subBlockLength = SPC_UNI_WB_AMR_LEN;
    break;
  case SPC_UNI_GSM_EFR:
    header = MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_sp_MASK;
    subBlockLength = SPC_UNI_GSM_EFR_LEN;
    break;
  case SPC_UNI_GSM_FR:
    header = MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_sp_MASK;
    subBlockLength = SPC_UNI_GSM_FR_LEN;
    break;
  case SPC_UNI_GSM_HR:
    header = MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_sp_MASK;
    subBlockLength = SPC_UNI_GSM_HR_LEN;
    break;
  case SPC_NO:
    return;
  default:
    VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "Missing no audio support for 2G/3G codec=%d\n", gCSMedia[streamId].dataFormatRequest));
    return;
  }

  pEncodeAudioHdr = (ModemAudioSpcEncoderDataNtf*)(resp_packet + MHI_L2MUX_HEAD);
  pAudioSubBlockHdr = (MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR*) ((void*)pEncodeAudioHdr + sizeof (ModemAudioSpcEncoderDataNtf));
  pAudioHdr = (MODEM_AUDIO_HEADER*) ((void*) pAudioSubBlockHdr + sizeof (MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR));
  /* Prepare ISI header */
  pEncodeAudioHdr->media = PN_MEDIA_HOST_SSI;
  // Modem to consume this message
  pEncodeAudioHdr->receiver_dev = gCSMedia[streamId].modemSenderDev;
  pEncodeAudioHdr->sender_dev = gCSMedia[streamId].modemReceiverDev;
  // Audio resource
  pEncodeAudioHdr->res = PN_DSP_AUDIO;
  // Object processed by voice engine at the modem side
  pEncodeAudioHdr->receiver_obj = gCSMedia[streamId].modemSenderObj;
  // Any non-zero value
  pEncodeAudioHdr->sender_obj = gCSMedia[streamId].modemReceiverObj;
  // Increment transaction ID for current stream
  gCSMedia[streamId].transactionId += 1;
  // Update transaction ID
  pEncodeAudioHdr->TransactionId = gCSMedia[streamId].transactionId;
  pEncodeAudioHdr->MessageId = MODEM_AUDIO_SPC_ENCODER_DATA_NTF;
  PUTB16( &pEncodeAudioHdr->data_format, gCSMedia[streamId].dataFormatRequest );
  // Number of actual audio frame. We only support one sub frame
  PUTB16( &pEncodeAudioHdr->num_sb, 1 );
  /* Payload contains one block of audio */
  PUTB16( &pAudioSubBlockHdr->sb_id, MODEM_AUDIO_SB_SPC_CODEC_FRAME );
  PUTB16( &pAudioSubBlockHdr->sb_len, subBlockLength );
  /* Compute frame size that needs to be passed to modem */
  audioOffset = sizeof (tISI_ModemAudioSpcEncoderDataNtf) + sizeof (MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR) + sizeof (MODEM_AUDIO_HEADER);
  /* Find the actual audio length */
  audioLength = subBlockLength - sizeof (MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR) -  sizeof (MODEM_AUDIO_HEADER);
  /* Find the frame length */
  frameLength = audioOffset + audioLength;

  /* Set message length, which is 6 bytes shorter than the frame length */
  PUTB16( &pEncodeAudioHdr->length, (frameLength - 6) );

  /* Update audio header */
  pAudioHdr->reserved = 0;
  PUTB16(&pAudioHdr->header, header);

  switch (gCSMedia[streamId].dataFormatRequest){
  case SPC_UNI_GSM_EFR:
    memcpy ((void*) (resp_packet + MHI_L2MUX_HEAD + audioOffset),
            (void*) efr_silence, audioLength);
    break;
  case SPC_UNI_GSM_HR:
    memcpy ((void*) (resp_packet + MHI_L2MUX_HEAD + audioOffset),
            (void*) hr_silence, audioLength);
    break;
  case SPC_UNI_GSM_FR:
    memcpy ((void*) (resp_packet + MHI_L2MUX_HEAD + audioOffset),
            (void*) fr_silence, audioLength);
  default:
    /* Copy the audio into the buffer */
    memcpy ((void*) (resp_packet + MHI_L2MUX_HEAD + audioOffset),
            (void*) audioPayload, audioLength);
    break;
  }

  swap_short_word(resp_packet + MHI_L2MUX_HEAD, audioOffset);

  l2mux_audio_tx (resp_packet + MHI_L2MUX_HEAD, frameLength, PN_DEV_MODEM);

}

static int l2mux_audio_rx_func (unsigned char *buffer, size_t size, uint8_t phonet_dev_id)
{
     RTPPACKET* pktHdr;
     CNXSTATE*  cnx;
     unsigned int streamId = 0;                               // TODO: Make it run time configurable
     pn_isi_msg_head *pPnIsiHdr = (pn_isi_msg_head*)buffer;

     switch ( pPnIsiHdr->isi.MessageId ) {
     case AUDIO_CTRL_READY_REQ:
       {
         tISI_AudioCtrlReadyReq *pReadyRequest;
         tISI_AudioCtrlReadyRsp *pReadyResponse;
         unsigned short frameLength = sizeof(tISI_AudioCtrlReadyRsp) ;

         pReadyRequest  = (tISI_AudioCtrlReadyReq*)buffer;
         pReadyResponse = (tISI_AudioCtrlReadyRsp*)(resp_packet + MHI_L2MUX_HEAD);

         VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "Received AUDIO_CTRL_READY_REQ\n"));
         pReadyResponse->pn_hdr.media = pReadyRequest->pn_hdr.media;
         pReadyResponse->pn_hdr.receiver = pReadyRequest->pn_hdr.sender;
         pReadyResponse->pn_hdr.sender = pReadyRequest->pn_hdr.receiver;
         pReadyResponse->pn_hdr.receiver_obj = pReadyRequest->pn_hdr.sender_obj;
         pReadyResponse->pn_hdr.sender_obj = pReadyRequest->pn_hdr.receiver_obj;
         pReadyResponse->trans_id =  pReadyRequest->trans_id;
         pReadyResponse->message_id =  AUDIO_CTRL_READY_RESP;
         pReadyResponse->status = READY_RESP_OK;
         PUTB16(&pReadyResponse->pn_hdr.length, (frameLength - 6));
         l2mux_audio_tx (resp_packet + MHI_L2MUX_HEAD, frameLength, PN_DEV_MODEM);
       }
       return 0;
     case MODEM_AUDIO_SPC_TIMING_REQ:
       {
         tISI_ModemAudioSpcTimingReq  *pTimingReq;
         tISI_ModemAudioSpcTimingResp *pTimingRsp;
         unsigned short frameLength = 0;
         unsigned int nextDeliveryTime = 0;

         /* Stop sending UL audio packets */
         gAudioULReady = 0;

         pTimingReq = (tISI_ModemAudioSpcTimingReq*)buffer;
         pTimingRsp = (tISI_ModemAudioSpcTimingResp*)(resp_packet + MHI_L2MUX_HEAD);

         VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "Received MODEM_AUDIO_SPC_TIMING_REQ (%d)\n", GET_TIME_STAMP()));
         VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "MODEM_AUDIO_SPC_TIMING_REQ: 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X\n",
                       ((unsigned short*)pTimingReq)[0], ((unsigned short*)pTimingReq)[1],
                       ((unsigned short*)pTimingReq)[2], ((unsigned short*)pTimingReq)[3],
                       ((unsigned short*)pTimingReq)[4], ((unsigned short*)pTimingReq)[5],
                       ((unsigned short*)pTimingReq)[6], ((unsigned short*)pTimingReq)[7],
                       ((unsigned short*)pTimingReq)[8], ((unsigned short*)pTimingReq)[9]));


         nextDeliveryTime = (GETB16(&pTimingReq->timing_high) << 16) + GETB16(&pTimingReq->timing_low);
         VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "2G/3G Packet delivery time requested %u us \n", nextDeliveryTime));
         /* Adjust the first delivery timing */
         nextDeliveryTime -= 12000;
         gNextSyncTimeStamp = GET_TIME_STAMP() + (50000 * (nextDeliveryTime / 1000));


         frameLength = sizeof (tISI_ModemAudioSpcTimingResp);

         pTimingRsp->media = pTimingReq->media;
         pTimingRsp->receiver_dev = pTimingReq->sender_dev;
         pTimingRsp->sender_dev = pTimingReq->receiver_dev;
         pTimingRsp->res = pTimingReq->res;
         pTimingRsp->receiver_obj = pTimingReq->sender_obj;
         pTimingRsp->sender_obj = pTimingReq->receiver_obj;
         pTimingRsp->MessageId = MODEM_AUDIO_SPC_TIMING_RESP;
         gCSMedia[streamId].transactionId += 1;
         pTimingRsp->TransactionId = gCSMedia[streamId].transactionId;
         PUTB16(&pTimingRsp->status, SPC_TIMING_OK);
         PUTB16(&pTimingRsp->length, (frameLength - 6));

         swap_short_word (resp_packet + MHI_L2MUX_HEAD, frameLength);

         VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "MODEM_AUDIO_SPC_TIMING_RESP: 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X\n",
                       ((unsigned short*)pTimingRsp)[0], ((unsigned short*)pTimingRsp)[1],
                       ((unsigned short*)pTimingRsp)[2], ((unsigned short*)pTimingRsp)[3],
                       ((unsigned short*)pTimingRsp)[4], ((unsigned short*)pTimingRsp)[5],
                       ((unsigned short*)pTimingRsp)[6], ((unsigned short*)pTimingRsp)[7],
                       ((unsigned short*)pTimingRsp)[8], ((unsigned short*)pTimingRsp)[9]));

         l2mux_audio_tx (resp_packet + MHI_L2MUX_HEAD, frameLength, PN_DEV_MODEM);

         /* Based on the timing requirement add the delay and kick start the DMA */
         /* Reset if this is very first timing request */
         if (gAudioULStart == 0) {
           gCSMedia[streamId].rxSeqCount = 100;
         }

         if(0){
           /* Start the sync timer */
           init_timer(&syncTimer);
           syncTimer.data = msecs_to_jiffies( nextDeliveryTime / 1000 );
           syncTimer.function = audioSyncTimer;
           syncTimer.expires = jiffies + syncTimer.data;
           add_timer(&syncTimer);
         }
         else {
           bosSemGive( &vrgCSEncodeNotifySem );
         }
         return 0;
       }
     case MODEM_AUDIO_SPEECH_CODEC_REQ:
       {
         tISI_ModemAudioSpeechCodecReq  *pCodecReq = (tISI_ModemAudioSpeechCodecReq*)buffer;
         tISI_ModemAudioSpeechCodecResp *pCodecRsp = (tISI_ModemAudioSpeechCodecResp*)(resp_packet + MHI_L2MUX_HEAD);
         unsigned short frameLength = 0;
         unsigned short audioCodec = GETB16( &pCodecReq->SpeechCodec );
         unsigned short encodeConfig = GETB16(&pCodecReq->SpeechEncoderConfiguration);
         unsigned short codecUse = GETB16(&pCodecReq->SpeechCodecUse);
         unsigned short vad_type = 0;

         encodeConfig = (encodeConfig & MODEM_AUDIO_SPEECH_CODEC_REQ_AMR_encoder_MASK);
         encodeConfig >>= MODEM_AUDIO_SPEECH_CODEC_REQ_AMR_encoder_OFFSET;

         vad_type = (encodeConfig & MODEM_AUDIO_SPEECH_CODEC_REQ_DTX_use_MASK) ? HAPI_VAD_TYPE_BUILTIN : HAPI_VAD_TYPE_NONE;

         if (vad_type == HAPI_VAD_TYPE_BUILTIN) {
           VRG_LOG(("Discontinuous Transmission ON\n"));
         }
         else {
           VRG_LOG(("Discontinuous Transmission OFF\n"));
         }

         frameLength = sizeof (tISI_ModemAudioSpeechCodecResp);

         VRG_LOG_INFO((VRG_LOG_MOD_EPT, "Received MODEM_AUDIO_SPEECH_CODEC_REQ\n"));
         VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "MODEM_AUDIO_SPEECH_CODEC_REQ: 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X\n",
                       ((unsigned short*)pCodecReq)[0], ((unsigned short*)pCodecReq)[1],
                       ((unsigned short*)pCodecReq)[2], ((unsigned short*)pCodecReq)[3],
                       ((unsigned short*)pCodecReq)[4], ((unsigned short*)pCodecReq)[5],
                       ((unsigned short*)pCodecReq)[6], ((unsigned short*)pCodecReq)[7],
                       ((unsigned short*)pCodecReq)[8], ((unsigned short*)pCodecReq)[9]));

         pCodecRsp->media = pCodecReq->media;
         /* Save modem dev and obj information */
         gCSMedia[streamId].modemSenderDev = pCodecReq->sender_dev;
         gCSMedia[streamId].modemReceiverDev = pCodecReq->receiver_dev;
         gCSMedia[streamId].modemSenderObj = pCodecReq->sender_obj;
         gCSMedia[streamId].modemReceiverObj = pCodecReq->receiver_obj;

         pCodecRsp->receiver_dev = pCodecReq->sender_dev;
         pCodecRsp->sender_dev = pCodecReq->receiver_dev;
         pCodecRsp->res = pCodecReq->res;
         pCodecRsp->receiver_obj = pCodecReq->sender_obj;
         pCodecRsp->sender_obj = pCodecReq->receiver_obj;
         pCodecRsp->MessageId = MODEM_AUDIO_SPEECH_CODEC_RESP;
         gCSMedia[streamId].transactionId += 1;
         pCodecRsp->TransactionId = gCSMedia[streamId].transactionId;
         PUTB16(&pCodecRsp->Reason, SPC_RESP_OK);
         PUTB16(&pCodecRsp->length, (frameLength - 6));

         swap_short_word (resp_packet + MHI_L2MUX_HEAD, frameLength);

         VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "MODEM_AUDIO_SPEECH_CODEC_RESP: 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X\n",
                       ((unsigned short*)pCodecRsp)[0], ((unsigned short*)pCodecRsp)[1],
                       ((unsigned short*)pCodecRsp)[2], ((unsigned short*)pCodecRsp)[3],
                       ((unsigned short*)pCodecRsp)[4], ((unsigned short*)pCodecRsp)[5],
                       ((unsigned short*)pCodecRsp)[6], ((unsigned short*)pCodecRsp)[7],
                       ((unsigned short*)pCodecRsp)[8], ((unsigned short*)pCodecRsp)[9]));

         l2mux_audio_tx (resp_packet + MHI_L2MUX_HEAD, frameLength, PN_DEV_MODEM);

         switch (audioCodec) {
         case SPC_UNI_WB_AMR:
           VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "Selecting SPC_UNI_WB_AMR\n"));
           gCSMedia[streamId].hapiCodec = getAMRWBhapiCodec(encodeConfig);
           gCSMedia[streamId].hapiVad = vad_type;
           gCSMedia[streamId].dataFormatRequest = SPC_UNI_WB_AMR;
           gCSMedia[streamId].encodeConfig = encodeConfig;
           gCSMedia[streamId].codecUse = codecUse;
           break;
         case SPC_UNI_AMR:
           VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "Selecting SPC_UNI_AMR\n"));
           gCSMedia[streamId].hapiCodec = getAMRhapiCodec (encodeConfig);
           gCSMedia[streamId].hapiVad = vad_type;
           gCSMedia[streamId].dataFormatRequest = SPC_UNI_AMR;
           gCSMedia[streamId].encodeConfig = encodeConfig;
           gCSMedia[streamId].codecUse = codecUse;
           break;
         case SPC_UNI_GSM_FR:
           VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "Selecting SPC_UNI_GSM_FR\n"));
           gCSMedia[streamId].hapiCodec = HAPI_PKT_VC_GSM_FR_13;
           gCSMedia[streamId].hapiVad = vad_type; 
           gCSMedia[streamId].dataFormatRequest = SPC_UNI_GSM_FR;
           gCSMedia[streamId].encodeConfig = encodeConfig;
           gCSMedia[streamId].codecUse = codecUse;
           break;
         case SPC_UNI_GSM_HR:
           VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "Selecting SPC_UNI_GSM_HR\n"));
           gCSMedia[streamId].hapiCodec = HAPI_PKT_VC_GSM_HR_56;
           gCSMedia[streamId].hapiVad = vad_type;
           gCSMedia[streamId].dataFormatRequest = SPC_UNI_GSM_HR;
           gCSMedia[streamId].encodeConfig = encodeConfig;
           gCSMedia[streamId].codecUse = codecUse;
           break;
         case SPC_UNI_GSM_EFR:
           VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "Selecting SPC_UNI_GSM_EFR\n"));
           gCSMedia[streamId].hapiCodec = HAPI_PKT_VC_GSM_EFR_122;
           gCSMedia[streamId].hapiVad = vad_type;
           gCSMedia[streamId].dataFormatRequest = SPC_UNI_GSM_EFR;
           gCSMedia[streamId].codecUse = codecUse;
           break;
         case SPC_NO:
           VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "Selecting SPC_NO\n"));
           gAudioULReady = 0;
           gAudioULStart = 0;
           gCSMedia[streamId].hapiCodec = HAPI_PKT_VC_VOICE_NOCODE;
           gCSMedia[streamId].hapiVad = vad_type;
           gCSMedia[streamId].dataFormatRequest = SPC_NO;
           gCSMedia[streamId].encodeConfig = 0;
           gCSMedia[streamId].codecUpdated = 0;
           gCSMedia[streamId].codecUse = SPC_USE_NO;
           localTs = 100;
           return 0;
         default:
           VRG_LOG_ERROR ((VRG_LOG_MOD_EPT, "Unsupported audio codec %d\n", audioCodec));
           gAudioULReady = 0;
           gAudioULStart = 0;
           gCSMedia[streamId].hapiCodec = HAPI_PKT_VC_VOICE_NOCODE;
           gCSMedia[streamId].hapiVad = vad_type;
           gCSMedia[streamId].codecUpdated = 0;
           gCSMedia[streamId].encodeConfig = 0;
           gCSMedia[streamId].codecUse = SPC_USE_NO;
           return 0;
         }
         gCSMedia[streamId].sidStart = 0;
         gCSMedia[streamId].codecUpdated = 1;
         VRG_LOG(("Received Codec Update\n"));
         return 0;
       }
     case MODEM_AUDIO_SPC_DECODER_DATA_REQ:
       {
         tISI_ModemAudioSpcDecoderDataReq *pDataReq;
         MODEM_AUDIO_HEADER *pAudioHdr;
         void* audio_buffer;
         unsigned int audio_length;
         unsigned int timeStamp;
         unsigned int subBlockLen;


         pDataReq = (tISI_ModemAudioSpcDecoderDataReq*)buffer;
         pAudioHdr = (MODEM_AUDIO_HEADER*)(buffer
                                           + sizeof (tISI_ModemAudioSpcDecoderDataReq)
                                           + sizeof (MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR));

         audio_buffer = (void*)buffer
           + sizeof (tISI_ModemAudioSpcDecoderDataReq)
           + sizeof (MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR)
           + sizeof (MODEM_AUDIO_HEADER);


         subBlockLen = GETB16(&((MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR*)(buffer + sizeof (tISI_ModemAudioSpcDecoderDataReq)))->sb_len);
         audio_length = subBlockLen - ( sizeof (MODEM_AUDIO_HEADER)  + sizeof (MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR) + sizeof (timeStamp));

         if ( audio_length ) {
           unsigned short header;
           unsigned short data_format;
           unsigned char  frame_type = 0;

           /* Get packet the time stamp */
           timeStamp = get_rx_time_stamp (buffer, pDataReq);

           header = GETB16(&pAudioHdr->header);
           data_format = GETB16(&pDataReq->data_format);
           gCSMedia[streamId].dataFormatRequest = data_format;

           switch (data_format){
           case SPC_UNI_WB_AMR:
           case SPC_UNI_AMR:
             {
               /* Extract frame type */
               if (gCSMedia[streamId].codecUse == SPC_USE_GSM_MODEM) {
                 unsigned short rxType;
                 rxType = header & MODEM_AUDIO_SPC_DATA_UNI_AMR_rx_type_MASK;
                 rxType = rxType >> MODEM_AUDIO_SPC_DATA_UNI_AMR_rx_type_OFFSET;
                 if ( rxType == MODEM_AUDIO_RX_NO_DATA ){
                   frame_type = MODEM_AUDIO_NO_RX_TX << MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_OFFSET;
                 }
                 else if (rxType >= MODEM_AUDIO_RX_SID_FIRST) {
                   if (data_format == SPC_UNI_AMR){
                     frame_type = MODEM_AUDIO_AMR_CN << MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_OFFSET;
                   }
                   else {
                     frame_type = MODEM_AUDIO_WB_AMR_SID << MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_OFFSET;
                   }
                 }
                 else {
                   frame_type = header & MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_MASK;
                 }
               }
               else if (gCSMedia[streamId].codecUse == SPC_USE_WCDMA_MODEM){
                 frame_type = header & MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_MASK;
               }

               /* Add 2 byte AMR header before the actual payload */
               rx_media[sizeof (RTPPACKET)]     = 0xF0;
               rx_media[sizeof (RTPPACKET) + 1] = 0x4 | frame_type;

               memcpy ((void*)rx_media + sizeof (RTPPACKET) + 2, (void*)audio_buffer, audio_length);

               frame_type >>= 3;
               /* Find the actual audio length based on the codec type */
               if (data_format == SPC_UNI_AMR) {
                 switch (frame_type) {
                 case MODEM_AUDIO_AMR_475:
                   audio_length = (AMR_CORE_FRM_NUM_BITS_AMR_475 + 7) / 8;
                   break;
                 case MODEM_AUDIO_AMR_515:
                   audio_length = (AMR_CORE_FRM_NUM_BITS_AMR_515 + 7) / 8;
                   break;
                 case MODEM_AUDIO_AMR_590:
                   audio_length = (AMR_CORE_FRM_NUM_BITS_AMR_590 + 7) / 8;
                   break;
                 case MODEM_AUDIO_AMR_670:
                   audio_length = (AMR_CORE_FRM_NUM_BITS_AMR_670 + 7) / 8;
                   break;
                 case MODEM_AUDIO_AMR_740:
                   audio_length = (AMR_CORE_FRM_NUM_BITS_AMR_740 + 7) / 8;
                   break;
                 case MODEM_AUDIO_AMR_795:
                   audio_length = (AMR_CORE_FRM_NUM_BITS_AMR_795 + 7) / 8;
                   break;
                 case MODEM_AUDIO_AMR_102:
                   audio_length = (AMR_CORE_FRM_NUM_BITS_AMR_102 + 7) / 8;
                   break;
                 case MODEM_AUDIO_AMR_122:
                   audio_length = (AMR_CORE_FRM_NUM_BITS_AMR_122 + 7) / 8;
                   break;
                 case MODEM_AUDIO_AMR_CN:
                   audio_length = (AMR_CORE_FRM_NUM_BITS_AMR_SID + 7) / 8;
                   break;
                 case MODEM_AUDIO_NO_RX_TX:
                   audio_length = (AMR_CORE_FRM_NUM_BITS_NO_DATA + 7) / 8;
                   break;
                 default:
                   VRG_LOG_ERROR ((VRG_LOG_MOD_EPT, "Unknown AMR-NB frame type (%d)\n", frame_type));
                 }
               }
               else {
                 switch (frame_type) {
                 case MODEM_AUDIO_WB_AMR_660:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_AMRWB_660 + 7) / 8;
                   break;
                 case MODEM_AUDIO_WB_AMR_885:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_AMRWB_885 + 7) / 8;
                   break;
                 case MODEM_AUDIO_WB_AMR_1265:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_AMRWB_1265 + 7) / 8;
                   break;
                 case MODEM_AUDIO_WB_AMR_1425:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_AMRWB_1425 + 7) / 8;
                   break;
                 case MODEM_AUDIO_WB_AMR_1585:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_AMRWB_1585 + 7) / 8;
                   break;
                 case MODEM_AUDIO_WB_AMR_1825:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_AMRWB_1825 + 7) / 8;
                   break;
                 case MODEM_AUDIO_WB_AMR_1985:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_AMRWB_1985 + 7) / 8;
                   break;
                 case MODEM_AUDIO_WB_AMR_2305:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_AMRWB_2305 + 7) / 8;
                   break;
                 case MODEM_AUDIO_WB_AMR_2385:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_AMRWB_2385 + 7) / 8;
                   break;
                 case MODEM_AUDIO_WB_AMR_SID:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_AMRWB_SID + 7) / 8;
                   break;
                 case MODEM_AUDIO_WB_AMR_SPEECH_LOST:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_SPEECH_LOST + 7) / 8;
                   break;
                 case MODEM_AUDIO_WB_AMR_NO_RX_TX:
                   audio_length = (AMRWB_CORE_FRM_NUM_BITS_NO_DATA + 7) / 8;
                   break;
                 default:
                   VRG_LOG_ERROR ((VRG_LOG_MOD_EPT, "Unknown AMR-WB frame type (%d)\n", frame_type));
                 }
               }
               /* Include the length for AMR header */
               audio_length += 2;
             }
             break;
           case SPC_UNI_GSM_HR:
             {
               char* eg_ptr = rx_media;
               int mode = 0, i = 0, bit = 0;

               /* Copy the audio for egress processing */
               memcpy((void*)eg_ptr, audio_buffer, SPC_UNI_GSM_HR_LEN);
               /* Swap bits in bytes */
               swap_bits_in_bytes (eg_ptr, SPC_UNI_GSM_HR_LEN);
               /* Swap the words */
               swap_short_word (eg_ptr, SPC_UNI_GSM_HR_LEN);

               /* Collect all the bits together, because there is a hole of
                  17 bits in the frame after 95th bit position
               */
               /* clear 95th bit */
               eg_ptr[ 94 >> 3] &= 0xFE;
               /* collect bits for 95 to 111 positions by skipping 17 bits */
               for (i = 95; i <=111; i++ ){
                 bit = (((char*)eg_ptr)[(i + 17) >> 3] & (1 << (7 - ((i + 17) & 0x7)))) ? 1 : 0;
                 eg_ptr[i>>3] |= bit << (7 - (i & 0x7));
               }

               /* Descramble HR Audio */
               /* Read mode from bit position 93 and 94 */
               mode = (eg_ptr[93 >> 3] & (1 << (7 - (93 & 0x7)))) | (eg_ptr[94 >> 3] & (1 << (7 - (94 & 0x7))));

               if (mode) {
                 /* Non-zero mode is treated as voice */
                 descramble_audio (eg_ptr, audio_buffer, hr_voice_scramble_table, array_size(hr_voice_scramble_table));
               }
               else {
                 /* Zero mode is treated as unvoice */
                 descramble_audio (eg_ptr, audio_buffer, hr_unvoice_scramble_table, array_size(hr_unvoice_scramble_table));
               }

               /* Encode HR frame type */
               if (header & MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_sid_MASK){
                 ((char*)(rx_media + sizeof (RTPPACKET)))[0] = MODEM_AUDIO_HR_FRAME_RX_SID << MODEM_AUDIO_HR_FRAME_RX_TYPE_SHIFT;
               }
               else if (header & MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_sp_MASK){
                 ((char*)(rx_media + sizeof (RTPPACKET)))[0] = MODEM_AUDIO_HR_FRAME_RX_SPEECH << MODEM_AUDIO_HR_FRAME_RX_TYPE_SHIFT;
               }
               else {
                 ((char*)(rx_media + sizeof (RTPPACKET)))[0] = MODEM_AUDIO_HR_FRAME_RX_NO_DATA << MODEM_AUDIO_HR_FRAME_RX_TYPE_SHIFT;
               }

               if (header & MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_taf_MASK) {
                 ((char*)(rx_media + sizeof (RTPPACKET)))[0] |= MODEM_AUDIO_HR_FRAME_RX_TAF << MODEM_AUDIO_HR_FRAME_RX_TYPE_SHIFT;
               }
               if (header & MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_ufi_MASK) {
                 ((char*)(rx_media + sizeof (RTPPACKET)))[0] |= MODEM_AUDIO_HR_FRAME_RX_UFI << MODEM_AUDIO_HR_FRAME_RX_TYPE_SHIFT;
               }
               if (header & MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_bfi_MASK) {
                 ((char*)(rx_media + sizeof (RTPPACKET)))[0] |= MODEM_AUDIO_HR_FRAME_RX_BFI << MODEM_AUDIO_HR_FRAME_RX_TYPE_SHIFT;
               }

               /* Leave one byte for HR frame type (SPC/SID/..) and then copy the audio */
               memcpy ((void*)rx_media + sizeof (RTPPACKET) + 1, (void*)audio_buffer, audio_length);

               audio_length = 15;
             }
             break;
           case SPC_UNI_GSM_FR:
             {
               unsigned char temp;
               int i = 0;
               char* eg_ptr = rx_media;
               char flag = 0;

               /* Copy the audio for egress processing */
               memcpy((void*)eg_ptr, audio_buffer, SPC_UNI_GSM_FR_LEN);

               swap_bits_in_bytes (eg_ptr, SPC_UNI_GSM_FR_LEN);
               /* Swap the words */
               swap_short_word (eg_ptr, SPC_UNI_GSM_FR_LEN);
               /* Descramble FR Audio */
               descramble_audio(eg_ptr, audio_buffer, fr_scramble_table, array_size(fr_scramble_table));

               /* Re format the buffer based on RTP framing by right shifting them by 4 bits*/
               for ( i = SPC_UNI_GSM_FR_LEN; i > 1; i--){
                 temp = ((unsigned char*)audio_buffer)[i - 1];
                 ((unsigned char*)audio_buffer)[i - 1] = (temp & 0xF0) >> 4;
                 temp = ((unsigned char*)audio_buffer)[i - 2];
                 ((unsigned char*)audio_buffer)[i - 1] |= (temp & 0x0F) << 4;
               }
               temp = ((unsigned char*)audio_buffer)[0];

               if (header & MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_bfi_MASK) {
                 flag = MODEM_AUDIO_FR_RX_FLAG_BFI;
               }
               if (header & MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_taf_MASK) {
                 flag |= MODEM_AUDIO_FR_RX_FLAG_TAF;
               }
               if (header & MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_sid_MASK) {
                 flag |= MODEM_AUDIO_FR_RX_FLAG_SID;
               }

               ((char*)audio_buffer)[0] = (flag << 4) | (temp & 0xF0) >> 4;

               audio_length = SPC_UNI_GSM_FR_LEN;
               memcpy ((void*)rx_media + sizeof (RTPPACKET), (void*)audio_buffer, audio_length);
               audio_length = 33;
             }
             break;
           case SPC_UNI_GSM_EFR:
             {
               unsigned char temp;
               int i = 0;
               char* egPtr = rx_media;
               char flag = 0;

               /* Copy the audio for egress processing */
               memcpy((void*)egPtr, audio_buffer, SPC_UNI_GSM_EFR_LEN);

               swap_bits_in_bytes (egPtr, SPC_UNI_GSM_EFR_LEN);
               /* Swap the words */
               swap_short_word (egPtr, SPC_UNI_GSM_EFR_LEN);
               /* Descramble EFR Audio */
               descramble_audio(egPtr, audio_buffer, efr_scramble_table, array_size(efr_scramble_table));
               /* Decode 2G EFR audio for egress RTP processing */
               efrDecodeAudio(audio_buffer, egPtr);

               memcpy((void*)audio_buffer, (void*)egPtr, 33);
               /* For EFR the total payload is 31 bytes */
               audio_length = 31;
               /* Re format the buffer based on RTP framing by right shifting them by 4 bits*/
               for ( i = audio_length; i > 1; i--){
                 temp = ((unsigned char*)audio_buffer)[i - 1];
                 ((unsigned char*)audio_buffer)[i - 1] = (temp & 0xF0) >> 4;
                 temp = ((unsigned char*)audio_buffer)[i - 2];
                 ((unsigned char*)audio_buffer)[i - 1] |= (temp & 0x0F) << 4;
               }
               temp = ((unsigned char*)audio_buffer)[0];

               if (header & MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_bfi_MASK) {
                 flag = MODEM_AUDIO_EFR_RX_FLAG_BFI;
               }
               if (header & MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_taf_MASK) {
                 flag |= MODEM_AUDIO_EFR_RX_FLAG_TAF;
               }
               if (header & MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_sid_MASK) {
                 flag |= MODEM_AUDIO_EFR_RX_FLAG_SID;
               }

               ((char*)audio_buffer)[0] = (flag << 4) | ((temp & 0xF0) >> 4);

               memcpy ((void*)rx_media + sizeof (RTPPACKET), (void*)audio_buffer, audio_length);
             }
             break;
           default:
             VRG_LOG_ERROR ((VRG_LOG_MOD_EPT, "Invalid Rx Codec\n"));
             return 0;
           }

           pktHdr = (RTPPACKET*)rx_media;
           pktHdr->version = 2;
           pktHdr->p       = 0;
           pktHdr->x       = 0;
           pktHdr->cc      = 0;
           pktHdr->m       = 0;

           if ( gCSCnxId != -1 ) {
             cnx = GetCnxState( gCSCnxId );
             if( cnx ) {
               if (gCSMedia[streamId].codecUpdated) {
                 if ( hdspVhdSetVoiceCodec( cnx->vhdhdl,
                                            gCSMedia[streamId].hapiCodec,
                                            gCSMedia[streamId].hapiVad,
                                            160, 0 ) == HAPISUCCESS){
                   gCSMedia[streamId].codecUpdated = 0;
                   VRG_LOG(("Configured codec %d on cnx->vhfhdl %d\n", gCSMedia[streamId].hapiCodec, cnx->vhdhdl));
                 }
               }

               pktHdr->pt = getRTPAudioPayloadType (cnx, data_format);


               PUTB16 ( pktHdr->seq, gCSMedia[streamId].rxSeqCount );
               PUTB32 ( pktHdr->ts,  timeStamp );
               PUTB32 ( pktHdr->ssrc, 12345678 );
               gCSMedia[streamId].rxSeqCount++;

               if ( (gCSMedia[streamId].hapiCodec != HAPI_PKT_VC_VOICE_NOCODE)) {
                 if (HAPISUCCESS != hdspSendCmdDataAsync( cnx->vhdhdl,
                                                          HAPINET_EGRESSRTPPKT,
                                                          HSCMDEXTDATA_ASYNC,
                                                          sizeof(RTPPACKET) + audio_length,
                                                          (RTPPACKET *) rx_media )) {
                   VRG_LOG_ERROR ((VRG_LOG_MOD_EPT, "Failed to push incoming packet\n"));
                 }
               }
             }
           }
         }
       }
       break;
     default:
       VRG_LOG_ERROR ((VRG_LOG_MOD_EPT, "Not ready to handle message ID 0x%02X from modem (%d)\n", pPnIsiHdr->isi.MessageId, size));
       break;
     }
     return 0;
}

/***************************************************************************
* FUNCTION:    VrgCSMediaTxTaskMain
* PURPOSE:     Thread to deliver early media
* PARAMETERS:
*
* RETURNS:
* NOTE:
****************************************************************************/
void VrgCSMediaTxTaskMain( BOS_TASK_ARG taskArg )
{

  int syncAudio = 0;

  VRG_LOG_DBG ((VRG_LOG_MOD_EPT, "Running CS early media thread.."));

  /* Register L2MUX audio receive function */
  rx_handle = l2mux_audio_rx_register (l2mux_audio_rx_func);

  /* Create semaphore for command queue */
  if ( bosSemCreate("vrgCSEncodeNotifySem", 0, 1, &vrgCSEncodeNotifySem) != BOS_STATUS_OK ) {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Error creating vrgCSEncodeNotifySem"));
      return ;
  }

  while ( 1 ) {
    unsigned int checkCount;

    bosSemTake( &vrgCSEncodeNotifySem );
    syncAudio = 1;
    gAudioULStart = 0;
    checkCount = 0;
    while( (gAudioULReady || syncAudio) && (!gAudioULStart)) {

      local_irq_disable ();

      if (syncAudio) {
        int time_diff = GET_TIME_DIFF ( gNextSyncTimeStamp - GET_TIME_STAMP() );

        while (time_diff > 0 ) {
          if ( checkCount > 0x200000 ) {
            local_irq_enable ();
            VRG_LOG_ERROR((VRG_LOG_MOD_EPT,"Failed syncing %d\n", GET_TIME_STAMP()));
          }
          else {
            checkCount++;
          }
          time_diff = GET_TIME_DIFF ( gNextSyncTimeStamp - GET_TIME_STAMP() );
        }
        send_audio_encode_notification ();
        syncAudio = 0;
        gAudioULReady = 1;
      }
      else {
        send_audio_encode_notification ();
      }

      local_irq_enable ();

      bosSleep(20);
    }
  }
}

/***************************************************************************
* FUNCTION:    process_cs_ingress_audio
* PURPOSE:     Function to deliver ingress audio packet
* PARAMETERS:
*
* RETURNS:
* NOTE:
****************************************************************************/
void process_cs_ingress_audio (void* buffer, unsigned short length)
{
  unsigned short frameLength;
  unsigned short audio_length;
  unsigned short payloadID;
  unsigned short audioOffset;
  ModemAudioSpcEncoderDataNtf* pEncodeAudioHdr;
  MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR* pAudioSubBlockHdr;
  MODEM_AUDIO_HEADER* pAudioHdr;
  unsigned short header = 0;
  unsigned int streamId = 0;                               // TODO: Make it run time configurable
  unsigned short dataFormat = 0;
  audio_length = length;                                    // Audio payload length

  if ( gAudioULReady ) {

    gAudioULStart = 1;

    /* Identify the payload and audio */
    if (audio_length) {
      payloadID = ((HAPI_PKT_HDR*)buffer)->payloadID & 0xFF;

      if ( (gCSMedia[streamId].dataFormatRequest == SPC_UNI_WB_AMR)
           && (payloadID >= HAPI_PKT_VC_WB_AMR_660)
           && (payloadID <= HAPI_PKT_VC_WB_AMR_OA_2385) ) {
        dataFormat = SPC_UNI_WB_AMR;
        /* discard 2 bytes of AMR header */
        audio_length = audio_length - sizeof (HAPI_PKT_HDR) - 2;
        /* Point to the actual audio */
        buffer = buffer + sizeof (HAPI_PKT_HDR) + 2;
        /* Extract frame type from AMR header */
        if ( gCSMedia[streamId].codecUse == SPC_USE_GSM_MODEM ) {
          header = ((char*)buffer)[-1] & MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_MASK;
          /* Get the frame type */
          header = header >> MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_OFFSET;

          if (header == MODEM_AUDIO_WB_AMR_NO_RX_TX) {
            /* No data packet */
            header = MODEM_AUDIO_TX_TYPE_NO_DATA << MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_tx_type_OFFSET;
            /* Encode frame type */
            header |= gCSMedia[streamId].encodeConfig << MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_OFFSET;
          }
          else if (header > MODEM_AUDIO_WB_AMR_2385) {
            header = MODEM_AUDIO_TX_TYPE_SID << MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_tx_type_OFFSET;
            /* Encode frame type */
            header |= gCSMedia[streamId].encodeConfig << MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_OFFSET;
          }
          else {
            header = header << MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_OFFSET;
          }
        }
        else {
          header = ((char*)buffer)[-1] & MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_frame_type_MASK;
        }
        header |= MODEM_AUDIO_SPC_DATA_UNI_WB_AMR_quality_MASK;
      }
      else if ( (gCSMedia[streamId].dataFormatRequest == SPC_UNI_AMR)
                && (payloadID >= HAPI_PKT_VC_GSM_AMR_475)
                && (payloadID <= HAPI_PKT_VC_GSM_AMR_OA_122) ) {
        dataFormat = SPC_UNI_AMR;
        /* discard 2 bytes of AMR header */
        audio_length = audio_length - sizeof (HAPI_PKT_HDR) - 2;
        /* Point to the actual audio */
        buffer = buffer + sizeof (HAPI_PKT_HDR) + 2;
        /* Extract frame type from AMR header */
        if ( gCSMedia[streamId].codecUse == SPC_USE_GSM_MODEM ) {
          header = ((char*)buffer)[-1] & MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_MASK;
          /* Get the frame type */
          header = header >> MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_OFFSET;
          if (header == MODEM_AUDIO_NO_RX_TX) {
            /* No data packet */
            header = MODEM_AUDIO_TX_TYPE_NO_DATA << MODEM_AUDIO_SPC_DATA_UNI_AMR_tx_type_OFFSET;
            /* Encode frame type */
            header |= gCSMedia[streamId].encodeConfig << MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_OFFSET;
          }
          else if (header > MODEM_AUDIO_AMR_122) {
            header = MODEM_AUDIO_TX_TYPE_SID << MODEM_AUDIO_SPC_DATA_UNI_AMR_tx_type_OFFSET;
            /* Encode frame type */
            header |= gCSMedia[streamId].encodeConfig << MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_OFFSET;
          }
          else {
            header = header << MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_OFFSET;
          }
        }
        else if (gCSMedia[streamId].codecUse == SPC_USE_WCDMA_MODEM){
          header = ((char*)buffer)[-1] & MODEM_AUDIO_SPC_DATA_UNI_AMR_frame_type_MASK;
        }

        header |= MODEM_AUDIO_SPC_DATA_UNI_AMR_quality_MASK;
      }
      else if ( (gCSMedia[streamId].dataFormatRequest == SPC_UNI_GSM_HR )
               && (payloadID == HAPI_PKT_VC_GSM_HR_56)) {
        char* in_ptr;
        int i, mode = 0;
        char bit;

        dataFormat = SPC_UNI_GSM_HR;
        audio_length = audio_length - sizeof (HAPI_PKT_HDR);
        /* Point to the payload */
        buffer = buffer + sizeof (HAPI_PKT_HDR);

        /* non-zero first byte indicates voice */
        if ( ((char*)buffer)[0] ) {
          header = MODEM_AUDIO_SPC_DATA_UNI_GSM_HR_sp_MASK;
        }

        /* Point to the actual audio, becuase 1st byte contrains the HR header */
        buffer++;

        in_ptr = gsm_tx_audio_buffer[streamId];
        memcpy (in_ptr, buffer, audio_length);
        /* Clear the remaining bytes */
        memset (in_ptr + audio_length, 0, AUDIO_BUFF_SIZE - audio_length);
        /* Read mode from bit position 34 and 35 */
        mode = (in_ptr[34 >> 3] & (1 << (7 - (34 & 0x7)))) | (in_ptr[35 >> 3] & (1 << (7 - (35 & 0x7))));
        /* Scramble the audio based on mode (voice/unvoice) */
        if (mode) {
          /* non-zero mode is for voice */
          scramble_audio (in_ptr, buffer, hr_voice_scramble_table, array_size(hr_voice_scramble_table));
        }
        else {
          /* zero mode is for unvoice */
          scramble_audio (in_ptr, buffer, hr_unvoice_scramble_table, array_size(hr_unvoice_scramble_table));
        }

        /* Copy audio for futher processing before transmit to the cellular network */
        memcpy ((void*)in_ptr, buffer, audio_length);
        /* clear bit 95 */
        in_ptr[ 94 >> 3] &= 0xFE;
        /* clear remaining bits */
        memset ((void*)(in_ptr + (94 >> 3) + 1), 0, SPC_UNI_GSM_HR_LEN - (94 >> 3));
        /* collect bits from 95 to 111 positions */
        for (i = 95; i <=111; i++ ){
          bit = (((char*)buffer)[i >> 3] & (1 << (7 - ( i & 7) ))) ? 1 : 0;
          /* skip 17 bits in output buffer */
          in_ptr[(i + 17) >> 3] |= bit << (7 - ((i+17) & 7));
        }

        /* point to the cellular audio */
        buffer = in_ptr;

        swap_bits_in_bytes (buffer, 18);
      }
      else if ( (gCSMedia[streamId].dataFormatRequest == SPC_UNI_GSM_FR)
                && (payloadID == HAPI_PKT_VC_GSM_FR_13)) {
        char* in_ptr;
        unsigned char temp;
        int i = 0;

        dataFormat = SPC_UNI_GSM_FR;
        audio_length = audio_length - sizeof (HAPI_PKT_HDR);
        /* Point to the actual audio */
        buffer = buffer + sizeof (HAPI_PKT_HDR);
        in_ptr = gsm_tx_audio_buffer[streamId];
        /* Copy the audio */
        memcpy (in_ptr, buffer, audio_length);
        /* Clear the remaining bytes */
        memset (in_ptr + audio_length, 0, AUDIO_BUFF_SIZE - audio_length);
        /* non-zero first 4 bits indicate voice */
        if ( in_ptr[0] & 0xF0 ) {
          header = MODEM_AUDIO_SPC_DATA_UNI_GSM_FR_sp_MASK;
        }

        /* Re format the buffer based on 2G framing by left shifting them by 4 bits*/
        for ( i = 0; i < audio_length; i++){
          temp = ((unsigned char*)in_ptr)[i];
          ((unsigned char*)in_ptr)[i] = (temp & 0x0F) << 4;
          temp = ((unsigned char*)in_ptr)[i + 1];
          ((unsigned char*)in_ptr)[i] |= (temp & 0xF0) >> 4;
        }
        temp = ((unsigned char*)in_ptr)[audio_length];
        ((char*)in_ptr)[audio_length] |= ((temp & 0x0F) << 4);
        /* Scramble the audio */
        scramble_audio (in_ptr, buffer, fr_scramble_table, array_size(fr_scramble_table));
        swap_bits_in_bytes (buffer, 33);
      }

      else if ( (gCSMedia[streamId].dataFormatRequest == SPC_UNI_GSM_EFR)
                && (payloadID == HAPI_PKT_VC_GSM_EFR_122)) {
        unsigned char temp;
        unsigned char *in_ptr;
        unsigned char crc;
        int i = 0;

        dataFormat = SPC_UNI_GSM_EFR;
        audio_length = audio_length - sizeof (HAPI_PKT_HDR);
        /* Point to the actual audio */
        buffer = buffer + sizeof (HAPI_PKT_HDR);
        /* Copy audio for efr processing */
        in_ptr = gsm_tx_audio_buffer[streamId];
        memcpy (in_ptr, buffer, audio_length);
        /* non-zero first 4 bits indicate voice */
        if ( in_ptr[0] & 0xF0 ) {
          header = MODEM_AUDIO_SPC_DATA_UNI_GSM_EFR_sp_MASK;
        }

        /* Re format the buffer based on 2G framing by left shifting them by 4 bits*/
        for ( i = 0; i < audio_length; i++){
          temp = ((unsigned char*)in_ptr)[i];
          ((unsigned char*)in_ptr)[i] = (temp & 0x0F) << 4;
          temp = ((unsigned char*)in_ptr)[i + 1];
          ((unsigned char*)in_ptr)[i] |= (temp & 0xF0) >> 4;
        }
        temp = ((unsigned char*)in_ptr)[audio_length];
        ((char*)in_ptr)[audio_length] |= ((temp & 0x0F) << 4);

        /* Calculate CRC */
        crc = efr_crc_calculate (in_ptr);
        /* Re-package the audio for 2G */
        efr_encode_audio (in_ptr, buffer, crc);
        scramble_audio (buffer, in_ptr,  efr_scramble_table, array_size(efr_scramble_table));
        memcpy((void*)buffer, (void*)in_ptr, 33);
        swap_bits_in_bytes (buffer, 33);
      }
      else {
        /* Invalid payload */
        VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "Invalid payload received len:%d pid:%d\n", audio_length, payloadID));
        return;
      }
    }
    else {
      /* Transmit NO Audio frame and return */
      send_audio_encode_notification ();
      return;
    }

    /* Prepare 2G/3G frame head */
    pEncodeAudioHdr = (ModemAudioSpcEncoderDataNtf*)(csPacket + MHI_L2MUX_HEAD);
    pAudioSubBlockHdr = (MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR*) ((void*)pEncodeAudioHdr + sizeof (ModemAudioSpcEncoderDataNtf));
    pAudioHdr = (MODEM_AUDIO_HEADER*) ((void*) pAudioSubBlockHdr + sizeof (MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR));

    /* Prepare ISI header */
    pEncodeAudioHdr->media = PN_MEDIA_HOST_SSI;
    // Modem to consume this message
    pEncodeAudioHdr->receiver_dev = gCSMedia[streamId].modemSenderDev;
    pEncodeAudioHdr->sender_dev = gCSMedia[streamId].modemReceiverDev;
    // Audio resource
    pEncodeAudioHdr->res = PN_DSP_AUDIO;
    // Object processed by voice engine at the modem side
    pEncodeAudioHdr->receiver_obj = gCSMedia[streamId].modemSenderObj;
    // Any non-zero value
    pEncodeAudioHdr->sender_obj = gCSMedia[streamId].modemReceiverObj;
    // Increment transaction ID for current stream
    gCSMedia[streamId].transactionId += 1;
    // Update transaction ID
    pEncodeAudioHdr->TransactionId = gCSMedia[streamId].transactionId;
    pEncodeAudioHdr->MessageId = MODEM_AUDIO_SPC_ENCODER_DATA_NTF;
    PUTB16( &pEncodeAudioHdr->data_format, dataFormat );
    // Number of actual audio frame. We only support one sub frame
    PUTB16( &pEncodeAudioHdr->num_sb, 1 );
    /* Payload contains one block of audio */
    PUTB16( &pAudioSubBlockHdr->sb_id, MODEM_AUDIO_SB_SPC_CODEC_FRAME );

    switch (gCSMedia[streamId].dataFormatRequest){
    case SPC_UNI_AMR:
      PUTB16( &pAudioSubBlockHdr->sb_len, SPC_UNI_AMR_LEN);
      audio_length = SPC_UNI_AMR_LEN ;
      break;
    case SPC_UNI_WB_AMR:
      PUTB16( &pAudioSubBlockHdr->sb_len, SPC_UNI_WB_AMR_LEN);
      audio_length = SPC_UNI_WB_AMR_LEN;
      break;
    case SPC_UNI_GSM_FR:
      PUTB16( &pAudioSubBlockHdr->sb_len, SPC_UNI_GSM_FR_LEN);
      audio_length = SPC_UNI_GSM_FR_LEN;
      break;
    case SPC_UNI_GSM_HR:
      PUTB16( &pAudioSubBlockHdr->sb_len, SPC_UNI_GSM_HR_LEN);
      audio_length = SPC_UNI_GSM_HR_LEN;
      break;
    case SPC_UNI_GSM_EFR:
      PUTB16( &pAudioSubBlockHdr->sb_len, SPC_UNI_GSM_EFR_LEN);
      audio_length = SPC_UNI_GSM_EFR_LEN;
      break;
    case SPC_NO:
      return;
    default:
      VRG_LOG_ERROR(( VRG_LOG_MOD_EPT, "Unexpected codec selection\n")); 
    }

    /* Compute frame size that needs to be passed to modem */
    audioOffset = sizeof (tISI_ModemAudioSpcEncoderDataNtf) + sizeof (MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR) + sizeof (MODEM_AUDIO_HEADER);

    audio_length = audio_length - sizeof (MODEM_AUDIO_SB_SPC_CODEC_FRAME_STR) - sizeof (MODEM_AUDIO_HEADER);

    frameLength = audioOffset + audio_length;

    /* Set message length, which is 6 bytes shorter than the frame length */
    PUTB16( &pEncodeAudioHdr->length, (frameLength - 6) );

    /* Update audio header */
    pAudioHdr->reserved = 0;
    PUTB16(&pAudioHdr->header, header);

    /* Copy the audio into the buffer */
    memcpy ((void*) (csPacket + MHI_L2MUX_HEAD + audioOffset),
            (void*) buffer, audio_length);

    switch ( gCSMedia[streamId].dataFormatRequest ) {
    case SPC_UNI_GSM_EFR:
    case SPC_UNI_GSM_FR:
    case SPC_UNI_GSM_HR:
      /* Swap upto the beging of the audio */
      swap_short_word(csPacket + MHI_L2MUX_HEAD, audioOffset);
      break;
    default:
      /* Swap the entire packet */
      swap_short_word(csPacket + MHI_L2MUX_HEAD, frameLength);
    }

    VRG_LOG_DBG(( VRG_LOG_MOD_EPT, "MODEM_AUDIO_SPC_ENCODER_DATA_NTF - %d 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X\n", GET_TIME_STAMP(),
                  ((unsigned short*)(csPacket + MHI_L2MUX_HEAD))[0], ((unsigned short*)(csPacket + MHI_L2MUX_HEAD))[1],
                  ((unsigned short*)(csPacket + MHI_L2MUX_HEAD))[2], ((unsigned short*)(csPacket + MHI_L2MUX_HEAD))[3],
                  ((unsigned short*)(csPacket + MHI_L2MUX_HEAD))[4], ((unsigned short*)(csPacket + MHI_L2MUX_HEAD))[5],
                  ((unsigned short*)(csPacket + MHI_L2MUX_HEAD))[6], ((unsigned short*)(csPacket + MHI_L2MUX_HEAD))[7],
                  ((unsigned short*)(csPacket + MHI_L2MUX_HEAD))[8], ((unsigned short*)(csPacket + MHI_L2MUX_HEAD))[9]));

    l2mux_audio_tx (csPacket + MHI_L2MUX_HEAD, frameLength, PN_DEV_MODEM);
  } /* if ( gAudioULReady ) */
}
#endif /* VRG_ENDPT_2G3G_SUPPORT */
