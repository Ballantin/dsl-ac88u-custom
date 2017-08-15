
ccflags-$(CONFIG_PM_DEBUG)	:= -DDEBUG

obj-y				+= qos.o
obj-$(CONFIG_PM)		+= main.o
obj-$(CONFIG_VT_CONSOLE_SLEEP)	+= console.o
obj-$(CONFIG_FREEZER)		+= process.o
obj-$(CONFIG_SUSPEND)		+= suspend.o
obj-$(CONFIG_PM_TEST_SUSPEND)	+= suspend_test.o
obj-$(CONFIG_HIBERNATION)	+= hibernate.o snapshot.o swap.o user.o \
				   block_io.o
ifdef BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
ifneq ($(strip $(CONFIG_BCM_ANDROID)),)
obj-$(CONFIG_PM_AUTOSLEEP)	+= autosleep.o
obj-$(CONFIG_PM_WAKELOCKS)	+= wakelock.o
obj-$(CONFIG_SUSPEND_TIME)	+= suspend_time.o
endif
endif # BCM_KF # defined(CONFIG_BCM_KF_ANDROID)

obj-$(CONFIG_MAGIC_SYSRQ)	+= poweroff.o
ifdef BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
ifneq ($(strip $(CONFIG_BCM_ANDROID)),)
obj-$(CONFIG_ANDROID)	+= wakelock.o
endif
endif # BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
