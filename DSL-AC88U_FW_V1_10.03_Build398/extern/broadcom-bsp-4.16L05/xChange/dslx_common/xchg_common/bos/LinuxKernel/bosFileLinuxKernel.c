/****************************************************************************
*
*     Copyright (c) 2001 Broadcom Corporation
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom Corporation
*           16251 Laguna Canyon Road
*           Irvine, California  92618
*
*     All information contained in this document is Broadcom Corporation
*     company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    bosFileLinuxUser.c
*
*  @brief   LinuxUser implementation of the BOS File Module.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <bosCfg.h>

#if BOS_CFG_FILE

#include <bosFile.h>
#include <bosError.h>
#include <bosErrorLinuxKernel.h>
#include <bosLog.h>
#include <bosFilePrivate.h>

/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
static   BOS_BOOL gBosFileInitialized = BOS_FALSE;
/* ---- Private Function Prototypes -------------------------------------- */
/* ---- Functions -------------------------------------------------------- */

/**
 * @addtogroup bosFile
 * @{
 */

/***************************************************************************/

BOS_STATUS bosFileInit( void )
{
   BOS_ASSERT( !gBosFileInitialized );

   bosFileInitFunctionNames();

   gBosFileInitialized = BOS_TRUE;

   return BOS_STATUS_OK;

} /* bosFileInit */

/***************************************************************************/

BOS_STATUS bosFileTerm( void )
{
   BOS_ASSERT( gBosFileInitialized );

   gBosFileInitialized = BOS_FALSE;

   return BOS_STATUS_OK;

} /* bosFileTerm */

/*
*****************************************************************************
 * Function Name: bosFileOpen
 * Description  : Opens a BOS_FILE
 * Arguments    : fileP - Pointer to an uninitializaed BOS_FILE object
                  fileName - Name of file to open
                  flags - File open option flags
 * Returns      : BOS_STATUS_OK	 = successful
 *                BOS_STATUS_ERR = failed
*****************************************************************************
*/
BOS_STATUS bosFileOpen
(    
   BOS_FILE *fileP,
   const char *fileName,   
   BOS_UINT32 flags
)
{
   mm_segment_t old_fs;
   int errNumber;

   /* Check if file already opened */
   if( fileP->fileHdl == NULL )
   {
      /* open file */
      old_fs = get_fs();
      set_fs( KERNEL_DS );
     
      
      fileP->fileHdl = filp_open(fileName, flags, 0644);
                          
      set_fs( old_fs );                          
                          
      if ( IS_ERR( fileP->fileHdl ) )
      {
         errNumber = PTR_ERR ( fileP->fileHdl );                      
           
         switch ( errNumber )
         {
            case (-ENOENT):
            {                             
                blogStrErr(( gBosBlog, "ERROR: bosFileOpen - file \'%s\' does not exist",fileName ));  
            }            
            break;
            
            default:
            {
                blogStrErr(( gBosBlog, "ERROR: bosFileOpen - open \'%s\' Failed with error %d!",fileName, errNumber ));
            }            
            break;
         }
            
         fileP->fileHdl = NULL;
         
         return BOS_STATUS_ERR;
      } 
      
      memset( fileP->fileName, '\0', BOS_FILE_NAME_STR_LEN +1 );      
      strncpy( fileP->fileName, fileName, BOS_FILE_NAME_STR_LEN );
      
      blogStrUsr(( gBosBlog, BOS_LOG_PARAMS, "bosFileOpen: Successfully opened file \'%s\' \n", fileName));              
      return BOS_STATUS_OK;
   }   
   else
   {
       blogStrErr(( gBosBlog, "ERROR: bosFileOpen - File Descriptor already in use for file \'%s\' \n", fileName));        
      return BOS_STATUS_ERR;
   }     
}

/*
*****************************************************************************
 * Function Name: bosFileClose
 * Description  : Closes a BOS_FILE
 * Arguments    : fileP - Pointer to a valid BOS_FILE object

 * Returns      : BOS_STATUS_OK	 = successful
 *                BOS_STATUS_ERR = failed
*****************************************************************************
*/
BOS_STATUS bosFileClose
(
   BOS_FILE *fileP
)
{
   mm_segment_t old_fs;
   if ( fileP->fileHdl )
   {
      old_fs = get_fs();
      set_fs( KERNEL_DS );     
      filp_close( fileP->fileHdl, NULL );
      set_fs( old_fs ); 
      fileP->fileHdl = NULL;
      
      blogStrUsr(( gBosBlog, BOS_LOG_PARAMS, "bosFileClose: Successfully closed file %s\n", fileP->fileName));        
      
      memset( fileP->fileName, '\0', BOS_FILE_NAME_STR_LEN+1 );
   }
   else
   {
       blogStrErr(( gBosBlog, "ERROR: bosFileClose - Uninitialized file descriptor for file %s \n", fileP->fileName));        
   }     
   
   return BOS_STATUS_OK; 
}

