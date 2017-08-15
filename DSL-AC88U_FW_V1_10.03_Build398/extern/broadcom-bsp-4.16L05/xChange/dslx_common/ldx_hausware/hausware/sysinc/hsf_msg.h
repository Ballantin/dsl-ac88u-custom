/***************************************************************************
*    Copyright 2000  Broadcom
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom
*             16215 Alton Parkway
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom
*    company private, proprietary, and trade secret.
*
****************************************************************************/
#ifndef HSF_MSG_H                        /* support nested includes */
#define HSF_MSG_H

#include <hapi_opcode.h>

/*************************************************************************
**             HSF Messaging interface structures                      **
*************************************************************************/

typedef struct
{
   /*
   ** Message header contains following information:
   **
   **   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
   ** +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   ** |Reserved |    |    |    |    |    |    |Std | Evt|    |    |    |    |    |    |
   ** +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   ** |         |     Service ID              |  1 | 1/0| Message opcode(srv specific)|
   ** +---------+-----------------------------+---------------------------------------+
   **
   **  Std:  1 = Standard Fixed size message (op1 and op2 and values)
   **        0 = Extended data message - see HSFMSGEXT(op1 = length, op2 = *bufp )
   **  Evt:  1 = Asynchronous Event
   **        0 = Command/Response to a command
   */
   HAPI_UINT16 header;
   HAPI_UINT16 op1;     /* fixed values used instead of variable size buffer */
   HAPI_UINT16 op2;     /* fixed values used instead of variable size buffer */
} HSFMSGSTD;

typedef struct
{
   /*
   ** Message header contains following information:
   **
   **   15   14   13   12   11   10    9    8    7    6    5    4    3    2    1    0
   ** +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   ** |Reserved |    |    |    |    |    |    |Std | Evt|    |    |    |    |    |    |
   ** +----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+----+
   ** |         |     Service ID              |  0 | 1/0| Message opcode(srv specific)|
   ** +---------+-----------------------------+---------------------------------------+
   **
   **  Std:  1 = Standard Fixed size message (op1 and op2 and values)
   **        0 = Extended data message - see HSFMSGEXT(op1 = length, op2 = *bufp )
   **  Evt:  1 = Asynchronous Event
   **        0 = Command/Response to a command
   */
   HAPI_UINT16 header;
   HAPI_UINT16 length;  /* length(in bytes) of data pointed to by bufp */
   void   *bufp;   /* pointer to a variable length buffer */
} HSFMSGEXT;

typedef union
{
   HSFMSGSTD std;
   HSFMSGEXT ext;

} HSFMSG;

#define HSFSTD_MASK     HAPI_OPCODE_STD_MASK
#define HSFEXT_MASK     HAPI_OPCODE_EXT_MASK
#define HSFSRV_MASK     HAPI_OPCODE_SRVID_MASK
#define HSFEVT_MASK     HAPI_OPCODE_EVT_MASK

#define HSF_IS_STD(header)          (header & HSFSTD_MASK)
#define HSF_IS_EXT(header)          (!(HSF_IS_STD(header)))
#define HSF_IS_EVT(header)          (header & HSFEVT_MASK)

/*
** These macros work in the following types.
** HSFMSG.std.header
** HSZCMD.hsxcmd
** LDX_ExternalSubMsg.header
*/
#define HSFMKSRVID(srvid)           (srvid << 8)
#define HSFGETSRVID(header)         ((HAPISRVID)((header) >> 8))
#define HSFCLRSRVID(header)         (header & (~HSFSRV_MASK))


#endif
