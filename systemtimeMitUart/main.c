/*
Auftrag:
Portierung eines hochauflösenden 32-Bit-Systemzeitgebers vom ATmega8 
auf den ATmega328P

Quelle:
https://www.mikrocontroller.net/topic/avr-timer-mit-32-bit#new

Anforderungen:
- Timer 2 
- CTC-Modus
- Vorteiler 8
- Einarbeitung der zum ATmega328 veränderten Pfade und Registernamen
- Erweiterung: Rücksetzen nach jeweils exakt 24h

- ein Timer Tick bei prescaler8:
	16 000 000/ 8= 2 000 000
	0,5µs
- Zeit bis 32 Bit voll
	4Byte max value 4 294 967 296
	/ 2 000 000
	= 2147,48 s
	= 35,79 min
- Anzhal Ticks / min
	2 000 000 * 60 = 120 000 000
- Zeit bis 64 bit voll
	sehr viel
	ca. 2 192 092,68 Jahre

*/

#include <avr/io.h>
#include <avr/interrupt.h>
#include <string.h>

typedef unsigned char  u8;
typedef   signed char  s8;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned long  u32;
typedef   signed long  s32;

#include "systemtime.h"
#include "uart.h"


//u32 t2_soft;	//zählt 0.5 µs ticks
char str [42];
long zeitAlt = 0;

int main( void )
{
	u32 timerticks;

	t2_init();
	sei();

	for(;;){		//kranke endlosschleife
		timerticks = get_ticks();

		if (zeitAlt + 2000000 < timerticks)
		{
		strcpy (str, "Uhrzeit: %f", (timerticks/2000000));

		zeitAlt = timerticks;
		}
		//rücksetzen nach 24h
		if (timerticks == 675000000)
		timerticks = 0;
	}

	
}

