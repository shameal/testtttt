#include "main.h"

extern volatile RC_Ctl_t RC_Ctl;

// global control information req
volatile int16_t cireq_drive;  // -660 ~ 660
volatile int16_t cireq_strafe; // -660 ~ 660
volatile int16_t cireq_rotate; // -660 ~ 660
volatile int16_t cireq_pitch;  // -660 ~ 660
volatile int16_t cireq_yaw;    // -660 ~ 660
volatile int16_t cireq_fire;   // 0 ~ 1
volatile int16_t control_state;

// calculated pitch and yaw degrees
volatile float cireq_pitch_deg; // -180 ~ 180
volatile float cireq_yaw_deg;   // -180 ~ 180

// private variables
volatile int drive_ramp;
volatile int strafe_ramp;
volatile int rotate_ramp;

int ramp_limit;

// remote task runs on tim6 interrupt
void remote_task() {
    if (CHECK_REMOTE_ON) {
        /**** translate gimbal commands ****/
        cireq_pitch = (RC_Ctl.rc.ch1 - RC_CH_VALUE_OFFSET); // -660 ~ 660
        cireq_yaw = (RC_Ctl.rc.ch0 - RC_CH_VALUE_OFFSET); // -660 ~ 660

        cireq_pitch_deg = cireq_pitch * (180.0 / 660.0);
        cireq_yaw_deg = cireq_yaw * (180.0 / 660.0);

        /**** translate keyboard mouse commands ****/
        if (RC_Ctl.key.v & KEY_PRESSED_OFFSET_Q) { // Q is actually SHIFT
            ramp_limit = RAMP_LIMIT_TURBO;
        } else {
            ramp_limit = RAMP_LIMIT_NORMAL;
        }

        if (RC_Ctl.key.v & KEY_PRESSED_OFFSET_W) {
            if (drive_ramp < ramp_limit) {drive_ramp+=RAMP_STEP_UP;}
            if (drive_ramp > ramp_limit) {drive_ramp-=RAMP_STEP_DOWN;}
        } else {
            if (drive_ramp > 0) {drive_ramp-=RAMP_STEP_DOWN;}
        }
        if (RC_Ctl.key.v & KEY_PRESSED_OFFSET_S) {
            if (drive_ramp > -(ramp_limit)) {drive_ramp-=RAMP_STEP_UP;}
            if (drive_ramp < -(ramp_limit)) {drive_ramp+=RAMP_STEP_DOWN;}
        } else {
            if (drive_ramp < 0) {drive_ramp+=RAMP_STEP_DOWN;}
        }

        if (RC_Ctl.key.v & KEY_PRESSED_OFFSET_D) {
            if (strafe_ramp < ramp_limit) {strafe_ramp+=RAMP_STEP_UP;}
            if (strafe_ramp > ramp_limit) {strafe_ramp-=RAMP_STEP_DOWN;}
        } else {
            if (strafe_ramp > 0) {strafe_ramp-=RAMP_STEP_DOWN;}
        }
        if (RC_Ctl.key.v & KEY_PRESSED_OFFSET_A) {
            if (strafe_ramp > -(ramp_limit)) {strafe_ramp-=RAMP_STEP_UP;}
            if (strafe_ramp < -(ramp_limit)) {strafe_ramp+=RAMP_STEP_DOWN;}
        } else {
            if (strafe_ramp < 0) {strafe_ramp+=RAMP_STEP_DOWN;}
        }

        if (RC_Ctl.mouse.press_l) {
            cireq_fire = 1;
        } else {
            cireq_fire = 0;
        }

        /**** translate chassis commands ****/
        cireq_drive = (RC_Ctl.rc.ch3 - RC_CH_VALUE_OFFSET) + (int) drive_ramp * (660.0 / 2000.0);
        cireq_strafe = (RC_Ctl.rc.ch2 - RC_CH_VALUE_OFFSET) + (int) strafe_ramp * (660.0 / 2000.0);
        cireq_rotate = (RC_Ctl.rc.ch0 - RC_CH_VALUE_OFFSET) + RC_Ctl.mouse.x * MOUSE_SENSITIVITY;

        // limit commands
        limit_range_v(&cireq_drive, -660, 660);
        limit_range_v(&cireq_strafe, -660, 660);
        limit_range_v(&cireq_rotate, -660, 660);
    }
}

void limit_range_v(volatile int16_t* value, int min, int max) {
    if (*value < min) {
        *value = min;
    }
    if (*value > max) {
        *value = max;
    }
}
