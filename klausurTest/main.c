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
	//Timer auswählen -> modus: Normal auf Timer 0
	// läuft bis 0xFF, wert steht in TCNT0 (T Timer - CNT CouNT - 0 Timer 0)
	//wgm: 0
	//TCCR0A und TCCR0B (T Timer - C Counter - C Control - R Register - 0 Timer 0 - A/B erstes/zweites ...)
	// CS (C Clock - S Select) | Bits des TTCRnB auch Flags genannt
	//Gewähler Vorteiler: 64 -> 3
	TCCR0A = TCCR0B = 0; //zur sicherheit/ init
	TCCR0B = TCCR0B | (1<<CS00) | (1<<CS01);
	//stumpf:
	TCCR0B = TCCR0B | 3;
	//einzelnes schieben
	TCCR0B |= (1<<0) | (1<<1);
	//schön:
	TCCR0B = TCCR0B | PS64;
	//kurz
	TCCR0B |= 3;
	//für kranke
	TCCR0B |= 0b00000011;

	//weitere Einstellungen im TIMSK0 (TImerMaSKregister0)
	//freischalten des Überlauf Interrupts
	TIMSK0 |= (1<<TOIE0); //Interruppt bei ovfl
	//OCIEnA/B OnCompareInteruptEnable

	//bei Normal wird meist ein Startwert benötigt (preoload-Wert)
	//soll 100 Takte bis überlauf laufen (256 - <Tick-Zahl>)
	//ovfl bei 255 -> start bei 156
	//254 - 255 - 0 INT -> TCNT = 156 - 157 - 158
	//das muss nach jedem überlauf gemacht werden:
	TCNT0 = 156;

	//RECHNEN
	//16 000 000 / 64 = 250 000 ticks des Timers / sek 16MHz -> 250 kHz | 1/f = T mit T = 4µs
	//Überlauf bei TCNT = 255+1 -> warten von 100 Ticks 
	//fT0/ 100 = 250 000 / 100 = 2 500 überläufe / sek 1/f = T = 400µs = 0,4ms je Überlauf

	//Ersan möchte genau 1ms erfassen können
	//volatile int preload_val = 256 - F_CPU / PRESCALER / F_OVF;
	//PRESCALER Vorteiler
	//F_OVF : Anzahl Überläufe / sek
	volatile int perLVal = 256 - F_CPU / 64 / 1000;	//= 6

	//timer initialisieren
    /* Replace with your application code */
    while (1) 
    {
		char str [42];
		sprintf (str, "%i", TCNT0);	// irgendwas zwischen 156-255 und 0
    }
}

