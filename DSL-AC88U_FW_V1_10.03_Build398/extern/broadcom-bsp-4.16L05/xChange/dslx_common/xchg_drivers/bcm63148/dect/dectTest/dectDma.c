/* Broadcom Corp. */

/* 
** Always include this file before any other include files
** to avoid conflicts with remapped printf command 
*/
#include <kernelMainWrapper.h>
#include <linux/dma-mapping.h>

#include <xdrvTypes.h>
#define IO_ADDRESS(x)           (((x) & 0x00ffffff) + 0xfc000000)
#include "63148_map_part.h"



#define XFER_SIZE_8_BYTES    0  // INVALID TEST SIZE
#define XFER_SIZE_16_BYTES   0
#define XFER_SIZE_20_BYTES   0
#define XFER_SIZE_40_BYTES   0
#define XFER_SIZE_80_BYTES   0
#define XFER_SIZE_128_BYTES  1  
#define XFER_SIZE_320_BYTES  0

#define DECT_DMA_NUM_CHAN    1         /* Number of DECT slots */

#define REG_TRIGGER_TEST                 0    //
#define CNT_DECT_CLK_TRIGGER_TEST        1    // 
#define CNT_PCM_CLK_TRIGGER_TEST         0    //
#define CNT_DSP_IRQ_DECT_TRIGGER_TEST    0    //
#define CNT_DSP_IRQ_PCM_TRIGGER_TEST     0    //
#define ZCROSS_TRIGGER_TEST              0    //



#define ALIGN_DATA(addr, boundary)       ((addr + boundary - 1) & ~(boundary - 1))
#define DMA_UBUS_BYTES           8

#if XFER_SIZE_8_BYTES
#define DECT_DMA_TRANSFER_SIZE_BYTES       8                                       /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_TX_TRANSFER_SIZE_BYTES    8                                       /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_BUFFER_SIZE_BYTES         (80 * DECT_DMA_TRANSFER_SIZE_BYTES )    /* Size for single direction, one slot, double buffered  */
#define DECT_DMA_TX_BUFFER_SIZE_BYTES      (80 * DECT_DMA_TX_TRANSFER_SIZE_BYTES ) /* Size for single direction, one slot, double buffered  */
#endif 

#if XFER_SIZE_16_BYTES
#define DECT_DMA_TRANSFER_SIZE_BYTES       16                                       /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_TX_TRANSFER_SIZE_BYTES    16                                       /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_BUFFER_SIZE_BYTES         (40 * DECT_DMA_TRANSFER_SIZE_BYTES )    /* Size for single direction, one slot, double buffered  */
#define DECT_DMA_TX_BUFFER_SIZE_BYTES      (40 * DECT_DMA_TX_TRANSFER_SIZE_BYTES ) /* Size for single direction, one slot, double buffered  */
#endif
 
#if XFER_SIZE_20_BYTES
#define DECT_DMA_TRANSFER_SIZE_BYTES       20                                       /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_TX_TRANSFER_SIZE_BYTES    20                                       /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_BUFFER_SIZE_BYTES         (32 * DECT_DMA_TRANSFER_SIZE_BYTES )    /* Size for single direction, one slot, double buffered  */
#define DECT_DMA_TX_BUFFER_SIZE_BYTES      (32 * DECT_DMA_TX_TRANSFER_SIZE_BYTES ) /* Size for single direction, one slot, double buffered  */
#endif

#if XFER_SIZE_40_BYTES
#define DECT_DMA_TRANSFER_SIZE_BYTES       40                                       /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_TX_TRANSFER_SIZE_BYTES    40                                       /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_BUFFER_SIZE_BYTES         (16 * DECT_DMA_TRANSFER_SIZE_BYTES )      /* Size for single direction, one slot, double buffered  */
#define DECT_DMA_TX_BUFFER_SIZE_BYTES      (16 * DECT_DMA_TX_TRANSFER_SIZE_BYTES )   /* Size for single direction, one slot, double buffered  */
#endif

#if XFER_SIZE_80_BYTES
#define DECT_DMA_TRANSFER_SIZE_BYTES       80                                       /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_TX_TRANSFER_SIZE_BYTES    80                                       /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_BUFFER_SIZE_BYTES         (8 * DECT_DMA_TRANSFER_SIZE_BYTES )      /* Size for single direction, one slot, double buffered  */
#define DECT_DMA_TX_BUFFER_SIZE_BYTES      (8 * DECT_DMA_TX_TRANSFER_SIZE_BYTES )   /* Size for single direction, one slot, double buffered  */
#endif

