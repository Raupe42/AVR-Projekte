/*
* MiniProjektA.c
*Das Ziel ist es selbst‰ndig ein kleines Programm zu erstellen, dass Ein und Ausgaben beinhaltet

MultiPseudoSimultanLedTasterVerarbeitungsDevice

* Created: 06.12.2018 19:33:22
* Author : jensh
*/

#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

#define DELAY_TASTER 5	//PORTD
#define DELAY_LED 7		//PORTD
#define BLINKE_TASTER 0 //PORTB
#define BLINKE_LED 1	//PORTB
#define TOGGLE_TASTER 2	//PORTD
#define TOGGLE_LED 3	//PORTD
#define SAMMEL_TASTER 2	//PORTB
#define SAMMEL_LED 3	//PORTB

int elekWuerfwel ();
long warteLED (long counter);
int taster (int linksshift, volatile uint8_t* p_PinRegister);
int outputSet (int state, volatile uint8_t* p_PortRegister, int linksshift);
void anAus ();
void blinke ();
void sammelLED();

//int letzterTaster = 0;

int main(void)
{
	long counter = 0;


	//INIT - 1 OUT 0 IN
	DDRB = DDRB = 0;
	DDRD |= (1<<TOGGLE_LED);
	DDRD |= (1<<DELAY_LED);
	DDRB |= (1<<BLINKE_LED);
	DDRB |= (1<<SAMMEL_LED);
	PORTB = ~DDRB;
	PORTD = ~DDRD;

	while (1)
	{
		sammelLED();
		//elekWuerfwel();
		counter = warteLED(counter);
		blinke();
		anAus();
		
		_delay_ms(1);	// Zyklusbremse
		
	}
}

//nicht benutzt
int elekWuerfwel ()
{
	int state = 0;
	int out = 0;
	while (1)
	{
		//if (!(PIND & (1<<IN1)))
		state ++;
		out = (state %6);
		PORTB = out + 1;

	}
}
//nicht benutzt
int siebenSeg (int zahl)
{
	//Anzeige einer Zah auf einem 7 Seg
	//an einem PORT angeschlossen
	/*
	.db 0b00111111 ;0
	.db 0b00000110 ;1
	.db	0b01011011 ;2
	.db 0b01001111 ;3
	.db 0b01100110 ;4
	.db 0b01101101 ;5
	.db 0b01111101 ;6
	.db 0b00000111 ;7
	.db 0b01111111 ;8
	.db 0b01101111 ;9
	*/

	int out = 0;
	switch (zahl)
	{
		case 0:
		out = 0b00111111;
		break;
		case 1:
		out = 0b00000110;
		break;
		case 2:
		out = 0b01011011;
		break;
		case 3:
		out = 0b01001111;
		break;
		case 4:
		out = 0b01100110;
		break;
		case 5:
		out = 0b01101101;
		break;
		case 6:
		out = 0b01111101;
		break;
		case 7:
		out = 0b00000111;
		break;
		case 8:
		out = 0b01111111;
		break;
		case 9:
		out = 0b01101111;
		break;
	}
	return out;
}

long warteLED (long counter)
{
	static long delayWarte = 0;
	static long delay_act = 0;
	static int ersterDruck = 1;
	static int taster_lastStateWarte = 0;
	int tasterVal = taster(DELAY_TASTER, &PIND);
	
	// Taster behandlung
	if (tasterVal && !taster_lastStateWarte) // Flankenerkennung
	{
		if (ersterDruck)
		{
			counter = 0;
			delayWarte = counter;
			ersterDruck = 0;
		}
		else
		{
			delayWarte = counter - delayWarte;
			ersterDruck = 1;
			delay_act = 0;
			//counter = 0;
		}
	}

	// Leuchten in gew‰hltem Abstand
	if (!delay_act)
	outputSet(1, &PORTD, DELAY_LED);	// LED Ein
	else if (delayWarte <= delay_act)
	outputSet(0, &PORTD, DELAY_LED);	// LED Aus
	else if (delay_act > delayWarte * 2)
	
	delay_act = 0;
	
	//counter ++
	counter ++;
	delay_act ++;

	taster_lastStateWarte = tasterVal;

	return counter;
}

void anAus ()
{
	static int status = 0;
	static int letzterTaster = 0;
	int tasterVal = 0;
	tasterVal = taster(TOGGLE_TASTER, &PIND);
	if (tasterVal && !letzterTaster)
	{
		if (status)
		status = 0;
		else
		status = 1;
	}
	outputSet(status, &PORTD, TOGGLE_LED);
	letzterTaster = tasterVal;
}


void blinke ()
{
	//static long counter = 0;
	static long actual = 0;
	static long delay = 200;
	static long waitTime = 0;
	static int state = 1;
	static int letzterTaster = 0;
	int dieserTaster = taster(BLINKE_TASTER, &PINB);
	
	//flanke
	if (dieserTaster && !letzterTaster)
	{
		switch (state)
		{
			case 1:
			waitTime = 0;
			break;
			case 2:
			delay = waitTime * 2;
			//counter = 0;
			state = 0;
			break;
			default:
			state = 1;
		}
		state ++;
		
	}
	
	if (actual < delay)
	{
		outputSet(1, &PORTB, BLINKE_LED);
	}
	else
	{
		outputSet(0, &PORTB, BLINKE_LED);
	}
	
	if (actual > delay * 2)
	actual = 0;
	//counter ++;
	actual ++;
	waitTime ++;
	
	letzterTaster = dieserTaster;
	return;
}


void sammelLED ()
{
	static int data [2] [400];
	static long delaySammelLed = 0;
	static long counterSammel = 0;
	const long delayCount = 3;
	static int stateSammel = 0;
	static int tempSammel;
	
	if (delaySammelLed >= delayCount)
	{
		delaySammelLed = 0;

		data [stateSammel][counterSammel] = taster(SAMMEL_TASTER, &PINB);
		counterSammel ++;
	}
	
	if (stateSammel)
	tempSammel = data [0] [counterSammel];
	
	else
	tempSammel = data [1] [counterSammel];
	
	outputSet(tempSammel, &PORTB, SAMMEL_LED);
	//outputSet(temp, &PORTD, 6);

	if (counterSammel == 399)
	{
		counterSammel = 0;
		if (!stateSammel)
		stateSammel = 1;
		else
		stateSammel = 0;
	}
	
	
	delaySammelLed ++;
	
	return;
}

// Hilfsfunktionen

/*
R¸ckgabewert:
0: Nicht bet‰tigt
1: Bet‰tigt
-> Schlieﬂer
*/
int taster (int linksshift, volatile uint8_t* p_PinRegister)
{
	int retVal = 0;
	if (!(*p_PinRegister & (1<<linksshift)))
	retVal = 1;
	return retVal;
}

/*
F¸r ein angenehmeres benutzen von Outputs
(Datentyp des Port Pointers aus den Warnungen entnommen)
Parameter:
state: 0 f¸r aus; != 0 Ein
p_PortRegister: Addresse des Ports
linksshift: Anzahl der Linksshifts f¸r den bertreffenden Pin
*/
int outputSet (int state, volatile uint8_t* p_PortRegister, int linksshift)
{
	if (state)
	*p_PortRegister |= (1<<linksshift);	// Output Ein
	else
	*p_PortRegister &= ~(1<<linksshift);
	return *p_PortRegister;
}