#if defined(CONFIG_BCM_KF_DPI) && defined(CONFIG_BRCM_DPI)
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
/******************************************************************************
 Filename:       dpi.c
*****************************************************************************/

#include <linux/module.h>
#include <linux/types.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/ipv6.h>
#include <linux/if_ether.h>
#include <linux/netfilter_ipv6.h>

#include <linux/devinfo.h>
#include <linux/urlinfo.h>
//#include <linux/dpi_engine.h>
//#include <linux/dpi_appdb.h>
#include <linux/dpi_ctk.h>
#include <linux/netlink.h>
#include <bcmdpi.h>
#include <dpimgmt.h>

#include <tdts.h>

#if defined(CONFIG_BCM_KF_BLOG)
#include <linux/blog.h>
#endif

static struct nf_hook_ops forward_hook_ops_fwfilter;
static struct nf_hook_ops forward_hook_ops_infirst;
static struct nf_hook_ops forward_hook_ops_fwfilterv6;
static struct sock *nl_sk = NULL;
static int dpi_enable_g = 1;

//#define CC_DPI_SUPPORT_DEBUG
#ifndef DPI_NULL_STMT
#define DPI_NULL_STMT                   do { /* NULL BODY */ } while (0)
#endif

#define DPI_ENGINE_RET_SUCCESS   0
#define DPI_ENGINE_RET_FAIL      -1

#if defined(CC_DPI_SUPPORT_DEBUG)

