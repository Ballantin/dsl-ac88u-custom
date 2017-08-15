#
# <:copyright-BRCM:2013:DUAL/GPL:standard
# 
#    Copyright (c) 2013 Broadcom 
#    All Rights Reserved
# 
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License, version 2, as published by
# the Free Software Foundation (the "GPL").
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# 
# A copy of the GPL is available at http://www.broadcom.com/licenses/GPLv2.php, or by
# writing to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
# 
# :> 
#

# remove optimization for debug 
#EXTRA_CFLAGS += -O0 -g
obj-y	 := rdp_bbh_arrays.o       \
            rdp_drv_bbh.o          \
            rdp_drv_bpm.o          \
            rdp_drv_ih.o           \
            rdp_drv_sbpm.o         \
            rdp_runner_arrays.o    \
            rdp_dma_arrays.o       \

EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/rdp
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/rdp/base
EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)
ifeq ($(strip $(BRCM_CHIP)),63138)
EXTRA_CFLAGS += -DDSL_63138
endif
ifeq ($(strip $(BRCM_CHIP)),63148)
EXTRA_CFLAGS += -DDSL_63148
endif
ifeq ($(strip $(BRCM_CHIP)),6838)
EXTRA_CFLAGS += -D__OREN__
endif
ifeq ($(strip $(BRCM_CHIP)),6848)
EXTRA_CFLAGS += -D__OREN__
endif

-include $(TOPDIR)/Rules.make

clean:
	rm -f core *.o *.a *.s