#if XFER_SIZE_128_BYTES
#define DECT_DMA_TRANSFER_SIZE_BYTES       128                                      /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_TX_TRANSFER_SIZE_BYTES    128                                      /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_BUFFER_SIZE_BYTES         (5 * DECT_DMA_TRANSFER_SIZE_BYTES )      /* Size for single direction, one slot, double buffered  */
#define DECT_DMA_TX_BUFFER_SIZE_BYTES      (5 * DECT_DMA_TX_TRANSFER_SIZE_BYTES )   /* Size for single direction, one slot, double buffered  */
#endif

#if XFER_SIZE_320_BYTES
#define DECT_DMA_TRANSFER_SIZE_BYTES       320                                      /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_TX_TRANSFER_SIZE_BYTES    320                                      /* 10ms tick, 80 Wideband-16bit samples                  */
#define DECT_DMA_BUFFER_SIZE_BYTES         (2 * DECT_DMA_TRANSFER_SIZE_BYTES )      /* Size for single direction, one slot, double buffered  */
#define DECT_DMA_TX_BUFFER_SIZE_BYTES      (2 * DECT_DMA_TX_TRANSFER_SIZE_BYTES )   /* Size for single direction, one slot, double buffered  */
#endif

#define DECT_DMA_TRANSFER_SIZE_WORDS   (DECT_DMA_TRANSFER_SIZE_BYTES/4)    /* In 32-bit words                                       */
#define DECT_DMA_BUFFER_SIZE_WORDS     (DECT_DMA_BUFFER_SIZE_BYTES/4)      /* In 32-bit words                                       */

#define DECT_DMA_TX_TRANSFER_SIZE_WORDS   (DECT_DMA_TX_TRANSFER_SIZE_BYTES/4)    /* In 32-bit words                                       */
#define DECT_DMA_TX_BUFFER_SIZE_WORDS     (DECT_DMA_TX_BUFFER_SIZE_BYTES/4)      /* In 32-bit words                                       */

#define DECT_DMA_ALIGNMENT_OVERHEAD    DMA_UBUS_BYTES              /* DMA buffers need to be 64-bit aligned                 */

/* DECT DDR DMA Buffers */
//XDRV_UINT32 dectTxDMABuff[ (DECT_DMA_NUM_CHAN * DECT_DMA_BUFFER_SIZE_BYTES) + (DECT_DMA_ALIGNMENT_OVERHEAD/4) ];
//XDRV_UINT32 dectRxDMABuff[ (DECT_DMA_NUM_CHAN * DECT_DMA_BUFFER_SIZE_BYTES) + (DECT_DMA_ALIGNMENT_OVERHEAD/4) ];

/* Allocate DMA Buffers in Linux OS */
typedef dma_addr_t   DMA_HANDLE;
DMA_HANDLE           dmaBufHandleTx, dmaBufHandleRx;


unsigned short rxBufferArea = sizeof(unsigned long)*(DECT_DMA_NUM_CHAN * DECT_DMA_BUFFER_SIZE_BYTES/4) + (DECT_DMA_ALIGNMENT_OVERHEAD/4);
unsigned short txBufferArea = sizeof(unsigned long)*(DECT_DMA_NUM_CHAN * DECT_DMA_BUFFER_SIZE_BYTES/4) + (DECT_DMA_ALIGNMENT_OVERHEAD/4);

volatile XDRV_UINT32 *dectTxDMABuff; 
volatile XDRV_UINT32 *dectRxDMABuff;


/* Pointer to DMA buffer start addresses */
volatile XDRV_UINT32 * pDectTxDmaDDRStartAddr[DECT_DMA_NUM_CHAN];
volatile XDRV_UINT32 * pDectRxDmaDDRStartAddr[DECT_DMA_NUM_CHAN];

volatile XDRV_UINT32 * pDectTxDmaDDRStartAddrV[DECT_DMA_NUM_CHAN];
volatile XDRV_UINT32 * pDectRxDmaDDRStartAddrV[DECT_DMA_NUM_CHAN];


volatile unsigned long * pDectTxDmaAHBStartAddr[DECT_DMA_NUM_CHAN] = { (unsigned long *) DECT_AHB_CHAN0_RX }; 

volatile unsigned long * pDectRxDmaAHBStartAddr[DECT_DMA_NUM_CHAN] = { (unsigned long *) DECT_AHB_CHAN0_TX }; 



