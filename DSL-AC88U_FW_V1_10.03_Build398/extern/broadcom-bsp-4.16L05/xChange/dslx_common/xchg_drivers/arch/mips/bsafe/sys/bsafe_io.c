/*
 * Bsafe io implementations
 *
 * $Copyright Open Broadcom Corporation$      
 * $Id: bsafe_io.c,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <bsafe_osl.h>
#include <bsafe.h>
#include <bsafe_regs.h>
#include <bsafe_io.h>
#include <bsafe_context.h>
#include <bsafe_core_if.h>
#include <bsafe_keymgmt.h>



/* Local prototypes */
int allocate_inputbuf_for_cmd_with_one_variabledata(bsafe_context_t * cmd_context, int varData_len);
int allocate_inputbuf_for_ld_cfg(bsafe_context_t * cmd_context);
int allocate_inputbuf(bsafe_context_t * cmd_context, int command,  
		                void * arg, int cmdio_size, void **cmdio_ret,
		                int input_cmd_size, void **input_cmd_ret,
		                void *callback, int output_cmd_size);
                        
int allocate_outputbuf_for_cmd_with_one_variabledata(bsafe_context_t * cmd_context, bsafe_ocmd_t *ocmd);

void get_inputoutputbuf(bsafe_context_t * cmd_context, void **cmdio_ret, void **input_cmd_ret, void **output_cmd_ret);
void get_outputbuf(bsafe_context_t * cmd_context, void **cmdio_ret, void **output_cmd_ret);

int push_input_to_device(bsafe_context_t *cmd_context);
int pop_output_from_device(void);
	
void free_contextbufs(bsafe_context_t * cmd_context);

void bsafe_isr_callback(void *param);
void bsafe_completion_callback(bsafe_context_t * cmd_context);

int bsafe_init_devicekey_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_dlies_msg_dec_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_dlies_gen_ks_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_dev_kdi_public_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_dev_kdc_public_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_userauth_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_user_pke_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_user_3des_hmacsha1_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_selftest_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_kek_akey_status_in(bsafe_context_t  * cmd_context, void * arg, int command);
int bsafe_clr_key_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_ld_userkey_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_ld_cfg_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_fips_clr_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_user_status_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_user_nop_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_user_random_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_user_sha1_in(bsafe_context_t  * cmd_context, void * arg);
int bsafe_le_bignum(bsafe_context_t  * cmd_context, void * arg);
int bsafe_bn_2_bignum(bsafe_context_t  * cmd_context, void * arg);





int push_input_to_device(bsafe_context_t *cmd_context)
{
	bsafe_cmd_t * cmd;
	unsigned short push_size;
	unsigned char *ptr;

	PUSH_CONTEXT(cmd_context);

	/* really need not do this it is already there in push_size */
	cmd = (bsafe_cmd_t *) cmd_context->push_ptr;	
	push_size = SWAP2BYTES(cmd->structSize);


	ptr = (unsigned char *) cmd_context->push_ptr;

	return WRITE_TO_DEVICE(cmd_context->dev_context,ptr, push_size);

}

int allocate_outputbuf(bsafe_context_t * cmd_context, bsafe_ocmd_t *ocmd);

void bsafe_completion_callback(bsafe_context_t * cmd_context)
{

	if (READ_FROM_DEVICE(cmd_context->dev_context, (unsigned char *) cmd_context->ocmd, sizeof(bsafe_ocmd_t)) == BSAFE_SUCCESS) {

		/* Change the read bytes to the host endianess which was read from chip  */
		cmd_context->ocmd->cmd.command = SWAP2BYTES(cmd_context->ocmd->cmd.command);
		cmd_context->ocmd->cmd.structSize = SWAP2BYTES(cmd_context->ocmd->cmd.structSize);
		cmd_context->ocmd->returnCode = SWAP4BYTES(cmd_context->ocmd->returnCode);

		/* pop the rest of the output based on the output_cmd_size set during inputbuf creation */
		allocate_outputbuf(cmd_context, cmd_context->ocmd);
	}

}

int pop_output_from_device()	
{
	bsafe_context_t * cmd_context; 

	cmd_context = POP_CONTEXT();

	
	bsafe_completion_callback(cmd_context);

	/* change the state */
	cmd_context->state = BSAFE_COMPLETION_CALLBACK_DONE;

	return BSAFE_SUCCESS;
}

void bsafe_isr_callback(void *param)
{
   (void) param;

	pop_output_from_device();
	return ;
}

int bsafe_initialize(void * dev_context, int reset)
{
    UINT32 cmd;
    

    // TODO: Should this be an option to clear keys instead??    
    if ( reset )
    {
        cmd = GLB_CMD_CTL_CMD_SOFTRESET;
        WRITE_UINT32_TO_DEVICE( dev_context, GLB_CMD_CTL, &cmd );
        do
        {
            READ_UINT32_FROM_DEVICE( dev_context, GLB_DEV_STATUS, &cmd );
        }
        while( !(cmd & GLB_DEV_STATUS_INIT_DONE) );
       
    }
    
	/*initialize_bsafe_glb_register_array(base_address) */
	initialize_kek_akey_localstatus(dev_context);
	return BSAFE_SUCCESS;
}
int bsafe_clean()
{
	return BSAFE_SUCCESS;
}

