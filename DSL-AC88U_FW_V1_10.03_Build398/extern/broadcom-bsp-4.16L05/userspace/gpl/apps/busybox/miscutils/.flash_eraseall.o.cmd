cmd_miscutils/flash_eraseall.o := /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/bin/arm-unknown-linux-uclibcgnueabi-gcc -Wp,-MD,miscutils/.flash_eraseall.o.d   -std=gnu99 -Iinclude -Ilibbb  -include include/autoconf.h -D_GNU_SOURCE -DNDEBUG -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE -D_FILE_OFFSET_BITS=64 -D"BB_VER=KBUILD_STR(1.17.2)" -DBB_BT=AUTOCONF_TIMESTAMP  -Wall -Wshadow -Wwrite-strings -Wundef -Wstrict-prototypes -Wunused -Wunused-parameter -Wunused-function -Wunused-value -Wmissing-prototypes -Wmissing-declarations -Wdeclaration-after-statement -Wold-style-definition -fno-builtin-strlen -finline-limit=0 -fomit-frame-pointer -ffunction-sections -fdata-sections -fno-guess-branch-probability -funsigned-char -static-libgcc -falign-functions=1 -falign-jumps=1 -falign-labels=1 -falign-loops=1 -Os -I/home/users/popeye_shen/ac88u-gpl/bcm963xx/targets/VR9517UAC44-A-ASU/fs.build/public/include -I/home/users/popeye_shen/ac88u-gpl/bcm963xx/userspace/public/include -I/home/users/popeye_shen/ac88u-gpl/bcm963xx/userspace/public/include/linux -DTFTPD  -DCMS_LOG3  -DLINUX -Os -march=armv7-a -fomit-frame-pointer -mno-thumb-interwork -mabi=aapcs-linux -marm -fno-common -ffixed-r8 -msoft-float -D__ARM_ARCH_7A__ -Werror=return-type -Werror=uninitialized -Wall -Darm -g -fPIC -DMDM_SHARED_MEM -DCMS_MEM_DEBUG  -DSUPPORT_DM_HYBRID -DDMP_BASELINE_1 -DDMP_X_BROADCOM_COM_BASELINE_1 -DDMP_ETHERNETLAN_1 -DDMP_X_BROADCOM_COM_DEBUG_1 -DDMP_X_BROADCOM_COM_SECURITY_1 -DDMP_DEVICEASSOCIATION_1 -DDMP_IPPING_1 -DDMP_DUSTATECHNGCOMPLPOLICY_1 -DDMP_DEVICE2_BASELINE_1 -DDMP_DEVICE2_X_BROADCOM_COM_BASELINE_1 -DDMP_DEVICE2_BASELINE_2 -DDMP_DEVICE2_BASELINE_3 -DDMP_DEVICE2_IPINTERFACE_1 -DDMP_DEVICE2_IPINTERFACE_2  -DDMP_DEVICE2_ETHERNETLINK_1 -DDMP_DEVICE2_ETHERNETINTERFACE_1 -DDMP_DEVICE2_VLANTERMINATION_1 -DDMP_DEVICE2_PPPINTERFACE_1 -DDMP_DEVICE2_PPPINTERFACE_2 -DDMP_DEVICE2_IPPING_1 -DDMP_DEVICE2_TRACEROUTE_1 -DDMP_DEVICE2_ROUTING_1 -DDMP_DEVICE2_ROUTING_2 -DDMP_DEVICE2_DUSTATECHNGCOMPLPOLICY_1 -DSUPPORT_DSL -DDMP_ADSLWAN_1 -DDMP_X_BROADCOM_COM_ADSLWAN_1  -DDMP_X_BROADCOM_COM_XTMSTATS_1 -DDMP_VDSL2WAN_1 -DDMP_X_BROADCOM_COM_VDSL2WAN_1 -DNONE -DSUPPORT_DSL_GFAST -DDMP_X_BROADCOM_COM_ATMWAN_1 -DDMP_ATMLOOPBACK_1 -DDMP_X_BROADCOM_COM_ATMLOOPBACK_1  -DSUPPORT_PTM -DDMP_PTMWAN_1 -DDMP_X_BROADCOM_COM_PTMWAN_1 -DSUPPORT_ETHWAN -DDMP_ETHERNETWAN_1 -DSUPPORT_RDPA -DSUPPORT_TMCTL -DSUPPORT_GRE_TUNNEL -DSUPPORT_IPSEC -DDMP_X_BROADCOM_COM_IPSEC_1 -DSUPPORT_TR64C -DDMP_X_BROADCOM_COM_TR64_1 -DSUPPORT_IPV6 -DDMP_X_BROADCOM_COM_DEV2_IPV6_1 -DDMP_DEVICE2_DSLITE_1 -DDMP_DEVICE2_DSLITE_2 -DDMP_DEVICE2_IPV6RD_1 -DDMP_DEVICE2_IPV6INTERFACE_1 -DDMP_DEVICE2_IPV6ROUTING_1 -DDMP_DEVICE2_DHCPV6CLIENT_1 -DDMP_DEVICE2_DHCPV6CLIENTSERVERIDENTITY_1  -DDMP_DEVICE2_DHCPV6SERVER_1  -DDMP_DEVICE2_DHCPV6SERVERADV_1 -DDMP_DEVICE2_DHCPV6SERVERCLIENTINFO_1 -DDMP_DEVICE2_NEIGHBORDISCOVERY_1 -DDMP_DEVICE2_ROUTERADVERTISEMENT_1 -DSUPPORT_TR69C -DDMP_DOWNLOAD_1 -DDMP_UPLOAD_1 -DDMP_DOWNLOADTCP_1 -DDMP_UPLOADTCP_1 -DDMP_UDPECHO_1 -DDMP_UDPECHOPLUS_1 -DDMP_PERIODICSTATSBASE_1 -DDMP_PERIODICSTATSADV_1 -DSUPPORT_XMPP -DDMP_DEVICE2_XMPPCONNREQ_1 -DDMP_DEVICE2_XMPPBASIC_1 -DDMP_DEVICE2_XMPPADVANCED_1 -DSUPPORT_CPU_MEMORY_WEB_PAGE -DSUPPORT_JQPLOT -DSUPPORT_WEB_SOCKETS -DSUPPORT_QUICKSETUP -DDMP_X_BROADCOM_COM_QUICKSETUP_1 -DSUPPORT_HTTPD -DSUPPORT_HTTPD_BASIC_SSL -DSUPPORT_CLI_CMD -DCLI_CMD_EDIT -DSUPPORT_CONSOLED -DSUPPORT_TELNETD -DSUPPORT_SSHD -DSUPPORT_FTPD -DSUPPORT_EBTABLES -DSUPPORT_TOD -DDMP_X_BROADCOM_COM_ACCESSTIMERESTRICTION_1 -DSUPPORT_URLFILTER -DSUPPORT_POLICYROUTING -DSUPPORT_UPNP -DDMP_X_BROADCOM_COM_UPNP_1 -DSUPPORT_DDNSD -DDMP_X_BROADCOM_COM_DYNAMICDNS_1 -DSUPPORT_DNSPROXY -DDMP_X_BROADCOM_COM_DNSPROXY_1 -DSUPPORT_IPP -DDMP_X_BROADCOM_COM_IPPRINTING_1 -DDMP_X_BROADCOM_COM_DLNA_1 -DSUPPORT_FCCTL -DSUPPORT_DSLDIAGD -DSUPPORT_SNTP -DDMP_TIME_1 -DSUPPORT_UDHCP -DDHCP_SERVER_DEFAULT -DDMP_X_BROADCOM_COM_ETHERNETOAM_1 -DSUPPORT_ETHSWCTL -DSUPPORT_PWRMNGT -DDMP_X_BROADCOM_COM_PWRMNGT_1 -DSUPPORT_HOSTMIPS_PWRSAVE -DSUPPORT_ETH_PWRSAVE -DSUPPORT_ENERGY_EFFICIENT_ETHERNET -DSUPPORT_ETH_DEEP_GREEN_MODE -DSUPPORT_BMU -DDMP_X_BROADCOM_COM_BMU_1 -DSUPPORT_STORAGESERVICE -DDMP_STORAGESERVICE_1 -DSUPPORT_NTFS_3G -DSUPPORT_SAMBA -DDMP_X_BROADCOM_COM_MCAST_1 -DDMP_X_BROADCOM_COM_IGMPSNOOP_1 -DDMP_X_BROADCOM_COM_MLDSNOOP_1 -DSUPPORT_IGMP -DDMP_X_BROADCOM_COM_IGMP_1 -DSUPPORT_MLD -DDMP_X_BROADCOM_COM_MLD_1 -DSUPPORT_PPTP -DSUPPORT_SIP -DSUPPORT_NF_MANGLE -DSUPPORT_INTF_GROUPING -DSUPPORT_PORT_MAP -DDMP_BRIDGING_1  -DSUPPORT_WANVLANMUX -DSUPPORT_VLANCTL -DSUPPORT_LANVLAN -DSUPPORT_CMS_ALLOW_REAL_HW_SWITCHING -DSUPPORT_QOS -DDMP_QOS_1 -DDMP_QOSDYNAMICFLOW_1 -DDMP_X_BROADCOM_COM_QOS_1 -DSUPPORT_RATE_LIMIT -DSUPPORT_DEBUG_TOOLS -DSUPPORT_CERT -DDMP_X_BROADCOM_COM_DIGITALCERTIFICATES_1 -DSUPPORT_RIP -DCOMPRESSED_CONFIG_FILE -DCMS_CONFIG_COMPAT -DDMP_WIFILAN_1 -DDMP_X_BROADCOM_COM_WIFILAN_1 -DCHIP_63138	-DCONFIG_BCM963138 -DBRCM_CMS_BUILD -DCONFIG_BCM_MAX_GEM_PORTS=1 -DSUPPORT_SECURE_BOOT -DSUPPORT_INCREMENTAL_FLASHING  -DCMS_LOG3 -I/opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/include -Werror=return-type -Werror=uninitialized -I/home/users/popeye_shen/ac88u-gpl/bcm963xx/kernel     -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(flash_eraseall)"  -D"KBUILD_MODNAME=KBUILD_STR(flash_eraseall)" -c -o miscutils/flash_eraseall.o miscutils/flash_eraseall.c

