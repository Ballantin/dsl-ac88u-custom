#ifndef IO_H_INCLUDED
#define IO_H_INCLUDED

/*
 * -----------------------------------------------------------------------------
 * (c) Copyright 2008, SiTel Semiconductor BV
 * All Rights Reserved
 * ------------------------------------------------------------------------------
 * SITEL SEMICONDUCTOR CONFIDENTIAL
 * ------------------------------------------------------------------------------
 * This code includes Confidential, Proprietary Information and is a Trade 
 * Secret of Sitel Semiconductor BV. All use, disclosure, and/or reproduction 
 * is prohibited unless authorized in writing.
 * ------------------------------------------------------------------------------
 *
 * Description: IO and memory map selector
 *
 * ----------------------------------------------------------------------------- 
 * Synchronicity history: 
 * 
 *  $Log: sitel_io.h.rca $
 *  
 *   Revision: 1.3 Tue Feb  3 14:40:53 2009 snijders
 *   Updated some comment.
 *  
 *   Revision: 1.2 Mon Dec 22 15:01:42 2008 snijders
 *   Added official SiTel header.
 *  
 * ----------------------------------------------------------------------------- 
*/

typedef unsigned char      uint8;
typedef char               int8;
typedef unsigned short     uint16;
typedef short              int16;
typedef unsigned long int  uint32;
typedef long int           int32;

#ifndef defineCPP
typedef unsigned char      bool;
#endif

typedef unsigned char      BYTE;
typedef unsigned int       WORD;
typedef unsigned long int  DWORD;
typedef enum {
  FALSE  = 0,
  TRUE   = 1
} BOOLEAN, BOOL;

#ifdef IAR
#define  __far
#define inline	
#endif

#ifdef CR16C_SW_V4
#define  __far
#endif

#ifndef NULL
#define NULL (BYTE*)(-1)
#endif

#include "datasheet.h"

#define AHB_AUTO_SWAP  /* BRCM add */

#endif
