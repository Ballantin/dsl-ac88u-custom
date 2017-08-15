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
*    Filename: debugIpc.h
*
****************************************************************************
*    Description:
*
*     Interface for IPC debug module.
*
****************************************************************************/

#ifndef DEBUG_IPC_H
#define DEBUG_IPC_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */
/* ---- Constants and Types ---------------------------------------------- */
/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

/*
*****************************************************************************
** FUNCTION:   debugIpcInit
**
** PURPOSE:    Initialize debug IPC module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void debugIpcInit( void );


/*
*****************************************************************************
** FUNCTION:   debugIpcDeinit
**
** PURPOSE:    De-initialize debug IPC module.
**
** PARAMETERS: None.
**
** RETURNS:    Nothing.
**
** NOTE:
*****************************************************************************
*/
void debugIpcDeinit( void );



#ifdef __cplusplus
    }
#endif

#endif  /* DEBUG_IPC_H  */
