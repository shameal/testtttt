#include "main.h"

// send messages
CanTxMsg tx_gimbal_message;
CanTxMsg tx_chassis_message;

// raw messages
volatile int16_t enc_chassis_201_vel;
volatile int16_t enc_chassis_202_vel;
volatile int16_t enc_chassis_203_vel;
volatile int16_t enc_chassis_204_vel;
volatile int16_t enc_yaw_angle; // 0~8191
volatile int16_t enc_pitch_angle; // 0~8191

volatile float enc_yaw_angle_deg;
volatile float enc_pitch_angle_deg;

void can2_rx_process(CanRxMsg* rx_message) {
    switch (rx_message->StdId) {
        case 0x201: {
            chassis_enc_process(&enc_chassis_201_vel, rx_message);
        } break;
        case 0x202: {
            chassis_enc_process(&enc_chassis_202_vel, rx_message);
        } break;
        case 0x203: {
            chassis_enc_process(&enc_chassis_203_vel, rx_message);
        } break;
        case 0x204: {
            chassis_enc_process(&enc_chassis_204_vel, rx_message);
        } break;
        case 0x205: { // yaw
            int16_t yaw_data0 = rx_message->Data[0];
            int16_t yaw_data1 = rx_message->Data[1];
            enc_yaw_angle = (yaw_data0) << 8 | (yaw_data1);
            enc_yaw_angle_deg = enc_yaw_angle * (360.0 / 8191.0) - ENC_YAW_OFFSET_DEG;
        } break;
        case 0x206: { // pitch
            int16_t pitch_data0 = rx_message->Data[0];
            int16_t pitch_data1 = rx_message->Data[1];
            enc_pitch_angle = (pitch_data0) << 8 | (pitch_data1);
            enc_pitch_angle_deg = enc_pitch_angle * (360.0 / 8191.0) - ENC_PITCH_OFFSET_DEG;
        } break;
        default: {}
    }
}

void chassis_enc_process(volatile int16_t* enc_chassis_vel, CanRxMsg* rx_message) {
    *enc_chassis_vel = (rx_message->Data[2] << 8) | rx_message->Data[3];
}

void send_chassis(int16_t vel_201, int16_t vel_202, int16_t vel_203, int16_t vel_204) {
    tx_chassis_message.StdId = 0x200;
    tx_chassis_message.DLC = 0x08;
    tx_chassis_message.RTR = CAN_RTR_Data;
    tx_chassis_message.IDE = CAN_Id_Standard;
    tx_chassis_message.Data[0] = vel_201 >> 8;
    tx_chassis_message.Data[1] = vel_201 & 0xFF;
    tx_chassis_message.Data[2] = vel_202 >> 8;
    tx_chassis_message.Data[3] = vel_202 & 0xFF;
    tx_chassis_message.Data[4] = vel_203 >> 8;
    tx_chassis_message.Data[5] = vel_203 & 0xFF;
    tx_chassis_message.Data[6] = vel_204 >> 8;
    tx_chassis_message.Data[7] = vel_204 & 0xFF;
    CAN_Transmit(CAN2, &tx_chassis_message);
}

void send_gimbal(int16_t pitch_current, int16_t yaw_current) {
    tx_gimbal_message.StdId = 0x1FF;
    tx_gimbal_message.DLC = 0x08;
    tx_gimbal_message.RTR = CAN_RTR_Data;
    tx_gimbal_message.IDE = CAN_Id_Standard;
    tx_gimbal_message.Data[0] = yaw_current >> 8;
    tx_gimbal_message.Data[1] = yaw_current & 0xFF;
    tx_gimbal_message.Data[2] = pitch_current >> 8;
    tx_gimbal_message.Data[3] = pitch_current & 0xFF;
    tx_gimbal_message.Data[4] = 0x00;
    tx_gimbal_message.Data[5] = 0x00;
    tx_gimbal_message.Data[6] = 0x00;
    tx_gimbal_message.Data[7] = 0x00;
    CAN_Transmit(CAN2, &tx_gimbal_message);
}
