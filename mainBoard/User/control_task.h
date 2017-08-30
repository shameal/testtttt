#ifndef _CONTROL_TASK_H_
#define _CONTROL_TASK_H_

#define ESC_MAX (5000.0)
#define CHASSIS_ESC_MAX (5000)

typedef struct PID_Regulator_t {
    int sign;
    float kp;
    float ki;
    float kd;
} PID_Regulator_t;

#define CHASSIS_VEL_PID_DEFAULT {1, 0.6, 0.0, 0.0}

#define PITCH_POS_PID_DEFAULT {-1, 10.0, 0.0, 0.0}
#define PITCH_VEL_PID_DEFAULT {1, 18.5, 0.0, 0.0}

#define YAW_POS_PID_DEFAULT {1, 1.0, 0.0, 0.0}
#define YAW_VEL_PID_DEFAULT {1, 10.0, 0.0, 0.0}


void control_task(void);
void control_task_chassis(void);
int16_t chassis_pid_update(int can_address, int target_vel);
void control_task_gimbal(void);
static float pid_control(float measured, float target, PID_Regulator_t * pid);

void map(int16_t* val, int in_min, int in_max, int out_min, int out_max);
void limit_range(int16_t* value, int min, int max);

#endif
