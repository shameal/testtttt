#include "main.h"
#include "led.h"

volatile unsigned char usart3_rx_buf[32];
volatile uint16_t usart3_tx_buf[32];
DMA_InitTypeDef dma_usart_3;
volatile arduino_data data_usart_3;

volatile extern int16_t cireq_drive;  // -660 ~ 660
volatile extern int16_t cireq_strafe; // -660 ~ 660
volatile extern int16_t cireq_rotate; // -660 ~ 660
volatile extern int16_t cireq_pitch;  // -660 ~ 660
volatile extern int16_t cireq_yaw;    // -660 ~ 660
volatile extern int16_t cireq_fire;   // 0 ~ 1
volatile extern int16_t control_state;

volatile extern RC_Ctl_t RC_Ctl;

/*-----USART3_TX-----PB10---*/
/*-----USART3_RX-----PB11---*/

void USART3_Configuration(void)
{
    USART_InitTypeDef usart3;
    GPIO_InitTypeDef  gpio;
    NVIC_InitTypeDef  nvic;

    /* -------------- Enable Module Clock Source ----------------------------*/
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB | RCC_AHB1Periph_DMA1,ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);

    /* -------------- Configure GPIO ---------------------------------------*/
    gpio.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_10;
    gpio.GPIO_Mode = GPIO_Mode_AF;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_Speed = GPIO_Speed_100MHz;
    gpio.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB,&gpio);

    usart3.USART_BaudRate = 115200;
    usart3.USART_WordLength = USART_WordLength_8b;
    usart3.USART_StopBits = USART_StopBits_1;
    //usart3.USART_Parity = USART_Parity_Even;
    usart3.USART_Parity = USART_Parity_No;
    usart3.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
    usart3.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_Init(USART3,&usart3);

    USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART3,ENABLE);
    //USART_DMACmd (USART3, USART_DMAReq_Rx, ENABLE);

    /* -------------- Configure NVIC ---------------------------------------*/
    nvic.NVIC_IRQChannel = USART3_IRQn;//DMA1_Stream1_IRQn;

    nvic.NVIC_IRQChannelPreemptionPriority = 1;
    nvic.NVIC_IRQChannelSubPriority = 1;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    /* -------------- Configure DMA -----------------------------------------*/
    // DMA_DeInit(DMA1_Stream1);
    // dma_usart_3.DMA_Channel = DMA_Channel_4;
    // dma_usart_3.DMA_PeripheralBaseAddr = (uint32_t)&(USART3->DR);
    // dma_usart_3.DMA_Memory0BaseAddr = (uint32_t)usart3_rx_buf;
    // dma_usart_3.DMA_DIR = DMA_DIR_PeripheralToMemory;
    // dma_usart_3.DMA_BufferSize = 32;
    // dma_usart_3.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // dma_usart_3.DMA_MemoryInc = DMA_MemoryInc_Enable;
    // dma_usart_3.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    // dma_usart_3.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    // dma_usart_3.DMA_Mode = DMA_Mode_Circular;
    // dma_usart_3.DMA_Priority = DMA_Priority_VeryHigh;
    // dma_usart_3.DMA_FIFOMode = DMA_FIFOMode_Disable;
    // dma_usart_3.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
    // dma_usart_3.DMA_MemoryBurst = DMA_Mode_Normal;
    // dma_usart_3.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
    // DMA_Init(DMA1_Stream1,&dma_usart_3);
    // DMA_ITConfig(DMA1_Stream1,DMA_IT_TC,ENABLE);
    // DMA_Cmd(DMA1_Stream1,ENABLE);
}

void USART3_SendChar(unsigned char b)
{
    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
    USART_SendData(USART3,b);
}

