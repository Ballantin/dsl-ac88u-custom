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
/*   MODULE:  common63148.h                                            */
/*   DATE:    09/08/13                                                 */
/*   PURPOSE: Define addresses of major hardware components of         */
/*            BCM63148                                                 */
/*                                                                     */
/***********************************************************************/
#ifndef __COMMON_63148_H
#define __COMMON_63148_H

#ifdef __cplusplus
extern "C" {
#endif

/* IO_ADDRESS() macro taken from arch/arm/mach-bcm963xx/include/mach/hardware.h */
#define IO_ADDRESS(x)		(((x) & 0x00ffffff) + 0xfc000000)

#define REG_BASE		IO_ADDRESS(0x80000000)
#define PER_BASE		IO_ADDRESS(0xfffe0000)
#define BCM_IO_ADDR(x)		IO_ADDRESS(x)

#define BCM63148_PERF_BASE                   (PER_BASE + 0x00008000)  /* chip control */
#define BCM63148_TIMR_BASE                   (PER_BASE + 0x00008080)  /* timer registers */
#define BCM63148_NAND_INTR_BASE              (PER_BASE + 0x000080f0)  /* nand interrupt control */
#define BCM63148_GPIO_BASE                   (PER_BASE + 0x00008100)  /* gpio registers */
#define BCM63148_MISC_BASE                   (PER_BASE + 0x00008180)  /* Miscellaneous Registers */
#define BCM63148_SOTP_BASE                   (PER_BASE + 0x00008200)
#define BCM63148_PKA_BASE                    (PER_BASE + 0x00008280)
#define BCM63148_RNG_BASE                    (PER_BASE + 0x00008300)
#define BCM63148_UART_BASE                   (PER_BASE + 0x00008600)  /* uart registers */
#define BCM63148_UART1_BASE                  (PER_BASE + 0x00008620)  /* uart registers */
#define BCM63148_LED_BASE                    (PER_BASE + 0x00008700)  /* LED control registers */
#define BCM63148_I2S_BASE                    (PER_BASE + 0x00008900)
#define BCM63148_AES0_BASE                   (PER_BASE + 0x00008980)
#define BCM63148_AES1_BASE                   (PER_BASE + 0x00008a00)
#define BCM63148_HSSPIM_BASE                 (PER_BASE + 0x00009000)  /* High-Speed SPI registers */
#define BCM63148_NAND_REG_BASE               (PER_BASE + 0x0000a000)  /* nand interrupt control */
#define BCM63148_NAND_CACHE_BASE             (PER_BASE + 0x0000a400)
#define BCM63148_BROM_SEC_BASE               (PER_BASE + 0x0000b600)
#define BCM63148_SRAM_SEC_BASE               (PER_BASE + 0x0000b800)
#define BCM63148_PER_SEC_BASE                (PER_BASE + 0x0000b820)
#define BCM63148_JTAG_OTP_BASE               (PER_BASE + 0x0000bb00)
#define BCM63148_JTAG_IOTP_BASE              (PER_BASE + 0x0000bd00)
#define BCM63148_BOOTLUT_BASE                (PER_BASE + 0x00010000)

#define BCM63148_USB_CTL_BASE                (REG_BASE + 0x00001000)  /* USB 2.0 device control */
#define BCM63148_USB_DMA_BASE                (REG_BASE + 0x00001800)  /* USB 2.0 device DMA */

#define BCM63148_MEMC_BASE                   (REG_BASE + 0x00002000)  /* DDR IO Buf Control */
#define BCM63148_MEMC_BASE_OFF_4K            (MEMC_BASE + 0x00001000)
#define BCM63148_DDRPHY_BASE                 (REG_BASE + 0x00003000)
#define BCM63148_SAR_BASE                    (REG_BASE + 0x00004000)
#define BCM63148_SAR_DMA_BASE                (REG_BASE + 0x00007800)  /* ATM SAR DMA control */
#define BCM63148_SATA_BASE                   (REG_BASE + 0x00008000)

#define BCM63148_USBH_BASE                   (REG_BASE + 0x0000c000)
#define BCM63148_USBH_CFG_BASE               (REG_BASE + 0x0000c200)
#define BCM63148_USB_EHCI_BASE               (REG_BASE + 0x0000c300)  /* USB host registers */
#define BCM63148_USB_OHCI_BASE               (REG_BASE + 0x0000c400)  /* USB host registers */
#define BCM63148_USB_XHCI_BASE               (REG_BASE + 0x0000d000)
#define BCM63148_USB_XHCI1_BASE              (REG_BASE + 0x0000e000)
#define BCM63148_UART2_BASE                  (REG_BASE + 0x00019000)  /* ARM UART base */

#define BCM63148_ERROR_PORT_BASE             (REG_BASE + 0x00010000)
#define BCM63148_AIP_BASE                    (REG_BASE + 0x00018000)
#define BCM63148_UART2_BASE                  (REG_BASE + 0x00019000)  /* ARM UART base */
#define BCM63148_ETC_BASE                    (REG_BASE + 0x0001a000)
#define BCM63148_L2C_BASE                    (REG_BASE + 0x0001d000)
#define BCM63148_SCU_BASE                    (REG_BASE + 0x0001e000)
#define BCM63148_ARMCFG_BASE                 (REG_BASE + 0x00020000)

#define BCM63148_DECT_BASE                   (REG_BASE + 0x00040000)
#define BCM63148_DECT_AHB_REG_BASE           (REG_BASE + 0x00040000)
#define BCM63148_DECT_SHIM_CTRL_BASE         (REG_BASE + 0x00050000)
#define BCM63148_DECT_SHIM_DMA_CTRL_BASE     (REG_BASE + 0x00050050)
#define BCM63148_DECT_APB_REG_BASE           (REG_BASE + 0x00050800)
#define BCM63148_PCIE_BASE                   (REG_BASE + 0x00060000)
#define BCM63148_PCIE1_BASE                  (REG_BASE + 0x00070000)

#define BCM63148_SF2_BASE                    (REG_BASE + 0x00080000)
#define BCM63148_SWITCH_BASE                 SF2_BASE
#define BCM63148_APM_BASE                    (REG_BASE + 0x00100000)  /* APM control registers - APM functionality not present in 63148 */
#define BCM63148_PCM_BASE                    (REG_BASE + 0x00100200)  /* PCM control registers */
#define BCM63148_APM_PCM_DMA_BASE            (REG_BASE + 0x00100800)  /* PCM DMA control registers */
#define BCM63148_RDP_BASE                    (REG_BASE + 0x00200000)
#define BCM63148_PMC_BASE                    (REG_BASE + 0x00400000)
#define BCM63148_PROC_MON_BASE               (REG_BASE + 0x00480000)
#define BCM63148_DSLPHY_BASE                 (REG_BASE + 0x00600000)
#define BCM63148_DSLLMEM_BASE                (REG_BASE + 0x00700000)

/* We do not create default MMU entry for PCIE MEM */
#define BCM63148_PCIE_MEM_BASE               (0x90000000)
#define BCM63148_PCIE1_MEM_BASE              (0xa0000000)

#define BCM63148_DECT_STARTCTL      (BCM63148_DECT_AHB_REG_BASE + 0x0818)
#define BCM63148_PCM_BUFF_CTL3      (BCM63148_DECT_AHB_REG_BASE + 0x082c)
#define BCM63148_PCM_BUFF_CTL7      (BCM63148_DECT_AHB_REG_BASE + 0x083c)
//#define BCM63148_DECT_AHB_CHAN0_RX  (BCM63148_DECT_AHB_REG_BASE + 0x0a20)
//#define BCM63148_DECT_AHB_CHAN1_RX  (BCM63148_DECT_AHB_REG_BASE + 0x0de0)
//#define BCM63148_DECT_AHB_CHAN2_RX  (BCM63148_DECT_AHB_REG_BASE + 0x11a0)
//#define BCM63148_DECT_AHB_CHAN3_RX  (BCM63148_DECT_AHB_REG_BASE + 0x1560)
//#define BCM63148_DECT_AHB_CHAN0_TX  (BCM63148_DECT_AHB_REG_BASE + 0x0840)
//#define BCM63148_DECT_AHB_CHAN1_TX  (BCM63148_DECT_AHB_REG_BASE + 0x0c00)
//#define BCM63148_DECT_AHB_CHAN2_TX  (BCM63148_DECT_AHB_REG_BASE + 0x0fc0)
//#define BCM63148_DECT_AHB_CHAN3_TX  (BCM63148_DECT_AHB_REG_BASE + 0x1380)
//#define BCM63148_DECT_CLKEN         0x00000040 

#ifdef __cplusplus
}
#endif

#endif /* __BCM63148_MAP_COMMON_H */
