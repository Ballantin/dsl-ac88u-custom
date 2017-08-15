
#
# Try to positively identify the OS. Keep the first match.
#

TARGET_OSTYPE=
ENDIAN=

# See if OSTYPE contains "bsd" (case insensitive)
ifndef TARGET_OSTYPE
  ifneq ($(shell echo $(OSTYPE) | grep -i bsd),)
    TARGET_OSTYPE= bsd
    MAKE=gmake
  endif
endif

# See if OSTYPE contains "linux" (case insensitive)
ifndef TARGET_OSTYPE
  ifneq ($(shell uname -a | grep -i linux),)
    MAKE=make
#	TARGET_OSTYPE=-DLINUX
	ENDIAN = -DLE_ENDIAN
  endif
endif

# See if OSTYPE contains "vxworks" (case insensitive)
ifndef TARGET_OSTYPE
  ifneq ($(shell echo $(OSTYPE) | grep -i vxworks),)
    TARGET_OSTYPE= vxworks
  endif
endif

# See if uname contains "SunOS" (case insensitive)
ifndef TARGET_OSTYPE
  ifneq ($(shell uname -a | grep -i SunOS),)
    MAKE=gmake
	SOCKLIB=-lsocket
#	TARGET_OSTYPE=-DLINUX
  endif
endif

SIMULATION= -DBSAFE_SOCKET_SIM -DBLOCK_IOCTL_MODE
BSAFE_WRAPPER=bsafe_sim

EXTRA_CFLAGS = $(TARGET_OSTYPE) $(ENDIAN) $(SOCKLIB) $(SIMULATION)

BSAFE_BASE=.
BSAFE_MOD=bsafemod.o
BSAFE_LIB=libbsafeuhsm.so
BSAFE_LIB_A=libbsafeuhsm.a
BSAFE_SHARED_DIR = $(BSAFE_BASE)/shared
BSAFE_MOD_DIR = $(BSAFE_BASE)/sys
BSAFE_LIB_DIR = $(BSAFE_BASE)/lib
BSAFE_DIAG_DIR = $(BSAFE_BASE)/diag

SHARED_DEP = $(BSAFE_SHARED_DIR)/bsafe_osl.h


#DEBUG=-ggdb -DDEBUG


all: $(BSAFE_SHARED_DIR)/bsafe_vers.h $(BSAFE_MOD_DIR)/$(BSAFE_MOD) $(BSAFE_LIB_DIR)/$(BSAFE_LIB) #$(BSAFE_DIAG_DIR)/bsafe_uhsm_lib_test


$(BSAFE_MOD_DIR)/$(BSAFE_MOD): $(BSAFE_MOD_DIR)/bsafe.h $(BSAFE_MOD_DIR)/bsafe_io.h $(BSAFE_MOD_DIR)/bsafe_io.c $(BSAFE_MOD_DIR)/bsafe_misc.c $(SHARED_DEP) $(BSAFE_MOD_DIR)/bsafe_readwrite.c $(BSAFE_MOD_DIR)/$(BSAFE_WRAPPER).c
	gcc  -c $(BSAFE_MOD_DIR)/bsafe_io.c    -I$(BSAFE_SHARED_DIR) -I$(BSAFE_MOD_DIR) $(EXTRA_CFLAGS)  $(DEBUG) -o $(BSAFE_MOD_DIR)/bsafe_io.o
	gcc  -c $(BSAFE_MOD_DIR)/bsafe_misc.c  -I$(BSAFE_SHARED_DIR) -I$(BSAFE_MOD_DIR) $(EXTRA_CFLAGS)  $(DEBUG) -o $(BSAFE_MOD_DIR)/bsafe_misc.o
	gcc  -c $(BSAFE_MOD_DIR)/bsafe_readwrite.c  -I$(BSAFE_SHARED_DIR) -I$(BSAFE_MOD_DIR) $(EXTRA_CFLAGS)  $(DEBUG) -o $(BSAFE_MOD_DIR)/bsafe_readwrite.o
	gcc  -c $(BSAFE_MOD_DIR)/$(BSAFE_WRAPPER).c  -I$(BSAFE_SHARED_DIR) -I$(BSAFE_MOD_DIR) $(EXTRA_CFLAGS)  $(DEBUG) -o $(BSAFE_MOD_DIR)/$(BSAFE_WRAPPER).o
	ld  -r  $(BSAFE_MOD_DIR)/bsafe_io.o  $(BSAFE_MOD_DIR)/bsafe_misc.o   $(BSAFE_MOD_DIR)/$(BSAFE_WRAPPER).o -o $(BSAFE_MOD_DIR)/$(BSAFE_MOD)
	@echo ""
	@echo ""
	@echo "CLEAN THE WARNINGS ABOVE THIS LINE."
	@echo ""

