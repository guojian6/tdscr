
/************************************************************
 * Copyright (c) 2004-2005, POWERCOM Co., All Rights Reserved.
 * File        :"NEW_I2C.c"
 * Description :

 * Author      : Xu Hailun
 * Date        : 2006-09-05
 * Version     : 1.0
 * Revisions   :
 * Component   :   FM24C16A-S(铁电存储器)
 ************************************************************/
#include "def.h"
#include "all.h"

extern uint crc_fun(uchar *pbuf, uchar numc);
//#ifdef __UP_PA_MODULE__
//	#define DEFSDAPIN   PINC  //定义DEFSDAPIN代表PIND（SDA所在端口）
//	#define EE_SDA   PORTC4  //SDA是PORTD6脚
//	#define SDAPORT   PORTC  //定义SDAPORT代表SDA脚
//	#define SCLPORT   PORTC  //定义SCLPORT代表SCL脚
//	#define DEFSCLDDR    DDRC  //定义用DEFSCLDDR代表时钟脚端口方向寄存器
//	#define DEFSDADDR    DDRC  //定义用DEFSCLDDR代表数据脚端口方向寄存器
//	#define SDAHIGH   0x10   //SDA脚为高电平，SDA在PORTD6脚， 01000000 所以为0X40
//	#define SDALOW   0xef    //SDA脚为低电平，SDA在PORTD6脚， 10111111 所以为0Xbf
//	#define SCLHIGH   0x20   //SCL脚为高电平，SCL在PORTD5脚， 00100000 所以为0X20
//	#define SCLLOW   0xdf    //SDA脚为低电平，SCL在PORTD5脚， 11011111 所以为0Xdf
//#else
//	#define DEFSDAPIN   PIND  //定义DEFSDAPIN代表PIND（SDA所在端口）
//	#define EE_SDA   PORTD6  //SDA是PORTD6脚
//	#define SDAPORT   PORTD  //定义SDAPORT代表SDA脚
//	#define SCLPORT   PORTD  //定义SCLPORT代表SCL脚
//	#define DEFSCLDDR    DDRD  //定义用DEFSCLDDR代表时钟脚端口方向寄存器
//	#define DEFSDADDR    DDRD  //定义用DEFSCLDDR代表数据脚端口方向寄存器
//	#define SDAHIGH   0x40   //SDA脚为高电平，SDA在PORTD6脚， 01000000 所以为0X40
//	#define SDALOW   0xbf    //SDA脚为低电平，SDA在PORTD6脚， 10111111 所以为0Xbf
//	#define SCLHIGH   0x20   //SCL脚为高电平，SCL在PORTD5脚， 00100000 所以为0X20
//	#define SCLLOW   0xdf    //SDA脚为低电平，SCL在PORTD5脚， 11011111 所以为0Xdf
//#endif

#define DEFSDAPIN   PINC  //定义DEFSDAPIN代表PIND（SDA所在端口）
#define EE_SDA   PORTC1  //SDA是PORTD6脚
#define SDAPORT   PORTC  //定义SDAPORT代表SDA脚
#define SCLPORT   PORTC  //定义SCLPORT代表SCL脚
#define DEFSCLDDR    DDRC  //定义用DEFSCLDDR代表时钟脚端口方向寄存器
#define DEFSDADDR    DDRC  //定义用DEFSCLDDR代表数据脚端口方向寄存器
#define SDAHIGH   0x02   //SDA脚为高电平，SDA在PORTD6脚， 01000000 所以为0X40
#define SDALOW   0xfd    //SDA脚为低电平，SDA在PORTD6脚， 10111111 所以为0Xbf
#define SCLHIGH   0x01   //SCL脚为高电平，SCL在PORTD5脚， 00100000 所以为0X20
#define SCLLOW   0xfe    //SDA脚为低电平，SCL在PORTD5脚， 11011111 所以为0Xdf

