#ifndef _ST7735R_H
#define _ST7735R_H

#include "gpio.h"
    
/********************************************API接口函数************************************************

#define oled_init()                                st7735r_init()
#define oled_rectangle(x,y,w,h,rgb565)             st7735r_rectangle(x,y,w,h,rgb565)
#define oled_clear(rgb565)                         st7735r_clear(rgb565)
#define oled_point(x,y,rgb565)                     st7735r_piont(x,y,rgb565)
#define oled_char(x,y,ascii,color,bcolor)          st7735r_char(x,y,ascii,color,bcolor)
#define oled_string(x,y,string,color,bcolor)       st7735r_string(x,y,string,color,bcolor)
#define oled_num(x,y,num,color,bcolor)             st7735r_num(x,y,num,color,bcolor)
#define oled_numlen(x,y,num,max_len,color,bcolor)  st7735r_numlen(x,y,num,max_len,color,bcolor)

**********************************************END*******************************************************/
//如果8位数据段为0-7
#define DATA0_7         1

//定义默认方向(0...3)
#define ST7735R_DIR     0

//定义宽度与高度
#define ST7735R_H       (128)
#define ST7735R_W       (128)


//管脚定义
#define ST7735R_CS 	                  (PCout(0))
#define ST7735R_RST                   (PCout(1))	
#define ST7735R_RS 	                  (PCout(2))
#define ST7735R_WR 	                  (PCout(3))		  
#define ST7735R_RD 	                  (PCout(4))
#define ST7735R_D0                    (PAout(0))
#define ST7735R_D1                    (PAout(1))
#define ST7735R_D2                    (PAout(2))
#define ST7735R_D3                    (PAout(3))
#define ST7735R_D4                    (PAout(4))
#define ST7735R_D5                    (PAout(5))
#define ST7735R_D6                    (PAout(6))
#define ST7735R_D7                    (PAout(7))

#define ST7735R_CS_PORT               (GPIOC) 	 
#define ST7735R_RST_PORT              (GPIOC)	
#define ST7735R_RS_PORT 	            (GPIOC)
#define ST7735R_WR_PORT 	 	          (GPIOC)  
#define ST7735R_RD_PORT	              (GPIOC)
#define ST7735R_D_PORT                (GPIOA)

#define ST7735R_CS_Pin                (GPIO_Pin_0) 	 
#define ST7735R_RST_Pin               (GPIO_Pin_1)	
#define ST7735R_RS_Pin 	              (GPIO_Pin_2)
#define ST7735R_WR_Pin 	 	            (GPIO_Pin_3)  
#define ST7735R_RD_Pin 	              (GPIO_Pin_4)
#define ST7735R_D0_Pin                (GPIO_Pin_0)
#define ST7735R_D1_Pin                (GPIO_Pin_1)
#define ST7735R_D2_Pin                (GPIO_Pin_2)
#define ST7735R_D3_Pin                (GPIO_Pin_3)
#define ST7735R_D4_Pin                (GPIO_Pin_4)
#define ST7735R_D5_Pin                (GPIO_Pin_5)
#define ST7735R_D6_Pin                (GPIO_Pin_6)
#define ST7735R_D7_Pin                (GPIO_Pin_7)


extern void ST7735R_init(void);
extern void ST7735R_dir(u8 option);
extern void ST7735R_setregion(u16 x,u16 y,u16 w,u16 h);

//API函数接口
extern void st7735r_init(void);
extern void st7735r_rectangle(u16 x,u16 y,u16 w,u16 h,u16 rgb565);
extern void st7735r_clear(u16 rgb565);
extern void st7735r_piont(u16 x,u16 y,u16 rgb565);
extern void st7735r_char(u16 x,u16 y,u8 ascii,u16 color,u16 bcolor);
extern void st7735r_string(u16 x,u16 y,u8 *string,u16 color,u16 bcolor);
extern void st7735r_num(u16 x,u16 y,u32 num,u16 color,u16 bcolor);
extern void st7735r_numlen(u16 x,u16 y,u32 num,u8 max_len,u16 color,u16 bcolor);
extern void st7735r_nnum(u16 x,u16 y,int num,u8 max_len,u16 color,u16 bcolor);
//API 重命名
#define oled_init()                                st7735r_init()
#define oled_rectangle(x,y,w,h,rgb565)             st7735r_rectangle(x,y,w,h,rgb565)
#define oled_clear(rgb565)                         st7735r_clear(rgb565)
#define oled_point(x,y,rgb565)                     st7735r_piont(x,y,rgb565)
#define oled_char(x,y,ascii,color,bcolor)          st7735r_char(x,y,ascii,color,bcolor)
#define oled_string(x,y,string,color,bcolor)       st7735r_string(x,y,string,color,bcolor)
#define oled_num(x,y,num,color,bcolor)             st7735r_num(x,y,num,color,bcolor)
#define oled_numlen(x,y,num,max_len,color,bcolor)  st7735r_numlen(x,y,num,max_len,color,bcolor)
#define oled_nnum(x,y,num,max_len,color,bcolor)  st7735r_nnum(x,y,num,max_len,color,bcolor)
//写数据与指令用函数调用太慢，改用宏定义
//数据输出
#if  DATA0_7 == 1
   #define ST7735R_DATAOUT(data)          ST7735R_D_PORT->ODR=(ST7735R_D_PORT->ODR & 0xff00) | data;
#else
   #define ST7735R_DATAOUT(data)  do \
		                        {\
	                            ST7735R_D0=(data>>0)&0X01;\
	                            ST7735R_D1=(data>>1)&0X01;\
	                            ST7735R_D2=(data>>2)&0X01;\
	                            ST7735R_D3=(data>>3)&0X01;\
	                            ST7735R_D4=(data>>4)&0X01;\
	                            ST7735R_D5=(data>>5)&0X01;\
	                            ST7735R_D6=(data>>6)&0X01;\
	                            ST7735R_D7=(data>>7)&0X01;\
														}while(0)														
#endif
//写数据
#define ST7735R_WR_DATA(data) do \
														{\
                               ST7735R_RD=1;\
	                             ST7735R_RS=1;\
	                             ST7735R_CS=0;\
	                             ST7735R_DATAOUT(data);\
	                             ST7735R_WR=0;\
	                             ST7735R_WR=1;\
	                             ST7735R_CS=1;\
                            }while(0) 
//写u16数据
#define ST7735R_WR_Data(data)do{ST7735R_WR_DATA((u8)(data >> 8));ST7735R_WR_DATA((u8)data);}while(0)													
//写指令
#define ST7735R_WR_CMD(data)  do \
														{ \
                               ST7735R_RD=1; \
	                             ST7735R_RS=0; \
	                             ST7735R_CS=0; \
	                             ST7735R_DATAOUT(data); \
	                             ST7735R_WR=0; \
	                             ST7735R_WR=1; \
	                             ST7735R_CS=1; \
                            }while(0)  
														
#endif
