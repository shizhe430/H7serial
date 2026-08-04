#ifndef __TOUCH_H
#define __TOUCH_H
#include "sys.h"

#define TP_PRES_DOWN 0x80
#define TP_CATH_PRES 0x40
#define CT_MAX_TOUCH 10

/* GT9147引脚: PH7(INT), PI8(RST) */
#define GT_RST(n)  HAL_GPIO_WritePin(GPIOI,GPIO_PIN_8,n?GPIO_PIN_SET:GPIO_PIN_RESET)
#define GT_INT     HAL_GPIO_ReadPin(GPIOH,GPIO_PIN_7)

#define GT_CMD_WR  0X28
#define GT_CMD_RD  0X29

#define GT_CTRL_REG   0X8040
#define GT_CFGS_REG   0X8047
#define GT_CHECK_REG  0X80FF
#define GT_PID_REG    0X8140
#define GT_GSTID_REG  0X814E
#define GT_TP1_REG    0X8150

typedef struct {
    u8  (*init)(void);
    u8  (*scan)(u8);
    void (*adjust)(void);
    u16 x[CT_MAX_TOUCH];
    u16 y[CT_MAX_TOUCH];
    u16 sta;
    float xfac, yfac;
    short xoff, yoff;
    u8 touchtype;
} _m_tp_dev;

extern _m_tp_dev tp_dev;

u8 GT9147_Init(void);
u8 GT9147_Scan(u8 mode);
u8 GT9147_WR_Reg(u16 reg, u8 *buf, u8 len);
void GT9147_RD_Reg(u16 reg, u8 *buf, u8 len);
u8 TP_Init(void);
u8 TP_Scan(u8 tp);

#endif
