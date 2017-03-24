#ifndef __CONFIG_H
#define __CONFIG_H


//修改密码
#define PASSWORD_VALUE  0X55//
//修改密码
#define INIT_PASSWORD 123456


//修改时间
#define ADXL345_VALUE  0Xaa//（控制阈值和方向）
//修改X,Y,Z轴报警记录阈值
#define X_CRITICAL  3.6
#define Y_CRITICAL  2.5
#define Z_CRITICAL  5.5

//定义垂直的坐标轴

/*
************************************************************************************************************************
*
*     .X轴向上       .X轴向下       .Y轴向上       .Y轴向下        .Z轴向上       .Z轴向下 
*    +--------------+--------------+--------------+--------------+--------------+--------------+
*    |  1           |  2           | 3            | 4            | 5            | 6            |
*    +--------------+--------------+--------------+--------------+--------------+--------------+ 
*
************************************************************************************************************************
*/

#define VERTICAL_DIRECTION 5

//#define SERIALPORT_TOUCH_VALUE 1  //是否向串口发送触摸屏读到的键值

//修改时间
#define STORAGE_VALUE  0X0505
#define SET_TIME RTC_Set(2016,02,17,20,39,30)  //设置时间
		 				    
#endif
