/****************************************************************************
*
* Copyright 2000-2008 Broadcom
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
/** @file tpdCtl.h
 *
 * This module contains the functions of the tpd monitoring service.
 */
#ifndef TPDCTL_H
#define TPDCTL_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <vrgTypes.h>
#include <xdrvTpd.h>
#include <tpdCtlCfg.h>

/* ---- Constants and Types ---------------------------------------------- */

/** TPD Results. */
typedef enum TPD_CTL_STATUS
{
   TPD_CTL_SUCCESS,
   TPD_CTL_FAILURE

} TPD_CTL_STATUS;

/** TPD Signal Requests. */
typedef enum TPD_CTL_SIGNAL
{
   TPD_CTL_SIGNAL_LINE_RESET,                  /**< Line reset. */
   TPD_CTL_SIGNAL_PROV_TESTS,                  /**< Standard TPD test suite w/ prov support. */
   TPD_CTL_SIGNAL_NOPROV_TESTS,                /**< Standard TPD test suite wo/ prov support. */
   TPD_CTL_SIGNAL_PC15_TESTS,                  /**< Standard TPD test suite w/ PC15 prov support. */
   TPD_CTL_SIGNAL_CALIBRATION_TESTS            /**< Calibration test suite. */

} TPD_CTL_SIGNAL;

/** TPD Telemetry Events. */
typedef enum TPD_CTL_EVENT
{
   TPD_CTL_EVENT_TEST_START = 0,     /**< TPD Status Update. */
   TPD_CTL_EVENT_TEST_STOP,          /**< TPD Usage Update. */
   TPD_CTL_EVENT_TEST_VALID,         /**< TPD Telemetry Signal. */
   TPD_CTL_EVENT_TEST_RESULT,        /**< TPD Telemetry Signal. */
   TPD_CTL_EVENT_TEST_FAILED,        /**< TPD Telemetry Signal. */
   TPD_CTL_EVENT_TEST_VALUE,         /**< TPD Telemetry Signal. */

   TPD_CTL_EVENT_PC_TEST_START,      /**< TPD Status Update. */
   TPD_CTL_EVENT_PC_TEST_CLEAR,      /**< TPD Usage Update. */
   TPD_CTL_EVENT_PC_TEST_VALID,      /**< TPD Telemetry Signal. */
   TPD_CTL_EVENT_PC_TEST_RESULT,     /**< TPD Telemetry Signal. */

   TPD_CTL_EVENT_CALIBRATION_DONE,   /**< TPD Calibration Results. */

   TPD_CTL_EVENT_TEST_IN_PROGRESS,   /**< TPD Status event. */
   TPD_CTL_EVENT_TEST_DONE	          /**< TPD Status event. */

} TPD_CTL_EVENT;

/** TPD Test Cases. */
typedef enum TPD_CTL_TEST
{
   TPD_CTL_TEST_TG_SHORT     = 0x80000000,  /**< TPD Tip to Ground Short Test. */
   TPD_CTL_TEST_RG_SHORT     = 0x40000000,  /**< TPD Ring to Ground Short Test. */
   TPD_CTL_TEST_TR_SHORT     = 0x20000000,  /**< TPD Tip to Ring Short Test. */
   TPD_CTL_TEST_REN          = 0x10000000,  /**< TPD Ringer Equivalency Network Test. */
   TPD_CTL_TEST_SELF_CHECK   = 0x08000000,  /**< TPD Selfcheck Test. */
   TPD_CTL_TEST_OFFHOOK      = 0x04000000,  /**< TPD Offhook Simulation Test. */
   TPD_CTL_TEST_CROSS2       = 0x02000000,  /**< TPD Line 2 Tip to Line 1 Ring Short Test. */
   TPD_CTL_TEST_CROSS1       = 0x01000000,  /**< TPD Line 1 Tip to Line 2 Ring Short Test. */
   TPD_CTL_TEST_HAZ_POT      = 0x00800000,  /**< TPD Hazardous Potential Test. */
   TPD_CTL_TEST_FOR_EMF      = 0x00400000,  /**< TPD Foreign Electromotive Force Test .*/
   TPD_CTL_TEST_REC_OFFHOOK  = 0x00200000,  /**< TPD Receiver Offhook Test. */
   TPD_CTL_TEST_PTC_CHECK    = 0x00100000   /**< TPD PTC Status Check. */

} TPD_CTL_TEST;

/** TPD PacketCable 1.5 Test Cases. */
typedef enum TPD_CTL_TEST_PC
{
   TPD_CTL_TEST_PC_FOREIGN_VOLTAGE      = 0x80,
   TPD_CTL_TEST_PC_HAZARDOUS_POTENTIAL  = 0x40

} TPD_CTL_TEST_PC;

/** Mask to force TPD test execution. */
#define TPD_CTL_TEST_FORCE 0x00000001

/** Mask all supported Test Cases. */
#define TPD_CTL_ALL_TESTS ( TPD_CTL_TEST_TG_SHORT | TPD_CTL_TEST_RG_SHORT | TPD_CTL_TEST_TR_SHORT | \
                            TPD_CTL_TEST_REN | TPD_CTL_TEST_SELF_CHECK | TPD_CTL_TEST_OFFHOOK | \
                            TPD_CTL_TEST_HAZ_POT | TPD_CTL_TEST_FOR_EMF | TPD_CTL_TEST_REC_OFFHOOK | TPD_CTL_TEST_PTC_CHECK )

