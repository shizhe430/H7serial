#include "oledfont.h"
#include "oled_1.h"
#include "main.h"  // HAL库必需


//==============================================================================
// 引脚定义（和你CubeMX配置的引脚必须一致！）
//==============================================================================
/* Pin mapping comes from main.h so the driver stays aligned with project GPIO labels. */

//==============================================================================
// 引脚操作宏（HAL库版本）
//==============================================================================
#define OLED_SCL_Set()   HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_SET)
#define OLED_SCL_Clr()   HAL_GPIO_WritePin(OLED_SCL_GPIO_Port, OLED_SCL_Pin, GPIO_PIN_RESET)

#define OLED_SDA_Set()   HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_SET)
#define OLED_SDA_Clr()   HAL_GPIO_WritePin(OLED_SDA_GPIO_Port, OLED_SDA_Pin, GPIO_PIN_RESET)

#define OLED_RES_Set()   HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_SET)
#define OLED_RES_Clr()   HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, GPIO_PIN_RESET)

#define OLED_DC_Set()    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET)
#define OLED_DC_Clr()    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET)

#define OLED_CS_Set()    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_SET)
#define OLED_CS_Clr()    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, GPIO_PIN_RESET)


// 饮水机UI缓存变量，仅上电初始化一次静态界面
u8 last_water_level = 0;
u8 last_water_out_state = 0;
u8 last_hot_cold_mode = 0;
u16 last_temp_val = 0;
u16 last_dev_id = 0;
u8 oled_init_draw = 0; // 0=未绘制静态文字，1=已绘制

//******************************************************************************
// 函数说明：OLED写入一个数据（模拟SPI）
//******************************************************************************
void OLED_WR_Bus(u8 dat)
{
	u8 i;
	OLED_CS_Clr();
	for(i=0;i<8;i++)
	{
		OLED_SCL_Clr();
		if(dat&0x80)
		{
			OLED_SDA_Set();
		}
		else
		{
			OLED_SDA_Clr();
		}
		OLED_SCL_Set();
		dat<<=1;
	}
	OLED_CS_Set();
}

//******************************************************************************
// 函数说明：OLED写入指令
//******************************************************************************
void OLED_WR_REG(u8 reg)
{
	OLED_DC_Clr();
  OLED_WR_Bus(reg);
  OLED_DC_Set();
}

//******************************************************************************
// 函数说明：OLED写入数据
//******************************************************************************
void OLED_WR_Byte(u8 dat)
{
  OLED_WR_Bus(dat);
}

//******************************************************************************
// 函数说明：列地址设置
//******************************************************************************
void Column_Address(u8 a,u8 b)
{
	if(USE_HORIZONTAL==0)
	{
	 OLED_WR_REG(0x15);
	 OLED_WR_Byte(0x20+a);
	 OLED_WR_Byte(0x20+b);
	}
	else
	{
	 OLED_WR_REG(0x15);
	 OLED_WR_Byte(0x18+a);
	 OLED_WR_Byte(0x18+b);
	}
}

//******************************************************************************
// 函数说明：行地址设置
//******************************************************************************
void Row_Address(u8 a,u8 b)
{
	OLED_WR_REG(0x75);
	OLED_WR_Byte(a);
	OLED_WR_Byte(b);
	OLED_WR_REG(0x5C);
}

//******************************************************************************
// 函数说明：OLED填充
//******************************************************************************
void OLED_Fill(u16 xstr,u8 ystr,u16 xend,u8 yend,u8 color)
{
	u8 x,y;
	xstr/=4;
	xend/=4;
	Column_Address(xstr,xend-1);
	Row_Address(ystr,yend-1);
	for(x=xstr;x<xend;x++)
	{
		for(y=ystr;y<yend;y++)
		{
			OLED_WR_Byte(color);
			OLED_WR_Byte(color);
    }
  }
}