/* Function prototypes */
void dectDMATestStart ( void );
void dectDMAConfig( void );
void dectDMATest( void );   
void dectDMAahbTest(void);
void dectDMAapbTest(void);
void ddrTest(void);
void dumpRegs( void );
void dumpDataMemBytes( void );
void dumpDataMemWords( void );
XDRV_UINT32 timerGetCountMips( void );

extern void gprint(const char *fmt, unsigned long arg0, unsigned long arg1, unsigned long arg2);
extern int dectip_aux( void );
extern unsigned launchTp1( void );
/* Source Code */

//#define rdtscl(dest) __asm__ __volatile__("mfc0 %0,$9; nop" : "=r" (dest)) 
#define read_counters(x)    asm volatile("mrc p15, 0, %0, c9, c13, 0; isb" : "=r" (x))

XDRV_UINT32 timerGetCountMips( void )
{
   XDRV_UINT32 localCount=0; 

   //rdtscl(localCount);           /* obtain local count */
   read_counters(localCount);
	
   return localCount;
}


void dectDMATestStart( void )
{

   dectRxDMABuff = dma_alloc_coherent( NULL, rxBufferArea, &dmaBufHandleRx, GFP_KERNEL );
   gprint("RxDMABuffv: 0x%08x, RxDMABuffp: 0x%08x\n", (XDRV_UINT32)dectRxDMABuff, (unsigned int)dmaBufHandleRx, 0);
   dectTxDMABuff = dma_alloc_coherent( NULL, txBufferArea, &dmaBufHandleTx, GFP_KERNEL );
   gprint("TxDMABuff: 0x%08x, TxDMABuffp: 0x%08x\n", (XDRV_UINT32)dectTxDMABuff, (unsigned int)dmaBufHandleTx, 0);
      
   /* Setup DMA buffers */
   dectDMAConfig();

   gprint("Before test: MIPS count 0x%08x, PCM pulse count 0x%08x, DECT pulse count 0x%08x\n", 
           timerGetCountMips(), DECT_CTRL->dect_shm_pcm_clk_cntr, DECT_CTRL->dect_shm_dect_clk_cntr_sh);
   
   /* Dump Regs */
   //dumpRegs();
   
   /* Start test */
   //launchTp1();
   dectDMATest();   
}
   
           
void dectDMAConfig()
{
   unsigned int i,j;
   
   for ( i=0; i < DECT_DMA_NUM_CHAN; i++ )
   {
      /* Align DMA buffers along 64-bit boundaries */
      pDectTxDmaDDRStartAddr[i] = (volatile unsigned long *) ( (unsigned long) dmaBufHandleTx + i * DECT_DMA_BUFFER_SIZE_WORDS );
      pDectRxDmaDDRStartAddr[i] = (volatile unsigned long *) ( (unsigned long) dmaBufHandleRx + i * DECT_DMA_BUFFER_SIZE_WORDS ); 
      pDectTxDmaDDRStartAddrV[i] = (volatile unsigned long *) ( (unsigned long) dectRxDMABuff + i * DECT_DMA_BUFFER_SIZE_WORDS );
      pDectRxDmaDDRStartAddrV[i] = (volatile unsigned long *) ( (unsigned long) dectTxDMABuff + i * DECT_DMA_BUFFER_SIZE_WORDS );                  
   }

   for ( i=0; i < DECT_DMA_NUM_CHAN; i++ )
   {
      gprint("########### pDectTxDmaDDRStartAddr[%d] 0x%08x ###########\n", i, (XDRV_UINT32)pDectTxDmaDDRStartAddr[i], 0);
   }
   for ( i=0; i < DECT_DMA_NUM_CHAN; i++ )
   {
      gprint("########### pDectRxDmaDDRStartAddr[%d] 0x%08x ###########\n", i, (XDRV_UINT32)pDectRxDmaDDRStartAddr[i], 0);
   }
   for ( i=0; i < DECT_DMA_NUM_CHAN; i++ )
   {
      gprint("########### pDectTxDmaAHBStartAddr[%d] 0x%08x ###########\n", i, (XDRV_UINT32)pDectTxDmaAHBStartAddr[i], 0);
   }
   for ( i=0; i < DECT_DMA_NUM_CHAN; i++ )
   {
      gprint("########### pDectRxDmaAHBStartAddr[%d] 0x%08x ###########\n\n", i, (XDRV_UINT32)pDectRxDmaAHBStartAddr[i], 0);
   }
   
   /* Setup DDR Start addresses -- Must convert to Physical addresses*/                  
   DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s0 = ((unsigned int)pDectTxDmaDDRStartAddr[0] );
   DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s0 = ((unsigned int)pDectRxDmaDDRStartAddr[0] );
#if (DECT_DMA_NUM_CHAN > 1)
   DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s1 = ((unsigned int)pDectTxDmaDDRStartAddr[1] );
   DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s1 = ((unsigned int)pDectRxDmaDDRStartAddr[1] );
#if (DECT_DMA_NUM_CHAN > 2)
   DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s2 = ((unsigned int)pDectTxDmaDDRStartAddr[2] );
   DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s2 = ((unsigned int)pDectRxDmaDDRStartAddr[2] );
#if (DECT_DMA_NUM_CHAN > 3)
   DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s3 = ((unsigned int)pDectTxDmaDDRStartAddr[3] );
   DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s3 = ((unsigned int)pDectRxDmaDDRStartAddr[3] );
#endif
#endif
#endif

   /* Setup AHB Shared mem start addresses -- These must be stored as offsets from DECT_AHB_SHARED_RAM_BASE */        
   DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_tx_s01  = ((unsigned int)pDectTxDmaAHBStartAddr[0] - (unsigned int)DECT_AHB_REG_PHYS_BASE) << 16;  /* Slot 0 */
   DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_rx_s01  = ((unsigned int)pDectRxDmaAHBStartAddr[0] - (unsigned int)DECT_AHB_REG_PHYS_BASE) << 16;  /* Slot 0 */
#if (DECT_DMA_NUM_CHAN > 1)
   DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_tx_s01 |= ((unsigned int)pDectTxDmaAHBStartAddr[1] - (unsigned int)DECT_AHB_REG_PHYS_BASE);         /* Slot 1 */
   DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_rx_s01 |= ((unsigned int)pDectRxDmaAHBStartAddr[1] - (unsigned int)DECT_AHB_REG_PHYS_BASE);         /* Slot 1 */
#if (DECT_DMA_NUM_CHAN > 2)
   DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_tx_s23  = ((unsigned int)pDectTxDmaAHBStartAddr[2] - (unsigned int)DECT_AHB_REG_PHYS_BASE) << 16;  /* Slot 2 */
   DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_rx_s23  = ((unsigned int)pDectRxDmaAHBStartAddr[2] - (unsigned int)DECT_AHB_REG_PHYS_BASE) << 16;  /* Slot 2 */
#if (DECT_DMA_NUM_CHAN > 3)
   DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_tx_s23 |= ((unsigned int)pDectTxDmaAHBStartAddr[3] - (unsigned int)DECT_AHB_REG_PHYS_BASE);         /* Slot 3 */
   DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_rx_s23 |= ((unsigned int)pDectRxDmaAHBStartAddr[3] - (unsigned int)DECT_AHB_REG_PHYS_BASE);         /* Slot 3 */
#endif
#endif
#endif


   /* Setup DMA transfer size in terms of 32-bit words*/
   DECT_DMA_CTRL->dect_shm_dma_xfer_size_tx = (   (DECT_DMA_TX_TRANSFER_SIZE_WORDS) << 24     /* Slot0 */
                                                | (DECT_DMA_TX_TRANSFER_SIZE_WORDS) << 16     /* Slot1 */
                                                | (DECT_DMA_TX_TRANSFER_SIZE_WORDS) << 8      /* Slot2 */
                                                | (DECT_DMA_TX_TRANSFER_SIZE_WORDS) << 0 );   /* Slot3 */

   DECT_DMA_CTRL->dect_shm_dma_xfer_size_rx = (   (DECT_DMA_TRANSFER_SIZE_WORDS) << 24        /* Slot0 */
                                                | (DECT_DMA_TRANSFER_SIZE_WORDS) << 16        /* Slot1 */
                                                | (DECT_DMA_TRANSFER_SIZE_WORDS) << 8         /* Slot2 */
                                                | (DECT_DMA_TRANSFER_SIZE_WORDS) << 0 );      /* Slot3 */


   /* Setup total size of dma buff per slot in terms of DMA transfer size */
   DECT_DMA_CTRL->dect_shm_dma_buf_size_tx = (    ( DECT_DMA_TX_BUFFER_SIZE_WORDS / DECT_DMA_TX_TRANSFER_SIZE_WORDS ) << 24    /* Slot0 */
                                                | ( DECT_DMA_TX_BUFFER_SIZE_WORDS / DECT_DMA_TX_TRANSFER_SIZE_WORDS ) << 16    /* Slot1 */
                                                | ( DECT_DMA_TX_BUFFER_SIZE_WORDS / DECT_DMA_TX_TRANSFER_SIZE_WORDS ) << 8     /* Slot2 */
                                                | ( DECT_DMA_TX_BUFFER_SIZE_WORDS / DECT_DMA_TX_TRANSFER_SIZE_WORDS ) << 0 );  /* Slot3 */

   DECT_DMA_CTRL->dect_shm_dma_buf_size_rx = (    ( DECT_DMA_BUFFER_SIZE_WORDS / DECT_DMA_TRANSFER_SIZE_WORDS ) << 24      /* Slot0 */
                                                | ( DECT_DMA_BUFFER_SIZE_WORDS / DECT_DMA_TRANSFER_SIZE_WORDS ) << 16      /* Slot1 */
                                                | ( DECT_DMA_BUFFER_SIZE_WORDS / DECT_DMA_TRANSFER_SIZE_WORDS ) << 8       /* Slot2 */
                                                | ( DECT_DMA_BUFFER_SIZE_WORDS / DECT_DMA_TRANSFER_SIZE_WORDS ) << 0 );    /* Slot3 */
 
   
   /* Setup DMA burst size */    
   DECT_DMA_CTRL->dect_shm_dma_ctrl &= ~MAX_BURST_CYCLE_MASK;
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( 8 << MAX_BURST_CYCLE_SHIFT );

   DECT_DMA_CTRL->dect_shm_dma_ctrl |= DMA_RX_TRIG_FIRST;
   
   
   /* Use SWAP based access to keep things simple */
   DECT_CTRL->dect_shm_ctrl &= ~AHB_SWAP_MASK;  
   DECT_CTRL->dect_shm_ctrl |= AHB_SWAP_ACCESS;
   /* Use 16-bit swap for DMA */  
   DECT_DMA_CTRL->dect_shm_dma_ctrl &= ~DMA_SUBWORD_SWAP_MASK;
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= DMA_SWAP_8_BIT;
   
   /* Prime TX and RX */
   for ( i=0; i<DECT_DMA_NUM_CHAN; i++ )
   {
      for ( j=0; j<DECT_DMA_BUFFER_SIZE_WORDS; j++ )
      {
         *( pDectTxDmaDDRStartAddrV[i] + j ) =  0x01234567;
         *( pDectRxDmaDDRStartAddrV[i] + j ) =  0x01112131;
         *( (unsigned int *)(IO_ADDRESS((unsigned int)(pDectTxDmaAHBStartAddr[i]))) + j ) =  0x02122232;
         *( (unsigned int *)(IO_ADDRESS((unsigned int)(pDectRxDmaAHBStartAddr[i]))) + j ) =  0xFEDCBA98;                  
      }
   }

   /* Verify memory locations */
   gprint("################## DMA Memories Before ##################\n", 0, 0, 0);   
   dumpDataMemWords();
   gprint("################## DMA Memories Before ##################\n\n", 0, 0, 0);   
   
   /* Clear all status bits */
   DECT_CTRL->dect_shm_irq_status |= DECT_CTRL->dect_shm_irq_status;

}
      
