/**********************************************************************************
** Copyright (c) 2011-2012 Broadcom
**
** This program  is the  proprietary software  of Broadcom  Corporation and/or  its
** licensors, and may only be used, duplicated, modified or distributed pursuant to
** the  terms and  conditions of  a separate,  written license  agreement executed
** between you and Broadcom (an "Authorized  License").  Except as set forth in  an
** Authorized License, Broadcom  grants no license  (express or implied),  right to
** use, or waiver of any kind with respect to the Software, and Broadcom  expressly
** reserves all rights in and to the Software and all intellectual property  rights
** therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE  THIS
** SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE  ALL
** USE OF THE SOFTWARE.
**
** Except as expressly set forth in the Authorized License,
**
** 1.      This  program,  including  its  structure,  sequence  and  organization,
** constitutes  the valuable  trade secrets  of Broadcom,  and you  shall use  all
** reasonable  efforts to  protect the  confidentiality thereof,  and to  use this
** information only  in connection  with your  use of  Broadcom integrated  circuit
** products.
**
** 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS  IS"
** AND  WITH  ALL  FAULTS  AND  BROADCOM  MAKES  NO  PROMISES,  REPRESENTATIONS  OR
** WARRANTIES, EITHER EXPRESS,  IMPLIED, STATUTORY, OR  OTHERWISE, WITH RESPECT  TO
** THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED WARRANTIES OF
** TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A PARTICULAR PURPOSE,  LACK
** OF  VIRUSES,  ACCURACY OR  COMPLETENESS,  QUIET ENJOYMENT,  QUIET  POSSESSION OR
** CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE ENTIRE RISK ARISING OUT OF USE  OR
** PERFORMANCE OF THE SOFTWARE.
**
** 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
** LICENSORS BE  LIABLE FOR  (i) CONSEQUENTIAL,  INCIDENTAL, SPECIAL,  INDIRECT, OR
** EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF  OR IN ANY WAY RELATING TO  YOUR USE
** OF OR INABILITY  TO USE THE  SOFTWARE EVEN IF  BROADCOM HAS BEEN  ADVISED OF THE
** POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS OF THE AMOUNT ACTUALLY
** PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER IS GREATER. THESE LIMITATIONS
** SHALL APPLY  NOTWITHSTANDING ANY  FAILURE OF  ESSENTIAL PURPOSE  OF ANY  LIMITED
** REMEDY.
***********************************************************************************/

/***********************************************************************************
**
**    Description:
**
**      Interface to the digit manipulation engine, including digit map support.
**
***********************************************************************************/
#ifndef __CM_DM__H__INCLUDED__
#define __CM_DM__H__INCLUDED__

/* ---- Include Files ----------------------------------------------------------- */
#include <bosTypes.h>

/* ---- Constants and Types ----------------------------------------------------- */

/* Callback to be registered with the digit map module that is used
** by the digit map to query the value of 'external' variables.
**
** External Variables are variables referenced within the digit map
** package which value is run time configured outside of the digit map
** package.
*/
typedef void ( *CMDMEXTVARCB )( BOS_UINT32 uGuid,
                                const BOS_UINT8  *pVarName,
                                BOS_UINT8  *pVarValue,
                                BOS_UINT32 *puVarValueLen );


#define CMDM_MAP_BUFFER_SIZE            10000
#define CMDM_ACTION_BUFFER_SIZE         200

#define CMDM_INTERDIGIT_TMR_SHORT       'S'
#define CMDM_LONG_DURATION_TMR          'Z'

/* Return codes used by functions in this module */
typedef enum
{
   eCMDM_SUCCESS,
   eCMDM_SYNTAXERROR,
   eCMDM_NOMATCH,
   eCMDM_ERROR
} CMDMSTATUS;

