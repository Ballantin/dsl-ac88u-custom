/****************************************************************************
*    Copyright © 2008 Broadcom
*
*    This program is the proprietary software of Broadcom and/or
*    its licensors, and may only be used, duplicated, modified or
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized
*    License").  Except as set forth in an Authorized License, Broadcom
*    grants no license (express or implied), right to use, or waiver of any
*    kind with respect to the Software, and Broadcom expressly reserves all
*    rights in and to the Software and all intellectual property rights
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.
*
*
*    Except as expressly set forth in the Authorized License,
*
*    1.     This program, including its structure, sequence and
*    organization, constitutes the valuable trade secrets of Broadcom, and
*    you shall use all reasonable efforts to protect the confidentiality
*    thereof, and to use this information only in connection with your use
*    of Broadcom integrated circuit products.
*
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT
*    OF USE OR PERFORMANCE OF THE SOFTWARE.
*
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL,
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
*****************************************************************************
*
*  apm6828_dsp.h - BCM6828 APM Definitions for the DSP only
*
*  PURPOSE:
*
*  NOTES:
*
****************************************************************************/

#ifndef APM6828_DSP_H
#define APM6828_DSP_H

#if defined(__cplusplus)
  extern "C" {
#endif

/* ---- Include Files ----------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvApm.h>
#include <mipsDefines.h>


/* ---- Public Constants and Types ---------------------------------------- */

#define BCM6828_APM_CH_MIN          0
#define BCM6828_APM_CH_MAX          1
#define BCM6828_APM_CH_NUM          (BCM6828_APM_CH_MAX - BCM6828_APM_CH_MIN + 1)

/* ---- Constants and Types ---------------------------------------------- */

/*
** This likely doesn't belong here. It should go somewhere more generic, but
** I'm not sure where at the moment...
*/
#define ALIGN_DATA(addr, boundary) ((addr + boundary - 1) & ~(boundary - 1))


#define APM6828_NUM_CHANNELS             2  /* Number of channels supported by this device */
#define BCM6828_CHANNEL_0_ONLY           0  /* APM Ch0 Rx */
#define BCM6828_CHANNEL_1_ONLY           0  /* APM Ch0 Tx */

/*
** BCM6828 APM Register Structure and definitions
*/
typedef struct
{
    XDRV_UINT16      apm_dev_diag_sel;
#define   BCM6828_APM_DIAG_HI_SEL                 0xff00
#define   BCM6828_APM_DIAG_LO_SEL                 0x00ff

    XDRV_UINT16      apm_dev_irq_status;                     // (0x00) read only block interrupt status
#define   BCM6828_DEV_INTERRUPT                   0x0001        // DMA interrupt pending
#define   BCM6828_APM_INTERRUPT_1                 0x0002        // APM interrupt pending from bank 1
#define   BCM6828_APM_INTERRUPT_2                 0x0004        // APM interrupt pending from bank 2
#define   BCM6828_APM_INTERRUPT_3                 0x0008        // APM interrupt pending from bank 3
#define   BCM6828_PCM_INTERRUPT                   0x0010        // PCM interrupt pending

// semantic from IUDMA perpesctive
// Tx mem to APM
// Rx APM to mem
  XDRV_UINT32        apm_dev_irq_pend;                       // (0x04) DMA interrupt pending register
#define   BCM6828_DMA_A_RX                        0x00000001    // Ch A receive channel interrupt
#define   BCM6828_DMA_A_TX                        0x00000002    // Ch A transmit channel interrupt
#define   BCM6828_DMA_B_RX                        0x00000004    // Ch B receive channel interrupt
#define   BCM6828_DMA_B_TX                        0x00000008    // Ch B transmit channel interrupt
#define   BCM6828_DMA_TX_UNDERFLOW_A              0x00000010    // Ch A transmit channel underflow
#define   BCM6828_DMA_TX_UNDERFLOW_B              0x00000020    // Ch B transmit channel underflow
#define   BCM6828_DMA_RX_OVERFLOW_A               0x00000040    // Ch A receive channel overflow
#define   BCM6828_DMA_RX_OVERFLOW_B               0x00000080    // Ch B receive channel overflow
#define   BCM6828_DMA_PCM_RX                      0x00000100    // PCM Rx DMA IRQ
#define   BCM6828_DMA_PCM_TX                      0x00000200    // PCM Tx DMA IRQ
#define   BCM6828_DEV_BMU_IRQ                     0x00000400    // BMU block IRQ

  XDRV_UINT32        apm_dev_irq_mask;                       // (0x08)DMA interrupt mask register

// Note semantic change
// IUDMA refers to mem to periph as Tx
// and periph to mem as Rx
// APM core refers to path from mem as Rx
// and path to mem as Tx

  XDRV_UINT32        apm_dev_control;                        // (0x0c) Device control register
#define   BCM6828_RX_PACKET_SIZE_A                0x000000ff    // Number of samples to form quasi packet to mem channel A
#define   BCM6828_RX_PACKET_SIZE_B                0x0000ff00    // Number of samples to form quasi packet to mem channel B

#define   BCM6828_RX_DMA_ENABLE_A                 0x10000000    // Enable for Ch A DMA to mem
#define   BCM6828_RX_DMA_ENABLE_A_SHIFT           28

#define   BCM6828_RX_DMA_ENABLE_B                 0x20000000    // Enable for Ch B DMA to mem
#define   BCM6828_RX_DMA_ENABLE_B_SHIFT           29

#define   BCM6828_TX_DMA_ENABLE_A                 0x40000000    // Enable for Ch A DMA from mem
#define   BCM6828_TX_DMA_ENABLE_A_SHIFT           30

#define   BCM6828_TX_DMA_ENABLE_B                 0x80000000    // Enable for Ch B DMA from mem
#define   BCM6828_TX_DMA_ENABLE_B_SHIFT           31

// APM core registers
  XDRV_UINT32        reg_apm_coeff_wr_data;                  // (0x10) 20 bit 2's comp coefficient to be written into coeff RAM
#define   BCM6828_COEFF_PROG_INPUT                0x000FFFFF

  XDRV_UINT32        reg_apm_coeff_config;                   // (0x14)
#define   BCM6828_COEFF_PROG_ADDR                 0x000000FF    //8 bit address into coefficient RAM space
#define   BCM6828_COEFF_PROG_WE                   0x00000100    //1=write into memory 0= read from memory
#define   BCM6828_EQ_TX_ACCESS_COEFF_RAM_A        0x00010000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a equalizer
#define   BCM6828_EQ_RX_ACCESS_COEFF_RAM_A        0x00020000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a equalizer
#define   BCM6828_HISPD_HYBAL_ACCESS_COEFF_RAM_A  0x00040000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6828_LOSPD_HYBAL_ACCESS_COEFF_RAM_A  0x00080000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6828_YFLTR_ACCESS_COEFF_RAM_A        0x00100000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a hybrid balance
#define   BCM6828_ASRC_INT_ACCESS_COEFF_RAM_A     0x00200000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b hybrid balance
#define   BCM6828_ASRC_DEC_ACCESS_COEFF_RAM_A     0x00400000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a y filter
#define   BCM6828_EQ_TX_ACCESS_COEFF_RAM_B        0x00800000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b y filter
#define   BCM6828_EQ_RX_ACCESS_COEFF_RAM_B        0x01000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b y filter
#define   BCM6828_HISPD_HYBAL_ACCESS_COEFF_RAM_B  0x02000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6828_LOSPD_HYBAL_ACCESS_COEFF_RAM_B  0x04000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6828_YFLTR_ACCESS_COEFF_RAM_B        0x08000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b asrc interpolator
#define   BCM6828_ASRC_INT_ACCESS_COEFF_RAM_B     0x10000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc decimator
#define   BCM6828_ASRC_DEC_ACCESS_COEFF_RAM_B     0x20000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b asrc decimator

#define   APM6828_HYBAL_NUM_TAPS          5
#define   APM6828_YFLTR_FIR_NUM_TAPS      18
#define   APM6828_YFLTR_IIR1_NUM_TAPS     1
#define   APM6828_YFLTR_IIR2_NUM_TAPS     5
#define   APM6828_YFLTR_BLOCK_SIZE        28
#define   APM6828_RX_EQ_NUM_TAPS          80
#define   APM6828_TX_EQ_NUM_TAPS          80
#define   APM6828_ASRC_DEC_NUM_TAPS       120
#define   APM6828_ASRC_INT_NUM_TAPS       120

  XDRV_UINT32        reg_cha_test_config;                    // (0x18)
#define   BCM6828_RX_IN_MUX_CTRL                  0x00000003    //2 bits mux control  at eq input 0=test stimulus 1=rx_data_in 2=loopback from tx path channel a
#define   BCM6828_RX_IN_MUX_CTRL_SHIFT            0

#define   BCM6828_NETWORK_LPBK                    0x00000004    //1=take data inputs and loopback to data outputs (network side) channel a
#define   BCM6828_NETWORK_LPBK_SHIFT              2

#define   BCM6828_RX_98K_MUX_CTRL                 0x00000018    //1=bypass 0=use eq output on rx side channel a
#define   BCM6828_RX_98K_MUX_CTRL_SHIFT           3

#define   BCM6828_ADC_MUX_CTRL                    0x00000060    //2 bits mux at input to tx path 0=loopback 1=adc out 2=test stimulus channel a
#define   BCM6828_ADC_MUX_CTRL_SHIFT              5

#define   BCM6828_ADC_DAC_LPBK                    0x00000080    //1=take 4 bit ADC codes and send them to the DEM and DAC
#define   BCM6828_ADC_DAC_LPBK_SHIFT              7

#define   BCM6828_RX_TX_98K_LPBK                  0x00000100    //1=loopback from rx to tx path channel a
#define   BCM6828_RX_TX_98K_LPBK_SHIFT            8

#define   BCM6828_SLEW_INSTANT                    0x00000200    //1=slew scale, 0=make scale changes instantaneously
#define   BCM6828_SLEW_INSTANT_SHIFT              9

#define   BCM6828_ASRC_EN                         0x00000400    //1=asrc and eq are active 0=inactive
#define   BCM6828_ASRC_EN_SHIFT                   10


  XDRV_UINT32        reg_chb_test_config;                    // (0x1c)

  XDRV_UINT32        reg_cha_eq_config;                      // (0x20)
#define   BCM6828_EQ_RX_NUM_TAPS                  0x0000007f    //7 bit number of recieve taps
#define   BCM6828_EQ_RX_NUM_TAPS_SHIFT            0

#define   BCM6828_EQ_TX_NUM_TAPS                  0x00007f00    //7 bit number of transmit taps
#define   BCM6828_EQ_TX_NUM_TAPS_SHIFT            8

#define   BCM6828_EQ_TX_SHFT                      0x000f0000    //4 bits controls output shifter (powers of 2 gain control) for rx path channel a
#define   BCM6828_EQ_TX_SHFT_SHIFT                16

#define   BCM6828_EQ_RX_SHFT                      0x00f00000    //4 bits controls output shifter (powers of 2 gain control) for tx path channel a
#define   BCM6828_EQ_RX_SHFT_SHIFT                20

#define   BCM6828_EQ_RX_IMP_RESP                  0x01000000    //1=use ext RAM for eq coefficients 0=use int ROM for eq coefficients channel a
#define   BCM6828_EQ_RX_IMP_RESP_SHIFT            24

#define   BCM6828_EQ_TX_IMP_RESP                  0x02000000    //1 the eq has an impulse shape response, 0= use the RAM or ROM for eq coefficients
#define   BCM6828_EQ_TX_IMP_RESP_SHIFT            25

#define   BCM6828_EQ_RX_ENABLE                    0x04000000    //equalizer transmit enable
#define   BCM6828_EQ_RX_ENABLE_SHIFT              26

#define   BCM6828_EQ_TX_ENABLE                    0x08000000    //equalizer receive enable
#define   BCM6828_EQ_TX_ENABLE_SHIFT              27

#define   BCM6828_SOFT_INIT                       0x80000000    //initializes state machines and registers
#define   BCM6828_SOFT_INIT_SHIFT                 31

  XDRV_UINT32        reg_cha_hybal_config;                   // (0x24)
#define   BCM6828_HISPD_HYBAL_SHFT                0x00000007    //
#define   BCM6828_HISPD_HYBAL_SHFT_SHIFT          0

#define   BCM6828_LOSPD_HYBAL_SHFT                0x00000038    //
#define   BCM6828_LOSPD_HYBAL_SHFT_SHIFT          3

#define   BCM6828_HISPD_HYBAL_EN                  0x00000080    //
#define   BCM6828_HISPD_HYBAL_EN_SHIFT            7

#define   BCM6828_LOSPD_HYBAL_EN                  0x00000100    //
#define   BCM6828_LOSPD_HYBAL_EN_SHIFT            8

#define   BCM6828_HISPD_HYBAL_SMPL_OFFSET         0x00007000    //
#define   BCM6828_HISPD_HYBAL_SMPL_OFFSET_SHIFT   12

#define   BCM6828_YFLTR_EN                        0x00008000    //1=y-filter is active 0=y-filter outputs 0 only
#define   BCM6828_YFLTR_EN_SHIFT                  15

#define   BCM6828_HPF_EN                          0x00010000    //1=high pass filter is active 0=high pass filter outputs 0 only
#define   BCM6828_HPF_EN_SHIFT                    16

#define   BCM6828_LOSPD_HYBAL_SMPL_OFFSET         0x3f000000    //
#define   BCM6828_LOSPD_HYBAL_SMPL_OFFSET_SHIFT   24

  XDRV_UINT32        reg_cha_int_config_1;                   // (0x28)
#define   BCM6828_ASRC_INT_NUM_SECT               0x001f0000    //5 bits--if symmetric it represents 1/2 of the number of sections (minus 1) in the filter channel a
#define   BCM6828_ASRC_INT_NUM_SECT_SHIFT         16            //  if asymmetric, it represents the number of sections (minus 1) channel a

#define   BCM6828_ASRC_INT_HRNR_SHFT              0x03000000    //2 bits defines shifting at output of asrc int hrnr channel a
#define   BCM6828_ASRC_INT_HRNR_SHFT_SHIFT        24

#define   BCM6828_ASRC_INT_SYM_ASYM               0x04000000    //1=filter coefficients are symmetrical (only 1/2 are stored) channel a
#define   BCM6828_ASRC_INT_SYM_ASYM_SHIFT         26

#define   BCM6828_ASRC_INT_COEFF_SEL              0x18000000    //
#define   BCM6828_ASRC_INT_COEFF_SEL_SHIFT        27

  XDRV_UINT32        reg_cha_int_config_2;                   // (0x2c)
#define   BCM6828_ASRC_INT_SCALE                  0x0000ffff    //16 bit scale value channel a
#define   BCM6828_ASRC_INT_SCALE_SHIFT            0

#define   BCM6828_ASRC_INT_FIR_SHFT               0x00070000    //3 bits defines shifting at output of mac before writing into data RAM channel a
#define   BCM6828_ASRC_INT_FIR_SHFT_SHIFT         16

#define   BCM6828_ASRC_SLEW_SPEED                 0x00780000    //
#define   BCM6828_ASRC_SLEW_SPEED_SHIFT           19

  XDRV_UINT32        reg_cha_pm_config_1;                    // (0x30)
#define   BCM6828_PM_ON_TIME                      0x00000fff    //12 bit number in 2 msec units for on-time channel a
#define   BCM6828_PM_ON_TIME_SHIFT                0

#define   BCM6828_PM_OFF_TIME                     0x0fff0000    //12 bit number in 2 msec units for off-time channel a
#define   BCM6828_PM_OFF_TIME_SHIFT               16

#define   BCM6828_PM_FREQ_16_12                   0x10000000    //1=16 kHz frequency 0=12 kHz frequency channel a
#define   BCM6828_PM_FREQ_16_12_SHIFT             28

#define   BCM6828_PM_TIME_MS                      2 / 5


  XDRV_UINT32        reg_cha_pm_config_2;                    // (0x34)
#define   BCM6828_PM_SCALE                        0x0000ffff    //16 bit 2's complement number that scales the output from full scale channel a
#define   BCM6828_PM_SCALE_SHIFT                  0

#define   BCM6828_PM_RISE_INC                     0x00ff0000    //8 bit number goes into accumulator for rise time channel a
#define   BCM6828_PM_RISE_INC_SHIFT               16

#define   BCM6828_PM_FALL_INC                     0xff000000    //8 bit number goes into accumulator for fall time channel a
#define   BCM6828_PM_FALL_INC_SHIFT               24

  XDRV_UINT32        reg_cha_pm_config_3;                    // (0x38)
#define   BCM6828_PM_BURST_LENGTH                 0x0000ffff    //16 bit number defines the number of bursts to send out
#define   BCM6828_PM_BURST_LENGTH_SHIFT           0

#define   BCM6828_PM_BURST_CONT                   0x00010000    //1=send out infinite number of bursts 0=send out "pm_burst_length"
#define   BCM6828_PM_BURST_CONT_SHIFT             16

#define   BCM6828_PM_BURST_START                  0x00020000    //strobe the indicates that a burst should start (this will be edge detected)
#define   BCM6828_PM_BURST_START_SHIFT            17

#define   BCM6828_PM_ENABLE                       0x00040000    //1=pulse metering is active and on, 0=inactive and off channel a
#define   BCM6828_PM_ENABLE_SHIFT                 18

  XDRV_UINT32        reg_cha_cic_config;                     // (0x3c)
#define   BCM6828_CIC_DEC_SHFT                    0x00000007    //3 bits controls output shifter (powers of 2 gain control) channel a
#define   BCM6828_CIC_DEC_SHFT_SHIFT              0

#define   BCM6828_CIC_INC_SHFT                    0x00000038    //3 bits controls output shifter (powers of 2 gain control) channel a
#define   BCM6828_CIC_INC_SHFT_SHIFT              3

  XDRV_UINT32        reg_cha_asrc_dec_config;                // (0x40)
#define   BCM6828_ASRC_DEC_SCALE                  0x0000ffff    //16 bit 2's complement scale value channel a
#define   BCM6828_ASRC_DEC_SCALE_SHIFT            0

#define   BCM6828_ASRC_DEC_NUM_SECT               0x001f0000    //5 bit number 0-20 defines the number of sections in the filter channel a
#define   BCM6828_ASRC_DEC_NUM_SECT_SHIFT         16            //   if symmetric it equals the number of sections/2 -1
                                                        //   if asymmetric it equals the number of sections -1
#define   BCM6828_ASRC_DEC_SHFT                   0x0f000000    //4 bits defines msb location at input [38:31] channel a
#define   BCM6828_ASRC_DEC_SHFT_SHIFT             24

#define   BCM6828_ASRC_DEC_SYM_ASYM               0x10000000    //1=symmetric filter 0=asymmetric filter channel a
#define   BCM6828_ASRC_DEC_SYM_ASYM_SHIFT         28

#define   BCM6828_ASRC_DEC_COEFF_SEL              0x60000000    //ASRC DEC coefficient select
#define   BCM6828_ASRC_DEC_COEFF_SEL_SHIFT        29

  XDRV_UINT32        reg_cha_fcw_config_1;                   // (0x44)
#define   BCM6828_FCW_SCALE                       0xffffffff    //32 bit unsigned scale value for frequency control word channel a

  XDRV_UINT32        reg_cha_fcw_config_2;                   // (0x48)
#define   BCM6828_FCW_SHFT                        0x0000000f    //4 bit shift control for fcw channel a
#define   BCM6828_FCW_SHFT_SHIFT                  0

#define   BCM6828_CLOCK_SOURCE                    0x00000000    //2 bit mux control 0=DPLL 1=misc 2=NTP 3=TRC
#define   BCM6828_CLOCK_SOURCE_SHIFT              4


  XDRV_UINT32        reg_cha_digmod_config;                  // (0x4c)
#define   BCM6828_DIGMOD_DEM_TONE                 0x00000001    //1=suppress tones at fs/2 channel a
#define   BCM6828_DIGMOD_DEM_TONE_SHIFT           0

#define   BCM6828_DIGMOD_DEM_DIS                  0x00000002    //1=use balanced code as output instead of dynamic element matcher channel a
#define   BCM6828_DIGMOD_DEM_DIS_SHIFT            1

#define   BCM6828_DIGMOD_DITH_SCALE               0x0000ff00    // new 10/1
#define   BCM6828_DIGMOD_DITH_SCALE_SHIFT         8


// CH B use same bit position defines as Ch A
  XDRV_UINT32        reg_chb_eq_config;                      // (0x50)
  XDRV_UINT32        reg_chb_hybal_config;                   // (0x54)
  XDRV_UINT32        reg_chb_int_config_1;                   // (0x58)
  XDRV_UINT32        reg_chb_int_config_2;                   // (0x5c)

  XDRV_UINT32        reg_chb_pm_config_1;                    // (0x60)
  XDRV_UINT32        reg_chb_pm_config_2;                    // (0x64)
  XDRV_UINT32        reg_chb_pm_config_3;                    // (0x68)
  XDRV_UINT32        reg_chb_cic_config;                     // (0x6c)
  XDRV_UINT32        reg_chb_asrc_dec_config;                // (0x70)
  XDRV_UINT32        reg_chb_fcw_config_1;                   // (0x74)
  XDRV_UINT32        reg_chb_fcw_config_2;                   // (0x78)
  XDRV_UINT32        reg_chb_digmod_config;                  // (0x7c)

  XDRV_UINT32        reg_fcw_config_1;                       // (0x80)
#define   BCM6828_FCW_REG                         0xffffffff    //32 bit frequency control word from NTP block

  XDRV_UINT32        reg_fcw_config_2;                       // (0x84)
#define   BCM6828_LOAD_NTP                        0x00000001    // triggers pulse to load FCW to NTP
#define   BCM6828_LOAD_NTP_SHIFT                  0

#define   BCM6828_LOAD_MISC_A                     0x00000002    // triggers pulse to load FCW to ch A
#define   BCM6828_LOAD_MISC_A_SHIFT               1

#define   BCM6828_LOAD_MISC_B                     0x00000004    // triggers pulse to load FCW to ch B
#define   BCM6828_LOAD_MISC_B_SHIFT               2

#define   BCM6828_LOAD_FCW_NOM_A                  0x00000008    //strobe to load nominal FCW value
#define   BCM6828_LOAD_FCW_NOM_A_SHIFT            3

#define   BCM6828_LOAD_FCW_NOM_B                  0x00000010    //strobe to load nominal FCW value
#define   BCM6828_LOAD_FCW_NOM_B_SHIFT            4

#define   BCM6828_FCW_LIMIT                       0x00000f00    //4 bits defines allowable error on FCW relative to nominal
#define   BCM6828_FCW_LIMIT_SHIFT                 8

  XDRV_UINT32        reg_ntp_config_1;                       // (0x88)
#define   BCM6828_NTP_TIME_DVD                    0x0000ffff    //16 bit divide value for time counter (16'd374 is default)

  XDRV_UINT32        reg_ntp_config_2;                       // (0x8c)
#define   BCM6828_NTP_READ_TIME                   0x00000001    //strobe asks for new time count value

  XDRV_UINT32        reg_ring_config_1;                      // (0x90)
#define   BCM6828_RING_OFFSET_A                   0x000000ff    //
#define   BCM6828_RING_OFFSET_A_SHIFT             0

#define   BCM6828_RING_SCALE_A                    0x0000ff00    //
#define   BCM6828_RING_SCALE_A_SHIFT              8

#define   BCM6828_RING_OFFSET_B                   0x00ff0000    //
#define   BCM6828_RING_OFFSET_B_SHIFT             16

#define   BCM6828_RING_SCALE_B                    0xff000000    //
#define   BCM6828_RING_SCALE_B_SHIFT              24

  XDRV_UINT32        reg_ring_config_2;                      // (0x94)
#define   BCM6828_RING_CLIP_A                     0x00007fff    //15 bit number defining the clipping value to flatten
#define   BCM6828_RING_CLIP_A_SHIFT               0

#define   BCM6828_RING_GEN_ENABLE_A               0x00008000    //1=enabled
#define   BCM6828_RING_GEN_ENABLE_A_SHIFT         15

#define   BCM6828_RING_CLIP_B                     0x7fff0000    //15 bit number defining the clipping value to flatten
#define   BCM6828_RING_CLIP_B_SHIFT               16

#define   BCM6828_RING_GEN_ENABLE_B               0x80000000    //1=enabled
#define   BCM6828_RING_GEN_ENABLE_B_SHIFT         31

  XDRV_UINT32        reg_ring_config_3;                      // (0x98)
#define   BCM6828_RING_OMEGA_NORM_A               0x000000ff    //the omega man is Charlton Heston on channel A
#define   BCM6828_RING_OMEGA_NORM_A_SHIFT         0

#define   BCM6828_RING_OMEGA_NORM_B               0x0000ff00    //the omega man is Charlton Heston on channel B
#define   BCM6828_RING_OMEGA_NORM_B_SHIFT         8

#define   BCM6828_RING_BREAK_PHASE_A              0x003f0000    //break phase
#define   BCM6828_RING_BREAK_PHASE_A_SHIFT        16

#define   BCM6828_RING_BREAK_PHASE_B              0x0fc00000    //break phase
#define   BCM6828_RING_BREAK_PHASE_B_SHIFT        22

#define   BCM6828_RING_BREAK_PHASE_SIGN_A         0x10000000    //1=break phase sign
#define   BCM6828_RING_BREAK_PHASE_SIGN_A_SHIFT   28

#define   BCM6828_RING_BREAK_PHASE_SIGN_B         0x20000000    //1=break phase sign
#define   BCM6828_RING_BREAK_PHASE_SIGN_B_SHIFT   29

#define   BCM6828_RING_START_CADENCE_A            0x40000000    //1=start cadence
#define   BCM6828_RING_START_CADENCE_A_SHIFT      30

#define   BCM6828_RING_START_CADENCE_B            0x80000000    //1=start cadence
#define   BCM6828_RING_START_CADENCE_B_SHIFT      31

  XDRV_UINT32        reg_ring_config_4;                      // (0x9c)
#define   BCM6828_RING_STOP_IMMED_A               0x00000001    // stop ring now!
#define   BCM6828_RING_STOP_IMMED_A_SHIFT         0

#define   BCM6828_RING_STOP_IMMED_B               0x00000002    // stop ring now!
#define   BCM6828_RING_STOP_IMMED_B_SHIFT         1

#define   BCM6828_RING_STOP_NEXT_BREAK_A          0x00000004    // stop ring at next break
#define   BCM6828_RING_STOP_NEXT_BREAK_A_SHIFT    2

#define   BCM6828_RING_STOP_NEXT_BREAK_B          0x00000008    // stop ring at next break
#define   BCM6828_RING_STOP_NEXT_BREAK_B_SHIFT    3

  XDRV_UINT32        reg_ser_config;                         // (0xa0)
#define   BCM6828_SER_MUX_SEL                     0x00000003
#define   BCM6828_SER_MUX_SEL_SHIFT               0

//#define   SER_TST_SHFT_CTRL               0x07800000    //
#define   BCM6828_AUD_SER_TST_FCW_MUX_CTRL        0x08000000    //
#define   BCM6828_AUD_SER_TST_FCW_MUX_CTRL_SHIFT  27

//#define   SER_TST_LOAD_REGFILE            0x20000000    //1->use channel b requests for transfer 0-> use channel a
#define   BCM6828_SER_TST_CLKS                    0x40000000    //1=use clocks supplied by SER 0=generate internal clocks
#define   BCM6828_SER_TST_CLKS_SHIFT              30

#define   BCM6828_SOFT_INIT_AP                    0x80000000
#define   BCM6828_SOFT_INIT_AP_SHIFT              31

  XDRV_UINT32        reg_spare;                              // (0xa4)
//#define   SER_TST_SCALE                   0xffffffff

  XDRV_UINT32        reg_stats_config;                            // (0xa8)
#define   BCM6828_START_STATS                     0x00008000    //[15]
#define   BCM6828_STATS_MUX_CTL_VADCA             0x00000800    //[11]
#define   BCM6828_STATS_MUX_CTL_VADCB             0x00000400    //[10]
#define   BCM6828_STATS_CHOP_ENABLE_A             0x00000200    //[9]
#define   BCM6828_STATS_CHOP_ENABLE_B             0x00000100    //[8]
#define   BCM6828_STATS_DURATION                  0x000000f0    //[7:4]
#define   BCM6828_STATS_CHOP_PERIOD               0x0000000f    //[3:0]

  // APM Status Registers
  XDRV_UINT32        reg_apm_status_1;                       // (0xac)
#define   BCM6828_DIGMOD_SAT_QUANT                0x01000000    //strobe indicating quantizer has saturated
#define   BCM6828_DIGMOD_SAT_COUNT_0              0x00ff0000    //8 bit counter of saturation on integrator 0 channel a
#define   BCM6828_DIGMOD_SAT_COUNT_1              0x0000ff00    //8 bit counter of saturation on integrator 1 channel a
#define   BCM6828_DIGMOD_SAT_COUNT_QUANT          0x000000ff    //8 bit counter of saturation on quantizer channel a

  XDRV_UINT32        reg_apm_status_2;                       // (0xb0) same as status 1 for ch B

  XDRV_UINT32        reg_apm_status_3;                       // (0xb4)
#define   BCM6828_AP_XMT_UFLOW                    0x00200000    //level indicating xnt fifo is empty when read attempted
#define   BCM6828_AP_XMT_OVLOW                    0x00100000    //level indicating xmt fifo is full when write attempted
#define   BCM6828_AP_NCO_SATURATED                0x00080000    // NCO is saturated
#define   BCM6828_AP_XMT_FIFO_DEPTH               0x00070000    //3 bit depth number on xmt fifo
#define   BCM6828_AP_RCV_UFLOW                    0x00000020    //level indicating rcv fifo is empty when read attempted
#define   BCM6828_AP_RCV_OVLOW                    0x00000010    //level indicating rcv fifo is full when write attempted
#define   BCM6828_AP_RCV_FIFO_DEPTH               0x00000007    //3 bit depth number on receive fifo

  XDRV_UINT32        reg_apm_status_4;                       // (0xb8)
#define   BCM6828_EQ_RX_COEFF                     0xffff0000    //16 bit 2's complement coefficients from RAM
#define   BCM6828_EQ_TX_COEFF                     0x0000ffff    //16 bit coefficient read from RAM or ROM

  XDRV_UINT32        reg_apm_status_5;                       // (0xbc)
#define   BCM6828_ASRC_INT_COEFF                  0xffff0000    //16 bit 2's comp current coefficient value
#define   BCM6828_ASRC_DEC_COEFF                  0x0000ffff    //16 bit coefficient from RAM or ROM

  XDRV_UINT32        reg_apm_status_6;                       // (0xc0)
#define   BCM6828_YFLTR_COEFF                     0x000fffff    //20 bit 2's complement coefficients from RAM

// Status 9-7 same as 6-4 for Ch B
  XDRV_UINT32        reg_apm_status_7;                       // (0xc4)
  XDRV_UINT32        reg_apm_status_8;                       // (0xc8)
  XDRV_UINT32        reg_apm_status_9;                       // (0xcc)

  XDRV_UINT32        reg_apm_status_10;                      // (0xd0)
#define   BCM6828_NTP_TIME_COUNT_INTEGER          0x0001ffff    //17 integer bits (in seconds)

  XDRV_UINT32        reg_apm_status_11;                      // (0xd4)
#define   BCM6828_NTP_TIME_COUNT_FRACT            0x0003ffff    //18 fractional bits (in seconds)

  XDRV_UINT32        reg_apm_status_12;                      // (0xd8)
#define   BCM6828_RING_CADENCE_STOPPED_A          0x00000008    //
#define   BCM6828_RING_ON_OFF_A                   0x00000004    //
#define   BCM6828_RING_CADENCE_STOPPED_B          0x00000002    //
#define   BCM6828_RING_ON_OFF_B                   0x00000001    //

  XDRV_UINT32        reg_apm_status_13;                      // (0xdc)
#define   BCM6828_HISPD_HYBAL_COEFF_A             0xffff0000    //
#define   BCM6828_HISPD_HYBAL_COEFF_B             0x0000ffff    //

  XDRV_UINT32        reg_apm_status_14;                      // (0xe0)
#define   BCM6828_LOSPD_HYBAL_COEFF_A             0xffff0000    //
#define   BCM6828_LOSPD_HYBAL_COEFF_B             0x0000ffff    //

  XDRV_SINT32        reg_apm_stats_1;                        // (0xe4)
#define   BCM6828_MEAN                            0xffffffff    // 16 bit mean stats

  XDRV_SINT32        reg_apm_stats_2;                        // (0xe8), same as reg_apm_stats_1 for channel B
  XDRV_UINT32        reg_apm_stats_3;                        // (0xec)
#define   BCM6828_MEAN_SQUARE_LO                  0xffffffff    // lower 32 bits of 40 bit mean square value

  XDRV_UINT32        reg_apm_stats_4;                        // (0xf0)
#define   BCM6828_MEAN_SAT                        0x00040000
#define   BCM6828_MEAN_SQUARE_SAT                 0x00020000
#define   BCM6828_STATS_BUSY                      0x00010000    // 1 bit stats busy
#define   BCM6828_MEAN_SQUARE_HI                  0x00003fff    // upper 14 bits of 46 bit mean square value

  XDRV_UINT32        reg_apm_stats_5;                        // (0xf4), same as reg_apm_stats_3 for channel B
  XDRV_UINT32        reg_apm_stats_6;                        // (0xf8), same as reg_apm_stats_4 for channel B

  XDRV_UINT32        reg_apm_audio_fcw_a;                    // (0xfc)
  XDRV_UINT32        reg_apm_audio_fcw_b;                    // (0x100)

  XDRV_UINT32        reg_apm_irq_pend_1;                     // (0x104) Ch A status
#define   BCM6828_PM_BURST_DONE                   0x80000000    //strobe indicating that PM burst has completed
#define   BCM6828_ASRC_INT_DONE_SLEWING           0x40000000    //strobe indicating that slewing has completed
#define   BCM6828_ASRC_DEC_DONE_SLEWING           0x20000000    //strobe indicating that slewing has completed
#define   BCM6828_RX_PATH_SAT                     0x10000000    //strobed high when audio + pulse meter + y-filter exceeds 16 bit number
#define   BCM6828_EQ_TX_SAT                       0x08000000    //strobe indicates that the result on the eq tx path was saturated
#define   BCM6828_EQ_RX_SAT                       0x04000000    //strobe indicates that the result on the rx path was saturated at output
#define   BCM6828_EQ_TX_RQST_ERR                  0x02000000    //strobe indicating a tx_rqst was asked for while a tx was pending
#define   BCM6828_EQ_RX_RQST_ERR                  0x01000000    //strobe indicating a rx_rqst was asked for while a rx was pending
#define   BCM6828_LOSPD_HYBAL_SAT                 0x00800000    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6828_HISPD_HYBAL_SAT                 0x00400000    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6828_HISPD_HYBAL_RQST_ERR            0x00200000    //strobe indicating a output_rqst was asked for while computation engine was busy
#define   BCM6828_YFLTR_IIR1_SAT                  0x00100000    //strobe indicating iir-1 is saturating
#define   BCM6828_YFLTR_FIR_SAT                   0x00080000    //strobe indicating fir is saturating
#define   BCM6828_YFLTR_IIR2_SAT                  0x00040000    //strobe indicating iir-2 is saturating
#define   BCM6828_YFLTR_IIR1_ACCUM_SAT            0x00020000    //strobe indicating iir1 accumulator saturated
#define   BCM6828_YFLTR_FIR_ACCUM_SAT             0x00010000    //strobe indicating fir accumulator saturated
#define   BCM6828_YFLTR_IIR2_ACCUM_SAT            0x00008000    //strobe indicating iir2 accumulator saturated
#define   BCM6828_YFLTR_RQST_ERR                  0x00004000    //strobe indicating a output_rqst was asked for while computation engine was busy
#define   BCM6828_NCO_SAT                         0x00002000    //strobe indicates scaling and shifting saturated the result
#define   BCM6828_ASRC_INT_HRNR_TIM_ERR           0x00001000    //strobe indicating delta has rolled over too quickly before fir has completed computation
#define   BCM6828_ASRC_INT_HRNR_SAT               0x00000800    //strobe indicating data has saturated at the mac output
#define   BCM6828_ASRC_INT_FIR_TIM_ERR            0x00000400    //strobe indicating delta has rolled over too quickly before fir has completed computation
#define   BCM6828_ASRC_INT_FIR_BANK_ERR           0x00000200    //strobe indicating that horner ngn asked for a bank switch while fir was busy
#define   BCM6828_ASRC_INT_FIR_SAT                0x00000100    //strobe indicating data has saturated at the mac output
#define   BCM6828_ASRC_INT_HRNR_MAC_SAT           0x00000080    //strobe indicating ser mac in hrnr block has saturated
#define   BCM6828_CIC_INT_SAT                     0x00000040    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6828_DIGMOD_SAT_0                    0x00000020    //strobe indicating the integrator 0 has saturated
#define   BCM6828_DIGMOD_SAT_1                    0x00000010    //strobe indicating integrator 1 has saturated
#define   BCM6828_TX_PATH_SAT                     0x00000008
#define   BCM6828_CIC_DEC_SAT                     0x00000004    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6828_ASRC_DEC_TIM_ERR                0x00000002    //strobe asserted when "input_data_ready" is asserted while ngn is busy
#define   BCM6828_ASRC_DEC_SAT                    0x00000001    //strobe indicating that the input was saturated

  XDRV_UINT32        reg_apm_irq_mask_1;                     // (0x108)

  XDRV_UINT32        reg_apm_irq_pend_2;                     // (0x10c) same as APM_IRQ_1 for Ch B
  XDRV_UINT32        reg_apm_irq_mask_2;                     // (0x110)

  XDRV_UINT32        reg_apm_irq_pend_3;                     // (0x114)
#define   BCM6828_STATS_DONE_A                    0x00000400    //strobe indicating statistics gathered and computed chA
#define   BCM6828_STATS_DONE_B                    0x00000200    //strobe indicating statistics gathered and computed chB
#define   BCM6828_FCW_OOB_A                       0x00000100    //level indicating that FCW error is too large
#define   BCM6828_FCW_OOB_B                       0x00000080    //level indicating that FCW error is too large
#define   BCM6828_HPF_SAT_A                       0x00000040    //strobe indicating high pass filter has saturated, Channel A
#define   BCM6828_HPF_SAT_B                       0x00000020    //strobe indicating high pass filter has saturated, Channel B
#define   BCM6828_ASRC_INT_DELTA_ADJ_DONE_A       0x00000010    //strobe indicating TODO
#define   BCM6828_ASRC_INT_DELTA_ADJ_DONE_B       0x00000008    //strobe indicating TODO
#define   BCM6828_LOSPD_HYBAL_RQST_ERR_A          0x00000004    //strobe indicating a output_rqst was asked for while computation engine was busy, channel A
#define   BCM6828_LOSPD_HYBAL_RQST_ERR_B          0x00000002    //strobe indicating a output_rqst was asked for while computation engine was busy, channel B
#define   BCM6828_NTP_TIME_VALID                  0x00000001    //strobe in clk_200 domain (4 clock periods)

  XDRV_UINT32        reg_apm_irq_mask_3;                     // (0x118)
  XDRV_UINT32        reg_spare_2;                            // (0x11c)

  XDRV_UINT32        reg_codec_config_0;                     // (0x120)
#define   BCM6828_REF_CONTROL                     0x00000007    // control for bandgap voltage switching matrix
#define   BCM6828_REF_REFAMP_PD                   0x00000010    // power down for 50uA ref amp
#define   BCM6828_REF_BIASAMP_PD                  0x00000020    // power down for 10uA bias amp
#define   BCM6828_REF_BANDGAP_PD                  0x00000040    // power down for bandgap
#define   BCM6828_REF_SPARE                       0x00000f00    // reserved for future use

  XDRV_UINT32        reg_codec_config_1;                     // (0x124) VTX1
#define   BCM6828_VTX_PGA_MUX                     0x00000300    // analog loopback VDAC out to MICPGA in
#define   BCM6828_VTX_PG                          0x00001800    // gain setting
#define   BCM6828_VTX_CALEN                       0x00002000    // calibration enable
#define   BCM6828_VTX_OSCRST                      0x00004000    // unstable status reset
#define   BCM6828_VTX_PWD                         0x00008000    // ADC power down
#define   BCM6828_VTX_VOCM_CTL                    0x00070000    // opamp output common mode control
#define   BCM6828_VTX_FBIAS_CTL                   0x00300000    // flash bias control
#define   BCM6828_VTX_INT1BIAS_CTL                0x00c00000    // int1 bias control
#define   BCM6828_VTX_INT2BIAS_CTL                0x03000000    // int2 bias control
#define   BCM6828_VTX_VOS_ENA                     0x04000000    // ADC voltage offset enable
#define   BCM6828_VTX_SHUFF_DIS                   0x08000000    // shuffle disable
#define   BCM6828_VTX_SHUFF_DELAY_DIS             0x10000000    // shuffle feedback delay disable
#define   BCM6828_VTX_DITHER_DIS                  0x20000000    // dither disable
#define   BCM6828_VTX_DCANCEL_DIS                 0x40000000    // dither cancellation algorithm disable

#define   BCM6828_PGA_CTL_VADC                    0x30000000
#define   BCM6828_PGA_CTL_VADC_SHIFT                      28

#define   BCM6828_PGA_GAIN_CTL_VDAC_A             0x00000100
#define   BCM6828_PGA_GAIN_CTL_VDAC_A_SHIFT                8

#define   BCM6828_PGA_GAIN_CTL_VDAC_B             0x00000010
#define   BCM6828_PGA_GAIN_CTL_VDAC_B_SHIFT                4

  XDRV_UINT32        reg_codec_config_2;                     // (0x128) same as config 16 for VTX2

  XDRV_UINT32        reg_codec_config_3;                     // (0x12c) VRX1
#define   BCM6828_VRX_PG                          0x00000003    // PGA gain setting
#define   BCM6828_VRX_ASP_BYPASS                  0x00000080    // bypass testmode
#define   BCM6828_VRX_AUXPG                       0x00000300    // aux PGA gain setting
#define   BCM6828_VRX_PD                          0x00008000    // power down
#define   BCM6828_VRX_CMCTL                       0x00070000    // common mode voltage control
#define   BCM6828_VRX_PDDELAY                     0x00300000
#define   BCM6828_VRX_IDAC_CM_CTL                 0x03000000    // IDAC common mode voltage control
#define   BCM6828_VRX_ALOOPBACK                   0x0c000000    // Analog loopback test mode

  XDRV_UINT32        reg_codec_config_4;                     // (0x130) same as config 18 for VRX2
#define   BCM6828_VREF_CTL_VDAC                   0x70000000
#define   BCM6828_PWRDN_REF_VDAC                  0x01000000
#define   BCM6828_PWRDN_REF_VDAC_A                0x00020000
#define   BCM6828_TESTEN_VDAC_A                   0x00010000
#define   BCM6828_DRVVOCMCTL_VDAC                 0x00007000
#define   BCM6828_PGA_GAIN_CTL_VDAC_A             0x00000100
#define   BCM6828_PWRDN_REF_VDAC_B                0x00000040
#define   BCM6828_TESTEN_VDAC_B                   0x00000020
#define   BCM6828_PGA_GAIN_CTL_VDAC_B             0x00000010

  XDRV_UINT32        reg_codec_config_5;                     // (0x134) 32Khz Ring Gen config
#define   BCM6828_RING_PD                         0x00000001
#define   BCM6828_RING_RESETB                     0x00000002
#define   BCM6828_REF_RESETB                      0x00000004
#define   BCM6828_VTX1_RESETB                     0x00000008
#define   BCM6828_VTX2_RESETB                     0x00000010
#define   BCM6828_VRX1_RESETB                     0x00000020
#define   BCM6828_VRX2_RESETB                     0x00000040

  XDRV_UINT32        reg_codec_config_6;                     // (0x138) reserved
  XDRV_UINT32        reg_codec_config_7;                     // (0x13c) reserved
  XDRV_UINT32        reg_codec_config_8;                     // (0x140) reserved

  XDRV_UINT32        reg_apm_codec_fb_0;                     // (0x144) 32Khz Feedback
  XDRV_UINT32        reg_mbist_0;                            // (0x148) MBIST control register
#define   BCM6828_P16_CORE_INST_MEM_TM            0x0000000f
#define   BCM6828_P16_CORE_INST_ROM_TM            0x000001f0
#define   BCM6828_BMU_BUS_MAIN_IF_TM              0x00000600
#define   BCM6828_APM_EQ_DATA_RAM_TM_RX_A         0x00001800
#define   BCM6828_APM_EQ_DATA_RAM_TM_RX_B         0x00006000
#define   BCM6828_APM_EQ_DATA_RAM_TM_TX_A         0x00018000
#define   BCM6828_APM_EQ_DATA_RAM_TM_TX_B         0x00060000
#define   BCM6828_APM_EQ_COEFF_RAM_TM_RX_A        0x00180000
#define   BCM6828_APM_EQ_COEFF_RAM_TM_RX_B        0x00600000
#define   BCM6828_APM_EQ_COEFF_RAM_TM_TX_A        0x01800000
#define   BCM6828_APM_EQ_COEFF_RAM_TM_TX_B        0x06000000
#define   BCM6828_APM_Y_FLTR_TM_A                 0x18000000
#define   BCM6828_APM_Y_FLTR_TM_B                 0x60000000
  XDRV_UINT32        reg_mbist_1;                            // (0x14c) MBIST control register
#define   BCM6828_ASRC_DEC_COEFF_RAM_TM_A         0x00000003
#define   BCM6828_ASRC_DEC_DATA_RAM_TM_A          0x0000000c
#define   BCM6828_ASRC_DEC_COEFF_RAM_TM_B         0x00000030
#define   BCM6828_ASRC_DEC_DATA_RAM_TM_B          0x000000c0
#define   BCM6828_ASRC_INTERPFIR_COEFF_RAM_TM_A   0x00000300
#define   BCM6828_ASRC_INTERPFIR_DATA_RAM_TM_A    0x00000c00
#define   BCM6828_ASRC_INTERPFIR_COEFF_RAM_TM_B   0x00003000
#define   BCM6828_ASRC_INTERPFIR_DATA_RAM_TM_B    0x0000c000
  XDRV_UINT32        reg_otp_0;                              // (0x150) OTP read back, otp_apm[31:0]
#define   BCM6828_APM_OTP_TRIM                    0x00000FC0
#define   BCM6828_APM_OTP_TRIM_SHIFT                       6
  XDRV_UINT32        reg_otp_1;                              // (0x154) OTP read back, otp_apm[63:32]
  XDRV_UINT32        reg_diag_readback;                      // (0x158) diag bus read back

  XDRV_UINT32        audio_spare2;                           // (0x15c) Audio Spare Register 2
  XDRV_UINT32        dpll_control;                           // (0x160) DPLL control register
#define   BCM6828_DPLL_SOFT_INIT                  0x80000000
#define   BCM6828_DPLL_FORCE_ACQ                  0x40000000
#define   BCM6828_DPLL_OPEN_LOOP                  0x20000000
#define   BCM6828_DPLL_CONST_K                    0x10000000
#define   BCM6828_DPLL_PHS_THSHLD_SHIFT           4
  XDRV_UINT32        dpll_nom_freq;                          // (0x164) DPLL nominal frequency (control)
  XDRV_UINT32        dpll_div;                               // (0x168) DPLL divide register
#define   BCM6828_DPLL_REF_DIV_SHIFT              16
  XDRV_UINT32        dpll_acq_freq;                          // (0x16c) DPLL acquired frequency
  XDRV_UINT32        dpll_status;                            // (0x170) DPLL status register
#define   BCM6828_DPLL_IN_SYNC                    0x80000000
#define   BCM6828_DPLL_ACQ_FREQ_VALID             0x40000000
#define   BCM6828_DPLL_IN_SYNC                    0x80000000
#define   BCM6828_DPLL_K0_SHIFT                   8

   /* PCM registers - these are detailed in pcm6828.h */
   /* HVG registers - these are detailed in hvg6828.h */
} BCM6828_APM;

#define BCM6828_APM_PTR               ((volatile BCM6828_APM *)BCM6828_APM_BASE)


#define BCM6828_DMA_RX_NUM_BUFFERS    2     /* Only need two buffers for double buffering */
#define BCM6828_DMA_TX_NUM_BUFFERS    2     /* Only need two buffers for double buffering */

#define APM6828_DMA_MAX_BUFFER_SIZE 48    /* This is in 32-bit words.
                                      **  Allocate enaough for (5ms + 1ms prime) worth of 16kHz data:
                                      **   1ms=16*16-bit=8 32-bit words
                                      **  6ms=6*8*32-bit=48 32bit words */

#define APM6828_DMA_MAX_BUFFER_SIZE_BYTES   (APM6828_DMA_MAX_BUFFER_SIZE*sizeof(XDRV_UINT32))



#if defined(__cplusplus)
}
#endif

#endif /* APM6828_DSP_H */
