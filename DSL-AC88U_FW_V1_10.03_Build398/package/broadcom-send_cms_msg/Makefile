#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-send_cms_msg
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-send_cms_msg
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary send_cms_msg
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx +nvram +broadcom-libwlcsm +broadcom-libwlbcmcrypto +broadcom-libwlbcmshared
endef

define Package/broadcom-send_cms_msg/description
 This package contains the proprietary send_cms_msg application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/send_cms_msg/bin/send_cms_msg $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-send_cms_msg/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/send_cms_msg $(1)/bin/
endef

$(eval $(call BuildPackage,broadcom-send_cms_msg))
