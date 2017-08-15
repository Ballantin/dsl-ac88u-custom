#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=traceroute
PKG_VERSION:=2.0.21
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=http://sourceforge.net/projects/traceroute/files/traceroute/traceroute-2.0.21/
#PKG_MD5SUM:=14c5c81debb324adc261db971730f4c6

include $(INCLUDE_DIR)/package.mk

define Package/traceroute
  SECTION:=net
  CATEGORY:=Base system
  TITLE:=A lightweight traceroute
  URL:=http://sourceforge.net/projects/traceroute/files/traceroute/
endef

define Package/traceroute/description
  Supports both IPv4 and IPv6, additional types of trace (including TCP).
endef

TARGET_LDFLAGS += -L../libsupp/

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		CFLAGS="$(TARGET_CFLAGS)" \
		BINDIR="/usr/sbin" \
		MANDIR="/usr/man" \
		AWK="awk" \
		LDFLAGS="$(TARGET_LDFLAGS) -Wl,--gc-sections" \
		all
endef

define Package/traceroute/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/traceroute/traceroute $(1)/usr/sbin/
#	$(INSTALL_DIR) $(1)/etc/config
#	$(INSTALL_DATA) ./files/dhcp.conf $(1)/etc/config/dhcp
#	$(INSTALL_DATA) ./files/dnsmasq.conf $(1)/etc/dnsmasq.conf
#	$(INSTALL_DIR) $(1)/etc/init.d
#	$(INSTALL_BIN) ./files/dnsmasq.init $(1)/etc/init.d/dnsmasq
endef

$(eval $(call BuildPackage,traceroute))
