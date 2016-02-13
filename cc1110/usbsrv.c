#include "a.h"

/*
*/

enum
{
	Uidle,
	Uready,
	Urxing
};

void
srvinit()
{
	configure_serial();
}

void
srvrx()
{
}

void
srvrxlower()
{
}

void
urx0intr(void) __interrupt URX0_VECTOR
{
}

void
srvtx()
{
}

void
utx0intr(void) __interrupt UTX0_VECTOR
{
}
