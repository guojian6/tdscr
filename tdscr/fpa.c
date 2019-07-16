/************************************************************
 * Copyright (c) 2004-2005, POWERCOM Co., All Rights Reserved.
 * File        :"fpa.c"
 * Description :

 * Author      : Lu yi
 * Date        : 2005-02-20
 * Version     : 1.0
 * Revisions   :
 × Revision, 
 * 	20050616, by xhl
 *		增加光模块部门,关键字:__OPT_MODULE__
 ************************************************************/
//#ifdef __PA_MODULE__
#include "def.h"
#include "all.h"
#define A_CLK		4
#define A_DIN		5
#define A_UD_LE		4
#define A_UW_LE		6
#define A_DD_LE		3
#define A_DW_LE		7


extern uint crc_fun(uchar *pbuf, uchar numc);
extern void UPDATE_EEPROM(uint START_ADDR, uchar NUM);
extern uchar LOAD_EEPROM(uint START_ADDR, uchar NUM);

extern void Update_DAC(uchar addr, uint v);


extern void PA_PowerOff();
extern void PA_PowerOn();
extern void delayx(uint x);   

extern __flash struct ParaListStruct ParaList_TABD[];

uchar  T_vswr_table[40]={
             50,50,50,50,45,35,30,25,23,21,
             19,18,17,16,15,14,14,13,13,13,
             12,12,12,11,11,11,11,11,11,11,
             11,11,11,11,10,10,10,10,10,10,
            };

uchar VerName[]="GSM_LTE190213";

extern schar CalibrationWrite[4];  //校准值设置标志
extern schar CalibrationWrite_ro[4];  //校准值设置标志
extern schar CalibrationWrite_pi;  //校准值设置标志
extern schar TempTabParaCount;
extern schar  TempTabBuf[200];
extern schar FreqTabBuf[10],FreqTabParaCount;
extern void Init_IT_Proc();

uchar cur_temp, old_temp;
uint ch_last;
uchar My_Fre(uint cch)      //ly 2005-04-28
{
	uchar i;
	for(i=0;i<3;i++){
		if((cch >= ITab.CTab[i])&&(cch < ITab.CTab[i+1])) return i+1;
		if(cch > ITab.CTab[3]) return 3;
		}
	return 0;//查找不超过返回之间某个值
}

uchar My_Temp()
{
	  uchar i;
	  for(i=0;i<MAX_TEMPTABLEN;i++){   	  
		  if((my.temp >= ITab.TTab[i])&&(my.temp < ITab.TTab[i+1]))return i+1;
		  if (my.temp > ITab.TTab[MAX_TEMPTABLEN-1]) return MAX_TEMPTABLEN-1;
		  	 
	}
	return 0;//查找不超过返回之间某个值
}

void Set_DW_W_Attenuation()//gsm下数
{
    uchar i,j;
	uchar atten;
	schar i1;
  	i1 = My_Temp()*TempTabParaCount;  
   atten = my.dp_atten[0]+my.dp_atten[2] + TempTabBuf[i1+2];
//atten = my.dp_atten[0]+my.dp_atten[2] ;
  if((atten > 31)&&(atten<128)) atten =31;
  else if(atten>=128) atten=0;
  atten<<=1;
  atten&=0x3f;
  clrbit(PORTC,A_DW_LE);
  for(j=0;j<5;j++);
  for(i=0;i<6;i++)
  	{
  	clrbit(PORTC, A_CLK);
	for(j=0;j<5;j++);
	if(atten&0x20) setbit(PORTC, A_DIN);
	else clrbit(PORTC, A_DIN);
	for(j=0;j<5;j++);
	setbit(PORTC, A_CLK);
	for(j=0;j<5;j++);
	atten<<=1;
  	}
  	clrbit(PORTC,A_CLK);
  		for(j=0;j<5;j++);
  	setbit(PORTC,A_DW_LE);
  		for(j=0;j<50;j++);
  	clrbit(PORTC,A_DW_LE);
  
}


