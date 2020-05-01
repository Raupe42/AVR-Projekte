/*
 * HardwareZuweisung.c
 *
 * Created: 11.10.2018 20:11:06
 * Author : jensh
 */ 
 #define F_CPU 16000000

 #define L1 4
 #define L2 5

#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB = ~0;
    /* Replace with your application code */
    while (1) 
    {
		PORTB = (1<<L1);
		#ifndef SIM
			_delay_ms(200);
		#endif
		PORTB = (1<<L2);
		#ifndef SIM
		_delay_ms(200);
		#endif
    }
}

