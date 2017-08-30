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
    // //delay 500ms�� �ȴ�mpu6050�ϵ��ȶ�    
    // delay_ms(500);    
    // while(MPU6050_Initialization() == 0xff) 
    // {
    //     i++;     //���һ�γ�ʼ��û�гɹ����Ǿ�����һ��                     
    //     if(i>10) //�����ʼ��һֱ���ɹ����Ǿ�ûϣ���ˣ�������ѭ����������һֱ��
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
    // //�趨ռ�ձ�Ϊ1000����ʼ��Ħ���ֵ��
    // PWM1 = 1000;
    // PWM2 = 1000;    
    // delay_ms(1000);
    
    // //��ʼ���͵��������
    // Motor_Reset(MOTOR_NUM1);    
    // delay_ms(30);//��ʱ        
    // Motor_Init(MOTOR_NUM1,PWM_MODE);
    // delay_ms(30);	
    
    // while(1)
    // {
    //     CurrentProtect();//�����������     
    // }
}


