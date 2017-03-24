#ifndef __MY_GUI_TOUCH_H
#define __MY_GUI_TOUCH_H

#include "sys.h"

//�˵�����ʾ��������
void Gui_Disp_Menu(void); 
//�������
void Gui_Disp_Direction(void);
//ʱ�����
void Gui_Disp_Time(void);
//��ֵ����
void Gui_Disp_Threshold(void);
//�������
void Gui_Disp_Theme(void);
//�¶Ƚ���
void Gui_Disp_Temp(void);

//�˵������ֵ����
void Touch_Gui_Disp_Menu(void);
//��������ֵ����
void Touch_Gui_Disp_Direction(void);
//ʱ������ֵ����
void Touch_Gui_Disp_Time(void);
//��ֵ�����ֵ����
void Touch_Gui_Disp_Threshold(void);
//��������ֵ����
void Touch_Gui_Disp_Password(void);
//�������ý����ֵ����
void Touch_Gui_Set_Password(void);
//��FLASH�ж�ȡ����
u8 FLASH_Read_Password(void);
//��FLASH��д������
u8 FLASH_Write_Password(void);

//ʱ������˸����
void Gui_Currsor_Blinks_Time(void);
u8 RTC_Set_Time(void);

#endif






