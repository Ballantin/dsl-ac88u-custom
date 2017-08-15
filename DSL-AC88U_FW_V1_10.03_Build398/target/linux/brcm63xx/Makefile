
# Copyright (C) 2006-2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
include $(TOPDIR)/rules.mk

#chriskuo+note: for DUMP=1, rules.mk does not include .config
ARCH:=arm
BOARD:=brcm63xx
BOARDNAME:=Broadcom BCM63xx
#chriskuo+e
FEATURES:=squashfs jffs2 usb atm pci pcmcia
LINUX_VERSION:=3.4.11-rt19


include $(INCLUDE_DIR)/target.mk

DEFAULT_PACKAGES += kmod-switch gpioctl

define Target/Description
	Build firmware images for Broadcom based xDSL/routers
	currently supports BCM6338, BCM6348 and BCM6358 based devices.
	(e.g. Inventel Livebox, Siemens SE515, Neufbox 4)
endef

define Kernel/Configure
	ln -sf $(LINUX_DIR)/include/generated/autoconf.h $(LINUX_DIR)/include/linux/autoconf.h
endef

define Kernel/CompileModules
	@echo "==============Kernel CompileModules============"
	@echo "TOPDIR=[$(TOPDIR)]"
endef

define Kernel/CompileImage
	rm -f $(TARGET_DIR)/init
	$(call Kernel/CopyImage)
endef

$(eval $(call BuildTarget))
