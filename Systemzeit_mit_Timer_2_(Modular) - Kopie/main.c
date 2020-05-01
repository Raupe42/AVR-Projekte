/*

Demo für den hochauflösenden 64-Bit-Systemzeitgeber auf dem ATmega328P
Beispiel für Modularisierung: Der Quellcode für den Systemzeitgeber
wurde in die Datei systemtime.h ausgelagert

*/

// Bisher Spitzklammern: Suchpfad gleich Standard-Installationspfad
#include <AVR/io.h>

// Neu: Anführungsstriche statt Spitzklammern, d.h. Suchpfad gleich Projektordner
#include <stdio.h>
#include "systemtime.h"
#include "uart.h"

#define F_CPU 16000000
#define BUS_TAKT_BAUD_RATE_DES_UART_KANN_HEISSEN_WIE_ES_WILL 115200

int main( void )
{
	uint64_t timerticks=0;			// Zeit in Timerticks seit Bootvorgang
	double t=0;						// Zeit in Sekunden
	//neu
	double tAlt = 0;
	char s [42];

	t2_init();						// siehe systemtime.h

	//UART init
	uart_init(UART_BAUD_SELECT(BUS_TAKT_BAUD_RATE_DES_UART_KANN_HEISSEN_WIE_ES_WILL, F_CPU));
	sei();

	while(1){
		timerticks = get_ticks();	// Angabe in Vielfachen von 0.5us
		t = get_ticks()*SEC;		// Angabe in Sekunden, SEC siehe systemtime.h
		

		//einmal pro sekunde zeit senden

		//je Sekunde einen Aufruf durchführen
		if (t >= tAlt + 1.)
		{
			sprintf(s, "%lf", t);
			uart_puts(s);
			//tAlt = t; //wird UNGENAUER über Zeit
			tAlt ++;
		}

	}
}

