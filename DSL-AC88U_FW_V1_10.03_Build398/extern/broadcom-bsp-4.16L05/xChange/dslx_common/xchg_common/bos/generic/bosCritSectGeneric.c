/****************************************************************************
*
*     Copyright (c) 2001 Broadcom Corporation
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosCritSectGeneric.c
*
*  @brief   Contains OS independant definitions for the BOS CritSect module.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_CRITSECT

#include <bosError.h>
#include <bosCritSect.h>
#include <bosCritSectPrivate.h>

#if BOS_CFG_RESET_SUPPORT
#include <bosTask.h>
#endif

/**
 * @addtogroup bosCritSect
 * @{
 */

/* ---- Public Variables ------------------------------------------------- */

#if BOS_CFG_ERROR_FUNCTION_STRINGS

const char *gBosCritSectFunctionName[ BOS_CRITSECT_NUM_FUNC_ID ];

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */

/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

#if BOS_CFG_ERROR_FUNCTION_STRINGS

/***************************************************************************/
/**
*  Initializes the contents of @a gBosCritSectFunctionName, which is used to
*  translate function IDs into character strings.
*/

void bosCritSectInitFunctionNames( void )
{
   gBosCritSectFunctionName[ BOS_CRITSECT_INIT_FUNC_ID ]    = "bosCritSectInit";
   gBosCritSectFunctionName[ BOS_CRITSECT_TERM_FUNC_ID ]    = "bosCritSectTerm";
   gBosCritSectFunctionName[ BOS_CRITSECT_CREATE_FUNC_ID ]  = "bosCritSectCreate";
   gBosCritSectFunctionName[ BOS_CRITSECT_DESTROY_FUNC_ID ] = "bosCritSectDestroy";
   gBosCritSectFunctionName[ BOS_CRITSECT_ENTER_FUNC_ID ]   = "bosCritSectEnter";
   gBosCritSectFunctionName[ BOS_CRITSECT_LEAVE_FUNC_ID ]   = "bosCritSectLeave";

   bosErrorAddFunctionStrings( BOS_CRITSECT_MODULE, gBosCritSectFunctionName, BOS_CRITSECT_NUM_FUNC_ID );

} /* bosCritSectInitFunctioNames */

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */


/** @} */

#endif   /* BOS_CFG_CRITSECT */

