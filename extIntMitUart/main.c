
// Lichtschranke, Zeitmessung, Ausgabe an LCD

#define F_CPU	16000000
#define BAUD 115200
#include <stdio.h>
#include <avr/io.h>
#include "lcd.h"
#include "systemtime.h"
#include "uart.h"
#define LT_IN 0
#define INT_IN 2
volatile int counter;
volatile uint64_t t_high, t_low;
ISR (INT0_vect)
{
	counter++;
	if (!(PIND & (1<<INT_IN))	)
		t_high = get_ticks();
	else
		t_low = get_ticks();
}

double betrag (double in)
{
	if (in < 0)
		return (in*-1.0);
	return in;
}

int main(void){
	counter = 0;
	DDRB = DDRB | (1<<2);
	uint8_t ltFp, ltFn, state, in;
	uint64_t start, stop;
	double outTimer = 0.0;
	char str [40];
	int counter_Alt = 0;
	//externe Interrupts
	EIMSK |= (1<<0);
	EICRA |= (1<<ISC00);
	//end

	sei();

	//UART
	uart_init(UART_BAUD_SELECT(BAUD, F_CPU));
	//END_UART


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
		//UART Kommunikation
		if (counter_Alt < counter)
		{
				sprintf (str, "Ein: %g\nAus:%g\nDelta: %g\n\n", (t_high * SEC), (t_low * SEC), (betrag (SEC * t_high - SEC * t_low)));
			uart_puts(str);
			counter_Alt = counter;
		}
		//END_UART Kom


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
				sprintf(str, "%i Zeit: %g", counter, ((stop - start) * SEC));
			else
				sprintf(str, "%i Zeit: %g", counter, ((get_ticks()-start) * SEC));
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

