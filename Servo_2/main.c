/*
 * Servo_2.c
Ein Servo wird über zwei Taster (TL/TR) Links/Rechts-positioniert.
Pulserzeugung über Timer 1 im CTC-Modus 
 */ 
#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
//#include <util/delay.h>
#include "systemtime.h"
// verwendete Bits in Port B:
#define TL	0	// Taster Links in Port B
#define TR	2	// ...Rechts...
#define S1	1	// Servo 1 Puls

#define T_UP_MIN	1000	// Mikrosekunden
#define T_UP_MAX	2000	//   "
#define T_ZYC		20000	//   "
#define OCR_MIN		T_UP_MIN/4	// 4us pro Timertick
#define OCR_MAX		T_UP_MAX/4

// Zeit in Timerticks bis zum nächsten COMP-Interrupt:
volatile uint16_t t_up = T_UP_MIN/4;

ISR( TIMER1_COMPA_vect )            // Interrupt Compare Match Interrupt A
{

	if ( !(PINB & (1<<S1)) ){			// Pulsbeginn?
		OCR1A = t_up-1;					// ja.
		PORTB = PORTB | (1<<S1);		// Puls nach oben.
	}else{
		OCR1A = T_ZYC/4 - t_up-1;		// nein. Übrige Zeit Puls unten.
		PORTB = PORTB & ~(1<<S1);
	}
	
}

int main (void)
{
	double t_sec = 0.;
	double t_next = 0.025;
	PORTB = PORTB | (1<<TL) | (1<<TR);	// Pullups für Taster
	DDRB = DDRB | (1<<S1);				// Treiber für Servo-Puls
	TCCR1B = (1<<WGM12) | (1<<CS11)| (1<<CS10); // CTC-Mode, Prescaler 64 
										// ergibt 4us pro Timertick
	TIMSK1  = (1<<OCIE1A);              // Timer-Compare Interrupt an
	OCR1A = t_up;
	sei();                              // Interrupts global an
	t2_init();
	double tmp = 0.01;
	while( 1 ) {
		
		t_sec = get_ticks() * SEC;
		if (t_sec >= tmp)
		{
			tmp += 0.01;
		}
		
		if (( !(PINB & (1<<TL)) ) && t_next < t_sec) {		// ggfs. Impuls-Zeit verlängern
			if ( t_up <= OCR_MAX-3 ){
				t_up = t_up + 3;			
				//_delay_ms(25);			// nicht zu schnell aktualisieren
				t_next = t_sec + 0.025;
			}
		}
		if (( !(PINB & (1<<TR)) ) && t_next < t_sec){		// ggfs. Impuls-Zeit verkürzen
			if ( t_up >= OCR_MIN+3 ){
				t_up = t_up - 3;
				//_delay_ms(25);
				t_next = t_sec + 0.025;
			}
		}
	}
	
}
