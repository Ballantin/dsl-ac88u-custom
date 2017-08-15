/****************************************************************************
*
*     Copyright (c) 2001 Broadcom
*           All Rights Reserved
*
*     No portions of this material may be reproduced in any form without the
*     written permission of:
*
*           Broadcom
*           16215 Alton Parkway
*           Irvine, California 92619-7013
*
*     All information contained in this document is Broadcom
*     company private, proprietary, and trade secret.
****************************************************************************/
#ifndef EXTENDEDRTS_LDX_H
#define EXTENDEDRTS_LDX_H

#if !defined( MAX )
#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#endif

#if !defined( MIN )
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#endif

#define LDX_MAX(a,b)         (((a) > (b)) ? (a) : (b))
#define LDX_MIN(a,b)         (((a) < (b)) ? (a) : (b))


#define LDX_MAX2(a,b)        (((a) > (b)) ? (a) : (b))
#define LDX_MIN2(a,b)        (((a) < (b)) ? (a) : (b))

#define LDX_MAX3(a,b,c)      LDX_MAX2( (LDX_MAX2((a),(b))), (c) )
#define LDX_MIN3(a,b,c)      LDX_MIN2( (LDX_MIN2((a),(b))), (c) )

#define LDX_MAX4(a,b,c,d)    LDX_MAX2( (LDX_MAX3((a),(b),(c))), (d) )
#define LDX_MIN4(a,b,c,d)    LDX_MIN2( (LDX_MIN3((a),(b),(c))), (d) )

#define LDX_MAX5(a,b,c,d,e)  LDX_MAX2( (LDX_MAX4((a),(b),(c),(d))), (e) )
#define LDX_MIN5(a,b,c,d,e)  LDX_MIN2( (LDX_MIN4((a),(b),(c),(d))), (e) )

#endif /* EXTENDEDRTS_LDX_H */


