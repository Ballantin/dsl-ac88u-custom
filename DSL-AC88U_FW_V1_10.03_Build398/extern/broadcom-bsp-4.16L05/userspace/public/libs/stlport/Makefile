#***********************************************************************
#
#  Copyright (c) 2006-2007  Broadcom Corporation
#  All Rights Reserved
#
#  <:label-BRCM:2006:DUAL/GPL:standard
#  
#  This program is free software; you can redistribute it and/or modify
#  it under the terms of the GNU General Public License, version 2, as published by
#  the Free Software Foundation (the "GPL").
#  
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#  
#  
#  A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
#  writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
#  Boston, MA 02111-1307, USA.
#  
#  :>
#
#***********************************************************************

LIB = libstlport.so.5.2
LIB-SRC = STLport-5.2.1

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

#undefined the CC and CXX from make.common as the stlport makefile is not compatible with that.
unexport CC
unexport CXX
PATH := $(TOOLCHAIN)/bin:$(PATH)

clean: generic_clean
	@if [ -e $(LIB-SRC)/Makefile ]; then \
	cd $(LIB-SRC); $(MAKE) clean; \
	fi
	rm -rf $(LIB-SRC)
	rm -rf lib
	rm -rf include
	rm -f $(INSTALL_DIR)/lib/public/$(LIB)

# The next line is a hint to our release scripts
# GLOBAL_RELEASE_SCRIPT_CALL_DISTCLEAN
distclean:
	rm -f $(LIB)
	rm -rf $(LIB-SRC)
	rm -rf lib
	rm -rf include
	rm -f $(INSTALL_DIR)/lib/public/$(LIB)

ifneq ($(strip $(BUILD_LIB_STLPORT)),)
conditional_build: generic_public_lib_install
else
conditional_build: sanity_check
	@echo "skipping $(LIB) (not configured)"
endif

check_untar_patch_configure: sanity_check
	@if [ ! -e $(LIB-SRC)/README ]; then \
	echo "Untarring original $(LIB-SRC) source"; \
	(tar xkf $(LIB-SRC).tar.bz2 2> /dev/null || true); \
	echo "Applying patches to $(LIB-SRC)"; \
	patch -p1 -b -d$(LIB-SRC) < bcm_patches/$(LIB-SRC).patch; \
	(cd $(LIB-SRC); ./configure --target=$(TOOLCHAIN_PREFIX) --prefix=.); \
	fi

check_lib_path:
	@if [ ! -e ./lib ]; then \
	ln -fs ./$(LIB-SRC)/build/lib/$(TOOLCHAIN_PREFIX)-lib ./lib; \
	fi

check_include_path:
	@if [ ! -e ./include ]; then \
	ln -fs ./$(LIB-SRC)/build/lib/include/stlport ./include; \
	fi

$(LIB): check_untar_patch_configure check_lib_path check_include_path
	cd $(LIB-SRC); $(MAKE) install-release-shared; 
	cp ./$(LIB-SRC)/build/lib/$(TOOLCHAIN_PREFIX)-lib/$(LIB) .