/* sees only cmd_context never sees what is the command inside[ie cmdio] or its structs */
/* ret value of the called functions indicate its status */
int bsafe_ioctl(void * dev_context, int command, void *cmdio)
{
	bsafe_context_t * cmd_context ;
	int ret=BSAFE_SUCCESS;
	bsafe_io_common_param_t * param;




	/* allocate the context	 */
	cmd_context = (bsafe_context_t *) OS_ALLOC(sizeof(bsafe_context_t));
	if (cmd_context == NULL) {
		ret = BSAFE_MEM_ALLOC_FAILED;
		goto ioctl_return;
	}

	/* play safe */
	memset(cmd_context,0x00,sizeof(bsafe_context_t));


	/* save the dev context */
	cmd_context->dev_context = dev_context;


	/* cmdio is allocated only after this switch */
	switch(command){
		case BSAFE_UHSM_INIT_DEVICEKEY_IOCTL:
			ret=bsafe_init_devicekey_in(cmd_context,  cmdio);
			break;
		case BSAFE_UHSM_DLIES_MSG_DEC_IOCTL:
			ret=bsafe_dlies_msg_dec_in(cmd_context,  cmdio);
			break;
		case BSAFE_UHSM_DLIES_GEN_KS_IOCTL:
			ret=bsafe_dlies_gen_ks_in(cmd_context,  cmdio);
			break;
		case BSAFE_UHSM_DEV_KDI_PUBLIC_IOCTL:
			ret=bsafe_dev_kdi_public_in(cmd_context,  cmdio);
			break;
		case BSAFE_UHSM_DEV_KDC_PUBLIC_IOCTL:
			ret=bsafe_dev_kdc_public_in(cmd_context,  cmdio);
			break;
		case BSAFE_UHSM_USERAUTH_IOCTL:
			ret=bsafe_userauth_in(cmd_context,  cmdio);
			break;
		case BSAFE_UHSM_USER_PKE_IOCTL:
			ret=bsafe_user_pke_in(cmd_context,  cmdio);
			break;
		case BSAFE_UHSM_USER_3DES_HMACSHA1_IOCTL:
			ret=bsafe_user_3des_hmacsha1_in(cmd_context,  cmdio);
			break;
		case BSAFE_UHSM_SELFTEST_IOCTL:
			ret=bsafe_selftest_in(cmd_context,  cmdio);
			break;
		case BSAFE_UHSM_KEK_STATUS_IOCTL:
			ret=bsafe_kek_akey_status_in(cmd_context, cmdio, UHSM_KEK_STATUS);
			break;
		case BSAFE_UHSM_AKEY_STATUS_IOCTL:
			ret=bsafe_kek_akey_status_in(cmd_context, cmdio, UHSM_AKEY_STATUS);
			break;
		case BSAFE_UHSM_CLR_KEY_IOCTL:
			ret=bsafe_clr_key_in(cmd_context, cmdio);
			break;
		case BSAFE_UHSM_LD_USERKEY_IOCTL:
			ret=bsafe_ld_userkey_in(cmd_context, cmdio);
			break;
		case BSAFE_UHSM_LD_CFG_IOCTL:
			ret=bsafe_ld_cfg_in(cmd_context, cmdio);
			break;
		case BSAFE_UHSM_FIPS_CLR_IOCTL:
			ret=bsafe_fips_clr_in(cmd_context, cmdio);
			break;
		case BSAFE_UHSM_USER_STATUS_IOCTL:
			ret=bsafe_user_status_in(cmd_context, cmdio);
			break;
		case BSAFE_UHSM_USER_NOP_IOCTL:
			ret=bsafe_user_nop_in(cmd_context, cmdio);
			break;
		case BSAFE_UHSM_USER_RANDOM_IOCTL:
			ret=bsafe_user_random_in(cmd_context, cmdio);
			break;
		case BSAFE_UHSM_USER_SHA1_IOCTL:
			ret=bsafe_user_sha1_in(cmd_context, cmdio);
			break;
		case BSAFE_LE_BIGNUM_IOCTL:
			ret=bsafe_le_bignum(cmd_context, cmdio);
			break;
		case BSAFE_BN_2_BIGNUM_IOCTL:
			ret=bsafe_bn_2_bignum(cmd_context, cmdio);
			break;
		default:
			PRINTF(("Unknown ioctl %08x\n",command));
			ret = BSAFE_UNKNOWN_IOCTL;
			break;
	}

	if (ret != BSAFE_SUCCESS)
		goto ioctl_failed_return;

	/* do it only if we have a push command? some ioctls are for driver not require push */
	if (cmd_context->push_ptr != NULL)  {
		push_input_to_device(cmd_context);
		cmd_context->state = BSAFE_PUSHED;
	}

//#define BLOCK_IOCTL_MODE	1
//#ifdef BLOCK_IOCTL_MODE	/* sync mode */
#ifndef AYSNC_IOCTL_MODE

	/* set the param to get the common return status */
	param = (bsafe_io_common_param_t *) cmd_context->cmdio;	


#ifdef BLOCK_LOOP_WITH_ISR
	/* loop till we see the state changed  and expect the isr to call the bsafe_isr_callback */
	while ( (cmd_context->state == BSAFE_PUSHED) &&
		( max_delay <= MAX_TIMEOUT) )  {
		long	max_delay=0;
		DELAY(DELAY_US);
		max_delay+=DELAY_US;
	}
#else		/* for sock simulation mode */
	/* get the output from the chip? only if we have a output cmd to receive */ 
	if (cmd_context->ocmd != NULL)  {
		bsafe_isr_callback(NULL);
	}
#endif
	/* if the bsafe_isr_callback has not been called till now then we are timedout */
	if (cmd_context->state == (UINT32) BSAFE_PUSHED)
		param->returnStatus = BSAFE_TIMEDOUT;


	/* call callback if the chipreturned successs, also call only if we have a callback  ...*/
	/* also handle the special cases where there is messages in return regarding the error 
		and pass it to the app */
	switch (param->returnCode)  {
		case UHSM_SUCCESS:
		case AKEY_BAD_LOC_ERR:
		case SELFTEST_HW_FAIL_ERR:
		if (*cmd_context->callback != NULL) {
			(*cmd_context->callback)((void *)cmd_context);
		}
		break;
	}
	

	COPY_TO_USER(cmd_context->cmdio_arg, cmd_context->cmdio,  cmd_context->cmdio_size); 

	
	/* lets check the returncode errors  if we have an ocmd */
	if ((param->returnCode != UHSM_SUCCESS) && (cmd_context->ocmd !=NULL) )
		print_returncode_error(param->returnCode);
	/* lets check the returnstatus errors  */
	if (param->returnStatus != BSAFE_SUCCESS)
		print_status_error(param->returnStatus);

//#define ALWAYS_RET_SUCCESS	1
#ifdef ALWAYS_RET_SUCCESS
	param->returnStatus = BSAFE_SUCCESS;
	PRINTF(("Warning: ALWAYS_RET_SUCCESS returning %d\n",param->returnStatus));
#endif

	/* free the contexts  and bufs*/
	free_contextbufs(cmd_context);

#else	/* Async mode don't free the context This does not work...*/
	goto ioctl_return;
#endif


ioctl_failed_return:

	OS_FREE(cmd_context,sizeof(bsafe_context_t));

ioctl_return:
	if (ret != BSAFE_SUCCESS)
		print_status_error(ret);
	return ret;
}

int allocate_inputbuf_for_cmd_with_one_variabledata(bsafe_context_t * cmd_context, int varData_len)
{

	UINT32 varData_offset;

	bsafe_ptr_t * input;
	BYTE * input_ptr;


	varData_offset = cmd_context->input_cmd_size;
	cmd_context->input_cmd_size +=varData_len;
	
	
	cmd_context->input_cmd = (void *) OS_DMA_ALLOC(cmd_context->dev_context, cmd_context->input_cmd_size);
	if (cmd_context->input_cmd == NULL)
		return BSAFE_MEM_ALLOC_FAILED;

	input = (bsafe_ptr_t *) cmd_context->input_cmd;
	input_ptr = (BYTE *) input; /* for increments in bytes for pointer additions */
	input->ptr = (BYTE *) ( input_ptr + varData_offset);
	input->len = varData_len;	
		

	/* set the push ptr  */
	cmd_context->push_ptr = input_ptr + sizeof(bsafe_ptr_t);


	return BSAFE_SUCCESS;

}

