#ifndef __MY_GUI_TOUCH_H
#define __MY_GUI_TOUCH_H

#include "sys.h"

//菜单栏显示触摸界面
void Gui_Disp_Menu(void); 
//方向界面
void Gui_Disp_Direction(void);
//时间界面
void Gui_Disp_Time(void);
//阈值界面
void Gui_Disp_Threshold(void);
//主题界面
void Gui_Disp_Theme(void);
//温度界面
void Gui_Disp_Temp(void);

//菜单界面键值处理
void Touch_Gui_Disp_Menu(void);
//方向界面键值处理
void Touch_Gui_Disp_Direction(void);
//时间界面键值处理
void Touch_Gui_Disp_Time(void);
//阈值界面键值处理
void Touch_Gui_Disp_Threshold(void);
//密码界面键值处理
void Touch_Gui_Disp_Password(void);
//密码设置界面键值处理
void Touch_Gui_Set_Password(void);
//在FLASH中读取密码
u8 FLASH_Read_Password(void);
//向FLASH中写入密码
u8 FLASH_Write_Password(void);

//时间光标闪烁函数
void Gui_Currsor_Blinks_Time(void);
u8 RTC_Set_Time(void);

#endif






