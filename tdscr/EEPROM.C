/************************************************************
 * Copyright (c) 2004-2005, POWERCOM Co., All Rights Reserved.
 * File        :"eeprom.c"
 * Description :

 * Author      : Xu Hailun
 * Date        : 2003-08-29
 * Version     : 1.0
 * Revisions   :
 ************************************************************/
#include "def.h"
#include "all.h"

#include <inavr.h>

#define uchar   unsigned char
#define uint    unsigned int

extern uint crc_fun(uchar *pbuf, uchar numc);
#ifndef __EXTEND_24C16__
#define MAX_EEPROM_WAIT			10000


void WRITE_EEPROM(uint START_ADDR, uchar NUM)
{
	uchar i;
	uint l;
	uchar cSREG;
	cSREG = SREG;
	asm("cli");
	for(i=0;i<NUM;i++){
		l = 0;
		while (chkbit(EECR,EEWE)){
			l++;
//			if(l>MAX_EEPROM_WAIT) goto ErrProc;
		}
		EEAR = START_ADDR+i;
		EEDR = sdb[i];
		if(eewp != 0x1234) break;
		setbit(EECR,EEMWE);
		setbit(EECR,EEWE);
		asm("wdr");//clear watchdog counter.
	}
	SREG = cSREG;
	asm("sei"); // enable interrupts
}

void READ_EEPROM(uint START_ADDR, uchar NUM)
{
	uchar i;
	uint l;
	uchar cSREG;
	cSREG = SREG;
	asm("cli");
	for(i=0;i<NUM;i++){
		l = 0;
		while (chkbit(EECR,EEWE)){
			l++;
			//if(l>MAX_EEPROM_WAIT) goto ErrProc;
		}
		EEAR = START_ADDR+i;
		setbit(EECR,EERE);
		sdb[i] = EEDR;
		asm("wdr");//clear watchdog counter.
	}
	SREG = cSREG;
	asm("sei"); // enable interrupts

}



void UPDATE_EEPROM(uint START_ADDR, uchar NUM)
{//保存的数据同时保存到1,2区.
	uint v1;
	v1 = crc_fun(sdb, NUM);
	sdb[NUM] = v1 & 0xff;
	sdb[NUM+1] = (v1 >> 8) & 0xff;
	WRITE_EEPROM( __BACKUP_AREA0__ + START_ADDR,  NUM+2);
#ifdef __BACKUP_E2PROM__
	WRITE_EEPROM( __BACKUP_AREA1__ + START_ADDR,  NUM+2);
#endif
}

uchar LOAD_EEPROM(uint START_ADDR, uchar NUM)
{
	uint v1, v2;
	uchar ok = 0;
	READ_EEPROM( __BACKUP_AREA0__ + START_ADDR,  NUM+2);
	v1 = crc_fun(sdb, NUM);
	v2 = sdb[NUM] + (sdb[NUM+1]<<8);
	if(v1 == v2) ok = 1;
#ifdef __BACKUP_E2PROM__
	READ_EEPROM( __BACKUP_AREA1__ + START_ADDR,  NUM+2);
	v1 = crc_fun(sdb, NUM);
	v2 = sdb[NUM] + (sdb[NUM+1]<<8);
	if(v1 == v2) ok = ok+2;
	if(ok == 1)
	{//第一区的数据正确,第二区数据错误, 用1区数据覆盖2区数据
		READ_EEPROM( __BACKUP_AREA0__ + START_ADDR,  NUM+2);
		WRITE_EEPROM( __BACKUP_AREA1__ + START_ADDR,  NUM+2);
		return 1;
	}
	else if(ok == 2)
	{//1区数据错误,2区数据正确, 用2区数据覆盖1区数据
		READ_EEPROM( __BACKUP_AREA1__ + START_ADDR,  NUM+2);
		WRITE_EEPROM( __BACKUP_AREA0__ + START_ADDR,  NUM+2);
		return 1;
	}
	else if(ok == 3) //1,2区的数据全部正确
		return 1;
#else
	if(ok)
		return 1;
#endif
	return 0;
}

//=========================24c04================================
//#ifdef __EXTEND_24C04__
#else

#define EE_SCL      PORTC5
#define EE_SDA      PORTC4
uchar EE_DATA,WORD_ADDR;
uint EE_ADDR;
//uchar i;
//#define EE_ADDR  0x00;

