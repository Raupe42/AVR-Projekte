/*
 * ErsteSchritte.c
 *
 * Created: 06.09.2018 19:50:05
 * Author : Raupe
 */ 

#include <avr/io.h>

int8_t counter = 1;
uint16_t unsignedCounter = 12435;

int main(void)
{

	DDRB = 32;			//Data Direction Register
		//Port B als niederohmige Ausgänge

	//DDRB = 0xFF;	

    /* Replace with your application code */
    while (1) 
    {
	++ counter;
	//PORTB = 0b00000001;
	//PORTB = 0b00100000;
	//PORTB = counter;
	if (counter == 1)
	{
		//counter ++;
		PORTB = 32;
    }
	
}
}

