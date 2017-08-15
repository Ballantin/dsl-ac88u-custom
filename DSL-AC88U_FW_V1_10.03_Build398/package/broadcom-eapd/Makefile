#
# Copyright (C) 2011 David Tang <david_tang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-eapd
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-eapd
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary wireless EAP utility
  MAINTAINER:=David Tang <david_tang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm47xx||@TARGET_brcm63xx +nvram +broadcom-libwlcsm +broadcom-libwlbcmcrypto +broadcom-libwlbcmshared
endef

define Package/broadcom-eapd/description
 This package contains the proprietary wireless EAP application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/eapd $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-eapd/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/eapd $(1)/bin/
endef

$(eval $(call BuildPackage,broadcom-eapd))