deps_miscutils/flash_eraseall.o := \
  miscutils/flash_eraseall.c \
    $(wildcard include/config/brcmnand/mtd/extension.h) \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/getopt.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/getopt.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/features.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uClibc_config.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/cdefs.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/types.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/types.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/wordsize.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/lib/gcc/arm-unknown-linux-uclibcgnueabi/4.6.2/include/stddef.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/typesizes.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/pthreadtypes.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/endian.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/endian.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/byteswap.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/byteswap.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/byteswap-common.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/time.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/select.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/select.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/sigset.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/time.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/sysmacros.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/stat.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/stat.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/ioctl.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/ioctls.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm/ioctls.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm-generic/ioctls.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/ioctl.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm/ioctl.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm-generic/ioctl.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/ioctl-types.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/ttydefaults.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/ctype.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uClibc_touplow.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/xlocale.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/locale.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uClibc_locale.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/stdlib.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/waitflags.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/waitstatus.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/alloca.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/stdio.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uClibc_stdio.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/wchar.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/wchar.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uClibc_mutex.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/pthread.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sched.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/sched.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uClibc_clk_tck.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/signal.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/setjmp.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uClibc_pthread.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/lib/gcc/arm-unknown-linux-uclibcgnueabi/4.6.2/include/stdarg.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/stdio_lim.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/string.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/errno.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/errno.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/errno.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm/errno.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm-generic/errno.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm-generic/errno-base.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/unistd.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/posix_opt.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/environments.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/confname.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/fcntl.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/fcntl.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uio.h \
  include/libbb.h \
    $(wildcard include/config/selinux.h) \
    $(wildcard include/config/locale/support.h) \
    $(wildcard include/config/feature/shadowpasswds.h) \
    $(wildcard include/config/use/bb/shadow.h) \
    $(wildcard include/config/use/bb/pwd/grp.h) \
    $(wildcard include/config/lfs.h) \
    $(wildcard include/config/feature/buffers/go/on/stack.h) \
    $(wildcard include/config/feature/buffers/go/in/bss.h) \
    $(wildcard include/config/feature/ipv6.h) \
    $(wildcard include/config/feature/seamless/lzma.h) \
    $(wildcard include/config/feature/seamless/bz2.h) \
    $(wildcard include/config/feature/seamless/gz.h) \
    $(wildcard include/config/feature/seamless/z.h) \
    $(wildcard include/config/feature/check/names.h) \
    $(wildcard include/config/feature/utmp.h) \
    $(wildcard include/config/feature/prefer/applets.h) \
    $(wildcard include/config/busybox/exec/path.h) \
    $(wildcard include/config/long/opts.h) \
    $(wildcard include/config/feature/getopt/long.h) \
    $(wildcard include/config/feature/pidfile.h) \
    $(wildcard include/config/feature/syslog.h) \
    $(wildcard include/config/feature/individual.h) \
    $(wildcard include/config/echo.h) \
    $(wildcard include/config/printf.h) \
    $(wildcard include/config/test.h) \
    $(wildcard include/config/kill.h) \
    $(wildcard include/config/chown.h) \
    $(wildcard include/config/ls.h) \
    $(wildcard include/config/xxx.h) \
    $(wildcard include/config/route.h) \
    $(wildcard include/config/feature/hwib.h) \
    $(wildcard include/config/desktop.h) \
    $(wildcard include/config/feature/crond/d.h) \
    $(wildcard include/config/use/bb/crypt.h) \
    $(wildcard include/config/feature/adduser/to/group.h) \
    $(wildcard include/config/feature/del/user/from/group.h) \
    $(wildcard include/config/ioctl/hex2str/error.h) \
    $(wildcard include/config/feature/editing.h) \
    $(wildcard include/config/feature/editing/history.h) \
    $(wildcard include/config/feature/editing/savehistory.h) \
    $(wildcard include/config/feature/tab/completion.h) \
    $(wildcard include/config/feature/username/completion.h) \
    $(wildcard include/config/feature/editing/vi.h) \
    $(wildcard include/config/feature/show/threads.h) \
    $(wildcard include/config/feature/ps/additional/columns.h) \
    $(wildcard include/config/feature/topmem.h) \
    $(wildcard include/config/feature/top/smp/process.h) \
    $(wildcard include/config/killall.h) \
    $(wildcard include/config/pgrep.h) \
    $(wildcard include/config/pkill.h) \
    $(wildcard include/config/pidof.h) \
    $(wildcard include/config/sestatus.h) \
    $(wildcard include/config/feature/devfs.h) \
  include/platform.h \
    $(wildcard include/config/werror.h) \
    $(wildcard include/config/nommu.h) \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/lib/gcc/arm-unknown-linux-uclibcgnueabi/4.6.2/include/stdbool.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/dirent.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/dirent.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/posix1_lim.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/local_lim.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/limits.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/uClibc_local_lim.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/inttypes.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/lib/gcc/arm-unknown-linux-uclibcgnueabi/4.6.2/include/stdint.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/stdint.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/netdb.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/netinet/in.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/socket.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/uio.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/socket.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/lib/gcc/arm-unknown-linux-uclibcgnueabi/4.6.2/include-fixed/limits.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/lib/gcc/arm-unknown-linux-uclibcgnueabi/4.6.2/include-fixed/syslimits.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/limits.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/posix2_lim.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/xopen_lim.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/sockaddr.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm/socket.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm/sockios.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/in.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/rpc/netdb.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/siginfo.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/netdb.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/setjmp.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/signum.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/sigaction.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/sigcontext.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm/sigcontext.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/sigstack.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/ucontext.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/procfs.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/time.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/user.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/sigthread.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/poll.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/poll.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/mman.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/mman.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/mman-common.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/wait.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/resource.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/resource.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/termios.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/termios.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/param.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/param.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm/param.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/mntent.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/paths.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/sys/statfs.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/bits/statfs.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/pwd.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/grp.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/arpa/inet.h \
  include/xatonum.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/mtd/mtd-user.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/mtd/mtd-abi.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/types.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm/types.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm-generic/int-ll64.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm/bitsperlong.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm-generic/bitsperlong.h \
    $(wildcard include/config/64bit.h) \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/posix_types.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/stddef.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm/posix_types.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/asm-generic/posix_types.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/version.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/jffs2.h \
  /opt/toolchains/crosstools-arm-gcc-4.6-linux-3.4-uclibc-0.9.32-binutils-2.21-NPTL/usr/arm-unknown-linux-uclibcgnueabi/sysroot/usr/include/linux/magic.h \

miscutils/flash_eraseall.o: $(deps_miscutils/flash_eraseall.o)

$(deps_miscutils/flash_eraseall.o):
