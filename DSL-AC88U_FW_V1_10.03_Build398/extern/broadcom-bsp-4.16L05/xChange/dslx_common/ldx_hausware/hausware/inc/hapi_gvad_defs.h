/***************************************************************************
* Copyright (c) 2002 Broadcom, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom
* company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*
*      This file is the public header file for the generic VAD and CNG
*
****************************************************************************/

#ifndef HAPI_GVAD_DEFS_H
#define HAPI_GVAD_DEFS_H

typedef enum
{
   HAPI_GVAD_MODE_OFF = 0,         /* Voice activity detector off */
   HAPI_GVAD_MODE_TRANSPARENT,     /* Voice activity detector transparent (default) */
   HAPI_GVAD_MODE_CONSERVATIVE,    /* Voice activity detector conservative */
   HAPI_GVAD_MODE_AGGRESSIVE       /* Voice activity detector agressive */

} HAPI_GVAD_MODE;

/* vadCng enumerated data types */
typedef enum
{
   HAPI_GCNG_NOISEOFF = 0,    /* Generate Silence */
   HAPI_GCNG_WHITENOISE,      /* Generate White noise */
   HAPI_GCNG_HOTHNOISE,       /* Generate Hoth noise (default) */
   HAPI_GCNG_MATCHSPECTRUM    /* Appendix II CNG or estimate spectrum at decoder */
} HAPI_GCNG_NOISETYPE;


#endif
