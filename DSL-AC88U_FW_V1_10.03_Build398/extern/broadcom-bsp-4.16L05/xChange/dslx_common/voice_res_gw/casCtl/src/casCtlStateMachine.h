/***************************************************************************
*    Copyright © 2000-2007 Broadcom                                        
*                                                                           
*    This program is the proprietary software of Broadcom and/or
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
*    Filename: casCtlStateMachine.h
*
****************************************************************************
*    Description:
*
*     Interface for internal CAS control state machine.
*
****************************************************************************/

#ifndef CAS_CTL_STATE_MACHINE_H
#define CAS_CTL_STATE_MACHINE_H

#ifdef __cplusplus
extern "C" {
#endif

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvCas.h>
#include <bosCritSect.h>
#include <casCtl.h>

/* ---- Constants and Types ---------------------------------------------- */

#define CAS_ENABLED      0x8000

/* Maximum number of states in state machine generator */
#define CAS_NUM_STATES           CAS_CTL_PROV_LONG_RING_MAX_INTERVAL

/* Flags associated with a CAS command */
#define CAS_CMD_FLAG_ASYNC       1 /* Command handled asynchronously */
#define CAS_CMD_FLAG_EXTENDTED   2 /* Command contains an extended buffer */
#define CAS_CMD_FLAG_MODIFIED    4 /* Extended buffer modified */
#define CAS_CMD_FLAG_DROP        8 /* Extended buffer can now be freed */

/* Operations on flag fields */
#define CAS_CMD_FLAG_IS_ASYNC(flags)    (*flags & CAS_CMD_FLAG_ASYNC)
#define CAS_CMD_FLAG_IS_EXTENDED(flags) (*flags & CAS_CMD_FLAG_EXTENDTED)
#define CAS_CMD_FLAG_IS_MODIFIED(flags) (*flags & CAS_CMD_FLAG_MODIFIED)
#define CAS_CMD_FLAG_IS_DROP(flags)     (*flags & CAS_CMD_FLAG_DROP)


/***************************************************************************
** CAS State Machine Creation
***************************************************************************/

typedef struct
{
   /* State action:
   ** Bits 31-18: Unused
   ** Bit  17:    Loop flag
   ** Bits 16-14: Interface state (enum FX_INTERFACE_STATE)
   ** Bits 13-8:  Next state
                  (CAS_NEXSTATE_0 to CAS_NEXTSTATE_60, or CAS_ENDING_STATE
   ** Bits 7-4:   CAS_CTL_EVENT event to generate
   ** Bits 3-0:   CAS bits generated in this state (DCBA)
   */
   VRG_UINT32 stateinfo;

   /* Duration of this state */
   VRG_UINT32 duration;

} CASSTATEINFO;

/* CAS Ring Profile */
typedef struct
{
   /* Loop counter */
   VRG_UINT16      loopcount;

   /* After loop state number */
   VRG_UINT16      afterloop;

   /* CAS state machine. Add an addition state for dummy state that causes
   ** state machine to transition to IDLE for non-repeating ring patterns that
   ** use all 216 states.*/
   CASSTATEINFO state[CAS_NUM_STATES + 1];

} CASSTM;

/* CAS Generation Profile */
typedef struct
{
   VRG_UINT16  index;                       /* Channel Index (0 to N) */
   CASSTM      casstm;

} CASPROFILE;



/* CAS event operand #2 */
typedef union
{
   VRG_UINT16   val;                          /* Op2 is a value */
   void*        ref;                          /* Op2 is a pointer */
} CASOP2;

/* CAS event interfaces */
typedef struct CASEVT
{
   VRG_UINT16 devhdl;                         /* device handle */
   VRG_UINT16 evt;                            /* event */
   VRG_UINT16 reqid;                          /* request ID */
   VRG_UINT16 op1;                            /* operand 1 */
   CASOP2     op2;                            /* operand 2 */
} CASEVT;

/* CAS control callback */
typedef void ( *CASCALLBKP )( CASEVT *, VRG_UINT16 );

/* CAS command interface */
typedef struct CASCMD
{
   VRG_UINT16 devhdl;                         /* device handle */
   VRG_UINT16 cmd;                            /* command */
   VRG_UINT16 reqid;                          /* request ID */
   VRG_UINT32 op1;                            /* operand 1 */
   CASOP2     op2;                            /* operand 2 */
   BOS_SEM        *semaphore;                 /* sempahore used for sync commands */
   CAS_CTL_STATUS *status;                    /* command status */
   VRG_UINT8      *flags;                     /* command flags */
} CASCMD;


/*
** Line Status Information
*/
typedef struct
{
   VRG_UINT16 channelnum;         /* Channel index number */
   VRG_UINT32 state;              /* Current state of line */
   VRG_UINT16 currentCAS;         /* Current CAS input bits */
   VRG_UINT16 currentCount;       /* Current count in this state */
   VRG_UINT16 deglitchCount;      /* Count down to ignore onhook glitches */
   VRG_UINT16 substate;           /* Current sub-state of line */
} CASSTATUS;


/* CAS Channel Information */
typedef struct
{
   /* Generation Information */
   VRG_UINT16 castype;            /* Type of CAS Interface */
   VRG_UINT16 channelnum;         /* Channel Index Number */
   VRG_UINT16 statechange;        /* Flag indicating state has changed */
   VRG_UINT32 state;              /* Bit defines for current state */
   VRG_UINT32 previousActiveState;/* Bit defines for previous state */
   VRG_UINT16 ignoreHookState;    /* Control variable to ignore hookstate */

   VRG_SINT16 counter;            /* Counter for current state */
   CASPROFILE *profilep;          /* Pointer to stored profile */
   XDRV_CAS   *casDriver;         /* Pointer to the CAS Driver */

   /* State change notification function */
   CAS_CTL_SLIC_CALLBACK casCallback;

   /* Monitor Information */
   VRG_UINT16 currentCAS;
   VRG_UINT16 currentCount;
   VRG_UINT16 deglitchCount;
   VRG_UINT16 newCount;
   VRG_UINT16 newCountFxs;
   VRG_UINT16 substate;
   VRG_UINT16 earlyOnState;
   VRG_UINT16 earlyOffState;
   VRG_UINT16 delayOnState;
   VRG_UINT16 delayOffState;

   /* Monitor Threshholds */
   CAS_CTL_FXO_PARMS fxoParms;
} CASCHANNELINFO;


#define CAS_ENABLE          0xE901   /* enable/disable CAS service */
                                     /* OP1: 1=ENABLE, 0=DISABLE   */
                                     /* OP2: reserved = 0          */

#define CAS_CONFIG_FXO      0xE902   /* Configs 1 of the N Channel profiles */
                                     /* OP1: Not Used */
                                     /* OP2: Pointer to FXO parameters */

#define CAS_CONFIG          0xE903   /* Configs 1 of the N Channel profiles */
                                     /* OP1: Channel Type (0=Inactive(Default) 1=FXO, 2=FXS, 4=E&M) */
                                     /* OP2: Channel Index (0 to N-1) */

#define CAS_STATECTL        0xE904   /* Controls what state to put channel in */
                                     /* OP1: sizeof(CASPROFILE) */
                                     /* OP2: * to state info */
                                     /* Extended data:  Channel Index */
                                     /*                 State Machine Info */

#define CAS_GET_LINESTATUS  0xE905   /* Get status of channel */
                                     /* OP1: sizeof(CASSTATUS)  */
                                     /* OP2: * to cas status info  */

#define CAS_SLIC_CMD        0xE906   /* Process SLIC command */
                                     /* ReqID: DAA command name */
                                     /* OP1: Command arguments */
                                     /* OP2: Command arguments */

#define CAS_DAA_CMD         0xE907   /* Process DAA command */
                                     /* ReqID: DAA command name */
                                     /* OP1: Command arguments */
                                     /* OP2: Command arguments */

#define CAS_APM_CMD         0xE908   /* Process APM command */
                                     /* ReqID: APM command name */
                                     /* OP1: Command arguments */
                                     /* OP2: Command arguments */

#define CAS_EXIT            0xE909   /* Cleanup resourses associated with CAS channel. */

#define CASTYPE_INACTIVE    0x0000
#define CASTYPE_FXO         0x0001


/*
** CAS Bits Defintions DCBA
*/
#define CASBITS_MASK     0x000F
#define CAS_ABCD_MASK    0x000F

/* FXO Outputs/FXS Inputs */
#define CASBITS_RINGING  0x0000
#define CASBITS_RLCF     0x0002
#define CASBITS_LCF      0x000A
#define CASBITS_LCFO     0x000F

/* FXS Outputs/FXO Inputs */
#define CASBITS_RG       0x0000
#define CASBITS_LO       0x000A
#define CASBITS_LC       0x000F

/* DSO common alarm signals (input/outputs) */
#define CASBITS_AIS      0x0004
#define CASBITS_YELLOW   0x000E

/* Control and Detection Events */
#define CAS_EVENT_MASK        0x000003F0
#define CAS_EVENT_SHIFT       4

#define CASEVENT_NOEVT           (CAS_CTL_EVENT_NONE << CAS_EVENT_SHIFT)
#define CASEVENT_ONHOOK          (CAS_CTL_EVENT_ONHOOK << CAS_EVENT_SHIFT)
#define CASEVENT_OFFHOOK         (CAS_CTL_EVENT_OFFHOOK << CAS_EVENT_SHIFT)
#define CASEVENT_WINK            (CAS_CTL_EVENT_WINK << CAS_EVENT_SHIFT)
#define CASEVENT_FLASH           (CAS_CTL_EVENT_FLASH << CAS_EVENT_SHIFT)
#define CASEVENT_RINGON          (CAS_CTL_EVENT_RINGON << CAS_EVENT_SHIFT)
#define CASEVENT_RINGOFF         (CAS_CTL_EVENT_RINGOFF << CAS_EVENT_SHIFT)
#define CASEVENT_FIRST_RINGOFF   (CAS_CTL_EVENT_FIRST_RINGOFF << CAS_EVENT_SHIFT)
#define CASEVENT_LCF             (CAS_CTL_EVENT_LCF << CAS_EVENT_SHIFT)
#define CASEVENT_RLCF            (CAS_CTL_EVENT_RLCF << CAS_EVENT_SHIFT)
#define CASEVENT_LCFO            (CAS_CTL_EVENT_LCFO << CAS_EVENT_SHIFT)
#define CASEVENT_EARLY_OFFHOOK   (CAS_CTL_EVENT_EARLY_OFFHOOK << CAS_EVENT_SHIFT)
#define CASEVENT_EARLY_ONHOOK    (CAS_CTL_EVENT_EARLY_ONHOOK << CAS_EVENT_SHIFT)
#define CASEVENT_DELAY_OFFHOOK   (CAS_CTL_EVENT_DELAY_OFFHOOK << CAS_EVENT_SHIFT)
#define CASEVENT_DELAY_ONHOOK    (CAS_CTL_EVENT_DELAY_ONHOOK << CAS_EVENT_SHIFT)
#define CASEVENT_PULSECOMPLETE   (CAS_CTL_EVENT_PULSECOMPLETE << CAS_EVENT_SHIFT)
#define CASEVENT_OSIEND          (CAS_CTL_EVENT_OSI_END << CAS_EVENT_SHIFT)
#define CASEVENT_TIMER_EXPIRE    (CAS_CTL_EVENT_TIMER_EXPIRE << CAS_EVENT_SHIFT)
#define CASEVENT_BRIEF_RING      (CAS_CTL_EVENT_BRIEF_RING << CAS_EVENT_SHIFT)


#define CAS_STATEBITS_MASK    0x000F0000
#define CAS_STATEBITS_SHIFT   16

#define CAS_NEXTBITS_MASK     0x0000FC00
#define CAS_NEXTBITS_SHIFT    10

#define CAS_NEXTSTATE_0       (0    << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_1       (1    << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_2       (2    << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_3       (3    << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_4       (4    << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_5       (5    << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_6       (6    << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_7       (7    << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_8       (8    << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_9       (9    << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_10      (10   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_11      (11   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_12      (12   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_13      (13   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_14      (14   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_15      (15   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_16      (16   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_17      (17   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_18      (18   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_19      (19   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_20      (20   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_21      (21   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_22      (22   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_23      (23   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_24      (24   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_25      (25   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_26      (26   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_27      (27   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_28      (28   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_29      (29   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_30      (30   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_31      (31   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_32      (32   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_33      (33   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_34      (34   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_35      (35   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_36      (36   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_37      (37   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_38      (38   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_39      (39   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_40      (40   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_41      (41   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_42      (42   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_43      (43   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_44      (44   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_45      (45   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_46      (46   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_47      (47   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_48      (48   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_49      (49   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_50      (50   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_51      (51   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_52      (52   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_53      (53   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_54      (54   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_55      (55   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_56      (56   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_57      (57   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_58      (58   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_59      (59   << CAS_NEXTBITS_SHIFT)
#define CAS_NEXTSTATE_60      (60   << CAS_NEXTBITS_SHIFT)
#define CAS_ENDING_STATE      (61   << CAS_NEXTBITS_SHIFT)

#define CAS_LOOP_FLAG         0x80000000

/* CAS FXO and FXS State Definitions */
#define FXOBITS_SERVICEDENIED     (CAS_CTL_FXO_SERVICEDENIED     << CAS_STATEBITS_SHIFT)
#define FXOBITS_IDLE              (CAS_CTL_FXO_IDLE              << CAS_STATEBITS_SHIFT)
#define FXOBITS_ALERTING          (CAS_CTL_FXO_ALERTING          << CAS_STATEBITS_SHIFT)
#define FXOBITS_ADDRESSING        (CAS_CTL_FXO_ADDRESSING        << CAS_STATEBITS_SHIFT)
#define FXOBITS_ACTIVE            (CAS_CTL_FXO_ACTIVE            << CAS_STATEBITS_SHIFT)
#define FXOBITS_JAPAN_ALERTING              (CAS_CTL_FXO_JAPAN_ALERTING              << CAS_STATEBITS_SHIFT)
#define FXOBITS_JAPAN_ALERTING_CLID         (CAS_CTL_FXO_JAPAN_ALERTING_CLID         << CAS_STATEBITS_SHIFT)
#define FXOBITS_JAPAN_ALERTING_CLID_OFFHOOK (CAS_CTL_FXO_JAPAN_ALERTING_CLID_OFFHOOK << CAS_STATEBITS_SHIFT)

#define FXSBITS_RELEASED          (CAS_CTL_FXS_RELEASED          << CAS_STATEBITS_SHIFT)
#define FXSBITS_RING_ON           (CAS_CTL_FXS_RING_ON           << CAS_STATEBITS_SHIFT)
#define FXSBITS_SEIZED            (CAS_CTL_FXS_SEIZED            << CAS_STATEBITS_SHIFT)
#define FXSBITS_CLIDRX            (CAS_CTL_FXS_CLIDRX            << CAS_STATEBITS_SHIFT)

/* Idle sub-state - used from the CAS starup until it figures out the correct state */
#define FXO_STARTUP_IDLE   0x8760

/* Idle wing sub-states */
#define FXO_IDLE_WINK      1

/* Active sub-state */
#define FXO_FLASH_STARTED  1234   /* Flag signalling that a possible flash signal
                                  ** has been detected on the endpoint */

/* CAS local memory structure */
typedef struct
{
   VRG_UINT16     status;              /* enabled/disabled status flags */
   CASCHANNELINFO channelinfo;
   BOS_CRITSECT   critSect;
} CASLOCLMEM;

/*
** CAS Service interface
*/
typedef struct CASSRV
{
   VRG_BOOL       inUse;
   CASCALLBKP     callbackp;
   CASLOCLMEM     localmem;
} CASSRV;

/* ---- Variable Externs ------------------------------------------------- */
/* ---- Function Prototypes ---------------------------------------------- */

void           casBlk( CASSRV *srvp );
CAS_CTL_STATUS casIO ( CASLOCLMEM *localp, CASCMD *cmdp );
#if CAS_CTL_CFG_STAGGER_RING_SUPPORT
void           casStaggerRingClean( VRG_UINT16 idx );
#endif

#ifdef __cplusplus
    }
#endif

#endif  /* CAS_CTL_STATE_MACHINE_H  */
