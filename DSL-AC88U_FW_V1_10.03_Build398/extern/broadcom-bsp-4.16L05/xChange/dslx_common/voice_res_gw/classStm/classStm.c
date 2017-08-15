/***************************************************************************
*    Copyright © 2000-2008 Broadcom Corporation                                        
*                                                                           
*    This program is the proprietary software of Broadcom Corporation and/or
*    its licensors, and may only be used, duplicated, modified or           
*    distributed pursuant to the terms and conditions of a separate, written
*    license agreement executed between you and Broadcom (an "Authorized   
*    License").  Except as set forth in an Authorized License, Broadcom    
*    grants no license (express or implied), right to use, or waiver of any 
*    kind with respect to the Software, and Broadcom expressly reserves all 
*    rights in and to the Software and all intellectual property rights     
*    therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO 
*    USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM   
*    AND DISCONTINUE ALL USE OF THE SOFTWARE.                               
*                                                                           
*                                                                           
*    Except as expressly set forth in the Authorized License,               
*                                                                           
*    1.     This program, including its structure, sequence and             
*    organization, constitutes the valuable trade secrets of Broadcom, and  
*    you shall use all reasonable efforts to protect the confidentiality    
*    thereof, and to use this information only in connection with your use  
*    of Broadcom integrated circuit products.                               
*                                                                           
*    2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
*    "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES,          
*    REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR  
*    OTHERWISE, WITH RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY        
*    DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY,    
*    NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE, LACK OF VIRUSES,    
*    ACCURACY OR COMPLETENESS, QUIET ENJOYMENT, QUIET POSSESSION OR         
*    CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT  
*    OF USE OR PERFORMANCE OF THE SOFTWARE.                                 
*                                                                           
*    3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL       
*    BROADCOM OR ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, 
*    SPECIAL, INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN
*    ANY WAY RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN  
*    IF BROADCOM HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR    
*    (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE 
*    ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY
*    NOTWITHSTANDING ANY FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*
****************************************************************************
*
*    Filename: classstm.c
*
****************************************************************************
*    Description:
*
*     This module contains the state machines for class features and the action
*     functions.
*
****************************************************************************/

#include <string.h>
#include <vrgTypes.h>
#include <vrgLog.h>
#include <xchgAssert.h>
#include <hdspClass.h>
#include <hdspTone.h>
#include "classStm.h"
#include <sme.h>
#include <hapigdef.h>
#include <hapi_pte.h>
#include <casCtl.h>

#define  CLID_DEBUG           0    /* set to 1 to enable state transition logging */

/*
** VMWI Open Switch Interval - 300ms
*/
#define  VMWI_OSI_DURATION_MSEC                    300

/*
** Duration of ring pulse that is used as a alerting signal for onhook caller-id.
** This should be between 200 and 300 ms, as specified in ETSI EN 300 659-1
** v1.3.1, section 6.3.2.
*/
#define  RING_PULSE_DURATION_MSEC                  250

/*
** This is specific to DTMF dialstring playout.
** Represents a window of time after last DTMF digit in the dialstring
** to enable DTMF detection and report TONE_GEN event.
** This define should match the internal DSP define (PTE_GEN_DET_REENABLE)
** in pte.h
**
*/
#define  DTMF_TONE_GEN_EVENT_DELAY_MSEC            30

/*
** Duration of silence period after FSK for VMWI. This should be between 200 and
** 500 ms, as specified in ETSI EN 300 659-1 v1.3.1, section 6.2.
*/
#define  POST_VMWI_FSK_DELAY_MSEC                  250

/*
** Delay between subscriber alerting signal (call waiting)
** and channel alerting signal (DTAS).
** ETSI specs don't specify what this should be, while NA
** specs require it to be 0-50ms.
** We will set it to 10ms (knowing that the processing delay -
** see comment below - will kick it little higher, but
** hopefully not beyond the 50ms mark).
*/
#define CLASS_CLID_SAS_CAS_DELAY         10

/*
** The caller-id state machines are driven by events that are generated by
** the host and DSP. There is a small processing/propogation delay since events
** need to be sent from the DSP to host, and new commands need to be sent from
** the host to DSP when events are detected. The defines below are based upon
** empirical testing results, and are intended to be subtracted from the
** delay between states specified by the client.
*/
#define  FSK_AFTER_RINGING_PROCESSING_DELAY_MSEC   20
#define  FSK_AFTER_ALERTING_PROCESSING_DELAY_MSEC  30
#define  RING_AFTER_FSK_PROCESSING_DELAY_MSEC      10

#if CLID_DEBUG
#include <string.h>
#endif   /* CLID_DEBUG */

#if CLID_DEBUG
static void getStateString( const SME_STATE *state, char *string );
static void getEventString( int event, char *string );
char testString[128];
#endif


/* function prototypes */

static int actionClidTxFormatData( void *datap );
static int actionStartRing( void *datap );
static int actionClidTxStart( void *datap );
static int actionClidTxCleanup( void *datap );
static int actionSendDualToneSignal( void *datap );
static int actionStopRing( void *datap );
static int actionNormalPolarity( void *datap );
static int actionOffHookNormalPolarity( void *datap );
static int actionNormalPolarityIdle( void *datap );
static int actionVmwiOnFormatData( void *datap );
static int actionVmwiOffFormatData( void *datap );
static int actionVmwi( CLASSSTM_PARM *parmp, VRG_UINT16 type );
static int actionReversePolarityAndCarRing( void *datap );
static int actionStartFirstSas( void *datap );
static int actionStartCas( void *datap );
static int actionStartSasWithDelay( void *datap );
static int actionStopSasCas( void *datap );
static int actionOnhookTransmission( void *datap );
static int actionFskAfterDtasDelay( void *datap );
static int actionFskAfterLrDtasDelay( void *datap );
static int actionRingAfterFskDelay( void *datap );
static int actionSendRingPulseSignal( void *datap );
static int actionFskAfterRpasDelay( void *datap );
static int actionDtasAfterLrDelay( void *datap );
static int actionPostVmwiFskDelay( void *datap );
static int actionVmwiSupervisoryDisconnect( void *datap );
static int actionBlockPulseCasEvents( void *datap );
static int actionUnblockPulseCasEvents( void *datap );
static int actionEnableFastSlicModes( void *datap );
static int actionDisableFastSlicModes( void *datap );


/* define states used in the state machines */
extern const SME_STATE stateClidTxNaIdle[];
extern const SME_STATE stateClidTxNaWaitSilence[];
extern const SME_STATE stateClidTxNaSendFsk[];
extern const SME_STATE stateClidTxNaContFsk[];

extern const SME_STATE stateClidTxDtAsIdle[];
extern const SME_STATE stateClidTxDtAsSendDtAs[];
extern const SME_STATE stateClidTxDtAsPreClidTx[];
extern const SME_STATE stateClidTxDtAsClidTx[];
extern const SME_STATE stateClidTxDtAsPostClidTx[];
extern const SME_STATE stateClidTxDtAsSendRing[];

extern const SME_STATE stateClidTxRpAsIdle[];
extern const SME_STATE stateClidTxRpAsSendRpAs[];
extern const SME_STATE stateClidTxRpAsPreClidTx[];
extern const SME_STATE stateClidTxRpAsClidTx[];
extern const SME_STATE stateClidTxRpAsPostClidTx[];
extern const SME_STATE stateClidTxRpAsSendRing[];

extern const SME_STATE stateClidTxLrAsIdle[];
extern const SME_STATE stateClidTxLrAsPreDtas[];
extern const SME_STATE stateClidTxLrAsSendDtAs[];
extern const SME_STATE stateClidTxLrAsPreClidTx[];
extern const SME_STATE stateClidTxLrAsClidTx[];
extern const SME_STATE stateClidTxLrAsPostClidTx[];
extern const SME_STATE stateClidTxLrAsSendRing[];

extern const SME_STATE stateClass2Idle[];
extern const SME_STATE stateClass2PendingCallid[];
extern const SME_STATE stateClass2PendingSasDone[];
extern const SME_STATE stateClass2PendingAck[];
extern const SME_STATE stateClass2SendFsk[];
extern const SME_STATE stateClass2PendingDelayDone[];

