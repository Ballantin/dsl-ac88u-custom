#***********************************************************************
#
#  Copyright (c) 2012  Broadcom Corporation
#  All Rights Reserved
#
#***********************************************************************/

all dynamic install: conditional_build

#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common

UNTAR_COMPLETE_FILE = .untar_complete
CONFIG_ID_FILE = BRCM_CONFIG_ID_$(TOOLCHAIN_PREFIX)_$(LINUX_VER_STR)

APP = ipsec-tools-0.8.0
# ignore BRCM_WERROR_CFLAGS for this APP
APPCFLAGS:=$(filter-out $(BRCM_WERROR_CFLAGS),$(BRCM_COMMON_CFLAGS))

clean:
	rm -f $(INSTALL_DIR)/bin/setkey
	rm -f $(INSTALL_DIR)/bin/racoon
	rm -rf $(APP)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean: clean

check_untar:
ifeq ($(wildcard $(APP)/$(UNTAR_COMPLETE_FILE)),)
	(tar xkf $(APP).tar.bz2 2> /dev/null || true); 
	patch -p1 -b -s -d$(APP) < bcm_patches/$(APP)-$(PROFILE_KERNEL_VER).patch; \
	if [ $$? -ne 0 ]; then exit 1; fi;
	touch $(APP)/$(UNTAR_COMPLETE_FILE)
endif
	@echo "$(APP) is untarred"

check_config: check_untar
ifeq ($(wildcard $(APP)/$(CONFIG_ID_FILE)),)
	(cd $(APP); ./configure --host=$(PROFILE_ARCH)-linux-uclibc \
        --enable-natt --enable-hybrid --enable-dpd \
        --enable-ipv6 --enable-security-context=no \
        --prefix=$(INSTALL_DIR)/usr/local \
        --with-openssl=$(BCM_FSBUILD_DIR)/public \
        --with-kernel-headers=$(TOOLCHAIN_INCLUDE_DIR) \
        LDFLAGS=-L$(INSTALL_DIR)/lib\ -L$(INSTALL_DIR)/lib/public\
        CFLAGS="$(APPCFLAGS)");
	touch $(APP)/$(CONFIG_ID_FILE)
endif

ifneq ($(strip $(BUILD_IPSEC_TOOLS)),)
conditional_build: check_config
	cd $(APP); $(MAKE) -j1
	install -m 755 $(APP)/src/setkey/setkey $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/setkey
	install -m 755 $(APP)/src/racoon/racoon $(INSTALL_DIR)/bin
	$(STRIP) $(INSTALL_DIR)/bin/racoon
else
conditional_build: sanity_check
	@echo "skipping $(APP) (not configured)"
endif
