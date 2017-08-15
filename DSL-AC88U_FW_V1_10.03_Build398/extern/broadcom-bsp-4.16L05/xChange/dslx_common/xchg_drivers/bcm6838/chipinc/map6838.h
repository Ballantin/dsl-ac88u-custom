/*
<:copyright-broadcom 
 
 Copyright (c) 2007 Broadcom 
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
/*   MODULE:  map6838.h                                                */
/*   DATE:    05/30/08                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM6838                                                  */
/*                                                                     */
/***********************************************************************/
#ifndef __MAP6838_H
#define __MAP6838_H

#ifdef __cplusplus
extern "C" {
#endif

#include <bcmtypesCommon.h>
#include "common6838.h"

/*
** Misc Register Set Definitions.
*/
typedef struct  {
    uint32         IrqMask;
    uint32         IrqStatus;
} BCM6838IrqControl_t;


/*
** UART
*/
typedef struct BCM6838Uart {
  byte          unused0;
  byte          control;
#define BCM6838_BRGEN           0x80    /* Control register bit defs */
#define BCM6838_TXEN            0x40
#define BCM6838_RXEN            0x20
#define BCM6838_LOOPBK          0x10
#define BCM6838_TXPARITYEN      0x08
#define BCM6838_TXPARITYEVEN    0x04
#define BCM6838_RXPARITYEN      0x02
#define BCM6838_RXPARITYEVEN    0x01

  byte          config;
#define BCM6838_XMITBREAK       0x40
#define BCM6838_BITS5SYM        0x00
#define BCM6838_BITS6SYM        0x10
#define BCM6838_BITS7SYM        0x20
#define BCM6838_BITS8SYM        0x30
#define BCM6838_ONESTOP         0x07
#define BCM6838_TWOSTOP         0x0f
  /* 4-LSBS represent STOP bits/char
   * in 1/8 bit-time intervals.  Zero
   * represents 1/8 stop bit interval.
   * Fifteen represents 2 stop bits.
   */
  byte          fifoctl;
#define BCM6838_RSTTXFIFOS      0x80
#define BCM6838_RSTRXFIFOS      0x40
#define BCM6838_RSTTXCHARDONE   0x20
#define BCM6838_UART_RXTIMEOUT  0x1f

  /* 5-bit TimeoutCnt is in low bits of this register.
   *  This count represents the number of characters 
   *  idle times before setting receive Irq when below threshold
   */
  uint32        baudword;
  /* When divide SysClk/2/(1+baudword) we should get 32*bit-rate
   */

  byte          txf_levl;       /* Read-only fifo depth */
  byte          rxf_levl;       /* Read-only fifo depth */
  byte          fifocfg;        /* Upper 4-bits are TxThresh, Lower are
                                 *      RxThreshold.  Irq can be asserted
                                 *      when rx fifo> thresh, txfifo<thresh
                                 */
  byte          prog_out;       /* Set value of DTR (Bit0), RTS (Bit1)
                                 *  if these bits are also enabled to GPIO_o
                                 */
#define BCM6838_UART_DTR_OUT    0x01
#define BCM6838_UART_RTS_OUT    0x02
  byte          unused1;
  byte          DeltaIPEdgeNoSense;     /* Low 4-bits, set corr bit to 1 to 
                                         * detect irq on rising AND falling 
                                         * edges for corresponding GPIO_i
                                         * if enabled (edge insensitive)
                                         */
  byte          DeltaIPConfig_Mask;     /* Upper 4 bits: 1 for posedge sense
                                         *      0 for negedge sense if
                                         *      not configured for edge
                                         *      insensitive (see above)
                                         * Lower 4 bits: Mask to enable change
                                         *  detection IRQ for corresponding
                                         *  GPIO_i
                                         */
  byte          DeltaIP_SyncIP;         /* Upper 4 bits show which bits
                                         *  have changed (may set IRQ).
                                         *  read automatically clears bit
                                         * Lower 4 bits are actual status
                                         */

  uint16        intMask;          /* Same Bit defs for Mask and status */
  uint16        intStatus;
#define BCM6838_DELTAIP         0x0001
#define BCM6838_TXUNDERR        0x0002
#define BCM6838_TXOVFERR        0x0004
#define BCM6838_TXFIFOTHOLD     0x0008
#define BCM6838_TXREADLATCH     0x0010
#define BCM6838_TXFIFOEMT       0x0020
#define BCM6838_RXUNDERR        0x0040
#define BCM6838_RXOVFERR        0x0080
#define BCM6838_RXTIMEOUT       0x0100
#define BCM6838_RXFIFOFULL      0x0200
#define BCM6838_RXFIFOTHOLD     0x0400
#define BCM6838_RXFIFONE        0x0800
#define BCM6838_RXFRAMERR       0x1000
#define BCM6838_RXPARERR        0x2000
#define BCM6838_RXBRK           0x4000
#define BCM6838_TXCHARDONE      0x8000

  uint16        unused2;
  uint16        Data;                   /* Write to TX, Read from RX */
  uint32        unused3;
  uint32        unused4;

} BCM6838Uart;
#define BCM6838_UART ((volatile BCM6838Uart * const) BCM6838_UART_BASE)

/*
** Peripheral Controller
*/
typedef struct BCM6838PerfControl {
     uint32        RevID;                         /* (00) word 0 */
#define BCM6838_CHIP_ID_SHIFT   0x10
#define BCM6838_CHIP_ID_MASK    (0xffff << BCM6838_CHIP_ID_SHIFT)
#define BCM6838_REV_ID_MASK     0xffff  
     uint32        Unused[3];                     /* (04) word 1 */
     uint32        TimerControl;                 /* (10) word 4 */
#define BCM6838_SOFT_RESET_0			0x00000001		// viper watchdog
#define BCM6838_SOFT_RESET_1			0x00000002		// PMC watchdog

     uint32        RsvdIrqMask0;                  /* (14) word 5 */
     uint32        RsvdIrqStatus0;                /* (18) word 6 */
     uint32        RsvdrqMask1;                   /* (1c) word 7 */
     uint32        RsvdIrqStatus1;                /* (20) word 8 */
     uint32        RsvdIrqMask2;                  /* (24) word 9 */
     uint32        RsvdIrqStatus2;                /* (28) word 10 */
     uint32        unused2c;                      /* (2c) word 11 */
     uint32        PeriphIrqMask0;                /* (30) word 12 */
#define BCM6838_WAN_GPON_TX_IRQ	   	(1 << 31)	
#define BCM6838_WAN_GPON_RX_IRQ	   	(1 << 30)
#define BCM6838_WAN_EPON_IRQ		(1 << 29)
#define BCM6838_WAN_NCO_8KHZ_IRQ	(1 << 28)
#define BCM6838_RDP_UBUS_ERR_PORT_IRQ   (1 << 27)
#define BCM6838_RDP_UBUS_ERROR_IRQ	(1 << 26)
#define BCM6838_RDP_RUNNER_IRQ_OFFSET   16
#define BCM6838_RDP_RUNNER_IRQ_MASK     0x3FF00000
#define BCM6838_EXT_IRQ		        (1 << 15)
#define BCM6838_DSCRAM_RNG_READY_IRQ    (1 << 10)
#define BCM6838_DSCRAM_KEYDONE_IRQ      (1 << 9)
#define BCM6838_UART2_IRQ               (1 << 8)      
#define BCM6838_UBUS_REQOUT_ERR_IRQ     (1 << 7)      
#define BCM6838_PERIPH_ERROR_DET_IRQ    (1 << 6)
#define BCM6838_HSSPI_IRQ               (1 << 5)
#define BCM6838_I2CIRQ                  (1 << 4)      
#define BCM6838_NAND_FLASH_IRQ          (1 << 3)
#define BCM6838_UART1_IRQ               (1 << 2)      
#define BCM6838_UART_IRQ                (1 << 1) 
#define BCM6838_TIMRIRQ                 (1 << 0)
     uint32        PeriphIrqStatus0;              /* (34) word 13 */
     uint32        PeriphIrqMask1;                /* (38) word 14 */
     uint32        PeriphIrqStatus1;              /* (3c) word 15 */
	 union {
		 struct {
			 uint32        PeriphIrqMask2;                /* (40) word 16 */
			 uint32        PeriphIrqStatus2;              /* (44) word 17 */
			 uint32        PeriphIrqMask3;                /* (48) word 18 */
			 uint32        PeriphIrqStatus3;              /* (4c) word 19 */
		 };
		 BCM6838IrqControl_t  IrqControl[2];    /* (40) (48)*/
	 };
     uint32        IopIrqMask0;                   /* (50) word 20 */
#define BCM6838_PMCIRQ_TP0			(1 << 5)
#define BCM6838_PMCIRQ_TP1		    (1 << 13)
     uint32        IopIrqStatus0;                 /* (54) word 21 */
     uint32        IopIrqMask1;                   /* (58) word 22 */
     uint32        IopIrqStatus1;                 /* (5c) word 23 */
     uint32        Rsvd0IrqSense;             	  /* (60) word 24 */
     uint32        PeriphIrqSense;                /* (64) word 25 */
     uint32        IopIrqSense;                   /* (68) word 26 */
     uint32        ExtIrqCfg;                  	  /* (6c) word 27 */
#define BCM6838_EI_SENSE_SHFT		0
#define BCM6838_EI_STATUS_SHFT		6
#define BCM6838_EI_MASK_SHFT		12
#define BCM6838_EI_INSENS_SHFT		18
#define BCM6838_EI_LEVEL_SHFT		24
     uint32        ExtIrqCfg1;    	              /* (70) word 28 */
     uint32        ExtIrqCfg2;                    /* (74) word 29 */
     uint32        IrqOutMask;                    /* (78) word 30 */
#define BCM6838_SYSIRQ_OUT_IRQ0_IOP          (1 << 0)
#define BCM6838_SYSIRQ_OUT_IRQ1_IOP          (1 << 1)
#define BCM6838_SYSIRQ_OUT_IRQ0_PERIPH       (1 << 2)
#define BCM6838_SYSIRQ_OUT_IRQ1_PERIPH       (1 << 3)
#define BCM6838_SYSIRQ_OUT_IRQ2_PERIPH       (1 << 4)
#define BCM6838_SYSIRQ_OUT_IRQ0_DOCSIS       (1 << 5)
#define BCM6838_SYSIRQ_OUT_IRQ1_DOCSIS       (1 << 6)
#define BCM6838_SYSIRQ_OUT_IRQ2_DOCSIS       (1 << 7)
#define BCM6838_TESTBUS_OUT_IRQ0_IOP         (1 << 8)
#define BCM6838_TESTBUS_OUT_IRQ1_IOP         (1 << 9)
#define BCM6838_TESTBUS_OUT_IRQ0_PERIPH      (1 << 10)
#define BCM6838_TESTBUS_OUT_IRQ1_PERIPH      (1 << 11)
#define BCM6838_TESTBUS_OUT_IRQ2_PERIPH      (1 << 12)
#define BCM6838_TESTBUS_OUT_IRQ0_DOCSIS      (1 << 13)
#define BCM6838_TESTBUS_OUT_IRQ1_DOCSIS      (1 << 14)
#define BCM6838_TESTBUS_OUT_IRQ2_DOCSIS      (1 << 15)
#define BCM6838_UBUSCAPTURE_OUT_IRQ0_IOP     (1 << 16)
#define BCM6838_UBUSCAPTURE_OUT_IRQ1_IOP     (1 << 17)
#define BCM6838_UBUSCAPTURE_OUT_IRQ0_PERIPH  (1 << 18)
#define BCM6838_UBUSCAPTURE_OUT_IRQ1_PERIPH  (1 << 19)
#define BCM6838_UBUSCAPTURE_OUT_IRQ2_PERIPH  (1 << 20)
#define BCM6838_UBUSCAPTURE_OUT_IRQ0_DOCSIS  (1 << 21)
#define BCM6838_UBUSCAPTURE_OUT_IRQ1_DOCSIS  (1 << 22)
#define BCM6838_UBUSCAPTURE_OUT_IRQ2_DOCSIS  (1 << 23)
#define BCM6838_PCIE_OUT_IRQ0_IOP            (1 << 24)
#define BCM6838_PCIE_OUT_IRQ1_IOP            (1 << 25)
#define BCM6838_PCIE_OUT_IRQ0_PERIPH         (1 << 26)
#define BCM6838_PCIE_OUT_IRQ1_PERIPH         (1 << 27)
#define BCM6838_PCIE_OUT_IRQ2_PERIPH         (1 << 28)
#define BCM6838_PCIE_OUT_IRQ0_DOCSIS         (1 << 29)
#define BCM6838_PCIE_OUT_IRQ1_DOCSIS         (1 << 30)
#define BCM6838_PCIE_OUT_IRQ2_DOCSIS         (1 << 31)
       uint32        diagSelControl;              /* (7c) word 31 */
#define BCM6838_DIAG_HI_SEL_MASK        0x0000ff00
#define BCM6838_DIAG_HI_SEL_SHFT        8
#define BCM6838_DIAG_LO_SEL_MASK        0x000000ff
#define BCM6838_DIAG_LO_SEL_SHFT        0
#define BCM6838_DIAG_CLK_PHS_MASK       0x003f0000
#define BCM6838_DIAG_CLK_PHS_SHIFT      16
#define BCM6838_DIAG_LO_ENABLED         (1 << 24)
#define BCM6838_DIAG_CLK_LO_ENABLED     (1 << 25)
#define BCM6838_DIAG_HI_ENABLED         (1 << 26)
#define BCM6838_DIAG_CLK_HI_ENABLED     (1 << 27)
#define BCM6838_DIAG_UBUS_OBS_ENABLED   (1 << 28)
#define BCM6838_DIAG_PINMUX_OVERRIDE    (1 << 29)
#define BCM6838_DIAG_SPI_OVERRIDE       (1 << 31)
     uint32        diagReadBack;                  /* (80) word 32 */
     uint32        diagReadBackHi;                /* (84) word 33 */
     uint32        diagMiscControl;               /* (88) word 34 */
     uint32        pcie_softResetB_lo;            /* (8c) word 35 */
#define BCM6838_SOFT_RST_PCIE1_CORE 	   	(1 << 1)
#define BCM6838_SOFT_RST_PCIE0_CORE 		(1 << 0)
     uint32        mdio_irq;                      /* (90) word 36 */
#define BCM6838_MDIO_EXT_DONE_IRQ 	    (1 << 0)
#define BCM6838_MDIO_EXT_ERR_IRQ 	    (1 << 1)
#define BCM6838_EGPHY_EXT_DONE_IRQ	    (1 << 2)
#define BCM6838_EGPHY_EXT_ERR_IRQ	    (1 << 3)
#define BCM6838_MDIO_SATA_DONE_IRQ	    (1 << 4)
#define BCM6838_MDIO_SATA_ERR_IRQ	    (1 << 5)
#define BCM6838_MDIO_AE_DONE_IRQ	    (1 << 6)
#define BCM6838_MDIO_AE_ERR_IRQ			(1 << 7)
#define BCM6838_MDIO_EXT_DONE_IEN  		(1 << 8)
#define BCM6838_MDIO_EXT_ERR_IEN   		(1 << 9)
#define BCM6838_EGPHY_EXT_DONE_IEN 		(1 << 10)
#define BCM6838_EGPHY_EXT_ERR_IEN  		(1 << 11)
#define BCM6838_MDIO_SATA_DONE_IEN 		(1 << 12)
#define BCM6838_MDIO_SATA_ERR_IEN  		(1 << 13)
#define BCM6838_MDIO_AE_DONE_IEN   		(1 << 14)
#define BCM6838_MDIO_AE_ERR_IEN    		(1 << 15)
     uint32        dsramIrqStatus;                /* (94) word 37 */
     uint32        ext_irq_muxsel0;               /* (98) word 38 */
#define BCM6838_EXT_IRQ5_SEL	0x3E000000
#define BCM6838_EXT_IRQ4_SEL	0x01F00000
#define BCM6838_EXT_IRQ3_SEL	0x000F8000
#define BCM6838_EXT_IRQ2_SEL	0x00007C00
#define BCM6838_EXT_IRQ1_SEL	0x000002E0
#define BCM6838_EXT_IRQ0_SEL	0x0000001F
     uint32        IntPeriphIrqStatus;                 /* (9C) word 39 */
#define BCM6838_DIAG_IRQ                0x00000100      
#define BCM6838_RBUS_ERR_IRQ            0x00000080      
#define BCM6838_BRIDGE_TO_ERR_IRQ       0x00000040      
#define BCM6838_REQOUT_PLEN_ERR_IRQ     0x00000020      
#define BCM6838_U2R_REPOUT_ERR_IRQ      0x00000010      
#define BCM6838_BRIDGE_UBUS_ERR_IRQ     0x00000008      
#define BCM6838_DEVTIMEOUT_IRQ          0x00000004      
#define BCM6838_ERROR_PORT_IRQ          0x00000002      
#define BCM6838_BAD_BOOT_LOC_IRQ        0x00000001
     uint32        IntPeriphIrqMask;              /* (A0) word 40 */
     uint32        spare[6];  
     uint32        soft_no_watchdog_reset_b;      /* (BC) word 47 */
}BCM6838PerfControl;
#define BCM6838_PERF ((volatile BCM6838PerfControl * const) BCM6838_PERF_BASE)


/***********************************************************/
/*                    Perf ext intr block definition(PERF) */
/***********************************************************/
typedef struct BCM6838ExtIntr {
   uint32 ExtRsvd0irqmask3;  		//rsVD0 Interrupt Mask Register3 
   uint32 ExtRsvd0irqstatus3;  	//RSVD0 Interrupt Status Register3 
   uint32 ExtIopirqmask2;  		//iop Interrupt Mask Register2 
   uint32 ExtIopirqstatus2;  		//ioP Interrupt Status Register2 
   uint32 ExtRsvd0irqmask0_2;  	//RSVD0 Interrupt Mask Register0_2 
   uint32 ExtRsvd0irqstatus0_2;  	//RSVD0 Interrupt Status Register0_2 
   uint32 ExtRsvd0irqmask1_2;  	//RSVD0 Interrupt Mask Register1_2 
   uint32 ExtRsvd0irqstatus1_2;  	//RSVD0 Interrupt Status Register1_2 
   uint32 ExtRsvd0irqmask2_2;  	//RSVD0 Interrupt Mask Register2_2 
   uint32 ExtRsvd0irqstatus2_2;  	//RSVD0 Interrupt Status Register2_2 
   uint32 ExtRsvd0irqmask3_2;  	//RSVD0 Interrupt Mask Register3_2 
   uint32 ExtRsvd0irqstatus3_2;  	//RSVD0 Interrupt Status Register3_2 
   uint32 ExtRsvd0irqsense2;  		//rSVD0 Interrupt Sense Register2 
   uint32 ExtPeriphirqmask0_2;  	//Periph Interrupt Mask Register0_2 
   uint32 ExtPeriphirqstatus0_2;   //Periph Interrupt Status Register0_2 
   uint32 ExtPeriphirqmask1_2;  	//Periph Interrupt Mask Register1_2 
   uint32 ExtPeriphirqstatus1_2;  	//Periph Interrupt Status Register1_2 
   union {
      struct {
         uint32 ExtPeriphirqmask2_2;  	//Periph Interrupt Mask Register2_2 
         uint32 ExtPeriphirqstatus2_2;  	//Periph Interrupt Status Register2_2 
         uint32 ExtPeriphirqmask3_2;  	//RSVD0 Interrupt Mask Register3_2 
         uint32 ExtPeriphirqstatus3_2;  	//Periph Interrupt Status Register3_2 
      };
      BCM6838IrqControl_t  IrqControl[2];    /* (40) (48)*/
   };
   uint32 ExtPeriphirqsense2;  	//Periph Interrupt Sense Register2 
} BCM6838ExtIntr;
#define BCM6838_PERFEXT ((volatile BCM6838ExtIntr * const) BCM6838_PERF_EXT_INT)

/*
** Gpio Controller
*/
typedef struct BCM6838GpioControl {
  /* High in bit location enables output */
  uint32		GPIODir_low;		// 0
  uint32		GPIODir_mid0;		// 4
  uint32		GPIODir_mid1;		// 8
  uint32		reserved_GPIODir[7];// c
  uint32		unused0;			// 24
  uint32		GPIOData_low;		// 2C;
  uint32		GPIOData_mid0;		// 30 :
  uint32		GPIOData_mid1;		// 34:
  uint32		resered_GPIOData[7];// 38
  uint32		unused1;			// 54;
#define BCM6838_USB1_PULLUP			(1 << 23)
#define BCM6838_USB1_PULLDOWN		(1 << 22)
#define BCM6838_USB1_DISABLE_INPUT	(1 << 21)
#define BCM6838_USB1_HYS_ENABLE		(1 << 20)
#define BCM6838_USB0_PULLUP			(1 << 19)
#define BCM6838_USB0_PULLDOWN		(1 << 18)
#define BCM6838_USB0_DISABLE_INPUT	(1 << 17)
#define BCM6838_USB0_HYS_ENABLE		(1 << 16)
  uint32		PadControl;			// 58:
  uint32		SpiSlaveCfg;		// 5C
  uint32		reserved_diag1[3];	// 60
  uint32		TestControl;		// 6C
  uint32		USimControl;		// 70
  uint32		reserved_diag2[8];	// 74
  uint32		strap_bus;			// 94
#define BCM6838_MISC_STRAP_BUS_BOOT_SEL_MASK	0x000003800
#define BCM6838_MISC_STRAP_BUS_BOOT_SEL_SHIFT	0x7
#define BCM6838_MISC_STRAP_SPI_3BYTE_ADDR_MASK	0x4
#define BCM6838_MISC_STRAP_SPI_4BYTE_ADDR_MASK	0x5
  uint32		strap_override;		// 98
  uint32		qam_pll_status;		// 9C - reserved
  uint32		strap_out_bus;		// A0
  uint32		led_input_xor;		// A4
  uint32		reserved_diag3[12];	// A8
  uint32		ddr_pll_override;	// D8
  uint32        diag_kick_timer;    // DC:
  uint32        DieRevID;           // E0:
#define BCM6838_DIEID 0x00000047
  uint32		spi_master_control;	// E4
  uint32		clk_rst_misc;		// E8 - reserved
  uint32		dg_control;			// EC
  uint32		reserved_diag4[36];	// F0
  uint32		IRQ_out_mask1;		// 180
  uint32		sdram_space;		// 184
  uint32		ddr_16_en;			// 188
  uint32		memc_phy_control;	// 18C
  uint32		memc_control;		// 190
  uint32		port_reg_tpIn1;		// 194
  uint32		port_reg_tpIn2;		// 198
  uint32		port_block_en1;		// 19C
  uint32		port_block_en2;		// 1A0
  uint32		port_block_data1;	// 1A4
  uint32		port_block_data2;	// 1A8
#define BCM6838_PINMUX_DATA_SHIFT	12
  uint32		port_command;		// 1AC
#define BCM6838_LOAD_MUX_REG_CMD	0x21
} BCM6838GpioControl;
#define BCM6838_GPIO ((volatile BCM6838GpioControl * const) BCM6838_GPIO_BASE)

#define BCM6838_GPIO_NUM_TO_MASK(X)		((uint32)1 << (X))
#define BCM6838_GPIO_NUM_MAX                    73

/* Some GPIO defs to comply with flexible hal framework */
#define BCM6838_GPIO_NUM_TOTAL_BITS_MASK        BCM6818_GPIO_NUM_MAX //Max possible GPIOs in the system
#define BCM6838_GPIO_NUM_MAX_BITS_MASK          0x1f            // Max GPIO bits in a register

/*
** BCM APM Register Structure and definitions
*/
typedef struct BCM6838ApmControlRegisters
{
    uint16      apm_dev_diag_sel;
#define   BCM6838_APM_DIAG_HI_SEL                 0xff00
#define   BCM6838_APM_DIAG_LO_SEL                 0x00ff

    uint16      apm_dev_irq_status;                     // (0x00) read only block interrupt status
#define   BCM6838_DEV_INTERRUPT                   0x0001        // DMA interrupt pending
#define   BCM6838_APM_INTERRUPT_1                 0x0002        // APM interrupt pending from bank 1
#define   BCM6838_APM_INTERRUPT_2                 0x0004        // APM interrupt pending from bank 2
#define   BCM6838_APM_INTERRUPT_3                 0x0008        // APM interrupt pending from bank 3
#define   BCM6838_PCM_INTERRUPT                   0x0010        // PCM interrupt pending

// semantic from IUDMA perpesctive
// Tx mem to APM
// Rx APM to mem
  uint32        apm_dev_irq_pend;                       // (0x04) DMA interrupt pending register
#define   BCM6838_DMA_A_RX                        0x00000001    // Ch A receive channel interrupt
#define   BCM6838_DMA_A_TX                        0x00000002    // Ch A transmit channel interrupt
#define   BCM6838_DMA_B_RX                        0x00000004    // Ch B receive channel interrupt
#define   BCM6838_DMA_B_TX                        0x00000008    // Ch B transmit channel interrupt
#define   BCM6838_DMA_TX_UNDERFLOW_A              0x00000010    // Ch A transmit channel underflow
#define   BCM6838_DMA_TX_UNDERFLOW_B              0x00000020    // Ch B transmit channel underflow
#define   BCM6838_DMA_RX_OVERFLOW_A               0x00000040    // Ch A receive channel overflow
#define   BCM6838_DMA_RX_OVERFLOW_B               0x00000080    // Ch B receive channel overflow
#define   BCM6838_DMA_PCM_RX                      0x00000100    // PCM Rx DMA IRQ
#define   BCM6838_DMA_PCM_TX                      0x00000200    // PCM Tx DMA IRQ
#define   BCM6838_DEV_BMU_IRQ                     0x00000400    // BMU block IRQ

  uint32        apm_dev_irq_mask;                       // (0x08)DMA interrupt mask register

// Note semantic change
// IUDMA refers to mem to periph as Tx
// and periph to mem as Rx
// APM core refers to path from mem as Rx
// and path to mem as Tx

  uint32        apm_dev_control;                        // (0x0c) Device control register
#define   BCM6838_RX_PACKET_SIZE_A                0x000000ff    // Number of samples to form quasi packet to mem channel A
#define   BCM6838_RX_PACKET_SIZE_B                0x0000ff00    // Number of samples to form quasi packet to mem channel B

#define   BCM6838_RX_DMA_ENABLE_A                 0x10000000    // Enable for Ch A DMA to mem
#define   BCM6838_RX_DMA_ENABLE_A_SHIFT           28

#define   BCM6838_RX_DMA_ENABLE_B                 0x20000000    // Enable for Ch B DMA to mem
#define   BCM6838_RX_DMA_ENABLE_B_SHIFT           29

#define   BCM6838_TX_DMA_ENABLE_A                 0x40000000    // Enable for Ch A DMA from mem
#define   BCM6838_TX_DMA_ENABLE_A_SHIFT           30

#define   BCM6838_TX_DMA_ENABLE_B                 0x80000000    // Enable for Ch B DMA from mem
#define   BCM6838_TX_DMA_ENABLE_B_SHIFT           31

// APM core registers
  uint32        reg_apm_coeff_wr_data;                  // (0x10) 20 bit 2's comp coefficient to be written into coeff RAM
#define   BCM6838_COEFF_PROG_INPUT                0x000FFFFF

  uint32        reg_apm_coeff_config;                   // (0x14)
#define   BCM6838_COEFF_PROG_ADDR                 0x000000FF    //8 bit address into coefficient RAM space                                                       
#define   BCM6838_COEFF_PROG_WE                   0x00000100    //1=write into memory 0= read from memory                                                        
#define   BCM6838_EQ_TX_ACCESS_COEFF_RAM_A        0x00010000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a equalizer        
#define   BCM6838_EQ_RX_ACCESS_COEFF_RAM_A        0x00020000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a equalizer        
#define   BCM6838_HISPD_HYBAL_ACCESS_COEFF_RAM_A  0x00040000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6838_LOSPD_HYBAL_ACCESS_COEFF_RAM_A  0x00080000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6838_YFLTR_ACCESS_COEFF_RAM_A        0x00100000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a hybrid balance   
#define   BCM6838_ASRC_INT_ACCESS_COEFF_RAM_A     0x00200000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b hybrid balance   
#define   BCM6838_ASRC_DEC_ACCESS_COEFF_RAM_A     0x00400000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a y filter         
#define   BCM6838_EQ_TX_ACCESS_COEFF_RAM_B        0x00800000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b y filter         
#define   BCM6838_EQ_RX_ACCESS_COEFF_RAM_B        0x01000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b y filter         
#define   BCM6838_HISPD_HYBAL_ACCESS_COEFF_RAM_B  0x02000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6838_LOSPD_HYBAL_ACCESS_COEFF_RAM_B  0x04000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc interpolator
#define   BCM6838_YFLTR_ACCESS_COEFF_RAM_B        0x08000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b asrc interpolator
#define   BCM6838_ASRC_INT_ACCESS_COEFF_RAM_B     0x10000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel a asrc decimator   
#define   BCM6838_ASRC_DEC_ACCESS_COEFF_RAM_B     0x20000000    //1=processor intrf has control of coefficient RAM 0=normal operation channel b asrc decimator   

#define   BCM6838_APM_HYBAL_NUM_TAPS            5
#define   BCM6838_APM_YFLTR_FIR_NUM_TAPS       18
#define   BCM6838_APM_YFLTR_IIR1_NUM_TAPS       1
#define   BCM6838_APM_YFLTR_IIR2_NUM_TAPS       5
#define   BCM6838_APM_YFLTR_BLOCK_SIZE         28
#define   BCM6838_APM_RX_EQ_NUM_TAPS           80
#define   BCM6838_APM_TX_EQ_NUM_TAPS           80 
#define   BCM6838_APM_ASRC_DEC_NUM_TAPS       120
#define   BCM6838_APM_ASRC_INT_NUM_TAPS       120

  uint32        reg_cha_test_config;                    // (0x18)
#define   BCM6838_RX_IN_MUX_CTRL                  0x00000003    //2 bits mux control  at eq input 0=test stimulus 1=rx_data_in 2=loopback from tx path channel a
#define   BCM6838_RX_IN_MUX_CTRL_SHIFT            0

#define   BCM6838_NETWORK_LPBK                    0x00000004    //1=take data inputs and loopback to data outputs (network side) channel a
#define   BCM6838_NETWORK_LPBK_SHIFT              2

#define   BCM6838_RX_98K_MUX_CTRL                 0x00000018    //1=bypass 0=use eq output on rx side channel a
#define   BCM6838_RX_98K_MUX_CTRL_SHIFT           3

#define   BCM6838_ADC_MUX_CTRL                    0x00000060    //2 bits mux at input to tx path 0=loopback 1=adc out 2=test stimulus channel a
#define   BCM6838_ADC_MUX_CTRL_SHIFT              5

#define   BCM6838_ADC_DAC_LPBK                    0x00000080    //1=take 4 bit ADC codes and send them to the DEM and DAC
#define   BCM6838_ADC_DAC_LPBK_SHIFT              7

#define   BCM6838_RX_TX_98K_LPBK                  0x00000100    //1=loopback from rx to tx path channel a
#define   BCM6838_RX_TX_98K_LPBK_SHIFT            8

#define   BCM6838_SLEW_INSTANT                    0x00000200    //1=slew scale, 0=make scale changes instantaneously
#define   BCM6838_SLEW_INSTANT_SHIFT              9

#define   BCM6838_ASRC_EN                         0x00000400    //1=asrc and eq are active 0=inactive
#define   BCM6838_ASRC_EN_SHIFT                   10


  uint32        reg_chb_test_config;                    // (0x1c)

  uint32        reg_cha_eq_config;                      // (0x20)
#define   BCM6838_EQ_RX_NUM_TAPS                  0x0000007f    //7 bit number of recieve taps
#define   BCM6838_EQ_RX_NUM_TAPS_SHIFT            0

#define   BCM6838_EQ_TX_NUM_TAPS                  0x00007f00    //7 bit number of transmit taps
#define   BCM6838_EQ_TX_NUM_TAPS_SHIFT            8

#define   BCM6838_EQ_TX_SHFT                      0x000f0000    //4 bits controls output shifter (powers of 2 gain control) for rx path channel a               
#define   BCM6838_EQ_TX_SHFT_SHIFT                16

#define   BCM6838_EQ_RX_SHFT                      0x00f00000    //4 bits controls output shifter (powers of 2 gain control) for tx path channel a
#define   BCM6838_EQ_RX_SHFT_SHIFT                20

#define   BCM6838_EQ_RX_IMP_RESP                  0x01000000    //1=use ext RAM for eq coefficients 0=use int ROM for eq coefficients channel a
#define   BCM6838_EQ_RX_IMP_RESP_SHIFT            24

#define   BCM6838_EQ_TX_IMP_RESP                  0x02000000    //1 the eq has an impulse shape response, 0= use the RAM or ROM for eq coefficients
#define   BCM6838_EQ_TX_IMP_RESP_SHIFT            25

#define   BCM6838_EQ_RX_ENABLE                    0x04000000    //equalizer transmit enable
#define   BCM6838_EQ_RX_ENABLE_SHIFT              26

#define   BCM6838_EQ_TX_ENABLE                    0x08000000    //equalizer receive enable
#define   BCM6838_EQ_TX_ENABLE_SHIFT              27

#define   BCM6838_SOFT_INIT                       0x80000000    //initializes state machines and registers
#define   BCM6838_SOFT_INIT_SHIFT                 31

  uint32        reg_cha_hybal_config;                   // (0x24)
#define   BCM6838_HISPD_HYBAL_SHFT                0x00000007    //
#define   BCM6838_HISPD_HYBAL_SHFT_SHIFT          0

#define   BCM6838_LOSPD_HYBAL_SHFT                0x00000038    //
#define   BCM6838_LOSPD_HYBAL_SHFT_SHIFT          3

#define   BCM6838_HISPD_HYBAL_EN                  0x00000080    //
#define   BCM6838_HISPD_HYBAL_EN_SHIFT            7

#define   BCM6838_LOSPD_HYBAL_EN                  0x00000100    //
#define   BCM6838_LOSPD_HYBAL_EN_SHIFT            8

#define   BCM6838_HISPD_HYBAL_SMPL_OFFSET         0x00007000    //
#define   BCM6838_HISPD_HYBAL_SMPL_OFFSET_SHIFT   12

#define   BCM6838_YFLTR_EN                        0x00008000    //1=y-filter is active 0=y-filter outputs 0 only
#define   BCM6838_YFLTR_EN_SHIFT                  15

#define   BCM6838_HPF_EN                          0x00010000    //1=high pass filter is active 0=high pass filter outputs 0 only
#define   BCM6838_HPF_EN_SHIFT                    16

#define   BCM6838_LOSPD_HYBAL_SMPL_OFFSET         0x3f000000    //
#define   BCM6838_LOSPD_HYBAL_SMPL_OFFSET_SHIFT   24

  uint32        reg_cha_int_config_1;                   // (0x28)
#define   BCM6838_ASRC_INT_NUM_SECT               0x001f0000    //5 bits--if symmetric it represents 1/2 of the number of sections (minus 1) in the filter channel a
#define   BCM6838_ASRC_INT_NUM_SECT_SHIFT         16            //  if asymmetric, it represents the number of sections (minus 1) channel a                       

#define   BCM6838_ASRC_INT_HRNR_SHFT              0x03000000    //2 bits defines shifting at output of asrc int hrnr channel a    
#define   BCM6838_ASRC_INT_HRNR_SHFT_SHIFT        24

#define   BCM6838_ASRC_INT_SYM_ASYM               0x04000000    //1=filter coefficients are symmetrical (only 1/2 are stored) channel a
#define   BCM6838_ASRC_INT_SYM_ASYM_SHIFT         26

#define   BCM6838_ASRC_INT_COEFF_SEL              0x18000000    //
#define   BCM6838_ASRC_INT_COEFF_SEL_SHIFT        27

  uint32        reg_cha_int_config_2;                   // (0x2c)
#define   BCM6838_ASRC_INT_SCALE                  0x0000ffff    //16 bit scale value channel a
#define   BCM6838_ASRC_INT_SCALE_SHIFT            0

#define   BCM6838_ASRC_INT_FIR_SHFT               0x00070000    //3 bits defines shifting at output of mac before writing into data RAM channel a
#define   BCM6838_ASRC_INT_FIR_SHFT_SHIFT         16

#define   BCM6838_ASRC_SLEW_SPEED                 0x00780000    //
#define   BCM6838_ASRC_SLEW_SPEED_SHIFT           19

  uint32        reg_cha_pm_config_1;                    // (0x30)
#define   BCM6838_PM_ON_TIME                      0x00000fff    //12 bit number in 2 msec units for on-time channel a
#define   BCM6838_PM_ON_TIME_SHIFT                0

#define   BCM6838_PM_OFF_TIME                     0x0fff0000    //12 bit number in 2 msec units for off-time channel a
#define   BCM6838_PM_OFF_TIME_SHIFT               16

#define   BCM6838_PM_FREQ_16_12                   0x10000000    //1=16 kHz frequency 0=12 kHz frequency channel a
#define   BCM6838_PM_FREQ_16_12_SHIFT             28

#define   BCM6838_PM_TIME_MS                      2 / 5


  uint32        reg_cha_pm_config_2;                    // (0x34)
#define   BCM6838_PM_SCALE                        0x0000ffff    //16 bit 2's complement number that scales the output from full scale channel a
#define   BCM6838_PM_SCALE_SHIFT                  0

#define   BCM6838_PM_RISE_INC                     0x00ff0000    //8 bit number goes into accumulator for rise time channel a
#define   BCM6838_PM_RISE_INC_SHIFT               16

#define   BCM6838_PM_FALL_INC                     0xff000000    //8 bit number goes into accumulator for fall time channel a
#define   BCM6838_PM_FALL_INC_SHIFT               24

  uint32        reg_cha_pm_config_3;                    // (0x38)
#define   BCM6838_PM_BURST_LENGTH                 0x0000ffff    //16 bit number defines the number of bursts to send out
#define   BCM6838_PM_BURST_LENGTH_SHIFT           0

#define   BCM6838_PM_BURST_CONT                   0x00010000    //1=send out infinite number of bursts 0=send out "pm_burst_length"
#define   BCM6838_PM_BURST_CONT_SHIFT             16

#define   BCM6838_PM_BURST_START                  0x00020000    //strobe the indicates that a burst should start (this will be edge detected)
#define   BCM6838_PM_BURST_START_SHIFT            17

#define   BCM6838_PM_ENABLE                       0x00040000    //1=pulse metering is active and on, 0=inactive and off channel a
#define   BCM6838_PM_ENABLE_SHIFT                 18

  uint32        reg_cha_cic_config;                     // (0x3c)
#define   BCM6838_CIC_DEC_SHFT                    0x00000007    //3 bits controls output shifter (powers of 2 gain control) channel a
#define   BCM6838_CIC_DEC_SHFT_SHIFT              0

#define   BCM6838_CIC_INC_SHFT                    0x00000038    //3 bits controls output shifter (powers of 2 gain control) channel a
#define   BCM6838_CIC_INC_SHFT_SHIFT              3

#define   BCM6838_CIC_INC_EQ_EN                   0x00000100    //1=enable equalizer
#define   BCM6838_CIC_INC_EQ_EN_SHIFT             8

#define   BCM6838_CIC_DEC_EQ_EN                   0x00000200    //1=enable equalizer
#define   BCM6838_CIC_DEC_EQ_EN_SHIFT             9

  uint32        reg_cha_asrc_dec_config;                // (0x40)
#define   BCM6838_ASRC_DEC_SCALE                  0x0000ffff    //16 bit 2's complement scale value channel a
#define   BCM6838_ASRC_DEC_SCALE_SHIFT            0

#define   BCM6838_ASRC_DEC_NUM_SECT               0x001f0000    //5 bit number 0-20 defines the number of sections in the filter channel a
#define   BCM6838_ASRC_DEC_NUM_SECT_SHIFT         16            //   if symmetric it equals the number of sections/2 -1                   
                                                        //   if asymmetric it equals the number of sections -1                    
#define   BCM6838_ASRC_DEC_SHFT                   0x0f000000    //4 bits defines msb location at input [38:31] channel a
#define   BCM6838_ASRC_DEC_SHFT_SHIFT             24

#define   BCM6838_ASRC_DEC_SYM_ASYM               0x10000000    //1=symmetric filter 0=asymmetric filter channel a
#define   BCM6838_ASRC_DEC_SYM_ASYM_SHIFT         28

#define   BCM6838_ASRC_DEC_COEFF_SEL              0x60000000    //ASRC DEC coefficient select
#define   BCM6838_ASRC_DEC_COEFF_SEL_SHIFT        29

  uint32        reg_cha_fcw_config_1;                   // (0x44)
#define   BCM6838_FCW_SCALE                       0xffffffff    //32 bit unsigned scale value for frequency control word channel a

  uint32        reg_cha_fcw_config_2;                   // (0x48)
#define   BCM6838_FCW_SHFT                        0x0000000f    //4 bit shift control for fcw channel a
#define   BCM6838_FCW_SHFT_SHIFT                  0

#define   BCM6838_CLOCK_SOURCE                    0x00000030    //2 bit mux control 0=NTP 1=DPLL 2=misc for channel a
#define   BCM6838_CLOCK_SOURCE_SHIFT              4

  
  uint32        reg_cha_digmod_config;                  // (0x4c)
#define   BCM6838_DIGMOD_DEM_TONE                 0x00000001    //1=suppress tones at fs/2 channel a
#define   BCM6838_DIGMOD_DEM_TONE_SHIFT           0

#define   BCM6838_DIGMOD_DEM_DIS                  0x00000002    //1=use balanced code as output instead of dynamic element matcher channel a
#define   BCM6838_DIGMOD_DEM_DIS_SHIFT            1

#define   BCM6838_DIGMOD_DITH_SCALE               0x0000ff00    // new 10/1
#define   BCM6838_DIGMOD_DITH_SCALE_SHIFT         8


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
#define   BCM6838_FCW_REG                         0xffffffff    //32 bit frequency control word from NTP block

  uint32        reg_fcw_config_2;                       // (0x84)
#define   BCM6838_LOAD_NTP                        0x00000001    // triggers pulse to load FCW to NTP
#define   BCM6838_LOAD_NTP_SHIFT                  0

#define   BCM6838_LOAD_MISC_A                     0x00000002    // triggers pulse to load FCW to ch A
#define   BCM6838_LOAD_MISC_A_SHIFT               1

#define   BCM6838_LOAD_MISC_B                     0x00000004    // triggers pulse to load FCW to ch B
#define   BCM6838_LOAD_MISC_B_SHIFT               2

#define   BCM6838_LOAD_FCW_NOM_A                  0x00000008    //strobe to load nominal FCW value
#define   BCM6838_LOAD_FCW_NOM_A_SHIFT            3

#define   BCM6838_LOAD_FCW_NOM_B                  0x00000010    //strobe to load nominal FCW value
#define   BCM6838_LOAD_FCW_NOM_B_SHIFT            4

#define   BCM6838_FCW_LIMIT                       0x00000f00    //4 bits defines allowable error on FCW relative to nominal
#define   BCM6838_FCW_LIMIT_SHIFT                 8

  uint32        reg_ntp_config_1;                       // (0x88)
#define   BCM6838_NTP_TIME_DVD                    0x0000ffff    //16 bit divide value for time counter (16'd374 is default)

  uint32        reg_ntp_config_2;                       // (0x8c)
#define   BCM6838_NTP_READ_TIME                   0x00000001    //strobe asks for new time count value

  uint32        reg_ring_config_1;                      // (0x90)
#define   BCM6838_RING_START_IMMED_A              0x00000001    //1=start cadence
#define   BCM6838_RING_START_IMMED_A_SHIFT        0

#define   BCM6838_RING_START_IMMED_B              0x00000002    //1=start cadence
#define   BCM6838_RING_START_IMMED_B_SHIFT        1

#define   BCM6838_RING_START_NEXT_PHASE_A         0x00000004    //1=start cadence
#define   BCM6838_RING_START_NEXT_PHASE_A_SHIFT   2

#define   BCM6838_RING_START_NEXT_PHASE_B         0x00000008    //1=start cadence
#define   BCM6838_RING_START_NEXT_PHASE_B_SHIFT   3

#define   BCM6838_RING_STOP_IMMED_A               0x00000010    // stop ring now!
#define   BCM6838_RING_STOP_IMMED_A_SHIFT         4

#define   BCM6838_RING_STOP_IMMED_B               0x00000020    // stop ring now!
#define   BCM6838_RING_STOP_IMMED_B_SHIFT         5

#define   BCM6838_RING_STOP_NEXT_PHASE_A          0x00000040    // stop ring at next phase
#define   BCM6838_RING_STOP_NEXT_PHASE_A_SHIFT    6

#define   BCM6838_RING_STOP_NEXT_PHASE_B          0x00000080    // stop ring at next phase
#define   BCM6838_RING_STOP_NEXT_PHASE_B_SHIFT    7

#define   BCM6838_RING_OMEGA_NORM                 0x000fff00    //the omega man is Charlton Heston
#define   BCM6838_RING_OMEGA_NORM_SHIFT           8

#define   BCM6838_RING_DELAY                      0x01f00000    //ring start delay
#define   BCM6838_RING_DELAY_SHIFT                20

#define   BCM6838_RING_GEN_SOFT_INIT              0x80000000    //soft init bit
#define   BCM6838_RING_GEN_SOFT_INIT_SHIFT        31

  uint32        reg_ring_config_2;                      // (0x94)
  uint32        reg_ring_config_3;                      // (0x98)
#define   BCM6838_RING_OFFSET                     0xffff0000    //ring offset
#define   BCM6838_RING_OFFSET_SHIFT               16

#define   BCM6838_RING_SCALE                      0x0000ffff    //ring scale
#define   BCM6838_RING_SCALE_SHIFT                0

  uint32        reg_ring_config_4;                      // (0x9c)
#define   BCM6838_RING_CREST_FACTOR_A             0x00000007    //3 bit crest factor
#define   BCM6838_RING_CREST_FACTOR_A_SHIFT       0

#define   BCM6838_RING_GEN_ENABLE_A               0x00000008    //1=enabled
#define   BCM6838_RING_GEN_ENABLE_A_SHIFT         3

#define   BCM6838_RING_GEN_PHASE_A                0x00003ff0    //start/stop phase
#define   BCM6838_RING_GEN_PHASE_A_SHIFT          4

#define   BCM6838_RING_CREST_FACTOR_B             0x00070000    //3 bit crest factor
#define   BCM6838_RING_CREST_FACTOR_B_SHIFT       16

#define   BCM6838_RING_GEN_ENABLE_B               0x00080000    //1=enabled
#define   BCM6838_RING_GEN_ENABLE_B_SHIFT         19

#define   BCM6838_RING_GEN_PHASE_B                0x3ff00000    //start/stop phase
#define   BCM6838_RING_GEN_PHASE_B_SHIFT          20

  uint32        reg_ser_config;                         // (0xa0)
#define   BCM6838_SER_MUX_SEL                     0x00000003
#define   BCM6838_SER_MUX_SEL_SHIFT               0

//#define   BCM6838_SER_TST_SHFT_CTRL               0x07800000    //
#define   BCM6838_AUD_SER_TST_FCW_MUX_CTRL        0x08000000    //
#define   BCM6838_AUD_SER_TST_FCW_MUX_CTRL_SHIFT  27

//#define   BCM6838_SER_TST_LOAD_REGFILE            0x20000000    //1->use channel b requests for transfer 0-> use channel a
#define   BCM6838_SER_TST_CLKS                    0x40000000    //1=use clocks supplied by SER 0=generate internal clocks
#define   BCM6838_SER_TST_CLKS_SHIFT              30

#define   BCM6838_SOFT_INIT_AP                    0x80000000
#define   BCM6838_SOFT_INIT_AP_SHIFT              31

  uint32        reg_spare_unused;                       // (0xa4)
//#define   BCM6838_SER_TST_SCALE                   0xffffffff

  uint32        reg_stats_config;                       // (0xa8)
#define   BCM6838_START_STATS                     0x00008000    //[15]
#define   BCM6838_STATS_MUX_CTL_VADCA             0x00000800    //[11]
#define   BCM6838_STATS_MUX_CTL_VADCB             0x00000400    //[10]
#define   BCM6838_STATS_CHOP_ENABLE_A             0x00000200    //[9]
#define   BCM6838_STATS_CHOP_ENABLE_B             0x00000100    //[8]
#define   BCM6838_STATS_DURATION                  0x000000f0    //[7:4]
#define   BCM6838_STATS_CHOP_PERIOD               0x0000000f    //[3:0]

  // APM Status Registers
  uint32        reg_apm_status_1;                       // (0xac)
#define   BCM6838_DIGMOD_SAT_QUANT                0x01000000    //strobe indicating quantizer has saturated
#define   BCM6838_DIGMOD_SAT_COUNT_0              0x00ff0000    //8 bit counter of saturation on integrator 0 channel a
#define   BCM6838_DIGMOD_SAT_COUNT_1              0x0000ff00    //8 bit counter of saturation on integrator 1 channel a
#define   BCM6838_DIGMOD_SAT_COUNT_QUANT          0x000000ff    //8 bit counter of saturation on quantizer channel a

  uint32        reg_apm_status_2;                       // (0xb0) same as status 1 for ch B

  uint32        reg_apm_status_3;                       // (0xb4)
#define   BCM6838_AP_XMT_UFLOW                    0x00200000    //level indicating xnt fifo is empty when read attempted
#define   BCM6838_AP_XMT_OVLOW                    0x00100000    //level indicating xmt fifo is full when write attempted
#define   BCM6838_AP_NCO_SATURATED                0x00080000    // NCO is saturated
#define   BCM6838_AP_XMT_FIFO_DEPTH               0x00070000    //3 bit depth number on xmt fifo
#define   BCM6838_AP_RCV_UFLOW                    0x00000020    //level indicating rcv fifo is empty when read attempted
#define   BCM6838_AP_RCV_OVLOW                    0x00000010    //level indicating rcv fifo is full when write attempted
#define   BCM6838_AP_RCV_FIFO_DEPTH               0x00000007    //3 bit depth number on receive fifo

  uint32        reg_apm_status_4;                       // (0xb8)
#define   BCM6838_EQ_RX_COEFF                     0xffff0000    //16 bit 2's complement coefficients from RAM
#define   BCM6838_EQ_TX_COEFF                     0x0000ffff    //16 bit coefficient read from RAM or ROM

  uint32        reg_apm_status_5;                       // (0xbc)
#define   BCM6838_ASRC_INT_COEFF                  0xffff0000    //16 bit 2's comp current coefficient value
#define   BCM6838_ASRC_DEC_COEFF                  0x0000ffff    //16 bit coefficient from RAM or ROM

  uint32        reg_apm_status_6;                       // (0xc0)
#define   BCM6838_YFLTR_COEFF                     0x000fffff    //20 bit 2's complement coefficients from RAM

// Status 9-7 same as 6-4 for Ch B
  uint32        reg_apm_status_7;                       // (0xc4)
  uint32        reg_apm_status_8;                       // (0xc8)
  uint32        reg_apm_status_9;                       // (0xcc)

  uint32        reg_apm_status_10;                      // (0xd0)
#define   BCM6838_NTP_TIME_COUNT_INTEGER          0x0001ffff    //17 integer bits (in seconds)

  uint32        reg_apm_status_11;                      // (0xd4)
#define   BCM6838_NTP_TIME_COUNT_FRACT            0x0003ffff    //18 fractional bits (in seconds)

  uint32        reg_apm_status_12;                      // (0xd8)
#define   BCM6838_RING_CADENCE_STOPPED_A          0x00000008    //
#define   BCM6838_RING_ON_OFF_A                   0x00000004    //
#define   BCM6838_RING_CADENCE_STOPPED_B          0x00000002    //
#define   BCM6838_RING_ON_OFF_B                   0x00000001    //

  uint32        reg_apm_status_13;                      // (0xdc)
#define   BCM6838_HISPD_HYBAL_COEFF_A             0xffff0000    //
#define   BCM6838_HISPD_HYBAL_COEFF_B             0x0000ffff    //

  uint32        reg_apm_status_14;                      // (0xe0)
#define   BCM6838_LOSPD_HYBAL_COEFF_A             0xffff0000    //
#define   BCM6838_LOSPD_HYBAL_COEFF_B             0x0000ffff    //

  int32        reg_apm_stats_1;                        // (0xe4)
#define   BCM6838_MEAN                            0xffffffff    // 16 bit mean stats

  int32        reg_apm_stats_2;                        // (0xe8), same as reg_apm_stats_1 for channel B
  uint32        reg_apm_stats_3;                        // (0xec)
#define   BCM6838_MEAN_SQUARE_LO                  0xffffffff    // lower 32 bits of 40 bit mean square value

  uint32        reg_apm_stats_4;                        // (0xf0)
#define   BCM6838_MEAN_SAT                        0x00040000
#define   BCM6838_MEAN_SQUARE_SAT                 0x00020000
#define   BCM6838_STATS_BUSY                      0x00010000    // 1 bit stats busy
#define   BCM6838_MEAN_SQUARE_HI                  0x00003fff    // upper 14 bits of 46 bit mean square value

  uint32        reg_apm_stats_5;                        // (0xf4), same as reg_apm_stats_3 for channel B
  uint32        reg_apm_stats_6;                        // (0xf8), same as reg_apm_stats_4 for channel B

  uint32        reg_apm_audio_fcw_a;                    // (0xfc)
  uint32        reg_apm_audio_fcw_b;                    // (0x100)

  uint32        reg_apm_irq_pend_1;                     // (0x104) Ch A status
#define   BCM6838_PM_BURST_DONE                   0x80000000    //strobe indicating that PM burst has completed
#define   BCM6838_ASRC_INT_DONE_SLEWING           0x40000000    //strobe indicating that slewing has completed
#define   BCM6838_ASRC_DEC_DONE_SLEWING           0x20000000    //strobe indicating that slewing has completed
#define   BCM6838_RX_PATH_SAT                     0x10000000    //strobed high when audio + pulse meter + y-filter exceeds 16 bit number
#define   BCM6838_EQ_TX_SAT                       0x08000000    //strobe indicates that the result on the eq tx path was saturated
#define   BCM6838_EQ_RX_SAT                       0x04000000    //strobe indicates that the result on the rx path was saturated at output
#define   BCM6838_EQ_TX_RQST_ERR                  0x02000000    //strobe indicating a tx_rqst was asked for while a tx was pending
#define   BCM6838_EQ_RX_RQST_ERR                  0x01000000    //strobe indicating a rx_rqst was asked for while a rx was pending
#define   BCM6838_LOSPD_HYBAL_SAT                 0x00800000    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6838_HISPD_HYBAL_SAT                 0x00400000    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6838_HISPD_HYBAL_RQST_ERR            0x00200000    //strobe indicating a output_rqst was asked for while computation engine was busy
#define   BCM6838_YFLTR_IIR1_SAT                  0x00100000    //strobe indicating iir-1 is saturating
#define   BCM6838_YFLTR_FIR_SAT                   0x00080000    //strobe indicating fir is saturating
#define   BCM6838_YFLTR_IIR2_SAT                  0x00040000    //strobe indicating iir-2 is saturating
#define   BCM6838_YFLTR_IIR1_ACCUM_SAT            0x00020000    //strobe indicating iir1 accumulator saturated
#define   BCM6838_YFLTR_FIR_ACCUM_SAT             0x00010000    //strobe indicating fir accumulator saturated
#define   BCM6838_YFLTR_IIR2_ACCUM_SAT            0x00008000    //strobe indicating iir2 accumulator saturated
#define   BCM6838_YFLTR_RQST_ERR                  0x00004000    //strobe indicating a output_rqst was asked for while computation engine was busy
#define   BCM6838_NCO_SAT                         0x00002000    //strobe indicates scaling and shifting saturated the result
#define   BCM6838_ASRC_INT_HRNR_TIM_ERR           0x00001000    //strobe indicating delta has rolled over too quickly before fir has completed computation
#define   BCM6838_ASRC_INT_HRNR_SAT               0x00000800    //strobe indicating data has saturated at the mac output
#define   BCM6838_ASRC_INT_FIR_TIM_ERR            0x00000400    //strobe indicating delta has rolled over too quickly before fir has completed computation
#define   BCM6838_ASRC_INT_FIR_BANK_ERR           0x00000200    //strobe indicating that horner ngn asked for a bank switch while fir was busy
#define   BCM6838_ASRC_INT_FIR_SAT                0x00000100    //strobe indicating data has saturated at the mac output
#define   BCM6838_ASRC_INT_HRNR_MAC_SAT           0x00000080    //strobe indicating ser mac in hrnr block has saturated
#define   BCM6838_CIC_INT_SAT                     0x00000040    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6838_DIGMOD_SAT_0                    0x00000020    //strobe indicating the integrator 0 has saturated
#define   BCM6838_DIGMOD_SAT_1                    0x00000010    //strobe indicating integrator 1 has saturated
#define   BCM6838_TX_PATH_SAT                     0x00000008
#define   BCM6838_CIC_DEC_SAT                     0x00000004    //strobe indicates that the result on the tx path was saturated at output
#define   BCM6838_ASRC_DEC_TIM_ERR                0x00000002    //strobe asserted when "input_data_ready" is asserted while ngn is busy
#define   BCM6838_ASRC_DEC_SAT                    0x00000001    //strobe indicating that the input was saturated
        
  uint32        reg_apm_irq_mask_1;                     // (0x108)

  uint32        reg_apm_irq_pend_2;                     // (0x10c) same as APM_IRQ_1 for Ch B
  uint32        reg_apm_irq_mask_2;                     // (0x110)

  uint32        reg_apm_irq_pend_3;                     // (0x114)
#define   BCM6838_STATS_DONE_A                    0x00000400    //strobe indicating statistics gathered and computed chA
#define   BCM6838_STATS_DONE_B                    0x00000200    //strobe indicating statistics gathered and computed chB
#define   BCM6838_FCW_OOB_A                       0x00000100    //level indicating that FCW error is too large
#define   BCM6838_FCW_OOB_B                       0x00000080    //level indicating that FCW error is too large
#define   BCM6838_HPF_SAT_A                       0x00000040    //strobe indicating high pass filter has saturated, Channel A
#define   BCM6838_HPF_SAT_B                       0x00000020    //strobe indicating high pass filter has saturated, Channel B
#define   BCM6838_ASRC_INT_DELTA_ADJ_DONE_A       0x00000010    //strobe indicating TODO
#define   BCM6838_ASRC_INT_DELTA_ADJ_DONE_B       0x00000008    //strobe indicating TODO
#define   BCM6838_LOSPD_HYBAL_RQST_ERR_A          0x00000004    //strobe indicating a output_rqst was asked for while computation engine was busy, channel A
#define   BCM6838_LOSPD_HYBAL_RQST_ERR_B          0x00000002    //strobe indicating a output_rqst was asked for while computation engine was busy, channel B
#define   BCM6838_NTP_TIME_VALID                  0x00000001    //strobe in clk_200 domain (4 clock periods)

  uint32        reg_apm_irq_mask_3;                     // (0x118)
  uint32        reg_spare_2;                            // (0x11c)

  uint32        reg_codec_config_0;                     // (0x120)
  uint32        reg_codec_config_1;                     // (0x124) VTX1
  uint32        reg_codec_config_2;                     // (0x128) same as config 16 for VTX2

  uint32        reg_codec_config_3;                     // (0x12c) VRX1
#define   BCM6838_PGA_CTL_VADC                    0x30000000
#define   BCM6838_PGA_CTL_VADC_SHIFT                      28

  uint32        reg_codec_config_4;                     // (0x130) same as config 18 for VRX2
#define   BCM6838_VREF_CTL_VDAC                   0x70000000 
#define   BCM6838_PWRDN_REF_VDAC                  0x01000000 
#define   BCM6838_PWRDN_REF_VDAC_A                0x00020000 
#define   BCM6838_TESTEN_VDAC_A                   0x00010000 
#define   BCM6838_DRVVOCMCTL_VDAC                 0x00007000 
#define   BCM6838_PGA_GAIN_CTL_VDAC_A             0x00000180 
#define   BCM6838_PGA_GAIN_CTL_VDAC_A_SHIFT                7
#define   BCM6838_PWRDN_REF_VDAC_B                0x00000040 
#define   BCM6838_TESTEN_VDAC_B                   0x00000020 
#define   BCM6838_PGA_GAIN_CTL_VDAC_B             0x00000018 
#define   BCM6838_PGA_GAIN_CTL_VDAC_B_SHIFT                3

  uint32        reg_codec_config_5;                     // (0x134) 32Khz Ring Gen config
  uint32        reg_codec_config_6;                     // (0x138) reserved
#define   BCM6838_DIAG_TYPE_VADC_A                0x00070000 
#define   BCM6838_DIAG_TYPE_VADC_A_SHIFT                  16
#define   BCM6838_DIAG_TYPE_VADC_B                0x00000007 
#define   BCM6838_DIAG_TYPE_VADC_B_SHIFT                   0

  uint32        reg_codec_config_7;                     // (0x13c) reserved
  uint32        reg_codec_config_8;                     // (0x140) reserved

  uint32        reg_apm_codec_fb_0;                     // (0x144) 32Khz Feedback
  uint32        reg_mbist_0_unused;                     // (0x148) MBIST control register
  uint32        reg_mbist_1_unused;                     // (0x14c) MBIST control register
  uint32        reg_otp_0;                              // (0x150) OTP read back, otp_apm[31:0]
#define   BCM6838_APM_OTP_TRIM                    0x00000FC0
#define   BCM6838_APM_OTP_TRIM_SHIFT                       6
  uint32        reg_otp_1;                              // (0x154) OTP read back, otp_apm[63:32]
  uint32        reg_diag_readback;                      // (0x158) diag bus read back
  uint32        audio_spare_2;                          // (0x15C) audio spare register 2
  uint32        dpll_control;                           // (0x160) DPLL control register
#define   BCM6838_DPLL_SOFT_INIT                  0x80000000
#define   BCM6838_DPLL_FORCE_ACQ                  0x40000000
#define   BCM6838_DPLL_OPEN_LOOP                  0x20000000
#define   BCM6838_DPLL_CONST_K                    0x10000000
#define   BCM6838_DPLL_PHS_THSHLD_SHIFT           4
  uint32        dpll_nom_freq;                          // (0x164) DPLL nominal frequency (control)
  uint32        dpll_div;                               // (0x168) DPLL divide register
#define   BCM6838_DPLL_REF_DIV_SHIFT              16
  uint32        dpll_acq_freq;                          // (0x16c) DPLL acquired frequency
  uint32        dpll_status;                            // (0x170) DPLL status register
#define   BCM6838_DPLL_IN_SYNC                    0x80000000
#define   BCM6838_DPLL_ACQ_FREQ_VALID             0x40000000
#define   BCM6838_DPLL_IN_SYNC                    0x80000000
#define   BCM6838_DPLL_K0_SHIFT                   8
} BCM6838ApmControlRegisters;

#define BCM6838_APM ((volatile BCM6838ApmControlRegisters * const) BCM6838_APM_BASE)

typedef struct BCM6838PcmControlRegisters
{
    uint32 pcm_ctrl;                            // 00 offset from PCM_BASE
#define  BCM6838_PCM_ENABLE              0x80000000     // PCM block master enable
#define  BCM6838_PCM_ENABLE_SHIFT        31
#define  BCM6838_PCM_SLAVE_SEL           0x40000000     // PCM TDM slave mode select (1 - TDM slave, 0 - TDM master)
#define  BCM6838_PCM_SLAVE_SEL_SHIFT     30
#define  BCM6838_PCM_CLOCK_INV           0x20000000     // PCM SCLK invert select (1 - invert, 0 - normal)
#define  BCM6838_PCM_CLOCK_INV_SHIFT     29
#define  BCM6838_PCM_FS_INVERT           0x10000000     // PCM FS invert select (1 - invert, 0 - normal)
#define  BCM6838_PCM_FS_INVERT_SHIFT     28
#define  BCM6838_PCM_FS_FREQ_16_8        0x08000000     // PCM FS 16/8 Khz select (1 - 16Khz, 0 - 8Khz)
#define  BCM6838_PCM_FS_FREQ_16_8_SHIFT  27
#define  BCM6838_PCM_FS_LONG             0x04000000     // PCM FS long/short select (1 - long FS, 0 - short FS)
#define  BCM6838_PCM_FS_LONG_SHIFT       26
#define  BCM6838_PCM_FS_TRIG             0x02000000     // PCM FS trigger (1 - falling edge, 0 - rising edge trigger)
#define  BCM6838_PCM_FS_TRIG_SHIFT       25
#define  BCM6838_PCM_DATA_OFF            0x01000000     // PCM data offset from FS (1 - one clock from FS, 0 - no offset)
#define  BCM6838_PCM_DATA_OFF_SHIFT      24
#define  BCM6838_PCM_DATA_16_8           0x00800000     // PCM data word length (1 - 16 bits, 0 - 8 bits)
#define  BCM6838_PCM_DATA_16_8_SHIFT     23
#define  BCM6838_PCM_CLOCK_SEL           0x00700000     // PCM SCLK freq select
#define  BCM6838_PCM_CLOCK_SEL_SHIFT     20
                                                  // 000 - 8192 Khz
                                                  // 001 - 4096 Khz
                                                  // 010 - 2048 Khz
                                                  // 011 - 1024 Khz
                                                  // 100 - 512 Khz
                                                  // 101 - 256 Khz
                                                  // 110 - 128 Khz
                                                  // 111 - reserved
#define  BCM6838_PCM_LSB_FIRST           0x00040000     // PCM shift direction (1 - LSBit first, 0 - MSBit first)
#define  BCM6838_PCM_LSB_FIRST_SHIFT     18
#define  BCM6838_PCM_LOOPBACK            0x00020000     // PCM diagnostic loobback enable
#define  BCM6838_PCM_LOOPBACK_SHIFT      17
#define  BCM6838_PCM_EXTCLK_SEL          0x00010000     // PCM external timing clock select -- Maybe removed in 6828
#define  BCM6838_PCM_EXTCLK_SEL_SHIFT    16
#define  BCM6838_PCM_NTR_ENABLE          0x00008000     // PCM NTR counter enable -- Nayve removed in 6828
#define  BCM6838_PCM_NTR_ENABLE_SHIFT    15
#define  BCM6838_PCM_BITS_PER_FRAME_1024 0x00000400     // 1024 - Max
#define  BCM6838_PCM_BITS_PER_FRAME_256  0x00000100     // 256
#define  BCM6838_PCM_BITS_PER_FRAME_8    0x00000008     // 8    - Min
#define  BCM6838_PCM_AP_SEL              0x00000001     // 1 - Connect pcm to pads, 0 - Connects audio test interface

    uint32 pcm_chan_ctrl;                       // 04
#define  BCM6838_PCM_TX0_EN              0x00000001     // PCM transmit channel 0 enable
#define  BCM6838_PCM_TX1_EN              0x00000002     // PCM transmit channel 1 enable
#define  BCM6838_PCM_TX2_EN              0x00000004     // PCM transmit channel 2 enable
#define  BCM6838_PCM_TX3_EN              0x00000008     // PCM transmit channel 3 enable
#define  BCM6838_PCM_TX4_EN              0x00000010     // PCM transmit channel 4 enable
#define  BCM6838_PCM_TX5_EN              0x00000020     // PCM transmit channel 5 enable
#define  BCM6838_PCM_TX6_EN              0x00000040     // PCM transmit channel 6 enable
#define  BCM6838_PCM_TX7_EN              0x00000080     // PCM transmit channel 7 enable
#define  BCM6838_PCM_RX0_EN              0x00000100     // PCM receive channel 0 enable
#define  BCM6838_PCM_RX1_EN              0x00000200     // PCM receive channel 1 enable
#define  BCM6838_PCM_RX2_EN              0x00000400     // PCM receive channel 2 enable
#define  BCM6838_PCM_RX3_EN              0x00000800     // PCM receive channel 3 enable
#define  BCM6838_PCM_RX4_EN              0x00001000     // PCM receive channel 4 enable
#define  BCM6838_PCM_RX5_EN              0x00002000     // PCM receive channel 5 enable
#define  BCM6838_PCM_RX6_EN              0x00004000     // PCM receive channel 6 enable
#define  BCM6838_PCM_RX7_EN              0x00008000     // PCM receive channel 7 enable
#define  BCM6838_PCM_RX_PACKET_SIZE      0x00ff0000     // PCM Rx DMA quasi-packet size
#define  BCM6838_PCM_RX_PACKET_SIZE_SHIFT  16

    uint32 pcm_int_pending;                     // 08
    uint32 pcm_int_mask;                        // 0c
#define  BCM6838_PCM_TX_UNDERFLOW        0x00000001     // PCM DMA receive overflow
#define  BCM6838_PCM_RX_OVERFLOW         0x00000002     // PCM DMA transmit underflow
#define  BCM6838_PCM_TDM_FRAME           0x00000004     // PCM frame boundary
#define  BCM6838_PCM_RX_IRQ              0x00000008     // IUDMA interrupts
#define  BCM6838_PCM_TX_IRQ              0x00000010

    uint32        reg_pcm_clk_cntl_0;             // (0x210) PCM Clock Control 0 (NCO_FCW_MISC)
    uint32        reg_pcm_clk_cntl_1;             // (0x214) PCM Clock Control 1 (NCO_SCALE)
    uint32        reg_pcm_clk_cntl_2;             // (0x218) PCM Clock Control 2
#define   BCM6838_PCM_NCO_SHIFT          0x0000000f
#define   BCM6838_PCM_NCO_MUX_CNTL       0x00000030
#define   BCM6838_PCM_NCO_LOAD_MISC      0x00000040
#define   BCM6838_PCM_NCO_SOFT_INIT      0x00000080

    uint32        pcm_apm_fcw_readback;            // FCW after scaling
    uint32        pcm_zds_intf;                    // ZSI Config register ( New in 6828 )
    uint32        pcm_msif_intf;                   // ISI Config register ( New in 6828 )

    uint32 unused[6];
#define  PCM_MAX_TIMESLOT_REGS   16             // Number of PCM time slot registers in the table.
                                                // Each register provides the settings for 8 timeslots (4 bits per timeslot)
    uint32 pcm_slot_alloc_tbl[PCM_MAX_TIMESLOT_REGS];
#define  PCM_TS_VALID            0x8            // valid marker for TS alloc ram entry
} BCM6838PcmControlRegisters;

#define BCM6838_PCM ((volatile BCM6838PcmControlRegisters * const) BCM6838_PCM_BASE)

typedef struct BCM6838HvgControlRegisters
{
   uint32 reg_hvg_vhvg_lores_cond;
   uint32 reg_hvg_vhvg_hires_cond;

   uint32 reg_hvg_vtip_lores_cond;
   uint32 reg_hvg_vtip_hires_cond;

   uint32 reg_hvg_vring_lores_cond;
   uint32 reg_hvg_vring_hires_cond;

   uint32 reg_hvg_vinput_cond;
   uint32 reg_hvg_cal_cond;
   uint32 reg_hvg_vbat_cond;
#define   BCM6838_HVG_COND_SHIFT                  0xf0000000
#define   BCM6838_HVG_COND_OFFSET                 0x0fff8000
#define   BCM6838_HVG_COND_SCALE                  0x00007fff
#define   BCM6838_HVG_COND_SHIFT_SHIFT                    28
#define   BCM6838_HVG_COND_OFFSET_SHIFT                   15
#define   BCM6838_HVG_COND_SCALE_SHIFT                     0

   uint32 reg_hvg_soft_init;
#define   BCM6838_HVG_SOFT_INIT                   0x80000000
#define   BCM6838_HVG_PWM_DISABLE                 0x40000000
#define   BCM6838_HVG_MAX_VOLTAGE                 0x00fff000
#define   BCM6838_RING_TRACK_MAX                  0x00000fff

   uint32 reg_hvg_duty_cycle;
#define   BCM6838_MAX_DUTY_CYCLE                  0x000000ff
#define   BCM6838_MIN_DUTY_CYCLE                  0x0000ff00
#define   BCM6838_MIN_ON_TIME                     0x00ff0000
#define   BCM6838_SHORT_CIRCUIT_THSHLD            0xff000000

   uint32 reg_hvg_ring_setup;
#define   BCM6838_RING_GAIN                       0x0000ffff
#define   BCM6838_RING_OVHD                       0x0fff0000
#define   BCM6838_HVG_COMMON_RING_REF             0x80000000
#define   BCM6838_RING_GAIN_SHIFT                          0
#define   BCM6838_RING_OVHD_SHIFT                         16
#define   BCM6838_HVG_COMMON_RING_REF_SHIFT               31

   uint32 reg_hvg_off_hook_setup;
#define   BCM6838_OFF_HOOK_OVHD                   0x000001ff
#define   BCM6838_OFF_HOOK_MIN                    0x0003fe00
#define   BCM6838_OFF_HOOK_MAX                    0xfff00000

   uint32 reg_hvg_circuit_parms;
#define   BCM6838_HVG_TURNS_RATIO                 0x00003ff0
#define   BCM6838_HVG_DIODE_DROP                  0x0000000f

   uint32 reg_hvg_fasd_multi_speed;
   uint32 reg_hvg_hvg_sar_timing;
   uint32 reg_hvg_bmu_sar_timing;
   uint32 reg_hvg_hi_sens_setup;
   uint32 reg_hvg_clk_ser_intrf;
   uint32 reg_hvg_zar_if_config;
   uint32 reg_hvg_zar_if_xmt_data;

   uint32 reserved1[15];               

   uint32 reg_hvg_cha_reg_0;
#define   BCM6838_HVG_WINDOW_SIZE                 0x0000000f
#define   BCM6838_TIP_WINDOW_SIZE                 0x000000f0
#define   BCM6838_RING_WINDOW_SIZE                0x00000f00
#define   BCM6838_INPUT_WINDOW_SIZE               0x0000f000
#define   BCM6838_TP_RG_WINDOW_SIZE               0x000f0000
#define   BCM6838_CAL_WINDOW_SIZE                 0x00f00000
#define   BCM6838_CAL_HI_LO_STATIC                0x01000000
#define   BCM6838_CAL_TOGGLE                      0x02000000
#define   BCM6838_VHG_HI_LO_AUTO                  0x04000000
#define   BCM6838_CONT_OR_BLOCK                   0x08000000
#define   BCM6838_SLEW_WINDOW_SIZES               0x10000000
#define   BCM6838_NEW_BLK_RQST                    0x20000000
#define   BCM6838_VHG_HI_SENS_MODE                0x40000000
#define   BCM6838_VTIP_VRING_HI_SENS_MODE         0x80000000

   uint16 reg_hvg_cha_max_hvg_slic;               // reg_hvg_cha_reg_1 - bits 31 to 16
#define   BCM6838_MAX_HVG_2_SLIC                  0x0fff0000
#define   BCM6838_HVG_GOAL_SLEW_TIME              0xf0000000

   uint16 reg_hvg_cha_const_volt_goal;            // reg_hvg_cha_reg_1 - bits 15 to 0
#define   BCM6838_CONST_VOLT_GOAL                 0x00000fff

   uint32 reg_hvg_cha_reg_2;
#define   BCM6838_K_PROP                          0x0000000f
#define   BCM6838_K_INTEG                         0x000000f0
#define   BCM6838_SER_TST_OUTPUT_SEL              0x00000700
#define   BCM6838_HVG_MODE                        0x00003000
#define   BCM6838_HVG_MODE_RING_TRACKING          0x00000000
#define   BCM6838_HVG_MODE_OFFHOOK_TRACKING       0x00001000
#define   BCM6838_HVG_MODE_ONHOOK_FIXED           0x00002000
#define   BCM6838_HVG_MODE_OFFHOOK_FASD           0x00003000
#define   BCM6838_HVG_SOFT_INIT_0                 0x00004000
#define   BCM6838_HVG_RR_SINGLE                   0x00008000

   uint32 reserved2[5];

   uint32 reg_hvg_chb_reg_0;

   uint16 reg_hvg_chb_max_hvg_slic;               // reg_hvg_chb_reg_1 - bits 31 to 16
#define   BCM6838_MAX_HVG_2_SLIC                  0x0fff0000
#define   BCM6838_HVG_GOAL_SLEW_TIME              0xf0000000

   uint16 reg_hvg_chb_const_volt_goal;            // reg_hvg_chb_reg_1 - bits 15 to 0
#define   BCM6838_CONST_VOLT_GOAL                 0x00000fff

   uint32 reg_hvg_chb_reg_2;

   uint32 reserved3[5];               

   uint32        reg_hvg_status0;
#define   BCM6838_HVG_OVERVOLTAGE_B               0x80000000
#define   BCM6838_HVG_OOB_B                       0x40000000
#define   BCM6838_HVG_INST_VALID_B                0x20000000
#define   BCM6838_HVG_INST_B                      0x1fff0000
#define   BCM6838_HVG_OVERVOLTAGE_A               0x00008000
#define   BCM6838_HVG_OOB_A                       0x00004000
#define   BCM6838_HVG_INST_VALID_A                0x00002000
#define   BCM6838_HVG_INST_A                      0x00001fff

   uint32        reg_hvg_status1;
#define   BCM6838_TIP_OOB_B                       0x40000000
#define   BCM6838_TIP_INST_VALID_B                0x20000000
#define   BCM6838_TIP_INST_B                      0x1fff0000
#define   BCM6838_TIP_OOB_A                       0x00004000
#define   BCM6838_TIP_INST_VALID_A                0x00002000
#define   BCM6838_TIP_INST_A                      0x00001fff

   uint32        reg_hvg_status2;
#define   BCM6838_RING_OOB_B                      0x40000000
#define   BCM6838_RING_INST_VALID_B               0x20000000
#define   BCM6838_RING_INST_B                     0x1fff0000
#define   BCM6838_RING_OOB_A                      0x00004000
#define   BCM6838_RING_INST_VALID_A               0x00002000
#define   BCM6838_RING_INST_A                     0x00001fff

   uint32        reg_hvg_status3;
#define   BCM6838_INPUT_OOB_B                     0x40000000
#define   BCM6838_INPUT_INST_VALID_B              0x20000000
#define   BCM6838_INPUT_INST_B                    0x1fff0000
#define   BCM6838_INPUT_OOB_A                     0x00004000
#define   BCM6838_INPUT_INST_VALID_A              0x00002000
#define   BCM6838_INPUT_INST_A                    0x00001fff

   uint32        reg_hvg_status4;               
#define   BCM6838_CAL_LO_OOB_B                       0x40000000
#define   BCM6838_CAL_LO_OOB_INST_VALID_B            0x20000000
#define   BCM6838_CAL_LO_INST_B                      0x1fff0000
#define   BCM6838_CAL_LO_OOB_A                       0x00004000
#define   BCM6838_CAL_LO_OOB_INST_VALID_A            0x00002000
#define   BCM6838_CAL_LO_INST_A                      0x00001fff

   uint32        reg_hvg_status5;               
#define   BCM6838_CAL_HI_OOB_B                       0x40000000
#define   BCM6838_CAL_HI_OOB_INST_VALID_B            0x20000000
#define   BCM6838_CAL_HI_INST_B                      0x1fff0000
#define   BCM6838_CAL_HI_OOB_A                       0x00004000
#define   BCM6838_CAL_HI_OOB_INST_VALID_A            0x00002000
#define   BCM6838_CAL_HI_INST_A                      0x00001fff

   uint32        reg_hvg_status6;               
#define   BCM6838_HVG_DUTY_CYCLE_B                0xffC00000
#define   BCM6838_HVG_DUTY_CYCLE_A                0x003ff000
#define   BCM6838_HVG_SHORT_FLAG_B                0x00000800
#define   BCM6838_HVG_SHORT_FLAG_A                0x00000400
#define   BCM6838_HVG_DUTY_CYCLE_LIMITED_B        0x00000200
#define   BCM6838_HVG_DUTY_CYCLE_LIMITED_A        0x00000100
#define   BCM6838_MAX_DIV_OOB_B                   0x00000080
#define   BCM6838_MAX_DIV_OOB_A                   0x00000040
#define   BCM6838_MAX_MULT_OOB_B                  0x00000020
#define   BCM6838_MAX_MULT_OOB_A                  0x00000010
#define   BCM6838_ARBITER_ERR_B                   0x00000008
#define   BCM6838_ARBITER_ERR_A                   0x00000004
#define   BCM6838_AVG_BLOCK_DONE_B                0x00000002
#define   BCM6838_AVG_BLOCK_DONE_A                0x00000001

   uint32        reg_hvg_status7;               
   uint32        reg_hvg_status8;               
#define   BCM6838_HVG_SLWEING_GOAL                0x40000000
#define   BCM6838_HVG_AVG_VALID                   0x20000000
#define   BCM6838_HVG_SAT                         0x10000000
#define   BCM6838_HVG_WINDOW_SLEW                 0x0f000000
#define   BCM6838_HVG_MEAN                        0x001fffff

   uint32        reg_hvg_status9;               
   uint32        reg_hvg_status10;              
#define   BCM6838_TIP_AVG_VALID                   0x20000000
#define   BCM6838_TIP_SAT                         0x10000000
#define   BCM6838_TIP_WINDOW_SLEW                 0x0f000000
#define   BCM6838_TIP_MEAN                        0x001fffff

   uint32        reg_hvg_status11;              
   uint32        reg_hvg_status12;              
#define   BCM6838_RING_AVG_VALID                  0x20000000
#define   BCM6838_RING_SAT                        0x10000000
#define   BCM6838_RING_WINDOW_SLEW                0x0f000000
#define   BCM6838_RING_MEAN                       0x001fffff

   uint32        reg_hvg_status13;              
   uint32        reg_hvg_status14;              
#define   BCM6838_TP_RG_AVG_VALID                 0x20000000
#define   BCM6838_TP_RG_SAT                       0x10000000
#define   BCM6838_TP_RG_WINDOW_SLEW               0x0f000000
#define   BCM6838_TP_RG_MEAN                      0x001fffff

   uint32        reg_hvg_status15;              
   uint32        reg_hvg_status16;              
#define   BCM6838_INPUT_AVG_VALID                 0x20000000
#define   BCM6838_INPUT_SAT                       0x10000000
#define   BCM6838_INPUT_WINDOW_SLEW               0x0f000000
#define   BCM6838_INPUT_MEAN                      0x001fffff

   uint32        reg_hvg_status17;              
   uint32        reg_hvg_status18;              
#define   BCM6838_CAL_LO_AVG_VALID                0x20000000
#define   BCM6838_CAL_LO_SAT                      0x10000000
#define   BCM6838_CAL_LO_WINDOW_SLEW              0x0f000000
#define   BCM6838_CAL_LO_MEAN                     0x001fffff

   uint32        reg_hvg_status19;           
   uint32        reg_hvg_status20;           
#define   BCM6838_CAL_HI_AVG_VALID                0x20000000
#define   BCM6838_CAL_HI_SAT                      0x10000000
#define   BCM6838_CAL_HI_WINDOW_SLEW              0x0f000000
#define   BCM6838_CAL_HI_MEAN                     0x001fffff

   uint32        reg_hvg_status21;              
   uint32        reg_hvg_status22;              
   uint32        reg_hvg_status23;              
   uint32        reg_hvg_status24;              
   uint32        reg_hvg_status25;              
   uint32        reg_hvg_status26;              
   uint32        reg_hvg_status27;              
   uint32        reg_hvg_status28;              
   uint32        reg_hvg_status29;              
   uint32        reg_hvg_status30;              
   uint32        reg_hvg_status31;              
   uint32        reg_hvg_status32;              
   uint32        reg_hvg_status33;              
   uint32        reg_hvg_status34;              
   uint32        reg_hvg_status35;              
   uint32        reg_hvg_status36;              

} BCM6838HvgControlRegisters;

#define BCM6838_HVG ((volatile BCM6838HvgControlRegisters * const) BCM6838_APM_HVG_BASE)

typedef struct BCM6838ApmPcmIudmaRegisters
{
   uint32 ctrlConfig;
#define IUDMA_REGS_CTRLCONFIG_MASTER_EN        0x0001
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH1_EN     0x0002
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH3_EN     0x0004
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH5_EN     0x0008
#define IUDMA_REGS_CTRLCONFIG_FLOWC_CH7_EN     0x0010

	// Flow control Ch1
   uint16 reserved1;
   uint16 ch1_FC_Low_Thr;

   uint16 reserved2;
   uint16 ch1_FC_High_Thr;

   uint16 reserved3;
   uint16 ch1_Buff_Alloc;

   // Flow control Ch3
   uint16 reserved4;
   uint16 ch3_FC_Low_Thr;

   uint16 reserved5;
   uint16 ch3_FC_High_Thr;

   uint16 reserved6;
   uint16 ch3_Buff_Alloc;

   // Flow control Ch5
   uint16 reserved7;
   uint16 ch5_FC_Low_Thr;

   uint16 reserved8;
   uint16 ch5_FC_High_Thr;

   uint16 reserved9;
   uint16 ch5_Buff_Alloc;

   // Flow control Ch7
   uint16 reserved10;
   uint16 ch7_FC_Low_Thr;

   uint16 reserved11;
   uint16 ch7_FC_High_Thr;

   uint16 reserved12;
   uint16 ch7_Buff_Alloc;

   uint16 reserved13;
   uint16 channelReset;

   uint16 reserved14;
   uint16 channelDebug;

   uint32 reserved15;

   uint32 gbl_int_stat;
   #define APM_IUDMA_IRQ_TX_PCM      0x00000020
   #define APM_IUDMA_IRQ_RX_PCM      0x00000010
   #define APM_IUDMA_IRQ_TX_B        0x00000008
   #define APM_IUDMA_IRQ_RX_B        0x00000004
   #define APM_IUDMA_IRQ_TX_A        0x00000002
   #define APM_IUDMA_IRQ_RX_A        0x00000001

   uint32 gbl_int_mask;
} BCM6838ApmPcmIudmaRegisters;

typedef struct BCM6838ApmPcmIudmaChannelCtrl
{
    uint16 reserved1;
    uint16 config;
#define BCM6838_IUDMA_CONFIG_ENDMA       0x0001
#define BCM6838_IUDMA_CONFIG_PKTHALT     0x0002
#define BCM6838_IUDMA_CONFIG_BURSTHALT   0x0004

    uint16 reserved2;
    uint16 intStat;
#define BCM6838_IUDMA_INTSTAT_BDONE   0x0001
#define BCM6838_IUDMA_INTSTAT_PDONE   0x0002
#define BCM6838_IUDMA_INTSTAT_NOTVLD  0x0004
#define BCM6838_IUDMA_INTSTAT_MASK    0x0007
#define BCM6838_IUDMA_INTSTAT_ALL     BCM6828_IUDMA_INTSTAT_MASK

    uint16 reserved3;
    uint16 intMask;
#define BCM6838_IUDMA_INTMASK_BDONE   0x0001
#define BCM6838_IUDMA_INTMASK_PDONE   0x0002
#define BCM6838_IUDMA_INTMASK_NOTVLD  0x0004

    uint32 maxBurst;
#define BCM6828_IUDMA_MAXBURST_SIZE 16 /* 32-bit words */

} BCM6838ApmPcmIudmaChannelCtrl;


typedef struct BCM6838ApmPcmIudmaStateRam
{
   uint32 baseDescPointer;                /* pointer to first buffer descriptor */

   uint32 stateBytesDoneRingOffset;       /* state info: how manu bytes done and the offset of the
                                             current descritor in process */
#define BCM6828_IUDMA_STRAM_DESC_RING_OFFSET 0x3fff


   uint32 flagsLengthStatus;              /* Length and status field of the current descriptor */

   uint32 currentBufferPointer;           /* pointer to the current descriptor */

} BCM6838ApmPcmIudmaStateRam;

#define BCM6828_MAX_APM_PCM_DMA_CHANNELS 6

typedef struct BCM6838ApmPcmIudma
{
   BCM6838ApmPcmIudmaRegisters regs;                                        //
   uint32 reserved1[110];                                             //
   BCM6838ApmPcmIudmaChannelCtrl ctrl[BCM6828_MAX_APM_PCM_DMA_CHANNELS];    //
   uint32 reserved2[104];                                             //
   BCM6838ApmPcmIudmaStateRam stram[BCM6828_MAX_APM_PCM_DMA_CHANNELS];      //

} BCM6838ApmPcmIudma;

#define BCM6838_APM_PCM_IUDMA ((volatile BCM6838ApmPcmIudma * const) BCM6838_APM_PCM_DMA_BASE)


#ifdef __cplusplus
}
#endif

#endif /* __MAP6838_H */

