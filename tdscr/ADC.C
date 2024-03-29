/************************************************************
 * Copyright (c) 2004-2005, POWERCOM Co., All Rights Reserved.
 * File        :"adc.c"
 * Description :

 * Author      : Xu Hailun
 * Date        : 2004-07-24
 * Version     : 1.0
 * Revisions   :
 * Revision, 
 *	1.0.01, 2005-03-01 by xhl
 *		fix ADC BUG
 *  1.0.01, 2005-03-31 by ly
 *		fix UPA power off BUG
 ************************************************************/
//#define __ADC_FREE_RUN_MODE__
#define __DAC108__
#include "def.h"
#include "all.h"
uchar adc_flag;
#define MAX_ADC_WAIT		1000
#define MAX_FPOWER			1023//maximum forward power.

//#define DALDAC				3  //portD
#define DAFS				  7 //portD
#define DASCLK				6  //portD
#define DADIN				  5  //portD

extern void delayx(uint uiv);


void Init_ADC()
{
  	//DDRF = 0;
  	//PORTF = 0;
  	  	//PORTF = 0;
  	ADCSRA = 0X00;
  	adc_mux = 0;
  	ADMUX = (1<<REFS0)|(adc_mux&0x0f);//参考电压AVCC，通道０
  	ACSR = (1<<ACD);//analog comparator disable，模拟比较器禁止
#ifdef __ADC_FREE_RUN_MODE__
  	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADFR)|(1<<ADPS2)|(1<<ADPS1);//disabel ADC interrupte, free runnig,64分频,adfr自动触发位
#else
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1);//disabel ADC interrupte, 64分频,
	//ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADPS3)|(1<<ADPS2)|(1<<ADPS1);//disabel ADC interrupte, 128分频,晶振3.8m，adc最大时钟1m，13时钟周期转换完成，13us
	//一个TDMA帧分八个时隙，总长4.615ms，所以每个时隙0.577ms,保证检测在250us一次，超过30次就可以保障检测成功
#endif
}

uchar Read_ADC(uchar num)
{
    uint k;
    if(adc_flag !=num)//通道不变不切换
    	{
				ADMUX = (1<<REFS0)|(num&0x0f);
				adc_flag =num;
			}
//	ADCSR |= (1<<ADSC);
#ifdef __ADC_FREE_RUN_MODE__
  	ADCSR = (1<<ADEN)|(1<<ADSC)|(1<<ADFR)|(1<<ADPS2)|(1<<ADPS1);//disabel ADC interrupte, free runnig,64分频,
	adc_rel=ADC;

#else
	ADCSRA = (1<<ADEN)|(1<<ADSC)|(1<<ADPS2)|(1<<ADPS1);//disabel ADC interrupte, 64分频,
	k = 0;
	while(chkbit(ADCSRA,ADSC)){
		k++;
		if(k>MAX_ADC_WAIT) return 0;//超时失败
	}
	adc_rel=ADC;
#endif

	return 1;
}

extern void VoltageConvertTo(uchar i0);


//__flash uchar lp[] = {0,1,2,3,4,5,6,7};
__flash uchar lp[] = {1,2,3,4,5,6,7};

//void ADC_Control()//求时隙功率，连续采样取最大值
//{//invoke per 10ms
//    uchar i,j,ci;
//    uint l,adc_buf,itemp;
//   
//	switch(ADC_state[lp[ctri]]){
//		case 0:
//			ADC_state[lp[ctri]] = 1;
//			ADC_count[lp[ctri]] = 0;
//			break;
//		case 1:		
//				if(ADC_count[lp[ctri]] < MAX_SAMPLING){
//				adc_buf = 0;
//                        for (ci = 0; ci <= 29; ci++) {
//                                if (Read_ADC(lp[ctri])) {
//                                        itemp = adc_rel;
//                                }
//                                else{
//                                        itemp = 0;
//                                }
//                                if(itemp > 1023) {
//                                        itemp = 0;
//                                }
//                                if (itemp > adc_buf){
//                                        adc_buf = itemp;
//                                }
//                        }
//                        uiADC[ADC_count[lp[ctri]]] = adc_buf;
//                      ADC_count[lp[ctri]]++;}
//			else
//				{
//				l = 0;j = MAX_SAMPLING - 2;
//				for(i=2;i<MAX_SAMPLING;i++)
//				{      
//					if(uiADC[i]>MAX_FPOWER) j--; //数字滤波
//					else		
//	      			        l += uiADC[i];
//				}
//				adct[lp[ctri]].result = l/j;//求平均值
//				ADC_state[lp[ctri]] = 0;
//       			VoltageConvertTo(lp[ctri]);
//			  if(++ctri>=MAX_ADCCH) ctri = 0;/////由于采样不准，因此把函数最后一行移到此
//			}
//			break;
//		default:
//			ADC_state[lp[ctri]] = 0;
//			break;
//	}
//}

