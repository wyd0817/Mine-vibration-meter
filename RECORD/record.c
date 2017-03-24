#include "record.h"
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
#include "my_GUI_TOUCH.h"


//����ʹ�ñ���
u8 key;

//TF��ʹ�ñ���
u8 res=0;
u8 size=0;
u8 csv_line1[]="ʱ��,����,x��,y��,z��\r\n";	 
u8 Open_file_name[]="00��00�� 00��00�� �����¼.csv";
u32 __total,__free;
static FIL fdst;


//RTCʹ�ñ���
u8 t=0;
u8 Time_a[]="2000/01/01";
u8 Time_b[]="00:00";
//	extern _calendar_obj calendar;//ʱ�ӽṹ�� 

//ADXL345ʹ�ñ���
extern u8 Vertical_Direction;
extern float X_Critical,Y_Critical,Z_Critical;//�޸�X,Y,Z�ᱨ����¼��ֵ
u8 Record_Num = 0;
u8 *p,*p_csv;
u8 array[] = "   2000-00-00   00:00    000        Υ�����                            ";
u8 array_csv[] = "2000/00/00,00:00,+0:00,+0:00,+0:00";
short x,y,z;  	    
short angx,angy,angz; 	
short angx_New,angy_New,angz_New,angx,angy,angz;
int ACC_x,ACC_y,ACC_z;
u32 Overproof_Num=0;

extern u32 Init_Password;

u32 Touch_Time=0;
	
