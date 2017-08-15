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
*    Filename: boot6818.h
*
****************************************************************************
*    Description:
*
*     This file contains the public API for bootloading the "secondary"
*     MIPS thread-processor on the 6818. The bootload is performed by the
*     "primary" thread-processor. In a typical application, the "secondary"
*     thread-processor would be running voice-processing (DSP) algorithms.
*     However, this module should be agnostic to the type of application
*     that is being loaded.
*
****************************************************************************/

#ifndef BOOT6818_H
#define BOOT6818_H

/* ---- Include Files ----------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvItpc.h>


/* ---- Constants and Types ----------------------------------------------- */

/*
** Thread processor image structure. The image buffer is an array of
** binary data that represents the .text and .data sections of the thread
** processor image.
*/
typedef struct
{
   const XDRV_UINT16   *imageBuf;
   unsigned int         imageSizeInBytes;

} BOOT6818_THREAD_PROCESSOR_IMAGE;


/* ---- Function Prototypes ----------------------------------------------- */

/*****************************************************************************
*
*  FUNCTION:   boot6818Init
*
*  PURPOSE:    Initializes the 6818, MIPS thread-processor bootloader.
*
*
*  PARAMETERS: itpcDrv  (in)  Inter-thread processor communication driver.
*                             Used to signal other TP during boot sequence.
*
*  RETURNS:    0 on success
*
*  NOTES:
*
*****************************************************************************/
int boot6818Init( XDRV_ITPC *itpcDrv );


/*****************************************************************************
*
*  FUNCTION:   boot6818Deinit
*
*  PURPOSE:    De-initializes the 6818 thread-processor bootloader.
*
*  PARAMETERS: None.
*
*  RETURNS:    0 on success
*
*  NOTES:
*
*****************************************************************************/
int boot6818Deinit( void );


/*
*****************************************************************************
** FUNCTION:   boot6818LoadImage
**
** PURPOSE:    To load an image to the secondary thread processor.
**
** PARAMETERS: image       (in)  Image to load onto secondary thread processor.
**             loadAddress (in)  Load address of image.
**
** RETURNS:    0 on success.
**
** NOTE:
*****************************************************************************
*/
int boot6818LoadImage
(
   const BOOT6818_THREAD_PROCESSOR_IMAGE *image,
   unsigned int                          *loadAddress
);


/*
*****************************************************************************
** FUNCTION:   boot6818SendIpc
**
** PURPOSE:    Send shared memory command to secondary thread-processor
**             indicating entry-point.
**
** PARAMETERS: ipcStruct   (in)  Shared memory command.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void boot6818SendIpc( void *ipcStruct );



#endif  /* BOOT6818_H */
