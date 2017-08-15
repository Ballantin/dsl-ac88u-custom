/***************************************************************************
*
* Copyright (c) 2003 Broadcom, All Rights Reserved.
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
*      xChange config for HausFrame.  To override defaults in xcfg_hsf.h
*    make a file called xcfg_hsf_custom.h in your build directory.
*
****************************************************************************/
#ifndef XCFG_HSF_H    /* support nested includes */
#define XCFG_HSF_H

/* ---- Include Files ---------------------------------------------------- */

#include <xcfg_hsf_custom.h>

/* ---- Constants and Types ---------------------------------------------- */

/*
** 1 to enable wideband (16kHz sampling rate) support
*/
#ifndef XCFG_HSF_WIDEBAND_SUPPORT
#define XCFG_HSF_WIDEBAND_SUPPORT      1
#endif

#ifndef XCFG_HSF_IORW_SUPPORT
#define XCFG_HSF_IORW_SUPPORT          0
#else
#if !defined( __mips__ )
#error XCFG_HSF_IORW_SUPPORT cannot be enabled for non-MIPS CPU builds
#endif
#endif

/*
** Enables support for Bottom-of-PXD connections
*/
#ifndef XCFG_HSF_BOP_CONNECT_SUPPORT
#define XCFG_HSF_BOP_CONNECT_SUPPORT   0
#endif

#endif   /* XCFG_HSF_H */
