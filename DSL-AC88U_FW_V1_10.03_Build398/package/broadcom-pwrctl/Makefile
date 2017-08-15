#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-pwrctl
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-pwrctl
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary pwrctl
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +broadcom-libpwrctl broadcom-libcms +broadcom-libethswctl
endef

define Package/broadcom-pwrctl/description
 This package contains the proprietary pwrctl application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/pwrctl $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-pwrctl/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/pwrctl $(1)/bin/
	ln -sf pwrctl  $(1)/bin/pwr
endef

$(eval $(call BuildPackage,broadcom-pwrctl))
