#include <avr/io.h>
#include "adc.ino"

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