#ifndef __BCMDPI_H_INCLUDED__
#define __BCMDPI_H_INCLUDED__
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

#define NETLINK_DPI             27
#define DPI_APPID_INVALID       0

typedef struct
{
    int sock_nl;
    char *sock_buff;
} DpictlNlInfo_t;

typedef struct
{
    unsigned short type;
    unsigned short len;
} DpictlMsgHdr_t;

typedef enum
{
    DPICTL_NLMSG_BASE = 0,
    DPICTL_NLMSG_ENABLE,
    DPICTL_NLMSG_DISABLE,
    DPICTL_NLMSG_STATUS,
    DPICTL_NLMSG_CONFIG_OPT,
    DPICTL_NLMSG_ADD_PARENTAL,
    DPICTL_NLMSG_DEL_PARENTAL,
    DPICTL_NLMSG_MAX
} DpictlNlMsgType_t;

#define DPICTL_NL_SET_HDR_TYPE(x, v)  (((DpictlMsgHdr_t *)x)->type = v)
#define DPICTL_NL_SET_HDR_LEN(x, v)  (((DpictlMsgHdr_t *)x)->len = v)

typedef enum
{
    DPI_CONFIG_BASE = 0,
    DPI_CONFIG_PARENTAL,
    DPI_CONFIG_QOS,
    DPI_CONFIG_MAX
} DpictlConfigOpt_t;

typedef struct
{
    unsigned short option;
    unsigned short value;
} DpictlConfig_t;

typedef struct
{
    int appid;
//    char mac[];
//    char url[];
} DpictlParentalConfig_t;
#endif

