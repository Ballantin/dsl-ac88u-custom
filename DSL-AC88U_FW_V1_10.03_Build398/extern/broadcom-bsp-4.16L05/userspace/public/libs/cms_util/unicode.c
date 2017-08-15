/***********************************************************************
 *
 *  Copyright (c) 2008  Broadcom Corporation
 *  All Rights Reserved
 *
<:label-BRCM:2012:DUAL/GPL:standard

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as published by
the Free Software Foundation (the "GPL").

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.


A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.

:>
 * 
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>                                                              
                                                                                
#include "cms.h"
#include "cms_mem.h"
#include "cms_log.h"


// chec if string should contains XML Unicode characters
// &#xhhhh; or &#dddd, where h is any hex character [0-9,a-f]
// and d is any decimal digit [0-9]
// such as &#x4A; or &#67;
// This function will look for any number of digits, but only 8 bit unicode
// is supported right now.
UBOOL8 cmsUnicode_isUnescapeNeeded
   (const char *string)
{
   char *pStart = NULL, *pChar = NULL;
   UBOOL8 found = FALSE;

   if (string == NULL)
      return found;

   pStart = (char *)string;

   while (found == FALSE)
   {
      if ((pChar = strstr(pStart, "&#")) != NULL)
      {
         for (pChar += 2;
              found == FALSE && *pChar != '&' && *pChar != '\0';
              pChar++)
         {
            if (*pChar == ';')
               found = TRUE;
         }

         pStart = pChar;
      }
      else
      {
         break;
      }
   }

   return found;
}

static CmsRet consumeUnicode
   (const char *string, unsigned char *pConvertedChar, UINT32 *offset)
{
   char *tmpStr = NULL;
   char *pSemi = NULL, *pAmp = NULL;
   UINT16 num = 0;
   UINT32 i = 0;
   CmsRet ret = CMSRET_SUCCESS;
   
   if (string == NULL)
   {
      return CMSRET_INVALID_PARAM_VALUE;
   }

   // See valid format strings above
   tmpStr = cmsMem_strdup(string);

   *pConvertedChar = tmpStr[i];
   *offset = 1;

   if (tmpStr[i] == '&' && tmpStr[i+1] == '#')
   {
      // tmpStr = '&#x;' ==>  invalid number to convert
      if (tmpStr[i+2] == 'x' && tmpStr[i+3] == ';')
      {
         ret = CMSRET_INVALID_PARAM_VALUE;
         cmsLog_error("invalid format, skipping data");
         *offset = 4;
      }
      // tmpStr = '&#;' ==> invalid number to convert
      else if (tmpStr[i+2] == ';')
      {
         ret = CMSRET_INVALID_PARAM_VALUE;
         cmsLog_error("invalid format, skipping data");
         *offset = 3;
      }
      else
      {
         pSemi = strstr(&tmpStr[i+2], ";");
         if (pSemi != NULL)
         {
            pAmp = strstr(&tmpStr[i+2], "&");
            if (pAmp == NULL || pAmp > pSemi)
            {
               *pSemi = '\0';
               if (tmpStr[i+2] == 'x')
                  num = strtoul(&tmpStr[i+3], (char **)NULL, 16);
               else
                  num = strtoul(&tmpStr[i+2], (char **)NULL, 10);
               // TO-DO: need to take care unicode 16 (2 bytes)
               // right now only take care unicode 8 (1 byte)
               if (num > 127)
               {
                  cmsLog_error("multi-byte unicode not supported. "
                              "expected num <= 127, got num %d", num);
               }
               *pConvertedChar = (unsigned char) btowc(num);
               *offset = (pSemi - tmpStr) + 1;
            }
         }
      }
   }

   CMSMEM_FREE_BUF_AND_NULL_PTR(tmpStr);

   return ret;
}

CmsRet cmsUnicode_unescapeString
   (const char *string, char **unicodedString)
{
   char *tmpStr = NULL;
   unsigned char convertedChar;
   UINT32 len = 0, i = 0, j = 0, offset = 0;
   CmsRet ret = CMSRET_SUCCESS;

   if (string == NULL)
   {
      return ret;
   }

   len = strlen(string);

   if ((tmpStr = cmsMem_alloc(len, ALLOC_ZEROIZE)) == NULL)
   {
      cmsLog_error("failed to allocate %d bytes", len);
      return CMSRET_RESOURCE_EXCEEDED;
   }

   while (i < len)
   {
      ret = consumeUnicode(&string[i], &convertedChar, &offset);
      // note, only 8 bit unicode is suported right now, so we can only
      // get at most 1 byte convertedChar
      if (ret == CMSRET_SUCCESS)
         tmpStr[j++] = convertedChar;

      i += offset;
   }
   
   *unicodedString = tmpStr;

   return ret;
}