int allocate_inputbuf_for_ld_cfg(bsafe_context_t * cmd_context)
{
	bsafe_ld_cfg_io_t * cmdio =(bsafe_ld_cfg_io_t *) cmd_context->cmdio;

	UINT32 dlies_msg_offset;
	UINT32 authSig_r_offset; /* len 20 */
	UINT32 authSig_s_offset; /* len 20 */

	bsafe_ld_cfg_input_t * input;
	BYTE * input_ptr;

	dlies_msg_offset = cmd_context->input_cmd_size;
	cmd_context->input_cmd_size +=cmdio->dlies_msg_len;
	
	authSig_r_offset = cmd_context->input_cmd_size;
	cmd_context->input_cmd_size +=20;
	
	authSig_s_offset = cmd_context->input_cmd_size;
	cmd_context->input_cmd_size +=20;
	
	
	cmd_context->input_cmd = (void *) OS_DMA_ALLOC(cmd_context->dev_context, cmd_context->input_cmd_size);
	if (cmd_context->input_cmd == NULL)
		return BSAFE_MEM_ALLOC_FAILED;

	input = (bsafe_ld_cfg_input_t *) cmd_context->input_cmd;
	input_ptr = (BYTE *) input; /* for increments in bytes for pointer additions */
	input->dlies_msg.ptr = (BYTE *) ( input_ptr + dlies_msg_offset);
	input->dlies_msg.len = cmdio->dlies_msg_len;	
	input->authSig_r_ptr = (BYTE *) (input_ptr + authSig_r_offset);
	input->authSig_s_ptr = (BYTE *) (input_ptr + authSig_s_offset);
		

	/* set the push ptr  */
	cmd_context->push_ptr = &input->cmd;


	return BSAFE_SUCCESS;

}

/* pass in NULL for callback if no specific parameters handling is required for the output  */
/* pass in command == -1 for driver ioctl special handling */

int allocate_inputbuf(bsafe_context_t * cmd_context, int command,  
		void * arg, int cmdio_size, void **cmdio_ret,
		int input_cmd_size, void **input_cmd_ret,
		 void *callback, int output_cmd_size)
{
	bsafe_io_common_param_t * param;


	/* save the arg */
	cmd_context->cmdio_arg = arg;

	/* allocate and copy the cmd from user space */
	cmd_context->cmdio = (void *) OS_ALLOC(cmdio_size);
	if (cmd_context->cmdio == NULL)
		return BSAFE_MEM_ALLOC_FAILED;

	/* save the size alloced */
	cmd_context->cmdio_size = cmdio_size;

	/* sync user space arg to cmdio kernel space */
	/* can save a few unwanted sync bytes of the output values in this sync */
	COPY_FROM_USER(cmd_context->cmdio, arg, cmdio_size);

	/* set the cmdio return Status */
	param = (bsafe_io_common_param_t *) cmd_context->cmdio;	
	param->returnStatus =  BSAFE_SUCCESS;	/* be optimistic */

	/* save the callback in the context */
	cmd_context->callback = callback;

	/* start the input cmd size/output_cmd_size with the known sizes */
	cmd_context->input_cmd_size = input_cmd_size;
	cmd_context->output_cmd_size = output_cmd_size;

	

	/* allocate the input_cmd and assign its associate ptrs */
	/* allways pass &input_cmd , &input_cmd_size, &push_ptr_size */
	/* all commands switch should take of host endianess for the write chip command */
	/* currently endianess would be taken care for only values; ptrs or uhsm-unaware data not taken care by the driver */ 
	switch (command) {	
		case -1:	/* special case for driver ioctl's */
			cmd_context->input_cmd = NULL;
			cmd_context->push_ptr = NULL;
			cmd_context->ocmd = NULL;
			return BSAFE_SUCCESS; 
			break;
		case UHSM_DLIES_MSG_DEC:
			allocate_inputbuf_for_cmd_with_one_variabledata(cmd_context, 
				((bsafe_dlies_msg_dec_io_t*) cmd_context->cmdio)->dlies_msg_len);
			break;
		case UHSM_USER_PKE:
			allocate_inputbuf_for_cmd_with_one_variabledata(cmd_context, 
				((bsafe_user_pke_io_t*) cmd_context->cmdio)->pkeMsg_len);
			break;
		case UHSM_USER_3DES_HMACSHA1:
			allocate_inputbuf_for_cmd_with_one_variabledata(cmd_context, 
				((bsafe_user_3des_hmacsha1_io_t*) cmd_context->cmdio)->data_len);
			break;
		case UHSM_LD_USERKEY:
			allocate_inputbuf_for_cmd_with_one_variabledata(cmd_context, 
				((bsafe_ld_userkey_io_t*) cmd_context->cmdio)->keyData_len);
			break;
		case UHSM_USER_NOP:
			allocate_inputbuf_for_cmd_with_one_variabledata(cmd_context, 
				((bsafe_user_nop_io_t*) cmd_context->cmdio)->data_len);
			break;
		case UHSM_USER_SHA1:
			allocate_inputbuf_for_cmd_with_one_variabledata(cmd_context, 
				((bsafe_user_sha1_io_t*) cmd_context->cmdio)->data_len);
			break;
		case UHSM_LD_CFG:
			allocate_inputbuf_for_ld_cfg(cmd_context);
			break;
		default:	/* input cmd and input cmd struct same size */
			cmd_context->input_cmd = (void *) OS_DMA_ALLOC(cmd_context->dev_context, cmd_context->input_cmd_size);
			cmd_context->push_ptr = cmd_context->input_cmd;
			break;
			
	}

	if (cmd_context->input_cmd == NULL) {
		param->returnStatus =  BSAFE_MEM_ALLOC_FAILED;
		return BSAFE_MEM_ALLOC_FAILED;
	}

	/* allocate memory for the common return params */
	cmd_context->ocmd = (void *) OS_DMA_ALLOC(cmd_context->dev_context, sizeof(bsafe_ocmd_t));
	if (cmd_context->ocmd == NULL) {
		param->returnStatus =  BSAFE_MEM_ALLOC_FAILED;
		return BSAFE_MEM_ALLOC_FAILED;
	}
	memset(cmd_context->ocmd,0x00, sizeof(bsafe_ocmd_t));
	


	/* assign the cmd and size for the chip context */
	((bsafe_cmd_t *)cmd_context->push_ptr)->command = SWAP2BYTES(command);
	((bsafe_cmd_t *)cmd_context->push_ptr)->structSize = 
		SWAP2BYTES(cmd_context->input_cmd_size - ( (BYTE *)cmd_context->push_ptr - (BYTE*) cmd_context->input_cmd));


	

	
	
	/* assign the ret for the caller */	
	*cmdio_ret = cmd_context->cmdio;
	*input_cmd_ret = cmd_context->input_cmd;
	

	
	return BSAFE_SUCCESS;
}



