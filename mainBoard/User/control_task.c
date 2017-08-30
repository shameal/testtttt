#include "main.h"

/**** pid regulators ****/
PID_Regulator_t chassis_201_pid = CHASSIS_VEL_PID_DEFAULT;
PID_Regulator_t chassis_202_pid = CHASSIS_VEL_PID_DEFAULT;
PID_Regulator_t chassis_203_pid = CHASSIS_VEL_PID_DEFAULT;
PID_Regulator_t chassis_204_pid = CHASSIS_VEL_PID_DEFAULT;

PID_Regulator_t pitch_vel_pid = PITCH_VEL_PID_DEFAULT;
PID_Regulator_t pitch_pos_pid = PITCH_POS_PID_DEFAULT;

PID_Regulator_t yaw_vel_pid = YAW_VEL_PID_DEFAULT;
PID_Regulator_t yaw_pos_pid = YAW_POS_PID_DEFAULT;
/********/

/**** sensor sources ****/
extern volatile int16_t enc_chassis_201_vel;
extern volatile int16_t enc_chassis_202_vel;
extern volatile int16_t enc_chassis_203_vel;
extern volatile int16_t enc_chassis_204_vel;
extern volatile float yaw_angle, pitch_angle, roll_angle; // imu angles
extern volatile MPU6050_REAL_DATA MPU6050_Real_Data; // imu raw
/********/

/**** control information sources ****/
extern volatile int16_t cireq_drive;
extern volatile int16_t cireq_strafe;
extern volatile int16_t cireq_rotate;
/********/

// control task runs on tim6 interrupt
void control_task(void) {
    control_task_chassis();
    control_task_gimbal();
}

void control_task_chassis(void) {
    // translate cireq to target velocities
    int16_t target_vel_201 = (cireq_drive + cireq_strafe + cireq_rotate);
    int16_t target_vel_202 = (cireq_drive - cireq_strafe + cireq_rotate);
    int16_t target_vel_203 = -(cireq_drive + cireq_strafe - cireq_rotate);
    int16_t target_vel_204 = -(cireq_drive - cireq_strafe - cireq_rotate);

    // convert rc range to current range
    map(&target_vel_201, -660, 660, -8171, 8171);
    map(&target_vel_202, -660, 660, -8171, 8171);
    map(&target_vel_203, -660, 660, -8171, 8171);
    map(&target_vel_204, -660, 660, -8171, 8171);

    // update pid
    int16_t output_vel_201 = chassis_pid_update(201, target_vel_201);
    int16_t output_vel_202 = chassis_pid_update(202, target_vel_202);
    int16_t output_vel_203 = chassis_pid_update(203, target_vel_203);
    int16_t output_vel_204 = chassis_pid_update(204, target_vel_204);

    // limit the output
    limit_range(&output_vel_201, (int) -CHASSIS_ESC_MAX, (int) CHASSIS_ESC_MAX);
    limit_range(&output_vel_202, (int) -CHASSIS_ESC_MAX, (int) CHASSIS_ESC_MAX);
    limit_range(&output_vel_203, (int) -CHASSIS_ESC_MAX, (int) CHASSIS_ESC_MAX);
    limit_range(&output_vel_204, (int) -CHASSIS_ESC_MAX, (int) CHASSIS_ESC_MAX);

    // send the output
    send_chassis(output_vel_201, output_vel_202, output_vel_203, output_vel_204);
}

int16_t chassis_pid_update(int can_address, int target_vel) {
    int output_vel;
    switch (can_address) {
        case 201:
            output_vel = pid_control(enc_chassis_201_vel, target_vel, &chassis_201_pid);
            break;
        case 202:
            output_vel = pid_control(enc_chassis_202_vel, target_vel, &chassis_202_pid);
            break;
        case 203:
            output_vel = pid_control(enc_chassis_203_vel, target_vel, &chassis_203_pid);
            break;
        case 204:
            output_vel = pid_control(enc_chassis_204_vel, target_vel, &chassis_204_pid);
            break;
        default:
            break;
    }
    return output_vel;
}

void control_task_gimbal(void) {
    // yaw
    //float yaw_pos_change = pid_control(yaw_angle, 0.0, &yaw_pos_pid);
    //float yaw_vel_change = pid_control(MPU6050_Real_Data.Gyro_Y, 0.0, &yaw_vel_pid);
    float yaw_vel_change = 0.0;

    // pitch
    float pitch_pos_change = pid_control(pitch_angle, 0.0, &pitch_pos_pid);
    float pitch_vel_change = pid_control(MPU6050_Real_Data.Gyro_Y, pitch_pos_change, &pitch_vel_pid);

    // limit the output
    if (pitch_vel_change > (float) ESC_MAX) {
        pitch_vel_change = (float) ESC_MAX;
    }
    
    if (pitch_vel_change < (float) -ESC_MAX) {
        pitch_vel_change = (float) -ESC_MAX;
    }
    // limit_range(&yaw_vel_change, -ESC_MAX, ESC_MAX);
    // limit_range(&pitch_vel_change, -ESC_MAX, ESC_MAX);

    // send the output
    send_gimbal((int16_t) pitch_vel_change, (int16_t) yaw_vel_change);
}

static float pid_control(float measured, float target, PID_Regulator_t * pid) {
    static float error[2] = {0.0, 0.0}; // note: static init retains value
    static float output = 0;
    static float inte = 0;

    error[0] = error[1]; // previous error
    error[1] = target - measured; // current error
    inte += error[1];

    output = pid->kp * error[1]
           + pid->ki * inte
           + pid->kd * (error[1] - error[0]);
    output = output * pid->sign;

    return (float) output;
}

void map(int16_t* val, int in_min, int in_max, int out_min, int out_max) {
    *val = (*val - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void limit_range(int16_t* value, int min, int max) {
    if (*value < min) {
        *value = min;
    }
    if (*value > max) {
        *value = max;
    }
}
