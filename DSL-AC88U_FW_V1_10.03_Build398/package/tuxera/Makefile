#
# Copyright (C) 2010 Arcadyan Corporation
# All Rights Reserved.
#
include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
include $(INCLUDE_DIR)/package.mk
include $(TOPDIR)/.config

PKG_NAME:=tuxera
PKG_RELEASE:=1

ifeq ($(CONFIG_ARCADYAN_PROJECT_NAME),"VR9517UAC44-A-ASU")
  PKG_VERSION=3015.1.29.4-dsl-ac88u-bcm63138
else ifeq ($(CONFIG_ARCADYAN_PROJECT_NAME),"VR9517UAC44-B-ASU")
  PKG_VERSION=3015.1.29.4-dsl-ac88u-bcm63138
else ifeq ($(CONFIG_ARCADYAN_PROJECT_NAME),"VRV9510RWAC34-1-A-ASU")
  ifeq ($(CONFIG_BSP_FOLDER_NAME), "broadcom-bsp-4.16L05")
    PKG_VERSION:=3015.1.29.4-dsl-ac87u-bcm6313x
  else
    PKG_VERSION:=3015.1.29.3-dsl-ac87u-bcm6313x
  endif
else ifeq ($(CONFIG_ARCADYAN_PROJECT_NAME),"VRV9510RWAC34-1-B-ASU")
  ifeq ($(CONFIG_BSP_FOLDER_NAME), "broadcom-bsp-4.16L05")
    PKG_VERSION:=3015.1.29.4-dsl-ac87u-bcm6313x
  else
    PKG_VERSION:=3015.1.29.3-dsl-ac87u-bcm6313x
  endif
else ifeq ($(CONFIG_ARCADYAN_PROJECT_NAME),"VRV9510RWAC33-B-ASU")
    PKG_VERSION:=3015.1.29.4-dsl-ac87u-bcm6313x
else ifeq ($(CONFIG_ARCADYAN_PROJECT_NAME),"VRV9519RWAC34-1-A-ZZ")
  PKG_VERSION=3015.1.29-arc-zz-bcm63136
else
  PKG_VERSION=3015.1.29-dsl-ac88u-bcm63138
endif



PKG_SOURCE:=tuxera-file-systems-$(PKG_VERSION).gz

define KernelPackage/$(PKG_NAME)
	SUBMENU:=Kernel Module
	CATEGORY:=Arcadyan
	TITLE:=Tuxera File Systems
	URL:=http://www.tuxera.com/
	FILES:=$(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/exfat/kernel-module/texfat.ko
	FILES:=$(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/fat/kernel-module/tfat.ko
	FILES:=$(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/hfsplus/kernel-module/thfsplus.ko
	FILES:=$(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/ntfs/kernel-module/tntfs.ko
endef

define Package/$(PKG_NAME)-tools
  SECTION:=net
  SUBMENU:=Utilities
  CATEGORY:=Arcadyan
  TITLE:=Tuxera File Systems userspace tools
  URL:=http://www.tuxera.com/
endef

define Package/$(PKG_NAME)-samba
  SECTION:=net
  CATEGORY:=Network
  SUBMENU:=Filesystem
  TITLE:=TUXERA NETWORK STORAGE ACCELARATOR(samba)
  DEPENDS:=+samba3
  URL:=http://www.tuxera.com/
endef

define KernelPackage/$(PKG_NAME)/description
 Tuxera file system kernel drivers.
endef

define Package/$(PKG_NAME)-tools/description
  Tuxera file system userspace tools.
endef

define Package/$(PKG_NAME)-samba/description
	TUXERA NETWORK STORAGE ACCELARATOR(samba)
endef

define KernelPackage/$(PKG_NAME)/config
  source "$(SOURCE)/Config.in"
endef


define Build/Prepare
	echo $(PKG_VERSION)
	echo "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXx"
	mkdir -p $(PKG_BUILD_DIR)
	tar zxf ./tuxera-file-systems-$(PKG_VERSION).gz -C $(PKG_BUILD_DIR)
endef

define Build/Compile
	echo "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@22"
endef

FIXME_LATER_MODULES_SUBDIR := $(MODULES_SUBDIR)-rt19/extra
define KernelPackage/$(PKG_NAME)/install
	echo "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO"
	echo "Done."
	$(INSTALL_DIR) $(1)/$(FIXME_LATER_MODULES_SUBDIR)
	
	#$(INSTALL_DIR) $(1)/sbin
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/exfat/tools/* $(1)/sbin; \
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/fat/tools/* $(1)/sbin; \
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/hfsplus/tools/* $(1)/sbin; \
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/ntfs/tools/* $(1)/sbin; \

	if [ "$(CONFIG_tuxera_EXFAT)" == "y" ]; then \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/exfat/kernel-module/texfat.ko $(1)/$(FIXME_LATER_MODULES_SUBDIR); \
	fi
	if [ "$(CONFIG_tuxera_FAT)" == "y" ]; then \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/fat/kernel-module/tfat.ko $(1)/$(FIXME_LATER_MODULES_SUBDIR); \
	fi
	if [ "$(CONFIG_tuxera_HFSPLUS)" == "y" ]; then \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/hfsplus/kernel-module/thfsplus.ko $(1)/$(FIXME_LATER_MODULES_SUBDIR); \
	fi
	if [ "$(CONFIG_tuxera_NTFS)" == "y" ]; then \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/ntfs/kernel-module/tntfs.ko $(1)/$(FIXME_LATER_MODULES_SUBDIR); \
	fi
endef

define Package/$(PKG_NAME)-tools/install
	$(INSTALL_DIR) $(1)/sbin
	
	#$(INSTALL_DIR) $(1)/sbin
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/exfat/tools/* $(1)/sbin; \
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/fat/tools/* $(1)/sbin; \
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/hfsplus/tools/* $(1)/sbin; \
	#$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/ntfs/tools/* $(1)/sbin; \

	if [ "$(CONFIG_tuxera_EXFAT)" == "y" ]; then \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/exfat/tools/exfatck $(1)/sbin; \
	fi
	if [ "$(CONFIG_tuxera_FAT)" == "y" ]; then \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/fat/tools/fatfsck $(1)/sbin; \
	fi
	if [ "$(CONFIG_tuxera_HFSPLUS)" == "y" ]; then \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/hfsplus/tools/fsck_hfs $(1)/sbin; \
	fi
	if [ "$(CONFIG_tuxera_NTFS)" == "y" ]; then \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/ntfs/tools/ntfsck $(1)/sbin; \
	fi

	# Jared: Install tuxera benchmark utility
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/tools/tuxera.bench $(1)/sbin

endef

define Package/$(PKG_NAME)-samba/install
	if [ -f $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/tnsa/smbd ]; then \
        $(INSTALL_DIR) $(1)/usr/sbin; \
		$(INSTALL_BIN) $(PKG_BUILD_DIR)/tuxera-file-systems-$(PKG_VERSION)/tnsa/smbd $(1)/usr/sbin; \
	fi
endef


$(eval $(call KernelPackage,$(PKG_NAME)))
$(eval $(call BuildPackage,$(PKG_NAME)-tools))
$(eval $(call BuildPackage,$(PKG_NAME)-samba))


