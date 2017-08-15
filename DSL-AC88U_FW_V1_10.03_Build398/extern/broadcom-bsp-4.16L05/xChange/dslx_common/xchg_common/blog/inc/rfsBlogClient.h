/***************************************************************************
*
*     Copyright (c) 2002 Broadcom, All Rights Reserved.
*     Contains proprietary and confidential information.
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           P.O. Box 57013
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Description:
*      This file contains the API for the remote file server module to
*      be used with the blog module.
*
*      Note: This implementation works with the RFS implementation (V1) in 
*            $/Test Code/RFS_Server in SourceSafe.
*
*            This module should be improved to collect log messages and 
*            and form larger packets before sending them to the RFS.
*
****************************************************************************/

#ifndef _RFSBLOGCLIENT_H
#define _RFSBLOGCLIENT_H

/* ---- Include Files ---------------------------------------- */
#include "bosTypes.h"

/* ---- Constants and Types ---------------------------------- */
/* ---- Variable Externs ------------------------------------- */

/* ---- Function Prototypes ---------------------------------- */

/*****************************************************************************
*
*  FUNCTION:   rfsBlogInit
*
*  PURPOSE:    Initializates the RFS module
*
*  PARAMETERS:
*          serverName - pointer to IP address of RFS in string format
*
*  RETURNS:    BOS_TRUE or BOS_FALSE on error
*
*  NOTES:
*
*****************************************************************************/
BOS_BOOL rfsBlogInit( const char* serverName );

/*****************************************************************************
*
*  FUNCTION:   rfsBlogTerm
*
*  PURPOSE:    Terminates the RFS module
*
*  PARAMETERS: 
*
*  RETURNS:    
*
*  NOTES:
*
*****************************************************************************/
void rfsBlogTerm( void );

/*****************************************************************************
*
*  FUNCTION:   rfsBlogOpen
*
*  PURPOSE:    Sends a file open request to the remote server.  
*
*  PARAMETERS: 
*          fileName - pointer to name of the file to open
*
*  RETURNS:    BOS_TRUE or BOS_FALSE on error
*
*  NOTES:      If a file was previously opened, it will be closed before
*              the new file is opened.
*
*****************************************************************************/
BOS_BOOL rfsBlogOpen( const char *fileName );

/*****************************************************************************
*
*  FUNCTION:   rfsBlogWrite
*
*  PURPOSE:    Writes data to the currently open file.  
*
*  PARAMETERS: 
*          data   - the string to be written to the file
*          length - length of data
*
*  RETURNS:    The number of characters sent to the remote file server
*
*  NOTES:      If a file is not already open, 0 will be returned.
*
*****************************************************************************/
int rfsBlogWrite( const char* data, BOS_UINT16 length );

/*****************************************************************************
*
*  FUNCTION:   rfsBlogClose
*
*  PURPOSE:    Sends a file close request to the remote file server.
*
*  PARAMETERS: 
*
*  RETURNS:    BOS_TRUE or BOS_FALSE on error
*
*  NOTES:      
*
*****************************************************************************/
BOS_BOOL rfsBlogClose( void );

#endif /* RFSBLOGCLIENT_H */
