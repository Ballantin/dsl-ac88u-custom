/*
<:copyright-broadcom 
 
 Copyright (c) 2013 Broadcom 
 All Rights Reserved 
 No portions of this material may be reproduced in any form without the 
 written permission of: 
          Broadcom 
          16215 Alton Parkway 
          Irvine, California 92619 
 All information contained in this document is Broadcom 
 company private, proprietary, and trade secret. 
 
:>
*/
/***********************************************************************/
/*                                                                     */
/*   MODULE:  map63148.h                                               */
/*   DATE:    09/08/13                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM63148 used by the DSP only                            */
/*                                                                     */
/***********************************************************************/
#ifndef __MAP_63148_H
#define __MAP_63148_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bcmtypesCommon.h>
#include "common63148.h"

/*
** BCM APM Register Structure and definitions - APM is logic core it NOT present in 63148
*/
typedef struct BCM63148ApmControlRegisters
{
  uint32      apm_dev_diag_sel_irq_status;                     // (0x00) read only block interrupt status
#define   BCM63148_APM_DIAG_HI_SEL                 0xff000000
#define   BCM63148_APM_DIAG_LO_SEL                 0x00ff0000
#define   BCM63148_DEV_INTERRUPT                   0x00000001        // DMA interrupt pending
#define   BCM63148_APM_INTERRUPT_1                 0x00000002        // APM interrupt pending from bank 1
#define   BCM63148_APM_INTERRUPT_2                 0x00000004        // APM interrupt pending from bank 2
#define   BCM63148_APM_INTERRUPT_3                 0x00000008        // APM interrupt pending from bank 3
#define   BCM63148_PCM_INTERRUPT                   0x00000010        // PCM interrupt pending

// semantic from IUDMA perpesctive
// Tx mem to APM
// Rx APM to mem
  uint32        apm_dev_irq_pend;                       // (0x04) DMA interrupt pending register
#define   BCM63148_DMA_A_RX                        0x00000001    // Ch A receive channel interrupt
#define   BCM63148_DMA_A_TX                        0x00000002    // Ch A transmit channel interrupt
#define   BCM63148_DMA_B_RX                        0x00000004    // Ch B receive channel interrupt
#define   BCM63148_DMA_B_TX                        0x00000008    // Ch B transmit channel interrupt
#define   BCM63148_DMA_TX_UNDERFLOW_A              0x00000010    // Ch A transmit channel underflow
#define   BCM63148_DMA_TX_UNDERFLOW_B              0x00000020    // Ch B transmit channel underflow
#define   BCM63148_DMA_RX_OVERFLOW_A               0x00000040    // Ch A receive channel overflow
#define   BCM63148_DMA_RX_OVERFLOW_B               0x00000080    // Ch B receive channel overflow
#define   BCM63148_DMA_PCM_RX                      0x00000100    // PCM Rx DMA IRQ
#define   BCM63148_DMA_PCM_TX                      0x00000200    // PCM Tx DMA IRQ
#define   BCM63148_DEV_BMU_IRQ                     0x00000400    // BMU block IRQ

  uint32        apm_dev_irq_mask;                       // (0x08)DMA interrupt mask register

// Note semantic change
// IUDMA refers to mem to periph as Tx
// and periph to mem as Rx
// APM core refers to path from mem as Rx
// and path to mem as Tx

  uint32        apm_dev_control;                        // (0x0c) Device control register
#define   BCM63148_RX_PACKET_SIZE_A                0x000000ff    // Number of samples to form quasi packet to mem channel A
#define   BCM63148_RX_PACKET_SIZE_B                0x0000ff00    // Number of samples to form quasi packet to mem channel B

#define   BCM63148_RX_DMA_ENABLE_A                 0x10000000    // Enable for Ch A DMA to mem
#define   BCM63148_RX_DMA_ENABLE_A_SHIFT           28

#define   BCM63148_RX_DMA_ENABLE_B                 0x20000000    // Enable for Ch B DMA to mem
#define   BCM63148_RX_DMA_ENABLE_B_SHIFT           29

#define   BCM63148_TX_DMA_ENABLE_A                 0x40000000    // Enable for Ch A DMA from mem
#define   BCM63148_TX_DMA_ENABLE_A_SHIFT           30

#define   BCM63148_TX_DMA_ENABLE_B                 0x80000000    // Enable for Ch B DMA from mem
#define   BCM63148_TX_DMA_ENABLE_B_SHIFT           31

// APM core registers
  uint32        reg_apm_coeff_wr_data;                  // (0x10) 20 bit 2's comp coefficient to be written into coeff RAM
#define   BCM63148_COEFF_PROG_INPUT                0x000FFFFF

  uint32        reg_apm_coeff_config;                   // (0x14)
#define   BCM63148_COEFF_PROG_ADDR                 0x000000FF    //8 bit address into coefficient RAM space                                                       
#define   BCM63148_COEFF_PROG_WE                   0x00000100    //1=write into memory 0= read from memory                                                        
#define   BCM63148_EQ_TX_ACCESS_COEFF_RAM_A        0x00010000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a equalizer        
#define   BCM63148_EQ_RX_ACCESS_COEFF_RAM_A        0x00020000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a equalizer        
#define   BCM63148_HISPD_HYBAL_ACCESS_COEFF_RAM_A  0x00040000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM63148_LOSPD_HYBAL_ACCESS_COEFF_RAM_A  0x00080000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM63148_YFLTR_ACCESS_COEFF_RAM_A        0x00100000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a hybrid balance   
#define   BCM63148_ASRC_INT_ACCESS_COEFF_RAM_A     0x00200000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b hybrid balance   
#define   BCM63148_ASRC_DEC_ACCESS_COEFF_RAM_A     0x00400000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a y filter         
#define   BCM63148_EQ_TX_ACCESS_COEFF_RAM_B        0x00800000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b y filter         
#define   BCM63148_EQ_RX_ACCESS_COEFF_RAM_B        0x01000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b y filter         
#define   BCM63148_HISPD_HYBAL_ACCESS_COEFF_RAM_B  0x02000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM63148_LOSPD_HYBAL_ACCESS_COEFF_RAM_B  0x04000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM63148_YFLTR_ACCESS_COEFF_RAM_B        0x08000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b asrc interpolator
#define   BCM63148_ASRC_INT_ACCESS_COEFF_RAM_B     0x10000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc decimator   
#define   BCM63148_ASRC_DEC_ACCESS_COEFF_RAM_B     0x20000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b asrc decimator   

#define   BCM63148_APM_HYBAL_NUM_TAPS            5
#define   BCM63148_APM_YFLTR_FIR_NUM_TAPS       18
#define   BCM63148_APM_YFLTR_IIR1_NUM_TAPS       1
#define   BCM63148_APM_YFLTR_IIR2_NUM_TAPS       5
#define   BCM63148_APM_YFLTR_BLOCK_SIZE         28
#define   BCM63148_APM_RX_EQ_NUM_TAPS           80
#define   BCM63148_APM_TX_EQ_NUM_TAPS           80 
#define   BCM63148_APM_ASRC_DEC_NUM_TAPS       120
#define   BCM63148_APM_ASRC_INT_NUM_TAPS       120

  uint32        reg_cha_test_config;                    // (0x18)
#define   BCM63148_RX_IN_MUX_CTRL                  0x00000003    //2 bits mux control  at eq input 0=test stimulus 1=rx_data_in 2=loopback from tx path channel a
#define   BCM63148_RX_IN_MUX_CTRL_SHIFT            0

#define   BCM63148_NETWORK_LPBK                    0x00000004    //1=take data inputs and loopback to data outputs (network side) channel a
#define   BCM63148_NETWORK_LPBK_SHIFT              2

#define   BCM63148_RX_98K_MUX_CTRL                 0x00000018    //1=bypass 0=use eq output on rx side channel a
#define   BCM63148_RX_98K_MUX_CTRL_SHIFT           3

#define   BCM63148_ADC_MUX_CTRL                    0x00000060    //2 bits mux at input to tx path 0=loopback 1=adc out 2=test stimulus channel a
#define   BCM63148_ADC_MUX_CTRL_SHIFT              5

#define   BCM63148_ADC_DAC_LPBK                    0x00000080    //1=take 4 bit ADC codes and send them to the DEM and DAC
#define   BCM63148_ADC_DAC_LPBK_SHIFT              7

#define   BCM63148_RX_TX_98K_LPBK                  0x00000100    //1=loopback from rx to tx path channel a
#define   BCM63148_RX_TX_98K_LPBK_SHIFT            8

#define   BCM63148_SLEW_INSTANT                    0x00000200    //1=slew scale, 0=make scale changes instantaneously
#define   BCM63148_SLEW_INSTANT_SHIFT              9

#define   BCM63148_ASRC_EN                         0x00000400    //1=asrc and eq are active 0=inactive
#define   BCM63148_ASRC_EN_SHIFT                   10


  uint32        reg_chb_test_config;                    // (0x1c)

  uint32        reg_cha_eq_config;                      // (0x20)
#define   BCM63148_EQ_RX_NUM_TAPS                  0x0000007f    //7 bit number of recieve taps
#define   BCM63148_EQ_RX_NUM_TAPS_SHIFT            0

#define   BCM63148_EQ_TX_NUM_TAPS                  0x00007f00    //7 bit number of transmit taps
#define   BCM63148_EQ_TX_NUM_TAPS_SHIFT            8

#define   BCM63148_EQ_TX_SHFT                      0x000f0000    //4 bits controls output shifter (powers of 2 gain control) for rx path channel a               
#define   BCM63148_EQ_TX_SHFT_SHIFT                16

#define   BCM63148_EQ_RX_SHFT                      0x00f00000    //4 bits controls output shifter (powers of 2 gain control) for tx path channel a
#define   BCM63148_EQ_RX_SHFT_SHIFT                20

#define   BCM63148_EQ_RX_IMP_RESP                  0x01000000    //1=use ext RAM for eq coefficients 0=use int ROM for eq coefficients channel a
#define   BCM63148_EQ_RX_IMP_RESP_SHIFT            24

#define   BCM63148_EQ_TX_IMP_RESP                  0x02000000    //1 the eq has an impulse shape response, 0= use the RAM or ROM for eq coefficients
#define   BCM63148_EQ_TX_IMP_RESP_SHIFT            25

#define   BCM63148_EQ_RX_ENABLE                    0x04000000    //equalizer transmit enable
#define   BCM63148_EQ_RX_ENABLE_SHIFT              26

#define   BCM63148_EQ_TX_ENABLE                    0x08000000    //equalizer receive enable
#define   BCM63148_EQ_TX_ENABLE_SHIFT              27

#define   BCM63148_SOFT_INIT                       0x80000000    //initializes state machines and registers
#define   BCM63148_SOFT_INIT_SHIFT                 31

  uint32        reg_cha_hybal_config;                   // (0x24)
#define   BCM63148_HISPD_HYBAL_SHFT                0x00000007    //
#define   BCM63148_HISPD_HYBAL_SHFT_SHIFT          0

#define   BCM63148_LOSPD_HYBAL_SHFT                0x00000038    //
#define   BCM63148_LOSPD_HYBAL_SHFT_SHIFT          3

#define   BCM63148_HISPD_HYBAL_EN                  0x00000080    //
#define   BCM63148_HISPD_HYBAL_EN_SHIFT            7

#define   BCM63148_LOSPD_HYBAL_EN                  0x00000100    //
#define   BCM63148_LOSPD_HYBAL_EN_SHIFT            8

#define   BCM63148_HISPD_HYBAL_SMPL_OFFSET         0x00007000    //
#define   BCM63148_HISPD_HYBAL_SMPL_OFFSET_SHIFT   12

#define   BCM63148_YFLTR_EN                        0x00008000    //1=y-filter is active 0=y-filter outputs 0 only
#define   BCM63148_YFLTR_EN_SHIFT                  15

#define   BCM63148_HPF_EN                          0x00010000    //1=high pass filter is active 0=high pass filter outputs 0 only
#define   BCM63148_HPF_EN_SHIFT                    16

#define   BCM63148_LOSPD_HYBAL_SMPL_OFFSET         0x3f000000    //
#define   BCM63148_LOSPD_HYBAL_SMPL_OFFSET_SHIFT   24

  uint32        reg_cha_int_config_1;                   // (0x28)
#define   BCM63148_ASRC_INT_NUM_SECT               0x001f0000    //5 bits--if symmetric it represents 1/2 of the number of sections (minus 1) in the filter channel a
#define   BCM63148_ASRC_INT_NUM_SECT_SHIFT         16            //  if asymmetric, it represents the number of sections (minus 1) channel a                       

#define   BCM63148_ASRC_INT_HRNR_SHFT              0x03000000    //2 bits defines shifting at output of asrc int hrnr channel a    
#define   BCM63148_ASRC_INT_HRNR_SHFT_SHIFT        24

#define   BCM63148_ASRC_INT_SYM_ASYM               0x04000000    //1=filter coefficients are symmetrical (only 1/2 are stored) channel a
#define   BCM63148_ASRC_INT_SYM_ASYM_SHIFT         26

#define   BCM63148_ASRC_INT_COEFF_SEL              0x18000000    //
#define   BCM63148_ASRC_INT_COEFF_SEL_SHIFT        27

  uint32        reg_cha_int_config_2;                   // (0x2c)
#define   BCM63148_ASRC_INT_SCALE                  0x0000ffff    //16 bit scale value channel a
#define   BCM63148_ASRC_INT_SCALE_SHIFT            0

#define   BCM63148_ASRC_INT_FIR_SHFT               0x00070000    //3 bits defines shifting at output of mac before writing into data RAM channel a
#define   BCM63148_ASRC_INT_FIR_SHFT_SHIFT         16

#define   BCM63148_ASRC_SLEW_SPEED                 0x00780000    //
#define   BCM63148_ASRC_SLEW_SPEED_SHIFT           19

  uint32        reg_cha_pm_config_1;                    // (0x30)
#define   BCM63148_PM_ON_TIME                      0x00000fff    //12 bit number in 2 msec units for on-time channel a
#define   BCM63148_PM_ON_TIME_SHIFT                0

#define   BCM63148_PM_OFF_TIME                     0x0fff0000    //12 bit number in 2 msec units for off-time channel a
#define   BCM63148_PM_OFF_TIME_SHIFT               16

#define   BCM63148_PM_FREQ_16_12                   0x10000000    //1=16 kHz frequency 0=12 kHz frequency channel a
#define   BCM63148_PM_FREQ_16_12_SHIFT             28

#define   BCM63148_PM_TIME_MS                      2 / 5


  uint32        reg_cha_pm_config_2;                    // (0x34)
#define   BCM63148_PM_SCALE                        0x0000ffff    //16 bit 2's complement number that scales the output from full scale channel a
#define   BCM63148_PM_SCALE_SHIFT                  0

#define   BCM63148_PM_RISE_INC                     0x00ff0000    //8 bit number goes into accumulator for rise time channel a
#define   BCM63148_PM_RISE_INC_SHIFT               16

#define   BCM63148_PM_FALL_INC                     0xff000000    //8 bit number goes into accumulator for fall time channel a
#define   BCM63148_PM_FALL_INC_SHIFT               24

  uint32        reg_cha_pm_config_3;                    // (0x38)
#define   BCM63148_PM_BURST_LENGTH                 0x0000ffff    //16 bit number defines the number of bursts to send out
#define   BCM63148_PM_BURST_LENGTH_SHIFT           0

#define   BCM63148_PM_BURST_CONT                   0x00010000    //1=send out infinite number of bursts 0=send out "pm_burst_length"
#define   BCM63148_PM_BURST_CONT_SHIFT             16

#define   BCM63148_PM_BURST_START                  0x00020000    //strobe the indicates that a burst should start (this will be edge detected)
#define   BCM63148_PM_BURST_START_SHIFT            17

#define   BCM63148_PM_ENABLE                       0x00040000    //1=pulse metering is active and on, 0=inactive and off channel a
#define   BCM63148_PM_ENABLE_SHIFT                 18

  uint32        reg_cha_cic_config;                     // (0x3c)
#define   BCM63148_CIC_DEC_SHFT                    0x00000007    //3 bits controls output shifter (powers of 2 gain control) channel a
#define   BCM63148_CIC_DEC_SHFT_SHIFT              0

#define   BCM63148_CIC_INC_SHFT                    0x00000038    //3 bits controls output shifter (powers of 2 gain control) channel a
#define   BCM63148_CIC_INC_SHFT_SHIFT              3

#define   BCM63148_CIC_INC_EQ_EN                   0x00000100    //1=enable equalizer
#define   BCM63148_CIC_INC_EQ_EN_SHIFT             8

#define   BCM63148_CIC_DEC_EQ_EN                   0x00000200    //1=enable equalizer
#define   BCM63148_CIC_DEC_EQ_EN_SHIFT             9

  uint32        reg_cha_asrc_dec_config;                // (0x40)
#define   BCM63148_ASRC_DEC_SCALE                  0x0000ffff    //16 bit 2's complement scale value channel a
#define   BCM63148_ASRC_DEC_SCALE_SHIFT            0

#define   BCM63148_ASRC_DEC_NUM_SECT               0x001f0000    //5 bit number 0-20 defines the number of sections in the filter channel a
#define   BCM63148_ASRC_DEC_NUM_SECT_SHIFT         16            //   if symmetric it equals the number of sections/2 -1                   
                                                        //   if asymmetric it equals the number of sections -1                    
#define   BCM63148_ASRC_DEC_SHFT                   0x0f000000    //4 bits defines msb location at input [38:31] channel a
#define   BCM63148_ASRC_DEC_SHFT_SHIFT             24

#define   BCM63148_ASRC_DEC_SYM_ASYM               0x10000000    //1=symmetric filter 0=asymmetric filter channel a
#define   BCM63148_ASRC_DEC_SYM_ASYM_SHIFT         28

#define   BCM63148_ASRC_DEC_COEFF_SEL              0x60000000    //ASRC DEC coefficient select
#define   BCM63148_ASRC_DEC_COEFF_SEL_SHIFT        29

  uint32        reg_cha_fcw_config_1;                   // (0x44)
#define   BCM63148_FCW_SCALE                       0xffffffff    //32 bit unsigned scale value for frequency control word channel a

  uint32        reg_cha_fcw_config_2;                   // (0x48)
#define   BCM63148_FCW_SHFT                        0x0000000f    //4 bit shift control for fcw channel a
#define   BCM63148_FCW_SHFT_SHIFT                  0

#define   BCM63148_CLOCK_SOURCE                    0x00000030    //2 bit mux control 0=NTP 1=DPLL 2=misc for channel a
#define   BCM63148_CLOCK_SOURCE_SHIFT              4

  
  uint32        reg_cha_digmod_config;                  // (0x4c)
#define   BCM63148_DIGMOD_DEM_TONE                 0x00000001    //1=suppress tones at fs/2 channel a
#define   BCM63148_DIGMOD_DEM_TONE_SHIFT           0

#define   BCM63148_DIGMOD_DEM_DIS                  0x00000002    //1=use balanced code as output instead of dynamic element matcher channel a
#define   BCM63148_DIGMOD_DEM_DIS_SHIFT            1

#define   BCM63148_DIGMOD_DITH_SCALE               0x0000ff00    // new 10/1
#define   BCM63148_DIGMOD_DITH_SCALE_SHIFT         8


// CH B use same bit position defines as Ch A
  uint32        reg_chb_eq_config;                      // (0x50)
  uint32        reg_chb_hybal_config;                   // (0x54)
  uint32        reg_chb_int_config_1;                   // (0x58)
  uint32        reg_chb_int_config_2;                   // (0x5c)

  uint32        reg_chb_pm_config_1;                    // (0x60)
  uint32        reg_chb_pm_config_2;                    // (0x64)
  uint32        reg_chb_pm_config_3;                    // (0x68)
  uint32        reg_chb_cic_config;                     // (0x6c)
  uint32        reg_chb_asrc_dec_config;                // (0x70)
  uint32        reg_chb_fcw_config_1;                   // (0x74)
  uint32        reg_chb_fcw_config_2;                   // (0x78)
  uint32        reg_chb_digmod_config;                  // (0x7c)

  uint32        reg_fcw_config_1;                       // (0x80)
#define   BCM63148_FCW_REG                         0xffffffff    //32 bit frequency control word from NTP block

  uint32        reg_fcw_config_2;                       // (0x84)
#define   BCM63148_LOAD_NTP                        0x00000001    // triggers pulse to load FCW to NTP
#define   BCM63148_LOAD_NTP_SHIFT                  0

#define   BCM63148_LOAD_MISC_A                     0x00000002    // triggers pulse to load FCW to ch A
#define   BCM63148_LOAD_MISC_A_SHIFT               1

#define   BCM63148_LOAD_MISC_B                     0x00000004    // triggers pulse to load FCW to ch B
#define   BCM63148_LOAD_MISC_B_SHIFT               2

#define   BCM63148_LOAD_FCW_NOM_A                  0x00000008    //strobe to load nominal FCW value
#define   BCM63148_LOAD_FCW_NOM_A_SHIFT            3

#define   BCM63148_LOAD_FCW_NOM_B                  0x00000010    //strobe to load nominal FCW value
#define   BCM63148_LOAD_FCW_NOM_B_SHIFT            4

#define   BCM63148_FCW_LIMIT                       0x00000f00    //4 bits defines allowable error on FCW relative to nominal
#define   BCM63148_FCW_LIMIT_SHIFT                 8

  uint32        reg_ntp_config_1;                       // (0x88)
#define   BCM63148_NTP_TIME_DVD                    0x0000ffff    //16 bit divide value for time counter (16'd374 is default)

  uint32        reg_ntp_config_2;                       // (0x8c)
#define   BCM63148_NTP_READ_TIME                   0x00000001    //strobe asks for new time count value

  uint32        reg_ring_config_1;                      // (0x90)
#define   BCM63148_RING_START_IMMED_A              0x00000001    //1=start cadence
#define   BCM63148_RING_START_IMMED_A_SHIFT        0

#define   BCM63148_RING_START_IMMED_B              0x00000002    //1=start cadence
#define   BCM63148_RING_START_IMMED_B_SHIFT        1

#define   BCM63148_RING_START_NEXT_PHASE_A         0x00000004    //1=start cadence
#define   BCM63148_RING_START_NEXT_PHASE_A_SHIFT   2

#define   BCM63148_RING_START_NEXT_PHASE_B         0x00000008    //1=start cadence
#define   BCM63148_RING_START_NEXT_PHASE_B_SHIFT   3

#define   BCM63148_RING_STOP_IMMED_A               0x00000010    // stop ring now!
#define   BCM63148_RING_STOP_IMMED_A_SHIFT         4

#define   BCM63148_RING_STOP_IMMED_B               0x00000020    // stop ring now!
#define   BCM63148_RING_STOP_IMMED_B_SHIFT         5

#define   BCM63148_RING_STOP_NEXT_PHASE_A          0x00000040    // stop ring at next phase
#define   BCM63148_RING_STOP_NEXT_PHASE_A_SHIFT    6

#define   BCM63148_RING_STOP_NEXT_PHASE_B          0x00000080    // stop ring at next phase
#define   BCM63148_RING_STOP_NEXT_PHASE_B_SHIFT    7

#define   BCM63148_RING_OMEGA_NORM                 0x000fff00    //the omega man is Charlton Heston
#define   BCM63148_RING_OMEGA_NORM_SHIFT           8

#define   BCM63148_RING_DELAY                      0x01f00000    //ring start delay
#define   BCM63148_RING_DELAY_SHIFT                20

#define   BCM63148_RING_GEN_SOFT_INIT              0x80000000    //soft init bit
#define   BCM63148_RING_GEN_SOFT_INIT_SHIFT        31

  uint32        reg_ring_config_2;                      // (0x94)
  uint32        reg_ring_config_3;                      // (0x98)
#define   BCM63148_RING_OFFSET                     0xffff0000    //ring offset
#define   BCM63148_RING_OFFSET_SHIFT               16

#define   BCM63148_RING_SCALE                      0x0000ffff    //ring scale
#define   BCM63148_RING_SCALE_SHIFT                0

  uint32        reg_ring_config_4;                      // (0x9c)
#define   BCM63148_RING_CREST_FACTOR_A             0x00000007    //3 bit crest factor
#define   BCM63148_RING_CREST_FACTOR_A_SHIFT       0

#define   BCM63148_RING_GEN_ENABLE_A               0x00000008    //1=enabled
#define   BCM63148_RING_GEN_ENABLE_A_SHIFT         3

#define   BCM63148_RING_GEN_PHASE_A                0x00003ff0    //start/stop phase
#define   BCM63148_RING_GEN_PHASE_A_SHIFT          4

#define   BCM63148_RING_CREST_FACTOR_B             0x00070000    //3 bit crest factor
#define   BCM63148_RING_CREST_FACTOR_B_SHIFT       16

#define   BCM63148_RING_GEN_ENABLE_B               0x00080000    //1=enabled
#define   BCM63148_RING_GEN_ENABLE_B_SHIFT         19

#define   BCM63148_RING_GEN_PHASE_B                0x3ff00000    //start/stop phase
#define   BCM63148_RING_GEN_PHASE_B_SHIFT          20

  uint32        reg_spare_unused;                       // (0xa0)
//#define   BCM63148_SER_TST_SCALE                   0xffffffff

  uint32        reg_ser_config;                         // (0xa4)
#define   BCM63148_SER_MUX_SEL                     0x00000003
#define   BCM63148_SER_MUX_SEL_SHIFT               0

//#define   BCM63148_SER_TST_SHFT_CTRL               0x07800000    //
#define   BCM63148_AUD_SER_TST_FCW_MUX_CTRL        0x08000000    //
#define   BCM63148_AUD_SER_TST_FCW_MUX_CTRL_SHIFT  27

//#define   BCM63148_SER_TST_LOAD_REGFILE            0x20000000    //1->use channel b requests for transfer 0-> use channel a
#define   BCM63148_SER_TST_CLKS                    0x40000000    //1=use clocks supplied by SER 0=generate internal clocks
#define   BCM63148_SER_TST_CLKS_SHIFT              30

#define   BCM63148_SOFT_INIT_AP                    0x80000000
#define   BCM63148_SOFT_INIT_AP_SHIFT              31

  uint32        reg_stats_config;                       // (0xa8)
#define   BCM63148_START_STATS                     0x00008000    //[15]
#define   BCM63148_STATS_MUX_CTL_VADCA             0x00000800    //[11]
#define   BCM63148_STATS_MUX_CTL_VADCB             0x00000400    //[10]
#define   BCM63148_STATS_CHOP_ENABLE_A             0x00000200    //[9]
#define   BCM63148_STATS_CHOP_ENABLE_B             0x00000100    //[8]
#define   BCM63148_STATS_DURATION                  0x000000f0    //[7:4]
#define   BCM63148_STATS_CHOP_PERIOD               0x0000000f    //[3:0]

  // APM Status Registers
  uint32        reg_apm_status_1;                       // (0xac)
#define   BCM63148_DIGMOD_SAT_QUANT                0x01000000    //strobe indicating quantizer has saturated
#define   BCM63148_DIGMOD_SAT_COUNT_0              0x00ff0000    //8 bit counter of saturation on integrator 0 channel a
#define   BCM63148_DIGMOD_SAT_COUNT_1              0x0000ff00    //8 bit counter of saturation on integrator 1 channel a
#define   BCM63148_DIGMOD_SAT_COUNT_QUANT          0x000000ff    //8 bit counter of saturation on quantizer channel a

  uint32        reg_apm_status_2;                       // (0xb0) same as status 1 for ch B

  uint32        reg_apm_status_3;                       // (0xb4)
#define   BCM63148_AP_XMT_UFLOW                    0x00200000    //level indicating xnt fifo is empty when read attempted
#define   BCM63148_AP_XMT_OVLOW                    0x00100000    //level indicating xmt fifo is full when write attempted
#define   BCM63148_AP_NCO_SATURATED                0x00080000    // NCO is saturated
#define   BCM63148_AP_XMT_FIFO_DEPTH               0x00070000    //3 bit depth number on xmt fifo
#define   BCM63148_AP_RCV_UFLOW                    0x00000020    //level indicating rcv fifo is empty when read attempted
#define   BCM63148_AP_RCV_OVLOW                    0x00000010    //level indicating rcv fifo is full when write attempted
#define   BCM63148_AP_RCV_FIFO_DEPTH               0x00000007    //3 bit depth number on receive fifo

  uint32        reg_apm_status_4;                       // (0xb8)
#define   BCM63148_EQ_RX_COEFF                     0xffff0000    //16 bit 2's complement coefficients from RAM
#define   BCM63148_EQ_TX_COEFF                     0x0000ffff    //16 bit coefficient read from RAM or ROM

  uint32        reg_apm_status_5;                       // (0xbc)
#define   BCM63148_ASRC_INT_COEFF                  0xffff0000    //16 bit 2's comp current coefficient value
#define   BCM63148_ASRC_DEC_COEFF                  0x0000ffff    //16 bit coefficient from RAM or ROM

  uint32        reg_apm_status_6;                       // (0xc0)
#define   BCM63148_YFLTR_COEFF                     0x000fffff    //20 bit 2's complement coefficients from RAM

// Status 9-7 same as 6-4 for Ch B
  uint32        reg_apm_status_7;                       // (0xc4)
  uint32        reg_apm_status_8;                       // (0xc8)
  uint32        reg_apm_status_9;                       // (0xcc)

  uint32        reg_apm_status_10;                      // (0xd0)
#define   BCM63148_NTP_TIME_COUNT_INTEGER          0x0001ffff    //17 integer bits (in seconds)

  uint32        reg_apm_status_11;                      // (0xd4)
#define   BCM63148_NTP_TIME_COUNT_FRACT            0x0003ffff    //18 fractional bits (in seconds)

  uint32        reg_apm_status_12;                      // (0xd8)
#define   BCM63148_RING_CADENCE_STOPPED_A          0x00000008    //
#define   BCM63148_RING_ON_OFF_A                   0x00000004    //
#define   BCM63148_RING_CADENCE_STOPPED_B          0x00000002    //
#define   BCM63148_RING_ON_OFF_B                   0x00000001    //

  uint32        reg_apm_status_13;                      // (0xdc)
#define   BCM63148_HISPD_HYBAL_COEFF_A             0xffff0000    //
#define   BCM63148_HISPD_HYBAL_COEFF_B             0x0000ffff    //

  uint32        reg_apm_status_14;                      // (0xe0)
#define   BCM63148_LOSPD_HYBAL_COEFF_A             0xffff0000    //
#define   BCM63148_LOSPD_HYBAL_COEFF_B             0x0000ffff    //

  int32        reg_apm_stats_1;                        // (0xe4)
#define   BCM63148_MEAN                            0xffffffff    // 16 bit mean stats

  int32        reg_apm_stats_2;                        // (0xe8), same as reg_apm_stats_1 for channel B
  uint32        reg_apm_stats_3;                        // (0xec)
#define   BCM63148_MEAN_SQUARE_LO                  0xffffffff    // lower 32 bits of 40 bit mean square value

  uint32        reg_apm_stats_4;                        // (0xf0)
#define   BCM63148_MEAN_SAT                        0x00040000
#define   BCM63148_MEAN_SQUARE_SAT                 0x00020000
#define   BCM63148_STATS_BUSY                      0x00010000    // 1 bit stats busy
#define   BCM63148_MEAN_SQUARE_HI                  0x00003fff    // upper 14 bits of 46 bit mean square value

  uint32        reg_apm_stats_5;                        // (0xf4), same as reg_apm_stats_3 for channel B
  uint32        reg_apm_stats_6;                        // (0xf8), same as reg_apm_stats_4 for channel B

  uint32        reg_apm_audio_fcw_a;                    // (0xfc)
  uint32        reg_apm_audio_fcw_b;                    // (0x100)

  uint32        reg_apm_irq_pend_1;                     // (0x104) Ch A status
#define   BCM63148_PM_BURST_DONE                   0x80000000    //strobe indicating that PM burst has completed
#define   BCM63148_ASRC_INT_DONE_SLEWING           0x40000000    //strobe indicating that slewing has completed
#define   BCM63148_ASRC_DEC_DONE_SLEWING           0x20000000    //strobe indicating that slewing has completed
#define   BCM63148_RX_PATH_SAT                     0x10000000    //strobed high when audio + pulse meter + y-filter exceeds 16 bit number
#define   BCM63148_EQ_TX_SAT                       0x08000000    //strobe indicates that the result on the eq tx path was saturated
#define   BCM63148_EQ_RX_SAT                       0x04000000    //strobe indicates that the result on the rx path was saturated at output
#define   BCM63148_EQ_TX_RQST_ERR                  0x02000000    //strobe indicating a tx_rqst was asked for while a tx was pending
#define   BCM63148_EQ_RX_RQST_ERR                  0x01000000    //strobe indicating a rx_rqst was asked for while a rx was pending
#define   BCM63148_LOSPD_HYBAL_SAT                 0x00800000    //strobe indicates that the result on the tx path was saturated at output
#define   BCM63148_HISPD_HYBAL_SAT                 0x00400000    //strobe indicates that the result on the tx path was saturated at output
#define   BCM63148_HISPD_HYBAL_RQST_ERR            0x00200000    //strobe indicating a output_rqst was asked for while computation engine was busy
#define   BCM63148_YFLTR_IIR1_SAT                  0x00100000    //strobe indicating iir-1 is saturating
#define   BCM63148_YFLTR_FIR_SAT                   0x00080000    //strobe indicating fir is saturating
#define   BCM63148_YFLTR_IIR2_SAT                  0x00040000    //strobe indicating iir-2 is saturating
#define   BCM63148_YFLTR_IIR1_ACCUM_SAT            0x00020000    //strobe indicating iir1 accumulator saturated
#define   BCM63148_YFLTR_FIR_ACCUM_SAT             0x00010000    //strobe indicating fir accumulator saturated
#define   BCM63148_YFLTR_IIR2_ACCUM_SAT            0x00008000    //strobe indicating iir2 accumulator saturated
#define   BCM63148_YFLTR_RQST_ERR                  0x00004000    //strobe indicating a output_rqst was asked for while computation engine was busy
#define   BCM63148_NCO_SAT                         0x00002000    //strobe indicates scaling and shifting saturated the result
#define   BCM63148_ASRC_INT_HRNR_TIM_ERR           0x00001000    //strobe indicating delta has rolled over too quickly before fir has completed computation
#define   BCM63148_ASRC_INT_HRNR_SAT               0x00000800    //strobe indicating data has saturated at the mac output
#define   BCM63148_ASRC_INT_FIR_TIM_ERR            0x00000400    //strobe indicating delta has rolled over too quickly before fir has completed computation
#define   BCM63148_ASRC_INT_FIR_BANK_ERR           0x00000200    //strobe indicating that horner ngn asked for a bank switch while fir was busy
#define   BCM63148_ASRC_INT_FIR_SAT                0x00000100    //strobe indicating data has saturated at the mac output
#define   BCM63148_ASRC_INT_HRNR_MAC_SAT           0x00000080    //strobe indicating ser mac in hrnr block has saturated
#define   BCM63148_CIC_INT_SAT                     0x00000040    //strobe indicates that the result on the tx path was saturated at output
#define   BCM63148_DIGMOD_SAT_0                    0x00000020    //strobe indicating the integrator 0 has saturated
#define   BCM63148_DIGMOD_SAT_1                    0x00000010    //strobe indicating integrator 1 has saturated
#define   BCM63148_TX_PATH_SAT                     0x00000008
#define   BCM63148_CIC_DEC_SAT                     0x00000004    //strobe indicates that the result on the tx path was saturated at output
#define   BCM63148_ASRC_DEC_TIM_ERR                0x00000002    //strobe asserted when "input_data_ready" is asserted while ngn is busy
#define   BCM63148_ASRC_DEC_SAT                    0x00000001    //strobe indicating that the input was saturated
        
  uint32        reg_apm_irq_mask_1;                     // (0x108)

  uint32        reg_apm_irq_pend_2;                     // (0x10c) same as APM_IRQ_1 for Ch B
  uint32        reg_apm_irq_mask_2;                     // (0x110)

  uint32        reg_apm_irq_pend_3;                     // (0x114)
#define   BCM63148_STATS_DONE_A                    0x00000400    //strobe indicating statistics gathered and computed chA
#define   BCM63148_STATS_DONE_B                    0x00000200    //strobe indicating statistics gathered and computed chB
#define   BCM63148_FCW_OOB_A                       0x00000100    //level indicating that FCW error is too large
#define   BCM63148_FCW_OOB_B                       0x00000080    //level indicating that FCW error is too large
#define   BCM63148_HPF_SAT_A                       0x00000040    //strobe indicating high pass filter has saturated, Channel A
#define   BCM63148_HPF_SAT_B                       0x00000020    //strobe indicating high pass filter has saturated, Channel B
#define   BCM63148_ASRC_INT_DELTA_ADJ_DONE_A       0x00000010    //strobe indicating TODO
#define   BCM63148_ASRC_INT_DELTA_ADJ_DONE_B       0x00000008    //strobe indicating TODO
#define   BCM63148_LOSPD_HYBAL_RQST_ERR_A          0x00000004    //strobe indicating a output_rqst was asked for while computation engine was busy, channel A
#define   BCM63148_LOSPD_HYBAL_RQST_ERR_B          0x00000002    //strobe indicating a output_rqst was asked for while computation engine was busy, channel B
#define   BCM63148_NTP_TIME_VALID                  0x00000001    //strobe in clk_200 domain (4 clock periods)

  uint32        reg_apm_irq_mask_3;                     // (0x118)
  uint32        reg_spare_2;                            // (0x11c)

  uint32        reg_codec_config_0;                     // (0x120)
  uint32        reg_codec_config_1;                     // (0x124) VTX1
  uint32        reg_codec_config_2;                     // (0x128) same as config 16 for VTX2

  uint32        reg_codec_config_3;                     // (0x12c) VRX1
#define   BCM63148_PGA_CTL_VADC                    0x30000000
#define   BCM63148_PGA_CTL_VADC_SHIFT                      28

  uint32        reg_codec_config_4;                     // (0x130) same as config 18 for VRX2
#define   BCM63148_VREF_CTL_VDAC                   0x70000000 
#define   BCM63148_PWRDN_REF_VDAC                  0x01000000 
#define   BCM63148_PWRDN_REF_VDAC_A                0x00020000 
#define   BCM63148_TESTEN_VDAC_A                   0x00010000 
#define   BCM63148_DRVVOCMCTL_VDAC                 0x00007000 
#define   BCM63148_PGA_GAIN_CTL_VDAC_A             0x00000180 
#define   BCM63148_PGA_GAIN_CTL_VDAC_A_SHIFT                7
#define   BCM63148_PWRDN_REF_VDAC_B                0x00000040 
#define   BCM63148_TESTEN_VDAC_B                   0x00000020 
#define   BCM63148_PGA_GAIN_CTL_VDAC_B             0x00000018 
#define   BCM63148_PGA_GAIN_CTL_VDAC_B_SHIFT                3

  uint32        reserved;                               // (0x134) 
  uint32        reg_otp_0;                              // (0x138) OTP read back, otp_apm[31:0]
#define   BCM63148_APM_OTP_TRIM                    0x00000FC0
#define   BCM63148_APM_OTP_TRIM_SHIFT                       6
  uint32        reg_otp_1;                              // (0x13c) OTP read back, otp_apm[63:32]
  uint32        reg_diag_readback;                      // (0x140) diag bus read back
  uint32        dpll_control;                           // (0x144) DPLL control register
#define   BCM63148_DPLL_SOFT_INIT                  0x80000000
#define   BCM63148_DPLL_FORCE_ACQ                  0x40000000
#define   BCM63148_DPLL_OPEN_LOOP                  0x20000000
#define   BCM63148_DPLL_CONST_K                    0x10000000
#define   BCM63148_DPLL_PHS_THSHLD_SHIFT           4
  uint32        dpll_nom_freq;                          // (0x148) DPLL nominal frequency (control)
  uint32        dpll_div;                               // (0x14c) DPLL divide register
#define   BCM63148_DPLL_REF_DIV_SHIFT              16
  uint32        dpll_acq_freq;                          // (0x150) DPLL acquired frequency
  uint32        dpll_status;                            // (0x154) DPLL status register
#define   BCM63148_DPLL_IN_SYNC                    0x80000000
#define   BCM63148_DPLL_ACQ_FREQ_VALID             0x40000000
#define   BCM63148_DPLL_IN_SYNC                    0x80000000
#define   BCM63148_DPLL_K0_SHIFT                   8
} BCM63148ApmControlRegisters;

