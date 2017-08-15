/***************************************************************************
*    Copyright 2013  Broadcom Corporation
*    All Rights Reserved
*    No portions of this material may be reproduced in any form without the
*    written permission of:
*             Broadcom Corporation
*             16215 Alton Parkway
*             P.O. Box 57013
*             Irvine, California 92619-7013
*    All information contained in this document is Broadcom Corporation
*    company private, proprietary, and trade secret.
*
****************************************************************************
*
*    Filename: neon.c
*
****************************************************************************
*    Description:
*
*     Implements the power on stage for NEON on the ARM core.
*
****************************************************************************/

/* ---- Include Files ---------------------------------------------------- */

#include <xdrvTypes.h>
#include <xdrvLog.h>
#include <xdrvNeon.h>

#include <kernelMainWrapper.h>
#include <pmc/pmc_drv.h>
#include <pmc/pmc_cpu_core.h>
#include <pmc/BPCM.h>
#include <bosSleep.h>


/* ---- Public Variables ------------------------------------------------- */
/* ---- Private Constants and Types -------------------------------------- */
/* ---- Private Variables ------------------------------------------------ */
/* ---- Private Function Prototypes -------------------------------------- */
int  cpu_neon_power_up(void);
static void enable_neon(void);
static int  cpu_is_neon_powered_up(void);

/* ---- Functions -------------------------------------------------------- */
static int cpu_is_neon_powered_up(void)
{   
#if defined(CONFIG_BCM963138)
   int ret;

   ARM_CONTROL_REG arm_ctrl;
   ARM_CPUx_PWR_CTRL_REG arm_pwr_ctrl;

   ret = ReadBPCMRegister(PMB_ADDR_AIP, ARMBPCMRegOffset(arm_control), &arm_ctrl.Reg32);
   if(ret)
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "Unable to read ARM control register." ));
      return 0;
   }

   ret = ReadBPCMRegister(PMB_ADDR_AIP, ARMBPCMRegOffset(arm_neon_l2), &arm_pwr_ctrl.Reg32);
   if(ret)
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "Unable to read NEON/L2 control register." ));
      return 0;
   }

   /* check if neon is on and running aready. */
   return ((arm_ctrl.Bits.neon_reset_n == 1) &&
           (arm_pwr_ctrl.Bits.clamp_on == 0) &&
           (arm_pwr_ctrl.Bits.pwr_ok & 0x1) &&
           (arm_pwr_ctrl.Bits.pwr_on & 0x1));             
#elif defined(CONFIG_BCM963148)
   /* NEON core is always powered up on 63148 */
   return 1;
#else
   /* Error condition, no NEON support */
   XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "No NEON support for this CPU" ));
   return 0;
#endif /* defined(CONFIG_BCM963138)  */
}

int cpu_neon_power_up (void)
{   

#if defined(CONFIG_BCM963138)   
   int ret;

   ARM_CONTROL_REG arm_ctrl;
   ARM_CPUx_PWR_CTRL_REG arm_pwr_ctrl;

   ret = ReadBPCMRegister(PMB_ADDR_AIP, ARMBPCMRegOffset(arm_control), &arm_ctrl.Reg32);
   if (ret)
      return ret;

   ret = ReadBPCMRegister(PMB_ADDR_AIP, ARMBPCMRegOffset(arm_neon_l2), &arm_pwr_ctrl.Reg32);
   if (ret)
      return ret;

   /* 1) Power up Neon */
   arm_pwr_ctrl.Bits.pwr_on |= 1;

   ret = WriteBPCMRegister(PMB_ADDR_AIP, ARMBPCMRegOffset(arm_neon_l2), arm_pwr_ctrl.Reg32);
   if (ret)
      return ret;

   do {
      ret = ReadBPCMRegister(PMB_ADDR_AIP, ARMBPCMRegOffset(arm_neon_l2), &arm_pwr_ctrl.Reg32);
      if (ret)
         return ret;
   } while ((arm_pwr_ctrl.Bits.pwr_on_status & 0x1) == 0);

   arm_pwr_ctrl.Bits.pwr_ok |= 1;

   ret = WriteBPCMRegister(PMB_ADDR_AIP, ARMBPCMRegOffset(arm_neon_l2), arm_pwr_ctrl.Reg32);
   if (ret)
      return ret;

   do {
      ret = ReadBPCMRegister(PMB_ADDR_AIP, ARMBPCMRegOffset(arm_neon_l2), &arm_pwr_ctrl.Reg32);
      if (ret)
         return ret;
   } while ((arm_pwr_ctrl.Bits.pwr_ok_status & 0x1) == 0);

   arm_pwr_ctrl.Bits.clamp_on = 0;

   ret = WriteBPCMRegister(PMB_ADDR_AIP, ARMBPCMRegOffset(arm_neon_l2), arm_pwr_ctrl.Reg32);
   if (ret)
      return ret;

   /* 2) De-assert reset to Neon */
   arm_ctrl.Bits.neon_reset_n = 1;

   ret = WriteBPCMRegister(PMB_ADDR_AIP, ARMBPCMRegOffset(arm_control), arm_ctrl.Reg32);
   if (ret)
      return ret;
   XDRV_LOG_INFO(( XDRV_LOG_MOD_XDRV, "NEON powered up" ));

   return 0;
#elif defined(CONFIG_BCM963148)
   /* NEON core is always powered up on 63148 */
   return 0;
#else
   /* Error condition, no NEON support */
   XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "No NEON support for this CPU" ));
   return 1;