//******************************************************************************
// 16x16汉字
//******************************************************************************
void OLED_ShowChinese16x16(u8 x, u8 y, const char *s, u8 sizey, u8 mode)
{
    u8 i,j,k,DATA=0;
    u16 HZnum;
    u16 TypefaceNum;
    const typFNT_UTF8_16 *pFont = utf8_tfont16;

    TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
    HZnum=utf8_tfont16_len;

    while(*s != '\0')
    {
        // 处理 UTF-8 编码的中文字符（固定占3字节）
        uint8_t utf8[3] = {
            (uint8_t)*s,
            (uint8_t)*(s+1),
            (uint8_t)*(s+2)
        };

        // 在 UTF-8 字库中查找汉字
        for(k=0;k<HZnum;k++)
        {
            if ((pFont[k].Utf8[0]==utf8[0]) &&
                (pFont[k].Utf8[1]==utf8[1]) &&
                (pFont[k].Utf8[2]==utf8[2]))
            {
                // 每个汉字单独设置地址
                Column_Address(x/4, x/4+sizey/4-1);
                Row_Address(y, y+sizey-1);

                for(i=0;i<TypefaceNum;i++)
                {
                    for(j=0;j<4;j++)
                    {
                        DATA=0;
                        if(pFont[k].Msk[i]&(0x01<<(j*2+0))) DATA=0xf0;
                        if(pFont[k].Msk[i]&(0x01<<(j*2+1))) DATA|=0x0f;
                        if(mode) OLED_WR_Byte(~DATA);
                        else OLED_WR_Byte(DATA);
                    }
                }
                x += sizey; // 下一个汉字右移16像素
                break;
            }
        }
        s += 3; // 移动指针，处理下一个UTF-8中文字符
    }
}
//******************************************************************************
// 24x24汉字
//******************************************************************************
void OLED_ShowChinese24x24(u8 x,u8 y,u8 *s,u8 sizey,u8 mode)
{
	u8 i,j,k,DATA=0;
	u16 HZnum;
	u16 TypefaceNum;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=tfont24_len;
	Column_Address(x/4,x/4+sizey/4-1);
	Row_Address(y,y+sizey-1);
	for(k=0;k<HZnum;k++)
	{
		if ((tfont24[k].Index[0]==*(s))&&(tfont24[k].Index[1]==*(s+1)))
		{
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<4;j++)
				{
					if(tfont24[k].Msk[i]&(0x01<<(j*2+0))) DATA=0xf0;
					if(tfont24[k].Msk[i]&(0x01<<(j*2+1))) DATA|=0x0f;
					if(mode) OLED_WR_Byte(~DATA);
					else OLED_WR_Byte(DATA);
					DATA=0;
				}
			}
		}
		continue;
	}
}

//******************************************************************************
// 32x32汉字
//******************************************************************************
void OLED_ShowChinese32x32(u8 x,u8 y,u8 *s,u8 sizey,u8 mode)
{
	u8 i,j,k,DATA=0;
	u16 HZnum;
	u16 TypefaceNum;
	TypefaceNum=(sizey/8+((sizey%8)?1:0))*sizey;
	HZnum=tfont32_len;
	Column_Address(x/4,x/4+sizey/4-1);
	Row_Address(y,y+sizey-1);
	for(k=0;k<HZnum;k++)
	{
		if ((tfont32[k].Index[0]==*(s))&&(tfont32[k].Index[1]==*(s+1)))
		{
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<4;j++)
				{
					if(tfont32[k].Msk[i]&(0x01<<(j*2+0))) DATA=0xf0;
					if(tfont32[k].Msk[i]&(0x01<<(j*2+1))) DATA|=0x0f;
					if(mode) OLED_WR_Byte(~DATA);
					else OLED_WR_Byte(DATA);
					DATA=0;
				}
			}
		}
		continue;
	}
}


