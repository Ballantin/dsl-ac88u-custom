#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-bridgeutil
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-bridgeutil
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary bridgeutil
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx
endef

define Package/broadcom-bridgeutil/description
 This package contains the proprietary bridgeutil application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libbridgeutil.so $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-bridgeutil/install
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libbridgeutil.so $(1)/lib/
endef

$(eval $(call BuildPackage,broadcom-bridgeutil))
