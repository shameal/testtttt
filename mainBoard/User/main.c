#include "main.h"
uint32_t Upload_Speed = 10;   // debug printing frequency in hz
#define upload_time (1000000/Upload_Speed)

uint32_t system_micrsecond;

/**** temporary extern debug vars ****/
extern volatile float yaw_angle, pitch_angle, roll_angle; // imu angles
extern volatile MPU6050_REAL_DATA MPU6050_Real_Data; // imu raw

extern volatile int16_t cireq_drive;
extern volatile int16_t cireq_strafe;
extern volatile int16_t cireq_rotate;
extern volatile float cireq_pitch_deg;
extern volatile float cireq_yaw_deg;

extern volatile float enc_yaw_angle_deg;
extern volatile float enc_pitch_angle_deg;

extern volatile int drive_ramp;
extern volatile int strafe_ramp;
extern volatile int rotate_ramp;

/********/
/*
0 - 8191 convert to degrees

input (360 / 8191)

-600 to 600


*/

int main(void)
{
    BSP_Init();
    system_micrsecond = Get_Time_Micros();

    while (1) {
        IMU_getYawPitchRoll(angle);
        if (DEBUG_MODE != DEBUG_MODE_OFF) {
            if ((Get_Time_Micros() - system_micrsecond) > upload_time)
            {
                system_micrsecond = Get_Time_Micros();  
                // handle debug printlns
                if (DEBUG_MODE == DEBUG_MODE_GIMBAL) { // gimbal debug
                    printf("pitch:%f\tyaw:%f\n",
                        enc_pitch_angle_deg,
                        enc_yaw_angle_deg);
                } else if (DEBUG_MODE == DEBUG_MODE_USER) { // user debug
                    // printf("imu_angle:%f\tpitch_angle:%f\tpitch_vel:%f\n",
                    //     pitch_angle,
                    //     enc_pitch_angle_deg,
                    //     MPU6050_Real_Data.Gyro_Y);
                    printf("drive: %d\tstrafe: %d\trotate: %d\n", cireq_drive, cireq_strafe, cireq_rotate);
                }
                delay_ms(1);
            }
        }
    }

    /**** hello world ****/
    // LED_Configuration();
    // while (1) {
    //     delay_ms(100);
    //     LED1_TOGGLE();
    // }

    /**** old code below ****/
    // int i = 0;
    // BSP_Init();
    // //delay 500ms， 等待mpu6050上电稳定    
    // delay_ms(500);    
    // while(MPU6050_Initialization() == 0xff) 
    // {
    //     i++;     //如果一次初始化没有成功，那就再来一次                     
    //     if(i>10) //如果初始化一直不成功，那就没希望了，进入死循环，蜂鸣器一直叫
    //     {
    //         while(1) 
    //         {
    //             LED1_TOGGLE();
    //             delay_ms(50);
                
    //         }
    //     }  
    // }    
    // MPU6050_Gyro_calibration();
    
    // MPU6050_Interrupt_Configuration(); 
        
    // PWM_Configuration();        
    // //设定占空比为1000，初始化摩擦轮电调
    // PWM1 = 1000;
    // PWM2 = 1000;    
    // delay_ms(1000);
    
    // //初始化送弹电机驱动
    // Motor_Reset(MOTOR_NUM1);    
    // delay_ms(30);//延时        
    // Motor_Init(MOTOR_NUM1,PWM_MODE);
    // delay_ms(30);	
    
    // while(1)
    // {
    //     CurrentProtect();//电调电流保护     
    // }
}


