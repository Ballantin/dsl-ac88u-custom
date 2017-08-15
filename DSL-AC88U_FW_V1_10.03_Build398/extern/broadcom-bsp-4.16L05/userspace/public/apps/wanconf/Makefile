
#***********************************************************************
#
#  <:copyright-BRCM:2015:DUAL/GPL:standard
#  
#     Copyright (c) 2015 Broadcom 
#     All Rights Reserved
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
#***********************************************************************/


EXE = wanconf
OBJS = wanconf.o

LIBS = -lcms_util -lcms_msg -lcms_boardctl -lbcm_crc -lbcm_flashutil
ifneq ($(strip $(BUILD_GPONCTL)),)
LIBS += -lgponctl
endif

all dynamic install: conditional_build


include $(BUILD_DIR)/make.common

clean: generic_clean
	rm -f $(INSTALL_DIR)/bin/$(EXE)
	rm -f $(INSTALL_DIR)/etc/init.d/wanconf.sh
	rm -f $(INSTALL_DIR)/etc/rc3.d/S85wanconf
	rm -f $(EXE)
	rm -f $(OBJS)
	rm -f *.d

ifneq ($(strip $(WAN_AUTODETECT)),)	
conditional_build: $(EXE) generic_exe_install
	mkdir -p $(INSTALL_DIR)/etc/init.d
	mkdir -p $(INSTALL_DIR)/etc/rc3.d
	install scripts/wanconf.sh $(INSTALL_DIR)/etc/init.d
	(cd $(INSTALL_DIR)/etc/rc3.d; rm -f S85wanconf; ln -s ../init.d/wanconf.sh S85wanconf)


#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))



#
# Private apps and libs are allowed to include header files from the
# private and public directories.
#
# WARNING: Do not modify this section unless you understand the
# license implications of what you are doing.

ALLOWED_INCLUDE_PATHS := -I.\
                         -I$(BUILD_DIR)/userspace/public/include  \
                         -I$(BUILD_DIR)/userspace/public/include/$(OALDIR) \
                         -I$(BUILD_DIR)/userspace/private/include  \
                         -I$(BUILD_DIR)/userspace/private/include/$(OALDIR) \
                         -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)  \
                         -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD) \
                         -I$(INC_RDPA_PATH) \
                         -I$(INC_BDMF_PATH)/framework \
                         -I$(BUILD_DIR)/bcmdrivers/opensource/char/opticaldet/impl1

#
# Private apps and libs are allowed to link with libraries from the
# private and public directories.
#
# WARNING: Do not modify this section unless you understand the
# license implications of what you are doing.
#
ALLOWED_LIB_DIRS := /lib:/lib/private:/lib/public


CFLAGS += -Werror -Wfatal-errors
LIBS += -lrt -lrdpactl

#
# Implicit rule will make the .c into a .o
# Implicit rule is $(CC) -c $(CPPFLAGS) $(CFLAGS)
# See Section 10.2 of Gnu Make manual
# 
$(EXE): $(OBJS)
	$(CC) -o $@ $(OBJS) -Wl,-rpath,$(CMS_LIB_RPATH) $(CMS_LIB_PATH) $(LIBS)



#
# Include the rule for making dependency files.
# The '-' in front of the second include suppresses
# error messages when make cannot find the .d files.
# It will just regenerate them.
# See Section 4.14 of Gnu Make.
#

include $(BUILD_DIR)/make.deprules

$(OBJS): wanconf.c
	$(CC) -c $(CFLAGS) wanconf.c -DKERNELVER=\"${LINUX_VER_STR}\"
else
conditional_build: 
	@echo "skip wanconf"
endif

-include $(OBJS:.o=.d)


