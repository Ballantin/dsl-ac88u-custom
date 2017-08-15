/****************************************************************************
*
*  haus_cmd.h
*
*  PURPOSE:
*     Defines the structures used to define Commands and Events used
*     by the "old style" Hausware model.
*
*     These definitions were extracted from hapigdef.h so that the old and
*     new could coexist at the same time.
*
*  NOTES:
*
*        (c)   Copyright Broadcom Inc. 2000
*              All Rights Reserved
*
***************************************************************************/

#if !defined( HAUS_CMD_H )
#define  HAUS_CMD_H

#if !defined( HAPI_TYPES_H )
#  include <hapi_types.h>
#endif

/*
* A response is generated if hapiIOctl used
* No response generated if hapiAsyncIOctl used
*/
#define  HSCMDDATA               0xff00   /* standard command op1=value1, op2=value2 */
#define  HSCMDEXTDATA            0xfe00   /* extended command op1=len, op2=pointer   */

#define  HSEVTDATA               0xfb00   /* standard event op1=value1, op2=value2 */
#define  HSEVTEXTDATA            0xfa00   /* extended event op1=len, op2=pointer   */

/* obsolete but maintained for legacy. */
#define  HSCMDEXTDATA_ASYNC      HSCMDEXTDATA
#define  HSCMDEXTDATA_SYNC       HSCMDEXTDATA
#define  HSCMDRESP               HSCMDEXTDATA

typedef struct HSZCMD HSZCMD;
typedef struct HSZEVT HSZEVT;

typedef union
{
   HAPI_UINT16  val;                          /* Op2 is a value */
   void*        ref;                          /* Op2 is a pointer */

}  HSUOP2;

struct HSZCMD                         /* HAPI command interface */
{
   HAPI_UINT16 hsxdevhdl;                /* device handle */
   HAPI_UINT16 hsxcmd;                   /* command (refer to hapi_opcode.h) */
   HAPI_UINT16 hsxreqid;                 /* request ID */
   HAPI_UINT16 hsxop1;                   /* operand 1 */
   HSUOP2      hsxop2;                   /* operand 2 */
};

struct HSZEVT                         /* HAPI event interface */
{
   HAPI_UINT16 hsxdevhdl;                /* device handle */
   HAPI_UINT16 hsxevt;                   /* event (refer to hapi_opcode.h) */
   HAPI_UINT16 hsxreqid;                 /* request ID */
   HAPI_UINT16 hsxop1;                   /* operand 1 */
   HSUOP2      hsxop2;                   /* operand 2 */
};

#endif   /* HAUS_CMD_H  */

