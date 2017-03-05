/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __FONTS_H
#define __FONTS_H
#ifdef __cplusplus
 extern "C" {
#endif
#include <stdint.h>

typedef struct _tFont
{    
  uint8_t Width;
  uint8_t Height;
  uint8_t BytesPreChar;
  const uint8_t *table;  
} sFONT;          //���ڿ����ַ���С�Ľṹ��

extern sFONT Font24x48;
extern sFONT Font16x24;
extern sFONT Font12x24;
extern sFONT Font8x16;
extern sFONT Font8x12;
extern sFONT Font8x8;    //��ʾ�ַ��Ĵ�С

extern sFONT * pFontTable[];
#define LINE(x) ((x) * (((sFONT *)LCD_GetFont())->Height))
#ifdef __cplusplus
}
#endif 
#endif
