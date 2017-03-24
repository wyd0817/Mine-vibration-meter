#include "adxl345.h"
#include "sys.h"
#include "delay.h"
#include "math.h" 
#include "lcd.h"
#include "stmflash.h"
#include "config.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//ADXL345 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/12
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

u8 Adxl345_TEXT_Buffer[14];
#define ADXL345_SIZE sizeof(Adxl345_TEXT_Buffer)	 			  	//数组长度
#define ADXL345_SAVE_ADDR  (0X08070020)			//设置FLASH 保存地址(必须为偶数)
u8 Adxl345_datatemp[ADXL345_SIZE];

//修改X,Y,Z轴报警记录阈值
float X_Critical,Y_Critical,Z_Critical;
u8 Vertical_Direction;

//保存校准参数										    
void Adxl345_Save_Adjdata(void)
{
	u8 *p1;
	 
	Adxl345_TEXT_Buffer[0]=ADXL345_VALUE;//标记校准过了
	p1 = (u8 *)&X_Critical;//将浮点型X_Critical变为四个字节
	Adxl345_TEXT_Buffer[1] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[2] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[3] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[4] = *p1;
	p1 = (u8 *)&Y_Critical;//将浮点型Y_Critical变为四个字节
	Adxl345_TEXT_Buffer[5] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[6] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[7] = *p1;
	p1++;
	Adxl345_TEXT_Buffer[8] = *p1;
	p1 = (u8 *)&Z_Critical;//将浮点型Z_Critical变为四个字节
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
		
	tempfac=Adxl345_datatemp[0];//读取标记字,看是否校准过！	

	if(tempfac==ADXL345_VALUE)//已经校准过了			   
	{   
		X_Critical=*(float *)(Adxl345_datatemp+1);//得到X轴阈值
		Y_Critical=*(float *)(Adxl345_datatemp+5);//得到Y轴阈值
		Z_Critical=*(float *)(Adxl345_datatemp+9);//得到Y轴阈值
		Vertical_Direction=Adxl345_datatemp[13];
		printf("Vertical_Direction=%d\r\n",Vertical_Direction);		
		return 1;	 
	}
	return 0;
}	 

//初始化ADXL345.
//返回值:0,初始化成功;1,初始化失败.
u8 ADXL345_Init(void)
{				  
	IIC_Init();							//初始化IIC总线
	if(ADXL345_RD_Reg(DEVICE_ID)==0XE5)	//读取器件ID
	{  
		ADXL345_WR_Reg(DATA_FORMAT,0X2B);	//低电平中断输出,13位全分辨率,输出数据右对齐,16g量程 
		ADXL345_WR_Reg(BW_RATE,0x0A);		//数据输出速度为100Hz
		ADXL345_WR_Reg(POWER_CTL,0x28);	   	//链接使能,测量模式
		ADXL345_WR_Reg(INT_ENABLE,0x00);	//不使用中断		 
	 	ADXL345_WR_Reg(OFSX,0x00);
		ADXL345_WR_Reg(OFSY,0x00);
		ADXL345_WR_Reg(OFSZ,0x00);
		
		if(Adxl345_Get_Adjdata())return 0;//已经校准
		else			   //未校准
		{ 										    
			X_Critical=X_CRITICAL;//重新赋值
			Y_Critical=Y_CRITICAL;
			Z_Critical=Z_CRITICAL;
			Adxl345_Save_Adjdata();	 
		}
		return 0;
	}			
	return 1;	   								  
}   

