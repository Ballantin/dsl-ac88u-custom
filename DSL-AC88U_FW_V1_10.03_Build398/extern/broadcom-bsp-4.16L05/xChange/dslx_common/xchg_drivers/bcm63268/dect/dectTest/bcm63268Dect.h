
#ifndef BCM63268DECT_H
#define BCM63268DECT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <xdrvTypes.h>

#define DECT_SHIM_CTRL_BASE         0xb000a000
#define DECT_SHIM_DMA_CTRL_BASE     0xb000a050
#define DECT_SHIM_TEST_BASE         0xb000a0f0
#define DECT_APB_REG_BASE           0xb000a800
#define DECT_AHB_SHARED_RAM_BASE    0xb06f0000
#define DECT_AHB_REG_BASE           0xb06f7f80
#define INT_CTRL_BASE               0xb0000000
#define GPIO_BLOCK_CTRL_BASE        0xb00000c0

#define PCM_CTRL_BASE               0xb000b000


typedef enum  DECT_SHM_ENABLE_BITS
{
   DECT_SHM_IRQ_DSP_INT,
   DECT_SHM_IRQ_DSP_IRQ_OUT,
   DECT_SHM_IRQ_DIP_INT,
   DECT_SHM_H2D_BUS_ERR,
   DECT_SHM_IRQ_TX_DMA_DONE,                 
   DECT_SHM_IRQ_RX_DMA_DONE,
   DECT_SHM_IRQ_PLL_PHASE_LOCK = DECT_SHM_IRQ_RX_DMA_DONE + 2, /* Skip reserved bit */
   DECT_SHM_REG_DSP_BREAK,
   DECT_SHM_REG_DIP_BREAK,
   DECT_SHM_REG_IRQ_TO_IP = DECT_SHM_REG_DIP_BREAK + 2, /* Skip reserved bit */
   DECT_SHM_TX_DMA_DONE_TO_IP,
   DECT_SHM_RX_DMA_DONE_TO_IP, 
} DECT_SHM_ENABLE_BITS;

typedef struct intControl 
{
	XDRV_UINT32 int_revid;	                        /*  0xb0000000  Revision ID Register                             */
	XDRV_UINT32 int_clkcontrol;	                  /*  0xb0000004  Clock Control Register                           */
#define  DECT_CLKEN                          0x00000040   
	XDRV_UINT32 int_timer_control;	               /*  0xb0000008  Timer Control Register                           */
	XDRV_UINT32 int_devicetimeouten;	            /*  0xb000000c  Device Timeout Enable Register                   */
	XDRV_UINT32 int_softresetb;	                  /*  0xb0000010  Soft Reset Register */
#define SOFT_RST_DECT           (1 << 15)                              
	XDRV_UINT32 int_diagcontrol;	                  /*  0xb0000014  Diagnostic and MBIST Control Register            */
	XDRV_UINT32 int_extirqcontrol;	               /*  0xb0000018  External Interrupt Configuration Register        */
	XDRV_UINT32 int_reserved0;	                  /*  0xb000001c  Reserved                                         */
	XDRV_UINT32 int_extrachipirqmask;	            /*  0xb0000020  MIPS Core 0 Extra Chip Interrupt Mask Register   */
	XDRV_UINT32 int_chipirqmask;	                  /*  0xb0000024  MIPS Core 0 Chip Interrupt Mask Register         */
	XDRV_UINT32 int_extrachipirqstatus;	         /*  0xb0000028  MIPS Core 0 Extra Chip Interrupt Status Register */
	XDRV_UINT32 int_chipirqstatus;	               /*  0xb000002c  MIPS Core 0 Chip Interrupt Status Register       */
	XDRV_UINT32 int_extrachipirqmask1;	            /*  0xb0000030  MIPS Core 1 Extra Chip Interrupt Mask Register   */
	XDRV_UINT32 int_chipirqmask1;	               /*  0xb0000034  MIPS Core 1 Chip Interrupt Mask Register         */
	XDRV_UINT32 int_extrachipirqstatus1;	         /*  0xb0000038  MIPS Core 1 Extra Chip Interrupt Status Register */
	XDRV_UINT32 int_chipirqstatus1;	               /*  0xb000003c  MIPS Core 1 Chip Interrupt Status Register       */
} intControl;

