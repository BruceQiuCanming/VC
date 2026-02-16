#ifndef _TM1638_H_
#define _TM1638_H_

	#define LEDA 0x01
	#define LEDB 0x02
	#define LEDC 0x04
	#define LEDD 0x08
	#define LEDE 0x10
	#define LEDF 0x20
	#define LEDG 0x40
	#define LEDH 0x80

//#define LED_OVERFLOW 	0xFF-LEDF
//#define LED_NULL		0xFF

//	    -         a
//	   |  |     f   b
//    	-         g
//     |  |     e   c
//       -  .     d   h


#define LED_0	0x3F
#define LED_1	0x06
#define LED_2	0x5B
#define LED_3	0x4F
#define LED_4	0x66
#define LED_5	0x6D
#define LED_6	0x7D
#define LED_7	0x07
#define LED_8	0x7F
#define LED_9	0x6F

//定义片选端口
#define U1_STB_PORT 	PORTB
#define U1_STB_DDR 		DDRB
#define U1_STB_BIT  	0
#define U1_STB_ON()		U1_STB_PORT |=	BIT(U1_STB_BIT)
#define U1_STB_OFF()	U1_STB_PORT &= ~BIT(U1_STB_BIT)


//定义CLK时钟数据输入端口
#define U1_CLK_PORT 	PORTB
#define U1_CLK_DDR 		DDRB
#define U1_CLK_BIT  	1
#define U1_CLK_ON()		U1_CLK_PORT |=	BIT(U1_CLK_BIT)
#define U1_CLK_OFF()	U1_CLK_PORT &= ~BIT(U1_CLK_BIT)

//定义DIO数据端口
#define U1_DIO_PORT 	PORTC
#define U1_DIO_PIN 		PINC
#define U1_DIO_DDR 		DDRC
#define U1_DIO_BIT  	2
#define U1_DIO_ON()		U1_DIO_PORT |=	BIT(U1_DIO_BIT)
#define U1_DIO_OFF()	U1_DIO_PORT &= ~BIT(U1_DIO_BIT)
#define U1_DIO_IN()		(U1_DIO_PIN  &  BIT(U1_DIO_BIT))


//定义片选端口
#define U2_STB_PORT 	PORTC
#define U2_STB_DDR 		DDRC
#define U2_STB_BIT  	6
#define U2_STB_ON()		U2_STB_PORT |=	BIT(U2_STB_BIT)
#define U2_STB_OFF()	U2_STB_PORT &= ~BIT(U2_STB_BIT)


//定义CLK时钟数据输入端口
#define U2_CLK_PORT 	PORTC
#define U2_CLK_DDR 		DDRC
#define U2_CLK_BIT  	5
#define U2_CLK_ON()		U2_CLK_PORT |=	BIT(U2_CLK_BIT)
#define U2_CLK_OFF()	U2_CLK_PORT &= ~BIT(U2_CLK_BIT)

//定义DIO数据端口
#define U2_DIO_PORT 	PORTC
#define U2_DIO_PIN 		PINC
#define U2_DIO_DDR 		DDRC
#define U2_DIO_BIT  	4
#define U2_DIO_ON()		U2_DIO_PORT |=	BIT(U2_DIO_BIT)
#define U2_DIO_OFF()	U2_DIO_PORT &= ~BIT(U2_DIO_BIT)
#define U2_DIO_IN()		(U2_DIO_PIN  &  BIT(U2_DIO_BIT))

//定义片选端口
#define U3_STB_PORT 	PORTB
#define U3_STB_DDR 		DDRB
#define U3_STB_BIT  	4
#define U3_STB_ON()		U3_STB_PORT |=	BIT(U3_STB_BIT)
#define U3_STB_OFF()	U3_STB_PORT &= ~BIT(U3_STB_BIT)


//定义CLK时钟数据输入端口
#define U3_CLK_PORT 	PORTB
#define U3_CLK_DDR 		DDRB
#define U3_CLK_BIT  	7
#define U3_CLK_ON()		U3_CLK_PORT |=	BIT(U3_CLK_BIT)
#define U3_CLK_OFF()	U3_CLK_PORT &= ~BIT(U3_CLK_BIT)

//定义DIO数据端口
#define U3_DIO_PORT 	PORTC
#define U3_DIO_PIN 		PINC
#define U3_DIO_DDR 		DDRC
#define U3_DIO_BIT  	7
#define U3_DIO_ON()		U3_DIO_PORT |=	BIT(U3_DIO_BIT)
#define U3_DIO_OFF()	U3_DIO_PORT &= ~BIT(U3_DIO_BIT)
#define U3_DIO_IN()		(U3_DIO_PIN  &  BIT(U3_DIO_BIT))


void TM1638_Display(unsigned char unit);
void TM1638_Port_Init(void);
char ReadKeyboard(void);
extern unsigned char TM1638_DisplayBuf[16];

extern unsigned char G_cLedDisplayBuf[9];

#endif