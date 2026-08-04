#include "touch.h"
#include "ctiic.h"
#include "lcd.h"
#include "delay.h"
#include <stdio.h>
#include <string.h>

_m_tp_dev tp_dev;

static const u16 GT_TPX_TBL[5] = {0x8150, 0x8158, 0x8160, 0x8168, 0x8170};

u8 GT9147_WR_Reg(u16 reg, u8 *buf, u8 len)
{
    u8 i, ret=0;
    CT_IIC_Start(); CT_IIC_Send_Byte(GT_CMD_WR); CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg>>8); CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg&0XFF); CT_IIC_Wait_Ack();
    for(i=0;i<len;i++) { CT_IIC_Send_Byte(buf[i]); ret=CT_IIC_Wait_Ack(); if(ret)break; }
    CT_IIC_Stop(); return ret;
}

void GT9147_RD_Reg(u16 reg, u8 *buf, u8 len)
{
    u8 i;
    CT_IIC_Start(); CT_IIC_Send_Byte(GT_CMD_WR); CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg>>8); CT_IIC_Wait_Ack();
    CT_IIC_Send_Byte(reg&0XFF); CT_IIC_Wait_Ack();
    CT_IIC_Start(); CT_IIC_Send_Byte(GT_CMD_RD); CT_IIC_Wait_Ack();
    for(i=0;i<len;i++) buf[i]=CT_IIC_Read_Byte(i==(len-1)?0:1);
    CT_IIC_Stop();
}

u8 GT9147_Init(void)
{
    u8 temp[5];
    GPIO_InitTypeDef g;

    __HAL_RCC_GPIOH_CLK_ENABLE(); __HAL_RCC_GPIOI_CLK_ENABLE();

    g.Pin=GPIO_PIN_7; g.Mode=GPIO_MODE_INPUT; g.Pull=GPIO_PULLUP;
    g.Speed=GPIO_SPEED_FREQ_VERY_HIGH; HAL_GPIO_Init(GPIOH, &g);

    g.Pin=GPIO_PIN_8; g.Mode=GPIO_MODE_OUTPUT_PP;
    HAL_GPIO_Init(GPIOI, &g);

    CT_IIC_Init();
    GT_RST(0); delay_ms(10); GT_RST(1); delay_ms(10);

    g.Pin=GPIO_PIN_7; g.Pull=GPIO_NOPULL; HAL_GPIO_Init(GPIOH, &g);
    delay_ms(100);

    GT9147_RD_Reg(GT_PID_REG, temp, 4); temp[4]=0;
    /* 检查支持: GT911/GT9147/GT1158/GT9271 */
    if(strcmp((char*)temp,"911") && strcmp((char*)temp,"9147")
    && strcmp((char*)temp,"1158") && strcmp((char*)temp,"9271")) {
        return 1;
    }

    /* 启用触摸（不写配置表，保持芯片出厂/已存配置） */
    temp[0]=0X02; GT9147_WR_Reg(GT_CTRL_REG,temp,1);  /* 关闭触摸 */
    delay_ms(10);
    temp[0]=0X00; GT9147_WR_Reg(GT_CTRL_REG,temp,1);  /* 开启触摸 */
    return 0;
}

u8 GT9147_Scan(u8 mode)
{
    u8 buf[4], i=0, res=0, temp, tempsta;
    static u8 t=0;
    t++;
    if((t%10)==0||t<10) {
        GT9147_RD_Reg(GT_GSTID_REG,&mode,1);
        if(mode&0X80&&((mode&0XF)<6)) { temp=0; GT9147_WR_Reg(GT_GSTID_REG,&temp,1); }
        if((mode&0XF)&&((mode&0XF)<6)) {
            temp=0XFF<<(mode&0XF);
            tempsta=tp_dev.sta;
            tp_dev.sta=(~temp)|TP_PRES_DOWN|TP_CATH_PRES;
            tp_dev.x[4]=tp_dev.x[0]; tp_dev.y[4]=tp_dev.y[0];
            for(i=0;i<5;i++) {
                if(tp_dev.sta&(1<<i)) {
                    GT9147_RD_Reg(GT_TPX_TBL[i],buf,4);
                    if(lcddev.id==0X5510) {
                        if(tp_dev.touchtype&0X01) {
                            tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
                            tp_dev.x[i]=800-(((u16)buf[3]<<8)+buf[2]);
                        } else {
                            tp_dev.x[i]=((u16)buf[1]<<8)+buf[0];
                            tp_dev.y[i]=((u16)buf[3]<<8)+buf[2];
                        }
                    } else {
                        tp_dev.x[i]=((u16)buf[1]<<8)+buf[0];
                        tp_dev.y[i]=((u16)buf[3]<<8)+buf[2];
                    }
                }
            }
            res=1;
            if(tp_dev.x[0]>lcddev.width||tp_dev.y[0]>lcddev.height) {
                if((mode&0XF)>1) { tp_dev.x[0]=tp_dev.x[1]; tp_dev.y[0]=tp_dev.y[1]; t=0; }
                else { tp_dev.x[0]=tp_dev.x[4]; tp_dev.y[0]=tp_dev.y[4]; mode=0X80; tp_dev.sta=tempsta; }
            } else t=0;
        }
    }
    if((mode&0X8F)==0X80) {
        if(tp_dev.sta&TP_PRES_DOWN) tp_dev.sta&=~(1<<7);
        else { tp_dev.x[0]=0xFFFF; tp_dev.y[0]=0xFFFF; tp_dev.sta&=0XE0; }
    }
    if(t>240) t=10;
    return res;
}

u8 TP_Init(void)
{
    tp_dev.init = GT9147_Init;
    tp_dev.scan = GT9147_Scan;
    tp_dev.touchtype = 0X81;  /* 横屏模式：XY交换坐标映射 */
    if(tp_dev.init()==0) { return 0; }
    return 1;
}

u8 TP_Scan(u8 tp) { return tp_dev.scan(tp); }
