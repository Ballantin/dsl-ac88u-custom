/***********************************************************************
 *
 * <:copyright-BRCM:2006:DUAL/GPL:standard
 * 
 *    Copyright (c) 2006 Broadcom 
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

/*
 * This header file contains all the functions exported by the
 * OS Adaptation Layer (OAL).  The OAL functions live under the
 * directory with the name of the OS, e.g. linux, ecos.
 * The Make system will automatically compile the appropriate files
 * and link them in with the final executable based on the TARGET_OS
 * variable, which is set by make menuconfig.
 */

#ifndef __OAL_H__
#define __OAL_H__

#include "cms.h"
#include "cms_eid.h"
#include "cms_dlist.h"
#include "cms_log.h"
#include "cms_tms.h"
#include "cms_net.h"
#include "prctl.h"


extern void oalLog_init(void);
extern void oalLog_syslog(CmsLogLevel level, const char *buf);
extern void oalLog_cleanup(void);

/* in oal_readlog.c (legacy method of reading syslog) */
extern int oal_readLogPartial(int ptr, char* buffer);


void *oal_malloc(UINT32 size);
void oal_free(void *buf);

/* in oal_timestamp.c */
void oalTms_get(CmsTimestamp *tms);
CmsRet oalTms_getXSIDateTime(UINT32 t, char *buf, UINT32 bufLen);
CmsRet oalTms_getXSIDateTimeMicroseconds(UINT32 t, char *buf, UINT32 bufLen);

/* in oal_strconv.c */
CmsRet oal_strtol(const char *str, char **endptr, SINT32 base, SINT32 *val);
CmsRet oal_strtoul(const char *str, char **endptr, SINT32 base, UINT32 *val);
CmsRet oal_strtol64(const char *str, char **endptr, SINT32 base, SINT64 *val);
CmsRet oal_strtoul64(const char *str, char **endptr, SINT32 base, UINT64 *val);

/* in oal_pid.c */
SINT32 oal_getPid(void);

/* in oal_prctl.c */
extern CmsRet oal_spawnProcess(const SpawnProcessInfo *spawnInfo, SpawnedProcessInfo *procInfo);
extern CmsRet oal_collectProcess(const CollectProcessInfo *collectInfo, SpawnedProcessInfo *procInfo);
extern CmsRet oal_terminateProcessGracefully(SINT32 pid);
extern CmsRet oal_terminateProcessForcefully(SINT32 pid);
extern CmsRet oal_signalProcess(SINT32 pid, SINT32 sig);
extern int oal_getPidByName(const char *name);
extern int oal_getNameByPid(int pid, char *nameBuf, int nameBufLen);
extern CmsRet oal_setScheduler(SINT32 pid, SINT32 policy, SINT32 priority);
extern CmsRet oal_setCpuMask(SINT32 pid, UINT32 cpuMask);
extern CmsRet oal_setCgroup(SINT32 pid, const char *groupBase, const char *groupName);

/* in oal_network.c */
extern CmsRet oal_getLanInfo(const char *lan_ifname, struct in_addr *lan_ip, struct in_addr *lan_subnetmask);
extern UBOOL8 oal_isInterfaceUp(const char *ifname);
extern UBOOL8 oal_isInterfaceLinkUp(const char *ifname);
extern CmsRet oal_Net_getIfNameList(char **ifNameList);
CmsRet oal_Net_getPersistentWanIfNameList(char **PersistentWanifNameList);
extern CmsRet oal_Net_getGMACPortIfNameList(char **GMACPortIfNameList);
extern CmsRet oal_Net_WANOnlyEthPortIfNameList(char **WANOnlyEthPortIfNameList __attribute__((unused)));
extern CmsRet oal_Net_LANOnlyEthPortIfNameList(char **LANOnlyEthPortIfNameList __attribute__((unused)));


int oal_getIfindexByIfname(const char *ifname);
int oal_getIfnameByIndex(int index, char *intfName);
CmsRet oal_saveIfNameFromSocket(SINT32 socketfd, char *connIfName);

#ifdef SUPPORT_IPV6
extern CmsRet oal_getLanAddr6(const char *ifname, char *ipAddr);
extern CmsRet oal_getIfAddr6(const char *ifname, UINT32 addrIdx,
                      char *ipAddr, UINT32 *ifIndex, UINT32 *prefixLen, UINT32 *scope, UINT32 *ifaFlags);
#endif

/* in oal_file.c */
extern UBOOL8 oalFil_isFilePresent(const char *filename);
extern SINT32 oalFil_getSize(const char *filename);
extern CmsRet oalFil_copyToBuffer(const char *filename, UINT8 *buf, UINT32 *bufSize);
extern CmsRet oalFil_writeToProc(const char *procFilename, const char *s);
extern CmsRet oalFil_writeBufferToFile(const char *filename, const UINT8 *buf,
                                       UINT32 bufLen);
extern UBOOL8 oalFil_isDirPresent(const char *dirname);
extern CmsRet oalFil_removeDir(const char *dirname);
extern CmsRet oalFil_makeDir(const char *dirname);
extern CmsRet oalFil_getOrderedFileList(const char *dirname, DlistNode *dirHead);
extern CmsRet oalFil_getNumFilesInDir(const char *dirname, UINT32 *num);
extern CmsRet oalFil_renameFile(const char *oldName, const char *newName);
extern CmsRet oalFil_getNumericalOrderedFileList(const char *dirname, DlistNode *dirHead);
extern CmsRet oal_getIntPrefixFromFileName(char *fileName, UINT32 *pNum);
extern CmsRet oalFil_readFirstlineFromFile(char *fileName, char *line, UINT32 lineSize);

/* in oal_passwd.c */
extern char * oalPass_crypt(const char *clear, const char *salt);

extern void oal_getRandomBytes(unsigned char *buf, int len);
extern int oal_getRandomUuid(char *uuidStr, int len);

#endif /* __OAL_H__ */