void Set_DW_Dcs_Attenuation()//lte dn 
{
    uchar i,j;
	uchar atten;
		schar i1;
  	i1 = My_Temp()*TempTabParaCount;  
	atten= my.dp_atten[1]+my.dp_atten[3] + TempTabBuf[i1+3];
	//my.dp_atten[3] = TempTabBuf[i1+3];
	//atten= my.dp_atten[1]+my.dp_atten[3] ;
  if((atten > 31)&&(atten<128)) atten =31;
  else if(atten>=128) atten=0;
  atten<<=1;
  atten&=0x3f;
  clrbit(PORTB,A_DD_LE);
  for(j=0;j<5;j++);
  for(i=0;i<6;i++)
  	{
  	clrbit(PORTC, A_CLK);
	for(j=0;j<5;j++);
	if(atten&0x20) setbit(PORTC, A_DIN);
	else clrbit(PORTC, A_DIN);
	for(j=0;j<5;j++);
	setbit(PORTC, A_CLK);
	for(j=0;j<5;j++);
	atten<<=1;
  	}
  	clrbit(PORTC,A_CLK);
  		for(j=0;j<5;j++);
  	setbit(PORTB,A_DD_LE);
  		for(j=0;j<50;j++);
  	clrbit(PORTB,A_DD_LE);
	
}

void Set_Up_W_Attenuation()
{
uchar i,j;
	uchar atten;
		schar i1;
  	i1 = My_Temp()*TempTabParaCount;  
		atten = my.lna_atten[0] + my.lna_atten[2] + TempTabBuf[i1+0];		
		//atten = my.lna_atten[0] + my.lna_atten[2] ;		
		  if((atten > 31)&&(atten<128)) atten =31;
  else if(atten>=128) atten=0;
  atten<<=1;
  atten&=0x3f;
  clrbit(PORTC,A_UW_LE);
  for(j=0;j<5;j++);
  for(i=0;i<6;i++)
  	{
  	clrbit(PORTC, A_CLK);
	for(j=0;j<5;j++);
	if(atten&0x20) setbit(PORTC, A_DIN);
	else clrbit(PORTC, A_DIN);
	for(j=0;j<5;j++);
	setbit(PORTC, A_CLK);
	for(j=0;j<5;j++);
	atten<<=1;
  	}
  	clrbit(PORTC,A_CLK);
  		for(j=0;j<5;j++);
  	setbit(PORTC,A_UW_LE);
  		for(j=0;j<50;j++);
  	clrbit(PORTC,A_UW_LE);
}


void Set_Up_Dcs_Attenuation()
{
    uchar i,j;
	uchar atten;
    schar i1;
  	i1 = My_Temp()*TempTabParaCount;  
	atten= my.lna_atten[1] + my.lna_atten[3] + TempTabBuf[i1+1];  
	//atten= my.lna_atten[1] + my.lna_atten[3] ;  
if((atten > 31)&&(atten<128)) atten =31;
  else if(atten>=128) atten=0;
  atten<<=1;
  atten&=0x3f;
  clrbit(PORTB,A_UD_LE);
  for(j=0;j<5;j++);
  for(i=0;i<6;i++)
  	{
  	clrbit(PORTC, A_CLK);
	for(j=0;j<5;j++);
	if(atten&0x20) setbit(PORTC, A_DIN);
	else clrbit(PORTC, A_DIN);
	for(j=0;j<5;j++);
	setbit(PORTC, A_CLK);
	for(j=0;j<5;j++);
	atten<<=1;
  	}
  	clrbit(PORTC,A_CLK);
  		for(j=0;j<5;j++);
  	setbit(PORTB,A_UD_LE);
  		for(j=0;j<50;j++);
  	clrbit(PORTB,A_UD_LE);
}

