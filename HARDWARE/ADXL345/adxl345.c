#include "adxl345.h"
#include "sys.h"
#include "delay.h"
#include "math.h" 
#include "lcd.h"
#include "stmflash.h"
#include "config.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//ADXL345 ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/12
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

u8 Adxl345_TEXT_Buffer[14];
#define ADXL345_SIZE sizeof(Adxl345_TEXT_Buffer)	 			  	//���鳤��
#define ADXL345_SAVE_ADDR  (0X08070020)			//����FLASH �����ַ(����Ϊż��)
u8 Adxl345_datatemp[ADXL345_SIZE];

//�޸�X,Y,Z�ᱨ����¼��ֵ
float X_Critical,Y_Critical,Z_Critical;
u8 Vertical_Direction;

//����У׼����										    
void Adxl345_Save_Adjdata(void)
{
	u8 *p1;
	 
	Adxl345_TEXT_Buffer[0]=ADXL345_VALUE;//���У׼����
	p1 = (u8 *)&X_Critical;//��������X_Critical��Ϊ�ĸ��ֽ�
	Adxl345_TEXT_Buffer[1] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[2] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[3] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[4] = *p1;
	p1 = (u8 *)&Y_Critical;//��������Y_Critical��Ϊ�ĸ��ֽ�
	Adxl345_TEXT_Buffer[5] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[6] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[7] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[8] = *p1;
	p1 = (u8 *)&Z_Critical;//��������Z_Critical��Ϊ�ĸ��ֽ�
	Adxl345_TEXT_Buffer[9] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[10] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[11] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[12] = *p1;
	Adxl345_TEXT_Buffer[13]=Vertical_Direction;

	STMFLASH_Write(ADXL345_SAVE_ADDR,(u16*)Adxl345_TEXT_Buffer,ADXL345_SIZE);	
}
u8 Adxl345_Get_Adjdata(void)
{					  
	s32 tempfac;
	STMFLASH_Read(ADXL345_SAVE_ADDR,(u16*)Adxl345_datatemp,ADXL345_SIZE);
		
	tempfac=Adxl345_datatemp[0];//��ȡ�����,���Ƿ�У׼����	

	if(tempfac==ADXL345_VALUE)//�Ѿ�У׼����			   
	{   
		X_Critical=*(float *)(Adxl345_datatemp+1);//�õ�X����ֵ
		Y_Critical=*(float *)(Adxl345_datatemp+5);//�õ�Y����ֵ
		Z_Critical=*(float *)(Adxl345_datatemp+9);//�õ�Y����ֵ
		Vertical_Direction=Adxl345_datatemp[13];
		printf("Vertical_Direction=%d\r\n",Vertical_Direction);		
		return 1;	 
	}
	return 0;
}	 

//��ʼ��ADXL345.
//����ֵ:0,��ʼ���ɹ�;1,��ʼ��ʧ��.
u8 ADXL345_Init(void)
{				  
	IIC_Init();							//��ʼ��IIC����
	if(ADXL345_RD_Reg(DEVICE_ID)==0XE5)	//��ȡ����ID
	{  
		ADXL345_WR_Reg(DATA_FORMAT,0X2B);	//�͵�ƽ�ж����,13λȫ�ֱ���,��������Ҷ���,16g���� 
		ADXL345_WR_Reg(BW_RATE,0x0A);		//��������ٶ�Ϊ100Hz
		ADXL345_WR_Reg(POWER_CTL,0x28);	   	//����ʹ��,����ģʽ
		ADXL345_WR_Reg(INT_ENABLE,0x00);	//��ʹ���ж�		 
	 	ADXL345_WR_Reg(OFSX,0x00);
		ADXL345_WR_Reg(OFSY,0x00);
		ADXL345_WR_Reg(OFSZ,0x00);
		
		if(Adxl345_Get_Adjdata())return 0;//�Ѿ�У׼
		else			   //δУ׼
		{ 										    
			X_Critical=X_CRITICAL;//���¸�ֵ
			Y_Critical=Y_CRITICAL;
			Z_Critical=Z_CRITICAL;
			Adxl345_Save_Adjdata();	 
		}
		return 0;
	}			
	return 1;	   								  
}   

