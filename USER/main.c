#include "record.h"
#include "config.h"

#include "led.h"
#include "adxl345.h"

int main(void)
{
	//���ܣ�ϵͳ��ʼ������
	Sys_Init();	
	
	while(1)
	{
		//���ܣ�����ʱ�亯��
		Update_Time();
		//���ܣ�����¼����
		Test_Record();
		//���ܣ���TF���д���һ���µ�csv�ļ�����
		Creat_New_Csv();
		//���ܣ���ⴥ��������
		Scan_Touch();

		LED0 = !LED0;	

	}
}
















