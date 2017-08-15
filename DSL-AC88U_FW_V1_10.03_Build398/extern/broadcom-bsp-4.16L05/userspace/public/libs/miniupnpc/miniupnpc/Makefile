# $Id: Makefile,v 1.120 2015/01/07 09:07:32 nanard Exp $
# MiniUPnP Project
# http://miniupnp.free.fr/
# http://miniupnp.tuxfamily.org/
# https://github.com/miniupnp/miniupnp
# (c) 2005-2015 Thomas Bernard

CC ?= gcc
CFLAGS ?= -O
CFLAGS += -Wall
CFLAGS += -W -Wstrict-prototypes
CFLAGS += -fno-common
CFLAGS += -DMINIUPNPC_SET_SOCKET_TIMEOUT
CFLAGS += -DMINIUPNPC_GET_SRC_ADDR
CFLAGS += -D_BSD_SOURCE -D_POSIX_C_SOURCE=200112L
CFLAGS += -ansi

# APIVERSION is used to build SONAME
APIVERSION = 12

SRCS = igd_desc_parse.c miniupnpc.c minixml.c minisoap.c miniwget.c \
       upnpc.c upnpcommands.c upnpreplyparse.c testminixml.c \
       minixmlvalid.c testupnpreplyparse.c minissdpc.c \
       upnperrors.c testigddescparse.c testminiwget.c \
       connecthostport.c portlistingparse.c receivedata.c \
       testportlistingparse.c miniupnpcmodule.c \
       minihttptestserver.c \
       listdevices.c

LIBOBJS = miniwget.o minixml.o igd_desc_parse.o minisoap.o \
          miniupnpc.o upnpreplyparse.o upnpcommands.o upnperrors.o \
          connecthostport.o portlistingparse.o receivedata.o


CFLAGS := -fPIC $(CFLAGS)
LIBOBJS := $(LIBOBJS) minissdpc.o


OBJS = $(patsubst %.c,%.o,$(SRCS))

# HEADERS to install
HEADERS = miniupnpc.h miniwget.h upnpcommands.h igd_desc_parse.h \
          upnpreplyparse.h upnperrors.h miniupnpctypes.h \
          portlistingparse.h \
          declspec.h

# library names
LIBRARY = libminiupnpc.a
SHAREDLIBRARY = libminiupnpc.so
SONAME = $(SHAREDLIBRARY).$(APIVERSION)

EXECUTABLES = upnpc-shared

LIBDIR ?= lib



.PHONY:	clean depend all

all:	$(LIBRARY) $(SHAREDLIBRARY)

clean:
	$(RM) $(LIBRARY) $(SHAREDLIBRARY) $(EXECUTABLES) $(OBJS)


distclean: clean

depend:
	makedepend -Y -- $(CFLAGS) -- $(SRCS) 2>/dev/null

$(LIBRARY):	$(LIBOBJS)
	$(AR) crs $@ $?

$(SHAREDLIBRARY):	$(LIBOBJS)
#	$(CC) -shared $(LDFLAGS) -Wl,-soname,$(SONAME) -o $@ $^
	$(CC) -shared $(LDFLAGS) -o $@ $^

upnpc-shared:	upnpc.o $(SHAREDLIBRARY)
	$(CC) $(LDFLAGS) -o $@ $^

minixmlvalid:	minixml.o minixmlvalid.o


# DO NOT DELETE THIS LINE -- make depend depends on it.

igd_desc_parse.o: igd_desc_parse.h
miniupnpc.o: miniupnpc.h declspec.h igd_desc_parse.h minissdpc.h miniwget.h
miniupnpc.o: minisoap.h minixml.h upnpcommands.h upnpreplyparse.h
miniupnpc.o: portlistingparse.h miniupnpctypes.h connecthostport.h
miniupnpc.o: receivedata.h
minixml.o: minixml.h
minisoap.o: minisoap.h miniupnpcstrings.h
miniwget.o: miniupnpcstrings.h miniwget.h declspec.h connecthostport.h
miniwget.o: receivedata.h
upnpc.o: miniwget.h declspec.h miniupnpc.h igd_desc_parse.h upnpcommands.h
upnpc.o: upnpreplyparse.h portlistingparse.h miniupnpctypes.h upnperrors.h
upnpcommands.o: upnpcommands.h upnpreplyparse.h portlistingparse.h declspec.h
upnpcommands.o: miniupnpctypes.h miniupnpc.h igd_desc_parse.h
upnpreplyparse.o: upnpreplyparse.h minixml.h
testminixml.o: minixml.h igd_desc_parse.h
minixmlvalid.o: minixml.h
testupnpreplyparse.o: upnpreplyparse.h
minissdpc.o: minissdpc.h miniupnpc.h declspec.h igd_desc_parse.h codelength.h
upnperrors.o: upnperrors.h declspec.h upnpcommands.h upnpreplyparse.h
upnperrors.o: portlistingparse.h miniupnpctypes.h miniupnpc.h
upnperrors.o: igd_desc_parse.h
testigddescparse.o: igd_desc_parse.h minixml.h miniupnpc.h declspec.h
testminiwget.o: miniwget.h declspec.h
connecthostport.o: connecthostport.h
portlistingparse.o: portlistingparse.h declspec.h miniupnpctypes.h minixml.h
receivedata.o: receivedata.h
testportlistingparse.o: portlistingparse.h declspec.h miniupnpctypes.h
miniupnpcmodule.o: miniupnpc.h declspec.h igd_desc_parse.h upnpcommands.h
miniupnpcmodule.o: upnpreplyparse.h portlistingparse.h miniupnpctypes.h
miniupnpcmodule.o: upnperrors.h
listdevices.o: miniupnpc.h declspec.h igd_desc_parse.h
