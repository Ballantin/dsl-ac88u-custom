#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-dspgctl
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-dspgctl
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary dspgctl
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +libpthread
endef

define Package/broadcom-dspgctl/description
 This package contains the proprietary dspgctl application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/dspgctl $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-dspgctl/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/dspgctl $(1)/bin/
endef

$(eval $(call BuildPackage,broadcom-dspgctl))
