/**********************************************************************************
** Copyright (c) 2009-2012 Broadcom Corporation
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
**      This module implements digit manipulation engine, including digit map
**      support.
**
***********************************************************************************/


/* ---- Include Files ----------------------------------------------------------- */
#include <cmInc.h>

/* ---- Public Variables -------------------------------------------------------- */

/* ---- Private Constants and Types --------------------------------------------- */
#define CMDM_MAX_PATTERN_REFS 7 /* Max sub-pattern depth */
#define CMDM_MAP_VALUE_SIZE 128 /* Max map value string */
#define CMDM_COMPILED_PATTERN_SIZE 100 /* Max compiled pattern size */

/* Types used in the TLV encoded compiled digit map */
typedef enum
{
   eCMDM_TLVDIGITMAP         = 0xF0, /* The entire compiled map */
   eCMDM_TLVMAP              = 0xF1, /* A map within the digit map */
   eCMDM_TLVRULE             = 0xF2,
   eCMDM_TLVACTION           = 0xF3,
   eCMDM_TLVPARAMETER        = 0xF4,
   eCMDM_TLVPATTERNREF       = 0xF5,
   eCMDM_TLVPATTERNVALREF    = 0xF6,
   eCMDM_TLVPATTERN          = 0xF7,
   eCMDM_TLVSTRING           = 0xF8,
   eCMDM_TLVREFERENCE        = 0xF9,
   eCMDM_TLVINPUT            = 0xFA,
   eCMDM_TLVCOUNT            = 0xFB,
   eCMDM_TLVTIMER            = 0xFC,
   eCMDM_TLVDEREFPATTERNREF  = 0xFD,
   eCMDM_TLVDEREFACTIONREF   = 0xFE,
   eCMDM_TLVDEREFDEF         = 0xFF,
} CMDMTLVTYPE;

typedef struct
{
   BOS_UINT8  uType;
   BOS_UINT8  uReserved;
   BOS_UINT16 uLength;
   BOS_UINT8  uValue[1]; /* Flexible array */
} CMDMTLVHEADER;

typedef struct
{
   BOS_UINT8      uType;
   BOS_UINT8      uReserved;
   BOS_UINT16     uLength;
   CMDMACTIONTYPE eActionType;
   BOS_UINT8      uValue[1]; /* Flexible array */
} CMDMCOMPILEDACTION;

typedef struct
{
   BOS_UINT8  uType;
   BOS_UINT8  uPatternNum;
   BOS_UINT16 uLength;
   BOS_UINT8  uValue[1]; /* Flexible array */
} CMDMCOMPILEDPATTERNREF;

typedef struct
{
   BOS_UINT8  uType;
   BOS_UINT8  cTimer;
   BOS_UINT16 uDigits;
   BOS_UINT8  uValue[1]; /* Flexible array */
} CMDMCOMPILEDINPUT;

typedef struct
{
   BOS_UINT8  uType;
   BOS_UINT8  uReserved;
   BOS_UINT8  uMin;
   BOS_UINT8  uMax;
   BOS_UINT8  uValue[1]; /* Flexible array */
} CMDMCOMPILEDCOUNT;

typedef struct
{
   BOS_UINT8  uType;
   BOS_UINT8  uReserved;
   BOS_UINT16 uOffset;
   BOS_UINT8  uValue[1]; /* Flexible array */
} CMDMCOMPILEDREFERENCE;

typedef struct
{
   BOS_UINT8  cTimer;
   BOS_UINT8  uReserved;
   BOS_UINT16 uMSec;
   BOS_UINT8  uValue[1]; /* Flexible array */
} CMDMCOMPILEDTIMER;

typedef struct
{
   BOS_UINT32       uGuid;
   const BOS_UINT8  *pcDigitMapText;
   const BOS_UINT8 **ppcDigitMapTextProgress;
   CMDMDIGITMAP     *pDigitMap;
   CMDMTLVHEADER    *pCurrentEntry;
   CMDMTLVHEADER    *pDerefTableStart;
   void             *pDerefTableEnd;

   /* pDigitMap is divided into three sections during compilation:
    * compiled map, temporary storage and the unused portion. The buffer layout
    * is as follows:
    *
    * +---+ <- 0
    * | C |
    * | O |
    * | M |
    * | P |
    * | I |
    * | L |
    * | E |
    * | D |
    * +---+ <- pCurrentEntry
    * | U |
    * | N |
    * | U |
    * | S |
    * | E |
    * | D |
    * +---+ <- pDerefTableStart
    * | T |
    * | E |
    * | M |
    * | P |
    * | O |
    * | R |
    * | A |
    * | R |
    * | Y |
    * +---+ <- pDerefTableEnd
    *
    * The pCurrentEntry and pDerefTableStart pointers grow towards each other.
    * If they ever reach one another (meaning there is no unused space left)
    * then there is not enough space in the buffer to compile the digit map.
    */
} CMDMCOMPILERSTATE;

typedef struct
{
   BOS_UINT32 uOffset;
   BOS_UINT8  cIdent[1]; /* Flexible array */
} CMDMDEREFENTRY;

typedef struct
{
   BOS_UINT32 uNumRefs;
   BOS_UINT16 uReferences[1]; /* Flexible array */
} CMDMDEREFREFS;

typedef enum
{
   eCMDM_PATTERNREF,
   eCMDM_ACTIONREF
} CMDMREFTYPE;

typedef struct
{
   BOS_UINT32 uStart;
   BOS_UINT32 uEnd;
} CMDMPATTERNREFERENCE;

typedef struct
{
   /* Global identifier */
   BOS_UINT32             uGuid;

   /* Compiled map */
   const CMDMDIGITMAP     *pDigitMap;
   const CMDMTLVHEADER    *pCurrentEntry;

   /* Action storage */
   CMDMACTIONSTATE        *pActionState;

   /* Dialstring */
   const BOS_UINT8        *pcDialStr;
   BOS_UINT32              uDialStrIdx;

   /* Value string */
   BOS_UINT8              *pcValueStr;
   BOS_UINT32              uValueStrIdx;

   /* Pattern references */
   CMDMPATTERNREFERENCE   *pPatternRefs;
   CMDMPATTERNREFERENCE   *pPatternValRefs;
   BOS_UINT32              uPatternRefsIdx;
} CMDMPROCESSORSTATE;

/* ---- Private Variables ------------------------------------------------------- */
static CMMAPID keyMap[] =
{
   { '0',                  0x1 << 0x0 },
   { '1',                  0x1 << 0x1 },
   { '2',                  0x1 << 0x2 },
   { '3',                  0x1 << 0x3 },
   { '4',                  0x1 << 0x4 },
   { '5',                  0x1 << 0x5 },
   { '6',                  0x1 << 0x6 },
   { '7',                  0x1 << 0x7 },
   { '8',                  0x1 << 0x8 },
   { '9',                  0x1 << 0x9 },
   { 'A',                  0x1 << 0xA },
   { 'B',                  0x1 << 0xB },
   { 'C',                  0x1 << 0xC },
   { 'D',                  0x1 << 0xD },
   { '*',                  0x1 << 0xE },
   { '#',                  0x1 << 0xF },
   { CMGR_INVALID_HANDLE,  CMGR_INVALID_HANDLE }
};

/* Callback for external variable value retrieval.
*/
CMDMEXTVARCB gCmDmExtVarCb = NULL;