typedef struct gpioControl
{
   XDRV_UINT32 gpio_reserved0;              /* 0xb00000080 Reserved */
   XDRV_UINT32 gpio_dir;                    /* 0xb00000084 GPIO Low Pin Direction Control Register */
   XDRV_UINT32 gpio_reserved1;              /* 0xb00000088 Reserved */
   XDRV_UINT32 gpio_data;                   /* 0xb0000008c GPIO Low Pin Data register */
   XDRV_UINT32 gpio_led_ctrl;               /* 0xb00000090 Legacy LED Control Register */
   XDRV_UINT32 gpio_spi_config;             /* 0xb00000094 SPI Configuration Register */
   XDRV_UINT32 gpio_mode_ctrl;              /* 0xb00000098 GPIO Pin Mode Control Register */
   XDRV_UINT32 gpio_reserved2;              /* 0xb0000009c reserved */        
   XDRV_UINT32 gpio_reserved3;              /* 0xb000000a0 reserved */ 
   XDRV_UINT32 gpio_reserved4;              /* 0xb000000a4 reserved */ 
   XDRV_UINT32 gpio_test_control;           /* 0xb000000a8 Test Control register */  
   XDRV_UINT32 gpio_osc_control;            /* 0xb000000ac Osc Control */
   XDRV_UINT32 gpio_switch_ledcontrol;      /* 0xb000000b0 ROBOSW LED Control register */
   XDRV_UINT32 gpio_switch_led_lsr;         /* 0xb000000b4 ROBOSW LED Serial shift register */
   XDRV_UINT32 gpio_base_mode;              /* 0xb000000b8 base mode */
   XDRV_UINT32 gpio_ephy_ctl;               /* 0xb000000bc Ephy Control */
   XDRV_UINT32 gpio_switch_ctl;             /* 0xb000000c0 Robosw Control */
   XDRV_UINT32 gpio_regfile_tm_ctl;         /* 0xb000000c4 Regfile TM Control */ 
   XDRV_UINT32 gpio_ring_osc_ctl0;          /* 0xb000000c8 Ring Oscillator Control 0 register */
   XDRV_UINT32 gpio_ring_osc_ctl1;          /* 0xb000000cc Ring Oscillator Control 1 register */
   XDRV_UINT32 gpio_dis_tpout;              /* 0xb000000d0 GPIO Disable TPOUT */
   XDRV_UINT32 gpio_reserved7;              /* 0xb000000d4 Reserved */
   XDRV_UINT32 gpio_reserved8;              /* 0xb000000d8 Reserved */
   XDRV_UINT32 gpio_reserved9;              /* 0xb000000dc Reserved */
   XDRV_UINT32 gpio_reserved10;             /* 0xb000000e0 Reserved */
   XDRV_UINT32 gpio_reserved11;             /* 0xb000000e4 Reserved */
   XDRV_UINT32 gpio_dierevid;               /* 0xb000000e8 Die Revision Id Register */
   XDRV_UINT32 gpio_reserved12;             /* 0xb000000ec Reserved */
   XDRV_UINT32 gpio_diagselcontrol;         /* 0xb000000f0 Diagnostic Select Control Register for SPI only */
   XDRV_UINT32 gpio_diagreadback;           /* 0xb000000f4 Diagnostic Readback Register */
   XDRV_UINT32 gpio_diagreadbackhi;         /* 0xb000000f8 Diagnostic High Readback Register */
   XDRV_UINT32 gpio_diagmisccontrol;        /* 0xb000000fc Miscellaneous Control Register */
} gpioControl;