#define BCM63148_APM ((volatile BCM63148ApmControlRegisters * const) BCM63148_APM_BASE)

typedef struct BCM63148PcmControlRegisters
{
    uint32 pcm_ctrl;                            // 00 offset from PCM_BASE
#define  BCM63148_PCM_ENABLE              0x80000000     // PCM block master enable
#define  BCM63148_PCM_ENABLE_SHIFT        31
#define  BCM63148_PCM_SLAVE_SEL           0x40000000     // PCM TDM slave mode select (1 - TDM slave, 0 - TDM master)
#define  BCM63148_PCM_SLAVE_SEL_SHIFT     30
#define  BCM63148_PCM_CLOCK_INV           0x20000000     // PCM SCLK invert select (1 - invert, 0 - normal)
#define  BCM63148_PCM_CLOCK_INV_SHIFT     29
#define  BCM63148_PCM_FS_INVERT           0x10000000     // PCM FS invert select (1 - invert, 0 - normal)
#define  BCM63148_PCM_FS_INVERT_SHIFT     28
#define  BCM63148_PCM_FS_FREQ_16_8        0x08000000     // PCM FS 16/8 Khz select (1 - 16Khz, 0 - 8Khz)
#define  BCM63148_PCM_FS_FREQ_16_8_SHIFT  27
#define  BCM63148_PCM_FS_LONG             0x04000000     // PCM FS long/short select (1 - long FS, 0 - short FS)
#define  BCM63148_PCM_FS_LONG_SHIFT       26
#define  BCM63148_PCM_FS_TRIG             0x02000000     // PCM FS trigger (1 - falling edge, 0 - rising edge trigger)
#define  BCM63148_PCM_FS_TRIG_SHIFT       25
#define  BCM63148_PCM_DATA_OFF            0x01000000     // PCM data offset from FS (1 - one clock from FS, 0 - no offset)
#define  BCM63148_PCM_DATA_OFF_SHIFT      24
#define  BCM63148_PCM_DATA_16_8           0x00800000     // PCM data word length (1 - 16 bits, 0 - 8 bits)
#define  BCM63148_PCM_DATA_16_8_SHIFT     23
#define  BCM63148_PCM_CLOCK_SEL           0x00700000     // PCM SCLK freq select
#define  BCM63148_PCM_CLOCK_SEL_SHIFT     20
                                                  // 000 - 8192 Khz
                                                  // 001 - 4096 Khz
                                                  // 010 - 2048 Khz
                                                  // 011 - 1024 Khz
                                                  // 100 - 512 Khz
                                                  // 101 - 256 Khz
                                                  // 110 - 128 Khz
                                                  // 111 - reserved
#define  BCM63148_PCM_LSB_FIRST           0x00040000     // PCM shift direction (1 - LSBit first, 0 - MSBit first)
#define  BCM63148_PCM_LSB_FIRST_SHIFT     18
#define  BCM63148_PCM_LOOPBACK            0x00020000     // PCM diagnostic loobback enable
#define  BCM63148_PCM_LOOPBACK_SHIFT      17
#define  BCM63148_PCM_EXTCLK_SEL          0x00010000     // PCM external timing clock select -- Maybe removed in 63148
#define  BCM63148_PCM_EXTCLK_SEL_SHIFT    16
#define  BCM63148_PCM_NTR_ENABLE          0x00008000     // PCM NTR counter enable -- Nayve removed in 63148
#define  BCM63148_PCM_NTR_ENABLE_SHIFT    15
#define  BCM63148_PCM_BITS_PER_FRAME_1024 0x00000400     // 1024 - Max
#define  BCM63148_PCM_BITS_PER_FRAME_256  0x00000100     // 256
#define  BCM63148_PCM_BITS_PER_FRAME_8    0x00000008     // 8    - Min
#define  BCM63148_PCM_AP_SEL              0x00000001     // 1 - Connect pcm to pads, 0 - Connects audio test interface

    uint32 pcm_chan_ctrl;                       // 04
#define  BCM63148_PCM_TX0_EN              0x00000001     // PCM transmit channel 0 enable
#define  BCM63148_PCM_TX1_EN              0x00000002     // PCM transmit channel 1 enable
#define  BCM63148_PCM_TX2_EN              0x00000004     // PCM transmit channel 2 enable
#define  BCM63148_PCM_TX3_EN              0x00000008     // PCM transmit channel 3 enable
#define  BCM63148_PCM_TX4_EN              0x00000010     // PCM transmit channel 4 enable
#define  BCM63148_PCM_TX5_EN              0x00000020     // PCM transmit channel 5 enable
#define  BCM63148_PCM_TX6_EN              0x00000040     // PCM transmit channel 6 enable
#define  BCM63148_PCM_TX7_EN              0x00000080     // PCM transmit channel 7 enable
#define  BCM63148_PCM_RX0_EN              0x00000100     // PCM receive channel 0 enable
#define  BCM63148_PCM_RX1_EN              0x00000200     // PCM receive channel 1 enable
#define  BCM63148_PCM_RX2_EN              0x00000400     // PCM receive channel 2 enable
#define  BCM63148_PCM_RX3_EN              0x00000800     // PCM receive channel 3 enable
#define  BCM63148_PCM_RX4_EN              0x00001000     // PCM receive channel 4 enable
#define  BCM63148_PCM_RX5_EN              0x00002000     // PCM receive channel 5 enable
#define  BCM63148_PCM_RX6_EN              0x00004000     // PCM receive channel 6 enable
#define  BCM63148_PCM_RX7_EN              0x00008000     // PCM receive channel 7 enable
#define  BCM63148_PCM_RX_PACKET_SIZE      0x00ff0000     // PCM Rx DMA quasi-packet size
#define  BCM63148_PCM_RX_PACKET_SIZE_SHIFT  16

    uint32 pcm_int_pending;                     // 08
    uint32 pcm_int_mask;                        // 0c
#define  BCM63148_PCM_TX_UNDERFLOW        0x00000001     // PCM DMA receive overflow
#define  BCM63148_PCM_RX_OVERFLOW         0x00000002     // PCM DMA transmit underflow
#define  BCM63148_PCM_TDM_FRAME           0x00000004     // PCM frame boundary
#define  BCM63148_PCM_RX_IRQ              0x00000008     // IUDMA interrupts
#define  BCM63148_PCM_TX_IRQ              0x00000010

    uint32        reg_pcm_clk_cntl_0;             // (0x210) PCM Clock Control 0 (NCO_FCW_MISC)
    uint32        reg_pcm_clk_cntl_1;             // (0x214) PCM Clock Control 1 (NCO_SCALE)
    uint32        reg_pcm_clk_cntl_2;             // (0x218) PCM Clock Control 2
#define   BCM63148_PCM_NCO_SHIFT          0x0000000f
#define   BCM63148_PCM_NCO_MUX_CNTL       0x00000030
#define   BCM63148_PCM_NCO_LOAD_MISC      0x00000040
#define   BCM63148_PCM_NCO_SOFT_INIT      0x00000080

    uint32        pcm_apm_fcw_readback;            // FCW after scaling
    uint32        pcm_zds_intf;                    // ZSI Config register ( New in 63148 )
    uint32        pcm_msif_intf;                   // ISI Config register ( New in 63148 )
    uint32        pcm_ntr_div_clk_ctrl;            // NTR feedback clock divider ( New in 63148 )
    
    uint32 unused[5];
#define  PCM_MAX_TIMESLOT_REGS   16             // Number of PCM time slot registers in the table.
                                                // Each register provides the settings for 8 timeslots (4 bits per timeslot)
    uint32 pcm_slot_alloc_tbl[PCM_MAX_TIMESLOT_REGS];
#define  PCM_TS_VALID            0x8            // valid marker for TS alloc ram entry
} BCM63148PcmControlRegisters;

