/*
 * Bsafe uHSM context structures and a few local function definitions
 *
 * $Copyright Open Broadcom$      
 * $Id: bsafe_context.h,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#ifndef __BSAFE_CONTEXT_H_
#define __BSAFE_CONTEXT_H_

/* abstract DEV_CONTEXT ..just used as pass through for the device ...visible only in the wrapper */
#include <bsafe_core_if.h>

typedef enum bsafe_context_state {
	BSAFE_PUSHED=-1,
	BSAFE_COMPLETION_CALLBACK_DONE,
} bsafe_context_state_t;

/* allocate/get struct with command/structSize and ptrs assigned */
/* context accross interrupts */
typedef struct bsafe_context  {
	void * dev_context;	/* the opaque context to pass through the core driver to/from the wrarpper driver */
	UINT32	state;		/* maintains a state of the context */
	void * cmdio_arg;
	void * cmdio;
	UINT32 cmdio_size;
	void * input_cmd;
	UINT32 input_cmd_size;
	void * push_ptr; 	/* ptr in input_cmd to push to device; size is  input_cmd.structSize */
	bsafe_ocmd_t *ocmd;	/* for read of the return common params */
	void * output_cmd;	/* filled after callback */
	UINT32 output_cmd_size;
	void * read_ptr;	/* ptr for reading output in output_cmd which includes the ocmd */
	void (* callback)(
		void * 		/* bsafe_context_t*  */ 
		);
} bsafe_context_t;


/* only one command is inside chip */
/* or it is  FIFO based  */
int bsafe_push_context(bsafe_context_t * context) ;
bsafe_context_t * bsafe_pop_context(void) ;
#define PUSH_CONTEXT(context)	bsafe_push_context(context)
#define POP_CONTEXT()	bsafe_pop_context()


/* master copy routines */

extern void (*g_bsafe_copy_from_user)(void *, void *, int);
extern void (*g_bsafe_copy_to_user)(void *, void *, int);
extern void (*g_bsafe_swap_bignum)(BSAFE_BIGNUM *, int);
#define COPY_FROM_USER	(*g_bsafe_copy_from_user)
#define COPY_TO_USER	(*g_bsafe_copy_to_user)
/* swap used only if enabled else assigned to dummy values */
#define SWAPBIGNUM	(*g_bsafe_swap_bignum)

extern int bsafe_swap_bn(unsigned char * src, int size);
/* since word swap of bn format is BIGNUM */
#define BN2BIGNUM	bsafe_swap_bn

/* misc routines */
void initialize_kek_akey_localstatus(void * dev_context);
void print_status_error(INT errorcode);
void print_returncode_error(UINT32 returncode);
#endif /* __BSAFE_CONTEXT_H_ */
