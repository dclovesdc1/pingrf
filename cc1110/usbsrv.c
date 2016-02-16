#include "a.h"
#define uint32_t uint32
#define uint16_t uint16
#define uint8_t uint8

#include "usb.h"
/*
*/

static volatile uint8 nrx, ntx;

void
srvinit()
{
  usb_init( );
  usb_enable( );
  usb_up( );
  usb_flush( );
}

void
srvrx()
{
	flag &= ~Frxcall;
	nrx = 0;
}

void
srvrxpeek()
{
	uint8 length;
	uint8 in_byte;
	int bytes_to_read;

	if ((bytes_to_read = usb_is_char_present()) > 0) {
		in_byte = usb_getchar();

		if (nrx == 0) {
			if (in_byte == 0) {
				dprint("getting 0 size. error\n");
				return;
			}

			// if the index is still at 0, here is the length
			dprint("\nrcv length : %d\n", in_byte);
			dprint("0x%x ", in_byte);
			rxcall[nrx++] = in_byte;
		}

		length = rxcall[0];
		while (nrx < length) {
			if ((bytes_to_read = usb_is_char_present()) > 0) {
				in_byte = usb_getchar();

				dprint("0x%x ", in_byte);
				rxcall[nrx++] = in_byte;
				if(nrx == sizeof rxcall)
					panic("usb: rxcall overrun");
			} else {
				return;
			}
		}

		if (nrx == length) {
			flag |= Frxcall;
			dprint("\n");
		}
	}
}

void
srvrxlower()
{
}

void
srvtx()
{
	int i;
	uint8 length;

	flag &= ~Ftxcall;
	length = txcall[0];
	dprint("\nsending %d bytes\n", length);
	for (ntx = 0; ntx < length; ntx++) {
		dprint("0x%x ", txcall[ntx]);
		usb_putchar(txcall[ntx]);
	}
	dprint("\n");
	usb_flush( );
	flag |= Ftxcall;
}
