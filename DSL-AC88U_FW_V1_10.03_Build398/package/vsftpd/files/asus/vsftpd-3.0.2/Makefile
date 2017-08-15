# Makefile for systems with GNU tools
#CC 	=	gcc
#INSTALL	=	install
IFLAGS  = -idirafter dummyinc
#CFLAGS = -g
#CFLAGS	=	-O2 -fPIE -fstack-protector --param=ssp-buffer-size=4 \
#	-Wall -W -Wshadow -Werror -Wformat-security \
#	-D_FORTIFY_SOURCE=2 \
#	#-pedantic -Wconversion
#
#LIBS	=	`./vsf_findlibs.sh`
#LINK	=	-Wl,-s
#LDFLAGS	=	-fPIE -pie -Wl,-z,relro -Wl,-z,now

CFLAGS	+= -O2 -Wall $(EXTRACFLAGS) -ffunction-sections -fdata-sections -W -Wshadow \
	#-pedantic -Wconversion
CFLAGS  += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE=1 -D_LARGEFILE64_SOURCE=1 -D_LARGE_FILES=1
LDFLAGS += -ffunction-sections -fdata-sections -Wl,--gc-sections

LINK	=


#CFLAGS  += 	-I$(TOP)/libdisk
LDFLAGS  +=	-L$(TOPDIR)/extern/arc-release/usr/lib -larcgpl
ifeq ($(RTCONFIG_BCMARM),y)
#CFLAGS += -I$(SRCBASE)/common/include
#LDFLAGS += -lgcc_s
endif

ifeq ($(RTCONFIG_QTN),y)
#LDFLAGS += -L$(TOP)/libqcsapi_client -lqcsapi_client
endif

OBJS	=	main.o utility.o prelogin.o ftpcmdio.o postlogin.o privsock.o \
		tunables.o ftpdataio.o secbuf.o ls.o \
		postprivparent.o logging.o str.o netstr.o sysstr.o strlist.o \
    banner.o filestr.o parseconf.o secutil.o \
    ascii.o oneprocess.o twoprocess.o privops.o standalone.o hash.o \
    tcpwrap.o ipaddrparse.o access.o features.o readwrite.o opts.o \
    ssl.o sslslave.o ptracesandbox.o ftppolicy.o sysutil.o sysdeputil.o \
    seccompsandbox.o


.c.o:
	$(CC) -c $*.c $(CFLAGS) $(IFLAGS)

vsftpd: $(OBJS) 
	$(CC) -o vsftpd $(OBJS) $(LINK) $(LDFLAGS) $(LIBS)

install:
	mkdir -p $(DESTDIR)/usr/sbin
	$(INSTALL) -m 755 vsftpd $(DESTDIR)/usr/sbin/
	$(INSTALL_BIN) $(TOPDIR)/extern/arc-release/usr/lib/libarcgpl.so*  $(CONFIG_PREFIX)/usr/lib
	#mkdir -p $(DESTDIR)/usr/share/man/man8
	#$(INSTALL) -m 644 vsftpd.8 $(DESTDIR)/usr/share/man/man8/
	#mkdir -p $(DESTDIR)/usr/share/man/man5
	#$(INSTALL) -m 644 vsftpd.conf.5 $(DESTDIR)/usr/share/man/man5/
	#mkdir -p $(DESTDIR)/etc/xinetd.d
	#$(INSTALL) -m 644 xinetd.d/vsftpd $(DESTDIR)/etc/xinetd.d/

uninstall:
	rm -f $(DESTDIR)/usr/sbin/vsftpd
	rm -f $(DESTDIR)/usr/share/man/man8/vsftpd.8
	rm -f $(DESTDIR)/usr/share/man/man5/vsftpd.conf.5
	rm -f $(DESTDIR)/etc/xinetd.d/vsftpd

clean:
	rm -f *.o *.swp vsftpd

