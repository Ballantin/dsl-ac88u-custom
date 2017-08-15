/***************************************************************************
*
*     Copyright (c) 2002 Broadcom Corporation, All Rights Reserved.
*     Contains proprietary and confidential information.
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
****************************************************************************
*
*    Description:
*      This file contains the implementation for the remote file server 
*      module to be used with the blog module.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------- */
#include "rfsBlogClient.h"
#include "rfsCommon.h"
#include "bosError.h"
#include "str.h"
#include "bosSocket.h"


/* ---- Private Constants and Types -------------------------- */

/* The maximum data length for each packet sent to the server
 * Each packet contains a RFS header followed by the data.
 */
#define  MSG_DATA_SIZE        1000
#define  MSG_TOTAL_SIZE   ( RFS_MSG_HEADER_SIZE + MSG_DATA_SIZE )

/* ---- Private Variables ------------------------------------ */

static BOS_BOOL      rfsInitialized = BOS_FALSE;
static BOS_SOCKET    rfsSocket;
static BOS_UINT8     rfsFileHandle = 1;
static BOS_UINT8     rfsFileSeqNum = 0;
static BOS_BOOL      logFileOpen = BOS_FALSE;

/* 
 * Buffer for packets sent to RFS.  
 * Data is written to the buffer until it's full or the file is closed.
 * The current position in the buffer is tracked by the writePos index.
 */
static char          sendBufp[MSG_TOTAL_SIZE];
static BOS_UINT16    writePos  = 0;

/* ---- Private Function Prototypes -------------------------- */

BOS_UINT16 sendRfsMsg( BOS_UINT8 cmd, RFS_MSG *msg, size_t dataLen );

/* ---- Functions -------------------------------------------- */

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
BOS_BOOL rfsBlogInit( const char* serverName )
{
   BOS_SOCKET_IP_ADDR   rfsServerIpAddr;
   BOS_SOCKET_IP_PORT   rfsServerIpPort = RFS_SERVERPORT;
   char                 errStr[ 100 ];

   if ( rfsInitialized )
   {
      return BOS_TRUE;
   }
   else
   {
      /*
       * Translate the servername into an IP address and port number.
       */

      if ( bosSocketLookupServerAndPort( serverName, 
                       &rfsServerIpAddr, &rfsServerIpPort,
                          errStr, sizeof( errStr )) != BOS_STATUS_OK )
      {
         printf( "Unable to translate '%s' into an IP address and port: %s\n", 
                   serverName, errStr );

         return BOS_FALSE;
      }

      /*
       * Connect to the RFS Server
       */

      if ( bosSocketCreateStream( &rfsSocket ) != BOS_STATUS_OK )
      {
         printf( "Unable to create socket for RFS Server communications.\n" );
         return BOS_FALSE;
      }

      if ( bosSocketConnectIp( &rfsSocket, rfsServerIpAddr, rfsServerIpPort ) 
              != BOS_STATUS_OK )
      {
         printf( "Unable to connect to RFS Server '%s' on port %d.\n", 
                   serverName, rfsServerIpPort );
         printf( "Check to see if the RFS Server is running.\n" );
         return BOS_FALSE;
      }
      
      /* Initialize all variables */
      rfsFileSeqNum = 0;
      logFileOpen = BOS_FALSE;
      writePos  = 0;
      rfsInitialized = BOS_TRUE;

      return BOS_TRUE;
   }
}

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
void rfsBlogTerm( void )
{

   BOS_ASSERT(rfsInitialized);

   rfsInitialized = BOS_FALSE;

   /* Close log file if open */
   if (logFileOpen == BOS_TRUE)
   {
      rfsBlogClose();
   }
   
   bosSocketClose( &rfsSocket );
   rfsSocket = BOS_SOCKET_INVALID;
   
}

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
BOS_BOOL rfsBlogOpen( const char *fileName )
{

   BOS_UINT16  fileNameLen;
   BOS_UINT16  dataLen;
   char        mode = 'w';
   RFS_MSG    *openMsg;
   BOS_UINT16 bytesSent = 0;

   BOS_ASSERT(rfsInitialized);
     
   if ( logFileOpen == BOS_TRUE )
   {
      /* close the currently open file first */
      if ( rfsBlogClose() == BOS_FALSE )
      {
         return BOS_FALSE;
      }
   }

   /* Check the lengths of the strings. Note the '+1' is for the '\0' */
   fileNameLen = strlen( fileName ) + 1;
   dataLen = fileNameLen + strlen( &mode ) + 1;
   BOS_ASSERT( dataLen <= MSG_DATA_SIZE );

   /* Copy the file name and the mode to the send buffer */
   openMsg = (RFS_MSG*)&sendBufp[0];
   strMaxCpy( openMsg->data, fileName, MSG_DATA_SIZE );
   strMaxCpy( &( openMsg->data[ fileNameLen ] ), &mode, 
               MSG_DATA_SIZE - fileNameLen );

   /* Send the open request and check for success */
   bytesSent = sendRfsMsg(RFS_FILE_OPEN, openMsg, dataLen);
   if ( bytesSent > 0)
   {
      logFileOpen = BOS_TRUE;
      return BOS_TRUE;
   }
   else
   {
      return BOS_FALSE;
   }
}

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
int rfsBlogWrite( const char* data, BOS_UINT16 length )
{
   RFS_MSG    *writeMsg;
   BOS_UINT16 bytesSent = 0;

   BOS_ASSERT(rfsInitialized);
     
   if (logFileOpen == BOS_FALSE)
   {
      printf("Cannot write to RFS - no log file open\n");
      return 0;
   }
   
   BOS_ASSERT( length <= MSG_DATA_SIZE );
   writeMsg = (RFS_MSG*) &(sendBufp[0]);   
   strMaxCpy( writeMsg->data, data, MSG_DATA_SIZE );
   bytesSent = sendRfsMsg(RFS_FILE_WRITE, writeMsg, length);
   return bytesSent;
   
}

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
BOS_BOOL rfsBlogClose( void )
{

   RFS_MSG    *closeMsg;

   BOS_ASSERT(rfsInitialized);
     
   if (logFileOpen == BOS_FALSE)
   {
      return BOS_TRUE;
   }


   /* Send the message and check the number of bytes sent */
   closeMsg = (RFS_MSG*) &(sendBufp[0]);      
   if ( sendRfsMsg(RFS_FILE_CLOSE, closeMsg, 0) > 0 )
   {
      logFileOpen = BOS_FALSE;
      return BOS_TRUE;
   }
   else
   {
      return BOS_FALSE;
   }    

}

