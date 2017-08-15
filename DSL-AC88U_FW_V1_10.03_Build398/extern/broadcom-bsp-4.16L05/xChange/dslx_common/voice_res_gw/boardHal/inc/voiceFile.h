/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: voicefile.h
*
****************************************************************************
*    Description:
*
*      This file contains the prototype for the function that loads the 
*      DSP images from the Linux filesystem into the DSP_IMAGE_ARCHIVE 
*      struct. The structure containing other DSP related information i.e.
*      sizes, codecs, etc is also defined here and is read from the same 
*      binary file that contains the images. 
****************************************************************************/

#ifndef VOICEFILE_H
#define VOICEFILE_H

#if __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <dspImageArchive.h>

/* ---- Constants and Types ---------------------------------------------- */
/* ---- Public Variables ------------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */

/* The contents of this structure will be read from the binary file */
extern DSP_IMAGE_ARCHIVE zsp3341HwAppArchive;

/* ---- Function Prototypes ---------------------------------------------- */

/*
********************************************************************************
** FUNCTION:   VoiceLoadImageArchive
**
** PURPOSE:    Reads the DSP image archive struct from the binary stored in the
**             Linux filesystem. This struct contains all the DSP capabilities
**             and pointers which can later be used to point to the temporary
**             buffers containing the DSP images.               
**
** PARAMETERS: fname = Name of the file as stored in cramFS.
**
** RETURNS:    0 on success.
**
********************************************************************************
*/
extern int VoiceLoadImageArchive(char *fname);

#if __cplusplus
}
#endif


#endif   /* VOICEFILE_H */
