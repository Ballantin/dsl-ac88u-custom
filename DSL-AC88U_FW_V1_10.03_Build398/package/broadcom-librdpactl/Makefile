#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-librdpactl
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-librdpactl
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary rdpactl library 
  MAINTAINER:=Andy Hsu <andy_hsu@arcadyan.com>
  DEPENDS:=@TARGET_brcm63xx
endef

define Package/broadcom-librdpactl/description
 This package contains the proprietary rdpactl library on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/lib/private/librdpactl.so $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-librdpactl/install
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/librdpactl.so $(1)/lib/
endef

$(eval $(call BuildPackage,broadcom-librdpactl))