extern const SME_STATE stateClidTxJpIdle[];
extern const SME_STATE stateClidTxJpSendPr[];
extern const SME_STATE stateClidTxJpSendCar[];
extern const SME_STATE stateClidTxJpSendFsk[];
extern const SME_STATE stateClidTxJpSendIr[];
extern const SME_STATE stateClidTxJpWaitOnHook[];

extern const SME_STATE stateVmwiIdle[];
extern const SME_STATE stateVmwiSendOsi[];
extern const SME_STATE stateVmwiSendFsk[];

extern const SME_STATE stateVmwiTxDtAsIdle[];
extern const SME_STATE stateVmwiTxDtAsSendDtAs[];
extern const SME_STATE stateVmwiTxDtAsPreClidTx[];
extern const SME_STATE stateVmwiTxDtAsClidTx[];
extern const SME_STATE stateVmwiTxDtAsPostClidTx[];

extern const SME_STATE stateVmwiTxRpAsIdle[];
extern const SME_STATE stateVmwiTxRpAsSendRpAs[];
extern const SME_STATE stateVmwiTxRpAsPreClidTx[];
extern const SME_STATE stateVmwiTxRpAsClidTx[];
extern const SME_STATE stateVmwiTxRpAsPostClidTx[];

extern const SME_STATE stateVmwiTxLrAsIdle[];
extern const SME_STATE stateVmwiTxLrAsPreDtas[];
extern const SME_STATE stateVmwiTxLrAsSendDtAs[];
extern const SME_STATE stateVmwiTxLrAsPreClidTx[];
extern const SME_STATE stateVmwiTxLrAsClidTx[];
extern const SME_STATE stateVmwiTxLrAsPostClidTx[];




/* --------------------------------------------------------------------------
** On-hook caller-id state machines.
*/