//******************************************************************************
// 显示汉字串
//******************************************************************************
void OLED_ShowChinese(u8 x,u8 y,u8 *s,u8 sizey,u8 mode)
{
	while(*s!=0)
	{
		if(sizey==16) OLED_ShowChinese16x16(x,y,(const char *)s,sizey,mode);
		else if(sizey==24) OLED_ShowChinese24x24(x,y,s,sizey,mode);
		else if(sizey==32) OLED_ShowChinese32x32(x,y,s,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}

//******************************************************************************
// 显示字符
//******************************************************************************
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 sizey,u8 mode)
{
	u8 c,i,k,m,t=4,size2,data1=0,DATA=0;
	size2=(sizey/16+((sizey%16)?1:0))*sizey;
	c=chr-' ';
	Column_Address(x/4,x/4+sizey/8-1);
	Row_Address(y,y+sizey-1);
	for(i=0;i<size2;i++)
	{
		if(sizey==16)      data1=ascii_1608[c][i];
		else if(sizey==24) data1=ascii_2412[c][i];
		else if(sizey==32) data1=ascii_3216[c][i];

		if(sizey%16)
		{
			m=sizey/16+1;
			if(i%m) t=2; else t=4;
		}
		for(k=0;k<t;k++)
		{
			if(data1&(0x01<<(k*2+0))) DATA=0xf0;
			if(data1&(0x01<<(k*2+1))) DATA|=0x0f;
			if(mode) OLED_WR_Byte(~DATA);
			else OLED_WR_Byte(DATA);
			DATA=0;
		}
  }
}

//******************************************************************************
// 显示字符串
//******************************************************************************
void OLED_ShowString(u8 x,u8 y,u8 *dp,u8 sizey,u8 mode)
{
	while(*dp!='\0')
	{
	  OLED_ShowChar(x,y,*dp,sizey,mode);
		dp++;
		x+=sizey/2;
	}
}

//******************************************************************************
// 幂运算
//******************************************************************************
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;
	while(n--)result*=m;
	return result;
}

//******************************************************************************
// 显示数字
//******************************************************************************
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey,u8 mode)
{
	u8 t,temp;
	u8 enshow=0;
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(sizey/2)*t,y,' ',sizey,mode);
				continue;
			}else enshow=1;
		}
	 	OLED_ShowChar(x+(sizey/2)*t,y,temp+'0',sizey,mode);
	}
}

//******************************************************************************
// 显示灰度图
//******************************************************************************
void OLED_DrawBMP(u8 x,u8 y,u16 length,u8 width,const u8 BMP[],u8 mode)
{
	u16 i,num;
	length=(length/4+((length%4)?1:0))*4;
	num=length/2*width;
	x/=4;
	length/=4;
	Column_Address(x,x+length-1);
	Row_Address(y,y+width-1);
	for(i=0;i<num;i++)
	{
		if(mode) OLED_WR_Byte(~BMP[i]);
		else OLED_WR_Byte(BMP[i]);
	}
}

//******************************************************************************
// 显示单色图
//******************************************************************************
void OLED_DrawSingleBMP(u8 x,u8 y,u16 length,u8 width,const u8 BMP[],u8 mode)
{
	u8 k,DATA=0;
	u16 i,num;
	length=(length/8+((length%8)?1:0))*8;
	num=length*width/8;
	x/=4;
	length/=4;
	Column_Address(x,x+length-1);
	Row_Address(y,y+width-1);
	for(i=0;i<num;i++)
	{
		for(k=0;k<4;k++)
		{
			if(BMP[i]&(0x01<<(k*2+0))) DATA=0xf0;
			if(BMP[i]&(0x01<<(k*2+1))) DATA|=0x0f;
			if(mode) OLED_WR_Byte(~DATA);
			else OLED_WR_Byte(DATA);
			DATA=0;
		}
	}
}



/* @brief 全屏清屏 屏幕256宽 × 64高
/* @note 复用你现有 Column_Address、Row_Address、OLED_WR_Byte 底层接口
/*/
void OLED_Clear(void)
{
   // 整屏坐标：x起点0，x终点255；y起点0，y终点63
   OLED_Fill(0, 0, 256, 64, 0x00);
}

