/*

Servo_1.c, ein simples Demo-Programm für Modellbauservos

Aufträge, Fragen:
1.	Analysieren Sie den Programmablauf, indem Sie in main() geeignete Zeilenkommentare 
	einfügen (10 min)
2.	Skizzieren Sie den Signalverlauf, der durch diesen Programmablauf erzeugt wird.
3.  Testen Sie Ihr Servo mit diesem Programm.
4.	Fernziel: Servos mit Timern steuern. Sammeln Sie Ideen dazu.


Bei Modellbauservo:
1 von 20 ms : 0°
2 von 20 ms : 180°

 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>

#define S1_PORT		PORTB
#define S1_DDR		DDRB
#define S1_BIT		4

int main (void)
{
	S1_DDR = S1_DDR | (1<<S1_BIT);

	while( 1 ) {
	//basis 20ms
		for(uint16_t i = 0; i<50; i++)	//20 * 50 = 1 s 
		{	S1_PORT |= (1<<S1_BIT);
			_delay_us( 1000 );	//	0°
			S1_PORT &= ~(1<<S1_BIT);
			_delay_ms( 19 );
		}

		for(uint16_t i = 0; i<50; i++)	//10%
		{	S1_PORT |= (1<<S1_BIT);
			_delay_us( 1500 );		//180°
			S1_PORT &= ~(1<<S1_BIT);
			_delay_ms( 18.5 );
		}
	
	}
}
