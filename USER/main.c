#include "record.h"
#include "config.h"

#include "led.h"
#include "adxl345.h"

int main(void)
{
	//功能：系统初始化函数
	Sys_Init();	
	
	while(1)
	{
		//功能：更新时间函数
		Update_Time();
		//功能：检测记录函数
		Test_Record();
		//功能：在TF卡中创建一个新的csv文件函数
		Creat_New_Csv();
		//功能：检测触摸屏函数
		Scan_Touch();

		LED0 = !LED0;	

	}
}
















