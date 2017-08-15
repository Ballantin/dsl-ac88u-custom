/*****************************************************************************
* Copyright 2005 - 2011 Broadcom.  All rights reserved.
*
* This program is the proprietary software of Broadcom and/or
* its licensors, and may only be used, duplicated, modified or distributed
* pursuant to the terms and conditions of a separate, written license
* agreement executed between you and Broadcom (an "Authorized License").
* Except as set forth in an Authorized License, Broadcom grants no license
* (express or implied), right to use, or waiver of any kind with respect to
* the Software, and Broadcom expressly reserves all rights in and to the
* Software and all intellectual property rights therein.  IF YOU HAVE NO
* AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY
* WAY, AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF
* THE SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
* 1. This program, including its structure, sequence and organization,
*    constitutes the valuable trade secrets of Broadcom, and you shall use
*    all reasonable efforts to protect the confidentiality thereof, and to
*    use this information only in connection with your use of Broadcom
*    integrated circuit products.
* 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
*    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
*    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
*    RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
*    IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS
*    FOR A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS,
*    QUIET ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU
*    ASSUME THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
* 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
*    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
*    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
*    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
*    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
*    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
*****************************************************************************/





/*
*
****************************************************************************
*
*    Filename: secureStream.h
*
****************************************************************************
*    Description:
*
*      Secure Media APIs for SRTP (RFC-3711)
*    
*****************************************************************************/


#ifndef SECURE_MEDIA_API_H
#define SECURE_MEDIA_API_H

/* ---- Include Files ---------------------------------------------------- */
/* ---- Contants and Types ----------------------------------------------- */
#ifndef NULL   
#define NULL 0
#endif 

#ifndef IN
#define IN
#endif 

#ifndef OUT
#define OUT
#endif 

#ifndef INOUT
#define INOUT
#endif 