#endif /* defined(CONFIG_BCM963138)  */

}


int cpu_neon_power_down(void)
{   
#if defined(CONFIG_BCM963138) 

	int ret;
	ARM_CONTROL_REG arm_ctrl;
	ARM_CPUx_PWR_CTRL_REG arm_pwr_ctrl;

	ret = ReadBPCMRegister(PMB_ADDR_AIP,
			ARMBPCMRegOffset(arm_control),
			&arm_ctrl.Reg32);
	if (ret)
		return ret;

	ret = ReadBPCMRegister(PMB_ADDR_AIP,
			ARMBPCMRegOffset(arm_neon_l2),
			&arm_pwr_ctrl.Reg32);
	if (ret)
		return ret;

	/* 1) assert reset to Neon */
	arm_ctrl.Bits.neon_reset_n = 0;

	ret = WriteBPCMRegister(PMB_ADDR_AIP,
			ARMBPCMRegOffset(arm_control),
			arm_ctrl.Reg32);
	if (ret)
		return ret;

	/* 2) Power down Neon */
	arm_pwr_ctrl.Bits.clamp_on = 1;

	ret = WriteBPCMRegister(PMB_ADDR_AIP,
			ARMBPCMRegOffset(arm_neon_l2),
			arm_pwr_ctrl.Reg32);
	if (ret)
		return ret;

	arm_pwr_ctrl.Bits.pwr_ok &= 0xe;

	ret = WriteBPCMRegister(PMB_ADDR_AIP,
			ARMBPCMRegOffset(arm_neon_l2),
			arm_pwr_ctrl.Reg32);
	if (ret)
		return ret;

	do {
		ret = ReadBPCMRegister(PMB_ADDR_AIP,
				ARMBPCMRegOffset(arm_neon_l2),
				&arm_pwr_ctrl.Reg32);
		if (ret)
			return ret;
	} while ((arm_pwr_ctrl.Bits.pwr_ok_status & 0x1) == 1);

	arm_pwr_ctrl.Bits.pwr_on &= 0xe;

	ret = WriteBPCMRegister(PMB_ADDR_AIP,
			ARMBPCMRegOffset(arm_neon_l2),
			arm_pwr_ctrl.Reg32);
	if (ret)
		return ret;

	do {
		ret = ReadBPCMRegister(PMB_ADDR_AIP,
				ARMBPCMRegOffset(arm_neon_l2),
				&arm_pwr_ctrl.Reg32);
		if (ret)
			return ret;
	} while ((arm_pwr_ctrl.Bits.pwr_on_status & 0x1) == 1);

	return 0;
#elif defined(CONFIG_BCM963148)
   /* NEON core is always powered up on 63148 */
   return 0;
#else
   /* Error condition, no NEON support */
   XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "No NEON support for this CPU" ));
   return 1;
#endif 
}


static void enable_neon(void)
{
   /* Enable NEON using CP15 registers */
   asm volatile ( "mrc p15, 0, r0, c1, c0, 2" );
   asm volatile ( "orr r0, r0, #0x00f00000" );
   asm volatile ( "mcr p15, 0, r0, c1, c0, 2" );
   asm volatile ( "isb" );
   asm volatile ( "mov r0, #0x40000000" );
   asm volatile ( ".word 0xeee80a10" ); //msr fpexc, r0
   
}

static void disable_neon ( void )
{
   asm volatile ( "mov r0, #0x00000000" );
   asm volatile ( ".word 0xeee80a10" ); //msr fpexc, r0
}


/*
*****************************************************************************
** FUNCTION:   xdrvNeonPowerup
**
** PURPOSE:    Powerup and enable NEON core
**
** PARAMETERS: None.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS xdrvNeonPowerup( void )
{
   int ret;

   /* check if neon is on and running aready. */
   if(cpu_is_neon_powered_up())
   {
      XDRV_LOG_NOTICE(( XDRV_LOG_MOD_XDRV, "NEON is running." ));
   }
   else
   {
      if( (ret = cpu_neon_power_up()) )
      {
         XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "Couldn't power up neon - %d \n", ret));
         return XDRV_STATUS_ERR;
      }
   }

   /* Make sure the NEON coprocessor is initialized after power-on */
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Enabling NEON core..." ));
   enable_neon();
   XDRV_LOG_NOTICE(( XDRV_LOG_MOD_XDRV, "NEON enabled." ));

   return ( XDRV_STATUS_OK );

}

/*
*****************************************************************************
** FUNCTION:   xdrvNeonPowerDown
**
** PURPOSE:    Powerdown and disable NEON core
**
** PARAMETERS: None.
**
** RETURNS:    XDRV_STATUS_OK or error code.
**
** NOTE:
*****************************************************************************
*/
XDRV_STATUS xdrvNeonPowerDown( void )
{
   int ret;

   /* Make sure the NEON coprocessor is initialized after power-on */
   XDRV_LOG_DBG(( XDRV_LOG_MOD_XDRV, "Disabling NEON core..." ));
   disable_neon();

   if( (ret = cpu_neon_power_down()) )
   {
      XDRV_LOG_ERROR(( XDRV_LOG_MOD_XDRV, "Couldn't power up neon - %d \n", ret));
      return XDRV_STATUS_ERR;
   } 
   
   return ( XDRV_STATUS_OK );
}


