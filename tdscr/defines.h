//*****************************************************************************
// MCU selection
// 		by xuhailun
//    	20060208
//*****************************************************************************
#define MEGATYPE	32

#if (MEGATYPE == 16)
	#include <iom16.h>
/**************************
	#define ADCSR		ADCSRA
	#define WDTCR		WDTCSR
	#define TCCR0		TCCR0B
	#define TIMSK		TIMSK0
	#define EEWE		EEPE
	#define EEMWE		EEMPE
	#define USART_RXC_vect	USART_RX_vect
	#define UCSRA 		 UCSR0A
	#define UCSRB  	 	 UCSR0B
	#define UCSRC 		 UCSR0C
	#define UBRRL		 UBRR0L
	#define UBRRH 		 UBRR0H
	#define UDR		 	 UDR0
	#define UDRE		UDRE0
	#define RXC			RXC0
***************************/
#else
	#include <iom32.h>
	#define  ADFR  ADATE

#endif