/* ---- Private Function Prototypes --------------------------------------------- */
static CMDMSTATUS digitMapPackage(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS mapDef(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS rule(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS pattern(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS colon(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS action(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS verb(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS parameter(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS stringParam(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS paramPiece(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS patternBody(CMDMCOMPILERSTATE *pCompilerState,
                              BOS_BOOL bAllowReferences);
static CMDMSTATUS target(CMDMCOMPILERSTATE *pCompilerState,
                         BOS_BOOL bAllowReferences);
static CMDMSTATUS count(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS key(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS subPattern(CMDMCOMPILERSTATE *pCompilerState,
                             BOS_BOOL bAllowReferences);
static CMDMSTATUS comma(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS semi(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS reference(CMDMCOMPILERSTATE *pCompilerState,
                            CMDMREFTYPE eType);
static CMDMSTATUS nameDef(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS timerDef(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS symbolDef(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS constant(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS equal(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS swsc(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS lwsc(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS identifier(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS lineBreak(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS comment(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS nonAmpersand(CMDMCOMPILERSTATE *pCompilerState);
static BOS_UINT32 stringPad(BOS_UINT32 uStrLen);
static CMDMSTATUS linkStage(CMDMCOMPILERSTATE *pCompilerState);
static CMDMSTATUS matchMap(CMDMPROCESSORSTATE *pProcState);
static CMDMSTATUS matchRule(CMDMPROCESSORSTATE *pProcState);
static CMDMSTATUS processAction(CMDMPROCESSORSTATE *pProcState,
                                BOS_UINT8 *pcMapValue,
                                BOS_UINT32 uMapValueSz);
static CMDMSTATUS processGeneralAction(CMDMPROCESSORSTATE *pProcState);
static CMDMSTATUS processUsemapAction(CMDMPROCESSORSTATE *pProcState);
static CMDMSTATUS processReturnAction(CMDMPROCESSORSTATE *pProcState,
                                      BOS_UINT8 *pcMapValue,
                                      BOS_UINT32 uMapValueSz);
static CMDMSTATUS getStringParameter(CMDMPROCESSORSTATE *pProcState,
                                     BOS_UINT8 *pcStrParam,
                                     BOS_UINT32 uStrParamSz);
static CMDMSTATUS matchPattern(CMDMPROCESSORSTATE *pProcState);
static CMDMSTATUS matchTarget(CMDMPROCESSORSTATE *pProcState);
static CMDMSTATUS matchTargetInputState(CMDMPROCESSORSTATE *pProcState);
static CMDMSTATUS matchTargetReferenceState(CMDMPROCESSORSTATE *pProcState);
static CMDMTLVHEADER *getNextTarget(const CMDMTLVHEADER *pCurrentTarget);
static CMDMTLVHEADER *getNextElement(const CMDMTLVHEADER *pElement);
static CMDMSTATUS getNextAction(CMDMACTIONSTATE *pActionState,
                                CMDMACTIONTYPE *peAction);

/* ---- Functions --------------------------------------------------------------- */
#define CMDM_MIN(a,b)     (((a) < (b)) ? (a) : (b))


/***********************************************************************************
**  FUNCTION:   cmDmCompile
**
**  PURPOSE:    Compiles the given pcDigitMapText to a state machine that can
**              process dialstrings.
**
**  PARAMETERS:
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
                       BOS_UINT32 uDigitMapSz)
{
   CMDMCOMPILERSTATE pCompilerState;
   const BOS_UINT8  *pcDigitMapTextProgress = pcDigitMapText;
   CMDMSTATUS        eStatus;

   /* Sanity check inputs */
   if( (pcDigitMapText == NULL) || (pDigitMap == NULL) )
   {
      CMGRLOG_WARNING(("cmDmCompile: Digit map is NULL."));
      return eCMDM_ERROR;
   }

   /* Make sure the compiled map buffer isn't too big for our implementation */
   if( uDigitMapSz > (BOS_UINT16)(-1) )
   {
      CMGRLOG_WARNING(("cmDmCompile: Buffer is too big."));
      return eCMDM_ERROR;
   }

   /* The pDigitMap buffer must start on a word boundry */
   if(((BOS_UINT32)pDigitMap & (sizeof(BOS_UINT32)-1)) != 0)
   {
      CMGRLOG_WARNING(("cmDmCompile: Buffer must start on a word boundry."));
      return eCMDM_ERROR;
   }

   /* Set up the compiler state structure */
   pCompilerState.uGuid = uGuid;
   pCompilerState.pcDigitMapText = pcDigitMapText;
   pCompilerState.ppcDigitMapTextProgress = &pcDigitMapTextProgress;
   pCompilerState.pDigitMap = pDigitMap;
   pCompilerState.pCurrentEntry = (CMDMTLVHEADER *)pDigitMap;
   /* Make sure the pDerefTableEnd pointer is word aligned */
   pCompilerState.pDerefTableEnd = (void *)((BOS_UINT32)pDigitMap +
                                       (uDigitMapSz & ~(sizeof(BOS_UINT32)-1)));

   /* The dereferencing table is for identifier definitions and references. */
   pCompilerState.pDerefTableStart =
      (CMDMTLVHEADER *)pCompilerState.pDerefTableEnd;

   /* Compile the input pcDigitMapText to a set of state machines */
   eStatus = digitMapPackage(&pCompilerState);

   /* If the digit map contains a syntax error, inform the user of its
    * location. */
   if( eStatus == eCMDM_SYNTAXERROR )
   {
      BOS_UINT32 uLine = 1;
      const BOS_UINT8 *pcText;

      for( pcText = pcDigitMapText;
           (*pcText != '\0') && (pcText < pcDigitMapTextProgress);
           pcText++ )
      {
         if( (pcText[0] == '\r') && (pcText[1] == '\n') )
         {
            uLine++;
         }
      }

      CMGRLOG_WARNING(("cmDmCompile: Syntax error at line %u", (unsigned int)uLine));
   }
   else if( eStatus == eCMDM_ERROR )
   {
      CMGRLOG_WARNING(("cmDmCompile: Not enough space to compile"));
   }
   else
   {
      /* If the digit map did not contain any errors then we need to finish
       * with a linking phase to link references to definitions. */
      if( (eStatus = linkStage(&pCompilerState)) != eCMDM_SUCCESS )
      {
         CMGRLOG_WARNING(("cmDmCompile: Could not link digit map"));
      }
      else
      {
         CMGRLOG_DEBUG(("cmDmCompile: Compiled digit map uses %u bytes",
                        (pDigitMap->uLength + offsetof(CMDMDIGITMAP, uValue))));
      }
   }

   CMGRLOG_INFORMATION (( "cmDmCompile (0x%X): \'%u\'",
                          uGuid, eStatus ));

   return eStatus;
}

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
CMDMSTATUS cmDmAssignExtVarCb( CMDMEXTVARCB extVarCb )
{
   CMDMSTATUS eStatus = eCMDM_SUCCESS;

   if ( extVarCb == NULL )
   {
      eStatus = eCMDM_ERROR;
   }

   gCmDmExtVarCb = extVarCb;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   digitMapPackage
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a DigitMapPackage as defined in Annex A of the PKT-SP-RSTF
**              Specification. Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS digitMapPackage(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus = eCMDM_ERROR;

   /* DigitMapPackage = *(TimerDef / SymbolDef / MapDef / LineBreak) */
   if( ((BOS_UINT32)localState.pCurrentEntry->uValue <=
        (BOS_UINT32)localState.pDerefTableStart) &&
       ((BOS_UINT32)localState.pCurrentEntry ==
        (BOS_UINT32)localState.pDigitMap) )
   {
      eStatus = eCMDM_SUCCESS;

      /* Fill in the digit map header */
      localState.pDigitMap->uType = eCMDM_TLVDIGITMAP; /* uType */
      localState.pDigitMap->uReserved = 0;
      localState.pCurrentEntry = (CMDMTLVHEADER *)localState.pDigitMap->uValue;

      while( (localState.pcDigitMapText[0] != '\0')
             && (eStatus == eCMDM_SUCCESS) &&
             ((BOS_UINT32)localState.pCurrentEntry <=
              (BOS_UINT32)localState.pDerefTableStart) )
      {
         eStatus = mapDef(&localState);

         if( eStatus == eCMDM_SYNTAXERROR )
         {
            eStatus = timerDef(&localState);
         }

         if( eStatus == eCMDM_SYNTAXERROR )
         {
            eStatus = symbolDef(&localState);
         }

         if( eStatus == eCMDM_SYNTAXERROR )
         {
            eStatus = lineBreak(&localState);
         }
      }

      if( eStatus == eCMDM_SUCCESS )
      {
         /* Length without the type and length fields */
         pCompilerState->pCurrentEntry->uLength =
            (BOS_UINT16)( (BOS_UINT32)localState.pCurrentEntry -
                          (BOS_UINT32)pCompilerState->pCurrentEntry->uValue );
         *pCompilerState = localState;
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   mapDef
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a MapDef as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS mapDef(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* MapDef = "Map" 1*WSP MapNameDef SWSC "=" 1*( LWSC Rule LineBreak ) */
   if( strCmpNoCaseSize( (const char *)localState.pcDigitMapText,
                         "Map", strlen("Map")) == 0 )
   {
      BOS_UINT32 uLoopCounter;

      localState.pcDigitMapText += strlen("Map");

      for( uLoopCounter = 0;
           (localState.pcDigitMapText[0] == ' ') ||
           (localState.pcDigitMapText[0] == '\t');
           localState.pcDigitMapText++,uLoopCounter++ );

      if(uLoopCounter >= 1)
      {
         if( ((eStatus = nameDef(&localState)) == eCMDM_SUCCESS) )
         {
            if( ((eStatus = swsc(&localState)) == eCMDM_SUCCESS) &&
                (localState.pcDigitMapText[0] == '=') )
            {
               localState.pcDigitMapText++;

               if( (BOS_UINT32)localState.pCurrentEntry->uValue <=
                   (BOS_UINT32)localState.pDerefTableStart )
               {
                  CMDMCOMPILERSTATE subState;

                  localState.pCurrentEntry->uType = eCMDM_TLVMAP; /* type */
                  localState.pCurrentEntry->uReserved = 0;
                  localState.pCurrentEntry =
                     (CMDMTLVHEADER *)localState.pCurrentEntry->uValue;

                  for( uLoopCounter = 0, subState = localState;
                       (eStatus == eCMDM_SUCCESS); )
                  {
                     if( (eStatus = lwsc(&subState)) == eCMDM_SUCCESS )
                     {
                        if( (eStatus = rule(&subState)) == eCMDM_SUCCESS)
                        {
                           if( (eStatus = lineBreak(&subState)) == eCMDM_SUCCESS)
                           {
                              localState = subState;
                              uLoopCounter++;
                           }
                        }
                     }
                  }

                  if( (eStatus == eCMDM_SYNTAXERROR) && (uLoopCounter >= 1) )
                  {
                     /* Length without the type and length fields */
                     pCompilerState->pCurrentEntry->uLength =
                        (BOS_UINT16) ( (BOS_UINT32)localState.pCurrentEntry -
                               (BOS_UINT32)pCompilerState->pCurrentEntry->uValue );
                     *pCompilerState = localState;
                     eStatus = eCMDM_SUCCESS;
                  }
               }
               else
               {
                  eStatus = eCMDM_ERROR;
               }
            }
            else
            {
               eStatus = eCMDM_SYNTAXERROR;
            }
         }
      }
      else
      {
         eStatus = eCMDM_SYNTAXERROR;
      }
   }
   else
   {
      eStatus = eCMDM_SYNTAXERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   rule
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a Rule as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS rule(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   if( (BOS_UINT32)localState.pCurrentEntry->uValue <=
       (BOS_UINT32)localState.pDerefTableStart )
   {
      localState.pCurrentEntry->uType = eCMDM_TLVRULE; /* type */
      localState.pCurrentEntry->uReserved = 0;
      localState.pCurrentEntry =
         (CMDMTLVHEADER *)localState.pCurrentEntry->uValue;

      /* Rule = Pattern COLON Action *( SEMI Action ) */
      if( (eStatus = pattern(&localState)) == eCMDM_SUCCESS )
      {
         if( (eStatus = colon(&localState)) == eCMDM_SUCCESS )
         {
            if( (eStatus = action(&localState)) == eCMDM_SUCCESS )
            {
               CMDMCOMPILERSTATE subState;

               for( subState = localState;
                    ( ((eStatus = semi(&subState)) == eCMDM_SUCCESS) &&
                      ((eStatus = action(&subState)) == eCMDM_SUCCESS) );
                    localState = subState );

               if( eStatus != eCMDM_ERROR )
               {
                  /* Length without the type and length fields */
                  pCompilerState->pCurrentEntry->uLength =
                     (BOS_UINT16) ( (BOS_UINT32)localState.pCurrentEntry -
                           (BOS_UINT32)pCompilerState->pCurrentEntry->uValue );
                  *pCompilerState = localState;
                  eStatus = eCMDM_SUCCESS;
               }
            }
         }
      }
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   pattern
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a pattern as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS pattern(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* Pattern = DQUOTE PatternBody DQUOTE */
   if( localState.pcDigitMapText[0] == '"' )
   {
      /* Make sure there is enough room for the TLV header */
      if( (BOS_UINT32)localState.pCurrentEntry->uValue <=
          (BOS_UINT32)localState.pDerefTableStart )
      {
         localState.pcDigitMapText++;

         localState.pCurrentEntry->uType = eCMDM_TLVPATTERN; /* type */
         localState.pCurrentEntry->uReserved = 0;
         localState.pCurrentEntry =
            (CMDMTLVHEADER *)localState.pCurrentEntry->uValue;

         if( (eStatus = patternBody(&localState, BOS_TRUE)) == eCMDM_SUCCESS )
         {
               if( localState.pcDigitMapText[0] == '"' )
               {
                  localState.pcDigitMapText++;
                  /* Length without the type and length fields */
                  pCompilerState->pCurrentEntry->uLength =
                     (BOS_UINT16) ( (BOS_UINT32)localState.pCurrentEntry -
                           (BOS_UINT32)pCompilerState->pCurrentEntry->uValue );
                  *pCompilerState = localState;
               }
               else
               {
                  eStatus = eCMDM_SYNTAXERROR;
               }
         }
      }
      else
      {
         eStatus = eCMDM_ERROR;
      }
   }
   else
   {
      eStatus = eCMDM_SYNTAXERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   colon
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a COLON as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS colon(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* COLON = SWSC ":" SWSC */
   eStatus = swsc(&localState);

   if( (eStatus == eCMDM_SUCCESS) && (localState.pcDigitMapText[0] == ':') )
   {
      localState.pcDigitMapText++;
      eStatus = swsc(&localState);
   }
   else
   {
      eStatus = eCMDM_SYNTAXERROR;
   }

   if( eStatus == eCMDM_SUCCESS )
   {
      *pCompilerState = localState;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   action
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a Action as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS action(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus = eCMDM_ERROR;

   /* Action = Verb [SWSC "(" SWSC Parameter *( COMMA Parameter ) SWSC ")"] */
   /* Make sure there is enough room for the TLV header */
   if( (BOS_UINT32)localState.pCurrentEntry->uValue <=
       (BOS_UINT32)localState.pDerefTableStart )
   {
      localState.pCurrentEntry->uType = eCMDM_TLVACTION; /* type */
      localState.pCurrentEntry->uReserved = 0;

      if( (eStatus = verb(&localState)) == eCMDM_SUCCESS )
      {
         CMDMCOMPILERSTATE subState = localState;

         if( (swsc(&subState) == eCMDM_SUCCESS) &&
             (subState.pcDigitMapText[0] == '(') )
         {
            localState = subState;
            localState.pcDigitMapText++;

            if( ((eStatus = swsc(&localState)) == eCMDM_SUCCESS) &&
                ((eStatus = parameter(&localState)) == eCMDM_SUCCESS) )
            {
               /* Possibly more than one parameter */
               for( subState = localState;
                    ((eStatus = comma(&subState)) == eCMDM_SUCCESS) &&
                    ((eStatus = parameter(&subState)) == eCMDM_SUCCESS);
                    localState = subState );

               if( (eStatus == eCMDM_SYNTAXERROR) &&
                   ((eStatus = swsc(&localState)) == eCMDM_SUCCESS) )
               {
                  if( localState.pcDigitMapText[0] == ')' )
                  {
                     /* Successful with parameters */
                     localState.pcDigitMapText++;
                     /* Length without the type and length fields */
                     pCompilerState->pCurrentEntry->uLength =
                        (BOS_UINT16) ( (BOS_UINT32)localState.pCurrentEntry -
                              (BOS_UINT32)pCompilerState->pCurrentEntry->uValue );
                     *pCompilerState = localState;
                  }
                  else
                  {
                     eStatus = eCMDM_SYNTAXERROR;
                  }
               }
            }
         }
         else
         {
            /* Successful without parameters */
            /* Length without the type and length fields */
            pCompilerState->pCurrentEntry->uLength =
               (BOS_UINT16) ( (BOS_UINT32)localState.pCurrentEntry -
                     (BOS_UINT32)pCompilerState->pCurrentEntry->uValue );
            *pCompilerState = localState;
         }
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   verb
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a Verb as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS verb(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMCOMPILEDACTION *pCompiledAction =
      (CMDMCOMPILEDACTION *)localState.pCurrentEntry;
   CMDMSTATUS eStatus = eCMDM_SYNTAXERROR;
   BOS_UINT32 i;

   /* Verb = "RETURN" / "USEMAP" / "ACR-ACTIVATE" / "ACR-DEACTIVATE" /
   ** "AC-ACTIVATE" / "AC-DEACTIVATE" / "MAKE-CALL" / "AR-ACTIVATE" /
   ** "AR-DEACTIVATE" / "CID-SUPPRESS" / "CID-DELIVER" / "COT-ACTIVATE" /
   ** "CW-TOGGLE" / "HOLD-ACTIVATE" / "EMERGENCY-CALL" / "REORDER" / "RECALL" /
   ** "FEATURE-CHECK" / "ACTIVATED-CHECK" / "DEACTIVATED-CHECK" / Identifier */
   /* Search for the action ID */
   for( i = 0 ; (cmMapDmAction[i].uId != CMGR_INVALID_HANDLE) &&
        (strCmpNoCaseSize((const char *)localState.pcDigitMapText,
                          (const char *)cmMapDmAction[i].pMap,
                          strlen( (const char *)cmMapDmAction[i].pMap)) != 0) ;
        i++ );

   if( cmMapDmAction[i].uId != CMGR_INVALID_HANDLE )
   {
      /* Make sure there is enough room for the action type */
      if( (BOS_UINT32)pCompiledAction->uValue <=
          (BOS_UINT32)localState.pDerefTableStart )
      {
         localState.pcDigitMapText += strlen((const char *)cmMapDmAction[i].pMap);
         pCompiledAction->eActionType = cmMapDmAction[i].uId;
         localState.pCurrentEntry = (CMDMTLVHEADER *)pCompiledAction->uValue;
         *pCompilerState = localState;
         eStatus = eCMDM_SUCCESS;
      }
      else
      {
         eStatus = eCMDM_ERROR;
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   parameter
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a Parameter as defined in Annex A of the PKT-SP-RSTF
**              Specification. Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS parameter(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* Parameter = StringParam / MapParam
   **
   ** At this point in the compilation we don't know and don't care whether a
   ** reference is a symbol reference or a map reference because they look the
   ** same and don't know how the identifiers have been defined. We figure this
   ** out when we process a dialstring and dereference the reference. That being
   ** so we just process all parameters as string parameters.
   **
   ** Make sure there is enough room for the TLV header
   */
   if( (BOS_UINT32)localState.pCurrentEntry->uValue <=
       (BOS_UINT32)localState.pDerefTableStart )
   {
      localState.pCurrentEntry->uType = eCMDM_TLVPARAMETER; /* type */
      localState.pCurrentEntry->uReserved = 0;
      localState.pCurrentEntry =
         (CMDMTLVHEADER *)localState.pCurrentEntry->uValue;

      if( (eStatus = stringParam(&localState)) == eCMDM_SUCCESS )
      {
         /* Length without the type and length fields */
         pCompilerState->pCurrentEntry->uLength =
            (BOS_UINT16) ( (BOS_UINT32)localState.pCurrentEntry -
                           (BOS_UINT32)pCompilerState->pCurrentEntry->uValue );
         *pCompilerState = localState;
      }
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   stringParam
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a stringParam as defined in Annex A of the PKT-SP-RSTF
**              Specification. Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS stringParam(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* StringParam = ParamPiece *( [SWSC] ParamPiece ) */
   if( (eStatus = paramPiece(&localState)) == eCMDM_SUCCESS )
   {
      CMDMCOMPILERSTATE subState;

      for( subState = localState;
           ((eStatus = swsc(&subState)) == eCMDM_SUCCESS) &&
           ((eStatus = paramPiece(&subState)) == eCMDM_SUCCESS);
           localState = subState );

      /* Since this is a repetition, a syntax error just means we reached the
       * end of the repeating pattern */
      if( eStatus == eCMDM_SYNTAXERROR )
      {
         eStatus = eCMDM_SUCCESS;
         *pCompilerState = localState;
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   paramPiece
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a ParamPiece as defined in Annex A of the PKT-SP-RSTF
**              Specification.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS paramPiece(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus = eCMDM_ERROR;

   /* Make sure there is enough room for the TLV header */
   if( (BOS_UINT32)localState.pCurrentEntry->uValue <=
       (BOS_UINT32)localState.pDerefTableStart )
   {
      /* ParamPiece = Constant / "=" SymbolNameRef / "#" Number */
      if( localState.pcDigitMapText[0] == '=' )
      {
         localState.pcDigitMapText++;

         if( (eStatus = reference(&localState, eCMDM_ACTIONREF)) ==
             eCMDM_SUCCESS )
         {
            *pCompilerState = localState;
         }
      }
      else if( localState.pcDigitMapText[0] == '#' )
      {
         CMDMCOMPILEDPATTERNREF *pCompiledPatternRef =
            (CMDMCOMPILEDPATTERNREF *)pCompilerState->pCurrentEntry;
         BOS_UINT32 uLoopCounter;
         BOS_UINT8 uRefNumber = 0;

         localState.pcDigitMapText++;

         for( uLoopCounter = 0; isdigit(localState.pcDigitMapText[0]);
              uLoopCounter++, localState.pcDigitMapText++ )
         {
            uRefNumber = (uRefNumber * 10) +
                         (localState.pcDigitMapText[0] - '0');
         }

         if(uLoopCounter >= 1)
         {
            /* Check whether this is a "value" pattern reference */
            if( toupper(localState.pcDigitMapText[0]) == 'V' )
            {
               pCompiledPatternRef->uType = eCMDM_TLVPATTERNVALREF; /* type */
               localState.pcDigitMapText++;
            }
            else
            {
               pCompiledPatternRef->uType = eCMDM_TLVPATTERNREF; /* type */
            }

            pCompiledPatternRef->uLength = 0; /* length */

            pCompiledPatternRef->uPatternNum = uRefNumber;
            localState.pCurrentEntry =
               (CMDMTLVHEADER *)pCompiledPatternRef->uValue;
            *pCompilerState = localState;
            eStatus = eCMDM_SUCCESS;
         }
         else
         {
            eStatus = eCMDM_SYNTAXERROR;
         }
      }
      else
      {
         localState.pCurrentEntry =
            (CMDMTLVHEADER *)localState.pCurrentEntry->uValue;

         if( (eStatus = constant(&localState)) == eCMDM_SUCCESS )
         {
            BOS_UINT32 uStringLen = (BOS_UINT32)localState.pCurrentEntry -
               (BOS_UINT32)pCompilerState->pCurrentEntry->uValue;

            /* Make sure there's room to add padding */
            if( ((BOS_UINT32)localState.pCurrentEntry + stringPad(uStringLen))
                <= (BOS_UINT32)localState.pDerefTableStart )
            {
               /* Add padding */
               memset(&pCompilerState->pCurrentEntry->uValue[uStringLen], '\0',
                      stringPad(uStringLen));
               localState.pCurrentEntry = (CMDMTLVHEADER *)
                  ((BOS_UINT32)localState.pCurrentEntry +
                   stringPad(uStringLen));

               pCompilerState->pCurrentEntry->uType = eCMDM_TLVSTRING;
               pCompilerState->pCurrentEntry->uReserved = 0;
               pCompilerState->pCurrentEntry->uLength =
                  (BOS_UINT16) ( uStringLen + stringPad(uStringLen) );

               *pCompilerState = localState;
            }
            else
            {
               eStatus = eCMDM_ERROR;
            }
         }
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   comma
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a COMMA as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS comma(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* COMMA = SWSC "," SWSC */
   if( (eStatus = swsc(&localState)) == eCMDM_SUCCESS )
   {
      if( localState.pcDigitMapText[0] == ',' )
      {
         localState.pcDigitMapText++;

         if( (eStatus = swsc(&localState)) == eCMDM_SUCCESS )
         {
            *pCompilerState = localState;
         }
      }
      else
      {
         eStatus = eCMDM_SYNTAXERROR;
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   semi
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a SEMI as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS semi(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* SEMI = SWSC ";" SWSC */
   if( (eStatus = swsc(&localState)) == eCMDM_SUCCESS )
   {
      if( localState.pcDigitMapText[0] == ';' )
      {
         localState.pcDigitMapText++;

         if( (eStatus = swsc(&localState)) == eCMDM_SUCCESS )
         {
            *pCompilerState = localState;
         }
      }
      else
      {
         eStatus = eCMDM_SYNTAXERROR;
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   patternBody
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a PatternBody as defined in Annex A of the PKT-SP-RSTF
**              Specification. Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**              bAllowReferences - Boolean to indicate whether references should
**                                 be allowed in this pPattern body. References
**                                 are not allowed when compiling symbols in the
**                                 link stage.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS patternBody(CMDMCOMPILERSTATE *pCompilerState,
                              BOS_BOOL bAllowReferences)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus = eCMDM_SUCCESS;
   BOS_UINT32 uLoopCounter;

   /* PatternBody = 1*( Target [ Count ] ) */
   for( uLoopCounter = 0; (eStatus == eCMDM_SUCCESS); )
   {
      CMDMCOMPILERSTATE subState = localState;

      if( (eStatus = target(&subState, bAllowReferences)) == eCMDM_SUCCESS )
      {
         if( count(&subState) == eCMDM_ERROR )
         {
            eStatus = eCMDM_ERROR;
         }
         else
         {
            localState = subState;
            uLoopCounter++;
         }
      }
   }

   if( (uLoopCounter >= 1) && (eStatus != eCMDM_ERROR) )
   {
      *pCompilerState = localState;
      eStatus = eCMDM_SUCCESS;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   target
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a Target as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**              bAllowReferences - Boolean to indicate whether references should
**                                 be allowed in this pattern body. References
**                                 are not allowed when compiling symbols in the
**                                 link stage.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS target(CMDMCOMPILERSTATE *pCompilerState,
                          BOS_BOOL bAllowReferences)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* Target = [PrefixTimer] Key / StandaloneTimer / SubPattern / Noise */
   if( toupper(localState.pcDigitMapText[0]) == 'Z' )
   {
      CMDMCOMPILEDINPUT *pCompiledTarget =
         (CMDMCOMPILEDINPUT *)localState.pCurrentEntry;

      /* Make sure there is enough room for a timer state */
      if( (BOS_UINT32)pCompiledTarget->uValue <=
          (BOS_UINT32)localState.pDerefTableStart )
      {
         /* Add prefix timer state */
         pCompiledTarget->uType = eCMDM_TLVINPUT; /* type */
         pCompiledTarget->cTimer = (BOS_UINT8) toupper(localState.pcDigitMapText[0]);
         pCompiledTarget->uDigits = 0;
         localState.pCurrentEntry = (CMDMTLVHEADER *)pCompiledTarget->uValue;

         localState.pcDigitMapText++;

         if( (eStatus = key(&localState)) == eCMDM_SUCCESS )
         {
            *pCompilerState = localState;
         }
      }
      else
      {
         eStatus = eCMDM_ERROR;
      }
   }
   else if( (eStatus = key(&localState)) == eCMDM_SUCCESS )
   {
      *pCompilerState = localState;
   }
   else if( (eStatus == eCMDM_SYNTAXERROR) &&
            (toupper(localState.pcDigitMapText[0]) == 'S') )
   {
      CMDMCOMPILEDINPUT *pCompiledTarget =
         (CMDMCOMPILEDINPUT *)localState.pCurrentEntry;

      /* Make sure there is enough room for a timer state */
      if( (BOS_UINT32)pCompiledTarget->uValue <=
          (BOS_UINT32)localState.pDerefTableStart )
      {
         /* Add timer state */
         pCompiledTarget->uType = eCMDM_TLVINPUT; /* type */
         pCompiledTarget->cTimer = (BOS_UINT8) toupper(localState.pcDigitMapText[0]);
         pCompiledTarget->uDigits = 0;
         localState.pCurrentEntry = (CMDMTLVHEADER *)pCompiledTarget->uValue;

         localState.pcDigitMapText++;

         *pCompilerState = localState;
         eStatus = eCMDM_SUCCESS;
      }
      else
      {
         eStatus = eCMDM_ERROR;
      }
   }
   else if( (eStatus == eCMDM_SYNTAXERROR) &&
            ((eStatus = subPattern(&localState, bAllowReferences)) ==
             eCMDM_SUCCESS) )
   {
      *pCompilerState = localState;
   }
   else if( (eStatus == eCMDM_SYNTAXERROR) &&
            ((*localState.pcDigitMapText == '-') ||
             (*localState.pcDigitMapText == '.') ||
             (*localState.pcDigitMapText == ' ') ||
             (*localState.pcDigitMapText == '\t')) )
   {
      localState.pcDigitMapText++;
      *pCompilerState = localState;
      eStatus = eCMDM_SUCCESS;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   count
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a Count as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS count(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus = eCMDM_SUCCESS;
   BOS_UINT8 uMinRepeat = 0;
   BOS_UINT8 uMaxRepeat = 0;

   /* Count = "{" [Number] ["-"] Number "}" */
   if( localState.pcDigitMapText[0] == '{' )
   {
      localState.pcDigitMapText++;

      if( isdigit(localState.pcDigitMapText[0]) )
      {
         for( ; isdigit(localState.pcDigitMapText[0]);
                localState.pcDigitMapText++ )
         {
            uMinRepeat = (uMinRepeat * 10) +
                         (localState.pcDigitMapText[0] - '0');
         }

         if( localState.pcDigitMapText[0] == '-' )
         {
            BOS_UINT32 uLoopCounter;

            localState.pcDigitMapText++;

            for( uLoopCounter = 0; isdigit(localState.pcDigitMapText[0]);
                 localState.pcDigitMapText++, uLoopCounter++ )
            {
               uMaxRepeat = (uMaxRepeat * 10) +
                            (localState.pcDigitMapText[0] - '0');
            }

            if( (uLoopCounter < 1) || (uMaxRepeat < uMinRepeat) )
            {
               eStatus = eCMDM_SYNTAXERROR;
            }
         }
         else
         {
            uMaxRepeat = uMinRepeat; /* Not a range */
         }
      }
      else if( localState.pcDigitMapText[0] == '-' )
      {
         BOS_UINT32 uLoopCounter;

         localState.pcDigitMapText++;

         for( uLoopCounter = 0; isdigit(localState.pcDigitMapText[0]);
              localState.pcDigitMapText++, uLoopCounter++ )
         {
            uMaxRepeat = (uMaxRepeat * 10) +
                         (localState.pcDigitMapText[0] - '0');
         }

         if( uLoopCounter < 1 )
         {
            eStatus = eCMDM_SYNTAXERROR;
         }
      }
      else
      {
         eStatus = eCMDM_SYNTAXERROR;
      }

      if( eStatus == eCMDM_SUCCESS )
      {
         if( localState.pcDigitMapText[0] == '}' )
         {
            CMDMCOMPILEDCOUNT *pCompiledCount =
               (CMDMCOMPILEDCOUNT *)localState.pCurrentEntry;

            /* Now we sucessfully have the repetition counts so add to the state
             * machine */
            /* Make sure there is enough room for a counting state */
            if( (BOS_UINT32)pCompiledCount->uValue <=
                (BOS_UINT32)localState.pDerefTableStart )
            {
               pCompiledCount->uType = eCMDM_TLVCOUNT; /* type */
               pCompiledCount->uReserved = 0;
               pCompiledCount->uMin = uMinRepeat;
               pCompiledCount->uMax = uMaxRepeat;
               localState.pCurrentEntry =
                  (CMDMTLVHEADER *)pCompiledCount->uValue;

               localState.pcDigitMapText++;

               *pCompilerState = localState;
            }
            else
            {
               eStatus = eCMDM_ERROR;
            }
         }
         else
         {
            eStatus = eCMDM_SYNTAXERROR;
         }
      }
   }
   else
   {
      eStatus = eCMDM_SYNTAXERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   key
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a Key as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS key(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMCOMPILEDINPUT *pCompiledTarget =
      (CMDMCOMPILEDINPUT *)localState.pCurrentEntry;
   CMDMSTATUS eStatus = eCMDM_SUCCESS;

   /* Make sure there is enough room for a target state */
   if( (BOS_UINT32)pCompiledTarget->uValue <=
       (BOS_UINT32)localState.pDerefTableStart )
   {
      BOS_UINT16 uDigits = 0;

      /* Key = DIGIT / "X" / "*" / "#" / "A" /  "B" / "C" / "D" /
       *       "[" ["^"] 1*((DIGIT ["-" DIGIT]) /
       *       (("A" /  "B" / "C" / "D") ["-" ("A" /  "B" / "C" / "D")]) /
       *       "*" / "#") "]" */
      if( (isdigit(localState.pcDigitMapText[0])) ||
          ((toupper(localState.pcDigitMapText[0]) >= 'A') &&
           (toupper(localState.pcDigitMapText[0]) <= 'D')) ||
          (localState.pcDigitMapText[0] == '*') ||
          (localState.pcDigitMapText[0] == '#') )
      {
         uDigits =
            (BOS_UINT16) cmMapGetId(keyMap, toupper(localState.pcDigitMapText[0]));
         localState.pcDigitMapText++;
      }
      else if( toupper(localState.pcDigitMapText[0]) == 'X' )
      {
         uDigits = 0x03ff;
         localState.pcDigitMapText++;
      }
      else if( localState.pcDigitMapText[0] == '[' )
      {
         BOS_UINT32 uLoopCounter;
         BOS_BOOL bExclude = BOS_FALSE;

         localState.pcDigitMapText++;

         if( localState.pcDigitMapText[0] == '^' )
         {
            localState.pcDigitMapText++;
            bExclude = BOS_TRUE;
         }

         for( uLoopCounter = 0; ( eStatus == eCMDM_SUCCESS ); uLoopCounter++ )
         {
            if( isdigit(localState.pcDigitMapText[0]) )
            {
               uDigits |= cmMapGetId(keyMap, localState.pcDigitMapText[0]);
               localState.pcDigitMapText++;

               if( localState.pcDigitMapText[0] == '-' )
               {
                  localState.pcDigitMapText++;

                  if( isdigit(localState.pcDigitMapText[0]) &&
                      (localState.pcDigitMapText[0] > localState.pcDigitMapText[-2]) )
                  {
                     BOS_UINT8 d;

                     for( d = localState.pcDigitMapText[-2] + 1;
                          d <= localState.pcDigitMapText[0];
                          d++)
                     {
                        uDigits |= cmMapGetId(keyMap, d);
                     }

                     localState.pcDigitMapText++;
                  }
                  else
                  {
                     eStatus = eCMDM_SYNTAXERROR;
                  }
               }
            }
            else if( (toupper(localState.pcDigitMapText[0]) >= 'A') &&
                     (toupper(localState.pcDigitMapText[0]) <= 'D') )
            {
               uDigits |= cmMapGetId(keyMap, toupper(localState.pcDigitMapText[0]));
               localState.pcDigitMapText++;

               if( localState.pcDigitMapText[0] == '-' )
               {
                  localState.pcDigitMapText++;

                  if( (toupper(localState.pcDigitMapText[0]) >= 'A') &&
                      (toupper(localState.pcDigitMapText[0]) <= 'D') &&
                      (localState.pcDigitMapText[0] >
                       localState.pcDigitMapText[-2]) )
                  {
                     BOS_UINT8 d;

                     for( d = (localState.pcDigitMapText[-2] + 1);
                          d <= localState.pcDigitMapText[0]; d++)
                     {
                        uDigits |= cmMapGetId(keyMap,
                                            toupper(localState.pcDigitMapText[0]));
                     }

                     localState.pcDigitMapText++;
                  }
                  else
                  {
                     eStatus = eCMDM_SYNTAXERROR;
                  }
               }
            }
            else if( (localState.pcDigitMapText[0] == '*') ||
                     (localState.pcDigitMapText[0] == '#') )
            {
               uDigits |= cmMapGetId(keyMap, localState.pcDigitMapText[0]);
               localState.pcDigitMapText++;
            }
            else if( localState.pcDigitMapText[0] == ']' )
            {
               localState.pcDigitMapText++;
               break;
            }
            else
            {
               eStatus = eCMDM_SYNTAXERROR;
            }
         }

         if( bExclude )
         {
            /* We actually want to exclude all the digits we just included so
             * flip it */
            uDigits = ~uDigits;
         }
      }
      else
      {
         eStatus = eCMDM_SYNTAXERROR;
      }

      if( (eStatus == eCMDM_SUCCESS) && (uDigits != 0) )
      {
         pCompiledTarget->uType = eCMDM_TLVINPUT;
         pCompiledTarget->cTimer = 0;
         pCompiledTarget->uDigits = uDigits;
         localState.pCurrentEntry = (CMDMTLVHEADER *)pCompiledTarget->uValue;

         *pCompilerState = localState;
      }
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   subPattern
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a SubPattern as defined in Annex A of the PKT-SP-RSTF
**              Specification. Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**              bAllowReferences - Boolean to indicate whether references should
**                                be allowed in this pPattern body. References
**                                are not allowed when compiling symbols in the
**                                link stage.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS subPattern(CMDMCOMPILERSTATE *pCompilerState,
                             BOS_BOOL bAllowReferences)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* SubPattern = "(" PatternBody ")" / "(" "=" Reference ")" */
   if( localState.pcDigitMapText[0] == '(' )
   {
      localState.pcDigitMapText++;

      if( (localState.pcDigitMapText[0] == '=') && bAllowReferences )
      {
         localState.pcDigitMapText++;

         if( (eStatus = reference(&localState, eCMDM_PATTERNREF)) ==
             eCMDM_SUCCESS )
         {
            if(localState.pcDigitMapText[0] == ')')
            {
               localState.pcDigitMapText++;
               *pCompilerState = localState;
            }
            else
            {
               eStatus = eCMDM_SYNTAXERROR;
            }
         }
      }
      else
      {
         /* Make sure there is enough room for the TLV header */
         if( (BOS_UINT32)localState.pCurrentEntry->uValue <=
             (BOS_UINT32)localState.pDerefTableStart )
         {
            localState.pCurrentEntry->uType = eCMDM_TLVPATTERN; /* type */
            localState.pCurrentEntry->uReserved = 0;
            localState.pCurrentEntry =
               (CMDMTLVHEADER *)localState.pCurrentEntry->uValue;

            if( (eStatus = patternBody(&localState, bAllowReferences)) ==
                eCMDM_SUCCESS )
            {
               if( localState.pcDigitMapText[0] == ')' )
               {
                  localState.pcDigitMapText++;

                  /* Length without the type and length fields */
                  pCompilerState->pCurrentEntry->uLength =
                     (BOS_UINT16) ( (BOS_UINT32)localState.pCurrentEntry -
                               (BOS_UINT32)pCompilerState->pCurrentEntry->uValue );
                  *pCompilerState = localState;
               }
               else
               {
                  eStatus = eCMDM_SYNTAXERROR;
               }
            }
         }
         else
         {
            eStatus = eCMDM_ERROR;
         }
      }
   }
   else
   {
      eStatus = eCMDM_SYNTAXERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   reference
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a Reference as defined in Annex A of the PKT-SP-RSTF
**              Specification. Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**              eType - The type of reference being added.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
**
***********************************************************************************/
static CMDMSTATUS reference(CMDMCOMPILERSTATE *pCompilerState,
                            CMDMREFTYPE eType)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;
   const BOS_UINT8 *pcNameRefStart = localState.pcDigitMapText;

   /* Reference = MapNameRef / SymbolNameRef */
   if( (eStatus = identifier(&localState)) == eCMDM_SUCCESS )
   {
      BOS_UINT32 uStringLen = localState.pcDigitMapText - pcNameRefStart;
      BOS_UINT32 uEntrySize = offsetof(CMDMTLVHEADER, uValue) +
                              offsetof(CMDMDEREFENTRY, cIdent) +
                              uStringLen + stringPad(uStringLen);

      /* Setup for new entry in the dereferencing table */
      localState.pDerefTableStart =
         (CMDMTLVHEADER *)((BOS_UINT32)(localState.pDerefTableStart) -
                           uEntrySize);

      /* Make sure there's space to add a reference to both the
       * dereferencing table and the compiled map. */
      if( ((BOS_UINT32)localState.pCurrentEntry +
           offsetof(CMDMCOMPILEDREFERENCE, uValue)) <=
          (BOS_UINT32)localState.pDerefTableStart )
      {
         CMDMCOMPILEDREFERENCE *pCompiledReference =
            (CMDMCOMPILEDREFERENCE *)localState.pCurrentEntry;
         CMDMDEREFENTRY *pDerefEntry =
            (CMDMDEREFENTRY *)localState.pDerefTableStart->uValue;

         /* Set the reference in the map to null for now */
         pCompiledReference->uType = eCMDM_TLVREFERENCE;
         pCompiledReference->uReserved = 0;
         pCompiledReference->uOffset = 0;
         localState.pCurrentEntry =
            (CMDMTLVHEADER *)pCompiledReference->uValue;

         /* Add the dereferencing table entry */
         localState.pDerefTableStart->uType = (eType == eCMDM_PATTERNREF) ?
            eCMDM_TLVDEREFPATTERNREF : eCMDM_TLVDEREFACTIONREF;
         localState.pDerefTableStart->uReserved = 0;
         localState.pDerefTableStart->uLength =
                                       offsetof(CMDMDEREFENTRY, cIdent) +
                                       (BOS_UINT16)uStringLen +
                                       (BOS_UINT16)stringPad(uStringLen);
         pDerefEntry->uOffset = (BOS_UINT16)( (BOS_UINT32)pCompiledReference -
                                 (BOS_UINT32)localState.pDigitMap );

         /* Copy identifier */
         strncpy((char *)pDerefEntry->cIdent,
                 (const char *)pcNameRefStart, uStringLen);
         /* Zero padding to next word */
         memset((pDerefEntry->cIdent + uStringLen), '\0',
                stringPad(uStringLen));

         *pCompilerState = localState;
      }
      else
      {
         eStatus = eCMDM_ERROR;
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   nameDef
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a MapNameDef/SymbolNameDef as defined in Annex A of the
**              PKT-SP-RSTF Specification. Adds to the digit map state machine as
**              required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS nameDef(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;
   const BOS_UINT8 *pcNameDefStart = localState.pcDigitMapText;

   /* MapNameDef = Identifier */
   /* SymbolNameDef = Identifier */
   if( (eStatus = identifier(&localState)) == eCMDM_SUCCESS )
   {
      BOS_UINT32 uStringLen = localState.pcDigitMapText - pcNameDefStart;
      BOS_UINT32 uEntrySize = offsetof(CMDMTLVHEADER, uValue) +
                              offsetof(CMDMDEREFENTRY, cIdent) +
                              uStringLen + stringPad(uStringLen);

      /* Setup for new entry in the dereferencing table */
      CMDMTLVHEADER *pNewDerefTableStart = 
         (CMDMTLVHEADER *)((BOS_UINT32)(localState.pDerefTableStart) -
                           uEntrySize);

      /* Make sure there's space to add an entry to the dereferencing table. */
      if( (BOS_UINT32)localState.pCurrentEntry <=
          (BOS_UINT32)pNewDerefTableStart )
      {
         /* Look for the identifier in the dereferencing table */
         CMDMTLVHEADER *pDerefHeader = localState.pDerefTableStart;

         while( (BOS_UINT32)pDerefHeader <
                (BOS_UINT32)localState.pDerefTableEnd )
         {
            if( pDerefHeader->uType == eCMDM_TLVDEREFDEF )
            {
               CMDMDEREFENTRY *pDerefEntry =
                  (CMDMDEREFENTRY *)pDerefHeader->uValue;

               if( strCmpNoCaseSize((const char *)pDerefEntry->cIdent,
                                    (const char *)pcNameDefStart,
                                    uStringLen) == 0 )
               {
                  /* Found duplicate definition */
                  break;
               }
            }

            /* No duplicate found yet. Move to the next entry. */
            pDerefHeader = getNextElement(pDerefHeader);
         }

         /* Check if a duplicate was found */
         if( (BOS_UINT32)pDerefHeader == (BOS_UINT32)localState.pDerefTableEnd )
         {
            /* No duplicate. Add definition entry to dereferencing table. */
            CMDMDEREFENTRY *pDerefEntry;

            localState.pDerefTableStart = pNewDerefTableStart;
            localState.pDerefTableStart->uType = eCMDM_TLVDEREFDEF;
            localState.pDerefTableStart->uReserved = 0;
            localState.pDerefTableStart->uLength =
               offsetof(CMDMDEREFENTRY, cIdent) + (BOS_UINT16)uStringLen +
               (BOS_UINT16)stringPad(uStringLen);

            pDerefEntry =
               (CMDMDEREFENTRY *)localState.pDerefTableStart->uValue;
            pDerefEntry->uOffset = (BOS_UINT16)
               ( (BOS_UINT32)localState.pCurrentEntry -
                 (BOS_UINT32)localState.pDigitMap );

            /* Copy identifier */
            strncpy((char *)pDerefEntry->cIdent,
                    (const char *)pcNameDefStart, uStringLen);
            /* Zero padding to next word */
            memset((pDerefEntry->cIdent + uStringLen), '\0',
                   stringPad(uStringLen));

            *pCompilerState = localState;
         }
         else
         {
            /* Duplicate found */
            eStatus = eCMDM_SYNTAXERROR;
         }
      }
      else
      {
         eStatus = eCMDM_ERROR;
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   timerDef
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a TimerDef as defined in Annex A of the PKT-SP-RSTF
**              Specification. Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS timerDef(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;
   BOS_UINT32 uLoopCounter;

   /* TimerDef = "Timer" 1*WSP Timer EQUAL Fraction LineBreak */
   if( strCmpNoCaseSize((const char *)localState.pcDigitMapText, "Timer",
            strlen("Timer")) == 0 )
   {
      localState.pcDigitMapText += strlen("Timer");

      for( uLoopCounter = 0;
           (localState.pcDigitMapText[0] == ' ') ||
           (localState.pcDigitMapText[0] == '\t');
           localState.pcDigitMapText++,uLoopCounter++ );

      if(uLoopCounter >= 1)
      {
         switch( toupper(localState.pcDigitMapText[0]) )
         {
            case 'S':
            case 'Z':
            {
               /* Make sure there is enough room for a timer definition */
               if( ((BOS_UINT32)localState.pCurrentEntry->uValue +
                        offsetof(CMDMCOMPILEDTIMER, uValue)) <=
                   (BOS_UINT32)localState.pDerefTableStart )
               {
                  CMDMCOMPILEDTIMER *pCompiledTimer =
                     (CMDMCOMPILEDTIMER *)localState.pCurrentEntry->uValue;

                  /* type */
                  localState.pCurrentEntry->uType = eCMDM_TLVTIMER;
                  /* length */
                  localState.pCurrentEntry->uLength =
                     offsetof(CMDMCOMPILEDTIMER, uValue);

                  pCompiledTimer->cTimer =
                        (BOS_UINT8) toupper(localState.pcDigitMapText[0]);
                  pCompiledTimer->uReserved = 0;

                  localState.pCurrentEntry =
                     (CMDMTLVHEADER *)pCompiledTimer->uValue;
                  localState.pcDigitMapText++;

                  if( ((eStatus = equal(&localState)) == eCMDM_SUCCESS) &&
                      ( isdigit(localState.pcDigitMapText[0]) ||
                       ((localState.pcDigitMapText[0] == '.') &&
                        isdigit((localState.pcDigitMapText[1])))) )
                  {
                     BOS_UINT16 uTimerVal;

                     for( uTimerVal = 0; isdigit(localState.pcDigitMapText[0]);
                          localState.pcDigitMapText++ )
                     {
                        uTimerVal = (uTimerVal*10) +
                                   (localState.pcDigitMapText[0] - '0');
                     }

                     /* Convert from seconds to milliseconds */
                     uTimerVal *= 1000;

                     if( localState.pcDigitMapText[0] == '.' )
                     {
                        BOS_SINT32 i;
                        localState.pcDigitMapText++;

                        /* Only three digits after the decimal are significant
                         * since we just have millisecond accuracy. */
                        for( i = 2; isdigit(localState.pcDigitMapText[0]);
                             localState.pcDigitMapText++ )
                        {
                           if( i >= 0 )
                           {
                              /* uTimerVal +=
                               *    (localState.pcDigitMapText[0] - '0') * 10^i */
                              BOS_UINT32 j;
                              BOS_UINT32 uDecimalVal =
                                 localState.pcDigitMapText[0] - '0';

                              for( j = 0; j < (BOS_UINT32)i; j++ )
                              {
                                 uDecimalVal *= 10;
                              }

                              uTimerVal = uTimerVal + (BOS_UINT16) uDecimalVal;
                              i--;
                           }
                        }
                     }

                     if( (eStatus = lineBreak(&localState)) == eCMDM_SUCCESS )
                     {
                        pCompiledTimer->uMSec = uTimerVal;

                        *pCompilerState = localState;
                        eStatus = eCMDM_SUCCESS;
                     }
                  }
                  else
                  {
                     eStatus = eCMDM_SYNTAXERROR;
                  }
               }
               else
               {
                  eStatus = eCMDM_ERROR;
               }
            }
            break;

            default:
            {
               eStatus = eCMDM_SYNTAXERROR;
            }
            break;
         }
      }
      else
      {
         eStatus = eCMDM_SYNTAXERROR;
      }
   }
   else
   {
      eStatus = eCMDM_SYNTAXERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   symbolDef
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a SymbolDef as defined in Annex A of the PKT-SP-RSTF
**              Specification. Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS symbolDef(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* SymbolDef = SymbolNameDef EQUAL SymbolVal LineBreak */
   /* SymbolVal = Constant *(SWSC Constant) / "&" NonAmpersand */
   if( (eStatus = nameDef(&localState)) == eCMDM_SUCCESS)
   {
      if( (eStatus = equal(&localState)) == eCMDM_SUCCESS)
      {
         /* Make sure there is enough room for the type and length */
         if( (BOS_UINT32)localState.pCurrentEntry->uValue <=
             (BOS_UINT32)localState.pDerefTableStart )
         {
            CMDMTLVHEADER *pcCompiledString = localState.pCurrentEntry;

            /* type */
            pcCompiledString->uType = eCMDM_TLVSTRING;
            pcCompiledString->uReserved = 0;
            localState.pCurrentEntry =
               (CMDMTLVHEADER *)pcCompiledString->uValue;

            /* if starts with '&', it is an external variable. */
            if( *localState.pcDigitMapText == '&' )
            {
               localState.pcDigitMapText++;

               if ( (eStatus = nonAmpersand( &localState )) == eCMDM_SUCCESS )
               {
                  if( (eStatus = lineBreak( &localState )) == eCMDM_SUCCESS )
                  {
                     BOS_UINT32 uStringLen =
                        (BOS_UINT32)localState.pCurrentEntry -
                        (BOS_UINT32)pcCompiledString->uValue;

                     /* Make sure there's room to add padding */
                     if( ((BOS_UINT32)localState.pCurrentEntry +
                          stringPad(uStringLen)) <=
                         (BOS_UINT32)localState.pDerefTableStart )
                     {
                        /* Add padding */
                        memset(&pcCompiledString->uValue[uStringLen], '\0',
                               stringPad(uStringLen));
                        localState.pCurrentEntry = (CMDMTLVHEADER *)
                           ((BOS_UINT32)localState.pCurrentEntry +
                            stringPad(uStringLen));
                        pcCompiledString->uLength =
                           (BOS_UINT16) ( uStringLen + stringPad(uStringLen) );
                        *pCompilerState = localState;
                     }
                     else
                     {
                        eStatus = eCMDM_ERROR;
                     }
                  }
               }
            }
            /* otherwise check for constant. */
            else if( (eStatus = constant(&localState)) == eCMDM_SUCCESS )
            {
               CMDMCOMPILERSTATE subState;

               for( subState = localState; ; localState = subState )
               {
                  if( (eStatus = swsc(&subState)) == eCMDM_SUCCESS )
                  {
                     if( (eStatus = constant(&subState)) != eCMDM_SUCCESS )
                     {
                        break;
                     }
                  }
                  else
                  {
                     break;
                  }
               }

               if( eStatus == eCMDM_SYNTAXERROR )
               {
                  if( (eStatus = lineBreak(&localState)) == eCMDM_SUCCESS )
                  {
                     BOS_UINT32 uStringLen =
                        (BOS_UINT32)localState.pCurrentEntry -
                        (BOS_UINT32)pcCompiledString->uValue;

                     /* Make sure there's room to add padding */
                     if( ((BOS_UINT32)localState.pCurrentEntry +
                          stringPad(uStringLen)) <=
                         (BOS_UINT32)localState.pDerefTableStart )
                     {
                        /* Add padding */
                        memset(&pcCompiledString->uValue[uStringLen], '\0',
                               stringPad(uStringLen));
                        localState.pCurrentEntry = (CMDMTLVHEADER *)
                           ((BOS_UINT32)localState.pCurrentEntry +
                            stringPad(uStringLen));
                        pcCompiledString->uLength =
                           (BOS_UINT16) ( uStringLen + stringPad(uStringLen) );
                        *pCompilerState = localState;
                     }
                     else
                     {
                        eStatus = eCMDM_ERROR;
                     }
                  }
               }
            }
         }
         else
         {
            eStatus = eCMDM_ERROR;
         }
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   constant
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a Constant as defined in Annex A of the PKT-SP-RSTF
**              Specification. Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS constant(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* The following is the official rule from the RSTF:
    *
    * Constant = DQUOTE 1*NonDQuote DQUOTE / SQUOTE 1*NonSQuote SQUOTE
    *
    * However, some customers have expressed a need to be able to assign an
    * empty string to a symbol. This seems reasonable and shouldn't bother
    * anyone so the actual rule we implement here is:
    *
    * Constant = DQUOTE *NonDQuote DQUOTE / SQUOTE *NonSQuote SQUOTE
    */
   if(*localState.pcDigitMapText == '"')
   {
      const BOS_UINT8 *pcConstStart;

      localState.pcDigitMapText++;

      /* Find the end of the constant */
      for( pcConstStart = localState.pcDigitMapText;
           ( isgraph(localState.pcDigitMapText[0]) &&
           (localState.pcDigitMapText[0] != '"') );
           localState.pcDigitMapText++ );

      /* Make sure we properly found the end of the constant */
      if( (localState.pcDigitMapText >= pcConstStart) &&
          (localState.pcDigitMapText[0] == '"') )
      {
         /* Make sure there's enough space to copy the string to the compiled
          * digit map */
         if( ((BOS_UINT32)localState.pCurrentEntry +
              ((BOS_UINT32)localState.pcDigitMapText - (BOS_UINT32)pcConstStart)
              + 1) <= (BOS_UINT32)localState.pDerefTableStart )
         {
            strncpy((char *)localState.pCurrentEntry,
                    (const char *)pcConstStart,
                  ((BOS_UINT32)localState.pcDigitMapText - (BOS_UINT32)pcConstStart));
            localState.pCurrentEntry = (CMDMTLVHEADER *)
               ((BOS_UINT32)localState.pCurrentEntry +
               (localState.pcDigitMapText - pcConstStart));
            localState.pcDigitMapText++;
            *pCompilerState = localState;
            eStatus = eCMDM_SUCCESS;
         }
         else
         {
            eStatus = eCMDM_ERROR;
         }
      }
      else
      {
         eStatus = eCMDM_SYNTAXERROR;
      }
   }
   else if(localState.pcDigitMapText[0] == '\'')
   {
      const BOS_UINT8 *pcConstStart;

      localState.pcDigitMapText++;

      /* Find the end of the constant */
      for( pcConstStart = localState.pcDigitMapText;
           isgraph(localState.pcDigitMapText[0]) &&
           (localState.pcDigitMapText[0] != '\'');
           localState.pcDigitMapText++ );

      /* Make sure we properly found the end of the constant */
      if( (localState.pcDigitMapText >= pcConstStart) &&
          (localState.pcDigitMapText[0] == '\'') )
      {
         /* Make sure there's enough space to copy the string to the compiled
          * digit map */
         if( ((BOS_UINT32)localState.pCurrentEntry +
              (localState.pcDigitMapText - pcConstStart) + 1) <=
             (BOS_UINT32)localState.pDerefTableStart )
         {
            strncpy((char *)localState.pCurrentEntry,
                    (const char *)pcConstStart,
                    (localState.pcDigitMapText - pcConstStart));
            localState.pCurrentEntry = (CMDMTLVHEADER *)
               ((BOS_UINT32)localState.pCurrentEntry +
                (localState.pcDigitMapText - pcConstStart));
            localState.pcDigitMapText++;
            *pCompilerState = localState;
            eStatus = eCMDM_SUCCESS;
         }
         else
         {
            eStatus = eCMDM_ERROR;
         }
      }
      else
      {
         eStatus = eCMDM_SYNTAXERROR;
      }
   }
   else
   {
      eStatus = eCMDM_SYNTAXERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   equal
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a EQUAL as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS equal(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* EQUAL = SWSC "=" SWSC */
   if( (eStatus = swsc(&localState)) == eCMDM_SUCCESS )
   {
      if( localState.pcDigitMapText[0] == '=' )
      {
         localState.pcDigitMapText++;

         if( (eStatus = swsc(&localState)) == eCMDM_SUCCESS )
         {
            *pCompilerState = localState;
         }
      }
      else
      {
         eStatus = eCMDM_SYNTAXERROR;
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   swsc
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a SWSC as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS swsc(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;

   /* SWSC = [LWSC] */
   if( lwsc(&localState) == eCMDM_SUCCESS )
   {
      *pCompilerState = localState;
   }

   return eCMDM_SUCCESS;
}

/***********************************************************************************
**  FUNCTION:   lwsc
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a LWSC as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS lwsc(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;
   BOS_UINT32 uLoopCounter;

   /* LWSC = *LineBreak 1*WSP */
   while(lineBreak(&localState) == eCMDM_SUCCESS);

   for( uLoopCounter = 0;
        (localState.pcDigitMapText[0] == ' ') ||
        (localState.pcDigitMapText[0] == '\t');
        localState.pcDigitMapText++,uLoopCounter++ );

   if( uLoopCounter >= 1 )
   {
      eStatus = eCMDM_SUCCESS;
      *pCompilerState = localState;
   }
   else
   {
      eStatus = eCMDM_SYNTAXERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   identifier
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a Identifier as defined in Annex A of the PKT-SP-RSTF
**              Specification.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS identifier(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* Identifier = ALPHA *( [ "_" / "-" ] 1*Alphanum ) */
   if( isalpha(localState.pcDigitMapText[0]) )
   {
      localState.pcDigitMapText++;

      while( isalnum(localState.pcDigitMapText[0]) ||
             ((localState.pcDigitMapText[0] == '_') &&
              isalnum(localState.pcDigitMapText[1])) ||
             ((localState.pcDigitMapText[0] == '-') &&
              isalnum(localState.pcDigitMapText[1])) )
      {
         localState.pcDigitMapText++;
      }

      eStatus = eCMDM_SUCCESS;
      *pCompilerState = localState;
   }
   else
   {
      eStatus = eCMDM_SYNTAXERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   lineBreak
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a LineBreak as defined in Annex A of the PKT-SP-RSTF
**              Specification. Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS lineBreak(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus = eCMDM_SUCCESS;
   BOS_UINT32 uLoopCounter;

   /* LineBreak = 1*(*WSP (CRLF / Comment)) */
   for( uLoopCounter = 0; (eStatus == eCMDM_SUCCESS); )
   {
      while( (localState.pcDigitMapText[0] == ' ') ||
             (localState.pcDigitMapText[0] == '\t') )
      {
         localState.pcDigitMapText++;
      }

      if( (localState.pcDigitMapText[0] == '\r') &&
               (localState.pcDigitMapText[1] == '\n') )
      {
         localState.pcDigitMapText += 2;
         if( localState.pcDigitMapText > *localState.ppcDigitMapTextProgress )
         {
            *localState.ppcDigitMapTextProgress = localState.pcDigitMapText;
         }
      }
      else
      {
         eStatus = comment(&localState);
      }

      if(eStatus == eCMDM_SUCCESS)
      {
         *pCompilerState = localState;
         uLoopCounter++;
      }
   }

   if(uLoopCounter >= 1)
   {
      eStatus = eCMDM_SUCCESS;
   }
   else
   {
      eStatus = eCMDM_SYNTAXERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   comment
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a Comment as defined in Annex A of the PKT-SP-RSTF Specification.
**              Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS comment(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus = eCMDM_SYNTAXERROR;

   /* Comment = "//" *CommentText CRLF */
   /* CommentText = VCHAR / WSP */
   if( (localState.pcDigitMapText[0] == '/') &&
       (localState.pcDigitMapText[1] == '/') )
   {
      localState.pcDigitMapText += 2;

      while( (isgraph(localState.pcDigitMapText[0])) ||
             (localState.pcDigitMapText[0] == ' ') ||
             (localState.pcDigitMapText[0] == '\t') )
      {
         localState.pcDigitMapText++;
      }

      if( (localState.pcDigitMapText[0] == '\r') &&
               (localState.pcDigitMapText[1] == '\n') )
      {
         localState.pcDigitMapText += 2;
         if( localState.pcDigitMapText > *localState.ppcDigitMapTextProgress )
         {
            *localState.ppcDigitMapTextProgress = localState.pcDigitMapText;
         }

         eStatus = eCMDM_SUCCESS;
         *pCompilerState = localState;
      }
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   stringPad
**
**  PURPOSE:    Calculate the amount of padding required for a string of a given
**              length. All elements of the compiled digit map have to be aligned
**              on word boundaries to simplify access. Strings need at least one
**              character of '\0' at the end, but also need the rest of the bytes
**              up to the next word boundary padded out as well. This tells how
**              many bytes of padding (including the '\0' byte) are needed for a
**              given string uLength.
**
**  PARAMETERS:
**              uStrLen - The length of the string in question.
**
**  RETURNS:    The pad length.
**
**  NOTES:
***********************************************************************************/
static BOS_UINT32 stringPad(BOS_UINT32 uStrLen)
{
   return (((uStrLen + sizeof(BOS_UINT32)) & ~(sizeof(BOS_UINT32)-1)) - uStrLen);
}

/***********************************************************************************
**  FUNCTION:   nonAmpersand
**
**  PURPOSE:    Determines if the given text block conforms to the definition of
**              a NonAmpersand as defined in Annex A of the PKT-SP-RSTF
**              Specification. Adds to the digit map state machine as required.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS nonAmpersand(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus;

   /* NonAmpersand = %x21-%x25 / %x27-7E */
   const BOS_UINT8 *pcExtVarStart;

   for( pcExtVarStart = localState.pcDigitMapText;
        ( isgraph(localState.pcDigitMapText[0]) &&
        (localState.pcDigitMapText[0] != '&') );
        localState.pcDigitMapText++ );

   if( localState.pcDigitMapText > pcExtVarStart )
   {
      BOS_UINT8 uExtVarName[ CMDM_ACTION_BUFFER_SIZE ];
      BOS_UINT8 uExtVarValue[ CMDM_ACTION_BUFFER_SIZE ];
      BOS_UINT32 uExtVarLen = CMDM_ACTION_BUFFER_SIZE;

      memset ( (void *) &uExtVarName,
               0,
               CMDM_ACTION_BUFFER_SIZE );
      strncpy((char *)uExtVarName,
              (const char *)pcExtVarStart,
              ((BOS_UINT32)localState.pcDigitMapText - (BOS_UINT32)pcExtVarStart));

      /* Get the external variable value.
      */
      if ( gCmDmExtVarCb )
      {
         gCmDmExtVarCb ( localState.uGuid,
                         uExtVarName,
                         uExtVarValue,
                         &uExtVarLen );
      }
      else
      {
         uExtVarLen = 0;
      }

      /* Make sure there's enough space to copy the string to the compiled
       * digit map */
      if( ((BOS_UINT32)localState.pCurrentEntry +
           uExtVarLen + 1) <= (BOS_UINT32)localState.pDerefTableStart )
      {
         strncpy((char *)localState.pCurrentEntry,
                 (const char *)uExtVarValue,
                 uExtVarLen);
         /*Advance the pointer in the digitmap forward
           by the size of the external variable value*/
         localState.pCurrentEntry = (CMDMTLVHEADER *)
            ((BOS_UINT32)localState.pCurrentEntry + uExtVarLen);
         
         *pCompilerState = localState;
         eStatus = eCMDM_SUCCESS;
      }
      else
      {
         eStatus = eCMDM_ERROR;
      }
   }
   else
   {
      eStatus = eCMDM_SYNTAXERROR;
   }

   return eStatus;
}

/**********************************************************************************
**  FUNCTION:   linkStage
**
**  PURPOSE:    The linking stage uses the temporary dereferencing table to
**              link references with the definitions and also to compile symbols
**              that are referenced from patterns.
**
**  PARAMETERS:
**              pCompilerState - Pointer to the compiler state structure.
**
**  RETURNS:    eCMDM_SYNTAXERROR if the digit map file does not conform to the
**              grammar specified in Annex A of the PKT-SP-RSTF Specification.
**              eCMDM_ERROR if there was a non-syntax problem in compiling the
**              digit map.
**              eCMDM_SUCCESS otherwise.
**
**  NOTES:      pCompilerState is left unchanged if a value other than
**              eCMDM_SUCCESS is returned.
***********************************************************************************/
static CMDMSTATUS linkStage(CMDMCOMPILERSTATE *pCompilerState)
{
   CMDMCOMPILERSTATE localState = *pCompilerState;
   CMDMSTATUS eStatus = eCMDM_SUCCESS;
   CMDMTLVHEADER *pDerefHeader = localState.pDerefTableStart;

   /* Iterate through each definition in the dereferencing table. For each reference,
   ** set the reference to point to the definition. In addition, compile symbol
   ** definitions that have at least one pattern reference and set the pattern
   ** references to the compiled symbol definition.
   */
   while( (eStatus == eCMDM_SUCCESS) &&
          ((BOS_UINT32)pDerefHeader <
           (BOS_UINT32)localState.pDerefTableEnd) )
   {
      if( pDerefHeader->uType == eCMDM_TLVDEREFDEF )
      {
         CMDMDEREFENTRY *pDerefDefEntry =
            (CMDMDEREFENTRY *)pDerefHeader->uValue;
         CMDMTLVHEADER *pDefinition = (CMDMTLVHEADER *)
            ((BOS_UINT32)localState.pDigitMap + pDerefDefEntry->uOffset);
         CMDMTLVHEADER *pCompiledDefinition = NULL;
         CMDMTLVHEADER *pDerefRefHeader = localState.pDerefTableStart;

         while( (eStatus == eCMDM_SUCCESS) &&
                ((BOS_UINT32)pDerefRefHeader <
                 (BOS_UINT32)localState.pDerefTableEnd) )
         {
            CMDMDEREFENTRY *pDerefRefEntry =
               (CMDMDEREFENTRY *)pDerefRefHeader->uValue;

            if( ((pDerefRefHeader->uType == eCMDM_TLVDEREFACTIONREF) ||
                 (pDerefRefHeader->uType == eCMDM_TLVDEREFPATTERNREF)) &&
                (strCmpNoCase((const char *)pDerefDefEntry->cIdent,
                              (const char *)pDerefRefEntry->cIdent) == 0) )
            {
               CMDMCOMPILEDREFERENCE *pCompiledReference =
                  (CMDMCOMPILEDREFERENCE *)
                  ((BOS_UINT32)localState.pDigitMap + pDerefRefEntry->uOffset);

               if( (pDerefRefHeader->uType == eCMDM_TLVDEREFPATTERNREF) &&
                   (pDefinition->uType == eCMDM_TLVSTRING) )
               {
                  /* A pattern reference to a symbol definition means we need to
                   * compile the symbol definition as a pattern. We only want to
                   * do this once though. */
                  if( pCompiledDefinition == NULL )
                  {
                     CMDMCOMPILERSTATE subState = localState;

                     if( (BOS_UINT32)subState.pCurrentEntry->uValue <=
                         (BOS_UINT32)subState.pDerefTableStart )
                     {
                        subState.pCurrentEntry->uType = eCMDM_TLVPATTERN;
                        subState.pCurrentEntry->uReserved = 0;
                        subState.pCurrentEntry =
                           (CMDMTLVHEADER *)localState.pCurrentEntry->uValue;

                        /* Fake out the compiler state so patternBody() compiles
                         * the symbol string */
                        subState.pcDigitMapText = pDefinition->uValue;

                        /* Let the compiler know that references are not allowed
                         * in this pattern. Allowing references in symbols would
                         * be a mess because we could recursively compile and
                         * relink forever. */
                        if( (eStatus = patternBody(&subState, BOS_FALSE)) ==
                            eCMDM_SUCCESS )
                        {
                           /* Make sure we were able to compile the entire
                            * pattern */
                           if( strlen((const char *)subState.pcDigitMapText) ==
                                 0 )
                           {
                              /* Set the pCompiledDefinition pointer so we don't
                               * compile this symbol again */
                              pCompiledDefinition = localState.pCurrentEntry;

                              /* Set the length */
                              pCompiledDefinition->uLength = (BOS_UINT16)
                                 ( (BOS_UINT32)subState.pCurrentEntry -
                                   (BOS_UINT32)pCompiledDefinition->uValue );

                              localState = subState;
                           }
                           else
                           {
                              eStatus = eCMDM_SYNTAXERROR;
                           }
                        }
                     }
                     else
                     {
                        eStatus = eCMDM_ERROR;
                     }
                  }

                  if( eStatus == eCMDM_SUCCESS )
                  {
                     /* Set the reference to the compiled version of the
                      * symbol. */
                     pCompiledReference->uOffset = (BOS_UINT16)
                        ( (BOS_UINT32)pCompiledDefinition -
                          (BOS_UINT32)localState.pDigitMap );
                  }
               }
               else
               {
                  pCompiledReference->uOffset =
                                          (BOS_UINT16)pDerefDefEntry->uOffset;
               }

               /* Change this reference to zero since we don't need it again and
                * then we'll know that we found a definition for it. */
               pDerefRefEntry->uOffset = 0;
            }

            /* Move to the next entry. */
            pDerefRefHeader = getNextElement(pDerefRefHeader);
         }
      }

      /* Move to the next entry. */
      pDerefHeader = getNextElement(pDerefHeader);
   }

   /* Lastly we need to go through and make sure there are no references that
    * were not defined. */
   pDerefHeader = localState.pDerefTableStart;

   while( (eStatus == eCMDM_SUCCESS) &&
          ((BOS_UINT32)pDerefHeader <
           (BOS_UINT32)localState.pDerefTableEnd) )
   {
      CMDMDEREFENTRY *pDerefEntry =
         (CMDMDEREFENTRY *)pDerefHeader->uValue;

      if( ((pDerefHeader->uType == eCMDM_TLVDEREFACTIONREF) ||
           (pDerefHeader->uType == eCMDM_TLVDEREFPATTERNREF)) &&
          (pDerefEntry->uOffset != 0) )
      {
         CMGRLOG_WARNING(( "cmDm: linkStage - Undefined reference \"%s\"\n",
                           pDerefEntry->cIdent ));
         eStatus = eCMDM_SYNTAXERROR;
      }

      /* Move to the next entry. */
      pDerefHeader = getNextElement(pDerefHeader);
   }

   if( eStatus == eCMDM_SUCCESS )
   {
      /* Update the digitmap length without the type and length fields */
      localState.pDigitMap->uLength =
         (BOS_UINT16) ( (BOS_UINT32)localState.pCurrentEntry -
                        (BOS_UINT32)localState.pDigitMap->uValue );
      *pCompilerState = localState;
   }

   return eStatus;
}

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
CMDMSTATUS cmDmReset(CMDMACTIONSTATE *pActionState, BOS_UINT32 uStateSz)

{
   CMDMSTATUS eStatus;

   /* Reset the state structure */
   if( (pActionState != NULL) )
   {
      pActionState->uStateBufSz = (BOS_UINT16) uStateSz;
      pActionState->uLength = 0;
      pActionState->uActionsRetrieved = 0;
      pActionState->uParametersRetrieved = 0;
      eStatus = eCMDM_SUCCESS;
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

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
**  RETURNS:    eCMDM_SUCCESS is returned when a match is made and new action is
**              available for processing.
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
                               CMDMACTIONSTATE *pActionState)

{
   CMDMSTATUS eStatus = eCMDM_NOMATCH;

   CMGRLOG_INFORMATION (( "cmDmCheckDialString (0x%X): \'%s\'",
                          uGuid, pcDialString ));

   /* Validate the compiled map */
   if( (pDigitMap != NULL) && (pDigitMap->uType == eCMDM_TLVDIGITMAP) )
   {
      CMDMPROCESSORSTATE procState;
      BOS_UINT8 cValueStrArray[CMDM_MAP_VALUE_SIZE];

      /* Reset the action and parameter list so we don't add duplicates */
      pActionState->uLength = 0;

      /* Initialize digit map processor state */
      procState.uGuid = uGuid;
      procState.pDigitMap = pDigitMap;
      procState.pCurrentEntry = (CMDMTLVHEADER *)pDigitMap->uValue;
      procState.pActionState = pActionState;
      procState.pcDialStr = pcDialString;
      procState.uDialStrIdx = 0;
      procState.pcValueStr = cValueStrArray;
      procState.uValueStrIdx = 0;
      procState.pPatternRefs = NULL;
      procState.pPatternValRefs = NULL;
      procState.uPatternRefsIdx = 0;

      /* Iterate through each element in the digit map to find the first map */
      for( ;
           procState.pCurrentEntry < getNextElement((CMDMTLVHEADER *)pDigitMap);
           procState.pCurrentEntry = getNextElement(procState.pCurrentEntry) )
      {
         if( procState.pCurrentEntry->uType == eCMDM_TLVMAP )
         {
            /* Match against the first map */
            eStatus = matchMap(&procState);
            break;
         }
      }

      /* If the match was successful check out the action state to see if there
       * is a new action to be processed. */
      if( eStatus == eCMDM_SUCCESS )
      {
         CMDMACTIONTYPE eAction;

         if( getNextAction(pActionState, &eAction) != eCMDM_SUCCESS )
         {
            eStatus = eCMDM_NOMATCH;
         }
      }
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   matchMap
**
**  PURPOSE:    Match against a map.
**
**  PARAMETERS:
**              pProcState - State structure of the digit map processor.
**
**  RETURNS:    eCMDM_SUCCESS is returned if the map matches the dialstring.
**              eCMDM_NOMATCH is returned if the dialstring does not match any
**              rule in the map.
**              eCMDM_ERROR is returned if there is a problem processing the
**              compiled digit map.
**
**  NOTES:
***********************************************************************************/
static CMDMSTATUS matchMap(CMDMPROCESSORSTATE *pProcState)
{
   CMDMSTATUS eStatus = eCMDM_ERROR;

   /* Make sure we're at a map element */
   if( pProcState->pCurrentEntry->uType == eCMDM_TLVMAP )
   {
      const CMDMTLVHEADER *pMap = pProcState->pCurrentEntry;

      /* Iterate through each rule in the map */
      for( pProcState->pCurrentEntry = (CMDMTLVHEADER *)pMap->uValue;
           pProcState->pCurrentEntry < getNextElement(pMap);
           pProcState->pCurrentEntry =
               getNextElement(pProcState->pCurrentEntry) )
      {
         if( pProcState->pCurrentEntry->uType == eCMDM_TLVRULE )
         {
            /* Attempt match against the rule */
            if( (eStatus = matchRule(pProcState)) != eCMDM_NOMATCH )
            {
               break;
            }
         }
         else
         {
            /* Bad compiled map format */
            eStatus = eCMDM_ERROR;
            break;
         }
      }
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   matchRule
**
**  PURPOSE:    Match against a rule.
**
**  PARAMETERS:
**              pProcState - State structure of the digit map processor.
**
**  RETURNS:    eCMDM_SUCCESS is returned if the rule matches the dialstring.
**              eCMDM_NOMATCH is returned if the dialstring does not match the
**              rule.
**              eCMDM_ERROR is returned if there is a problem processing the
**              compiled digit map.
**
**  NOTES:
***********************************************************************************/
static CMDMSTATUS matchRule(CMDMPROCESSORSTATE *pProcState)
{
   CMDMSTATUS eStatus;

   /* Make sure we're at a rule element */
   if( pProcState->pCurrentEntry->uType == eCMDM_TLVRULE )
   {
      CMDMPATTERNREFERENCE patternRefs[CMDM_MAX_PATTERN_REFS];
      CMDMPATTERNREFERENCE patternValRefs[CMDM_MAX_PATTERN_REFS];
      const CMDMTLVHEADER *pRule = pProcState->pCurrentEntry;
      CMDMPATTERNREFERENCE *pOldPatternRefs = pProcState->pPatternRefs;
      CMDMPATTERNREFERENCE *pOldPatternValRefs = pProcState->pPatternValRefs;
      BOS_UINT32 uOldPatternRefsIdx = pProcState->uPatternRefsIdx;

      pProcState->pCurrentEntry =
         (CMDMTLVHEADER *)pProcState->pCurrentEntry->uValue;
      pProcState->pPatternRefs = patternRefs;
      pProcState->pPatternValRefs = patternValRefs;
      pProcState->uPatternRefsIdx = 0;

      /* Match pattern */
      if( (eStatus = matchPattern(pProcState)) == eCMDM_SUCCESS )
      {
         BOS_UINT8 cMapValue[CMDM_MAP_VALUE_SIZE] = "";

         /* Loop through all the actions for this rule */
         for( pProcState->pCurrentEntry =
                 getNextElement((CMDMTLVHEADER *)pRule->uValue);
              (pProcState->pCurrentEntry->uType == eCMDM_TLVACTION) &&
              (pProcState->pCurrentEntry < getNextElement(pRule)) &&
              (eStatus == eCMDM_SUCCESS);
              pProcState->pCurrentEntry =
                 getNextElement(pProcState->pCurrentEntry) )
         {
            const CMDMTLVHEADER *actionPtr = pProcState->pCurrentEntry;
            eStatus = processAction(pProcState, cMapValue, CMDM_MAP_VALUE_SIZE);
            pProcState->pCurrentEntry = actionPtr;
         }

         /* A return value of eCMDM_NOMATCH from processAction() just means that
          * an action that further processes the dialstring (USEMAP) didn't
          * match. We likely just need to stop processing actions and wait for
          * more digits. Note that this means the user could wait forever for
          * further feedback if the referenced map cannot be matched with the
          * input dialstring. To avoid this any map referenced from a USEMAP
          * action should have a catchall rule as its last. Anyway, the match on
          * this rule was still successful so return eCMDM_SUCCESS. */
         if( eStatus == eCMDM_NOMATCH )
         {
            eStatus = eCMDM_SUCCESS;
         }

         strncpy((char *)pProcState->pcValueStr,
                 (const char *)cMapValue, (CMDM_MAP_VALUE_SIZE-1));
         pProcState->uValueStrIdx = strlen((const char *)cMapValue);
      }
      else
      {
         pProcState->pCurrentEntry = pRule;
      }

      /* Set the pattern references back to whatever they were before because
       * ours aren't valid after we return anyway */
      pProcState->pPatternRefs = pOldPatternRefs;
      pProcState->pPatternValRefs = pOldPatternValRefs;
      pProcState->uPatternRefsIdx = uOldPatternRefsIdx;
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   processAction
**
**  PURPOSE:    Process an action. Add actions to the global list and process
**              built-in actions (RETURN, USEMAP).
**
**  PARAMETERS:
**              pProcState - State structure of the digit map processor.
**              pcMapValue - Location to store the resulting map value if it is
**                           modified by a RETURN action. You need a second
**                           buffer for this because there may be more than one
**                           RETURN action and only the last one affects the
**                           value of the map.
**              uMapValueSz - Size of the pcMapValue buffer.
**
**  RETURNS:    eCMDM_SUCCESS is returned if the action was successfully
**              processed.
**              eCMDM_ERROR is returned if there is a problem processing the
**              action.
**
**  NOTES:
***********************************************************************************/
static CMDMSTATUS processAction(CMDMPROCESSORSTATE *pProcState,
                                BOS_UINT8 *pcMapValue,
                                BOS_UINT32 uMapValueSz)
{
   CMDMSTATUS eStatus;

   /* Make sure we're at an action element */
   if( pProcState->pCurrentEntry->uType == eCMDM_TLVACTION )
   {
      CMDMCOMPILEDACTION *pCompiledAction =
         (CMDMCOMPILEDACTION *)pProcState->pCurrentEntry;

      /* Switch on the action type */
      switch( pCompiledAction->eActionType )
      {
         /* Process internal actions or add external actions by default */
         case eCMDM_RETURN:
            {
               eStatus = processReturnAction(pProcState, pcMapValue, uMapValueSz);
            }
            break;
         case eCMDM_USEMAP:
            {
               eStatus = processUsemapAction(pProcState);
            }
            break;
         default:
            {
               eStatus = processGeneralAction(pProcState);
            }
            break;
      }
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   processGeneralAction
**
**  PURPOSE:    Process an action. The action is placed in the list of actions
**              with all parameters.
**
**  PARAMETERS:
**              pProcState - State structure of the digit map processor.
**
**  RETURNS:    eCMDM_SUCCESS is returned if the action was successfully
**              processed.
**              eCMDM_ERROR is returned if there is a problem processing the
**              action.
**
**  NOTES:
***********************************************************************************/
static CMDMSTATUS processGeneralAction(CMDMPROCESSORSTATE *pProcState)
{
   CMDMSTATUS eStatus = eCMDM_SUCCESS;
   CMDMACTIONSTATE *pActionState = pProcState->pActionState;

   /* Make sure we have space to add an action to the action state */
   if( (pActionState->uStateBufSz - pActionState->uLength -
        offsetof(CMDMACTIONSTATE, uValue)) >=
       offsetof(CMDMCOMPILEDACTION, uValue) )
   {
      CMDMCOMPILEDACTION *pCompiledAction =
         (CMDMCOMPILEDACTION *)pProcState->pCurrentEntry;
      CMDMCOMPILEDACTION *pNewAction =
         (CMDMCOMPILEDACTION *)&pActionState->uValue[pActionState->uLength];
      CMDMTLVHEADER *pParam;

      pActionState->uLength += offsetof(CMDMCOMPILEDACTION, uValue);

      pNewAction->uType = eCMDM_TLVACTION;
      pNewAction->uReserved = 0;
      pNewAction->uLength = 0;
      pNewAction->eActionType = pCompiledAction->eActionType;

      /* Loop through each parameter for this action */
      for( pParam = (CMDMTLVHEADER *)pCompiledAction->uValue;
           (eStatus == eCMDM_SUCCESS) &&
           (pParam < getNextElement((CMDMTLVHEADER *)pCompiledAction));
           pParam = getNextElement((CMDMTLVHEADER *)pParam) )
      {
         /* Calculate size for string parameter */
         BOS_UINT32 uStringParamSz =
            (BOS_UINT32)&pActionState->uValue[pActionState->uStateBufSz] -
            (BOS_UINT32)&pNewAction->uValue[pNewAction->uLength];

         /* Make sure we have space to add a string parameter */
         if( uStringParamSz >=
               (offsetof(CMDMTLVHEADER, uValue) + sizeof(BOS_UINT32)) )
         {
            CMDMTLVHEADER *pStringParam =
               (CMDMTLVHEADER *)&pNewAction->uValue[pNewAction->uLength];

            pStringParam->uType = eCMDM_TLVSTRING;
            pStringParam->uReserved = 0;

            pProcState->pCurrentEntry = (CMDMTLVHEADER *)pParam;

            eStatus = getStringParameter(pProcState, pStringParam->uValue,
                         (uStringParamSz - offsetof(CMDMTLVHEADER, uValue)) );

            /* Pad out to the next word boundary */
            memset(&pStringParam->uValue[strlen((const char *)pStringParam->uValue)],
                   '\0', stringPad(strlen((const char *)pStringParam->uValue)));
            pStringParam->uLength =
               (BOS_UINT16) ( strlen((const char *)pStringParam->uValue) +
                          stringPad(strlen((const char *)pStringParam->uValue)) );
            pNewAction->uLength =
               (BOS_UINT16) (
                     (BOS_UINT32)&pStringParam->uValue[pStringParam->uLength] -
                     (BOS_UINT32)&pNewAction->uValue );
         }
         else
         {
            eStatus = eCMDM_ERROR;
            break;
         }
      }

      pActionState->uLength = pActionState->uLength + pNewAction->uLength;
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   processUsemapAction
**
**  PURPOSE:    Process a USEMAP type action.
**
**  PARAMETERS:
**              pProcState - State structure of the digit map processor.
**
**  RETURNS:    eCMDM_SUCCESS is returned if the action was successfully
**              processed.
**              eCMDM_ERROR is returned if there is a problem processing the
**              action.
**
**  NOTES:
***********************************************************************************/
static CMDMSTATUS processUsemapAction(CMDMPROCESSORSTATE *pProcState)
{
   CMDMCOMPILEDACTION *pCompiledAction =
      (CMDMCOMPILEDACTION *)pProcState->pCurrentEntry;
   CMDMSTATUS eStatus;

   /* Processing of a new map requires a new value string and new pattern
    * references */
   BOS_UINT8 cSubValueStr[CMDM_MAP_VALUE_SIZE];
   BOS_UINT32 uSavedDialStrIdx = pProcState->uDialStrIdx;
   BOS_UINT8 *cSavedValueStr = pProcState->pcValueStr;
   BOS_UINT32 uSavedValueStrIdx = pProcState->uValueStrIdx;
   CMDMPATTERNREFERENCE *pSavedPatternRefs = pProcState->pPatternRefs;
   CMDMPATTERNREFERENCE *pSavedPatternValRefs = pProcState->pPatternValRefs;
   BOS_UINT32 uSavedPatternRefsIdx = pProcState->uPatternRefsIdx;

   memset(cSubValueStr, '\0', sizeof(cSubValueStr));
   pProcState->pcValueStr = cSubValueStr;
   pProcState->uValueStrIdx = 0;
   pProcState->pPatternRefs = NULL;
   pProcState->pPatternValRefs = NULL;
   pProcState->uPatternRefsIdx = 0;

   pProcState->pCurrentEntry = (CMDMTLVHEADER *)pCompiledAction->uValue;

   /* Get map reference parameter */
   if( (pProcState->pCurrentEntry <
            getNextElement((CMDMTLVHEADER *)pCompiledAction)) &&
       (pProcState->pCurrentEntry->uType == eCMDM_TLVPARAMETER) )
   {
      pProcState->pCurrentEntry =
         (CMDMTLVHEADER *)pProcState->pCurrentEntry->uValue;

      if( pProcState->pCurrentEntry->uType == eCMDM_TLVREFERENCE )
      {
         CMDMCOMPILEDREFERENCE *pCompiledReference =
            (CMDMCOMPILEDREFERENCE *)pProcState->pCurrentEntry;
         pProcState->pCurrentEntry = (CMDMTLVHEADER *)
            ((BOS_UINT32)pProcState->pDigitMap + pCompiledReference->uOffset);

         /* Jump to referenced map */
         eStatus = matchMap(pProcState);
      }
      else
      {
         eStatus = eCMDM_ERROR;
      }
   }
   else
   {
      eStatus = eCMDM_ERROR;

      /* The first map defined within the DigitMapPackage is used when no
       * parameter is specified for a USEMAP action. Iterate through each
       * element in the digit map to find the first map */
      for( pProcState->pCurrentEntry =
            (CMDMTLVHEADER *)pProcState->pDigitMap->uValue;
           pProcState->pCurrentEntry <
              getNextElement((CMDMTLVHEADER *)pProcState->pDigitMap);
           pProcState->pCurrentEntry = getNextElement(pProcState->pCurrentEntry) )
      {
         if( pProcState->pCurrentEntry->uType == eCMDM_TLVMAP )
         {
            /* Match against the first map */
            eStatus = matchMap(pProcState);
            break;
         }
      }
   }

   /* Restore the saved context upon returning from the map */
   pProcState->uDialStrIdx = uSavedDialStrIdx;
   pProcState->pcValueStr = cSavedValueStr;
   pProcState->uValueStrIdx = uSavedValueStrIdx;
   pProcState->pPatternRefs = pSavedPatternRefs;
   pProcState->pPatternValRefs = pSavedPatternValRefs;
   pProcState->uPatternRefsIdx = uSavedPatternRefsIdx;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   processReturnAction
**
**  PURPOSE:    Process a RETURN type action.
**
**  PARAMETERS:
**              pProcState - State structure of the digit map processor.
**              pcMapValue - Location to store the resulting map value if it is
**                           modified by a RETURN action. You need a second
**                           buffer for this because there may be more than one
**                           RETURN action and only the last one affects the
**                           value of the map.
**              uMapValueSz - Size of the pcMapValue buffer.
**
**  RETURNS:    eCMDM_SUCCESS is returned if the action was successfully
**              processed.
**              eCMDM_ERROR is returned if there is a problem processing the
**              action.
**
**  NOTES:
***********************************************************************************/
static CMDMSTATUS processReturnAction(CMDMPROCESSORSTATE *pProcState,
                                      BOS_UINT8 *pcMapValue,
                                      BOS_UINT32 uMapValueSz)
{
   CMDMCOMPILEDACTION *pCompiledAction =
      (CMDMCOMPILEDACTION *)pProcState->pCurrentEntry;
   CMDMSTATUS eStatus;

   pProcState->pCurrentEntry = (CMDMTLVHEADER *)pCompiledAction->uValue;

   if( (pProcState->pCurrentEntry <
            getNextElement((CMDMTLVHEADER *)pCompiledAction)) &&
       (pProcState->pCurrentEntry->uType == eCMDM_TLVPARAMETER) )
   {
      eStatus = getStringParameter(pProcState, pcMapValue, uMapValueSz);
   }
   else if(uMapValueSz >= 1)
   {
      /* There are no parameters */
      pcMapValue[0] = '\0';
      eStatus = eCMDM_SUCCESS;
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   getStringParameter
**
**  PURPOSE:    Get the processed string specified by a string parameter.
**
**  PARAMETERS:
**              pProcState - State structure of the digit map processor.
**              pcStrParam - Location to store the resulting string.
**              uStrParamSz - Size of the pcStrParam buffer.
**
**  RETURNS:    eCMDM_SUCCESS is returned if a string parameter was successfully
**              processed.
**              eCMDM_ERROR is returned if there is a problem processing the
**              string parameter.
**
**  NOTES:
***********************************************************************************/
static CMDMSTATUS getStringParameter(CMDMPROCESSORSTATE *pProcState,
                                     BOS_UINT8 *pcStrParam,
                                     BOS_UINT32 uStrParamSz)
{
   CMDMSTATUS eStatus = eCMDM_SUCCESS;

   if( pProcState->pCurrentEntry->uType == eCMDM_TLVPARAMETER )
   {
      CMDMTLVHEADER *pParam =
         (CMDMTLVHEADER *)pProcState->pCurrentEntry;
      pcStrParam[0] = '\0'; /* Init to empty string */

      /* Set the parameter string by looping through all string pieces */
      for( pProcState->pCurrentEntry =
            (CMDMTLVHEADER *)pProcState->pCurrentEntry->uValue;
           (pProcState->pCurrentEntry < getNextElement(pParam)) &&
           (eStatus == eCMDM_SUCCESS); )
      {
         switch( pProcState->pCurrentEntry->uType )
         {
            case eCMDM_TLVSTRING:
               {
                  strCatSize( (char *)pcStrParam,
                      (const char *)pProcState->pCurrentEntry->uValue,
                      CMDM_MIN( (uStrParamSz-strlen((const char *)pcStrParam)-1),
                                pProcState->pCurrentEntry->uLength ) );

                  pProcState->pCurrentEntry =
                     getNextElement(pProcState->pCurrentEntry);
               }
               break;
            case eCMDM_TLVREFERENCE:
               {
                  CMDMCOMPILEDREFERENCE *pCompiledReference =
                     (CMDMCOMPILEDREFERENCE *)pProcState->pCurrentEntry;

                  CMDMTLVHEADER *referencedElement = (CMDMTLVHEADER *)
                     ((BOS_UINT32)pProcState->pDigitMap +
                      pCompiledReference->uOffset);

                  if( referencedElement->uType == eCMDM_TLVSTRING )
                  {
                     strCatSize( (char *)pcStrParam,
                        (const char *)referencedElement->uValue,
                        CMDM_MIN( (uStrParamSz-strlen((const char *)pcStrParam)-1),
                                  referencedElement->uLength ) );

                     pProcState->pCurrentEntry =
                        (CMDMTLVHEADER *)pProcState->pCurrentEntry->uValue;
                  }
                  else
                  {
                     eStatus = eCMDM_ERROR;
                  }
               }
               break;
            case eCMDM_TLVPATTERNREF:
               {
                  CMDMCOMPILEDPATTERNREF *pCompiledpatternRef =
                     (CMDMCOMPILEDPATTERNREF *)pProcState->pCurrentEntry;

                  /* Check reference */
                  if( pCompiledpatternRef->uPatternNum <
                        pProcState->uPatternRefsIdx )
                  {
                     BOS_UINT32 uStartIdx = pProcState->pPatternRefs
                           [pCompiledpatternRef->uPatternNum].uStart;
                     BOS_UINT32 uRefLength = pProcState->pPatternRefs
                        [pCompiledpatternRef->uPatternNum].uEnd - uStartIdx;

                     strCatSize( (char *)pcStrParam,
                         (const char *)&pProcState->pcDialStr[uStartIdx],
                         CMDM_MIN( (uStrParamSz-strlen((const char *)pcStrParam)-1),
                                   uRefLength) );

                     pProcState->pCurrentEntry =
                        (CMDMTLVHEADER *)pProcState->pCurrentEntry->uValue;
                  }
                  else
                  {
                     eStatus = eCMDM_ERROR;
                  }
               }
               break;
            case eCMDM_TLVPATTERNVALREF:
               {
                  CMDMCOMPILEDPATTERNREF *pCompiledpatternRef =
                     (CMDMCOMPILEDPATTERNREF *)pProcState->pCurrentEntry;

                  /* Check reference */
                  if( pCompiledpatternRef->uPatternNum <
                        pProcState->uPatternRefsIdx )
                  {
                     BOS_UINT32 uStartIdx = pProcState->pPatternValRefs
                           [pCompiledpatternRef->uPatternNum].uStart;
                     BOS_UINT32 uRefLength = pProcState->pPatternValRefs
                        [pCompiledpatternRef->uPatternNum].uEnd - uStartIdx;

                     strncat( (char *)pcStrParam,
                         (const char *)&pProcState->pcValueStr[uStartIdx],
                         CMDM_MIN( (uStrParamSz-strlen((const char *)pcStrParam)-1),
                                   uRefLength) );

                     pProcState->pCurrentEntry =
                        (CMDMTLVHEADER *)pProcState->pCurrentEntry->uValue;
                  }
                  else
                  {
                     eStatus = eCMDM_ERROR;
                  }
               }
               break;
            default:
               {
                  eStatus = eCMDM_ERROR;
               }
               break;
         }
      }
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   matchPattern
**
**  PURPOSE:    Match against a pattern and add pattern references.
**
**  PARAMETERS:
**              pProcState - State structure of the digit map processor.
**
**  RETURNS:    eCMDM_SUCCESS is returned if the pattern matches the dialstring.
**              eCMDM_NOMATCH is returned if the dialstring does not match the
**              pPattern.
**              eCMDM_ERROR is returned if there is a problem processing the
**              compiled digit map.
**
**  NOTES:      This is a complicated recursive function that performs the
**              backtracking for regular expression matching. Wikipedia has a
**              decent introductory article on backtracking.
***********************************************************************************/
static CMDMSTATUS matchPattern(CMDMPROCESSORSTATE *pProcState)
{
   CMDMSTATUS eStatus = eCMDM_SUCCESS;

   /* Make sure we're at a pattern element and that there is enough memory */
   if( (pProcState->pCurrentEntry->uType == eCMDM_TLVPATTERN) &&
       (pProcState->uPatternRefsIdx < CMDM_MAX_PATTERN_REFS) )
   {
      const CMDMTLVHEADER *pPatternStart = pProcState->pCurrentEntry;
      BOS_BOOL bFixedCount = BOS_FALSE;

      /* First go through the pattern and find the first count range. */
      while( (pProcState->pCurrentEntry < getNextElement(pPatternStart)) &&
             (eStatus == eCMDM_SUCCESS) && (bFixedCount == BOS_FALSE) )
      {
         switch( pProcState->pCurrentEntry->uType )
         {
            case eCMDM_TLVINPUT:
            case eCMDM_TLVREFERENCE:
            case eCMDM_TLVPATTERN:
               {
                  /* Keep looking */
                  pProcState->pCurrentEntry =
                     (CMDMTLVHEADER *)pProcState->pCurrentEntry->uValue;
               }
               break;
            case eCMDM_TLVCOUNT:
               {
                  CMDMCOMPILEDCOUNT *pCompiledCount =
                     (CMDMCOMPILEDCOUNT *)pProcState->pCurrentEntry;

                  if( pCompiledCount->uMin != pCompiledCount->uMax )
                  {
                     BOS_UINT8 uPatternCopy[CMDM_COMPILED_PATTERN_SIZE];
                     CMDMCOMPILEDCOUNT *pCountCopy;
                     BOS_UINT32 uCopySize;
                     BOS_SINT32 i;

                     bFixedCount = BOS_TRUE;

                     /* Copy this pattern to another buffer where we can modify
                      * it. */
                     uCopySize = offsetof(CMDMTLVHEADER, uValue) +
                                 pPatternStart->uLength;

                     if( uCopySize < sizeof(uPatternCopy) )
                     {
                        memcpy(uPatternCopy, pPatternStart, uCopySize);

                        /* Set the pCountCopy pointer to point to the same count
                         * structure in the pattern copy. */
                        pCountCopy =
                           (CMDMCOMPILEDCOUNT *)((BOS_UINT32)uPatternCopy +
                              ((BOS_UINT32)pCompiledCount -
                               (BOS_UINT32)pPatternStart));

                        /* We run matchPattern on our modified pattern buffer */
                        pProcState->pCurrentEntry =
                           (CMDMTLVHEADER *)uPatternCopy;

                        /* Try all possibilities of this counter for a match.
                         * Backtracking. */
                        for(i = pCompiledCount->uMax, eStatus = eCMDM_NOMATCH;
                            (i >= pCompiledCount->uMin) &&
                            (eStatus == eCMDM_NOMATCH);
                            i--)
                        {
                           /* Fix the count */
                           pCountCopy->uMin = pCountCopy->uMax = (BOS_UINT8)i;

                           eStatus = matchPattern(pProcState);
                        }
                     }
                     else
                     {
                        eStatus = eCMDM_ERROR;
                     }
                  }
                  else
                  {
                     /* Keep looking */
                     pProcState->pCurrentEntry =
                        (CMDMTLVHEADER *)pProcState->pCurrentEntry->uValue;
                  }
               }
               break;
            default:
               eStatus = eCMDM_ERROR;
               break;
         }

      }

      /* If we didn't actually need to fix any count ranges then we can go ahead
       * and match the pattern. Otherwise we just cleanup before exiting. */
      if( eStatus == eCMDM_SUCCESS )
      {
         if( bFixedCount == BOS_FALSE )
         {
            /* There are no count ranges in the pattern to be fixed. Now we can
             * start matching. Backup old values first. */
            BOS_UINT32 uOldDialStrIdx = pProcState->uDialStrIdx;
            BOS_UINT32 uOldValueStrIdx = pProcState->uValueStrIdx;
            BOS_UINT32 uOldPatternRefsIdx = pProcState->uPatternRefsIdx;

            /* Set pattern references */
            pProcState->pPatternRefs[uOldPatternRefsIdx].uStart =
               pProcState->uDialStrIdx;
            pProcState->pPatternValRefs[uOldPatternRefsIdx].uStart =
               pProcState->uValueStrIdx;
            pProcState->uPatternRefsIdx++;

            /* Iterate through the pattern */
            for( pProcState->pCurrentEntry =
                  (CMDMTLVHEADER *)pPatternStart->uValue;
                 (pProcState->pCurrentEntry < getNextElement(pPatternStart)) &&
                    (pProcState->pCurrentEntry != NULL) &&
                    (eStatus == eCMDM_SUCCESS);
                 pProcState->pCurrentEntry =
                    getNextTarget(pProcState->pCurrentEntry) )
            {
               BOS_UINT32 uCountMin = 1, uCountMax = 1, j;
               CMDMTLVHEADER *pNextElement =
                  getNextElement(pProcState->pCurrentEntry);

               /* Look for a counter after this target. */
               if( (pNextElement < getNextElement(pPatternStart)) &&
                   (pNextElement->uType == eCMDM_TLVCOUNT) )
               {
                  /* Get counters. Min and max will be the same now. */
                  CMDMCOMPILEDCOUNT *pCompiledCount =
                     (CMDMCOMPILEDCOUNT *)pNextElement;
                  uCountMin = pCompiledCount->uMin;
                  uCountMax = pCompiledCount->uMax;
               }

               if( (uCountMin == 0) &&
                   ( (pProcState->pCurrentEntry->uType == eCMDM_TLVPATTERN) ||
                     (pProcState->pCurrentEntry->uType == eCMDM_TLVREFERENCE) ) )
               {
                  /* Even if we are going to count this pattern zero times we
                   * still need to have an entry in the pattern references table
                   * because otherwise all the subsequent references will be
                   * off. */
                  pProcState->pPatternRefs[pProcState->uPatternRefsIdx].uStart =
                     pProcState->uDialStrIdx;
                  pProcState->pPatternValRefs[pProcState->uPatternRefsIdx].uStart =
                     pProcState->uValueStrIdx;
                  pProcState->pPatternRefs[pProcState->uPatternRefsIdx].uEnd =
                     pProcState->uDialStrIdx;
                  pProcState->pPatternValRefs[pProcState->uPatternRefsIdx].uEnd =
                     pProcState->uValueStrIdx;
                  pProcState->uPatternRefsIdx++;
               }
               else
               {
                  /* Iterate through counter */
                  for( j = 1;
                       (j <= uCountMax) && (eStatus == eCMDM_SUCCESS); j++ )
                  {
                     eStatus = matchTarget(pProcState);
                  }
               }
            }

            if( eStatus == eCMDM_SUCCESS )
            {
               /* Set pattern references */
               pProcState->pPatternRefs[uOldPatternRefsIdx].uEnd =
                  pProcState->uDialStrIdx;
               pProcState->pPatternValRefs[uOldPatternRefsIdx].uEnd =
                  pProcState->uValueStrIdx;
            }
            else
            {
               /* Restore old values */
               pProcState->uPatternRefsIdx = uOldPatternRefsIdx;
               pProcState->uValueStrIdx = uOldValueStrIdx;
               pProcState->uDialStrIdx = uOldDialStrIdx;
               pProcState->pCurrentEntry = pPatternStart;
            }
         }
         else
         {
            pProcState->pCurrentEntry = getNextElement(pPatternStart);
         }
      }
      else
      {
         pProcState->pCurrentEntry = pPatternStart;
      }
   }
   else
   {
      /* Not enough memory or bad compiled map format */
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   matchTarget
**
**  PURPOSE:    Match a target.
**
**  PARAMETERS:
**              pProcState - State structure of the digit map processor.
**
**  RETURNS:    eCMDM_SUCCESS is returned if the target matches the input.
**              eCMDM_NOMATCH is returned if the input does not match the
**              target.
**              eCMDM_ERROR is returned if there is a problem processing the
**              compiled digit map.
**
**  NOTES:
***********************************************************************************/
static CMDMSTATUS matchTarget(CMDMPROCESSORSTATE *pProcState)
{
   CMDMSTATUS eStatus;
   const CMDMTLVHEADER *pSavedElement = pProcState->pCurrentEntry;

   switch( pProcState->pCurrentEntry->uType )
   {
      case eCMDM_TLVINPUT:
         {
            eStatus = matchTargetInputState(pProcState);
         }
         break;
      case eCMDM_TLVREFERENCE:
         {
            eStatus = matchTargetReferenceState(pProcState);
         }
         break;
      case eCMDM_TLVPATTERN:
         {
            eStatus = matchPattern(pProcState);
         }
         break;
      default:
         eStatus = eCMDM_ERROR;
         break;
   }

   pProcState->pCurrentEntry = pSavedElement;

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   matchTargetInputState
**
**  PURPOSE:    Match a target input state.
**
**  PARAMETERS:
**              pProcState - State structure of the digit map processor.
**
**  RETURNS:    eCMDM_SUCCESS is returned if the target matches the input.
**              eCMDM_NOMATCH is returned if the input does not match the
**              target.
**              eCMDM_ERROR is returned if there is a problem processing the
**              compiled digit map.
**
**  NOTES:
***********************************************************************************/
static CMDMSTATUS matchTargetInputState(CMDMPROCESSORSTATE *pProcState)
{
   CMDMSTATUS eStatus;
   CMDMCOMPILEDINPUT *pCompiledInput =
      (CMDMCOMPILEDINPUT *)pProcState->pCurrentEntry;

   if( (pCompiledInput->uDigits == 0) && (pCompiledInput->cTimer != 0) )
   {
      if( pProcState->uDialStrIdx < strlen((const char *)pProcState->pcDialStr) )
      {
         /* This is a timer input state. Check for a match */
         if( toupper(pProcState->pcDialStr[pProcState->uDialStrIdx]) ==
             pCompiledInput->cTimer )
         {
            /* Copy to the value string */
            if( pProcState->uValueStrIdx < CMDM_MAP_VALUE_SIZE )
            {
               pProcState->pcValueStr[pProcState->uValueStrIdx] =
                  pProcState->pcDialStr[pProcState->uDialStrIdx];
               pProcState->uValueStrIdx++;
               pProcState->uDialStrIdx++;
               eStatus = eCMDM_SUCCESS;
            }
            else
            {
               eStatus = eCMDM_ERROR;
            }
         }
         else
         {
            eStatus = eCMDM_NOMATCH;
         }
      }
      else
      {
         eStatus = eCMDM_NOMATCH;
      }
   }
   else if( (pCompiledInput->uDigits != 0) && (pCompiledInput->cTimer == 0) )
   {
      if( pProcState->uDialStrIdx < strlen((const char *)pProcState->pcDialStr) )
      {
         /* This is a digit input state. Check for a match */
         BOS_UINT32 uInput = cmMapGetId(keyMap,
                   toupper(pProcState->pcDialStr[pProcState->uDialStrIdx]));

         if( uInput != CMGR_INVALID_HANDLE )
         {
            if( (BOS_UINT16)uInput & pCompiledInput->uDigits )
            {
               /* Copy to the value string */
               if( pProcState->uValueStrIdx < CMDM_MAP_VALUE_SIZE )
               {
                  pProcState->pcValueStr[pProcState->uValueStrIdx] =
                     pProcState->pcDialStr[pProcState->uDialStrIdx];
                  pProcState->uValueStrIdx++;
                  pProcState->uDialStrIdx++;
                  eStatus = eCMDM_SUCCESS;
               }
               else
               {
                  eStatus = eCMDM_ERROR;
               }
            }
            else
            {
               eStatus = eCMDM_NOMATCH;
            }
         }
         else
         {
            eStatus = eCMDM_NOMATCH;
         }
      }
      else
      {
         eStatus = eCMDM_NOMATCH;
      }
   }
   else
   {
      /* Unknown input state type */
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   matchTargetReferenceState
**
**  PURPOSE:    Match a target reference state.
**
**  PARAMETERS:
**              pProcState - State structure of the digit map processor.
**
**  RETURNS:    eCMDM_SUCCESS is returned if the target matches the input.
**              eCMDM_NOMATCH is returned if the input does not match the
**              target.
**              eCMDM_ERROR is returned if there is a problem processing the
**              compiled digit map.
**
**  NOTES:
***********************************************************************************/
static CMDMSTATUS matchTargetReferenceState(CMDMPROCESSORSTATE *pProcState)
{
   CMDMSTATUS eStatus;
   CMDMCOMPILEDREFERENCE *pCompiledReference =
      (CMDMCOMPILEDREFERENCE *)pProcState->pCurrentEntry;
   BOS_UINT32 uCurrPatternRefsIdx = pProcState->uPatternRefsIdx;

   /* Set up for the jump to the reference */
   pProcState->pCurrentEntry = (CMDMTLVHEADER *)
      ((BOS_UINT32)pProcState->pDigitMap + pCompiledReference->uOffset);

   /* Call appropriate function based on target type */
   if( pProcState->pCurrentEntry->uType == eCMDM_TLVPATTERN )
   {
      eStatus = matchPattern(pProcState);
   }
   else if( (pProcState->pCurrentEntry->uType == eCMDM_TLVMAP) &&
            (uCurrPatternRefsIdx < CMDM_MAX_PATTERN_REFS) )
   {
      /* Processing of a new map requires a new value string and new pattern
       * references */
      BOS_UINT8 cSubValueStr[CMDM_MAP_VALUE_SIZE];
      BOS_UINT8 *cSavedValueStr = pProcState->pcValueStr;
      BOS_UINT32 uSavedValueStrIdx = pProcState->uValueStrIdx;
      BOS_UINT32 uSavedDialStrIdx = pProcState->uDialStrIdx;
      CMDMPATTERNREFERENCE *pSavedPatternRefs = pProcState->pPatternRefs;
      CMDMPATTERNREFERENCE *pSavedPatternValRefs = pProcState->pPatternValRefs;

      memset(cSubValueStr, '\0', sizeof(cSubValueStr));
      pProcState->pcValueStr = cSubValueStr;
      pProcState->uValueStrIdx = 0;
      pProcState->pPatternRefs = NULL;
      pProcState->pPatternValRefs = NULL;
      pProcState->uPatternRefsIdx = 0;

      eStatus = matchMap(pProcState);

      if( eStatus == eCMDM_SUCCESS )
      {
         /* Add start sub-pPattern reference point */
         pSavedPatternRefs[uCurrPatternRefsIdx].uStart = uSavedDialStrIdx;
         pSavedPatternValRefs[uCurrPatternRefsIdx].uStart = uSavedValueStrIdx;

         /* Update value string with value of map */
         strncpy((char *)&cSavedValueStr[uSavedValueStrIdx],
                 (const char *)cSubValueStr,
                 (CMDM_MAP_VALUE_SIZE-uSavedValueStrIdx-1));
         uSavedValueStrIdx +=
            CMDM_MIN((CMDM_MAP_VALUE_SIZE-uSavedValueStrIdx-1),
                     pProcState->uValueStrIdx);

         /* Add end sub-pPattern reference point */
         pSavedPatternRefs[uCurrPatternRefsIdx].uEnd = pProcState->uDialStrIdx;
         pSavedPatternValRefs[uCurrPatternRefsIdx].uEnd = uSavedValueStrIdx;
      }

      /* Restore the saved context upon returning from the map */
      pProcState->pcValueStr = cSavedValueStr;
      pProcState->uValueStrIdx = uSavedValueStrIdx;
      pProcState->pPatternRefs = pSavedPatternRefs;
      pProcState->pPatternValRefs = pSavedPatternValRefs;
   }
   else
   {
      /* Unknown reference type or not enough memory */
      eStatus = eCMDM_ERROR;
   }

   /* Restore appropriate state */
   pProcState->pCurrentEntry = (CMDMTLVHEADER *)pCompiledReference;

   if( eStatus == eCMDM_SUCCESS )
   {
      /* Even if a reference contains other subpatterns it only counts as a
       * single pattern at this level so just set the index to include only the
       * outside pattern */
      pProcState->uPatternRefsIdx = uCurrPatternRefsIdx + 1;
   }
   else
   {
      pProcState->uPatternRefsIdx = uCurrPatternRefsIdx;
   }

   return eStatus;
}

/***********************************************************************************
**  FUNCTION:   getNextTarget
**
**  PURPOSE:    Return a pointer to the next target state given the current
**              target state.
**
**  PARAMETERS:
**              pCurrentTarget - Current target state.
**
**  RETURNS:    Returns a pointer to the next target state.
**
**  NOTES:      The processor state is not changed by this function.
***********************************************************************************/
static CMDMTLVHEADER *getNextTarget(const CMDMTLVHEADER *pCurrentTarget)
{
   CMDMTLVHEADER *pNextTarget = getNextElement(pCurrentTarget);

   /* Check for a counting state that we have to skip over */
   if( pNextTarget->uType == eCMDM_TLVCOUNT )
   {
      pNextTarget = (CMDMTLVHEADER *)pNextTarget->uValue;
   }

   return pNextTarget;
}

/***********************************************************************************
**  FUNCTION:   getNextElement
**
**  PURPOSE:    Read the TLV header and return the address of the next element.
**
**  PARAMETERS:
**              pElement - The element to move past.
**
**  RETURNS:    Pointer to the next element.
**
**  NOTES:
***********************************************************************************/
static CMDMTLVHEADER *getNextElement(const CMDMTLVHEADER *pElement)
{
   CMDMTLVHEADER *pNextElement = NULL;

   switch( pElement->uType )
   {
      case eCMDM_TLVREFERENCE:
      case eCMDM_TLVINPUT:
      case eCMDM_TLVCOUNT:
         pNextElement = (CMDMTLVHEADER *)pElement->uValue;
         break;
      default:
         pNextElement = (CMDMTLVHEADER *)
            ((BOS_UINT32)pElement->uValue + pElement->uLength);
         break;
   }

   return pNextElement;
}

/***********************************************************************************
**  FUNCTION:   getNextAction
**
**  PURPOSE:    This function retrieves the next action from the given digit map
**              state structure. Actions are specified by the digit map during a
**              call to cmDmCheckDialString(...). This is the interal version of
**              this function that actually does all the work. It is used in a
**              couple of places so that's the reason for an internal version.
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
static CMDMSTATUS getNextAction(CMDMACTIONSTATE *pActionState,
                                CMDMACTIONTYPE *peAction)
{
   CMDMSTATUS eStatus;

   /* Sanity check */
   if( ((pActionState->uLength + offsetof(CMDMACTIONSTATE, uValue)) <=
        pActionState->uStateBufSz) )
   {
      CMDMCOMPILEDACTION *pAction = (CMDMCOMPILEDACTION *)pActionState->uValue;
      BOS_UINT32 i;

      /* Skip over actions we already got */
      for( i = 0;
           (i < pActionState->uActionsRetrieved) &&
           (pAction->uType == eCMDM_TLVACTION) &&
           ((BOS_UINT32)&pAction->uValue[pAction->uLength] <=
            (BOS_UINT32)&pActionState->uValue[pActionState->uLength]);
           i++ )
      {
         pAction = (CMDMCOMPILEDACTION *)&pAction->uValue[pAction->uLength];
      }

      /* Sanity check */
      if( i == pActionState->uActionsRetrieved )
      {
         /* Check if there is an action to return */
         if( (pAction->uType == eCMDM_TLVACTION) &&
             ((BOS_UINT32)&pAction->uValue[pAction->uLength] <=
              (BOS_UINT32)&pActionState->uValue[pActionState->uLength]) )
         {
            /* Return the next action */
            *peAction = pAction->eActionType;
            eStatus = eCMDM_SUCCESS;
         }
         else
         {
            /* No action to get */
            eStatus = eCMDM_NOMATCH;
         }
      }
      else
      {
         /* Something is wrong */
         eStatus = eCMDM_ERROR;
      }
   }
   else
   {
      /* Something is wrong */
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}


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
                         CMDMACTIONTYPE *peAction)
{
   CMDMSTATUS eStatus;

   eStatus = getNextAction(pActionState, peAction);

   if( eStatus == eCMDM_SUCCESS )
   {
      /* Got the next action. Make it so this action can't be retrieved a second
       * time and setup for parameters to be retrieved. */
      pActionState->uActionsRetrieved++;
      pActionState->uParametersRetrieved = 0;

      CMGRLOG_INFORMATION (( "cmDmGetAction: %s (0x%X), cnt %d",
                (char *) cmMapGetStr ( cmMapDmAction, *peAction ),
                *peAction, pActionState->uActionsRetrieved ));
   }

   return eStatus;
}

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
                            BOS_UINT8 **ppcParameter)
{
   CMDMSTATUS eStatus;

   /* Sanity check */
   if( ((pActionState->uLength + offsetof(CMDMACTIONSTATE, uValue)) <=
        pActionState->uStateBufSz) && (pActionState->uActionsRetrieved > 0) )
   {
      CMDMCOMPILEDACTION *pAction = (CMDMCOMPILEDACTION *)pActionState->uValue;
      BOS_UINT16 i;

      /* Skip over actions we already got */
      for( i = 0;
           (i < (pActionState->uActionsRetrieved-1)) &&
           (pAction->uType == eCMDM_TLVACTION) &&
           ((BOS_UINT32)&pAction->uValue[pAction->uLength] <=
            (BOS_UINT32)&pActionState->uValue[pActionState->uLength]);
           i++ )
      {
         pAction = (CMDMCOMPILEDACTION *)&pAction->uValue[pAction->uLength];
      }

      /* Sanity check */
      if( i == (pActionState->uActionsRetrieved-1) )
      {
         /* Check if there is an action from which to get parameters */
         if( (pAction->uType == eCMDM_TLVACTION) &&
             ((BOS_UINT32)&pAction->uValue[pAction->uLength] <=
              (BOS_UINT32)&pActionState->uValue[pActionState->uLength]) )
         {
            CMDMTLVHEADER *pParameter = (CMDMTLVHEADER *)pAction->uValue;

            /* Skip over parameters we already got */
            for( i = 0;
                 (i < pActionState->uParametersRetrieved) &&
                 (pParameter->uType == eCMDM_TLVSTRING) &&
                 ((BOS_UINT32)&pParameter->uValue[pParameter->uLength] <=
                  (BOS_UINT32)&pAction->uValue[pAction->uLength]);
                 i++ )
            {
               pParameter =
                  (CMDMTLVHEADER *)&pParameter->uValue[pParameter->uLength];
            }

            /* Sanity check */
            if( i == pActionState->uParametersRetrieved )
            {
               /* Check if there is a parameter to return */
               if( (pParameter->uType == eCMDM_TLVSTRING) &&
                   ((BOS_UINT32)&pParameter->uValue[pParameter->uLength] <=
                    (BOS_UINT32)&pAction->uValue[pAction->uLength]) )
               {
                  /* Return the next action */
                  *ppcParameter = pParameter->uValue;
                  pActionState->uParametersRetrieved++;
                  eStatus = eCMDM_SUCCESS;

                  CMGRLOG_INFORMATION (( "cmDmGetParameter: \'%s\', cnt %d",
                            pParameter->uValue,
                            pActionState->uParametersRetrieved ));
               }
               else
               {
                  /* No action to get */
                  eStatus = eCMDM_NOMATCH;
               }
            }
            else
            {
               /* Something is wrong */
               eStatus = eCMDM_ERROR;
            }
         }
         else
         {
            /* No action to get */
            eStatus = eCMDM_NOMATCH;
         }
      }
      else
      {
         /* Something is wrong */
         eStatus = eCMDM_ERROR;
      }
   }
   else
   {
      /* Something is wrong */
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}

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
                             BOS_UINT32 *puMsValue)
{
   CMDMSTATUS eStatus;

   /* Validate the compiled map */
   if( (pDigitMap != NULL) && (pDigitMap->uType == eCMDM_TLVDIGITMAP) )
   {
      CMDMTLVHEADER *pCurrentEntry = (CMDMTLVHEADER *)pDigitMap->uValue;
      eStatus = eCMDM_NOMATCH;

      /* Iterate through each element in the digit map */
      for( ;
           pCurrentEntry < getNextElement((CMDMTLVHEADER *)pDigitMap);
           pCurrentEntry = getNextElement(pCurrentEntry) )
      {
         if( pCurrentEntry->uType == eCMDM_TLVTIMER )
         {
            CMDMCOMPILEDTIMER *pCompiledTimer =
               (CMDMCOMPILEDTIMER *)pCurrentEntry->uValue;

            if( pCompiledTimer->cTimer == toupper(cTimer) )
            {
               /* Found our timer */
               *puMsValue = pCompiledTimer->uMSec;
               eStatus = eCMDM_SUCCESS;
               break;
            }
         }
      }
   }
   else
   {
      eStatus = eCMDM_ERROR;
   }

   return eStatus;
}
