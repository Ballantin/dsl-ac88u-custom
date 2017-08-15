/****************************************************************************
*
* <:copyright-BRCM:2014:DUAL/GPL:standard
* 
*    Copyright (c) 2014 Broadcom
*    All Rights Reserved
* 
* Unless you and Broadcom execute a separate written software license
* agreement governing use of this software, this software is licensed
* to you under the terms of the GNU General Public License version 2
* (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php,
* with the following added to such license:
* 
*    As a special exception, the copyright holders of this software give
*    you permission to link this software with independent modules, and
*    to copy and distribute the resulting executable under terms of your
*    choice, provided that you also meet, for each linked independent
*    module, the terms and conditions of the license of that module.
*    An independent module is a module which is not derived from this
*    software.  The special exception does not apply to any modifications
*    of the software.
* 
* Not withstanding the above, under no circumstances may you combine
* this software in any way with any other Broadcom software provided
* under a license other than the GPL, without Broadcom's express prior
* written consent.
* 
* :>
*
*****************************************************************************/
/**
*
*  @file    bosLog.h
*
*  @brief   Contains the BOS Logging interface.
*
****************************************************************************/
/**
*  @defgroup   bosLog   BOS Log
*
*  @brief      Common logging routines for BOS modules.
*
*  The BOS logging module provides helper routines used by other BOS modules.
*  It initializes and interfaces to the Broadcom logging (blog) module.
*
****************************************************************************/

#if !defined( BOS_LOG_H )
#define BOS_LOG_H            /**< Include Guard                           */

/* ---- Include Files ---------------------------------------------------- */

#if !defined( BOSTYPES_H )
#  include "bosTypes.h"
#endif

#if !defined( BOSCFG_H )
#  include "bosCfg.h"
#endif


#if !defined( BLOG_H )
#  include <blog.h>
#endif



#if defined( __cplusplus )
extern "C"
{
#endif

/**
 * @addtogroup bosLog
 * @{
 */

/* ---- Constants and Types ---------------------------------------------- */


/*
** BOS_LOG has been deprecated. Use the Broadcom logging module instead (blog).
** This macro has only been maintained to provide backwards compatibility. It
** should eventually be eliminated.
**
** BOS_LOG_ERR and BOS_LOG_PARAM have been obsoleted.
*/
#ifndef BOS_LOG
#define BOS_LOG(exp) printf exp;
#endif


/*
** Logging handle. Use this in actual logging macros.
*/
#if BOS_OVERRIDE_NATIVE_BLOG
#define gBosBlog	BOS_LOG_MOD_BOS
#else
#define gBosBlog        BLOG_HDL( gBosBlogSettings )
#endif


/*
** Application specific logging severity for function parameter output.
** Socket parameters have their own severity level, but users may call select
** periodically, potentially resulting in tons of unwanted logs.
*/
#define BOS_LOG_PARAMS           BLOG_SEVERITY_USR1
#define BOS_LOG_SOCKET_PARAMS    BLOG_SEVERITY_USR8


/* ---- Variable Externs ------------------------------------------------- */

/*
** Logging settings handle. Use this handle to create/destroy the logging
** module instance and change module attributes.
*/
extern BLOG_MOD_HNDL    gBosBlogSettings;


/* ---- Function Prototypes ---------------------------------------------- */

BOS_STATUS bosLogInit( void );
BOS_STATUS bosLogTerm( void );


/** @} */

#if defined( __cplusplus )
}
#endif

#endif /* BOS_LOG_H */
