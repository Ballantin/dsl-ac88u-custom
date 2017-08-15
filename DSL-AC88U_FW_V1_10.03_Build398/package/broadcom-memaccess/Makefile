#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-memaccess
PKG_RELEASE:=1
INSTALL_TARGET:=usr/bcmbin
PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-memaccess
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary memaccess
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx 
endef

define Package/broadcom-memaccess/description
 This package contains the proprietary memaccess application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/memaccess $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-memaccess/install
	$(INSTALL_DIR) $(1)/$(INSTALL_TARGET)
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/memaccess $(1)/$(INSTALL_TARGET)
#Jared: Create symlink in $(1)/bin to avoid conflict with ASUS download master /opt
# If INSTALL_TARGET is modified, DON't forget to sync to /usr/scripts/usb30interference.sh
	ln -sf /$(INSTALL_TARGET)/memaccess $(1)/$(INSTALL_TARGET)/dw
	ln -sf /$(INSTALL_TARGET)/memaccess $(1)/$(INSTALL_TARGET)/dh
	ln -sf /$(INSTALL_TARGET)/memaccess $(1)/$(INSTALL_TARGET)/db
	ln -sf /$(INSTALL_TARGET)/memaccess $(1)/$(INSTALL_TARGET)/sw
	ln -sf /$(INSTALL_TARGET)/memaccess $(1)/$(INSTALL_TARGET)/sh
	ln -sf /$(INSTALL_TARGET)/memaccess $(1)/$(INSTALL_TARGET)/sb
	ln -sf /$(INSTALL_TARGET)/memaccess $(1)/$(INSTALL_TARGET)/fw
	ln -sf /$(INSTALL_TARGET)/memaccess $(1)/$(INSTALL_TARGET)/fh
	ln -sf /$(INSTALL_TARGET)/memaccess $(1)/$(INSTALL_TARGET)/fb
endef

$(eval $(call BuildPackage,broadcom-memaccess))
