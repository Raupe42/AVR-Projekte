

// Demo für HD44780-kompatibles LC-Display
// Frank Simon 
// 22.11.2014

#define F_CPU	16000000
#include <stdio.h>
#include <avr/io.h>
#include "lcd.h"

int main(void){

	lcd_init();

	// Simplen Text direkt ausgeben:
	lcd_pos(0,3);
	lcd_text( (char *)"Hallo Welt!" );

	// Formatierte Ausgabe:
    char s[20];
  	//uint32_t n = 123456;
	uint32_t n = 1;
	while(1){
		lcd_pos(1,0);
		// Erst in einen Textpuffer schreiben:
		sprintf( s, "Konto: %lu", n );
	    // Dann den Puffer ausgeben:
		lcd_text( s );
		_delay_ms( 100 );
		n++;
	}
}

