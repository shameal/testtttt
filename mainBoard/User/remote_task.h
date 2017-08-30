#ifndef _REMOTE_TASK_H_
#define _REMOTE_TASK_H_

#define CHECK_REMOTE_ON (RC_Ctl.rc.ch2 >= RC_CH_VALUE_MIN || RC_Ctl.rc.ch3 >= RC_CH_VALUE_MIN)

#define RAMP_LIMIT_NORMAL (1000)
#define RAMP_LIMIT_TURBO (2000)
#define RAMP_STEP_UP (1)   // value to increment during ramp
#define RAMP_STEP_DOWN (2) // (higher number = faster)

#define MOUSE_SENSITIVITY (10)

void remote_task(void);
void limit_range_v(volatile int16_t* value, int min, int max);

#endif