#define INT_CTRL ((volatile intControl * const) INT_CTRL_BASE)

#define GPIO_BLOCK_CTL ((volatile gpioControl * const) GPIO_BLOCK_CTRL_BASE)


typedef struct DECTShimControl 
{
   XDRV_UINT32 dect_shm_ctrl;                     /*  0xb000b000  DECT shim control registers                    */
#define APB_SWAP_MASK             0x0000C000
#define APB_SWAP_16_BIT           0x00000000
#define APB_SWAP_8_BIT            0x00004000
#define AHB_SWAP_MASK             0x00003000
#define AHB_SWAP_16_BIT           0x00003000
#define AHB_SWAP_8_BIT            0x00002000
#define AHB_SWAP_ACCESS           0x00001000
#define AHB_SWAP_NONE             0x00000000
#define DECT_PULSE_COUNT_ENABLE   0x00000200
#define PCM_PULSE_COUNT_ENABLE    0x00000100
#define DECT_SOFT_RESET           0x00000010
#define PHCNT_CLK_SRC_PLL         0x00000008
#define PHCNT_CLK_SRC_XTAL        0x00000000
#define DECT_CLK_OUT_PLL          0x00000004
#define DECT_CLK_OUT_XTAL         0x00000000
#define DECT_CLK_CORE_PCM         0x00000002
#define DECT_CLK_CORE_DECT        0x00000000
#define DECT_PLL_REF_PCM          0x00000001
#define DECT_PLL_REF_DECT         0x00000000

   XDRV_UINT32 dect_shm_pcm_clk_cntr;             /*  0xb000b004  PCM clock counter                              */
   XDRV_UINT32 dect_shm_dect_clk_cntr;            /*  0xb000b008  DECT clock counter                             */
   XDRV_UINT32 dect_shm_dect_clk_cntr_sh;         /*  0xb000b00c  DECT clock counter snapshot                    */
   XDRV_UINT32 dect_shm_irq_enable;               /*  0xb000b010  DECT interrupt enable register                 */
   XDRV_UINT32 dect_shm_irq_status;               /*  0xb000b014  DECT Interrupt status register                 */
   XDRV_UINT32 dect_shm_irq_trig;                 /*  0xb000b018  DECT DSP ext IRQ trigger and IRQ test register */
   XDRV_UINT32 dect_shm_dma_status;               /*  0xb000b01c  DECT DMA STATUS register                       */
   XDRV_UINT32 dect_shm_xtal_ctrl;                /*  0xb000b020  DECT analog tunable XTAL control register      */
   XDRV_UINT32 dect_shm_bandgap_ctrl;             /*  0xb000b024  DECT analog bandgap control register           */
   XDRV_UINT32 dect_shm_afe_tx_ctrl;              /*  0xb000b028  DECT analog TX DAC control register            */
   XDRV_UINT32 dect_shm_afe_test_ctrl;            /*  0xb000b02c  DECT analog test control register              */
   XDRV_UINT32 dect_shm_pll_reg_0;                /*  0xb000b030  DECT PLL configuration register 0              */
   XDRV_UINT32 dect_shm_pll_reg_1;                /*  0xb000b034  DECT PLL configuration register 1              */
#define  PLL_PWRDWN                          0x01000000   
#define  PLL_REFCOMP_PWRDOWN                 0x02000000
#define  PLL_NDIV_PWRDOWN                    0x04000000
#define  PLL_CH1_PWRDOWN                     0x08000000
#define  PLL_CH2_PWRDOWN                     0x10000000
#define  PLL_CH3_PWRDOWN                     0x20000000
#define  PLL_DRESET                          0x40000000
#define  PLL_ARESET                          0x80000000

   XDRV_UINT32 dect_shm_pll_reg_2;                /*  0xb000b038  DECT PLL Ndiv configuration register           */
   XDRV_UINT32 dect_shm_pll_reg_3;                /*  0xb000b03c  DECT PLL Pdiv and Mdiv configuration register  */
} DECTShimControl;

