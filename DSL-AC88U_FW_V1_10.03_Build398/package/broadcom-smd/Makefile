#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-smd
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-smd
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary smd
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +nvram +broadcom-libwlcsm +broadcom-libwlbcmcrypto +broadcom-libwlbcmshared +broadcom-libnanoxml +broadcom-libpwrctl
endef

define Package/broadcom-smd/description
 This package contains the proprietary smd application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/smd/bin/smd $(PKG_BUILD_DIR)
	cp -rf $(TOPDIR)/extern/target-install/smd/etc/cms_entity_info.d $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-smd/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/smd $(1)/bin/
	$(INSTALL_DIR) $(1)/etc
	cp -rf $(PKG_BUILD_DIR)/cms_entity_info.d $(1)/etc/
endef

$(eval $(call BuildPackage,broadcom-smd))
