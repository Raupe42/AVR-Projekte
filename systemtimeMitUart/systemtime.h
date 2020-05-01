/*
 * systemtime.h
 *
 * Created: 13.06.2019 19:45:51
 *  Author: jensh
 */ 


#ifndef SYSTEMTIME_H_
#define SYSTEMTIME_H_

#include <avr/interrupt.h>

	t2_soft = 0;
	TCCR2B = 1<<CS21;	// mode 0, prescaler 8
	TIMSK2 |= 1<<TOIE2;	// enable overflow interrupt

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




#endif /* SYSTEMTIME_H_ */