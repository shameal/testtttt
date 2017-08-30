#ifndef _CAN_BUS_TASK_H_
#define _CAN_BUS_TASK_H_

void can2_rx_process(CanRxMsg *msg);
void chassis_enc_process(volatile int16_t* enc_chassis_vel, CanRxMsg* rx_message);
void send_chassis(int16_t vel_201, int16_t vel_202, int16_t vel_203, int16_t vel_204);
void send_gimbal(int16_t pitch_current, int16_t yaw_current);

typedef struct wheels_rx_data_t {
    int16_t measured_angle;
    int16_t measured_velocity;
} wheels_rx_data_t;

#endif
