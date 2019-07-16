/****************************************************************************
 **             - iom8.h -
 **
 **     This file declares the internal register addresses for ATmega8.
 **
 **     Used with iccAVR and aAVR.
 **
 **     Copyright IAR Systems 2001. All rights reserved.
 **
 **     File version: $Revision: 1.5 $
 **
 ***************************************************************************/

#include "iomacro.h"

#if TID_GUARD(1)
#error This file should only be compiled with iccavr or aavr whith processor option -v1
#endif /* TID_GUARD(1) */

/* Include the SFR part if this file has not been included before,
 * OR this file is included by the assembler (SFRs must be defined in
 * each assembler module). */
#if !defined(__IOM8_H) || defined(__IAR_SYSTEMS_ASM__)

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
 * AVR_Bit5 = 1;
 ***************************************************************************/

SFR_B(TWBR,   0x00) /* TWI Bit rate Register */
SFR_B(TWSR,   0X01) /* TWI status Register */
SFR_B(TWAR,   0x02) /* TWI Address Register */
SFR_B(TWDR,   0x03) /* TWI Data Register */
SFR_W(ADC,    0x04) /* ADC Data register */
SFR_B(ADCSR,  0x06) /* ADC Control and Status Register */
SFR_B(ADMUX,  0x07) /* ADC MUX */
SFR_B(ACSR,   0x08) /* Analog Comparator Control and Status Register */
SFR_B(UBRRL, 0x09) /* UART Baud Rate Register */
SFR_B(UCSRB,  0x0A) /* UART Control and Status Register B */
SFR_B(UCSRA,  0x0B) /* UART Control and Status Register A */
SFR_B(UDR,    0x0C) /* UART I/O Data Register */
SFR_B(SPCR,   0x0D) /* SPI Control Register */
SFR_B(SPSR,   0x0E) /* SPI Status Register */
SFR_B(SPDR,   0x0F) /* SPI I/O Data Register */
SFR_B(PIND,   0x10) /* Input Pins, Port D */
SFR_B(DDRD,   0x11) /* Data Direction Register, Port D */
SFR_B(PORTD,  0x12) /* Data Register, Port D */
SFR_B(PINC,   0x13) /* Input Pins, Port C */
SFR_B(DDRC,   0x14) /* Data Direction Register, Port C */
SFR_B(PORTC,  0x15) /* Data Register, Port C */
SFR_B(PINB,   0x16) /* Input Pins, Port B */
SFR_B(DDRB,   0x17) /* Data Direction Register, Port B */
SFR_B(PORTB,  0x18) /* Data Register, Port B */



SFR_B(EECR,   0x1C) /* EEPROM Control Register */
SFR_B(EEDR,   0x1D) /* EEPROM Data Register */
SFR_W(EEAR,   0x1E) /* EEPROM Address Register */
SFR_B2(UBRRH, UCSRC,  0x20)  
/* UART Baud Rate Register HI */
/* UART Control Register C */
SFR_B(WDTCR,  0x21) /* Watchdog Timer Control Register */
SFR_B(ASSR,   0x22) /* ASynchronous mode Status Register */
SFR_B(OCR2,   0x23) /* Timer/Counter2 Output Compare Register */
SFR_B(TCNT2,  0x24) /* Timer/Counter 2 */
SFR_B(TCCR2,  0x25) /* Timer/Counter 2 Control Register */
SFR_W(ICR1,   0x26) /* T/C 1 Input Capture Register */
SFR_W(OCR1B,  0x28) /* Timer/Counter1 Output Compare Register B */
SFR_W(OCR1A,  0x2A) /* Timer/Counter1 Output Compare Register A */
SFR_W(TCNT1,  0x2C) /* Timer/Counter 1 */
SFR_B(TCCR1B, 0x2E) /* Timer/Counter 1 Control and Status Register */
SFR_B(TCCR1A, 0x2F) /* Timer/Counter 1 Control Register */
SFR_B(SFIOR,  0x30) /* Special Function I/O Register */
SFR_B(OSCCAL, 0x31) /* OSCillator CALibration Register */
SFR_B(TCNT0,  0x32) /* Timer/Counter 0 */
SFR_B(TCCR0,  0x33) /* Timer/Counter 0 Control Register */
SFR_B(MCUCSR,  0x34) /* MCU general Control/Status Register */
SFR_B(MCUCR,  0x35) /* MCU general Control Register */
SFR_B(TWCR,   0x36) /* TWI Control Register */
SFR_B(SPMCR,  0x37) /* SPM Control Register */
SFR_B(TIFR,   0x38) /* Timer/Counter Interrupt Flag register */
SFR_B(TIMSK,  0x39) /* Timer/Counter Interrupt MaSK register */
SFR_B(GIFR,   0x3A) /* General Interrupt Flag Register */
SFR_B(GICR,  0x3B) /* General Interrupt Control register */

SFR_W(SP,     0x3D) /* Stack Pointer */
SFR_B(SREG,   0x3F) /* Status REGister */

#ifndef __IOM8_H
#define __IOM8_H

