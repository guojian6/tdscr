/*******************************************************
 * Project : for 3G Module
 * Version : 1.0
 * Date    : 2004-11-29
 * Author  : Xu Hailun
 * Company : Shenzhen Powercom Co.,Ltd 
 * Compiler: IAR
 * Comments: This project can fit in with 10WPA,5WPA,FSU(S/D),LNA(S/D),UPA.
 * Chip type           : ATMEGA8
 * Clock frequency     : 3.686400 MHz
 * Memory model        : small
 * External SRAM size  : 0
 * Data Stack size     : 256
 *
 * Revisions   :
 *	
 * 	20050616, by xhl
 *		增加光模块部门,关键字:__OPT_MODULE__
 **********************************************************/
#define __MAIN_PROGRAM__


#include "def.h"
#include "all.h"

//extern void USART_Send();
extern void ADC_Control();
extern void Init_ADC();
extern void DAC_Init();
extern void BiasCompensate_Proc();
extern void Scan_Warn();
extern void Update_DAC(uchar addr, uint v);
extern void Enable_RS485(uchar op);

extern uchar Init_Load();
extern void USART_Init();

extern void Send_Log();
extern void Msg_Proc();
extern void PA_PowerOn();
extern void PA_PowerOff();
extern void Set_Attenuation(uchar v);
extern void IT_Msg_Proc();


extern uchar rx_state,rev_count;

void delayx(uint x)
{//Unit 1ms
    uint i0,i1;
	for(i0=0;i0<x;i0++){
		for(i1=0;i1<600;i1++){
			i1++;
			i1--;
		}
		asm("wdr");
	}
}


#define WDCE  4
#define WDE   3
void WDT_off(void)
{
	/* Write logical one to WDCE and WDE */
	WDTCR = 0x18;
	WDTCR = 0x00;/* Turn off WDT */	
}

void WDT_Init(void)
{
	asm("wdr");
	WDTCR = 0x18;
	WDTCR = 0x0e;
}

void Timer0_Init()
{
	TCCR0 = 0X04;//fosc/256
	TCNT0 = 0XF1;/*fosc/256*(256-x) = 1/1ms. ===> x = TCNT0 = 0XF1*/
	TIMSK = 0X01;
}

void Atmega8_Init()
{
	WDT_off();//first line
         PORTA=0x00;
	DDRA=0x01;	
	PORTB=0x00;
	DDRB=0xff;
	PORTC=0x00;
	DDRC=0xff;
	PORTD=0x00;
	DDRD=0xfe;
	MCUCR = 0X00;
	
	USART_Init();
	Init_Load();
	Init_ADC();
	DAC_Init();
	Timer0_Init();
	WDT_Init();
	asm("sei");
}

#pragma vector = TIMER0_OVF_vect
__interrupt void timer0_isr()//1ms
{
	TCNT0 = 0XF1;
	fast_flag++;
	led_flag++;
	if(fast_flag>24){
		fast_flag = 0;
		slow_flag++;
		if(++rev_count>40) rx_state = 0;
	}
}



void System_Init()
{
	unsigned char i;
	Atmega8_Init();
	my.alarm_state = 0;
	my.alarm_state_po1 = 0;
	my.alarm_state_po2 = 0;
	my.alarm_state_po3= 0;
	my.alarm_state_po4 = 0;
	led_flag =0;
	for(i=0;i<=7;i++) Update_DAC(i,0);
	
          PA_PowerOn();
				                
}

uchar led_c,led;
void main()
{
	uchar j;
	asm("cli");
	delayx(800);
	reset_f = IDLE;
	eewp = 0x1234;
Start:
	System_Init();
	reset_f = IDLE;
	while(1){
		if(slow_flag>5){//10ms
			asm("wdr");//clear watchdog counter.
			slow_flag = 0;
			if((reset_f == RESTART)||(reset_f == RESET_PARA)) goto Start;
			if((reset_f == REBOOT)||(reset_f == UPDATE_FLASH)) while(1);//reboot system	
			//for(j=0;j<4;j++)
			ADC_Control();
			
		}
      	if(my.rfswchan==2)
      		{
					   if(led_flag==500)
					   PORTB |=0x20;
					   if(led_flag>=1000)
					   {
					   	PORTB &=0xDF;
					   	led_flag =0;
					  }
      			}
      		else
      			{
      				PORTB |=0x20;
      		
      				}
		Msg_Proc();
		if(cmp_flag == 1){
		      BiasCompensate_Proc();
		      cmp_flag = 0;
		}	
	}
}


