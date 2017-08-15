/***********************************************************************
 *
 * <:copyright-BRCM:2015:DUAL/GPL:standard
 * 
 *    Copyright (c) 2015 Broadcom 
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
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>	 /* needed to define getpid() */
#include <signal.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "cms_psp.h"
#include <sys/socket.h>
#include <linux/if.h>

#include <bcmnet.h>
#include <cms_mem.h>
#include <cms_msg.h>
#include <rdpa_types.h>
#include "board.h"
#include "detect.h"

int rdpaCtl_RdpaMwWanConf(void);
int rdpaCtl_time_sync_init(void);

#define wc_log_err(fmt, arg...) fprintf(stderr, "wanconf %s:%d >> " fmt "\n", __FILE__, __LINE__, ##arg);
#define wc_log(fmt, arg...) printf("wanconf: " fmt "\n", ##arg);

#define AUTO_STR  "AUTO"
#define GPON_STR  "GPON"
#define EPON_STR  "EPON"
#define AE_STR    "AE"
#define GBE_STR   "GBE"

#define insmod(d) insmod_param(d, NULL)

static CmsRet sendWanOpStateMsg(void *msgHandle, WanConfPhyType phyType, UBOOL8 opState);

int insmod_param(char *driver, char *param)
{
    char cmd[128];
    int ret;

    sprintf(cmd, "insmod %s %s", driver, param ? : "");
    ret = system(cmd);
    if (ret)
        wc_log_err("unable to load module: %s\n", cmd);

    return ret;
}

CmsRet smd_start_app(UINT32 wordData)
{
    void *msgBuf;
    CmsRet ret;
    CmsMsgHeader *msg;
    void *msgHandle;
    int pid;

    ret = cmsMsg_initWithFlags(EID_WANCONF, 0, &msgHandle);
    if (ret)
    {
        wc_log_err("message init failed ret=%d\n", ret);
        return ret;
    }

    msgBuf = cmsMem_alloc(sizeof(CmsMsgHeader), ALLOC_ZEROIZE);
    if (msgBuf == NULL)
    {
        wc_log_err("message allocation failed\n");
        return CMSRET_INTERNAL_ERROR;
    }

    msg = (CmsMsgHeader *)msgBuf;
    msg->src = EID_WANCONF;
    msg->dst = EID_SMD;
    msg->flags_event = FALSE;
    msg->type = CMS_MSG_START_APP;
    msg->wordData = wordData;
    msg->dataLength = 0;

    pid = (int)cmsMsg_sendAndGetReply(msgHandle, msg);
    if (pid == CMS_INVALID_PID)
    {
        wc_log_err("Failed to start app\n");
        return CMSRET_INTERNAL_ERROR;
    }

    if (wordData == EID_EPON_APP)
    {
        ret = sendWanOpStateMsg(msgHandle, WANCONF_PHY_TYPE_EPON, TRUE);
    }
    else if (wordData == EID_OMCID)
    {
        ret = sendWanOpStateMsg(msgHandle, WANCONF_PHY_TYPE_GPON, TRUE);
    }

    CMSMEM_FREE_BUF_AND_NULL_PTR(msgBuf);
    cmsMsg_cleanup(&msgHandle);

    return ret;
}

int load_gpon_modules(void)
{
    int rc = insmod("/lib/modules/"KERNELVER"/extra/bcmgpon.ko");

    rc = rc ? : system("cat /dev/rgs_logger &");
    if (!rc && (smd_start_app(EID_OMCID)) != CMSRET_SUCCESS)
    {
        wc_log_err("Failed to start omcid app\n");
        return -1;
    }

    return rc;
}

int load_epon_modules(void)
{
    int rc = insmod_param("/lib/modules/"KERNELVER"/extra/epon_stack.ko", "epon_usr_init=1");

    if (!rc && (smd_start_app(EID_EPON_APP) != CMSRET_SUCCESS))
    {
        wc_log_err("Failed to start eponapp\n");
        return -1;
    }

    return rc;
}