void ADC_Control()
{
        uchar  ci;
        uint   itemp;

        switch (lp[ctri]) {
                case 0:
                case 1:
                case 2:
                case 3:
                case 4:
                case 5:
                case 6:
                case 7:
                        adct[lp[ctri]].result = 0;
                        for (ci = 0; ci <= 29; ci++) {
                                if (Read_ADC(lp[ctri])) {
                                        itemp = adc_rel;
                                }
                                else{
                                        itemp = 0;
                                }
                                if(itemp > 1023) {
                                        itemp = 0;
                                }
                                if (itemp > adct[lp[ctri]].result){
                                        adct[lp[ctri]].result = itemp;
                                }
                        }
                        VoltageConvertTo(lp[ctri]);
                        if (++ctri > 7) {
                                ctri = 0;
                        }
                        break;
                default:
                        break;
        }
}



#define PROTECT_VOLTAGE		1023
/*=====================================D/A convert========================================*/


void Update_DAC_Model(uchar addr, uint v)
//void Update_DAC()
{   uchar i;
    uint k,m;
    ulong lt;
	
	setbit(PORTD,DASCLK);//SetPortValue(0, 20, buf);
	setbit(PORTD,DADIN);
	clrbit(PORTD,DAFS);
	

  k = (addr<<12)+ v;
	for(i=0;i<16;i++){
		if(k&0x8000)	setbit(PORTD,DADIN);
		else 			clrbit(PORTD,DADIN);
		clrbit(PORTD,DASCLK);
		k = k<<1;
		setbit(PORTD,DASCLK);
	}
	setbit(PORTD,DASCLK);
	setbit(PORTD,DADIN);
	setbit(PORTD,DAFS);
	i++;//delay	
	i++;//delay
	
}



void Update_DAC(uchar addr, uint v)
{   uchar i;
    uint k,m;
    ulong lt;
	setbit(PORTD,DASCLK);
	setbit(PORTD,DADIN);
	clrbit(PORTD,DAFS);
	lt = (ulong)v << 7;
	lt = lt/625;
	m = (uint)lt;
  k = (addr<<12) +((m&0x3ff)<<2);
	for(i=0;i<16;i++){
		if(k&0x8000)	setbit(PORTD,DADIN);
		else 			clrbit(PORTD,DADIN);
		clrbit(PORTD,DASCLK);
		k = k<<1;
		setbit(PORTD,DASCLK);
	}
	setbit(PORTD,DASCLK);
	setbit(PORTD,DADIN);
	setbit(PORTD,DAFS);
	i++;//delay
	i++;//delay
}

void Preset_DAC()
{   
	uint k;

	Update_DAC_Model(8,0);
	Update_DAC_Model(9,0);
}

void DAC_Init()
{

	setbit(PORTD,DAFS);
	setbit(PORTD,DASCLK);
	setbit(PORTD,DADIN);
	Preset_DAC();
}


			
extern void BiasCompensate_Proc();
			

void PA_PowerOn()
{
   // uchar i;
   // for(i=0;i<8;i++) Update_DAC(i, my.voltage[i]);
  //  BiasCompensate_Proc();  
 // my.upsw0 =0;
  // my.upsw1 =0;
        
	if (my.sw0==0)	PORTD &= 0xfB;
        else			PORTD |= 0x04;
        	delayx(10);
//	if (my.sw1==0 )	PORTC &=0xF7;
//        else			PORTC |= 0x08;
   
        
        	delayx(10);
         if (my.upsw0==0 )	PORTD &= 0xf7;
        else			PORTD |=0x08; 
        		delayx(10);
//	if (my.upsw1==0 )	PORTC &= 0xfb;
//        else			PORTC |= 0x04;  
my.sw2 = 0;
if(my.sw2 == 0) //自动都关或者高阻待定
				    	     { 
				    	       PORTC &=0xF7;
				    	       PORTC &=0xFB;
				             }
        	BiasCompensate_Proc();
}


void PA_PowerOff()
{

    uchar i;	
    for(i=0;i<8;i++) Update_DAC(i,0);
}

