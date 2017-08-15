#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-fw_upgrade
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-fw_upgrade
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary fw_upgrade
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +broadcom-libcms +broadcom-libethswctl +broadcom-libhomeplugctl +broadcom-libnanoxml +broadcom-libpwrctl +PACKAGE_broadcom-bridgeutil:broadcom-bridgeutil +PACKAGE_broadcom-snoopctl:broadcom-snoopctl
endef

define Package/broadcom-fw_upgrade/description
 This package contains the proprietary fw_upgrade application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/fw_upgrade $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-fw_upgrade/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/fw_upgrade $(1)/bin/
endef

$(eval $(call BuildPackage,broadcom-fw_upgrade))
