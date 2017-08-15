/*
 * Bsafe uHSM readwrite wrapper functions
 *
 * $Copyright Open Broadcom Corporation$
 * $Id: bsafe_readwrite.c,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bsafe_osl.h>
#include <bsafe_regs.h>
#include <bsafe_status.h>

#include <bsafe_core_if.h>


/* Can fix: but why?
* can remove the rem_bytes [UNALIGN_CHECK] mechanisum here
* but then the dma malloced buffer should always be aligned and
* dma free should always be freed to aligned buffer size
*/

#define BSAFE_WRITE(ctx, addr, in)	if (WRITE_UINT32_TO_DEVICE(ctx, addr,in) < 1)  {	\
					printf("bsafe_write_uint32 failed\n");	\
					return BSAFE_WRITE_FAILED;		\
				}
#define BSAFE_READ(ctx, addr, out)	if (READ_UINT32_FROM_DEVICE(ctx, addr, out) < 1) { \
					printf("bsafe_read_uint32 failed\n");	\
					return BSAFE_WRITE_FAILED;		\
				}

#ifdef DUMP_DEBUG
#define D_DUMP(arg)	{			\
		int i=0;			\
		printf("%s:\t",arg);		\
		for (i=0;i < BSAFE_MAX_REG_SIZE; i++) 	\
			printf("%02x ",ptr[i]);		\
		printf("\t %08x\n", *i_ptr);		\
		}
#else
#define D_DUMP(arg)
#endif

/* block read/write primitives */

int bsafe_write_to_device(void * dev_context,unsigned char *ptr,int size) {
	int *i_ptr;
	int rem_bytes=0;
	unsigned char rem_buf[4];

	unsigned int cmd_ctl;


	rem_bytes=size % BSAFE_MAX_REG_SIZE;

	size = size / BSAFE_MAX_REG_SIZE;
	D_PRINTF(("write_to_device %d\n",size));


	/* write the start of command */
	BSAFE_READ(dev_context, GLB_CMD_CTL, &cmd_ctl);
	cmd_ctl |= SWAP4BYTES(GLB_CMD_CTL_CMD_ISYNC);
	BSAFE_WRITE(dev_context, GLB_CMD_CTL, &cmd_ctl);


//	while (size-- > 0  ) {
	for ( ; size > 0; size-- ) {

write_this:
        #if 0
        /*
         * Need to check the state of the IRDY before writing data
         * to make sure overflow does not occur.
         */
        unsigned int cmd_tmp;
 
        while(1)
        {
	        BSAFE_READ(dev_context, GLB_CMD_CTL, &cmd_tmp);
            if (cmd_tmp & SWAP4BYTES(GLB_CMD_CTL_CMD_IRDY))
                break;
        }
        #endif
 
		i_ptr=(unsigned int *)ptr;

		/* write the end of command; yes have to pull it down before last 4 bytes */
		if ((size == 1) && (rem_bytes == 0)) {
			cmd_ctl &= SWAP4BYTES(~GLB_CMD_CTL_CMD_ISYNC);
			BSAFE_WRITE(dev_context, GLB_CMD_CTL, &cmd_ctl);
		}
	
		BSAFE_WRITE(dev_context, GLB_CMD_DATA_IN, i_ptr);

		D_DUMP("WRITE");

		ptr  = ptr + BSAFE_MAX_REG_SIZE;
	}

/* required to fix the last few unalign bytes */
#define UNALIGN_CHECK	1
#ifdef UNALIGN_CHECK
	if (rem_bytes != 0) {
		PRINTF(("WARNING: UNALIGNED rem bytes %d \n",rem_bytes));
		OS_MEMCPY(rem_buf, ptr, rem_bytes);
		ptr=(unsigned char *)rem_buf;
		rem_bytes=0;	/* so we dont loop */
        size = 1;
		goto write_this;
	}
#endif
	return  BSAFE_SUCCESS;
}

#define READ_BLOCKING   0

int bsafe_read_from_device(void * dev_context, unsigned char *ptr,int size)	 {
	unsigned int *i_ptr;
	int rem_bytes=0;
	int prev_rem_bytes=0;
	unsigned char rem_buf[4];
	unsigned char *prev_ptr=NULL;

	unsigned int cmd_ctl;

	rem_bytes=size % BSAFE_MAX_REG_SIZE;

	size = size / BSAFE_MAX_REG_SIZE;
	D_PRINTF(("read_from_device %d\n",size));

#if READ_BLOCKING
	/*
     * Set the start of command (OSYNC) so there isn't a need to
     * check the status of the ORDY bit.
     */
	BSAFE_READ(dev_context, GLB_CMD_CTL, &cmd_ctl);
	cmd_ctl |= SWAP4BYTES(GLB_CMD_CTL_CMD_OSYNC);
	BSAFE_WRITE(dev_context, GLB_CMD_CTL, &cmd_ctl);
#endif

//	while(size-- > 0) {
	for ( ; size > 0; size-- ) {

read_this:
    #if READ_BLOCKING
		/* write the end of command; yes have to pull it down before last 4 bytes */
		if ((size == 1) && (rem_bytes == 0))
        {
			cmd_ctl &= SWAP4BYTES(~GLB_CMD_CTL_CMD_OSYNC);
			BSAFE_WRITE(dev_context, GLB_CMD_CTL, &cmd_ctl);
		}
    #else
        while(1)
        {
	        BSAFE_READ(dev_context, GLB_CMD_CTL, &cmd_ctl);
            if (cmd_ctl & SWAP4BYTES(GLB_CMD_CTL_CMD_ORDY))
                break;
        }
    #endif

 
		i_ptr=(unsigned int *)ptr;

		BSAFE_READ(dev_context, GLB_CMD_DATA_OUT, i_ptr);

		D_DUMP("READ");
	
		ptr  = ptr + BSAFE_MAX_REG_SIZE;
	}

/* required to fix the last few unalign bytes */
#define UNALIGN_CHECK	1
#ifdef UNALIGN_CHECK
	if (rem_bytes != 0)  {	/* do it only once check */
		PRINTF(("WARNING: UNALIGNED rem bytes %d \n",rem_bytes));
		prev_ptr = ptr;
		ptr=(unsigned char *)rem_buf;
		prev_rem_bytes=rem_bytes;
		rem_bytes=0;	/* so we dont loop */
        size = 1;
		goto read_this;
	} else {	/* so now we have read it copy to the ptr */
		if (prev_ptr  != NULL) {	/* so now we have some thing to copy */
			ptr = prev_ptr;
			OS_MEMCPY(ptr, rem_buf, prev_rem_bytes);
		}
	}
#endif
	return  BSAFE_SUCCESS;
}