/*
** State machine for North America (FSK during ring).
**
** EVENT                      NEXT STATE                 ACTION FUNCTIONS
*/
const SME_STATE stateClidTxNaIdle[] = {
{CLASSSTM_CLID_RING,          stateClidTxNaWaitSilence,  {actionClidTxFormatData, actionStartRing, 0}},
{CLASSSTM_RING_OFF,           stateClidTxNaIdle,         {actionStopRing, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxNaWaitSilence[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxNaSendFsk,      {actionClidTxStart, 0 /*actionClidTimerAbortNa*/, 0}},
{CLASSSTM_RING_OFF,           stateClidTxNaIdle,         {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxNaIdle,         {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxNaSendFsk[] = {
{CLASSSTM_CLID_TX_END,        stateClidTxNaIdle,         {0, 0, 0}},
{CLASSSTM_TONE_GEN_EVT,       stateClidTxNaIdle,         {0, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxNaContFsk,      {0, 0, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxNaIdle,         {actionClidTxCleanup, 0, 0}},
{CLASSSTM_TIMER_EXPIRE,       stateClidTxNaIdle,         {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxNaContFsk[] = {
{CLASSSTM_CLID_TX_END,        stateClidTxNaIdle,         {actionStopRing, 0, 0}},
{CLASSSTM_TIMER_EXPIRE,       stateClidTxNaIdle,         {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_CLID_FIRST_RINGOFF, stateClidTxNaContFsk,      {actionClidTxStart, 0 /*actionClidTimerAbortNa*/, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxNaIdle,         {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};



/*
** DT-AS (dual tone alert signal), FSK and ringing.
**
** EVENT                      NEXT STATE                 ACTION FUNCTIONS
*/
const SME_STATE stateClidTxDtAsIdle[] = {
{CLASSSTM_CLID_RING,          stateClidTxDtAsSendDtAs,   {actionOnhookTransmission, actionClidTxFormatData, actionSendDualToneSignal}},
{CLASSSTM_RING_OFF,           stateClidTxDtAsIdle,       {actionStopRing, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxDtAsSendDtAs[] = {
{CLASSSTM_TONE_GEN_EVT,       stateClidTxDtAsPreClidTx,  {actionFskAfterDtasDelay, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxDtAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxDtAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxDtAsPreClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxDtAsClidTx,     {actionClidTxStart, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxDtAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxDtAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxDtAsClidTx[] = {
{CLASSSTM_CLID_TX_END,        stateClidTxDtAsPostClidTx, {actionRingAfterFskDelay, 0, 0}},
{CLASSSTM_TONE_GEN_EVT,       stateClidTxDtAsPostClidTx, {actionRingAfterFskDelay, 0, 0}},
{CLASSSTM_TIMER_EXPIRE,       stateClidTxDtAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_RING_OFF,           stateClidTxDtAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxDtAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxDtAsPostClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxDtAsSendRing,   {actionStartRing, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxDtAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxDtAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxDtAsSendRing[] = {
{CLASSSTM_RING_OFF,           stateClidTxDtAsIdle,       {actionStopRing, 0, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxDtAsIdle,       {actionStopRing, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};



/*
** RP-AS (ring pulse alert signal), FSK and ringing.
**
** EVENT                      NEXT STATE                 ACTION FUNCTIONS
*/
const SME_STATE stateClidTxRpAsIdle[] = {
{CLASSSTM_CLID_RING,          stateClidTxRpAsSendRpAs,   {actionClidTxFormatData, actionSendRingPulseSignal,0}},
{CLASSSTM_RING_OFF,           stateClidTxRpAsIdle,       {actionStopRing, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxRpAsSendRpAs[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxRpAsPreClidTx,  {actionFskAfterRpasDelay, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxRpAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxRpAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxRpAsPreClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxRpAsClidTx,     {actionClidTxStart, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxRpAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxRpAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxRpAsClidTx[] = {
{CLASSSTM_CLID_TX_END,        stateClidTxRpAsPostClidTx, {actionRingAfterFskDelay, 0, 0}},
{CLASSSTM_TONE_GEN_EVT,       stateClidTxRpAsPostClidTx, {actionRingAfterFskDelay, 0, 0}},
{CLASSSTM_TIMER_EXPIRE,       stateClidTxRpAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_RING_OFF,           stateClidTxRpAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxRpAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxRpAsPostClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxRpAsSendRing,   {actionStartRing, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxRpAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxRpAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxRpAsSendRing[] = {
{CLASSSTM_RING_OFF,           stateClidTxRpAsIdle,       {actionStopRing, 0, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxRpAsIdle,       {actionStopRing, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};



/*
** LR-AS (Loop reversal, dual tone alert signal), FSK and ringing.
**
** EVENT                      NEXT STATE                 ACTION FUNCTIONS
*/
const SME_STATE stateClidTxLrAsIdle[] = {
{CLASSSTM_CLID_RING,          stateClidTxLrAsPreDtas,    {actionClidTxFormatData, actionDtasAfterLrDelay}},
{CLASSSTM_RING_OFF,           stateClidTxLrAsIdle,       {actionStopRing, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxLrAsPreDtas[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxLrAsSendDtAs,   {actionSendDualToneSignal, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxLrAsIdle,       {actionClidTxCleanup, actionStopRing, actionNormalPolarity}},
{CLASSSTM_OFFHOOK,            stateClidTxLrAsIdle,       {actionClidTxCleanup, actionStopRing, actionNormalPolarity}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxLrAsSendDtAs[] = {
{CLASSSTM_TONE_GEN_EVT,       stateClidTxLrAsPreClidTx,  {actionFskAfterLrDtasDelay, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxLrAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxLrAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxLrAsPreClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxLrAsClidTx,     {actionClidTxStart, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxLrAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxLrAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxLrAsClidTx[] = {
{CLASSSTM_CLID_TX_END,        stateClidTxLrAsPostClidTx, {actionRingAfterFskDelay, 0, 0}},
{CLASSSTM_TONE_GEN_EVT,       stateClidTxLrAsPostClidTx, {actionRingAfterFskDelay, 0, 0}},
{CLASSSTM_TIMER_EXPIRE,       stateClidTxLrAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_RING_OFF,           stateClidTxLrAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxLrAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxLrAsPostClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxLrAsSendRing,   {actionStartRing, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxLrAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxLrAsIdle,       {actionClidTxCleanup, actionStopRing, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxLrAsSendRing[] = {
{CLASSSTM_RING_OFF,           stateClidTxLrAsIdle,       {actionStopRing, 0, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxLrAsIdle,       {actionStopRing, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};



/*
** Japanese state machine.
**
** EVENT                      NEXT STATE                 ACTION FUNCTIONS
*/
const SME_STATE stateClidTxJpIdle[] = {
{CLASSSTM_CLID_RING,          stateClidTxJpSendPr,       {actionClidTxFormatData, actionReversePolarityAndCarRing, 0}},
{CLASSSTM_CLID_NODATA_RING,   stateClidTxJpSendIr,       {actionStartRing, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxJpSendPr[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxJpSendCar,      {0, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxJpIdle,         {actionNormalPolarityIdle, actionClidTxCleanup, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxJpIdle,         {actionNormalPolarity, actionClidTxCleanup, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxJpSendCar[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxJpSendIr,       {actionClidTxCleanup, actionStartRing, 0}},
{CLASSSTM_OFFHOOK,            stateClidTxJpSendFsk,      {actionClidTxStart, 0 /*actionClidTimerAbortJp*/, 0}},
{CLASSSTM_RING_OFF,           stateClidTxJpIdle,         {actionClidTxCleanup, actionNormalPolarityIdle, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxJpSendFsk[] = {
{CLASSSTM_TIMER_EXPIRE,       stateClidTxJpIdle,         {actionClidTxCleanup, actionNormalPolarityIdle, 0}},
{CLASSSTM_CLID_TX_END,        stateClidTxJpSendFsk,      {0 /*actionClidTimerAbortJp*/, 0, 0}},
{CLASSSTM_ONHOOK,             stateClidTxJpSendIr,       {actionClidTxCleanup, actionStartRing, 0}},
{CLASSSTM_FLASH,              stateClidTxJpIdle,         {actionClidTxCleanup, actionOffHookNormalPolarity, 0}},
{CLASSSTM_RING_OFF,           stateClidTxJpWaitOnHook,   {0, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxJpSendIr[] = {
{CLASSSTM_OFFHOOK,            stateClidTxJpIdle,         {actionNormalPolarity, 0, 0}},
{CLASSSTM_RING_OFF,           stateClidTxJpIdle,         {actionNormalPolarityIdle, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateClidTxJpWaitOnHook[] = {
{CLASSSTM_ONHOOK,             stateClidTxJpIdle,         {actionNormalPolarityIdle, actionClidTxCleanup, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};



/* --------------------------------------------------------------------------
** Off-hook caller-id state machines.
*/

/*
**    In order to transmit off-hook caller id, the subscriber alerting signal (SAS)
**    is first sent to the handset. The SAS tone usually consists of a call waiting
**    tone. This is followed by the CPE alerting signal (CAS). This indicates
**    to the handset that caller id info is about to be transmitted. In response
**    to the CAS, the handset transmits an ACK signal (DTMF digit). Upon reception
**    of the ACK, the caller id data is transmitted to the handset.
**
**
** EVENT                      NEXT STATE                    ACTION FUNCTIONS
*/

/* Idle state */
const SME_STATE stateClass2Idle[] = {
{CLASSSTM_CLID_SAS_CAS,       stateClass2PendingCallid,   {0, 0, 0}},
{CLASSSTM_CLID_SAS_ONLY,      stateClass2PendingDelayDone,{actionStartSasWithDelay, 0, 0}},
{CLASSSTM_CLID_SAS_CAS_OFF,   stateClass2Idle,            {actionStopSasCas, 0, 0}},
{SME_EVENT_NULL,              0,                          {0, 0, 0}}};

/* The SAS & CAS tone profiles should have already been set. Wait for the arrival
** of caller id data before starting SAS tone. */
const SME_STATE stateClass2PendingCallid[] = {
{CLASSSTM_CLID_INFO,          stateClass2PendingSasDone,  {actionClidTxFormatData, actionStartFirstSas, 0}},
{CLASSSTM_CLID_SAS_CAS_OFF,   stateClass2Idle,            {actionStopSasCas, 0, 0}},
{SME_EVENT_NULL,              0,                          {0, 0, 0}}};

/* Wait for the SAS tone to complete ebfore sending CAS tone profile */
const SME_STATE stateClass2PendingSasDone[] = {
{CLASSSTM_TONE_GEN_EVT,       stateClass2PendingAck,      {actionBlockPulseCasEvents, actionEnableFastSlicModes, actionStartCas}},
{CLASSSTM_CLID_SAS_CAS_OFF,   stateClass2Idle,            {actionStopSasCas, 0, 0}},
{SME_EVENT_NULL,              0,                          {0, 0, 0}}};

/* Wait for the reception of the ACK (DTMFx tone) from the handset. A timeout
** will skip the sending of FSK and continue on to repeat the SAS tone */
const SME_STATE stateClass2PendingAck[] = {
{CLASSSTM_CLID_ACK_RCVD,      stateClass2SendFsk,         {actionDisableFastSlicModes, actionClidTxStart, 0}},
{CLASSSTM_TONE_GEN_EVT,       stateClass2PendingDelayDone,{actionDisableFastSlicModes, 0, 0}},
{CLASSSTM_CLID_SAS_CAS_OFF,   stateClass2Idle,            {actionDisableFastSlicModes, actionUnblockPulseCasEvents, actionStopSasCas}},
{CLASSSTM_ONHOOK,             stateClass2Idle,            {actionDisableFastSlicModes, actionUnblockPulseCasEvents, actionStopSasCas}},
{SME_EVENT_NULL,              0,                          {0, 0, 0}}};

/* Transmitting caller id info. Wait until transmission is done. */
const SME_STATE stateClass2SendFsk[] = {
{CLASSSTM_CLID_TX_END,        stateClass2PendingDelayDone,{actionUnblockPulseCasEvents, 0, 0}},
{CLASSSTM_TIMER_EXPIRE,       stateClass2PendingDelayDone,{actionClidTxCleanup, actionUnblockPulseCasEvents, 0}},
{CLASSSTM_CLID_SAS_CAS_OFF,   stateClass2Idle,            {actionClidTxCleanup, actionUnblockPulseCasEvents, actionStopSasCas}},
{SME_EVENT_NULL,              0,                          {0, 0, 0}}};

/* Wait for the end of the last tone profile, then repeat the SAS tone with delay */
const SME_STATE stateClass2PendingDelayDone[] = {
{CLASSSTM_TONE_GEN_EVT,       stateClass2Idle,            {actionStartSasWithDelay, actionUnblockPulseCasEvents, 0}},
{CLASSSTM_CLID_SAS_CAS_OFF,   stateClass2Idle,            {actionStopSasCas, actionUnblockPulseCasEvents, 0}},
{SME_EVENT_NULL,              0,                          {0, 0, 0}}};



/* --------------------------------------------------------------------------
** Visual Message Waiting Indicator (VMWI) state machines.
*/

/*
** VMWI using OSI (open switch interval) - North America.
**
** EVENT                      NEXT STATE                 ACTION FUNCTIONS
*/
const SME_STATE stateVmwiIdle[] = {
{CLASSSTM_VMWI_ON,            stateVmwiSendOsi,          {actionVmwiOnFormatData, actionVmwiSupervisoryDisconnect, 0}},
{CLASSSTM_VMWI_OFF,           stateVmwiSendOsi,          {actionVmwiOffFormatData, actionVmwiSupervisoryDisconnect, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiSendOsi[] = {
{CLASSSTM_VMWI_OSI_END,       stateVmwiSendFsk,          {actionClidTxStart, 0, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiIdle,             {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiSendFsk[] = {
{CLASSSTM_CLID_TX_END,        stateVmwiIdle,             {0, 0, 0}},
{CLASSSTM_TONE_GEN_EVT,       stateVmwiIdle,             {0, 0, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiIdle,             {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};



/*
** DT-AS (dual tone alert signal) VMWI.
**
** EVENT                      NEXT STATE                 ACTION FUNCTIONS
*/
const SME_STATE stateVmwiTxDtAsIdle[] = {
{CLASSSTM_VMWI_ON,            stateVmwiTxDtAsSendDtAs,   {actionOnhookTransmission, actionVmwiOnFormatData, actionSendDualToneSignal}},
{CLASSSTM_VMWI_OFF,           stateVmwiTxDtAsSendDtAs,   {actionOnhookTransmission, actionVmwiOffFormatData, actionSendDualToneSignal}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxDtAsSendDtAs[] = {
{CLASSSTM_TONE_GEN_EVT,       stateVmwiTxDtAsPreClidTx,  {actionFskAfterDtasDelay, 0, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxDtAsIdle,       {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxDtAsPreClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateVmwiTxDtAsClidTx,     {actionClidTxStart, 0, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxDtAsIdle,       {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxDtAsClidTx[] = {
{CLASSSTM_CLID_TX_END,        stateVmwiTxDtAsPostClidTx, {actionPostVmwiFskDelay, 0, 0}},
{CLASSSTM_TONE_GEN_EVT,       stateVmwiTxDtAsPostClidTx, {actionPostVmwiFskDelay, 0, 0}},
{CLASSSTM_TIMER_EXPIRE,       stateVmwiTxDtAsIdle,       {actionClidTxCleanup, actionNormalPolarityIdle, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxDtAsIdle,       {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxDtAsPostClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateVmwiTxDtAsIdle,       {actionNormalPolarityIdle, 0, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxDtAsIdle,       {0, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};



/*
** RP-AS (ring pulse alert signal) VMWI.
**
** EVENT                      NEXT STATE                 ACTION FUNCTIONS
*/
const SME_STATE stateVmwiTxRpAsIdle[] = {
{CLASSSTM_VMWI_ON,            stateVmwiTxRpAsSendRpAs,   {actionVmwiOnFormatData, actionSendRingPulseSignal,0}},
{CLASSSTM_VMWI_OFF,           stateVmwiTxRpAsSendRpAs,   {actionVmwiOffFormatData, actionSendRingPulseSignal,0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxRpAsSendRpAs[] = {
{CLASSSTM_TIMER_EXPIRE,       stateVmwiTxRpAsPreClidTx,  {actionFskAfterRpasDelay, 0, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxRpAsIdle,       {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxRpAsPreClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateVmwiTxRpAsClidTx,     {actionClidTxStart, 0, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxRpAsIdle,       {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxRpAsClidTx[] = {
{CLASSSTM_CLID_TX_END,        stateVmwiTxRpAsPostClidTx, {actionPostVmwiFskDelay, 0, 0}},
{CLASSSTM_TONE_GEN_EVT,       stateVmwiTxRpAsPostClidTx, {actionPostVmwiFskDelay, 0, 0}},
{CLASSSTM_TIMER_EXPIRE,       stateVmwiTxRpAsIdle,       {actionClidTxCleanup, actionNormalPolarityIdle, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxRpAsIdle,       {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxRpAsPostClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateVmwiTxRpAsIdle,       {actionNormalPolarityIdle, 0, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxRpAsIdle,       {0, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};



/*
** LR-AS (Loop reversal, dual tone alert signal) VMWI.
**
** EVENT                      NEXT STATE                 ACTION FUNCTIONS
*/
const SME_STATE stateVmwiTxLrAsIdle[] = {
{CLASSSTM_VMWI_ON,            stateVmwiTxLrAsPreDtas,    {actionVmwiOnFormatData, actionDtasAfterLrDelay,0}},
{CLASSSTM_VMWI_OFF,           stateVmwiTxLrAsPreDtas,    {actionVmwiOffFormatData, actionDtasAfterLrDelay,0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxLrAsPreDtas[] = {
{CLASSSTM_TIMER_EXPIRE,       stateVmwiTxLrAsSendDtAs,   {actionSendDualToneSignal, 0, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxLrAsIdle,       {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxLrAsSendDtAs[] = {
{CLASSSTM_TONE_GEN_EVT,       stateVmwiTxLrAsPreClidTx,  {actionFskAfterLrDtasDelay, 0, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxLrAsIdle,       {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxLrAsPreClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateVmwiTxLrAsClidTx,     {actionClidTxStart, 0, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxLrAsIdle,       {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxLrAsClidTx[] = {
{CLASSSTM_CLID_TX_END,        stateVmwiTxLrAsPostClidTx, {actionPostVmwiFskDelay, 0, 0}},
{CLASSSTM_TONE_GEN_EVT,       stateVmwiTxLrAsPostClidTx, {actionPostVmwiFskDelay, 0, 0}},
{CLASSSTM_TIMER_EXPIRE,       stateVmwiTxLrAsIdle,       {actionClidTxCleanup, actionNormalPolarityIdle, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxLrAsIdle,       {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};

const SME_STATE stateVmwiTxLrAsPostClidTx[] = {
{CLASSSTM_TIMER_EXPIRE,       stateVmwiTxLrAsIdle,       {actionClidTxCleanup, actionNormalPolarityIdle, 0}},
{CLASSSTM_OFFHOOK,            stateVmwiTxLrAsIdle,       {actionClidTxCleanup, 0, 0}},
{SME_EVENT_NULL,              0,                         {0, 0, 0}}};




/* --------------------------------------------------------------------------
** Action functions.
*/

/*-----------------------*/
/* CAS control functions */
/*-----------------------*/


static int actionStartRing( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   /* Ensure CAS processing is enabled */
   casCtlIgnoreHookState( parmp->casCtlHandle, CAS_IGNOREHOOKSTATE_OFF );

   if ( parmp->classType == CLASS_MSG_ONHOOK_CLID )
   {
      /* clidTxDelay defines the timing of the CLID TX data transmission start
      ** (FSK) after the end of the first ring. Because the command to start
      ** the FSK has to go through Host-DSP interface, there is a processing
      ** delay we need to account for in order to meet the exact timing. */
      if (parmp->ringInfo->clidTxDelay > FSK_AFTER_RINGING_PROCESSING_DELAY_MSEC)
      {
         parmp->ringInfo->clidTxDelay -= FSK_AFTER_RINGING_PROCESSING_DELAY_MSEC;
      }
      else
      {
         parmp->ringInfo->clidTxDelay = 0;
      }

      /* Call CAS control function to turn ring profile on */
      casCtlRingProfileOn( parmp->casCtlHandle, parmp->ringInfo, VRG_TRUE );
   }

   return( SME_EVENT_NULL );
}



static int actionStopRing( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   /* Ensure CAS processing is enabled */
   casCtlIgnoreHookState( parmp->casCtlHandle, CAS_IGNOREHOOKSTATE_OFF );

   /* call CAS control function to turn ring off and go on hook */
   casCtlRingOff( parmp->casCtlHandle, VRG_FALSE );

   return( SME_EVENT_NULL );
}


static int actionReversePolarityAndCarRing( void *datap )
{
   CLASSSTM_PARM       *parmp;
   CLASSSTM_RING_STATE  ringState;

   parmp = (CLASSSTM_PARM *)datap;

   /* block certain CAS events from the host */
   *parmp->evBlockp = CLASSSTM_CASEVT_BLOCK_ALL;

   /* start ring from CAS control module */
#if VRG_COUNTRY_CFG_JAPAN
   ringState.pattern = CAS_CTL_RING_JAPAN_CAR;
#else
   ringState.pattern = 0;
#endif
   ringState.ringType = CAS_CTRL_STATIC_RING;
   casCtlRingProfileOn( parmp->casCtlHandle, &ringState, VRG_TRUE );

   return( SME_EVENT_NULL );
}


static int actionNormalPolarity( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   /* call CAS control function to set the polarity */
   casCtlChangePolarity( parmp->casCtlHandle,
                         CAS_CTL_POLARITY_FORWARD,
                         CAS_CTL_EVENT_NONE,
                         0 );

   return( SME_EVENT_NULL );
}


static int actionOffHookNormalPolarity( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   /* call CAS control function to set the polarity AND generate off-hook event */
   casCtlChangePolarity( parmp->casCtlHandle,
                         CAS_CTL_POLARITY_FORWARD,
                         CAS_CTL_EVENT_OFFHOOK,
                         0 );

   return( SME_EVENT_NULL );
}


static int actionNormalPolarityIdle( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   /* send CAS control command to go idle with normal polarity */
   casCtlSetState( parmp->casCtlHandle, CAS_CTL_FXO_IDLE );

   return( SME_EVENT_NULL );
}


static int actionOnhookTransmission( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   /* send CAS control command to go on-hook data transmission mode. */
   casCtlSetState( parmp->casCtlHandle, CAS_CTL_FXO_ALERTING_OHT );

   return( SME_EVENT_NULL );
}


static int actionSendRingPulseSignal( void *datap )
{
   CLASSSTM_PARM   *parmp;
   VRG_UINT32      duration;

   parmp = (CLASSSTM_PARM *)datap;

   duration = RING_PULSE_DURATION_MSEC;
#if VRG_COUNTRY_CFG_INDIA
   if (parmp->country == VRG_COUNTRY_INDIA)
   {
      duration = 400;
   }
#endif

   /* send CAS control command to go on-hook data transmission mode. */
   casCtlSetStateGenEvent( parmp->casCtlHandle,
                           CAS_CTL_FXO_ALERTING_RING,
                           duration );


   return( SME_EVENT_NULL );
}


static int actionFskAfterDtasDelay( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   casCtlSetStateGenEvent( parmp->casCtlHandle,
                           CAS_CTL_FXO_ALERTING_OHT,
                           parmp->fskAfterDtas - FSK_AFTER_ALERTING_PROCESSING_DELAY_MSEC );

   return( SME_EVENT_NULL );
}

static int actionFskAfterLrDtasDelay( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   /* Maintain line reversal state and delay the
   ** start of CLID data for the required time,
   ** then generate an TIMER_EXPIRY event to move on */
   casCtlSetStateGenEvent( parmp->casCtlHandle,
                           CAS_CTL_FXO_ALERTING_OHTR,
                           parmp->fskAfterDtas - FSK_AFTER_ALERTING_PROCESSING_DELAY_MSEC );

   return( SME_EVENT_NULL );
}

static int actionFskAfterRpasDelay( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   casCtlSetStateGenEvent( parmp->casCtlHandle,
                           CAS_CTL_FXO_ALERTING_OHT,
                           parmp->fskAfterRp - FSK_AFTER_RINGING_PROCESSING_DELAY_MSEC );

   return( SME_EVENT_NULL );
}


static int actionRingAfterFskDelay( void *datap )
{
   CLASSSTM_PARM   *parmp;
   VRG_UINT32       duration;

   parmp = (CLASSSTM_PARM *)datap;

   switch ( parmp->classType )
   {
      case CLASS_MSG_ONHOOK_CLID:
      {
         if ( parmp->clidProtocol == CLASS_SIG_TYPE_DTMF_CLID )
         {
            duration = parmp->ringAfterFsk - parmp->dtmfParmp->pulseTime - DTMF_TONE_GEN_EVENT_DELAY_MSEC;
         }
         else
         {
            duration = parmp->ringAfterFsk;
         }
      }
      break;

      default:
      {
         duration = parmp->ringAfterFsk;
      }
      break;
   }

   casCtlSetStateGenEvent( parmp->casCtlHandle,
                           CAS_CTL_FXO_ALERTING_OHT,
                           duration - RING_AFTER_FSK_PROCESSING_DELAY_MSEC );

   return( SME_EVENT_NULL );
}


static int actionDtasAfterLrDelay( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   /* Disable CAS processing on this endpoint */
   casCtlIgnoreHookState( parmp->casCtlHandle, CAS_IGNOREHOOKSTATE_ON );

   /* Perform a line reversal and delay for required time
   ** before generating TIMER EXPIRY event */
   casCtlSetStateGenEvent( parmp->casCtlHandle,
                           CAS_CTL_FXO_ALERTING_OHTR,
                           parmp->dtasAfterLr - FSK_AFTER_RINGING_PROCESSING_DELAY_MSEC );

   return( SME_EVENT_NULL );
}


static int actionPostVmwiFskDelay( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   casCtlSetStateGenEvent( parmp->casCtlHandle,
                           CAS_CTL_FXO_ALERTING_OHT,
                           POST_VMWI_FSK_DELAY_MSEC );

   return( SME_EVENT_NULL );
}



/*--------------------------*/
/* CLIDTX Service functions */
/*--------------------------*/

static int actionClidTxStart( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   switch ( parmp->classType )
   {
      case CLASS_MSG_VMWI:
      {
         if ( parmp->vmwiProtocol == CLASS_SIG_TYPE_DTMF_VMWI )
         {
            /* Call HDSP function to start the DTMF transmitter */
            hdspClassDtmfTransmitVmwi( parmp->vhdHdl, parmp->vmwiType, parmp->dtmfParmp );
         }
         else
         {
            /* Call HDSP function to start the CLID transmitter */
            hdspClassTransmit( parmp->vhdHdl );
         }
      }
      break;

      case CLASS_MSG_ONHOOK_CLID:
      {
         if ( parmp->clidProtocol == CLASS_SIG_TYPE_DTMF_CLID )
         {
            /* Call HDSP function to start the DTMF transmitter */
            hdspClassDtmfTransmit( parmp->vhdHdl, parmp->clidInfo, parmp->dtmfParmp );
         }
         else
         {
            /* Call HDSP function to start the CLID transmitter */
            hdspClassTransmit( parmp->vhdHdl );
         }
      }
      break;

      default:
      {
         /* Call HDSP function to start the CLID transmitter */
         hdspClassTransmit( parmp->vhdHdl );
      }
      break;
   }

   return( SME_EVENT_NULL );
}


static int actionSendDualToneSignal( void *datap )
{
   CLASSSTM_PARM          *parmp;
   parmp = (CLASSSTM_PARM *)datap;

	/* Static profile will be used for CAS tone, tone pattern bit = NULL */
   if ( HAPISUCCESS != hdspToneOn( parmp->vhdHdl,
                                   parmp->lineId,
                                   CT_DTAS_ONHOOK,
                                   0,
                                   0,
                                   NULL,
                                   EGRESS_TONE,
                                   VRG_FALSE,
                                   parmp->country ))
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "ERROR: actionSendDualToneSignal - failed to play DTAS signal " ));
   }

   return( SME_EVENT_NULL );
}


static int actionClidTxCleanup( void *datap )
{
   CLASSSTM_PARM   *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   /* turn off CAS event blocking */
   *parmp->evBlockp = CLASSSTM_CASEVT_BLOCK_NONE;

   /* call HDSP function to stop the CLIDTX transmitter */
   if ( hdspClassStopTransmit( parmp->vhdHdl ))
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "HDSP: Class Stop Failed in startup on vhdHdl %x", parmp->vhdHdl ));
   }

   /* Ensure CAS processing is enabled */
   casCtlIgnoreHookState( parmp->casCtlHandle, CAS_IGNOREHOOKSTATE_OFF );

   return( SME_EVENT_NULL );
}


static int actionVmwiOnFormatData( void *datap )
{
   CLASSSTM_PARM *parmp = (CLASSSTM_PARM *)datap;

   if ( parmp->vmwiProtocol == CLASS_SIG_TYPE_DTMF_VMWI )
   {
      /* DTMF VMWI - no class message is necessary */
      return( SME_EVENT_NULL );
   }

   return ( actionVmwi((CLASSSTM_PARM *)datap, CLASS_VMWI_ACTIVATE ));
}


static int actionVmwiOffFormatData( void *datap )
{
   CLASSSTM_PARM *parmp = (CLASSSTM_PARM *)datap;

   if ( parmp->vmwiProtocol == CLASS_SIG_TYPE_DTMF_VMWI )
   {
      /* DTMF VMWI - no class message is necessary */
      return( SME_EVENT_NULL );
   }

   return ( actionVmwi((CLASSSTM_PARM *)datap, CLASS_VMWI_DEACTIVATE ));
}


static int actionVmwi( CLASSSTM_PARM *parmp, VRG_UINT16 type )
{
   VRG_SINT16 rc;

   /* Stop CLIDTX Srv so that its TX queue is flushed of any previous CLASS packets */
   hdspClassStopTransmit( parmp->vhdHdl );

   /* Assemble VMWI message */
   rc = hdspClassVmwi( parmp->vhdHdl, parmp->lineId, type, parmp->vmwiMsgFormat, parmp->fskParmp, parmp->country );
   if ( rc != HAPISUCCESS )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "actionVmwi: failed to send data, vhdHdl %d", parmp->vhdHdl ));
      return( SME_EVENT_NULL );
   }

   return( SME_EVENT_NULL );
}


static int actionVmwiSupervisoryDisconnect( void *datap )
{
   CLASSSTM_PARM    *parmp = (CLASSSTM_PARM *)datap;

   casCtlSupervisoryDisconnect( parmp->casCtlHandle, VMWI_OSI_DURATION_MSEC, CAS_OSI_VMWI );

   return( SME_EVENT_NULL );
}


static int actionClidTxFormatData( void *datap )
{
   CLASSSTM_PARM    *parmp;
   VRG_SINT16        rc;
   VRG_UINT16        clidTxDelay = 0;


   parmp = (CLASSSTM_PARM *)datap;


   /*
   ** Stop CLIDTX Srv so that its TX queue is flushed of any previous CLASS packets
   */
   if (hdspClassStopTransmit( parmp->vhdHdl ))
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "CLASSSTM: hdspClassStopTransmit failed - vhdHdl=0x(%x)!!!", parmp->vhdHdl));
   }

   switch ( parmp->classType )
   {
      case CLASS_MSG_ONHOOK_CLID:
      {
         if ( parmp->clidProtocol == CLASS_SIG_TYPE_DTMF_CLID )
         {
            /* DTMF Caller ID - no class message is necessary */
            return( SME_EVENT_NULL );
         }
         else
         {
            /*
            ** The delay between the end of the alerting signal and start of FSK
            ** is handled by the CAS state machine. Therefore, tell hausware
            ** to generate the FSK immediately.
            */
            clidTxDelay = 0;
        }
      }
      break;

      case CLASS_MSG_OFFHOOK_CLID:
      {
         clidTxDelay = CLASS_CLID_OFFHOOK_DELAY_DEFAULT;
      }
      break;

      default:
      {
      }
      break;
   }

   /*
   ** Packetize the caller ID data
   */
   rc = hdspClassMessage( parmp->clidInfo,
                          parmp->vhdHdl,
                          parmp->lineId,
                          parmp->country,
                          parmp->classType,
                          parmp->fskParmp,
                          clidTxDelay );

   if ( rc )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "actionClidTxFormatData: failed to send data, vhdHdl %d", parmp->vhdHdl ));
   }

   return( SME_EVENT_NULL );
}


/*--------------------------*/
/* Tone Service functions   */
/*--------------------------*/


static int actionStartFirstSas( void *datap )
{
   CLASSSTM_PARM          *parmp;
   HDSP_TONE_PROV_PATTERN *pToneProfile;

   parmp = (CLASSSTM_PARM *)datap;

   pToneProfile = &parmp->callWtgInfo->sasCustomProfile;

   /* Provisioned tone pattern will be used for SAS tone, tone name will only be used to send an event in hdspToneon*/
   if ( HAPISUCCESS != hdspToneOn( parmp->vhdHdl,
                                   parmp->lineId, 
                                   parmp->callWtgInfo->profile,
                                   CLASS_CLID_SAS_CAS_DELAY,
                                   HAPI_PTE_CADBIT_COMPLETE | HAPI_PTE_CADBIT_EVENT, /* Need event to repeat CW tone */
                                   pToneProfile,
                                   EGRESS_TONE,
                                   VRG_FALSE,
                                   parmp->country ))
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "ERROR: actionStartSasCas - failed to turn on tone" ));
   }

   return ( SME_EVENT_NULL );
}


static int actionStartSasWithDelay( void *datap )
{
   CLASSSTM_PARM          *parmp;
   HDSP_TONE_PROV_PATTERN *pToneProfile;

   parmp = (CLASSSTM_PARM *)datap;

   pToneProfile = &parmp->callWtgInfo->sasCustomProfile;

   /* Provisioned tone pattern will be used for SAS tone, tone name will only be used to send an event in hdspToneon*/
   if ( HAPISUCCESS != hdspToneOn( parmp->vhdHdl,
                                   parmp->lineId,
                                   parmp->callWtgInfo->profile,
                                   (VRG_UINT32)parmp->callWtgInfo->delay,
                                   0,
                                   pToneProfile,
                                   EGRESS_TONE,
                                   VRG_FALSE,
                                   parmp->country ))
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "ERROR: actionStartSasCas - failed to turn on tone" ));
   }

   return ( SME_EVENT_NULL );
}


static int actionStartCas( void *datap )
{
   CLASSSTM_PARM          *parmp;
   parmp = (CLASSSTM_PARM *)datap;

	/* Static profile will be used for CAS tone, tone pattern bit = NULL */
   if ( HAPISUCCESS != hdspToneOn( parmp->vhdHdl,
                                   parmp->lineId,
                                   CT_DTAS_OFFHOOK,
                                   parmp->callWtgInfo->delay,
                                   0,
                                   NULL,
                                   EGRESS_TONE,
                                   VRG_FALSE,
                                   parmp->country ))
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "ERROR: actionStartSasCas - failed to turn on tone" ));
   }

   return ( SME_EVENT_NULL );
}


static int actionStopSasCas( void *datap )
{
   CLASSSTM_PARM   *parmp;
   VRG_SINT16 rc;

   parmp = (CLASSSTM_PARM *)datap;

   rc = hdspToneOff( parmp->vhdHdl, EGRESS_TONE );
   if ( rc != HAPISUCCESS )
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT,  "ERROR: actionStopSasCas - failed to turn off tone" ));
   }

   return ( SME_EVENT_NULL );
}

static int actionBlockPulseCasEvents( void *datap )
{
   CLASSSTM_PARM          *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   /* Block CAS pulse-dial events from being notified.
   ** This is because some CWCID devices purposfully present an onhook state
   ** on the line to check for presence of extension phones.
   ** This may look like a pulse digit event if we don't block
   ** the CAS events here */
   *parmp->evBlockp = CLASSSTM_CASEVT_BLOCK_PULSE;

   return ( SME_EVENT_NULL );
}

static int actionUnblockPulseCasEvents( void *datap )
{
   CLASSSTM_PARM          *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   /* Unblock CAS events, let them be notified */
   *parmp->evBlockp = CLASSSTM_CASEVT_BLOCK_NONE;

   return ( SME_EVENT_NULL );
}

static int actionEnableFastSlicModes( void *datap )
{
   CLASSSTM_PARM          *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   casCtlEnableFastSlicStateMode(parmp->casCtlHandle, VRG_TRUE);

   return ( SME_EVENT_NULL );
}

static int actionDisableFastSlicModes( void *datap )
{
   CLASSSTM_PARM          *parmp;

   parmp = (CLASSSTM_PARM *)datap;

   casCtlEnableFastSlicStateMode(parmp->casCtlHandle, VRG_FALSE);

   return ( SME_EVENT_NULL );
}


/****************************************************************************
*
*  classStmEventProcess - custom local area signalling service state machine
*
*****************************************************************************
*
*  PURPOSE:
*     This function processes events through the state machine engine.
*
*  PARAMETERS:
*     stmMemp  - pointer to state machine instance memory
*     event    - event to process
*
*  RETURNS:
*     Nothing
*
*  NOTE:
*
****************************************************************************/
void classStmEventProcess( CLASSSTM_MEM *clidState, int event, int lineId )
{
   int            transitions;
   CLASSSTM_PARM  stmParm;


   /* Don't process event if state-machine hasn't been initialized */
   if ( clidState->currentStatep == NULL )
   {
      return;
   }


   /* fill in the parmeter structure for the state machine */
   stmParm.casCtlHandle    = clidState->casCtlHandle;
   stmParm.vhdHdl          = clidState->vhdHdl;
   stmParm.ringInfo        = &clidState->ringInfo;
   stmParm.evBlockp        = &clidState->evBlock;
   stmParm.clidInfo        = &clidState->clidInfo;
   stmParm.classType       = clidState->classType;
   stmParm.vmwiType        = clidState->vmwiType;
   stmParm.country         = clidState->country;
   stmParm.callWtgInfo     = &clidState->callWtgInfo;
   stmParm.fskAfterRing    = clidState->fskAfterRing;
   stmParm.ringAfterFsk    = clidState->ringAfterFsk;
   stmParm.fskAfterDtas    = clidState->fskAfterDtas;
   stmParm.fskAfterRp      = clidState->fskAfterRp;
   stmParm.dtasAfterLr     = clidState->dtasAfterLr;
   stmParm.clidProtocol    = clidState->clidProtocol;
   stmParm.vmwiProtocol    = clidState->vmwiProtocol;
   stmParm.vmwiMsgFormat   = clidState->vmwiMsgFormat;
   stmParm.fskParmp        = clidState->fskParmp;
   stmParm.dtmfParmp       = clidState->dtmfParmp;
   stmParm.lineId          = lineId;


#if CLID_DEBUG
   getEventString( event, testString );
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "*** Input event '%s' 0x%lx", testString, event ));
   getStateString( clidState->currentStatep, testString );
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "*** Line %d: Old state = %s", clidState->casCtlHandle, testString ));
#endif


   /* run the event through the state machine */
   clidState->currentStatep = smeProcess( clidState->currentStatep,
                                          &stmParm,
                                          event,
                                          &transitions );

#if CLID_DEBUG
   getStateString( clidState->currentStatep, testString );
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "*** Line %d: New state = %s", clidState->casCtlHandle, testString ));
#endif

}


/****************************************************************************
*
*  classStmInit
*
*****************************************************************************
*
*  PURPOSE:
*     Initialize the state machine memory for one channel.
*
*  PARAMETERS:
*     stmMemp  - pointer to state machine instance memory
*     casCtlHandle    - physical endpoint number (0-referenced)
*     vhdhdl   - VHD handle used to transmit caller id
*     country  - country type
*
*  RETURNS:
*     Nothing
*
*  NOTE:
*
****************************************************************************/
VRG_SINT16 classStmInit
(
   CLASSSTM_MEM  *stmMemp,
   VRG_UINT16     casCtlHandle,
   VRG_UINT16     vhdhdl,
   VRG_COUNTRY    country
)
{
   memset( stmMemp, 0, sizeof( CLASSSTM_MEM ));

   stmMemp->casCtlHandle   = casCtlHandle;
   stmMemp->vhdHdl         = vhdhdl;
   stmMemp->country        = country;
   stmMemp->evBlock        = VRG_FALSE;
 
   classStmReset( stmMemp, CLASSSTM_TYPE_ONHOOK_RING_CLID );


   if ( hdspClassEnable( vhdhdl ))
   {
      VRG_LOG_ERROR((VRG_LOG_MOD_EPT, "HDSP: Class enable Failed on vhdHdl %x", vhdhdl ));
      return ( HAPIFAILURE );
   }

   return ( HAPISUCCESS );
}


/****************************************************************************
*
*  classStmDeinit
*
*****************************************************************************
*
*  PURPOSE:
*     De-initialize the state machine memory for one channel.
*
*  PARAMETERS:
*     stmMemp - pointer to state machine instance memory
*
*  RETURNS:
*     Nothing
*
*  NOTE:
*
****************************************************************************/
void classStmDeinit( CLASSSTM_MEM *stmMemp )
{
#if 0
   /* CLID service is disabled with hapiClose */
   hdspClassDisable( stmMemp->vhdHdl );
#endif
   memset( stmMemp, 0, sizeof( CLASSSTM_MEM ));
}



/****************************************************************************
*
*  classStmReset
*
*****************************************************************************
*
*  PURPOSE:
*     Reset state machine data. The state is set based upon the requested
*     CLASS type (e.g. CLASS1, CLASS2, VMWI). Since multiple logical state
*     machines (one per CLASS type) are multiplexed using a single physical
*     state machine, it is important this function be invoked prior to sending
*     events to any of the logical state machines.
*
*  PARAMETERS:
*     stmMemp     - pointer to state machine instance memory
*     classType   - class type (class1, class2, vmwi...)
*
*  RETURNS:
*     Nothing
*
*  NOTE:
*
****************************************************************************/
void classStmReset( CLASSSTM_MEM *clidState, CLASSSTM_TYPE classStmType )
{
   CAS_CTL_HANDLE casCtlHandle   = clidState->casCtlHandle;
   VRG_UINT16     vhdHdl         = clidState->vhdHdl;
   VRG_COUNTRY    country        = clidState->country;


   XCHG_ASSERT( clidState->country < VRG_COUNTRY_MAX );

   memset( clidState, 0, sizeof( CLASSSTM_MEM ) );

   clidState->vhdHdl       = vhdHdl;
   clidState->casCtlHandle = casCtlHandle;
   clidState->country      = country;

   /*
   ** North American and European state machine.
   */
   if (classStmType == CLASSSTM_TYPE_OFFHOOK_CLID)
   {
      clidState->currentStatep   = stateClass2Idle;
      clidState->classType       = CLASS_MSG_OFFHOOK_CLID;
   }
   else if (classStmType == CLASSSTM_TYPE_ONHOOK_RING_CLID)
   {
      clidState->currentStatep   = stateClidTxNaIdle;
      clidState->classType       = CLASS_MSG_ONHOOK_CLID;
   }
   else if ( classStmType == CLASSSTM_TYPE_ONHOOK_LRCARIR_CLID )
   {
      clidState->currentStatep   = stateClidTxJpIdle;
      clidState->classType       = CLASS_MSG_ONHOOK_CLID;
   }
   else if (classStmType == CLASSSTM_TYPE_ONHOOK_DTAS_CLID)
   {
      clidState->currentStatep   = stateClidTxDtAsIdle;
      clidState->classType       = CLASS_MSG_ONHOOK_CLID;
   }
   else if (classStmType == CLASSSTM_TYPE_ONHOOK_RPAS_CLID)
   {
      clidState->currentStatep   = stateClidTxRpAsIdle;
      clidState->classType       = CLASS_MSG_ONHOOK_CLID;
   }
   else if (classStmType == CLASSSTM_TYPE_ONHOOK_LRAS_CLID)
   {
      clidState->currentStatep   = stateClidTxLrAsIdle;
      clidState->classType       = CLASS_MSG_ONHOOK_CLID;
   }
   else if ( classStmType == CLASSSTM_TYPE_OSI_VMWI )
   {
      clidState->currentStatep   = stateVmwiIdle;
      clidState->classType       = CLASS_MSG_VMWI;
   }
   else if ( classStmType == CLASSSTM_TYPE_DTAS_VMWI )
   {
      clidState->currentStatep   = stateVmwiTxDtAsIdle;
      clidState->classType       = CLASS_MSG_VMWI;
   }
   else if ( classStmType == CLASSSTM_TYPE_RPAS_VMWI )
   {
      clidState->currentStatep   = stateVmwiTxRpAsIdle;
      clidState->classType       = CLASS_MSG_VMWI;
   }
   else if ( classStmType == CLASSSTM_TYPE_LRAS_VMWI )
   {
      clidState->currentStatep   = stateVmwiTxLrAsIdle;
      clidState->classType       = CLASS_MSG_VMWI;
   }
   else
   {
      clidState->currentStatep = NULL;
   }

   /* Initialize state variables */
   clidState->idleStatep   = clidState->currentStatep;

   /* Ensure CAS processing is enabled */
   casCtlIgnoreHookState( casCtlHandle, CAS_IGNOREHOOKSTATE_OFF );

#if CLID_DEBUG
   getStateString( clidState->currentStatep, testString );
   VRG_LOG_DBG((VRG_LOG_MOD_EPT,  "*** classStmReset: Line %d; State = %s", clidState->casCtlHandle, testString ));
#endif
}


/****************************************************************************
*
*  classStmIsIdle - CLASS state machine check for idle state
*
*****************************************************************************
*
*  PURPOSE:
*     Check if the state machine is in the idle state
*
*  PARAMETERS:
*     stmMemp - pointer to state machine instance memory
*
*  RETURNS:
*     TRUE if in idle, otherwise FALSE
*
*  NOTE:
*
****************************************************************************/
VRG_BOOL classStmIsIdle( CLASSSTM_MEM *stmMemp )
{
   if ( stmMemp->classType == CLASS_MSG_OFFHOOK_CLID )
   {
      /*
      ** Offhook caller-id state machine. This is a special case because the
      ** offhook caller-id state machine also doubles as the call-waiting tone
      ** generation state machine. We will consider the call-waiting generation
      ** states to be "idle",  i.e. not part of the clid generation protocol.
      */

      if ( ( stmMemp->currentStatep != stateClass2Idle               ) &&
           ( stmMemp->currentStatep != stateClass2PendingDelayDone   )     )
      {
         return ( VRG_FALSE );
      }
   }
   else
   {
      /*
      ** Onhook caller-id or VMWI.
      */

      if ( stmMemp->currentStatep != stmMemp->idleStatep )
      {
         return ( VRG_FALSE );
      }
   }


   return ( VRG_TRUE );
}


/* Debug code */
#if CLID_DEBUG
void getStateString( const SME_STATE *state, char *string )
{
   string[0] = '\0';


   if ( state == stateClidTxNaIdle )
   {
      strcpy( string, "stateClidTxNaIdle" );
   }
#define ELSEIF_STATE_STRING( s )    \
   else if ( state == s )           \
   {                                \
      strcpy( string, #s );         \
   }
   ELSEIF_STATE_STRING( stateClidTxNaIdle           )
   ELSEIF_STATE_STRING( stateClidTxNaWaitSilence    )
   ELSEIF_STATE_STRING( stateClidTxNaSendFsk        )
   ELSEIF_STATE_STRING( stateClidTxNaContFsk        )

   ELSEIF_STATE_STRING( stateClidTxDtAsIdle         )
   ELSEIF_STATE_STRING( stateClidTxDtAsSendDtAs     )
   ELSEIF_STATE_STRING( stateClidTxDtAsPreClidTx    )
   ELSEIF_STATE_STRING( stateClidTxDtAsClidTx       )
   ELSEIF_STATE_STRING( stateClidTxDtAsPostClidTx   )
   ELSEIF_STATE_STRING( stateClidTxDtAsSendRing     )

   ELSEIF_STATE_STRING( stateClidTxRpAsIdle         )
   ELSEIF_STATE_STRING( stateClidTxRpAsSendRpAs     )
   ELSEIF_STATE_STRING( stateClidTxRpAsPreClidTx    )
   ELSEIF_STATE_STRING( stateClidTxRpAsClidTx       )
   ELSEIF_STATE_STRING( stateClidTxRpAsPostClidTx   )
   ELSEIF_STATE_STRING( stateClidTxRpAsSendRing     )

   ELSEIF_STATE_STRING( stateClidTxLrAsIdle         )
   ELSEIF_STATE_STRING( stateClidTxLrAsPreDtas      )
   ELSEIF_STATE_STRING( stateClidTxLrAsSendDtAs     )
   ELSEIF_STATE_STRING( stateClidTxLrAsPreClidTx    )
   ELSEIF_STATE_STRING( stateClidTxLrAsClidTx       )
   ELSEIF_STATE_STRING( stateClidTxLrAsPostClidTx   )
   ELSEIF_STATE_STRING( stateClidTxLrAsSendRing     )

   ELSEIF_STATE_STRING( stateClass2Idle             )
   ELSEIF_STATE_STRING( stateClass2PendingCallid    )
   ELSEIF_STATE_STRING( stateClass2PendingSasDone   )
   ELSEIF_STATE_STRING( stateClass2PendingAck       )
   ELSEIF_STATE_STRING( stateClass2SendFsk          )
   ELSEIF_STATE_STRING( stateClass2PendingDelayDone )

   ELSEIF_STATE_STRING( stateClidTxJpIdle           )
   ELSEIF_STATE_STRING( stateClidTxJpSendPr         )
   ELSEIF_STATE_STRING( stateClidTxJpSendCar        )
   ELSEIF_STATE_STRING( stateClidTxJpSendFsk        )
   ELSEIF_STATE_STRING( stateClidTxJpSendIr         )
   ELSEIF_STATE_STRING( stateClidTxJpWaitOnHook     )

   ELSEIF_STATE_STRING( stateVmwiIdle               )
   ELSEIF_STATE_STRING( stateVmwiSendOsi            )
   ELSEIF_STATE_STRING( stateVmwiSendFsk            )

   ELSEIF_STATE_STRING( stateVmwiTxDtAsIdle         )
   ELSEIF_STATE_STRING( stateVmwiTxDtAsSendDtAs     )
   ELSEIF_STATE_STRING( stateVmwiTxDtAsPreClidTx    )
   ELSEIF_STATE_STRING( stateVmwiTxDtAsClidTx       )
   ELSEIF_STATE_STRING( stateVmwiTxDtAsPostClidTx   )

   ELSEIF_STATE_STRING( stateVmwiTxRpAsIdle         )
   ELSEIF_STATE_STRING( stateVmwiTxRpAsSendRpAs     )
   ELSEIF_STATE_STRING( stateVmwiTxRpAsPreClidTx    )
   ELSEIF_STATE_STRING( stateVmwiTxRpAsClidTx       )
   ELSEIF_STATE_STRING( stateVmwiTxRpAsPostClidTx   )

   ELSEIF_STATE_STRING( stateVmwiTxLrAsIdle         )
   ELSEIF_STATE_STRING( stateVmwiTxLrAsPreDtas      )
   ELSEIF_STATE_STRING( stateVmwiTxLrAsSendDtAs     )
   ELSEIF_STATE_STRING( stateVmwiTxLrAsPreClidTx    )
   ELSEIF_STATE_STRING( stateVmwiTxLrAsClidTx       )
   ELSEIF_STATE_STRING( stateVmwiTxLrAsPostClidTx   )
   else
   {
      strcpy( string, "State unknown!" );
   }
}


void getEventString( int event, char *string )
{
   string[0] = '\0';

#define CASE_EVENT_STRING( event )  \
      case event:                   \
      {                             \
         strcpy( string, #event );  \
      }                             \
      break;

   switch ( event )
   {
      CASE_EVENT_STRING( CLASSSTM_CLID_RING           )
      CASE_EVENT_STRING( CLASSSTM_CLID_NODATA_RING    )
      CASE_EVENT_STRING( CLASSSTM_RING_OFF            )
      CASE_EVENT_STRING( CLASSSTM_CLID_FIRST_RINGOFF  )
      CASE_EVENT_STRING( CLASSSTM_OFFHOOK             )
      CASE_EVENT_STRING( CLASSSTM_ONHOOK              )
      CASE_EVENT_STRING( CLASSSTM_FLASH               )
      CASE_EVENT_STRING( CLASSSTM_TIMER_EXPIRE        )
      CASE_EVENT_STRING( CLASSSTM_CLID_TX_END         )
      CASE_EVENT_STRING( CLASSSTM_CLID_SAS_CAS        )
      CASE_EVENT_STRING( CLASSSTM_CLID_SAS_ONLY       )
      CASE_EVENT_STRING( CLASSSTM_CLID_INFO           )
      CASE_EVENT_STRING( CLASSSTM_CLID_SAS_CAS_OFF    )
      CASE_EVENT_STRING( CLASSSTM_TONE_GEN_EVT        )
      CASE_EVENT_STRING( CLASSSTM_CLID_ACK_RCVD       )
      CASE_EVENT_STRING( CLASSSTM_VMWI_ON             )
      CASE_EVENT_STRING( CLASSSTM_VMWI_OFF            )
      CASE_EVENT_STRING( CLASSSTM_VMWI_OSI_END        )
      default:
      {
         strcpy( string, "Event unknown!" );
      }
      break;
   }
}
#endif
