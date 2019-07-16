/************************************************************
 * Copyright (c) 2004-2005, POWERCOM Co., All Rights Reserved.
 * File        :"msg.c"
 * Description :

 * Author      : Xu Hailun
 * Date        : 2005-02-25
 * Version     : 1.0
 * Revisions   :
 ************************************************************/
#include "def.h"
#include "all.h"
#include "paralist.h"

#define PTYPE		1
#define PVER		2
#define PCODE		4
#define PCMD		5
#define PANS		6
//#define PLEN  	7
#define PDUA  		7

#define MP_TYPE				1
#define MP_VERSION  		2
#define BROADMSG_ID	  		0xff
#define MP_channel  		2
#define MP_ro_channel  		2
#define MP_pi_channel  		1

extern uchar rx_state,rev_count;

extern uint crc_fun(uchar *pbuf, uchar numc);

extern void UPDATE_EEPROM(uint START_ADDR, uchar NUM);

extern void PLL_Init();
extern void Enable_RS485(uchar op);
extern void delayx(uint x);
extern void PA_PowerOff();
extern void PA_PowerOn();
extern void Set_Up_W_Attenuation();
extern void Set_Up_Dcs_Attenuation();
extern void Set_DW_W_Attenuation();
extern void Set_DW_Dcs_Attenuation();
extern void PA_Dcs_PowerOn();
extern void PA_W_PowerOn();
extern void PA_Dcs_PowerOff();
extern void PA_W_PowerOff();

extern void IT_Msg_Proc();
extern void BiasCompensate_Proc();

void Send_Msg_Loop()
{

	Enable_RS485(1);
	delayx(2);
	asm("wdr");
	while(sbr!=sbw){
		if(chkbit(UCSRA,UDRE)){
        	UDR = sdb[sbr++];
			if(sbr>=MAX_SDBLEN) sbr = 0;
		}
	}
	delayx(5);
	Enable_RS485(0);
}


/*————————————————————————————————————————————————————————————————
  头，版本，模块ID，消息ID，应答标志，参数地址，长度，数据n,校验和2， 结束标志
  -1  0    1      2      3       4,5     6     n    n+8,n+9  n+10
  ————————————————————————————————————————————————————————————————*/
void s_command(uchar b0,uint i0,uchar b1)
{/* agument:msgid, para addr,operate */
	uint vv;

    uchar j;
	//if (rvb[PCODE] == BROADMSG_ID) return;
	if ((rvb[PCODE] == BROADMSG_ID)&&((rvb[PCMD] != 0x50)&&(rvb[PCMD] != 0x40)))   return;
	switch(b0){
		case 0x40:
		case 0x50:
		case 0x60:
		case 0xc1://接收正确确认消息
		   	rvb[PANS] = b1;
		   	break;		
		case 0xc2://查询响应消息
		    rvb[PANS] = b1;
		   	break;  
	    	case 0xd1://接收正确确认消息
		   	rvb[PANS] = b1;
		   	break;		
		case 0xd2://查询响应消息
		    rvb[PANS] = b1;
		   	break;  
        default:
			rvb[PANS] = b1;
			break;  	
	}
	vv = crc_fun(&rvb[1], rbw-4);
	rvb[rbw-3] = vv & 0xff;
	rvb[rbw-2] = vv  >> 8;
	
	//sdb[sbw++] = SYNCB;	//起始标志
	if(sbw >= MAX_SDBLEN) sbw = 0;
	sbr = sbw = 0;
	sdb[sbw++] = SYNCB;
	for(j = 1; j < rbw-1; j++){ // 拆分
		if (rvb[j] == SYNCB){
			sdb[sbw++] = 0x5e;
			sdb[sbw++] = 0x4d;
		}
		else if (rvb[j] == 0x5e){
			sdb[sbw++] = 0x5e;
			sdb[sbw++] = 0x5d;
		}
		else
			sdb[sbw++] = rvb[j];//divide_2_table[(rvb[j]&0xf0)>>4];
		if(sbw >= MAX_SDBLEN) sbw = 0;
		
	}
	sdb[sbw++] = SYNCB;	//起始标志
	if(sbw >= MAX_SDBLEN) sbw = 0;
	Send_Msg_Loop();
}

uchar get_msg()
{
	uint ll, kk;
	if(rx_state > 1){
		if(rvb[0] != SYNCB) goto get_msg_Fail;//判包头
		if(rvb[PTYPE] != MP_TYPE) goto get_msg_Fail;//判协议类型
		if(rvb[PANS] != 0xff) goto get_msg_Fail;
		if(rvb[PCMD] == 0x40) goto get_msg_Ok;
		if((rvb[PVER]&0x0f) != MP_VERSION) goto get_msg_Fail;//判模块协议版本
		//cc = rvb[PLEN];
		if(rbw < 11) goto get_msg_Fail;//判包长
		if(rvb[rbw-1] != SYNCB) goto get_msg_Fail;//判包尾
		//if((rvb[PCODE] != BROADMSG_ID)&&(rvb[PCODE] != DN_MODULE_ADDRE))
		//if((rvb[PCODE] != BROADMSG_ID)&&(rvb[PCODE] != own.addr))
		 if((rvb[PCODE] != BROADMSG_ID)&&(rvb[PCODE] !=  own.addr))
		{
			goto get_msg_Fail;
		}
get_msg_Ok:
		kk = (rvb[rbw-2]<<8) + rvb[rbw-3];
		ll = crc_fun(&rvb[1],rbw-4);
		if(kk == ll){// 检验和判决
			return 1;
		}
get_msg_Fail:
  		rx_state = rbw = 0;
	}
	return 0;
}

