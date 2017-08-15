/*
 * Bsafe uHSM regs definitions
 *
 * $Copyright Open Broadcom$      
 * $Id: bsafe_regs.h,v 1.1.1.1 2004/07/20 01:07:36 gigis Exp $
 */

#ifndef __BSAFE_REGS_H_
#define __BSAFE_REGS_H_

#define BSAFE_BASE_OFFSET       0x200
#define BSAFE_MAX_REG_SIZE      4
#define BSAFE_MAX_REGS  8

typedef enum bsafe_glb_register {
	GLB_TIMER=0x200,
	GLB_PRESCALE=0x204,
	GLB_UHSM_CFG=0x208,
	GLB_PROD_CFG=0x20c,
	GLB_DEV_STATUS=0x210,
	GLB_CMD_CTL=0x214,
	GLB_CMD_DATA_IN=0x218,
	GLB_CMD_DATA_OUT=0x21c,
	GLB_INT_CTRL=0x220
} bsafe_glb_register_t;

typedef enum bsafe_glb_cmd_cfg {
	GLB_CMD_CTL_CMD_IRDY=1<<0,
	GLB_CMD_CTL_CMD_ISYNC=1<<1,
	GLB_CMD_CTL_CMD_ORDY=1<<2,
	GLB_CMD_CTL_CMD_OSYNC=1<<3,
    GLB_CMD_CTL_CMD_SOFTRESET=1<<7
    
} bsafe_glb_cmd_cfg_t;

typedef enum bsafe_glb_dev_status_t {

	GLB_DEV_STATUS_INIT_DONE = 1<<4
    
} bsafe_glb_dev_status_t;

#endif /* __BSAFE_REGS_H_ */
