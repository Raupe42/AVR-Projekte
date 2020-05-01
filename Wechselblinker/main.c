/*
 * Wechselblinker.c
 *
 * Created: 27.09.2018 19:53:23
 * Author : jensh
 */ 

#define F_CPU 16000000		//DEFINES VOR Includes!!!!!

#include <avr/io.h>
#include <util/delay.h>



int main(void)
{
	DDRB = 0b00110000;
	int value = 5;
	int i;
    /* Replace with your application code */
    while (1)
    {
		PORTB = 0b00100000;
		for (i = 0; i < value; i++)
			_delay_ms(20);

		PORTB = 0b00010000;
		for (i = 0; i < value; i++)
			_delay_ms(20);

		if (PORTB)
		{
			value ++;
		}
    }
}

