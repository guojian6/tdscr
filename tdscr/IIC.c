
/************************************************************
 * Copyright (c) 2004-2005, POWERCOM Co., All Rights Reserved.
 * File        :"NEW_I2C.c"
 * Description :

 * Author      : Xu Hailun
 * Date        : 2006-09-05
 * Version     : 1.0
 * Revisions   :
 * Component   :   FM24C16A-S(����洢��)
 ************************************************************/
#include "def.h"
#include "all.h"

extern uint crc_fun(uchar *pbuf, uchar numc);
//#ifdef __UP_PA_MODULE__
//	#define DEFSDAPIN   PINC  //����DEFSDAPIN����PIND��SDA���ڶ˿ڣ�
//	#define EE_SDA   PORTC4  //SDA��PORTD6��
//	#define SDAPORT   PORTC  //����SDAPORT����SDA��
//	#define SCLPORT   PORTC  //����SCLPORT����SCL��
//	#define DEFSCLDDR    DDRC  //������DEFSCLDDR����ʱ�ӽŶ˿ڷ���Ĵ���
//	#define DEFSDADDR    DDRC  //������DEFSCLDDR�������ݽŶ˿ڷ���Ĵ���
//	#define SDAHIGH   0x10   //SDA��Ϊ�ߵ�ƽ��SDA��PORTD6�ţ� 01000000 ����Ϊ0X40
//	#define SDALOW   0xef    //SDA��Ϊ�͵�ƽ��SDA��PORTD6�ţ� 10111111 ����Ϊ0Xbf
//	#define SCLHIGH   0x20   //SCL��Ϊ�ߵ�ƽ��SCL��PORTD5�ţ� 00100000 ����Ϊ0X20
//	#define SCLLOW   0xdf    //SDA��Ϊ�͵�ƽ��SCL��PORTD5�ţ� 11011111 ����Ϊ0Xdf
//#else
//	#define DEFSDAPIN   PIND  //����DEFSDAPIN����PIND��SDA���ڶ˿ڣ�
//	#define EE_SDA   PORTD6  //SDA��PORTD6��
//	#define SDAPORT   PORTD  //����SDAPORT����SDA��
//	#define SCLPORT   PORTD  //����SCLPORT����SCL��
//	#define DEFSCLDDR    DDRD  //������DEFSCLDDR����ʱ�ӽŶ˿ڷ���Ĵ���
//	#define DEFSDADDR    DDRD  //������DEFSCLDDR�������ݽŶ˿ڷ���Ĵ���
//	#define SDAHIGH   0x40   //SDA��Ϊ�ߵ�ƽ��SDA��PORTD6�ţ� 01000000 ����Ϊ0X40
//	#define SDALOW   0xbf    //SDA��Ϊ�͵�ƽ��SDA��PORTD6�ţ� 10111111 ����Ϊ0Xbf
//	#define SCLHIGH   0x20   //SCL��Ϊ�ߵ�ƽ��SCL��PORTD5�ţ� 00100000 ����Ϊ0X20
//	#define SCLLOW   0xdf    //SDA��Ϊ�͵�ƽ��SCL��PORTD5�ţ� 11011111 ����Ϊ0Xdf
//#endif

#define DEFSDAPIN   PINC  //����DEFSDAPIN����PIND��SDA���ڶ˿ڣ�
#define EE_SDA   PORTC1  //SDA��PORTD6��
#define SDAPORT   PORTC  //����SDAPORT����SDA��
#define SCLPORT   PORTC  //����SCLPORT����SCL��
#define DEFSCLDDR    DDRC  //������DEFSCLDDR����ʱ�ӽŶ˿ڷ���Ĵ���
#define DEFSDADDR    DDRC  //������DEFSCLDDR�������ݽŶ˿ڷ���Ĵ���
#define SDAHIGH   0x02   //SDA��Ϊ�ߵ�ƽ��SDA��PORTD6�ţ� 01000000 ����Ϊ0X40
#define SDALOW   0xfd    //SDA��Ϊ�͵�ƽ��SDA��PORTD6�ţ� 10111111 ����Ϊ0Xbf
#define SCLHIGH   0x01   //SCL��Ϊ�ߵ�ƽ��SCL��PORTD5�ţ� 00100000 ����Ϊ0X20
#define SCLLOW   0xfe    //SDA��Ϊ�͵�ƽ��SCL��PORTD5�ţ� 11011111 ����Ϊ0Xdf