/**********************************************************
** 功能说明:  延时函数(约10uS)
** 输入参数:   			  类型：uchar   ：0或1
** 输出参数:  
** 返 回 值:  无
** 引用函数: 
***********************************************************/
void _NOP()
{
    uchar i = 1;
    while(i)i--;
}
/**********************************************************
** 功能说明:  对sda端口进行赋值
** 输入参数:  buf			  类型：uchar   ：0或1
** 输出参数:  
** 返 回 值:  无
** 引用函数: 
***********************************************************/
void sda(uchar buf)
{
    if(buf)
	{
 	    SDAPORT |= SDAHIGH;//PORTD6置1
	}
	else
	{
		SDAPORT &= SDALOW;//PORTD6置0
	}
}
/**********************************************************
** 功能说明:  对scl端口进行赋值
** 输入参数:  buf			  类型：uchar  ：0或1
** 输出参数:  
** 返 回 值:  无
** 引用函数: 
***********************************************************/
void scl(uchar buf)
{
    if(buf)
    {
        SCLPORT |= SCLHIGH;//PORTD5置1
    }
    else
    {
        SCLPORT &= SCLLOW;//PORTD5置0
	}	
}
/**********************************************************
** 功能说明:  操作eeprom时发起始命令
** 输入参数:  
** 输出参数:  
** 返 回 值:  无
** 引用函数: 
***********************************************************/
void IIC_STAR()
{
	DEFSCLDDR|= SCLHIGH;//D5(SCL)为输出
	DEFSDADDR|= SDAHIGH;//D6(SDA)为输出
	scl(1);
	sda(1);
	sda(0);
	scl(0);
	_NOP();
}
/**********************************************************
** 功能说明:  操作eeprom完成时发结束命令
** 输入参数: 
** 输出参数:  
** 返 回 值:  无
** 引用函数: 
***********************************************************/
void IIC_STOP()
{
	DEFSCLDDR|= SCLHIGH;//D5(SCL)为输出
	DEFSDADDR|= SDAHIGH;//D6(SDA)为输出
	sda(0);
	scl(1);
	_NOP();
	sda(1);
	_NOP();
	scl(1);
	_NOP();
}
/**********************************************************
** 功能说明:  发数据给eeprom
** 输入参数: buf			类型：uchar		：需要发送的数据
** 输出参数:  
** 返 回 值:  无
** 引用函数: 
***********************************************************/

void IIC_SEND(uchar buf)
{
	uchar i;
	DEFSCLDDR|= SCLHIGH;//D5(SCL)为输出
	DEFSDADDR|= SDAHIGH;//D6(SDA)为输出
	
	for(i = 0 ; i < 8 ;i++)
	{
	    
		if( buf & 0x80 )
		{
			sda(1);
		}
		else
		{
			sda(0);
		}
		
		_NOP();
		scl(1);
		scl(0);
		buf = buf << 1;
	}
	
}
/**********************************************************
** 功能说明:  从eeprom获取数据
** 输入参数: 
** 输出参数:  buf 		类型：uchar		：获取的数据
** 返 回 值:  无 
** 引用函数: 
***********************************************************/	
uchar IIC_GET()
{
	uchar  i,buf;
	buf = 0x00;
	DEFSCLDDR|= SCLHIGH;    //D5(SCL)为输出
	DEFSDADDR&= SDALOW;    //D6(SDA)为输入
	
	for(i = 0 ; i < 8 ;i++)
	{		
		scl(1);
		_NOP();
		buf = buf <<1;
		
	    if(chkbit(DEFSDAPIN,EE_SDA))
	    {
		    buf |= 0x01;
	    }
	
		scl(0);
		_NOP();	
	}
	
	return buf;
}
/**********************************************************
** 功能说明:  eeprom回应命令体
** 输入参数:  buf          类型：uchar 	：0响应/1无响应
** 输出参数:  
** 返 回 值:  无
** 引用函数: 
***********************************************************/
uchar IIC_ACK()
{
	uchar buf;
	DEFSCLDDR|= SCLHIGH;    //D5(SCL)为输出
	DEFSDADDR&= SDALOW;    //D6(SDA)为输入
	sda(1);
	_NOP();
	scl(1);

	if(chkbit(DEFSDAPIN,EE_SDA))
	{
			buf = 1;
	}
	else
	{
			buf = 0;
	}
	
	scl(0);
	_NOP();
	DEFSDADDR|= SDAHIGH;  //D6(SDA)为输出
	_NOP();
	return buf;
}

