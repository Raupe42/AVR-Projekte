
//	Treiber für LCD-Display mit Industriestandard-Befehlssatz
//	Frank Simon
//	15.11.2016

#include <util/delay.h>

// Daten-Richtungsregister für die 3 Steuersignale...
#define DDR_RS		DDRB	// Register Select
#define DDR_E		DDRB	// Enable
#define DDR_DAT		DDRD	// ...und den Datenbus

// Ebenso für die Ausgaberegister:
#define PORT_RS		PORTB
#define PORT_E		PORTB
#define PORT_DAT	PORTD

// Bitpositionen, alles ist erlaubt:
#define RS_		4
#define E_		3
// nur 4-Bit-Zugriffsbreite:
#define D4_		5
#define D5_		4
#define D6_		3
#define D7_		2


//------------   Low-Level-Routinen ---------------------------------

// RS-Signal kontrollieren:
void RS(uint8_t x)
{	if( x ) PORT_RS |= (1<<RS_);	// Daten-Register wählen
	else	PORT_RS &= ~(1<<RS_);	// Befehls-Register wählen
}

// Enable-Puls erzeugen:
void E_Pulse(void)
{	PORT_E |= (1<<E_);
	_delay_us( 1 );			// 1us
	PORT_E &= ~(1<<E_);		// Übernahme mit fallender Flanke
}

// Halb-Byte übertragen
static void lcd_nibble( uint8_t d )
{	// Das obere Nibble von d wird auf den 4-Bit-Bus abgebildet:
	if(d & (1<<4)) PORT_DAT |= (1<<D4_); else PORT_DAT &= ~(1<<D4_);
	if(d & (1<<5)) PORT_DAT |= (1<<D5_); else PORT_DAT &= ~(1<<D5_);
	if(d & (1<<6)) PORT_DAT |= (1<<D6_); else PORT_DAT &= ~(1<<D6_);
	if(d & (1<<7)) PORT_DAT |= (1<<D7_); else PORT_DAT &= ~(1<<D7_);
	E_Pulse();
}

// Befehls- oder Daten-Byte übertragen
static void lcd_byte( uint8_t d )
{	lcd_nibble( d );		// High nibble
	lcd_nibble( d<<4 );		// Low nibble	
	_delay_us( 45 );
}

// Befehls-Byte übertragen
void lcd_command( uint8_t d )
{	RS(0);			// Instruction-Register wählen
	lcd_byte( d );
	switch( d ){	// Display löschen und 
		case 1:		// Cursor home brauchen Zeit
		case 2:
		case 3: _delay_ms( 2 );
	}
	RS(1);			// Daten-Register wählen
}


//------------   High-Level-Routinen --------------------------------

// Controller und LC-Display initialisieren:
void lcd_init( void )
{	// Alle Steuersignale aus, dann Treiber aktivieren
	PORT_RS &= ~(1<<RS_);
	PORT_E &= ~(1<<E_);
	PORT_DAT &= ~((1<<D4_)|(1<<D5_)|(1<<D6_)|(1<<D7_));
	DDR_RS |= (1<<RS_);
	DDR_E |= (1<<E_);
	DDR_DAT |= ((1<<D4_)|(1<<D5_)|(1<<D6_)|(1<<D7_));
	_delay_ms(15);		// Ende des Power-On-Resets abwarten
	// Magische Abfolge, 8-Bit Mode, aber nur die 
	// obersten 4 Bit werden übertragen
	lcd_nibble( 0x30 );	_delay_us( 4100 );
	lcd_nibble( 0x30 );	_delay_us( 100 );
	lcd_nibble( 0x30 );	_delay_us( 100 );
	lcd_nibble( 0x20 );	_delay_us( 100 );	// 4-Bit-Modus
	lcd_command( 0x28);						// Function Set: 2-zeilig, Raster 5*8
	lcd_command( 0x08 );					// Display off
	lcd_command( 0x01 );					// Display clear
	lcd_command( 0x06 );					// Cursor auto-increment
	lcd_command( 0x0C );					// on, no cursor, no blink
}

// Curosor positionieren:
void lcd_pos( uint8_t line, uint8_t column )
{	column = column + line * 64;	
	lcd_command( 0x80 + column );
}

// Ausgabe eines Textes an der aktuellen Cursorposition:
void lcd_text( char *t )
{	while( *t )
	{	lcd_byte( *t );
		t++;
	}
}



