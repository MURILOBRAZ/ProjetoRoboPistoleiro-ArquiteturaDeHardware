#include <avr/io.h>
//#include <avr/interrupt.h> // Interrupções não utilizadas

int valorADC0 = 0;
int valorMapeado0 = 0;
short cont = 3;
short tempo = 0;

ISR(TIMER0_OVF_vect) {
  valorADC0 = ReadADC(0); // Leitura do valor do conversor analógico digital (ADC)
}

ISR(TIMER1_OVF_vect) {
  PORTB ^= (1 << 0); // Alterna o estado do pino B0
  tempo++;
}

// Inicialização do Timer0
void Init_timer0() {
  // Configuração do Timer0 para controle de PWM
  TCCR0A |= (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
  TCCR0B |= (1 << CS00); // Seleção de prescaler para o Timer0
  OCR0A = valorMapeado0; // Valor do registro de comparação para o Timer0
  TIMSK0 = (1 << TOIE0); // Habilita interrupção de overflow do Timer0
  sei(); // Habilita interrupções globais
}

// Inicialização do Timer1
void Init_timer1() {
  TCCR1A = 0;
  TCCR1B |= (1 << CS12); // Seleção de prescaler para o Timer1
  TCNT1 = 0; // Zera o contador do Timer1
  TIMSK1 |= (1 << TOIE1); // Habilita interrupção de overflow do Timer1
}

// Inicialização do Timer2
void Init_timer2() {
  // Configuração do Timer2 para controle de PWM
  TCCR2A |= (1 << COM2B1) | (1 << WGM21) | (1 << WGM20);
  TCCR2B |= (1 << CS20); // Seleção de prescaler para o Timer2
}

// Inicialização do conversor analógico digital (ADC)
void InitADC() {
  ADMUX |= (1 << REFS0); // Seleção da referência de tensão para o ADC
  ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0) | (1 << ADEN); // Configuração do prescaler e habilitação do ADC
}

void NossoD(unsigned int ms) {
    for (unsigned int i = 0; i < ms; i++) {
        // Ajuste a contagem para um atraso de aproximadamente 1 milissegundo
        for (unsigned int j = 0; j < F_CPU / 4000; j++) {
            asm volatile("nop"); // Operação nula para atraso
        }
    }
}

// Função para ler o valor do ADC em um canal específico
uint16_t ReadADC(uint8_t channel) {
  ADMUX = (ADMUX & 0XF0) | (channel & 0x0F); // Seleção do canal de leitura do ADC
  ADCSRA |= (1 << ADSC); // Inicia a conversão do ADC
  while (ADCSRA & (1 << ADSC)); // Aguarda a conclusão da conversão
  return ADC; // Retorna o valor convertido
}

// Função principal
int main(void) {
  // Configuração dos pinos de entrada e saída
  // DDRx define se o pino é entrada (0) ou saída (1)
  DDRD |= (1 << 2) | (1 << 3) | (1 << 7); // Configuração dos pinos D2, D3 e D7 como saída
  DDRD |= (1 << 4) | (1 << 5) | (1 << 6); // Configuração dos pinos D4, D5 e D6 como saída
  DDRB |= (1 << 2) | (1 << 3) | (1 << 4); // Configuração dos pinos D10, D11 e D12 como saída
  DDRB |= (1 << 0); // Configuração do pino D8 como saída

  // Configuração dos pinos de controle (ativação de motores)
  //PORTD |= (1 << 7); // Ativação do motor direito
  //PORTD |= (1 << 4); // Ativação do motor esquerdo
  PORTB |= (1 << 2) | (1 << 3) | (1 << 4); // Ativação dos motores de controle D10, D11 e D12

  InitADC(); // Inicialização do ADC
  Init_timer0(); // Inicialização do Timer0
  Init_timer1(); // Inicialização do Timer1
  Init_timer2(); // Inicialização do Timer2


  while (1) {
    // Verifica se o pino PINC1 está ativado (bit 1 do registrador PINC)
    if (PINC & (1 << PINC3)) {
    // Verifica se o bit PINC1 no registrador PINC está ativado
    // Configura o movimento para a frente
    
    // Ativa a direção para a frente nos pinos específicos (D2 e D4)
    PORTD |= (1 << 5);//seta o pino
    PORTD |= (1 << 7);//seta o pino

    PORTD &= ~(1 << 2);//limpa o pino
    PORTD &= ~(1 << 4);//limpa o pino
    // Define o valor do PWM para o motor no Timer0 e Timer2
    OCR0A = 255;
    OCR2B = 255;
    }
    // Verifica se o bit PINC2 no registrador PINC está ativado
    else if(PINC & (1 << PINC1)) {
      // Configura o movimento para a direita
      
      // Define o valor do PWM para o motor no Timer0 e Timer2 para a direita
      OCR0A = 255;
      OCR2B = 0;
    }
    // Verifica se o bit PINC3 no registrador PINC está ativado
    else if(PINC & (1 << PINC4)) {
      // Configura o movimento para a esquerda
      
      // Define o valor do PWM para o motor no Timer0 e Timer2 para a esquerda
      OCR0A = 0;
      OCR2B = 255;
    }
    // Verifica se o bit PINC4 no registrador PINC está ativado
    else if(PINC & (1 << PINC2)) {
      // Configura o movimento para trás
      
      // Define a direção dos pinos para trás e define o valor do PWM para o motor no Timer0 e Timer2
      PORTD |= (1 << 2);//seta o pino
      PORTD |= (1 << 4);//seta o pino

      PORTD &= ~(1 << 7);//limpa o pino
      PORTD &= ~(1 << 5);//limpa o pino
      OCR0A = 255;
      OCR2B = 255;
    }
    else {
      // Se nenhuma das condições anteriores for atendida, interrompe o movimento
      
      // Desativa todos os pinos relacionados ao movimento
      PORTD &= ~(1 << 7);
      PORTD &= ~(1 << 5);
      PORTD |= (1 << 2);
      PORTD |= (1 << 4);
      
      // Define o valor do PWM para o motor no Timer0 e Timer2 como zero
      OCR0A = 0;
      OCR2B = 0;
    }

    // Verifica se o valor do ADC lido é maior que 500
    if (valorADC0 > 920) {

      // Introduz um atraso antes de prosseguir
     NossoD(1); // Atraso de 100 milissegundos

      // Verifica e executa ações com base no valor da variável 'cont'
      if (cont == 3) {
        PORTB &= ~(1 << 2); // Desativa um dos pinos no PORTB (bit 2)
      } else if (cont == 2) {
        PORTB &= ~(1 << 3); // Desativa um dos pinos no PORTB (bit 3)
      } else if (cont == 1){
        PORTB &= ~(1 << 4); // Desativa um dos pinos no PORTB (bit 4)
      }
      OCR0A = 0;
      OCR2B = 0;
      if (tempo == 5) {
        OCR0A = 0; // Define o valor do PWM para 0 no Timer0
        cont = 3;  // Reinicializa 'cont' para 3
      } else {
        OCR0A = 255; // Define o valor do PWM para o máximo no Timer0
      }
      cont--; // Decrementa a variável 'cont'
    }
    else {
      tempo = 0; // Reseta 'tempo' se 'cont' não chegou a zero
    }
    
  }
}
