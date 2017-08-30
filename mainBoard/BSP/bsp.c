#include "bsp.h"

void BSP_Init(void){
    
    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);   
    LED_Configuration();            //ָʾ�Ƴ�ʼ��
    //CAN1_Configuration();            //��ʼ��CAN
    CAN2_Configuration(); 
    USART1_Configuration();          //����1��ʼ��
    USART3_Configuration();
    TIM2_Configuration();
    MPU6050_Initialize();
    MPU6050_IntConfiguration();     
    MPU6050_EnableInt();        

    delay_ms(2000);
    IMU_getYawPitchRoll(angle);
    delay_ms(500);
    TIM6_Configuration();
    TIM6_Start();
}


