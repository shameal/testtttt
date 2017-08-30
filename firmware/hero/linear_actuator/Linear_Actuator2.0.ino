#include "DualVNH5019MotorShield.h"

// Motor Driver Objects for the Linear Actuators
DualVNH5019MotorShield md(21,24,26,A0,4,5,6,A1);
DualVNH5019MotorShield md2(2,3,4,A2,51,52,53,A3);
  
/**** pc key definition ****/
#define KEY_PRESSED_OFFSET_W            ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S            ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A            ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D            ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q            ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E            ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT        ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL         ((uint16_t)0x01<<7)

// Communication Headers
#define AUX_IN_HEADER (0x7E)

//Pin Definitions
#define LED_PIN (13)
#define LED_ON() digitalWrite(LED_PIN, HIGH)
#define LED_OFF() digitalWrite(LED_PIN, LOW)

//Com Buffer
char aux_in_buf[32];

//Data Buffers
int16_t aux_in_data[16]; //16 bit integer


void setup() {
  // put your setup code here, to run once:
  Serial1.begin(115200);
  Serial2.begin(115200);
  Serial.begin(115200);
  md.init();
  md2.init();
}

void loop() {
  
  // (1) Recieve from AUX COMHUB:
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
    Serial.println(aux_in_buf[20]);


    if(aux_in_buf[20] == 3) {
      if (aux_in_data[6] < 600) {
        md.setM1Speed(-400); // M1
        md.setM2Speed(-400); // M2
        md2.setM1Speed(-400); // M3
        md2.setM2Speed(-400); // M4
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

