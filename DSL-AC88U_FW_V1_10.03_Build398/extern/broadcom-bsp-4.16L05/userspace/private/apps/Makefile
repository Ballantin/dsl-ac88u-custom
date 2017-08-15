#***********************************************************************
#
#  Copyright (c) 2006  Broadcom Corporation
#  All Rights Reserved
#
#
#***********************************************************************/


#
# You do not need to modify this Makefile to add an app.
# Simply create a .autodetect in the subdir and this Makefile will enter
# that directory during make and make clean.  (Auto detected directories
# are listed in the AUTODETECT_SUBDIRS variable.  Type make show_subdirs)
#
HARDCODED_SUBDIRS := smd ssk omcid tr69c wlan httpd consoled telnetd \
           xdslctl gponif vlanctl atmctl ethctl ethswctl cmfctl cmfviz \
           ddnsd dnsprobe dnsspoof sntp upnp ippd x_dms dsldiagd\
           snmp tr64 hotplug pmonctl bounce soapserver \
           spuctl pwrctl gpond gponctl cmfd omcipmdrvctl mcpd fapctl\
           fcctl eponapp mdkshell stress cellularapp \
           dumpsysinfo iqctl bpmctl arlctl mcpctl eponctl rastatus6 \
           plcnvm plcboot gmacctl tmsctl seltctl plc_l2upgrade bcmtm

# ARCADYAN
HARDCODED_SUBDIRS += arc_board fw_upgrade

#DECT & FXS verification tools
ifneq ($(strip $(BUILD_VODSL)),)
HARDCODED_SUBDIRS += dspgctl
endif
## ARCADYAN

SUBDIRS = $(HARDCODED_SUBDIRS) \
          $(AUTODETECT_SUBDIRS_BASE)

# === this is the default target ===
# Need to add a level of indirection here so that AUTODETECT_SUBDIRS and
# MODSW_SUBDIRS can be populated in make.common/make.modsw before we build.
#
all : make_all_subdirs


#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))
export BUILD_DIR

include $(BUILD_DIR)/make.common

# include make.modsw for autodetect variables
include $(BUILD_DIR)/make.modsw

ifneq ($(strip $(BUILD_VODSL)),)
include $(BUILD_DIR)/make.voice
endif



#
# We need to declare these directory names as phony targets,
# otherwise make will think the directories themselves are
# the targets and will never execute the cmds.
# See GNU Make, 4.6 
#
.PHONY : $(SUBDIRS)


base : $(SUBDIRS)

make_all_subdirs : base


clean: generic_subdir_clean



##############################################################
# 
# Rules for inidividual apps
# apps can be compiled or not depending on the profile (which
# is modified via make menuconfig)
#
##############################################################


$(AUTODETECT_SUBDIRS_BASE): 
	$(MAKE) -C $@


# smd is the system management daemon, it replaces cfm.
# ssk is a helper daemon for smd.
ifneq ($(strip $(BUILD_BRCM_CMS)),)

smd: 
	$(MAKE) -C $@

ssk:
	$(MAKE) -C $@

else

smd:
	@echo "skipping smd (CMS not configured)"

ssk:
	@echo "skipping ssk (CMS not configured)"

endif




#
# There are three possible variables for tr69c in make menuconfig:
# BUILD_TR69C 
# BUILD_TR69C_SSL
# BUILD_TR69C_BCM_SSL
# (or none)
# If any of the tr69c variants are defined, just go into the tr69c directory
# and make the target.

ifneq ($(strip $(BUILD_BRCM_CMS)),)
ifneq ($(strip $(BUILD_TR69C)),)
tr69c:
	$(MAKE) -C $@
else
ifneq ($(strip $(BUILD_TR69C_SSL)),)
tr69c:
	$(MAKE) -C $@
else
ifneq ($(strip $(BUILD_TR69C_BCM_SSL)),)
tr69c:
	$(MAKE) -C $@
else
tr69c:
	@echo "skipping tr69c (not configured)"
