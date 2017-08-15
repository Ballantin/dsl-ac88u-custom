ifdef BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
ifneq ($(strip $(CONFIG_BCM_ANDROID)),)
ccflags-y += -I$(src)			# needed for trace events
endif
endif # BCM_KF # defined(CONFIG_BCM_KF_ANDROID)

obj-$(CONFIG_ANDROID_BINDER_IPC)	+= binder.o
obj-$(CONFIG_ASHMEM)			+= ashmem.o
obj-$(CONFIG_ANDROID_LOGGER)		+= logger.o
obj-$(CONFIG_ANDROID_PERSISTENT_RAM)	+= persistent_ram.o
obj-$(CONFIG_ANDROID_RAM_CONSOLE)	+= ram_console.o
obj-$(CONFIG_ANDROID_TIMED_OUTPUT)	+= timed_output.o
obj-$(CONFIG_ANDROID_TIMED_GPIO)	+= timed_gpio.o
obj-$(CONFIG_ANDROID_LOW_MEMORY_KILLER)	+= lowmemorykiller.o
obj-$(CONFIG_ANDROID_SWITCH)		+= switch/
ifdef BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
ifneq ($(strip $(CONFIG_BCM_ANDROID)),)
obj-$(CONFIG_ANDROID_INTF_ALARM_DEV)	+= alarm-dev.o
obj-$(CONFIG_PERSISTENT_TRACER)		+= trace_persistent.o

CFLAGS_REMOVE_trace_persistent.o = -pg
else
obj-$(CONFIG_ANDROID_INTF_ALARM)	+= alarm.o
obj-$(CONFIG_ANDROID_INTF_ALARM_DEV)	+= alarm-dev.o
endif
else # BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
obj-$(CONFIG_ANDROID_INTF_ALARM)	+= alarm.o
obj-$(CONFIG_ANDROID_INTF_ALARM_DEV)	+= alarm-dev.o
endif # BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
