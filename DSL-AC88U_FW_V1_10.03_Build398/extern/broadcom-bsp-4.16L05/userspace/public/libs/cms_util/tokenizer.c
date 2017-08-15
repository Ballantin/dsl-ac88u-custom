/***********************************************************************
 *
 *  Copyright (c) 2012  Broadcom Corporation
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

#define _GNU_SOURCE
#include <stdio.h>
// extern ssize_t getline(char **lineptr, size_t *n, FILE *stream);

#include "cms.h"
#include "cms_util.h"
#include "oal.h"





static UBOOL8 is_terminal_char(UBOOL8 is_quoted, char c)
{
   if (is_quoted && c == '"')
      return TRUE;
   else if (!is_quoted && isspace(c))
      return TRUE;
   return FALSE;
}

UBOOL8 cmsTok_tokenizeLine(const char *line, UINT32 len,
                           const struct cms_token_map_entry *token_map,
                           struct cms_token_value_pair *pair)
{
   UBOOL8 found=FALSE;
   UINT32 i=0;

   /* skip leading whitespace */
   while (i < len && isspace(line[i]))
         i++;

   /* grab symbol */
   if (i < len)
   {
      UINT32 j=0;
      char tmpfield[128]={0};
      while (!isspace(line[i]) && line[i] != '=' && i < len && j < sizeof(tmpfield)-1)
      {
         tmpfield[j] = line[i];
         j++;
         i++;
      }
      for (j=0; token_map[j].keyword; j++)
      {
         if (!strcasecmp(tmpfield, token_map[j].keyword))
         {
            pair->token = token_map[j].token;
            // printf("field %s => token %d\n", tmpfield, pair->token);
            found = TRUE;
            break;
         }
      }

      if (!found)
      {
         cmsLog_error("Unrecognized token %s", tmpfield);
         return found;
      }
   }


   /* skip middle whitespace, including = sign */
   while (i < len && (isspace(line[i]) || line[i] == '='))
      i++;

   /* grab value string */
   if (i < len)
   {
      UINT32 j=0;
      UBOOL8 is_quoted=FALSE;
      if (line[i] == '"')
      {
         is_quoted = TRUE;
         i++;
      }

      while (i < len && j < sizeof(pair->valuebuf) &&
             !is_terminal_char(is_quoted, line[i]))
      {
         pair->valuebuf[j] = line[i];
         j++;
         i++;
      }
      // printf("valuebuf(%d)==>%s<==\n", j, pair->valuebuf);
   }

   return found;
}


UBOOL8 cmsTok_isDataLine(const char *buf, int len)
{
   int i=0;

   if (len > 0 && buf[0] == '#')
   {
      return FALSE;
   }

   for (i=0; i < len; i++)
   {
      if (!isspace(buf[i]))
         return TRUE;
   }

   return FALSE;
}


UBOOL8 cmsTok_getNextDataLine(FILE *fp, char *line, UINT32 *len)
{
   char *buf=NULL;
   ssize_t cnt=0;
   size_t n=0;

   while ((cnt = getline(&buf, &n, fp)) != -1)
   {
      if (cmsTok_isDataLine(buf, (int) cnt))
      {
         cmsUtl_strncpy(line, buf, (SINT32) *len);
         *len = (UINT32) n;
         free(buf);
         return 1;
      }
      free(buf);
      buf = NULL;
      n = 0;
   }

   return 0;
}

