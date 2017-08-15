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
/* This file contains the implementation for 6838 Common Phys Driver          */
/*                                                                            */
/******************************************************************************/

/*****************************************************************************/
/*                                                                           */
/* Include files                                                             */
/*                                                                           */
/*****************************************************************************/
#include "phys_common_drv.h"
#include "egphy_drv_impl1.h"
#include "extphy_drv_impl1.h"
#include "mdio_drv_impl1.h"
#include "boardparms.h"

typedef struct S_PhyCache
{
	uint32_t	phyId;
	uint32_t	ifFlags;
	uint32_t	mdio;
	uint32_t	phyvalid;
}S_PhyCache;

static S_PhyCache 	phyAddrCache[BP_MAX_SWITCH_PORTS] = {[0 ... (BP_MAX_SWITCH_PORTS-1)] = {0,0,0,0} };
static uint32_t 	PhyCacheInitialized = 0;
static const ETHERNET_MAC_INFO*    pE;

static phy_rate_t pcs_get_line_rate_and_duplex(void)
{
	uint16_t status1_reg;

	/* Read PCS_STATUS1_REG */
	if (mdio_read_c22_register(MDIO_AE, PCS_ADDRESS, PCS_STATUS1_REG, &status1_reg)
			== MDIO_ERROR)
	{
		/*put some log error*/
		return PHY_RATE_ERR;
	}

	/* Get link state */
	if (status1_reg & PCS_LINK )
	{
		if ((status1_reg & PCS_1000FULL) == PCS_1000FULL)
			return PHY_RATE_1000_FULL;

		if ((status1_reg & PCS_100FULL) == PCS_100FULL)
			return PHY_RATE_100_FULL;

		if ((status1_reg & PCS_10FULL) == PCS_10FULL)
			return PHY_RATE_10_FULL;

		if ((status1_reg & PCS_100HALF) == PCS_100HALF)
			return PHY_RATE_100_HALF;

		if ((status1_reg & PCS_10HALF) == PCS_10HALF)
			return PHY_RATE_10_HALF;
	}
	else
		return PHY_RATE_LINK_DOWN;
}

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   phy_get_link_state							                              */
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
static int32_t phy_get_link_state(uint32_t phyID,MDIO_TYPE mdioType)
{
	uint16_t statReg;
	uint32_t retCode;
	phy_rate_t pcsLink;

	/* PCS handle */
	if (mdioType == MDIO_AE)
	{
		pcsLink = pcs_get_line_rate_and_duplex();
		if (pcsLink == PHY_RATE_LINK_DOWN)		
			return PHY_LINK_OFF ;
		else if (pcsLink == PHY_RATE_ERR)		
			return PHY_RATE_ERR;
		else		
			return PHY_LINK_ON;
	}

	if (mdio_read_c22_register(mdioType, phyID, MII_BMSR, &statReg)
			== MDIO_ERROR)
	{
		/*put some log error*/
		return -1;
	}
	/*check the link stat bit*/
	if (statReg & BMSR_LSTATUS)
	{
		retCode = PHY_LINK_ON;
	}
	else
	{
		retCode = PHY_LINK_OFF;
	}
	return retCode;
}