//���ܣ�ϵͳ��ʼ������
void Sys_Init(void)
{
	p=array;
	p_csv=array_csv;
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	usmart_dev.init(72);	//��ʼ��USMART		
 	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();				//��ʼ������
	BEEP_Init();			//��ʼ��������	
	RTC_Init();				//��ʼ��ʵʱʱ��
	LCD_Init();			   		//��ʼ��LCD  
	
	LCD_Clear(BLACK);		//����
	POINT_COLOR=WHITE;
	BACK_COLOR=BLACK;
	
 	my_mem_init(SRAMIN);		//��ʼ���ڲ��ڴ��
	tp_dev.init(); 
	

	if(FLASH_Read_Password()==0)
	{ 	
		Init_Password=INIT_PASSWORD;//���¸�ֵ		
		FLASH_Write_Password();	 
	}

	key=KEY_Scan(0);
	if(key==KEY0_PRES||key==KEY1_PRES)	//K1 ����,��ִ��У׼����
	{
		LED0 = 1;
		LCD_Clear(WHITE);//����
		TP_Adjust();  //��ĻУ׼  //�õ��ĸ�У׼����
		TP_Save_Adjdata();	  		
		//Load_Drow_Dialog();
		LCD_Clear(BLACK);		//����
		POINT_COLOR=WHITE;
		BACK_COLOR=BLACK;
	}			
			
	while(ADXL345_Init())	//3D���ٶȴ�������ʼ��
	{
		POINT_COLOR=RED;			//��������Ϊ��ɫ 	
		LCD_ShowString(60,30,200,16,16,"ADXL345 Error");
		delay_ms(500);
		LCD_ShowString(60,30,200,16,16,"Please Check!             ");
 		delay_ms(500);
	}	
	LCD_ShowString(60,30,200,16,16,"ADXL345 OK!");
	while(SD_Init())//��ⲻ��SD��
	{
		POINT_COLOR=RED;			//��������Ϊ��ɫ 	
		LCD_ShowString(60,50,200,16,16,"TF Card Error!            ");
		delay_ms(500);					
		LCD_ShowString(60,50,200,16,16,"Please Check!             ");
		delay_ms(500);
		LED0=!LED0;//DS0��˸
	}
	LCD_ShowString(60,50,200,16,16,"TF Card OK!");
	exfuns_init();							//Ϊfatfs��ر��������ڴ�				 
	f_mount(fs[0],"0:",1); 					//����SD�� 
											    			
	while(exf_getfree("0",&__total,&__free))	//�õ�SD������������ʣ������
	{
		LCD_ShowString(60,70,200,16,16,"TF Card Fatfs Error!");
		delay_ms(200);
		LCD_Fill(60,70,60+200,190+16,WHITE);	//�����ʾ			  
		delay_ms(200);
		LED0=!LED0;//DS0��˸
	}	
	
 	POINT_COLOR=BLUE;//��������Ϊ��ɫ	   
	LCD_ShowString(60,70,200,16,16,"FATFS OK!                 ");	 
	LCD_ShowString(60,90,200,16,16,"SD Total Size:     MB    ");	 
	LCD_ShowString(60,110,200,16,16,"SD  Free Size:     MB    "); 	    
 	LCD_ShowNum(60+8*14,90,__total>>10,5,16);				//��ʾSD�������� MB
	LCD_ShowNum(60+8*14,110,__free>>10,5,16);					//��ʾSD��ʣ������ MB
	LCD_ShowString(60,130,130,16,16,"SYSTEM OK!");	
	/***************************************************************************************/

	Open_file_name[0]  = calendar.w_month/10+'0';
	Open_file_name[1]  = calendar.w_month%10+'0';
	Open_file_name[4]  = calendar.w_date/10+'0';
	Open_file_name[5]  = calendar.w_date%10+'0';
	Open_file_name[9] = calendar.hour/10+'0';
 	Open_file_name[10] = calendar.hour%10+'0';
	Open_file_name[13] = calendar.min/10+'0';
	Open_file_name[14] = calendar.min%10+'0';
	
	/***********************************оƬ��������******************************************/
	
	ADXL345_AUTO_Adjust((char*)&x,(char*)&y,(char*)&z);//�Զ�У׼
	
	res = f_open(&fdst, Open_file_name, FA_CREATE_ALWAYS | FA_WRITE);

	if(res != FR_OK)	printf("open file error : %d\n\r",res);
	else if(res==FR_OK){
		LCD_Clear(BLACK);
		POINT_COLOR=GREEN;			//��������Ϊ��ɫ 	
		LCD_ShowString(60,70,200,16,16,"Mine vibration instrument");	
		LCD_ShowString(60,90,200,16,16,"       ^_^ CYSY ^_^      ");	
		LCD_ShowString(60,110,200,16,16,"    Condor Technology    ");
		LCD_ShowString(60,130,200,16,16,"      www.syrobo.com     ");
		LCD_ShowString(60,130,200,16,16,"Copyright (C) 2010-2020 ");			
		delay_ms(500);	
		LCD_Clear(WHITE);
		BACK_COLOR=WHITE;
		
		size=sizeof(csv_line1)-1;			  	
		res = f_write(&fdst, csv_line1,size, &bw);
		delay_us(100);
		
		//f_write(&fdst,"\r\n",2,&bw);
		//delay_us(100);
		
		// Write it to the dst file 
		f_sync(&fdst); 
		
		LCD_Show_XYZ(0,138,13);//X
		LCD_Show_XYZ(0+15*1,138,11);//_
		LCD_Show_XYZ(0+15*2,138,17);//A
		LCD_Show_XYZ(0+15*3,138,18);//C
		LCD_Show_XYZ(0+15*4,138,18);//C
		LCD_Show_XYZ(0+15*5,138,12);//:
		LCD_Show_XYZ(170,138,13);//X
		LCD_Show_XYZ(170+15*1,138,11);//_
		LCD_Show_XYZ(170+15*2,138,17);//A
		LCD_Show_XYZ(170+15*3,138,19);//N
		LCD_Show_XYZ(170+15*4,138,20);//G
		LCD_Show_XYZ(170+15*5,138,12);//:
		
		LCD_Show_XYZ(0,165,14);//Y
		LCD_Show_XYZ(0+15*1,165,11);//_
		LCD_Show_XYZ(0+15*2,165,17);//A
		LCD_Show_XYZ(0+15*3,165,18);//C
		LCD_Show_XYZ(0+15*4,165,18);//C
		LCD_Show_XYZ(0+15*5,165,12);//:
		LCD_Show_XYZ(170,165,14);//Y
		LCD_Show_XYZ(170+15*1,165,11);//_
		LCD_Show_XYZ(170+15*2,165,17);//A
		LCD_Show_XYZ(170+15*3,165,19);//N
		LCD_Show_XYZ(170+15*4,165,20);//G
		LCD_Show_XYZ(170+15*5,165,12);//:
		
		LCD_Show_XYZ(0,192,15);//Z
		LCD_Show_XYZ(0+15*1,192,11);//_
		LCD_Show_XYZ(0+15*2,192,17);//A
		LCD_Show_XYZ(0+15*3,192,18);//C
		LCD_Show_XYZ(0+15*4,192,18);//C
		LCD_Show_XYZ(0+15*5,192,12);//:
		LCD_Show_XYZ(170,192,15);//Z
		LCD_Show_XYZ(170+15*1,192,11);//_
		LCD_Show_XYZ(170+15*2,192,17);//A
		LCD_Show_XYZ(170+15*3,192,19);//N
		LCD_Show_XYZ(170+15*4,192,20);//G
		LCD_Show_XYZ(170+15*5,192,12);//:
		
	// 	size=sizeof(kszdy_1)-1;			  	
	// 	res = f_write(&fdst, kszdy_1,size, &bw);
	// 	size=sizeof(kszdy_2)-1;
	// 	res = f_write(&fdst, kszdy_2,size, &bw); 				
	// 	f_write(&fdst,"\r\n",2,&bw);	
	// 	f_sync(&fdst); 	
		ADXL345_Read_Average(&x,&y,&z,10);	//��ȡX,Y,Z��������ļ��ٶ�ֵ 
		angx=ADXL345_Get_Angle(x,y,z,1);    
		angy=ADXL345_Get_Angle(x,y,z,2);   
		angz=ADXL345_Get_Angle(x,y,z,0);	 
	}
}

