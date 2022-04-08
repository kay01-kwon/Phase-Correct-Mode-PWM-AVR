#include <avr/interrupt.h>
#define CPU_CLK 16000000UL

int val;

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  PORTB = 0x00;
  DDRB = 0xFF;

  // Initiate Timer Counter Registor
  TCCR1A = 0x00;
  TCCR1B = 0x00;

  
  TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
  TCCR1B |= (1<<WGM13) | (1<<WGM11) | (1<<CS11);
  TCCR1C = 0;

  OCR1A = 1000;
  OCR1B = 1000;
  ICR1 = 2000;

}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available())
  {
    val = Serial.parseInt();
    OCR1A = val;
  }


}