int allocate_outputbuf_for_cmd_with_one_variabledata(bsafe_context_t * cmd_context, bsafe_ocmd_t *ocmd)
{
	
	int varData_offset;
	int varData_len;	/* has to be signed int for -ve math */

	bsafe_ptr_t * output;
	BYTE * output_ptr;

	/* the variable len is difference of chip output + a header ptr struct - the output struct - variable data */
	varData_len = ocmd->cmd.structSize + sizeof(bsafe_ptr_t)  - cmd_context->output_cmd_size  ;
	varData_len = (((int)varData_len < 0)? 0: varData_len);
	varData_offset = cmd_context->output_cmd_size;
	cmd_context->output_cmd_size += varData_len;
	
	
	cmd_context->output_cmd = (void *) OS_DMA_ALLOC(cmd_context->dev_context, cmd_context->output_cmd_size);
	if (cmd_context->output_cmd == NULL)
		return BSAFE_MEM_ALLOC_FAILED;

	output = (bsafe_ptr_t *) cmd_context->output_cmd;

	output_ptr = (BYTE *) output; /* for increments in bytes for pointer additions */
	output->ptr = (BYTE *) ( output_ptr + varData_offset);
	output->len = varData_len;	

		
	/* set the return values */
	cmd_context->read_ptr = output_ptr + sizeof(bsafe_ptr_t);

	return BSAFE_SUCCESS;
}


void get_outputbuf(bsafe_context_t * cmd_context, 
	void **cmdio_ret, void **output_cmd_ret) {

	/* assign the ret for the caller */	
	*cmdio_ret = cmd_context->cmdio;
	*output_cmd_ret = cmd_context->output_cmd;
}

void get_inputoutputbuf(bsafe_context_t * cmd_context, 
	void **cmdio_ret, void **input_cmd_ret, void **output_cmd_ret) {

   (void) input_cmd_ret;

	/* assign the ret for the caller */	
	*cmdio_ret = cmd_context->cmdio;
    /* TODO: The assignment from cmd_context->input_cmd doesn't seem correct */
	*output_cmd_ret = cmd_context->input_cmd;
	*output_cmd_ret = cmd_context->output_cmd;
}


/* done this way because we need to write buffer to the structure which might be larger than the required output because of pointer into  output buffer for variable buffer manupulation. */
/* by now ocmd is swapped to host endianess */
/* start the output cmd size with the known output_cmd_size which was set when we allocate the input bufs */

int allocate_outputbuf(bsafe_context_t * cmd_context, bsafe_ocmd_t *ocmd)
{

	short get_size;
	unsigned char *read_ptr;

	bsafe_io_common_param_t * param;

	/* set the cmdio return code  to be same as what we got from chip(ocmd) */
	param = (bsafe_io_common_param_t *) cmd_context->cmdio;	
	param->returnCode = ocmd->returnCode;


	if (ocmd->cmd.structSize < cmd_context->output_cmd_size ) {
		PRINTF(("Received output cmd size %d less than the minimum expected output cmd size %d \n", ocmd->cmd.structSize, cmd_context->output_cmd_size));
		param->returnStatus =  BSAFE_OUTPUT_CMD_SIZE_MISMATCH;
	}


	/* allocate the output_cmd and assign its associate ptrs */
	/* all commands switch should take of host endianess for the new data read */
	switch(ocmd->cmd.command) {
		case UHSM_USER_PKE:
		case UHSM_USER_3DES_HMACSHA1:
		case UHSM_USER_NOP:
			allocate_outputbuf_for_cmd_with_one_variabledata(cmd_context, ocmd);
			break;
		default:	/* output cmd and output struct fixed size */
			if (ocmd->cmd.structSize != cmd_context->output_cmd_size) {	
				/* some mismatch any way lets allocate the biggest buffer */
				PRINTF(("Received output cmd size %d and expected output cmd size %d mismatch\n", ocmd->cmd.structSize, cmd_context->output_cmd_size));
				cmd_context->output_cmd_size = ((ocmd->cmd.structSize > cmd_context->output_cmd_size)? ocmd->cmd.structSize: cmd_context->output_cmd_size);
				param->returnStatus =  BSAFE_OUTPUT_CMD_SIZE_MISMATCH;
			}
			cmd_context->output_cmd = (void *) OS_DMA_ALLOC(cmd_context->dev_context, cmd_context->output_cmd_size);
			cmd_context->read_ptr = cmd_context->output_cmd;

			break;
	}

	if (cmd_context->output_cmd == NULL) {
		param->returnStatus =  BSAFE_MEM_ALLOC_FAILED;
		return BSAFE_MEM_ALLOC_FAILED;
	}

#ifdef NOT_REQUIRED  /* already ocmd has these values, and return code has been copied */
	/* assign the cmd and size and return*/
	((bsafe_ocmd_t *)cmd_context->read_ptr)->cmd.command = ocmd->cmd.command;
	((bsafe_ocmd_t *)cmd_context->read_ptr)->cmd.structSize = ocmd->cmd.structSize;
	((bsafe_ocmd_t *)cmd_context->read_ptr)->returnCode = ocmd->returnCode;
#endif

	



	/* read output from the device */
	read_ptr = (unsigned char *) cmd_context->read_ptr + sizeof(bsafe_ocmd_t);
	get_size = ocmd->cmd.structSize - sizeof(bsafe_ocmd_t);

	if (get_size > 0) {
		READ_FROM_DEVICE(cmd_context->dev_context, read_ptr, get_size);
	}



	return BSAFE_SUCCESS;
}

void  free_contextbufs(bsafe_context_t * cmd_context)
{
	if (cmd_context->output_cmd != NULL)
		OS_DMA_FREE(cmd_context->dev_context, cmd_context->output_cmd, cmd_context->output_cmd_size);
	if (cmd_context->ocmd != NULL)
		OS_DMA_FREE(cmd_context->dev_context, cmd_context->ocmd, sizeof(bsafe_ocmd_t));
	if (cmd_context->input_cmd != NULL)
		OS_DMA_FREE(cmd_context->dev_context, cmd_context->input_cmd, cmd_context->input_cmd_size);
	if (cmd_context->cmdio != NULL)
		OS_FREE(cmd_context->cmdio, cmd_context->cmdio_size);
	
}



/* currently all return code return the value returned from the chip*/

/* gen ks */

void bsafe_dlies_gen_ks_out(bsafe_context_t * cmd_context);

int bsafe_dlies_gen_ks_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_dlies_gen_ks_io_t *cmdio;
	bsafe_dlies_gen_ks_input_t *input_cmd;

	allocate_inputbuf(cmd_context, UHSM_DLIES_GEN_KS, 
		arg, sizeof(bsafe_dlies_gen_ks_io_t), (void *) &cmdio,
		sizeof(bsafe_dlies_gen_ks_input_t), (void *) &input_cmd,
		(void *) bsafe_dlies_gen_ks_out,
		 sizeof(bsafe_dlies_gen_ks_output_t) );

	input_cmd->kekLoc = SWAP2BYTES(cmdio->kekLoc);
	OS_MEMSET(input_cmd->reserved, 0x00, sizeof(input_cmd->reserved));
	COPY_FROM_USER(input_cmd->challenge, cmdio->challenge, sizeof(input_cmd->challenge));

	return BSAFE_SUCCESS;
}

