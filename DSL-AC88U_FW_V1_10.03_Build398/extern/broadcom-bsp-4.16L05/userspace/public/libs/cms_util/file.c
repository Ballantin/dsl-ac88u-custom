/***********************************************************************
 *
 *  Copyright (c) 2007  Broadcom Corporation
 *  All Rights Reserved
 *
 * <:label-BRCM:2012:DUAL/GPL:standard
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, version 2, as published by
 * the Free Software Foundation (the "GPL").
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * 
 * A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
 * writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 * 
 * :>
 * 
 ************************************************************************/

#include "cms.h"
#include "cms_util.h"
#include "oal.h"

UBOOL8 cmsFil_isFilePresent(const char *filename)
{
   return (oalFil_isFilePresent(filename));
}


SINT32 cmsFil_getSize(const char *filename)
{
   return (oalFil_getSize(filename));
}


CmsRet cmsFil_copyToBuffer(const char *filename, UINT8 *buf, UINT32 *bufSize)
{
   return (oalFil_copyToBuffer(filename, buf, bufSize));
}

CmsRet cmsFil_writeToProc(const char *procFilename, const char *s)
{
   return (oalFil_writeToProc(procFilename, s));
}

CmsRet cmsFil_writeBufferToFile(const char *filename, const UINT8 *buf,
                                UINT32 bufLen)
{
   return (oalFil_writeBufferToFile(filename, buf, bufLen));
}

CmsRet cmsFil_removeDir(const char *dirname)
{
   return (oalFil_removeDir(dirname));
}

CmsRet cmsFil_makeDir(const char *dirname)
{
   return (oalFil_makeDir(dirname));
}

CmsRet cmsFil_getOrderedFileList(const char *dirname, DlistNode *dirHead)
{
   return (oalFil_getOrderedFileList(dirname, dirHead));
}


void cmsFil_freeFileList(DlistNode *dirHead)
{
   DlistNode *tmp = dirHead->next;

   while (tmp != dirHead)
   {
      dlist_unlink(tmp);
      cmsMem_free(tmp);
      tmp = dirHead->next;
   }
}


UINT32 cmsFil_scaleSizetoKB(long nblks, long blkSize)
{

	return (nblks * (long long) blkSize + KILOBYTE/2 ) / KILOBYTE;

}

CmsRet cmsFil_renameFile(const char *oldName, const char *newName)
{
   return(oalFil_renameFile(oldName,newName));
}

CmsRet cmsFil_getNumFilesInDir(const char *dirname, UINT32 *num)
{
   return(oalFil_getNumFilesInDir(dirname, num));
}

UBOOL8 cmsFil_isDirPresent(const char *dirname)
{
   return (oalFil_isDirPresent(dirname));
}

CmsRet cmsFil_getNumericalOrderedFileList(const char *dirname, DlistNode *dirHead)
{
   return (oalFil_getNumericalOrderedFileList(dirname, dirHead));
}

CmsRet cmsFil_getIntPrefixFromFileName(char *fileName, UINT32 *pNum)
{
   return(oal_getIntPrefixFromFileName(fileName,pNum));
}

CmsRet cmsFil_readFirstlineFromFile(char *fileName, char *line, UINT32 lineSize)
{
   return (oalFil_readFirstlineFromFile(fileName, line, lineSize));
}

CmsRet cmsFil_readFirstlineFromFileWithBasedir(char *baseDir, char *fileName, char *line, UINT32 lineSize)
{
   char fileNameBuf[BUFLEN_64];

   snprintf(fileNameBuf, sizeof(fileNameBuf), "%s/%s", baseDir, fileName);

   return cmsFil_readFirstlineFromFile(fileNameBuf, line, lineSize);   
}