endif
endif
endif
else
	@echo "skipping tr69c (CMS not configured)"
	
endif

ifneq ($(strip $(BUILD_BRCM_CMS)),)
ifneq ($(strip $(BUILD_OMCI)),)
omcid: 
	$(MAKE) -C $@
omcipmdrvctl:
	$(MAKE) -C $@
endif
else
	@echo "skipping omcid ( CMS not configured)"
endif

# httpd - Uses libraries built in wlan folder
ifneq ($(strip $(BUILD_BRCM_CMS)),)
ifneq ($(strip $(BUILD_HTTPD)),)
httpd: wlan 
	$(MAKE) -C $@
else
ifneq ($(strip $(BUILD_HTTPD_SSL)),)
httpd: wlan
	$(MAKE) -C $@
else
ifneq ($(strip $(BUILD_HTTPD_BASIC_SSL)),)
httpd: wlan
	$(MAKE) -C $@
else
httpd:
	@echo "skipping httpd (not configured)"
endif
endif
endif
else
	@echo "skipping httpd (CMS not configured)"
endif



# consoled
ifneq ($(strip $(BUILD_BRCM_CMS)),)
ifneq ($(strip $(BUILD_CONSOLED)),)
consoled:
	$(MAKE) -C $@
else
consoled:
	@echo "skipping consoled (not configured)"
endif
else
	@echo "skipping consoled (CMS not configured)"
endif

# telnetd
ifneq ($(strip $(BUILD_BRCM_CMS)),)
ifneq ($(strip $(BUILD_TELNETD)),)
telnetd:
	$(MAKE) -C $@
else
telnetd:
	@echo "skipping telnetd (not configured)"
endif
else
telnetd:
	@echo "skipping telnetd (CMS not configured)"

endif

# atmctl
ifneq ($(strip $(BUILD_XTMCTL)),)
atmctl:
	$(MAKE) -C $@
else
atmctl:
	@echo "skipping atmctl (not configured)"
endif


# xdslctl
ifneq ($(strip $(DESKTOP_LINUX)),)
	@echo "skipping xdslctl (not used on desktop linux)"
else
ifneq ($(strip $(BUILD_XDSLCTL)),)
xdslctl: 
	$(MAKE) -C $@
else
xdslctl:
	@echo "skipping xdslctl (not configured)"
endif
endif

#seltctl
ifneq ($(strip $(DESKTOP_LINUX)),)
	@echo "skipping seltctl (not used on desktop linux)"
else
ifneq ($(strip $(BUILD_DSL_SELT_TEST)),)
seltctl: 
	$(MAKE) -C $@
else
seltctl:
	@echo "skipping seltctl (not configured)"
endif
endif

# vlanctl
ifneq ($(strip $(BUILD_VLANCTL)),)
vlanctl: 
	$(MAKE) -C $@
else
vlanctl:
	@echo "skipping vlanctl (not configured)"
endif

# gponif
ifneq ($(strip $(BUILD_GPONIF)),)
gponif: 
	$(MAKE) -C $@
else
gponif:
	@echo "skipping gponif (not configured)"
endif

# gpond
ifneq ($(strip $(BUILD_GPOND)),)
gpond: 
	$(MAKE) -C $@
else
gpond:
	@echo "skipping gpond (not configured)"
endif

# gponctl
ifneq ($(strip $(BUILD_GPONCTL)),)
gponctl:
	$(MAKE) -C $@
else
gponctl:
	@echo "skipping gponctl (not configured)"
endif

# ethswctl
ifneq ($(strip $(BUILD_ETHSWCTL)),)
ethswctl: 
	$(MAKE) -C $@
else
ethswctl:
	@echo "skipping ethswctl (not configured)"
endif

#dynamic dns
ifneq ($(strip $(BUILD_DDNSD)),)
ddnsd:
	$(MAKE) -C ddnsd $(BUILD_DDNSD)
else
ddnsd:
	@echo "skipping ddnsd (not configured)"
endif

