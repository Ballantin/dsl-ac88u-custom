#
# Copyright (C) 2014 David Tang <david_tang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-lltd
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-lltd
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary lltd utility
  MAINTAINER:=David Tang <david_tang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +nvram +broadcom-libwlcsm +broadcom-libwlbcmcrypto +broadcom-libwlbcmshared
endef

define Package/broadcom-lltd/description
 This package contains the proprietary lltd application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/lld2d $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/etc/lld2d.conf $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-lltd/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lld2d $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/lld2d.conf $(1)/etc/
endef

$(eval $(call BuildPackage,broadcom-lltd))
