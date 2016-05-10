all: pingrf
HASLIBTASK := 1

include ../../root.mk


OBJECTS+=\
	$(TINY9OBJECTS)\
	$(LIBPUMPOBJECTS)\
	$(LIBRADIOOBJECTS)\
	$(PINGRFOBJECTS)\
	$(LIBTASKOBJECTS)\
	$(RADIODRIVEROBJECTS)

OBJECTS+=\
	u.o\
	rpigpio.o\
	tty.o

CLEANFILES+=pingrf

ifdef	CC1111
CFLAGS += -DCC1111
CFLAGS += -I../libusb
endif

pingrf: $(O)/pingrf.o $(OFILES) $(HFILES)
	$(CC) -o pingrf $^
