/*

Demo für den hochauflösenden 64-Bit-Systemzeitgeber auf dem ATmega328P
Beispiel für Modularisierung: Der Quellcode für den Systemzeitgeber
wurde in die Datei systemtime.h ausgelagert

*/

// Bisher Spitzklammern: Suchpfad gleich Standard-Installationspfad
#include <AVR/io.h>
#include <stdio.h>
#define F_CPU 16000000
#define UART_BAUD_RATE 115200

// Neu: Anführungsstriche statt Spitzklammern, d.h. Suchpfad gleich Projektordner
#include "systemtime.h"
#include "uart.h"

volatile char tobisVar = 0;
volatile int sollAusgegebenWerden = 0;
volatile double t=0, tStop = 0;

ISR (INT1_vect)
{		//zeitstempel setzen
	if (t == 0.0)//ausgangszustand
		t = get_ticks() * SEC; //Angabe in Sekunden, SEC siehe systemtime.h - Auflösung 500ns
	else if (t > 0.0 && tStop == 0)
	{
		tStop = get_ticks() * SEC;
		sollAusgegebenWerden = 1;
	}
	//für reset
	else if (t > 0.0 && tStop > t)
		t = tStop = 0.0;
}

int main( void )
{

	DDRD |= (1<<DDD5);
	PORTD |= (1<<PORTD3);	//Pullup für PIND3/INT1 setzen
	EIMSK |= (1<<INT1);
	EICRA |= (1<<ISC11);
	uart_init ( UART_BAUD_SELECT( UART_BAUD_RATE, F_CPU));
	t2_init();						// siehe systemtime.h
	sei ();
	
	while(1){
	//Idee delta T = tStop - t;
	//Wann soll die Ausgabe erfolgen? wenn zeit fertig gestoppt
	//Wann ist das?
	if (sollAusgegebenWerden == 1)
	{
		sprintf (str, "Zeit: %F\n", tStop - t);
		uart_puts(str);
		sollAusgegebenWerden = 0;
	}
}

