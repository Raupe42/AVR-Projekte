/*
 * Tod_Spiel.c
 *
 * Created: 31.01.2019 19:40:52
 * Author : jensh
 Realisierung als endlicher Automat
 */ 

#include <avr/io.h>

#define TASTER 2	//PINB
#define LED_RT 5		//PORTB
#define LED_GN 4
#define Z_STOPP 1
#define Z_RUN 2
int main(void)
{
uint8_t zustand = 0, taster = 0, taster_alt = 0;
double t = 0.0;

	//Hardware vorbereiten
	DDRB = 0 | (1<<LED_RT) | (1<<LED_GN);		//1 für Ausgang
	PORTB = (1<<TASTER); //PULLUP

	zustand = Z_STOPP;	//1 Stopp, 2 Run
	taster_alt =0;	//1 gedrückt 

    //Schleife
    while (1) 
    {
		if ((PINB & (1<<TASTER)) == 0)
			taster = 1;
		else
			taster = 0;

		// oder kurz
		// taster = !(PINB & (1<<TASTER));
		//durch ! auch eine 1 in eine Zahl
		t ++;
		if (taster > taster_alt)
		{
		switch (zustand)	//Wenn Zustand != Z_STOPP dann ist Z_RUN
		{
			case 1:
			t = 0;
			PORTB &= ~(1<<)
			break;
			case 2:
			break;
			default:;

		}
		}
    }
}

