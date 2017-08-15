#
# Copyright (C) 2014 Crazy Liang <crazy_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-vlanctl
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-vlanctl
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary vlanctl
  MAINTAINER:=Crazy Liang <crazy_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +broadcom-libcms
endef

define Package/broadcom-vlanctl/description
 This package contains the proprietary vlanctl application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/vlanctl/bin/vlanctl $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/vlanctl/bin/rawSocketTest $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-vlanctl/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/vlanctl $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/rawSocketTest $(1)/bin/
	ln -s vlanctl $(1)/bin/bcm_vlanctl
endef

$(eval $(call BuildPackage,broadcom-vlanctl))