void dectDMATest()
{
   int transferCount = 0;
   unsigned int tempIrqStatus;
   int j = 0;  
 
   /* Enable channels */
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( RX_EN_0 | TX_EN_0 );  
#if (DECT_DMA_NUM_CHAN > 1)
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( RX_EN_1 | TX_EN_1 );  
#if (DECT_DMA_NUM_CHAN > 2)
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( RX_EN_2 | TX_EN_2 );  
#if (DECT_DMA_NUM_CHAN > 3)
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( RX_EN_3 | TX_EN_3 ); 
#endif 
#endif 
#endif
  
   /* Start dma */
#if REG_TRIGGER_TEST
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( DMA_RX_REG_TRIG_EN | DMA_TX_REG_TRIG_EN ); 
#endif

#if ZCROSS_TRIGGER_TEST
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( DMA_RX_INT_TRIG_EN | DMA_TX_INT_TRIG_EN ); 
   AHB_REGISTERS->dsp_zcross1_out = 0x8000;
   AHB_REGISTERS->dsp_zcross2_out = 0x8000;
#endif
  
#if CNT_DECT_CLK_TRIGGER_TEST 
   DECT_DMA_CTRL->dect_shm_dma_trig_cnt_preset = 0x12FC00; /* 0x12FC00 is 10 milliseconds at DECT clock (12*10.368 MHz = 124.416 MHz) */
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( DMA_RX_CNT_TRIG_EN | DMA_TX_CNT_TRIG_EN );
#endif

#if CNT_PCM_CLK_TRIGGER_TEST
   DECT_DMA_CTRL->dect_shm_dma_trig_cnt_preset = 0x28000; /* 0x28000 is 10 milliseconds at PCM clock (16.384 MHz) */
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= DMA_TRIG_CNT_CLK_SEL_PCM;
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( DMA_RX_CNT_TRIG_EN | DMA_TX_CNT_TRIG_EN );
   //dectip_aux(); /* AUX2 test used to verify that the ext_int is triggered to Gen2DSP from DECT shim once DMA completes */
#endif

#if CNT_DSP_IRQ_DECT_TRIGGER_TEST
   DECT_DMA_CTRL->dect_shm_dma_trig_cnt_preset = 0x12FC00; /* 0x12FC00 is 10 milliseconds at DECT clock (12*10.368 MHz = 124.416 MHz) */
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= DMA_TRIG_CNT_IRQ_EN;
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( DMA_RX_CNT_TRIG_EN | DMA_TX_CNT_TRIG_EN );
   dectip_aux();  /* NOTE: this must be the AUX test that triggers the dsp_irq from Gen2DSP to DECT shim */
#endif
 
#if CNT_DSP_IRQ_PCM_TRIGGER_TEST
   DECT_DMA_CTRL->dect_shm_dma_trig_cnt_preset = 0x28000; /* 0x28000 is 10 milliseconds at PCM clock (16.384 MHz) */
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= DMA_TRIG_CNT_CLK_SEL_PCM;
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= DMA_TRIG_CNT_IRQ_EN;
   DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( DMA_RX_CNT_TRIG_EN | DMA_TX_CNT_TRIG_EN );
   dectip_aux(); /* NOTE: this must be the AUX test that triggers the dsp_irq from Gen2DSP to DECT shim */
#endif

   //gprint("In DMA test, before while loop.\n", 0, 0, 0);  
   //dumpRegs();


   /* Check dma */                       
   while(transferCount < DECT_DMA_BUFFER_SIZE_WORDS/DECT_DMA_TRANSFER_SIZE_WORDS)  /* Number of transfers depends of transfer and buffer sizes for 1 complete buffer */
   {
      tempIrqStatus = DECT_CTRL->dect_shm_irq_status;
      
      /* Check if both TX and RX transfers are done */
      if ( ((tempIrqStatus & (unsigned int)0x00006030) == (unsigned int)0x00006030)/* || (j == 3 )*/ )
      {
         j = 0;
#if ZCROSS_TRIGGER_TEST
         AHB_REGISTERS->dsp_zcross1_out = 0x0000;
         AHB_REGISTERS->dsp_zcross2_out = 0x0000;
#endif
         /* Increment Transfer count */
         transferCount++;
         //if (transferCount == DECT_DMA_BUFFER_SIZE_WORDS/DECT_DMA_TRANSFER_SIZE_WORDS) break;
       
       //  gprint("DECT DMA run: xfer %d\n", transferCount, 0, 0); 
       //  gprint("DECT count 0x%08x, PCM count 0x%08x\n", DECT_CTRL->dect_shm_dect_clk_cntr, DECT_CTRL->dect_shm_pcm_clk_cntr, 0);
 #if 0  /* Cannot have this check because counter wraps to 0 on last value. */
         /* We only check the first channel's counts for now */ 
         if ( (DECT_DMA_CTRL->dect_shm_dma_xfer_cntr_rx >> 24) != transferCount || (DECT_DMA_CTRL->dect_shm_dma_xfer_cntr_tx >> 24) != transferCount )
         { 
            gprint("xfer count does not match! ixferCount %d, rx reg 0x%08x, tx reg 0x%08x\n", transferCount, DECT_DMA_CTRL->dect_shm_dma_xfer_cntr_rx, 
                    DECT_DMA_CTRL->dect_shm_dma_xfer_cntr_tx);
            break;
         }         
 #endif
         //gprint("transferCount: %d, rx reg 0x%08x, tx reg 0x%08x\n", transferCount, DECT_DMA_CTRL->dect_shm_dma_xfer_cntr_rx, 
         //       DECT_DMA_CTRL->dect_shm_dma_xfer_cntr_tx);
 
         /* Clear all status bits */
         DECT_CTRL->dect_shm_irq_status |= DECT_CTRL->dect_shm_irq_status;

#if REG_TRIGGER_TEST         
         DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( RX_REG_TRIG_EN | TX_REG_TRIG_EN );
#endif

#if ZCROSS_TRIGGER_TEST
         AHB_REGISTERS->dsp_zcross1_out = 0x8000;
         AHB_REGISTERS->dsp_zcross2_out = 0x8000;
#endif

#if (CNT_DECT_CLK_TRIGGER_TEST || CNT_PCM_CLK_TRIGGER_TEST || CNT_DSP_IRQ_DECT_TRIGGER_TEST || CNT_DSP_IRQ_PCM_TRIGGER_TEST)
         DECT_DMA_CTRL->dect_shm_dma_ctrl |= ( DMA_RX_CNT_TRIG_EN | DMA_TX_CNT_TRIG_EN );
#endif
      }

      j++;
   }

   
   gprint("\n###################### TEST COMPLETE ###############################\n", 0, 0, 0);
   gprint("After test: CPU count 0x%08x, PCM pulse count 0x%08x, DECT pulse count 0x%08x\n", 
           timerGetCountMips(), DECT_CTRL->dect_shm_pcm_clk_cntr, DECT_CTRL->dect_shm_dect_clk_cntr_sh);
   dumpDataMemBytes();
   //dumpDataMemWords();
   gprint("\n********************** POST TEST **********************************\n", 0, 0, 0);
   dumpRegs();
}