void BiasCompensate_Proc()
{
         sint n1,n2,n3,n4,n5,n6,n7,n8;
	uchar i1;
	
	   if ((my.realstatus==1)||(my.alarm_state == 1)) return;
    	  i1 = My_Temp()*TempTabParaCount;  //727 
    	  
    	    Set_DW_W_Attenuation();
  Set_DW_Dcs_Attenuation();
     	    Set_Up_W_Attenuation();
  Set_Up_Dcs_Attenuation();
  
      if(my.rfswmode==0)
      {
      	if(my.rfswchan==2)
      		{
      			PORTA &=0xfe;
      			PORTB &=0xFE;
      			delayx(10);
      			PORTB |=0x02;

      			delayx(10);
      			PORTB |=0x04;
      			delayx(10);
      				if (my.sw0==0)	PORTD &= 0xfB;
        else			PORTD |= 0x04;
        //	delayx(100);
//	if (my.sw1==0 )	PORTC &=0xF7;
//        else			PORTC |= 0x08;
   
        
        	delayx(10);
         if (my.upsw0==0 )	PORTD &= 0xf7;
        else			PORTD |=0x08; 
        		delayx(10);
//	if (my.upsw1==0 )	PORTC &= 0xfb;
//        else			PORTC |= 0x04;  
//my.sw2 = 0;
if(my.sw2 == 0) //自动都关或者高阻待定
				    	     { 
				    	       PORTC &=0xF7;//无上拉
				    	       PORTC &=0xFB;//无上拉
				    	       DDRC &=0xF7;//设置成输入  
				    	       DDRC &=0xFB;//设置成输入  
				             }
      			}
      		else
      			{
      			
      			PORTB &=0xFD; 			
      			PORTB &=0xFB;
      			delayx(10);
      			PORTB |=0x01;
      			delayx(10);
      			PORTA |=0x01;
      			delayx(10);
      			
				    	       PORTC &=0xF7;//无上拉
				    	       PORTC &=0xFB;//无上拉
				    	       DDRC &=0xF7;//设置成输入  
				    	       DDRC &=0xFB;//设置成输入  
      			}
    	}
    	else
    	{
    		
    	}
//  	  n1 = ((signed int)(TempTabBuf[i1+0]))*10;
//  	  n2 = ((signed int)(TempTabBuf[i1+1]))*10;
//  	  n3 = ((signed int)(TempTabBuf[i1+2]))*10;
//  	  n4 = ((signed int)(TempTabBuf[i1+3]))*10;
  	  n1 = 0;
  	  n2 = 0;
  	  n3 = 0;
  	  n4 = 0;
//  	  n5 = ((signed int)(TempTabBuf[i1+4]));
//	  n6 = ((signed int)(TempTabBuf[i1+5]));
//	  n7 = ((signed int)(TempTabBuf[i1+6]));
//	  	  n8 = ((signed int)(TempTabBuf[i1+7]));
           if(my.upsw0 == 1) Update_DAC(DAC_B,my.voltage [DAC_B]+n1); 
           else                      Update_DAC(DAC_B,0);
//           if(my.upsw1== 1) Update_DAC(DAC_C,my.voltage [DAC_C]+n2);
//	   			 else                     Update_DAC(DAC_C,0);
           if(my.sw0 == 1) Update_DAC(DAC_A,my.voltage [DAC_A]+n3);
           else                      Update_DAC(DAC_A,0);
//		       if(my.sw1 == 1) Update_DAC(DAC_D,my.voltage [DAC_D]+n4);
//           else                      Update_DAC(DAC_D,0);
          if(my.sw2 == 2){  //TD UP
           Update_DAC(DAC_C,my.voltage [DAC_C]+n2); 
           Update_DAC(DAC_D,0);

           }
           else if(my.sw2 == 1)//郭坚add DN
           {
           Update_DAC(DAC_C,0);
           Update_DAC(DAC_D,my.voltage [DAC_D]+n4);
           }
           else{
           Update_DAC(DAC_C,my.voltage [DAC_C]+n2); 
           Update_DAC(DAC_D,my.voltage [DAC_D]+n4);
           }           	
          // Update_DAC(DAC_B,my.voltage [DAC_B]+n1);  
          // Update_DAC(DAC_C,my.voltage [DAC_C]+n3); 
          // Update_DAC(DAC_D,my.voltage [DAC_D]+n4); 
          // Update_DAC(DAC_G,my.voltage [DAC_G]+n2);

}


uint Get_Voltage(uchar ch)
{ 
    uint k;
	k = adct[ch].result * 5;
	return (k);
}

