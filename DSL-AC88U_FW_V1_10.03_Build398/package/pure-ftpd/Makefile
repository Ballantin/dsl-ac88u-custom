#
# Copyright (C) 2007 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk
include $(TOPDIR)/.config

PKG_NAME:=pure-ftpd
PKG_VERSION:=1.0.31
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.bz2
PKG_SOURCE_URL:=http://download.pureftpd.org/pub/pure-ftpd/releases
PKG_MD5SUM:=fba0d592a6316f45b5c312a78789ad97
PKG_BUILD_DEPENDS:= libiconv

include $(INCLUDE_DIR)/package.mk

define Package/pure-ftpd/Default
  SUBMENU:=FTP
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libelf
  TITLE:=Secure FTP made easy!
  URL:=http://www.pureftpd.org/project/pure-ftpd
endef

define Package/pure-ftpd
  $(call Package/pure-ftpd/Default)
  TITLE+= (no TLS)
  VARIANT:=notls
endef

define Package/pure-ftpd-tls
  $(call Package/pure-ftpd/Default)
  TITLE+= (TLS)
  VARIANT:=tls
  DEPENDS+=+libopenssl
endef


define Package/pure-ftpd/description
 Pure-FTPd is a free (BSD), secure, production-quality and standard-conformant
 FTP server. It doesn't provide useless bells and whistles, but focuses on
 efficiency and ease of use.
 It provides simple answers to common needs, plus unique  useful features for
 personal users as well as hosting providers.
endef

Package/pure-ftpd-tls/description=$(Package/pure-ftpd/description)

define Package/pure-ftpd/conffiles
/etc/config/pure-ftpd
endef

Package/pure-ftpd-tls/conffiles=$(Package/pure-ftpd/conffiles)

define Package/pure-ftpd/config
config PACKAGE_pure-ftpd-ANON_CAN_DELETE
   depends PACKAGE_pure-ftpd || PACKAGE_pure-ftpd-tls
   bool "Allow anonymous has delete permission"
   default y

config PACKAGE_pure-ftpd-ANON_CAN_RESUME
   depends PACKAGE_pure-ftpd || PACKAGE_pure-ftpd-tls
   bool "Allow anonymous has resume permission"
   default y

config PACKAGE_pure-ftpd-ANON_CAN_RENAME
   depends PACKAGE_pure-ftpd || PACKAGE_pure-ftpd-tls
   bool "Allow anonymous has resume permission"
   default y


config PACKAGE_pure-ftpd-with-ARC_ISO_SYMLINK
   depends PACKAGE_pure-ftpd || PACKAGE_pure-ftpd-tls
   bool "ARCADYAN isolate symblic out of usb storage"
   default y
   	
		menu "mount point"
			depends PACKAGE_pure-ftpd-with-ARC_ISO_SYMLINK || PACKAGE_pure-ftpd-tls

			config PACKAGE_pure-ftpd-usb-mountpoint-tmp-media
			bool "/tmp/mnt"
			default y
			
			config PACKAGE_pure-ftpd-usb-mountpoint-tmp-usbMountNameReversal
			bool "/tmp/usbMountNameReversal"
			default n

		endmenu
endef


define Build/Prepare
	$(call Build/Prepare/Default)
	$(CP) ./src/* $(PKG_BUILD_DIR)/src/.
endef

# turn on debug mode
#TARGET_CFLAGS+=-DARC_DEBUG

# pure-pw accept -P option
TARGET_CFLAGS+=-DARC_PW_CLI

# pure-pw accept -A option
TARGET_CFLAGS+=-DARC_PW_PRIVILEGE

# turn on open access mode
#TARGET_CFLAGS+=-DARC_OPEN_ACCESS_MODE

# turn on ram read only
TARGET_CFLAGS+=-DARC_RAM_READ_ONLY

# turn on IPv6 address checking.
TARGET_CFLAGS+=-DARC_IPV6_SUBNET

# turn on READ only capability
#TARGET_CFLAGS+=-DARC_ACCOUNT_READ_ONLY_SUPPORT

# Jared added:
ifeq ($(CONFIG_PACKAGE_pure-ftpd-with-ARC_ISO_SYMLINK),y)
	TARGET_CFLAGS+=-DDENY_SYMLINK_TARGET_OUT_OF_USB_MOUNT_POINT
endif

ifeq ($(CONFIG_PACKAGE_pure-ftpd-usb-mountpoint-tmp-media),y)
	TARGET_CFLAGS += -DUSB_MOUNT_POINT_TMP_MDEIA
endif
ifeq ($(CONFIG_PACKAGE_pure-ftpd-ANON_CAN_DELETE),y)
	TARGET_CFLAGS += -DANON_CAN_DELETE
endif
ifeq ($(CONFIG_PACKAGE_pure-ftpd-ANON_CAN_RESUME),y)
	TARGET_CFLAGS += -DANON_CAN_RESUME
endif
ifeq ($(CONFIG_PACKAGE_pure-ftpd-ANON_CAN_RENAME),y)
	TARGET_CFLAGS += -DANON_CAN_RENAME
endif

CONFIGURE_ARGS += \
	--with-everything \
	--with-virtualchroot \
	--with-rfc2640 \
	--without-ascii

ifeq ($(BUILD_VARIANT),tls)
CONFIGURE_ARGS += \
	--with-certfile=/usr/local/etc/ssl/private/pure-ftpd.pem \
	--with-tls
endif

define Package/pure-ftpd/install
	#$(INSTALL_DIR) $(1)/etc/pure-ftpd $(1)/etc/init.d $(1)/usr/sbin
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(foreach i,ftpd pw ftpwho, $(PKG_BUILD_DIR)/src/pure-$(i)) $(1)/usr/sbin
	
endef

define Package/pure-ftpd-tls/install
	$(Package/pure-ftpd/install)
	$(INSTALL_DIR) $(1)/usr/local/etc/ssl/private
	$(INSTALL_CONF) ./files/$(PKG_NAME).pem $(1)/usr/local/etc/ssl/private/$(PKG_NAME).pem
endef

$(eval $(call BuildPackage,pure-ftpd))
$(eval $(call BuildPackage,pure-ftpd-tls))