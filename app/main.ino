#include <avr/io.h>
#include <avr/interrupt.h>

int valorADC0 = 0;
int valorMapeado0 = 0;
short cont = 3;

ISR(TIMER0_OVF_vect){
	valorADC0 = ReadADC(0);
  
}

ISR(TIMER1_OVF_vect){
  PORTB ^= (1 << 0);
  tempo++;
  
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void Init_timer0(){

	TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
	TCCR0B |= (1 << CS00);
	OCR0A = valorMapeado0;
	TIMSK0 = (1 << TOIE0);
	sei();
}

void Init_timer1(){
  TCCR1A = 0;
  TCCR1B |= (1 << CS12);
  TCNT1 = 0;
  TIMSK1 |= (1 << TOIE1);
}

void Init_timer2(){

	TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
	TCCR2B |= (1 << CS20);
	
}

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



int main(void){
	DDRD |= (1 << 2) | (1 << 3) | (1 << 7);//d2 d3 d7
	DDRD |= (1 << 4) | (1 << 5) | (1 << 6);//d4 d5 d6
	DDRB |= (1 << 2) | (1 << 3) | (1 << 4);//d10 d11 d12
	DDRB |= (1 << 0);//d8
	DDRC |= (1 << 0) | (1 << 1) | (1 << 2);//a0 a1 a2
	DDRC |= (1 << 3) | (1 << 4);//a3 a5
	PORTD |= (1 << 6) | (1 << 7);//direita
	PORTD |= (1 << 3) | (1 << 4);//esquerda
	InitADC();
	Init_timer0();
	Init_timer1();
    Init_timer2();
	Serial.begin(9600);
	while(1){
		
		//FRENTE
    if(PINC & (1 << PINC1)){
      PORTD |= (1 << 2);//direita
      OCR0A = 95;
      OCR2B = 95;
    }
    //DIREITA
    else if(PINC & (1 << PINC2)){
      OCR0A = 95;
      OCR2B = 85;
    }
    //ESQUERDA
    else if(PINC & (1 << PINC3)){
      OCR0A = 85;
      OCR2B = 95;
    }
    //TRÁS
    else if(PINC & (1 << PINC4)){
      PORTD &= ~(1 << 2);
      PORTD &= ~(1 << 4);
      PORTD |= (1 << 7);//direita
      PORTD |= (1 << 5);
	    OCR0A = 255;
      OCR2B = 255;
    }
    else{
      PORTD &= ~(1 << 7);
      PORTD &= ~(1 << 5);
      PORTD |= (1 << 2);//direita
      PORTD |= (1 << 4);
		  OCR0A = 0;
      OCR2B = 0;
    }
    if(valorADC0 > 500){
    
      if(cont == 3){
        PORTB &= ~(1 << 2);
      }
      else if(cont == 2){
        PORTB &= ~(1 << 3);
      }
      else{
        PORTB &= ~(1 << 4);
        
      }
      cont--;
    }
    if(cont == 0){
      if(tempo == 2){
        OCR0A = 0;
        cont = 3;
      }
      else{
        OCR0A = 255;
      }

    }
    else{
      tempo = 0;
    }
		
	}

}
