// Climbing Algorithm for the Hero_Bot

// Libraries
#include "DualVNH5019MotorShield.h"

// Communication Headers
#define AUX_IN_HEADER (0x7E)
  
//****Pin Definitions (Using Arduino Mega 2560)****

//Linear Actuator Pins
#define INA_1 45
#define INB_1 44
#define EN_1DIAG_1 44
#define CS_1 A12

#define INA_2 23
#define INB_2 25
#define EN_2DIAG_2 27
#define CS_2 A1 29

#define INA_3 30
#define INB_3 32
#define EN_3DIAG_3 34
#define CS_3 A2 36

#define INA_4 31
#define INB_4 33
#define EN_4DIAG_4 35
#define CS_4 A3 

#define PWM_1/2 9
#define PWM_3/4 10

//***Objects***
// Linear Actuators
DualVNH5019MotorShield md_12(INA_1,INB_1,EN_1DIAG_1,CS_1,INA_2,INB_2,EN_2DIAG_2,CS_2);
DualVNH5019MotorShield md_34(INA_3,INB_3,EN_3DIAG_3,CS_3,INA_4,INB_4,EN_4DIAG_4,CS_4);

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
  md_12.init();
  md_34.init();
}

void loop() {
  // (1) Recieve from aux:
  if (Serial1.peek() != AUX_IN_HEADER) {
    Serial1.read();
  } else {
    
    // aux into buffer
    Serial1.readBytes((char*) aux_in_buf, 32);
    

    // converting to int16 buffer
    for(int i = 0; i < 16; i++){
      aux_in_data[i] = ((int16_t) aux_in_buf[i * 2] << 8) | (aux_in_buf[i * 2 + 1] & 255);
    }
    
    Serial.println(aux_in_data[6]);
    
    if(aux_in_buf[20] == 3) {
      if (aux_in_data[6] < 600) {
       // md.setM1Speed(-400); // M1
       // md.setM2Speed(-400); // M2
        md2.setM1Speed(-400); // M3
        //md2.setM2Speed(-400); // M4
        stopIfFault();
      } else if (aux_in_data[6] > 1400 ) {
        md.setM1Speed(400); // M1
        md.setM2Speed(400); // M2
        md2.setM1Speed(400); // M3
        md2.setM2Speed(400); // M4
        stopIfFault();
      } else {
        md.setM1Speed(0); // M1
        md.setM2Speed(0); // M2
        md2.setM1Speed(0); // M3
        md2.setM2Speed(0); // M4
        stopIfFault();
      }
      
      
    }

  }
}

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
}

