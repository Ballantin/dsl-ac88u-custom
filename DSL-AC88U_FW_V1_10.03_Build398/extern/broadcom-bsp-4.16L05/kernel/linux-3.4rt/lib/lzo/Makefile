lzo_compress-objs := lzo1x_compress.o
ifdef BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
ifneq ($(strip $(CONFIG_BCM_ANDROID)),)
lzo_decompress-objs := lzo1x_decompress_safe.o
else
lzo_decompress-objs := lzo1x_decompress.o
endif
else # BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
lzo_decompress-objs := lzo1x_decompress.o
endif # BCM_KF # defined(CONFIG_BCM_KF_ANDROID)

obj-$(CONFIG_LZO_COMPRESS) += lzo_compress.o
obj-$(CONFIG_LZO_DECOMPRESS) += lzo_decompress.o
