#
# Copyright (C) 2006-2011 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=syslog-ng
PKG_VERSION:=3.3.9
PKG_RELEASE:=1

PKG_SOURCE_URL:=http://www.balabit.com/downloads/files/syslog-ng/open-source-edition/$(PKG_VERSION)/source/
PKG_SOURCE:=$(PKG_NAME)_$(PKG_VERSION).tar.gz
#PKG_MD5SUM:=28f0d9ff2243b330e8cd6311ef9b2f12

PKG_FIXUP:=libtool
PKG_INSTALL:=1

PKG_CONFIG_DEPENDS:= \
	CONFIG_IPV6 \

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/nls.mk

define Package/syslog-ng3
  SECTION:=admin
  CATEGORY:=Administration
  DEPENDS:=+libpcre +glib2 +libeventlog +libopenssl +libwrap +libpthread +librt +zlib +libdbi
  TITLE:=A powerful syslog daemon
  URL:=http://www.balabit.com/network-security/syslog-ng/opensource-logging-system/
endef

define Package/syslog-ng3/description
  syslog-ng reads and logs messages to the system console, log
  files, other machines and/or users as specified by its
  configuration file.
endef

define Package/syslog-ng3/conffiles
/etc/syslog-ng.conf
endef

define Build/Configure
	$(SED) 's,-I/usr/include,,' $(PKG_BUILD_DIR)/configure
	$(Build/Configure/Default)
endef

CONFIGURE_ARGS +=  \
	$(call autoconf_bool,CONFIG_IPV6,ipv6) \
	--disable-dependency-tracking \
	--enable-tcp-wrapper \
	--disable-glibtest \
	--disable-spoof-source \
	--disable-sql \
	--enable-prce \
	--disable-linux-caps \
	--without-compile-date \
	--disable-ssl \

CONFIGURE_ARGS += \
	--enable-tls=no \

CONFIGURE_ARGS += \
	--enable-epoll=no \


TARGET_CPPFLAGS += \
	-I$(STAGING_DIR)/usr/include/eventlog

CONFIGURE_VARS += \
	LIBDBI_CFLAGS="-I$(STAGING_DIR)/usr/include"

define Build/Compile
	$(call Build/Compile/Default, \
		all \
	)
	$(MAKE) -C $(PKG_BUILD_DIR) \
		DESTDIR="$(PKG_INSTALL_DIR)" \
		install
endef

define Package/syslog-ng3/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/syslog-ng $(1)/usr/sbin/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libsyslog-ng*.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/syslog-ng
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/syslog-ng/lib*.so* $(1)/usr/lib/syslog-ng/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/syslog-ng.init $(1)/etc/init.d/syslog-ng
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_DATA) ./files/syslog-ng.conf $(1)/etc
endef

$(eval $(call BuildPackage,syslog-ng3))
