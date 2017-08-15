/****************************************************************************
*
* Copyright (c) 2002 Broadcom Corporation, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom Corporation
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom Corporation
* company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    boxLogGeneric.c
*
*  @brief   Contains OS independant logging related functions.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>
#include <xchgAssert.h>
#include <bosLog.h>


/* ---- Public Variables ------------------------------------------------- */

/*
** Logging settings handle. Use this handle to create/destroy the logging
** module instance and change module attributes.
*/
BLOG_MOD_HNDL     gBosBlogSettings;


/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup bosLog
 * @{
 */

/***************************************************************************/
/**
*  Initializes the BOS logging module.
*
*  @return  BOS_STATUS_OK if the initialization was successful, or an error
*           code if some type of error occured.
*/

BOS_STATUS bosLogInit( void )
{
   BOS_BOOL    status;

   /*
   ** Initialize the Broadcom logging (BLOG) module. This is initialized here
   ** because there is a coupling between BOS and BLOG - BOS uses BLOG, and BLOG
   ** uses BOS. BOS must be initialized prior to BLOG, so that BLOG can create
   ** the OS primites that it requires. And BLOG must be initialized before
   ** BOS uses any severity based logging.
   **
   ** The BLOG API allows the user to specify non-default callbacks for the
   ** print callback and timestamp callback as part of the call to blogInit.
   ** However, I don't support that here. If the user wants to use non-default
   ** callbacks, they can use other APIs in the blog interface to set them.
   */
   status = blogInit( NULL, NULL );
   XCHG_ASSERT( status == BOS_TRUE );


   /* Create a logging module instance for BOS. */
   gBosBlogSettings = blogAddDefaultModule( "BOS" );

   return ( BOS_STATUS_OK );
}

/***************************************************************************/
/**
*  Terminates the BOS logging module.
*
*  @return  BOS_STATUS_OK if the termination was successful, or an error
*           code if some type of error occured.
*/

BOS_STATUS bosLogTerm( void )
{

   /* Cleanup BOS logging module instance. */
   blogRemoveModule( gBosBlogSettings );

   /* Terminate the entire logging module. */
   blogTerm();

   return ( BOS_STATUS_OK );
}

/** @} */
