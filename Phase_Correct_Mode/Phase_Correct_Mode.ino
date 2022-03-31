#include <avr/interrupt.h>
#define CPU_CLK 16000000UL

int value;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  PORTD = 0x00;
  PORTB = 0x00;
  
  DDRD = 0x60;    // Set PD5 and PD6 to OUTPUT
  DDRB = 0x06;    // Set PB1 and PB2 to OUTPUT

  TCCR0A = 0x00;    // Initiate TCCR0A registor
  TCCR0A = (1<<COM0A1)|(1<<COM0B1)|(1<<WGM00);  
  TCCR0B = (1<<CS01) | (1<<CS00);   // Prescale : 64

  TCCR1A = 0x00;
  TCCR1A = (1<<COM1A1)|(1<<COM1B1)|(1<<WGM10);   
  TCCR1B = (1<<CS11) | (1<<CS10);   // Prescale : 64

  
  OCR0A = 150;
  OCR0B = 50;

  OCR1A = 10;
  OCR1B = 120;
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()>0)
  {
    value = Serial.parseInt();
    OCR0A = value;
    OCR0B = value;
  }
}
