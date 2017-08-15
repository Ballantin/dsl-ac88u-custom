#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=LPRng
PKG_VERSION:=3.8.22
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tgz
PKG_SOURCE_URL:=http://lprng.com/DISTRIB/LPRng/

PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)
#PACKAGE_INSTALL_DIR:=$(PKG_BUILD_DIR)/install

PKG_BUILD_DEPENDS:=opkg/host

include $(INCLUDE_DIR)/package.mk
include $(TOPDIR)/.config

TARGET_CFLAGS+= -I$(PKG_BUILD_DIR) -I$(PKG_BUILD_DIR)/src -I$(PKG_BUILD_DIR)/src/include 

ifeq ($(CONFIG_PACKAGE_LPRng),y)
TARGET_CFLAGS+= -DANOTHER_SPACE_AVAIL
endif
ifeq ($(CONFIG_PACKAGE_LPRng-asus),y)
TARGET_CFLAGS+= -DWINDOW_1 -DTEST_WRITE -DNODEBUG -DLPR_with_ASUS -DJYDEBUG
endif

define Package/$(PKG_NAME)/Default
  SECTION:=net
  CATEGORY:=Network
  TITLE:=LPD/LPR printing
  SUBMENU:=Printing
  URL:=http://lprng.com/DISTRIB/LPRng/
endef

define Package/$(PKG_NAME)
$(call Package/$(PKG_NAME)/Default)
  VARIANT:=nomodified
  TITLE+= (no modified)
endef

define Package/$(PKG_NAME)-asus
$(call Package/$(PKG_NAME)/Default)
  VARIANT:=asus
  TITLE+= (ASUS)
endef


define Package/$(PKG_NAME)/description
	LPRng printing
endef



CONFIGURE_ARGS += \
	--prefix=/usr/local \
	--disable-werror \
	--disable-kerberos \
	--disable-ssl \
	--disable-mit_kerberos4 \
	--with-userid=root \
	--with-groupid=root \
	--disable-setuid \
	--disable-nls \
	--enable-static \
	--disable-strip \
	
ifeq ($(BUILD_VARIANT),nomodified)
define Build/Prepare
    $(call Build/Prepare/Default)
	$(SED) 's/SHELL="@SHELL@"//' $(PKG_BUILD_DIR)/Makefile.in
endef
endif


ifeq ($(BUILD_VARIANT),asus)
define Build/Prepare
    $(call Build/Prepare/Default)
	$(SED) 's/SHELL="@SHELL@"//' $(PKG_BUILD_DIR)/Makefile.in
	$(CP) ./src/common/* $(PKG_BUILD_DIR)/src/common
	$(CP) ./src/include/* $(PKG_BUILD_DIR)/src/include
	$(CP) ./src/Makefile.asus $(PKG_BUILD_DIR)/src/Makefile
endef
endif

ifeq ($(BUILD_VARIANT),nomodified)
define Build/Compile
	$(CP) ./src1/common/* $(PKG_BUILD_DIR)/src/common
	$(call Build/Compile/Default)
endef
endif

ifeq ($(BUILD_VARIANT),asus)
define Build/Compile
	$(CP) ./src/common/* $(PKG_BUILD_DIR)/src/common
	$(CP) ./src/include/* $(PKG_BUILD_DIR)/src/include
	$(CP) ./src/Makefile.asus $(PKG_BUILD_DIR)/src/Makefile
	$(call Build/Compile/Default, \
		USER_OBJS=utilities_arc.lo \
	)
endef
endif


define Package/$(PKG_NAME)/install
	$(INSTALL_DIR) $(1)/etc/lpd
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/lpd.conf $(1)/etc/lpd/lpd.conf
	$(INSTALL_CONF) $(PKG_BUILD_DIR)/lpd.perms $(1)/etc/lpd/lpd.perms
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/src/lpd $(1)/usr/bin/
endef

Package/$(PKG_NAME)-asus/install=$(Package/$(PKG_NAME)/install)

$(eval $(call BuildPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-asus))