void OWN_Msg_Proc()
{     
	uchar i,j,len;
	uint mm;
	uchar *pstr;
	   if(rvb[PCODE] != own.addr)
	   {
	     if(rvb[PCODE] != BROADMSG_ID)
	        {
	            return; 
	        }
	        else
	        {
	           if(rvb[PCMD] != 0x40)
	           {
	             return;  
	            }
	        }
	     }  
	  
	i = 0;
	while(i < (rbw-3-PDUA))//rvb[PLEN]
	{
		mm = (rvb[PDUA+1+i]<<8) + rvb[PDUA+i];
		len = rvb[PDUA+2+i];
		switch(mm){
			case 0xf000:
				if (len>=10) len = 10;
				if(rvb[PCMD] == 0x60){
					for(j=0;j<len;j++)
				  		own.mname[j] = rvb[PDUA+3+j+i];
				}
				else {
					for(j=0;j<len;j++)
						rvb[PDUA+3+j+i] = own.mname[j];  
				}
				break;
			case 0xf001:     //模块ID
				if (len>=30) len = 30;
				if(rvb[PCMD] == 0x60){
					for(j=0;j<len;j++)
				  		own.id[j] = rvb[PDUA+3+j+i];
				}
				else {
					
                                           for(j=0;j<len;j++)
					rvb[PDUA+3+j+i] = own.id[j];
				}
				break;
			case 0xf002:    //模块地址
				if(rvb[PCMD] == 0x60)
				{
//					if (len != 1)
//					{
//						reply = 0x07;
//						break;
//					  }     
					own.addr = rvb[PDUA+3+i];//为防止别人修改模块ID,固定死，不让改
				}
				else if (rvb[PCMD] == 0x40)
				{
//					if (len != 1)
//					 { 
//					 	reply = 0x08;
//					 	break;
//					}
						rvb[PDUA+3+i] = own.addr;//MODULE_ADDRE;
				}	
//				else 
//				{
//					reply = 1;
//				}	
					//rvb[PDUA+3+i] = own.addr;
				break;
			case 0xf003:     //协议版本
				if(rvb[PCMD] == 0x60){
					//own.pver = rvb[PDUA+3+i];
				}
				else rvb[PDUA+3+i] = MP_VERSION; 
				break;
			case 0xf017:   //硬件协议版本号
				if (len>=10) len = 10;
				if(rvb[PCMD] == 0x60){
					for(j=0;j<len;j++)
						own.hver[j] = rvb[PDUA+3+i+j];
				}
				else {
					for(j=0;j<len;j++)
						rvb[PDUA+3+i+j] = own.hver[j];
				}
				break;
			case 0xf018:  //软件协议版本号
				if(rvb[PCMD] == 0x60){
					own.sver[0] = rvb[PDUA+3+i];
					own.sver[1] = rvb[PDUA+4+i];
				}
				else {
					rvb[PDUA+3+i] = own.sver[0];
					rvb[PDUA+4+i] = own.sver[1];
				}
				break;
			case 0xf019:  //系列号
				if (len>=20) len = 20;
				if(rvb[PCMD] == 0x60){
					for (j=0;j<len;j++)
						own.serial[j] = rvb[PDUA+3+i+j];
				}
				else {
					for (j=0;j<len;j++)
						rvb[PDUA+3+i+j] = own.serial[j];
				}
				break;

			case 0xf036:case 0xf030:case 0xf032:case 0xf037:
			case 0xf031:case 0xf033:case 0xf035:case 0xf034:
				j = rvb[PDUA+i] - 0x30;//(mm&0x0030) - 0x30;
				if(rvb[PCMD] == 0x60)
				{
				   own.OffsetAdc[j] = rvb[PDUA+3+i]+(rvb[PDUA+4+i]<<8);
				}
				else
				{ 
				     rvb[PDUA+3+i] = own.OffsetAdc[j]&0xff;
				     rvb[PDUA+4+i] = own.OffsetAdc[j]>>8; 
				}
				break;    
			case 0xf040://过流门限值
				if(rvb[PCMD] == 0x60){
					own.thrMaxCurrent = rvb[PDUA+3+i]+(rvb[PDUA+4+i]<<8);
				}
				else {
					rvb[PDUA+3+i] = own.thrMaxCurrent&0xff;
					rvb[PDUA+4+i] = own.thrMaxCurrent>>8;
				}
				break;
			case 0xf350://欠流门限值
				if(rvb[PCMD] == 0x60){
					own.thrMinCurrent = rvb[PDUA+3+i]+(rvb[PDUA+4+i]<<8);
				}
				else {
					rvb[PDUA+3+i] = own.thrMinCurrent&0xff;
					rvb[PDUA+4+i] = own.thrMinCurrent>>8;
				}
				break;
			
			case 0xf353:     
				if(rvb[PCMD] == 0x60){
					//own.pchannel= rvb[PDUA+3+i];
				}
				else rvb[PDUA+3+i] = MP_channel; 
				break;	
			case 0xf352:     
				if(rvb[PCMD] == 0x60){
					//own.pchannel= rvb[PDUA+3+i];
				}
				else rvb[PDUA+3+i] = MP_ro_channel; 
				break;
/*******************					
			case 0xf357:     
				if(rvb[PCMD] == 0x60){
					//own.pchannel= rvb[PDUA+3+i];
				}
				else rvb[PDUA+3+i] = MP_pi_channel; 
				break;
*******************/
				default:
				break;		
		}
		i += rvb[PDUA+2+i]+3;
	}
	s_command(rvb[PCMD],mm,0);	
	if(rvb[PCMD] == 0x60){
		pstr = (uchar *) &own;
		for (i = 0;i < sizeof(own); i++) sdb[i] = pstr[i];//EEbuf
		UPDATE_EEPROM(EE_ADDR_OWN, sizeof(own));
	
	  pstr = (uchar *)&my;
		for(i = 0;i<40;i++)	sdb[i] = pstr[i];//EEBuf
		//EEBuf[3] = 0;//解决监控设置衰减后设置模块参数时保存监控设置的值BUG;
		UPDATE_EEPROM(EE_ADDR_MY, 60);
	}
}