//дADXL345�Ĵ���
//addr:�Ĵ�����ַ
//val:Ҫд���ֵ
//����ֵ:��
void ADXL345_WR_Reg(u8 addr,u8 val) 
{
	IIC_Start();  				 
	IIC_Send_Byte(ADXL_WRITE);     	//����д����ָ��	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(addr);   			//���ͼĴ�����ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(val);     		//����ֵ					   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();						//����һ��ֹͣ���� 	   
}
//��ADXL345�Ĵ���
//addr:�Ĵ�����ַ
//����ֵ:������ֵ
u8 ADXL345_RD_Reg(u8 addr) 		
{
	u8 temp=0;		 
	IIC_Start();  				 
	IIC_Send_Byte(ADXL_WRITE);	//����д����ָ��	 
	temp=IIC_Wait_Ack();	   
    IIC_Send_Byte(addr);   		//���ͼĴ�����ַ
	temp=IIC_Wait_Ack(); 	 										  		   
	IIC_Start();  	 	   		//��������
	IIC_Send_Byte(ADXL_READ);	//���Ͷ�����ָ��	 
	temp=IIC_Wait_Ack();	   
    temp=IIC_Read_Byte(0);		//��ȡһ���ֽ�,�������ٶ�,����NAK 	    	   
    IIC_Stop();					//����һ��ֹͣ���� 	    
	return temp;				//���ض�����ֵ
}  
//��ȡADXL��ƽ��ֵ
//x,y,z:��ȡ10�κ�ȡƽ��ֵ
void ADXL345_RD_Avval(short *x,short *y,short *z)
{
	short tx=0,ty=0,tz=0;	   
	u8 i;  
	for(i=0;i<10;i++)
	{
		ADXL345_RD_XYZ(x,y,z);
		delay_ms(10);
		tx+=(short)*x;
		ty+=(short)*y;
		tz+=(short)*z;	   
	}
	*x=tx/10;
	*y=ty/10;
	*z=tz/10;
} 
//�Զ�У׼
//xval,yval,zval:x,y,z���У׼ֵ
void ADXL345_AUTO_Adjust(char *xval,char *yval,char *zval)
{
	short tx,ty,tz;
	u8 i;
	short offx=0,offy=0,offz=0;
	ADXL345_WR_Reg(POWER_CTL,0x00);	   	//�Ƚ�������ģʽ.
	delay_ms(100);
	ADXL345_WR_Reg(DATA_FORMAT,0X2B);	//�͵�ƽ�ж����,13λȫ�ֱ���,��������Ҷ���,16g���� 
	ADXL345_WR_Reg(BW_RATE,0x0A);		//��������ٶ�Ϊ100Hz
	ADXL345_WR_Reg(POWER_CTL,0x28);	   	//����ʹ��,����ģʽ
	ADXL345_WR_Reg(INT_ENABLE,0x00);	//��ʹ���ж�		 

	ADXL345_WR_Reg(OFSX,0x00);
	ADXL345_WR_Reg(OFSY,0x00);
	ADXL345_WR_Reg(OFSZ,0x00);
	delay_ms(12);
	for(i=0;i<10;i++)
	{
		ADXL345_RD_Avval(&tx,&ty,&tz);
		offx+=tx;
		offy+=ty;
		offz+=tz;
	}	 		
	offx/=10;
	offy/=10;
	offz/=10;
	*xval=-offx/4;
	*yval=-offy/4;
	*zval=-(offz-256)/4;	  
 	ADXL345_WR_Reg(OFSX,*xval);
	ADXL345_WR_Reg(OFSY,*yval);
	ADXL345_WR_Reg(OFSZ,*zval);	
} 
//��ȡ3���������
//x,y,z:��ȡ��������
void ADXL345_RD_XYZ(short *x,short *y,short *z)
{
	u8 buf[6];
	u8 i;
	IIC_Start();  				 
	IIC_Send_Byte(ADXL_WRITE);	//����д����ָ��	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(0x32);   		//���ͼĴ�����ַ(���ݻ������ʼ��ַΪ0X32)
	IIC_Wait_Ack(); 	 										  		   
 
 	IIC_Start();  	 	   		//��������
	IIC_Send_Byte(ADXL_READ);	//���Ͷ�����ָ��
	IIC_Wait_Ack();
	for(i=0;i<6;i++)
	{
		if(i==5)buf[i]=IIC_Read_Byte(0);//��ȡһ���ֽ�,�������ٶ�,����NACK  
		else buf[i]=IIC_Read_Byte(1);	//��ȡһ���ֽ�,������,����ACK 
 	}	        	   
    IIC_Stop();					//����һ��ֹͣ����
	*x=(short)(((u16)buf[1]<<8)+buf[0]); 	    
	*y=(short)(((u16)buf[3]<<8)+buf[2]); 	    
	*z=(short)(((u16)buf[5]<<8)+buf[4]); 	   
}
//��ȡADXL345������times��,��ȡƽ��
//x,y,z:����������
//times:��ȡ���ٴ�
void ADXL345_Read_Average(short *x,short *y,short *z,u8 times)
{
	u8 i;
	short tx,ty,tz;
	*x=0;
	*y=0;
	*z=0;
	if(times)//��ȡ������Ϊ0
	{
		for(i=0;i<times;i++)//������ȡtimes��
		{
			ADXL345_RD_XYZ(&tx,&ty,&tz);
			*x+=tx;
			*y+=ty;
			*z+=tz;
			delay_ms(5);
		}
		*x/=times;
		*y/=times;
		*z/=times;
	}
}
//�õ��Ƕ�
//x,y,z:x,y,z������������ٶȷ���(����Ҫ��λ,ֱ����ֵ����)
//dir:Ҫ��õĽǶ�.0,��Z��ĽǶ�;1,��X��ĽǶ�;2,��Y��ĽǶ�.
//����ֵ:�Ƕ�ֵ.��λ0.1��.
short ADXL345_Get_Angle(float x,float y,float z,u8 dir)
{
	float temp;
 	float res=0;
	switch(dir)
	{
		case 0://����ȻZ��ĽǶ�
 			temp=sqrt((x*x+y*y))/z;
 			res=atan(temp);
 			break;
		case 1://����ȻX��ĽǶ�
 			temp=x/sqrt((y*y+z*z));
 			res=atan(temp);
 			break;
 		case 2://����ȻY��ĽǶ�
 			temp=y/sqrt((x*x+z*z));
 			res=atan(temp);
 			break;
 	}
	return res*1800/3.14;
}