/*
*****************************************************************************
 * Function Name: bosFileRead
 * Description  : Reads data from a BOS_FILE.
 * Arguments    : fileP - Pointer to a valid BOS_FILE object
                  pData - Pointer to buffer to which data is to be written
                  numBytes - Number of bytes to read
                  
 * Returns      : Number of bytes Wrutten
*****************************************************************************
*/
BOS_UINT32 bosFileRead
(
   BOS_FILE *fileP,
   char * pData,
   BOS_UINT32 numBytes
)
{
   mm_segment_t old_fs;
   int errNumber;
   BOS_SINT32 numBytesRead = 0;

   if( fileP->fileHdl )
   {
      old_fs = get_fs();
      set_fs(KERNEL_DS);           
      numBytesRead = vfs_read( fileP->fileHdl, pData, numBytes, &(fileP->fileHdl->f_pos));     
      set_fs(old_fs);            
      
      if( numBytesRead < 0 )
      {
         errNumber = numBytesRead;                      
           
         switch ( errNumber )
         {           
            case (-EBADF):
            {                             
                blogStrErr(( gBosBlog, "ERROR: bosFileRead - Bad file number for file %s", fileP->fileName ));  
            }            
            break;
            
            case (-EINVAL):
            {                             
                blogStrErr(( gBosBlog, "ERROR: bosFileRead - Invalid arguments for file %s", fileP->fileName ));  
            }            
            break;
            
            case (-EFAULT):
            {                             
                blogStrErr(( gBosBlog, "ERROR: bosFileRead - Bad address for file %s", fileP->fileName ));  
            }            
            break;
            
            default:
            {
                blogStrErr(( gBosBlog, "ERROR: bosFileRead - Failed with error %d! for file %s", errNumber, fileP->fileName ));
            }            
            break;
         }
         
         /* Indicate that no bytes were written */
         numBytesRead = 0;
      }       
   }
   else
   {
       blogStrErr(( gBosBlog, "ERROR: bosFileRead - Uninitialized file descriptor\n"));              
   }
   
   return numBytesRead;   
}

/*
*****************************************************************************
 * Function Name: bosFileWrite
 * Description  : Writes data to the BOS_FILE.
 * Arguments    : fileP - Pointer to a valid BOS_FILE object
                  pData - Pointer to the data to be written
                  numBytes - Number of bytes to write
                  
 * Returns      : Number of bytes Wrutten
*****************************************************************************
*/
BOS_SINT32 bosFileWrite
( 
   BOS_FILE *fileP,
   char * pData,
   BOS_UINT32 numBytes
)
{
   mm_segment_t old_fs;
   int errNumber;
   BOS_SINT32 numBytesWritten = 0;

   if( fileP->fileHdl )
   {
      old_fs = get_fs();
      set_fs(KERNEL_DS);           
      numBytesWritten = vfs_write( fileP->fileHdl, pData, numBytes, &(fileP->fileHdl->f_pos));     
      set_fs(old_fs); 

      if( numBytesWritten < 0 )
      {
         errNumber = numBytesWritten;                      
           
         switch ( errNumber )
         {           
            case (-EBADF):
            {                             
                blogStrErr(( gBosBlog, "ERROR: bosFileWrite - Bad file number for file %s", fileP->fileName ));  
            }            
            break;
            
            case (-EINVAL):
            {                             
                blogStrErr(( gBosBlog, "ERROR: bosFileWrite - Invalid arguments for file %s", fileP->fileName ));   
            }            
            break;
            
            case (-EFAULT):
            {                             
                blogStrErr(( gBosBlog, "ERROR: bosFileWrite - Bad address for file %s", fileP->fileName ));  
            }            
            break;
            
            default:
            {
                blogStrErr(( gBosBlog, "ERROR: bosFileWrite - Failed with error %d! for file %s", errNumber, fileP->fileName ));
            }            
            break;
         }
         
         /* Indicate that no bytes were written */
         numBytesWritten = 0;
      }       
   }
   else
   {
       blogStrErr(( gBosBlog, "ERROR: bosFileWrite - Uninitialized file descriptor for file %s", fileP->fileName ));    
   }
   
   return numBytesWritten;
}


/*
*****************************************************************************
 * Function Name: bosFileSeek
 * Description  : Seek to specific offset in a BOS_FILE
 * Arguments    : fileP  - Pointer to a valid BOS_FILE object
                  offset - Offset to seek to
                  whence - seek mode ( see BOS_FILE_SEEK_XXX modes )
 * Returns      : BOS_STATUS_OK if succeeded
*****************************************************************************
*/
BOS_STATUS bosFileSeek
(    
   BOS_FILE *fileP,
   BOS_SINT64 offset,   
   BOS_SINT32 whence
)
{
   mm_segment_t old_fs;
   int errNumber = 0;
   BOS_SINT64 newPosition = 0;   
   BOS_STATUS status = BOS_STATUS_ERR;

   if( fileP->fileHdl )
   {
      old_fs = get_fs();
      set_fs(KERNEL_DS);           
      newPosition = vfs_llseek( fileP->fileHdl, offset, whence);     
      set_fs(old_fs); 
      
      if ( newPosition < 0 )
      {          
         errNumber = newPosition;
         switch ( errNumber )
         {
            case (-ESPIPE):
            {                             
                blogStrErr(( gBosBlog, "ERROR: bosFileSeek - file \'%s\' does not support SEEK functionality",fileP->fileName ));  
            }            
            break;
            
            default:
            {
                blogStrErr(( gBosBlog, "ERROR: bosFileSeek - file \'%s\' Failed with error %d!",fileP->fileName, errNumber ));
            }            
            break;
         }
                     
         status = BOS_STATUS_ERR;
      }       
      else
      {
         status = BOS_STATUS_OK;
      }
   } 
   else
   {
       blogStrErr(( gBosBlog, "ERROR: bosFileSeek - Uninitialized file descriptor for file %s", fileP->fileName ));    
       status = BOS_STATUS_ERR;
   }
   
   return status;      
}

/** @} */


#endif      /* BOS_CFG_FILE */
