/****************************************************************************
*
*  Copyright (c) 2005-2008 Broadcom
*
*  This program is the proprietary software of Broadcom and/or
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
*    Description:
*		This is the Legerity Le88111 - Single Channel Digital SLIC Device
*		driver definition.
*
****************************************************************************/
#ifndef SLAC_LE88111_H    /* support nested includes */
#define SLAC_LE88111_H

#include <vrgCountry.h>

#include <xdrvSlic.h>
#include <bcmChip.h>
#include <board.h>

#define NUM_LINES    1

/* ---- Constants and Types ---------------------------------- */
/* With API-2, these constants are only needed in debug mode where we read registers directly */
#define LE88111_MAX_CMD_LENGTH              16

/* LE88111 commands set */
#define CMD_LE88111_WR_INVALID              0x00
#define CMD_LE88111_SW_RST                  0x02
#define CMD_LE88111_HW_RST                  0x04
#define CMD_LE88111_NOP                     0x06
#define CMD_LE88111_WR_TX_TS                0x40
#define CMD_LE88111_RD_TX_TS                0x41
#define CMD_LE88111_WR_RX_TS                0x42
#define CMD_LE88111_RD_RX_TS                0x43
#define CMD_LE88111_WR_CLK_SLOT_TX_EDGE     0x44
#define CMD_LE88111_RD_CLK_SLOT_TX_EDGE     0x45
#define CMD_LE88111_WR_CFG_REG              0x46
#define CMD_LE88111_RD_CFG_REG              0x47
#define CMD_LE88111_WR_CE_MODE_REG          0x4A
#define CMD_LE88111_RD_CE_MODE_REG          0x4B
#define CMD_LE88111_RD_SIG_REG              0x4F
#define CMD_LE88111_WR_VP_GAINS             0x50
#define CMD_LE88111_RD_VP_GAINS             0x51
#define CMD_LE88111_WR_INOUT_DATA_REG       0x52
#define CMD_LE88111_RD_INOUT_DATA_REG       0x53
#define CMD_LE88111_WR_INOUT_DIR_REG        0x54
#define CMD_LE88111_RD_INOUT_DIR_REG        0x55
#define CMD_LE88111_WR_SYS_STATE            0x56
#define CMD_LE88111_RD_SYS_STATE            0x57
#define CMD_LE88111_WR_OP_FUNC              0x60
#define CMD_LE88111_RD_OP_FUNC              0x61
#define CMD_LE88111_WR_SYS_STATE_CFG        0x68
#define CMD_LE88111_RD_SYS_STATE_CFG        0x69
#define CMD_LE88111_WR_INT_MSK_REG          0x6C
#define CMD_LE88111_RD_INT_MSK_REG          0x6D
#define CMD_LE88111_WR_OP_COND              0x70
#define CMD_LE88111_RD_OP_COND              0x71
#define CMD_LE88111_RD_REV_PCN              0x73
#define CMD_LE88111_WR_GX_FILT_COEFF        0x80
#define CMD_LE88111_RD_GX_FILT_COEFF        0x81
#define CMD_LE88111_WR_GR_FILT_COEFF        0x82
#define CMD_LE88111_RD_GR_FILT_COEFF        0x83
#define CMD_LE88111_WR_B_FILT_FIR_COEFF     0x86
#define CMD_LE88111_RD_B_FILT_FIR_COEFF     0x87
#define CMD_LE88111_WR_X_FILT_COEFF         0x88
#define CMD_LE88111_RD_X_FILT_COEFF         0x89
#define CMD_LE88111_WR_R_FILT_COEFF         0x8A
#define CMD_LE88111_RD_R_FILT_COEFF         0x8B
#define CMD_LE88111_WR_B_FILT_IIR_COEFF     0x96
#define CMD_LE88111_RD_B_FILT_IIR_COEFF     0x97
#define CMD_LE88111_WR_Z_FILT_FIR_COEFF     0x98
#define CMD_LE88111_RD_Z_FILT_FIR_COEFF     0x99
#define CMD_LE88111_WR_Z_FILT_IIR_COEFF     0x9A
#define CMD_LE88111_RD_Z_FILT_IIR_COEFF     0x9B
#define CMD_LE88111_WR_CONV_CFG             0xA6
#define CMD_LE88111_RD_CONV_CFG             0xA7
#define CMD_LE88111_WR_LOOP_SPRV_PARMS      0xC2
#define CMD_LE88111_RD_LOOP_SPRV_PARMS      0xC3
#define CMD_LE88111_WR_DC_FEED_PARMS        0xC6
#define CMD_LE88111_RD_DC_FEED_PARMS        0xC7
#define CMD_LE88111_WR_DISN                 0xCA
#define CMD_LE88111_RD_DISN                 0xCB
#define CMD_RD_TX_PCM_TEST_DATA            0xCD
#define CMD_LE88111_WR_METERING_PARMS       0xD0
#define CMD_LE88111_RD_METERING_PARMS       0xD1
#define CMD_LE88111_WR_SIG_GEN_AB_PARMS     0xD2
#define CMD_LE88111_RD_SIG_GEN_AB_PARMS     0xD3
#define CMD_LE88111_WR_SIG_GEN_CD_PARMS     0xD4
#define CMD_LE88111_RD_SIG_GEN_CD_PARMS     0xD5
#define CMD_LE88111_WR_SIG_GEN_CTRL         0xDE
#define CMD_LE88111_RD_SIG_GEN_CTRL         0xDF
#define CMD_LE88111_WR_CADENCE_TIMER        0xE0
#define CMD_LE88111_RD_CADENCE_TIMER        0xE1
#define CMD_LE88111_WR_CALLID_DATA          0xE2
#define CMD_LE88111_RD_CALLID_DATA          0xE3
#define CMD_LE88111_WR_SWREG_PARMS          0xE4
#define CMD_LE88111_RD_SWREG_PARMS          0xE5
#define CMD_LE88111_WR_SWREG_CTRL           0xE6
#define CMD_LE88111_RD_SWREG_CTRL           0xE7
#define CMD_LE88111_WR_CALLID_PARMS         0xEA
#define CMD_LE88111_RD_CALLID_PARMS         0xEB