void EE_CMD(uchar CTRL_BYTE, uint OP_ADDR)
{
	uchar WORD_ADDR;
	uchar i;
	// Set page block address 
	if (OP_ADDR > 0xFF)	CTRL_BYTE = CTRL_BYTE | 0x07;
	// Get word address
	WORD_ADDR = OP_ADDR;
	// Start bit  
	clrbit(PORTC,EE_SCL);
	setbit(PORTC,EE_SDA);
	setbit(PORTC,EE_SCL);
	clrbit(PORTC,EE_SDA);

	for(i = 0 ; i <= 7;i++)
	{
		clrbit(PORTC,EE_SCL);
		if ((CTRL_BYTE & 0x80) == 0) clrbit(PORTC,EE_SDA);
		else setbit(PORTC,EE_SDA);

		setbit(PORTC,EE_SCL);
		CTRL_BYTE = CTRL_BYTE<<1;

	}

	// Acknowledge clock 
	clrbit(PORTC,EE_SCL);
	setbit(PORTC,EE_SCL);

	// Word address 
	for(i = 0 ; i<= 7;i++)
	{
		clrbit(PORTC,EE_SCL);
		if ((WORD_ADDR & 0x80) == 0) clrbit(PORTC,EE_SDA);
		else setbit(PORTC,EE_SDA);
		setbit(PORTC,EE_SCL);
		WORD_ADDR = WORD_ADDR<<1;
	}
	// Acknowledge clock 
	clrbit(PORTC,EE_SCL);
	setbit(PORTC,EE_SCL);
}

void UPDATE_EEPROM(uint START_ADDR, uchar NUM)
{
	uchar i,j,k;
	for(j = 0; j<= NUM-1;j++)
	{
		EE_ADDR = START_ADDR + j;
		EE_DATA = sdb[j];
		EE_CMD(0xA0, EE_ADDR);

		for(i = 0;i <= 7; i++)
		{
			clrbit(PORTC,EE_SCL);
			if ((EE_DATA & 0x80) > 0) setbit(PORTC,EE_SDA);
			else clrbit(PORTC,EE_SDA);
			setbit(PORTC,EE_SCL);
			EE_DATA = EE_DATA<<1;
		}
		// Acknowledge clock
		clrbit(PORTC,EE_SCL);
		setbit(PORTC,EE_SCL);

		// Stop bit  0
		clrbit(PORTC,EE_SCL);
		clrbit(PORTC,EE_SDA);
		setbit(PORTC,EE_SCL);
		setbit(PORTC,EE_SDA);

		DDRC=0x2c;
		// Acknowledge polling  
		clrbit(PORTC,EE_SCL);
		k = 0;
		setbit(PORTC,EE_SCL);
		while (chkbit(PINC,EE_SDA) && (k < 1000))
		{
//			clrbit(PORTC,EE_SCL);
			k++;
			asm("wdr");//clear watchdog counter.
//			setbit(PORTC,EE_SCL);
		}
		DDRC=0x3c;		
	}
}

uchar LOAD_EEPROM(uint START_ADDR, uchar NUM)
{
	
	uchar i,j;
	//uchar ok;
	uint v1, v2;
	for (j = 0; j <= NUM-1;j++)
	{
		EE_DATA = 0;
		EE_ADDR = START_ADDR + j;
		EE_CMD(0xA0, EE_ADDR);

		// Start bit 
		clrbit(PORTC,EE_SCL);
		setbit(PORTC,EE_SDA);
		setbit(PORTC,EE_SCL);
		clrbit(PORTC,EE_SDA);

		// Random read contro2l byte 
		for( i = 0 ;i <= 7;i++)
		{
			clrbit(PORTC,EE_SCL);
			switch(i)
			{
				case 0:
				case 2:
				case 7:setbit(PORTC,EE_SDA);break;
				case 1:
				case 3:
				case 4:
				case 5:clrbit(PORTC,EE_SDA);break;
				case 6:
					if(j > 0xFF) setbit(PORTC,EE_SDA);
					else clrbit(PORTC,EE_SDA);
			}
			setbit(PORTC,EE_SCL);

		}

		// Acknowledge clock
		clrbit(PORTC,EE_SCL);
		setbit(PORTC,EE_SCL);

		DDRC=0x2c;
		for(i = 0;i<= 7;i++)
		{
			clrbit(PORTC,EE_SCL);
			EE_DATA = EE_DATA << 1;
			if (chkbit(PINC,EE_SDA))
				EE_DATA = EE_DATA | 1;
			setbit(PORTC,EE_SCL);
		}

		clrbit(PORTC,EE_SCL);
		setbit(PORTC,EE_SDA);
		setbit(PORTC,EE_SCL);

		sdb[j] = EE_DATA;
		DDRC=0x3c;
		// Stop bit 
		clrbit(PORTC,EE_SCL);
		clrbit(PORTC,EE_SDA);
		setbit(PORTC,EE_SCL);
		setbit(PORTC,EE_SDA);
		//ok = 1;
		asm("wdr");//clear watchdog counter.
	}
	//if(ok == 1) return 1;
	//return 0;
	v1 = crc_fun(sdb, NUM);
	v2 = sdb[NUM] + (sdb[NUM+1]<<8);
	
	if(v1 == v2) return 1;
	return 0;
}
/*
void test_Init()
{
	sdb[0] = 0xaa;
	for(l=0;l<511;l++) UPDATE_EEPROM1(l,1);
	l=0;
	for(;;){
		LOAD_EEPROM1(l,1);
		if(sdb[0] == 0xaa){
			sdb[0] = 0;
			l = (l+1)%512;
			if(l==511) return;
		}
		else for(;;);
	}
}
*/
#endif

//**********************************************************************/
