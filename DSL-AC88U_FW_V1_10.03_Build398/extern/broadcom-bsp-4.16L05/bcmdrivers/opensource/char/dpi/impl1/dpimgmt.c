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
 Filename:       dpimgmt.c
 This file is to manage DPI features.
*****************************************************************************/

#include <dpimgmt.h>
#include <linux/blog.h>
#include <linux/module.h>

uint8_t DpiOptionList[DPI_CONFIG_MAX];
DpictlParentalConfig_t parental[DPI_PARENTAL_MAX];

int dm_config_option(unsigned short option, unsigned short value)
{
    if (option >= DPI_CONFIG_MAX)
        return -1;

    switch (option)
    {
        case DPI_CONFIG_PARENTAL:
            DpiOptionList[option] = value;
            break;

        default:
            printk("option<%d> is not supported in DPI feature\n", option);
            break;
    }

    return 0;
}

static int dm_parental_alloc(DpictlParentalConfig_t *cfg)
{
    int i;
    int ret = 0;

    for (i=0;i<DPI_PARENTAL_MAX;i++)
    {
        if (parental[i].appid == DPI_APPID_INVALID)
        {
            parental[i].appid = cfg->appid;
            break;
        }
    }

    if (i == DPI_PARENTAL_MAX)
        ret = -1;

    return ret;
}

static int dm_parental_free(int idx)
{
    parental[idx].appid = DPI_APPID_INVALID;
    return 0;
}

static int dm_parental_find(DpictlParentalConfig_t *cfg)
{
    int i;

    for (i=0;i<DPI_PARENTAL_MAX;i++)
    {
        if (parental[i].appid == cfg->appid)
            break;
    }

    return i;
}

int dm_config_parental(int action, DpictlParentalConfig_t *cfg)
{
    int idx;

    idx = dm_parental_find(cfg);

    if (action == DPI_CONFIG_ADD)
    {
        if (idx != DPI_INVALID_IDX)
        {
            printk("DPI parental entry exists\n");
        }
        else
        {
            if (dm_parental_alloc(cfg) < 0)
                printk("dm_parental_alloc fails\n");

#if defined(CONFIG_BCM_KF_BLOG)
            blog_dm(DPI_PARENTAL, cfg->appid, 0);
#endif
        }
    }
    else
    {
        if (idx != DPI_INVALID_IDX)
        {
            dm_parental_free(idx);
        }
        else
        {
            printk("DPI parental entry does not exist\n");
        }
    }

    return 0;
}

uint32_t dm_lookup(struct sk_buff *skb)
{
    uint32_t verdict = NF_ACCEPT;
#if 0
    DpictlParentalConfig_t info;

    info.appid = ((struct nf_conn *)skb->nfct)->dpi.app_id;

    if (dm_parental_find(&info) != DPI_INVALID_IDX)
        verdict = NF_DROP;
#endif

    return verdict;
}

int dm_construct(void)
{
    int i;

    memset(DpiOptionList, 0, sizeof(DpiOptionList));

    for (i=0;i<DPI_PARENTAL_MAX;i++)
        parental[i].appid = DPI_APPID_INVALID;

    return 0;
}

int dm_destruct()
{
    return 0;
}

#endif /*if defined(CONFIG_BCM_KF_DPI) && defined(CONFIG_BRCM_DPI) */