#define BCM63148_PCM ((volatile BCM63148PcmControlRegisters * const) BCM63148_PCM_BASE)

typedef struct BCM63148ApmPcmIudmaRegisters
{
   uint32 ctrlConfig;
#define IUDMA_REGS_CTRLCONFIG_MASTER_EN        0x0001
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH1_EN     0x0002
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH3_EN     0x0004
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH5_EN     0x0008
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH7_EN     0x0010

	// Flow control Ch1
   uint32 ch1_FC_Low_Thr;
   uint32 ch1_FC_High_Thr;
   uint32 ch1_Buff_Alloc;

   // Flow control Ch3
   uint32 ch3_FC_Low_Thr;
   uint32 ch3_FC_High_Thr;
   uint32 ch3_Buff_Alloc;

   // Flow control Ch5
   uint32 ch5_FC_Low_Thr;
   uint32 ch5_FC_High_Thr;
   uint32 ch5_Buff_Alloc;

   // Flow control Ch7
   uint32 ch7_FC_Low_Thr;
   uint32 ch7_FC_High_Thr;
   uint32 ch7_Buff_Alloc;

   uint32 channelReset;
   uint32 channelDebug;

   uint32 reserved;
   uint32 gbl_int_stat;

   #define APM_IUDMA_IRQ_TX_PCM      0x00000020
   #define APM_IUDMA_IRQ_RX_PCM      0x00000010
   #define APM_IUDMA_IRQ_TX_B        0x00000008
   #define APM_IUDMA_IRQ_RX_B        0x00000004
   #define APM_IUDMA_IRQ_TX_A        0x00000002
   #define APM_IUDMA_IRQ_RX_A        0x00000001

   uint32 gbl_int_mask;
} BCM63148ApmPcmIudmaRegisters;

