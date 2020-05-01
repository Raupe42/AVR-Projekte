/*
 * klausurTest.c
 *
 * Created: 01.05.2020 11:49:16
 * Author : jensh
 */ 


#define PS64 3
#define  F_CPU 16000000
#include <avr/io.h>


int main(void)
{ 
	//Timer ausw�hlen -> modus: Normal auf Timer 0
	// l�uft bis 0xFF, wert steht in TCNT0 (T Timer - CNT CouNT - 0 Timer 0)
	//wgm: 0
	//TCCR0A und TCCR0B (T Timer - C Counter - C Control - R Register - 0 Timer 0 - A/B erstes/zweites ...)
	// CS (C Clock - S Select) | Bits des TTCRnB auch Flags genannt
	//Gew�hler Vorteiler: 64 -> 3
	TCCR0A = TCCR0B = 0; //zur sicherheit/ init
	TCCR0B = TCCR0B | (1<<CS00) | (1<<CS01);
	//stumpf:
	TCCR0B = TCCR0B | 3;
	//einzelnes schieben
	TCCR0B |= (1<<0) | (1<<1);
	//sch�n:
	TCCR0B = TCCR0B | PS64;
	//kurz
	TCCR0B |= 3;
	//f�r kranke
	TCCR0B |= 0b00000011;

	//weitere Einstellungen im TIMSK0 (TImerMaSKregister0)
	//freischalten des �berlauf Interrupts
	TIMSK0 |= (1<<TOIE0); //Interruppt bei ovfl
	//OCIEnA/B OnCompareInteruptEnable

	//bei Normal wird meist ein Startwert ben�tigt (preoload-Wert)
	//soll 100 Takte bis �berlauf laufen (256 - <Tick-Zahl>)
	//ovfl bei 255 -> start bei 156
	//254 - 255 - 0 INT -> TCNT = 156 - 157 - 158
	//das muss nach jedem �berlauf gemacht werden:
	TCNT0 = 156;

	//RECHNEN
	//16 000 000 / 64 = 250 000 ticks des Timers / sek 16MHz -> 250 kHz | 1/f = T mit T = 4�s
	//�berlauf bei TCNT = 255+1 -> warten von 100 Ticks 
	//fT0/ 100 = 250 000 / 100 = 2 500 �berl�ufe / sek 1/f = T = 400�s = 0,4ms je �berlauf

	//Ersan m�chte genau 1ms erfassen k�nnen
	//volatile int preload_val = 256 - F_CPU / PRESCALER / F_OVF;
	//PRESCALER Vorteiler
	//F_OVF : Anzahl �berl�ufe / sek
	volatile int perLVal = 256 - F_CPU / 64 / 1000;	//= 6

	//timer initialisieren
    /* Replace with your application code */
    while (1) 
    {
		char str [42];
		sprintf (str, "%i", TCNT0);	// irgendwas zwischen 156-255 und 0
    }
}

