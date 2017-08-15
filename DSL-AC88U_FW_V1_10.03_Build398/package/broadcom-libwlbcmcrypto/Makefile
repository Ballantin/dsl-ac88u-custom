#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-libwlbcmcrypto
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-libwlbcmcrypto
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary bcmcrypto library 
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx
endef

define Package/broadcom-libwlbcmcrypto/description
 This package contains the proprietary bcmcrypto library on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/lib/libwlbcmcrypto.so  $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-libwlbcmcrypto/install
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libwlbcmcrypto.so $(1)/lib/
endef

$(eval $(call BuildPackage,broadcom-libwlbcmcrypto))
