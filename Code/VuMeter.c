
//----------------------------------------------------------------------------------------
// This is the vu meter of the 5 band equalizer, this software realized for the ATmega32
// and allows to the controller to drive a LED matrix connected to the port c & d to show 
// the intensity of the signal's frequency for each channel of the equalizer.
//----------------------------------------------------------------------------------------

#include <avr/io.h>
#include <avr/delay.h>

#define F_CPU 1000000UL									// 1 MHz frequency of the CPU
const int rit= 100;										// delay in ms for the intro 
double valore [5]={127,127,127,127,127};				// actual value of each channel
double vmedio [5]={255,255,255,255,255};				// average value of each channel

void ADC_init()
{
	ADMUX=(1<<ADLAR);									// ADC left adjust result
	ADCSRA=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	// ADC enable, ADC Prescaler division factor 128
}

unsigned int ADC_read(unsigned char ch)
{
	ch= ch & 0b00000111;								// channel must be b/w 0 to 7
	ADMUX =(1<<ADLAR)|ch;								// selecting channel
	
	ADCSRA|=(1<<ADSC);									// start conversion
	while(!(ADCSRA & (1<<ADIF)));						// waiting for ADIF, conversion complete
	ADCSRA|=(1<<ADIF);									// clearing of ADIF, it is done by writing 1 to it
	return (ADCH);
}

void port_init()
{
	DDRA=0x00;											// PORT A as input for ADC
	DDRC=0xFF;											// PORT C as output to drive the LED matrix's rows 
	DDRD=0xFF;											// PORT D as output to drive the LED matrix's columns
}

int pot (int esp)
{
	int ris = 1;
	for (int i = 0; i < esp; i++)
	{
		ris*=2;
	}
	return ris;
}

void intro ()
{
	PORTD = 0xff;
	PORTC = 0X00;
	int acc=0;
	
	for( int i = 0; i<8; i++)
	{
		int  a= pot(i);
		acc += a;
		PORTC =acc;
		_delay_ms(rit);
	}
	
	for (int i = 8; i>0; i--)
	{
		int  a= pot(i);
		acc -= a;
		PORTC =acc;
		_delay_ms(rit);
	}
	PORTC=0x00;
	_delay_ms(rit);
	
	//blink
	for (int i=0;i<2;i++)
	{
		PORTD=0xff;
		PORTC=0xff;
		_delay_ms(500);
		PORTD=0x00;
		PORTC=0x00;
		_delay_ms(500);
	}
}


void accendi (int col, int val)
{
	
	// switching on the row
	int c = pot(col);
	PORTD = c;
	
	// switching on the column
	int b=0;
	
	for( int i = 0; i<val; i++)
	{
		int  a= pot(i);
		b += a;
	}
	
	PORTC=b;
}


void elab (int col, double val)
{
	// adjusting the reading
	val-=vmedio[col];
	if (val<0) val*=-1;
	
	// amplification values are obtained trough laboratory tests
	if(col==4)val*=110;		
	else val*=65;
	
	if(col==0)
		{
		valore[col] = 253*valore[col]+3*val;
		valore[col] /= 256;
		}
	else
		{
		valore[col] = 230*valore[col]+26*val;
		valore[col] /= 256;
		}
	int acc = valore[col]/32;
	accendi(col,acc);
}

void valmedio(int col, double val)
{
	// compute the average value for the column col
	
	vmedio[col]=230*vmedio[col]+26*val;
	vmedio[col] /=256;
}

int main(void)
{
	port_init();
	ADC_init();
	intro();
	while (1)
	{
		// read each channel
		for (int i=0; i<5;i++)
		{
			double lettura = ADC_read(i);
			valmedio(i,lettura);
			elab(i,lettura);
		}
		_delay_ms(3);
	}
}