/* SFRs are local in assembler modules (so this file may need to be */
/* included in more than one module in the same source file), */
/* but #defines must only be made once per source file. */

/*==============================*/
/* Interrupt Vector Definitions */
/*==============================*/

/* NB! vectors are specified as byte addresses */

#define    RESET_vect           (0x00)
#define    INT0_vect            (0x02)
#define    INT1_vect            (0x04)
#define    TIMER2_COMP_vect     (0x06)
#define    TIMER2_OVF_vect      (0x08)
#define    TIMER1_CAPT_vect     (0x0A)
#define    TIMER1_COMPA_vect    (0x0C)
#define    TIMER1_COMPB_vect    (0x0E)
#define    TIMER1_OVF_vect      (0x10)
#define    TIMER0_OVF_vect      (0x12)
#define    SPI_STC_vect         (0x14)
#define    USART_RXC_vect       (0x16)
#define    USART_UDRE_vect      (0x18)
#define    USART_TXC_vect       (0x1A)
#define    ADC_vect             (0x1C)
#define    EE_RDY_vect          (0x1E)
#define    ANA_COMP_vect        (0x20)
#define    TWI_vect             (0x22)
#define    SPM_RDY_vect         (0x24)

#ifdef __IAR_SYSTEMS_ASM__   
#ifndef ENABLE_BIT_DEFINITIONS
#define  ENABLE_BIT_DEFINITIONS
#endif /* ENABLE_BIT_DEFINITIONS */
#endif /* __IAR_SYSTEMS_ASM__ */

#ifdef ENABLE_BIT_DEFINITIONS

/* Bit definitions for use with the IAR Assembler   
   The Register Bit names are represented by their bit number (0-7).
*/
 
/* General Interrupt Control register */
#define    INT1     7
#define    INT0     6

#define    IVSEL    1
#define    IVCE     0
 
/* General Interrupt Flag Register */
#define    INTF1    7
#define    INTF0    6
 
/* Timer/Counter Interrupt MaSK register */
#define    OCIE2    7
#define    TOIE2    6
#define    TICIE1   5
#define    OCIE1A   4
#define    OCIE1B   3
#define    TOIE1    2

#define    TOIE0    0
 
/* Timer/Counter Interrupt Flag register */
#define    OCF2     7
#define    TOV2     6
#define    ICF1     5
#define    OCF1A    4
#define    OCF1B    3
#define    TOV1     2

#define    TOV0     0

/* SPM Control Register */			
#define    SPMIE    7
#define    RWWSB    6

#define    RWWSRE   4
#define    BLBSET   3
#define    PGWRT    2
#define    PGERS    1
#define    SPMEN    0

/* TWI Control Register */
#define    TWINT    7
#define    TWEA     6
#define    TWSTA    5
#define    TWSTO    4
#define    TWWC     3
#define    TWEN     2

#define    TWIE     0

/* MCU general Control Register */
#define    SE       7
#define    SM2      6
#define    SM1      5
#define    SM0      4
#define    ISC11    3
#define    ISC10    2
#define    ISC01    1
#define    ISC00    0

/* MCU general Control and Status Register */

#define    WDRF     3
#define    BORF     2
#define    EXTRF    1
#define    PORF     0
 
/* Timer/Counter 0 Control Register */
#define    CS02     2
#define    CS01     1
#define    CS00     0
  
/* Special Function I/O Register */		

#define    ADHSM    4
#define    ACME     3
#define    PUD      2
#define    PSR2     1
#define    PSR10    0

/* Timer/Counter 1A Control Register */
#define    COM1A1   7
#define    COM1A0   6
#define    COM1B1   5
#define    COM1B0   4
#define    FOC1A    3
#define    FOC1B    2
#define    WGM11    1
#define    WGM10    0
 
/* Timer/Counter 1B Control Register */
#define    ICNC1    7
#define    ICES1    6

#define    WGM13    4
#define    WGM12    3
#define    CS12     2
#define    CS11     1
#define    CS10     0
 
/* Timer/Counter 2 Control Register */
#define    FOC2     7
#define    WGM20    6
#define    COM21    5
#define    COM20    4
#define    WGM21    3
#define    CS22     2
#define    CS21     1
#define    CS20     0
 
/* Asynchronous mode Status Register */

#define    AS2      3
#define    TCN2UB   2
#define    OCR2UB   1
#define    TCR2UB   0
                        
/* Watchdog Timer Control Register */

#define    WDTOE    4
#define    WDCE     4
#define    WDE      3
#define    WDP2     2
#define    WDP1     1
#define    WDP0     0
 
/* UART UBRRH and UCSRC */
#define    URSEL    7
#define    UMSEL    6
#define    UPM1     5
#define    UPM0     4
#define    USBS     3
#define    UCSZ1    2
#define    UCSZ0    1
#define    UCPOL    0

/* EEPROM Control Register */

#define    EERIE    3
#define    EEMWE    2
#define    EEWE     1
#define    EERE     0
 
/* Data Register, Port B */
#define    PB7      7
#define    PB6      6
#define    PB5      5
#define    PB4      4
#define    PB3      3
#define    PB2      2
#define    PB1      1
#define    PB0      0
 
 /* Data Register, Port B */