# dnsprobe
ifneq ($(strip $(BUILD_DNSPROBE)),)
dnsprobe:
	$(MAKE) -C dnsprobe $(BUILD_DNSPROBE)
else
dnsprobe:
	@echo "skipping dnsprobe (not configured)"
endif


# dnsspoof
ifneq ($(strip $(BUILD_DNSSPOOF)),)
dnsspoof:
	$(MAKE) -C dnsspoof $(BUILD_DNSSPOOF)
else
dnsspoof:
	@echo "skipping dnsspoof (not configured)"
endif


#Always compile Ethernet control utility
ethctl:
	$(MAKE) -C ethctl dynamic


ifneq ($(strip $(BUILD_BRCM_CMS)),)
ifneq ($(strip $(BUILD_MCAST_PROXY)),)
mcpd:
	$(MAKE) -C mcpd $(BUILD_MCAST_PROXY) BUILD_BRCM_FTTDP=$(BUILD_BRCM_FTTDP)
else
mcpd:
	@echo "skipping multicast proxy (mcpd) (not configured)"
endif
else
	@echo "skipping multicast proxy (mcpd) (CMS not configured)"
endif

ifneq ($(strip $(BUILD_MCAST_PROXY)),)
mcpctl:
	$(MAKE) -C mcpctl $(BUILD_MCAST_PROXY)
else
mcpctl:
	@echo "skipping mcpctl (not configured)"
endif

# Time synchronization daemon
ifneq ($(strip $(BUILD_SNTP)),)
sntp:
	$(MAKE) -C sntp $(BUILD_SNTP)
else
sntp:
	@echo "skipping sntp (not configured)"
endif


# UPNP is dependent on iptables
ifneq ($(strip $(BUILD_BRCM_CMS)),)
ifneq ($(strip $(BUILD_IPTABLES)),)
ifneq ($(strip $(BUILD_UPNP)),)
upnp:
	$(MAKE) -C upnp $(BUILD_UPNP)
else
upnp:
	@echo "skipping upnp (not configured)"
endif
else
upnp:
	@echo Warning: upnp requires iptables, please enable iptables.
endif
else
	@echo "skipping upnp (CMS not configured)"
endif


# IP Printing Daemon
ifneq ($(strip $(BUILD_IPPD)),)
ippd:
	$(MAKE) -C ippd $(BUILD_IPPD)
else
ippd:
	@echo "skipping ippd (not configured)"
endif


# DSL Diag Daemon
ifneq ($(strip $(BUILD_DSLDIAGD)),)
dsldiagd:
	$(MAKE) -C $@
else
dsldiagd:
	@echo "skipping dsldiagd (not configured)"
endif


# Packet CMF Control Utility
ifneq ($(strip $(BUILD_CMFCTL)),)
cmfctl:
	$(MAKE) -C $@ $(BUILD_CMFCTL)
else
cmfctl:
	@echo "Skipping cmfctl application (Packet CMF not configured)"
endif

# Packet CMF Viz Application
ifneq ($(strip $(BUILD_CMFVIZ)),)
cmfviz:
	$(MAKE) -C cmfviz $(BUILD_CMFVIZ)
else
cmfviz:
	@echo "Skipping cmfviz application (Packet CMF not configured)"
endif

# Packet CMF Daemon Application
ifneq ($(strip $(BUILD_CMFD)),)
cmfd:
	$(MAKE) -C cmfd $(BUILD_CMFD)
else
cmfd:
	@echo "Skipping CMF Daemon application"
endif

# FAP Control Utility
ifneq ($(strip $(BUILD_FAPCTL)),)
fapctl:
	$(MAKE) -C $@
else
fapctl:
	@echo "Skipping fapctl application (not configured)"
endif

# wlan software
ifneq ($(strip $(BRCM_DRIVER_WIRELESS)),)
wlan:
	$(MAKE) -C $@
else
wlan:
	@echo "skipping wlan (not configured)"
endif


