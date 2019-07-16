/****************************************************************************
 **             - iom168.h -
 **
 **     This file declares the internal register addresses for ATmega168.
 **
 **     Used with iccAVR and aAVR.
 **
 **     Copyright IAR Systems 2003. All rights reserved.
 **
 **     File version: $Revision: 1.13 $
 **
 ***************************************************************************/

#include "iomacro.h"

#if TID_GUARD(3)
#error This file should only be compiled with iccavr or aavr whith processor option -v3
#endif /* TID_GUARD(3) */

#ifdef __HAS_RAMPZ__
#error This file should not have RAMPZ enabled, use --cpu or --64k_flash
#endif /* __HAS_RAMPZ__ */

/* Include the SFR part if this file has not been included before,
 * OR this file is included by the assembler (SFRs must be defined in
 * each assembler module). */
#if !defined(__IOM168_H) || defined(__IAR_SYSTEMS_ASM__)

#pragma language=extended

/*==========================*/
/* Predefined SFR Addresses */
/*==========================*/

/****************************************************************************
 * An example showing the SFR_B() macro call, 
 * the expanded result and usage of this result:
 *
 * SFR_B(AVR,   0x1F) Expands to:
 * __io union {
 *             unsigned char AVR;                 // The sfrb as 1 byte
 *             struct {                           // The sfrb as 8 bits
 *                     unsigned char AVR_Bit0:1,
 *                                   AVR_Bit1:1,
 *                                   AVR_Bit2:1,
 *                                   AVR_Bit3:1,
 *                                   AVR_Bit4:1,
 *                                   AVR_Bit5:1,
 *                                   AVR_Bit6:1,
 *                                   AVR_Bit7:1;
 *                    };
 *            } @ 0x1F;
 * Examples of how to use the expanded result:
 * AVR |= (1<<5);
 * or like this:
 * AVR_Bit5)
 ***************************************************************************/

