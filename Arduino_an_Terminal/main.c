/*

Demo Arduino an PC über USB:
Der PC verwendet einen virtuellen COM-Port, der uC den UART.
Hardware: arduino/funduino, ATMega328, 16 MHz

Aufträge:
1. Bringen Sie dieses Demos auf Ihrem uC in Gang. Klären Sie die Funktionsweise.
2. Weisen Sie die Funktion mit dem beiliegenden Terminal-
Programm HTerm.exe nach. (Starten, COM-Port wählen, Connect)
*/

#define F_CPU	16000000
#define UART_BAUD_RATE 115200

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "uart.h"

char s[20];				// In dieses Feld hineinschreiben
uint8_t i = 0;

int main(void) {

	uart_init( UART_BAUD_SELECT(UART_BAUD_RATE,F_CPU) );
	sei();
	
	while( 1 )
	{
		sprintf( s, "Hallo Welt, die %d.\n", i++);
		uart_puts(s);
		_delay_ms(250);
	}
}



/*
Folgende Einstellung ist nötig, damit ein Projekt printf nutzen kann:
Unter Projekt-Properies-Toolchain-Linker-Miscallaneous:
-Wl,-u,vfprintf

What the linker flags mean
---> -Wl // means that linker options follow with each linker option separated by a comma.
---> -u  // forces linking of library module that follows it.
---> vfprintf // function which is used by printf type functions. Used with the -u option
// forces the linker to link this function from the library which is necessary
// to use printf(), sprintf(), etc.
*/