$(BSAFE_LIB_DIR)/$(BSAFE_LIB): $(BSAFE_LIB_DIR)/bsafe_uhsm_lib.h $(BSAFE_LIB_DIR)/bsafe_uhsm_lib.c $(SHARED_DEP)
#	gcc   $(BSAFE_LIB_DIR)/bsafe_uhsm_lib.c  -I$(BSAFE_SHARED_DIR) -I$(BSAFE_MOD_DIR) -I$(BSAFE_LIB_DIR)  $(EXTRA_CFLAGS)  -shared -o $(BSAFE_LIB_DIR)/$(BSAFE_LIB)
# simulation
	gcc  $(BSAFE_MOD_DIR)/bsafe_io.c $(BSAFE_MOD_DIR)/bsafe_misc.c 	\
		$(BSAFE_MOD_DIR)/bsafe_readwrite.c		\
		$(BSAFE_MOD_DIR)/$(BSAFE_WRAPPER).c		\
		 $(BSAFE_LIB_DIR)/bsafe_uhsm_lib.c  -I$(BSAFE_SHARED_DIR) -I$(BSAFE_MOD_DIR) -I$(BSAFE_LIB_DIR) $(EXTRA_CFLAGS) -shared  -o $(BSAFE_LIB_DIR)/$(BSAFE_LIB) 
	gcc  $(BSAFE_MOD_DIR)/bsafe_io.c $(BSAFE_MOD_DIR)/bsafe_misc.c $(BSAFE_LIB_DIR)/bsafe_uhsm_lib.c  -I$(BSAFE_SHARED_DIR) -I$(BSAFE_MOD_DIR) -I$(BSAFE_LIB_DIR) $(EXTRA_CFLAGS)  -r -o $(BSAFE_LIB_DIR)/$(BSAFE_LIB_A) 
	@echo ""
	@echo ""
	@echo "CLEAN THE WARNINGS ABOVE THIS LINE."
	@echo ""


$(BSAFE_DIAG_DIR)/bsafe_uhsm_lib_test: $(BSAFE_DIAG_DIR)/bsafe_uhsm_lib_test.c   $(BSAFE_LIB_DIR)/$(BSAFE_LIB)  
	gcc  $(BSAFE_DIAG_DIR)/bsafe_uhsm_lib_test.c  -I$(BSAFE_SHARED_DIR) -I$(BSAFE_LIB_DIR) -L$(BSAFE_LIB_DIR) -lbsafeuhsm   -o $(BSAFE_DIAG_DIR)/bsafe_uhsm_lib_test 
# libbsafeuhsm.so has to be copied to /usr/lib/libbsafeuhsm.so ???
#simulation
	gcc  $(BSAFE_MOD_DIR)/bsafe_io.c $(BSAFE_MOD_DIR)/bsafe_misc.c $(BSAFE_MOD_DIR)/bsafe_readwrite.c $(BSAFE_MOD_DIR)/$(BSAFE_WRAPPER).c $(BSAFE_LIB_DIR)/bsafe_uhsm_lib.c $(BSAFE_DIAG_DIR)/bsafe_uhsm_lib_test.c  -I$(BSAFE_SHARED_DIR) -I$(BSAFE_MOD_DIR) -I$(BSAFE_LIB_DIR) $(EXTRA_CFLAGS) $(DEBUG)  -o $(BSAFE_DIAG_DIR)/bsafe_uhsm_lib_test 

clean: 
	rm -rf $(BSAFE_MOD_DIR)/*.o  $(BSAFE_MOD_DIR)/bsafemod.o
	rm -rf $(BSAFE_LIB_DIR)/*.o $(BSAFE_LIB_DIR)/libbsafeuhsm.a $(BSAFE_LIB_DIR)/libbsafeuhsm.so
	rm -rf $(BSAFE_DIAG_DIR)/*.o $(BSAFE_DIAG_DIR)/bsafe_uhsm_lib_test


release_tar:
	./release_tar.sh release
