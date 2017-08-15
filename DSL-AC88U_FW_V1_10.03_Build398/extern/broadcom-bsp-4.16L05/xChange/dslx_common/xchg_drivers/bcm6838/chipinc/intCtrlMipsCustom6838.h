/***************************************************************************
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
****************************************************************************
*
*    Filename: intCtrlMipsCustom.h
*
****************************************************************************
*    Description:
*
*     Chip specific interrupt control module definitions. This provides an
*     extension to the generic MIPS interrupt control module, which allows
*     users to register interrupt service routines for different types of
*     interrupts (external, peripheral, software, and timer).
*
*     Since each BRCM MIPS-based processor provides a different set of peripheral
*     interrupt sources, it is necessary to provide this custom list of
*     peripheral interrupts. This allows user to register chip specific interrupts
*     while sharing the same interrupt control implementation.
*
*     This file provides custom interrupt information for the 6838.
*
****************************************************************************/

#ifndef INT_CTRL_MIPS_CUSTOM_6838_H
#define INT_CTRL_MIPS_CUSTOM_6838_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/*
** List of 6838 peripheral interrupt id's applicable to voip. These should be used to register user
** interrupt service routines via the xdrvIntCtrlInstallInterrupt() API.
** We chose to represent the IRQs from the Low Irq mask/status registers since we are only interested in 
** PCM RX DMA interrupts and APM RX DMA B interrupts, both of which happen to be in the Low bank
** see 6838_intr.h in the shared/opensource directory to confirm
*/

#define INTERRUPT_ID_SOFTWARE_0           0
#define INTERRUPT_ID_SOFTWARE_1           1

/*=====================================================================*/
/* BCM6838 Timer Interrupt Level Assignments                          */
/*=====================================================================*/
#define MIPS_TIMER_INT                  7

/*=====================================================================*/
/* Peripheral ISR Table Offset                                              */
/*=====================================================================*/
#define INTERNAL_ISR_TABLE_OFFSET   	8
#define INTERNAL_EXT_ISR_TABLE_OFFSET   (INTERNAL_ISR_TABLE_OFFSET + 64)

enum
{
   /* Peripheral interrupt id space must be unique. It must not overlap with
   ** the MIPS core interrupt id's. The standard MIPS has 8 interrupts
   ** (id's 0 - 7). The 6838 has two levels of Peripheral ISRs, APM/PCM DMA
   ** interrupts are part of the the external peripheral ISR block which starts
   ** at an offset of 64, therfore we start at 64+8 */
   BCM6838_INT_CTRL_MIPS_PERIPH_START=INTERNAL_EXT_ISR_TABLE_OFFSET,
   
   /* Do not change the order of interrupts. It must exactly match the bit
   ** ordering in the peripheral interrupt mask register. */
   
   BCM6838_INT_CTRL_MIPS_PERIPH_RDP_MS1588=BCM6838_INT_CTRL_MIPS_PERIPH_START,
   BCM6838_INT_CTRL_MIPS_PERIPH_RDP_SBPM,
   BCM6838_INT_CTRL_MIPS_PERIPH_RDP_BPM,
   BCM6838_INT_CTRL_MIPS_PERIPH_RDP_QEGPHY1,
   BCM6838_INT_CTRL_MIPS_PERIPH_RDP_QEGPHY2,
   BCM6838_INT_CTRL_MIPS_PERIPH_RDP_QEGPHY3,
   BCM6838_INT_CTRL_MIPS_PERIPH_RDP_QEGPHY4,
   BCM6838_INT_CTRL_MIPS_PERIPH_WAN_NCO_GPON,
   BCM6838_INT_CTRL_MIPS_PERIPH_USBH_OHCI,
   BCM6838_INT_CTRL_MIPS_PERIPH_USBH_EHCI,
   BCM6838_INT_CTRL_MIPS_PERIPH_USBH_DISCON,
   BCM6838_INT_CTRL_MIPS_PERIPH_USBH_CCS,
   BCM6838_INT_CTRL_MIPS_PERIPH_PCIE0,
   BCM6838_INT_CTRL_MIPS_PERIPH_PCIE1,
   BCM6838_INT_CTRL_MIPS_PERIPH_APM,
   BCM6838_INT_CTRL_MIPS_PERIPH_APM_IUDMA,
   BCM6838_INT_CTRL_MIPS_PERIPH_RESERVED,
   BCM6838_INT_CTRL_MIPS_PERIPH_MDIO_EXT,
   BCM6838_INT_CTRL_MIPS_PERIPH_MDIO_EGPHY,
   BCM6838_INT_CTRL_MIPS_PERIPH_MDIO_SATA,
   BCM6838_INT_CTRL_MIPS_PERIPH_MDIO_AEPCS,
   BCM6838_INT_CTRL_MIPS_PERIPH_USBD_SYS,
   BCM6838_INT_CTRL_MIPS_PERIPH_PER_ERR_PORT=(INTERNAL_EXT_ISR_TABLE_OFFSET + 28),
   BCM6838_INT_CTRL_MIPS_PERIPH_END = BCM6838_INT_CTRL_MIPS_PERIPH_PER_ERR_PORT,

};




/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* INT_CTRL_MIPS_CUSTOM_6838_H  */
