ifdef BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
# kbuild trick to avoid linker error. Can be omitted if a module is built.
obj- := dummy.o

# List of programs to build
hostprogs-y := uhid-example

# Tell kbuild to always build the programs
always := $(hostprogs-y)

HOSTCFLAGS_uhid-example.o += -I$(objtree)/usr/include

endif # BCM_KF # defined(CONFIG_BCM_KF_ANDROID)
