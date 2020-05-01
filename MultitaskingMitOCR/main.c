/*
 * MultitaskingMitOCR.c
 *
 * Created: 11.04.2019 19:45:06
 * Author : jensh
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

volatile uint16_t t_ms = 0;

ISR (TIMER0_COMPA_vect)
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
		if (!(t_ms % 100))
			PORTD ^= (1 << 7);
    }
}

