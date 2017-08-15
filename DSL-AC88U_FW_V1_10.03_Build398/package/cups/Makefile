#
# Copyright (C) 2006-2012 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=cups
PKG_VERSION:=1.5.4
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-source.tar.bz2
PKG_SOURCE_URL:= http://www.cups.org/software/$(PKG_VERSION)
PKG_MD5SUM:=de3006e5cf1ee78a9c6145ce62c4e982

TARGET_LDFLAGS=-Wl,-rpath-link=$(STAGING_DIR)/usr/lib

include $(INCLUDE_DIR)/package.mk

define Package/cups/Default
  URL:=http://www.cups.org/
  SUBMENU:=Printing
endef

define Package/cups
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  #DEPENDS:=+libcups +libcupsmime +libcupscgi +libcupsppdc +libusb-1.0
  DEPENDS:=+zlib +libpthread +libstdcpp
  TITLE:=Common UNIX Printing System (daemon)
endef

define Package/cups/description
	Common UNIX Printing System (daemon)
endef

define Package/cups/conffiles
/etc/cups/classes.conf
/etc/cups/cupsd.conf
/etc/cups/printers.conf
endef

define Package/cups-bsd
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libcups
  TITLE:=Common UNIX Printing System - BSD commands (old)
endef

define Package/cups-bsd/description
	Common UNIX Printing System - BSD commands (old)
endef

define Package/cups-client
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libcups +libcupsimage +libcupsmime
  TITLE:=Common UNIX Printing System - Client commands
endef

define Package/cups-client/conffiles
/etc/cups/client.conf
endef

define Package/cups-client/description
	Common UNIX Printing System - Client commands
endef

define Package/cups-filters
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  DEPENDS:=+libcupsimage +libcupsdriver
  TITLE:=Common UNIX Printing System - Filter
endef

define Package/cups-filters/description
	Common UNIX Printing System - Filter
endef





