
/************************************************************
 * Copyright (c) 2004-2005, POWERCOM Co., All Rights Reserved.
 * File        :"def.h"
 * Description :

 * Author      : Xu Hailun
 * Date        : 2003-08-24
 * Version     : 1.0
 *
 * Revisions   :
 * 20050616, by ly
 * 只包括6W/12W/16W/上行功放
 ************************************************************/
#include "defines.h"

/*############################ start ##########################*/
#define uchar   unsigned char
#define uint    unsigned int
#define sint    signed int
#define ushort  unsigned short
#define ulong   unsigned long
#define slong   signed long
#define schar   signed char

#define __MODULE_DPA__
////------------------for message process-----------------
#define STXB					0x48
#define EOIB					0x4d
#define SYNCB					0x4E
//#define MSG_SYNC				0x5C
#define MAX_RVBLEN			    180
#define MAX_SDBLEN			    180
#define MAX_MSGLEN				80
#define MAX_TEMP				80
/*--------------- constant declared for system. ----------------*/

#define UP_MODULE_ID0         'U'          //上行模块ID
#define UP_MODULE_ID1         'P'  
#define UP_MODULE_ADDRE       5            //上行模块地址
#define DN_MODULE_ID0         'D'          //下行模块ID
#define DN_MODULE_ID1         'P'  
#define DN_MODULE_ADDRE       6            //下行模块地址

//#define PA_I_ALM_D2			PIND2
//#define PA_OVER_C3			PINC3

//#define ON			1
//#define OFF			0

#define   chkbit(var,bit)  		(var & (0x01 << bit))
#define   setbit(var,bit)    	(var |= (0x01 << bit))
#define   clrbit(var,bit)    	(var &= (~(0x01 << bit)))



#define WAIT1M					30
#define WAIT1S					10

#define IDLE                	    0xee
#define IDLEW                	    0xeeee

#define RESTART 	            	0x52//Rstart
#define	RESET_PARA  	            0x49//Initialize
#define REBOOT	 	           	 	0x42//Boot
#define	ENTER_TEST  	            0x54//Test
#define	UPDATE_FLASH    	    	0x55//Update


/*补偿表长度*/
#define MAX_TEMPTABLEN			27
#define MAX_FREQTABLEN			4

//EEprom map
#define __BACKUP_AREA0__		0x004
#define __BACKUP_AREA1__		0x100
//#define __BACKUP_AREA2__		0x100

#define EE_ADDR_OWN			  	0x00//72
#define EE_ADDR_MY				0x100//25
#define EE_ADDR_ITTAB			0x200//90
#define EE_ADDR_TEMPTAB			0x300//120
#define EE_ADDR_FREQTAB			0x400//120
#define EE_ADDR_CALIBRATION  0x500    //校准值dbm+校准值mv+校准值k+校准值b （存储格式）    输出功率
#define EE_ADDR_CALIBRATION_RO  0x540    //校准值dbm+校准值mv+校准值k+校准值b （存储格式） 反向功率
#define EE_ADDR_CALIBRATION_PI  0x580    //校准值dbm+校准值mv+校准值k+校准值b （存储格式） 输入功率

#define MAX_ADC_WAIT		    1000
#define MAX_SAMPLING		    6
#define MAX_ADCCH		    	  8

/*
struct Time_Stru{
	uint year;
	uchar month;
	uchar day;
	uchar hour;
	uchar minute;
	uchar second;
	uchar week;
};
*/

struct ADC_Para_Stru{
	uint result;
};

struct module_addr{//EEPROM:0-15 //15
     uchar mname[10];
     uchar id[30];//模块ID号
     uchar addr;//模块地址
     uchar pver;//协议版本号
     uchar hver[10];//程序ID
     uchar sver[2];//软件版本
     uchar serial[20];  
     sint  OffsetAdc[8];
     uint  thrMinCurrent;//电流检测的最小告警门限
     uint  thrMaxCurrent;//电流检测的最大告警门限
     
     //以上保存在EEPROM
};

struct PA_Para_Stru{//EEPROM :从16--  共21字节
         uchar upsw0,upsw1,rfswmode;//开关
	uchar sw0,sw1,sw2,rfswchan;//开关
	uchar lna_atten[4],dp_atten[4];//衰减,20050622 增加一个固定衰减:atten[1]
	schar  po_maxref1,po_maxref2,po_maxref3,po_maxref4;
	//sint  maxpo1_ctr_3g,maxpo2_ctr_3g,maxpo3_ctr_3g,maxpo4_ctr_3g;
	uint voltage[8];
	schar  maxpo1_ctr,maxpo2_ctr,maxpo3_ctr,maxpo4_ctr;
	////以上量要保存eeprom;
	sint po1,po2,po3,po4;//输出功率
	schar po1_2g,po2_2g,po3_2g,po4_2g;//输出功率



	schar temp;//温度检测 硬件未设置，置为30度�
	uint current;//电流
	
	uchar alarm_state,alarm_state_po1,alarm_state_po2,alarm_state_po3,alarm_state_po4; 

	uint vopo1,vopo2,vopo3,vopo4;//输出功率的电压值
	uint vocurrent;//电流的电压检测值
	uint votemp;//温度的电压检测
	uchar realstatus;
	uchar currentstatus;
	uchar state;


	uint wait,wait1,wait2,wait3,wait4;

	//其它标志
};

struct Msg_Struct{
	uchar stx;
	uchar ver;
	uchar m_add;
	uchar comm;
	uchar flag;
	uchar param_add[2];
	uchar len;
	uchar data_info[40];
	uchar chk[2];
	uchar eoi;
};

enum{
	MT_INIT=0,
	MT_LEN,
	MT_TYPE,
	MT_RVMSG,
	MT_RVVER
};

///==========for ADC ===================
#define VREF				5 //current refrence voltage.

enum{
	DAC_A=0,
	DAC_B,
	DAC_C,
	DAC_D,
	DAC_E,
	DAC_F,
	DAC_G,
	DAC_H,
	DAC_CTRL0,
	DAC_CTRL1,
	DAC_PRESET,
	DAC_RESERVED,
	DAC_A_B,
	DAC_C_D,
	DAC_E_F,
	DAC_G_H
};

//智能化表格
struct ITab_Struct{
	schar TTab[30];//温度分段
	sint  CTab[10];//信道分段
};

struct ParaListStruct{
	uint Para;
	uchar Len;
	uchar Arith;
};

union buf_union
{
	uint buf1;
	uchar buf2[2];
};

struct Calibration_Struct{
//输出
	schar value1[4];
	uint  voltage1[4];
	schar value2[4];
	uint  voltage2[4];
	sint  power_k[4];
	sint  power_b[4];
////反向
//	schar value1_ro[3];
//        uint  voltage1_ro[3];
//	schar value2_ro[3];
//	uint  voltage2_ro[3];
///	sint  power_k_ro[3];
//	sint  power_b_ro[3];
////输入
//	char value1_pi[2];
//	uint  voltage1_pi[2];
//	char value2_pi[2];
//	uint  voltage2_pi[2];
//	int  power_k_pi[2];
//	int  power_b_pi[2];		
};