SFR_B_R(0xC6,UDR0)
SFR_W_R(0xC4,UBRR0)
SFR_B_N(0xC2,UCSR0C,UMSEL01,UMSEL00,UPM01,UPM00,USBS0,UCSZ01,UCSZ00,UCPOL0)
SFR_B_N(0xC1,UCSR0B,RXCIE0,TXCIE0,UDRIE0,RXEN0,TXEN0,UCSZ02,RXB80,TXB80)
SFR_B_N(0xC0,UCSR0A,RXC0,TXC0,UDRE0,FE0,DOR0,UPE0,U2X0,MPCM0)
SFR_B_N(0xBD,TWAMR,TWAM6,TWAM5,TWAM4,TWAM3,TWAM2,TWAM1,TWAM0,Dummy)
SFR_B_N(0xBC,TWCR,TWINT,TWEA,TWSTA,TWSTO,TWWC,TWEN,Dummy1,TWIE)
SFR_B_R(0xBB,TWDR)
SFR_B_N(0xBA,TWAR,TWA6,TWA5,TWA4,TWA3,TWA2,TWA1,TWA0,TWGCE)
SFR_B_N(0xB9,TWSR,TWS7,TWS6,TWS5,TWS4,TWS3,Dummy2,TWPS1,TWPS0)
SFR_B_R(0xB8,TWBR)
SFR_B_N(0xB6,ASSR,Dummy7,EXCLK,AS2,TCN2UB,OCR2AUB,OCR2BUB,TCR2AUB,TCR2BUB)
SFR_B_R(0xB4,OCR2B)
SFR_B_R(0xB3,OCR2A)
SFR_B_R(0xB2,TCNT2)
SFR_B_N(0xB1,TCCR2B,FOC2A,FOC2B,Dummy5,Dummy4,WGM22,CS22,CS21,CS20)
SFR_B_N(0xB0,TCCR2A,COM2A1,COM2A0,COM2B1,COM2B0,Dummy3,Dummy2,WGM21,WGM20)
SFR_W_R(0x8A,OCR1B)
SFR_W_R(0x88,OCR1A)
SFR_W_R(0x86,ICR1)
SFR_W_R(0x84,TCNT1)
SFR_B_N(0x82,TCCR1C,FOC1A,FOC1B,Dummy5,Dummy4,Dummy3,Dummy2,Dummy1,Dummy0)
SFR_B_N(0x81,TCCR1B,ICNC1,ICES1,Dummy5,WGM13,WGM12,CS12,CS11,CS10)
SFR_B_N(0x80,TCCR1A,COM1A1,COM1A0,COM1B1,COM1B0,Dummy3,Dummy2,WGM11,WGM10)
SFR_B_N(0x7F,DIDR1,Dummy7,Dummy6,Dummy5,Dummy4,Dummy3,Dummy2,AIN1D,AIN0D)
SFR_B_N(0x7E,DIDR0,Dummy7,Dummy6,ADC5D,ADC4D,ADC3D,ADC2D,ADC1D,ADC0D)
SFR_B_N(0x7C,ADMUX,REFS1,REFS0,ADLAR,Dummy4,MUX3,MUX2,MUX1,MUX0)
SFR_B_N(0x7B,ADCSRB,Dummy7,ACME,Dummy5,Dummy4,Dummy3,ADTS2,ADTS1,ADTS0)
SFR_B_N(0x7A,ADCSRA,ADEN,ADSC,ADATE,ADIF,ADIE,ADPS2,ADPS1,ADPS0)
SFR_W_R(0x78,ADC)
SFR_B_N(0x70,TIMSK2,Dummy7,Dummy6,Dummy5,Dummy4,Dummy3,OCIE2B,OCIE2A,TOIE2)
SFR_B_N(0x6F,TIMSK1,Dummy7,Dummy6,ICIE1,Dummy4,Dummy3,OCIE1B,OCIE1A,TOIE1)
SFR_B_N(0x6E,TIMSK0,Dummy7,Dummy6,Dummy5,Dummy4,Dummy3,OCIE0B,OCIE0A,TOIE0)
SFR_B_N(0x6D,PCMSK2,PCINT23,PCINT22,PCINT21,PCINT20,PCINT19,PCINT18,PCINT17,PCINT16)
SFR_B_N(0x6C,PCMSK1,Dummy7,PCINT14,PCINT13,PCINT12,PCINT11,PCINT10,PCINT9,PCINT8)
SFR_B_N(0x6B,PCMSK0,PCINT7,PCINT6,PCINT5,PCINT4,PCINT3,PCINT2,PCINT1,PCINT0)
SFR_B_N(0x69,EICRA,Dummy7,Dummy6,Dummy5,Dummy4,ISC11,ISC10,ISC01,ISC00)
SFR_B_N(0x68,PCICR,Dummy7,Dummy6,Dummy5,Dummy4,Dummy3,PCIE2,PCIE1,PCIE0)
SFR_B_R(0x66,OSCCAL)
SFR_B_N(0x64,PRR,PRTWI,PRTIM2,PRTIM0,Dummy4,PRTIM1,PRSPI,PRUSART0,PRADC)
SFR_B_N(0x61,CLKPR,CLKPCE,Dummy6,Dummy5,Dummy4,CLKPS3,CLKPS2,CLKPS1,CLKPS0)
SFR_B_N(0x60,WDTCSR,WDIF,WDIE,WDP3,WDCE,WDE,WDP2,WDP1,WDP0)
SFR_B_N(0x3F,SREG,I,T,H,S,V,N,Z,C)
SFR_B_N(0x3E,SPH,Dummy7,Dummy6,Dummy5,Dummy4,Dummy3,SP10,SP9,SP8)
SFR_B_N(0x3D,SPL,SP7,SP6,SP5,SP4,SP3,SP2,SP1,SP0)
SFR_B_N(0x37,SPMCSR,SPMIE,RWWSB,Dummy5,RWWSRE,BLBSET,PGWRT,PGERS,SELFPRGEN)
SFR_B_N(0x35,MCUCR,Dummy7,Dummy6,Dummy5,PUD,Dummy3,Dummy2,IVSEL,IVCE)
SFR_B_N(0x34,MCUSR,Dummy7,Dummy6,Dummy5,Dummy4,WDRF,BORF,EXTRF,PORF)
SFR_B_N(0x33,SMCR,Dummy7,Dummy6,Dummy5,Dummy4,SM2,SM1,SM0,SE)
SFR_B_R(0x31,MONDR)
SFR_B_N(0x30,ACSR,ACD,ACBG,ACO,ACI,ACIE,ACIC,ACIS1,ACIS0)
SFR_B_R(0x2E,SPDR)
SFR_B_N(0x2D,SPSR,SPIF,WCOL,Dummy5,Dummy4,Dummy3,Dummy2,Dummy1,SPI2X)
SFR_B_N(0x2C,SPCR,SPIE,SPE,DORD,MSTR,CPOL,CPHA,SPR1,SPR0)
SFR_B_R(0x2B,GPIOR2)
SFR_B_R(0x2A,GPIOR1)
SFR_B_R(0x28,OCR0B)
SFR_B_R(0x27,OCR0A)
SFR_B_R(0x26,TCNT0)
SFR_B_N(0x25,TCCR0B,FOC0A,FOC0B,Dummy5,Dummy4,WGM02,CS02,CS01,CS00)
SFR_B_N(0x24,TCCR0A,COM0A1,COM0A0,COM0B1,COM0B0,Dummy3,Dummy2,WGM01,WGM00)
SFR_B_N(0x23,GTCCR,TSM,Dummy6,Dummy5,Dummy4,Dummy3,Dummy2,PSRASY,PSRSYNC)
SFR_W_R(0x21,EEAR)
SFR_B_R(0x20,EEDR)
SFR_B_N(0x1F,EECR,Dummy7,Dummy6,EEPM1,EEPM0,EERIE,EEMPE,EEPE,EERE)
SFR_B_R(0x1E,GPIOR0)
SFR_B_N(0x1D,EIMSK,Dummy7,Dummy6,Dummy5,Dummy4,Dummy3,Dummy2,INT1,INT0)
SFR_B_N(0x1C,EIFR,Dummy7,Dummy6,Dummy5,Dummy4,Dummy3,Dummy2,INTF1,INTF0)
SFR_B_N(0x1B,PCIFR,Dummy7,Dummy6,Dummy5,Dummy4,Dummy3,PCIF2,PCIF1,PCIF0)
SFR_B_N(0x17,TIFR2,Dummy7,Dummy6,Dummy5,Dummy4,Dummy3,OCF2B,OCF2A,TOV2)
SFR_B_N(0x16,TIFR1,Dummy7,Dummy6,ICF1,Dummy4,Dummy3,OCF1B,OCF1A,TOV1)
SFR_B_N(0x15,TIFR0,Dummy7,Dummy6,Dummy5,Dummy4,Dummy3,OCF0B,OCF0A,TOV0)
SFR_B_N(0x0B,PORTD,PORTD7,PORTD6,PORTD5,PORTD4,PORTD3,PORTD2,PORTD1,PORTD0)
SFR_B_N(0x0A,DDRD,DDD7,DDD6,DDD5,DDD4,DDD3,DDD2,DDD1,DDD0)
SFR_B_N(0x09,PIND,PIND7,PIND6,PIND5,PIND4,PIND3,PIND2,PIND1,PIND0)
SFR_B_N(0x08,PORTC,Dummy7,PORTC6,PORTC5,PORTC4,PORTC3,PORTC2,PORTC1,PORTC0)
SFR_B_N(0x07,DDRC,Dummy7,DDC6,DDC5,DDC4,DDC3,DDC2,DDC1,DDC0)
SFR_B_N(0x06,PINC,Dummy7,PINC6,PINC5,PINC4,PINC3,PINC2,PINC1,PINC0)
SFR_B_N(0x05,PORTB,PORTB7,PORTB6,PORTB5,PORTB4,PORTB3,PORTB2,PORTB1,PORTB0)
SFR_B_N(0x04,DDRB,DDB7,DDB6,DDB5,DDB4,DDB3,DDB2,DDB1,DDB0)
SFR_B_N(0x03,PINB,PINB7,PINB6,PINB5,PINB4,PINB3,PINB2,PINB1,PINB0)

