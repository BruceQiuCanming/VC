#ifndef _DISPLAY_H_
#define _DISPLAY_H_


//	    -         a
//	   |  |     f   b
//    	-         g
//     |  |     e   c
//       -  .     d   h

#ifdef HEATBOX128_V1_0
	#define Q1_PORT	 PORTB
	#define Q1_DDR	 DDRB
	#define Q1_BIT	 0
	#define Q2_PORT	 PORTB
	#define Q2_DDR	 DDRB
	#define Q2_BIT	 1	 
	#define Q3_PORT	 PORTB
	#define Q3_DDR	 DDRB
	#define Q3_BIT	 2
	#define Q4_PORT	 PORTB
	#define Q4_DDR	 DDRB
	#define Q4_BIT	 3
#endif
#define PL_IN_PORT	 PORTD
#define PL_IN_DDR	 DDRD
#define PL_IN_BIT	 4

/*#define LED_PL_IN_PORT	 PORTD
#define LED_PL_IN_DDR	 DDRD
#define LED_PL_IN_BIT	 3
*/
#ifdef HEATBOX128_V1_0
	#define PL_IN_LOW()    		PL_IN_PORT &=~BIT(PL_IN_BIT)
	#define PL_IN_HIGH()   		PL_IN_PORT |= BIT(PL_IN_BIT)  	
//	#define LED_PL_IN_LOW()    	LED_PL_IN_PORT &=~BIT(LED_PL_IN_BIT)
//	#define LED_PL_IN_HIGH()   	LED_PL_IN_PORT |= BIT(LED_PL_IN_BIT)  	
#endif

#define ALL_LED_OFF() Q1_PORT |= BIT(Q1_BIT);Q2_PORT |= BIT(Q2_BIT);Q3_PORT |= BIT(Q3_BIT);Q4_PORT |= BIT(Q4_BIT)

#define ALL_LED_ON()  Q1_PORT &=~BIT(Q1_BIT);Q2_PORT &=~BIT(Q2_BIT);Q3_PORT &=~BIT(Q3_BIT);Q4_PORT&=~BIT(Q4_BIT)

#ifdef HEATBOX128_V1_0
	#define KEY1_PORT	PORTC
	#define KEY1_PIN	PINC
	#define KEY1_DDR	DDRC
	#define KEY1_BIT	0
	#define KEY2_PORT	PORTC
	#define KEY2_PIN	PINC
	#define KEY2_DDR	DDRC
	#define KEY2_BIT	1
	#define KEY3_PORT	PORTC
	#define KEY3_PIN	PINC
	#define KEY3_DDR	DDRC
	#define KEY3_BIT	2
	#define KEY1_DOWN()  ((KEY1_PIN & BIT(KEY1_BIT)) == 0)
	#define KEY2_DOWN()  ((KEY2_PIN & BIT(KEY2_BIT)) == 0)
	#define KEY3_DOWN()  ((KEY3_PIN & BIT(KEY3_BIT)) == 0)
#endif

enum KEY_VALUE{KEY_NONE=0,KEY_F=0x04,KEY_UP=0x01,KEY_LR=0x02};
#define OVER_HIGH	1
#define OVER_LOW	2  

#define MAX_LED_ID 	3

#define LED_TEST_PORT	PORTD
#define LED_TEST_DDR	DDRD
#define LED_TEST_BIT	6

#define LED_ALARM_PORT	PORTD
#define LED_ALARM_DDR	DDRD
#define LED_ALARM_BIT	6

#define LED_AT_PORT		PORTD
#define LED_AT_DDR		DDRD
#define LED_AT_BIT		7
//467

#define LED_TEST_OFF()    	LED_TEST_PORT |= BIT(LED_TEST_BIT)
#define LED_TEST_ON() 		LED_TEST_PORT &=~BIT(LED_TEST_BIT)

#define LED_ALARM_OFF()   	LED_ALARM_PORT |= BIT(LED_ALARM_BIT)
#define LED_ALARM_ON() 		LED_ALARM_PORT &=~BIT(LED_ALARM_BIT)

#define LED_AT_OFF()    LED_AT_PORT |= BIT(LED_AT_BIT)
#define LED_AT_ON() 	LED_AT_PORT &=~BIT(LED_AT_BIT)

extern unsigned char leftright;
extern unsigned char G_cLedBuf[16];
extern unsigned char LedFlash;
extern unsigned char MinusFlag;

extern unsigned char G_cSwitchStatus[(MAX_LED_NR+7)/8];
//extern unsigned char G_cSwitchStatus_cur[16];    //上次输出的开关状态
extern unsigned char G_cSwitchOpenFlash[(MAX_LED_NR+7)/8];    		//闪动标志
extern unsigned char G_cSwitchCloseFlash[(MAX_LED_NR+7)/8];    		//闪动标志

extern unsigned char G_cSwitchNormalOpen;  // = 0 常闭, = 1常开

void WriteOver(char flag);
void WriteOverHigh(void);
void WriteOverLow(void);
void LedTest(void);
void Display(void);
void SelectLed(void);
void SelfDisplayCheck(void);

void DisplayInit(void);
void DisplaySwitchStatus(void);
//unsigned char CheckSwitchFlash(unsigned char switch_last_status, unsigned char switch_cur_status, unsigned char switch_flash );

#endif