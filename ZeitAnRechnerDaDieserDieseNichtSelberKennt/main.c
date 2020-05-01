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

int main( void )
{
	uart_init ( UART_BAUD_SELECT( UART_BAUD_RATE, F_CPU));
	sei ();

	/*
	sprintf( s, "Hallo Welt, die %d.\n", i++);
	uart_puts(s);
	_delay_ms(250);
	*/

	uint64_t timerticks=0;			// Zeit in Timerticks seit Bootvorgang
	double t=0, tAlt = 0;						// Zeit in Sekunden
	char str [80];
	t2_init();						// siehe systemtime.h

	while(1){
		timerticks = get_ticks();	// Angabe in Vielfachen von 0.5us
		t = (get_ticks()*SEC);		// Angabe in Sekunden, SEC siehe systemtime.h
		
		if (timerticks >= 2000)		// sinnloses Demo für die Zeit in Timerticks
		{							// Vorteil: weniger Prozessorlast wg. Integer 
			DDRB = 1;
		}

		if (t>=0.02)				// sinnloses Demo für die Zeit in Sekunden
		{							// Vorteil: leicht lesbares Programm.
			DDRB = 2;
		}

		if (t > tAlt+1.)
		{
			sprintf (str, "Zeit: %F\n", t);
			uart_puts(str);
			tAlt ++;

		}

	}
}

