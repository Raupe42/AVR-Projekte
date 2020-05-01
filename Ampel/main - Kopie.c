/*
 * ampel.c
 *
 *Eine Ampel mit Fu�g�ngerampel mit Bedarfsschaltung
 *
 * Created: 15.11.2018 20:09:11
 * Author : Raupe
 */ 

#include <avr/io.h>

#define AUTO_ROT 1
#define AUTO_GELB 2
#define AUTO_GRUEN 3
#define LEUTE_ROT 5
#define LEUTE_GRUEN 6
#define TASTER 0

int state = 0;

int main(void)
{
	DDRB = ~0 & ~(1);
	PORTB = 0x01;	//PORT Output und PullUp
					//PIN Input
					//damit ist der 0. (0..7) Pin mit PullUp versehen
	int anforderung = 0;
	long zaehler;
	while (1) 
    {
		if (PINB &(1<<TASTER) == 0)	//au�er bit 3 (0..7)ALLES ausmaskiert, an besagter Stelle transparent
								//durch PullUp invertierte Logik
		{
			//PORTB = 0xFF;
			anforderung = 1;
		}
		switch (state)
		{
			state 1 : //auto gr�n
			PORTB |= (1 << AUTO_GRUEN);
			PORTB &= (0 << AUTO_GELB);
			PORTB &= (0 << AUTO_ROT);
			PORTB &= (0 << LEUTE_GRUEN);
			PORTB |= (1 << LEUTE_ROT);
			break;
			state 2:	// Auto Gelb 
			PORTB &= (0 << AUTO_GRUEN);
			PORTB |= (1 << AUTO_GELB);
			PORTB &= (0 << AUTO_ROT);
			PORTB &= (0 << LEUTE_GRUEN);
			PORTB |= (1 << LEUTE_ROT);
			break;
			deafault:;
		}
		zaehler++;
		if (zaehler >= 100000)
		{
			zaehler = 0;
			if (state < 2)
				state++;
			else
				state = 1;
		}
	}
}

