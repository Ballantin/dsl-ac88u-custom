/***************************************************************************
*    Copyright 2012  Broadcom
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
*    Filename: hal_dsp_version.h
*
****************************************************************************
*    Description:
*
*    HAL interface for retrieving DSP version information.
*
****************************************************************************/

#ifndef HAL_DSP_VERSION_H
#define HAL_DSP_VERSION_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */

/* Maximum length of the DSP version string */
#define HAL_DSP_VERSION_STRING_LENGTH 100

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   halDspGetVersion
**
** PURPOSE:    Gets the DSP version string 
**
** PARAMETERS: version - The string to which the version information is copied
**             size    - Size of the version string
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
int halDspGetVersion( char* version, int size );

#ifdef __cplusplus
    }
#endif

#endif  /* HAL_DSP_VERSION_H  */
