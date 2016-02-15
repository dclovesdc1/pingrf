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

	in_byte = usb_pollchar();

	if (in_byte != (uint8) USB_READ_AGAIN) {
		if (nrx == 0) {
			// if the index is still at 0, here is the length
			dprint("\nrcv length : %d\n", in_byte);
			rxcall[nrx++] = in_byte;
		}

		length = rxcall[0];
		while (nrx < length) {
			if ((in_byte = usb_pollchar()) == (uint8) USB_READ_AGAIN) {
				return;
			}

			dprint("0x%x ", in_byte);
			rxcall[nrx++] = in_byte;
			if(nrx == sizeof rxcall)
				panic("usb: rxcall overrun");
		}

		if (nrx == length) {
			flag |= Frxcall;
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
