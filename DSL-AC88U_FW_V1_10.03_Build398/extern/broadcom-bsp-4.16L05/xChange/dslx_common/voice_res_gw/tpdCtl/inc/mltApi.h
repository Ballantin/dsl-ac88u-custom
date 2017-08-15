/****************************************************************************
 *
 * Copyright 2008 Broadcom
 *
 * This program is the proprietary software of Broadcom and/or its
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
/** @file mltApi.h
 * An API to drive underlying Metallic Line Testing (MLT) capability. The following
 * provides an alternative to Telephony Port Diagnostic (TPD) Control API used within
 * the Voice Residential Gateway (VRG) component. TPD exposes many details associated
 * with the underlying measurement mechanism while MLT focuses on abstract Test Cases
 * and the associated Results collection.
 */
#ifndef MLTAPI_H_
#define MLTAPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <vrgTypes.h>

/** Maximum number of supported Ports. */
#define MLT_PORT_MAX 8

/** Port identifier. */
typedef int MLT_PORT;

/** Module-wide status indication. */
typedef enum MLT_STATUS
{
   MLT_SUCCESS, /**< Request was successful. */
   MLT_FAILURE  /**< Request was unsuccessful. */
} MLT_STATUS;

/** Test Case/Measurement descriptions. */
typedef enum MLT_CASE
{
   MLT_RES,      /**< Resistive faults. */
   MLT_REN,      /**< Ring load equivalence. */
   MLT_FEMF,     /**< Foreign Electro-motive force. */
   MLT_OFFHOOK,  /**< Off-hook detection. */
   MLT_ALL,      /**< All external tests. */
   MLT_SELF_ALL, /**< All internal tests. */
   MLT_LAST      /**< Reserved for internal use. */
} MLT_CASE;

/** Measurements associated with MLT_RES. Precision: 1/10 ohms */
typedef struct
{
   VRG_SINT32 tipGnd;  /**< Tip-Ground impedance. */
   VRG_SINT32 ringGnd; /**< Ring-Ground impedance. */
   VRG_SINT32 tipRing; /**< Tip-Ring impedance. */
} MLT_RES_RESULT;

/** Measurements associated with MLT_FEMF. Precision: milli-Volts */
typedef struct
{
   VRG_SINT32 tipAc;  /**< Volts AC on Tip. */
   VRG_SINT32 tipDc;  /**< Volts DC on Tip. */
   VRG_SINT32 ringAc; /**< Volts AC on Ring. */
   VRG_SINT32 ringDc; /**< Volts DC on Ring. */
} MLT_VOLTAGE_RESULT;

/** Self-test Report */
typedef struct
{
   VRG_SINT32 loop;    /**< Feed Current Self-test. */
   VRG_SINT32 battery; /**< On-hook Voltage Self-test. */
   VRG_SINT32 ring;    /**< Ring Voltage in mV. */
   VRG_SINT32 cap;     /**< Capacitance in nF. */   
} MLT_SELFTEST_RESULT;

/** Raw measurement results.
 * Emphasis on composition here indicates that measurements will only
 * be sent one case at a time.
 */
typedef union MLT_RESULT
{
   MLT_RES_RESULT impedance;   /**< All resistance measurements. */
   MLT_VOLTAGE_RESULT voltage; /**< All voltage measurements. */
   VRG_SINT32 ren;             /**< Measurement associated with MLT_REN. Precision: milli-REN */
   MLT_SELFTEST_RESULT self;   /**< Self-test pass/fail status. */
   VRG_BOOL isOnhook;          /**< Measurement associated with MLT_OFFHOOK. Precision: true/false. */
} MLT_RESULT;

/** Test Case together with associated Result(s). */
typedef struct MLT_REPORT
{
   MLT_CASE testCase;     /**< Requested test. */
   MLT_RESULT testResult; /**< Associated result. */
} MLT_REPORT;

/** Callback used to capture MLT measurement Results.
 * @param portId Port identifier.
 * @param pResult Reference to associated Result(s).
 */
typedef void (*MLT_CALLBACK)(MLT_PORT portId, MLT_REPORT *pResult);

/** Initialization of MLT API.
 * @param callback Function to be called whenever Results are available.
 * @return MLT_SUCCESS if all goes well
 */
MLT_STATUS mltInit(MLT_CALLBACK callback);

/** Release MLT API.
 * @return MLT_SUCCESS if all goes well
 */
MLT_STATUS mltDeinit(void);

/** Test/Measurement Request mechanism.
 * Request is made on behalf of caller to underlying measurement capability.
 * @param portId Port identifier.
 * @param testCase Test Case identifier.
 * @param isLineActive Port identifier.
 * @return MLT_SUCCESS if request was successful
 */
MLT_STATUS mltRequest(MLT_PORT portId, MLT_CASE testCase, VRG_BOOL isLineActive);

#ifdef __cplusplus
}
#endif

#endif /* MLTAPI_H_ */