/* Actions supported by the digit map module */
typedef enum
{
   eCMDM_ACACTIVATE,
   eCMDM_ACDEACTIVATE,
   eCMDM_ACRACTIVATE,
   eCMDM_ACRDEACTIVATE,
   eCMDM_ACTIVATEDCHECK,
   eCMDM_ARACTIVATE,
   eCMDM_ARDEACTIVATE,
   eCMDM_CIDDELIVER,
   eCMDM_CIDSUPPRESS,
   eCMDM_CFPROGRAM,
   eCMDM_CFACTIVATE,
   eCMDM_CFDEACTIVATE,
   eCMDM_CBACTIVATEALL,
   eCMDM_CBACTIVATEDM,
   eCMDM_CBDEACTIVATE,
   eCMDM_CNDBTOGGLE,
   eCMDM_COTACTIVATE,
   eCMDM_CWTOGGLE,
   eCMDM_DEACTIVATEDCHECK,
   eCMDM_DNDACTIVATE,
   eCMDM_DNDDEACTIVATE,
   eCMDM_EMERGENCYCALL,
   eCMDM_FEATURECHECK,
   eCMDM_HOLDACTIVATE,
   eCMDM_HOTLINEINT, /* Internal only */
   eCMDM_HOOKFLASHACTIVATE, /* Internal only */
   eCMDM_HOOKFLASHDEACTIVATE, /* Internal only */
   eCMDM_MAKECALL,
   eCMDM_MAKECALLINT,
   eCMDM_RECALL,
   eCMDM_REORDER,
   eCMDM_RETURN, /* Internal only */
   eCMDM_UNKNOWNACTION,
   eCMDM_USEMAP, /* Internal only */
   eCMDM_WARMLINEINT, /* Internal only */
#if CMGRCFG_WARMLINE_DIGITMAP_ACT_SUPPORT
   eCMDM_WARMLINE_DM_ACTIVATE,   /* Activate warmline via diigitmap vsc*/
   eCMDM_WARMLINE_DM_DEACTIVATE, /* Deactivate warmline via digitmap vsc */
#endif
} CMDMACTIONTYPE;

/* The compiled digit map structure */
typedef struct
{
   BOS_UINT8  uType;
   BOS_UINT8  uReserved;
   BOS_UINT16 uLength;
   BOS_UINT8  uValue[1]; /* Flexible array */
} CMDMDIGITMAP;

/* The digit map processing state structure */
typedef struct
{
   BOS_UINT16 uStateBufSz;
   BOS_UINT16 uLength;
   BOS_UINT16 uActionsRetrieved;
   BOS_UINT16 uParametersRetrieved;
   BOS_UINT8  uValue[1]; /* Flexible array */
} CMDMACTIONSTATE;