void Auto_ProtectPA()
{
	sint     maxpo_t1, maxpo_t2, maxpo_t3, maxpo_t4; 
	maxpo_t1 = my.po_maxref1;
	maxpo_t1 = maxpo_t1*10;
	maxpo_t2 = my.po_maxref2;
	maxpo_t2 = maxpo_t2*10;
	maxpo_t3 = my.po_maxref3;
	maxpo_t3 = maxpo_t3*10;
	maxpo_t4 = my.po_maxref4;
	maxpo_t4 = maxpo_t4*10;
	switch(my.state){
		case 0:
			if(my.temp >= MAX_TEMP)	
			{
			  my.state = 1;  
			  break;
			}

			if(my.po1>= maxpo_t1)	
			{
			  my.state = 4;
			  break;
			}
			if(my.po2>= maxpo_t2)	
			{
			  my.state = 7;
			  break;
			}
			if(my.po3>= maxpo_t3)	
			{
			  my.state = 10;
			  break;
			}	
			if(my.po4>= maxpo_t4)	
			{
			  my.state = 13;
			  break;
			}	            
			break;
		case 1://过温度处理
			if(my.temp >= MAX_TEMP)	my.state = 2;
			else 					my.state = 0;
			break;
		case 2:
			if(my.temp >= MAX_TEMP){
				my.state = 3;
				PORTD &= 0xf7;//过问告警，上下行功放全部关掉
				PORTC &= 0xfB;//过问告警，上下行功放全部关掉
				PORTD &= 0xfB;//过问告警，上下行功放全部关掉
				PORTC &= 0xf7;//过问告警，上下行功放全部关掉
        Update_DAC(DAC_B,0);
				Update_DAC(DAC_C,0);
				Update_DAC(DAC_A,0);
				Update_DAC(DAC_D,0);
				my.realstatus = 1;
			}
			else my.state = 0;
			break;
		case 3://等待温度下降到60度
			if(my.temp < 60){//过温度恢复 重新开功放
				my.state = 0;
				if(my.realstatus == 1)
				{
				  my.realstatus = 0;
				 // PA_PowerOn(); 
				 
				       if (my.upsw0==0 )	PORTD &= 0xf7;
        else			PORTD |=0x08; 
	if (my.sw0==0 )	PORTC &= 0xfb;
        else			PORTC |= 0x04;  
//	if (my.upsw1==0)	PORTD &= 0xfB;
//        else			PORTD |= 0x04;
//	if (my.sw1==0 )	PORTC &=0xF7;
//        else			PORTC |= 0x08;

   
       BiasCompensate_Proc(); 
			  }
			}
			break;
		case 4:
			if(my.po1 >= maxpo_t1) 	my.state = 5;
			else 			my.state = 0;
			break;
		case 5:
			if(my.po1 >= maxpo_t1) 	{
				my.state = 6;
				//PORTC |= 0x04;//过功关闭功放
				//PORTD &= 0xf7;
				my.wait1 = 0;
				my.alarm_state_po1= 1;//过功率告警
			}
			else 			my.state = 0;
			break;
		case 6:
			if(++my.wait1 > WAIT1M){
				my.state = 0;
				if(my.alarm_state_po1 == 1)
				{
				  my.alarm_state_po1 = 0;
				  //PORTC &=0xfb;//dcs-on
				  if (my.upsw0 )
				  PORTD |=0x08; 
			  }
			}
			break;
		case 7:
			if(my.po2 >= maxpo_t2) 	my.state = 8;
			else 			my.state = 0;
			break;
		case 8:
			if(my.po2 >= maxpo_t2) 	{
				my.state =9;
				//PORTC |= 0x10;//过功关闭功放
				//PORTC |= 0x40;
				//PORTC &= 0xfB;
				my.wait2 = 0;
				my.alarm_state_po2= 1;//过功率告警
			}
			else 			my.state = 0;
			break;
		case 9:
			if(++my.wait2 > WAIT1M){
				my.state = 0;
				if(my.alarm_state_po2 == 1)
				{
				  my.alarm_state_po2 = 0;
				  //PORTC &=0xef;//w-on
				  //PORTC &= 0xbf;
				  //if (my.upsw1)	
				  //PORTC |= 0x04;
			  }
			}
			break;
		case 10:
			if(my.po3 >= maxpo_t3) 	my.state = 11;
			else 			my.state = 0;
			break;
		case 11:
			if(my.po3>= maxpo_t3) 	{
				my.state =12;
				//PORTC |= 0x40;//过功关闭功放
				//PORTC |= 0x10;
				//PORTD &= 0xfB;
				my.wait3= 0;
				my.alarm_state_po3= 1;//过功率告警
			}
			else 			my.state = 0;
			break;
		case 12:
			if(++my.wait3 > WAIT1M){
				my.state = 0;
				if(my.alarm_state_po3 == 1)
				{
				  my.alarm_state_po3 = 0;
				 // PORTC &= 0xbf;//lte-on
				 //PORTC &=0xef;//w-on
				 if (my.sw0)
				 PORTD |= 0x04;
			  }
			}
			break;
				case 13:
			if(my.po4 >= maxpo_t4) 	my.state = 14;
			else 			my.state = 0;
			break;
		case 14:
			if(my.po4>= maxpo_t4) 	{
				my.state =15;
				//PORTC |= 0x40;//过功关闭功放
				//PORTC |= 0x10;
				//PORTC &= 0xf7;
				my.wait4= 0;
				my.alarm_state_po4= 1;//过功率告警
			}
			else 			my.state = 0;
			break;
		case 15:
			if(++my.wait4 > WAIT1M){
				my.state = 0;
				if(my.alarm_state_po4 == 1)
				{
				  my.alarm_state_po4 = 0;
				 // PORTC &= 0xbf;//lte-on
				 //PORTC &=0xef;//w-on
				 //if (my.sw1)
				 //PORTC |= 0x08;
			  }
			}
			break;


			
		default:
			my.state = 0;
	}
}

