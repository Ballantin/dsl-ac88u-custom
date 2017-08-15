# 
# Copyright (C) 2007 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# $Id$

include $(TOPDIR)/rules.mk

PKG_NAME:=nghttp2
PKG_VERSION:=2.16.x
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=@SF/nghttp2
PKG_MD5SUM:=c56f41ec195bc1fe016369bf74efc5a1

include $(INCLUDE_DIR)/package.mk

define Package/nghttp2
  SECTION:=net
  CATEGORY:=Network
  TITLE:=nghttp2
  DEPENDS:=
  URL:=https://nghttp2.org/
endef

define Package/nghttp2/description
	http/2
endef

define Build/Prepare
	$(call Build/Prepare/Default)
endef

TARGET_CFLAGS +=

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS) -std=gnu99"
endef

define Package/nghttp2/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_BUILD_DIR)/lib/.libs/libnghttp2.so* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,nghttp2))
