#include "RPM_control.h"

void setup() {
  // put your setup code here, to run once:
  pinMode(A0,INPUT);
  setupPWM();
  
  nh.initNode();
  nh.advertise(pub);
  nh.subscribe(sub);
}

void loop() {
  // put your main code here, to run repeatedly:
  rpmRead();

}
