DRV_OBJS := unimac/unimac_drv_impl1.o
ifneq ("$(RDPA_PLATFORM)", "dsl")
    ifeq ($(strip ${BRCM_CHIP}),6838)
        DRV_OBJS += mdio/mdio_drv_impl1.o
        DRV_OBJS += phys/extphy/extphy_drv_impl1.o
        DRV_OBJS += phys/egphy/egphy_drv_impl1.o
        DRV_OBJS += phys/phys_common_drv_impl1.o
    endif
    ifeq ($(strip ${BRCM_CHIP}),6848)
        DRV_OBJS += mdio/mdio_drv_impl2.o
        DRV_OBJS += phys/extphy/extphy_drv_impl2.o
        DRV_OBJS += phys/egphy/egphy_drv_impl2.o
        DRV_OBJS += phys/phys_common_drv_impl2.o
    endif
endif

obj-y +=  $(DRV_OBJS)	

EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/drv/ -I$(INC_BRCMSHARED_PUB_PATH)/drv/phys/ -I$(INC_BRCMSHARED_PUB_PATH)/drv/phys/egphy 
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/drv/phys/extphy -I$(INC_BRCMSHARED_PUB_PATH)/drv/mdio  
EXTRA_CFLAGS += -I$(INC_RDPA_PATH) -I$(INC_BRCMSHARED_PUB_PATH)/rdp/  -I$(INC_BRCMSHARED_PUB_PATH)/rdp/base 
EXTRA_CFLAGS += -I$(INC_BDMF_PATH)/framework -I$(INC_BRCMSHARED_PUB_PATH)/drv/unimac  -I$(INC_BRCMSHARED_PUB_PATH)/pmc 
EXTRA_CFLAGS += -I$(INC_BRCMDRIVER_PUB_PATH)/$(BRCM_BOARD)
EXTRA_CFLAGS += -I$(INC_BRCMSHARED_PUB_PATH)/$(BRCM_BOARD)

-include $(TOPDIR)/Rules.make

