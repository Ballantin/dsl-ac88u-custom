#
# Copyright (C) 2006-2010 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=email
PKG_VERSION:=3.2.3
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
#PKG_SOURCE_URL:=
#PKG_MD5SUM:=
PKG_BUILD_DEPENDS:=libopenssl

include $(INCLUDE_DIR)/package.mk

define Package/email/Default
  SECTION:=net
  CATEGORY:=Network
endef

define Package/email
$(call Package/email/Default)
  DEPENDS+=+libopenssl
  TITLE:=eMail module
endef

define Package/email/description
  'email' is a program I designed that will send email via the command line
  to remote smtp servers or use 'sendmail' internally.
endef

define Build/Configure
	$(call Build/Configure/Default,)
endef

EXT_SRC=src.VdslIadASUS

define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) $(PKG_BUILD_DIR)/include/utils.h $(PKG_BUILD_DIR)/src/.
	if [ -d "./$(EXT_SRC)" ] ; then \
		$(CP) ./$(EXT_SRC)/* $(PKG_BUILD_DIR)/. ; \
	fi
endef

TARGET_CFLAGS += -I$(STAGING_DIR)/usr/include
TARGET_LDFLAGS += -I$(STAGING_DIR)/usr/lib -lssl -lcrypto

define Package/email/install
	@echo "++++++++++PKG_BUILD_DIR = $(1) ++++++++++"
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/email $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,email,+libopenssl))