void dumpDataMemWords()
{
   unsigned int i,j;
   for ( i=0; i<DECT_DMA_NUM_CHAN; i++ )
   {
      for ( j=0; j<DECT_DMA_BUFFER_SIZE_WORDS; j++ )
      {
         gprint("%d TxD 0x%08x, TxA 0x%08x, ",  i,
                                                *( pDectTxDmaDDRStartAddrV[i] + j ),        
                                                *( (unsigned int *)(IO_ADDRESS((unsigned int)(pDectTxDmaAHBStartAddr[i]))) + j ) );                                                               
         gprint("RxA 0x%08x, RxD 0x%08x %d\n",  *( (unsigned int *)(IO_ADDRESS((unsigned int)(pDectRxDmaAHBStartAddr[i]))) + j ),        
                                                *( pDectRxDmaDDRStartAddrV[i] + j ), j );                  
      }
      gprint("\n",0,0,0);
   }
}

void dumpDataMemBytes()
{
   unsigned int i,j;
   for ( i=0; i<DECT_DMA_NUM_CHAN; i++ )
   {
      for ( j=0; j<DECT_DMA_BUFFER_SIZE_BYTES; j++ )
      {
         gprint("%d TxD 0x%02x, TxA 0x%02x, ",  i,
                                                *( (unsigned char *)(pDectTxDmaDDRStartAddrV[i]) + j ),
                                                *( (unsigned char *)(IO_ADDRESS((unsigned int)(pDectTxDmaAHBStartAddr[i]))) + j ) );                                                                        
         gprint("RxA 0x%02x, RxD 0x%02x %d\n",  *( (unsigned char *)(IO_ADDRESS((unsigned int)(pDectRxDmaAHBStartAddr[i]))) + j ),
                                                *( (unsigned char *)(pDectRxDmaDDRStartAddrV[i]) + j ), j );
      }
      gprint("\n",0,0,0);
   }
}