void bsafe_dlies_gen_ks_out(bsafe_context_t * cmd_context)
{
	bsafe_dlies_gen_ks_output_t *output_cmd;
	bsafe_dlies_gen_ks_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);

	/*SWAPBIGNUM(output_cmd->ks_cp, sizeof(output_cmd->ks_cp));*/
	COPY_TO_USER(cmdio->ks_cp, output_cmd->ks_cp, sizeof(output_cmd->ks_cp));
	SWAPBIGNUM(output_cmd->msgSig_r, sizeof(output_cmd->msgSig_r));
	COPY_TO_USER(cmdio->msgSig_r, output_cmd->msgSig_r, sizeof(output_cmd->msgSig_r));
	SWAPBIGNUM(output_cmd->msgSig_s, sizeof(output_cmd->msgSig_s));
	COPY_TO_USER(cmdio->msgSig_s, output_cmd->msgSig_s, sizeof(output_cmd->msgSig_s));

}




/* msg dec */

void bsafe_dlies_msg_dec_out(bsafe_context_t * cmd_context);
 
int bsafe_dlies_msg_dec_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_dlies_msg_dec_io_t *cmdio;
	bsafe_dlies_msg_dec_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_DLIES_MSG_DEC, 
		arg, sizeof(bsafe_dlies_msg_dec_io_t), (void *) &cmdio,
		sizeof(bsafe_dlies_msg_dec_input_t), (void *) &input_cmd,
		(void *) bsafe_dlies_msg_dec_out,
		sizeof(bsafe_dlies_msg_dec_output_t));

	/* if flags is DRIVER_GET_KEY_LOC get free keyloc*/
	if (cmdio->flags == BSAFE_UHSM_DRIVER_GET_KEY_LOC) {

		/* get a free location */
		if ((cmdio->kekLoc != BSAFE_INVALID_LOC)  && (cmdio->akeyLoc == BSAFE_INVALID_LOC)) {
			input_cmd->kekLoc = SWAP2BYTES(bsafe_get_free_kekloc(2048));
			input_cmd->akeyLoc = BSAFE_INVALID_LOC;
			if (input_cmd->kekLoc == BSAFE_INVALID_LOC)  
				return BSAFE_NO_FREE_KEKLOC;
		}else if ((cmdio->kekLoc == BSAFE_INVALID_LOC)  && (cmdio->akeyLoc != BSAFE_INVALID_LOC)) {
			input_cmd->kekLoc = BSAFE_INVALID_LOC;
			input_cmd->akeyLoc = SWAP2BYTES(bsafe_get_free_akeyloc());
			if  (input_cmd->akeyLoc == BSAFE_INVALID_LOC)
				return BSAFE_NO_FREE_AKEYLOC;
		} else 
			return BSAFE_INVALID_KEYLOC_PARAM;

	} else {
		input_cmd->kekLoc = SWAP2BYTES(cmdio->kekLoc);
		input_cmd->akeyLoc = SWAP2BYTES(cmdio->akeyLoc);
	}

	input_cmd->nestLevel = SWAP2BYTES(cmdio->nestLevel);
	input_cmd->cache = 0x00;

	input_cmd->decLocArray[0] = BSAFE_INVALID_LOC;
	input_cmd->decLocArray[1] = BSAFE_INVALID_LOC;
	COPY_FROM_USER(input_cmd->decLocArray, cmdio->decLocArray, cmdio->nestLevel*sizeof(UINT16));
	input_cmd->decLocArray[0] = SWAP2BYTES(input_cmd->decLocArray[0]);
	input_cmd->decLocArray[1] = SWAP2BYTES(input_cmd->decLocArray[1]);
	COPY_FROM_USER(input_cmd->dlies_msg.ptr, cmdio->dlies_msg, cmdio->dlies_msg_len);

	input_cmd->msgID = SWAP4BYTES(BSAFE_CRYPTTEXT);	

	CMD_PRINTF("%d", cmdio->dlies_msg_len);

	return BSAFE_SUCCESS;
}

void bsafe_dlies_msg_dec_out(bsafe_context_t * cmd_context)
{
	bsafe_dlies_msg_dec_output_t *output_cmd;
	bsafe_dlies_msg_dec_io_t *cmdio;


	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);


	cmdio->msgId = SWAP4BYTES(output_cmd->msgId);

	cmdio->kType = SWAP2BYTES(output_cmd->kType);
	cmdio->kSize = SWAP2BYTES(output_cmd->kNumLocations);
	CMD_PRINTF("%d", cmdio->kSize);

	/* adjust the used key locations  and sync the values to the output cmdio*/
	if ((cmdio->kekLoc != BSAFE_INVALID_LOC)  && (cmdio->akeyLoc == BSAFE_INVALID_LOC)) {
		/* kek cache */
		cmdio->kekLoc = SWAP2BYTES(((bsafe_dlies_msg_dec_input_t *)cmd_context->input_cmd)->kekLoc);
		bsafe_adjust_used_kekloc(cmdio->kekLoc, cmdio->kSize);
	}else  {	/* believe on the input validation  */
		/* akey cache */
		cmdio->akeyLoc = SWAP2BYTES(((bsafe_dlies_msg_dec_input_t *)cmd_context->input_cmd)->akeyLoc);
		bsafe_adjust_used_akeyloc(cmdio->akeyLoc, cmdio->kSize);
	}

}

/* kdi public */

void bsafe_dev_kdi_public_out(bsafe_context_t * cmd_context);

int bsafe_dev_kdi_public_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_dev_kdi_public_io_t *cmdio;
	bsafe_dev_kdi_public_input_t *input_cmd;

	allocate_inputbuf(cmd_context, UHSM_DEV_KDI_PUBLIC, 
		arg, sizeof(bsafe_dev_kdi_public_io_t), (void *) &cmdio,
		sizeof(bsafe_dev_kdi_public_input_t), (void *) &input_cmd,
		(void *) bsafe_dev_kdi_public_out,
		sizeof(bsafe_dev_kdi_public_output_t));

	input_cmd->options = SWAP4BYTES(cmdio->options);
	
	return BSAFE_SUCCESS;
}

void bsafe_dev_kdi_public_out(bsafe_context_t * cmd_context)
{
	bsafe_dev_kdi_public_output_t *output_cmd;
	bsafe_dev_kdi_public_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);

	cmdio->keyType = SWAP2BYTES(output_cmd->keyType);
	COPY_TO_USER(cmdio->configuration, output_cmd->configuration, sizeof(output_cmd->configuration));
	COPY_TO_USER(cmdio->authorization, output_cmd->authorization, sizeof(output_cmd->authorization));
	
	COPY_TO_USER(cmdio->kdi_p.p, output_cmd->kdi_p.p, sizeof(output_cmd->kdi_p.p));
	COPY_TO_USER(cmdio->kdi_p.g, output_cmd->kdi_p.g, sizeof(output_cmd->kdi_p.g));
	COPY_TO_USER(cmdio->kdi_p.q, output_cmd->kdi_p.q, sizeof(output_cmd->kdi_p.q));
	COPY_TO_USER(cmdio->kdi_p.y, output_cmd->kdi_p.y, sizeof(output_cmd->kdi_p.y));
	
}


