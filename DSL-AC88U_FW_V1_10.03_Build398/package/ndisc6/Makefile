#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ndisc6
PKG_VERSION:=1.0.3
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://remlab.net/files/ndisc6

PKG_INSTALL:=1

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
include $(INCLUDE_DIR)/package.mk

define Package/ndisc6/Default
  SECTION:=ipv6
  CATEGORY:=IPv6
  DEPENDS:=+librt 
  TITLE:=IPv6 diagnostic tools
  URL:=http://remlab.net/files/ndisc6
endef

define Package/ndisc6
  $(call Package/ndisc6/Default)
endef

define Package/ndisc6/description
NDisc6 is a small collection of useful tools for IPv6 networking.
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR)
endef


define Package/ndisc6/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/rdisc6 $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,ndisc6))