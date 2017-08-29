#ifndef __USART3_H__
#define __USART3_H__

#include <stm32f4xx.h>
#include <stdio.h>
#include "main.h"

#define MAKE_INT16(a, b) (((int16_t) a << 8)) | (b & 255)
#define TX_MSB(a) (a >> 8) & 255
#define TX_LSB(a) a & 255

typedef struct {
    struct { 
        int16_t header;
        int16_t rpireq_drive;
        int16_t rpireq_strafe;
        int16_t rpireq_rotate;
        int16_t rpireq_pitch;
        int16_t rpireq_yaw;
        int16_t rpireq_fire;
    } packet;
} arduino_data;

/**** forward declarations ****/
void USART3_Configuration(void);
void USART3_SendChar(unsigned char);
void RS232_Print( USART_TypeDef*, u8* );
void RS232_VisualScope_USART3( USART_TypeDef*, u8*, u16 );
#endif
