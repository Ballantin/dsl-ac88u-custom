/****************************************************************************
* <:copyright-BRCM:2013:DUAL/GPL:standard
* 
*    Copyright (c) 2013 Broadcom 
*    All Rights Reserved
* 
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License, version 2, as published by
* the Free Software Foundation (the "GPL").
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* 
* A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
* writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
* Boston, MA 02111-1307, USA.
* 
* :>
*
* File Name  : dectshimdrv.c
*
* Description: This file contains Linux character device driver entry points
*              for the dectshim driver.
*
* Updates    : 03/04/2013  Alliu Created.
***************************************************************************/

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <dectshimdrv.h>

static int __init mod_init(void)
{
   printk("Loading DECT Shim driver \n");

   dectShimDrvInit();
   return 0;
}

static void __exit mod_cleanup(void)
{
   printk("Unloading DECT shim driver...\n");
   dectShimDrvCleanup();
}

module_init(mod_init);
module_exit(mod_cleanup);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Broacom");
MODULE_DESCRIPTION("DECT shim layer device driver");
