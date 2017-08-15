/*
*    Copyright (c) 2012 Broadcom Corporation
*    All Rights Reserved
*
<:label-BRCM:2012:DUAL/GPL:standard

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

#ifndef _BR_FLOWS_H
#define _BR_FLOWS_H

#if defined(CONFIG_BCM_KF_BLOG)

extern int br_flow_blog_rules(struct net_bridge *br,
                              struct net_device *rxVlanDev_p,
                              struct net_device *txVlanDev_p);
                       
extern int br_flow_path_delete(struct net_bridge *br,
                               struct net_device *rxVlanDev_p,
                               struct net_device *txVlanDev_p);
                        
#endif

#endif /* _BR_FLOWS_H */

