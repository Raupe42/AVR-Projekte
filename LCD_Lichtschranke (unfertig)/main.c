
// Lichtschranke, Zeitmessung, Ausgabe an LCD

#define F_CPU	16000000
#include <stdio.h>
#include <avr/io.h>
#include "lcd.h"
#include "systemtime.h"
#define LT_IN 0
int main(void){

	DDRB = DDRB | (1<<2);
	uint8_t ltFp, ltFn, state, in;
	uint64_t start, stop;
	double outTimer = 0.0;
	char str [40];
	//PORTC |= (1<<0);
	lcd_init();
	t2_init();

	// Simplen Text direkt ausgeben:
	lcd_pos(0,1);
	lcd_text( (char *)"Lichtschranke" );

	// Formatierte Ausgabe:

	while(1){
		//_delay_ms(200);
		//Flankenauswertung
		in = !(PINC & (1<<LT_IN));
		ltFp = in && !state;
		ltFn = !in && state;
		state = in;
		//END Flankenauswertung

		if ( !(PINC & (1<<LT_IN) ))
		{
			//lcd_pos(0,1);
			//lcd_text( (char *)"LS belegt" );
			PORTB = PORTB | (1<<2);
			}
		else
		{
		//lcd_pos(0,1);
			//lcd_text( (char *)"LS frei" );
			PORTB = PORTB &~(1<<2);
		}

		if (ltFp)
		{
			start = get_ticks();
		}
		if (ltFn)
			stop = get_ticks();
		

		if (outTimer < get_ticks()*SEC + 1)
		//if (ltFn)
		{
			lcd_pos (0,1);
			if (start < stop)
				sprintf(str, "Zeit: %g", ((stop - start) * SEC));
			else
				sprintf(str, "Zeit: %g", ((get_ticks()-start) * SEC));
			lcd_text(str);
			outTimer += 0.2;

			lcd_pos(1,1);
			if (in)				
				lcd_text ("LT belegt");
			else
				lcd_text ("LT Frei    ");
		}

	}
}