define Package/libcups
$(call Package/cups/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+zlib +libpthread +libpng +libjpeg
  TITLE:=Common UNIX Printing System - Core library
endef

define Package/libcups/description
	Common UNIX Printing System - Core library
endef

define Package/libcupscgi
$(call Package/cups/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libcups
  TITLE:=Common UNIX Printing System - CGI library
endef

define Package/libcupscgi/description
	Common UNIX Printing System - CGI library
endef

define Package/libcupsdriver
$(call Package/cups/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libcups
  TITLE:=Common UNIX Printing System - Driver library
endef

define Package/libcupsdriver/description
	Common UNIX Printing System - Driver library
endef

define Package/libcupsimage
$(call Package/cups/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libcups +libpng +libjpeg
  TITLE:=Common UNIX Printing System - Image library
endef

define Package/libcupsimage/description
	Common UNIX Printing System - Image library
endef

define Package/libcupsmime
$(call Package/cups/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libcups
  TITLE:=Common UNIX Printing System - MIME library
endef

define Package/libcupsmime/description
	Common UNIX Printing System - MIME library
endef

define Package/libcupsppdc
$(call Package/cups/Default)
  SECTION:=libs
  CATEGORY:=Libraries
  DEPENDS:=+libcups +libstdcpp
  TITLE:=Common UNIX Printing System - PPDC library
endef

define Package/libcupsppdc/description
	Common UNIX Printing System - PPDC library
endef

define Package/cups-locale-de
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale de
endef

define Package/cups-locale-de/description
	Common UNIX Printing System - Locale de
endef

define Package/cups-locale-nl
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale nl
endef

define Package/cups-locale-nl/description
	Common UNIX Printing System - Locale nl
endef

define Package/cups-locale-hu
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale hu
endef

define Package/cups-locale-hu/description
	Common UNIX Printing System - Locale hu
endef

define Package/cups-locale-pt
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale pt
endef

define Package/cups-locale-pt/description
	Common UNIX Printing System - Locale pt
endef

define Package/cups-locale-pt_BR
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale pt_BR
endef

define Package/cups-locale-pt_BR/description
	Common UNIX Printing System - Locale pt_BR
endef

define Package/cups-locale-fi
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale fi
endef

define Package/cups-locale-fi/description
	Common UNIX Printing System - Locale fi
endef

define Package/cups-locale-sv
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale sv
endef

define Package/cups-locale-sv/description
	Common UNIX Printing System - Locale sv
endef

define Package/cups-locale-da
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale da
endef

define Package/cups-locale-da/description
	Common UNIX Printing System - Locale da
endef

define Package/cups-locale-ko
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale ko
endef

define Package/cups-locale-ko/description
	Common UNIX Printing System - Locale ko
endef

define Package/cups-locale-zh
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale zh
endef

define Package/cups-locale-zh/description
	Common UNIX Printing System - Locale zh
endef

define Package/cups-locale-ja
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale ja
endef

define Package/cups-locale-ja/description
	Common UNIX Printing System - Locale ja
endef

define Package/cups-locale-es
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale es
endef

define Package/cups-locale-es/description
	Common UNIX Printing System - Locale es
endef

define Package/cups-locale-zh_TW
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale zh_TW
endef

define Package/cups-locale-zh_TW/description
	Common UNIX Printing System - Locale zh_TW
endef

define Package/cups-locale-pl
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale pl
endef

define Package/cups-locale-pl/description
	Common UNIX Printing System - Locale pl
endef

define Package/cups-locale-no
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale no
endef

define Package/cups-locale-no/description
	Common UNIX Printing System - Locale no
endef

define Package/cups-locale-ru
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale ru
endef

define Package/cups-locale-ru/description
	Common UNIX Printing System - Locale ru
endef

define Package/cups-locale-eu
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale eu
endef

define Package/cups-locale-eu/description
	Common UNIX Printing System - Locale eu
endef

define Package/cups-locale-fr
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale fr
endef

define Package/cups-locale-fr/description
	Common UNIX Printing System - Locale fr
endef

define Package/cups-locale-it
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale it
endef

define Package/cups-locale-it/description
	Common UNIX Printing System - Locale it
endef

define Package/cups-locale-id
$(call Package/cups/Default)
  SECTION:=net
  CATEGORY:=Network
  TITLE:=Common UNIX Printing System - Locale id
endef

define Package/cups-locale-id/description
	Common UNIX Printing System - Locale id
endef

define Build/Configure
	$(call Build/Configure/Default, \
		--libdir=$(STAGING_DIR)/usr/lib \
		--with-printcap="/tmp/printcap" \
		--with-cups-user="nobody" \
		--with-cups-group="nogroup" \
		--with-components="embedded" \
		--with-pdftops="none" \
		--without-perl \
		--without-python \
		--without-php \
		--without-arch64flags \
		--enable-shared \
		--disable-image \
		--disable-dbus \
		--disable-dnssd \
		--disable-launchd \
		--disable-ldap \
		--disable-pam \
		--disable-slp \
		--disable-gnutls \
		--disable-openssl \
		--disable-cdsassl \
		--disable-ssl \
		--disable-gssapi \
		--disable-64bit \
		--disable-option-checking \
		--disable-tiff, \
		UNAME="Linux" \
		LIBS="$(TARGET_LDFLAGS)" \
	)
endef

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) \
		$(TARGET_CONFIGURE_OPTS) \
		DSTROOT="$(PKG_INSTALL_DIR)" \
		STRIP="/bin/true" \
		all install
endef

define Package/cups/install
	rm -rf $(1)/etc/cups
	#Simon@2014/12/25, Prepare cupsd.conf based on lan ip settings before udev
	@echo "Do some dirty hacking..."
	$(CP) ./files/etc/cups/cupsd*.conf  $(PKG_INSTALL_DIR)/etc/cups/
	$(INSTALL_DIR) $(1)/etc/cups
	$(CP) $(PKG_INSTALL_DIR)/etc/cups/* $(1)/etc/cups/
	rm -rf $(1)/etc/cups/certs
	ln -sf /tmp $(1)/etc/cups/certs
	
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/* $(1)/usr/bin/
	rm -f $(1)/usr/bin/cups-config
	
	$(INSTALL_DIR) $(1)/usr/lib
	### ctc ###
	$(CP) $(PKG_BUILD_DIR)/cups/libcups*.so*       $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/filter/libcups*.so*     $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/scheduler/libcups*.so*  $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/ppdc/libcups*.so*    $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/cgi-bin/libcups*.so*  $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/driver/libcups*.so*  $(1)/usr/lib/
	###########
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/cups $(1)/usr/lib/

#	$(INSTALL_DIR) $(1)/usr/share/cups/templates
#	$(CP) $(PKG_INSTALL_DIR)/usr/share/cups/templates/*.tmpl \
#		$(1)/usr/share/cups/templates/
	$(INSTALL_DIR) $(1)/usr/share/cups/mime
	$(CP) $(PKG_INSTALL_DIR)/usr/share/cups/mime/* $(1)/usr/share/cups/mime/
#	$(INSTALL_DIR) $(1)/usr/share/doc/cups
#	$(CP) $(PKG_INSTALL_DIR)/usr/share/doc/cups/index.html \
#		$(1)/usr/share/doc/cups/
#	$(CP) $(PKG_INSTALL_DIR)/usr/share/doc/cups/*.css \
#		$(1)/usr/share/doc/cups/
#	$(CP) $(PKG_INSTALL_DIR)/usr/share/doc/cups/images \
#		$(1)/usr/share/doc/cups/
	
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) \
		$(PKG_INSTALL_DIR)/usr/sbin/{cupsaccept,cupsaddsmb,cupsctl,cupsd,cupsfilter,lpadmin,lpc,lpinfo,lpmove} \
		$(1)/usr/sbin/
	(cd $(1)/usr/sbin; ln -sf cupsaccept cupsreject; ln -sf cupsaccept cupsenable; ln -sf cupsaccept cupsdisable;)
	
	# overwrite default config with our own
	$(CP) ./files/etc/cups/* $(1)/etc/cups/
	
	# install initscript with priority 60
	#$(INSTALL_DIR) $(1)/etc/init.d
	#$(INSTALL_BIN) ./files/cupsd.init $(1)/etc/init.d/cupsd

    # 06-Nov-2012, Leo
	$(CP) $(1)/usr/lib/cups/filter/* $(1)/usr/bin


endef

define Build/InstallDev
	$(INSTALL_DIR) $(2)/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/cups-config $(2)/bin/
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/cups $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	### ctc ###

	$(CP) $(PKG_BUILD_DIR)/cups/libcups*.so*    $(1)/usr/lib/
	$(CP) $(PKG_BUILD_DIR)/filter/libcups*.so*  $(1)/usr/lib/
	###########
endef

$(eval $(call BuildPackage,cups))
$(eval $(call BuildPackage,libcups))
$(eval $(call BuildPackage,libcupscgi))
#$(eval $(call BuildPackage,libcupsdriver))
#$(eval $(call BuildPackage,libcupsimage))
#$(eval $(call BuildPackage,libcupsmime))
#$(eval $(call BuildPackage,libcupsppdc))
#$(eval $(call BuildPackage,cups-bsd))
#$(eval $(call BuildPackage,cups-client))
#$(eval $(call BuildPackage,cups-filters))
#$(eval $(call BuildPackage,cups-locale-de))
$(eval $(call BuildPackage,cups-locale-nl))
$(eval $(call BuildPackage,cups-locale-hu))
$(eval $(call BuildPackage,cups-locale-pt))
$(eval $(call BuildPackage,cups-locale-pt_BR))
$(eval $(call BuildPackage,cups-locale-fi))
$(eval $(call BuildPackage,cups-locale-sv))
$(eval $(call BuildPackage,cups-locale-da))
$(eval $(call BuildPackage,cups-locale-ko))
$(eval $(call BuildPackage,cups-locale-zh))
$(eval $(call BuildPackage,cups-locale-ja))
$(eval $(call BuildPackage,cups-locale-es))
$(eval $(call BuildPackage,cups-locale-zh_TW))
$(eval $(call BuildPackage,cups-locale-pl))
$(eval $(call BuildPackage,cups-locale-no))
$(eval $(call BuildPackage,cups-locale-ru))
$(eval $(call BuildPackage,cups-locale-eu))
$(eval $(call BuildPackage,cups-locale-fr))
$(eval $(call BuildPackage,cups-locale-it))
$(eval $(call BuildPackage,cups-locale-id))
