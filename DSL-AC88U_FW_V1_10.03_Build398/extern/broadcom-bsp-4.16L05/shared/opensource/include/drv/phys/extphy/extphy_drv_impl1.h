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
/* This file contains the definition for External Phy				          */
/*                                                                            */
/******************************************************************************/
#ifndef __EXTPHY_DRV_IMPL1_H
#define __EXTPHY_DRV_IMPL1_H
/*****************************************************************************/
/*                                                                           */
/* Include files                                                             */
/*                                                                           */
/*****************************************************************************/
#include "bl_os_wraper.h"
#include "phys_common_drv.h"


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   extphy_reset								                              */
/*                                                                            */
/* Title:                                                                     */
/*                                                                            */
/*   reset the Ext PHY IP in 			         				              */
/*                                                                            */
/* Abstract:                                                                  */
/*   do some initializations before accessing the phy						  */
/* 	 operations                                                               */
/*                                                                            */
/* Input:                                                                     */
/*                                                                            */
/* Output:                                                                    */
/*                                                                            */
/******************************************************************************/
void extphy_reset(void);

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   extphy_auto_enable_extra_config			                              */
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
void extphy_auto_enable_extra_config(uint32_t phyID, int mode);

/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   extphy_read_register						    		                  */
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
uint16_t extphy_read_register(uint32_t phyID,uint32_t regOffset);


/******************************************************************************/
/*                                                                            */
/* Name:                                                                      */
/*                                                                            */
/*   extphy_write_register                                                    */
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
int32_t extphy_write_register(uint32_t phyID,uint32_t regOffset,uint16_t regValue);

#endif
