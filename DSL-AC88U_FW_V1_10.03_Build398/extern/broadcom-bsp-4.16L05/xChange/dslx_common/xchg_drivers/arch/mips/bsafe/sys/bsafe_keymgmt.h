/*
 * Bsafe uHSM key management wrapper definitions
 *
 * $Copyright Open Broadcom$      
 * $Id: bsafe_context.h,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#ifndef __BSAFE_KEYMGMT_H_
#define __BSAFE_KEYMGMT_H_

unsigned short bsafe_get_free_kekloc(int req_size);
void bsafe_adjust_used_kekloc(unsigned short kekloc, unsigned int ksize);
void bsafe_free_kekloc(unsigned short kekloc, unsigned int ksize);
unsigned short bsafe_get_free_akeyloc(void);
void bsafe_adjust_used_akeyloc(unsigned short akeyloc, unsigned int ksize);
void bsafe_free_akeyloc(unsigned short akeyloc, unsigned int ksize);

#endif /* __BSAFE_KEYMGMT_H_ */
