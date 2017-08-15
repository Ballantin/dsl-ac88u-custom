/****************************************************************************
 *
 * Copyright 2008 Broadcom Corporation
 *
 * This program is the proprietary software of Broadcom Corporation and/or its
 * licensors, and may only be used, duplicated, modified or distributed
 * pursuant to the terms and conditions of a separate, written license
 * agreement executed between you and Broadcom (an "Authorized License").
 * Except as set forth in an Authorized License, Broadcom grants no license
 * (express or implied), right to use, or waiver of any kind with respect to
 * the Software, and Broadcom expressly reserves all rights in and to the
 * Software and all intellectual property rights therein.  IF YOU HAVE NO
 * AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
 * AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
 * SOFTWARE.
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 *        constitutes the valuable trade secrets of Broadcom, and you shall
 *        use all reasonable efforts to protect the confidentiality thereof,
 *        and to use this information only in connection with your use of
 *        Broadcom integrated circuit products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 *        "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,
 *        REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY,
 *        OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY
 *        DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,
 *        NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,
 *        ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR
 *        CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING
 *        OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM
 *        OR ITS LICENSORS BE LIABLE FOR
 *        (i)  CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT, OR EXEMPLARY
 *             DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
 *             YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 *             HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR
 *        (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE
 *             SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE
 *             LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
 *             ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *
 *****************************************************************************/

#include "mltApi.h"
#include "tpdCtl.h"

#include <vrgLog.h>

/* Control structure used to drive asynchronous TPD_CTL API */
typedef struct
{
   VRG_UINT32 start; /* Mask identifying Test Request */
   VRG_UINT32 valid; /* Mask identifying Test(s) associated with Result(s) */
} TPD_TEST_CTL;

static TPD_TEST_CTL tpdCtl[MLT_PORT_MAX];

/* Mapping of TPD Control to interface to MTA API */
typedef struct
{
   MLT_CASE   mltApiTestCase; /* Test Cases as identified by MLT API */
   VRG_UINT32 tpdCtlTestCase; /* Test Cases as identified by TPD_CTL */
} MLT_TPD_TEST_MAP;

static const MLT_TPD_TEST_MAP testCases[] = {
   {MLT_RES,     (TPD_CTL_TEST_TG_SHORT | TPD_CTL_TEST_RG_SHORT | TPD_CTL_TEST_TR_SHORT)},
   {MLT_REN,      TPD_CTL_TEST_REN},
   {MLT_FEMF,     TPD_CTL_TEST_FOR_EMF},
   {MLT_OFFHOOK,  TPD_CTL_TEST_REC_OFFHOOK},
   {MLT_ALL,     (TPD_CTL_TEST_FOR_EMF |
                 (TPD_CTL_TEST_TG_SHORT | TPD_CTL_TEST_RG_SHORT | TPD_CTL_TEST_TR_SHORT) |
                  TPD_CTL_TEST_REC_OFFHOOK |
                  TPD_CTL_TEST_REN)},
   {MLT_SELF_ALL, TPD_CTL_TEST_SELF_CHECK},
   {MLT_LAST,     0}
};

static void tpdCtlCallback(TPD_CTL_EVENT event, VRG_BOOL mibSet, void *value, MLT_PORT portId);
static MLT_CALLBACK mltCallback = NULL;

static MLT_STATUS translateRequest(MLT_PORT portId, MLT_CASE testCase);
static MLT_STATUS translateReport(MLT_REPORT *pMltReport, TPD_CTL_REPORT *pTpdCtlReport);

static void propogateResults(MLT_PORT portIdx, TPD_CTL_REPORT *pTpdCtlReport);

MLT_STATUS mltInit(MLT_CALLBACK callback)
{
   TPD_CTL_STATUS tpdStatus = tpdCtlInit(tpdCtlCallback);
   if (tpdStatus != TPD_CTL_SUCCESS)
   {
      return( MLT_FAILURE );
   }

   mltCallback = callback;
   memset(&tpdCtl[0], 0, (MLT_PORT_MAX * sizeof(tpdCtl[0])));

   return( MLT_SUCCESS );
}

MLT_STATUS mltDeinit(void)
{
   TPD_CTL_STATUS tpdStatus = tpdCtlDeinit();
   if (tpdStatus != TPD_CTL_SUCCESS)
   {
      VRG_LOG_WARN((VRG_LOG_MOD_TPD, "WARNING (%s): TPD Deinit failed.", __FUNCTION__));
   }

   mltCallback = NULL;

   return( MLT_SUCCESS );
}

MLT_STATUS mltRequest(MLT_PORT portId, MLT_CASE testCase, VRG_BOOL isLineActive)
{
   MLT_STATUS mltStatus = MLT_FAILURE;
   TPD_CTL_STATUS tpdStatus;

   /* Basic range check */
   if (portId >= MLT_PORT_MAX)
   {
      VRG_LOG_WARN((VRG_LOG_MOD_TPD, "WARNING (%s): Port ID out-of-range, ignoring...", __FUNCTION__));
      return( mltStatus );
   }

   /* Note: TPD_CTL is not in this context
    * so it is possible to interrupt an earlier request
    * but there's no intrinsic harm in this outcome (excluding pathological clients).
    */
   mltStatus = translateRequest(portId, testCase);
   if ((mltStatus != MLT_SUCCESS) || (tpdCtl[portId].start == 0))
   {
      return( mltStatus );
   }

   /* Force all tests for now */
   tpdCtl[portId].start |= TPD_CTL_TEST_FORCE;

   tpdStatus = tpdCtlProcessSignal(TPD_CTL_SIGNAL_NOPROV_TESTS, portId, isLineActive, tpdCtl[portId].start);
   if (tpdStatus != TPD_CTL_SUCCESS)
   {
      mltStatus = MLT_FAILURE;
   }

   return( mltStatus );
}

