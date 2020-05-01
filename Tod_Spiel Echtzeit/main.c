/*
* Tod_Spiel.c
*
* Created: 31.01.2019 19:40:52
* Author : jensh
Realisierung als endlicher Automat
*/

#include <avr/io.h>
#include <avr/interrupt.h>
#define  F_CPU 16000000

#define TASTER 2	//PINB
#define LED_RT 1		//PORTB
#define LED_GN 0
#define Z_STOPP 1
#define Z_RUN 2
#define T 0

typedef unsigned char  u8;
typedef   signed char  s8;
typedef unsigned short u16;
typedef   signed short s16;
typedef unsigned long  u32;
typedef   signed long  s32;

u32 t2_soft;	//zählt 0.5 µs ticks

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




int main(void)
{
	uint8_t zustand = 0, taster = 0, taster_alt = 0;
	double t = 0.0, t0 = 0.0;

	u32 timerticks;		

	t2_init();
	sei();


	//Hardware vorbereiten
	DDRB = 0 | (1<<LED_RT) | (1<<LED_GN);		//1 für Ausgang
	PORTB = (1<<TASTER); //PULLUP

	zustand = Z_STOPP;	//1 Stopp, 2 Run
	taster_alt =0;	//1 gedrückt

	//Schleife
	while (1)
	{
		
		timerticks = get_ticks();
		if ((PINB & (1<<TASTER)) == 0)
			taster = 1;
		else
			{taster = 0;}
		t = (timerticks /2000000.);	//2 000 000
		//t = timerticks * 0.0000005;
		if (t >= 1.0)
			PORTD = 0xFF;
		// oder kurz
		// taster = !(PINB & (1<<TASTER));
		//durch ! auch eine 1 in eine Zahl
		//t ++;
		/*if (zustand == Z_RUN && t < 0.8)
			PORTB |= (1<<LED_RT);
		else 
			PORTB &= ~(1<<LED_RT);*/
		if (taster > taster_alt)
		{
			switch (zustand)	//Wenn Zustand != Z_STOPP dann ist Z_RUN
			{
				case Z_STOPP:
					t0 = t;
					PORTB &= ~(1<<LED_GN);
					PORTB &= ~(1<<LED_RT);
					zustand = Z_RUN;
				break;
				case Z_RUN:
					if (t - t0 > 2.2 || t-t0 < 1.8)
					{// schlecht 
						PORTB |= (1<<LED_RT);
						if (t-t0 < 1.8)
						PORTB |= (1<<LED_GN);
					}
					else
						PORTB |= (1<<LED_GN);
					zustand = Z_STOPP;
				break;
				default:;

			}
		}
		taster_alt = taster;
	}
}
