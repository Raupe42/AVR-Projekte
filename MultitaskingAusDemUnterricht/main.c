/*
 * Demo für Multitasking, 2 Blinker:
 * Rot soll 5 mal pro Sekunde blinken, Gelb soll 6 mal pro Sekunde blinken
 */ 

#define F_CPU 16000000	//Quarzfrequenz
#include <avr/io.h>
#include <util/delay.h>

#define ROT		1		// Hardware wird symbolisch beschrieben
#define GELB	2
#define GRUEN	5
#define T		0

uint8_t t=254, t1=0, t2=0;	// Zeitticker

int main(void)
{
	DDRB = (1<<ROT) | (1<<GELB) | (1<<GRUEN);
	PORTB = PORTB | (1<<T);

    while (1) 
    {
		//_delay_ms(1);		// Fälschung einer Systemzeituhr
		t++;

		// 1. Blinker, ROT:  f=5Hz T=1/f=Periodendauer=0.2s=200ms
		if( t-t1 >= 100 ){				// t >= t1+100
			PORTB = PORTB ^ (1<<ROT);	// ^ ist XOR, toggelt
			t1 = t1 + 100;				// Zwischenzeit
		}


		// 2. Blinker, GELB:  6 Hz T=166ms
		if( t-t2 >= 83 ){
			PORTB = PORTB ^ (1<<GELB);	// ^ ist XOR, toggelt
			t2 = t2 + 83;
		}
	
		// 3. Fingermelder
		if( (PINB & (1<<T)) == 0 )
			PORTB = PORTB | (1<<GRUEN);
		else
			PORTB = PORTB &~(1<<GRUEN);

    }
}

/*		// geht auch
		if( t >= t1 + 100 ){
			PORTB = PORTB ^ (1<<ROT);	// ^ ist XOR, toggelt
			t1 = t1 + 100;				// Zwischenzeit
		}
*/