static void tpdCtlCallback(TPD_CTL_EVENT event, VRG_BOOL mibSet, void *value, MLT_PORT portId)
{

   /* Not strictly necessary (TPD_CTL is the only caller)
    * but method is de facto public so range-check is encouraged.
    */
   if (portId >= MLT_PORT_MAX)
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_TPD, "ERROR (%s): Port out-of-range.", __FUNCTION__));
      return;
   }

   switch( event )
   {
      case TPD_CTL_EVENT_TEST_VALID:
      {
         if ( mibSet )
         {
            tpdCtl[portId].valid = *(VRG_UINT32 *)value;
         }
      }
      break;

      case TPD_CTL_EVENT_TEST_VALUE:
      {
         if ( mibSet )
         {
            propogateResults(portId, value);
         }
      }
      break;

      case TPD_CTL_EVENT_TEST_START:
         // fall through
      case TPD_CTL_EVENT_TEST_STOP:
         // fall through
      case TPD_CTL_EVENT_TEST_RESULT:
         // fall through
      case TPD_CTL_EVENT_TEST_FAILED:
         // fall through
      case TPD_CTL_EVENT_PC_TEST_START:
         // fall through
      case TPD_CTL_EVENT_PC_TEST_CLEAR:
         // fall through
      case TPD_CTL_EVENT_PC_TEST_VALID:
         // fall through
      case TPD_CTL_EVENT_PC_TEST_RESULT:
         // fall through
      case TPD_CTL_EVENT_TEST_IN_PROGRESS:
         // fall through
      case TPD_CTL_EVENT_TEST_DONE:
      {
         // Nothing to do
      }
      break;

      default:
      {
         VRG_LOG_WARN((VRG_LOG_MOD_TPD, "WARNING (%s): Unrecognized TPD event.", __FUNCTION__));
      }
   }
}

static MLT_STATUS translateRequest(MLT_PORT portIdx, MLT_CASE testCase)
{
   unsigned int mapIdx = 0;
   while (testCases[mapIdx].mltApiTestCase != MLT_LAST)
   {
      if (testCases[mapIdx].mltApiTestCase == testCase)
      {
         tpdCtl[portIdx].start |= testCases[mapIdx].tpdCtlTestCase;
         break;
      }
      mapIdx++;
   }

   return MLT_SUCCESS;
}

static void propogateResults(MLT_PORT portIdx, TPD_CTL_REPORT *pTpdCtlReport)
{
   /* The goal is to dispatch a result for each case TPD_CTL considered valid */
   MLT_REPORT result;
   MLT_STATUS status;

   unsigned int mapIdx = 0;
   while (testCases[mapIdx].mltApiTestCase != MLT_LAST)
   {
      if ((testCases[mapIdx].tpdCtlTestCase & tpdCtl[portIdx].valid) == 0)
      {
         mapIdx++;
         continue;
      }

      result.testCase = testCases[mapIdx].mltApiTestCase;
      status = translateReport(&result, pTpdCtlReport);
      if (status != MLT_SUCCESS)
      {
         mapIdx++;
         continue;
      }

      if (mltCallback != NULL)
      {
         mltCallback(portIdx, &result);
      }
      else
      {
         VRG_LOG_WARN((VRG_LOG_MOD_TPD, "WARNING (%s): Callback not assigned (?)", __FUNCTION__));
      }

      mapIdx++;
   }

   memset(&tpdCtl[portIdx], 0, sizeof(tpdCtl[portIdx]));
}

static MLT_STATUS translateReport(MLT_REPORT *pMltReport, TPD_CTL_REPORT *pTpdCtlReport)
{
   MLT_STATUS status = MLT_SUCCESS;

   switch (pMltReport->testCase)
   {
      case MLT_RES:
      {
         pMltReport->testResult.impedance.ringGnd = pTpdCtlReport->ringGroundImpedance;
         pMltReport->testResult.impedance.tipGnd = pTpdCtlReport->tipGroundImpedance;
         pMltReport->testResult.impedance.tipRing = pTpdCtlReport->tipRingImpedance;
      }
      break;

      case MLT_REN:
      {
         pMltReport->testResult.ren = pTpdCtlReport->ren;
      }
      break;

      case MLT_FEMF:
      {
         pMltReport->testResult.voltage.ringAc = pTpdCtlReport->ringAC;
         pMltReport->testResult.voltage.ringDc = pTpdCtlReport->ringDC;
         pMltReport->testResult.voltage.tipAc = pTpdCtlReport->tipAC;
         pMltReport->testResult.voltage.tipDc = pTpdCtlReport->tipDC;
      }
      break;

      case MLT_OFFHOOK:
      {
         pMltReport->testResult.isOnhook = pTpdCtlReport->phoneOnhook;
      }
      break;

      case MLT_SELF_ALL:
      {
         pMltReport->testResult.self.loop    = pTpdCtlReport->loop;
         pMltReport->testResult.self.battery = pTpdCtlReport->battery;
         pMltReport->testResult.self.ring    = pTpdCtlReport->ring;
         pMltReport->testResult.self.cap     = pTpdCtlReport->cap;
      }
      break;

      default:
      {
         status = MLT_FAILURE;
      }
   }

   return( status );
}