typedef struct BCM63148ApmPcmIudmaChannelCtrl
{
    uint32 config;
#define BCM63148_IUDMA_CONFIG_ENDMA       0x00000001
#define BCM63148_IUDMA_CONFIG_PKTHALT     0x00000002
#define BCM63148_IUDMA_CONFIG_BURSTHALT   0x00000004

    uint32 intStat;
#define BCM63148_IUDMA_INTSTAT_BDONE   0x00000001
#define BCM63148_IUDMA_INTSTAT_PDONE   0x00000002
#define BCM63148_IUDMA_INTSTAT_NOTVLD  0x00000004
#define BCM63148_IUDMA_INTSTAT_MASK    0x00000007
#define BCM63148_IUDMA_INTSTAT_ALL     BCM63148_IUDMA_INTSTAT_MASK

    uint32 intMask;
#define BCM63148_IUDMA_INTMASK_BDONE   0x00000001
#define BCM63148_IUDMA_INTMASK_PDONE   0x00000002
#define BCM63148_IUDMA_INTMASK_NOTVLD  0x00000004

    uint32 maxBurst;
#define BCM63148_IUDMA_MAXBURST_SIZE 16 /* 32-bit words */

} BCM63148ApmPcmIudmaChannelCtrl;


typedef struct BCM63148ApmPcmIudmaStateRam
{
   uint32 baseDescPointer;                /* pointer to first buffer descriptor */

   uint32 stateBytesDoneRingOffset;       /* state info: how manu bytes done and the offset of the
                                             current descritor in process */
#define BCM63148_IUDMA_STRAM_DESC_RING_OFFSET 0x3fff


   uint32 flagsLengthStatus;              /* Length and status field of the current descriptor */

   uint32 currentBufferPointer;           /* pointer to the current descriptor */

} BCM63148ApmPcmIudmaStateRam;

