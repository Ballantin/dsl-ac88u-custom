#
# Makefile for cpuidle.
#

obj-y += cpuidle.o driver.o governor.o sysfs.o governors/
ifdef BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
ifneq ($(strip $(CONFIG_BCM_ANDROID)),)
obj-$(CONFIG_ARCH_NEEDS_CPU_IDLE_COUPLED) += coupled.o
endif
endif # BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