#include <linux/bcm_colors.h>
#define dpi_print(fmt, arg...)                                           \
    if ( dpi_dbg )                                                       \
        printk( CLRc "DPI %s :" fmt CLRnl, __FUNCTION__, ##arg )
#define dpi_assertv(cond)                                                \
    if ( !cond ) {                                                       \
        printk( CLRerr "DPI ASSERT %s : " #cond CLRnl, __FUNCTION__ );   \
        return;                                                          \
    }
#define dpi_assertr(cond, rtn)                                           \
    if ( !cond ) {                                                       \
        printk( CLRerr "DPI ASSERT %s : " #cond CLRnl, __FUNCTION__ );   \
        return rtn;                                                      \
    }
#define DPI_DBG(debug_code)    do { debug_code } while(0)
#else
#define dpi_print(fmt, arg...) DPI_NULL_STMT
#define dpi_assertv(cond) DPI_NULL_STMT
#define dpi_assertr(cond, rtn) DPI_NULL_STMT
#define DPI_DBG(debug_code) DPI_NULL_STMT
#endif

#define dpi_get_app_id(cat_id, app_id, beh_id)  \
    (uint32_t) ( \
        (uint32_t) (((uint8_t) cat_id) << 24) | \
        (uint32_t) (((uint16_t) app_id) << 8) | \
        (uint32_t) ((uint8_t) beh_id) \
    )

#define dpi_get_unknown_app_id()    dpi_get_app_id(0, 0, 0)

int dpi_dbg = 0;

inline unsigned int conntrack_get_appid( const struct sk_buff *skb )
{
    return (((struct nf_conn *)skb->nfct)->dpi.app_id);
}

inline void conntrack_set_appid( struct sk_buff *skb, unsigned int id )
{
    (((struct nf_conn *)skb->nfct)->dpi.app_id) = id;
}

inline uint16_t conntrack_get_dpi_status( const struct sk_buff *skb,
                                          uint16_t mask )
{
    return (((((struct nf_conn *)skb->nfct)->dpi.flags) & mask) == mask);
}

inline void conntrack_set_dpi_status( struct sk_buff *skb, uint16_t status )
{
    (((struct nf_conn *)skb->nfct)->dpi.flags) |= status;
}

inline uint16_t conntrack_get_devid( const struct sk_buff *skb )
{
    return (((struct nf_conn *)skb->nfct)->dpi.dev_key);
}

inline void conntrack_set_devid( struct sk_buff *skb, uint16_t id )
{
    (((struct nf_conn *)skb->nfct)->dpi.dev_key) = id;
}

inline void conntrack_set_urlid( struct sk_buff *skb, uint16_t id )
{
    (((struct nf_conn *)skb->nfct)->dpi.url_id) = id;
}

static inline void devid_in_handle( uint16_t dev_idx, 
                                    tdts_pkt_parameter_t *pkt_param )
{
    dpi_print("devIdx<%u> SW_DEVID<%d>", dev_idx,
              tdts_check_pkt_parameter_res(pkt_param, TDTS_RES_TYPE_DEVID));

    if (tdts_check_pkt_parameter_res(pkt_param, TDTS_RES_TYPE_DEVID))
    {
        DevInfoEntry_t entry;

        devinfo_get(dev_idx, &entry);

        if (entry.flags != DEVINFO_DONE)
        {
            entry.vendor_id = TDTS_PKT_PARAMETER_RES_DEVID_VENDOR_ID(pkt_param);
            entry.os_id = TDTS_PKT_PARAMETER_RES_DEVID_OS_NAME_ID(pkt_param);
            entry.class_id=TDTS_PKT_PARAMETER_RES_DEVID_OS_CLASS_ID(pkt_param);
            entry.type_id = TDTS_PKT_PARAMETER_RES_DEVID_DEV_CAT_ID(pkt_param);
            entry.dev_id = TDTS_PKT_PARAMETER_RES_DEVID_DEV_ID(pkt_param);
            //       TDTS_PKT_PARAMETER_RES_DEVID_DEV_FAMILY_ID
            entry.flags = DEVINFO_DONE;

            devinfo_set(&entry);
        }
    }

    return;
}

static inline void devid_fw_handle( struct sk_buff *skb, 
                                    tdts_pkt_parameter_t *pkt_param )
{
    /* device identification is only for LAN device */
    if (!conntrack_get_dpi_status(skb, DEVID_STATUS_NOMORE) &&
        !(skb->dev->priv_flags & IFF_WANDEV))
    {
        uint16_t dev_idx;
        DevInfoEntry_t entry;

        dpi_print("IN: nfct<0x%08x> status<0x%08x> devID<%u> SW_DEVID<%u>",
                  (int)skb->nfct, ((struct nf_conn *)skb->nfct)->dpi.flags,
                  (((struct nf_conn *)skb->nfct)->dpi.dev_key),
                  tdts_check_pkt_parameter_res(pkt_param, TDTS_RES_TYPE_DEVID));

        dev_idx = conntrack_get_devid(skb);

        if (dev_idx == DEVINFO_IX_INVALID)
        {
            uint8_t *ptr = eth_hdr(skb)->h_source;

            dpi_print("mac<%02x:%02x:%02x:%02x:%02x:%02x>",
                      ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

            dev_idx = devinfo_lookup(ptr);

            if (dev_idx != DEVINFO_IX_INVALID)
                conntrack_set_devid(skb, dev_idx);
            else
            {
                dpi_print("fail to alloc devid?");
                return;
            }
        }

        devinfo_get(dev_idx, &entry);

        dpi_print("Got entry: idx<%u> flag<%u>", entry.idx, entry.flags);
        if (entry.flags == DEVINFO_DONE)
        {
            conntrack_set_dpi_status(skb, DEVID_STATUS_NOMORE);
            dpi_print(" -> status<%08x>",
                      ntohl(((struct nf_conn *)skb->nfct)->dpi.flags));
            return;
        }
        else
        {
            if (tdts_check_pkt_parameter_res(pkt_param, TDTS_RES_TYPE_DEVID))
            {
                entry.vendor_id = 
                        TDTS_PKT_PARAMETER_RES_DEVID_VENDOR_ID(pkt_param);
                entry.os_id = 
                        TDTS_PKT_PARAMETER_RES_DEVID_OS_NAME_ID(pkt_param);
                entry.class_id = 
                        TDTS_PKT_PARAMETER_RES_DEVID_OS_CLASS_ID(pkt_param);
                entry.type_id = 
                        TDTS_PKT_PARAMETER_RES_DEVID_DEV_CAT_ID(pkt_param);
                entry.dev_id = 
                        TDTS_PKT_PARAMETER_RES_DEVID_DEV_ID(pkt_param);
                //       TDTS_PKT_PARAMETER_RES_DEVID_DEV_FAMILY_ID
                entry.flags = DEVINFO_DONE;

                devinfo_set(&entry);
            
                conntrack_set_dpi_status(skb, DEVID_STATUS_NOMORE);

                dpi_print(" -> status<%08x>\n"
                          "    vendor<%u> os<%u> class<%u> type<%u> dev<%u>",
                          ntohl(((struct nf_conn *)skb->nfct)->dpi.flags),
                          entry.vendor_id, entry.os_id,
                          entry.class_id, entry.type_id, entry.dev_id);

                return;
            }
        }
    }
}

static inline void dpi_result_update( struct sk_buff *skb, 
                                      tdts_pkt_parameter_t *pkt_param )
{
    uint32_t behinst;

    if (tdts_check_pkt_parameter_res(pkt_param, TDTS_RES_TYPE_APPID))
    {
        /*
         * application classification result.
         */
        uint32_t ct_appid;

#if 0
        behinst = dpi_get_app_id(TDTS_PKT_PARAMETER_RES_APPID_CAT_ID(pkt_param),
                                TDTS_PKT_PARAMETER_RES_APPID_APP_ID(pkt_param),
                                TDTS_PKT_PARAMETER_RES_APPID_BEH_ID(pkt_param));
#else
        behinst = dpi_get_app_id(TDTS_PKT_PARAMETER_RES_APPID_CAT_ID(pkt_param),
                                 TDTS_PKT_PARAMETER_RES_APPID_APP_ID(pkt_param),
                                 0);
#endif

        ct_appid = conntrack_get_appid(skb);
        if (ct_appid != behinst)
        {
            if (behinst != ct_appid)
            {
                /* app id of this ctk entry is changed. Add a resync bit. */
                dpi_print("appid: from 0x%08x to 0x%08x", ct_appid, behinst);
            }

            conntrack_set_appid(skb, behinst);
        }

        conntrack_set_dpi_status(skb, APPID_STATUS_IDENTIFIED);

        if (TDTS_PKT_PARAMETER_RES_APPID_CHECK_FINAL(pkt_param))
        {
            conntrack_set_dpi_status(skb, APPID_STATUS_FINAL);
        }

#ifdef DPI_URL_RECORD
        /* url record */
        if (TDTS_PKT_PARAMETER_RES_URL_DOMAIN(pkt_param) && 
            TDTS_PKT_PARAMETER_RES_URL_DOMAIN_LEN(pkt_param) > 0)
        {
            if (TDTS_PKT_PARAMETER_RES_URL_DOMAIN_LEN(pkt_param) > 
                URLINFO_MAX_HOST_LEN)
            {
                printk("URL<%s> is greater than %d\n", 
                       TDTS_PKT_PARAMETER_RES_URL_DOMAIN(pkt_param),
                       URLINFO_MAX_HOST_LEN);
            }
            else
            {
                UrlInfoEntry_t url;
                uint16_t url_idx;

                url.hostlen = TDTS_PKT_PARAMETER_RES_URL_DOMAIN_LEN(pkt_param);
                strncpy(url.host, TDTS_PKT_PARAMETER_RES_URL_DOMAIN(pkt_param), 
                        TDTS_PKT_PARAMETER_RES_URL_DOMAIN_LEN(pkt_param));

                url_idx = urlinfo_lookup(&url);

                if (url_idx != URLINFO_IX_INVALID)
                    conntrack_set_urlid(skb, url_idx);
                else
                    dpi_print("fail to alloc urlidx?");
            }
        }

#if 0  /* Now we only record host. We may record more in the future */
        if (TDTS_PKT_PARAMETER_RES_URL_PATH(pkt_param) && 
            TDTS_PKT_PARAMETER_RES_URL_PATH_LEN(pkt_param) > 0)
        {
           char url[1024]={0};

           strncpy(url, TDTS_PKT_PARAMETER_RES_URL_PATH(pkt_param),
                   TDTS_PKT_PARAMETER_RES_URL_PATH_LEN(pkt_param));
           printk("!!path(%d): %s\n", 
                   TDTS_PKT_PARAMETER_RES_URL_PATH_LEN(pkt_param), url);
        }

        if (TDTS_PKT_PARAMETER_RES_URL_REFERER(pkt_param) && 
            TDTS_PKT_PARAMETER_RES_URL_REFERER_LEN(pkt_param) > 0)
        {
           char url[1024]={0};

           strncpy(url, TDTS_PKT_PARAMETER_RES_URL_REFERER(pkt_param), 
                   TDTS_PKT_PARAMETER_RES_URL_REFERER_LEN(pkt_param));
           printk("##referer(%d): %s\n\n", 
                   TDTS_PKT_PARAMETER_RES_URL_REFERER_LEN(pkt_param), url);
        }
#endif
#endif

#if 0
        if ((param->flags & SW_FG_NOMORE) ||
            conntrack_max_dpi_pkt((struct nf_conn *)skb->nfct, DPI_MAX_PKT))
#else
        if (TDTS_PKT_PARAMETER_RES_APPID_CHECK_NOMORE(pkt_param))
#endif
        {
            conntrack_set_dpi_status(skb, APPID_STATUS_NOMORE);

            dpi_print("  Cat:%u(%s) App:%u(%s) Beh:%u(%s)\n"
                      "  Final<%d> Nomore<%d> NOINT<%d> APPID<%u> status<%08x>",
                    TDTS_PKT_PARAMETER_RES_APPID_CAT_ID(pkt_param),
                    TDTS_PKT_PARAMETER_RES_APPID_CAT_NAME(pkt_param),
                    TDTS_PKT_PARAMETER_RES_APPID_APP_ID(pkt_param),
                    TDTS_PKT_PARAMETER_RES_APPID_APP_NAME(pkt_param),
                    TDTS_PKT_PARAMETER_RES_APPID_BEH_ID(pkt_param),
                    TDTS_PKT_PARAMETER_RES_APPID_BEH_NAME(pkt_param),
                    TDTS_PKT_PARAMETER_RES_APPID_CHECK_FINAL(pkt_param),
                    TDTS_PKT_PARAMETER_RES_APPID_CHECK_NOMORE(pkt_param),
                    TDTS_PKT_PARAMETER_RES_APPID_CHECK_NOINT(pkt_param),
                    ((struct nf_conn *)skb->nfct)->dpi.app_id,
                    ntohl(((struct nf_conn *)skb->nfct)->dpi.flags));
        }
        else
        {
#if defined(CONFIG_BCM_KF_BLOG)
            blog_skip(skb);
#endif
        }
    }
    else
    {
        if (!conntrack_get_dpi_status(skb, APPID_STATUS_IDENTIFIED))
        {
            /*
             * Unknown app.
             */
            dpi_print("unknown app");
            conntrack_set_appid(skb, dpi_get_unknown_app_id());
        }

#if 0
        if ((param->flags & SW_FG_NOTIA) || (param->flags & SW_FG_NOMORE) || 
             conntrack_max_dpi_pkt((struct nf_conn *)skb->nfct, DPI_MAX_PKT))
#else
        if (TDTS_PKT_PARAMETER_RES_APPID_CHECK_NOINT(pkt_param) ||
            TDTS_PKT_PARAMETER_RES_APPID_CHECK_NOMORE(pkt_param)) 
#endif
        {
            conntrack_set_dpi_status(skb, APPID_STATUS_NOMORE);
            dpi_print("  App NOMORE: APPID<%u> status<%08x>",
                      ((struct nf_conn *)skb->nfct)->dpi.app_id,
                      ntohl(((struct nf_conn *)skb->nfct)->dpi.flags));
        }
        else
        {
#if defined(CONFIG_BCM_KF_BLOG)
            blog_skip(skb);
#endif
        }
    }

    devid_fw_handle(skb, pkt_param);
}

static inline void dpi_classify( struct sk_buff *skb )
{
    int ret;
    tdts_pkt_parameter_t pkt_param;

    if (conntrack_get_dpi_status(skb, DEVID_STATUS_NOMORE))
        tdts_init_pkt_parameter(&pkt_param, SW_APP, 0);
    else
        tdts_init_pkt_parameter(&pkt_param, SW_APP | SW_DEVID, 0);

    ret = tdts_shell_dpi_l3_skb(skb, &pkt_param);
    switch (ret)
    {
    case DPI_ENGINE_RET_SUCCESS:
        dpi_result_update(skb, &pkt_param);
        break;
    case DPI_ENGINE_RET_FAIL:
        dpi_print("Cannot scan this packet");
        break;
    default:
        dpi_print("Unexpected return value (fixme)");
        break;
    }
}

static inline void devid_classify( uint16_t dev_idx, struct sk_buff *skb )
{
	int ret;
    tdts_pkt_parameter_t pkt_param;

    tdts_init_pkt_parameter(&pkt_param, SW_DEVID, 0);

    ret = tdts_shell_dpi_l3_skb(skb, &pkt_param);
	switch (ret)
	{
	case DPI_ENGINE_RET_SUCCESS:
		devid_in_handle(dev_idx, &pkt_param);
		break;
	case DPI_ENGINE_RET_FAIL:
		dpi_print("Cannot scan this packet");
		break;
	default:
		// do nothing
		dpi_print("Unexpected return value (fixme)");
		break;
	}
}

static int is_udp_pkt( const struct sk_buff *skb )
{
    switch (ntohs(skb->protocol))
    {
        case ETH_P_IP:
            return (ip_hdr(skb)->protocol == IPPROTO_UDP);
        case ETH_P_IPV6:    //FIXME: parse extention header?
            return (ipv6_hdr(skb)->nexthdr == IPPROTO_UDP);
        default:
            break;
    }

    return 0;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24))
static uint32_t forward_hook_local_devid(
        uint32_t hooknum, struct sk_buff *skb, const struct net_device *in_dev,
        const struct net_device *out_dev, int (*okfn)(struct sk_buff *) )
#else
uint32_t forward_hook_local( 
        uint32_t hooknum, struct sk_buff **pskb,
        const struct net_device *in_dev, const struct net_device *out_dev,
        int (*okfn)(struct sk_buff *))
#endif
{
#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
    struct sk_buff *skb = *pskb;
#endif
    uint32_t verdict = NF_ACCEPT;

    if (!dpi_enable_g)
        return verdict;

    if (unlikely(skb == NULL || skb->dev == NULL))
    {
        dpi_assertr((skb != NULL), NF_ACCEPT); /* If skb is NULL, let's panic */
        return verdict;
    }

    /* Assume only UDP packets (DHCP) contain device info */
    if (!(skb->dev->priv_flags & IFF_WANDEV) && is_udp_pkt(skb))
    {
        uint16_t dev_idx = devinfo_lookup(eth_hdr(skb)->h_source);
        DevInfoEntry_t entry;

        if (dev_idx == DEVINFO_IX_INVALID)
        {
            dpi_print("fail to alloc devid?");
            return verdict;
        }

        devinfo_get(dev_idx, &entry);

        if (entry.flags != DEVINFO_DONE)
            devid_classify(dev_idx, skb);
    }

    return verdict;
}

#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,24))
static uint32_t forward_hook_fwfilter( 
        uint32_t hooknum, struct sk_buff *skb, const struct net_device *in_dev,
        const struct net_device *out_dev, int (*okfn)(struct sk_buff *) )
#else
uint32_t forward_hook_fwfilter(	
        uint32_t hooknum, struct sk_buff **pskb,
        const struct net_device *in_dev, const struct net_device *out_dev,
        int (*okfn)(struct sk_buff *))
#endif
{
    uint32_t verdict = NF_ACCEPT;

#if (LINUX_VERSION_CODE < KERNEL_VERSION(2,6,24))
    struct sk_buff *skb = *pskb;
#endif

    if (!dpi_enable_g)
        return verdict;

    if (unlikely(skb == NULL || skb->dev == NULL || skb->nfct == NULL))
    {
        dpi_assertr((skb != NULL), NF_ACCEPT); /* If skb is NULL, let's panic */
        return verdict;
    }

    /* 
     * Assumption: For one conntrack, device identification should be done
     * before application identification
     */
    if (likely(!conntrack_get_dpi_status(skb, APPID_STATUS_NOMORE)))
    {
        dpi_classify(skb);
    }
    else if ((conntrack_get_devid(skb) == DEVINFO_IX_INVALID) &&
             !(skb->dev->priv_flags & IFF_WANDEV))
    {
        uint16_t dev_idx;
        uint8_t *ptr = eth_hdr(skb)->h_source;

        dpi_print("found mac after APP_NOMORE<%02x:%02x:%02x:%02x:%02x:%02x>",
                   ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5]);

        dev_idx = devinfo_lookup(ptr);

        if (dev_idx != DEVINFO_IX_INVALID)
            conntrack_set_devid(skb, dev_idx);
        else
        {
            dpi_print("fail to alloc devid?");
        }
    }

    if (likely(conntrack_get_dpi_status(skb, APPID_STATUS_NOMORE)))
        verdict = dm_lookup(skb);

    return verdict;
}

static void dpi_nl_process_config(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    char* ptr = NLMSG_DATA(nlh);

    DpictlConfig_t *cfg = (DpictlConfig_t *)(ptr + sizeof(DpictlMsgHdr_t));

    if (cfg == NULL) return;

    dm_config_option(cfg->option, cfg->value);

    return;
}

static void dpi_nl_config_parental(struct sk_buff *skb, uint32_t mode)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    char* ptr = NLMSG_DATA(nlh);
    DpictlParentalConfig_t *cfg = 
        (DpictlParentalConfig_t *)(ptr + sizeof(DpictlMsgHdr_t));

    if (cfg == NULL) return;

    if (mode == DPICTL_NLMSG_ADD_PARENTAL)
        dm_config_parental(DPI_CONFIG_ADD, cfg);
    else
        dm_config_parental(DPI_CONFIG_DEL, cfg);

    return;
}

static void dpi_nl_process_status(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    char *ptr  = NULL;
    struct sk_buff *new_skb = NULL;
    struct nlmsghdr *new_nlh = NULL;
    int buf_size;

    buf_size = NLMSG_SPACE((sizeof(DpictlMsgHdr_t) + sizeof(int)));

    new_skb = alloc_skb(buf_size, GFP_ATOMIC);

    if(!new_skb) {
        printk("dpi.c:%d %s() errr no mem\n", __LINE__, __FUNCTION__);
        return;
    }

    if(new_skb)
    {
        int pid;

        pid = nlh->nlmsg_pid; /*pid of sending process */
        new_nlh = nlmsg_put(new_skb, 0, 0, NLMSG_DONE, buf_size, 0);

        ptr = NLMSG_DATA(new_nlh);
        ((DpictlMsgHdr_t *)ptr)->type = DPICTL_NLMSG_STATUS;
        ((DpictlMsgHdr_t *)ptr)->len = sizeof(int);

        ptr += sizeof(DpictlMsgHdr_t);
        *((int *)ptr) = dpi_enable_g;

        NETLINK_CB(new_skb).dst_group = 0;

        if (netlink_unicast(nl_sk, new_skb, pid, MSG_DONTWAIT) < 0)
            printk("Error while sending DPI status\n");
    }

    return;
}

static inline void dpi_nl_handler(struct sk_buff *skb)
{
    struct nlmsghdr *nlh = (struct nlmsghdr *)skb->data;
    char *ptr  = NULL;
    unsigned short msg_type;

    if (skb->len >= NLMSG_SPACE(0)) 
    {
        if (nlh->nlmsg_len < sizeof(*nlh) || skb->len < nlh->nlmsg_len)
            return;

        ptr = NLMSG_DATA(nlh);

        msg_type = *(unsigned short *)ptr;
        switch(msg_type)
        {
            case DPICTL_NLMSG_ENABLE:
                dpi_enable_g = 1;
                break;

            case DPICTL_NLMSG_DISABLE:
                dpi_enable_g = 0;
                break;

            case DPICTL_NLMSG_STATUS:
                dpi_nl_process_status(skb);
                break;

            case DPICTL_NLMSG_CONFIG_OPT:
                dpi_nl_process_config(skb);
                break;

            case DPICTL_NLMSG_ADD_PARENTAL:
            case DPICTL_NLMSG_DEL_PARENTAL:
                dpi_nl_config_parental(skb, msg_type);
                break;

            default:
                printk("DPI Unknown usr->krnl msg type -%d- \n", msg_type);
        }
    }

    return;
}

static int __register_hook( struct nf_hook_ops *ops, unsigned int hooknum, 
                            u_int8_t pf, int prio, nf_hookfn *cb )
{
	memset(ops, 0x00, sizeof(*ops));

	ops->hooknum = hooknum;
	ops->pf = pf;
	ops->priority = prio;

	ops->hook = (nf_hookfn *) cb;

	if (nf_register_hook(ops) != 0)
	{
		return -1;
	}

	return 0;
}

static int __init dpi_init( void )
{
    DPI_DBG( printk( "DPI dpi_dbg<0x%08x> = %d\n", (int)&dpi_dbg, dpi_dbg ); );

    if (__register_hook(&forward_hook_ops_fwfilter,
        NF_INET_FORWARD, PF_INET, NF_IP_PRI_FILTER, forward_hook_fwfilter) < 0)
    {
        dpi_print("Cannot register forward_hook_ops_fwfilter");
        return -1;
    }

   	if (__register_hook(&forward_hook_ops_infirst,
		NF_INET_LOCAL_IN, PF_INET, NF_IP_PRI_FILTER,
        forward_hook_local_devid) < 0)
	{
		dpi_print("Cannot register forward_hook_ops_infirst");
		return -1;
	}

    if(__register_hook(&forward_hook_ops_fwfilterv6,
       NF_INET_FORWARD, PF_INET6, NF_IP6_PRI_FILTER, forward_hook_fwfilter) < 0)
    {
        dpi_print("Cannot register forward_hook_ops_fwfilterv6");
        return -1;
    }

    devinfo_init();
    urlinfo_init();
    dm_construct();

    nl_sk = netlink_kernel_create(&init_net, NETLINK_DPI, 0, 
                                  dpi_nl_handler, NULL, THIS_MODULE);

    if (nl_sk == NULL)
        dpi_print("DPICTL: fail to create kernel netlink socket");

    return 0;
}

static void __exit dpi_fnit(void)
{
    nf_unregister_hook(&forward_hook_ops_fwfilter);
    nf_unregister_hook(&forward_hook_ops_infirst);
}

module_init(dpi_init);
module_exit(dpi_fnit);

MODULE_LICENSE("GPL");
#endif /*if defined(CONFIG_BCM_KF_DPI) && defined(CONFIG_BRCM_DPI) */
