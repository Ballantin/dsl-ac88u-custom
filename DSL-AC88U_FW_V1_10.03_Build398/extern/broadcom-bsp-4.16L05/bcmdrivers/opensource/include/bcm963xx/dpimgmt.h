#ifndef __DPIMGMT_H_INCLUDED__
#define __DPIMGMT_H_INCLUDED__
/*
<:copyright-BRCM:2014:DUAL/GPL:standard

   Copyright (c) 2014 Broadcom 
   All Rights Reserved

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
#include <bcmdpi.h>
#include <linux/dpi_ctk.h>

#define DPI_PARENTAL_MAX        32
#define DPI_INVALID_IDX DPI_PARENTAL_MAX

#define DPI_CONFIG_ADD          0
#define DPI_CONFIG_DEL          1

int dm_config_option(unsigned short option, unsigned short value);
int dm_config_parental(int action, DpictlParentalConfig_t *cfg);
int dm_construct(void);

uint32_t dm_lookup(struct sk_buff *skb);
#endif

