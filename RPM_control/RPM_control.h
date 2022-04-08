#include <ros.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Float32.h>
#include <avr/interrupt.h>

#define MIN_PULSE_LENGTH 1000
#define MAX_PULSE_LENGTH 2000

using std_msgs::Float32;

int ref_signal = 200;
int hall_sensor_signal;
int val = 0;
int prev_val = 0;
unsigned long prev_t, curr_t, duration;
unsigned long loop_time_curr, loop_time_prev;

double rpm_data;

double a = -3382;
double b = 9601;
double c = -91;

double throttle;

double torque;

double Kp = 1.0;
double Kd = 0.7;
double Ki = 0.001;

double curr_error;
double prev_error;
double error_d;
double error_i;
unsigned long control_prev_t, control_curr_t, control_duration;

int pulse_input;
int pulse_input_prev;


Float32 encoder_msg;

void msgCallback(const Float32& msg)
{

  //control_duration = control_curr_t - control_prev_t;
  control_duration = 10;
  throttle = (-b + sqrt(b*b-4*a*(c-msg.data)))/2.0/a;

  curr_error = msg.data - rpm_data;
  error_d = (curr_error - prev_error)/control_duration*1000.0;
  error_i += curr_error*control_duration/1000.0;

  torque = Kp*curr_error + Kd*error_d + Ki*error_i;
  pulse_input = throttle*1000 + torque*control_duration/10000.0 + 1000;
  OCR1A = pulse_input;

  prev_error = curr_error;
}

ros::NodeHandle nh;
ros::Subscriber<Float32> sub("rpm_des",msgCallback);
ros::Publisher pub("encoder_data",&encoder_msg);

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

void rpmRead()
{
    
  hall_sensor_signal = analogRead(A0);

  if(hall_sensor_signal>ref_signal)
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
    if(duration > 500000)
      rpm_data = 0;
  }
  
  prev_val = val;

  loop_time_curr = millis();

  if(loop_time_curr - loop_time_prev >= 10)
  {
    encoder_msg.data = rpm_data;
    pub.publish(&encoder_msg);
    nh.spinOnce();
    loop_time_prev = loop_time_curr;
  }
  
}