# snmp
ifneq ($(strip $(BUILD_SNMP)),)
snmp:
#	$(MAKE) -C snmp $(BUILD_SNMP)
	$(MAKE) -C $@
else
snmp:
	@echo "skipping snmp (not configured)"
endif

# tr64
ifneq ($(strip $(BUILD_BRCM_CMS)),)
ifneq ($(strip $(BUILD_TR64)),)
tr64:
	$(MAKE) -C tr64
else
tr64:
	@echo "skipping tr64 (not configured)"
endif
else
	@echo "skipping tr64 (CMS not configured)"
endif
# PLC NVM
ifneq ($(strip $(BUILD_HOMEPLUG)),)
plcnvm:
	$(MAKE) -C $@
else
plcnvm:
	@echo "skipping plcnvm (not configured)"
endif

# PLC_L2UPGRADE
ifneq ($(strip $(BUILD_PLC_L2UPGRADE)),)
plc_l2upgrade:
	$(MAKE) -C $@
else
plc_l2upgrade:
	@echo "skipping plc_l2upgrade (not configured)"
endif

# PLC BOOT
ifneq ($(strip $(BUILD_HOMEPLUG)),)
plcboot:
	$(MAKE) -C $@
else
plcboot:
	@echo "skipping plcboot (not configured)"
endif

#hotplug software
ifneq ($(strip $(BUILD_NAS)),)
hotplug:
	$(MAKE) -C $@ dynamic
else
ifneq ($(strip $(LINUX_KERNEL_USBMASS)),)
hotplug:
	$(MAKE) -C $@ dynamic
else
hotplug:
	@echo "skipping hotplug (not configured)"
endif
endif

# Build the BUZZZ shell control utility
ifneq ($(strip $(BUILD_BUZZZ)),)
buzzzctl:
	$(MAKE) -C $@
else
buzzzctl:
	@echo "skipping $@ (not configured)"
endif

# Build the PMON shell control utility
ifneq ($(strip $(BUILD_PMON)),)
pmonctl:
	$(MAKE) -C $@
else
pmonctl:
	@echo "skipping $@ (not configured)"
endif

# Build the BOUNCE shell control utility
ifneq ($(strip $(BUILD_BOUNCE)),)
bounce:
	$(MAKE) -C $@
else
bounce:
	@echo "skipping $@ (not configured)"
endif

# soapserver
ifneq ($(strip $(BUILD_SOAP)),)
ifeq ($(strip $(BUILD_SOAP_VER)),3)
soapserver:
	$(MAKE) -C $@
endif
else
soapserver:
	@echo "skipping soapserver (not configured)"
endif

# spuctl
ifneq ($(strip $(BUILD_SPUCTL)),)
spuctl:
	$(MAKE) -C $@
else
spuctl:
	@echo "skipping spuctl (not configured)"
endif

# pwrctl
ifneq ($(strip $(BUILD_PWRCTL)),)
pwrctl:
	$(MAKE) -C $@
else
pwrctl:
	@echo "skipping pwrctl (not configured)"
endif

# Flow Cache Control Utility
ifneq ($(strip $(BUILD_FCCTL)),)
fcctl:
	$(MAKE) -C $@ $(BUILD_FCCTL)
else
fcctl:
	@echo "Skipping fcctl application (Flow Cache not configured)"
endif

#x_dms
ifneq ($(strip $(BUILD_DLNA)),)
x_dms:
	(tar xkfj x_dms.tar.bz2 2> /dev/null || true)
	$(MAKE) -j 1 -C x_dms dynamic
else
x_dms:
	@echo "skipping dlna (not configured)"
endif

eponapp:
ifneq ($(strip $(BUILD_EPON_SDK)),)
	$(MAKE) -C $@
else
	@echo "Skipping EPON SDK APP "
endif

cellularapp:
ifneq ($(strip $(BUILD_CELLULAR)),)
	$(MAKE) -C $@
else
	@echo "Skipping cellularapp"
endif

mdkshell:
ifneq ($(strip $(BUILD_MDK_SHELL)),)
	$(MAKE) -C $@
