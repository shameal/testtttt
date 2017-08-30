#include "DualVNH5019MotorShield.h"

DualVNH5019MotorShield md(21,24,26,A0,4,5,6,A1);
DualVNH5019MotorShield md2(14,15,16,A2,17,18,19,A3);
int val = 0;

void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while(1);
  }
    if (md2.getM1Fault())
  {
    Serial.println("M3 fault");
    while(1);
  }
  if (md2.getM2Fault())
  {
    Serial.println("M4 fault");
    while(1);
  }
}
void setup() {  
  Serial.begin(115200);
  pinMode(13, INPUT);
  md.init();
  md2.init();
}

void loop() {
  val = digitalRead(13);
  Serial.println(val);
  if(val == 1){
    md.setM1Speed(400); // M1
    md.setM2Speed(400); // M2
    md2.setM1Speed(400); // M3
    md2.setM2Speed(400); // M4
    stopIfFault();
  } else {
    md.setM1Speed(-400); // M1
    md.setM2Speed(-400); // M2
    md2.setM1Speed(-400); // M3
    md2.setM2Speed(-400); // M4
    stopIfFault();
  }
}
