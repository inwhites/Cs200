#ifndef INIT_H
#define INIT_H



#include <QLibrary>
#include<windows.h>
#include <iostream>



typedef INT(CALLBACK* USB_INI)(INT);
typedef INT(CALLBACK* USB_NUM)(VOID);
typedef INT(CALLBACK* USB_IO)(INT, LPSTR, INT, INT);
HINSTANCE hDll = LoadLibrary("Kmsecs200.dll"); //多字节字符集DEFINES  -= UNICODE QMAKE_CFLAGS += QMAKE_CXXFLAGS +=
USB_INI int_usb = (USB_INI)GetProcAddress(hDll, "int_usb");
USB_INI end_usb = (USB_INI)GetProcAddress(hDll, "end_usb");
USB_IO write64_usb = (USB_IO)GetProcAddress(hDll, "write64_usb");
USB_IO read64_usb = (USB_IO)GetProcAddress(hDll, "read64_usb");
USB_NUM get_num = (USB_NUM)GetProcAddress(hDll, "get_num");

static int openPipe=int_usb(0);
static int currentRow=0; //表格行控制
int x_coor = 0; //x—移动前的位置
int y_coor = 0;
int z_coor = 0;
int x_cur = 0; // x-移动后的位置
int y_cur = 0;
int z_cur = 0;
int pulse_cnt = 0; //脉冲数
boolean cancelFlag = true;
boolean openCom = true;
QString x_str = "0";
QString y_str = "0";
QString z_str= "0";
QString pulse_str = "0";
#endif // INIT_H 0 ';
