/*
   Copyright (c) 2013 Broadcom Corporation
   All Rights Reserved

    <:label-BRCM:2013:DUAL/GPL:standard
    
    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License, version 2, as published by
    the Free Software Foundation (the "GPL").
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.
    
    
    A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
    writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
    
    :>
*/
/******************************************************************************/
/*                                                                            */
/* File Description:                                                          */
/*                                                                            */
/* This file contains the implementation for Broadcom's QuadPhy block         */
/*                                                                            */
/******************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Include files                                                             */
/*                                                                           */
/*****************************************************************************/
#include "mdio_drv_impl1.h"
#include "egphy_drv_impl1.h"
#include "rdp_map.h"

/*
 * structure describing the EGPHY OUT register
 */
#pragma pack(push,1)
typedef struct
{
	 uint32_t	reserved1:17;//	 Reserved bits must be written with 0.  A read returns an unknown value.
	 uint32_t	PHY_TEST_EN:1;//	 Phy test enable	0x1 = Test_mode  	Reset value is 0x0.
	 uint32_t	PHYA:5;//	 phyaReset value is 0x1.
	 uint32_t	GPHY_CK25_DIS:1;//	 gphy_ck25_disable 	Reset value is 0x0.
	 uint32_t	IDDQ_BIAS:1;//	 i_mac_gphy_cfg_iddq_bias Reset value is 0x1.
	 uint32_t	DLL_EN:1;//	 i_mac_gphy_cfg_ext_force_dll_en Reset value is 0x0.
	 uint32_t	PWRDWN:4;//	 i_mac_gphy_cfg_ext_pwrdown	Reset value is 0xf.
	 uint32_t	reserved0:1;//	 Reserved bit must be written with 0.  A read returns an unknown value.
	 uint32_t	RST:1;// i_mac_gphy_cfg_reset_b	0x1 = not_rst Reset value is 0x0.
}EGPHY_GPHY_OUT;
#pragma pack(pop)


