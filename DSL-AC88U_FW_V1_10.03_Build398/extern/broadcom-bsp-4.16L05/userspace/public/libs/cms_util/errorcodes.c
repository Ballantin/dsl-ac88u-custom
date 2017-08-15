/***********************************************************************
 *
<:copyright-BRCM:2010:DUAL/GPL:standard

   Copyright (c) 2010 Broadcom 
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
 ************************************************************************/

#include "cms.h"
#include "cms_util.h"


const char *cmsErr_getStringBuf(CmsRet ret, char *buf, UINT32 buflen)
{
   char *errorStr;

   switch(ret)
   {
   case CMSRET_SUCCESS:
      errorStr = "Success";
      break;

   case CMSRET_METHOD_NOT_SUPPORTED:
      errorStr = "Method not supported";
      break;

   case CMSRET_REQUEST_DENIED:
      errorStr = "Request denied";
      break;

   case CMSRET_INTERNAL_ERROR:
      errorStr = "Internal Error";
      break;

   case CMSRET_INVALID_ARGUMENTS:
      errorStr = "Invalid arguments";
      break;

   case CMSRET_RESOURCE_EXCEEDED:
      errorStr = "Resource exceeded";
      break;

   case CMSRET_INVALID_PARAM_NAME:
      errorStr = "Invalid Parameter Name";
      break;

   case CMSRET_INVALID_PARAM_TYPE:
      errorStr = "Invalid parameter type";
      break;

   case CMSRET_INVALID_PARAM_VALUE:
      errorStr = "Invalid parameter value";
      break;

   case CMSRET_SET_NON_WRITABLE_PARAM:
      errorStr = "Attempt to set a non-writeable parameter";
      break;

   case CMSRET_NOTIFICATION_REQ_REJECTED:
      errorStr = "Notification request rejected";
      break;

   case CMSRET_DOWNLOAD_FAILURE:
      errorStr = "Download failure";
      break;

   case CMSRET_UPLOAD_FAILURE:
      errorStr = "Upload failure";
      break;

   case CMSRET_FILE_TRANSFER_AUTH_FAILURE:
      errorStr = "File transfer server authentication failure";
      break;

   case CMSRET_UNSUPPORTED_FILE_TRANSFER_PROTOCOL:
      errorStr = "Unsupported protocol for file transfer";
      break;

   case CMSRET_FILE_TRANSFER_UNABLE_JOIN_MULTICAST:
      errorStr = "File transfer failure: unable to join multicast group";
      break;

   case CMSRET_FILE_TRANSFER_UNABLE_CONTACT_FILE_SERVER:
      errorStr = "File transfer failure: unable to contact file server";
      break;
   case CMSRET_FILE_TRANSFER_UNABLE_ACCESS_FILE:
      errorStr = "File transfer failure: unable to access file";
      break;

   case CMSRET_FILE_TRANSFER_UNABLE_COMPLETE:
      errorStr = "File transfer failure: unable to complete download";
      break;

   case CMSRET_FILE_TRANSFER_FILE_CORRUPTED:
      errorStr = "File transfer failure: file corrupted";
      break;

   case CMSRET_FILE_TRANSFER_FILE_AUTHENTICATION_ERROR:
      errorStr = "File transfer failure: file authentication error";
      break;

   case CMSRET_FILE_TRANSFER_FILE_TIMEOUT:
      errorStr = "File transfer failure: download timeout";
      break;

   case CMSRET_FILE_TRANSFER_FILE_CANCELLATION_NOT_ALLOW:
      errorStr = "File transfer failure: cancellation not permitted";
      break;

   case CMSRET_INVALID_UUID_FORMAT:
      errorStr = "Invalid UUID format";
      break;

   case CMSRET_UNKNOWN_EE:
      errorStr = "Unknown execution environment";
      break;

   case CMSRET_EE_DISABLED:
      errorStr = "Execution environment disabled";
      break;

   case CMSRET_DU_EE_MISMATCH:
      errorStr = "Execution environment and deployment unit mismatch";
      break;

   case CMSRET_DU_DUPLICATE:
      errorStr = "Duplicate deployment unit";
      break;

   case CMSRET_SW_MODULE_SYSTEM_RESOURCE_EXCEEDED:
      errorStr = "Software modules system resources exceeded";
      break;

   case CMSRET_DU_UNKNOWN:
      errorStr = "Unknown deployment unit";
      break;

   case CMSRET_DU_STATE_INVALID:
      errorStr = "Invalid deployment unit state";
      break;

   case CMSRET_DU_UPDATE_DOWNGRADE_NOT_ALLOWED:
      errorStr = "Invalid deployment unit update: downgrade not permitted";
      break;

   case CMSRET_DU_UPDATE_VERSION_NOT_SPECIFIED:
      errorStr = "Invalid deployment unit update: version not specified";
      break;

   case CMSRET_DU_UPDATE_VERSION_EXISTED:
      errorStr = "Invalid deployment unit update: version already exists";
      break;

   default:
      errorStr = NULL;
      break;
   }

   if (errorStr)
   {
      cmsUtl_strncpy(buf, errorStr, buflen);
   }
   else
   {
      int rc;
      if (ret >= 9800 && ret <= 9899)
      {
         rc = snprintf(buf, buflen, "vendor defined fault code %d", ret);
      }
      else
      {
         rc = snprintf(buf, buflen, "no string available for code %d", ret);
      }
      if (rc >= (int) buflen)
      {
         cmsLog_error("buffer provided is too small (%d) for error string, truncated", buflen);
      }
   }

   return buf;
}

static char global_error_buf[BUFLEN_512];

const char *cmsErr_getString(CmsRet ret)
{
   return (cmsErr_getStringBuf(ret, global_error_buf, sizeof(global_error_buf)));
}
