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
#ifndef _BR_MLD_H
#define _BR_MLD_H

#if defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)
#include <linux/netdevice.h>
#include <linux/if_bridge.h>
#include <linux/igmp.h>
#include <linux/in6.h>
#include <linux/ipv6.h>
#include <linux/icmpv6.h>
#include "br_private.h"
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
#include <linux/blog.h>
#include "br_mcast.h"
#endif

#define SNOOPING_BLOCKING_MODE 2

#define TIMER_CHECK_TIMEOUT (2*HZ)
#define BR_MLD_MEMBERSHIP_TIMEOUT 260 /* RFC3810 */

#define BR_MLD_MULTICAST_MAC_PREFIX 0x33

#define BCM_IN6_ARE_ADDR_EQUAL(a,b)                                       \
       ((((__const uint32_t *) (a))[0] == ((__const uint32_t *) (b))[0])  \
	 && (((__const uint32_t *) (a))[1] == ((__const uint32_t *) (b))[1])  \
	 && (((__const uint32_t *) (a))[2] == ((__const uint32_t *) (b))[2])  \
	 && (((__const uint32_t *) (a))[3] == ((__const uint32_t *) (b))[3])) 

#define BCM_IN6_ASSIGN_ADDR(a,b)                                  \
    do {                                                          \
        ((uint32_t *) (a))[0] = ((__const uint32_t *) (b))[0];    \
        ((uint32_t *) (a))[1] = ((__const uint32_t *) (b))[1];    \
        ((uint32_t *) (a))[2] = ((__const uint32_t *) (b))[2];    \
        ((uint32_t *) (a))[3] = ((__const uint32_t *) (b))[3];    \
    } while(0)

#define BCM_IN6_IS_ADDR_MULTICAST(a) (((__const uint8_t *) (a))[0] == 0xff)
#define BCM_IN6_MULTICAST(x)   (BCM_IN6_IS_ADDR_MULTICAST(x))
#define BCM_IN6_IS_ADDR_MC_NODELOCAL(a) \
	(BCM_IN6_IS_ADDR_MULTICAST(a)					      \
	 && ((((__const uint8_t *) (a))[1] & 0xf) == 0x1))

#define BCM_IN6_IS_ADDR_MC_LINKLOCAL(a) \
	(BCM_IN6_IS_ADDR_MULTICAST(a)					      \
	 && ((((__const uint8_t *) (a))[1] & 0xf) == 0x2))

#define BCM_IN6_IS_ADDR_MC_SITELOCAL(a) \
	(BCM_IN6_IS_ADDR_MULTICAST(a)					      \
	 && ((((__const uint8_t *) (a))[1] & 0xf) == 0x5))

#define BCM_IN6_IS_ADDR_MC_ORGLOCAL(a) \
	(BCM_IN6_IS_ADDR_MULTICAST(a)					      \
	 && ((((__const uint8_t *) (a))[1] & 0xf) == 0x8))

#define BCM_IN6_IS_ADDR_MC_GLOBAL(a) \
	(BCM_IN6_IS_ADDR_MULTICAST(a) \
	 && ((((__const uint8_t *) (a))[1] & 0xf) == 0xe))

#define BCM_IN6_IS_ADDR_MC_SCOPE0(a) \
	(BCM_IN6_IS_ADDR_MULTICAST(a)					      \
	 && ((((__const uint8_t *) (a))[1] & 0xf) == 0x0))

/* Identify IPV6 L2 multicast by checking whether the most 12 bytes are 0 */
#define BCM_IN6_IS_ADDR_L2_MCAST(a)         \
    !((((__const uint32_t *) (a))[0])       \
        || (((__const uint32_t *) (a))[1])  \
        || (((__const uint32_t *) (a))[2]))

struct net_br_mld_mc_src_entry
{
	struct in6_addr   src;
	unsigned long     tstamp;
	int               filt_mode;
};

struct net_br_mld_mc_rep_entry
{
	struct in6_addr     rep;
	unsigned char       repMac[6];
	unsigned char       rep_proto_ver;
	unsigned long       tstamp;
	struct list_head    list;
};

struct net_br_mld_mc_fdb_entry
{
	struct hlist_node              hlist;
	struct net_bridge_port        *dst;
	struct in6_addr                grp;
	struct list_head               rep_list;
	struct net_br_mld_mc_src_entry src_entry;
	uint16_t                       lan_tci; /* vlan id */
	uint32_t                       wan_tci; /* vlan id */
	int                            num_tags;
	char                           type;
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
	uint32_t                       blog_idx;
	char                           root;
#endif
	uint32_t                       info; 
	int                            lanppp;
	struct net_device             *from_dev;
};

int br_mld_process_ignore_group_list (int count, t_MCPD_IGNORE_GROUP_ENTRY* ignoreMsgPtr);

int br_mld_blog_rule_update(struct net_br_mld_mc_fdb_entry *mc_fdb, int wan_ops);

int br_mld_mc_forward(struct net_bridge *br, 
                      struct sk_buff *skb, 
                      int forward,
                      int is_routed);

int br_mld_mc_fdb_add(struct net_device *from_dev,
                      int wan_ops,
                      struct net_bridge *br, 
                      struct net_bridge_port *prt, 
                      struct in6_addr *grp, 
                      struct in6_addr *rep,
                      unsigned char *repMac,
                      unsigned char rep_proto_ver,
                      int mode, 
                      uint16_t tci, 
                      struct in6_addr *src,
                      int lanppp,
                      uint32_t info);

void br_mld_mc_fdb_cleanup(struct net_bridge *br);

int br_mld_mc_fdb_remove(struct net_device *from_dev,
                         struct net_bridge *br, 
                         struct net_bridge_port *prt, 
                         struct in6_addr *grp, 
                         struct in6_addr *rep, 
                         int mode, 
                         struct in6_addr *src,
                         uint32_t info);

int br_mld_mc_fdb_update_bydev( struct net_bridge *br,
                                struct net_device *dev,
                                unsigned int       flushAll);

int br_mld_set_port_snooping(struct net_bridge_port *p,  void __user * userbuf);

int br_mld_clear_port_snooping(struct net_bridge_port *p,  void __user * userbuf);

void br_mld_wipe_reporter_for_port (struct net_bridge *br,
                                    struct in6_addr *rep, 
                                    u16 oldPort);

void br_mld_wipe_reporter_by_mac (struct net_bridge *br,
                                  unsigned char *repMac);

int br_mld_process_if_change(struct net_bridge *br, struct net_device *ndev);

struct net_br_mld_mc_fdb_entry *br_mld_mc_fdb_copy(struct net_bridge *br, 
                                     const struct net_br_mld_mc_fdb_entry *mld_fdb);
void br_mld_mc_fdb_del_entry(struct net_bridge *br, 
                             struct net_br_mld_mc_fdb_entry *mld_fdb,
                             struct in6_addr *rep,
                             unsigned char *repMac);
int __init br_mld_snooping_init(void);
void br_mld_snooping_fini(void);

void br_mld_wl_del_entry(struct net_bridge *br,struct net_br_mld_mc_fdb_entry *dst);

void br_mld_get_ip_icmp_hdrs( const struct sk_buff *pskb, struct ipv6hdr **ppipv6mcast, struct icmp6hdr **ppicmpv6, int *lanppp);

#endif /* defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP) */

#endif /* _BR_MLD_H */

