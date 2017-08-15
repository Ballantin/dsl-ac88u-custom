/***************************************************************************
*
*     Copyright (c) 2002 Broadcom, All Rights Reserved.
*     Contains proprietary and confidential information.
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*
*      Le88010 definitions
*
****************************************************************************/

#ifndef DAA_Le88010_DEFS_H
#define DAA_Le88010_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define LE88010_NUM_CHANNELS                       1

/* 
** Timeslot selection.
** 
** The number programmed into the 88010 represents the number of bytes after the frame sync where the 
**  DAA data begins. Current configuration has 16 bit samples, therefore to select the third sample
**  (timeslots 4 and 5) we set the timeslot value of 4
*/

/* Ring Validation control */
/* RCC - Ring Confirmation Count (ring start to validated start) 
     United Kingdom: 256 ms (must be <400ms, ring burst length)
     Spain, France : 512 ms (must be >300ms, RPAS length, RP ring must not be seen as a normal ring burst)
     All the reset 256ms
   RDY - Ring Delay (detect to imterupt)
     All countries: 256ms
   RTO - Ring Timeout (ring end to validated end)
     United Kingdom: 256 ms (must be >200+25%, ring off period between two bursts in double cadence
     All the rest:   256 ms  
*/ 

#define LE88010_MAX_CMD_LENGTH              16

/*#######################################################################
 #                                                                      #
 #  REGISTER DEFINITIONS                                                #  
 #                                                                      #  
 #######################################################################*/ 
#define CMD_LE88010_WR_INVALID              0x00
#define CMD_LE88010_SW_RST                  0x02
#define CMD_LE88010_HW_RST                  0x04
#define CMD_LE88010_NOP                     0x06
#define CMD_LE88010_RD_PERIOD_DET           0x1F
#define CMD_LE88010_WR_TX_TS                0x40
#define CMD_LE88010_RD_TX_TS                0x41
#define CMD_LE88010_WR_RX_TS                0x42
#define CMD_LE88010_RD_RX_TS                0x43
#define CMD_LE88010_WR_CLK_SLOT_TX_EDGE     0x44
#define CMD_LE88010_RD_CLK_SLOT_TX_EDGE     0x45
#define CMD_LE88010_WR_CFG_REG              0x46
#define CMD_LE88010_RD_CFG_REG              0x47
#define CMD_LE88010_WR_CE_MODE_REG          0x4A
#define CMD_LE88010_RD_CE_MODE_REG          0x4B
#define CMD_LE88010_RD_SIG_REG              0x4F
#define    LE88010_SIG_REG_LIU_BIT          0x01
#define    LE88010_SIG_REG_RNGDT_BIT        0x02
#define    LE88010_SIG_REG_DIS_BIT          0x04
#define CMD_LE88010_WR_VP_GAINS             0x50
#define CMD_LE88010_RD_VP_GAINS             0x51
#define CMD_LE88010_WR_INOUT_DATA_REG       0x52
#define CMD_LE88010_RD_INOUT_DATA_REG       0x53
#define CMD_LE88010_WR_INOUT_DIR_REG        0x54
#define CMD_LE88010_RD_INOUT_DIR_REG        0x55
#define CMD_LE88010_WR_SYS_STATE            0x56
#define CMD_LE88010_RD_SYS_STATE            0x57
#define CMD_LE88010_WR_OP_FUNC              0x60
#define CMD_LE88010_RD_OP_FUNC              0x61
#define CMD_LE88010_WR_SYS_STATE_CFG        0x68
#define CMD_LE88010_RD_SYS_STATE_CFG        0x69
#define CMD_LE88010_WR_INT_MSK_REG          0x6C
#define CMD_LE88010_RD_INT_MSK_REG          0x6D
#define CMD_LE88010_WR_OP_COND              0x70
#define CMD_LE88010_RD_OP_COND              0x71
#define CMD_LE88010_RD_REV_PCN              0x73
#define CMD_LE88010_WR_GX_FILT_COEFF        0x80
#define CMD_LE88010_RD_GX_FILT_COEFF        0x81
#define CMD_LE88010_WR_GR_FILT_COEFF        0x82
#define CMD_LE88010_RD_GR_FILT_COEFF        0x83
#define CMD_LE88010_WR_B_FILT_FIR_COEFF     0x86
#define CMD_LE88010_RD_B_FILT_FIR_COEFF     0x87
#define CMD_LE88010_WR_X_FILT_COEFF         0x88
#define CMD_LE88010_RD_X_FILT_COEFF         0x89
#define CMD_LE88010_WR_R_FILT_COEFF         0x8A
#define CMD_LE88010_RD_R_FILT_COEFF         0x8B
#define CMD_LE88010_WR_B_FILT_IIR_COEFF     0x96
#define CMD_LE88010_RD_B_FILT_IIR_COEFF     0x97
#define CMD_LE88010_WR_Z_FILT_FIR_COEFF     0x98
#define CMD_LE88010_RD_Z_FILT_FIR_COEFF     0x99
#define CMD_LE88010_WR_Z_FILT_IIR_COEFF     0x9A
#define CMD_LE88010_RD_Z_FILT_IIR_COEFF     0x9B
#define CMD_LE88010_WR_CONV_CFG             0xA6
#define CMD_LE88010_RD_CONV_CFG             0xA7
#define CMD_LE88010_WR_LOOP_SPRV_PARMS      0xC2
#define CMD_LE88010_RD_LOOP_SPRV_PARMS      0xC3
#define CMD_LE88010_WR_DISN                 0xCA
#define CMD_LE88010_RD_DISN                 0xCB
#define CMD_LE88010_RD_TX_PCM_TEST          0xCD
#define CMD_LE88010_WR_SIG_GEN_AB_PARMS     0xD2
#define CMD_LE88010_RD_SIG_GEN_AB_PARMS     0xD3
#define CMD_LE88010_WR_SIG_GEN_CTRL         0xDE
#define CMD_LE88010_RD_SIG_GEN_CTRL         0xDF
#define CMD_LE88010_WR_CADENCE_TIMER        0xE0
#define CMD_LE88010_RD_CADENCE_TIMER        0xE1

#ifdef __cplusplus
}
#endif

#endif /* DAA_Le88010_DEFS_H */
