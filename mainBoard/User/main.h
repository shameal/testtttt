#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm32f4xx.h"

// mpu includes
#include "mpu6050_i2c.h"
#include "mpu6050_interrupt.h"
#include "mpu6050_driver.h"
#include "mpu6050_process.h"
#include "imu.h"

#include "bsp.h"

// user includes
#include "can_bus_task.h"
#include "control_task.h"
#include "remote_task.h"

#include <stdio.h>
#include <string.h>
#include <math.h>

// private defines
#define DEBUG_MODE_OFF (0)
#define DEBUG_MODE_GIMBAL (1)
#define DEBUG_MODE_USER (128)

// upload configuration
#define DEBUG_MODE (DEBUG_MODE_OFF)

// target hardware defines
#define ENC_YAW_OFFSET_DEG (0.0)
#define ENC_PITCH_OFFSET_DEG (76.8)

#endif
