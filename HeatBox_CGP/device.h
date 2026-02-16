#ifndef _DEVICE_H_
#define _DEVICE_H_


#define RS485_RX_PORT	PORTD
#define RS485_RX_DDR	DDRD
#define RS485_RX_BIT	2


#define RS485_RX_OFF()  RS485_RX_PORT |= BIT(RS485_RX_BIT)
#define RS485_RX_ON() 	RS485_RX_PORT &=~BIT(RS485_RX_BIT)


#define DATA	PINA

#define ADDR0_PORT		PORTC
#define ADDR0_DDR		DDRC
#define ADDR0_BIT		1
#define ADDR0_ON()      ADDR0_PORT |= BIT(ADDR0_BIT)
#define ADDR0_OFF()     ADDR0_PORT &=~BIT(ADDR0_BIT)
		
#define ADDR1_PORT		PORTC
#define ADDR1_DDR		DDRC
#define ADDR1_BIT		0
#define ADDR1_ON()      ADDR1_PORT |= BIT(ADDR1_BIT)
#define ADDR1_OFF()     ADDR1_PORT &=~BIT(ADDR1_BIT)

#define ADDR2_PORT		PORTD
#define ADDR2_DDR		DDRD
#define ADDR2_BIT		7
#define ADDR2_ON()      ADDR2_PORT |= BIT(ADDR2_BIT)
#define ADDR2_OFF()     ADDR2_PORT &=~BIT(ADDR2_BIT)

#define ADDR3_PORT		PORTD
#define ADDR3_DDR		DDRD
#define ADDR3_BIT		6
#define ADDR3_ON()      ADDR3_PORT |= BIT(ADDR3_BIT)
#define ADDR3_OFF()     ADDR3_PORT &=~BIT(ADDR3_BIT)

#define ADDR_DIR_PORT	PORTC
#define ADDR_DIR_DDR	DDRC
#define ADDR_DIR_BIT	3
#define ADDR_DIR_ON()   ADDR_DIR_PORT |= BIT(ADDR_DIR_BIT)
#define ADDR_DIR_OFF()  ADDR_DIR_PORT &=~BIT(ADDR_DIR_BIT)


#define BTA_1_PORT	 	PORTD
#define BTA_1_DDR		DDRD
#define BTA_1_BIT	 	4
#define BTA_1_ON()		BTA_1_PORT &= ~BIT(BTA_1_BIT)
#define BTA_1_OFF()		BTA_1_PORT |=  BIT(BTA_1_BIT)

#define BTA_2_PORT	 	PORTD
#define BTA_2_DDR		DDRD
#define BTA_2_BIT	 	5
#define BTA_2_ON()		BTA_2_PORT &= ~BIT(BTA_2_BIT)
#define BTA_2_OFF()		BTA_2_PORT |=  BIT(BTA_2_BIT)

#define BTA_HEAT_ON()		BTA_1_ON()
#define BTA_HEAT_OFF()		BTA_1_OFF()

#define BTA_COOL_ON()		BTA_2_ON()
#define BTA_COOL_OFF()		BTA_2_OFF()


#define RLY_CTL_PORT	PORTD
#define RLY_CTL_DDR		DDRD
#define RLY_CTL_BIT		3
#define RLY_CTL_ON()   (RLY_CTL_PORT &= ~BIT(RLY_CTL_BIT))
#define RLY_CTL_OFF()  (RLY_CTL_PORT |=  BIT(RLY_CTL_BIT))


#define AD_SCK_PORT	 	PORTC
#define AD_SCK_DDR		DDRC
#define AD_SCK_BIT	 	3
#define AD_SCK_OFF()	AD_SCK_PORT &= ~BIT(AD_SCK_BIT)
#define AD_SCK_ON()	    AD_SCK_PORT |=  BIT(AD_SCK_BIT)

#define AD_DO_PORT	 	PORTC
#define AD_DO_PIN	 	PINC
#define AD_DO_DDR		DDRC
#define AD_DO_BIT	 	2
#define AD_DO_ON()		((AD_DO_PIN  & BIT(AD_DO_BIT)) == BIT(AD_DO_BIT))
#define AD_DO_OFF()		((AD_DO_PIN  & BIT(AD_DO_BIT)) == 0)

#define AC_CTL_PORT		PORTB
#define AC_CTL_DDR		DDRB
#define AC_CTL_BIT	 	5
#define AC_CTL_ON()		AC_CTL_PORT &= ~BIT(AC_CTL_BIT)
#define AC_CTL_OFF()	AC_CTL_PORT |=  BIT(AC_CTL_BIT)

/*
#define AC1_CTL_PORT	PORTC
#define AC1_CTL_DDR		DDRC
#define AC1_CTL_BIT	 	3
#define AC1_CTL_ON()	AC1_CTL_PORT &= ~BIT(AC1_CTL_BIT)
#define AC1_CTL_OFF()	AC1_CTL_PORT |=  BIT(AC1_CTL_BIT)

#define AC1_OUT_PORT	 	PORTC
#define AC1_OUT_PIN	 	PINC
#define AC1_OUT_DDR		DDRC
#define AC1_OUT_BIT	 	2
#define AC1_OUT_ON()		((AC1_OUT_PIN  & BIT(AC1_OUT_BIT)) == BIT(AC1_OUT_BIT))
#define AC1_OUT_OFF()		((AC1_OUT_PIN  & BIT(AC1_OUT_BIT)) == 0)
*/
extern int G_cSumTimer;
extern unsigned char one_second_flag;
extern unsigned char one_hundred_ms_flag;

extern unsigned char G_cSwitchFlashSpan;
extern char G_cAutoTest;	
extern long	G_iCurTemp;
extern int  newBTA;
extern char CalAdFlag;
extern char G_cDisplayType;

void init_devices(void);
void uart0_init(void);
char AnlysizeRs485Data(void);
void DealSend(void);
void copy4Int2SendBuf(int *source);
void copyRecBuf2FourInt(int* dest);
void memncpy(char *dest, const char *src, char size);
void watchdog_init(void);
void CalAcResult(void);
char SendChar(unsigned char data);
void ReadSwitchStatus(void);
unsigned int  ReadAdResult(void);
void DealSend_Crc(void);
void DealSend_Crc_Init(void);
void delay(unsigned int i);
long GetTemp(unsigned int val);

#endif