# Tested with GNU make v3.80
#OPENSSL=		/usr/local/openssl Gigi
OPENSSL=		/home/gigis/work/uhsm/openssl/openssl-0.9.7c/install
OPENSSL_INCLUDE=	$(OPENSSL)/include
OPENSSL_LIB=		$(OPENSSL)/lib

NCIPHER_FLAVOUR=	/gcc

#NFAST_HOME=		/opt/nfast  Gigi
NFAST_HOME=		/home/gigis/work/uhsm/opt/nfast
NFAST_KDP_PATH=		$(NFAST_HOME)/c/kdp$(NCIPHER_FLAVOUR)

VPATH=			.:../cutils:../stdmarbase:../emulator:../kdcp

# Location of cutils include directory
CUTILS_INCLUDEDIR=	$(NFAST_KDP_PATH)/include/cutils

# Location of exported library directory
KDP_LIBDIR=		$(NFAST_KDP_PATH)/lib

CC=			gcc 
#CPPFLAGS=		-g -O2 -Wall -Wwrite-strings -Wmissing-prototypes \
#			-Werror 
CPPFLAGS=		-g -O2 -Wall -Wwrite-strings -Wmissing-prototypes \
			-I. -I$(CUTILS_INCLUDEDIR) \
			-I../cutils -I../stdmarbase -I../emulator -I../kdcp \
			-I$(OPENSSL_INCLUDE) \
			-D_GNU_SOURCE \
			-DNFAST_CONF_H='"nfast-conf-auto.h"'
LINK=			gcc
LDFLAGS=		$(XLDFLAGS)
LDLIBS=			-L$(OPENSSL_LIB) -lccrypto
CUEXAMPLES_SYS_OBJS=	readpp-unix.o

CUEXAMPLES_OBJS=	getdate.o \
			nfopt-join.o nfopt-time.o nfopt.o nftypes.o \
			report-usage.o report.o tokenise.o \
			$(CUEXAMPLES_SYS_OBJS)

STDMARBASE_OBJS=	basictypes.o basicmar.o basicunmar.o \
			messages-abasic-enstr.o messages-abasic-free.o \
			messages-abasic-print.o messages-abasic-mar.o \
			messages-abasic-unmar.o

EMULATOR_OBJS=		clientmem.o commands.o dlies.o \
			kdpbignum.o kdpmar.o kdpunmar.o \
			messages-akdp-enstr.o messages-akdp-free.o \
			messages-akdp-print.o

KDCP_OBJS=		client.o kdcptypes.o kdcptypes.o keyhash.o \
			messages-akdcp-enstr.o messages-akdcp-free.o \
			messages-akdcp-mar.o messages-akdcp-print.o \
			messages-akdcp-unmar.o \
			messages-akdp-mar.o messages-akdp-unmar.o

TESTHARNESS_OBJS=	harness.o $(EMULATOR_OBJS) $(STDMARBASE_OBJS) \
			$(CUEXAMPLES_OBJS) $(KDP_LIBDIR)/libcutils.a

INITIALIZE_OBJS=	initialize.o $(EMULATOR_OBJS) $(STDMARBASE_OBJS) \
			$(CUEXAMPLES_OBJS) $(KDP_LIBDIR)/libcutils.a

GETKEY_OBJS=		getkey.o $(KDCP_OBJS) $(EMULATOR_OBJS) \
			$(STDMARBASE_OBJS) \
			$(CUEXAMPLES_OBJS) $(KDP_LIBDIR)/libcutils.a

LISTKEYS_OBJS=		listkeys.o $(KDCP_OBJS) $(EMULATOR_OBJS) \
			$(STDMARBASE_OBJS) \
			$(CUEXAMPLES_OBJS) $(KDP_LIBDIR)/libcutils.a

TARGETS=		uhsm-testharness uhsm-initialize \
			uhsm-getkey uhsm-listkeys

# BSAFE
DEBUG= -ggdb
CPPFLAGS += $(DEBUG)
BSAFE_DIR = 		/home/gigis/work/uhsm/bsafe/cvs
CPPFLAGS +=		-DUSE_BSAFE_UHSM -DBSAFE_SOCKET_SIM
CPPFLAGS +=		-I$(BSAFE_DIR) -I$(BSAFE_DIR)/shared -I$(BSAFE_DIR)/lib
LDLIBS+=		-L$(BSAFE_DIR)/lib -lbsafeuhsm

.SUFFIXES:		.c .o

all:			$(TARGETS)

.c.o:
	$(CC) $(CPPFLAGS) -c -o $@ $<

uhsm-testharness:	$(TESTHARNESS_OBJS)
	$(LINK) $(LDFLAGS) -o $@ $^ $(LDLIBS)

uhsm-initialize:	$(INITIALIZE_OBJS)
	$(LINK) $(LDFLAGS) -o $@ $^ $(LDLIBS)

uhsm-getkey:		$(GETKEY_OBJS)
	$(LINK) $(LDFLAGS) -o $@ $^ $(LDLIBS)

uhsm-listkeys:		$(LISTKEYS_OBJS)
	$(LINK) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# We supply an up-to-date getdate.c. Prevent it from being automatically
# rebuilt in the case where getdate.y's mtime is (usually accidentally)
# newer; if you want to modify it, run GNU bison over it afterwards.
getdate.c: ;

clean:
	rm -f *.o *~ $(TARGETS)
