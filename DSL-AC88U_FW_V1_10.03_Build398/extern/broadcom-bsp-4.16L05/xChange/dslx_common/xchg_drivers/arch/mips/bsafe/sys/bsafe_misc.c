/*
 * Bsafe uHSM misc functions
 *
 * $Copyright Open Broadcom Corporation$      
 * $Id: bsafe_misc.c,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bsafe_osl.h>
#include <bsafe.h>
#include <bsafe_regs.h>
#include <bsafe_context.h>
#include <bsafe_keymgmt.h>



typedef enum bsafe_copy_modes {
	BSAFE_APP_MODE=0,
	BSAFE_KERNAL_MODE=1,
} bsafe_copy_modes_t;

void (*g_bsafe_copy_from_user)(void *, void *, int) = (void *)OS_COPY_FROM_USER;
void (*g_bsafe_copy_to_user)(void *, void *, int) = (void *) OS_COPY_TO_USER;
void bsafe_swap_dummy(BSAFE_BIGNUM * src, int size);
void (*g_bsafe_swap_bignum)(BSAFE_BIGNUM *, int) = (void *) bsafe_swap_dummy;


void bsafe_set_copy_from_user ( void *ptr);
void bsafe_set_copy_to_user (void  *ptr);
void bsafe_set_swap_bignum(void *ptr);
void bsafe_swap_bignum(BSAFE_BIGNUM * src, int size);
int wordCopyFlip(void *in, void *out, unsigned int size );


void bsafe_set_copy_from_user ( void *ptr)
{
	g_bsafe_copy_from_user = ptr;
}
void bsafe_set_copy_to_user (void  *ptr)
{
	g_bsafe_copy_to_user = ptr;
}
void bsafe_set_swap_bignum(void *ptr)
{
	g_bsafe_swap_bignum = ptr;
}

void bsafe_swap_dummy(BSAFE_BIGNUM * src, int size)
{
   (void) src;
   (void) size;

	return ;
}

void bsafe_swap_bignum(BSAFE_BIGNUM * src, int size)
{
#if 0	/* enable it and get some weird crashes */
#define MYSWAP2BYTES(sval) ((((sval)&0xff00)>>8)+(((sval)&0x00ff)<<8))
#define MYSWAP4BYTES(lval) (((SWAP2BYTES((lval)>>16)))+((SWAP2BYTES((lval)&0xffff)<<16)))
	unsigned int * ptr;
	ptr = (unsigned int *) src;
	while ( size-=BSAFE_BIGNUM_BYTES > 0) {
		*ptr=MYSWAP4BYTES(*ptr);
		ptr++;
	}
#else
	int i=0;
	unsigned char array[4];
	for (i=0;i<size;i +=BSAFE_BIGNUM_BYTES){
		array[0] = src[i];
		array[1] = src[i+1];
		array[2] = src[i+2];
		array[3] = src[i+3];
		src[i] = array[3];
		src[i+1] = array[2];
		src[i+2] = array[1];
		src[i+3] = array[0];
	}

#endif
}
#if 0
void bsafe_swap_bn(unsigned char * src, int size)
{
#if 0
#if 0	/* enable it and get some weird crashes */
#define MYSWAP2BYTES(sval) ((((sval)&0xff00)>>8)+(((sval)&0x00ff)<<8))
#define MYSWAP4BYTES(lval) (((SWAP2BYTES((lval)>>16)))+((SWAP2BYTES((lval)&0xffff)<<16)))
	unsigned int * ptr;
	ptr = (unsigned int *) src;
	while ( size-=BSAFE_BIGNUM_BYTES > 0) {
		*ptr=MYSWAP4BYTES(*ptr);
		ptr++;
	}
#else
	int i=0;
	unsigned int tmp;
	unsigned int *my_src = (unsigned int *) src;
	unsigned int *my_dest = (unsigned int *) (src + size - BSAFE_BIGNUM_BYTES -1);
	for (i=0;i<size/BSAFE_BIGNUM_BYTES;i++){
		tmp = *((unsigned int * ) my_dest);
		my_dest = *((unsigned int *) my_src);
		my_src = *((unsigned int *) tmp);
		my_dest++;
		my_src++;	
	}

#endif
#endif
}
#else
#define byteCopy(src,dest, size) memcpy(dest,src,size)
#define MAX_MOD_BYTES 256
// Alignment structures
typedef struct align_td_S {
  unsigned int word;
} align_S;


int
wordCopyFlip(void *in, void *out, unsigned int size )
{
        unsigned int i;

        align_S *src = (align_S *)((unsigned char *)in + size - 4);
        align_S *dst = (align_S *)out;
        unsigned int  sizeWords = size / sizeof( unsigned int ); /* Note: remainder handled below. */

        for (i = 0; i < sizeWords; i++)
        {
                dst->word = src->word;
                dst++;
                src--;
        }
        byteCopy( in, (unsigned char *)out + sizeWords * sizeof( unsigned int ), size % sizeof( unsigned int ) );
                /* Above byteCopy() call relies on byte copy handling a zero length copy. */

        return 1;
}