#define    PORTB7   7
#define    PORTB6   6
#define    PORTB5   5
#define    PORTB4   4
#define    PORTB3   3
#define    PORTB2   2
#define    PORTB1   1
#define    PORTB0   0
 
/* Data Direction Register, Port B */
#define    DDB7     7
#define    DDB6     6
#define    DDB5     5
#define    DDB4     4
#define    DDB3     3
#define    DDB2     2
#define    DDB1     1
#define    DDB0     0
 
/* Input Pins, Port B */
#define    PINB7    7
#define    PINB6    6
#define    PINB5    5
#define    PINB4    4
#define    PINB3    3
#define    PINB2    2
#define    PINB1    1
#define    PINB0    0
 
/* Data Register, Port C */

#define    PC6      6
#define    PC5      5
#define    PC4      4
#define    PC3      3
#define    PC2      2
#define    PC1      1
#define    PC0      0
 
  /* Data Register, Port C */

#define    PORTC6   6
#define    PORTC5   5
#define    PORTC4   4
#define    PORTC3   3
#define    PORTC2   2
#define    PORTC1   1
#define    PORTC0   0
 
/* Data Direction Register, Port C */

#define    DDC6     6
#define    DDC5     5
#define    DDC4     4
#define    DDC3     3
#define    DDC2     2
#define    DDC1     1
#define    DDC0     0
 
/* Input Pins, Port C */

#define    PINC6    6
#define    PINC5    5
#define    PINC4    4
#define    PINC3    3
#define    PINC2    2
#define    PINC1    1
#define    PINC0    0
 
/* Data Register, Port D */
#define    PD7      7
#define    PD6      6
#define    PD5      5
#define    PD4      4
#define    PD3      3
#define    PD2      2
#define    PD1      1
#define    PD0      0
 
  /* Data Register, Port D */
#define    PORTD7   7
#define    PORTD6   6
#define    PORTD5   5
#define    PORTD4   4
#define    PORTD3   3
#define    PORTD2   2
#define    PORTD1   1
#define    PORTD0   0
 
/* Data Direction Register, Port D */
#define    DDD7     7
#define    DDD6     6
#define    DDD5     5
#define    DDD4     4
#define    DDD3     3
#define    DDD2     2
#define    DDD1     1
#define    DDD0     0
 
/* Input Pins, Port D */
#define    PIND7    7
#define    PIND6    6
#define    PIND5    5
#define    PIND4    4
#define    PIND3    3
#define    PIND2    2
#define    PIND1    1
#define    PIND0    0
  
/* SPI Status Register */
#define    SPIF     7
#define    WCOL     6





#define    SPI2X    0

/* SPI Control Register */
#define    SPIE     7
#define    SPE      6
#define    DORD     5
#define    MSTR     4
#define    CPOL     3
#define    CPHA     2
#define    SPR1     1
#define    SPR0     0

/* UART Control and Status Register A */
#define    RXC      7
#define    TXC      6
#define    UDRE     5
#define    FE       4
#define    DOR      3
#define    PE       2
#define    U2X      1
#define    MPCM     0
 
/* UART Control and Status Register B */
#define    RXCIE    7
#define    TXCIE    6
#define    UDRIE    5
#define    RXEN     4
#define    TXEN     3
#define    UCSZ2    2
#define    RXB8     1
#define    TXB8     0
 
/* Analog Comparator Control and Status Register */
#define    ACD      7
#define    ACBG     6
#define    ACO      5
#define    ACI      4
#define    ACIE     3
#define    ACIC     2
#define    ACIS1    1
#define    ACIS0    0
 
/* ADC MUX */
#define    REFS1    7
#define    REFS0    6
#define    ADLAR    5

#define    MUX3     3
#define    MUX2     2
#define    MUX1     1
#define    MUX0     0
 
/* ADC Control and Status Register */
#define    ADEN     7
#define    ADSC     6
#define    ADFR     5
#define    ADIF     4
#define    ADIE     3
#define    ADPS2    2
#define    ADPS1    1
#define    ADPS0    0
 
/* TWI Address Register */

#define    TWA6     7
#define    TWA5     6
#define    TWA4     5
#define    TWA3     4
#define    TWA2     3
#define    TWA1     2
#define    TWA0     1
#define    TWGCE    0

/* TWI status register */
#define    TWS7     7
#define    TWS6     6
#define    TWS5     5
#define    TWS4     4
#define    TWS3     3

#define    TWPS1    1
#define    TWPS0    0
 
/* Pointer definition   */
#define    XL     r26
#define    XH     r27
#define    YL     r28
#define    YH     r29
#define    ZL     r30
#define    ZH     r31
 
/* Constants */
#define    RAMEND   0x045F    /* Last On-Chip SRAM location */
#define    XRAMEND  0x045F
#define    E2END    0x01FF
#define    FLASHEND 0x1FFF
 
#endif /* ENABLE_BIT_DEFINITIONS */ 
#endif /* __IOM8_H (define part) */

#pragma language=default

#endif /* __IOM8_H (SFR part) */