void VoltageConvertTo(uchar i0)
{
        uchar jj,i,k;
	uint iv;
	slong ii;
	uchar *pstr;	
	iv = Get_Voltage(i0);//加15，表示修正值
	switch(i0){

		case 7://温度
			      ii = iv;
			      ii = 1000*ii/1024;
			      my.votemp = ii;
						ii = ii/10 - 50;
						my.temp = ii;
						my.temp  += (own.OffsetAdc[5]/10);					
			if (my.temp != temp1)
			{
				cur_temp = My_Temp();
		  	if(cur_temp != old_temp)
		  	{
		  		old_temp = cur_temp;
		  		cmp_flag = 1;
		  		if(my.realstatus == 0)
			    {
					  BiasCompensate_Proc();  
					}
		  	}  
				temp1 = my.temp;
			}
			break;	

		case 4:	
			ii = iv;     //gsm上行功放输出功率检测 
			ii = 1000*ii/1024;
			my.vopo1 = ii;
			
			if(CalibrationWrite[0] == 1)
			{
				CalibrationWrite[0] = 0;
        amplifier.voltage1[0] = my.vopo1;
		    pstr = (uchar *) &amplifier;
		    for (i = 0;i < sizeof(amplifier); i++) sdb[i] = pstr[i];
		    UPDATE_EEPROM(EE_ADDR_CALIBRATION, sizeof(amplifier));			
      }	
			if(CalibrationWrite[0] == 2)
			{
				CalibrationWrite[0] = 0;
        amplifier.voltage2[0] = my.vopo1;
        if((amplifier.voltage1[0] == amplifier.voltage2[0])||(amplifier.value1[0] == amplifier.value2[0]))
        {
        	amplifier.power_k[0] = 1;
        	amplifier.power_b[0] = 0;
        }	
        else
        {
          amplifier.power_k[0] = 1000*((long)(amplifier.value1[0]-amplifier.value2[0]))/((long)(amplifier.voltage1[0]-amplifier.voltage2[0]));	
          amplifier.power_b[0] = ((long)amplifier.value2[0])-((long)amplifier.power_k[0])*((long)amplifier.voltage2[0])/1000 ;                              
        }                            			
		    pstr = (uchar *) &amplifier;
		    for (i = 0;i < sizeof(amplifier); i++) sdb[i] = pstr[i];//EEbuf
		    UPDATE_EEPROM(EE_ADDR_CALIBRATION, sizeof(amplifier));
			}
			
									ii =  amplifier.power_k[0];
			ii =  ii*my.vopo1/100 + amplifier.power_b[0]*10;
			my.po1 = ii;
			my.po1 = my.po1 + own.OffsetAdc[0];//偏置调整,
			my.po1_2g = my.po1/10;
	break;
	case 2:
		ii = iv;     //lte下行功放输出功率检测 
			ii = 1000*ii/1024;
			my.vopo4 = ii;
			
			if(CalibrationWrite[3] == 1)
			{
				CalibrationWrite[3] = 0;
        amplifier.voltage1[3] = my.vopo4;
		    pstr = (uchar *) &amplifier;
		    for (i = 0;i < sizeof(amplifier); i++) sdb[i] = pstr[i];
		    UPDATE_EEPROM(EE_ADDR_CALIBRATION, sizeof(amplifier));			
      }	
			if(CalibrationWrite[3] == 2)
			{
				CalibrationWrite[3] = 0;
        amplifier.voltage2[3] = my.vopo4;
        if((amplifier.voltage1[3] == amplifier.voltage2[3])||(amplifier.value1[3] == amplifier.value2[3]))
        {
        	amplifier.power_k[3] = 1;
        	amplifier.power_b[3] = 0;
        }	
        else
        {
          amplifier.power_k[3] = 1000*((long)(amplifier.value1[3]-amplifier.value2[3]))/((long)(amplifier.voltage1[3]-amplifier.voltage2[3]));	
          amplifier.power_b[3] = ((long)amplifier.value2[3])-((long)amplifier.power_k[3])*((long)amplifier.voltage2[3])/1000 ;                              
        }                            			
		    pstr = (uchar *) &amplifier;
		    for (i = 0;i < sizeof(amplifier); i++) sdb[i] = pstr[i];//EEbuf
		    UPDATE_EEPROM(EE_ADDR_CALIBRATION, sizeof(amplifier));
			}
			
									ii =  amplifier.power_k[3];
			ii =  ii*my.vopo4/100 + amplifier.power_b[3]*10;
			my.po4 = ii;
			my.po4 = my.po4 + own.OffsetAdc[3];//偏置调整,
			my.po4_2g = my.po4/10;
				break;
	 case 5:	//w
			ii = iv;     //下行gsm输出功率检测 
			ii = 1000*ii/1024;
			my.vopo3 = ii;
			
			if(CalibrationWrite[2] == 1)
			{
				CalibrationWrite[2] = 0;
        amplifier.voltage1[2] = my.vopo3;
		    pstr = (uchar *) &amplifier;
		    for (i = 0;i < sizeof(amplifier); i++) sdb[i] = pstr[i];
		    UPDATE_EEPROM(EE_ADDR_CALIBRATION, sizeof(amplifier));			
      }	
			if(CalibrationWrite[2] == 2)
			{
				CalibrationWrite[2] = 0;
        amplifier.voltage2[2] = my.vopo3;
        if((amplifier.voltage1[2] == amplifier.voltage2[2])||(amplifier.value1[2] == amplifier.value2[2]))
        {
        	amplifier.power_k[2] = 1;
        	amplifier.power_b[2] = 0;
        }	
        else
        {
          amplifier.power_k[2] = 1000*((long)(amplifier.value1[2]-amplifier.value2[2]))/((long)(amplifier.voltage1[2]-amplifier.voltage2[2]));	
          amplifier.power_b[2] = ((long)amplifier.value2[2])-((long)amplifier.power_k[2])*((long)amplifier.voltage2[2])/1000 ;                              
        }                            			
		    pstr = (uchar *) &amplifier;
		    for (i = 0;i < sizeof(amplifier); i++) sdb[i] = pstr[i];//EEbuf
		    UPDATE_EEPROM(EE_ADDR_CALIBRATION, sizeof(amplifier));
			}
			
									ii =  amplifier.power_k[2];
			ii =  ii*my.vopo3/100 + amplifier.power_b[2]*10;
			my.po3 = ii;
			my.po3 = my.po3 + own.OffsetAdc[2];//偏置调整,
			my.po3_2g = my.po3/10;
		
	break;
	
 case 1:	//lte
			ii = iv;     //上行lte输出功率检测 
			ii = 1000*ii/1024;
			my.vopo2 = ii;
			
			if(CalibrationWrite[1] == 1)
			{
				CalibrationWrite[1] = 0;
        amplifier.voltage1[1] = my.vopo2;
		    pstr = (uchar *) &amplifier;
		    for (i = 0;i < sizeof(amplifier); i++) sdb[i] = pstr[i];
		    UPDATE_EEPROM(EE_ADDR_CALIBRATION, sizeof(amplifier));			
      }	
			if(CalibrationWrite[1] == 2)
			{
				CalibrationWrite[1] = 0;
        amplifier.voltage2[1] = my.vopo2;
        if((amplifier.voltage1[1] == amplifier.voltage2[1])||(amplifier.value1[1] == amplifier.value2[1]))
        {
        	amplifier.power_k[1] = 1;
        	amplifier.power_b[1] = 0;
        }	
        else
        {
          amplifier.power_k[1] = 1000*((long)(amplifier.value1[1]-amplifier.value2[1]))/((long)(amplifier.voltage1[1]-amplifier.voltage2[1]));	
          amplifier.power_b[1] = ((long)amplifier.value2[1])-((long)amplifier.power_k[1])*((long)amplifier.voltage2[1])/1000 ;                              
        }                            			
		    pstr = (uchar *) &amplifier;
		    for (i = 0;i < sizeof(amplifier); i++) sdb[i] = pstr[i];//EEbuf
		    UPDATE_EEPROM(EE_ADDR_CALIBRATION, sizeof(amplifier));
			}
			
	
						ii =  amplifier.power_k[1];
			ii =  ii*my.vopo2/100 + amplifier.power_b[1]*10;
			my.po2 = ii;
			my.po2 = my.po2 + own.OffsetAdc[1];//偏置调整,
			my.po2_2g = my.po2/10;
	break;

			
   			 		
		default:
			break;
	}	
	if((i0 == 1)||(i0 == 2)||(i0 ==4)||(i0 == 5)||(i0 == 7)) Auto_ProtectPA();

}