void dectDMAahbTest(void)
{
   XDRV_UINT32 * pAhbMem = (XDRV_UINT32 *)DECT_AHB_SHARED_RAM_BASE;
   gprint("#############AHB TEST##############\n", 0, 0, 0);
   gprint("## AHB BEFORE Write 0x%08x @ 0x%08x\n",  *pAhbMem, (XDRV_UINT32)pAhbMem, 0);
   *pAhbMem = 0xFFFFFFFF;
   gprint("## AHB AFTER  Write 0x%08x @ 0x%08x\n",  *pAhbMem, (XDRV_UINT32)pAhbMem, 0);
   gprint("#############AHB TEST##############\n", 0, 0, 0);
}

void dectDMAapbTest(void)
{
   XDRV_UINT16 * pApbMem = (XDRV_UINT16 *)DECT_APB_REG_BASE;
   gprint("## APB READ 0x%04x @ 0x%04x\n",  *pApbMem, (XDRV_UINT32)pApbMem, 0);
   *pApbMem = 0xFFFF;
   gprint("## APB WRITE 0x%04x @ 0x%04x\n",  *pApbMem, (XDRV_UINT32)pApbMem, 0);
}

void ddrTest(void)
{  
   gprint("## DDR DECTREG READ 0x%08x @ 0x%08x\n",  DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s0, (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s0, 0);
   DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s0 = 0xFFFFFFFF;
   gprint("## DDR DECTREG READ 0x%08x @ 0x%08x\n",  DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s0, (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s0, 0);
}
   
void dumpRegs()
{
//   gprint("## int_clkcontrol                  : 0x%08x @ 0x%08x ##\n", INT_CTRL->int_clkcontrol                      , (XDRV_UINT32)&INT_CTRL->int_clkcontrol                       ,0 );
   gprint("## dect_shm_ctrl                   : 0x%08x @ 0x%08x ##\n", DECT_CTRL->dect_shm_ctrl                      , (XDRV_UINT32)&DECT_CTRL->dect_shm_ctrl                       ,0 );
   gprint("## dect_shm_pll_reg_1              : 0x%08x @ 0x%08x ##\n", DECT_CTRL->dect_shm_pll_reg_1                 , (XDRV_UINT32)&DECT_CTRL->dect_shm_pll_reg_1                  ,0 );
   gprint("## dect_shm_dma_status             : 0x%08x @ 0x%08x ##\n", DECT_CTRL->dect_shm_dma_status                , (XDRV_UINT32)&DECT_CTRL->dect_shm_dma_status                 ,0 );
   gprint("## dect_shm_irq_status             : 0x%08x @ 0x%08x ##\n", DECT_CTRL->dect_shm_irq_status                , (XDRV_UINT32)&DECT_CTRL->dect_shm_irq_status                 ,0 );
   gprint("## dect_shm_dma_ctrl               : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ctrl              , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ctrl               ,0 );
   gprint("## dect_shm_dma_trig_cnt_preset    : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_trig_cnt_preset   , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_trig_cnt_preset    ,0 );   
   gprint("## dect_shm_dma_ddr_saddr_tx_s0    : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s0   , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s0    ,0 );   
   gprint("## dect_shm_dma_ddr_saddr_tx_s1    : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s1   , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s1    ,0 );   
   gprint("## dect_shm_dma_ddr_saddr_tx_s2    : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s2   , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s2    ,0 );   
   gprint("## dect_shm_dma_ddr_saddr_tx_s3    : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s3   , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_tx_s3    ,0 );   
   gprint("## dect_shm_dma_ddr_saddr_rx_s0    : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s0   , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s0    ,0 );   
   gprint("## dect_shm_dma_ddr_saddr_rx_s1    : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s1   , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s1    ,0 );   
   gprint("## dect_shm_dma_ddr_saddr_rx_s2    : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s2   , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s2    ,0 );   
   gprint("## dect_shm_dma_ddr_saddr_rx_s3    : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s3   , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ddr_saddr_rx_s3    ,0 );   
   gprint("## dect_shm_dma_ahb_saddr_tx_s01   : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_tx_s01  , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_tx_s01   ,0 );   
   gprint("## dect_shm_dma_ahb_saddr_tx_s23   : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_tx_s23  , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_tx_s23   ,0 );   
   gprint("## dect_shm_dma_ahb_saddr_rx_s01   : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_rx_s01  , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_rx_s01   ,0 );   
   gprint("## dect_shm_dma_ahb_saddr_rx_s23   : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_rx_s23  , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_ahb_saddr_rx_s23   ,0 );   
   gprint("## dect_shm_dma_xfer_size_tx       : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_xfer_size_tx      , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_xfer_size_tx       ,0 );   
   gprint("## dect_shm_dma_xfer_size_rx       : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_xfer_size_rx      , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_xfer_size_rx       ,0 );   
   gprint("## dect_shm_dma_buf_size_tx        : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_buf_size_tx       , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_buf_size_tx        ,0 );   
   gprint("## dect_shm_dma_buf_size_rx        : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_buf_size_rx       , (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_buf_size_rx        ,0 );   
   gprint("## dect_shm_dma_offset_addr_tx_s01 : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_offset_addr_tx_s01, (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_offset_addr_tx_s01 ,0 );   
   gprint("## dect_shm_dma_offset_addr_tx_s23 : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_offset_addr_tx_s23, (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_offset_addr_tx_s23 ,0 );   
   gprint("## dect_shm_dma_offset_addr_rx_s01 : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_offset_addr_rx_s01, (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_offset_addr_rx_s01 ,0 );   
   gprint("## dect_shm_dma_offset_addr_rx_s23 : 0x%08x @ 0x%08x ##\n", DECT_DMA_CTRL->dect_shm_dma_offset_addr_rx_s23, (XDRV_UINT32)&DECT_DMA_CTRL->dect_shm_dma_offset_addr_rx_s23 ,0 );   
}