void PA_MSG_Proc()
{
	uint mm;
	ulong lt;
	slong ii,itest;
		uchar uchartemp;
	//schar schartemp;
	uchar *pstr;
	uchar i,ans = 0,len,j;
	//ll = rvb[4]/4;
	   if(rvb[PCODE] != own.addr)
	   {
	            return; 
	    }
	i = 0;
	if(rvb[PCMD] == 0xc1)
	cmp_flag = 1;  
    while(i < (rbw-3-PDUA))
	{
		mm = (rvb[PDUA+1+i]<<8) + rvb[PDUA+i];
		len = rvb[PDUA+2+i]; 		
		switch(mm){
	                 case 0x0000://dcs-up
					if(rvb[PCMD] == 0xc1){
					if(len !=1) {
                                                     ans = 0x07;
						break;
					}
				    	my.upsw0 = rvb[PDUA+3+i];
				    	if(my.upsw0 >= 1) my.upsw0 = 1;
				    	if(my.upsw0 == 0)    PORTD  &= 0xF7; //PA_PowerOn();	
				    	else                            PORTD  |= 0x08; // PA_PowerOff();      
				    }
				    else{
						if(len !=1) {
                                                     ans = 0x08;
						break;
					}
				    	rvb[PDUA+3+i] = my.upsw0;
					}
    	  
    	    	                   break;
			    case 0x0008://dcs-dn
					if(rvb[PCMD] == 0xc1){
						if(len !=1) {
                                                     ans = 0x07;
						break;
					}
				    	my.sw0 = rvb[PDUA+3+i];
				    	if(my.sw0 >= 1) my.sw0 = 1;
				    	if(my.sw0 == 0)PORTD  &= 0xFB; 
				    	else                     PORTD  |= 0x04;     
				        // BiasCompensate_Proc();
				    }
				    else{
						if(len !=1) {
                                                     ans = 0x08;
						break;
					}
				    	rvb[PDUA+3+i] = my.sw0;
					}
    	  
    	    	break;
		           //case 0x0001://w-up
		           case 0x0001:
					if(rvb[PCMD] == 0xc1){
						if(len !=1) {
                                                     ans = 0x07;
						break;
					}
				    	my.upsw1 = rvb[PDUA+3+i];
				    	if(my.upsw1 >= 1) my.upsw1 = 1;
				    	if(my.upsw1 == 0)   PORTC &=0xFB;
				    	else                           PORTC |=0x04;	
				    }
				    else{
						if(len !=1) {
                                                     ans = 0x08;
						break;
					}
				    	rvb[PDUA+3+i] = my.upsw1;
					}
    	  
    	    	                     break;
				//case 0x0009://w-dn
				case 0x0009:
					if(rvb[PCMD] == 0xc1){
						if(len !=1) {
                                                     ans = 0x07;
						break;
					}
				    	my.sw1 = rvb[PDUA+3+i];
				    	if(my.sw1 >= 1) my.sw1 = 1;
				    	if(my.sw1 == 0)  PORTC &=0xF7;
				    	else                        PORTC |=0x08;

					//BiasCompensate_Proc();
				    }
				    else{
						if(len !=1) {
                                                     ans = 0x08;
						break;
					}
				    	rvb[PDUA+3+i] = my.sw1;
					}
    	  
    	    	                     break;
    	    	                         	    	                     case 0x060C://LTe1-dn
					if(rvb[PCMD] == 0xc1){
						if(len !=1) {
                                                     ans = 0x07;
						     break;
					           }
				    	        my.sw2 = rvb[PDUA+3+i];
				    	   if(my.sw2 == 0) //自动都关或者高阻待定
				    	     { 
				    	       				    	       PORTC &=0xF7;//无上拉
				    	       PORTC &=0xFB;//无上拉
				    	       DDRC &=0xF7;//设置成输入  
				    	       DDRC &=0xFB;//设置成输入  
				             }
				           else if((my.sw2 == 1)||(my.sw2 == 4))//dn,郭坚增加驻波测试模式，下行开同时关闭栅压
				           {
				               
				             DDRC |=0x08;
				             DDRC |=0x04;
				    	       PORTC &=0xFB;
				    	        delayx(10);
				    	       PORTC |=0x08;
				           }
				           else if(my.sw2 == 2)//up
				           {
				           	DDRC |=0x08;
				             DDRC |=0x04;
				           		PORTC &=0xF7;
				           		 delayx(10);
				              PORTC |=0x04;
				           }
				           else//全关
				           {
				            
				             PORTC &=0xF7;//无上拉
				    	       PORTC &=0xFB;//无上拉
				    	       DDRC &=0xF7;//设置成输入  
				    	       DDRC &=0xFB;//设置成输入  
				           } 
				    }
				    else{
						if(len !=1) {
                                                     ans = 0x08;
						break;
					}
				    	rvb[PDUA+3+i] = my.sw2;
					}
    	    	                     break;
		

                            case 0x0010:
				if(rvb[PCMD]==0xc1){
                                         if(len != 1){
                                               ans = 0x07;
				              break;
					 }
				    my.maxpo1_ctr = rvb[PDUA+3+i];// + (rvb[PDUA+4+i] << 8);
				  if(my.maxpo1_ctr>60) {
							my.maxpo1_ctr= 60;
							ans = 7;
				    }
				     ii= (slong)my.maxpo1_ctr;
				     itest =(slong)amplifier.power_b[0];
				     ii =1000*ii-itest*1000;
				     itest =(slong)amplifier.power_k[0];
				     ii = ii/itest;
//				     ii =1000*ii/amplifier.power_k[0] -amplifier.power_b[0]*1000/amplifier.power_k[0];
//			              po =   (ii*3-2405)/149;
//			              ii= (ii*149 + 2405)/3;
				     my.voltage[1] =(uint) ii;
				    //BiasCompensate_Proc();
				}
				else{
                                                       if(len !=1)
							{
							ans =0x08;
							break;	
							}				  	
				   rvb[PDUA+3+i] = my.maxpo1_ctr & 0xff;
				   //rvb[PDUA+4+i] = my.maxpo1_ctr_3g >> 8;
				}
				
                            break;
			//case 0x0019:
			case 0x0011:
				if(rvb[PCMD]==0xc1){
                                         if(len != 1){
                                               ans = 0x07;
				              break;
					 }
				    my.maxpo2_ctr = rvb[PDUA+3+i];// + (rvb[PDUA+4+i] << 8);
				  if(my.maxpo2_ctr>60) {
							my.maxpo2_ctr = 60;
							ans = 7;
				    }
				     				    				     ii= (slong)my.maxpo2_ctr;
				     itest =(slong)amplifier.power_b[1];
				     ii =1000*ii-itest*1000;
				     itest =(slong)amplifier.power_k[1];
				     ii = ii/itest;
				     my.voltage[2] =(uint) ii;
				    //BiasCompensate_Proc();
				}
				else{
                                                       if(len !=1)
							{
							ans =0x08;
							break;	
							}				  	
				   rvb[PDUA+3+i] = my.maxpo2_ctr & 0xff;
				   //rvb[PDUA+4+i] = my.maxpo2_ctr_3g >> 8;
				}
				
                            break;
                          case 0x0018:
			//case 0x001a:
				if(rvb[PCMD]==0xc1){
                                         if(len != 1){
                                               ans = 0x07;
				              break;
					 }
				    my.maxpo3_ctr= rvb[PDUA+3+i];// + (rvb[PDUA+4+i] << 8);
				  if(my.maxpo3_ctr>60) {
							my.maxpo3_ctr = 60;
							ans = 7;
				    }
				    				     ii= (slong)my.maxpo3_ctr;
				     itest =(slong)amplifier.power_b[2];
				     ii =1000*ii-itest*1000;
				     itest =(slong)amplifier.power_k[2];
				     ii = ii/itest;
				     my.voltage[0] =(uint) ii;
				    //BiasCompensate_Proc();
				}
				else{
                                                       if(len !=1)
							{
							ans =0x08;
							break;	
							}				  	
				   rvb[PDUA+3+i] = my.maxpo3_ctr & 0xff;
				   //rvb[PDUA+4+i] = my.maxpo3_ctr>> 8;
				}
				
                            break;
						                          case 0x0019:
			//case 0x001a:
				if(rvb[PCMD]==0xc1){
                                         if(len != 1){
                                               ans = 0x07;
				              break;
					 }
				    my.maxpo4_ctr= rvb[PDUA+3+i];// + (rvb[PDUA+4+i] << 8);
				  if(my.maxpo4_ctr>60) {
							my.maxpo4_ctr = 60;
							ans = 7;
				    }
				    				     ii= (slong)my.maxpo4_ctr;
				     itest =(slong)amplifier.power_b[3];
				     ii =1000*ii-itest*1000;
				     itest =(slong)amplifier.power_k[3];
				     ii = ii/itest;
//				     ii= (slong)my.maxpo4_ctr;
//				     ii =1000*ii/amplifier.power_k[3] -amplifier.power_b[3]*1000/amplifier.power_k[3];
//			              //ii= (ii*149 + 2405)/3;
				     my.voltage[3] =(uint) ii;
				    //BiasCompensate_Proc();
				}
				else{
                                                       if(len !=1)
							{
							ans =0x08;
							break;	
							}				  	
				   rvb[PDUA+3+i] = my.maxpo4_ctr & 0xff;
				   //rvb[PDUA+4+i] = my.maxpo3_ctr>> 8;
				}
				
                            break;	
			case 0x00d0:
				if(rvb[PCMD] == 0xc1){
                                        if(len != 1){
                                                   ans = 0x07;
                                                   break;												   
					}
					my.po_maxref1 =  rvb[PDUA+3+i] ;
					if(my.po_maxref1 >= 60){
                                                     my.po_maxref1 = 60;
						ans = 0x07;
						break;
					}  						
				}
				else{
                                             if(len != 1){
                                                   ans = 0x08;
				               break;
					 }
					rvb[PDUA+3+i] = my.po_maxref1;						 
				}
                           break;
			case 0x00d1:
				if(rvb[PCMD] == 0xc1){
                                        if(len != 1){
                                                   ans = 0x07;
                                                   break;												   
					}
					my.po_maxref2 =  rvb[PDUA+3+i] ;
					if(my.po_maxref2 >= 60){
                                                     my.po_maxref2 = 60;
						ans = 0x07;
						break;
					}  						
				}
				else{
                                             if(len != 1){
                                                   ans = 0x08;
				               break;
					 }
					rvb[PDUA+3+i] = my.po_maxref2;						 
				}
                           break;
			case 0x00d8:
				if(rvb[PCMD] == 0xc1){
                                        if(len != 1){
                                                   ans = 0x07;
                                                   break;												   
					}
					my.po_maxref3 =  rvb[PDUA+3+i] ;
					if(my.po_maxref3 >= 60){
                                                     my.po_maxref3 = 60;
						ans = 0x07;
						break;
					}  						
				}
				else{
                                             if(len != 1){
                                                   ans = 0x08;
				               break;
					 }
					rvb[PDUA+3+i] = my.po_maxref3;						 
				}
                           break;
						case 0x00d9:
				if(rvb[PCMD] == 0xc1){
                                        if(len != 1){
                                                   ans = 0x07;
                                                   break;												   
					}
					my.po_maxref4 =  rvb[PDUA+3+i] ;
					if(my.po_maxref4 >= 60){
                                                     my.po_maxref4 = 60;
						ans = 0x07;
						break;
					}  						
				}
				else{
                                             if(len != 1){
                                                   ans = 0x08;
				               break;
					 }
					rvb[PDUA+3+i] = my.po_maxref4;						 
				}
                           break;		
                            case 0x7002://dcs-up
					if(rvb[PCMD] == 0xc1){
					if(len !=1) {
                                                     ans = 0x07;
						break;
					}
				    	my.rfswmode = rvb[PDUA+3+i];
				    	if(my.rfswmode > 1) my.rfswmode = 0;
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
      			
PORTD &= 0xfB;
PORTD &= 0xf7;
PORTC &=0xF7;
				    	       PORTC &=0xFB;
      				}
    	}      
				    }
				    else{
						if(len !=1) {
                                                     ans = 0x08;
						break;
					}
				    	rvb[PDUA+3+i] = my.rfswmode;
					}
    	  
    	    	                   break;
    	    	                    case 0x7003://dcs-up
					if(rvb[PCMD] == 0xc1){
					if(len !=1) {
                                                     ans = 0x07;
						break;
					}
				    	my.rfswchan = rvb[PDUA+3+i];
				    	if(my.rfswchan > 2) my.rfswchan = 1;
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
      			
PORTD &= 0xfB;
PORTD &= 0xf7;
PORTC &=0xF7;
				    	       PORTC &=0xFB;
      				}   
				    }
				  }
				    else{
						if(len !=1) {
                                                     ans = 0x08;
						break;
					}
				    	rvb[PDUA+3+i] = my.rfswchan;
					}
    	 
    	    	                   break;
                           
                           			 case 0x0020://gsm上行数控衰减1
			 	if(rvb[PCMD] == 0xc1)
			 	   {			
	 	   	      if (len != 1)
	 	  	      {
	 	  		      ans = 0x07;
	 	  		       break;
	 	  	      }
			  	    uchartemp = rvb[PDUA+3+i];
			  	    if (uchartemp >= 32)	
			  	    {
			  	    	ans =0x07;
			  	    	break;
			  	    }			     			     	   
		     	    my.lna_atten[0] = rvb[PDUA+3+i];


		     	     Set_Up_W_Attenuation();       
				    }
				    else if (rvb[PCMD] == 0xc2)
				    {
				    	if (len != 1)
			 	  	      {
			 	  		      ans = 0x08;
			 	  		       break;
			 	  	      } 
				    	rvb[PDUA+3+i] = my.lna_atten[0];
					 }
					 else 
					     ans = 0x01;
			 	
			 	break;

    case 0x0021://下行数控衰减2
			 	if(rvb[PCMD] == 0xc1)
			 	   {			
	 	   	      if (len != 1)
	 	  	      {
	 	  		      ans = 0x07;
	 	  		       break;
	 	  	      }
			  	    uchartemp = rvb[PDUA+3+i];
			  	    if (uchartemp >= 32)	
			  	    {
			  	    	ans =0x07;
			  	    	break;
			  	    }			     			     	   
		     	    my.lna_atten[1] = rvb[PDUA+3+i];


		     	     Set_DW_Dcs_Attenuation();      
				    }
				    else if (rvb[PCMD] == 0xc2)
				    {
				    	if (len != 1)
			 	  	      {
			 	  		      ans = 0x08;
			 	  		       break;
			 	  	      } 
				    	rvb[PDUA+3+i] = my.lna_atten[1];
					 }
					 else 
					     ans = 0x01;
			 	
			 	break;		
			 				 case 0x0028://下行数控衰减1
			 	if(rvb[PCMD] == 0xc1)
			 	   {			
	 	   	      if (len != 1)
	 	  	      {
	 	  		      ans = 0x07;
	 	  		       break;
	 	  	      }
			  	    uchartemp = rvb[PDUA+3+i];
			  	    if (uchartemp >= 32)	
			  	    {
			  	    	ans =0x07;
			  	    	break;
			  	    }			     			     	   
		     	    my.lna_atten[2] = rvb[PDUA+3+i];


		     	     Set_Up_W_Attenuation();       
				    }
				    else if (rvb[PCMD] == 0xc2)
				    {
				    	if (len != 1)
			 	  	      {
			 	  		      ans = 0x08;
			 	  		       break;
			 	  	      } 
				    	rvb[PDUA+3+i] = my.lna_atten[2];
					 }
					 else 
					     ans = 0x01;
			 	
			 	break;

    case 0x0029://下行数控衰减2
			 	if(rvb[PCMD] == 0xc1)
			 	   {			
	 	   	      if (len != 1)
	 	  	      {
	 	  		      ans = 0x07;
	 	  		       break;
	 	  	      }
			  	    uchartemp = rvb[PDUA+3+i];
			  	    if (uchartemp >= 32)	
			  	    {
			  	    	ans =0x07;
			  	    	break;
			  	    }			     			     	   
		     	    my.lna_atten[3] = rvb[PDUA+3+i];


		     	     Set_DW_Dcs_Attenuation();      
				    }
				    else if (rvb[PCMD] == 0xc2)
				    {
				    	if (len != 1)
			 	  	      {
			 	  		      ans = 0x08;
			 	  		       break;
			 	  	      } 
				    	rvb[PDUA+3+i] = my.lna_atten[3];
					 }
					 else 
					     ans = 0x01;
			 	
			 	break;
			 				 case 0x0024://下行数控衰减1
			 	if(rvb[PCMD] == 0xc1)
			 	   {			
	 	   	      if (len != 1)
	 	  	      {
	 	  		      ans = 0x07;
	 	  		       break;
	 	  	      }
			  	    uchartemp = rvb[PDUA+3+i];
			  	    if (uchartemp >= 32)	
			  	    {
			  	    	ans =0x07;
			  	    	break;
			  	    }			     			     	   
		     	    my.dp_atten[0] = rvb[PDUA+3+i];


		     	     Set_DW_W_Attenuation();       
				    }
				    else if (rvb[PCMD] == 0xc2)
				    {
				    	if (len != 1)
			 	  	      {
			 	  		      ans = 0x08;
			 	  		       break;
			 	  	      } 
				    	rvb[PDUA+3+i] = my.dp_atten[0];
					 }
					 else 
					     ans = 0x01;
			 	
			 	break;

    case 0x0025://下行数控衰减2
			 	if(rvb[PCMD] == 0xc1)
			 	   {			
	 	   	      if (len != 1)
	 	  	      {
	 	  		      ans = 0x07;
	 	  		       break;
	 	  	      }
			  	    uchartemp = rvb[PDUA+3+i];
			  	    if (uchartemp >= 32)	
			  	    {
			  	    	ans =0x07;
			  	    	break;
			  	    }			     			     	   
		     	    my.dp_atten[1] = rvb[PDUA+3+i];


		     	     Set_DW_Dcs_Attenuation();      
				    }
				    else if (rvb[PCMD] == 0xc2)
				    {
				    	if (len != 1)
			 	  	      {
			 	  		      ans = 0x08;
			 	  		       break;
			 	  	      } 
				    	rvb[PDUA+3+i] = my.dp_atten[1];
					 }
					 else 
					     ans = 0x01;
			 	
			 	break;
			 				 case 0x002c://下行数控衰减1
			 	if(rvb[PCMD] == 0xc1)
			 	   {			
	 	   	      if (len != 1)
	 	  	      {
	 	  		      ans = 0x07;
	 	  		       break;
	 	  	      }
			  	    uchartemp = rvb[PDUA+3+i];
			  	    if (uchartemp >= 32)	
			  	    {
			  	    	ans =0x07;
			  	    	break;
			  	    }			     			     	   
		     	    my.dp_atten[2] = rvb[PDUA+3+i];


		     	     Set_DW_W_Attenuation();       
				    }
				    else if (rvb[PCMD] == 0xc2)
				    {
				    	if (len != 1)
			 	  	      {
			 	  		      ans = 0x08;
			 	  		       break;
			 	  	      } 
				    	rvb[PDUA+3+i] = my.dp_atten[2];
					 }
					 else 
					     ans = 0x01;
			 	
			 	break;

    case 0x002d://下行数控衰减2
			 	if(rvb[PCMD] == 0xc1)
			 	   {			
	 	   	      if (len != 1)
	 	  	      {
	 	  		      ans = 0x07;
	 	  		       break;
	 	  	      }
			  	    uchartemp = rvb[PDUA+3+i];
			  	    if (uchartemp >= 32)	
			  	    {
			  	    	ans =0x07;
			  	    	break;
			  	    }			     			     	   
		     	    my.dp_atten[3] = rvb[PDUA+3+i];


		     	     Set_DW_Dcs_Attenuation();      
				    }
				    else if (rvb[PCMD] == 0xc2)
				    {
				    	if (len != 1)
			 	  	      {
			 	  		      ans = 0x08;
			 	  		       break;
			 	  	      } 
				    	rvb[PDUA+3+i] = my.dp_atten[3];
					 }
					 else 
					     ans = 0x01;
			 	
			 	break;									
			case 0x0060:
				case 0x0062:
					case 0x0050:
						case 0x0052:
             if(mm==0x0050)j=1;
             	 if(mm==0x0052)j=2;
	  		     if(mm==0x0060)j=0;
	  		     	 if(mm==0x0062)j=3;
		   		if(rvb[PCMD] == 0xc1){
					if(len !=2) {
                                                     ans = 0x07;
						break;
					}
			     	lt = rvb[PDUA+3+i] + (rvb[PDUA+4+i] << 8);
			     	my.voltage[j] = lt;
			     	if(my.voltage[j]>5000) {
			     		my.voltage[j] = 5000;
			     		ans = 1;
			     	}
			     	//BiasCompensate_Proc();
			    }
			    else{
				     if(len !=2) {
                                                     ans = 0x08;
						break;
					}	
			     	rvb[PDUA+3+i] =  my.voltage[j]&0xff;
			     	rvb[PDUA+4+i] = my.voltage[j]>>8;
				}
		        break;	
	
				
			case 0x8000:  
				rvb[PDUA+3+i] = my.po1_2g&0xff;
				break;
			case 0x8001:  
				rvb[PDUA+3+i] = my.po2_2g&0xff;
				break;
			case 0x8004:  
				rvb[PDUA+3+i] = my.po3_2g&0xff;
				break;	
			case 0x8005:  
				rvb[PDUA+3+i] = my.po4_2g&0xff;
				break;	


				
			case 0x8045: 
				rvb[PDUA+3+i] = my.temp;
				break;
			case 0x8056:
				rvb[PDUA+3+i] = my.current & 0xff;
				rvb[PDUA+4+i] = my.current >> 8;
				break;	
			case 0x8086:
				rvb[PDUA+3+i] = my.vopo1 & 0xff;//正向功率检测电压
				rvb[PDUA+4+i] = my.vopo1 >> 8;
				break;
		         case 0x8088:
				rvb[PDUA+3+i] = my.vopo2 & 0xff;//正向功率检测电压
				rvb[PDUA+4+i] = my.vopo2 >> 8;
				//itest = - amplifier.power_b[2];
				//rvb[PDUA+3+i]= itest & 0xff;
				//rvb[PDUA+4+i]= itest >> 8;
				break;
			case 0x808e:
				rvb[PDUA+3+i] = my.vopo3 & 0xff;//正向功率检测电压
				rvb[PDUA+4+i] = my.vopo3 >> 8;
				//rvb[PDUA+3+i]= amplifier.power_k[2] & 0xff;
				//rvb[PDUA+4+i]= amplifier.power_k[2] >> 8;
				break;
					         case 0x8090:
				rvb[PDUA+3+i] = my.vopo4 & 0xff;//正向功率检测电压
				rvb[PDUA+4+i] = my.vopo4 >> 8;
				//itest = - amplifier.power_b[2];
				//rvb[PDUA+3+i]= itest & 0xff;
				//rvb[PDUA+4+i]= itest >> 8;
				break;

			case 0x80bc:
				rvb[PDUA+3+i] = my.votemp & 0xff;//detected voltage of temperature
				rvb[PDUA+4+i] = my.votemp >> 8;
				break;
			case 0x8076:
				rvb[PDUA+3+i] = my.vocurrent & 0xff;//正向功率检测电压
				rvb[PDUA+4+i] = my.vocurrent >> 8;
				break;		
		
			case 0x80c6://功放运行状态,0-正常,1-功放过温关闭,2-功放过功关闭
				rvb[PDUA+3+i] = my.realstatus;
				break;
			case 0x8028:
			        rvb[PDUA+3+i] = my.alarm_state;
			        break;
			case 0x8018:
			        rvb[PDUA+3+i] = my.alarm_state_po1;
					break;
			case 0x8019:
			        rvb[PDUA+3+i] = my.alarm_state_po2;
					break;
			case 0x801c:
			        rvb[PDUA+3+i] = my.alarm_state_po3;
					break;
							case 0x801d:
			        rvb[PDUA+3+i] = my.alarm_state_po4;
					break;
			default:
					break;
		}
	    i += rvb[PDUA+2+i]+3;
	}
	s_command(rvb[PCMD],mm,ans);
	//save own para to eeprom here
	if(rvb[PCMD] == 0xc1){
		/*为了解决开关电时，参数容易掉的情况，广播消息时不保存参数。*/
		if(ans>0) return;
		pstr = (uchar *)&my;
		for(i = 0;i<40;i++)	sdb[i] = pstr[i];//EEBuf
		//EEBuf[3] = 0;//解决监控设置衰减后设置模块参数时保存监控设置的值BUG;
		UPDATE_EEPROM(EE_ADDR_MY, 60);
		pstr = (uchar *) &own;
		for (i = 0;i < sizeof(own); i++) sdb[i] = pstr[i];//EEbuf
		UPDATE_EEPROM(EE_ADDR_OWN, sizeof(own));
		
	}
}





