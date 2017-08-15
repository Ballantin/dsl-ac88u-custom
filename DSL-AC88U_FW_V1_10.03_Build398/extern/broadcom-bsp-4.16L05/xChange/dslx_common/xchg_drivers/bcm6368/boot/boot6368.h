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
*    Filename: boot6368.h
*
****************************************************************************
*    Description:
*
*     This file contains the public API for bootloading the "secondary"
*     MIPS thread-processor on the 6368. The bootload is performed by the
*     "primary" thread-processor. In a typical application, the "secondary"
*     thread-processor would be running voice-processing (DSP) algorithms.
*     However, this module should be agnostic to the type of application
*     that is being loaded.
*
****************************************************************************/

#ifndef BOOT6368_H
#define BOOT6368_H

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

} BOOT6368_THREAD_PROCESSOR_IMAGE;


/* ---- Function Prototypes ----------------------------------------------- */

/*****************************************************************************
*
*  FUNCTION:   boot6368Init
*
*  PURPOSE:    Initializes the 6368, MIPS thread-processor bootloader.
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
int boot6368Init( XDRV_ITPC *itpcDrv );


/*****************************************************************************
*
*  FUNCTION:   boot6368Deinit
*
*  PURPOSE:    De-initializes the 6368 thread-processor bootloader.
*
*  PARAMETERS: None.
*
*  RETURNS:    0 on success
*
*  NOTES:
*
*****************************************************************************/
int boot6368Deinit( void );


/*
*****************************************************************************
** FUNCTION:   boot6368LoadImage
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
int boot6368LoadImage
(
   const BOOT6368_THREAD_PROCESSOR_IMAGE *image,
   unsigned int                          *loadAddress
);


/*
*****************************************************************************
** FUNCTION:   boot6368SendIpc
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
void boot6368SendIpc( void *ipcStruct );



#endif  /* BOOT6368_H */
