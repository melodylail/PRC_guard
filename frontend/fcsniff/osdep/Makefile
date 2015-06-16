AC_ROOT	= ..
include $(AC_ROOT)/common.mak

RTAP		= radiotap

LIB		= libosdep.a 
CFLAGS		+= $(PIC) -I.. $(LIBAIRPCAP)

OBJS_COMMON	= network.o file.o
OBJS		= osdep.o $(OBJS_COMMON)


OBJS_LINUX	= $(OBJS) linux.o linux_tap.o radiotap/radiotap.o common.o
OBJS_DUMMY	= $(OBJS) dummy.o dummy_tap.o
DOPCAP		=

all:
	@echo Building for $(OSNAME)
	@$(MAKE) .os.$(OSNAME)

.os.dummy: $(OBJS_DUMMY)
	$(AR) cru $(LIB) $(OBJS_DUMMY)
	$(RANLIB) $(LIB)
	touch $(@)

.os.Linux: $(OBJS_LINUX)
	$(AR) cru $(LIB) $(OBJS_LINUX)
	$(RANLIB) $(LIB)
	touch $(@)


.os.%: .os.dummy
	@echo "Your platform is unsupported by osdep, dummy code compiled."
	touch $(@)

install: all

uninstall:

clean: 
	$(MAKE) -C $(RTAP) clean
	rm -f $(LIB) *.o .os.*
