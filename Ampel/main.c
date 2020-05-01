/*
 * ampel.c
 *
 *Eine Ampel mit Fu�g�ngerampel mit Bedarfsschaltung
 *
 * Created: 15.11.2018 20:09:11
 * Author : Raupe
 */ 
#define F_CPU 16000000
#include <avr/io.h>
//#include <util/delay.h>

#define GRUNDTAKT 500000


#define AUTO_ROT 2
#define AUTO_GELB 1
#define AUTO_GRUEN 0
#define LEUTE_ROT 5
#define LEUTE_GRUEN 4
#define GRUEN_KOMMT 3
#define TASTER 2

int state = 0;

int main(void)
{
	DDRB = ~0;
	DDRD = 0;
	PORTD = (1<<TASTER);	//PORT Output und PullUp
					//PIN Input
					//damit ist der 0. (0..7) Pin mit PullUp versehen
	int anforderung = 0;
	long zaehler = 0;
	while (1) 
    {
		if ((PIND &(1<<TASTER)) == 0)	//au�er bit 3 (0..7)ALLES //ausmaskiert, an besagter Stelle transparent
								//durch PullUp invertierte Logik
		{
			//PORTB = PIND;
			anforderung = 1;
			//_delay_ms (500);
		}
		/*if (anforderung && (zaehler%100000 <= 50000))
		{
			if ((PORTB &(1<<GRUEN_KOMMT)) == 0)
				PORTB |= (1<<GRUEN_KOMMT);
		}
		else
		{
			if ((PORTB &(1<<GRUEN_KOMMT)) != 0)
				PORTB &= ~(1<<GRUEN_KOMMT);
		}*/
		if (anforderung)
			PORTB |= (1<<GRUEN_KOMMT);
		else
			PORTB &= ~(1<<GRUEN_KOMMT);
		switch (state)
		{
			case 1 : //auto gr�n
			PORTB |= (1 << AUTO_GRUEN);	//an
			PORTB &= ~(1 << AUTO_GELB);	//aus
			PORTB &= ~(1 << AUTO_ROT);	//aus
			PORTB &= ~(1 << LEUTE_GRUEN);//aus
			PORTB |= (1 << LEUTE_ROT);	//an
			break;
			case 2:	// Auto Gelb 
			PORTB &= ~(1 << AUTO_GRUEN);
			PORTB |= (1 << AUTO_GELB);
			PORTB &= ~(1 << AUTO_ROT);
			PORTB &= ~(1 << LEUTE_GRUEN);
			PORTB |= (1 << LEUTE_ROT);
			break;
			case 3:	// Auto Rot
			PORTB &= ~(1 << AUTO_GRUEN);
			PORTB &= ~(1 << AUTO_GELB);
			PORTB |= (1 << AUTO_ROT);
			PORTB &= ~(1 << LEUTE_GRUEN);
			PORTB |= (1 << LEUTE_ROT);
			break;
			case 4:	// Auto Gelb + Rot
			PORTB &= ~(1 << AUTO_GRUEN);
			PORTB |= (1 << AUTO_GELB);
			PORTB |= (1 << AUTO_ROT);
			PORTB &= ~(1 << LEUTE_GRUEN);
			PORTB |= (1 << LEUTE_ROT);
			break;
			case 5:	// Auto Rot, Leute Grün
			PORTB &= ~(1 << AUTO_GRUEN);
			PORTB &= ~(1 << AUTO_GELB);
			PORTB |= (1 << AUTO_ROT);
			PORTB |= (1 << LEUTE_GRUEN);
			PORTB &= ~(1 << LEUTE_ROT);
			
			break;
			default:;
		}
		zaehler++;
		if (zaehler >= GRUNDTAKT)
		{
			zaehler = 0;
			if (state < 4)
			{
				if (anforderung && state == 3)
				{
					state = 5;
					anforderung = 0;
				}
				else
					state++;

			}
			else if (state == 5)	//Ampel erst wieder alle Rot dann norma weiter
			{
				state = 3;
			}
			else
				state = 1;
		}
		
	}
}

