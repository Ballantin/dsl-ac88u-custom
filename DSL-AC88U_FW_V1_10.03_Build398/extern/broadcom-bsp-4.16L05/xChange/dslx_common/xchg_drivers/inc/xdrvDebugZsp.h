/***************************************************************************
*    Copyright 2005  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: zspContext.h
*
****************************************************************************
*    Description:
*
*
****************************************************************************/

#ifndef ZSP_CONTEXT
#define ZSP_CONTEXT

#ifdef __cplusplus
extern "C" {
#endif

#define ZSP_DEBUG_DOING_FINE     0x2345

/*
** Defines the structure of the save register context
** saved upon ASSERT condition or getreg queries.
*/
typedef struct
{
   XDRV_UINT16 ireq;
   XDRV_UINT16 imask;
   XDRV_UINT16 cb0_end;
   XDRV_UINT16 cb1_end;
   XDRV_UINT16 cb0_beg;
   XDRV_UINT16 cb1_beg;
   XDRV_UINT16 vitr;
   XDRV_UINT16 smode;
   XDRV_UINT16 loop2;
   XDRV_UINT16 loop3;
   XDRV_UINT16 loop0;
   XDRV_UINT16 loop1;
   XDRV_UINT16 fmode;
   XDRV_UINT16 guard;
   XDRV_UINT16 r10;
   XDRV_UINT16 r11;
   XDRV_UINT16 r8;
   XDRV_UINT16 r9;
   XDRV_UINT16 r6;
   XDRV_UINT16 r7;
   XDRV_UINT16 r4;
   XDRV_UINT16 r5;
   XDRV_UINT16 r2;
   XDRV_UINT16 r3;
   XDRV_UINT16 rpc;
   XDRV_UINT16 tpc;
   XDRV_UINT16 r0;
   XDRV_UINT16 r1;
   XDRV_UINT16 hwflag;
   XDRV_UINT16 r15;
   XDRV_UINT16 r14;
   XDRV_UINT16 r13;
   XDRV_UINT16 reserved;

} ZSP_CONTEXT_REG;


#ifdef __cplusplus
}
#endif

#endif  /* ZSP_CONTEXT_H  */
