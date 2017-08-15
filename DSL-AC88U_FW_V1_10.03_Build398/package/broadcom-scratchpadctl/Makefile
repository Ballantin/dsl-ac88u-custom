#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-scratchpadctl
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-scratchpadctl
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary scratchpadctl
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx 
endef

define Package/broadcom-scratchpadctl/description
 This package contains the proprietary scratchpadctl application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/scratchpadctl $(PKG_BUILD_DIR)
endef

define Build/InstallDev
endef

define Package/broadcom-scratchpadctl/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/scratchpadctl $(1)/bin/
	ln -sf /bin/scratchpadctl $(1)/bin/pspctl
endef

$(eval $(call BuildPackage,broadcom-scratchpadctl))
