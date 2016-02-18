
OBJ = _build

CC = sdcc
CFLAGS = --model-large --opt-code-size --std-c99
LDFLAGS_FLASH = \
	 --std-c99  \
	--out-fmt-ihx \
	--xram-loc 0xf000  \
	--iram-size 0x100 \
	--xram-size 0x0f00 \

ifdef CCLOADER
LDFLAGS_FLASH += --code-loc 0x1400 --code-size 0x8000
else
LDFLAGS_FLASH += --code-loc 0x000 --code-size 0x8000
endif

CFLAGS += -I../lib/8051
CFLAGS += -I../lib
CFLAGS += -I..
CFLAGS += -I.

SRC = main.c rf.c timer.c call.c callrx.c calltx.c calltxrx.c callping.c cobs.c u.c conv.c

ifdef CC1111
CFLAGS += -DCC1111
CFLAGS += -DTI_DONGLE
CFLAGS += -I../libusb
SRC += usbsrv.c usb.c hal.c usb_descriptors.c intel_hex.c
else
CFLAGS += -DCC1110
ifdef UART
SRC += uartsrv.c
else
SRC += spisrv.c
endif
endif

ifdef DEBUG
#CFLAGS += --debug
CFLAGS += -DDEBUG
SRC += cc1110print.c
#SRC += print.c
#SRC += radiofmt.c
else
SRC += noprint.c
endif

ADB=$(addprefix $(OBJ)/, $(SRC:.c=.adb))
ASM=$(addprefix $(OBJ)/, $(SRC:.c=.asm))
LNK=$(addprefix $(OBJ)/, $(SRC:.c=.lnk))
LST=$(addprefix $(OBJ)/, $(SRC:.c=.lst))
REL=$(addprefix $(OBJ)/, $(SRC:.c=.rel))
RST=$(addprefix $(OBJ)/, $(SRC:.c=.rst))
SYM=$(addprefix $(OBJ)/, $(SRC:.c=.sym))
PROGS=ping.hex
PCDB=$(addprefix $(OBJ)/, $(PROGS:.hex=.cdb))
PLNK=$(addprefix $(OBJ)/, $(PROGS:.hex=.lnk))
PMAP=$(addprefix $(OBJ)/, $(PROGS:.hex=.map))
PMEM=$(addprefix $(OBJ)/, $(PROGS:.hex=.mem))
PAOM=$(addprefix $(OBJ)/, $(PROGS:.hex=))

vpath %.c .
vpath %.c ../lib
vpath %.c ../libradio
vpath %.c ../libusb

all: $(OBJ)/ping.hex

$(OBJ):
	mkdir $@

$(OBJ)/%.rel : %.c
	$(CC) -c $(CFLAGS) -o$@ $<

$(OBJ)/ping.hex: $(OBJ) $(REL) Makefile a.h ../libradio/rcall.h ../lib/8051/u.h
	$(CC) $(LDFLAGS_FLASH) $(CFLAGS) -o $@ $(REL)

flash: $(OBJ)/ping.hex
	cc-tool -v -e -w $<

clean:
	rm -rf $(OBJ)