//���ܣ�У׼ADXL345����
void Calibration_adxl345()
{
	key=KEY_Scan(0);
	if(key==KEY0_PRES)
	{
		LED1=0;//�̵���,��ʾУ׼��
		ADXL345_AUTO_Adjust((char*)&x,(char*)&y,(char*)&z);//�Զ�У׼
		LED1=1;//�̵���,��ʾУ׼���
	}
}

//���ܣ�����ʱ�亯��
void Update_Time()
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
		
		POINT_COLOR=LIGHTBLUE ;			//��������Ϊ��ɫ 
		LCD_ShowString(200,100,200,16,16,Time_a);
		POINT_COLOR=BLUE;			//��������Ϊ��ɫ 
		//LCD_ShowString(140,70,200,16,16,Time_b);
		LCD_ShowBig_Num(40,30,calendar.hour/10);
		LCD_ShowBig_Num(40+30*1,30,calendar.hour%10);
		LCD_ShowBig_Num(40+30*2,30,10);
		LCD_ShowBig_Num(40+30*3,30,calendar.min/10);
		LCD_ShowBig_Num(40+30*4,30,calendar.min%10);
		LCD_ShowBig_Num(40+30*5,30,10);
		LCD_ShowBig_Num(40+30*6,30,calendar.sec/10);
		LCD_ShowBig_Num(40+30*7,30,calendar.sec%10);

		
	}		
}
extern u16 DANGER_POINT_COLOR;	//������ɫ
//���ܣ�����¼����
void Test_Record(void)
{
		//�õ�X,Y,Z��ļ��ٶ�ֵ(ԭʼֵ)
		ADXL345_Read_Average(&x,&y,&z,10);	//��ȡX,Y,Z��������ļ��ٶ�ֵ 
		
		if(Vertical_Direction == 1)				x-=251;
		else if(Vertical_Direction == 2)		x+=251;
		else if(Vertical_Direction == 3)		y-=251;
		else if(Vertical_Direction == 4)		y+=251;
		else if(Vertical_Direction == 5)		z-=251;
		else if(Vertical_Direction == 6)		z+=251;

		
		ACC_z=z*3.9;
		ACC_y=y*3.9;
		ACC_x=x*3.9;

		Adxl_Show_Num(0+15*6,138,ACC_x,0);		//��ʾ���ٶ�ԭʼֵ
		Adxl_Show_Num(0+15*6,165,ACC_y,0);
		Adxl_Show_Num(0+15*6,192,ACC_z,0);
		
		//�õ��Ƕ�ֵ,����ʾ
		angx_New=ADXL345_Get_Angle(x,y,z,1);    
		angy_New=ADXL345_Get_Angle(x,y,z,2);   
		angz_New=ADXL345_Get_Angle(x,y,z,0); 
		Adxl_Show_Num(170+15*6,138,angx_New,1);	//��ʾ�Ƕ�ֵ
		Adxl_Show_Num(170+15*6,165,angy_New,1);
		Adxl_Show_Num(170+15*6,192,angz_New,1);
	
	
		if(((x+0)>(X_Critical*25)) || ((x+0)<(-(X_Critical*25))) || (y>(Y_Critical*25)) || (y<(-(Y_Critical*25)))|| (z>(Z_Critical*25)) || (z<(-(Z_Critical*25))))
 		{
				
				BEEP=1; 
				DANGER_POINT_COLOR=	RED;
				Adxl_Show_Num(0+15*6,138,ACC_x,0);		//��ʾ���ٶ�ԭʼֵ
				Adxl_Show_Num(0+15*6,165,ACC_y,0);
				Adxl_Show_Num(0+15*6,192,ACC_z,0);
				
				//�õ��Ƕ�ֵ,����ʾ
				angx_New=ADXL345_Get_Angle(x,y,z,1);    
				angy_New=ADXL345_Get_Angle(x,y,z,2);   
				angz_New=ADXL345_Get_Angle(x,y,z,0); 
				Adxl_Show_Num(170+15*6,138,angx_New,1);	//��ʾ�Ƕ�ֵ
				Adxl_Show_Num(170+15*6,165,angy_New,1);
				Adxl_Show_Num(170+15*6,192,angz_New,1);
				LCD_Show_XYZ(0,138,13);//X
				LCD_Show_XYZ(0+15*1,138,11);//_
				LCD_Show_XYZ(0+15*2,138,17);//A
				LCD_Show_XYZ(0+15*3,138,18);//C
				LCD_Show_XYZ(0+15*4,138,18);//C
				LCD_Show_XYZ(0+15*5,138,12);//:
				LCD_Show_XYZ(170,138,13);//X
				LCD_Show_XYZ(170+15*1,138,11);//_
				LCD_Show_XYZ(170+15*2,138,17);//A
				LCD_Show_XYZ(170+15*3,138,19);//N
				LCD_Show_XYZ(170+15*4,138,20);//G
				LCD_Show_XYZ(170+15*5,138,12);//:
				
				LCD_Show_XYZ(0,165,14);//Y
				LCD_Show_XYZ(0+15*1,165,11);//_
				LCD_Show_XYZ(0+15*2,165,17);//A
				LCD_Show_XYZ(0+15*3,165,18);//C
				LCD_Show_XYZ(0+15*4,165,18);//C
				LCD_Show_XYZ(0+15*5,165,12);//:
				LCD_Show_XYZ(170,165,14);//Y
				LCD_Show_XYZ(170+15*1,165,11);//_
				LCD_Show_XYZ(170+15*2,165,17);//A
				LCD_Show_XYZ(170+15*3,165,19);//N
				LCD_Show_XYZ(170+15*4,165,20);//G
				LCD_Show_XYZ(170+15*5,165,12);//:
				
				LCD_Show_XYZ(0,192,15);//Z
				LCD_Show_XYZ(0+15*1,192,11);//_
				LCD_Show_XYZ(0+15*2,192,17);//A
				LCD_Show_XYZ(0+15*3,192,18);//C
				LCD_Show_XYZ(0+15*4,192,18);//C
				LCD_Show_XYZ(0+15*5,192,12);//:
				LCD_Show_XYZ(170,192,15);//Z
				LCD_Show_XYZ(170+15*1,192,11);//_
				LCD_Show_XYZ(170+15*2,192,17);//A
				LCD_Show_XYZ(170+15*3,192,19);//N
				LCD_Show_XYZ(170+15*4,192,20);//G
				LCD_Show_XYZ(170+15*5,192,12);//:
				delay_ms(200);
				
				
				BEEP=0;
				angx=angx_New;
				angy=angy_New;
				angx=angz_New;

				array_csv[0]=calendar.w_year/1000+'0';
				array_csv[1]=calendar.w_year%1000/100+'0';
				array_csv[2]=calendar.w_year%100/10+'0';
				array_csv[3]=calendar.w_year%10+'0';

				array_csv[5]=calendar.w_month%100/10+'0';
				array_csv[6]=calendar.w_month%10+'0';

				array_csv[8]=calendar.w_date%100/10+'0';
				array_csv[9]=calendar.w_date%10+'0';

				array_csv[11]=calendar.hour/10+'0';
				array_csv[12]=calendar.hour%10+'0';

				array_csv[14]=calendar.min/10+'0';
				array_csv[15]=calendar.min%10+'0';
			
				if(ACC_x<0)
				{	
					array_csv[18]='-';	 //��ʾ����		
					ACC_x=-ACC_x;						//תΪ����
				}else array_csv[18]='+';	//ȥ������
				array_csv[19]=(ACC_x/100+48);
				array_csv[20]='.';
				array_csv[21]=(ACC_x%100/10+48);
				array_csv[22]=(ACC_x%10+48);
				array_csv[23]=',';
			
				if(ACC_y<0)
				{	
					array_csv[24]='-';	 //��ʾ����		
					ACC_y=-ACC_y;						//תΪ����
				}else array_csv[24]='+';	//ȥ������
				array_csv[25]=(ACC_y/100+48);
				array_csv[26]='.';
				array_csv[27]=(ACC_y%100/10+48);
				array_csv[28]=(ACC_y%10+48);
				array_csv[29]=',';
				
				if(ACC_z<0)
				{	
					array_csv[30]='-';	 //��ʾ����		
					ACC_z=-ACC_z;						//תΪ����
				}else array_csv[30]='+';	//ȥ������
				array_csv[31]=(ACC_z/100+48);
				array_csv[32]='.';
				array_csv[33]=(ACC_z%100/10+48);
				array_csv[34]=(ACC_z%10+48);
				
				res = f_write(&fdst, p_csv, 35, &bw); 
				delay_us(100);
				f_write(&fdst,"\r\n",2,&bw);
				delay_us(100);
				// Write it to the dst file 
				f_sync(&fdst); 
				Record_Num = 0;
				Overproof_Num++;//���������1���ӵ�20000
				DANGER_POINT_COLOR=	DARKBLUE;
				Adxl_Show_Num(0+15*6,138,ACC_x,0);		//��ʾ���ٶ�ԭʼֵ
				Adxl_Show_Num(0+15*6,165,ACC_y,0);
				Adxl_Show_Num(0+15*6,192,ACC_z,0);
				
				//�õ��Ƕ�ֵ,����ʾ
				angx_New=ADXL345_Get_Angle(x,y,z,1);    
				angy_New=ADXL345_Get_Angle(x,y,z,2);   
				angz_New=ADXL345_Get_Angle(x,y,z,0); 
				Adxl_Show_Num(170+15*6,138,angx_New,1);	//��ʾ�Ƕ�ֵ
				Adxl_Show_Num(170+15*6,165,angy_New,1);
				Adxl_Show_Num(170+15*6,192,angz_New,1);
				LCD_Show_XYZ(0,138,13);//X
				LCD_Show_XYZ(0+15*1,138,11);//_
				LCD_Show_XYZ(0+15*2,138,17);//A
				LCD_Show_XYZ(0+15*3,138,18);//C
				LCD_Show_XYZ(0+15*4,138,18);//C
				LCD_Show_XYZ(0+15*5,138,12);//:
				LCD_Show_XYZ(170,138,13);//X
				LCD_Show_XYZ(170+15*1,138,11);//_
				LCD_Show_XYZ(170+15*2,138,17);//A
				LCD_Show_XYZ(170+15*3,138,19);//N
				LCD_Show_XYZ(170+15*4,138,20);//G
				LCD_Show_XYZ(170+15*5,138,12);//:
				
				LCD_Show_XYZ(0,165,14);//Y
				LCD_Show_XYZ(0+15*1,165,11);//_
				LCD_Show_XYZ(0+15*2,165,17);//A
				LCD_Show_XYZ(0+15*3,165,18);//C
				LCD_Show_XYZ(0+15*4,165,18);//C
				LCD_Show_XYZ(0+15*5,165,12);//:
				LCD_Show_XYZ(170,165,14);//Y
				LCD_Show_XYZ(170+15*1,165,11);//_
				LCD_Show_XYZ(170+15*2,165,17);//A
				LCD_Show_XYZ(170+15*3,165,19);//N
				LCD_Show_XYZ(170+15*4,165,20);//G
				LCD_Show_XYZ(170+15*5,165,12);//:
				
				LCD_Show_XYZ(0,192,15);//Z
				LCD_Show_XYZ(0+15*1,192,11);//_
				LCD_Show_XYZ(0+15*2,192,17);//A
				LCD_Show_XYZ(0+15*3,192,18);//C
				LCD_Show_XYZ(0+15*4,192,18);//C
				LCD_Show_XYZ(0+15*5,192,12);//:
				LCD_Show_XYZ(170,192,15);//Z
				LCD_Show_XYZ(170+15*1,192,11);//_
				LCD_Show_XYZ(170+15*2,192,17);//A
				LCD_Show_XYZ(170+15*3,192,19);//N
				LCD_Show_XYZ(170+15*4,192,20);//G
				LCD_Show_XYZ(170+15*5,192,12);//:
 		}
		printf("a%db%dc%d\n",ACC_x,ACC_y,ACC_z);		
} 
//���ܣ���TF���д���һ���µ�csv�ļ�����
void Creat_New_Csv()
{
		//����Ѿ�����12��Сʱ��
		if((fabs(Open_file_name[9]*10+Open_file_name[10] - calendar.hour)  ==12)||(Overproof_Num==20000))
		{
			f_close(&fdst); /* closed the file */
			/***************************************************************************************/

			Open_file_name[0]  = calendar.w_month/10+'0';
			Open_file_name[1]  = calendar.w_month%10+'0';
			Open_file_name[4]  = calendar.w_date/10+'0';
			Open_file_name[5]  = calendar.w_date%10+'0';
			Open_file_name[9] = calendar.hour/10+'0';
			Open_file_name[10] = calendar.hour%10+'0';
			Open_file_name[13] = calendar.min/10+'0';
			Open_file_name[14] = calendar.min%10+'0';
	
			/***********************************оƬ��������******************************************/
			res = f_open(&fdst, Open_file_name, FA_CREATE_ALWAYS | FA_WRITE);
			size=sizeof(csv_line1)-1;			  	
			res = f_write(&fdst, csv_line1,size, &bw);
			delay_us(100);
			//f_write(&fdst,"\r\n",2,&bw);
			//delay_us(100);
			// Write it to the dst file 
			f_sync(&fdst); 
			Overproof_Num=0;//�����������
		}
}
//���ܣ���ⴥ��������
void Scan_Touch()
{
	tp_dev.scan(0); 	
	key=KEY_Scan(0);
	if(key==KEY0_PRES||key==KEY1_PRES)
	{
		Touch_Time=0;
		Touch_Gui_Disp_Password();//��֤�������
		LCD_Clear(WHITE);//����
			LCD_Show_XYZ(0,138,13);//X
		LCD_Show_XYZ(0+15*1,138,11);//_
		LCD_Show_XYZ(0+15*2,138,17);//A
		LCD_Show_XYZ(0+15*3,138,18);//C
		LCD_Show_XYZ(0+15*4,138,18);//C
		LCD_Show_XYZ(0+15*5,138,12);//:
		LCD_Show_XYZ(170,138,13);//X
		LCD_Show_XYZ(170+15*1,138,11);//_
		LCD_Show_XYZ(170+15*2,138,17);//A
		LCD_Show_XYZ(170+15*3,138,19);//N
		LCD_Show_XYZ(170+15*4,138,20);//G
		LCD_Show_XYZ(170+15*5,138,12);//:
		
		LCD_Show_XYZ(0,165,14);//Y
		LCD_Show_XYZ(0+15*1,165,11);//_
		LCD_Show_XYZ(0+15*2,165,17);//A
		LCD_Show_XYZ(0+15*3,165,18);//C
		LCD_Show_XYZ(0+15*4,165,18);//C
		LCD_Show_XYZ(0+15*5,165,12);//:
		LCD_Show_XYZ(170,165,14);//Y
		LCD_Show_XYZ(170+15*1,165,11);//_
		LCD_Show_XYZ(170+15*2,165,17);//A
		LCD_Show_XYZ(170+15*3,165,19);//N
		LCD_Show_XYZ(170+15*4,165,20);//G
		LCD_Show_XYZ(170+15*5,165,12);//:
		
		LCD_Show_XYZ(0,192,15);//Z
		LCD_Show_XYZ(0+15*1,192,11);//_
		LCD_Show_XYZ(0+15*2,192,17);//A
		LCD_Show_XYZ(0+15*3,192,18);//C
		LCD_Show_XYZ(0+15*4,192,18);//C
		LCD_Show_XYZ(0+15*5,192,12);//:
		LCD_Show_XYZ(170,192,15);//Z
		LCD_Show_XYZ(170+15*1,192,11);//_
		LCD_Show_XYZ(170+15*2,192,17);//A
		LCD_Show_XYZ(170+15*3,192,19);//N
		LCD_Show_XYZ(170+15*4,192,20);//G
		LCD_Show_XYZ(170+15*5,192,12);//:
	}
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		if(tp_dev.x<lcddev.width&&tp_dev.y<lcddev.height)
		{	
			if(tp_dev.x>112&&tp_dev.x<208&&tp_dev.y> 96&&tp_dev.y<144) Touch_Time++;
			if(Touch_Time>=10) 
			{
				Touch_Time=0;
				Touch_Gui_Disp_Password();//��֤�������
				LCD_Clear(WHITE);//����
					LCD_Show_XYZ(0,138,13);//X
				LCD_Show_XYZ(0+15*1,138,11);//_
				LCD_Show_XYZ(0+15*2,138,17);//A
				LCD_Show_XYZ(0+15*3,138,18);//C
				LCD_Show_XYZ(0+15*4,138,18);//C
				LCD_Show_XYZ(0+15*5,138,12);//:
				LCD_Show_XYZ(170,138,13);//X
				LCD_Show_XYZ(170+15*1,138,11);//_
				LCD_Show_XYZ(170+15*2,138,17);//A
				LCD_Show_XYZ(170+15*3,138,19);//N
				LCD_Show_XYZ(170+15*4,138,20);//G
				LCD_Show_XYZ(170+15*5,138,12);//:
				
				LCD_Show_XYZ(0,165,14);//Y
				LCD_Show_XYZ(0+15*1,165,11);//_
				LCD_Show_XYZ(0+15*2,165,17);//A
				LCD_Show_XYZ(0+15*3,165,18);//C
				LCD_Show_XYZ(0+15*4,165,18);//C
				LCD_Show_XYZ(0+15*5,165,12);//:
				LCD_Show_XYZ(170,165,14);//Y
				LCD_Show_XYZ(170+15*1,165,11);//_
				LCD_Show_XYZ(170+15*2,165,17);//A
				LCD_Show_XYZ(170+15*3,165,19);//N
				LCD_Show_XYZ(170+15*4,165,20);//G
				LCD_Show_XYZ(170+15*5,165,12);//:
				
				LCD_Show_XYZ(0,192,15);//Z
				LCD_Show_XYZ(0+15*1,192,11);//_
				LCD_Show_XYZ(0+15*2,192,17);//A
				LCD_Show_XYZ(0+15*3,192,18);//C
				LCD_Show_XYZ(0+15*4,192,18);//C
				LCD_Show_XYZ(0+15*5,192,12);//:
				LCD_Show_XYZ(170,192,15);//Z
				LCD_Show_XYZ(170+15*1,192,11);//_
				LCD_Show_XYZ(170+15*2,192,17);//A
				LCD_Show_XYZ(170+15*3,192,19);//N
				LCD_Show_XYZ(170+15*4,192,20);//G
				LCD_Show_XYZ(170+15*5,192,12);//:
			}
			
		}
	}else delay_ms(10);	//û�а������µ�ʱ�� 
		
}