//写ADXL345寄存器
//addr:寄存器地址
//val:要写入的值
//返回值:无
void ADXL345_WR_Reg(u8 addr,u8 val) 
{
	IIC_Start();  				 
	IIC_Send_Byte(ADXL_WRITE);     	//发送写器件指令	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(addr);   			//发送寄存器地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(val);     		//发送值					   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();						//产生一个停止条件 	   
}
//读ADXL345寄存器
//addr:寄存器地址
//返回值:读到的值
u8 ADXL345_RD_Reg(u8 addr) 		
{
	u8 temp=0;		 
	IIC_Start();  				 
	IIC_Send_Byte(ADXL_WRITE);	//发送写器件指令	 
	temp=IIC_Wait_Ack();	   
    IIC_Send_Byte(addr);   		//发送寄存器地址
	temp=IIC_Wait_Ack(); 	 										  		   
	IIC_Start();  	 	   		//重新启动
	IIC_Send_Byte(ADXL_READ);	//发送读器件指令	 
	temp=IIC_Wait_Ack();	   
    temp=IIC_Read_Byte(0);		//读取一个字节,不继续再读,发送NAK 	    	   
    IIC_Stop();					//产生一个停止条件 	    
	return temp;				//返回读到的值
}  
//读取ADXL的平均值
//x,y,z:读取10次后取平均值
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
//自动校准
//xval,yval,zval:x,y,z轴的校准值
void ADXL345_AUTO_Adjust(char *xval,char *yval,char *zval)
{
	short tx,ty,tz;
	u8 i;
	short offx=0,offy=0,offz=0;
	ADXL345_WR_Reg(POWER_CTL,0x00);	   	//先进入休眠模式.
	delay_ms(100);
	ADXL345_WR_Reg(DATA_FORMAT,0X2B);	//低电平中断输出,13位全分辨率,输出数据右对齐,16g量程 
	ADXL345_WR_Reg(BW_RATE,0x0A);		//数据输出速度为100Hz
	ADXL345_WR_Reg(POWER_CTL,0x28);	   	//链接使能,测量模式
	ADXL345_WR_Reg(INT_ENABLE,0x00);	//不使用中断		 

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
//读取3个轴的数据
//x,y,z:读取到的数据
void ADXL345_RD_XYZ(short *x,short *y,short *z)
{
	u8 buf[6];
	u8 i;
	IIC_Start();  				 
	IIC_Send_Byte(ADXL_WRITE);	//发送写器件指令	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(0x32);   		//发送寄存器地址(数据缓存的起始地址为0X32)
	IIC_Wait_Ack(); 	 										  		   
 
 	IIC_Start();  	 	   		//重新启动
	IIC_Send_Byte(ADXL_READ);	//发送读器件指令
	IIC_Wait_Ack();
	for(i=0;i<6;i++)
	{
		if(i==5)buf[i]=IIC_Read_Byte(0);//读取一个字节,不继续再读,发送NACK  
		else buf[i]=IIC_Read_Byte(1);	//读取一个字节,继续读,发送ACK 
 	}	        	   
    IIC_Stop();					//产生一个停止条件
	*x=(short)(((u16)buf[1]<<8)+buf[0]); 	    
	*y=(short)(((u16)buf[3]<<8)+buf[2]); 	    
	*z=(short)(((u16)buf[5]<<8)+buf[4]); 	   
}
//读取ADXL345的数据times次,再取平均
//x,y,z:读到的数据
//times:读取多少次
void ADXL345_Read_Average(short *x,short *y,short *z,u8 times)
{
	u8 i;
	short tx,ty,tz;
	*x=0;
	*y=0;
	*z=0;
	if(times)//读取次数不为0
	{
		for(i=0;i<times;i++)//连续读取times次
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
//得到角度
//x,y,z:x,y,z方向的重力加速度分量(不需要单位,直接数值即可)
//dir:要获得的角度.0,与Z轴的角度;1,与X轴的角度;2,与Y轴的角度.
//返回值:角度值.单位0.1°.
short ADXL345_Get_Angle(float x,float y,float z,u8 dir)
{
	float temp;
 	float res=0;
	switch(dir)
	{
		case 0://与自然Z轴的角度
 			temp=sqrt((x*x+y*y))/z;
 			res=atan(temp);
 			break;
		case 1://与自然X轴的角度
 			temp=x/sqrt((y*y+z*z));
 			res=atan(temp);
 			break;
 		case 2://与自然Y轴的角度
 			temp=y/sqrt((x*x+z*z));
 			res=atan(temp);
 			break;
 	}
	return res*1800/3.14;
}

//x,y:开始显示的坐标位置
//num:要显示的数据
//mode:0,显示加速度值;1,显示角度值;
//x,y:开始显示的坐标位置
//num:要显示的数据
//mode:0,显示加速度值;1,显示角度值;
void Adxl_Show_Num(u16 x,u16 y,short num,u8 mode)
{
	if(mode==0)	//显示加速度值
	{
		if(num<0)
		{
			LCD_Show_XYZ(x,y,11);//显示负号
			num=-num;						//转为正数
		}else LCD_Show_XYZ(x,y,10);//去掉负号
		LCD_ShowNum_2(x+15,y,num,4,30);		//显示值		    
 	}else 		//显示角度值
	{
		if(num<0)
		{
			LCD_Show_XYZ(x,y,11);//显示负号
			num=-num;						//转为正数
		}else  LCD_Show_XYZ(x,y,10);//去掉负号
 		LCD_ShowNum_2(x+15,y,num/10,2,30);		//显示整数部分
		LCD_Show_XYZ(x+45,y,16);//显示小数点	
		LCD_ShowNum_2(x+50,y,num%10,1,30); 	//显示小数部分				
	}
}	









