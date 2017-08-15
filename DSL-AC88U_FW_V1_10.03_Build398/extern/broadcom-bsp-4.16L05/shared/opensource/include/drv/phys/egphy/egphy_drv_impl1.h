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
/* This file contains the definition for Broadcom's 40nm EGPHY IP	          */
/*                                                                            */
/******************************************************************************/
#ifndef __EGPHY_DRV_IMPL1_H
#define __EGPHY_DRV_IMPL1_H
/*****************************************************************************/
/*                                                                           */
/* Include files                                                             */
/*                                                                           */
/*****************************************************************************/
#include "bl_os_wraper.h"
#include "access_macros.h"
#include "phys_common_drv.h"

#define MII_RDB_ENABLE_OFFSET			0x17
#define MII_RDB_VALUE_OFFSET			0x15
#define EXP_RDP_ENABLE_REG				0x7E

/* rdb registers */
#define RDB_REG_ACCESS					0x8000
#define CORE_SHD1C_0D 					0x1d
#define CORE_SHD_BICOLOR_LED0			0xa
#define CORE_SHD18_111                  0x2f   //Miscellanous Control Register
#define CORE_SHD18_000                  0x28
#define CORE_SHD18_100                  0x2c
#define CORE_BASE19                     0x9

#define CORE_SHD18_000_EXT_LPBK         0x8000
#define CORE_SHD18_000_SM_DSP_CLK_EN    0x0400
#define CORE_SHD18_100_SM_TDK_FIX_EN    0x4004
#define CORE_SHD18_100_SWAP_RXMDIX      0x0014
#define CORE_SHD18_100_RMT_LPBK_EN      0x8000
#define CORE_BASE19_AUTONEG_COMPLETE    0x8000
#define CORE_BASE19_AUTONEG_HCD_MASK    0x0700
#define CORE_BASE19_AUTONEG_HCD_OFFSET  0x8

/* registers bits */
#define FORCE_AUTO_MDIX                 1<<9

#define EGPHY_RGMII_OUT_PORT_MODE_MII             2
#define EGPHY_RGMII_OUT_PORT_MODE_RGMII           3
#define EGPHY_RGMII_OUT_PORT_MODE_RVMII           4

#define EGPHY_RGMII_OUT_REF_25_MHZ                 0
#define EGPHY_RGMII_OUT_REF_50_MHZ                 1
#define EGPHY_RGMII_OUT_REF_OFFSET                 3
#define EGPHY_RGMII_OUT_PORT_ID_OFFSET             4
/****************************************************
 * mode of phy
 ****************************************************/





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
void egphy_reset(uint32_t port_map);

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   egphy_auto_enable_extra_config*/
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
void egphy_auto_enable_extra_config(uint32_t phyID, int mode);

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   egphy_read_register							    		                  */
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
uint16_t egphy_read_register(uint32_t phyID,uint32_t regOffset);


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   egphy_write_register*/
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
int32_t egphy_write_register(uint32_t phyID,uint32_t regOffset,uint16_t regValue);

#endif