/**********************************************************************************
   Method:      bsafe_swap_bn
   Reason:      This function flips words within a chunk of memory.  For more
                        discussion see wordCopyFlip.

   Assumptions: None
**********************************************************************************/

int
bsafe_swap_bn(unsigned char * buffer, int size)
{
        unsigned char temp[ MAX_MOD_BYTES ];     /* 256 bytes */

        if (size > MAX_MOD_BYTES) {
		PRINTF(("bsafe_swap_bn: failed size(%d) > MAX_MOD_BYTES(%d)\n", size, MAX_MOD_BYTES));
                return 0;
	}

        byteCopy(buffer, temp, size);
        wordCopyFlip( temp, buffer, size );

        return 1;
}

#endif

#if 0	/* not used */
ADDRESS bsafe_glb_register_array[MAX_REGS];

#define GETADDR(address) bsafe_glb_register_array[address]

void initialize_bsafe_glb_register_array(ADDRESS baseaddress)
{
	int i=0;

	baseaddress += BSAFE_BASE_OFFSET;
	
	for (i=0; i <= MAX_REGS; i++) {
		bsafe_glb_register_array[i] = baseaddress + (BSAFE_MAX_REG_SIZE*i);
	}
}
#else
#endif






/* simple context fifo */
#define BSAFE_MAX_CONTEXTS	1
bsafe_context_t *g_cmd_context_list[BSAFE_MAX_CONTEXTS];

int bsafe_push_context(bsafe_context_t * context) 
{
	g_cmd_context_list[0] = context;
	return BSAFE_SUCCESS;
}
bsafe_context_t * bsafe_pop_context(void) 
{
	return g_cmd_context_list[0];
}



/* simple key mgmt mechanisum */
#define BSAFE_KEY_USED	1
#define BSAFE_KEY_FREE	0

static unsigned short g_kekloc=0;	/* internal;  not loc g_kekloc + KEK3_LOC */
static unsigned char g_kekloc_used[BSAFE_MAX_KEKLOC];


unsigned short bsafe_get_free_kekloc(int req_size) 
{
#define FIXED_KEK	1
#ifdef FIXED_KEK
	g_kekloc = 0;
	return (g_kekloc + BSAFE_KEK3_LOC);
#endif
	/* loop around the used array */
	do {
		if (g_kekloc_used[g_kekloc] == BSAFE_KEY_FREE)  {
			if (((BSAFE_MAX_KEKLOC - g_kekloc) * BSAFE_KEKLOC_BUKSIZE ) >= (unsigned short) req_size)
            {
				KEY_PRINTF(("get g_kekloc %d [USE]\n",g_kekloc+BSAFE_KEK3_LOC));
            }
			return (g_kekloc + BSAFE_KEK3_LOC);	/* return with offset */
		}
	} while (g_kekloc++ <= BSAFE_MAX_KEKLOC ) ;

	KEY_PRINTF(("get g_kekloc No Free %d \n",BSAFE_INVALID_LOC));
	return BSAFE_INVALID_LOC;	
}
void bsafe_adjust_used_kekloc(unsigned short kekloc, unsigned int ksize)
{
	g_kekloc = kekloc - BSAFE_KEK3_LOC;	/* fix the offset */
	while (ksize-- > 0) {
		KEY_PRINTF(("adjust g_kekloc %d [USED]\n",g_kekloc+BSAFE_KEK3_LOC));
		g_kekloc_used[g_kekloc++] = BSAFE_KEY_USED;
	}
}
void bsafe_free_kekloc(unsigned short kekloc, unsigned int ksize)
{
	g_kekloc = kekloc - BSAFE_KEK3_LOC;	/* fix the offset */
	while (ksize-- > 0) {
		KEY_PRINTF(("free g_kekloc %d [FREE]\n",g_kekloc+BSAFE_KEK3_LOC));
		g_kekloc_used[g_kekloc++] = BSAFE_KEY_FREE;
	}
}


static unsigned short g_akeyloc=0;
static unsigned char g_akeyloc_used[BSAFE_MAX_AKEYLOC];


unsigned short bsafe_get_free_akeyloc() 
{

	/* loop around the used array */
	do {
		if (g_akeyloc_used[g_akeyloc] == BSAFE_KEY_FREE)  {
			KEY_PRINTF(("get g_akeyloc %d [USE]\n",g_akeyloc));
			return g_akeyloc;
		}
	} while (g_akeyloc++ <= BSAFE_MAX_AKEYLOC);

	KEY_PRINTF(("get g_akeyloc No Free %d \n",BSAFE_INVALID_LOC));
	return BSAFE_INVALID_LOC;	
}

void bsafe_adjust_used_akeyloc(unsigned short akeyloc, unsigned int ksize)
{
	g_akeyloc = akeyloc;
	while (ksize-- > 0) {
		KEY_PRINTF(("adjust g_akeyloc %d [USED]\n",g_akeyloc));
		g_akeyloc_used[g_akeyloc++] = BSAFE_KEY_USED;
	}
}

