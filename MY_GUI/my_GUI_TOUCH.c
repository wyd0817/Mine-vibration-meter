#include "my_GUI_TOUCH.h"
#include "lcd.h"
#include "touch.h"

#include "config.h"
#include "sys.h"
#include "delay.h"
#include "usart.h" 
#include "led.h" 		 	 
#include "lcd.h"  
#include "key.h"     
#include "usmart.h" 
#include "malloc.h"
#include "sdio_sdcard.h"     
#include "ff.h"  
#include "exfuns.h" 
#include "rtc.h" 
#include "adxl345.h"
#include "beep.h"
#include "touch.h"

u8 Password[]="      ";
u8 Flag_First=0;


u8 X_Temp[]="X_CRT=00.0";
u8 Y_Temp[]="Y_CRT=00.0";
u8 Z_Temp[]="Z_CRT=00.0";
extern float X_Critical,Y_Critical,Z_Critical;
	
	
u8 Cycle_Time=0;
u8 Temp_position=0;//光标位置

extern _m_tp_dev tp_dev;


extern short x,y,z; 

//RTC使用变量
extern u8 t;
extern u8 Time_a[];
extern u8 Time_b[];
extern _calendar_obj calendar;//时钟结构体 

extern u8 Vertical_Direction;

//刷新时间函数
void Gui_Refresh_Time(void)        
{
	if(t!=calendar.sec)
	{
		t=calendar.sec;

		Time_a[2] = calendar.w_year%100/10+'0';
		Time_a[3] = calendar.w_year%10+'0';
		Time_a[5] = calendar.w_month/10+'0';
		Time_a[6] = calendar.w_month%10+'0';
		Time_a[8] = calendar.w_date/10+'0';
		Time_a[9] = calendar.w_date%10+'0';

		Time_b[0] = calendar.hour/10+'0';
		Time_b[1] = calendar.hour%10+'0';
		Time_b[3] = calendar.min/10+'0';
		Time_b[4] = calendar.min%10+'0';

		LCD_ShowString( 40,52,120,68,16,Time_a);
		LCD_ShowString(220,52,260,68,16,Time_b);
		
	}		
}
//时间光标闪烁函数
void Gui_Currsor_Blinks_Time(void)        
{
	switch(Cycle_Time)			     				
	{		
		
		case 0:LCD_ShowChar(220+8*4,52,Time_b[4],16,0);LCD_ShowChar(40+8*1,52,Time_a[1],16,0); 
			if(calendar.sec%2==0)	LCD_ShowChar(40,52,Time_a[0],16,0);
			else  					LCD_Color_Fill( 40,52,40+7,52+15,BLACK);
			break;	
		case 1:LCD_ShowChar(40,52,Time_a[0],16,0);LCD_ShowChar(40+8*2,52,Time_a[2],16,0); 
			if(calendar.sec%2==0)	LCD_ShowChar(40+8*1,52,Time_a[1],16,0);
			else  					LCD_Color_Fill( 40+8*1,52,40+8*1+7,52+15,BLACK);
			break;	
		case 2:LCD_ShowChar(40+8*1,52,Time_a[1],16,0);LCD_ShowChar(40+8*3,52,Time_a[3],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(40+8*2,52,Time_a[2],16,0);
			else  					LCD_Color_Fill( 40+8*2,52,40+8*2+7,52+15,BLACK);
			break;	
		case 3:LCD_ShowChar(40+8*2,52,Time_a[2],16,0);LCD_ShowChar(40+8*5,52,Time_a[5],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(40+8*3,52,Time_a[3],16,0);
			else  					LCD_Color_Fill( 40+8*3,52,40+8*3+7,52+15,BLACK);
			break;	
		case 4:LCD_ShowChar(40+8*3,52,Time_a[3],16,0);LCD_ShowChar(40+8*6,52,Time_a[6],16,0); 
			if(calendar.sec%2==0)	LCD_ShowChar(40+8*5,52,Time_a[5],16,0);
			else  					LCD_Color_Fill( 40+8*5,52,40+8*5+7,52+15,BLACK);
			break;	
		case 5:LCD_ShowChar(40+8*5,52,Time_a[5],16,0);LCD_ShowChar(40+8*8,52,Time_a[8],16,0); 
			if(calendar.sec%2==0)	LCD_ShowChar(40+8*6,52,Time_a[6],16,0);
			else  					LCD_Color_Fill( 40+8*6,52,40+8*6+7,52+15,BLACK);
			break;	
		case 6: LCD_ShowChar(40+8*6,52,Time_a[6],16,0);LCD_ShowChar(40+8*9,52,Time_a[9],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(40+8*8,52,Time_a[8],16,0);
			else  					LCD_Color_Fill( 40+8*8,52,40+8*8+7,52+15,BLACK);
			break;	
		case 7:LCD_ShowChar(40+8*8,52,Time_a[8],16,0);LCD_ShowChar(220+8*0,52,Time_b[0],16,0); 
			if(calendar.sec%2==0)	LCD_ShowChar(40+8*9,52,Time_a[9],16,0);
			else  					LCD_Color_Fill( 40+8*9,52,40+8*9+7,52+15,BLACK);
			break;	
		case 8:LCD_ShowChar(40+8*9,52,Time_a[9],16,0);LCD_ShowChar(220+8*1,52,Time_b[1],16,0); 
			if(calendar.sec%2==0)	LCD_ShowChar(220+8*0,52,Time_b[0],16,0);
			else  					LCD_Color_Fill( 220+8*0,52,220+8*0+7,52+15,BLACK);
			break;	
		case 9:LCD_ShowChar(220+8*0,52,Time_b[0],16,0); LCD_ShowChar(220+8*3,52,Time_b[3],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(220+8*1,52,Time_b[1],16,0);
			else  					LCD_Color_Fill( 220+8*1,52,220+8*1+7,52+15,BLACK);
			break;	
		case 10:LCD_ShowChar(220+8*1,52,Time_b[1],16,0);LCD_ShowChar(220+8*4,52,Time_b[4],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(220+8*3,52,Time_b[3],16,0);
			else  					LCD_Color_Fill( 220+8*3,52,220+8*3+7,52+15,BLACK);
			break;	
		case 11:LCD_ShowChar(220+8*3,52,Time_b[3],16,0);LCD_ShowChar(40,52,Time_a[0],16,0); 
			if(calendar.sec%2==0)	LCD_ShowChar(220+8*4,52,Time_b[4],16,0);
			else  					LCD_Color_Fill( 220+8*4,52,220+8*4+7,52+15,BLACK);
			break;	
	}
}
//密码显示‘*’ 6位密码
void Gui_Currsor_Password(void)        
{
	switch(Cycle_Time)		
	{		
		case 0:LCD_ShowString(136,52,48,16,16,"      ");	
				break;	
		case 1:LCD_ShowString(136,52,48,16,16,"*     ");
				break;	
		case 2:LCD_ShowString(136,52,48,16,16,"**    ");
				break;	
		case 3:LCD_ShowString(136,52,48,16,16,"***   ");
				break;	
		case 4:LCD_ShowString(136,52,48,16,16,"****  ");
				break;	
		case 5:LCD_ShowString(136,52,48,16,16,"***** ");
				break;	
		case 6:LCD_ShowString(136,52,48,16,16,"******");
				break;	
	}
}
//密码光标闪烁函数
void Gui_Set_Password(void)        
{
	if(Flag_First==0)
	{
		switch(Cycle_Time)			     				
		{		
			case 0:LCD_ShowChar(136+8*5,52,Password[5],16,0);LCD_ShowChar(136+8*1,52,' ',16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136,52,Password[0],16,0);
				else  					LCD_Color_Fill( 136,52,136+7,52+15,BLACK);
				break;	
			case 1:LCD_ShowChar(136,52,Password[0],16,0);LCD_ShowChar(136+8*2,52,' ',16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136+8*1,52,Password[1],16,0);
				else  					LCD_Color_Fill( 136+8*1,52,136+8*1+7,52+15,BLACK);
				break;	
			case 2:LCD_ShowChar(136+8*1,52,Password[1],16,0);LCD_ShowChar(136+8*3,52,' ',16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136+8*2,52,Password[2],16,0);
				else  					LCD_Color_Fill( 136+8*2,52,136+8*2+7,52+15,BLACK);
				break;	
			case 3:LCD_ShowChar(136+8*2,52,Password[2],16,0);LCD_ShowChar(136+8*4,52,' ',16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136+8*3,52,Password[3],16,0);
				else  					LCD_Color_Fill( 136+8*3,52,136+8*3+7,52+15,BLACK);
				break;	
			case 4:LCD_ShowChar(136+8*3,52,Password[3],16,0);LCD_ShowChar(136+8*5,52,' ',16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136+8*4,52,Password[4],16,0);
				else  					LCD_Color_Fill( 136+8*4,52,136+8*4+7,52+15,BLACK);
				break;	
			case 5:LCD_ShowChar(136+8*4,52,Password[4],16,0);LCD_ShowChar(136,52,Password[0],16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136+8*5,52,Password[5],16,0);
				else  					LCD_Color_Fill( 136+8*5,52,136+8*5+7,52+15,BLACK);
				break;	
		}
	}
	else
	{
		switch(Cycle_Time)			     				
		{		
			case 0:LCD_ShowChar(136+8*5,52,Password[5],16,0);LCD_ShowChar(136+8*1,52,Password[1],16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136,52,Password[0],16,0);
				else  					LCD_Color_Fill( 136,52,136+7,52+15,BLACK);
				break;	
			case 1:LCD_ShowChar(136,52,Password[0],16,0);LCD_ShowChar(136+8*2,52,Password[2],16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136+8*1,52,Password[1],16,0);
				else  					LCD_Color_Fill( 136+8*1,52,136+8*1+7,52+15,BLACK);
				break;	
			case 2:LCD_ShowChar(136+8*1,52,Password[1],16,0);LCD_ShowChar(136+8*3,52,Password[3],16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136+8*2,52,Password[2],16,0);
				else  					LCD_Color_Fill( 136+8*2,52,136+8*2+7,52+15,BLACK);
				break;	
			case 3:LCD_ShowChar(136+8*2,52,Password[2],16,0);LCD_ShowChar(136+8*4,52,Password[4],16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136+8*3,52,Password[3],16,0);
				else  					LCD_Color_Fill( 136+8*3,52,136+8*3+7,52+15,BLACK);
				break;	
			case 4:LCD_ShowChar(136+8*3,52,Password[3],16,0);LCD_ShowChar(136+8*5,52,Password[5],16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136+8*4,52,Password[4],16,0);
				else  					LCD_Color_Fill( 136+8*4,52,136+8*4+7,52+15,BLACK);
				break;	
			case 5:LCD_ShowChar(136+8*4,52,Password[4],16,0);LCD_ShowChar(136,52,Password[0],16,0);
				if(calendar.sec%2==0)	LCD_ShowChar(136+8*5,52,Password[5],16,0);
				else  					LCD_Color_Fill( 136+8*5,52,136+8*5+7,52+15,BLACK);
				break;	
	}
}
}
//阈值光标闪烁函数
void Gui_Currsor_Blinks_Threshold(void)        
{
	switch(Cycle_Time)			     				
	{		
		case 0:LCD_ShowChar(288+8*3,52,Z_Temp[9],16,0);LCD_ShowChar(48+8*1,52,X_Temp[7],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(48,52,X_Temp[6],16,0);
			else  					LCD_Color_Fill( 48,52,48+7,52+15,BLACK);
			break;	
		case 1:LCD_ShowChar(48,52,X_Temp[6],16,0);LCD_ShowChar(48+8*3,52,X_Temp[9],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(48+8*1,52,X_Temp[7],16,0);
			else  					LCD_Color_Fill( 48+8*1,52,48+8*1+7,52+15,BLACK);
			break;	
		case 2:LCD_ShowChar(48+8*1,52,X_Temp[7],16,0);LCD_ShowChar(168,52,Y_Temp[6],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(48+8*3,52,X_Temp[9],16,0);
			else  					LCD_Color_Fill( 48+8*3,52,48+8*3+7,52+15,BLACK);
			break;	
		case 3:LCD_ShowChar(48+8*3,52,X_Temp[9],16,0);LCD_ShowChar(168+8*1,52,Y_Temp[7],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(168,52,Y_Temp[6],16,0);
			else  					LCD_Color_Fill( 168,52,168+7,52+15,BLACK);
			break;	
		case 4:LCD_ShowChar(168,52,Y_Temp[6],16,0);LCD_ShowChar(168+8*3,52,Y_Temp[9],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(168+8*1,52,Y_Temp[7],16,0);
			else  					LCD_Color_Fill( 168+8*1,52,168+8*1+7,52+15,BLACK);
			break;	
		case 5:LCD_ShowChar(168+8*1,52,Y_Temp[7],16,0);LCD_ShowChar(288,52,Z_Temp[6],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(168+8*3,52,Y_Temp[9],16,0);
			else  					LCD_Color_Fill( 168+8*3,52,168+8*3+7,52+15,BLACK);
			break;
		case 6:LCD_ShowChar(168+8*3,52,Y_Temp[9],16,0);LCD_ShowChar(288+8*1,52,Z_Temp[7],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(288,52,Z_Temp[6],16,0);
			else  					LCD_Color_Fill( 288,52,288+7,52+15,BLACK);
			break;	
		case 7:LCD_ShowChar(288,52,Z_Temp[6],16,0);LCD_ShowChar(288+8*3,52,Z_Temp[9],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(288+8*1,52,Z_Temp[7],16,0);
			else  					LCD_Color_Fill( 288+8*1,52,288+8*1+7,52+15,BLACK);
			break;	
		case 8:LCD_ShowChar(288+8*1,52,Z_Temp[7],16,0);LCD_ShowChar(48,52,X_Temp[6],16,0);
			if(calendar.sec%2==0)	LCD_ShowChar(288+8*3,52,Z_Temp[9],16,0);
			else  					LCD_Color_Fill( 288+8*3,52,288+8*3+7,52+15,BLACK);
			break;
		
	}
}
//菜单栏界面
void Gui_Disp_Menu(void)         
{
	LCD_Clear(WHITE);//清屏
	POINT_COLOR=BLACK; 
	LCD_DrawLine(0, 39, 319, 39);	//横
	LCD_DrawLine(0,139, 319,139);	//横
	LCD_DrawLine(106,39,106,239);	//竖
	LCD_DrawLine(214,39,214,239);	//竖
	
	
	POINT_COLOR=BLUE; 
	LCD_ShowString(144, 12,176,28,16,"MENU");//菜单
	LCD_ShowString( 17, 42+39, 17+8*9, 42+39+16,16,"Direction");//方向
	LCD_ShowString(143, 42+39,143+8*4, 42+39+16,16,"Time");//时间
	LCD_ShowString(231, 42+39,231+8*9, 42+39+16,16,"Threshold");//阈值
	LCD_ShowString( 21,142+39, 21+8*8,142+39+16,16,"Password");//主题
	LCD_ShowString(143,142+39,143+8*4,142+39+16,16,"Work");//工作
	LCD_ShowString(251,142+39,251+8*4,142+39+16,16,"ADXL345");//温度
	
}
//方向界面
void Gui_Disp_Direction(void)          
{
	LCD_Clear(WHITE);//清屏
	POINT_COLOR=BLACK; 
	LCD_DrawLine(0, 39, 319, 39);	//横
	LCD_DrawLine(0,139, 319,139);	//横
	LCD_DrawLine(106,39,106,239);	//竖
	LCD_DrawLine(214,39,214,239);	//竖
	
	
	POINT_COLOR=BLUE; 
	LCD_ShowString(124, 12,196,28,16,"DIRECTION");
	LCD_ShowString( 37, 42+39, 37+8*4, 42+39+16,16,"X_UP");
	LCD_ShowString(143, 42+39,143+8*4, 42+39+16,16,"Y_UP");
	LCD_ShowString(251, 42+39,251+8*9, 42+39+16,16,"Z_UP");
	LCD_ShowString( 29,142+39, 29+8*6,142+39+16,16,"X_DOWN");
	LCD_ShowString(135,142+39,135+8*6,142+39+16,16,"Y_DOWN");
	LCD_ShowString(243,142+39,243+8*4,142+39+16,16,"Z_DOWN");
}
//时间界面
void Gui_Disp_Time(void)        
{
	LCD_Clear(WHITE);//清屏
	POINT_COLOR=BLACK; 
	LCD_DrawLine(0, 39, 319, 39);	//横
	LCD_DrawLine(0, 79, 319, 79);	//横
	LCD_DrawLine(0,119, 319,119);	//横
	LCD_DrawLine(0,159, 319,159);	//横
	LCD_DrawLine(0,199, 319,199);	//横
	
	LCD_DrawLine( 79,119, 79,239);	//竖
	LCD_DrawLine(159, 79,159,239);	//竖
	LCD_DrawLine(239,119,239,239);	//竖
	
	
	POINT_COLOR=BLUE; 
	LCD_ShowString(144, 12,176,28,16,"Time");
	
	LCD_ShowString( 56, 92,104,108,16,"Cancel");
	LCD_ShowString(232, 92,392,108,16,"OK");
	
	LCD_ShowString( 35, 132, 35+8, 148,16,"1");
	LCD_ShowString(115, 132,115+8, 148,16,"2");
	LCD_ShowString(195, 132,195+8, 148,16,"3");
	LCD_ShowString(275, 132,275+8, 148,16,"<-");
	
	LCD_ShowString( 35, 172, 35+8, 188,16,"4");
	LCD_ShowString(115, 172,115+8, 188,16,"5");
	LCD_ShowString(195, 172,195+8, 188,16,"6");
	LCD_ShowString(275, 172,275+8, 188,16,"->");
	
	LCD_ShowString( 35, 212, 35+8, 228,16,"7");
	LCD_ShowString(115, 212,115+8, 228,16,"8");
	LCD_ShowString(195, 212,195+8, 228,16,"9");
	LCD_ShowString(275, 212,275+8, 228,16,"0");
	
	Gui_Refresh_Time();
		
}
//密码界面
void Gui_Disp_Password(void)        
{
	LCD_Clear(WHITE);//清屏
	POINT_COLOR=BLACK; 
	LCD_DrawLine(0, 39, 319, 39);	//横
	LCD_DrawLine(0, 79, 319, 79);	//横
	LCD_DrawLine(0,119, 319,119);	//横
	LCD_DrawLine(0,159, 319,159);	//横
	LCD_DrawLine(0,199, 319,199);	//横
	
	LCD_DrawLine( 79,119, 79,239);	//竖
	LCD_DrawLine(159, 79,159,239);	//竖
	LCD_DrawLine(239,119,239,239);	//竖
	
	
	POINT_COLOR=BLUE; 
	LCD_ShowString(48, 12,48+8*26,28,16,"Please enter your password");
	
	LCD_ShowString( 56, 92,104,108,16,"Cancel");
	LCD_ShowString(232, 92,392,108,16,"OK");
	
	LCD_ShowString( 35, 132, 35+8, 148,16,"1");
	LCD_ShowString(115, 132,115+8, 148,16,"2");
	LCD_ShowString(195, 132,195+8, 148,16,"3");
	LCD_ShowString(275, 132,275+8, 148,16,"<-");
	
	LCD_ShowString( 35, 172, 35+8, 188,16,"4");
	LCD_ShowString(115, 172,115+8, 188,16,"5");
	LCD_ShowString(195, 172,195+8, 188,16,"6");
	LCD_ShowString(275, 172,275+8, 188,16,"->");
	
	LCD_ShowString( 35, 212, 35+8, 228,16,"7");
	LCD_ShowString(115, 212,115+8, 228,16,"8");
	LCD_ShowString(195, 212,195+8, 228,16,"9");
	LCD_ShowString(275, 212,275+8, 228,16,"0");

}

//阈值界面
void Gui_Disp_Threshold(void)         
{
	LCD_Clear(WHITE);//清屏
	POINT_COLOR=BLACK; 
	LCD_DrawLine(0, 39, 319, 39);	//横
	LCD_DrawLine(0, 79, 319, 79);	//横
	LCD_DrawLine(0,119, 319,119);	//横
	LCD_DrawLine(0,159, 319,159);	//横
	LCD_DrawLine(0,199, 319,199);	//横
	
	LCD_DrawLine( 79,119, 79,239);	//竖
	LCD_DrawLine(159, 79,159,239);	//竖
	LCD_DrawLine(239,119,239,239);	//竖
	
	
	POINT_COLOR=BLUE; 
	LCD_ShowString(120, 12,200,28,16,"Threshold");
	
	LCD_ShowString( 56, 92,104,108,16,"Cancel");
	LCD_ShowString(232, 92,392,108,16,"OK");
	
	LCD_ShowString( 35, 132, 35+8, 148,16,"1");
	LCD_ShowString(115, 132,115+8, 148,16,"2");
	LCD_ShowString(195, 132,195+8, 148,16,"3");
	LCD_ShowString(275, 132,275+8, 148,16,"<-");
	
	LCD_ShowString( 35, 172, 35+8, 188,16,"4");
	LCD_ShowString(115, 172,115+8, 188,16,"5");
	LCD_ShowString(195, 172,195+8, 188,16,"6");
	LCD_ShowString(275, 172,275+8, 188,16,"->");
	
	LCD_ShowString( 35, 212, 35+8, 228,16,"7");
	LCD_ShowString(115, 212,115+8, 228,16,"8");
	LCD_ShowString(195, 212,195+8, 228,16,"9");
	LCD_ShowString(275, 212,275+8, 228,16,"0");
	
	X_Temp[6]=(int)(X_Critical*10)/ 100 % 10+'0';
	X_Temp[7]=(int)(X_Critical*10)/ 10 % 10+'0';
	X_Temp[9]=(int)(X_Critical*10)/ 1 % 10+'0';
	
	Y_Temp[6]=(int)(Y_Critical*10)/ 100 % 10+'0';
	Y_Temp[7]=(int)(Y_Critical*10)/ 10 % 10+'0';
	Y_Temp[9]=(int)(Y_Critical*10)/ 1 % 10+'0';
	
	Z_Temp[6]=(int)(Z_Critical*10)/ 100 % 10+'0';
	Z_Temp[7]=(int)(Z_Critical*10)/ 10 % 10+'0';
	Z_Temp[9]=(int)(Z_Critical*10)/ 1 % 10+'0';
	
	LCD_ShowString(  0,52,  0+8*10,68,16,X_Temp);
	LCD_ShowString(120,52,120+8*10,68,16,Y_Temp);
	LCD_ShowString(240,52,240+8*10,68,16,Z_Temp);
			
}
//主题界面
void Gui_Disp_Theme(void)        
{
	
}
//温度界面
void Gui_Disp_Temp(void)        
{
		
}

u8 Touch_Scan_Direction(void)
{
	tp_dev.scan(0); 
	if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
	{	
		if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)
		{	
			if(tp_dev.x>  0&&tp_dev.x<106&&tp_dev.y> 40&&tp_dev.y<140) return 0;
			if(tp_dev.x>106&&tp_dev.x<214&&tp_dev.y> 40&&tp_dev.y<140) return 1;
			if(tp_dev.x>214&&tp_dev.x<320&&tp_dev.y> 40&&tp_dev.y<140) return 2;
			if(tp_dev.x>  0&&tp_dev.x<106&&tp_dev.y>140&&tp_dev.y<240) return 3;
			if(tp_dev.x>106&&tp_dev.x<214&&tp_dev.y>140&&tp_dev.y<240) return 4;
			if(tp_dev.x>214&&tp_dev.x<320&&tp_dev.y>140&&tp_dev.y<240) return 5;
			else return 255;  			   
		}
	}else return 255;//没有按键按下的时候 	
}
//菜单界面键值处理
void Touch_Gui_Disp_Menu(void)	
{
	u8 value=255;
	Gui_Disp_Menu(); 	 //操作界面
	while(PEN==0)  //等待触摸释放
	{}
	while(1)
	{
		value=Touch_Scan_Direction();	   //获取触摸键值
		while(PEN==0)  //等待触摸释放
		{}
		
		switch(value)			     				 //相应键值的处理
		{	
			case 0:
				Touch_Gui_Disp_Direction();				
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Direction");
				printf("\r\n\r\n");
				#endif 
				break;
			case 1:
				Touch_Gui_Disp_Time();
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Time");
				printf("\r\n\r\n");
				#endif 
				break;
			case 2:
				Touch_Gui_Disp_Threshold();
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Threshold");
				printf("\r\n\r\n");
				#endif 
				break;
			case 3:
				Touch_Gui_Set_Password();
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Password");
				printf("\r\n\r\n");
				#endif 
				break;
			case 4:
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Work");
				printf("\r\n\r\n");
				#endif 
				break;
			case 5:
				LCD_Clear(WHITE);//清屏
				LCD_ShowString( 72, 52, 72+8*22, 68,16,"ADXL345 adjust...");
				ADXL345_AUTO_Adjust((char*)&x,(char*)&y,(char*)&z);//自动校准
				LCD_ShowString( 72, 72, 72+8*22, 68,16,"ADXL345 adjust successfully!");
				delay_ms(500);
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("ADXL345");
				printf("\r\n\r\n");
				#endif 
				break;
			default:	break;
		}
		if(value!=0xff)break;
		delay_ms(10);
	}
}
//方向界面键值处理
void Touch_Gui_Disp_Direction(void)	
{
	u8 value=255;
	Gui_Disp_Direction(); 	 //操作界面

	while(1)
	{
		value=Touch_Scan_Direction();	   //获取触摸键值
		while(PEN==0)  //等待触摸释放
		{}
	
		switch(value)			     				 //相应键值的处理
		{	
			case 0: 
				Vertical_Direction=1;
				Adxl345_Save_Adjdata();
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("X_UP");
				printf("\r\n\r\n");
				#endif 
				break;
			case 1:
				Vertical_Direction=3;
				Adxl345_Save_Adjdata();
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Y_UP");
				printf("\r\n\r\n");
				#endif 
				break;
			case 2:
				Vertical_Direction=5;
				Adxl345_Save_Adjdata();
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Z_UP");
				printf("\r\n\r\n");
				#endif 
				break;
			case 3:
				Vertical_Direction=2;
				Adxl345_Save_Adjdata();
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("X_DOWN");
				printf("\r\n\r\n");
				#endif 
				break;
			case 4:
				Vertical_Direction=4;
				Adxl345_Save_Adjdata();
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Y_DOWN");
				printf("\r\n\r\n");
				#endif 
				break;
			case 5:
				Vertical_Direction=6;
				Adxl345_Save_Adjdata();
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Z_DOWN");
				printf("\r\n\r\n");
				#endif 
				break;
			default:	break;
		}
		if(value!=0xff)break;
		delay_ms(10);
	}
}

u8 Touch_Scan_Time(void)
{
	tp_dev.scan(0); 
	if(tp_dev.sta&TP_PRES_DOWN)			//触摸屏被按下
	{	
		if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)
		{	
			if(tp_dev.x>  0&&tp_dev.x<160&&tp_dev.y> 80&&tp_dev.y<120) return 0;//Cancel
			if(tp_dev.x>160&&tp_dev.x<320&&tp_dev.y> 80&&tp_dev.y<120) return 1;//OK
			
			if(tp_dev.x>  0&&tp_dev.x< 80&&tp_dev.y>120&&tp_dev.y<160) return 2;//1
			if(tp_dev.x> 80&&tp_dev.x<160&&tp_dev.y>120&&tp_dev.y<160) return 3;//2
			if(tp_dev.x>160&&tp_dev.x<240&&tp_dev.y>120&&tp_dev.y<160) return 4;//3
			if(tp_dev.x>240&&tp_dev.x<320&&tp_dev.y>120&&tp_dev.y<160) return 5;//DEL
			
			
			if(tp_dev.x>  0&&tp_dev.x< 80&&tp_dev.y>160&&tp_dev.y<200) return 6;//4
			if(tp_dev.x> 80&&tp_dev.x<160&&tp_dev.y>160&&tp_dev.y<200) return 7;//5
			if(tp_dev.x>160&&tp_dev.x<240&&tp_dev.y>160&&tp_dev.y<200) return 8;//6
			if(tp_dev.x>240&&tp_dev.x<320&&tp_dev.y>160&&tp_dev.y<200) return 9;//.
			
			if(tp_dev.x>  0&&tp_dev.x< 80&&tp_dev.y>200&&tp_dev.y<240) return 10;//7
			if(tp_dev.x> 80&&tp_dev.x<160&&tp_dev.y>200&&tp_dev.y<240) return 11;//8
			if(tp_dev.x>160&&tp_dev.x<240&&tp_dev.y>200&&tp_dev.y<240) return 12;//9
			if(tp_dev.x>240&&tp_dev.x<320&&tp_dev.y>200&&tp_dev.y<240) return 13;//0
			else return 255;  			   
		}
	}else return 255;//没有按键按下的时候 	
}

//不判断备份区域直接设置时间
u8 RTC_Set_Time(void)
{
	//检查是不是第一次配置时钟
	_calendar_obj Temp_calendar;
	u8 temp=0;
	int sign=0,dayvalid;
 			
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问 
	BKP_DeInit();	//复位备份区域 	
	RCC_LSEConfig(RCC_LSE_ON);	//设置外部低速晶振(LSE),使用外设低速晶振
	while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
		{
		temp++;
		delay_ms(10);
		}
	if(temp>=250)return 1;//初始化时钟失败,晶振有问题	    
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
	RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	RTC_WaitForSynchro();		//等待RTC寄存器同步  
	RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
	RTC_EnterConfigMode();/// 允许配置	
	RTC_SetPrescaler(32767); //设置RTC预分频的值
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		
		
	Temp_calendar.w_year=(Time_a[0]-'0')*1000+(Time_a[1]-'0')*100+(Time_a[2]-'0')*10+(Time_a[3]-'0');
	Temp_calendar.w_month=(Time_a[5]-'0')*10+(Time_a[6]-'0');
	Temp_calendar.w_date=(Time_a[8]-'0')*10+(Time_a[9]-'0');
		
	Temp_calendar.hour=(Time_b[0]-'0')*10+(Time_b[1]-'0');
	Temp_calendar.min=(Time_b[3]-'0')*10+(Time_b[4]-'0');
		

	if((Temp_calendar.w_year<1970)||(Temp_calendar.w_year>2099)){printf("年份格式错误\n");return 1;}

	if ((Temp_calendar.w_year%4==0&&Temp_calendar.w_year%100!=0)||Temp_calendar.w_year%400==0)
	{
		sign =1;
	}
	if (Temp_calendar.w_month>=1&&Temp_calendar.w_month<=12)
	{
		switch (Temp_calendar.w_month)
		{
			case 1:
			case 3:
			case 5:
			case 7:
			case 8:
			case 10:
			case 12:
			dayvalid = 31;
			break;
			case 4:
			case 6:
			case 9:
			case 11:
			dayvalid = 30;
			break;
			case 2:
			if (sign ==1)
			dayvalid = 29;
			else
			dayvalid = 28;
			break;
			default:
			break;
		}
		if (!(Temp_calendar.w_date >0&&Temp_calendar.w_date <=dayvalid))
		{
			printf("日期格式错误\n");
			return 3;
			
		}
	}
	else
	{
		printf("月份格式错误：\n");
		return 2;
		
	}
	
	if((Temp_calendar.hour<0)||(Temp_calendar.hour>23)){printf("小时格式错误\n");return 4;}
	if((Temp_calendar.min<0)||(Temp_calendar.min>59)){printf("分钟格式错误\n");return 5;}

 
	RTC_Set(Temp_calendar.w_year,Temp_calendar.w_month,Temp_calendar.w_date,Temp_calendar.hour,Temp_calendar.min,0);  //设置时间	
	RTC_ExitConfigMode(); //退出配置模式  
	BKP_WriteBackupRegister(BKP_DR1, STORAGE_VALUE);	//向指定的后备寄存器中写入用户程序数据
	
	RTC_NVIC_Config();//RCT中断分组设置		    				     
	RTC_Get();//更新时间	
	return 0; //ok

}

//不判断直接往FLASH里设置阈值
u8 Flash_Set_Threshold(void)
{
	X_Critical=(X_Temp[6]-'0')*10+(X_Temp[7]-'0')*1+(X_Temp[9]-'0')*0.1;
	Y_Critical=(Y_Temp[6]-'0')*10+(Y_Temp[7]-'0')*1+(Y_Temp[9]-'0')*0.1;
	Z_Critical=(Z_Temp[6]-'0')*10+(Z_Temp[7]-'0')*1+(Z_Temp[9]-'0')*0.1;
	printf("%f\n\r%f\n\%f\n\r",X_Critical,Y_Critical,Z_Critical);
	if((X_Critical<0)||(X_Critical>16)) {printf("X方向阈值错误\n");return 1;}
	if((Y_Critical<0)||(Y_Critical>16)) {printf("Y方向阈值错误\n");return 2;}
	if((Z_Critical<0)||(Z_Critical>16)) {printf("Z方向阈值错误\n");return 3;}
	Adxl345_Save_Adjdata();	
	return 0; //ok
}

u32 Init_Password;
u8 Password_TEXT_Buffer[14];
#define PASSWORD_SIZE sizeof(Password_TEXT_Buffer)	 			  	//数组长度
#define PASSWORD_SAVE_ADDR  (0X08070040)			//设置FLASH 保存地址(必须为偶数)
u8 Password_datatemp[PASSWORD_SIZE];
//校验密码
u8 Flash_check_Password(void)
{
	u32 Password_Temp;
	FLASH_Read_Password();
	Password_Temp=(Password[0]-'0')*100000+(Password[1]-'0')*10000+(Password[2]-'0')*1000+(Password[3]-'0')*100+(Password[4]-'0')*10+(Password[5]-'0')*1;
	if(Password_Temp==Init_Password)	return 0; //ok
	else						return 1;
}


//向FLASH中写入密码
u8 FLASH_Write_Password(void)
{
	u8 *p1;
	if((Init_Password<0)||(Init_Password>999999)) {printf("The password is not valid!\r\n");return 1;}
	Password_TEXT_Buffer[0]=PASSWORD_VALUE;//标记校准过了
	p1 = (u8 *)&Init_Password;//将浮点型X_Critical变为四个字节
	Password_TEXT_Buffer[1] = *p1;
	p1++;
	Password_TEXT_Buffer[2] = *p1;
	p1++;
	Password_TEXT_Buffer[3] = *p1;
	p1++;
	Password_TEXT_Buffer[4] = *p1;
	
	STMFLASH_Write(PASSWORD_SAVE_ADDR,(u16*)Password_TEXT_Buffer,PASSWORD_SIZE);	
	
	return 0;
}


//向FLASH中写入密码2
u8 FLASH_Write_Password_2(void)
{
	u8 *p1;
	u32 Password_Temp;
	Password_Temp=(Password[0]-'0')*100000+(Password[1]-'0')*10000+(Password[2]-'0')*1000+(Password[3]-'0')*100+(Password[4]-'0')*10+(Password[5]-'0')*1;
	if((Password_Temp<0)||(Password_Temp>999999)) {printf("The password is not valid!\r\n");return 1;}
	Init_Password=Password_Temp;
	Password_TEXT_Buffer[0]=PASSWORD_VALUE;//标记校准过了
	p1 = (u8 *)&Init_Password;//将浮点型X_Critical变为四个字节
	Password_TEXT_Buffer[1] = *p1;
	p1++;
	Password_TEXT_Buffer[2] = *p1;
	p1++;
	Password_TEXT_Buffer[3] = *p1;
	p1++;
	Password_TEXT_Buffer[4] = *p1;
	
	STMFLASH_Write(PASSWORD_SAVE_ADDR,(u16*)Password_TEXT_Buffer,PASSWORD_SIZE);	
	
	return 0;
}

//在FLASH中读取密码
u8 FLASH_Read_Password()
{
	s32 tempfac;
	STMFLASH_Read(PASSWORD_SAVE_ADDR,(u16*)Password_datatemp,PASSWORD_SIZE);
	
	tempfac=Password_datatemp[0];//读取标记字,看是否校准过！	
	if(tempfac==PASSWORD_VALUE)//已经写入了			   
	{   
		Init_Password=*(u32 *)(Password_datatemp+1);//取出密码		
		return 1;	 
	}
	return 0;
}
//时间界面键值处理
void Touch_Gui_Disp_Time(void)	
{
	u8 value=255;
	Temp_position=0;
	Cycle_Time=0;
	Gui_Disp_Time(); 	 //操作界面

	while(1)
	{
		Gui_Currsor_Blinks_Time();
		value=Touch_Scan_Time();	   //获取触摸键值
		while(PEN==0)  //等待触摸释放
		{
			Gui_Currsor_Blinks_Time();
		}
		
					
		switch(value)			  //相应键值的处理
		{	
			case 0: 
				Cycle_Time=12;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Cancel");
				printf("\r\n\r\n");
				#endif 
				break;
			case 1:
				Cycle_Time=12;
				if(RTC_Set_Time()==0)
				{
					LCD_Clear(WHITE);
					POINT_COLOR=GREEN;			//设置字体为蓝色
					LCD_ShowString( 72, 52, 72+8*22, 68,16,"Time set successfully!");
				}					
				else 	
				{	
					LCD_Clear(WHITE);
					POINT_COLOR=RED;			//设置字体为红色					
					LCD_ShowString( 96, 52, 96+8*16, 68,16,"Time set Failed!");
				}
				delay_ms(500);
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("OK");
				printf("\r\n\r\n");
				#endif 
				break;
			case 2:
				if(Temp_position<100)	Time_a[Temp_position]=1+'0';
				else 					Time_b[Temp_position-100]=1+'0';
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("1");
				printf("\r\n\r\n");
				#endif 
				break;
			case 3:
				if(Temp_position<100)	Time_a[Temp_position]=2+'0';
				else 					Time_b[Temp_position-100]=2+'0';
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("2");
				printf("\r\n\r\n");
				#endif 
				break;
			case 4:
				if(Temp_position<100)	Time_a[Temp_position]=3+'0';
				else 					Time_b[Temp_position-100]=3+'0';
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("3");
				printf("\r\n\r\n");
				#endif 
				break;
			case 5:
				if(Cycle_Time>0)Cycle_Time--;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("DEL");
				printf("\r\n\r\n");
				#endif 
				break;
			case 6:
				if(Temp_position<100)	Time_a[Temp_position]=4+'0';
				else 					Time_b[Temp_position-100]=4+'0';
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("4");
				printf("\r\n\r\n");
				#endif 
				break;
			case 7:
				if(Temp_position<100)	Time_a[Temp_position]=5+'0';
				else 					Time_b[Temp_position-100]=5+'0';
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("5");
				printf("\r\n\r\n");
				#endif 
				break;
			case 8:
				if(Temp_position<100)	Time_a[Temp_position]=6+'0';
				else 					Time_b[Temp_position-100]=6+'0';
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("6");
				printf("\r\n\r\n");
				#endif 
				break;
			case 9:
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf(".");
				printf("\r\n\r\n");
				#endif 
				break;
			case 10:
				if(Temp_position<100)	Time_a[Temp_position]=7+'0';
				else 					Time_b[Temp_position-100]=7+'0';
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("7");
				printf("\r\n\r\n");
				#endif 
				break;
			case 11:
				if(Temp_position<100)	Time_a[Temp_position]=8+'0';
				else 					Time_b[Temp_position-100]=8+'0';
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("8");
				printf("\r\n\r\n");
				#endif 
				break;
			case 12:
				if(Temp_position<100)	Time_a[Temp_position]=9+'0';
				else 					Time_b[Temp_position-100]=9+'0';
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("9");
				printf("\r\n\r\n");
				#endif 
				break;
			case 13:
				if(Temp_position<100)	Time_a[Temp_position]=0+'0';
				else 					Time_b[Temp_position-100]=0+'0';
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("0");
				printf("\r\n\r\n");
				#endif 
				break;
			default:	break;
		}
		if(value!=0xff)//按键区被按下了
		{
			switch(Cycle_Time)
			{
				case 0:Temp_position=0;break;//年对应的位置
				case 1:Temp_position=1;break;
				case 2:Temp_position=2;break;
				case 3:Temp_position=3;break;
				
				case 4:Temp_position=5;break;//月对应的位置
				case 5:Temp_position=6;break;
				
				case 6:Temp_position=8;break;//日对应的位置
				case 7:Temp_position=9;break;
				
				case 8:Temp_position=100;break;//时对应的位置;
				case 9:Temp_position=101;break;
				
				case 10:Temp_position=103;break;//分对应的位置
				case 11:Temp_position=104;break;
			}
		}
		if(Cycle_Time==12) {Cycle_Time=0;break;}
		delay_ms(10);
	}
}



//阈值界面键值处理
void Touch_Gui_Disp_Threshold(void)
{
	u8 value=255;
	Temp_position=6;
	Cycle_Time=0;
	Gui_Disp_Threshold(); 	 //操作界面
	
	while(1)
	{
		Gui_Currsor_Blinks_Threshold();
		value=Touch_Scan_Time();	   //获取触摸键值
		while(PEN==0)  //等待触摸释放
		{
			Gui_Currsor_Blinks_Threshold();
		}
		switch(value)			  //相应键值的处理
		{	
			case 0: 
				Cycle_Time=9;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Cancel");
				printf("\r\n\r\n");
				#endif 
				break;
			case 1:
				Cycle_Time=9;
				if(Flash_Set_Threshold()==0)
				{
					LCD_Clear(WHITE);
					POINT_COLOR=GREEN;			//设置字体为蓝色
					LCD_ShowString( 72, 52, 72+8*22, 68,16,"Threshold set successfully!");
				}					
				else 	
				{	
					LCD_Clear(WHITE);
					POINT_COLOR=RED;			//设置字体为红色					
					LCD_ShowString( 96, 52, 96+8*16, 68,16,"Threshold set Failed!");
				}
				delay_ms(500);
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("OK");
				printf("\r\n\r\n");
				#endif 
				break;
			case 2:
				if(Temp_position<10)		X_Temp[Temp_position]=1+'0';
				else if(Temp_position<20) 	Y_Temp[Temp_position-10]=1+'0';
				else if(Temp_position<30) 	Z_Temp[Temp_position-20]=1+'0';
				if(Cycle_Time==8)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("1");
				printf("\r\n\r\n");
				#endif 
				break;
			case 3:
				if(Temp_position<10)		X_Temp[Temp_position]=2+'0';
				else if(Temp_position<20) 	Y_Temp[Temp_position-10]=2+'0';
				else if(Temp_position<30) 	Z_Temp[Temp_position-20]=2+'0';
				if(Cycle_Time==8)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("2");
				printf("\r\n\r\n");
				#endif 
				break;
			case 4:
				if(Temp_position<10)		X_Temp[Temp_position]=3+'0';
				else if(Temp_position<20) 	Y_Temp[Temp_position-10]=3+'0';
				else if(Temp_position<30) 	Z_Temp[Temp_position-20]=3+'0';
				if(Cycle_Time==8)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("3");
				printf("\r\n\r\n");
				#endif 
				break;
			case 5:
				if(Cycle_Time>0)Cycle_Time--;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("DEL");
				printf("\r\n\r\n");
				#endif 
				break;
			case 6:
				if(Temp_position<10)		X_Temp[Temp_position]=4+'0';
				else if(Temp_position<20) 	Y_Temp[Temp_position-10]=4+'0';
				else if(Temp_position<30) 	Z_Temp[Temp_position-20]=4+'0';
				if(Cycle_Time==8)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("4");
				printf("\r\n\r\n");
				#endif 
				break;
			case 7:
				if(Temp_position<10)		X_Temp[Temp_position]=5+'0';
				else if(Temp_position<20) 	Y_Temp[Temp_position-10]=5+'0';
				else if(Temp_position<30) 	Z_Temp[Temp_position-20]=5+'0';
				if(Cycle_Time==8)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("5");
				printf("\r\n\r\n");
				#endif 
				break;
			case 8:
				if(Temp_position<10)		X_Temp[Temp_position]=6+'0';
				else if(Temp_position<20) 	Y_Temp[Temp_position-10]=6+'0';
				else if(Temp_position<30) 	Z_Temp[Temp_position-20]=6+'0';
				if(Cycle_Time==8)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("6");
				printf("\r\n\r\n");
				#endif 
				break;
			case 9:
				if(Cycle_Time==8)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf(".");
				printf("\r\n\r\n");
				#endif 
				break;
			case 10:
				if(Temp_position<10)		X_Temp[Temp_position]=7+'0';
				else if(Temp_position<20) 	Y_Temp[Temp_position-10]=7+'0';
				else if(Temp_position<30) 	Z_Temp[Temp_position-20]=7+'0';
				if(Cycle_Time==8)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("7");
				printf("\r\n\r\n");
				#endif 
				break;
			case 11:
				if(Temp_position<10)		X_Temp[Temp_position]=8+'0';
				else if(Temp_position<20) 	Y_Temp[Temp_position-10]=8+'0';
				else if(Temp_position<30) 	Z_Temp[Temp_position-20]=8+'0';
				if(Cycle_Time==8)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("8");
				printf("\r\n\r\n");
				#endif 
				break;
			case 12:
				if(Temp_position<10)		X_Temp[Temp_position]=9+'0';
				else if(Temp_position<20) 	Y_Temp[Temp_position-10]=9+'0';
				else if(Temp_position<30) 	Z_Temp[Temp_position-20]=9+'0';
				if(Cycle_Time==8)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("9");
				printf("\r\n\r\n");
				#endif 
				break;
			case 13:
				if(Temp_position<10)		X_Temp[Temp_position]=0+'0';
				else if(Temp_position<20) 	Y_Temp[Temp_position-10]=0+'0';
				else if(Temp_position<30) 	Z_Temp[Temp_position-20]=0+'0';
				if(Cycle_Time==8)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("0");
				printf("\r\n\r\n");
				#endif 
				break;
			default:	break;
		}
		if(value!=0xff)//按键区被按下了
		{
			switch(Cycle_Time)
			{
				case 0:Temp_position=6;break;//X对应的位置
				case 1:Temp_position=7;break;
				case 2:Temp_position=9;break;
				
				case 3:Temp_position=16;break;
				case 4:Temp_position=17;break;//Y对应的位置
				case 5:Temp_position=19;break;
				
				case 6:Temp_position=26;break;//Z对应的位置
				case 7:Temp_position=27;break;
				case 8:Temp_position=29;break;
				
			}
		}
		if(Cycle_Time==9) {Cycle_Time=0;break;}
		delay_ms(10);
	}
}

//验证密码界面键值处理
void Touch_Gui_Disp_Password(void)
{
	u8 value=255;
	Temp_position=0;
	Gui_Disp_Password(); 	 //操作界面
	while(PEN==0)  //等待触摸释放
	{
	}

	while(1)
	{
		Gui_Currsor_Password();
		value=Touch_Scan_Time();	   //获取触摸键值
		while(PEN==0)  //等待触摸释放
		{
			Gui_Currsor_Password();
		}
					
		switch(value)			  //相应键值的处理
		{	
			case 0: 
				Cycle_Time=7;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Cancel");
				printf("\r\n\r\n");
				#endif 
				break;
			case 1:
				if(Flash_check_Password()==0)
				{
					Touch_Gui_Disp_Menu();//进入菜单函数
					LCD_Clear(WHITE);
		
				}					
				else 	
				{	
					LCD_Clear(WHITE);
					POINT_COLOR=RED;			//设置字体为红色					
					LCD_ShowString( 96, 52, 96+8*16, 68,16,"Password error!");
				
				}
				Cycle_Time=7;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("OK");
				printf("\r\n\r\n");
				#endif 
				break;
			case 2:
				Password[Temp_position]=1+'0';
				if(Cycle_Time==6)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("1");
				printf("\r\n\r\n");
				#endif 
				break;
			case 3:
				Password[Temp_position]=2+'0';
				if(Cycle_Time==6)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("2");
				printf("\r\n\r\n");
				#endif 
				break;
			case 4:
				Password[Temp_position]=3+'0';
				if(Cycle_Time==6)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("3");
				printf("\r\n\r\n");
				#endif 
				break;
			case 5:
				if(Cycle_Time>0)Cycle_Time--;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("DEL");
				printf("\r\n\r\n");
				#endif 
				break;
			case 6:
				Password[Temp_position]=4+'0';
				if(Cycle_Time==6)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("4");
				printf("\r\n\r\n");
				#endif 
				break;
			case 7:
				Password[Temp_position]=5+'0';
				if(Cycle_Time==6)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("5");
				printf("\r\n\r\n");
				#endif 
				break;
			case 8:
				Password[Temp_position]=6+'0';
				if(Cycle_Time==6)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("6");
				printf("\r\n\r\n");
				#endif 
				break;
			case 9:
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf(".");
				printf("\r\n\r\n");
				#endif 
				break;
			case 10:
				Password[Temp_position]=7+'0';
				if(Cycle_Time==6)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("7");
				printf("\r\n\r\n");
				#endif 
				break;
			case 11:
				Password[Temp_position]=8+'0';
				if(Cycle_Time==6)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("8");
				printf("\r\n\r\n");
				#endif 
				break;
			case 12:
				Password[Temp_position]=9+'0';
				if(Cycle_Time==6)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("9");
				printf("\r\n\r\n");
				#endif 
				break;
			case 13:
				Password[Temp_position]=0+'0';
				if(Cycle_Time==6)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("0");
				printf("\r\n\r\n");
				#endif 
				break;
			default:	break;
		}
		if(value!=0xff)//按键区被按下了
		{
			switch(Cycle_Time)
			{
				case 0:Temp_position=0;break;
				case 1:Temp_position=1;break;
				case 2:Temp_position=2;break;
				case 3:Temp_position=3;break;
				case 4:Temp_position=4;break;
				case 5:Temp_position=5;break;
				case 6:Temp_position=6;break;
			}
		}
		if(Cycle_Time==7) {Cycle_Time=0;break;}
		delay_ms(10);
	}
}

//密码设置界面键值处理
void Touch_Gui_Set_Password(void)
{
	u8 value=255;
	Cycle_Time=0;
	Temp_position=0;
	Flag_First=0;
	Password[0]=' ';Password[1]=' ';Password[2]=' ';Password[3]=' ';Password[4]=' ';Password[5]=' ';
	Gui_Disp_Password(); 	 //操作界面
	while(PEN==0)  //等待触摸释放
	{
	}
	while(1)
	{
		Gui_Set_Password();
		value=Touch_Scan_Time();	   //获取触摸键值
		while(PEN==0)  //等待触摸释放
		{
			Gui_Set_Password();
		}
					
		switch(value)			  //相应键值的处理
		{	
			case 0: 
				Cycle_Time=6;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("Cancel");
				printf("\r\n\r\n");
				#endif 
				break;
			case 1:
				Cycle_Time=6;
				if(FLASH_Write_Password_2()==0)
				{
					LCD_Clear(WHITE);
					POINT_COLOR=GREEN;			//设置字体为蓝色
					LCD_ShowString( 72, 52, 72+8*22, 68,16,"Password set successfully!");
				}					
				else 	
				{	
					LCD_Clear(WHITE);
					POINT_COLOR=RED;			//设置字体为红色					
					LCD_ShowString( 96, 52, 96+8*16, 68,16,"Password Setting failed!");
				
				}
				delay_ms(500);
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("OK");
				printf("\r\n\r\n");
				#endif 
				break;
			case 2:
				Password[Temp_position]=1+'0';
				if(Cycle_Time==5){Cycle_Time=0;Flag_First=1;}else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("1");
				printf("\r\n\r\n");
				#endif 
				break;
			case 3:
				Password[Temp_position]=2+'0';
				if(Cycle_Time==5){Cycle_Time=0;Flag_First=1;}else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("2");
				printf("\r\n\r\n");
				#endif 
				break;
			case 4:
				Password[Temp_position]=3+'0';
				if(Cycle_Time==5){Cycle_Time=0;Flag_First=1;}else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("3");
				printf("\r\n\r\n");
				#endif 
				break;
			case 5:
				if(Cycle_Time>0)Cycle_Time--;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("DEL");
				printf("\r\n\r\n");
				#endif 
				break;
			case 6:
				Password[Temp_position]=4+'0';
				if(Cycle_Time==5){Cycle_Time=0;Flag_First=1;}else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("4");
				printf("\r\n\r\n");
				#endif 
				break;
			case 7:
				Password[Temp_position]=5+'0';
				if(Cycle_Time==5){Cycle_Time=0;Flag_First=1;}else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("5");
				printf("\r\n\r\n");
				#endif 
				break;
			case 8:
				Password[Temp_position]=6+'0';
				if(Cycle_Time==5){Cycle_Time=0;Flag_First=1;}else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("6");
				printf("\r\n\r\n");
				#endif 
				break;
			case 9:
				if(Cycle_Time==11)Cycle_Time=0;else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf(".");
				printf("\r\n\r\n");
				#endif 
				break;
			case 10:
				Password[Temp_position]=7+'0';
				if(Cycle_Time==5){Cycle_Time=0;Flag_First=1;}else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("7");
				printf("\r\n\r\n");
				#endif 
				break;
			case 11:
				Password[Temp_position]=8+'0';
				if(Cycle_Time==5){Cycle_Time=0;Flag_First=1;}else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("8");
				printf("\r\n\r\n");
				#endif 
				break;
			case 12:
				Password[Temp_position]=9+'0';
				if(Cycle_Time==5){Cycle_Time=0;Flag_First=1;}else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("9");
				printf("\r\n\r\n");
				#endif 
				break;
			case 13:
				Password[Temp_position]=0+'0';
				if(Cycle_Time==5){Cycle_Time=0;Flag_First=1;}else Cycle_Time++;
				#ifdef SERIALPORT_TOUCH_VALUE
				printf("0");
				printf("\r\n\r\n");
				#endif 
				break;
			default:	break;
		}
		if(value!=0xff)//按键区被按下了
		{
			switch(Cycle_Time)
			{
				case 0:Temp_position=0;break;
				case 1:Temp_position=1;break;
				case 2:Temp_position=2;break;
				case 3:Temp_position=3;break;
				case 4:Temp_position=4;break;
				case 5:Temp_position=5;break;
			}
		}
		if(Cycle_Time==6) {Cycle_Time=0;break;}
		delay_ms(10);
	}
}

