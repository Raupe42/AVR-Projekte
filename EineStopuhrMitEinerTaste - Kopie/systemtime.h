/*
 * systemtime.h
 *
 * Created: 13.06.2019 19:43:45
 *  Author: FET6U

Quelle:
https://www.mikrocontroller.net/topic/avr-timer-mit-32-bit#new

Timer 2, Dauer eines Timer-Ticks beim Vorteiler 8:
f=16000000Hz/8=2000000Hz  T=1/f=0.0000005s=0.5us
Zeit bis zum 64-Bit-Überlauf: 2^64**0.5*10^-6=292Kilojahre (nie)
*/

#ifndef SYSTEMTIME_H_
#define SYSTEMTIME_H_

#include <AVR/interrupt.h>

volatile uint64_t t2_soft;		// Zählt 0.5us-Ticks

#define SEC	0.0000005

ISR( TIMER2_OVF_vect )
{
	t2_soft += 256;
}

void t2_init( void )
{
	t2_soft = 0;
	TCCR2B = 1<<CS21;		// mode 0, prescaler 8
	TIMSK2 |= 1<<TOIE2;		// enable overflow interrupt
	sei();					// Globale Interrupt Freigabe
}


uint64_t get_ticks( void )  // read T2 as 64 bit timer
{
	uint64_t val;
	uint8_t tifr;

	cli();					// Interrupts global sperren
	val = t2_soft + TCNT2;	// val enthält den Zeitstempel
	tifr = TIFR2;			// read interrupt flags
	sei();					// Interrupts wieder an
	if( (tifr & (1<<TOV2) ) && !(val & 0x80) ) // overflow prior reading TCNT2 ?
		val += 256;         // then add overflow
	return val;
}

#endif /* SYSTEMTIME_H_ */