/* kdc public */

void bsafe_dev_kdc_public_out(bsafe_context_t * cmd_context);

int bsafe_dev_kdc_public_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_dev_kdc_public_io_t *cmdio;
	bsafe_dev_kdc_public_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_DEV_KDC_PUBLIC, 
		arg, sizeof(bsafe_dev_kdc_public_io_t), (void *) &cmdio,
		sizeof(bsafe_dev_kdc_public_input_t), (void *) &input_cmd,
		(void *) bsafe_dev_kdc_public_out,
		sizeof(bsafe_dev_kdc_public_output_t));


	input_cmd->options = SWAP4BYTES(cmdio->options);
	
	return BSAFE_SUCCESS;
}

void bsafe_dev_kdc_public_out(bsafe_context_t * cmd_context)
{
	bsafe_dev_kdc_public_output_t *output_cmd;
	bsafe_dev_kdc_public_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);

	cmdio->keyType = SWAP2BYTES(output_cmd->keyType);
	/*SWAPBIGNUM(output_cmd->kdc_cp, sizeof(output_cmd->kdc_cp));*/
	COPY_TO_USER(cmdio->kdc_cp, output_cmd->kdc_cp, sizeof(output_cmd->kdc_cp));
	SWAPBIGNUM(output_cmd->msgSig_r, sizeof(output_cmd->msgSig_r));
	COPY_TO_USER(cmdio->msgSig_r, output_cmd->msgSig_r, sizeof(output_cmd->msgSig_r));
	SWAPBIGNUM(output_cmd->msgSig_s, sizeof(output_cmd->msgSig_s));
	COPY_TO_USER(cmdio->msgSig_s, output_cmd->msgSig_s, sizeof(output_cmd->msgSig_s));
	
}

/* userauth */

void bsafe_userauth_out(bsafe_context_t * cmd_context);

int bsafe_userauth_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_userauth_io_t *cmdio;
	bsafe_userauth_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_USERAUTH, 
		arg, sizeof(bsafe_userauth_io_t), (void *) &cmdio,
		sizeof(bsafe_userauth_input_t), (void *) &input_cmd,
		(void *) bsafe_userauth_out,
		sizeof(bsafe_userauth_output_t));

	COPY_FROM_USER(input_cmd->challenge, cmdio->challenge, sizeof(input_cmd->challenge));
	
	
	return BSAFE_SUCCESS;
}

void bsafe_userauth_out(bsafe_context_t * cmd_context)
{
	bsafe_userauth_output_t *output_cmd;
	bsafe_userauth_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);

	SWAPBIGNUM(output_cmd->msgSig_r, sizeof(output_cmd->msgSig_r));
	COPY_TO_USER(cmdio->msgSig_r, output_cmd->msgSig_r, sizeof(output_cmd->msgSig_r));
	SWAPBIGNUM(output_cmd->msgSig_s, sizeof(output_cmd->msgSig_s));
	COPY_TO_USER(cmdio->msgSig_s, output_cmd->msgSig_s, sizeof(output_cmd->msgSig_s));
	
}


/* pke  */

void bsafe_user_pke_out(bsafe_context_t * cmd_context);

int bsafe_user_pke_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_user_pke_io_t *cmdio;
	bsafe_user_pke_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_USER_PKE, 
		arg, sizeof(bsafe_user_pke_io_t), (void *) &cmdio,
		sizeof(bsafe_user_pke_input_t), (void *) &input_cmd,
		(void *) bsafe_user_pke_out,
		sizeof(bsafe_user_pke_output_t));

	input_cmd->kekLoc = BSAFE_INVALID_LOC;
	input_cmd->akekLoc = SWAP2BYTES(cmdio->akekLoc);
	
	input_cmd->opCode = SWAP2BYTES(cmdio->opCode); 
	input_cmd->contextLen = 0; 

	COPY_FROM_USER(input_cmd->pkeMsgIn.ptr, cmdio->pkeMsgIn, cmdio->pkeMsg_len);
	BN2BIGNUM(input_cmd->pkeMsgIn.ptr, cmdio->pkeMsg_len);	
	
	
	return BSAFE_SUCCESS;
}

void bsafe_user_pke_out(bsafe_context_t * cmd_context)
{
	bsafe_user_pke_output_t *output_cmd;
	bsafe_user_pke_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);

	BN2BIGNUM(output_cmd->pkeMsgOut.ptr, output_cmd->pkeMsgOut.len);	
	COPY_TO_USER(cmdio->pkeMsgOut, output_cmd->pkeMsgOut.ptr, output_cmd->pkeMsgOut.len);
	
}



/* 3des hmacsha1  */

void bsafe_user_3des_hmacsha1_out(bsafe_context_t * cmd_context);

int bsafe_user_3des_hmacsha1_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_user_3des_hmacsha1_io_t *cmdio;
	bsafe_user_3des_hmacsha1_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_USER_3DES_HMACSHA1, 
		arg, sizeof(bsafe_user_3des_hmacsha1_io_t), (void *) &cmdio,
		sizeof(bsafe_user_3des_hmacsha1_input_t), (void *) &input_cmd,
		(void *) bsafe_user_3des_hmacsha1_out,
		sizeof(bsafe_user_3des_hmacsha1_output_t));

	input_cmd->mode = SWAP4BYTES(cmdio->mode);
	input_cmd->akey3DesLoc = SWAP2BYTES(cmdio->akey3DesLoc);
	input_cmd->akeyHashLoc = SWAP2BYTES(cmdio->akeyHashLoc);

	OS_MEMSET(input_cmd->reserved, 0x00, sizeof(input_cmd->reserved));
	COPY_FROM_USER(input_cmd->iv, cmdio->iv, sizeof(input_cmd->iv));
	COPY_FROM_USER(input_cmd->dataIn.ptr, cmdio->dataIn, cmdio->data_len);
	
	
	return BSAFE_SUCCESS;
}

void bsafe_user_3des_hmacsha1_out(bsafe_context_t * cmd_context)
{
	bsafe_user_3des_hmacsha1_output_t *output_cmd;
	bsafe_user_3des_hmacsha1_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);

	COPY_TO_USER(cmdio->outDigest, output_cmd->outDigest, sizeof(output_cmd->outDigest));

	if ((cmdio->mode & BSAFE_MODE_DES_CBC) == BSAFE_MODE_DES_CBC)
		COPY_TO_USER(cmdio->dataOut, ((unsigned char *)output_cmd->dataOut.ptr)+8, output_cmd->dataOut.len -8 ); /* skip the iv */
	else
		COPY_TO_USER(cmdio->dataOut, output_cmd->dataOut.ptr, output_cmd->dataOut.len);

		
}




/* init devicekey  */

void bsafe_init_devicekey_out(bsafe_context_t * cmd_context);

