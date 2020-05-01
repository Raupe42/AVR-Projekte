/*
 * MehrLed.c
 *
 * Created: 04.10.2018 19:41:33
 * Author : jensh
 */ 
#define F_CPU 16000000
#define SIMULATION 0

#define SIM (!SIMULATION)

#include <avr/io.h>
#include <util/delay.h>

void laufen ();
void rueckwaerts ();
void blinken ();
void blinken2 (int mode);
void blinken3 ();
void blinken4 ();


int main(void)
{
int i;

	//set DDR
	//DDRB = (1<<5) | (1<<4);		//  | Bitweise ODER		//Treiber aktivieren
	DDRB = 0xFF;
    /* Replace with your application code */
    while (1) 
    {
		/*PORTB = (1<<5);
		PORTB = PORTB | (1<<4);

		PORTB = PORTB & ~(1<<5);
*/
		//laufen();
		//rueckwaerts();
		//blinken ();

		/*for (i = 0; i < 3; i++)
		{
			blinken2 (i);
			if SIM
			_delay_ms (200);
		}
		*/
		blinken4 ();
		if SIM
			_delay_ms (200);
    }
}

void laufen ()
{
	if (PORTB == 0 || PORTB == 32)
		{
			//PORTB = (1<<4);
			PORTB = 1;
		}
	else 
		PORTB = (PORTB << 1);
}

void rueckwaerts()
{	// PORTB == 0 ODER PORTB == 1 
	if (PORTB == 0 || PORTB == 1)	
		//PORTB = (1<<4);
		PORTB = 32;
	else
		PORTB = PORTB >> 1;
}

void blinken ()
{
	if (PORTB == 0)
		PORTB = 0b00111111;
	else
		PORTB = 0;
}

void blinken2 (int mode)
{
	int i, n = 0;

	switch (mode) {
		case 0: PORTB = 0; break;
		case 1: i = 0;
			for (i = 0; i < 2000; i ++)
			{
				blinken();
				for (n = 0; n < i / 100; n++)
				{
					_delay_ms (10);
				}
				
			} break;
		case 2: PORTB = 0b00111111; break;
	}

}

void blinken3 ()
{
	if (PORTB == 0 || PORTB > 0b00111111)
		PORTB = 1;
	PORTB ++;
	
}

void blinken4 ()
{
	int tmp;
	if (PORTB == 0 || PORTB == 0b00001100)
		PORTB = 1+ 32;

	else {
		tmp = (PORTB & 0b00000111) % 4;
		PORTB -= tmp;
		PORTB = PORTB / 2;
		PORTB += (tmp<<1);

	}
	//PORTB = ~0; 

}