int fputc(int ch, FILE *f)
{
    while (USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
    USART_SendData(USART3, (uint8_t)ch);
    return ch;
}

void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
        //USART_ClearITPendingBit(USART3,USART_IT_RXNE);
        if ((char) USART_ReceiveData(USART3) == 0x7D) { // 1st header byte
            while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
            if ((char) USART_ReceiveData(USART3) == 0x00) { // 2nd header byte
                LED1_TOGGLE();
                // (1) store data into receive buffer
                usart3_rx_buf[0] = 0xFA;
                usart3_rx_buf[1] = 0x00;
                for (int i = 2; i < 32; i++) {
                    while (USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == RESET);
                    usart3_rx_buf[i] = (char) USART_ReceiveData(USART3);
                }

                // store received data into
                // data_usart_3.packet.header = MAKE_INT16(usart3_rx_buf[0], usart3_rx_buf[1]);
                // data_usart_3.packet.feeder_motor_state = usart3_rx_buf[2] & 255;
                // data_usart_3.packet.friction_motor_state = usart3_rx_buf[3] & 255;
                // data_usart_3.packet.pitch_req = MAKE_INT16(usart3_rx_buf[4], usart3_rx_buf[5]);
                // data_usart_3.packet.yaw_req = MAKE_INT16(usart3_rx_buf[6], usart3_rx_buf[7]);
                // data_usart_3.packet.feeder_motor_pwm = MAKE_INT16(usart3_rx_buf[8], usart3_rx_buf[9]);
                // data_usart_3.packet.friction_motor_pwm = MAKE_INT16(usart3_rx_buf[10], usart3_rx_buf[11]);
                // data_usart_3.packet.drive_req = MAKE_INT16(usart3_rx_buf[12], usart3_rx_buf[13]);
                // data_usart_3.packet.strafe_req = MAKE_INT16(usart3_rx_buf[14], usart3_rx_buf[15]);
                // data_usart_3.packet.rotate_req = MAKE_INT16(usart3_rx_buf[16], usart3_rx_buf[17]);
                // data_usart_3.packet.mpu_x = MAKE_INT16(usart3_rx_buf[20], usart3_rx_buf[21]);
                // data_usart_3.packet.mpu_y = MAKE_INT16(usart3_rx_buf[22], usart3_rx_buf[23]);
                // data_usart_3.packet.mpu_z = MAKE_INT16(usart3_rx_buf[24], usart3_rx_buf[25]);
                // data_usart_3.packet.js_real_chassis_out_power = MAKE_INT16(usart3_rx_buf[26], usart3_rx_buf[27]);
                data_usart_3.packet.header = MAKE_INT16(usart3_rx_buf[0], usart3_rx_buf[1]);
                data_usart_3.packet.rpireq_drive = MAKE_INT16(usart3_rx_buf[2], usart3_rx_buf[3]);
                data_usart_3.packet.rpireq_strafe = MAKE_INT16(usart3_rx_buf[4], usart3_rx_buf[5]);
                data_usart_3.packet.rpireq_rotate = MAKE_INT16(usart3_rx_buf[6], usart3_rx_buf[7]);
                data_usart_3.packet.rpireq_pitch = MAKE_INT16(usart3_rx_buf[8], usart3_rx_buf[9]);
                data_usart_3.packet.rpireq_yaw = MAKE_INT16(usart3_rx_buf[10], usart3_rx_buf[11]);
                data_usart_3.packet.rpireq_fire = MAKE_INT16(usart3_rx_buf[12], usart3_rx_buf[13]);

                // // (2) reply with new packet, debug must be false
                if (DEBUG_MODE == DEBUG_MODE_OFF) {
                    for (int i = 0; i < 32; i++) {
                        usart3_tx_buf[i] = 0x00;
                    }
                    usart3_tx_buf[0] = 0x7A;
                    usart3_tx_buf[2] = TX_MSB(cireq_pitch);
                    usart3_tx_buf[3] = TX_LSB(cireq_pitch);
                    usart3_tx_buf[4] = TX_MSB(cireq_yaw);
                    usart3_tx_buf[5] = TX_LSB(cireq_yaw);
                    usart3_tx_buf[6] = TX_MSB(cireq_fire);
                    usart3_tx_buf[7] = TX_LSB(cireq_fire);
                    usart3_tx_buf[8] = TX_MSB(control_state);
                    usart3_tx_buf[9] = TX_LSB(control_state);
                    usart3_tx_buf[10] = 0x00;
                    usart3_tx_buf[11] = 0x00;
                    usart3_tx_buf[12] = TX_MSB(RC_Ctl.rc.ch0);
                    usart3_tx_buf[13] = TX_LSB(RC_Ctl.rc.ch0);
                    usart3_tx_buf[14] = TX_MSB(RC_Ctl.rc.ch1);
                    usart3_tx_buf[15] = TX_LSB(RC_Ctl.rc.ch1);
                    usart3_tx_buf[16] = TX_MSB(RC_Ctl.rc.ch2);
                    usart3_tx_buf[17] = TX_LSB(RC_Ctl.rc.ch2);
                    usart3_tx_buf[18] = TX_MSB(RC_Ctl.rc.ch3);
                    usart3_tx_buf[19] = TX_LSB(RC_Ctl.rc.ch3);
                    usart3_tx_buf[20] = RC_Ctl.rc.s1;
                    usart3_tx_buf[21] = RC_Ctl.rc.s2;
                    usart3_tx_buf[22] = TX_MSB(RC_Ctl.mouse.x);
                    usart3_tx_buf[23] = TX_LSB(RC_Ctl.mouse.x);
                    usart3_tx_buf[24] = TX_MSB(RC_Ctl.mouse.y);
                    usart3_tx_buf[25] = TX_LSB(RC_Ctl.mouse.y);
                    usart3_tx_buf[26] = TX_MSB(RC_Ctl.mouse.z);
                    usart3_tx_buf[27] = TX_LSB(RC_Ctl.mouse.z);
                    usart3_tx_buf[28] = RC_Ctl.mouse.press_l;
                    usart3_tx_buf[29] = RC_Ctl.mouse.press_r;
                    usart3_tx_buf[30] = TX_MSB(RC_Ctl.key.v);
                    usart3_tx_buf[31] = TX_LSB(RC_Ctl.key.v);

                    // send the packet
                    for (int i = 0; i < 32; i++) {
                        USART3_SendChar(usart3_tx_buf[i]);
                    }
                }
            }
        }
    }
}