/** Force all supported Test Cases. */
#define TPD_CTL_FORCE_ALL_TESTS ( TPD_CTL_ALL_TESTS | TPD_CTL_TEST_FORCE )

/** TPD control callback. */
typedef void ( *TPD_CTL_CALLBACK )
(
   TPD_CTL_EVENT       event,
   VRG_BOOL            mibSet,
   void               *value,
   int                 chan
);

#if TPD_CTL_CFG_REPORT
/** Report to next text-based clients. */
typedef struct TPD_CTL_REPORT {
   XDRV_SINT32 tipAC;  /**< Tip AC (mV). */
   XDRV_SINT32 tipDC;  /**< Tip DC (mV). */
   XDRV_SINT32 ringAC; /**< Ring AC (mV). */
   XDRV_SINT32 ringDC; /**< Ring DC (mV). */

   XDRV_UINT32 tipGroundImpedance;  /**< Tip-Ground Impedance (ohms). */
   XDRV_UINT32 ringGroundImpedance; /**< Ring-Ground Impedance (ohms). */
   XDRV_UINT32 tipRingImpedance;    /**< Tip-Ring Impedance (ohms). */

   XDRV_SINT32 ren; /**< Resistive Equivalence (milli-REN). */

   XDRV_SINT32 loop;    /**< Loop Current portion of DC Feed Self-test. */
   XDRV_SINT32 battery; /**< Voltage portion of DC Feed Self-test. */
   XDRV_SINT32 ring;    /**< Ring Self-test. */
   XDRV_SINT32 cap;     /**< Capacitance Self-test. */

   XDRV_SINT32 phoneOnhook; /**< Receiver On-hook (boolean). */

} TPD_CTL_REPORT;
#endif

/* TPD Testing Minimum Passing Values. */
#define TPD_SHORT_RESISTANCE        150000
#define TPD_MIN_MILLIRENS              175
#define TPD_MAX_MILLIRENS             5000
#define TPD_MAX_PTC_AC_VOLTAGE_MV    40000
#define TPD_MAX_PTC_DC_VOLTAGE_MV    40000
#define TPD_MAX_FEF_AC_VOLTAGE_MV    10000
#define TPD_MAX_FEF_DC_VOLTAGE_MV     6000
#define TPD_MAX_HP_AC_VOLTAGE_MV     50000
#define TPD_MAX_HP_DC_VOLTAGE_MV    120000

/* ---- Function Prototypes ---------------------------------------------- */

/** TPD controller init - call once during system initialization.
 *
 * <b>PURPOSE</b>: TPD controller module init - called once during system
 * initialization. Conceptually, the TPD controller manages the
 * TPD state machines for the board via the tpdery management API
 *
 * @param callback callback to access endpoint and callclient layer
 * @return TPD_CTL_SUCCESS or TPD_CTL_FAILURE
 */
TPD_CTL_STATUS tpdCtlInit( TPD_CTL_CALLBACK callback );

/** tpdCtlDeinit - tpd controller module de-initialization.
 *
 * <b>PURPOSE</b>: tpd module deinit  - call once during system shutdown. This
 * will shutdown the tpd task and free all resources used by
 * the tpd controller. ( i.e. this function should free all
 * resources allocated in tpdCtlInit()).
 *
 * <b>NOTE</b>: It is assumed that this function is only called after the tpd
 * task has been notified of a pending application reset, and it
 * has acknowledged the notification. This implies that the tpd
 * task is currently blocked, waiting to be resumed so that they may
 * complete the shutdown procedure.
 *
 * @return TPD_CTL_SUCCESS or TPD_CTL_FAILURE
 *         This function should only return an error under catastrophic
 *         circumstances. i.e. Something that cannot be fixed by re-invoking
 *         the module initialization function.
 */
TPD_CTL_STATUS tpdCtlDeinit( void );

/** Tell the TPD task to process the given signal on the given line.
 * @param signal the signal to process
 * @param lineId the line on which the signal applies
 * @param bActive boolean indicating if this line was active or not
 * @param value used for multiple purposes depending on signal
 * @return TPD_CTL_SUCCESS or TPD_CTL_FAILURE
 */
TPD_CTL_STATUS tpdCtlProcessSignal( TPD_CTL_SIGNAL signal, int lineId, VRG_BOOL bActive, int value );

/** Tell the TPD task that dialtone was requested on the line.
 * @param lineId The line on which the signal applies
 * @return TPD_CTL_SUCCESS or TPD_CTL_FAILURE
 */
TPD_CTL_STATUS tpdCtlSignalDialtone( int lineId );

/** Tell the TPD task that admin status (enabled/disabled) on the line has changed.
 * @param lineId the line on which the signal applies
 * @param bEnabled new admin status (true=enabled, false=disabled
 * @return
 */
TPD_CTL_STATUS tpdCtlSignalAdminStatus( int lineId, VRG_BOOL bEnabled );

#ifdef __cplusplus
}
#endif

#endif /* TPDCTL_H H */
