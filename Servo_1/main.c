/*

Datum: 16.5.2019

Servo_1.c, ein simples Demo-Programm für Modellbauservos

Aufträge, Fragen:
1.	Analysieren Sie den Programmablauf, indem Sie in main() geeignete Zeilenkommentare 
	einfügen (10 min)
2.	Skizzieren Sie den Signalverlauf, der durch diesen Programmablauf erzeugt wird.
3.  Testen Sie Ihr Servo mit diesem Programm. Braun: 0V, Rot: 5V, Orange: PWM
4.	Fernziel: Servos mit Timern steuern. Sammeln Sie Ideen dazu.
Ideen:
-- Betriebsmodus eines Timers: CTC (Clear Timer on Compare match)
-- Timer in passender Breite mit passendem Vorteiler wählen
-- Interrupt aktivieren
-- OCR (Output Compare Register) legt die Zeit bis zum nächsten Interrupt fest
-- OCR wird so geändert, dass der Compare match erst kurz, dann lang dauert 
-- in der ISR: Merker (oder Ausgang) testen, dann Ausgang toggeln und OCR anpassen.

 */ 

#define F_CPU 16000000UL

#include <avr/io.h>
//#include <util/delay.h>
#include <avr/interrupt.h>

#define S1_PORT		PORTD
#define S1_DDR		DDRD
#define S1_BIT		6

// Clock Select, Prescaler
#define PS1			1
#define PS8			2
#define PS64		3
#define PS256		4
#define PS1024		5

volatile uint32_t t_ms = 0;
uint16_t einInUs;
volatile uint8_t mode = 0;

ISR (TIMER1_COMPA_vect)
{
	uint16_t einT, ausT;
	einT = einInUs;
	ausT = 20000 - einInUs;
/*
Mit Timer 1 stehen 16 bit max OCR Value zur Verfügung
Bei Vorteiler 64: 250 tiks je ms
*/	
	switch (mode)
	{
	case 0:	//Ein-Zeit
	OCR1A = einT / 4;
	mode = 1;
	S1_PORT |= (1<<S1_BIT);
	break;
	case 1:
	OCR1A = ausT / 4;
	mode = 0;
	S1_PORT &= ~(1<<S1_BIT);
	break;
	default: ;
	}
	//OCR1A --;
}

ISR (TIMER0_COMPA_vect)
{
	t_ms++;
}

void initTimer1 ()
{
	TCCR1B |= PS64; // 250 tiks je ms // erster Versuch
	TCCR1B |= (1 << WGM12); //Aktiviere CTC Modus
	OCR1A = 249; //Erste Ladewert -> ein Aufruf je ms
	TIMSK1 |= (1<<OCIE1A); //Interrupt des OCR aktiv

}

void initTimer0 ()
{
	TCCR0B |= PS64; // 250 tiks je ms // erster Versuch
	TCCR0A |= (1 << WGM01); //Aktiviere CTC Modus
	OCR0A = 249; //Erste Ladewert -> ein Aufruf je ms
	TIMSK0 |= (1<<OCIE0A); //Interrupt des OCR aktiv
	
}

int main (void)
{
	initTimer0();
	initTimer1 ();
	S1_DDR = S1_DDR | (1<<S1_BIT);
	sei();
	while( 1 ) {

		/*
		// 50 mal das Gleiche:
		for(uint16_t i = 0; i<50; i++)
		{	S1_PORT |= (1<<S1_BIT);
			_delay_us( 1000 );			// 1ms an
			S1_PORT &= ~(1<<S1_BIT);
			_delay_ms( 19 );			// 19ms aus
		}
		// Dies ergibt eine Folge von 50 Pulsen, 
		// f=50Hz, Pulsbreite 1ms, Gesamtdauer: 1s

		// 50 mal das Gleiche:
		for(uint16_t i = 0; i<50; i++)
		{	S1_PORT |= (1<<S1_BIT);
			_delay_us( 2000 );			// 2ms an
			S1_PORT &= ~(1<<S1_BIT);
			_delay_ms( 18 );			// 18ms aus
		}
		// Dies ergibt eine Folge von 50 Pulsen, 
		// f=50Hz, Pulsbreite 2ms, Gesamtdauer: 1s
	*/
	if (t_ms < 1000)
	{
		einInUs = 1000;
	}
	else if (t_ms < 2000)
		einInUs = 2000;
	if (t_ms > 2000)
		t_ms = 0;	
	// Das Servo ist eine Sekunde lang auf Linksanschlag,
	// dann eine Sekunde auf Rechtsanschlag.

	}
}
