#
# Copyright (C) 2006-2013 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(TOPDIR)/.config

PKG_NAME:=vsftpd
PKG_VERSION:=3.0.2
PKG_RELEASE:=2

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://security.appspot.com/downloads/
PKG_MD5SUM:=8b00c749719089401315bd3c44dddbb2
PKG_BUILD_DEPENDS:=libarcgpl asus-apps libdisk


PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(BUILD_VARIANT)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/$(PKG_NAME)/Default
  SUBMENU:=File Transfer
  SECTION:=net
  CATEGORY:=Network
  TITLE:=A fast and secure FTP server
  URL:=http://vsftpd.beasts.org/
  MAINTAINER:=Cezary Jackiewicz <cezary@eko.one.pl>
endef


define Package/$(PKG_NAME)
$(call Package/$(PKG_NAME)/Default)
  VARIANT:=notls
  TITLE+= (no TLS)
endef

define Package/$(PKG_NAME)-tls
$(call Package/$(PKG_NAME)/Default)
  VARIANT:=tls
  TITLE+= (TLS)
  DEPENDS+=+libopenssl
endef

define Package/$(PKG_NAME)-asus
$(call Package/$(PKG_NAME)/Default)
  VARIANT:=asus
  TITLE+= (ASUS without SSL support)
  DEPENDS:=+libarcgpl +asus-apps +libdisk
endef
#  DEPENDS:=+libarcgpl +libdisk +lib-arc-cfg

#define Package/$(PKG_NAME)/conffiles
#/etc/vsftpd.conf
#endef

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include
TARGET_LDFLAGS += -L$(TOPDIR)/extern/arc-release/usr/lib -L$(STAGING_DIR)/usr/lib -larcgpl -lasusnvram -ldisk


# To support single file larger than 2G in size
TARGET_CFLAGS += -D_FILE_OFFSET_BITS=64

ifeq ($(BUILD_VARIANT),notls)
 define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		vsftpd
 endef
endif

ifeq ($(BUILD_VARIANT),tls)
 define Build/Compile
	$(SED) 's/#undef VSF_BUILD_SSL/#define VSF_BUILD_SSL/' $(PKG_BUILD_DIR)/builddefs.h
	$(SED) 's/-lcrypt -lnsl/-lcrypt -lnsl -lssl -lcrypto/' $(PKG_BUILD_DIR)/Makefile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		vsftpd
 endef
endif

ifeq ($(BUILD_VARIANT),asus)
 define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		CC="$(TARGET_CC)" \
		CFLAGS="$(TARGET_CFLAGS)" \
		LDFLAGS="$(TARGET_LDFLAGS)" \
		vsftpd
 endef
endif

ifeq ($(BUILD_VARIANT),asus)
 define Build/Prepare
    $(call Build/Prepare/Default)
	$(CP) ./files/asus/vsftpd-3.0.2/* $(PKG_BUILD_DIR)
 endef
endif


define Package/$(PKG_NAME)/install
	#2b) vsftpd needs the (empty) directory /usr/share/empty in the default
	$(INSTALL_DIR) $(1)/usr/share/empty
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/$(PKG_NAME) $(1)/usr/sbin/
	#$(INSTALL_DIR) $(1)/etc
	#$(INSTALL_CONF) ./files/$(PKG_NAME).conf $(1)/etc/$(PKG_NAME).conf
	#$(INSTALL_DIR) $(1)/etc/init.d
	#$(INSTALL_BIN) ./files/$(PKG_NAME).init $(1)/etc/init.d/$(PKG_NAME)
endef

Package/$(PKG_NAME)-tls/install=$(Package/$(PKG_NAME)/install)
Package/$(PKG_NAME)-asus/install=$(Package/$(PKG_NAME)/install)

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-tls))
$(eval $(call BuildPackage,$(PKG_NAME)-asus))
