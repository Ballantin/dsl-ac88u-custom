#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-libwlvisualization
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-libwlvisualization
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary wlbcmshared library 
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx
endef

define Package/broadcom-libwlvisualization/description
 This package contains the proprietary wlbcmshared library on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/lib/libwlvisualization.so $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-libwlvisualization/install
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libwlvisualization.so $(1)/lib/
endef

$(eval $(call BuildPackage,broadcom-libwlvisualization))