#define BCM63148_MAX_APM_PCM_DMA_CHANNELS 6

typedef struct BCM63148ApmPcmIudma
{
   BCM63148ApmPcmIudmaRegisters regs;                                        //
   uint32 reserved1[110];                                             //
   BCM63148ApmPcmIudmaChannelCtrl ctrl[BCM63148_MAX_APM_PCM_DMA_CHANNELS];    //
   uint32 reserved2[104];                                             //
   BCM63148ApmPcmIudmaStateRam stram[BCM63148_MAX_APM_PCM_DMA_CHANNELS];      //

} BCM63148ApmPcmIudma;

#define BCM63148_APM_PCM_IUDMA ((volatile BCM63148ApmPcmIudma * const) BCM63148_APM_PCM_DMA_BASE)

typedef struct BCM63148DECTShimDmaControl 
{
   uint32 dect_shm_dma_ctrl;                 /*  0xb000b050  DECT DMA control register                      */
#define  BCM63148_DMA_CLEAR                           0x80000000
#define  BCM63148_DMA_SWAP_16_BIT                     0x03000000
#define  BCM63148_DMA_SWAP_8_BIT                      0x02000000
#define  BCM63148_DMA_SWAP_NONE                       0x01000000
#define  BCM63148_DMA_SUBWORD_SWAP_MASK               0x03000000
#define  BCM63148_TRIG_CNT_CLK_SEL_PCM                0x00800000
#define  BCM63148_TRIG_CNT_IRQ_EN                     0x00400000
#define  BCM63148_RX_CNT_TRIG_EN                      0x00200000   
#define  BCM63148_TX_CNT_TRIG_EN                      0x00100000 
#define  BCM63148_RX_INT_TRIG_EN                      0x00080000 	
#define  BCM63148_TX_INT_TRIG_EN                      0x00040000 	
#define  BCM63148_RX_REG_TRIG_EN                      0x00020000 	
#define  BCM63148_TX_REG_TRIG_EN                      0x00010000 
#define  BCM63148_RX_TRIG_FIRST                       0x00008000
#define  BCM63148_MAX_BURST_CYCLE_MASK                0x00001F00
#define  BCM63148_MAX_BURST_CYCLE_SHIFT               8
#define  BCM63148_RX_EN_3                             0x00000080   
#define  BCM63148_RX_EN_2                             0x00000040   
#define  BCM63148_RX_EN_1                             0x00000020   
#define  BCM63148_RX_EN_0                             0x00000010   
#define  BCM63148_TX_EN_3                             0x00000008   
#define  BCM63148_TX_EN_2                             0x00000004   
#define  BCM63148_TX_EN_1                             0x00000002   
#define  BCM63148_TX_EN_0                             0x00000001   
 
   uint32 dect_shm_dma_trig_cnt_preset;      /*  0xb000b054  DECT DMA trigger counter preset value                */
   uint32 dect_shm_dma_ddr_saddr_tx_s0;      /*  0xb000b058  DECT DMA DDR buffer starting address for TX slot 0   */
   uint32 dect_shm_dma_ddr_saddr_tx_s1;      /*  0xb000b05c  DECT DMA DDR buffer starting address for TX slot 1   */
   uint32 dect_shm_dma_ddr_saddr_tx_s2;      /*  0xb000b060  DECT DMA DDR buffer starting address for TX slot 2   */
   uint32 dect_shm_dma_ddr_saddr_tx_s3;      /*  0xb000b064  DECT DMA DDR buffer starting address for TX slot 3   */
   uint32 dect_shm_dma_ddr_saddr_rx_s0;      /*  0xb000b068  DECT DMA DDR buffer starting address for RX slot 0   */
   uint32 dect_shm_dma_ddr_saddr_rx_s1;      /*  0xb000b06c  DECT DMA DDR buffer starting address for RX slot 1   */
   uint32 dect_shm_dma_ddr_saddr_rx_s2;      /*  0xb000b070  DECT DMA DDR buffer starting address for RX slot 2   */
   uint32 dect_shm_dma_ddr_saddr_rx_s3;      /*  0xb000b074  DECT DMA DDR buffer starting address for RX slot 3   */
   uint32 dect_shm_dma_ahb_saddr_tx_s01;     /*  0xb000b078  DECT DMA AHB shared memory buffer starting address for TX slots 0 and 1  */
   uint32 dect_shm_dma_ahb_saddr_tx_s23;     /*  0xb000b07c  DECT DMA AHB shared memory buffer starting address for TX slots 2 and 3  */
   uint32 dect_shm_dma_ahb_saddr_rx_s01;     /*  0xb000b080  DECT DMA AHB shared memory buffer starting address for RX slots 0 and 1  */
   uint32 dect_shm_dma_ahb_saddr_rx_s23;     /*  0xb000b084  DECT DMA AHB shared memory buffer starting address for RX slots 2 and 3  */
   uint32 dect_shm_dma_xfer_size_tx;         /*  0xb000b088  DECT DMA TX slots transfer size of each trigger      */
   uint32 dect_shm_dma_xfer_size_rx;         /*  0xb000b08c  DECT DMA RX slots transfer size of each trigger      */
   uint32 dect_shm_dma_buf_size_tx;          /*  0xb000b090  DECT DMA TX slots memory buffer size                 */
   uint32 dect_shm_dma_buf_size_rx;          /*  0xb000b094  DECT DMA RX slots memory buffer size                 */
   uint32 dect_shm_dma_offset_addr_tx_s01;   /*  0xb000b098  DECT DMA access offset address for TX slots 0 and 1  */
   uint32 dect_shm_dma_offset_addr_tx_s23;   /*  0xb000b09c  DECT DMA access offset address for TX slots 2 and 3  */
   uint32 dect_shm_dma_offset_addr_rx_s01;   /*  0xb000b0a0  DECT DMA access offset address for RX slots 0 and 1  */
   uint32 dect_shm_dma_offset_addr_rx_s23;   /*  0xb000b0a4  DECT DMA access offset address for RX slots 2 and 3  */
   uint32 dect_shm_dma_xfer_cntr_tx;         /*  0xb000b0a8  DECT DMA transfer count per slot in number of DMA transfer size */
   uint32 dect_shm_dma_xfer_cntr_rx;         /*  0xb000b0a8  DECT DMA transfer count per slot in number of DMA transfer size */   
} BCM63148DECTShimDmaControl;

#define BCM63148_DECT_DMA_CTRL ((volatile BCM63148DECTShimDmaControl * const) BCM63148_DECT_SHIM_DMA_CTRL_BASE)


#ifdef __cplusplus
}
#endif

#endif /* __MAP_63148_H */

