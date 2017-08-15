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
*  @file    boxErrorGeneric.c 
*
*  @brief   Contains OS independant error related functions.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosError.h>
#include <bosCfg.h>
#include <str.h>
#include <string.h>

#if BOS_CFG_MUTEX
#  include <bosMutex.h>
#endif

#if BOS_CFG_TASK
#  include <bosTask.h>
#endif

/* ---- Public Variables ------------------------------------------------- */

#if BOS_CFG_ERROR_FUNCTION_STRINGS

typedef struct
{
   int            numFuncIds; /**< Number of entries pointed to by @a funcStr.   */
   const char   **funcEntry;  /**< Array of function pointers to string names.   */

} BOS_ERROR_MODULE;

BOS_ERROR_MODULE gBosErrorModule[ BOS_NUM_MODULES ];

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */

/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup bosError
 * @{
 */
 
#if BOS_CFG_ERROR_FUNCTION_STRINGS

/***************************************************************************/
/**
*  Registers some function strings with the error module. These function
*  strings are used to translate a module ID/function ID into a readable
*  ASCII string.
*
*  @param   moduleId    
*  @param   funcStr     
*  @param   numFuncIds  (in)
*
*  @return  Description of return value.
*/

BOS_STATUS bosErrorAddFunctionStrings
(
   BOS_MODULE moduleId,    /**< (in) Module ID that function strings are being registered for.  */
   const char **funcEntry, /**< (in) Array of pointers to strings for each function.            */ 
   int numFuncIds          /**< (in) Number of entries in the array of pointers.                */
)
{
   BOS_ASSERT( moduleId < BOS_NUM_MODULES );

   gBosErrorModule[ moduleId ].funcEntry  = funcEntry;
   gBosErrorModule[ moduleId ].numFuncIds = numFuncIds;

   return BOS_STATUS_OK;

} /* bosErrorAddFunctionStrings */

#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */

/***************************************************************************/
/**
*  Translates an error source into a human readable ASCII string.
*
*  @param   errSrc      (in)  Numeric error source.
*  @param   paramName   (in)  Description
*
*  @return  Description of return value.
*/

void bosErrorFunctionStr( BOS_ERROR_SOURCE errSrc, char *outStr, size_t maxLen )
{
   BOS_MODULE        module = BOS_ERROR_MODULE( errSrc );
   BOS_FUNCTION_ID   funcId = BOS_ERROR_FUNC_ID( errSrc );
   
#if BOS_CFG_ERROR_FUNCTION_STRINGS
   const char       *funcStr = NULL;
   const char      **funcEntry;
   int               numFuncIds;

   if ( module < BOS_NUM_MODULES )
   {
      if (( funcEntry = gBosErrorModule[ module ].funcEntry ) != NULL )
      {
         numFuncIds = gBosErrorModule[ module ].numFuncIds;

         if ( funcId < numFuncIds )
         {
            funcStr = funcEntry[ funcId ];
         }
      }
   }

   if ( funcStr != NULL )
   {
      strPrintf( outStr, maxLen, "%.*s", maxLen, funcStr );
   }
   else
#endif   /* BOS_CFG_ERROR_FUNCTION_STRINGS */ 
   {
      strPrintf( outStr, maxLen, "Module:0x%02x, Function:0x%02x", module, funcId );
   }

} /* bosErrorFunctionStr */

/***************************************************************************/
/**
*  Initializes the BOS Error module.
*
*  @return  BOS_STATUS_OK if the initialization was successful, or an error
*           code if some type of error occured.
*/

BOS_STATUS bosErrorInit( void )
{
   memset( gBosErrorModule, 0, sizeof( gBosErrorModule ));

   return BOS_STATUS_OK;
}

/***************************************************************************/
/**
*  Terminates the BOS Error module.
*
*  @return  BOS_STATUS_OK if the termination was successful, or an error
*           code if some type of error occured.
*/

BOS_STATUS bosErrorTerm( void )
{
   return BOS_STATUS_OK;
}

/** @} */

