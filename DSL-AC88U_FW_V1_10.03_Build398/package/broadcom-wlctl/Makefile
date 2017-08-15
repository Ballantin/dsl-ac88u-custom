#
# Copyright (C) 2014 Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=broadcom-wlctl
PKG_RELEASE:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)

include $(INCLUDE_DIR)/package.mk

define Package/broadcom-wlctl
  SECTION:=utils
  CATEGORY:=Base system
  TITLE:=Broadcom proprietary wlctl
  MAINTAINER:=Jingwei Liang <jingwei_liang@sva-arcadyan.com.cn>
  DEPENDS:=@TARGET_brcm63xx
endef

define Package/broadcom-wlctl/description
 This package contains the proprietary wlctl application on Broadcom based devices.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Configure
endef

define Build/Compile
	$(CP) $(TOPDIR)/extern/target-install/bin/wlctl $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/bin/wl_server_socket $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/libwlctl.so $(PKG_BUILD_DIR)
	$(CP) $(TOPDIR)/extern/target-install/lib/libwl_server_socket.so $(PKG_BUILD_DIR)

endef

define Build/InstallDev
endef

define Package/broadcom-wlctl/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_DIR) $(1)/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wlctl $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/wl_server_socket $(1)/bin/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libwlctl.so $(1)/lib/
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/libwl_server_socket.so $(1)/lib/
	ln -sf wlctl $(1)/bin/wl
	ln -sf wl_server_socket $(1)/bin/wl_server
endef

$(eval $(call BuildPackage,broadcom-wlctl))