//******************************************************************************
// ====================== OLED初始化（HAL库版本）===============================
// 完全删除了标准库、删除锁SWD代码
//******************************************************************************
void OLED_Init(void)
{
	// 复位OLED
	OLED_RES_Clr();
	HAL_Delay(10);
	OLED_RES_Set();
	HAL_Delay(10);

	OLED_WR_REG(0xfd);
	OLED_WR_Byte(0x12);

	OLED_WR_REG(0xae);

	OLED_WR_REG(0x15);
	OLED_WR_Byte(0x1C);
	OLED_WR_Byte(0x5B);

	OLED_WR_REG(0x75);
	OLED_WR_Byte(0x00);
	OLED_WR_Byte(0x3F);

	OLED_WR_REG(0xa0);
	if(USE_HORIZONTAL) OLED_WR_Byte(0x16);
	else OLED_WR_Byte(0x04);

	OLED_WR_REG(0xa1); OLED_WR_Byte(0x00);
	OLED_WR_REG(0xa2); OLED_WR_Byte(0x00);
	OLED_WR_REG(0xa6);
	OLED_WR_REG(0xab); OLED_WR_Byte(0x01);
	OLED_WR_REG(0xb1); OLED_WR_Byte(0x74);
	OLED_WR_REG(0xb3); OLED_WR_Byte(0x91);
	OLED_WR_REG(0xb4); OLED_WR_Byte(0xa0); OLED_WR_Byte(0xB5);
	OLED_WR_REG(0xb6); OLED_WR_Byte(0x08);
	OLED_WR_REG(0xbb); OLED_WR_Byte(0x17);
	OLED_WR_REG(0xbe); OLED_WR_Byte(0x04);
	OLED_WR_REG(0xc1); OLED_WR_Byte(0xff);
	OLED_WR_REG(0xc7); OLED_WR_Byte(0x0f);
	OLED_WR_REG(0xca); OLED_WR_Byte(0x3f);
	OLED_WR_REG(0xd1); OLED_WR_Byte(0xA2); OLED_WR_Byte(0x20);
  OLED_WR_REG(0xb9);
  OLED_WR_REG(0x00);
  OLED_WR_REG(0xB8);
	OLED_WR_Byte(0x00);
	OLED_WR_Byte(0x0C);
	OLED_WR_Byte(0x18);
	OLED_WR_Byte(0x24);
	OLED_WR_Byte(0x30);
	OLED_WR_Byte(0x3C);
	OLED_WR_Byte(0x48);
	OLED_WR_Byte(0x54);
	OLED_WR_Byte(0x60);
	OLED_WR_Byte(0x6C);
	OLED_WR_Byte(0x78);
	OLED_WR_Byte(0x84);
	OLED_WR_Byte(0x90);
	OLED_WR_Byte(0x9C);
	OLED_WR_Byte(0xA8);

	OLED_Fill(0,0,256,64,0x00);

	OLED_WR_REG(0xaf);
}

/**
 * @brief 绘制固定静态文字（上电仅执行一次，全部标题带冒号）
 */
void Disp_DrawStatic(void)
{
    OLED_Fill(0,0,256,64,0x00);
    // 左上角固定ID标签 ID:
    OLED_ShowString(0, 0, (u8 *)"ID:", 16, 0);

    // 右上角固定 温度: C
    OLED_ShowChinese16x16(160, 0, "\xE6\xB8\xA9", 16, 0);
    OLED_ShowChinese16x16(176, 0, "\xE5\xBA\xA6", 16, 0);
    OLED_ShowString(192, 0, (u8 *)":", 16, 0);
    OLED_ShowChinese16x16(228, 0,"\x10\x10\x10", 16, 0);

    // 左下角固定 水位:
    OLED_ShowChinese16x16(0, 32, "\x02\x02\x02", 16, 0);  // 水
    OLED_ShowChinese16x16(16, 32, "\x06\x06\x06", 16, 0);  // 位
    OLED_ShowString(32, 32, (u8 *)":", 16, 0);

    // 右下角固定 状态:
    OLED_ShowChinese16x16(128, 32, "\x03\x03\x03", 16, 0); // 状
    OLED_ShowChinese16x16(144, 32, "\x04\x04\x04", 16, 0); // 态
    OLED_ShowString(160, 32, (u8 *)":", 16, 0);
}

/**
 * @brief 仅刷新变化的动态内容，无变化不刷新屏幕，彻底消除闪烁
 */