#define DECT_CTRL ((volatile DECTShimControl * const) DECT_SHIM_CTRL_BASE)

typedef struct DECTShimDmaControl 
{
   XDRV_UINT32 dect_shm_dma_ctrl;                 /*  0xb000b050  DECT DMA control register                      */
#define  DMA_CLEAR                           0x80000000
#define  DMA_SWAP_16_BIT                     0x03000000
#define  DMA_SWAP_8_BIT                      0x02000000
#define  DMA_SWAP_NONE                       0x01000000
#define  DMA_SUBWORD_SWAP_MASK               0x03000000
#define  TRIG_CNT_CLK_SEL_PCM                0x00800000
#define  TRIG_CNT_IRQ_EN                     0x00400000
#define  RX_CNT_TRIG_EN                      0x00200000   
#define  TX_CNT_TRIG_EN                      0x00100000 
#define  RX_INT_TRIG_EN                      0x00080000 	
#define  TX_INT_TRIG_EN                      0x00040000 	
#define  RX_REG_TRIG_EN                      0x00020000 	
#define  TX_REG_TRIG_EN                      0x00010000 
#define  RX_TRIG_FIRST                       0x00008000
#define  MAX_BURST_CYCLE_MASK                0x00001F00
#define  MAX_BURST_CYCLE_SHIFT               8
#define  RX_EN_3                             0x00000080   
#define  RX_EN_2                             0x00000040   
#define  RX_EN_1                             0x00000020   
#define  RX_EN_0                             0x00000010   
#define  TX_EN_3                             0x00000008   
#define  TX_EN_2                             0x00000004   
#define  TX_EN_1                             0x00000002   
#define  TX_EN_0                             0x00000001   
 
   XDRV_UINT32 dect_shm_dma_trig_cnt_preset;      /*  0xb000b054  DECT DMA trigger counter preset value                */
   XDRV_UINT32 dect_shm_dma_ddr_saddr_tx_s0;      /*  0xb000b058  DECT DMA DDR buffer starting address for TX slot 0   */
   XDRV_UINT32 dect_shm_dma_ddr_saddr_tx_s1;      /*  0xb000b05c  DECT DMA DDR buffer starting address for TX slot 1   */
   XDRV_UINT32 dect_shm_dma_ddr_saddr_tx_s2;      /*  0xb000b060  DECT DMA DDR buffer starting address for TX slot 2   */
   XDRV_UINT32 dect_shm_dma_ddr_saddr_tx_s3;      /*  0xb000b064  DECT DMA DDR buffer starting address for TX slot 3   */
   XDRV_UINT32 dect_shm_dma_ddr_saddr_rx_s0;      /*  0xb000b068  DECT DMA DDR buffer starting address for RX slot 0   */
   XDRV_UINT32 dect_shm_dma_ddr_saddr_rx_s1;      /*  0xb000b06c  DECT DMA DDR buffer starting address for RX slot 1   */
   XDRV_UINT32 dect_shm_dma_ddr_saddr_rx_s2;      /*  0xb000b070  DECT DMA DDR buffer starting address for RX slot 2   */
   XDRV_UINT32 dect_shm_dma_ddr_saddr_rx_s3;      /*  0xb000b074  DECT DMA DDR buffer starting address for RX slot 3   */
   XDRV_UINT32 dect_shm_dma_ahb_saddr_tx_s01;     /*  0xb000b078  DECT DMA AHB shared memory buffer starting address for TX slots 0 and 1  */
   XDRV_UINT32 dect_shm_dma_ahb_saddr_tx_s23;     /*  0xb000b07c  DECT DMA AHB shared memory buffer starting address for TX slots 2 and 3  */
   XDRV_UINT32 dect_shm_dma_ahb_saddr_rx_s01;     /*  0xb000b080  DECT DMA AHB shared memory buffer starting address for RX slots 0 and 1  */
   XDRV_UINT32 dect_shm_dma_ahb_saddr_rx_s23;     /*  0xb000b084  DECT DMA AHB shared memory buffer starting address for RX slots 2 and 3  */
   XDRV_UINT32 dect_shm_dma_xfer_size_tx;         /*  0xb000b088  DECT DMA TX slots transfer size of each trigger      */
   XDRV_UINT32 dect_shm_dma_xfer_size_rx;         /*  0xb000b08c  DECT DMA RX slots transfer size of each trigger      */
   XDRV_UINT32 dect_shm_dma_buf_size_tx;          /*  0xb000b090  DECT DMA TX slots memory buffer size                 */
   XDRV_UINT32 dect_shm_dma_buf_size_rx;          /*  0xb000b094  DECT DMA RX slots memory buffer size                 */
   XDRV_UINT32 dect_shm_dma_offset_addr_tx_s01;   /*  0xb000b098  DECT DMA access offset address for TX slots 0 and 1  */
   XDRV_UINT32 dect_shm_dma_offset_addr_tx_s23;   /*  0xb000b09c  DECT DMA access offset address for TX slots 2 and 3  */
   XDRV_UINT32 dect_shm_dma_offset_addr_rx_s01;   /*  0xb000b0a0  DECT DMA access offset address for RX slots 0 and 1  */
   XDRV_UINT32 dect_shm_dma_offset_addr_rx_s23;   /*  0xb000b0a4  DECT DMA access offset address for RX slots 2 and 3  */
   XDRV_UINT32 dect_shm_dma_xfer_cntr_tx;         /*  0xb000b0a8  DECT DMA transfer count per slot in number of DMA transfer size */
   XDRV_UINT32 dect_shm_dma_xfer_cntr_rx;         /*  0xb000b0a8  DECT DMA transfer count per slot in number of DMA transfer size */   
} DECTShimDmaControl;