#ifndef __IOM168_H
#define __IOM168_H


/* SFRs are local in assembler modules (so this file may need to be */
/* included in more than one module in the same source file), */
/* but #defines must only be made once per source file. */

/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

/* NB! vectors are specified as byte addresses */


#define  RESET_vect           (0x00) /*  External Pin, Power-on Reset, Brownout
                                         Reset and Watchdog Reset */
#define  INT0_vect            (0x04) /* External Interrupt Request 0 */
#define  INT1_vect            (0x08) /* External Interrupt Request 1 */
#define  PCINT0_vect          (0x0C) /* Pin Change Interrupt Request 0 */
#define  PCINT1_vect          (0x10) /* Pin Change Interrupt Request 1 */
#define  PCINT2_vect          (0x14) /* Pin Change Interrupt Request 2 */
#define  WDT_vect             (0x18) /* Watchdog Time-out Interrupt */
#define  TIMER2_COMPA_vect    (0x1C) /* Timer/Counter2 Compare Match A */
#define  TIMER2_COMPB_vect    (0x20) /* Timer/Counter2 Compare Match B */
#define  TIMER2_OVF_vect      (0x24) /* Timer/Counter2 Overflow */
#define  TIMER1_CAPT_vect     (0x28) /* Timer/Counter1 Capture Event */
#define  TIMER1_COMPA_vect    (0x2C) /* Timer/Counter1 Compare Match A */
#define  TIMER1_COMPB_vect    (0x30) /* Timer/Coutner1 Compare Match B */
#define  TIMER1_OVF_vect      (0x34) /* Timer/Counter1 Overflow */
#define  TIMER0_COMPA_vect    (0x38) /* Timer/Counter0 Compare Match A */
#define  TIMER0_COMPB_vect    (0x3C) /* Timer/Counter0 Compare Match B */
#define  TIMER0_OVF_vect      (0x40) /* Timer/Counter0 Overflow */
#define  SPI_STC_vect         (0x44) /* SPI Serial Transfer Complete */
#define  USART_RX_vect        (0x48) /* USART Rx Complete */
#define  USART_UDRE_vect      (0x4C) /* USART, Data Register Empty */
#define  USART_TX_vect        (0x50) /* USART, Tx Complete */
#define  ADC_vect             (0x54) /* ADC Conversion Complete */
#define  EE_RDY_vect          (0x58) /* EEPROM Ready */
#define  ANA_COMP_vect        (0x5C) /* Analog Comparator */
#define  TWI_vect             (0x60) /* 2-wire Serial Interface */
#define  SPM_READY_vect       (0x64) /* Store Program Memory Ready */

