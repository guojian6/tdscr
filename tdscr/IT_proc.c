/************************************************************
 * Copyright (c) 2004-2005, POWERCOM Co., All Rights Reserved.
 * File        :"log.c"
 * Description :

 * Author      : 
 * Date        : 2004-04-20
 * Version     : 1.0
 * Revisions   :
 ************************************************************/
#include "def.h"
#include "all.h"
#define PTYPE		1
#define PVER		2
#define PCODE		4
#define PCMD		5
#define PANS		6
#define PLEN  		7
#define PDUA  		8

extern void UPDATE_EEPROM(uint START_ADDR, uchar NUM);
extern uchar LOAD_EEPROM(uint START_ADDR, uchar NUM);
extern void s_command(uchar b0,uint i0,uchar b1);
extern void delayx(uint x);
extern void BiasCompensate_Proc();
extern void Set_Attenuation(uchar v);


schar TempTabBuf[200];
schar TempTabParaCount;
schar FreqTabBuf[10],FreqTabParaCount;
schar CalibrationWrite[4];  //校准值设置标志
schar CalibrationWrite_ro[4];  //校准值设置标志
schar CalibrationWrite_pi;  //校准值设置标志

__flash uint TempITabParaAddrU[] = 
	{
	#if defined(__UP_PA_MODULE__)//(__UPLINK_VER2__)&&defined(__STANDARD_MODULE__)
		//0x00b0,0x00b2,0x0028,0x0120,
	 0x0028,
	#endif	
	0xffff
	};
__flash uint TempITabParaAddrD[] = 
	{
		0x0028,0x0029,0x002c,0x002d,		
		0xffff
	};
	
__flash uint FreqITabParaAddrU[] = 
	{
		0xffff
	};
__flash uint FreqITabParaAddrD[] = 
	{
		0xffff
	};
		

void Init_IT_Proc()
{
	TempTabParaCount = 0;
	//FreqTabParaCount = 0;
	while(TempITabParaAddrD[TempTabParaCount]!= 0xffff) TempTabParaCount++;
	//while(FreqITabParaAddrD[FreqTabParaCount]!= 0xffff) FreqTabParaCount++;

}


