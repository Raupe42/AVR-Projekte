/*

Demo f�r den hochaufl�senden 64-Bit-Systemzeitgeber auf dem ATmega328P
Beispiel f�r Modularisierung: Der Quellcode f�r den Systemzeitgeber
wurde in die Datei systemtime.h ausgelagert

*/

// Bisher Spitzklammern: Suchpfad gleich Standard-Installationspfad
#include <AVR/io.h>

// Neu: Anf�hrungsstriche statt Spitzklammern, d.h. Suchpfad gleich Projektordner
#include "systemtime.h"

int main( void )
{
	uint64_t timerticks=0;			// Zeit in Timerticks seit Bootvorgang
	double t=0;						// Zeit in Sekunden

	t2_init();						// siehe systemtime.h

	while(1){
		timerticks = get_ticks();	// Angabe in Vielfachen von 0.5us
		t = get_ticks()*SEC;		// Angabe in Sekunden, SEC siehe systemtime.h
		
		if (timerticks >= 2000)		// sinnloses Demo f�r die Zeit in Timerticks
		{							// Vorteil: weniger Prozessorlast wg. Integer 
			DDRB = 1;
		}

		if (t>=0.02)				// sinnloses Demo f�r die Zeit in Sekunden
		{							// Vorteil: leicht lesbares Programm.
			DDRB = 2;
		}

	}
}