/* ---- Function Prototypes ----------------------------------------------------- */
#if defined(__cplusplus)
extern "C"
{
#endif

/***********************************************************************************
**  FUNCTION:   cmDmCompile
**
**  PURPOSE:    Compiles the given pcDigitMapText to a state machine that can
**              process dialstrings.
**
**  PARAMETERS:
**              uGuid - Unique identifier of the caller of this function, may be
**                      needed to retrieve external information during processing
**                      of some of the actions.
**              pcDigitMapText - This is the actual text of the digit map that
**                               has been provisioned through the configuration
**                               file. The character buffer is expected to be
**                               terminated with a '\0' character.
**              pDigitMap - Pointer to the buffer where the compiled digit map
**                          structure will be stored
**              uDigitMapSz - Size of the pDigitMap buffer.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the pcDigitMapText does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:
***********************************************************************************/
CMDMSTATUS cmDmCompile(BOS_UINT32 uGuid,
                       const BOS_UINT8 *pcDigitMapText,
                       CMDMDIGITMAP *pDigitMap,
                       BOS_UINT32 uDigitMapSz);

/***********************************************************************************
**  FUNCTION:   cmDmAssignExtVarCb
**
**  PURPOSE:    Assign the 'external variable' callback which is used to
**              query the value of variables referenced within the digit
**              map package but which actual variable value are defined
**              elsewhere.
**
**  PARAMETERS:
**              extVarCb - The callback function.
**
**  RETURNS:    eCMDM_ERROR if there was an error assigning the callback.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      When invoked, the callback shoudl return the information
**              looked up synchronously.
***********************************************************************************/
CMDMSTATUS cmDmAssignExtVarCb( CMDMEXTVARCB extVarCb );

/***********************************************************************************
**  FUNCTION:   cmDmReset
**
**  PURPOSE:    Reset the digit map state to process a new dialstring.
**
**  PARAMETERS:
**              pActionState - Pointer to the state structure to be used to process a
**                       dialstring.
**              uStateSz - Size of the buffer pointed to by pActionState.
**
**  RETURNS:    eCMDM_SUCCESS is returned when the reset has completed.
**              eCMDM_ERROR is returned if there is a problem resetting the digit
**              map state.
**
**  NOTES:
***********************************************************************************/
CMDMSTATUS cmDmReset(CMDMACTIONSTATE *pActionState, BOS_UINT32 uStateSz);

/***********************************************************************************
**  FUNCTION:   cmDmCheckDialString
**
**  PURPOSE:    This function checks the given dialstring against the given
**              compiled digit map. On a successful match the digit map state
**              structure is updated with any actions and associated parameters.
**              Actions and parameters are retrieved using cmDmGetAction(...)
**              and cmDmGetParameter(...).
**
**  PARAMETERS:
**              uGuid - Unique identifier of the caller of this function, may be
**                      needed to retrieve external information during processing
**                      of some of the actions.
**              pDigitMap - Pointer to the compiled digit map returned from a call
**                          to cmDmCompile(...).
**              pcDialString - String of digits collected from user.
**              pActionState - Pointer to the digit map state structure.
**
**  RETURNS:    eCMDM_SUCCESS is returned when a full match is made and a
**              resulting action has been attempted.
**              eCMDM_NOMATCH is returned if there was no match.
**              eCMDM_ERROR is returned if there is a problem processing the
**              compiled digit map or populating the given state structure.
**
**  NOTES:      This function may be called more than once between calls to
**              cmDmReset(...). Calling this function twice with the same
**              dialstring has no effect. Calling this function multiple times as
**              the dialstring is updated may add actions to the digit map state
**              structure as dictated by the digit map.
***********************************************************************************/
CMDMSTATUS cmDmCheckDialString(BOS_UINT32 uGuid,
                               const CMDMDIGITMAP *pDigitMap,
                               const BOS_UINT8 *pcDialString,
                               CMDMACTIONSTATE *pActionState);

/***********************************************************************************
**  FUNCTION:   cmDmGetAction
**
**  PURPOSE:    This function retrieves the next action from the given digit map
**              state structure. Actions are specified by the digit map during a
**              call to cmDmCheckDialString(...).
**
**  PARAMETERS:
**              pActionState - Pointer to the digit map state structure which has
**                             been populated through a call to
**                             cmDmCheckDialString(...).
**              peAction - The retrieved action is passed back to the calling
**                         function using this pointer.
**
**  RETURNS:    eCMDM_SUCCESS is returned when peAction is successfully assigned
**              the next action in the list from the digit map state structure.
**              eCMDM_NOMATCH is returned if there are no more actions to be
**              retrieved at this time.
**              eCMDM_ERROR is returned if there is a problem processing the state
**              structure.
**
**  NOTES:      Once an action is retrieved using this function, the previous
**              action (if any) is removed from the digit map state structure
**              along with associated parameters. Consecutive calls to this
**              function return ordered actions specified by the digit map during
**              previous calls to cmDmCheckDialString(...).
***********************************************************************************/
CMDMSTATUS cmDmGetAction(CMDMACTIONSTATE *pActionState,
                         CMDMACTIONTYPE *peAction);

/***********************************************************************************
**  FUNCTION:   cmDmGetParameter
**
**  PURPOSE:    This function retrieves the next parameter from the given digit
**              map state structure. Parameters are specified by the digit map
**              during a call to cmDmCheckDialString(...).
**
**  PARAMETERS:
**              pActionState - Pointer to the digit map state structure which has
**                             been populated through a call to
**                             cmDmCheckDialString(...).
**              ppcParameter - The retrieved parameter is passed back to the
**                             calling function using this pointer. The pointer
**                             will point to the internal storage location of the
**                             parameter.
**
**  RETURNS:    eCMDM_SUCCESS is returned when ppcParameter is successfully
**              assigned the next parameter in the list from the digit map state
**              structure.
**              eCMDM_NOMATCH is returned if there are no more parameters to be
**              retrieved from this action.
**              eCMDM_ERROR is returned if there is a problem processing the state
**              structure.
**
**  NOTES:      Once a parameter is retrieved using this function, the previous
**              parameter (if any) is removed from the digit map state structure.
**              Consecutive calls to this function return ordered parameters
**              specified by the digit map during previous calls to
**              cmDmCheckDialString(...). Since ppcParameter is set to the
**              internal storage location for the parameter there is no need to
**              allocate or free this buffer. The buffer will be freed
**              automatically on the next call to either cmDmGetAction(...),
**              cmDmGetParameter(...) or cmDmReset(...). The string will end in
**              '\0'.
***********************************************************************************/
CMDMSTATUS cmDmGetParameter(CMDMACTIONSTATE *pActionState,
                            BOS_UINT8 **ppcParameter);

/***********************************************************************************
**  FUNCTION:   cmDmGetTimerValue
**
**  PURPOSE:    Get timer values from the compiled digit map.
**
**  PARAMETERS:
**              pDigitMap - Pointer to the compiled digit map returned from a call
**                          to cmDmCompile(...).
**              cTimer - The identifier of the timer.
**              puMsValue - Returned value of the timer in milliseconds.
**
**  RETURNS:    eCMDM_SUCCESS is returned when the timer definition is found
**              and puMsValue contains the value of the requested timer.
**              eCMDM_NOMATCH is returned if the timer definition could not be
**              found. eCMDM_ERROR is returned if there is a problem processing
**              the compiled digit map.
**
**  NOTES:
**
***********************************************************************************/
CMDMSTATUS cmDmGetTimerValue(const CMDMDIGITMAP *pDigitMap,
                             const BOS_UINT8 cTimer,
                             BOS_UINT32 *puMsValue);

#if defined(__cplusplus)
}
#endif

#endif /* __CM_DM__H__INCLUDED__ */

