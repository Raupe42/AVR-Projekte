/*
 * Author : jensh
 */ 


#define F_CPU 16000000
#include <avr/io.h>

volatile int bloederWert = 0;

ISR (INT0_vect)
{
	PORTD ^= (1<<7);
	bloederWert += 1;
}

int main(void)
{
   DDRD = 0xFF;
   DDRD &= ~(1<<2);
   PORTD |= (1<<2);

   EICRA |= (1<<ISC01);
   EIMSK |= (1<<INT0);

    while (1) 
    {
		//Anwendung: INT0 für einen  ext. Taster zeit erfassen -> fallende Flanke Input
		//INT0 an PD2
		initI0fallend();
    }
}

void initI0aenderung ()
{
	EICRA |= (1<<ISC00);
	EICRA &= ~(1<<ISC01);
}
void initI0fallend ()
{
	EICRA &= ~(1<<ISC00);
	EICRA |= (1<<ISC01);
}
void initI0steigend ()
{
	EICRA |= (1<<ISC00);
	EICRA |= (1<<ISC01);
}