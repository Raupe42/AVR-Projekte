
// Name:
// Klasse:
// Datum:

// Timer/Counter 0 erzeugt periodische Ereignisse, f=2000Hz
// Wer es h�ren m�chte: Buzzer an Port PD7 (gegen Masse)

/* Auftr�ge:
1. �berpr�fen Sie die Funktion des Programms mit Hilfe des Debuggers -> Tut
2. Erkl�ren Sie den Begriff Vorladewert.
-> Wert der bei �berlauf des TCNT0 ( 0 wegen 328p) in der ISR geladen wird 
	(durch die Zeile TCNT0 = PRELOAD)
Ist hier dynamisch abh�ngig von F_CPU und Prescale sowie 
dem Wert F_OVF, hier 0,5�s also lambda halbe
3. Beim Saxophon erklingt beim Dr�cken der Oktav-Klappe ein genau
   doppelt so hoher Ton. Erweitern Sie das Programm um eine Oktav-Klappe
   -> siehe unten
   Achtung: Die Berechnung mittels PRELOAD/2 funktioiniert nur, weil der Precompiler den definierten text einsetzt
   So wird in wirklichkeit der F_OVF / 2 geteilt.
   So ergibt sich ein gr��erer Preload und damit eine k�rzere Periodendauer.
*/

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>



#define F_OVF		2000	//anzhal der Aufrufe der ISR
#define	PRESCALER	64
#define P_CODE_64	(1<<CS00)| (1<<CS01)

#define TASTER 2

// Vorladewert
#define PRELOAD		256 - F_CPU / PRESCALER / F_OVF
// 250 000 Timerticks
//125 Tiks f�r 500�s
//Vorladewert ist 256 - 125 = 131

//int preload_val = 256 - F_CPU / PRESCALER / F_OVF;
volatile int preload_val = 256 - F_CPU / PRESCALER / F_OVF; //volatile ist hier wichtig!!
ISR(TIMER0_OVF_vect){

	TCNT0 = preload_val;
	PORTD ^= (1<<7);		// Bit 7 toggeln �ber XOR
}

int main(void){

	DDRD |= (1<<7);			// Treiber f�r Buzzer an
	PORTD |= (1<<TASTER);
	TCNT0 = PRELOAD;
	TCCR0B = P_CODE_64;		// Vorteiler
	TIMSK0 |= (1<<TOIE0);	// �berlauf Interrupt freigeben
	sei();					// Interrupts global freigeben
	
	while (1){
		if (!(PIND & (1<<TASTER)))
			preload_val = PRELOAD / 2;
		else
			preload_val = PRELOAD;

	}
}
