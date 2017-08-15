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
*      Si3050 definitions
*
****************************************************************************/

#ifndef DAA_Si3050_DEFS_H
#define DAA_Si3050_DEFS_H

#ifdef __cplusplus
extern "C"
{
#endif

#define Si3050_NUM_CHANNELS                       1

#define SLAC_DEBUG
/* Slac PCLK_COUNT to drive/read MSB */

/* 
** Timeslot selection.
** 
** The assigned number corresponds to number of clock cycles after 
** frame sync before start of data transmission. For example, value
** of 32 means 32 clock cycles after frame sync. For 16-bit timeslots,
** this means that the data transmission will start after the second 
** timeslot after frame sync.
** 
*/

/* DEFAULT REGISTER SETTINGS */
/* PCM/SPI Mode */
/* Mode: No loopback, no SDO tristate, enable PCM, 16 bit linear, 1 PCLK/bit, tri-state DTX on pos clock */

#define SI3050_ULAW        0x28
#define SI3050_ALAW        0x20
#define SI3050_LINEAR      0x38


/* Interrupt mask: allow all interrupts (for the moment) */
#define REG_IRQMASK_DEFAULT 0xFF  

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
#define FIELD_RDLY10_DEFAULT 0x01
#define REG_RINGVAL2_DEFAULT 0x13
    
/* Wait this time (*10ms) on the software reset */
#define MAX_RESET_WAIT_PERIOD 40

/* Wait this time (*10ms) on the ISOCAP lock */
#define MAX_LOCK_WAIT_PERIOD 20

/* SPI control byte for reading and writing */
#define SPI_CONTROL_READ  0x60
#define SPI_CONTROL_WRITE 0x20


/* Parallel offhook detection factor in % (if v<f*min_openloop) */
#define PAROFFHFACTOR     50

/* Minimum line voltage to detect connected */
#define MIN_LINEVOLTAGE   4


/* Start CID Detection after last POLI interrupt counter (LRAS, RPAS) 
   This number must be bigger then the longest time between two POLI interrupts
   (37 ms for 15 Hz ringing) and smaller then the minimum time FSK follows the AS
   (100ms)
*/
#define STARTCIDTIMEOUT (60/SLAC_POLL_PERIOD)

/* Stop CID detection timeout */
/* For LRAS and RPAS, cid follows within 500 ms after AS
   For ringing, this is after max 2000 ms
   -> Safe margin
   FSK burst takes about 1300ms
*/
#define STOPCIDTIMEOUT (5000/SLAC_POLL_PERIOD)


/* Loop current requirements in 3.3 mA/bit */
#define OFFHLCMIN 0x04		/* minimum current needed for off-hook  */
#define OFFHLCMAX 0x1F		/* current indicates overload condition */



/*#######################################################################
 #                                                                      #
 #  REGISTER MAP                                                        #  
 #                                                                      #  
 #######################################################################*/ 
/* Format of the register addresses:
   0xbbaa
   aa=reg number
   bb=reserved mask of this register (to be sure that these bits are written zero)
*/
#define	GET_REG_ADDR(reg)  (reg       & 0x000000ff)
#define	GET_RESV_MASK(reg) ((reg>>8)  & 0x000000ff)

/* 0: Invalid register. */
#define REG_INVALID   0x0000

/* 1: Control 1 Register */
#define	REG_CONTROL1  0xBA01
#define	FIELD_SR      0x80
#define	FIELD_PWMM    0x30
#define	FIELD_PWME    0x04
#define	FIELD_IDL     0x02

/* 2: Control 2 Register */
#define	REG_CONTROL2  0xDF02
#define	FIELD_INTE    0x80
#define	FIELD_INTP    0x40
#define	FIELD_WDTEN   0x10
#define	FIELD_AL      0x08
#define	FIELD_RDI     0x04
#define	FIELD_HBE     0x02
#define	FIELD_RXE     0x01

/* 3: Interrupt Mask */
#define	REG_IRQMASK   0xFF03
#define	FIELD_RDTM    0x80
#define	FIELD_ROVM    0x40
#define	FIELD_FDTM    0x20
#define	FIELD_BTDM    0x10
#define	FIELD_DODM    0x08
#define	FIELD_LCSOM   0x04
#define	FIELD_TGDM    0x02
#define	FIELD_POLM    0x01

/* 4: Interrupt Source */
#define	REG_IRQSOURCE 0xFF04
#define	FIELD_RDTI    0x80
#define	FIELD_ROVI    0x40
#define	FIELD_FDTI    0x20
#define	FIELD_BTDI    0x10
#define	FIELD_DODI    0x08
#define	FIELD_LCSOI   0x04
#define	FIELD_TGDI    0x02
#define	FIELD_POLI    0x01

/* 5: DAA Control 1 */
#define	REG_DAACONTROL1 0x6D05
#define	FIELD_RDTN    0x40
#define	FIELD_RDTP    0x20
#define	FIELD_ONHM    0x08
#define	FIELD_RDT     0x04
#define	FIELD_OH      0x01

/* 6: DAA Control 2 */
#define	REG_DAACONTROL2 0x1806
#define	FIELD_PDL    0x10
#define	FIELD_PDN    0x08

/* 7: Sample rate */
#define	REG_SAMPRATE 0x0807
#define	FIELD_HSSM   0x08

/* 10: DAA Control 3 */
#define	REG_DAACONTROL3 0x010A
#define	FIELD_DDL    0x01

/* 11: System and Line side device revision */
#define	REG_SYSDEVREV 0xFF0B
#define	FIELD_LSID   0xF0
#define	FIELD_REVA   0x0F

#define	LSID_SI3018  0x01
#define	LSID_SI3019  0x03

/* 12: Line side device status */
#define	REG_LINEDEVSTAT 0x5F0C
#define	FIELD_FDT    0x40
#define	FIELD_LSC    0x1F

/* 13: Line side device revision */
#define	REG_LINEDEVREV 0x3C0D
#define	FIELD_REVB   0x3C

/* 14: DAA Control 4 */
#define	REG_DAACONTROL4 0x020E
#define	FIELD_RPOL   0x02

/* 15: TX/RX Gain Control 1 */
#define	REG_TXRXGAIN1 0x880F
#define	FIELD_TXM    0x80
#define	FIELD_RXM    0x08

/* 16: International Control 1 */
#define	REG_INTERNAT1 0x5310
#define	FIELD_OHS     0x40
#define	FIELD_IIRE    0x10
#define	FIELD_RZ      0x02
#define	FIELD_RT      0x01

/* 17: International Control 2 */
#define	REG_INTERNAT2 0xFF11
#define	FIELD_CALZ    0x80
#define	FIELD_MCAL    0x40
#define	FIELD_CALD    0x20
#define	FIELD_RT2     0x10
#define	FIELD_OPE     0x08
#define	FIELD_BTE     0x04
#define	FIELD_ROV     0x02
#define	FIELD_BTD     0x01

/* 18: International Control 3 */						  
#define	REG_INTERNAT3 0x0212
#define	FIELD_RFWE    0x02

/* 19: International Control 4 */
#define	REG_INTERNAT4 0x0713
#define	FIELD_OVL     0x04
#define	FIELD_DOD     0x02
#define	FIELD_OPD     0x01

/* 20: Call Progress RX Attenuation */
#define	REG_CALLPROGRESSRXATT 0xFF14
#define	FIELD_ARM     0xFF

/* 21: Call Progress TX Attenuation */
#define	REG_CALLPROGRESSTXATT 0xFF15
#define	FIELD_ATM     0xFF

/* 22: Ring Validation Control 1 */
#define	REG_RINGVAL1  0xFF16
#define	FIELD_RDLY10  0xC0
#define	FIELD_RMX     0x3F

/* 23: Ring Validation Control 2 */
#define	REG_RINGVAL2  0xFF17
#define	FIELD_RDLY2   0x80
#define	FIELD_RTO     0x78
#define	FIELD_RCC     0x07

/* 24: Ring Validation Control 2 */
#define	REG_RINGVAL3  0xBF18
#define	FIELD_RNGV    0x80
#define	FIELD_RAS     0x3F

/* 25: Resistor Calibration */
#define	REG_RESCALIBRATION 0xEF19
#define	FIELD_RCALS   0x80
#define	FIELD_RCALM   0x40
#define	FIELD_RCALD   0x20
#define	FIELD_RCAL    0x0F

/* 26: DC Termination control */
#define	REG_DCTERMCONTROL 0xF31A
#define	FIELD_DCV     0xC0
#define	FIELD_MINI    0x30
#define	FIELD_ILIM    0x02
#define	FIELD_DCR     0x01

/* 28: Loop current status */
#define	REG_LOOPCURRENTSTATUS 0xFF1C
#define	FIELD_LCS2    0xFF

/* 29: Line voltage status */
#define	REG_LINEVOLTAGESTATUS 0xFF1D
#define	FIELD_LVS     0xFF

/* 30: AC termination control */
#define	REG_ACTERMCONTROL 0x0F1E
#define	FIELD_ACIM    0x0F

/* 31: DAA Control 5 */
#define	REG_DAACONTROL5 0xEB1F
#define	FIELD_FULL    0x80
#define	FIELD_FOH     0x60
#define	FIELD_OHS2    0x08
#define	FIELD_FILT    0x02
#define	FIELD_LVFD    0x01

/* 32: Ground start control */
#define	REG_GROUNDSTARTCONTROL 0x0720
#define	FIELD_TGD     0x04
#define	FIELD_TGDE    0x02
#define	FIELD_RG      0x01

/* 33: PCM/SPI Mode Select */
#define	REG_PCMSPIMODE 0xFB21
#define	FIELD_PCML    0x80
#define	FIELD_SPIM    0x40
#define	FIELD_PCME    0x20
#define	FIELD_PCMF    0x18
#define	FIELD_PHCF    0x02
#define	FIELD_TRI     0x01

/* 34: PCM Transmit start count low byte */
#define	REG_PCMTRANSSTARTLOW 0xFF22
#define	FIELD_TXS70   0xFF

/* 35: PCM Transmit start count high byte */
#define	REG_PCMTRANSSTARTHIGH 0x0323
#define	FIELD_TXS98   0x03

/* 36: PCM Receive start count low byte */
#define	REG_PCMRECVSTARTLOW 0xFF24
#define	FIELD_RXS70   0xFF

/* 37: PCM Receive start count high byte */
#define	REG_PCMRECVSTARTHIGH 0x0325
#define	FIELD_RXS98   0x03

/* 38: TX Gain control 2 */
#define	REG_TXGAIN2   0x1F26
#define	FIELD_TGA2    0x10
#define	FIELD_TXG2    0x0F

/* 39: RX Gain control 2 */
#define	REG_RXGAIN2   0x1F27
#define	FIELD_RGA2    0x10
#define	FIELD_RXG2    0x0F

/* 40: TX Gain control 3 */
#define	REG_TXGAIN3   0x1F28
#define	FIELD_TGA3    0x10
#define	FIELD_TXG3    0x0F

/* 41: RX Gain control 3 */
#define	REG_RXGAIN3   0x1F29
#define	FIELD_RGA3    0x10
#define	FIELD_RXG3    0x0F

/* 42: GCI Control */
#define	REG_GCICONTROL 0x0F2A
#define	FIELD_GCIF    0x0C
#define	FIELD_B2D     0x02
#define	FIELD_B1D     0x01

/* 43: Line Current/Voltage Threshold interrupt */
#define REG_LCVTH     0xFF2B
#define	FIELD_CVT     0xFF

/* 44: Line Current/Voltage Threshold interrupt control */
#define REG_LCVTHCONTROL 0x0F2C
#define	FIELD_CVI     0x08
#define	FIELD_CVS     0x04
#define	FIELD_CVM     0x02
#define	FIELD_CVP     0x01

/* 45-52: Programmable Hybrid regs */
#define	REG_PROGHYBRID1 0xFF2D
#define	FIELD_HYB1      0xFF
#define	REG_PROGHYBRID2 0xFF2E
#define	FIELD_HYB2      0xFF
#define	REG_PROGHYBRID3 0xFF2F
#define	FIELD_HYB3      0xFF
#define	REG_PROGHYBRID4 0xFF30
#define	FIELD_HYB4      0xFF
#define	REG_PROGHYBRID5 0xFF31
#define	FIELD_HYB5      0xFF
#define	REG_PROGHYBRID6 0xFF32
#define	FIELD_HYB6      0xFF
#define	REG_PROGHYBRID7 0xFF33
#define	FIELD_HYB7      0xFF
#define	REG_PROGHYBRID8 0xFF34
#define	FIELD_HYB8      0xFF

/* 59: Spark Quenching Control */
#define	REG_SPARKQUENCHING 0x503B
#define	FIELD_SQ1     0x40
#define	FIELD_SQ2     0x10

#ifdef __cplusplus
}
#endif

#endif /* DAA_Si3050_DEFS_H */