#define DECT_DMA_CTRL ((volatile DECTShimDmaControl * const) DECT_SHIM_DMA_CTRL_BASE)


typedef struct ahbRegisters
{
   XDRV_UINT16 dsp_main_sync0;     /* 0xb0e57f80 DSP main counter outputs sel reg 0 */
   XDRV_UINT16 dsp_main_sync1;     /* 0xb0e57f82 DSP main counter outputs sel reg 1 */
   XDRV_UINT16 dsp_main_cnt;       /* 0xb0e57f84 DSP main counter reg */
   XDRV_UINT16 reserved1;          /* 0xb0e57f86 Reserved */
   XDRV_UINT16 reserved2;          /* 0xb0e57f88 Reserved */
   XDRV_UINT16 reserved3;          /* 0xb0e57f8a Reserved */
   XDRV_UINT16 reserved4;          /* 0xb0e57f8c Reserved */
   XDRV_UINT16 reserved5;          /* 0xb0e57f8e Reserved */
   XDRV_UINT16 reserved6;          /* 0xb0e57f90 Reserved */
   XDRV_UINT16 dsp_ram_out0;       /* 0xb0e57f92 DSP RAM output register 0 */
   XDRV_UINT16 dsp_ram_out1;       /* 0xb0e57f94 DSP RAM output register 1 */
   XDRV_UINT16 dsp_ram_out2;       /* 0xb0e57f96 DSP RAM output register 2 */
   XDRV_UINT16 dsp_ram_out3;       /* 0xb0e57f98 DSP RAM output register 3 */
   XDRV_UINT16 dsp_ram_in0;        /* 0xb0e57f9a DSP RAM input register 0 */
   XDRV_UINT16 dsp_ram_in1;        /* 0xb0e57f9c DSP RAM input register 1 */
   XDRV_UINT16 dsp_ram_in2;        /* 0xb0e57f9e DSP RAM input register 2 */
   XDRV_UINT16 dsp_ram_in3;        /* 0xb0e57fa0 DSP RAM input register 3 */
   XDRV_UINT16 dsp_zcross1_out;    /* 0xb0e57fa2 DSP RAM zero crossing 1 output reg */
   XDRV_UINT16 dsp_zcross2_out;    /* 0xb0e57fa4 DSP RAM zero crossing 2 output reg */
   XDRV_UINT16 reserved7;          /* 0xb0e57fa6 Reserved */
   XDRV_UINT16 reserved8;          /* 0xb0e57fa8 Reserved */
   XDRV_UINT16 reserved9;          /* 0xb0e57faa Reserved */
   XDRV_UINT16 reserved10;         /* 0xb0e57fac Reserved */
   XDRV_UINT16 reserved11;         /* 0xb0e57fae Reserved */
   XDRV_UINT16 reserved12;         /* 0xb0e57fb0 Reserved */
   XDRV_UINT16 reserved13;         /* 0xb0e57fb2 Reserved */
   XDRV_UINT16 reserved14;         /* 0xb0e57fb4 Reserved */
   XDRV_UINT16 reserved15;         /* 0xb0e57fb6 Reserved */
   XDRV_UINT16 reserved16;         /* 0xb0e57fb8 Reserved */
   XDRV_UINT16 reserved17;         /* 0xb0e57fba Reserved */
   XDRV_UINT16 dsp_main_ctrl;      /* 0xb0e57fbc DSP main counter control and preset reg */
   XDRV_UINT16 reserved18;         /* 0xb0e57fbe Reserved */
   XDRV_UINT16 reserved19;         /* 0xb0e57fc0 Reserved */
   XDRV_UINT16 reserved20;         /* 0xb0e57fc2 Reserved */
   XDRV_UINT16 reserved21;         /* 0xb0e57fc4 Reserved */
   XDRV_UINT16 reserved22;         /* 0xb0e57fc6 Reserved */
   XDRV_UINT16 reserved23;         /* 0xb0e57fc8 Reserved */
   XDRV_UINT16 reserved24;         /* 0xb0e57fca Reserved */
   XDRV_UINT16 reserved25;         /* 0xb0e57fce Reserved */
   XDRV_UINT16 dsp_ctrl;           /* 0xb0e57fd0 DSP control reg */
   XDRV_UINT16 dsp_pc;             /* 0xb0e57fd2 DSP program counter */
   XDRV_UINT16 dsp_pc_start;       /* 0xb0e57fd4 DSP program counter start */
   XDRV_UINT16 dsp_irq_start;      /* 0xb0e57fd6 DSP interrupt vector start */
   XDRV_UINT16 dsp_int;            /* 0xb0e57fd8 DSP to system bus interrupt vector */
   XDRV_UINT16 dsp_int_mask;       /* 0xb0e57fda DSP to system bus interrupt vector mask */
   XDRV_UINT16 dsp_int_prio1;      /* 0xb0e57fdc DSP interrupt mux 1 */
   XDRV_UINT16 dsp_int_prio2;      /* 0xb0e57fde DSP interrupt mux 2 */
   XDRV_UINT16 dsp_overflow;       /* 0xb0e57fe0 DSP to system bus interrupt overflow reg */
   XDRV_UINT16 dsp_jtbl_start;     /* 0xb0e57fe2 DSP jump table start address */
   XDRV_UINT16 reserved26;         /* 0xb0e57fe4 Reserved */
   XDRV_UINT16 reserved27;         /* 0xb0e57fe6 Reserved */
   XDRV_UINT16 reserved28;         /* 0xb0e57fe8 Reserved */
   XDRV_UINT16 reserved29;         /* 0xb0e57fea Reserved */
   XDRV_UINT16 reserved30;         /* 0xb0e57fec Reserved */
   XDRV_UINT16 reserved31;         /* 0xb0e57fee Reserved */
   XDRV_UINT16 dsp_debug_inst;     /* 0xb0e57ff0 DSP debug instruction register */
   XDRV_UINT16 reserved32;         /* 0xb0e57ff2 Reserved */
   XDRV_UINT16 dsp_debug_inout_l;  /* 0xb0e57ff4 DSP debug data (LSW) */
   XDRV_UINT16 dsp_debug_inout_h;  /* 0xb0e57ff6 DSP debug data (MSW) */
   XDRV_UINT16 reserved33;         /* 0xb0e57ff8 Reserved */
   XDRV_UINT16 reserved34;         /* 0xb0e57ffa Reserved */
   XDRV_UINT16 reserved35;         /* 0xb0e57ffc Reserved */
   XDRV_UINT16 reserved36;         /* 0xb0e57ffe Reserved */
} ahbRegisters;

