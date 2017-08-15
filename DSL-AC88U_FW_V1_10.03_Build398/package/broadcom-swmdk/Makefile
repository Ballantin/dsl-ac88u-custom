#
# Copyright (C) Arcadyan 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-swmdk
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-swmdk
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary swmdk
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +broadcom-libcms +broadcom-libethswctl +libpthread
endef

define Package/broadcom-swmdk/description
 This package contains the proprietary swmdk application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/swmdk   $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-swmdk/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/swmdk   $(1)/bin/
endef

$(eval $(call BuildPackage,broadcom-swmdk))
