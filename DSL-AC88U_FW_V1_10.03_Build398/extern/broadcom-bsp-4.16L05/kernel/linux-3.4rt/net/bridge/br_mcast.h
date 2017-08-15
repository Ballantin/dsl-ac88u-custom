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
#ifndef _BR_MCAST_H
#define _BR_MCAST_H

#if (defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)) || (defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP))

#include <linux/netdevice.h>
#include <linux/if_bridge.h>
#include <linux/igmp.h>
#include <linux/in.h>
#include "br_private.h"
#include <linux/if_vlan.h>
#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
#include <linux/blog.h>
#include <linux/blog_rule.h>
#endif

#define MCPD_IF_TYPE_UNKWN      0
#define MCPD_IF_TYPE_BRIDGED    1
#define MCPD_IF_TYPE_ROUTED     2

typedef enum br_mcast_proto_type {
    BR_MCAST_PROTO_NONE,
    BR_MCAST_PROTO_IGMP,
    BR_MCAST_PROTO_MLD
} t_BR_MCAST_PROTO_TYPE;

/* Identify IPV4 L2 multicast by checking whether the most significant byte is 0 */
#define SNOOP_IN_IS_ADDR_L2_MCAST(a)        \
    !(((__const unsigned char *) (a))[0])

#if defined(CONFIG_BCM_KF_BLOG) && defined(CONFIG_BLOG)
void br_mcast_blog_release(t_BR_MCAST_PROTO_TYPE proto, void *mc_fdb);
void br_mcast_vlan_notify_for_blog_update(struct net_device *ndev,
                                   blogRuleVlanNotifyDirection_t direction,
                                   uint32_t nbrOfTags);
int br_mcast_blog_process(struct net_bridge *br,
                            void *mc_fdb,
                            t_BR_MCAST_PROTO_TYPE proto);
#endif
int br_mcast_get_rep_info(struct net_device *repDev, unsigned char *repMac, uint32_t *info);
void br_mcast_handle_netdevice_events(struct net_device *ndev, unsigned long event);

#define PROXY_DISABLED_MODE 0
#define PROXY_ENABLED_MODE 1

#define SNOOPING_DISABLED_MODE 0
#define SNOOPING_ENABLED_MODE 1
#define SNOOPING_BLOCKING_MODE 2

typedef enum br_mcast_l2l_snoop_mode {
    BR_MCAST_L2L_SNOOP_DISABLED,
    BR_MCAST_L2L_SNOOP_ENABLED,
    BR_MCAST_L2L_SNOOP_ENABLED_LAN
} t_BR_MCAST_L2L_SNOOP_MODE;

#define MCPD_MAX_IFS            10
typedef struct mcpd_wan_info
{
	char                      if_name[IFNAMSIZ];
	int                       if_ops;
} t_MCPD_WAN_INFO;

typedef struct mcpd_igmp_snoop_entry
{
	char                      br_name[IFNAMSIZ];
	/* Internal, ignore endianness */
	unsigned short            port_no;
	unsigned int              mode;
	unsigned int              code;
	unsigned short            tci;/* vlan id */
	t_MCPD_WAN_INFO           wan_info[MCPD_MAX_IFS];
	int                       lanppp;
	int                       excludePort;  
	char                      enRtpSeqCheck;
	/* Standard, use big endian */
	struct                    in_addr rxGrp;
	struct                    in_addr txGrp;
	struct                    in_addr src;
	struct                    in_addr rep;
	unsigned char             repMac[6];
	unsigned char             rep_proto_ver;
} t_MCPD_IGMP_SNOOP_ENTRY;

typedef struct mcpd_ignore_group_entry {
  struct in6_addr address;
  struct in6_addr mask;
} t_MCPD_IGNORE_GROUP_ENTRY;

typedef struct mcpd_ignore_group_message {
  int                       proto;
  int                       count;
  t_MCPD_IGNORE_GROUP_ENTRY ignoreEntry[0];
} t_MCPD_IGNORE_GROUP_MESSAGE;

struct br_mcast_control_filter_entry {
	struct hlist_node node;
	struct in6_addr group;
	struct in6_addr mask;
};

typedef struct mcastCfg {
	int                          mcastPriQueue;
	int                          thereIsAnUplink;
	struct hlist_head            igmp_control_filter_list;
	struct hlist_head            mld_control_filter_list;
	spinlock_t                   cfgLock;
} t_MCAST_CFG;

extern t_MCAST_CFG multiConfig;

void br_mcast_set_pri_queue(int val);
int  br_mcast_get_pri_queue(void);
void br_mcast_set_skb_mark_queue(struct sk_buff *skb);
void br_mcast_set_uplink_exists(int uplinkExists);
int  br_mcast_get_lan2lan_snooping(t_BR_MCAST_PROTO_TYPE proto, struct net_bridge *br);

#if defined(CONFIG_BR_MLD_SNOOP) && defined(CONFIG_BCM_KF_MLD)
#define SNOOPING_ADD_ENTRY 0
#define SNOOPING_DEL_ENTRY 1
#define SNOOPING_FLUSH_ENTRY 2
#define SNOOPING_FLUSH_ENTRY_ALL 3
int mcast_snooping_call_chain(unsigned long val,void *v);
void br_mcast_wl_flush(struct net_bridge *br) ;

typedef struct mcpd_mld_snoop_entry
{
	char                      br_name[IFNAMSIZ];
	/* Internal, ignore endianness */
	unsigned short            port_no;
	unsigned int              mode;
	unsigned int              code;
	unsigned short            tci;
	t_MCPD_WAN_INFO           wan_info[MCPD_MAX_IFS];
	int                       lanppp;
	/* External, use big endian */
	struct                    in6_addr grp;
 	struct                    in6_addr src;
	struct                    in6_addr rep;
	unsigned char             repMac[6];
	unsigned char             rep_proto_ver;
} t_MCPD_MLD_SNOOP_ENTRY;
#endif

#endif /* (defined(CONFIG_BCM_KF_IGMP) && defined(CONFIG_BR_IGMP_SNOOP)) || (defined(CONFIG_BCM_KF_MLD) && defined(CONFIG_BR_MLD_SNOOP)) */

#endif /* _BR_MCAST_H */