#ifdef __IAR_SYSTEMS_ASM__   
#ifndef ENABLE_BIT_DEFINITIONS
#define  ENABLE_BIT_DEFINITIONS
#endif /* ENABLE_BIT_DEFINITIONS */
#endif /* __IAR_SYSTEMS_ASM__ */

#ifdef ENABLE_BIT_DEFINITIONS


/* Bit definitions for use with the IAR Assembler   
   The Register Bit names are represented by their bit number (0-7).
*/

/*UCSR0C*/
#define UMSEL01   7
#define UMSEL00   6
#define UPM01     5
#define UPM00     4
#define USBS0     3
#define UDORD0    2
#define UCPHA0    1
#define UCPOL0    0

#define UCSZ01    UDORD0
#define UCSZ00    UCPHA0

/*UCSR0B*/
#define RXCIE0    7
#define TXCIE0    6
#define UDRIE0    5
#define RXEN0     4
#define TXEN0     3
#define UCSZ02    2
#define RXB80     1
#define TXB80     0

/*UCSR0A*/
#define RXC0      7
#define TXC0      6
#define UDRE0     5
#define FE0       4
#define DOR0      3
#define UPE0      2
#define U2X0      1
#define MPCM0     0


/*TWAMR*/
#define TWAM6     7
#define TWAM5     6
#define TWAM4     5
#define TWAM3     4
#define TWAM2     3
#define TWAM1     2
#define TWAM0     1

/*TWCR*/
#define TWINT     7
#define TWEA      6
#define TWSTA     5
#define TWSTO     4
#define TWWC      3
#define TWEN      2
#define TWIE      0

/*TWAR*/
#define TWA6      7
#define TWA5      6
#define TWA4      5
#define TWA3      4
#define TWA2      3
#define TWA1      2
#define TWA0      1
#define TWGCE     0

/*TWSR*/
#define TWS7      7
#define TWS6      6
#define TWS5      5
#define TWS4      4
#define TWS3      3
#define TWPS1     1
#define TWPS0     0


/*ASSR*/
#define EXCLK     6
#define AS2       5
#define TCN2UB    4
#define OCR2AUB   3
#define OCR2BUB   2
#define TCR2AUB   1
#define TCR2BUB   0


/*TCCR2B*/
#define FOC2A     7
#define FOC2B     6
#define WGM22     3
#define CS22      2
#define CS21      1
#define CS20      0

/*TCCR2A*/
#define COM2A1    7
#define COM2A0    6
#define COM2B1    5
#define COM2B0    4
#define WGM21     1
#define WGM20     0


/*TCCR1C*/
#define FOC1A     7
#define FOC1B     6

