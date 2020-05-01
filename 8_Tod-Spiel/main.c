
#define F_CPU 16000000	//Quarzfrequenz
#include <avr/io.h>
#include <util/delay.h>

#define Z_Stopp		1		// Hardware wird symbolisch beschrieben
#define Z_Run		2
#define Rot			3
#define Gruen		4
#define T			0

uint32_t t=0;	// Zeitticker

int main(void)
{
	DDRB = (1<<Z_Stopp) | (1<<Z_Run) | (1<<Rot) | (1<<Gruen);
	PORTB = PORTB | (1<<T);

    while (1) 
    {
		_delay_ms(1);		// Fälschung einer Systemzeituhr
		t++;

		// 1. Blinker, ROT:  f=5Hz T=1/f=Periodendauer=0.2s=200ms
		if( t-t1 >= 100 ){
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