static inline void fill_bp_phy_cache(void)
{
	uint32_t			iter;

	/*already initialized*/
	if(PhyCacheInitialized)
		return;

	if ( (pE = BpGetEthernetMacInfoArrayPtr()) == NULL )
	{
		printk("ERROR:BoardID not Set in BoardParams\n");
		return ;
	}

	for( iter = 0 ; iter < BP_MAX_SWITCH_PORTS; iter++)
	{
	    /*is port enabled?*/
	    if(pE[0].sw.port_map & (1<<iter))
	    {
            phyAddrCache[iter].phyId 	= pE[0].sw.phy_id[iter] & BCM_PHY_ID_M;
            phyAddrCache[iter].ifFlags	= pE[0].sw.phy_id[iter] & MAC_IFACE;
            if(pE[0].sw.phy_id[iter] & PHY_EXTERNAL)
                phyAddrCache[iter].mdio = MDIO_EXT;
            else if (phyAddrCache[iter].ifFlags == MAC_IF_SERDES)
            {
                phyAddrCache[iter].mdio = MDIO_AE;
                /* for case of working mac to mac PHYCFG_VALID will not be configured in BP */
                if(pE[0].sw.phy_id[iter] & PHYCFG_VALID)
                    phyAddrCache[iter].phyvalid = 1;
            }                
            else
                phyAddrCache[iter].mdio = MDIO_EGPHY;
	    }
	}

	PhyCacheInitialized  = 1;
}

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   phy_reset									                              */
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
void phy_reset(uint32_t port_map)
{
	/*here you do some initialization needed before configuring the phy*/
	egphy_reset(port_map);
}
EXPORT_SYMBOL(phy_reset);
/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   phy_auto_enable                                                          */
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
void phy_auto_enable(uint32_t macId)
{
    uint16_t anAdvBits;
    uint16_t ctrlReg;
    uint32_t	phyId;
    MDIO_TYPE	mdioType;
    uint32_t	mode;


    fill_bp_phy_cache();
    if(!( pE[0].sw.port_map & (1<<macId) ))
    {
        printk("trying to access to phy of wrong Mac = %d\n",macId);
        return;
    }
    phyId = phyAddrCache[macId].phyId;
    mdioType = phyAddrCache[macId].mdio;
    mode   = phyAddrCache[macId].ifFlags;

    /* configure serdes based phy */	
    if (mdioType == MDIO_AE)
        return;

    /*reset phy*/
    if (mdio_write_c22_register(mdioType, phyId, MII_BMCR, BMCR_RESET)
            == MDIO_ERROR)
    {
        /*put some log error*/
        return;
    }

    udelay(300);
    /*write the autoneg bits*/
    anAdvBits = ADVERTISE_CSMA | ADVERTISE_10HALF | ADVERTISE_10FULL
        | ADVERTISE_100HALF | ADVERTISE_100FULL | ADVERTISE_PAUSE_CAP;
    if (mdio_write_c22_register(mdioType, phyId, MII_ADVERTISE, anAdvBits)
            == MDIO_ERROR)
    {
        /*put some log error*/
        return;
    }
    mdio_read_c22_register(mdioType, phyId, MII_CTRL1000, &anAdvBits);
    /* Favor clock master for better compatibility when in EEE */
    anAdvBits = ADVERTISE_REPEATER;
    if ( mode == MAC_IF_GMII ||  mode == MAC_IF_RGMII || mode == MAC_IF_SERDES)
    {
        /*if 1000 is supported also advertise 1000 capability*/
        anAdvBits |= ADVERTISE_1000FULL ;
    }
    if (mdio_write_c22_register(mdioType, phyId, MII_CTRL1000, anAdvBits)
                    == MDIO_ERROR)
    {
        /*put some log error*/
        return;
    }
    /*initiate autonegotiation*/
    if (mdio_read_c22_register(mdioType, phyId, MII_BMCR, &ctrlReg)
            == MDIO_ERROR)
    {
        /*put some log error*/
        return;
    }
    ctrlReg |= BMCR_ANENABLE | BMCR_ANRESTART;
    if (mdio_write_c22_register(mdioType, phyId, MII_BMCR, ctrlReg)
            == MDIO_ERROR)
    {
        /*put some log error*/
        return;
    }

    /*now do some extra configuration according to the phy type*/
    switch(mdioType)
    {
        case MDIO_EGPHY:
            egphy_auto_enable_extra_config(phyId,mode);
            break;
        case MDIO_EXT:
            extphy_auto_enable_extra_config(phyId, mode);
            break;
        default:
            /*nothing*/
            break;
    }
}
EXPORT_SYMBOL(phy_auto_enable);

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   phy_shut_down								                              */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*  shut down the phy                       				                  */
/*                                                                            */
/* Abstract:                                                                  */
/*  shut down the phy                       				                  */
/*                                                                            */
/* Input:                                                                     */
/*     macID	                                                              */
/* Output:                                                                    */
/*                                                                            */
/******************************************************************************/
void phy_shut_down(uint32_t macId)
{
	MDIO_TYPE	mdioType;
	uint32_t 	phyId ;


	fill_bp_phy_cache();
	if(!( pE[0].sw.port_map & (1<<macId) ))
	{
		printk("trying to access to phy of wrong Mac = %d\n",macId);
		return;
	}
	phyId	=  phyAddrCache[macId].phyId;
	mdioType = phyAddrCache[macId].mdio;

	if (mdio_write_c22_register(mdioType, phyId  , MII_BMCR, BMCR_PDOWN)
				== MDIO_ERROR)
	{
		/*put some log error*/
		return;
	}

}
EXPORT_SYMBOL(phy_shut_down);

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   phy_get_line_rate_and_duplex                                             */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*  function to retrieve the current status of the phy		                  */
/*                                                                            */
/* Abstract:                                                                  */
/*   get the current status of the phy in manners of rate and duplex          */
/*                                                                            */
/* Input:                                                                     */
/*     phyID	-	if of phy in MDIO bus                                     */
/* Output:																	  */
/*																			  */
/* Return:                                                                    */
/* 	  GPHY_RATE - enumation                                                   */
/******************************************************************************/
phy_rate_t phy_get_line_rate_and_duplex(uint32_t macId)
{
	uint32_t	phyId;
	MDIO_TYPE	mdioType;
	uint16_t	miiCtrl;
	uint16_t	ctrl1000;
	uint16_t	stat1000;
	uint16_t	lpa;
	uint16_t    adv;
	uint32_t 	result = PHY_RATE_LINK_DOWN;



	fill_bp_phy_cache();
	if(!( pE[0].sw.port_map & (1<<macId) ))
    {
    	printk("trying to access to phy of wrong Mac = %d\n",macId);
    	return PHY_RATE_ERR;
    }
	phyId 	 = phyAddrCache[macId].phyId;
	mdioType = phyAddrCache[macId].mdio;

	/* PCS handle */
	if (mdioType == MDIO_AE)
		return	pcs_get_line_rate_and_duplex();

	if (phy_get_link_state(phyId,mdioType) != PHY_LINK_ON)
	{
		return PHY_RATE_LINK_DOWN;
	}
	/*check if autoneg is enabled*/
	if (mdio_read_c22_register(mdioType, phyId, MII_BMCR, &miiCtrl)
			== MDIO_ERROR)
	{
		/*put some log error*/
		return PHY_RATE_ERR;
	}
	/*if autoneg enabled*/
	if (miiCtrl & BMCR_ANENABLE)
	{
		/*read the status register*/
		if (mdio_read_c22_register(mdioType, phyId, MII_BMSR, &miiCtrl)
				== MDIO_ERROR)
		{
			/*put some log error*/
			return PHY_RATE_ERR;
		}
		/*if autoneg didn't complete leave the function*/
		if (!(miiCtrl & BMSR_ANEGCOMPLETE))
		{
			return PHY_RATE_LINK_DOWN;
		}
		/*first read the 1000 ctrl reg*/
		if (mdio_read_c22_register(mdioType, phyId, MII_CTRL1000, &ctrl1000)
				== MDIO_ERROR)
		{
			/*put some log error*/
			return PHY_RATE_ERR;
		}

		/* read the 1000 stat reg*/
		if (mdio_read_c22_register(mdioType, phyId, MII_STAT1000, &stat1000)
				== MDIO_ERROR)
		{
			/*put some log error*/
			return PHY_RATE_ERR;
		}
		/*aligne ctrl1000 with stat1000*/
		stat1000 &= ctrl1000 << 2;

		/* read the read partner reg*/
		if (mdio_read_c22_register(mdioType, phyId, MII_LPA, &lpa)
				== MDIO_ERROR)
		{
			/*put some log error*/
			return PHY_RATE_ERR;
		}
		/* read the read advertise reg*/
		if (mdio_read_c22_register(mdioType, phyId, MII_ADVERTISE, &adv)
				== MDIO_ERROR)
		{
			/*put some log error*/
			return PHY_RATE_ERR;
		}
		/*match lpa with adv*/
		lpa &= adv;

		/*check speeds*/
		if (stat1000 & (LPA_1000FULL | LPA_1000HALF))
		{
			if (stat1000 & LPA_1000FULL)
			{
				result = PHY_RATE_1000_FULL;
			}
			else
			{
				result = PHY_RATE_1000_HALF;
			}
		}
		else if (lpa & (LPA_100FULL | LPA_100HALF))
		{
			if (lpa & LPA_100FULL)
			{
				result = PHY_RATE_100_FULL;
			}
			else
			{
				result = PHY_RATE_100_HALF;
			}
		}
		else if (lpa & (LPA_10FULL | LPA_10HALF))
		{
			if (lpa & LPA_10FULL)
			{
				result = PHY_RATE_10_FULL;
			}
			else
			{
				result = PHY_RATE_10_HALF;
			}
		}

	}
	else // no autoneg
	{
		/*read the status register*/
		if (mdio_read_c22_register(mdioType, phyId, MII_BMCR, &miiCtrl)
				== MDIO_ERROR)
		{
			/*put some log error*/
			return PHY_RATE_ERR;
		}
		if (miiCtrl & BMCR_SPEED1000)
		{
			result =
					(miiCtrl & BMCR_FULLDPLX) ?
							PHY_RATE_1000_FULL : PHY_RATE_1000_HALF;
		}
		else if (miiCtrl & BMCR_SPEED100)
		{
			result =
					(miiCtrl & BMCR_FULLDPLX) ?
							PHY_RATE_100_FULL : PHY_RATE_100_HALF;
		}
		else
		{
			result =
					(miiCtrl & BMCR_FULLDPLX) ?
							PHY_RATE_10_FULL : PHY_RATE_10_HALF;
		}
	}
	return result;
}
EXPORT_SYMBOL(phy_get_line_rate_and_duplex);

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   phy_read_register							    		                  */
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
uint16_t phy_read_register(uint32_t phyId,uint16_t regOffset)
{

	if ( phyId & PHY_EXTERNAL)
	{
		return extphy_read_register( phyId & BCM_PHY_ID_M ,regOffset );
	}
	else
	{
		return egphy_read_register( phyId & BCM_PHY_ID_M ,regOffset );
	}
}
EXPORT_SYMBOL(phy_read_register);

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   phy_write_register							    		                  */
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
int32_t phy_write_register(uint32_t phyId,uint16_t regOffset,uint16_t regValue)
{

	if ( phyId & PHY_EXTERNAL)
	{
		extphy_write_register(phyId & BCM_PHY_ID_M ,regOffset,regValue );
	}
	else
	{
		return egphy_write_register( phyId & BCM_PHY_ID_M ,regOffset,regValue );
	}
	return 0;
}
EXPORT_SYMBOL(phy_write_register);

uint32_t phy_link_status(uint32_t macId)
{
	uint32_t	phyId;
	MDIO_TYPE	mdioType;

	fill_bp_phy_cache();
	if(!( pE[0].sw.port_map & (1<<macId) ))
	{
	 	printk("trying to access to phy of wrong Mac = %d\n",macId);
	 	return PHY_RATE_ERR;
	}
	phyId = phyAddrCache[macId].phyId;
	mdioType = phyAddrCache[macId].mdio;

    return phy_get_link_state(phyId, mdioType);
}
EXPORT_SYMBOL(phy_link_status);

