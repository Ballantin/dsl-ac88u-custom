/****************************************************************************
*
*     Copyright (c) 2002 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: boot63268.h
*
****************************************************************************
*    Description:
*
*     This file contains the public API for bootloading the "secondary"
*     MIPS thread-processor on the 63268. The bootload is performed by the
*     "primary" thread-processor. In a typical application, the "secondary"
*     thread-processor would be running voice-processing (DSP) algorithms.
*     However, this module should be agnostic to the type of application
*     that is being loaded.
*
****************************************************************************/

#ifndef BOOT63268_H
#define BOOT63268_H

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

} BOOT63268_THREAD_PROCESSOR_IMAGE;


/* ---- Function Prototypes ----------------------------------------------- */

/*****************************************************************************
*
*  FUNCTION:   boot63268Init
*
*  PURPOSE:    Initializes the 63268, MIPS thread-processor bootloader.
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
int boot63268Init( XDRV_ITPC *itpcDrv );


/*****************************************************************************
*
*  FUNCTION:   boot63268Deinit
*
*  PURPOSE:    De-initializes the 63268 thread-processor bootloader.
*
*  PARAMETERS: None.
*
*  RETURNS:    0 on success
*
*  NOTES:
*
*****************************************************************************/
int boot63268Deinit( void );


/*
*****************************************************************************
** FUNCTION:   boot63268LoadImage
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
int boot63268LoadImage
(
   const BOOT63268_THREAD_PROCESSOR_IMAGE *image,
   unsigned int                          *loadAddress
);


/*
*****************************************************************************
** FUNCTION:   boot63268SendIpc
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
void boot63268SendIpc( void *ipcStruct );



#endif  /* BOOT63268_H */