void Disp_DrawDynamic(u8 water_level, u8 water_out_state, u16 temp_val, u8 hot_cold_mode, u16 dev_id)
{
    // 1. ID数字区域更新（ID: 后面数字）
    if(dev_id != last_dev_id)
    {
        OLED_Fill(30, 0, 60, 16, 0x00);
        OLED_ShowNum(30, 0, dev_id, 3, 16, 0);
        last_dev_id = dev_id;
    }

    // 2. 冷热模式文字更新（热水:/冷水:）
    if(hot_cold_mode != last_hot_cold_mode)
    {
        OLED_Fill(100, 0, 148, 16, 0x00);
        if(hot_cold_mode == 1)
        {
            OLED_ShowChinese16x16(100, 0, "\x05\x05\x05", 16, 0);
            OLED_ShowChinese16x16(116, 0, "\x02\x02\x02", 16, 0);
        }
        else
        {
            OLED_ShowChinese16x16(100, 0, "\x0E\x0E\x0E", 16, 0);
            OLED_ShowChinese16x16(116, 0, "\x02\x02\x02", 16, 0);
        }
        last_hot_cold_mode = hot_cold_mode;
    }

    // 3. 温度数字更新（温度: 后面数字）
    if(temp_val != last_temp_val)
    {
        OLED_Fill(212, 0, 228, 16, 0x00);
        OLED_ShowNum(212, 0, temp_val, 2, 16, 0);
        last_temp_val = temp_val;
    }

    // 4. 水位状态文字更新（水位: 后面文字）
    if(water_level != last_water_level)
    {
        OLED_Fill(48, 32, 64, 48, 0x00);
        if(water_level == 1)
        {
            OLED_ShowChinese16x16(48, 32, "\x09\x09\x09", 16, 0);
            OLED_ShowChinese16x16(64, 32, "\x0A\x0A\x0A", 16, 0);
        }
        else
        {
            OLED_ShowChinese16x16(48, 32, "\x07\x07\x07", 16, 0);
            OLED_ShowChinese16x16(64, 32, "\x08\x08\x08", 16, 0);
        }
        last_water_level = water_level;
    }

    // 5. 出水状态文字更新（状态: 后面文字）
    if(water_out_state != last_water_out_state)
    {
        OLED_Fill(176, 32, 224, 48, 0x00);
        switch(water_out_state)
        {
            case 0: // 出水结束
                OLED_ShowChinese16x16(176, 32, "\x01\x01\x01", 16, 0);
                OLED_ShowChinese16x16(192, 32, "\x02\x02\x02", 16, 0);
                OLED_ShowChinese16x16(208, 32, "\x0C\x0C\x0C", 16, 0);
                OLED_ShowChinese16x16(224, 32, "\x0D\x0D\x0D", 16, 0);
                break;
            case 1: // 出水中
                OLED_ShowChinese16x16(176, 32, "\x01\x01\x01", 16, 0);
                OLED_ShowChinese16x16(192, 32, "\x02\x02\x02", 16, 0);
                OLED_ShowChinese16x16(208, 32, "\x0F\x0F\x0F", 16, 0);
                break;
            case 2: // 出水异常
                OLED_ShowChinese16x16(176, 32, "\x01\x01\x01", 16, 0);
                OLED_ShowChinese16x16(192, 32, "\x02\x02\x02", 16, 0);
                OLED_ShowChinese16x16(208, 32, "\x0B\x0B\x0B", 16, 0);
                OLED_ShowChinese16x16(224, 32, "\x0A\x0A\x0A", 16, 0);
                break;
        }
        last_water_out_state = water_out_state;
    }
}

/**
 * @brief 饮水机UI总刷新入口，对外调用接口
 * @param water_level: 水位 0=过低 1=正常
 * @param water_out_state: 出水状态 0=出水结束 1=出水中 2=出水异常
 * @param hot_cold_mode: 冷热模式 0=冷水 1=热水
 * @param temp_val: 实时水温
 * @param dev_id: 设备ID
 */
void Disp_DrinkerUI(u8 water_level, u8 water_out_state, u16 temp_val, u8 hot_cold_mode, u16 dev_id)
{
    // 上电首次运行绘制全部静态固定文字，之后不再执行
    if(oled_init_draw == 0)
    {
        Disp_DrawStatic();
        oled_init_draw = 1;
    }
    // 仅更新发生变化的区域
    Disp_DrawDynamic(water_level, water_out_state, temp_val, hot_cold_mode, dev_id);
}
