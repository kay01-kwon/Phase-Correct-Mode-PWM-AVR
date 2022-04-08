#include <avr/io.h>
#include <avr/interrupt.h>
#define CPU_CLK 16000000UL
#define MIN_PULSE_LENGTH 1000
#define MAX_PULSE_LENGTH 2000

char data;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  PORTB = 0x00;
  PORTD = 0x00;
  DDRB = 0xFF;
  DDRD = 0xFF;

  // Initiate Timer Counter Registor 1A and 1B
  TCCR1A = 0x00;
  TCCR1B = 0x00;

  // Phase Correct PWM mode
  TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
  TCCR1B |= (1<<WGM13) | (1<<WGM11) | (1<<CS11);    //Prescale = 8
  TCCR1C = 0x00;


  OCR1A = 1000;
  ICR1 = 2000;

}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()){
    data = Serial.read();
        switch (data) {
            // 0
            case 48 : Serial.println("Sending minimum throttle");
                      OCR1A = MIN_PULSE_LENGTH;

            break;

            // 1
            case 49 : Serial.println("Sending maximum throttle");
                      OCR1A = MAX_PULSE_LENGTH;

            break;

            // 2
            case 50 : Serial.print("Running test in 3");
                      delay(1000);
                      Serial.print(" 2");
                      delay(1000);
                      Serial.println(" 1...");
                      delay(1000);
                      test();
            break;
        }
    
    }
}

void test()
{
  for(int i = MIN_PULSE_LENGTH; i <= MAX_PULSE_LENGTH;i +=5)
  {
    Serial.print("Pulse Length: ");
    Serial.println(i);
    OCR1A = i;
    delay(200);
  }

  Serial.println("stop");
  OCR1A = MIN_PULSE_LENGTH;
}

void displayInstructions()
{
    Serial.println("READY - PLEASE SEND INSTRUCTIONS AS FOLLOWING :");
    Serial.println("\t0 : Send min throttle");
    Serial.println("\t1 : Send max throttle");
    Serial.println("\t2 : Run test function\n");  
}
