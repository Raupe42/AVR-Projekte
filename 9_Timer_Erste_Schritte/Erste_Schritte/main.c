
// Name:
// Klasse: 
// Datum:  

// Auftrag: Timer/Counter 0 soll laufen

#include <avr/io.h>
#include <avr/interrupt.h>
/*
Zahlenwert für das Register
0 none/ timer steht
1 vorteiler/ presacaler 1
2 vort. 8
3 vort. 64
4 vort. 256
5 vort. 1024

bei überlauf 
TOV0 == 1
*/

//ISR = interrupt service routine
ISR (TIMER0_OVF_vect)	//OVF = Overflow interrupt des Timer0
{
	
}

int main(void)
{

	TCCR0B = 2;	//Vorteiler wählen
	TIMSK0 |= (1<<TOIE0);	//enable interrupts at Timer(TI) 0 im Maskenregister (MSK)
	//individuelle interruptfreigabe

	sei();	//set enable interrupts
	//globale interrupt freigabe



	while ( 1 )
	{

	}
}