static int detect_wan_type(rdpa_wan_type *wan_type, int *epon2g)
{
    int fd, ret, val;

    fd = open("/dev/opticaldetect", O_RDWR);
    if (fd < 0)
    {
        wc_log_err("%s: %s\n", "/dev/opt..", strerror(errno));
        return -EINVAL;
    }

    /* block till Optical Signal is detected */
    wc_log("block till Optical Signal is detected...\n");
    ret = ioctl(fd, OPTICALDET_IOCTL_SD, &val);
    if (ret)
        wc_log_err("ioctl failed, Errno[%s] ret=%d\n", strerror(errno), ret);

    wc_log("Optical Signal is detected!\n");

    /* got a signal. Define the type of the signal : GPON, EPON, AE */
    ret = ioctl(fd, OPTICALDET_IOCTL_DETECT, &val);
    if (ret)
        wc_log_err("ioctl failed, Errno[%s] ret=%d\n", strerror(errno), ret);

    close(fd);

    *wan_type = val & RDPA_WAN_MASK;
    *epon2g = val & EPON2G;
	
    return 0;
}

char *wan_type_str(rdpa_wan_type wan_type)
{
    switch (wan_type)
    {
    case rdpa_wan_gpon:
        return GPON_STR;
    case rdpa_wan_epon:
        return EPON_STR;
    case rdpa_wan_gbe: /* Detection code will return rdpa_wan_gbe for AE */
        return GBE_STR;
    default:
        return NULL;
    }
}

static rdpa_wan_type detect_and_set_scratchpad(void)
{
    char *wanStr = NULL;
    int epon2g;
    rdpa_wan_type wan_type;

    if (insmod("/lib/modules/"KERNELVER"/extra/opticaldet.ko"))
        return -1;

    while (!wanStr)
    {
        if (!detect_wan_type(&wan_type, &epon2g))
            wanStr = wan_type_str(wan_type);
        if (!wanStr)
            wc_log_err("Failed to autodetect, retrying\n");
    }

    /* Set detected WAN in scratchpad */

    wc_log("cmsPsp_set wanStr = %s\n", wanStr);
    if (cmsPsp_set(RDPA_WAN_TYPE_PSP_KEY, wanStr, strlen(wanStr)))
    {
        wc_log_err("cmsPsp_set failed to set new "RDPA_WAN_TYPE_PSP_KEY" = %s\n", wanStr);
        return -1;
    }

    if (wan_type == rdpa_wan_gbe) /* Detect rdpa_wan_gbe is always AE! */
    {
        char *emac_str = "EMAC5";

        if (cmsPsp_set(RDPA_WAN_OEMAC_PSP_KEY, emac_str, strlen(emac_str)))
            return -1;
    }
    else if (wan_type == rdpa_wan_epon)
    {
        char *epon_speed = epon2g ? "Turbo" : "Normal";

        if (cmsPsp_set(RDPA_EPON_SPEED_PSP_KEY, epon_speed, strlen(epon_speed)))
            return -1;
    }

    return wan_type;
}

