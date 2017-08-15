/****************************************************************************
*
* Copyright (c) 2002 Broadcom Corporation, All Rights Reserved.
* Contains proprietary and confidential information.
*
* No portions of this material may be reproduced in any form without the
* written permission of:
*
* Broadcom Corporation
* 16215 Alton Parkway
* P.O. Box 57013
* Irvine, California 92619-7013
*
* All information contained in this document is Broadcom Corporation
* company private, proprietary, and trade secret.
*
*****************************************************************************/
/**
*
*  @file    stdlib.c
*
*  @brief   Wrapper for the C-run time functions in stdlib.c
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <stdlib.h>

/* ---- Public Variables ------------------------------------------------- */

/* ---- Private Constants and Types -------------------------------------- */

#define _BCM_U	0x01	/* upper */
#define _BCM_L	0x02	/* lower */
#define _BCM_D	0x04	/* digit */
#define _BCM_C	0x08	/* cntrl */
#define _BCM_P	0x10	/* punct */
#define _BCM_S	0x20	/* white space (space/lf/tab) */
#define _BCM_X	0x40	/* hex digit */
#define _BCM_SP	0x80	/* hard space (0x20) */ 

#define __ismask(x)     (bcm_ctype[(int)(unsigned char)(x)])
#define bcm_isdigit(c)	((__ismask(c)&(_BCM_D)) != 0) 

/* ---- Private Variables ------------------------------------------------ */

unsigned char bcm_ctype[] = {
	_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,			/* 0-7 */
	_BCM_C,_BCM_C|_BCM_S,_BCM_C|_BCM_S,_BCM_C|_BCM_S,_BCM_C|_BCM_S,_BCM_C|_BCM_S,_BCM_C,_BCM_C,		/* 8-15 */
	_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,			/* 16-23 */
	_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,_BCM_C,			/* 24-31 */
	_BCM_S|_BCM_SP,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,			/* 32-39 */
	_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,			/* 40-47 */
	_BCM_D,_BCM_D,_BCM_D,_BCM_D,_BCM_D,_BCM_D,_BCM_D,_BCM_D,			/* 48-55 */
	_BCM_D,_BCM_D,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,			/* 56-63 */
	_BCM_P,_BCM_U|_BCM_X,_BCM_U|_BCM_X,_BCM_U|_BCM_X,_BCM_U|_BCM_X,_BCM_U|_BCM_X,_BCM_U|_BCM_X,_BCM_U,	/* 64-71 */
	_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,			/* 72-79 */
	_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,			/* 80-87 */
	_BCM_U,_BCM_U,_BCM_U,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,			/* 88-95 */
	_BCM_P,_BCM_L|_BCM_X,_BCM_L|_BCM_X,_BCM_L|_BCM_X,_BCM_L|_BCM_X,_BCM_L|_BCM_X,_BCM_L|_BCM_X,_BCM_L,	/* 96-103 */
	_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,			/* 104-111 */
	_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,			/* 112-119 */
	_BCM_L,_BCM_L,_BCM_L,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_C,			/* 120-127 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 128-143 */
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,		/* 144-159 */
	_BCM_S|_BCM_SP,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,   /* 160-175 */
	_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,_BCM_P,       /* 176-191 */
	_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,       /* 192-207 */
	_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_P,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_U,_BCM_L,       /* 208-223 */
	_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,       /* 224-239 */
	_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_P,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L,_BCM_L        /* 240-255 */
};

/* ---- Functions -------------------------------------------------------- */


/***************************************************************************/
/**
*  Convert string to integer
*
*  @param   nptr  Pointer to a string
*
*  @return  Converted value
*/
int atoi(const char *s)
{
	uint n;

	n = 0;

	while (bcm_isdigit(*s))
		n = (n * 10) + *s++ - '0';
	return (n);
} 

/** @} */