else
	@echo "Skipping MDK SHELL "
endif

# Stress application
stress:
ifneq ($(strip $(BUILD_STRESS)),)
	$(MAKE) -C $@
else
	@echo "Skipping $@ (not configured) "
endif

# Dump system info for bug reporting
# Lump it in with debug tools.  Don't know if we actually need
# a separate config option for this or not.
ifneq ($(strip $(BUILD_DEBUG_TOOLS)),)
dumpsysinfo:
	$(MAKE) -C $@
else
	@echo "Skipping $@ (not configured) "
endif


#osgid is now an autodetect subdir, type "make show_subdirs" to see


# Ingress QoS Control Utility
ifneq ($(strip $(BUILD_IQCTL)),)
iqctl: sanity_check
	$(MAKE) -C $@ $(BUILD_IQCTL)
else
iqctl: sanity_check
	@echo "Skipping iqctl application (Ingress QoS not configured)"
endif

# BPM Control Utility
ifneq ($(strip $(BUILD_BPMCTL)),)
bpmctl: sanity_check
	$(MAKE) -C $@ $(BUILD_BPMCTL)
else
bpmctl: sanity_check
	@echo "Skipping bpmctl application (BPM not configured)"
endif

# ARL Control Utility
ifneq ($(strip $(BUILD_ARLCTL)),)
arlctl:
	$(MAKE) -C $@
else
arlctl:
	@echo "Skipping arlctl application (not configured)"
endif

# EPON Control utility
ifneq ($(strip $(BUILD_EPONCTL)),)
eponctl: 
	$(MAKE) -C $@
else
eponctl:
	@echo "skipping eponctl (not configured)"
endif

# router advertisement monitor
ifneq ($(strip $(BUILD_BRCM_CMS)),)
ifneq ($(strip $(BUILD_IPV6)),)
rastatus6:
	$(MAKE) -C rastatus6
else
rastatus6:
	@echo "skipping rastatus6 (not configured)"
endif
endif

# GMAC Control Utility
ifneq ($(strip $(BUILD_GMACCTL)),)
gmacctl: sanity_check
	$(MAKE) -C $@ $(BUILD_GMACCTL)
else
gmacctl: sanity_check
	@echo "Skipping gmacctl application (GMAC configured)"
endif


# TMS 802.3ah, 802.1ag, Y1731
ifneq ($(strip $(BUILD_BRCM_CMS)),)
ifneq ($(strip $(BUILD_TMS)),)
-include $(KERNEL_DIR)/.config
tmsctl:
	@ rm -f $@/bsp; ln -sn $(BRCMDRIVERS_DIR)/broadcom/char/tms/impl$(CONFIG_BCM_TMS_IMPL)/bsp $@/bsp
	@ rm -f $@/nci; ln -sn $(BRCMDRIVERS_DIR)/broadcom/char/tms/impl$(CONFIG_BCM_TMS_IMPL)/nci $@/nci
	$(MAKE) -C $@
else
tmsctl:
	@echo "skipping tms application (not configured)"
endif
else
	@echo "skipping tms application (CMS not configured)"
endif

# BCMTM
ifneq ($(strip $(BUILD_BCMTM)),)
bcmtm: 
	$(MAKE) -C $@
else
bcmtm:
	@echo "skipping bcmtm (not configured)"
endif

# ARCADYAN
# arc_board to get wps-pin/serialnumber/mac... (jingwei_liang 2014/12)
ifneq ($(strip $(BUILD_ARC_BOARD)),)
arc_board:
	$(MAKE) -C $@
else
arc_board:
	@echo "skipping arc_board (not configured)"
endif
# FW UPGRADE
ifneq ($(strip $(BUILD_FW_UPGRADE)),)
fw_upgrade:
	$(MAKE) PLT=BRCM_63138 -C $@
else
fw_upgrade:
	@echo "skipping fw_upgrade (not configured)"
endif
## ARCADYAN