int bsafe_init_devicekey_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_init_devicekey_io_t *cmdio;
	bsafe_init_devicekey_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_INIT_DEVICEKEY, 
		arg, sizeof(bsafe_init_devicekey_io_t), (void *) &cmdio,
		sizeof(bsafe_init_devicekey_input_t), (void *) &input_cmd,
		(void *) bsafe_init_devicekey_out,
		sizeof(bsafe_init_devicekey_output_t));

	input_cmd->uhsmConfig = SWAP4BYTES(cmdio->uhsmConfig);
	input_cmd->numRowRetry = SWAP2BYTES(cmdio->numRowRetry);
	input_cmd->numColRetry = SWAP2BYTES(cmdio->numColRetry);

	COPY_FROM_USER(&input_cmd->prodConfig, cmdio->prodConfig, sizeof(input_cmd->prodConfig));
	COPY_FROM_USER(input_cmd->authorization, cmdio->authorization, sizeof(input_cmd->authorization));
		
	return BSAFE_SUCCESS;
}

void bsafe_init_devicekey_out(bsafe_context_t * cmd_context)
{
	bsafe_init_devicekey_output_t *output_cmd;
	bsafe_init_devicekey_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);

	COPY_TO_USER(cmdio->statistics, output_cmd->statistics, sizeof(output_cmd->statistics));
	
}


/* selftest  */

void bsafe_selftest_out(bsafe_context_t * cmd_context);

int bsafe_selftest_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_selftest_io_t *cmdio;
	bsafe_selftest_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_SELFTEST, 
		arg, sizeof(bsafe_selftest_io_t), (void *) &cmdio,
		sizeof(bsafe_selftest_input_t), (void *) &input_cmd,
		(void *) bsafe_selftest_out,
		sizeof(bsafe_selftest_output_t));

	input_cmd->runtest = SWAP4BYTES(cmdio->runtest);
		
	return BSAFE_SUCCESS;
}

void bsafe_selftest_out(bsafe_context_t * cmd_context)
{
	bsafe_selftest_output_t *output_cmd;
	bsafe_selftest_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);

	cmdio->result = SWAP4BYTES(output_cmd->result);

}

/* kek and akey command  */

void bsafe_kek_akey_status_out(bsafe_context_t * cmd_context);

int bsafe_kek_akey_status_in(bsafe_context_t  * cmd_context, void * arg, int command)
{
	bsafe_kek_akey_status_io_t *cmdio;
	bsafe_kek_akey_status_input_t *input_cmd;


	allocate_inputbuf(cmd_context, command, 
		arg, sizeof(bsafe_kek_akey_status_io_t), (void *) &cmdio,
		sizeof(bsafe_kek_akey_status_input_t), (void *) &input_cmd,
		(void *) bsafe_kek_akey_status_out,
		sizeof(bsafe_kek_akey_status_output_t));

	if (command == UHSM_KEK_STATUS) {
		input_cmd->kekLoc = SWAP2BYTES(cmdio->location);
		input_cmd->akeyLoc = 0x0;
	} else { /* UHSM_AKEY_STATUS */
		input_cmd->akeyLoc = SWAP2BYTES(cmdio->location);
		input_cmd->kekLoc = 0x0;
	} 
		
	return BSAFE_SUCCESS;
}

void bsafe_kek_akey_status_out(bsafe_context_t * cmd_context)
{
	bsafe_kek_akey_status_output_t *output_cmd;
	bsafe_kek_akey_status_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);

	cmdio->keyType = SWAP2BYTES(output_cmd->keyType);
	cmdio->kSize = SWAP4BYTES(output_cmd->kSize);
	

	COPY_TO_USER(cmdio->keyPolicy, output_cmd->keyPolicy, sizeof(output_cmd->keyPolicy));
	
}

/* clr key command  */

void bsafe_clr_key_out(bsafe_context_t * cmd_context);

int bsafe_clr_key_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_clr_key_io_t *cmdio;
	bsafe_clr_key_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_CLR_KEY, 
		arg, sizeof(bsafe_clr_key_io_t), (void *) &cmdio,
		sizeof(bsafe_clr_key_input_t), (void *) &input_cmd,
		(void *) bsafe_clr_key_out,
		sizeof(bsafe_clr_key_output_t));


	input_cmd->location = SWAP2BYTES(cmdio->location);
	input_cmd->cache = SWAP2BYTES(cmdio->cache);
		
	return BSAFE_SUCCESS;
}

void bsafe_clr_key_out(bsafe_context_t * cmd_context)
{
	bsafe_clr_key_output_t *output_cmd;
	bsafe_clr_key_io_t *cmdio;
	bsafe_clr_key_input_t *input_cmd;



	get_inputoutputbuf(cmd_context,  
		(void *) &cmdio, (void *) &input_cmd,
		(void *) &output_cmd);

	cmdio->kSize = SWAP4BYTES(output_cmd->kSize);

	/* free the locations from the local global array */
	if (input_cmd->cache == BSAFE_KEK_CACHE)
		bsafe_free_kekloc(input_cmd->location, cmdio->kSize);
	else if (input_cmd->cache == BSAFE_AKEY_CACHE)
		bsafe_free_akeyloc(input_cmd->location, cmdio->kSize);
	
}

/* ld userkey command  */

void bsafe_ld_userkey_out(bsafe_context_t * cmd_context);

int bsafe_ld_userkey_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_ld_userkey_io_t *cmdio;
	bsafe_ld_userkey_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_LD_USERKEY, 
		arg, sizeof(bsafe_ld_userkey_io_t), (void *) &cmdio,
		sizeof(bsafe_ld_userkey_input_t), (void *) &input_cmd,
		(void *) bsafe_ld_userkey_out,
		sizeof(bsafe_ld_userkey_output_t));

	input_cmd->kekLoc = 0x0;
	input_cmd->akeyLoc = SWAP2BYTES(cmdio->akeyLoc);
		
	COPY_FROM_USER(input_cmd->keyData.ptr, cmdio->keyData, cmdio->keyData_len);
		
	return BSAFE_SUCCESS;
}

void bsafe_ld_userkey_out(bsafe_context_t * cmd_context)
{
	bsafe_ld_userkey_output_t *output_cmd;
	bsafe_ld_userkey_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);


	cmdio->kSize = SWAP4BYTES(output_cmd->kSize);
	/* adjust the used global array */
	bsafe_adjust_used_akeyloc(cmdio->akeyLoc, cmdio->kSize);
	
}


/* ld cfg command  */

int bsafe_ld_cfg_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_ld_cfg_io_t *cmdio;
	bsafe_ld_cfg_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_LD_CFG, 
		arg, sizeof(bsafe_ld_cfg_io_t), (void *) &cmdio,
		sizeof(bsafe_ld_cfg_input_t), (void *) &input_cmd,
		(void *) NULL,
		sizeof(bsafe_ld_cfg_output_t));

	input_cmd->keyType = SWAP2BYTES(cmdio->keyType);
	
	COPY_FROM_USER(input_cmd->keyPolicy, cmdio->keyPolicy, sizeof(input_cmd->keyPolicy));
	COPY_FROM_USER(input_cmd->km_pub, cmdio->km_pub, sizeof(input_cmd->km_pub));
	input_cmd->msgID = SWAP4BYTES(BSAFE_CRYPTTEXT);	
	COPY_FROM_USER(input_cmd->dlies_msg.ptr, cmdio->dlies_msg, cmdio->dlies_msg_len);
	COPY_FROM_USER(input_cmd->authSig_r_ptr, cmdio->authSig_r, 20);
	SWAPBIGNUM(input_cmd->authSig_r_ptr, 20);
	COPY_FROM_USER(input_cmd->authSig_s_ptr, cmdio->authSig_s, 20);
	SWAPBIGNUM(input_cmd->authSig_s_ptr, 20);
		
	return BSAFE_SUCCESS;
}


