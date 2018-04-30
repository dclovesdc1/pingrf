#include "pumpimpl.h"

int
pstat(Pstat *ps)
{
	static Pcall tx, rx;

	memset(ps, 0, sizeof *ps);

	if(_presume() < 0)
		return -1;

	tx.type = Tstatus;
	if(_pcall(&tx, &rx) != 1)
		return -1;

	ps->month = rx.status.month;
	ps->day = rx.status.day;
	ps->hour = rx.status.hour;
	ps->minute = rx.status.minute;
	ps->year = rx.status.year;
	ps->basal = rx.status.basal;
	ps->insulinleft = rx.status.insulin;
	ps->temp = rx.status.temp;
	ps->temptime = rx.status.temptime;
	ps->temptotal = rx.status.temptotal;
	
	if(rx.flag&Fwarn)
		ps->haswarning = 1;
	
	tx.type = Tstatus4;
	if(_pcall(&tx, &rx) != 1)
		return -1;
	
	ps->comboactive = rx.status4.active;
	ps->comboyear = rx.status4.year;
	ps->combomonth = rx.status4.month;
	ps->comboday = rx.status4.day;
	ps->combostarthour = rx.status4.starthour;
	ps->combostartminute = rx.status4.startminute;
	ps->comboendhour = rx.status4.endhour;
	ps->comboendminute = rx.status4.endminute;
	ps->combodelivered = rx.status4.delivered;
	ps->combototal = rx.status4.total;

	if(_preset() < 0)
		return -1;

	tx.type = Tstatus2;
	if(_pcall(&tx, &rx) != 1)
		return -1;

	/* XXX last bolus time */

	ps->lastbolus = rx.status2.bolus;
	ps->iob = rx.status2.iob;
	
	if(_preset() < 0)
		return -1;

	tx.type = Tstatus;
	if(_pcall(&tx, &rx) != 1)
		return -1;

	tx.type = Tstatus3;
	if(_pcall(&tx, &rx) != 1)
		return -1;

	ps->dailybolus = rx.status3.bolus;
	ps->dailybasal = rx.status3.basal;

	_padjourn();

	return 0;
}

int
pstat_iob(Pstat *ps)
{
	static Pcall tx, rx;

	memset(ps, 0, sizeof *ps);

	if(_presume() < 0)
		return -1;

	tx.type = Tstatus2;
	if(_pcall(&tx, &rx) != 1)
		return -1;

	/* XXX last bolus time */

	ps->lastbolus = rx.status2.bolus;
	ps->iob = rx.status2.iob;

	_padjourn();

	return 0;
}

int
pstat_basal(Pstat *ps)
{
	static Pcall tx, rx;

	memset(ps, 0, sizeof *ps);

	if(_presume() < 0)
		return -1;

	tx.type = Tstatus;
	if(_pcall(&tx, &rx) != 1)
		return -1;

	ps->basal = rx.status.basal;
	
	if(rx.flag&Fwarn)
		ps->haswarning = 1;
	
	_padjourn();

	return 0;
}

int
pstat_combo(Pstat *ps)
{
	static Pcall tx, rx;

	memset(ps, 0, sizeof *ps);

	if(_presume() < 0)
		return -1;

	tx.type = Tstatus4;
	if(_pcall(&tx, &rx) != 1)
		return -1;
	
	ps->comboactive = rx.status4.active;
	ps->comboyear = rx.status4.year;
	ps->combomonth = rx.status4.month;
	ps->comboday = rx.status4.day;
	ps->combostarthour = rx.status4.starthour;
	ps->combostartminute = rx.status4.startminute;
	ps->comboendhour = rx.status4.endhour;
	ps->comboendminute = rx.status4.endminute;
	ps->combodelivered = rx.status4.delivered;
	ps->combototal = rx.status4.total;

	if(_preset() < 0)
		return -1;

	_padjourn();

	return 0;
}

int
pbolus(uint insulin, uint minutes)
{
	static Pcall tx, rx;

	if(minutes%6 != 0){
		werrstr("combo duration must be in increments of 6 minutes!");
		return -1;
	}

	if(_presume() < 0)
		return -1;

	tx.type = Tbolus;
	tx.bolus.insulin = insulin;
	tx.bolus.minutes = minutes;
	if(_pcall(&tx, &rx) != 1)
		return -1;

	if(rx.bolus.insulin != insulin || rx.bolus.minutes != minutes){
		werrstr("pump returned mismatch bolus parameters (i=%d, m=%d) vs expected (i=%d, m=%d)",
			rx.bolus.insulin, rx.bolus.minutes, insulin, minutes);
		return -1;
	}

	tx.type = minutes > 0 ? Tcomboack : Tbolusack;
	if(_pcall(&tx, &rx) != 1)
		return -1;

	for(;;){
		tx.type = Tdeliverystatus;
		if(_pcall(&tx, &rx) != 1)
			return -1;

		switch(rx.deliverystatus){
		case DeliveryBusy:
		case DeliveryUnknown:
			tx.type = Tdeliverycontinue;
			if(_pcall(&tx, &rx) != 1)
				return -1;
			continue;

		case DeliveryDone:
			break;
		}
		break;
	}

	if (minutes == 0) {
		tx.type = Tack3;
		if(_pcall(&tx, &rx) != 1)
			return -1;
	}

	_padjourn();

	return 0;
}

int
pcancel()
{
	static Pcall tx, rx;

	if(_presume() < 0)
		return -1;
	
	tx.type = Tcancelcombo;
	if(_pcall(&tx, &rx) != 1)
		return -1;
	
	_padjourn();
	return 0;
}

int
pclearwarning0()
{
	static Pcall tx, rx;

	if(_presume() < 0)
		return -1;

#if	0
	if(_pcallsimple(Tclearwarn) < 0)
		return -1;
#else
	tx.type = Tclearwarn;
	tx.warning.type = Twarning_0;
	if(_pcall(&tx, &rx) != 1)
		return -1;
#endif
	
	_padjourn();

	return 0;
}

int
pclearwarning1()
{
	static Pcall tx, rx;

	if(_presume() < 0)
		return -1;

	tx.type = Tclearwarn;
	tx.warning.type = Twarning_1;
	if(_pcall(&tx, &rx) != 1)
		return -1;
	
	_padjourn();

	return 0;
}