/* Special constants used for specific commands */
#define LE88111_SIGNALING_REG_CMD_LENGTH    2
#define LE88111_HOOK_STATUS_BYTE_INDEX      0
#define LE88111_HOOK_STATUS_MASK            0x01

#define LE88111_SYS_STATE_CMD_LENGTH        1
#define LE88111_SYS_STATE_MASK              0x1F
#define LE88111_SYS_STATE_POLARITY_NORMAL   0x00
#define LE88111_SYS_STATE_POLARITY_REVERSE  0x10
#define LE88111_SYS_STATE_INVALID           0xFF

#define LE88111_SW_REG_CTL_CMD_LENGTH       1
#define LE88111_SW_REG_CTL_LP               0x01

#define LE88111_TIMESLOT              0

typedef enum
{ 
   SLIC_SUCCESS = 0,
   SLIC_ERROR
} SLIC_STATUS;

/* These SLIC modes match the system state register masks for SS[0:3] bits */
typedef enum
{
   LE88111_MODE_DISCONNECT   = 0,
   LE88111_MODE_TIP_OPEN     = 1,
   LE88111_MODE_RING_OPEN    = 2,
   LE88111_MODE_ACTIVE       = 3,
   LE88111_MODE_IDLE         = 4,
   LE88111_MODE_LONG_TEST    = 5,
   LE88111_MODE_MET_TEST     = 6,
   LE88111_MODE_BAL_RING     = 7,
   LE88111_MODE_LOW_GAIN     = 8,
   LE88111_MODE_UNBAL_RING   = 10,
   LE88111_MODE_SHUTDOWN     = 15
} SLIC_LE88111_MODE;


/* 
 * LE88111 SLIC information
 */
typedef struct SLIC_LE88111_DRV
{
   /* Must be first element of this struct */
   const XDRV_SLIC_FUNCS  *pDrvFuncs;

   XDRV_BOOL              bDrvEnabled; /* SLIC driver state (enabled/disabled) */
   
   /*  Le88111 config info */
   int pcmMode;                        /* pcm mode 16 bit linear, 8 bit u/A law */

#define LE88111_LINEAR        1
#define LE88111_ULAW          2
#define LE88111_ALAW          3
#define LE88116_WIDEBAND      4

   int txTimeSlot;                     /* pcm highway transmit timeslot */
   int rxTimeSlot;                     /* pcm highway receive timeslot */
   int mspiDevId;                      /* Device ID on MSPI bus  */   

   VRG_COUNTRY country;

} SLIC_LE88111_DRV;

/*
   Le88111 Register Value Definition.
*/
typedef struct
{
   XDRV_UINT8    regWR;
   XDRV_UINT8    regLen;
   XDRV_UINT8    regVal[LE88111_MAX_CMD_LENGTH];

} LE88111_REGISTER_VALUE;

/* ---- Variable Externs ------------------------------------- */

/* ---- Function Prototypes ---------------------------------- */

SLIC_STATUS slicLe88111Init( SLIC_LE88111_DRV* pDev, int resetGpioPin );
SLIC_STATUS slicLe88111Deinit( void );

#ifdef DYING_GASP_API
SLIC_STATUS slicLe88221Shutdown( XDRV_UINT16 rstGpioPin );
#endif

#endif   /* SLAC_LE88111_H */
