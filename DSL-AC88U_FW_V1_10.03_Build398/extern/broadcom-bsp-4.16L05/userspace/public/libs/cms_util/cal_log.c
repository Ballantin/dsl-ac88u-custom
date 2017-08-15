/******************************************************************************
 *  
<:copyright-BRCM:2015:DUAL/GPL:standard

   Copyright (c) 2015 Broadcom 
   All Rights Reserved

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
******************************************************************************/

#include <stdarg.h>
#include <stdio.h>
#include "cms_actionlog.h"

#define CAL_LOG_FILE    "/var/cal.log"

inline void printlog( const char *buf )
{
   FILE *fd = fopen(CAL_LOG_FILE, "a+");

   if (fd == NULL)
   {
      fprintf(stderr, "callog error: cannot open %s", CAL_LOG_FILE);
      fflush(stderr);
   }
   else
   {
      fprintf(fd, "%s", buf);
      fclose(fd);
   }
}

void callog_log(CmsActionLogSrc src __attribute__((unused)), const char *pFmt, ... )
{
   va_list		ap;
   char buf[MAX_ACTIONLOG_LINE_LENGTH] = {0};
   int len=0, maxLen;

   maxLen = sizeof(buf);

   va_start(ap, pFmt);

#if 0
   {
      CmsTimestamp ts;

      cmsTms_get(&ts);
   }
#endif

   if (len < maxLen)
   {
      maxLen -= len;
      vsnprintf(&buf[len], maxLen, pFmt, ap);
   }

   printlog(buf);

   va_end(ap);
}  /* End of callog_log() */
