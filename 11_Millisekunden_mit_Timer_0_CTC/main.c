
// Name:	
// Klasse:	
// Datum:   

// Thema: Millisekunden mit Timer 0 im CTC-Modus

// Auftr�ge:
// 0.	Quelle suchen: Datenblatt ATmega328P, Timer0, CTC-Modus
	//CTC -> CountCompare/ Clear Timer on Compare Match
// 1.	Richten Sie Timer 0 im CTC-Modus so ein, dass eine globale Variable
//		namens t_ms jede Millisekunde im Interrupt erh�ht wird. Untersuchen 
//		Sie vorher rechnerisch, welche Vorteiler geeignet sind. Fertigen 
//		Sie daf�r eine entsprechende Tabelle an.
/*
f�r eine 1 ms Aufl�sung sollte am besten die 16 aus 16Mhz entfallen
bei PS = PS64 -> 250 000 inc/s
bedeutet 250 inc/ms

*/
// 2.	�berpr�fen Sie die Pr�zision des ms-Timers durch Messung im Debugger.
// 3.	Realisieren Sie damit im Hauptprogramm 2 asynchrone Blinker.

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>

//Timer 0 ist 8 bit breit


// Clock Select, Prescaler
#define PS1			1
#define PS8			2
#define PS64		3
#define PS256		4
#define PS1024		5

ISR(TIMER0_COMPA_vect)	//ausf�hrung wenn CMP wert erreicht
{

}


int main(void){
	TCCR0B |= 3;	//Vorteiler 64
	TCCR0A |= (1<<WGM01);// Setzen auf Modus CTC
	//WGM Waveform generator Mode  
	
	//Comparewert w�hlen, bei wechem Wert ein Interrupt erzeugt werden soll
	//anschlie�end wird neu gez�hlt
	OCR0A = 249; //100 als Beispiel ohen Bezug zur Aufgabe
	//Achtung: f�ngt bei 0 an zu z�hlen
	//hier als 250 Schritte 
	//249 eintragen 

	//Interrupt aktivieren
	TIMSK0 |= (1<<OCIE0A);	//Output compare interupt enable (timer) 0 A
	//global erlauben 
	sei();

	while ( 1 ){

	}
}

/* Checkliste f�r Timer:
1. Headerdatei f�r Interruptroutinen inkludieren
2. Bitbreite ermitteln (8/16)
3. Vorteiler w�hlen
4. Modus w�hlen (Normal/CTC/�)
5. Interrupt-Service-Routine deklarieren (OVF/COMPA/...)
6. Interrupt im Interrupt-Masken-Register aktivieren
7. Globale Freigabe f�r Interrupts ausf�hren
*/