__flash uint org_voltage[] = {2000,2000,2000,2000,2000,2000,2000,2000};
__flash schar org_temp[] = {-40,-35,-30,-25,-20,-15,-10,-5,0,5,10,15,20,25,30,35,40,45,50,55,60,65,70,75,80,85,90}; 
//__flash   uint org_freq[]	= {75,150,225,300}; 
__flash schar volt_comp1[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; 
__flash schar volt_comp2[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
__flash schar volt_comp3[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
__flash schar volt_comp4[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
//__flash schar volt_comp5[]={4,4,4,4,4,3,3,3,2,2,1,1,0,0,0,0,0,0,-1,-2,-2,-2,-2,-2,-2,-2,-2};
//__flash schar volt_comp6[]={4,4,4,4,4,4,3,3,2,2,1,1,0,0,0,0,0,0,0,-1,-1,-1,-1,-1,-1,-1,-1};
//__flash schar volt_comp7[]={2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,-2,-2,-2,-3,-3,-3,-3,-3};
//__flash schar volt_comp8[]={2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,-2,-2,-2,-2,-3,-3,-3,-3,-3};
uchar Init_Load()
{
	uchar i;
	uchar *pstr;
	slow_flag = 0;

	delayx(300);
	if((LOAD_EEPROM(EE_ADDR_OWN, sizeof(own)))&&(reset_f != RESET_PARA))
	{
    	pstr = (uchar *) &own;
    	for(i = 0;i<sizeof(own);i++) pstr[i] = sdb[i];
    }
    else {
    	own.addr = 1;
    	own.pver = 1;
    	own.thrMaxCurrent = 500;//单位：mA
    	own.thrMinCurrent =500;                                                                                                                                                                                                                           
	for(i = 0;i<8;i++) own.OffsetAdc[i] = 0;
     	
    }
     if((LOAD_EEPROM(EE_ADDR_ITTAB, 54))&&(reset_f != RESET_PARA))//温度频率表
    {
    	for(i = 0;i<MAX_TEMPTABLEN;i++) ITab.TTab[i] = sdb[i];
    	//for(i = 0;i<4;i++)  ITab.CTab[i] = sdb[(i<<1)+27]+(sdb[(i<<1)+28]<<8);
    }
	else
	{
		for(i =	0;i<MAX_TEMPTABLEN;i++) ITab.TTab[i] = org_temp[i];	
		//for(i =	0;i<4;i++) ITab.CTab[i]	= org_freq[i];
	}
	for(i = 0;i<200;i++){
		TempTabBuf[i] = 0;
	}
    if((LOAD_EEPROM(EE_ADDR_TEMPTAB, 200))&&(reset_f != RESET_PARA))//温度补偿表
    {
    	for(i = 0;i<200;i++) TempTabBuf[i] = sdb[i];
    	
    }
    else
    {
        for(i = 0;i<MAX_TEMPTABLEN;i++) 
        {
          TempTabBuf[4*i]=volt_comp1[i];
          TempTabBuf[4*i+1]=volt_comp2[i];
          TempTabBuf[4*i+2]=volt_comp3[i];
          TempTabBuf[4*i+3]=volt_comp4[i];
//          TempTabBuf[7*i+4]=volt_comp5[i];
//          TempTabBuf[7*i+5]=volt_comp6[i];
//          TempTabBuf[7*i+6]=volt_comp7[i];
//          TempTabBuf[7*i+7]=volt_comp8[i];
        }      
    }

    if((LOAD_EEPROM(EE_ADDR_MY, 60))&&(reset_f != RESET_PARA))
    {
	    pstr = (uchar *) &my;
	    for(i = 0;i<60;i++) pstr[i] = sdb[i];
	}
	else{
		//功放参数
		my.upsw0 = 1;//开关
		my.upsw1 = 0;
		my.rfswmode = 0;//手动
		my.sw0 = 1;//开关
		my.sw1 = 0;
		my.rfswchan = 2;//基站
		my.sw2 = 0;
		for(i = 0;i<4;i++)my.dp_atten[i] = 0;
		for(i = 0;i<4;i++)my.lna_atten[i] = 0;
		my.po_maxref1 = 32;
		my.po_maxref2 = 32;
		my.po_maxref3 = 32;
		my.po_maxref4 = 32;
		my.maxpo1_ctr = 30;
		my.maxpo2_ctr = 30;
		my.maxpo3_ctr = 30;
		my.maxpo4_ctr = 30;
		for(i = 0;i<8;i++) my.voltage[i] = org_voltage[i];

	}

//输出功率校准
	if((LOAD_EEPROM(EE_ADDR_CALIBRATION, sizeof(amplifier)))&&(reset_f != RESET_PARA))
	{
    	pstr = (uchar *) &amplifier;
    	for(i = 0;i<sizeof(amplifier);i++) pstr[i] = sdb[i];
  }
  else
  {
    for(i=0;i<4;i++)
    {
	     amplifier.value1[i]=0;
	     amplifier.voltage1[i]=0;
	     amplifier.value2[i]=0;
	     amplifier.voltage2[i]=0;
	     amplifier.power_k[i]=3000/149;
	     amplifier.power_b[i]=-2405/149;    	
    }		
  }
//反向功率校准  
	if((LOAD_EEPROM(EE_ADDR_CALIBRATION_RO, sizeof(amplifier_ro)))&&(reset_f != RESET_PARA))
	{
    	pstr = (uchar *) &amplifier_ro;
    	for(i = 0;i<sizeof(amplifier_ro);i++) pstr[i] = sdb[i];
  }
  else
  {
    for(i=0;i<4;i++)
    {
	     amplifier_ro.value1[i]=0;
	     amplifier_ro.voltage1[i]=0;
	     amplifier_ro.value2[i]=0;
	     amplifier_ro.voltage2[i]=0;
	     amplifier_ro.power_k[i]=1500/79;
	     amplifier_ro.power_b[i]=-5950/79;    	
    }		
  }
//输入功率校准   
	if((LOAD_EEPROM(EE_ADDR_CALIBRATION_PI, sizeof(amplifier_pi)))&&(reset_f != RESET_PARA))
	{
    	pstr = (uchar *) &amplifier_pi;
    	for(i = 0;i<sizeof(amplifier_pi);i++) pstr[i] = sdb[i];
  }
  else
  {
    for(i=0;i<4;i++)
    {
	     amplifier_pi.value1[i]=0;
	     amplifier_pi.voltage1[i]=0;
	     amplifier_pi.value2[i]=0;
	     amplifier_pi.voltage2[i]=0;
	     amplifier_pi.power_k[i]=1;
	     amplifier_pi.power_b[i]=0;    	
    }		
  }
 
	for (i = 0; i< 10;i++)	own.hver[i]=VerName[i]; 	
	own.sver[1] = 0;own.sver[0] = 1;//软件版本
	my.alarm_state=0;
	my.alarm_state_po1=0;
	my.alarm_state_po2=0;
	my.alarm_state_po3=0;
	my.alarm_state_po4=0; 
			//功放参数 ,上电默认关闭防止烧功放
		my.upsw0 = 0;//开关
		my.upsw1 = 0;
		my.rfswmode = 0;//手动
		my.sw0 = 0;//开关
		my.sw1 = 0;
		my.rfswchan = 2;//基站
		my.sw2 = 0;
	my.temp = 30;
	sum_paras = 0;
	ctri =0;
	while(ParaList_TABD[sum_paras].Para != 0xffff) sum_paras++;//统计参数地址个数

	Init_IT_Proc();
    return 0;  

}
