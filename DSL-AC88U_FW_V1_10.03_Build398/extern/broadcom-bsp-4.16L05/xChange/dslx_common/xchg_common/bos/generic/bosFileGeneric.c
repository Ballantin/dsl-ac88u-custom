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
*  @file    bosFileGeneric.c
*
*  @brief   Contains OS independant definitions for the BOS File module.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_FILE

#include <bosFile.h>
#include <bosFilePrivate.h>
#include <bosError.h>

#if BOS_CFG_RESET_SUPPORT
#include <bosTask.h>
#endif

/**
 * @addtogroup bosFile
 * @{
 */

/* ---- Public Variables ------------------------------------------------- */

#if BOS_CFG_ERROR_FUNCTION_STRINGS

const char *gBosFileFunctionName[ BOS_FILE_NUM_FUNC_ID ];

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */

/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

#if BOS_CFG_ERROR_FUNCTION_STRINGS

/***************************************************************************/
/**
*  Initializes the contents of @a gBosFileFunctionName, which is used to
*  translate function IDs into character strings.
*/

void bosFileInitFunctionNames( void )
{
   gBosFileFunctionName[ BOS_FILE_INIT_FUNC_ID  ] = "bosFileInit";
   gBosFileFunctionName[ BOS_FILE_TERM_FUNC_ID  ] = "bosFileTerm";
   gBosFileFunctionName[ BOS_FILE_OPEN_FUNC_ID  ] = "bosFileOpen";
   gBosFileFunctionName[ BOS_FILE_CLOSE_FUNC_ID ] = "bosFileClose";
   gBosFileFunctionName[ BOS_FILE_READ_FUNC_ID  ] = "bosFileRead";
   gBosFileFunctionName[ BOS_FILE_WRITE_FUNC_ID ] = "bosFileWrite";
   gBosFileFunctionName[ BOS_FILE_SEEK_FUNC_ID ] = "bosFileSeek";
   
   bosErrorAddFunctionStrings( BOS_FILE_MODULE, gBosFileFunctionName, BOS_FILE_NUM_FUNC_ID );

} /* bosFileInitFunctioNames */

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */

/***************************************************************************/

#if BOS_CFG_RESET_SUPPORT

/* Add logic for reset support code */

#endif   /* BOS_CFG_RESET_SUPPORT */

/** @} */

#endif   /* BOS_CFG_FILE */

