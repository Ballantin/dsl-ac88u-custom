/*
 * Bsafe uHSM core driver interface definitions
 *
 * $Copyright Open Broadcom$      
 * $Id: bsafe_core_if.h,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#ifndef _BSAFE_CORE_IF_H_
#define _BSAFE_CORE_IF_H_

/* dev_context ..just used as pass through for the device ...visible only in the wrapper */
int bsafe_initialize(void * dev_context, int reset);
int bsafe_clean(void);
int bsafe_ioctl(void * dev_context, int command, void *cmdio);


int bsafe_write_to_device(void * dev_context,unsigned char *ptr,int size);	
#define WRITE_TO_DEVICE	 bsafe_write_to_device

int bsafe_read_from_device(void * dev_context, unsigned char *ptr,int size);
#define READ_FROM_DEVICE bsafe_read_from_device

/* core interface functions for wrapper */
int bsafe_write_uint32(void * dev_context, int address, int *data_in);
#define WRITE_UINT32_TO_DEVICE	 bsafe_write_uint32
int bsafe_read_uint32(void * dev_context, int address, int *data_out);
#define READ_UINT32_FROM_DEVICE	 bsafe_read_uint32

#endif /* _BSAFE_CORE_IF_H_ */
