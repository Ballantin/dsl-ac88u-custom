/*
 * OS Abstraction Layer - Linux
 *
 * Copyright (C) 2013, Broadcom Corporation. All Rights Reserved.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * $Id: linux_osl_dslcpe_pktc.h $
 */

#ifndef _linux_osl_dslcpe_pktc_h_
#define _linux_osl_dslcpe_pktc_h_

#if defined(DSLCPE) && defined(PKTC)

/* Use 8 bytes of skb pktc_cb field to store below info */
struct chain_node {
        struct sk_buff  *link;
        unsigned int    flags:3, pkts:9, bytes:20;
};

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 36)
#define CHAINED (1 << 3)
#define PKTISCHAINED(skb)       (((struct sk_buff*)(skb))->pktc_flags & CHAINED)
#define CHAIN_NODE(skb)         ((struct chain_node*)(((struct sk_buff*)skb)->pktc_cb))
#else
#define	CHAINED	(1 << 15)
#define	PKTISCHAINED(skb)	(((struct sk_buff*)(skb))->mac_len & CHAINED)
#define CHAIN_NODE(skb)		((struct chain_node*)&(((struct sk_buff*)skb)->tstamp))
#endif

#define PKTCLINK(skb)           (CHAIN_NODE(skb)->link)

/* PKTC requests */
#define BRC_HOT_INIT                    1    /* Initialize hot bridge table */
#define BRC_HOT_GET_BY_DA               2    /* Get BRC_HOT pointer for pkt chaining by dest addr */
#define BRC_HOT_GET_BY_IDX              3    /* Get BRC_HOT pointer for pkt chaining by table index */
#define UPDATE_BRC_HOT                  4    /* To update BRC_HOT entry */
#define UPDATE_WLAN_HANDLE              5    /* To update wlan handle */
#define SET_PKTC_TX_MODE                6    /* To set pktc tx mode: enabled=0, disabled=1 */
#define GET_PKTC_TX_MODE                7    /* To get pktc tx mode: enabled=0, disabled=1 */
#define DELETE_BRC_HOT                  8    /* To delete BRC_HOT entry */
#define DUMP_BRC_HOT                    9    /* To dump BRC_HOT table */
#define BRC_HOT_GET_TABLE_TOP           10   /* Get the address/top of BRC_HOT table */
#define DELETE_WLAN_HANDLE              11   /* To delete wlan handle in wldev table */
#define UPDATE_WFD_IDX_BY_WL_DEV        12   /* To update WFD Index by WLAN Dev */
#define FLUSH_BRC_HOT                   13   /* To flush out entire BRC_HOT table */

extern uint32_t wl_pktc_req(int req_id, uint32_t param0, uint32_t param1, uint32_t param2);
extern uint32_t (*wl_pktc_req_hook)(int req_id, uint32_t param0, uint32_t param1, uint32_t param2);

#if defined(CONFIG_BCM963138) || defined(CONFIG_BCM963148) || defined(CONFIG_BCM96838) || defined(CONFIG_BCM96848)
#define INVALID_CHAIN_IDX  0x3FFE
#else
#define INVALID_CHAIN_IDX  0xFE
#endif

#define WFD_IDX_UINT16_BIT_MASK   (0xC000)
#define WFD_IDX_UINT16_BIT_POS    14

#endif /* DSLCPE && PKTC */

#endif