void IT_Msg_Proc()
{
	uchar i,j,len,pos;
	uchar Calibration_Channel = 0;//通道号
	uint Calibration_id = 0;  //校准值id
	uint mm;
	signed int vv;
	uchar w_flag = 0;
	uchar *pstr;
	{
		mm = rvb[PDUA];
		len = rvb[PLEN];
		pos = PDUA+2;
		switch(mm){
			case 1:
				//if (len > 30) 
				len = MAX_TEMPTABLEN;
				if(rvb[PCMD] == 0xd2){
					for(j=0;j<len;j++)
						ITab.TTab[j] = rvb[pos++];
					w_flag = 1;
					cmp_flag = 1;
				}
				else {
					for(j=0;j<len;j++)
						rvb[pos++] = ITab.TTab[j];
				}
				break;
			case 2:
				//if (len > 60) 
				len = MAX_FREQTABLEN;
				if(rvb[PCMD] == 0xd2){
					for(j=0;j<len;j++){
						ITab.CTab[j] = rvb[pos]+(rvb[pos+1]<<8);
						pos += 2;
					}
					w_flag = 1;
				}
				else {
					
					for(j=0;j<len;j++){
						rvb[pos++] = ITab.CTab[j];
						rvb[pos++] = ITab.CTab[j]>>8;
					}
				}
				break;
			case 3://温度参数
				if(rvb[PCMD] == 0xd1){
					for(j=0;j<TempTabParaCount;j++){				
							vv = TempITabParaAddrD[j];
						rvb[pos++] = vv&0xff;
						rvb[pos++] = vv>>8;
					}
				}
				break;
			case 4://频率参数
				if(rvb[PCMD] == 0xd1){
					for(j=0;j<FreqTabParaCount;j++){
								vv = FreqITabParaAddrD[j];	
						rvb[pos++] = vv&0xff;
						rvb[pos++] = vv>>8;
					}
				}
				break;
			case 6:
				len = (rvb[pos++]-1)*TempTabParaCount;  
				if(rvb[PCMD] == 0xd2){
					for(j=0;j<TempTabParaCount;j++){
						TempTabBuf[len+j] = rvb[pos++];
					}
					if(rvb[PDUA+2] == MAX_TEMPTABLEN) {
					w_flag = 1;
					cmp_flag = 1;
					}
					
				}
				else{
					for(j=0;j<TempTabParaCount;j++){
						rvb[pos++] = TempTabBuf[len+j];
					}
				}
				break;
			case 7:
				len = (rvb[pos++]-1)*FreqTabParaCount;
				if(rvb[PCMD] == 0xd2){
					for(j=0;j<FreqTabParaCount;j++){
						FreqTabBuf[len+j] = rvb[pos++];
					}
					if(rvb[PDUA+2]==MAX_FREQTABLEN) w_flag = 1;
				}
				else{
					for(j=0;j<FreqTabParaCount;j++){
						rvb[pos++] = FreqTabBuf[len+j];
					}
				}
				break;

			case 10:  
				if(rvb[PCMD] == 0xd2)
				{										
					Calibration_id = rvb[pos+0] + (rvb[pos+1]<<8);     
					Calibration_Channel = rvb[pos+2];										
       if(Calibration_id == 0xf353)
       {   
					amplifier.value1[Calibration_Channel-1+2] = rvb[pos+3];
					pos = pos + 4;
					CalibrationWrite[Calibration_Channel-1+2] = 1;
			 }
			 else if(Calibration_id == 0xf352)
       {   
					amplifier.value1[Calibration_Channel-1] = rvb[pos+3];
					pos = pos + 4;
					CalibrationWrite[Calibration_Channel-1] = 1;
			 }
			 
	// else if(Calibration_id == 0xf357)
     //  {   
	//				amplifier_pi.value1[Calibration_Channel-1] = rvb[pos+3];
	//				pos = pos + 4;
	//				CalibrationWrite_pi = 1;
	//		 }
			 			 
					w_flag = 1;
					delayx(5);
				}
				break;
				
			case 11:  
				if(rvb[PCMD] == 0xd2)
				{										
					Calibration_id = rvb[pos+0] + (rvb[pos+1]<<8);
					Calibration_Channel = rvb[pos+2];														
       if(Calibration_id == 0xf353)
       {   
					amplifier.value2[Calibration_Channel-1+2] = rvb[pos+3];
					pos = pos + 4;
					CalibrationWrite[Calibration_Channel-1+2] = 2;
			 }
			 else if(Calibration_id == 0xf352)
       {   
					amplifier.value2[Calibration_Channel-1] = rvb[pos+3];
					pos = pos + 4;
					CalibrationWrite[Calibration_Channel-1] = 2;
			 }
			 //else if(Calibration_id == 0xf357)
     //  {   
			//		amplifier_pi.value2[Calibration_Channel-1] = rvb[pos+3];
		//			pos = pos + 4;
	//				CalibrationWrite_pi[Calibration_Channel-1] = 2;
		//	 }		
					w_flag = 1;
					delayx(5);
				}
				break;
								
			case 12:  
				if(rvb[PCMD] == 0xd1)
				{										
					Calibration_id = rvb[pos+0] + (rvb[pos+1]<<8);
					Calibration_Channel = rvb[pos+2];	
					pos = pos +3;
					if(Calibration_id == 0xf353)
					{ 									
					  rvb[pos++] = amplifier.value1[Calibration_Channel-1+2];
					  rvb[pos++] = amplifier.voltage1[Calibration_Channel-1+2]&0xff;
					  rvb[pos++] = amplifier.voltage1[Calibration_Channel-1+2]>>8;
					  rvb[pos++] = amplifier.value2[Calibration_Channel-1+2];
					  rvb[pos++] = amplifier.voltage2[Calibration_Channel-1+2]&0xff;
					  rvb[pos++] = amplifier.voltage2[Calibration_Channel-1+2]>>8;
          }
					else if(Calibration_id == 0xf352)
					{ 									
					  rvb[pos++] = amplifier.value1[Calibration_Channel-1];
					  rvb[pos++] = amplifier.voltage1[Calibration_Channel-1]&0xff;
					  rvb[pos++] = amplifier.voltage1[Calibration_Channel-1]>>8;
					  rvb[pos++] = amplifier.value2[Calibration_Channel-1];
					  rvb[pos++] = amplifier.voltage2[Calibration_Channel-1]&0xff;
					  rvb[pos++] = amplifier.voltage2[Calibration_Channel-1]>>8;
          }
					//else if(Calibration_id == 0xf357)
					//{ 									
					//  rvb[pos++] = amplifier_pi.value1[Calibration_Channel-1];
					//  rvb[pos++] = amplifier_pi.voltage1[Calibration_Channel-1]&0xff;
					//  rvb[pos++] = amplifier_pi.voltage1[Calibration_Channel-1]>>8;
					//  rvb[pos++] = amplifier_pi.value2[Calibration_Channel-1];
					//  rvb[pos++] = amplifier_pi.voltage2[Calibration_Channel-1]&0xff;
					//  rvb[pos++] = amplifier_pi.voltage2[Calibration_Channel-1]>>8;
          //}                    
          delayx(5);				
				}
				break;

				case 0x0d:
					rvb[PANS] = 0x00;
					rbw = 12;
					s_command(rvb[PCMD],mm,0);
					asm("cli");
					(*((void(*)(void))(2*0x3800UL)))();
				break;
									
			default:
				//i++;
				return;
		}
		rvb[PLEN] = pos - 7;
		rbw = pos + 3;
	}
	s_command(rvb[PCMD],mm,0);	
	delayx(5);
	//BiasCompensate_Proc();
	if(w_flag == 1){
	

    	if(mm <= 2){
    		pstr = (uchar *) &ITab;
    		j = 54;
    		vv = EE_ADDR_ITTAB;
    	}
    	else if (mm == 6){
    		pstr = (uchar *) &TempTabBuf;
    		j = 200;
    		vv = EE_ADDR_TEMPTAB;
    	}
    	else if (mm == 7){
    		pstr = (uchar *) &FreqTabBuf;
    		j = 2;
    		vv = EE_ADDR_FREQTAB;
    	}
    	else if((mm == 10)||(mm == 11))
    	{
    		if(Calibration_id == 0xf353)
				{
    		  pstr = (uchar *) &amplifier;
    		  j = sizeof(amplifier);    		
    		  vv = EE_ADDR_CALIBRATION;
    		} 
    		else if(Calibration_id == 0xf352)
				{
    		  //pstr = (uchar *) &amplifier_ro;
    		  pstr = (uchar *) &amplifier;
    		  j = sizeof(amplifier);    		
    		  vv = EE_ADDR_CALIBRATION;
    		} 
    		//else if(Calibration_id == 0xf357)
	//			{
    		//  pstr = (uchar *) &amplifier_pi;
    		//  j = sizeof(amplifier_pi);    		
    		//  vv = EE_ADDR_CALIBRATION_PI;
    		//}     		    		 
    	}	   	
   	
    	if(mm <= 2){
    		for (i = 0;i < MAX_TEMPTABLEN; i++) sdb[i] = ITab.TTab[i];//EEBuf
    		for (i = 0;i < 4; i++) {
    			sdb[(i<<1)+MAX_TEMPTABLEN] = ITab.CTab[i];//EEBuf
    			sdb[(i<<1)+MAX_TEMPTABLEN+1] = ITab.CTab[i]>>8;//EEBuf
    		}
    	}
    	else
			for (i = 0;i < j; i++) sdb[i] = pstr[i];//EEBuf
			
		UPDATE_EEPROM(vv, j);
		delayx(5);
	}
}

