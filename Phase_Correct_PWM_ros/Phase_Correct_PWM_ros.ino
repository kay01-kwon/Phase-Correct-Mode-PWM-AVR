#include <Arduino.h>
#include <ros.h>
#include <actuator_msgs/quadrotor_actuator.h>
#include <avr/interrupt.h>

#define CPU_CLK 16000000UL

using actuator_msgs::quadrotor_actuator;
ros::NodeHandle nh;

void msgCallback(const quadrotor_actuator& msg)
{
  OCR0A = msg.motor1;
  OCR0B = msg.motor2;
  OCR1A = msg.motor3;
  OCR1B = msg.motor4;
  
}

ros::Subscriber<quadrotor_actuator> sub("quad_actuator",msgCallback);

void setup() {
  // put your setup code here, to run once:
  
  nh.initNode();
  nh.subscribe(sub);

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

  
  OCR0A = 114;
  OCR0B = 114;

  OCR1A = 114;
  OCR1B = 114;
}

void loop() {
  // put your main code here, to run repeatedly:

  nh.spinOnce();
  delay(1);

}
