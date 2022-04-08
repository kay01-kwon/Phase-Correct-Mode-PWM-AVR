#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <avr/interrupt.h>

#define MIN_PULSE_LENGTH 1000
#define MAX_PULSE_LENGTH 2000

using std_msgs::Int32;
using std_msgs::Float32;

ros::NodeHandle nh;

int ref_signal = 200;
int val = 0;
int prev_val = 0;
unsigned long prev_t, curr_t, duration;
unsigned long loop_time_curr, loop_time_prev;
double rpm_data;

void msgCallback(const Int32& msg)
{
  OCR1A = msg.data;
}

void setupPWM()
{

  PORTB = 0x00;
  DDRB = 0xFF;
  // Initiate Timer Counter Register
  TCCR1A = 0x00;
  TCCR1B = 0x00;

  // Set Phase Correct Mode | Prescale : 8
  TCCR1A |= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
  TCCR1B |= (1<<WGM13) | (1<<WGM11) | (1<<CS11);
  TCCR1C = 0x00;

  OCR1A = MIN_PULSE_LENGTH;
  OCR1B = MIN_PULSE_LENGTH;
  ICR1 = MAX_PULSE_LENGTH;
}

ros::Subscriber<Int32> sub("pwm",msgCallback);
Float32 encoder_msg;
ros::Publisher pub("encoder_data",&encoder_msg);



void setup() {
  // put your setup code here, to run once:

  //Serial.begin(115200);
  pinMode(A0,INPUT);
  setupPWM();
  
  nh.initNode();
  nh.advertise(pub);
  nh.subscribe(sub);
}

void loop() {
  // put your main code here, to run repeatedly:
  int signal_ = analogRead(A0);
 
  if(signal_>ref_signal)
    val = HIGH;
  else
    val = LOW;

  if(prev_val == 0 && val == 1)
  {
    curr_t = micros();
    duration = curr_t - prev_t;
    rpm_data = (double)500000*60/duration;
    prev_t = curr_t;
  }
  else
  {
    curr_t = micros();
    duration = curr_t - prev_t;
    if(duration > 1000000)
      rpm_data = 0;
  }
  prev_val = val;

  loop_time_curr = millis();

  if(loop_time_curr - loop_time_prev > 10)
  {
    encoder_msg.data = rpm_data;
    pub.publish(&encoder_msg);
    nh.spinOnce();
    loop_time_prev = loop_time_curr;
  }

}
