#
# Copyright (C) 2011 David Tang <david_tang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-wps
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-wps
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary WPS utility
  MAINTAINER:=David Tang <david_tang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm47xx||@TARGET_brcm63xx +nvram +broadcom-libwlcsm +broadcom-libwlbcmcrypto +broadcom-libwlbcmshared +broadcom-libwlcsm +broadcom-libwlupnp +broadcom-libcms +broadcom-brcmssp_util
endef

define Package/broadcom-wps/description
 This package contains the proprietary WPS application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/wps_monitor $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/libwps.so $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-wps/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wps_monitor $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libwps.so    $(1)/lib/
endef

$(eval $(call BuildPackage,broadcom-wps))
