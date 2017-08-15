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
*     This file provides custom interrupt information for the 6818.
*
****************************************************************************/

#ifndef INT_CTRL_MIPS_CUSTOM_6818_H
#define INT_CTRL_MIPS_CUSTOM_6818_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/*
** List of 6818 peripheral interrupt id's applicable to voip. These should be used to register user
** interrupt service routines via the xdrvIntCtrlInstallInterrupt() API.
** We chose to represent the IRQs from the Low Irq mask/status registers since we are only interested in 
** PCM RX DMA interrupts and APM RX DMA B interrupts, both of which happen to be in the Low bank
** see 6818_intr.h in the shared/opensource directory to confirm
*/
enum
{
   /* Peripheral interrupt id space must be unique. It must not overlap with
   ** the MIPS core interrupt id's. The standard MIPS has 8 interrupts
   ** (id's 0 - 7). So, we'll start at 8. */
   BCM6818_INT_CTRL_MIPS_PERIPH_START = 8,

   /* Do not change the order of interrupts. It must exactly match the bit
   ** ordering in the peripheral interrupt mask register. */
   

   BCM6818_INT_CTRL_MIPS_PERIPH_ENETSW_RX_DMA_0=BCM6818_INT_CTRL_MIPS_PERIPH_START,
   BCM6818_INT_CTRL_MIPS_PERIPH_ENETSW_RX_DMA_1,//1
   BCM6818_INT_CTRL_MIPS_PERIPH_ENETSW_RX_DMA_2,//2
   BCM6818_INT_CTRL_MIPS_PERIPH_ENETSW_RX_DMA_3,//3
   BCM6818_INT_CTRL_MIPS_PERIPH_ENETSW_TX_DMA_0,//4
   BCM6818_INT_CTRL_MIPS_PERIPH_ENETSW_TX_DMA_1,//5
   BCM6818_INT_CTRL_MIPS_PERIPH_ENETSW_TX_DMA_2,//6
   BCM6818_INT_CTRL_MIPS_PERIPH_ENETSW_TX_DMA_3,//7
   BCM6818_INT_CTRL_MIPS_PERIPH_FAP_0_AGAIN,//8
   BCM6818_INT_CTRL_MIPS_PERIPH_FAP_1_AGAIN,//9
   BCM6818_INT_CTRL_MIPS_PERIPH_DUMMY1,//10
   BCM6818_INT_CTRL_MIPS_PERIPH_DUMMY2,//11   
   BCM6818_INT_CTRL_MIPS_PERIPH_EPHY_ENERGY_0,//12
   BCM6818_INT_CTRL_MIPS_PERIPH_EPHY_ENERGY_1,//13
   BCM6818_INT_CTRL_MIPS_PERIPH_GPON_DMA_0,//14
   BCM6818_INT_CTRL_MIPS_PERIPH_GPON_DMA_1,//15
   BCM6818_INT_CTRL_MIPS_PERIPH_APM_DMA0,//16
   BCM6818_INT_CTRL_MIPS_PERIPH_APM_DMA_1,//17
   BCM6818_INT_CTRL_MIPS_PERIPH_APM_DMA2,//18
   BCM6818_INT_CTRL_MIPS_PERIPH_APM_DMA_3,//19
   BCM6818_INT_CTRL_MIPS_PERIPH_APM_DMA4,//20
   BCM6818_INT_CTRL_MIPS_PERIPH_APM_DMA_5,//21
   BCM6818_INT_CTRL_MIPS_PERIPH_IPSEC_DMA_0=28,
   BCM6818_INT_CTRL_MIPS_PERIPH_IPSEC_DMA_1,
   BCM6818_INT_CTRL_MIPS_PERIPH_I2C=31,
   BCM6818_INT_CTRL_MIPS_PERIPH_END = BCM6818_INT_CTRL_MIPS_PERIPH_I2C   

   // TODO ADD ALL OTHER INTERRUPTS

};

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* INT_CTRL_MIPS_CUSTOM_6818_H  */