/*****************************************************************************
*
*  FUNCTION:   sendRfsMsg
*
*  PURPOSE:    Creates and sends a RFS message.  The RFS message header
*              is created and added on to the message length.
*
*  PARAMETERS: 
*          cmd     - The type of message to be sent
*          msg     - The RFS message with the data already stored.
*          dataLen - The length of the data in msg.
*
*  RETURNS:    The number of bytes sent.
*
*  NOTES:      
*
*****************************************************************************/
BOS_UINT16 sendRfsMsg( BOS_UINT8 cmd, RFS_MSG *msg, size_t dataLen )
{
   BOS_UINT16  msgLen = RFS_MSG_HEADER_SIZE + dataLen;
   size_t      bytesSent;

   msg->length  = bosSocketHtoNS16( msgLen ); /* REVISIT */
   msg->command = cmd;
   msg->handle  = rfsFileHandle;
   msg->seqnum  = rfsFileSeqNum++;

   if ( rfsSocket != BOS_SOCKET_INVALID )
   {
#if 0
      /* Debug printing */
      printf( "SendRfsMsg: Len:%d Cmd:%d H:%d S:%d\n",
                msg->length, msg->command, msg->handle, msg->seqnum );
      if (cmd != RFS_FILE_CLOSE )
      {
         printf( "Data: %s\n", msg->data );
      }
#endif
      if ( bosSocketSend( &rfsSocket, msg, msgLen, 0, &bytesSent ) 
                           != BOS_STATUS_OK )
      {
         printf( "*** Unable to send message to RFS file ***\n" );
         printf( "*** RFS server has probably terminated ***\n" );

         bosSocketClose( &rfsSocket );
         rfsSocket = BOS_SOCKET_INVALID;
      }
   }

   return bytesSent;
}