/*TCCR1B*/
#define ICNC1     7
#define ICES1     6
#define WGM13     4
#define WGM12     3
#define CS12      2
#define CS11      1
#define CS10      0

/*TCCR1A*/
#define COM1A1    7
#define COM1A0    6
#define COM1B1    5
#define COM1B0    4
#define WGM11     1
#define WGM10     0

/*DIDR1*/
#define AIN1D     1
#define AIN0D     0

/*DIDR0*/
#define ADC5D     5
#define ADC4D     4
#define ADC3D     3
#define ADC2D     2
#define ADC1D     1
#define ADC0D     0


/*ADMUX*/
#define REFS1     7
#define REFS0     6
#define ADLAR     5
#define MUX3      3
#define MUX2      2
#define MUX1      1
#define MUX0      0

/*ADCSRB*/
#define ACME      6
#define ADTS2     2
#define ADTS1     1
#define ADTS0     0

/*ADCSRA*/
#define ADEN      7
#define ADSC      6
#define ADATE     5
#define ADIF      4
#define ADIE      3
#define ADPS2     2
#define ADPS1     1
#define ADPS0     0


/*TIMSK2*/
#define OCIE2B    2
#define OCIE2A    1
#define TOIE2     0

/*TIMSK1*/
#define ICIE1     5
#define OCIE1B    2
#define OCIE1A    1
#define TOIE1     0

/*TIMSK0*/
#define OCIE0B    2
#define OCIE0A    1
#define TOIE0     0

/*PCMSK2*/
#define PCINT23   7
#define PCINT22   6
#define PCINT21   5
#define PCINT20   4
#define PCINT19   3
#define PCINT18   2
#define PCINT17   1
#define PCINT16   0

/*PCMSK1*/
#define PCINT14   6
#define PCINT13   5
#define PCINT12   4
#define PCINT11   3
#define PCINT10   2
#define PCINT9    1
#define PCINT8    0

/*PCMSK0*/
#define PCINT7    7
#define PCINT6    6
#define PCINT5    5
#define PCINT4    4
#define PCINT3    3
#define PCINT2    2
#define PCINT1    1
#define PCINT0    0


/*EICRA*/
#define ISC11     3
#define ISC10     2
#define ISC01     1
#define ISC00     0

/*PCICR*/
#define PCIE2     2
#define PCIE1     1
#define PCIE0     0


/*PRR*/
#define PRTW1     7
#define PRTIM2    6
#define PRTIM0    5
#define PRTIM1    3
#define PRSPI     2
#define PRUSART0  1
#define PRADC     0


/*CLKPR*/
#define CLKPCE    7
#define CLKPS3    3
#define CLKPS2    2
#define CLKPS1    1
#define CLKPS0    0

/*WDTCSR*/
#define WDIF      7
#define WDIE      6
#define WDP3      5
#define WDCE      4
#define WDE       3
#define WDP2      2
#define WDP1      1
#define WDP0      0

/* Ordinary I/O space */

/*SPH*/
#define SP10      2
#define SP9       1
#define SP8       0

/*SPL*/
#define SP7       7
#define SP6       6
#define SP5       5
#define SP4       4
#define SP3       3
#define SP2       2
#define SP1       1
#define SP0       0


/*SPMCSR*/
#define SPMIE     7
#define RWWSB     6

#define RWWSRE    4
#define BLBSET    3
#define PGWRT     2
#define PGERS     1
#define SPMEN     0
#define SELFPRGEN 0

/*MCUCR*/
#define PUD       4
#define IVSEL     1
#define IVCE      0

/*MCUSR*/
#define WDRF      3
#define BORF      2
#define EXTRF     1
#define PORF      0

/*SMCR*/
#define SM2       3
#define SM1       2
#define SM0       1
#define SE        0


/*ACSR*/
#define ACD       7
#define ACBG      6
#define ACO       5
#define ACI       4
#define ACIE      3
#define ACIC      2
#define ACIS1     1
#define ACIS0     0


/*SPSR*/
#define SPIF      7
#define WCOL      6
#define SPI2X     0

/*SPCR*/
#define SPIE      7
#define SPE       6
#define DORD      5
#define MSTR      4
#define CPOL      3
#define CPHA      2
#define SPR1      1
#define SPR0      0


/*TCCR0B*/
#define FOC0A     7
#define FOC0B     6
#define WGM02     3
#define CS02      2
#define CS01      1
#define CS00      0

