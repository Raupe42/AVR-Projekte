/*
* Demo für Multitasking, 2 Blinker:
* Rot soll 5 mal pro Sekunde blinken, Gelb soll 6 mal pro Sekunde blinken
*/

#define F_CPU 16000000UL	//Quarzfrequenz
#include <avr/io.h>
#include <util/delay.h>

#define ROT		2		// Hardware wird symbolisch beschrieben
#define GELB	3

uint32_t t=0, t1 = 0;

int main(void)
{
	DDRB = (1<<ROT) | (1<<GELB);

	while (1)
	{
		_delay_ms(1);
		t++;

		// 1. Blinker:  5 Hz
		//if ((t % 100) == 0)
		if (t - t1 >= 100)
		{
			/*if (PORTB |~(1<<ROT) != 0)
			PORTB &= ~(1<<ROT);
			else
			PORTB |= (1<<ROT);
			*/
			PORTB ^= (1<<ROT);	//Bitweise Toggeln
			t1 = t;
		}
		

		// 2. Blinker:  6 Hz
		//if ((t % 83) == 0)	//kann Probleme bei ungleichmäßigem zählen erzeugen
		if (t % 166 < 83)
		//PORTB ^= (1<<GELB);
		PORTB &= ~(1<<GELB);
		else
		PORTB |= (1<<GELB);
		
	}
}