int create_bcmenet_vport(char *ifname)
{
    struct ifreq ifr;
    int err, skfd;
    struct ethctl_data ethctl;

    memset(&ethctl, 0x0, sizeof(struct ethctl_data));
    if ((skfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        printf("socket open error\n");
        return -1;
    }

    strcpy(ifr.ifr_name, "bcmsw");
    if ((err = ioctl(skfd, SIOCGIFINDEX, &ifr)) < 0 )
    {
        printf("bcmsw interface does not exist");
        goto exit;
    }

    ethctl.op = ETHINITWAN;
    ifr.ifr_data = (void *)&ethctl;
    err = ioctl(skfd, SIOCETHCTLOPS, &ifr);
    if (err < 0 )
        printf("Error %d bcmenet gbe wan port\n", err);

    strcpy(ifname, ethctl.ifname);

exit:
    close(skfd);
    return err;
}

static int sendEnablePort(char *if_name)
{
#define IFNAMESIZ 16
    char buf[sizeof(CmsMsgHeader) + IFNAMESIZ]={0};
    CmsMsgHeader *msg=(CmsMsgHeader *) buf;
    char *msg_ifname = (char *)(msg+1);
    CmsRet ret;
    void *msgHandle;

    if (strlen(if_name) > IFNAMESIZ -1)
        return -1;

    ret = cmsMsg_initWithFlags(EID_WANCONF, 0, &msgHandle);
    if (ret)
        return ret;

    msg->type = CMS_MSG_LAN_PORT_ENABLE;
    msg->src = EID_WANCONF;
    msg->dst = EID_SSK;
    msg->flags_event = 1;
    msg->dataLength = IFNAMESIZ;

    strcpy(msg_ifname, if_name);

    if ((ret = cmsMsg_send(msgHandle, msg)) != CMSRET_SUCCESS)
    {
        wc_log_err("could not send out CMS_MSG_LAN_PORT_ENABLE, ret=%d", ret);
        return -1;
    }

    cmsMsg_cleanup(&msgHandle);

    return 0;
}

static CmsRet sendWanOpStateMsg(void *msgHandle, WanConfPhyType phyType, UBOOL8 opState)
{
    CmsRet ret = CMSRET_SUCCESS;
    char buf[sizeof(CmsMsgHeader) + sizeof(WanConfPhyOpStateMsgBody)] = {0};
    CmsMsgHeader *msg = (CmsMsgHeader*)buf;
    WanConfPhyOpStateMsgBody *info;

    msg->type = CMS_MSG_WAN_PORT_SET_OPSTATE;
    msg->src = EID_WANCONF;
    msg->dst = EID_SSK;
    msg->flags_request = 1;
    msg->flags_response = 0;
    msg->flags_event = 0;
    msg->dataLength = sizeof(WanConfPhyOpStateMsgBody);
    msg->wordData = 0;

    info = (WanConfPhyOpStateMsgBody*)&(buf[sizeof(CmsMsgHeader)]);
    info->phyType = phyType;
    info->opState = opState;

    /* Wait for reply to serialize the processing. */
    ret = cmsMsg_sendAndGetReply(msgHandle, msg);
    if (ret != CMSRET_SUCCESS)
    {
        wc_log_err("cmsMsg_sendAndGetReply(CMS_MSG_WAN_PORT_SET_OPSTATE) failed, ret=%d", ret);
    }

    return ret;
}


int main(int argc __attribute__((unused)), char *argv[] __attribute__((unused)))
{
    pid_t childPid = fork();
    char buf[16];
    int count, rc;
    rdpa_wan_type wan_type = rdpa_wan_none;

    if (childPid < 0) /* Failed to fork */
        return -1;

    if (childPid != 0) /* Father always exists */
        return 0;

    /* read a scratchpad */
    memset(buf, 0, sizeof(buf));
    count = cmsPsp_get(RDPA_WAN_TYPE_PSP_KEY, buf, sizeof(buf)-1);
    if (count <= 0)
    {
        wc_log_err("unexpected data in %s, len=%d", RDPA_WAN_TYPE_PSP_KEY, count);
        return -1;
    }

    buf[count] = '\0';
    wc_log("cmsPsp_get: rdpaWanType=%s\n", buf);

    if (!strcmp(buf, GPON_STR))
        wan_type = rdpa_wan_gpon;
    else if (!strcmp(buf, EPON_STR))
        wan_type = rdpa_wan_epon;
    else if (!strcmp(buf, AE_STR))
        wan_type = rdpa_wan_gbe;
    else if (!strcmp(buf, GBE_STR))
        wan_type = rdpa_wan_gbe;
    else if (!strcmp(buf, AUTO_STR))
        wan_type = detect_and_set_scratchpad();
    else
    {
        wc_log_err("Unknown wan set in scratchpad %s = %s\n", RDPA_WAN_TYPE_PSP_KEY, buf);
        return -1;
    }

    rc = rdpaCtl_RdpaMwWanConf();
    if (rc)
    {
        wc_log("Failed to call rdpa_mw ioctl (rc=%d)\n", rc);
        return rc;
    }

    switch (wan_type)
    {
        case rdpa_wan_gpon:
            rc = load_gpon_modules();
            break;
        case rdpa_wan_epon:
            rc = load_epon_modules();
            break;
        case rdpa_wan_gbe:
            {
                char ifname[IFNAMESIZ];

                rc = create_bcmenet_vport(ifname);
                rc = rc ? : sendEnablePort(ifname);
                break;
            }
        default:
            wc_log_err("Unsupported wanconf type set in scratchpad %s = %s\n", RDPA_WAN_TYPE_PSP_KEY, buf);
            return -1;
    }

    if (rc)
    {
        wc_log("wanconf was not loaded successfully (rc=%d)\n", rc);
        return rc;
    }

    rc = rdpaCtl_time_sync_init();
    if (rc)
    {
        wc_log("Failed to call rdpaCtl_time_sync_init ioctl (rc=%d)\n", rc);
        return rc;
    }

    return 0;
}

