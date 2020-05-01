/*
 * ctc.c
 *
 * Created: 01.05.2020 13:56:16
 *  Author: jensh
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>

// Clock Select, Prescaler
#define PS1			1
#define PS8			2
#define PS64		3
#define PS256		4
#define PS1024		5

volatile int t_ms = 0;

void ISR (TIMER0_COMPA_vect)
{
	t_ms++;
}

int main(void)
{
   TCCR0B |= PS64;	//250 tiks je ms
   TCCR0A |= (1<<WGM01); //CTC Modus
   OCR0A = 249; //Comparematch bei 249 -> Event je ms
   TIMSK0 |= (1<<OCIE0A); //Interrupt des OCR aktiv
   sei();
   DDRD = ~0;

    while (1) 
    {
		if (!(t_ms % 80))
			PORTD ^= (1 << 3);
		if (!(t_ms % 100))		//ist 0
			PORTD ^= (1 << 7);
		
		if (t_ms % 150 == 0)
		{
			if (PORTD & (1<<4))
				PORTD = PORTD & ~(1<<4);
			else
				PORTD |= (1<<4);
				
		}
    }
}

