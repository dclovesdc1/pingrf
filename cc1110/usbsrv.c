#include "a.h"
#include "stdint.h"
#include "usb.h"
#include "intel_hex.h"

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

int
srvrxlower()
{
	uint8_t length;
	uint8_t in_byte;
	char nibbles[2];
	int bytes_to_read;

	nibbles[0] = usb_getchar();
	nibbles[1] = usb_getchar();
	in_byte = hex8(&nibbles[0]);

	if (nrx == 0) {
		if (in_byte == 0) {
			dprint("getting 0 size. error\n");
			flag &= ~Frxcall;
			return -1;
		}

		if (in_byte == 0xff) {
			dprint("radio reset\n");
			flag &= ~Frxcall;
			return -2;
		}

		// if the index is still at 0, here is the length
		rxcall[nrx++] = in_byte;
	}

	length = rxcall[0];
	while (nrx < length) {
		nibbles[0] = usb_getchar();
		nibbles[1] = usb_getchar();
		in_byte = hex8(&nibbles[0]);

		rxcall[nrx++] = in_byte;
		if(nrx == sizeof rxcall)
			panic("usb: rxcall overrun");
	}

	if (nrx == length) {
		flag |= Frxcall;
	}

	return 0;
}

void
srvpending()
{
	flag |= Frxcall;
}

//void
//srvrxlower()
//{
//}

void
srvtx()
{
	int i;
	uint8 length;
	char txcall_in_ascii[SPIMAX * 2];
	int ascii_idx;

	flag &= ~Ftxcall;
	length = txcall[0];

	for (ntx = 0; ntx < length; ntx++) {
		to_hex8_ascii(&txcall_in_ascii[ntx * 2], txcall[ntx]);
	}

	for (ascii_idx = 0; ascii_idx < length * 2; ascii_idx++) {
		usb_putchar(txcall_in_ascii[ascii_idx]);
	}
	usb_flush( );
	flag |= Ftxcall;
}
