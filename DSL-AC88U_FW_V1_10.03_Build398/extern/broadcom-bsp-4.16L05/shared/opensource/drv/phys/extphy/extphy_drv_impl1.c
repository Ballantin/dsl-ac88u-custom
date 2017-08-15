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

/*
 * This file contains the implementation for external RGMII Phy
 * In 6838 SV Boards there is B50612E 10/100/1000 Gigabit Ethernet Transceiver
 */

/*****************************************************************************/
/*                                                                           */
/* Include files                                                             */
/*                                                                           */
/*****************************************************************************/
#include "extphy_drv_impl1.h"
#include "mdio_drv_impl1.h"
#include "boardparms.h"


/*
 * Register Defines
 */
#define REG_SHADOW_ACCESS       0x18
#define SHADOW_MISC_ACCESS_MASK 0x7
#define SHADOW_SELECTOR_MASK    0x7
#define SHADOW_ACCESS_SHIFT     12
#define SHADOW_SELECTOR_SHIFT   0

#define MISC_ACCESS_RGMII_ENABLE           0x0080
#define MISC_ACCESS_RGMII_RXD_TO_RXC_SKEW  0x0100
#define MISC_ACCESS_WRITE_MASK_ACCESS      0x8000

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   egPhyReset									                              */
/*   extphy_reset								                              */
/*   egPhyReset								      */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   reset the ext PHY IP 			         				                  */
/*                                                                            */
/* Abstract:                                                                  */
/*   initialize the ext PHY block, must be called before any MAC			  */
/* 	 operations                                                               */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/******************************************************************************/
void extphy_reset(void)
{
        /*here you can put pre-initialization*/
}


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   extphy_auto_enable_extra_config			                              */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*  configure the phy to auto negotiation mode                                */
/*                                                                            */
/* Abstract:                                                                  */
/*   configure the phy to work in auto negotiation  with respect to the       */
/*   requested rate limit                                                     */
/*                                                                            */
/* Input:                                                                     */
/*     phyID    -       if of phy in MDIO bus                                 */
/*     mode    - rate limit to advertise ( 100,1000)                          */
/* Output:                                                                    */
/*                                                                            */
/******************************************************************************/
void extphy_auto_enable_extra_config(uint32_t phyID, int mode)
{
        uint16_t value = 0;
        int32_t  ret = 0;

        /*
           Form the code for shadow 111 selection
        */
        value = (SHADOW_MISC_ACCESS_MASK << SHADOW_ACCESS_SHIFT) | (SHADOW_SELECTOR_MASK << SHADOW_SELECTOR_SHIFT) ; /* bit 15 will be 0 as required */

	ret=mdio_write_c22_register(MDIO_EXT,phyID, REG_SHADOW_ACCESS,value);
	if (ret== MDIO_ERROR)
	{
	   printk("failed to write register phy %ul regOffset %ul\n",phyID,REG_SHADOW_ACCESS);
	   return;
	}

        if (mdio_read_c22_register(MDIO_EXT,phyID,REG_SHADOW_ACCESS,&value) == MDIO_ERROR)
        {
           printk("failed to read register phy %ul regOffset %ul\n",phyID,REG_SHADOW_ACCESS);
           return;
        }

        value |= (MISC_ACCESS_RGMII_ENABLE | MISC_ACCESS_RGMII_RXD_TO_RXC_SKEW | MISC_ACCESS_WRITE_MASK_ACCESS) ;

	ret=mdio_write_c22_register(MDIO_EXT,phyID, REG_SHADOW_ACCESS,value);
	if (ret== MDIO_ERROR)
	{
	   printk("failed to write register phy %ul regOffset %ul\n",phyID,REG_SHADOW_ACCESS);
	   return;
	}

}

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   extphy_read_register						    		                  */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*  function to retrieve a phy register content                               */
/*                                                                            */
/* Abstract:                                                                  */
/*   get the current status of the phy in manners of rate and duplex          */
/*                                                                            */
/* Input:                                                                     */
/*     phyID    -                                                             */
/*     regOffset -                                                            */
/* Output:                                                                    */
/*                                                                            */
/* Return:                                                                    */
/*        read value -                                                        */
/******************************************************************************/
uint16_t extphy_read_register(uint32_t phyID,uint32_t regOffset)
{
        uint16_t value = 0;

        if (mdio_read_c22_register(MDIO_EXT,phyID,regOffset,&value) == MDIO_ERROR)
        {
                printk("failed to read register phy %ul regOffset %ul\n",phyID,regOffset);
                value = 0;
        }

        return value;
}
EXPORT_SYMBOL(extphy_read_register);
/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   extphy_write_register						    		                  */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*  function to set a phy register content                                    */
/*                                                                            */
/* Abstract:                                                                  */
/*   get the current status of the phy in manners of rate and duplex          */
/*                                                                            */
/* Input:                                                                     */
/*     phyID    -                                                             */
/*     regOffset -                                                            */
/*     regValue -							      */
/* Output:								      */
/*									      */
/* Return:                                                                    */
/******************************************************************************/
int32_t extphy_write_register(uint32_t phyID,uint32_t regOffset,uint16_t regValue)
{
	int32_t ret;


	ret=mdio_write_c22_register(MDIO_EXT,phyID, regOffset,regValue);
	if (ret== MDIO_ERROR)
	{
			printk("failed to write register phy %ul regOffset %ul\n",phyID,regOffset);
			ret=-1;
	}

	return ret;
}
EXPORT_SYMBOL(extphy_write_register);
