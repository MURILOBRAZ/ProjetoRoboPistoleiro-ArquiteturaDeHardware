#include <avr/io.h>


void InitADC(){

	ADMUX |= (1 << REFS0);
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN);
}

uint16_t ReadADC(uint8_t channel){

	ADMUX = (ADMUX & 0XF0) | (channel & 0x0F);
	
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	return ADC;
}

int valorADC0 = 0;
int valorADC1 = 0;
int valorADC2 = 0;
int valorMapeado0 = 0;
int valorMapeado1 = 0;
int main(void){
	DDRD |= (1 << 2) | (1 << 3) | (1 << 7);
	DDRD |= (1 << 4) | (1 << 5) | (1 << 6);
	DDRB |= (1 << 0);
    InitADC();
	while(1){
		
		valorADC0 = ReadADC(0);
	}

}