/*TCCR0A*/
#define COM0A1    7
#define COM0A0    6
#define COM0B1    5
#define COM0B0    4
#define WGM01     1
#define WGM00     0

/*GTCCR*/
#define TSM       7
#define PSR2      1
#define PSRASY    1
#define PSR10     0
#define PSRSYNC   0


/*EECR*/
#define EERIE     3
#define EEMPE     2
#define EEPE      1
#define EERE      0

/*EIMSK*/
#define INT1      1
#define INT0      0

/*EIFR*/
#define INTF1     1
#define INTF0     0

/*PCIFR*/
#define PCIF2     2
#define PCIF1     1
#define PCIF0     0


/*TIFR2*/
#define OCF2B     2
#define OCF2A     1
#define TOV2      0

/*TIFR1*/
#define ICF1      5
#define OCF1B     2
#define OCF1A     1
#define TOV1      0

/*TIFR0*/
#define OCF0B     2
#define OCF0A     1
#define TOV0      0


/*PORTD*/
#define PORTD7    7
#define PORTD6    6
#define PORTD5    5
#define PORTD4    4
#define PORTD3    3
#define PORTD2    2
#define PORTD1    1
#define PORTD0    0

#define PD7       7
#define PD6       6
#define PD5       5
#define PD4       4
#define PD3       3
#define PD2       2
#define PD1       1
#define PD0       0

/*DDRD*/
#define DDD7      7
#define DDD6      6
#define DDD5      5
#define DDD4      4
#define DDD3      3
#define DDD2      2
#define DDD1      1
#define DDD0      0

/*PIND*/
#define PIND7     7
#define PIND6     6
#define PIND5     5
#define PIND4     4
#define PIND3     3
#define PIND2     2
#define PIND1     1
#define PIND0     0

/*PORTC*/
#define PORTC6    6
#define PORTC5    5
#define PORTC4    4
#define PORTC3    3
#define PORTC2    2
#define PORTC1    1
#define PORTC0    0

#define PC6       6
#define PC5       5
#define PC4       4
#define PC3       3
#define PC2       2
#define PC1       1
#define PC0       0

/*DDRC*/
#define DDC6      6
#define DDC5      5
#define DDC4      4
#define DDC3      3
#define DDC2      2
#define DDC1      1
#define DDC0      0

/*PINC*/
#define PINC6     6
#define PINC5     5
#define PINC4     4
#define PINC3     3
#define PINC2     2
#define PINC1     1
#define PINC0     0

/*PORTB*/
#define PORTB7    7
#define PORTB6    6
#define PORTB5    5
#define PORTB4    4
#define PORTB3    3
#define PORTB2    2
#define PORTB1    1
#define PORTB0    0

#define PB7       7
#define PB6       6
#define PB5       5
#define PB4       4
#define PB3       3
#define PB2       2
#define PB1       1
#define PB0       0

/*DDRB*/
#define DDB7      7
#define DDB6      6
#define DDB5      5
#define DDB4      4
#define DDB3      3
#define DDB2      2
#define DDB1      1
#define DDB0      0

/*PINB*/
#define PINB7     7
#define PINB6     6
#define PINB5     5
#define PINB4     4
#define PINB3     3
#define PINB2     2
#define PINB1     1
#define PINB0     0


/* Extended Fuse Byte */
#define BOOTSZ1   2
#define BOOTSZ0   1
#define SELFPRGEN 0

/* High Fuse Byte */
#define RSTDISBL  7
#define DWEN      6
#define SPIEN     5
#define WDTON     4
#define EESAVE    3
#define BODLEVEL2 2
#define BODLEVEL1 1
#define BODLEVEL0 0

/* Low Fuse Byte */
#define CKDIV8    7
#define CKOUT     6
#define SUT1      5
#define SUT0      4
#define CKSEL3    3
#define CKSEL2    2
#define CKSEL1    1
#define CKSEL0    0

/* Pointer definition */
#define    XL     r26
#define    XH     r27
#define    YL     r28
#define    YH     r29
#define    ZL     r30
#define    ZH     r31

/* Contants */
#define    RAMEND   0x04FF    /*Last On-Chip SRAM location*/
#define    XRAMEND  0x04FF
#define    E2END    0x01FF
#define    FLASHEND 0x3FFF

#endif /* ENABLE_BIT_DEFINITIONS */ 
#endif /* __IOM168_H (define part) */
#endif /* __IOM168_H (SFR part) */