#ifdef __cplusplus
extern "C"
{
#endif

/* external Callback function for Auth/Cipher transfrom */

/* Typedef of SMAPI_EXTERNALAUTHTRANSFROM */
typedef struct SMAPI_EXTERNALAUTHTRANSFROM SMAPI_EXTERNALAUTHTRANSFROM_t;

/* Authentication Function Table Prototype */
typedef int (AuthTransformAllocFunction)(OUT SMAPI_EXTERNALAUTHTRANSFROM_t ** ppContext);
typedef int (AuthTransformDeallocFunction)(IN SMAPI_EXTERNALAUTHTRANSFROM_t * pContext);
typedef int (AuthTransformInitFunction)(IN void* pContextState, IN const unsigned char *key, IN int key_len);
typedef int (AuthTransformStartFunction)(IN void* pContextState);
typedef int (AuthTransformUpdateFunction)(IN void* pContextState, IN const unsigned char *buffer, IN int octets_to_auth);
typedef int (AuthTransformFinalFunction)(IN void* pContextState, IN const unsigned char *buffer, IN int octets_to_auth, 
   OUT unsigned char * auth_tag, OUT int auth_tag_len);

/* The External Authentication Transformat Object 
 * Typedef definition is already defined in above section */
struct SMAPI_EXTERNALAUTHTRANSFROM
{
   void* pContextState;                   /* the context state of the external auth object */
   AuthTransformAllocFunction             *AuthTransformAlloc;   /* the alloc func */
   AuthTransformDeallocFunction           *AuthTransformDealloc; /* the dealloc func */
   AuthTransformInitFunction              *AuthTransformInit;    /* the init func */
   AuthTransformStartFunction             *AuthTransformStart;   /* the start func */
   AuthTransformUpdateFunction            *AuthTransformUpdate;  /* the update func */
   AuthTransformFinalFunction             *AuthTransformFinal;   /* the final func */
};

/* Typedef of SMAPI_EXTERNALCIPHERTRANSFROM */
typedef struct SMAPI_EXTERNALCIPHERTRANSFROM SMAPI_EXTERNALCIPHERTRANSFROM_t;

/* Cipher Function Table Prototype */
typedef int (CipherTransformAllocFunction)(OUT SMAPI_EXTERNALCIPHERTRANSFROM_t **ppContext);
typedef int (CipherTransformDeallocFunction)(IN SMAPI_EXTERNALCIPHERTRANSFROM_t* pContext);
typedef int (CipherTransformInitFunction)(IN void* pContextState, IN const unsigned char *key, IN int key_len);
typedef int (CipherTransformEncryptFunction)(IN void* pContextState, IN unsigned char *buffer, IN int octets_to_encrypt, 
   OUT unsigned char *out_buffer, INOUT int *out_buffer_len);
typedef int (CipherTransformDecryptFunction)(IN void* pContextState, IN unsigned char *buffer, IN int octets_to_decrypt, 
   OUT unsigned char *out_buffer, INOUT int *out_buffer_len);
typedef int (CipherTransformSetIVFunction)(IN void* pContextState, IN unsigned char * pIV_buffer, IN int IV_len);

/* The External Cipher Transformat Object 
 * Typedef definition is already defined in above section */
struct SMAPI_EXTERNALCIPHERTRANSFROM
{
   void* pContextState;                     /* the context state of the external auth object */
   CipherTransformAllocFunction             *CipherTransformAlloc;    /* the alloc func */
   CipherTransformDeallocFunction           *CipherTransformDealloc;  /* the dealloc func */
   CipherTransformInitFunction              *CipherTransformInit;     /* the init func */
   CipherTransformEncryptFunction           *CipherTransformEncrypt;  /* the encrypt func */
   CipherTransformDecryptFunction           *CipherTransformDecrypt;  /* the decrypt func */
   CipherTransformSetIVFunction             *CipherTransformSetIV;    /* the set IV func */
};

typedef enum SMAPI_COMPLIANCE
{
   SMAPI_RFC_COMPLIANT = 0,                      /* RFC compliant */
   SMAPI_RTCP_AUTH_NULL_COMPLIANCE = 0x00000001  /* allow RTCP AUTH_NULL to be used */
} SMAPI_COMPLIANCE;

typedef enum SMAPI_ERRORCODE
{
   SMAPI_ERROR_PACKET_TOO_BIG = -116,  /* RFC3711 Sect 4.1.1 SRTP can only support RTP/RTCP packet up to 2^23 bits */
      SMAPI_ERROR_KEY_EXPIRED = -115,  /* current key has expired; go renew key; ex. reach index limit */
      SMAPI_ERROR_REPLAY_FAIL = -114,  /* replay check failed; send/receive a previously sent/received packet */
      SMAPI_ERROR_AUTH_FAIL = -113,    /* authentication failure in packet when unprotecting */
      SMAPI_ERROR_INTERNAL_INCONSISTENCY = -112,           /* internal system error, inconsistence states */
      SMAPI_ERROR_PARAMETER_NOT_VALID = -111,              /* parameter is not valid in operation */
      SMAPI_ERROR_POLICY_NOT_READY = -110,                 /* policy specified is not ready for operation */
      SMAPI_ERROR_DESCRIPTOR_NOT_COMPLETED = -109,         /* the security descriptor is not completed */
      SMAPI_ERROR_OUTPUT_BUFFER_TOO_SMALL = -108,          /* the output buffer is too small for the operation */
      SMAPI_ERROR_NO_POLICY_FOUND = -107,                  /* policy specified is not found */
      SMAPI_ERROR_REMOVE_POLICY_FAIL = -106,               /* legacy error code, error when removing a policy */
      SMAPI_ERROR_TEMPLATE_POLICY_CANNOT_BE_REMOVED = -105, /* legacy error code, template policy used to be unremovable */
      SMAPI_ERROR_UNAVAILABLE_AUTH_TYPE = -104,            /* auth type is unavailable for descriptor */
      SMAPI_ERROR_UNAVAILABLE_CIPHER_TYPE = -103,          /* cipher type is unavailable for descriptor */
      SMAPI_ERROR_CREATE_POLICY_FAIL = -102,               /* error when creating a policy */
      SMAPI_ERROR_POLICY_EXIST_CANNOT_ADD_POLICY = -101,   /* adding a SSRC policy that already have sexisted */
      SMAPI_CONTEXT_NOT_FOUND = -100,                      /* context is not found */
      SMAPI_LIBRARY_NOT_INIT = -10,                        /* library is not initialized */
      SMAPI_ERROR_NOT_SUPPORTED = -5,                      /* operation is not supported */
      SMAPI_ERROR_NOT_IMPLEMENTED = -4,                    /* function not implemented */
      SMAPI_ERROR_INVALID_ARGUMENT = -3,                   /* invalid argument for function call */
      SMAPI_OUT_OF_MEMORY = -2,                            /* out of memory for memory allocation */
      SMAPI_FATAL_ERROR = -1,                              /* internal system error, error unknown */
      SMAPI_NO_ERROR = 0,                                  /* No error */
      SMAPI_NO_ERROR_TRUE = 1,                             /* No error, indication operation is true */
      SMAPI_NO_ERROR_FALSE = 2,                            /* No error, indication operation is false */
      SMAPI_NO_ERROR_A_NEW_POLICY_ADDED = 3                /* No error, a new policy has been added by on-demand from template */
} SMAPI_ERRORCODE;

typedef enum SMAPI_POLICYSTATUS
{
   SMAPI_NOT_READY,              /* Polciy is not ready; ex. secure descriptor is not completed */
      SMAPI_DISABLED,            /* Polciy is disabled by user/system */
      SMAPI_READY                /* Polciy is ready for use */
} SMAPI_POLICYSTATUS;

typedef enum SMAPI_TRANSFORMTYPE
{
   SMAPI_UNKNOWN = -1,           /* basic type identifier */
      SMAPI_CIPHER = 0,          /* basic type identifier */
      SMAPI_AUTH = 1,            /* basic type identifier */
      SMAPI_AUTH_NULL = 100,     /* no authetication, supported */
      SMAPI_AUTH_HMAC_SHA1,      /* For srtp, this auth support only 160 bits key, supported */
      SMAPI_CIPHER_NULL = 200,   /* no encryption, supported */
      SMAPI_CIPHER_AES_ICM_128,  /* For srtp, this cipher support only 128 bits key, supported */
      SMAPI_CIPHER_AES_F8,       /* for srtp, not supported (in RFC 3711, Optionally, and we have no implementation) */
} SMAPI_TRANSFORMTYPE;

typedef enum SMAPI_POLICYTYPE
{
   SMAPI_SSRC,
      SMAPI_ANY_INBOUND,      /* inbound template (network to us) */
      SMAPI_ANY_OUTBOUND      /* outbound template (us to network) */
} SMAPI_POLICYTYPE;

typedef enum SMAPI_PROTOCOLTYPE
{
   SMAPI_RTP,               /* RTP part of Media stream */
      SMAPI_RTCP               /* RTCP part of Media stream */
} SMAPI_PROTOCOLTYPE;

typedef enum SMAPI_DIRECTIONTYPE
{
   SMAPI_DIR_UNKNOWN,         /* policy is created, but direction is unknow
                                 as it has not yet send or recv any packet */
   SMAPI_INBOUND,             /* inbound policy - for receiving SRTP/SRTCP packet */
   SMAPI_OUTBOUND             /* outbound policy - for sending SRTP/SRTCP packet  */
} SMAPI_DIRECTIONTYPE;

/* Context Handle  */
typedef struct SMAPI_CONTEXTOBJ *SMAPI_CTXHANDLE;
/* RTP Decriptor Handle */
typedef struct SMAPI_RTP_DESCRIPTOR *PSMAPI_RTP_DESCRIPTOR_t;
/* Stream State Handle */
typedef struct SMAPI_STREAM_STATE *PSMAPI_STREAM_STATE_t;

/* ---- Variable Externs -------------------------------------------------- */
/* ---- Function Prototypes ----------------------------------------------- */
/*
** Functions for Handling RTP Descriptor Structures
*/
SMAPI_ERRORCODE SMAPI_AllocRTPDescriptor(PSMAPI_RTP_DESCRIPTOR_t *ppDescriptor);
SMAPI_ERRORCODE SMAPI_FreeRTPDescriptor(PSMAPI_RTP_DESCRIPTOR_t pDescriptor);
SMAPI_ERRORCODE SMAPI_CloneRTPDescriptor(PSMAPI_RTP_DESCRIPTOR_t destDescriptor, PSMAPI_RTP_DESCRIPTOR_t sourceDescriptor);
SMAPI_ERRORCODE SMAPI_GetSrtpPacketOverheadSize(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, 
   IN SMAPI_PROTOCOLTYPE protocol, OUT int *pOverheadSize);
SMAPI_ERRORCODE SMAPI_setMasterKey(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, IN unsigned char* pMasterKey, IN int keylen);
SMAPI_ERRORCODE SMAPI_getMasterKey(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, OUT unsigned char* pMasterKey, INOUT int* pKeylen);
SMAPI_ERRORCODE SMAPI_setMasterKeyLayout(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, IN int* pLayoutArray, IN int layoutArraySize);
SMAPI_ERRORCODE SMAPI_getMasterKeyLayout(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, OUT int* pLayoutArray, INOUT int* pLayoutArraySize);
SMAPI_ERRORCODE SMAPI_setTransform(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor,
   IN SMAPI_PROTOCOLTYPE protocol, IN SMAPI_TRANSFORMTYPE transform,
   IN SMAPI_TRANSFORMTYPE transformType);
SMAPI_ERRORCODE SMAPI_getTransform(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor,
   IN SMAPI_PROTOCOLTYPE protocol, IN SMAPI_TRANSFORMTYPE transform,
   OUT SMAPI_TRANSFORMTYPE * pTransformType);
SMAPI_ERRORCODE SMAPI_setAuthTagLen(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor,
   IN SMAPI_PROTOCOLTYPE protocol,
   IN int authlen);
SMAPI_ERRORCODE SMAPI_getAuthTagLen(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor,
   IN SMAPI_PROTOCOLTYPE protocol,
   OUT int * pAuthlen);
SMAPI_ERRORCODE SMAPI_setKeyGenerationFunctionPtr(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, IN SMAPI_PROTOCOLTYPE protocol,
   IN SMAPI_TRANSFORMTYPE transform, IN void* KeyGenerationFunctionPtr);
SMAPI_ERRORCODE SMAPI_getKeyGenerationFunctionPtr(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, IN SMAPI_PROTOCOLTYPE protocol,
   IN SMAPI_TRANSFORMTYPE transform, OUT void**KeyGenerationFunctionPtr);
SMAPI_ERRORCODE SMAPI_setKeyLifeTime(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, IN SMAPI_PROTOCOLTYPE protocol,
   IN SMAPI_TRANSFORMTYPE transform, IN int keyLifetime);
SMAPI_ERRORCODE SMAPI_getKeyLifeTime(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, IN SMAPI_PROTOCOLTYPE protocol,
   IN SMAPI_TRANSFORMTYPE transform, OUT int* keyLifetime);
SMAPI_ERRORCODE SMAPI_setLifetimeCallbackFunctionPtr(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, IN SMAPI_PROTOCOLTYPE protocol,
   IN SMAPI_TRANSFORMTYPE transform, IN void* LifetimeCallbackFunctionPtr);
SMAPI_ERRORCODE SMAPI_getLifetimeCallbackFunctionPtr(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, IN SMAPI_PROTOCOLTYPE protocol,
   IN SMAPI_TRANSFORMTYPE transform, OUT void** LifetimeCallbackFunctionPtr);
SMAPI_ERRORCODE SMAPI_setExternalAuthFunction(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor,
   IN SMAPI_PROTOCOLTYPE protocol,
   IN AuthTransformAllocFunction *pFunc);
SMAPI_ERRORCODE SMAPI_getExternalAuthFunction(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor,
   IN SMAPI_PROTOCOLTYPE protocol,
   OUT AuthTransformAllocFunction **ppFunc);
SMAPI_ERRORCODE SMAPI_setExternalCipherFunction(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor,
   IN SMAPI_PROTOCOLTYPE protocol,
   IN CipherTransformAllocFunction *pFunc);
SMAPI_ERRORCODE SMAPI_getExternalCipherFunction(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor,
   IN SMAPI_PROTOCOLTYPE protocol,
   OUT CipherTransformAllocFunction **ppFunc);
   
#define SMAPI_setRecvIndex SMAPI_setSRTPIndex
SMAPI_ERRORCODE SMAPI_setSRTPIndex(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, 
   IN unsigned int ROC, IN unsigned short int SEQ);
#define SMAPI_getRecvIndex SMAPI_getSRTPIndex   
SMAPI_ERRORCODE SMAPI_getSRTPIndex(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor, 
   OUT unsigned int *pROC, OUT unsigned short int *pSEQ);   
SMAPI_ERRORCODE SMAPI_setSRTCPIndex(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor,
   IN unsigned int index);
SMAPI_ERRORCODE SMAPI_getSRTCPIndex(IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor,
   OUT unsigned int *pIndex);   

/*
** Functions for Handling Stream State Structures
*/   
SMAPI_ERRORCODE SMAPI_AllocStreamState(PSMAPI_STREAM_STATE_t *ppState);
SMAPI_ERRORCODE SMAPI_FreeStreamState(PSMAPI_STREAM_STATE_t pState);
SMAPI_ERRORCODE SMAPI_getCurRTPIndex(IN PSMAPI_STREAM_STATE_t pState, 
   OUT unsigned int *pROC, OUT unsigned short int *pSEQ);   
SMAPI_ERRORCODE SMAPI_getCurRTCPIndex(IN PSMAPI_STREAM_STATE_t pState, 
   OUT unsigned int *pIndex);   
SMAPI_ERRORCODE SMAPI_getDirection(IN PSMAPI_STREAM_STATE_t pState, 
   OUT SMAPI_DIRECTIONTYPE *pDirection);     
SMAPI_ERRORCODE SMAPI_getSSRC(IN PSMAPI_STREAM_STATE_t pState, 
   OUT unsigned int *pSSRC);      

/***************************************************************************
** Main API functions 
***************************************************************************/

/*
*****************************************************************************
** FUNCTION:   SMAPI_GetLibraryVersion
**
** PURPOSE:    Provide Library Version
**
** PARAMETERS: pSMAPIVersionText - library version in text
**             SMAPIVersionTextLen - the size of the pSMAPIVersionText buffer
**             pSMAPIVersionNum - library version in hexidecimal 
**             pSMAPIBuildDateTime - library build date and time in text
**             SMAPIBuildDateTimeLen - the size of the pSMAPIBuildDateTime buffer
**
** RETURNS:    status:
** NOTES:      pSMAPIVersionText and pSMAPIBuildDateTime will not be longer than 64 octets
**             bosMutexInit() must be called once and only once at the start of the application 
**
*****************************************************************************
*/

SMAPI_ERRORCODE SMAPI_GetLibraryVersion(OUT char * pSMAPIVersionText, OUT int SMAPIVersionTextLen, 
   OUT unsigned int *pSMAPIVersionNum, OUT char * pSMAPIBuildDateTime, OUT int SMAPIBuildDateTimeLen);

/*
*****************************************************************************
** FUNCTION:   SMAPI_LibraryInit
**
** PURPOSE:    Library Initialization
**
** PARAMETERS: none
**
** RETURNS:    status:
** NOTES:       bosMutexInit() must be called once and only once at the start of the application 
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_LibraryInit(void);

/*
*****************************************************************************
** FUNCTION:   SMAPI_LibraryDeinit
**
** PURPOSE:    Library Deinitialization
**
** PARAMETERS: none
**
** RETURNS:    none
** NOTES:       bosMutexTerm() must be called once and only once at the end of application
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_LibraryDeinit(void);



/*
*****************************************************************************
** FUNCTION:   SMAPI_CreateContext
**             SMAPI_CreateContext2
**
** PURPOSE:    create an empty secure context
**
** PARAMETERS: pCxid - pointer to a context handle
**             compliance - bit field that indicates extension supports
**
** RETURNS:    none
**
** NOTES:      SMAPI_CreateContext call SMAPI_CreateContext2 with SMAPI_RFC_COMPLIANT
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_CreateContext(OUT SMAPI_CTXHANDLE* pCxid);
SMAPI_ERRORCODE SMAPI_CreateContext2(OUT SMAPI_CTXHANDLE* pCxid, SMAPI_COMPLIANCE compliance);

/*
*****************************************************************************
** FUNCTION:   SMAPI_DeleteContext
**
** PURPOSE:    cleanup
**
** PARAMETERS: cxid - context handle
**
** RETURNS:    none
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_DeleteContext(IN SMAPI_CTXHANDLE cxid);

/*
*****************************************************************************
** FUNCTION:   SMAPI_InitializeDescriptor(OUT PSMAPI_POLICY_t pPolicy)
**
** PURPOSE:    Initialize a stream policy structure with default secure policy
**
** PARAMETERS: pDescriptor  - pointer to the secure descriptor strucutre to be set 
**                        using the default descriptor.
**
** RETURNS:    none
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_InitializeDescriptor(OUT PSMAPI_RTP_DESCRIPTOR_t pDescriptor);

/*
*****************************************************************************
** FUNCTION:   SMAPI_CreatePolicy
**
** PURPOSE:    Initialize stream with secure policy
**
** PARAMETERS: cxid         - context handle
**             policyType   - policy type
**             ssrc         - ssrc if it is policy type SMAPI_SSRC 
**             pDescriptor  - pointer to secure descriptor parameters
**
** RETURNS:    none
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_CreatePolicy(IN SMAPI_CTXHANDLE cxid, IN SMAPI_POLICYTYPE policyType, unsigned int ssrc, 
   IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor);


/*
*****************************************************************************
** FUNCTION:   SMAPI_RemovePolicy
**
** PURPOSE:    Remove secure Policy from the context
**
** PARAMETERS: cxid     - context id
**             policyType   - policy type
**             ssrc         - ssrc if it is policy type SMAPI_SSRC 
**
** RETURNS:    none
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_RemovePolicy(IN SMAPI_CTXHANDLE cxid, IN SMAPI_POLICYTYPE policyType, unsigned int ssrc);

/*
*****************************************************************************
** FUNCTION:   SMAPI_ModifyPolicy
**
** PURPOSE:    set security parameters for policy
**
** PARAMETERS: cxid     - context id
**             policyType - policy type
**             ssrc       - SMAPI_SSRC if policy type is SMAPI_SSRC
**             modifyChildPolicy - flag to indicate that if all child policy dervied from this policy should also be
**                                 modified.  (only for template policy, etc policyType != SMAPI_SSRC)
**             pDescriptor  - pointer to the new secure descriptor parameters
**
** RETURNS:    NO_ERROR if success, else check with SMAPI_ERRORCODE
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_ModifyPolicy(IN SMAPI_CTXHANDLE cxid, IN SMAPI_POLICYTYPE policyType, unsigned int ssrc, 
   IN int modifyChildPolicy, IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor);
   
/*
*****************************************************************************
** FUNCTION:   SMAPI_ModifyPolicyRealTime
**
** PURPOSE:    set security parameters for policy "Real Time".  The current 
**             SRTP ROC, SRTP SEQ, SRTCP Index are maintained.
**
** PARAMETERS: cxid     - context id
**             policyType - policy type
**             ssrc       - SMAPI_SSRC if policy type is SMAPI_SSRC
**             modifyChildPolicy - flag to indicate that if all child policy dervied from this policy should also be
**                                 modified.  (only for template policy, etc policyType != SMAPI_SSRC)
**             pDescriptor  - pointer to the new secure descriptor parameters
**
** RETURNS:    NO_ERROR if success, else check with SMAPI_ERRORCODE
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_ModifyPolicyRealTime(IN SMAPI_CTXHANDLE cxid, IN SMAPI_POLICYTYPE policyType, unsigned int ssrc, 
   IN int modifyChildPolicy, IN PSMAPI_RTP_DESCRIPTOR_t pDescriptor);

/*
*****************************************************************************
** FUNCTION:   SMAPI_ProtectRTP
**
** PURPOSE:    protect RTP Packet
**
** PARAMETERS: cxid      - context id
**             pRTPPacket - pointer to RTP packet
**             len      - length of RTP packet
**             pOutBuffer - pointer to RTP packet after protection (see NOTES)
**             pOutLen  - IN: length of the pOutBuffer lenght / OUT: length of RTP packet after protection
**
** RETURNS:    NO_ERROR if success, 
**             NO_ERROR_A_NEW_POLICY_ADDED if success and a new policy is added based on a template policy
**             else check with SMAPI_ERRORCODE
**             ERROR_OUTPUT_BUFFER_TOO_SMALL if the length pOutBuffer is too small, then pOutLen will hold the required length
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_ProtectRTP(IN SMAPI_CTXHANDLE cxid, IN unsigned char* pRTPPacket, IN int len, 
   OUT unsigned char* pOutBuffer, INOUT int * pOutLen);

/*
*****************************************************************************
** FUNCTION:   SMAPI_ProtectRTCP
**
** PURPOSE:    protect RTCP Packet
**
** PARAMETERS: cxid      - connection id
**             pRTCPPacket - pointer to RTCP packet
**             len      - length of RTCP packet
**             pOutBuffer - pointer to RTCP packet after protection (see NOTES)
**             pOutLen  - IN: length of the pOutBuffer lenght / OUT: length of RTCP packet after protection
**
** RETURNS:    NO_ERROR if success, 
**             NO_ERROR_A_NEW_POLICY_ADDED if success and a new policy is added based on a template policy
**             else check with SMAPI_ERRORCODE
**             ERROR_OUTPUT_BUFFER_TOO_SMALL if the length pOutBuffer is too small, then pOutLen will hold the required length
**
******************************************************************************
*/
SMAPI_ERRORCODE SMAPI_ProtectRTCP(IN SMAPI_CTXHANDLE cxid, IN unsigned char* pRTCPPacket, IN int len, 
   OUT unsigned char*pOutBuffer, INOUT int * pOutLen);

/*
*****************************************************************************
** FUNCTION:   SMAPI_UnprotectRTP
**
** PURPOSE:    unprotect Protected RTP packet into normal RTP packet
**
** PARAMETERS: cxid      - connection id
**             pPacket   - pointer to protected RTP packet
**             len      - length of RTP packet
**             pOutBuffer - pointer to RTP packet after protection is removed   (see NOTES)
**             pOutLen  - IN: length of the pOutBuffer lenght / OUT: length of RTP packet after protection is removed
**
** RETURNS:    NO_ERROR if success, 
**             NO_ERROR_A_NEW_POLICY_ADDED if success and a new policy is added based on a template policy
**             else check with SMAPI_ERRORCODE
**             ERROR_OUTPUT_BUFFER_TOO_SMALL if the length pOutBuffer is too small, then pOutLen will hold the required length
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_UnprotectRTP(IN SMAPI_CTXHANDLE cxid, IN unsigned char* pPacket, IN int len, 
   OUT unsigned char* pOutBuffer, INOUT int * pOutLen);


/*
*****************************************************************************
** FUNCTION:   SMAPI_UnprotectRTCP
**
** PURPOSE:    unprotect Protected RTCP packet into normal RTCP packet
**
** PARAMETERS: cxid      - connection id
**             pPacket   - pointer to protected RTCP packet 
**             len      - length of RTCP packet
**             pOutBuffer - pointer to RTCP packet after protection is removed (see NOTES)
**             pOutLen  - IN: length of the pOutBuffer lenght / OUT: length of RTCP packet after protection is removed
**
** RETURNS:    NO_ERROR if success, 
**             NO_ERROR_A_NEW_POLICY_ADDED if success and a new policy is added based on a template policy
**             else check with SMAPI_ERRORCODE
**             ERROR_OUTPUT_BUFFER_TOO_SMALL if the length pOutBuffer is too small, then pOutLen will hold the required length
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_UnprotectRTCP(IN SMAPI_CTXHANDLE cxid, IN unsigned char* pPacket, IN int len, 
   OUT unsigned char* pOutBuffer, INOUT int * pOutLen);


/*
*****************************************************************************
** FUNCTION:   SMAPI_GetDescriptor
**
** PURPOSE:    get security parameters for policy specified
**
** PARAMETERS: cxid     - context id
**             policyType - policy type
**             ssrc       - SMAPI_SSRC if policy type is SMAPI_SSRC
**             pDescriptor  - pointer to the secure descriptor parameters
**
** RETURNS:    NO_ERROR if success, 
**               else check with SMAPI_ERRORCODE
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_GetDescriptor(IN SMAPI_CTXHANDLE cxid, IN SMAPI_POLICYTYPE policyType, unsigned int ssrc,
   OUT PSMAPI_RTP_DESCRIPTOR_t *pDescriptor);

/*
*****************************************************************************
** FUNCTION:   SMAPI_SetPolicyStatus
**
** PURPOSE:    enable or disable a policy in context, cannot set policy status if it is currently NOT_READY
**
** PARAMETERS: cxid     - context id
**             policyType - policy type
**             ssrc       - SMAPI_SSRC if policy type is SMAPI_SSRC
**             newStatus  - the new policy status
**
** RETURNS:    NO_ERROR if success, 
**             ERROR_POLICY_NOT_READY, if policy is not ready
**               else check with SMAPI_ERRORCODE
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_SetPolicyStatus(IN SMAPI_CTXHANDLE cxid, IN SMAPI_POLICYTYPE policyType, unsigned int ssrc,
   IN SMAPI_POLICYSTATUS newStatus);

/*
*****************************************************************************
** FUNCTION:   SMAPI_GetPolicyStatus
**
** PURPOSE:    get policy status of specified policy
**
** PARAMETERS: cxid     - context id
**             policyType - policy type
**             ssrc       - SMAPI_SSRC if policy type is SMAPI_SSRC
**             pStatus  - the current policy status
**
** RETURNS:    NO_ERROR if success, 
**               else check with SMAPI_ERRORCODE
**
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_GetPolicyStatus(IN SMAPI_CTXHANDLE cxid, IN SMAPI_POLICYTYPE policyType, unsigned int ssrc,
   IN SMAPI_POLICYSTATUS *pStatus);

/*
*****************************************************************************
** FUNCTION:   SMAPI_GetStreamState
**
** PURPOSE:    get the state of an specified policy, currently only supports
**             SSRC, direction, last ROC, and last SEQ.
**
** PARAMETERS: cxid     - context id
**             policyType - policy type
**             ssrc       - SMAPI_SSRC if policy type is SMAPI_SSRC
**             pStateArray   - an array of the state descriptor
**             pStateArraySize - IN: size of the array an array 
**                               OUT: number of returned State in pStateArray
**                                    matching the policyType specified
**
** RETURNS:    NO_ERROR if success
**             ERROR_OUTPUT_BUFFER_TOO_SMALL if pStateArray is too small, 
**                then pStateArraySize will hold the required size
**             else check with SMAPI_ERRORCODE
**
** NOTE:       Function may returns multiple state descriptors when policyType
**             is SMAPI_ANY_INBOUND or SMAPI_ANY_OUTBOUND.  It returns one
**             state descriptors when policyType is SMAPI_SSRC.
**            
*****************************************************************************
*/
SMAPI_ERRORCODE SMAPI_GetStreamState(IN SMAPI_CTXHANDLE cxid, 
   IN SMAPI_POLICYTYPE policyType, unsigned int ssrc,
   OUT PSMAPI_STREAM_STATE_t *pStateArray, INOUT int *pStateArraySize);
   
#ifdef __cplusplus
}
#endif

#endif /* SECURE_MEDIA_API_H */

