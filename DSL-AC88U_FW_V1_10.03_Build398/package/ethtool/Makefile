#
# Copyright (C) 2009 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=ethtool
PKG_VERSION:=3.10
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=ftp://ftp.gr.freebsd.org/pub/linux/kernel/software/network/ethtool/ethtool-3.10.tar.bz2
PKG_MD5SUM:=ea67920df719d2d12046840af61d293c

include $(INCLUDE_DIR)/package.mk

define Package/ethtool
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+ethtool
  TITLE:=Connection tracking userspace tools
  URL:=http://ethtool.sourceforge.org/
endef

define Package/ethtool/description
 The ethtool s a GNU/Linux net driver diagnostic and tuning tool 
 for the Linux 2.4.x (or later) series of kernels. It obtains 
 information and diagnostics related to media, link status, driver
 version, PCI (or other) bus location, and more.
endef


# uses GNU configure

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		all install
endef

define Package/ethtool/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/ethtool $(1)/bin/
endef

$(eval $(call BuildPackage,ethtool))