/**********************************************************
** ����˵��:  ��ʱ����(Լ10uS)
** �������:   			  ���ͣ�uchar   ��0��1
** �������:  
** �� �� ֵ:  ��
** ���ú���: 
***********************************************************/
void _NOP()
{
    uchar i = 1;
    while(i)i--;
}
/**********************************************************
** ����˵��:  ��sda�˿ڽ��и�ֵ
** �������:  buf			  ���ͣ�uchar   ��0��1
** �������:  
** �� �� ֵ:  ��
** ���ú���: 
***********************************************************/
void sda(uchar buf)
{
    if(buf)
	{
 	    SDAPORT |= SDAHIGH;//PORTD6��1
	}
	else
	{
		SDAPORT &= SDALOW;//PORTD6��0
	}
}
/**********************************************************
** ����˵��:  ��scl�˿ڽ��и�ֵ
** �������:  buf			  ���ͣ�uchar  ��0��1
** �������:  
** �� �� ֵ:  ��
** ���ú���: 
***********************************************************/
void scl(uchar buf)
{
    if(buf)
    {
        SCLPORT |= SCLHIGH;//PORTD5��1
    }
    else
    {
        SCLPORT &= SCLLOW;//PORTD5��0
	}	
}
/**********************************************************
** ����˵��:  ����eepromʱ����ʼ����
** �������:  
** �������:  
** �� �� ֵ:  ��
** ���ú���: 
***********************************************************/
void IIC_STAR()
{
	DEFSCLDDR|= SCLHIGH;//D5(SCL)Ϊ���
	DEFSDADDR|= SDAHIGH;//D6(SDA)Ϊ���
	scl(1);
	sda(1);
	sda(0);
	scl(0);
	_NOP();
}
/**********************************************************
** ����˵��:  ����eeprom���ʱ����������
** �������: 
** �������:  
** �� �� ֵ:  ��
** ���ú���: 
***********************************************************/
void IIC_STOP()
{
	DEFSCLDDR|= SCLHIGH;//D5(SCL)Ϊ���
	DEFSDADDR|= SDAHIGH;//D6(SDA)Ϊ���
	sda(0);
	scl(1);
	_NOP();
	sda(1);
	_NOP();
	scl(1);
	_NOP();
}
/**********************************************************
** ����˵��:  �����ݸ�eeprom
** �������: buf			���ͣ�uchar		����Ҫ���͵�����
** �������:  
** �� �� ֵ:  ��
** ���ú���: 
***********************************************************/

void IIC_SEND(uchar buf)
{
	uchar i;
	DEFSCLDDR|= SCLHIGH;//D5(SCL)Ϊ���
	DEFSDADDR|= SDAHIGH;//D6(SDA)Ϊ���
	
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
** ����˵��:  ��eeprom��ȡ����
** �������: 
** �������:  buf 		���ͣ�uchar		����ȡ������
** �� �� ֵ:  �� 
** ���ú���: 
***********************************************************/	
uchar IIC_GET()
{
	uchar  i,buf;
	buf = 0x00;
	DEFSCLDDR|= SCLHIGH;    //D5(SCL)Ϊ���
	DEFSDADDR&= SDALOW;    //D6(SDA)Ϊ����
	
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
** ����˵��:  eeprom��Ӧ������
** �������:  buf          ���ͣ�uchar 	��0��Ӧ/1����Ӧ
** �������:  
** �� �� ֵ:  ��
** ���ú���: 
***********************************************************/
uchar IIC_ACK()
{
	uchar buf;
	DEFSCLDDR|= SCLHIGH;    //D5(SCL)Ϊ���
	DEFSDADDR&= SDALOW;    //D6(SDA)Ϊ����
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
	DEFSDADDR|= SDAHIGH;  //D6(SDA)Ϊ���
	_NOP();
	return buf;
}

/**********************************************************
** ����˵��:  ��eeprom��ֵ
** �������:  addr		  ���ͣ�uint  �������ĵ�ַ
** �������:  buf          ���ͣ�uchar ������������
** �� �� ֵ:  ��
** ���ú���: 
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
	IIC_SEND((comm.buf2[1] <<1)| 0xa0);//д������
	IIC_ACK();

	IIC_SEND(comm.buf2[0]);
	IIC_ACK();

	IIC_STAR();
	IIC_SEND((comm.buf2[1] <<1)|0xa1);//������
   	IIC_ACK();
	
	buf = IIC_GET();
	IIC_STOP();

    return buf;
}

/**********************************************************
** ����˵��:  дeeprom
** �������:  addr		  ���ͣ�uchar ��д���ݵĵ�ַ
		 	 buf          ���ͣ�uchar ������������
** �������:  
** �� �� ֵ:  ��
** ���ú���: 
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
	IIC_SEND((comm.buf2[1]<<1) | 0xa0);//д������
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
	
	IIC_SEND(buf);//д����
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
** ����˵��: ��EEPROM��һ������
** �������: START_ADDR : ��ʼ��ַ
             NUM        : ��Ҫ��������ռ���ֽ�����������2���ֽڵ�У���룩
** �������:  
** �� �� ֵ:  ��
** ���ú���: 
***********************************************************/
uchar LOAD_EEPROM(uint START_ADDR, uchar NUM)
{
	uint v1, v2;
	uchar j;
	uint add;
    TIMSK = 0X0;
    asm("cli");       //���жϣ���ֹ��EEPROM�����з����жϣ����Ҷ�������

    for (j = 0; j <= NUM+1;j++)
	{
		add=__BACKUP_AREA0__ +START_ADDR+j;
		sdb[j]=RROM(add);
	}

	v1 = crc_fun(sdb, NUM);
	v2 = sdb[NUM] + (sdb[NUM+1]<<8);
	TIMSK = 0X01;
	asm("sei");    //��EEPROM��ɣ����ж�
	
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
** ����˵��: ��EEPROM��дһ������
** �������: START_ADDR : ��ʼ��ַ
             NUM        : ��Ҫд������ռ���ֽ�����������2���ֽڵ�У���룩
** �������:  
** �� �� ֵ:  ��
** ���ú���: 
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
	asm("cli");         //���жϣ���ֹдEEPROM�����з����жϣ�����д������
	
    for (j = 0; j <= NUM+1; j++)
	{
		add=__BACKUP_AREA0__ +START_ADDR+j;
		WROM(add,sdb[j]);
	}	
	
	TIMSK = 0X01;
	asm("sei");         //дEEPROM��ɣ����ж�
}
  