static void changeEGphyRDBAccess(uint32_t phyID,uint32_t enable)
{

	uint16_t value = 0x0f00 ;
	if (enable)
	{
		value	|= (uint16_t)EXP_RDP_ENABLE_REG;

		if( mdio_write_c22_register(MDIO_EGPHY,phyID,MII_RDB_ENABLE_OFFSET,value) == MDIO_ERROR)
		{

			return ;
		}
		if( mdio_write_c22_register(MDIO_EGPHY,phyID,MII_RDB_VALUE_OFFSET,0) == MDIO_ERROR)
		{
			/*put some log error*/
			return ;
		}


	}
	else
	{

		if( mdio_write_c22_register(MDIO_EGPHY,phyID,0x1e,0x0087) == MDIO_ERROR)
		{

			return ;
		}
		if( mdio_write_c22_register(MDIO_EGPHY,phyID,0x1f,0x8000) == MDIO_ERROR)
		{
			/*put some log error*/
			return ;
		}

	}

}
/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   egphy_reset									                              */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   reset the EGPHY IP in 			         				                  */
/*                                                                            */
/* Abstract:                                                                  */
/*   initialize the EGPHY block, must be called before any MAC				  */
/* 	 operations                                                               */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/******************************************************************************/
void egphy_reset(uint32_t port_map)
{
	/*read the egphy register*/
	EGPHY_GPHY_OUT gPhyOut;

	READ_32(EGPHY_RDP_UBUS_MISC_EGPHY_GPHY_OUT,gPhyOut);

	gPhyOut.RST			=	1;
	gPhyOut.IDDQ_BIAS	=	1;
	gPhyOut.PWRDWN		=	0xf;

	WRITE_32(EGPHY_RDP_UBUS_MISC_EGPHY_GPHY_OUT,gPhyOut);

	udelay(50);

	gPhyOut.IDDQ_BIAS	=	0;

	WRITE_32(EGPHY_RDP_UBUS_MISC_EGPHY_GPHY_OUT,gPhyOut);

	gPhyOut.PHYA		= 	1;
	gPhyOut.PWRDWN		=	~port_map & 0xf;

	WRITE_32(EGPHY_RDP_UBUS_MISC_EGPHY_GPHY_OUT,gPhyOut);

	//udelay(50);

//	/*set the value of PHYA to 1
//	 * all access to phy addresses is depended by this value, it's the offset
//	 * from the MDIO address*/
//	gPhyOut.RST			=	0;
//	WRITE_32(EGPHY_RDP_UBUS_MISC_EGPHY_GPHY_OUT,gPhyOut);
//	udelay(200);
//
//	gPhyOut.RST			=	1;
//	WRITE_32(EGPHY_RDP_UBUS_MISC_EGPHY_GPHY_OUT,gPhyOut);
//	udelay(50);

	/*phy is ready to go!*/
}


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   egphy_auto_enable_extra_config			                                  */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*  configure the phy to auto negotiation mode				                  */
/*                                                                            */
/* Abstract:                                                                  */
/*   configure the phy to work in auto negotiation  with respect to the		  */
/*   requested rate limit								                      */
/*                                                                            */
/* Input:                                                                     */
/*     phyID	-	if of phy in MDIO bus                                     */
/*		mode	- rate limit to advertise ( 100,1000)						  */
/* Output:                                                                    */
/*                                                                            */
/******************************************************************************/
void egphy_auto_enable_extra_config(uint32_t phyID, int mode)
{
	egphy_write_register(phyID,RDB_REG_ACCESS|CORE_SHD1C_0D,CORE_SHD_BICOLOR_LED0);
	egphy_write_register(phyID,RDB_REG_ACCESS|CORE_SHD18_111,FORCE_AUTO_MDIX);
}
/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   egphy_read_register	     				    		                  */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*  function to retrieve a phy register content				                  */
/*                                                                            */
/* Abstract:                                                                  */
/*   get the current status of the phy in manners of rate and duplex          */
/*                                                                            */
/* Input:                                                                     */
/*     phyID	-	                                     					  */
/*     regOffset -															  */
/* Output:																	  */
/*																			  */
/* Return:                                                                    */
/* 	  read value -			                                                   */
/******************************************************************************/
uint16_t egphy_read_register(uint32_t phyID,uint32_t regOffset)
{
	uint16_t value = 0;
	int 	rdbEnabled = regOffset & 0x8000;
	if(rdbEnabled)
	{
		changeEGphyRDBAccess(phyID,1);
		if ( mdio_write_c22_register(MDIO_EGPHY,phyID,0x1e,regOffset) == MDIO_ERROR)
		{
				printk("failed to write register phy %ul rdb regOffset %ul\n",phyID,regOffset);
				value=-1;
		}
	}

	if (mdio_read_c22_register(MDIO_EGPHY,phyID,rdbEnabled ? 0x1f : regOffset,&value) == MDIO_ERROR)
	{
		printk("failed to read register phy %ul regOffset %ul\n",phyID,regOffset);
		value = 0;
	}

	if(rdbEnabled)
		changeEGphyRDBAccess(phyID,0);

	return value;
}
EXPORT_SYMBOL(egphy_read_register);

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   egphy_write_register						    		                  */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*  function to set a phy register content				       		          */
/*                                                                            */
/* Abstract:                                                                  */
/*   get the current status of the phy in manners of rate and duplex          */
/*                                                                            */
/* Input:                                                                     */
/*     phyID	-	                                     					  */
/*     regOffset -															  */
/*     regValue -															  */
/* Output:																	  */
/*																			  */
/* Return:                                                                    */
/******************************************************************************/
int32_t egphy_write_register(uint32_t phyID,uint32_t regOffset,uint16_t regValue)
{
	int32_t ret = 0 ;
	int 	rdbEnabled = regOffset & 0x8000;

	if(rdbEnabled)
	{
		changeEGphyRDBAccess(phyID,1);
		if (mdio_write_c22_register(MDIO_EGPHY,phyID,0x1e,regOffset)== MDIO_ERROR)
		{
				printk("failed to write register phy %ul rdb regOffset %ul\n",phyID,regOffset);
				ret=-1;
		}
	}

	ret=mdio_write_c22_register(MDIO_EGPHY,phyID,rdbEnabled ? 0x1f : regOffset,regValue);
	if (ret== MDIO_ERROR)
	{
			printk("failed to write register phy %ul regOffset %ul\n",phyID,regOffset);
			ret=-1;
	}
	if(rdbEnabled)
		changeEGphyRDBAccess(phyID,0);

	return ret;
}
EXPORT_SYMBOL(egphy_write_register);
