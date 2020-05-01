/*
 * ErsterInput.c
 *
 * Created: 15.11.2018 20:09:11
 * Author : Raupe
 */ 

#include <avr/io.h>


int main(void)
{
	DDRB = ~0 & ~(1);
	PORTB = 0x08;	//PORT Output und PullUp
					//PIN Input
					//damit ist der 3. (0..7) Pin mit PullUp versehen
    while (1) 
    {
		if (PINB &(1<<3) == 0)	//außer bit 3 (0..7)ALLES ausmaskiert, an besagter Stelle transparent
								//durch PullUp invertierte Logik
		{
			PORTB = 0xFF;
		}
		else
			PORTB = 0;
    }
}

