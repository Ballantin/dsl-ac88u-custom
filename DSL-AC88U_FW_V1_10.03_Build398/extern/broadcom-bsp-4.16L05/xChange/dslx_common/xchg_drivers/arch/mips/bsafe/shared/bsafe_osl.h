/*
 * Bsafe uHSM OSL definitions.
 * bsafe OS Independent Layer 
 * any os specific calls should be in bsafe_<os>.c  and defined here as macros
 *
 * $Copyright Open Broadcom$      
 * $Id: bsafe_osl.h,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#ifndef __BSAFE_OSL_H_
#define __BSAFE_OSL_H_


#ifdef LINUX
#error "not yet implemented "
#else

typedef unsigned char BYTE;		/* 1 byte */
#ifndef __INCvxTypesOldh
typedef unsigned short UINT16;		/* 2 bytes */
typedef unsigned int UINT32;		/* 4 bytes */
#endif
typedef  int INT;			/* 4 bytes */

typedef unsigned int ADDRESS;		/* 4 bytes */



#define OS_DMA_ALLOC(dev,size) 		malloc(size)
#define OS_DMA_FREE(dev,ptr,size) 	free(ptr)
#define OS_ALLOC 			malloc
#define OS_FREE(ptr,size) 	free(ptr)
#define OS_COPY_FROM_USER	memcpy
#define OS_COPY_TO_USER		memcpy
#define OS_MEMCPY		memcpy
#define OS_MEMSET		memset


/* debuging logging capabilities */
#ifdef DEBUG
#define PRINTF(arg)			printf arg
#else
#define PRINTF(arg)
#endif


#define MAX_TIMEOUT	500000
#define DELAY_US	10
#define DELAY		delay


//#warning "Unknown generic OS"
#endif

#ifdef DEBUG
#define CMD_PRINTF(type,var)		PRINTF (("%s [",#var);printf(type,var);printf("]\n"));
#define KEY_PRINTF			PRINTF
#define D_PRINTF			PRINTF
#else
#define CMD_PRINTF(type,var)
#define KEY_PRINTF(arg)			
#define D_PRINTF(arg)			
#endif

/* some utils */
#ifdef LE_ENDIAN
#define SWAP2BYTES(sval) ((((sval)&0xff00)>>8)+(((sval)&0x00ff)<<8))
#define SWAP4BYTES(lval) (((SWAP2BYTES((lval)>>16)))+((SWAP2BYTES((lval)&0xffff)<<16)))
#else
#define SWAP2BYTES(sval) sval
#define SWAP4BYTES(lval) lval
#endif


#endif /* __BSAFE_OSL_H_ */
