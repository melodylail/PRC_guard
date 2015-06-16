AC_ROOT		= .
include $(AC_ROOT)/common.mak

CFLAGS		+= -Iinclude -I.

iCC             = $(shell find /opt/intel/cc/*/bin/icc)
iCFLAGS         = -w -mcpu=pentiumpro -march=pentiumpro $(COMMON_CFLAGS)
iOPTFLAGS       = -g -ip -ipo -D_FILE_OFFSET_BITS=64 -D_DEBUG
PROF_DIR	= $(PWD)/prof

BINFILES        = 

TARGET       = fcsniff

ASM_AC		= sha1-sse2.S

OBJS_ADU	= sniff.o ndeapi.o nde_log.o scan_firechat.o dump.o protocol.o common.o osdep/radiotap/radiotap.o 

ifneq ($(OSNAME), Linux)
	OBJS_ADU	+= osdep/common.o
endif

LDFLAGS := $(LDFLAGS) -Llib
OSD		= osdep
LIBS		:= -L$(OSD) -l$(OSD) $(LIBS) lib/libcurl.a
LIBOSD		= $(OSD)/lib$(OSD).a

all: userland $(TARGET)

userland: $(BINFILES)

$(LIBOSD):
	$(MAKE) -C $(OSD)

$(TARGET): $(OBJS_ADU) $(LIBOSD)
	$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS_ADU) -o $(@) $(LIBS)  

strip: $(BINFILES) $(TARGET)
	strip $(BINFILES) $(TARGET)

clean:
	$(MAKE) -C $(OSD) clean
	-rm -f $(TAEGET) $(BINFILES) $(OPTFILES) $(OBJS_ADU) a.out

distclean: clean

install: all
	$(MAKE) -C $(OSD) install
	install -d $(DESTDIR)$(bindir)
	install -m 755 $(BINFILES) $(DESTDIR)$(bindir)
	install -d $(DESTDIR)$(sbindir)
	install -m 755 $(TARGET) $(DESTDIR)$(sbindir)

uninstall:
	$(MAKE) -C $(OSD) uninstall
	-rm -f $(DESTDIR)$(sbindir)/$(TARGET)
	-rm -rf $(DESTDIR)$(etcdir)