uchar cpacks,curP;

void Query_Paralist_Proc()
{
	uchar j,pos;
	uchar cc;
	cc = rvb[PDUA];//包数
	if(cc == 1)//算总包数
	{
		cpacks = sum_paras/20;
		if (sum_paras%20) cpacks++;
	}
	//len = rvb[PDUA];
	rvb[PDUA] = cpacks;
	rvb[PDUA+1] = cc;//包数	
	pos = PDUA+2;
	j = ((cc-1)*20);
	while(j < sum_paras){

	

			rvb[pos++] = ParaList_TABD[j].Para;
			rvb[pos++] = ParaList_TABD[j].Para>>8;
			rvb[pos++] = ParaList_TABD[j].Len;
			rvb[pos++] = ParaList_TABD[j].Arith;
	
		j++;
		if(pos>=88) break;
 			if(ParaList_TABD[j].Para ==0xffff) break;

	}

	rbw = pos + 3;
	s_command(rvb[PCMD],0,0);
}


/*————————————————————————————————————————————————————————————————
  头，版本，模块ID，消息ID，应答标志，长度 ,参数地址，数据n,校验和2， 结束标志
  -1    0     1      2         3       4      5,6         n    n+8,n+9  n+10
  ————————————————————————————————————————————————————————————————*/
void Msg_Proc()
{
	if(get_msg() == 0) return;
	delayx(4);
	/*此处增加是为了解决短包时,响应时间跟长包差不多,短包时响应时间时2-3ms, 长包响应时间时4-6ms,因此延时一些. */
	if(rbw < 20) delayx(3);
	else if((rbw>=20)&&(rbw<40)) delayx(2);
	delayx(8);
	if (rvb[PCMD] == 0x50)//提取参数列表
		Query_Paralist_Proc();
	else if ((rvb[PCMD] == 0x60) || (rvb[PCMD] == 0x40))
	{
		OWN_Msg_Proc();
	}	
	else if((rvb[PCMD] == 0xc1)||(rvb[PCMD] == 0xc2))
	{		
		PA_MSG_Proc();
	}
	else if((rvb[PCMD] == 0xd1)||(rvb[PCMD] == 0xd2))
	{
		if(rvb[PCODE] != own.addr)
	  {
	            return; 
	  }		
		IT_Msg_Proc();
	}
	rx_state = rbw = 0;
}

