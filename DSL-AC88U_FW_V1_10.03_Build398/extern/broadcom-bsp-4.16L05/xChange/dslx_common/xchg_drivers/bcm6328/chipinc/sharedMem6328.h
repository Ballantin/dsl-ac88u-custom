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
*    Filename: sharedMem6328.h
*
****************************************************************************
*    Description:
*
*     Shared memory definitions used for handshaking and communication
*     between the MIPS thread-processors of the 6328.
*
****************************************************************************/

#ifndef SHARED_MEM_6328_H
#define SHARED_MEM_6328_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvItpc.h>
#include <xdrvSpinLock.h>


/* ---- Constants and Types ---------------------------------------------- */


/* Does this belong in this header file? */
#define HANDSHAKE_THREAD_PROCESSOR_BOOTED          0xC0FFEE
#define HANDSHAKE_THREAD_PROCESSOR_APP_INIT_DONE   0xBADF00D




/*
** Shared memory descriptor for communication between MIPS thread-processors.
** This contains pointers to shared memory regions used by various communication
** modules. It may be expanded in the future as needed...
*/
typedef struct SHARED_MEM_DESCRIPTOR_6328
{
   /* Shared memory used by the inter-processor communication (IPC) module.
   ** This is used to send messages between distributed portions of the voice
   ** processing stack. */
   volatile void             *ipcSharedMem;


   /* Shared memory used by the inter-thread processor communication (ITPC)
   ** module. This provides an interrupt based communication layer, used to
   ** send generic commands between the two MIPS thread-processors.
   */
   XDRV_ITPC_SHARED_MEM_DATA *itpcSharedMemData;


   /* Spin lock synchronization primitive used to prevent concurrent access
   ** to shared memory data structures by both MIPS cores. */
   XDRV_SPIN_LOCK            *spinLock;

   /* Shared memory used by Hausware for offloading support */
   volatile void             *hauswareSharedMem;

   /* HAL SHIM configuration table */
   volatile void             *halShimConfig;

   /* Number of entries in halShimConfig table */
   XDRV_UINT32               halShimEntryCount;

   /* DMA buffer size in unites of 4 byte word */
   XDRV_UINT32               dmaBufferSize;

   /* Shared memory used to report DSP assert stack dump */
   XDRV_ITPC_DSP_STACK_DUMP *dspStackDump;

} SHARED_MEM_DESCRIPTOR_6328;



/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */



#ifdef __cplusplus
    }
#endif

#endif  /* SHARED_MEM_6328_H  */
