#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-libnanoxml
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-libnanoxml
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary nanoxml library 
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx
endef

define Package/broadcom-libnanoxml/description
 This package contains the proprietary nanoxml library on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/lib/private/libnanoxml.so $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-libnanoxml/install
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libnanoxml.so $(1)/lib/
endef

$(eval $(call BuildPackage,broadcom-libnanoxml))
