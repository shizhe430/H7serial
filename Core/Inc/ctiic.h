#ifndef __CTIIC_H
#define __CTIIC_H

#include "sys.h"

/* I2C引脚: PH6(SCL), PI3(SDA) */
#define CT_SCL_PIN     GPIO_PIN_6
#define CT_SCL_PORT    GPIOH
#define CT_SDA_PIN     GPIO_PIN_3
#define CT_SDA_PORT    GPIOI

#define CT_SDA_IN()   do{ GPIOI->MODER &= ~(3<<(3*2)); GPIOI->MODER |= 0<<3*2; }while(0)
#define CT_SDA_OUT()  do{ GPIOI->MODER &= ~(3<<(3*2)); GPIOI->MODER |= 1<<3*2; }while(0)

#define CT_IIC_SCL(n) HAL_GPIO_WritePin(CT_SCL_PORT,CT_SCL_PIN,n?GPIO_PIN_SET:GPIO_PIN_RESET)
#define CT_IIC_SDA(n) HAL_GPIO_WritePin(CT_SDA_PORT,CT_SDA_PIN,n?GPIO_PIN_SET:GPIO_PIN_RESET)
#define CT_READ_SDA   HAL_GPIO_ReadPin(CT_SDA_PORT,CT_SDA_PIN)

void CT_IIC_Init(void);
void CT_IIC_Start(void);
void CT_IIC_Stop(void);
void CT_IIC_Send_Byte(u8 txd);
u8   CT_IIC_Read_Byte(unsigned char ack);
u8   CT_IIC_Wait_Ack(void);
void CT_IIC_Ack(void);
void CT_IIC_NAck(void);

#endif
