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



typedef unsigned char  u8;
typedef   signed char  s8;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned long  u32;
typedef   signed long  s32;


//u32 t2_soft;	//zählt 0.5 µs ticks
unsigned int t2_soft;

ISR( TIMER2_OVF_vect )
{
	t2_soft += 256;
}


void t2_init( void )
{
	t2_soft = 0;
	TCCR2B = 1<<CS21;	// mode 0, prescaler 8
	TIMSK2 |= 1<<TOIE2;	// enable overflow interrupt
	//TCCR2A |= (1<<WGM01); //CTC Modus
	//OCR2A = 249; //Comparematch bei 249 -> Event je ms
	//TIMSK2 |= (1<<OCIE2A); //Interrupt des OCR aktiv

}


u32 get_ticks( void )			  // read T2 as 32 bit timer
{
	u32 val;
	u8 tifr;

	cli();	//clear interrupt
	val = t2_soft + TCNT2;
	tifr = TIFR2;				  // read interrupt flags
	sei();
	if( (tifr & 1<<TOV2) && !(val & 0x80) ) // overflow prior reading TCNT2 ?
	val += 256;				  // then add overflow

	return val;
}


int main( void )
{
	u32 timerticks;

	t2_init();
	sei();

	for(;;){		//kranke endlosschleife
		timerticks = get_ticks();
		//rücksetzen nach 24h
		if (timerticks == 675000000)
		timerticks = 0;
	}

	
}