/**********************************************************
** 功能说明:  读eeprom的值
** 输入参数:  addr		  类型：uint  ：读数的地址
** 输出参数:  buf          类型：uchar ：读出的数据
** 返 回 值:  无
** 引用函数: 
***********************************************************/
uchar RROM(uint addr)
{
	uchar buf;
    union buf_union comm;

    asm("wdr");
	comm.buf1 = addr;
	_NOP();
	IIC_STOP();
	IIC_STAR();
	IIC_SEND((comm.buf2[1] <<1)| 0xa0);//写控制字
	IIC_ACK();

	IIC_SEND(comm.buf2[0]);
	IIC_ACK();

	IIC_STAR();
	IIC_SEND((comm.buf2[1] <<1)|0xa1);//读数据
   	IIC_ACK();
	
	buf = IIC_GET();
	IIC_STOP();

    return buf;
}

/**********************************************************
** 功能说明:  写eeprom
** 输入参数:  addr		  类型：uchar ：写数据的地址
		 	 buf          类型：uchar ：读出的数据
** 输出参数:  
** 返 回 值:  无
** 引用函数: 
***********************************************************/
uchar WROM(uint addr,uchar buf)
{
	uchar ubuf;
	uchar  wcount;
    union buf_union comm;

    wcount = 0;
	comm.buf1 = addr;
    asm("wdr");
    
iic_write_re:
	if( wcount > 4 )
	{
	    IIC_STOP();
		_NOP();
		wcount = 0;
		return 0;
	}
	
	_NOP();
	IIC_STAR();
	IIC_SEND((comm.buf2[1]<<1) | 0xa0);//写控制字
	ubuf = IIC_ACK();
	
	if(ubuf)
	{
		wcount++;
		_NOP();
		goto iic_write_re;
	}
	
	IIC_SEND(comm.buf2[0]);
	ubuf = IIC_ACK();
	
	if(ubuf)
	{
		wcount++;
		_NOP();
		goto iic_write_re;
	}
	
	IIC_SEND(buf);//写数据
	ubuf = IIC_ACK();
	
	if(ubuf)
	{
		wcount++;
		_NOP();
		goto iic_write_re;
	}	
	
	IIC_STOP();
	return 1;
}

/**********************************************************
** 功能说明: 从EEPROM读一串数据
** 输入参数: START_ADDR : 起始地址
             NUM        : 需要读的数据占用字节数（不包括2个字节的校验码）
** 输出参数:  
** 返 回 值:  无
** 引用函数: 
***********************************************************/
uchar LOAD_EEPROM(uint START_ADDR, uchar NUM)
{
	uint v1, v2;
	uchar j;
	uint add;
    TIMSK = 0X0;
    asm("cli");       //关中断，防止读EEPROM过程中发生中断，扰乱读操作。

    for (j = 0; j <= NUM+1;j++)
	{
		add=__BACKUP_AREA0__ +START_ADDR+j;
		sdb[j]=RROM(add);
	}

	v1 = crc_fun(sdb, NUM);
	v2 = sdb[NUM] + (sdb[NUM+1]<<8);
	TIMSK = 0X01;
	asm("sei");    //读EEPROM完成，开中断
	
	if((v1 != v2)||(v1 == 0))
	{
		return 0;
	}
    else
    {
    	return 1;
    }
    
}

/**********************************************************
** 功能说明: 向EEPROM中写一串数据
** 输入参数: START_ADDR : 起始地址
             NUM        : 需要写的数据占用字节数（不包括2个字节的校验码）
** 输出参数:  
** 返 回 值:  无
** 引用函数: 
***********************************************************/
void UPDATE_EEPROM(uint START_ADDR, uchar NUM)
{
	uint v1;
	uchar j;
	uint add;
	v1 = crc_fun(sdb, NUM);
	sdb[NUM] = v1 & 0xff;
	sdb[NUM+1] = (v1 >> 8) & 0xff;
	TIMSK = 0X0;
	asm("cli");         //关中断，防止写EEPROM过程中发生中断，扰乱写操作。
	
    for (j = 0; j <= NUM+1; j++)
	{
		add=__BACKUP_AREA0__ +START_ADDR+j;
		WROM(add,sdb[j]);
	}	
	
	TIMSK = 0X01;
	asm("sei");         //写EEPROM完成，开中断
}
  