//x,y:��ʼ��ʾ������λ��
//num:Ҫ��ʾ������
//mode:0,��ʾ���ٶ�ֵ;1,��ʾ�Ƕ�ֵ;
//x,y:��ʼ��ʾ������λ��
//num:Ҫ��ʾ������
//mode:0,��ʾ���ٶ�ֵ;1,��ʾ�Ƕ�ֵ;
void Adxl_Show_Num(u16 x,u16 y,short num,u8 mode)
{
	if(mode==0)	//��ʾ���ٶ�ֵ
	{
		if(num<0)
		{
			LCD_Show_XYZ(x,y,11);//��ʾ����
			num=-num;						//תΪ����
		}else LCD_Show_XYZ(x,y,10);//ȥ������
		LCD_ShowNum_2(x+15,y,num,4,30);		//��ʾֵ		    
 	}else 		//��ʾ�Ƕ�ֵ
	{
		if(num<0)
		{
			LCD_Show_XYZ(x,y,11);//��ʾ����
			num=-num;						//תΪ����
		}else  LCD_Show_XYZ(x,y,10);//ȥ������
 		LCD_ShowNum_2(x+15,y,num/10,2,30);		//��ʾ��������
		LCD_Show_XYZ(x+45,y,16);//��ʾС����	
		LCD_ShowNum_2(x+50,y,num%10,1,30); 	//��ʾС������				
	}
}	









