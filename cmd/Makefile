
all: pingrf

include ../lib.mk
include ../libradio.mk
include ../libpump.mk
include ../libusb.mk

OBJECTS += u.o
HEADERS += arg.h impl.h
CLEANFILES += pingrf

ifdef	CC1111
CFLAGS += -DCC1111
CFLAGS += -I../libusb
endif

pingrf: $(O)/pingrf.o $(OFILES) $(HFILES)
	$(CC) -o pingrf $(O)/pingrf.o $(OFILES)