/* fips clr command  */



int bsafe_fips_clr_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_fips_clr_io_t *cmdio;
	bsafe_fips_clr_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_FIPS_CLR, 
		arg, sizeof(bsafe_fips_clr_io_t), (void *) &cmdio,
		sizeof(bsafe_fips_clr_input_t), (void *) &input_cmd,
		(void *) NULL,
		sizeof(bsafe_fips_clr_output_t));

	input_cmd->clrVector = SWAP4BYTES(cmdio->clrVector);
	input_cmd->numRetry = SWAP2BYTES(cmdio->numRetry);
		
	OS_MEMSET(input_cmd->reserved, 0x00, sizeof(input_cmd->reserved));
	
	return BSAFE_SUCCESS;
}



/* user status command  */

void bsafe_user_status_out(bsafe_context_t * cmd_context);

int bsafe_user_status_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_user_status_io_t *cmdio;
	bsafe_user_status_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_USER_STATUS, 
		arg, sizeof(bsafe_user_status_io_t), (void *) &cmdio,
		sizeof(bsafe_user_status_input_t), (void *) &input_cmd,
		(void *) bsafe_user_status_out,
		sizeof(bsafe_user_status_output_t));

	/* nothig to be done */

	return BSAFE_SUCCESS;
}

void bsafe_user_status_out(bsafe_context_t * cmd_context)
{
	bsafe_user_status_output_t *output_cmd;
	bsafe_user_status_io_t *cmdio;


	get_outputbuf(cmd_context,  
		(void *) &cmdio, 
		(void *) &output_cmd);

	cmdio->salEventStatus = SWAP4BYTES(output_cmd->salEventStatus);

	
}


/* user nop command  */

void bsafe_user_nop_out(bsafe_context_t * cmd_context);

int bsafe_user_nop_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_user_nop_io_t *cmdio;
	bsafe_user_nop_input_t *input_cmd;

	allocate_inputbuf(cmd_context, UHSM_USER_NOP, 
		arg, sizeof(bsafe_user_nop_io_t), (void *) &cmdio,
		sizeof(bsafe_user_nop_input_t), (void *) &input_cmd,
		(void *) bsafe_user_nop_out,
		sizeof(bsafe_user_nop_output_t));

	COPY_FROM_USER(input_cmd->dataIn.ptr, cmdio->dataIn, cmdio->data_len);

	return BSAFE_SUCCESS;
}

void bsafe_user_nop_out(bsafe_context_t * cmd_context)
{
	bsafe_user_nop_output_t *output_cmd;
	bsafe_user_nop_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio, 
		(void *) &output_cmd);

	COPY_TO_USER(cmdio->dataOut, output_cmd->dataOut.ptr, cmdio->data_len);
		
}

/* user random command  */

void bsafe_user_random_out(bsafe_context_t * cmd_context);

int bsafe_user_random_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_user_random_io_t *cmdio;
	bsafe_user_random_input_t *input_cmd;

	allocate_inputbuf(cmd_context, UHSM_USER_RANDOM, 
		arg, sizeof(bsafe_user_random_io_t), (void *) &cmdio,
		sizeof(bsafe_user_random_input_t), (void *) &input_cmd,
		(void *) bsafe_user_random_out,
		sizeof(bsafe_user_random_output_t));

	input_cmd->rngType = SWAP4BYTES(cmdio->rngType);
	

	return BSAFE_SUCCESS;
}

void bsafe_user_random_out(bsafe_context_t * cmd_context)
{
	bsafe_user_random_output_t *output_cmd;
	bsafe_user_random_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio, 
		(void *) &output_cmd);

	COPY_TO_USER(cmdio->random, output_cmd->random, sizeof(output_cmd->random));
		
}


/* user sha1  */

void bsafe_user_sha1_out(bsafe_context_t * cmd_context);

int bsafe_user_sha1_in(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_user_sha1_io_t *cmdio;
	bsafe_user_sha1_input_t *input_cmd;


	allocate_inputbuf(cmd_context, UHSM_USER_SHA1, 
		arg, sizeof(bsafe_user_sha1_io_t), (void *) &cmdio,
		sizeof(bsafe_user_sha1_input_t), (void *) &input_cmd,
		(void *) bsafe_user_sha1_out,
		sizeof(bsafe_user_sha1_output_t));

	input_cmd->mode = SWAP2BYTES(cmdio->mode);
	input_cmd->prevLen = SWAP2BYTES(cmdio->prevLen);

	COPY_FROM_USER(input_cmd->inDigest, cmdio->inDigest, sizeof(input_cmd->inDigest));
	COPY_FROM_USER(input_cmd->data.ptr, cmdio->data, cmdio->data_len);
	
	
	return BSAFE_SUCCESS;
}

void bsafe_user_sha1_out(bsafe_context_t * cmd_context)
{
	bsafe_user_sha1_output_t *output_cmd;
	bsafe_user_sha1_io_t *cmdio;

	get_outputbuf(cmd_context,  
		(void *) &cmdio,
		(void *) &output_cmd);

	COPY_TO_USER(cmdio->outDigest, output_cmd->outDigest, sizeof(output_cmd->outDigest));
		
}


/* le_bignum ioctl */

extern int bsafe_set_swap_bignum(void *ptr);
extern void bsafe_swap_bignum(BSAFE_BIGNUM * src, int size);
int bsafe_le_bignum(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_bignum_prop_io_t cmdio;

	allocate_inputbuf(cmd_context, -1, 
		arg, sizeof(bsafe_bignum_prop_io_t), (void *) &cmdio,
		0, NULL,
		NULL,
		0);

	bsafe_set_swap_bignum(bsafe_swap_bignum);

	PRINTF(("NOTE: le swap for bignum enabled\n")); 

	return BSAFE_SUCCESS;
}

/* bn_2_bignum ioctl */

int bsafe_bn_2_bignum(bsafe_context_t  * cmd_context, void * arg)
{
	bsafe_bignum_prop_io_t cmdio;

	allocate_inputbuf(cmd_context, -1, 
		arg, sizeof(bsafe_bignum_prop_io_t), (void *) &cmdio,
		0, NULL,
		NULL,
		0);

	bsafe_set_swap_bignum(BN2BIGNUM);

	PRINTF(("NOTE: bn 2 bignum swap enabled\n")); 

	return BSAFE_SUCCESS;
}