void bsafe_free_akeyloc(unsigned short akeyloc, unsigned int ksize)
{
	g_akeyloc = akeyloc;
	while (ksize-- > 0) {
		KEY_PRINTF(("free g_akeyloc %d [FREE]\n",g_akeyloc));
		g_akeyloc_used[g_akeyloc++] = BSAFE_KEY_FREE;
	}
    /* Return global pointer to the start of the free region */
    g_akeyloc = akeyloc;
}



void initialize_kek_akey_localstatus(void * dev_context)
{

#include <bsafe_io.h>

#if 0
	PRINTF(("WARNING: Not doing initialize_kek_akey_localstatus............\n"));
#else

	int location;
    int rc;

	bsafe_kek_akey_status_io_t cmd;
	BYTE keyPolicy[6];
	

    KEY_PRINTF(("initialize_kek_akey_localstatus: begin\n"));
	/* dont use os_copy_from_user  we are in kernel*/
	bsafe_set_copy_from_user(OS_MEMCPY);
	bsafe_set_copy_to_user(OS_MEMCPY);

    memset(g_kekloc_used, BSAFE_KEY_FREE, sizeof(g_kekloc_used) );
    memset(g_akeyloc_used, BSAFE_KEY_FREE, sizeof(g_akeyloc_used) );

	for(location =BSAFE_KEK3_LOC; location <= BSAFE_KEK4_LOC; location++)
    {
		cmd.location = location;
		cmd.keyPolicy = keyPolicy;
		cmd.kSize =0;
        
		if ( (rc = bsafe_ioctl(dev_context, BSAFE_UHSM_KEK_STATUS_IOCTL, &cmd)) != 0)
        {
		    KEY_PRINTF(("initialize_kek_akey_localstatus: KEK_STATUS ioctl failed (%d)\n", rc));
            break;
        }
        
		if (cmd.kSize != 0)
			bsafe_adjust_used_kekloc(location, cmd.kSize);
	}
    
	for(location =BSAFE_MIN_AKEY_LOC; location <= BSAFE_MAX_AKEY_LOC; location++)
    {
		cmd.location = location;
		cmd.keyPolicy = keyPolicy;
		cmd.kSize =0;
        
		if ( (rc = bsafe_ioctl(dev_context, BSAFE_UHSM_AKEY_STATUS_IOCTL, &cmd)) != 0)
        {
		    KEY_PRINTF(("initialize_kek_akey_localstatus: AKEY_STATUS ioctl failed (%d)\n", rc));
            break;
        }
        
		if (cmd.kSize != 0)
			bsafe_adjust_used_akeyloc(location, cmd.kSize);
	}

	/* use os_copy_from_user  */
	bsafe_set_copy_from_user(OS_COPY_FROM_USER);
	bsafe_set_copy_to_user(OS_COPY_TO_USER);

    KEY_PRINTF(("initialize_kek_akey_localstatus: end\n"));
#endif

}

typedef struct errornamecode {
	char name[64];
	INT errorcode;
} errornamecode_t;

/* undef the define of the header file */
/* undef all the defines and recreate the preprocessor array to a struct array */
#undef __BSAFE_STATUS_H_
#undef BSAFE_STATUS_BEGIN
#undef BSAFE_STATUS_END
#undef BSAFE_ERRORCODE
#define BSAFE_STATUS_BEGIN errornamecode_t g_bsafe_status_errors[]= {
#define BSAFE_STATUS_END } ;
#define BSAFE_ERRORCODE(name,value) {#name,value},

#include <bsafe_status.h>

void print_status_error(INT errorcode)
{

	int sizeofarray ;
	sizeofarray= sizeof(g_bsafe_status_errors) / sizeof(errornamecode_t);


	while ( sizeofarray-- > 0) {
		if (errorcode == g_bsafe_status_errors[sizeofarray].errorcode) {
			PRINTF(("ERROR: returnstatus %s (%d)\n",
				g_bsafe_status_errors[sizeofarray].name, g_bsafe_status_errors[sizeofarray].errorcode));
			return;
		}
	}

	PRINTF(("ERROR: returnstatus %s (%d)\n",
		"UNKNOWN returnstatus" , errorcode));

}

#undef __BSAFE_RETURNCODE_H_
#undef BSAFE_RETURNCODE_BEGIN
#undef BSAFE_RETURNCODE_END
#undef BSAFE_RETURNCODE
#define BSAFE_RETURNCODE_BEGIN errornamecode_t g_bsafe_returncode_errors[]= {
#define BSAFE_RETURNCODE_END } ;
#define BSAFE_RETURNCODE(name,value) {#name,value},

#include <bsafe_returncode.h>
void print_returncode_error(UINT32 returncode)
{

	int sizeofarray ;
	sizeofarray= sizeof(g_bsafe_returncode_errors) / sizeof(errornamecode_t);

	while ( sizeofarray-- > 0) {
		if (returncode == (UINT32)g_bsafe_returncode_errors[sizeofarray].errorcode) {
			PRINTF(("ERROR: returncode %s (%08x)\n",
				g_bsafe_returncode_errors[sizeofarray].name,g_bsafe_returncode_errors[sizeofarray].errorcode));
			return;
		}
	}

	PRINTF(("ERROR: returncode %s (%08x)\n",
		"UNKNOWN returncode" , returncode));

}
