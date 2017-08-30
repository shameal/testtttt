#include <Servo.h>

// configuration defines
#define TPZ_TIMER_PERIOD (20)
#define RPI_TIMER_PERIOD (100)
#define AUX_TIMER_PERIOD (20)

// comm headers
#define TPZ_IN_HEADER (0x7A)
#define RPI_OUT_HEADER (0x7B)
#define RPI_IN_HEADER (0x7C)
#define TPZ_OUT_HEADER (0x7D)
#define AUX_OUT_HEADER (0x7E)

// pin definitions
#define LED_PIN (13)
#define FRICTION1_ESC_PIN (2)
#define FRICTION2_ESC_PIN (3)
#define BOLT_SERVO_PIN (8)

#define LED_ON() digitalWrite(LED_PIN, HIGH)
#define LED_OFF() digitalWrite(LED_PIN, LOW)
#define L_BYTE(b) (b >> 8) & 255
#define H_BYTE(b) (b & 255)
#define MAKE_INT16(a, b) (((int16_t) a << 8)) | (b & 255)

/**** rc switch definition ****/
#define RC_SW_UP                        ((uint16_t)1)
#define RC_SW_MID                       ((uint16_t)3)
#define RC_SW_DOWN                      ((uint16_t)2)

/**** pc key definition ****/
#define KEY_PRESSED_OFFSET_W            ((uint16_t)0x01<<0)
#define KEY_PRESSED_OFFSET_S            ((uint16_t)0x01<<1)
#define KEY_PRESSED_OFFSET_A            ((uint16_t)0x01<<2)
#define KEY_PRESSED_OFFSET_D            ((uint16_t)0x01<<3)
#define KEY_PRESSED_OFFSET_Q            ((uint16_t)0x01<<4)
#define KEY_PRESSED_OFFSET_E            ((uint16_t)0x01<<5)
#define KEY_PRESSED_OFFSET_SHIFT        ((uint16_t)0x01<<6)
#define KEY_PRESSED_OFFSET_CTRL         ((uint16_t)0x01<<7)

// app defines
#define FIRE_TIMER_PERIOD (200)
#define BOLT_STATE_OUT_PWM (1090)
#define BOLT_STATE_IN_PWM (1400)

// com buffers
char tpz_in_buf[32];
char rpi_in_buf[32];
char tpz_out_buf[32];
char rpi_out_buf[32];
char aux_out_buf[32];

// data buffers
int16_t tpz_in_data[16];
int16_t rpi_in_data[16];

// timing variables
unsigned long tpz_timer_prev_time;
unsigned long rpi_timer_prev_time;
unsigned long aux_timer_prev_time;
unsigned long tpz_timer_cur_time;
unsigned long rpi_timer_cur_time;
unsigned long aux_timer_cur_time;

// application variables
Servo friction1_esc;
Servo friction2_esc;
Servo bolt_servo;
// state machines
int fire_state = 0;
int bolt_state = 0; // 0 is out, 1 is in
// fire timers
unsigned long fire_timer_prev_time;
unsigned long fire_timer_cur_time;

void setup() {
    pinMode(LED_PIN, OUTPUT);
    LED_OFF();
    Serial.begin(9600); // rpi
    Serial1.begin(115200); // tpz
    Serial2.begin(115200); // aux

    app_setup();
}

void loop() {
    // (1) receive from rpi
    if (Serial.available() > 0) {
        if (Serial.peek() != RPI_IN_HEADER) {
            Serial.read();
        } else {
            // rx into buffer
            Serial.readBytes((char*) rpi_in_buf, 32);

            // cleanup buffer
            for (int i = 0; i < 32; i++) {
                rpi_in_buf[i] = (rpi_in_buf[i] & 255);
            }

            // convert to int16 buffer
            for (int i = 0; i < 16; i++) {
                rpi_in_data[i] = ((int16_t) rpi_in_buf[i * 2] << 8) | (rpi_in_buf[i * 2 + 1] & 255);
            }
        }
    }

    // (2) receive from tpz
    if (Serial1.available() > 0) {
        if (Serial1.peek() != TPZ_IN_HEADER) {
            Serial1.read();
        } else {
            // rx into buffer
            Serial1.readBytes((char*) tpz_in_buf, 32);

            // cleanup buffer
            for (int i = 0; i < 32; i++) {
                tpz_in_buf[i] = (tpz_in_buf[i] & 255);
            }

            // convert to int16 buffer
            for (int i = 0; i < 16; i++) {
                tpz_in_data[i] = ((int16_t) tpz_in_buf[i * 2] << 8) | (tpz_in_buf[i * 2 + 1] & 255);
            }
        }
    }

    // (3) transmit to tpz
    tpz_timer_cur_time = millis();
    if (tpz_timer_cur_time - tpz_timer_prev_time >= TPZ_TIMER_PERIOD) {
        tpz_timer_prev_time = tpz_timer_cur_time;

        // assemble outgoing packet
        // clear the buffer
        tpz_out_buf[0] = TPZ_OUT_HEADER;
        Serial1.write((uint8_t*) tpz_out_buf, 32);
    }

    // (4) transmit to rpi
    rpi_timer_cur_time = millis();
    if (rpi_timer_cur_time - rpi_timer_prev_time >= RPI_TIMER_PERIOD) {
        rpi_timer_prev_time = rpi_timer_cur_time;

        // assemble outgoing packet
        // clear the buffer
        Serial.println(tpz_in_data[1], DEC);
    }

    // (5) transmit to aux
    aux_timer_cur_time = millis();
    if (aux_timer_cur_time - aux_timer_prev_time >= AUX_TIMER_PERIOD) {
        aux_timer_prev_time = aux_timer_cur_time;

        // assemble outgoing packet
        for (int i = 0; i < 32; i++) {
            aux_out_buf[i] = tpz_in_buf[i];
        }
        aux_out_buf[0] = AUX_OUT_HEADER;
        Serial2.write((uint8_t*) aux_out_buf, 32);
    }

    // perform application
    app_loop();
}

void app_setup() {
    bolt_servo_setup();
    friction_esc_setup();
}

void app_loop() {
    if (tpz_in_data[2] > 30) {
        LED_ON();
        fire_state = 1;
    } else {
        LED_OFF();
        fire_state = 0;
    }

    // fire task
    fire_timer_cur_time = millis();
    if (fire_timer_cur_time - fire_timer_prev_time >= FIRE_TIMER_PERIOD) {
        fire_timer_prev_time = fire_timer_cur_time;
        if (fire_state) {
            if (bolt_state) { // bolt state in
                bolt_servo.writeMicroseconds(BOLT_STATE_IN_PWM);
                bolt_state = 0;
            } else { // bolt state out
                bolt_servo.writeMicroseconds(BOLT_STATE_OUT_PWM);
                bolt_state = 1;
            }
        } else {
            bolt_servo.writeMicroseconds(BOLT_STATE_OUT_PWM);
        }
    }
    delay(1);
}

void bolt_servo_setup() {
    bolt_servo.attach(BOLT_SERVO_PIN);
    bolt_servo.writeMicroseconds(1000);
}

void friction_esc_setup() {
    friction1_esc.attach(FRICTION1_ESC_PIN);
    friction2_esc.attach(FRICTION2_ESC_PIN);
    friction1_esc.writeMicroseconds(1000);
    friction2_esc.writeMicroseconds(1000);
    delay(1000);
    friction1_esc.writeMicroseconds(1500);
    friction2_esc.writeMicroseconds(1500);
    delay(1000);
    friction1_esc.writeMicroseconds(1750);
    friction2_esc.writeMicroseconds(1750);
}