#define AHB_REGISTERS ((volatile ahbRegisters * const) DECT_AHB_REG_BASE)


typedef struct pcmControl
{
    XDRV_UINT32 pcm_ctrl;                            // 00 offset from PCM_BASE
#define  PCM_ENABLE              0x80000000     // PCM block master enable
#define  PCM_ENABLE_SHIFT        31
#define  PCM_SLAVE_SEL           0x40000000     // PCM TDM slave mode select (1 - TDM slave, 0 - TDM master)
#define  PCM_SLAVE_SEL_SHIFT     30
#define  PCM_CLOCK_INV           0x20000000     // PCM SCLK invert select (1 - invert, 0 - normal)
#define  PCM_CLOCK_INV_SHIFT     29
#define  PCM_FS_INVERT           0x10000000     // PCM FS invert select (1 - invert, 0 - normal)
#define  PCM_FS_INVERT_SHIFT     28
#define  PCM_FS_FREQ_16_8        0x08000000     // PCM FS 16/8 Khz select (1 - 16Khz, 0 - 8Khz)
#define  PCM_FS_FREQ_16_8_SHIFT  27
#define  PCM_FS_LONG             0x04000000     // PCM FS long/short select (1 - long FS, 0 - short FS)
#define  PCM_FS_LONG_SHIFT       26
#define  PCM_FS_TRIG             0x02000000     // PCM FS trigger (1 - falling edge, 0 - rising edge trigger)
#define  PCM_FS_TRIG_SHIFT       25
#define  PCM_DATA_OFF            0x01000000     // PCM data offset from FS (1 - one clock from FS, 0 - no offset)
#define  PCM_DATA_OFF_SHIFT      24
#define  PCM_DATA_16_8           0x00800000     // PCM data word length (1 - 16 bits, 0 - 8 bits)
#define  PCM_DATA_16_8_SHIFT     23
#define  PCM_CLOCK_SEL           0x00700000     // PCM SCLK freq select
#define  PCM_CLOCK_SEL_SHIFT     20
                                                  // 000 - 8192 Khz
                                                  // 001 - 4096 Khz
                                                  // 010 - 2048 Khz
                                                  // 011 - 1024 Khz
                                                  // 100 - 512 Khz
                                                  // 101 - 256 Khz
                                                  // 110 - 128 Khz
                                                  // 111 - reserved
#define  PCM_LSB_FIRST           0x00040000     // PCM shift direction (1 - LSBit first, 0 - MSBit first)
#define  PCM_LSB_FIRST_SHIFT     18
#define  PCM_LOOPBACK            0x00020000     // PCM diagnostic loobback enable
#define  PCM_LOOPBACK_SHIFT      17
#define  PCM_EXTCLK_SEL          0x00010000     // PCM external timing clock select 
#define  PCM_EXTCLK_SEL_SHIFT    16
#define  PCM_NTR_ENABLE          0x00008000     // PCM NTR counter enable
#define  PCM_NTR_ENABLE_SHIFT    15
#define  PCM_BITS_PER_FRAME_1024 0x00000400     // 1024 - Max
#define  PCM_BITS_PER_FRAME_256  0x00000100     // 256
#define  PCM_BITS_PER_FRAME_8    0x00000008     // 8    - Min

    XDRV_UINT32 pcm_chan_ctrl;                       // 04
#define  PCM_TX0_EN              0x00000001     // PCM transmit channel 0 enable
#define  PCM_TX1_EN              0x00000002     // PCM transmit channel 1 enable
#define  PCM_TX2_EN              0x00000004     // PCM transmit channel 2 enable
#define  PCM_TX3_EN              0x00000008     // PCM transmit channel 3 enable
#define  PCM_TX4_EN              0x00000010     // PCM transmit channel 4 enable
#define  PCM_TX5_EN              0x00000020     // PCM transmit channel 5 enable
#define  PCM_TX6_EN              0x00000040     // PCM transmit channel 6 enable
#define  PCM_TX7_EN              0x00000080     // PCM transmit channel 7 enable
#define  PCM_RX0_EN              0x00000100     // PCM receive channel 0 enable
#define  PCM_RX1_EN              0x00000200     // PCM receive channel 1 enable
#define  PCM_RX2_EN              0x00000400     // PCM receive channel 2 enable
#define  PCM_RX3_EN              0x00000800     // PCM receive channel 3 enable
#define  PCM_RX4_EN              0x00001000     // PCM receive channel 4 enable
#define  PCM_RX5_EN              0x00002000     // PCM receive channel 5 enable
#define  PCM_RX6_EN              0x00004000     // PCM receive channel 6 enable
#define  PCM_RX7_EN              0x00008000     // PCM receive channel 7 enable
#define  PCM_RX_PACKET_SIZE      0x00ff0000     // PCM Rx DMA quasi-packet size
#define  PCM_RX_PACKET_SIZE_SHIFT  16

    XDRV_UINT32 pcm_int_pending;                     // 08
    XDRV_UINT32 pcm_int_mask;                        // 0c
#define  PCM_TX_UNDERFLOW        0x00000001     // PCM DMA receive overflow
#define  PCM_RX_OVERFLOW         0x00000002     // PCM DMA transmit underflow
#define  PCM_TDM_FRAME           0x00000004     // PCM frame boundary
#define  PCM_RX_IRQ              0x00000008     // IUDMA interrupts
#define  PCM_TX_IRQ              0x00000010

    XDRV_UINT32 pcm_pll_ctrl1;                       // 10
#define  PCM_PLL_PWRDN           0x80000000     // PLL PWRDN
#define  PCM_PLL_PWRDN_CH1       0x40000000     // PLL CH PWRDN
#define  PCM_PLL_REFCMP_PWRDN    0x20000000     // PLL REFCMP PWRDN
#define  PCM_CLK16_RESET         0x10000000     // 16.382 MHz PCM interface circuitry reset. 
#define  PCM_PLL_ARESET          0x08000000     // PLL Analog Reset
#define  PCM_PLL_DRESET          0x04000000     // PLL Digital Reset

    XDRV_UINT32 pcm_pll_ctrl2;                       // 14
    XDRV_UINT32 pcm_pll_ctrl3;                       // 18
    XDRV_UINT32 pcm_pll_ctrl4;                       // 1c

    XDRV_UINT32 pcm_pll_stat;                        // 20
#define  PCM_PLL_LOCK            0x00000001     // Asserted when PLL is locked to programmed frequency

    XDRV_UINT32 pcm_ntr_counter;                     // 24

    XDRV_UINT32 unused[7];
#define  PCM_MAX_TIMESLOT_REGS   16             // Number of PCM time slot registers in the table.
                                                // Each register provides the settings for 8 timeslots (4 bits per timeslot)
    XDRV_UINT32 pcm_slot_alloc_tbl[PCM_MAX_TIMESLOT_REGS];
#define  PCM_TS_VALID            0x8            // valid marker for TS alloc ram entry
} pcmControl;

#define PCM_CTRL ((volatile pcmControl * const) PCM_CTRL_BASE)

#ifdef __cplusplus
}
#endif

#endif   /* BCM63268DECT_H */
