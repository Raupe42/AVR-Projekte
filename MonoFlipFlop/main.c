/*
 * MonoFlipFlop.c
 *
 * Created: 02.05.2020 14:02:30
 * Author : jensh
 a) Klären Sie die Begriffe Monoflop und nachtriggerbar, zum Beispiel hier:
 https://de.wikibooks.org/wiki/Digitale_Schaltungstechnik/_Monoflop/_Allgemein
 oder im beiliegenden Dokument.
 b) Der Kunde verlangt ein negativ-flankengetriggertes nachtriggerbares Monoflop. Das Eingangsignal soll den 
 externen Interrupt INT0 auslösen und darüber den Ausgang PB0 aktivieren. PB0 soll immer mindestens 3 Sekunden lang aktiv bleiben.
 c) Zusatzaufgabe: Über die folgende Zeile
 #define NACHTRIGGERBAR 1 // bzw. 0 für nicht-nachtriggerbar
 soll das Programm für beide Anwendungen leicht anpassbar erstellt werden.

 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#define SEC 1/10000
#define NACHTRIGGERBAR 1

volatile uint64_t t_Ints = 0;	//je 0,1ms +=1
volatile double tStart = 0.;

ISR (TIMER0_COMPA_vect)
{
	t_Ints ++;
}

ISR (INT0_vect)
{
	if (NACHTRIGGERBAR == 0)
		if (tStart == 0.0)
			tStart = getTime();
	else 
		tStart = getTime();
	PORTB |= (1<<PORTB0);
}

int main(void)
{
	//Timer 0
	TCCR0A |= (1<<WGM01);	//CTC-Mode
	TCCR0B |= 2;			//CS von 8
	OCR0A = 200-1;			// 200 schritte
	// 16M / 8 / 200 = 2M / 200 = 10 000 Events/ sek
	TIMSK0 |= OCIE0A;		//Interrupt bei TCNT0 == OCR0A

	//INT0
	EIMSK |= (1<<INT0);
	EICRA |= (1<<ISC01);


	sei();

	//Init IOs
	DDRB |= (1<<DDB0);	//Treiber Ein PB0
	PORTD |= (1<<PORTD2);	//Input Pullup aktiv


    while (1) 
    {
		if (tStart > 0)
		{
			if (tStart + 3. <= getTime())
			{
				PORTB &= ~(1<<PORTB0);
				tStart = 0;
			}
		}
    }
}

double getTime ()
{
	double time;
	return t_Ints * SEC;
}