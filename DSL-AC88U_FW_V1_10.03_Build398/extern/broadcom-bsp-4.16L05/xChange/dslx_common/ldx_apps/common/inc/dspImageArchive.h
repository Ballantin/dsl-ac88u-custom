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
*    Filename: dspImageArchive
*
****************************************************************************
*    Description:
*
*     Interface for ...
*
****************************************************************************/

#ifndef DSP_IMAGE_ARCHIVE_H
#define DSP_IMAGE_ARCHIVE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */


typedef struct DSP_IMAGES
{
   /* Image to be loaded into DSP internal memory. */
   const unsigned short   *internalMemoryImage;

   /* Paged program overlay image. May be NULL. */
   const unsigned short   *prgOverlayImage;

   /* Size of paged program overlay in 16-bit words. */
   unsigned int            prgOverlaySizeWords;

   /* Paged instance memory overlay image. May be NULL. */
   const unsigned short   *instOverlayImage;

   /* Size of paged instance memory overlay in 16-bit words. */
   unsigned int            instOverlaySizeWords;

} DSP_IMAGES;


/*
** This structure defines the capabilities of a DSP image.
*/
typedef struct DSP_CAPABILITIES
{
   /* VHD capabilities. */
   unsigned int   numGatewayVhds;
   unsigned int   numPhoneVhds;
   unsigned int   numToneVhds;
   unsigned int   numConferenceVhds;
   unsigned int   numHalfVhds;
   unsigned int   numPstnVhds;
   unsigned int   numLineVhds;

   /* Vocoder capabilities. */
   unsigned int   g711Support;
   unsigned int   g722Support;
   unsigned int   g729Support;
   unsigned int   g729ASupport;
   unsigned int   g729ABSupport;
   unsigned int   g729ESupport;
   unsigned int   g7231Support;
   unsigned int   g7231ASupport;
   unsigned int   g728Support;
   unsigned int   g726Support;
   unsigned int   bv16Support;
   unsigned int   bv32Support;
   unsigned int   lpcm64Support;
   unsigned int   lpcm128Support;
   unsigned int   lpcm256Support;
   unsigned int   iLbcSupport;
   unsigned int   gsmamrSupport;
   unsigned int   gsmefrSupport;
   unsigned int   gsmfrSupport;
   unsigned int   gsmhrSupport;
   unsigned int   amrwbSupport;

   unsigned int   faxSupport;

   unsigned int   gplcSupport;
   unsigned int   bvcSupport;

   /* Idle CPU profiler. */
   unsigned int   icpSupport;

   /* Software equalizer. */
   unsigned int   equSupport;

} DSP_CAPABILITIES;


/*
** DSP image configuration info.
*/
typedef struct DSP_CONFIG
{
   /*
   ** Resampler coefficient data addresses and length parameters. These can be
   ** used by the host to download new coefficients at DSP boot-time.
   */
   unsigned int   resamplerInternationalSupport;
   unsigned int   resamplerCoeffInDataMem;
   unsigned int   resamplerInterpFilterLen;
   unsigned short resamplerInterpFilterAddr;
   unsigned int   resamplerDecimFilterLen;
   unsigned short resamplerDecimFilterAddr;

   /* Boolean parameter indicating whether distributed hausware is used. */
   unsigned int   isHauswareDistributed;


   /* Boolean indicating whether wideband (16kHz) is used (0 means narrowbannd - 8kHz) */
   unsigned int   isWidebandEnabled;

   /* DSP frame sync rate (number of 8kHz samples that are processed each time the hausware runs) */
   unsigned int   frameSyncSamples;

} DSP_CONFIG;

/*
** Composite DSP "image-archive" that contains images to be loaded, image
** capabilities, config info, etc...
*/
typedef struct DSP_IMAGE_ARCHIVE
{
   DSP_IMAGES        images;
   DSP_CAPABILITIES  caps;
   DSP_CONFIG        config;

} DSP_IMAGE_ARCHIVE;


/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

const DSP_IMAGE_ARCHIVE *dspImageArchiveGet( void );



#ifdef __cplusplus
    }
#endif

#endif  /* DSP_IMAGE_ARCHIVE_H  */
