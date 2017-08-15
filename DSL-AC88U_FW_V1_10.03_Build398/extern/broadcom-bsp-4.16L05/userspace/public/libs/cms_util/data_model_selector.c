/***********************************************************************
 *
 *  Copyright (c) 2006-2007  Broadcom Corporation
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

#include "cms.h"
#include "cms_util.h"


void cmsUtil_toggleDataModel(void)
{
#if defined(SUPPORT_DM_LEGACY98) || defined(SUPPORT_DM_HYBRID) ||  defined(SUPPORT_DM_PURE181)
   cmsLog_error("this function is only valid in DM Detect mode");
#elif defined(SUPPORT_DM_DETECT)
   SINT32 rv;
   CmsRet ret;
   UINT8 dmc[CMS_DATA_MODEL_PSP_VALUE_LEN]={0};

   rv = cmsPsp_get(CMS_DATA_MODEL_PSP_KEY, dmc, sizeof(dmc));
   if (rv != CMS_DATA_MODEL_PSP_VALUE_LEN)
   {
      /* No PSP file, create one with default entry of 1 (meaning Pure181) */
      dmc[0] = 1;
   }
   else
   {
      /* toggle to the other value */
      if (dmc[0] == 0)
      {
         dmc[0] = 1;
      }
      else if (dmc[0] == 1)
      {
         dmc[0] = 0;
      }
      else
      {
         cmsLog_error("Unexpected value in CMS Data Model PSP key=%d", dmc[0]);
         /* not a fatal error, just set to 1 */
         dmc[0] = 1;
      }
   }

   if ((ret = cmsPsp_set(CMS_DATA_MODEL_PSP_KEY, dmc, sizeof(dmc))) != CMSRET_SUCCESS)
   {
      cmsLog_error("set of CMS Data Model PSP key failed, ret=%d", ret);
   }
#endif
}


UBOOL8 cmsUtil_isDataModelDevice2(void)
{
   UBOOL8 b = FALSE;

#if defined(SUPPORT_DM_LEGACY98) || defined(SUPPORT_DM_HYBRID)

   b = FALSE;

#elif defined(SUPPORT_DM_PURE181)

   b = TRUE;

#elif defined(SUPPORT_DM_DETECT)
   SINT32 rv;
   UINT8 dmc[CMS_DATA_MODEL_PSP_VALUE_LEN]={0};

   rv = cmsPsp_get(CMS_DATA_MODEL_PSP_KEY, dmc, sizeof(dmc));
   if (rv != CMS_DATA_MODEL_PSP_VALUE_LEN)
   {
      /* No PSP file, assume default of TRUE (meaning Pure181) */
      b = TRUE;
   }
   else
   {
      if (dmc[0] == 0)
      {
         b = FALSE;
      }
      else if (dmc[0] == 1)
      {
         b = TRUE;
      }
      else
      {
         cmsLog_error("Unexpected value in CMS Data Model PSP key=%d", dmc[0]);
         /* not a fatal error, just assume TRUE */
         b = TRUE;
      }
   }
#endif

   return b;
}


void cmsUtil_setDataModelDevice2(void)
{
#if defined(SUPPORT_DM_LEGACY98) || defined(SUPPORT_DM_HYBRID) ||  defined(SUPPORT_DM_PURE181)
   cmsLog_error("this function is only valid in DM Detect mode");
#elif defined(SUPPORT_DM_DETECT)
   CmsRet ret;
   UINT8 dmc[CMS_DATA_MODEL_PSP_VALUE_LEN]={0};

   dmc[0] = 1;
   if ((ret = cmsPsp_set(CMS_DATA_MODEL_PSP_KEY, dmc, sizeof(dmc))) != CMSRET_SUCCESS)
   {
      cmsLog_error("set of CMS Data Model PSP key failed, ret=%d", ret);
   }
#endif
}


void cmsUtil_clearDataModelDevice2(void)
{
#if defined(SUPPORT_DM_LEGACY98) || defined(SUPPORT_DM_HYBRID) ||  defined(SUPPORT_DM_PURE181)
   cmsLog_error("this function is only valid in DM Detect mode");
#elif defined(SUPPORT_DM_DETECT)
   CmsRet ret;
   UINT8 dmc[CMS_DATA_MODEL_PSP_VALUE_LEN]={0};

   if ((ret = cmsPsp_set(CMS_DATA_MODEL_PSP_KEY, dmc, sizeof(dmc))) != CMSRET_SUCCESS)
   {
      cmsLog_error("clear of CMS Data Model PSP key failed, ret=%d", ret);
   }
#endif
}


