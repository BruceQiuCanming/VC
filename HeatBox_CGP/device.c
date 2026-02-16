//ICC-AVR application builder : 2011-3-31 下午 08:40:35
// Target : M16
// Crystal: 8.0000Mhz

#include <iom16v.h>
#include <macros.h>
#include "typedefs.h"
#include "device.h"
#include "pid.h"
#include "display.h"
#include "main.h"
#include "crc16.h"

#define MAX_AC_CYCLE	60000

//#pragma abs_address:AUTHOR_ADDRESS
const char AUTHOR[16] = {"3201026910302834"};
//#pragma end_abs_address

unsigned char yyyymm_sn[9];
unsigned char  G_cNo485Data;

#pragma abs_address:OSCCAL_ADDRESS
//const char OSCCAL_VAL[3] = {'O','S','C'};
#pragma end_abs_address

#pragma abs_address:SN_ADDRESS
const char SN[8] = {2,0,1,2,0,0,0,0};
#pragma end_abs_address

//char PowerOn = 1;

char display_flag = 1;
char SwitchChangFlag;
char G_cDisplayType;

long	G_iCurTemp;

int newBTA;

union
{
	MODBUS_RS485_CMD Com_Cmd;
	char buf[sizeof(MODBUS_RS485_CMD)+1];	  
}Com_InBuf;	
MODBUS_RS485_ANSWER  Com_Answer;

unsigned char   Com_InBuf_Index = 0;                  

char G_cAutoTest = 0;

#define AD_RESOLUTION	MAX_AC_CYCLE
#define PT100_R_(temp) (100.0f*(1+3.90802f*0.001f*(temp)-0.580195f*0.000001f*(temp)*(temp))-4.27350*0.000000000001*(temp-100)*(temp)*(temp)*(temp))

#define PT100_R(temp) (100.0f*(1+3.90802f*0.001f*(temp)-0.580195f*0.000001f*(temp)*(temp)))


#ifdef USED_R10
#define  RES_2
#ifdef RES_1
	#define	R6			 15000.0f
	#define	R7			160000.0f
	#define	GAIN		(R7/R6)
	#define	R1R3		1000.0f
	#define R2			82.0f
	#define R11			160000.0f
	#define R12			160000.0f
#endif

#ifdef RES_2
	#define	R6			 7500.0f
	#define	R7			130000.0f
	#define	GAIN		(R7/R6)
	#define	R1R3		2000.0f
	#define R2			82.0f
//	#define R11			160000.0f
//	#define R12			160000.0f
#endif







	

#define Vbridge		2.5f
#define Vad			2.5f
//(2.5-Vo)=GAIN*(PT_R100/(PT_R100+R1R3)-R82/(R2+R1R3))*2.5f
//AD_VALUE=(2.5f-GAIN*(PT_R100/(PT_R100+R1R3)-R82/(R2+R1R3))*2.5f)/5.0f*AD_RESOLUTION;

#define R1R3_AND_R6R7	(1.0f/(1.0f/R1R3+1.0f/(R6+R7)))
#define Vin_1			(Vbridge*R2/(R2+R1R3_AND_R6R7))
#define Vop_P			((Vbridge-Vin_1)*(R6/(R6+R7))+Vin_1)
#define	Vop_N			Vop_P

#define Vin_2_(temp)			((Vbridge/R1R3+Vop_N/R6)/(1/R1R3+1/PT100_R_(temp)+1/R6))
#define Vin_2(temp)			((Vbridge/R1R3+Vop_N/R6)/(1/R1R3+1/PT100_R(temp)+1/R6))
#define Vo_(temp)			(((Vop_N-Vin_2_(temp))/R6*R7+Vop_N))
#define Vo(temp)			(((Vop_N-Vin_2(temp))/R6*R7+Vop_N))

#else
//only used R6
#define R2		   270.0f
#define R1R3	  1000.0f
#define R6	      2000.0f
#define	R7		  7500.0f
	
#define AD_RESOLUTION	MAX_AC_CYCLE
#define Vbridge		2.5f
#define Vad			2.5f

#define Vop_P		(float)(Vbridge * R2 / (R2+R1R3))
#define	Vop_N		(float)Vop_P

#define Vin_2_(temp)		((Vop_N / R6  + Vbridge /R1R3) / ( 1.0f/R6 + 1.0f / R1R3 + 1.0f /  PT100_R_(temp)))
#define Vin_2(temp)			((Vop_N / R6  + Vbridge /R1R3) / ( 1.0f/R6 + 1.0f / R1R3 + 1.0f /  PT100_R(temp)))
//(Vop_N - Vin_2(temp)) / R6 + (Vbridge - Vin_2(temp)) / R1R3  - Vin_2(temp) / PT100_R(temp) = 0;
//Vop_N / R6 - Vin_2(temp) / R6 + Vbridge /R1R3 - Vin_2(temp) / R1R3 - Vin_2(temp) / PT100_R(temp) = 0;
//Vop_N / R6  + Vbridge /R1R3 = Vin_2(temp)/R6 + Vin_2(temp) / R1R3 + Vin_2(temp) / PT100_R(temp)
//Vin_2(temp) = (Vop_N / R6  + Vbridge /R1R3) / ( 1/R6 + 1 / R1R3 + 1 /  PT100_R(temp))
  
#define Vo_(temp)			(((Vop_N-Vin_2_(temp))/R6*R7+Vop_N))
#define Vo(temp)			(((Vop_N-Vin_2(temp))/R6*R7+Vop_N))

#endif


#define STD_VAL_MIN_TEMP	-50
#define STD_VAL_MAX_TEMP	450

#define LOW_TEMP	0
#define	HIGH_TEMP	300

#define STD_AD_VALUE_MAX	(unsigned int)(Vo_(-50.0f)*AD_RESOLUTION/Vad)
#define STD_AD_VALUE_MIN	(unsigned int)(Vo(250.0f)*AD_RESOLUTION/Vad)
const unsigned int STD_AD_VALUE[(STD_VAL_MAX_TEMP-STD_VAL_MIN_TEMP)/10+1]=
{
 Vo_(-50.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R_30/(PT_R_30+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo_(-40.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R_20/(PT_R_20+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo_(-30.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R_30/(PT_R_30+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo_(-20.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R_20/(PT_R_20+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo_(-10.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R_10/(PT_R_10+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
//000
 Vo(000.0f)*AD_RESOLUTION/Vad,
 Vo(010.0f)*AD_RESOLUTION/Vad,
 Vo(020.0f)*AD_RESOLUTION/Vad,
 Vo(030.0f)*AD_RESOLUTION/Vad,
 Vo(040.0f)*AD_RESOLUTION/Vad,
 Vo(050.0f)*AD_RESOLUTION/Vad,
 Vo(060.0f)*AD_RESOLUTION/Vad,
 Vo(070.0f)*AD_RESOLUTION/Vad,
 Vo(080.0f)*AD_RESOLUTION/Vad,
 Vo(090.0f)*AD_RESOLUTION/Vad,
 //(00 .0f)
 Vo(100.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R100/(PT_R100+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(110.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R110/(PT_R110+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(120.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R120/(PT_R120+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(130.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R130/(PT_R130+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(140.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R140/(PT_R140+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(150.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R150/(PT_R150+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(160.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R160/(PT_R160+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(170.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R170/(PT_R170+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(180.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R180/(PT_R180+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(190.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R190/(PT_R190+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 //(00 .0f)
 Vo(200.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R200/(PT_R200+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(210.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R210/(PT_R210+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(220.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R220/(PT_R220+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(230.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R230/(PT_R230+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(240.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R240/(PT_R240+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(250.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R250/(PT_R250+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(260.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R260/(PT_R260+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(270.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R270/(PT_R270+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(280.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R280/(PT_R280+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(290.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R290/(PT_R290+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 //300
 Vo(300.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R200/(PT_R200+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(310.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R210/(PT_R210+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(320.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R220/(PT_R220+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(330.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R230/(PT_R230+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(340.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R240/(PT_R240+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(350.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R250/(PT_R250+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(360.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R260/(PT_R260+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(370.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R270/(PT_R270+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(380.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R280/(PT_R280+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(390.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R290/(PT_R290+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,

//400
 Vo(400.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R200/(PT_R200+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(410.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R210/(PT_R210+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(420.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R220/(PT_R220+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(430.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R230/(PT_R230+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(440.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R240/(PT_R240+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,
 Vo(450.0f)*AD_RESOLUTION/Vad,//(Vbridge-GAIN*(PT_R250/(PT_R250+R1R3)-R2/(R2+R1R3))*Vbridge)/Vad*AD_RESOLUTION,

 

};



void port_init(void)
{
 PORTA = 0x00;
 DDRA  = 0x00;
 PORTB = 0x00;
 DDRB  = 0x00;
 PORTC = 0x00; //m103 output only
 DDRC  = 0x00;
 PORTD = 0x00;
 DDRD  = 0x00;
 
 
 ADDR_DIR_DDR |= BIT(ADDR_DIR_BIT);
 
 ADDR0_DDR 	|= BIT(ADDR0_BIT);
 ADDR1_DDR 	|= BIT(ADDR1_BIT);
 ADDR2_DDR 	|= BIT(ADDR2_BIT);
 ADDR3_DDR 	|= BIT(ADDR3_BIT);
 
 
 RS485_RX_DDR   |= BIT(RS485_RX_BIT);
 
	
	BTA_1_DDR |= BIT(BTA_1_BIT);
	BTA_2_DDR |= BIT(BTA_2_BIT);

	RLY_CTL_DDR |= BIT(RLY_CTL_BIT);
	RLY_CTL_OFF();

	AD_SCK_DDR  |= BIT(AD_SCK_BIT);
	AC_CTL_DDR  |= BIT(AC_CTL_BIT);
	
	BTA_1_OFF();
	BTA_2_OFF();
//	AD_DO_PORT  |= BIT(AD_DO_BIT);
	 			
}

//SPI initialize
// clock rate: 2000000hz
void spi_init(void)
{
 //SPCR = 0x51; //setup SPI
 //SPSR = 0x00; //setup SPI
#ifdef HEATBOX128_V1_0 
 	SPCR = (1<<SPE)|(1<<MSTR) |(1<<DORD) | (1<<SPR1) |(1<<SPR0);
#endif
	
	SPSR = 0x00;
}

//TIMER0 initialize - prescale:1
// WGM: Normal
// desired value: 64KHz /16M
// actual value: 64.000KHz (0.0%)
void timer0_init(void)
{
 TCCR0 = 0x00; //stop
 TCNT0 = 0x06; //set count
 OCR0  = 0xFA;  //set compare
 TCCR0 = 0x01; //start timer
}

//Comparator initialize
// trigger on: Output toggle
void comparator_init(void)
{
 	ACSR = ACSR & 0xF7; //ensure interrupt is off before changing
 	ACSR = 0x00;
}


unsigned int 		G_on_times;
unsigned int		G_ad_times;
unsigned char		CalAdFlag;

void StartAC(void)
{
	WDR();
	
	G_ad_times = MAX_AC_CYCLE;
	G_on_times	= 0;
	CLI();
	CalAdFlag = 0;	
	SEI();

}

unsigned char one_second_flag=1;//0;
unsigned char one_hundred_ms_flag=1;//0;

unsigned char ReadKeyFlag=1;//0
unsigned char CalAdFlag=0;//1;
//static unsigned char timer_16uS=125+1;
static unsigned char ten_ms=5;
static unsigned char one_hundred_ms=10;
static unsigned char one_seconds=10;
static unsigned char timer2ms;

#pragma interrupt_handler timer0_ovf_isr:10
void timer0_ovf_isr(void)
{
  //1/32ms
	

	static unsigned char reg_timer_64KHz;//=timer_16uS;
	unsigned char t;
	//int ad_times_loc;
	t=TCNT0;
 	TCNT0 = t +0x06+3;//reload  counter value
 	//reg_timer_16uS=timer_16uS;
	
	

	
	//t=TCNT0;
	//if(t!=183)
	//	t++;
	
	//G_cLedBuf[0][2] ++;
		if(t==17)
		{
			_NOP();
			_NOP();
			_NOP();
			//_NOP();
		}
		if(t==18)
		{
			_NOP();
			_NOP();
			//_NOP();
		}
		if(t==19)
		{
			_NOP();
		//	_NOP();
		}
		
		if((ACSR & BIT(ACO)))
		{
			_NOP();
			AC_CTL_ON();
		}
		else
		{
			AC_CTL_OFF();
			if(CalAdFlag == 0)
			{
        		G_on_times++;
        	}
		  	 
		}
		G_ad_times--;
		if(G_ad_times==0)
		{
			CalAdFlag=1;
		}


 	
 	//timer_16uS--;
 	reg_timer_64KHz++;
 	if(reg_timer_64KHz == 64*2)//2ms
 	{//2ms
 		reg_timer_64KHz = 0;
 		ten_ms--;
 		if(ten_ms==0)
 		{//10ms
 			ten_ms=5;
 			
 			G_Timer.BTA_T++;
 			//ReadKeyFlag = 1;
	
 			//set read key flag
	 		one_hundred_ms--;
	 		if(one_hundred_ms==0)
	 		{//100ms
	 			display_flag = 1;
	 			one_hundred_ms=10;
	 			one_seconds--;
	 			one_hundred_ms_flag ++;
				if(one_seconds == 0)
			 	{//1 second
			 		one_seconds = 10;
			 		one_second_flag = 1;
			 		G_cNo485Data ++;
			 		if(G_cNo485Data > 30 && G_cMode == NORMAL_MODE)
			 		{
			 		//	while(1);
			 		}
			 	} 			
	 		}
 		}
 	}
}


//call this routine to initialize all peripherals
void init_devices(void)
{
 //stop errant interrupts until set up
 CLI(); //disable all interrupts
 port_init();
 timer0_init();
// spi_init();
 uart0_init();
 watchdog_init();
 
 comparator_init();
 
 BTA_HEAT_OFF();
 
 
 MCUCR = 0x00;
 GICR  = 0x00;
 TIMSK = 0x01; //timer interrupt sources
 SEI(); //re-enable interrupts
 //all peripherals are now initialized
}

void ReadRx(void)
{
 //uart has received a character in UDR
    char i;
    
 	if (!(UCSRA & (1<<RXC)) )
 	{
 		return;
 	}
 		
 	
	//if(PowerOn == 0)
	{
		
		if(Com_InBuf_Index < sizeof(MODBUS_RS485_CMD))
		{
			Com_InBuf.buf[Com_InBuf_Index] = UDR;	
			Com_InBuf_Index ++;
		}
		else if(Com_InBuf_Index == sizeof(MODBUS_RS485_CMD))
		{
			for(i=0; i < sizeof(MODBUS_RS485_CMD) - 1 ; i++)
			{
				Com_InBuf.buf[i] = Com_InBuf.buf[i+1];	
			} 
			Com_InBuf.buf[sizeof(MODBUS_RS485_CMD)-1] = UDR;	
		}
		else
		{
			Com_InBuf_Index = sizeof(MODBUS_RS485_CMD);
		}
	}
}


//UART0 initialize
// desired baud rate: 9600
// actual: baud rate:9615 (0.2%)
// char size: 8 bit
// parity: Disabled
void uart0_init(void)
{
 UCSRB = 0x00; //disable while setting baud rate
 UCSRA = 0x00;
 UCSRC = BIT(URSEL) | 0x06;
 UBRRL = 0x67; //set baud rate lo
 UBRRH = 0x00; //set baud rate hi
 UCSRB = 0x18;
}

char SendChar(unsigned char data)
{

		if ( !( UCSRA & (1<<UDRE)) )
		{
			return 0;
		}

    	UDR = data;

		return 1;

}



#ifdef MODBUS
char Check_Com_InBuf_Crc(void)
{ 
	unsigned char crc[2];
	
		crc[0] = CRC_START_VAL_0;
		crc[1] = CRC_START_VAL_1;
		CRC16(&Com_InBuf.Com_Cmd.RS485_Addr, sizeof(MODBUS_RS485_CMD) - 2,crc, crc);
		if((crc[0] == Com_InBuf.Com_Cmd.crc[0])
			&&	(crc[1] == Com_InBuf.Com_Cmd.crc[1]))
		//if((0xEC == Com_InBuf.Com_Cmd.crc[0])
		//	&&	(0xEC == Com_InBuf.Com_Cmd.crc[1]))
		
		{
			return 1;
		}
		return 0;	
}

void ZeroComAnswer(void)
{
	char i;
	unsigned char *p = (unsigned char *)&Com_Answer;
	for(i = 0; i <  sizeof(Com_Answer); i++)
	{
		p[i] = 0;
	}
}

void ZeroComIn(void)
{
	char i;
	unsigned char *p = (unsigned char *)&Com_InBuf;
	for(i = 0; i <  sizeof(Com_InBuf); i++)
	{
		p[i] = 0;
	}
}

void delay(unsigned int i)
{
	while(i>0)
	{
		ReadRx();
		
		i--;
		WDR();
	}
}

extern char err;
char AnlysizeRs485Data(void)
{
	char i;
    int lasterror;	//=G_pid.LastError;
    int preverror;	//=G_pid.PrevError;
	unsigned char *p,*p1;
	MODBUS_RS485_READ_TEMP_ANSWER *pReadTempAnswer;
	MODBUS_RS485_READ_TEMP_CMD    *pReadTempCmd;
	long sumerror;

	if(Com_InBuf_Index == sizeof(MODBUS_RS485_CMD))
	{
		

		if(Com_InBuf.Com_Cmd.RS485_Addr == 	G_Para.RS485_id)
		{
			switch(Com_InBuf.Com_Cmd.cmd)
			{
				case CMD_READ_TEMP:
					if(Check_Com_InBuf_Crc()) 
					{//读取当前温度
						//#ifdef HIGH_VOLT
						// 附带的工作状态
						//para[0] : 是否在测试状态
						//para[1] : 测试阶段 CONTROL_STOP,CONTROL_DIRECT_HEAT,CONTROL_NORMAL_PID,CONTROL_INCREASE_PID
						//para[2] : 目标温度( 度）
						//para[3] : 目标温度( 度）
						//para[4] : 时间( 秒）
						//para[5] : 时间( 秒）
						/*
						pReadTempCmd = (MODBUS_RS485_READ_TEMP_CMD*)Com_InBuf.Com_Cmd.para;
						if(pReadTempCmd->TestMode == CONTROL_COOL_STEP_PID 
							|| 	pReadTempCmd->TestMode == CONTROL_COOL_PID)
						{
							G_cCoolSpeed = pReadTempCmd->CoolPercent;
						}
						*/	
						//#endif
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						pReadTempAnswer = (MODBUS_RS485_READ_TEMP_ANSWER*)Com_Answer.para;
						pReadTempAnswer->CurTemp = G_iCurTemp;
						pReadTempAnswer->CurSv   = G_HeatPid.SetPoint;
						pReadTempAnswer->HeatPercent = newBTA;
						pReadTempAnswer->CoolPercent = G_cCoolSpeed;		
								
						for(i = 0; i <  (MAX_LED_NR) / 8; i++)
						{
							pReadTempAnswer->SwitchStatus[i] = G_cSwitchStatus[i];
						}
						Com_InBuf_Index =0;
						ZeroComIn();
						DealSend_Crc_Init();
					}
					break;
					
				case CMD_SET_LOCK:
					
				case CMD_SET_UNLOCK:
					
				case CMD_AUTO_TEST_START:
   		   		//参数0，是否开始测试
   				//参数1，常闭常开
   				//参数2，闪动间隔
   					if(Check_Com_InBuf_Crc())
					{//
						G_cAutoTest = Com_InBuf.Com_Cmd.para[0] - '0';
						
						if(	G_cAutoTest != 0)
				   		{
					 	}
						else
						{
							G_cSubMode = CONTROL_STOP; //关闭加热
			
						}
					
						ZeroComAnswer();
						
						G_cSwitchNormalOpen = Com_InBuf.Com_Cmd.para[1] - '0';
						
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						
						Com_Answer.para[0] = G_cAutoTest;
						
						DealSend();
						
						Com_InBuf_Index =0;
						ZeroComIn();
					}
				break;
  				
  			case CMD_STOP_OUT: //关闭加热
  					if(Check_Com_InBuf_Crc())
					{//
					
						G_cSubMode = CONTROL_STOP;
						G_HeatPid.SetPoint = 0;
						//RLY_CTL_ON();
  						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						DealSend();
						Com_InBuf_Index =0;
						ZeroComIn();
					}
  					break;		
  					
  			case CMD_FULL_OUT:		
  					if(Check_Com_InBuf_Crc())
					{//
					
						G_cSubMode = CONTROL_DIRECT_HEAT;
						
						G_Para.Sv = *((int *)&Com_InBuf.Com_Cmd.para[0]);
						
						G_iTopTemp=G_Para.Sv*10L;
						G_HeatPid.SetPoint = G_iTopTemp;
						
						NewPID();
						PIDInit();
						
						
						
  						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						DealSend();
						Com_InBuf_Index =0;
						ZeroComIn();
					}
  				break;
  				
  			case CMD_NORMAL_PID_OUT:
  					if(Check_Com_InBuf_Crc())
					{//
					
						G_cSubMode = CONTROL_NORMAL_PID;
						
						G_iLastCoolTemp = G_iSumPidTemp; 
						G_Para.Sv = *((int *)&Com_InBuf.Com_Cmd.para[0]);
						//G_Para.Sv /= 10;
						G_iTopTemp=G_Para.Sv*10L;//0;
						G_HeatPid.SetPoint = G_iTopTemp;
						NewPID();
						PIDInit();
 		
  						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						DealSend();
						Com_InBuf_Index =0;
						ZeroComIn();
					}
  				break;
			
			case CMD_STEP_PID_OUT:
				if(Check_Com_InBuf_Crc())
					{//
							G_cSubMode = CONTROL_INCREASE_PID;
							
							
							G_Para.Sv = *((int *)&Com_InBuf.Com_Cmd.para[0]);
							//G_Para.Sv /= 10;
							
							
							
							G_iTopTemp=(long)(G_Para.Sv)* 10L;//100L;
							//传下来的秒
							G_Para.Pid_T = 1;
							G_iTopTime = (*((int *)&Com_InBuf.Com_Cmd.para[2])) /G_Para.Pid_T ;
							
							sumerror 	= 	G_HeatPid.SumError;
	      					
	      					lasterror	=	G_HeatPid.LastError;
	      					preverror	=	G_HeatPid.PrevError;
	      					
							NewPID();
							PIDInit();
	 		
	 						G_HeatPid.SumError  = 	sumerror;
	 						G_HeatPid.LastError =	lasterror;
	      					G_HeatPid.PrevError =	preverror;
      					
  						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						DealSend();
						
						Com_InBuf_Index =0;
						ZeroComIn();
						
						G_iLastCoolTemp = G_iSumPidTemp;
					}
				break;
				
				case CMD_COOL_FULL_OUT:
					if(Check_Com_InBuf_Crc())
					{//
						G_cSubMode = CONTROL_DIRECT_COOL;
						
						G_Para.Sv = *((int *)&Com_InBuf.Com_Cmd.para[0]);
						G_HeatPid.SetPoint = G_Para.Sv * 10L;
						//G_Para.Sv /= 10;
					
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						DealSend();
						
						Com_InBuf_Index =0;
						ZeroComIn();
					}
					break;
					
				case CMD_COOL_PID_OUT:
					if(Check_Com_InBuf_Crc())
					{//
						G_cSubMode = CONTROL_COOL_PID;
						
						
						G_Para.Sv = *((int *)&Com_InBuf.Com_Cmd.para[0]);
						
						G_HeatPid.SetPoint = G_Para.Sv * 10L;
						
						//G_Para.Sv /= 10;
					
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						DealSend();
						
						Com_InBuf_Index =0;
						ZeroComIn();
					}
					
					break;
					
				case CMD_COOL_STEP_PID_OUT:
					if(Check_Com_InBuf_Crc())
					{//
						G_cSubMode = CONTROL_COOL_STEP_PID;
						
						
						G_Para.Sv = *((int *)&Com_InBuf.Com_Cmd.para[0]);
						
						G_iTopTemp=(long)(G_Para.Sv)* 10L;
						G_HeatPid.SetPoint = G_iTopTemp;
						
						G_Para.Pid_T = 1;
						
						G_cCoolSpeed = Com_InBuf.Com_Cmd.para[2];
						G_cTestSpeed = G_cCoolSpeed;
						
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						DealSend();
						
						Com_InBuf_Index =0;
						ZeroComIn();
					}
				break;
					break;
							
				case CMD_READ_PID_T:
					if(Check_Com_InBuf_Crc())
					{//
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						for(i = 0; i < 8; i++)
						{
							Com_Answer.para[i] = ((char*)&G_Para.Pid_P)[i];
						}
						
						DealSend();
						
						Com_InBuf_Index =0;
						ZeroComIn();
					}
				break;
				
				case CMD_SET_PID_T:
					if(Check_Com_InBuf_Crc())
					{//
					
						for(i = 0; i < 8; i++)
						{
							((char*)&G_Para.Pid_P)[i] = Com_InBuf.Com_Cmd.para[i];
						}
						
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						DealSend();
						EnableEEPROMWrite();
						WritePara();
						EnableEEPROMWrite();
						WritePara_1();
						EnableEEPROMWrite();
						WritePara_2();
						EnableEEPROMWrite();
						WritePara_3();
						EnableEEPROMWrite();
						WritePara_4();
						EnableEEPROMWrite();
						WritePara_5();
						
						Com_InBuf_Index =0;
						ZeroComIn();
					}
				break;
				
				case CMD_SET_ADJUST_ALL:
					if(Check_Com_InBuf_Crc())
					{//
						for(i = 0; i < 2; i++)
						{
							((char*)&G_Para.AdjustAll)[i] = Com_InBuf.Com_Cmd.para[i];
						}
						
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						DealSend();
						if(G_Para.AdjustAll > -100 && G_Para.AdjustAll < 100)
						{
							EnableEEPROMWrite();
							WritePara();
							EnableEEPROMWrite();
							WritePara_1();
							EnableEEPROMWrite();
							WritePara_2();
							EnableEEPROMWrite();
							WritePara_3();
							EnableEEPROMWrite();
							WritePara_4();
							EnableEEPROMWrite();
							WritePara_5();
						}
						else
						{
							ReadPara();
						}
						Com_InBuf_Index =0;
						ZeroComIn();
						
					}
					break;
				case CMD_READ_ADJUST_ALL:
					if(Check_Com_InBuf_Crc())
					{//
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						for(i = 0; i < 2; i++)
						{
							Com_Answer.para[i] = ((char*)&G_Para.AdjustAll)[i];
						}
						DealSend();
						Com_InBuf_Index =0;
						ZeroComIn();
						
					}
					break;
				case CMD_SET_ADJUST_LOW:
					if(Check_Com_InBuf_Crc())
					{//
						for(i = 0; i < sizeof(G_Para.AdjustLow); i++)
						{
							((char*)&G_Para.AdjustLow)[i] = Com_InBuf.Com_Cmd.para[i];
						}
						
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						DealSend();
						if(G_Para.AdjustLow[0] > -200 && G_Para.AdjustLow[0] < 200)
						{
							EnableEEPROMWrite();
							WritePara();
							EnableEEPROMWrite();
							WritePara_1();
							EnableEEPROMWrite();
							WritePara_2();
							EnableEEPROMWrite();
							WritePara_3();
							EnableEEPROMWrite();
							WritePara_4();
							EnableEEPROMWrite();
							WritePara_5();
						}
						else
						{
							ReadPara();
						}
						Com_InBuf_Index =0;
						ZeroComIn();
						
					}
				break;
				case CMD_SET_ADJUST_HIGH:
					if(Check_Com_InBuf_Crc())
					{//
					
						for(i = 0; i <sizeof(G_Para.AdjustHigh); i++)
						{
							((char*)&G_Para.AdjustHigh)[i] = Com_InBuf.Com_Cmd.para[i];
						}
						
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						
						DealSend();
						if(G_Para.AdjustHigh[0] > -200 && G_Para.AdjustHigh[0] < 200)
						{
							EnableEEPROMWrite();
							WritePara();
							EnableEEPROMWrite();
							WritePara_1();
							EnableEEPROMWrite();
							WritePara_2();
							EnableEEPROMWrite();
							WritePara_3();
							EnableEEPROMWrite();
							WritePara_4();
							EnableEEPROMWrite();
							WritePara_5();
						}
						else
						{
							ReadPara();
						}
						
						Com_InBuf_Index =0;
						ZeroComIn();
					}
				break;
				
				case CMD_READ_ADJUST_LOW:
					if(Check_Com_InBuf_Crc())
					{//
					
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						for(i = 0; i < 8; i++)
						{
							Com_Answer.para[i] = ((char*)&G_Para.AdjustLow)[i];
						}
						
						DealSend();
						
						Com_InBuf_Index =0;
						ZeroComIn();
					}
				break;
				
				case CMD_READ_ADJUST_HIGH:
					if(Check_Com_InBuf_Crc())
					{//
					
						ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						for(i = 0; i < 8; i++)
						{
							Com_Answer.para[i] = ((char*)&G_Para.AdjustHigh)[i];
						}
						
						DealSend();
						Com_InBuf_Index =0;
						ZeroComIn();
					}
				break;
				
				case CMD_SET_RELAY:
					if(Check_Com_InBuf_Crc())
					{
						if(Com_InBuf.Com_Cmd.para[0] == 1)
				 		{
				 			RLY_CTL_ON();
				 		}
				 		else
				 		{
				 			RLY_CTL_OFF();
				 		}
				 		ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						Com_Answer.para[0] = Com_InBuf.Com_Cmd.para[0];
						
						DealSend();
						
						Com_InBuf_Index =0;
						ZeroComIn();
				 	}
					break;	
				case 	CMD_WRITE_STEP_PID_COUNT:
					if(Check_Com_InBuf_Crc())
					{
						G_Para.StepPidCount = *((unsigned int*)&Com_InBuf.Com_Cmd.para[0]);
				 		
				 		ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						Com_Answer.para[0] = Com_InBuf.Com_Cmd.para[0];
						Com_Answer.para[1] = Com_InBuf.Com_Cmd.para[1];
						DealSend();
						
						Com_InBuf_Index =0;
						ZeroComIn();
				 	}
					break;
				case 	CMD_READ_STEP_PID_COUNT:
					if(Check_Com_InBuf_Crc())
					{
						
				 		ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						Com_Answer.para[0] = G_Para.StepPidCount % 256;
						Com_Answer.para[1] = G_Para.StepPidCount / 256;
						
						DealSend();
						
						Com_InBuf_Index =0;
						ZeroComIn();
				 	}
					break;	
				case 	CMD_DISPLAY_TYPE:
					if(Check_Com_InBuf_Crc())
			 		{
			 			ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						for(i = 0; i < 16; i++)
						{
							Com_Answer.para[i] = 0;
						}
						G_cDisplayType = Com_InBuf.Com_Cmd.para[0];
						ZeroComIn();
			 			DealSend();
			 		}
					break;	
				
				default:
					if(Check_Com_InBuf_Crc())
					{
						Com_InBuf_Index =0;
						ZeroComIn();
				 	}
				
 	
			}
			
		}
		else if(Com_InBuf.Com_Cmd.RS485_Addr == (G_Para.RS485_id - '1' + 'A'))
		{
			switch(Com_InBuf.Com_Cmd.cmd)
			{
				case CMD_SET_RELAY_RESET:
					if(Check_Com_InBuf_Crc())
					{
						if(Com_InBuf.Com_Cmd.para[0] == 1)
				 		{
				 			RLY_CTL_ON();
				 		}
				 		else
				 		{
				 			RLY_CTL_OFF();
				 		}
				 		ZeroComAnswer();
						Com_Answer.RS485_Addr = Com_InBuf.Com_Cmd.RS485_Addr;
						Com_Answer.cmd = Com_InBuf.Com_Cmd.cmd;
						Com_Answer.para_len = sizeof(Com_Answer.para);
						Com_Answer.para[0] = Com_InBuf.Com_Cmd.para[0];
						
						DealSend();
						
						Com_InBuf_Index =0;
						ZeroComIn();
				 	}
					break;	 
			}
		}
	}
	return 0;
}

#else


#endif

char DealSend_Crc_Index = 0xFF;
char DealStatusSend_Crc_Index = 0xFF;
void DealSend_Crc_Init(void)
{
	Com_Answer.crc[0] = CRC_START_VAL_0;
	Com_Answer.crc[1] = CRC_START_VAL_1;
	CRC16((unsigned char *)&Com_Answer, sizeof(Com_Answer) - 4, Com_Answer.crc, Com_Answer.crc);
	DealSend_Crc_Index = 0;
	RS485_RX_OFF();
}

void DealSend_Crc(void)
{
	char i;
	char *p = (char*)&Com_Answer;
	
	if(DealSend_Crc_Index < sizeof(Com_Answer))
	{
		RS485_RX_OFF();
		if(SendChar(p[DealSend_Crc_Index]) != 0)
		{
			DealSend_Crc_Index ++;
		}
	}
	else if(DealSend_Crc_Index == sizeof(Com_Answer))
	{
		RS485_RX_OFF();
		//delay_us(2000);
		RS485_RX_ON();		
		Com_InBuf_Index = 0;
		DealSend_Crc_Index ++;
	}
		
}

void DealSend(void)
{
 	DealSend_Crc_Init();
}


void memncpy(char *dest, const char *src, char size)
{
	char i;
	for(i=0; i<size; i++)
	{
		dest[i] = src[i];
	}
}

//Watchdog initialize
// prescale: 512K
void watchdog_init(void)
{
 WDR(); //this prevents a timout on enabling
 WDTCR = 0x0D; //WATCHDOG ENABLED - dont forget to issue WDRs
}


unsigned int Filter(unsigned int sum)
{
	static unsigned int bak_sum = 0;
	unsigned int ret;
	
	ret = bak_sum;
	if(ret == 0)
		ret = sum;
	
	ret = sum/2+ret/2;
	
	bak_sum = ret;

	return ret;
}

long GetTemp(unsigned int val)
{//return value=xxx.xx
	char mid_pos;
	long int_temp;	//温度的整数值
	long decimal_temp;//x.xx has 2 digital after the decimal point
	long cur;
	//unsigned int delta;
	//static unsigned int v = 2000;
	//v+=(unsigned int)1000;
	//val = v;
	
	
	
	  for(mid_pos=0;mid_pos<sizeof(STD_AD_VALUE)/sizeof(unsigned int);mid_pos++)
      {
    	  	
      		if(val >= STD_AD_VALUE[mid_pos])
      			break;
      }
     
      int_temp=(mid_pos*10+STD_VAL_MIN_TEMP);

	//温度的小数部分0.xx
	//sum-(long)STD_AC_VALUE[mid_pos]的最大值是1000
	//STD_AC_VALUE[mid_pos-1]-STD_AC_VALUE[mid_pos]的最大值也是1000
	decimal_temp=(long)(val - STD_AD_VALUE[mid_pos])*1000L/(STD_AD_VALUE[mid_pos-1]-STD_AD_VALUE[mid_pos]);
	/*
	此算法有误差
	delta=(STD_AD_VALUE[mid_pos-1]-STD_AD_VALUE[mid_pos]);	
	decimal_temp=(20000+(int)delta/2)/(int)delta;
	delta=(val-STD_AD_VALUE[mid_pos]);
	decimal_temp=decimal_temp*(int)delta/20;
	*/
		
    cur=int_temp*100 - decimal_temp;
    	
    
	//cur+=((long)(G_Para.AdjustHigh[ch]-G_Para.AdjustLow[ch])*(long)cur)/HIGH_TEMP+G_Para.AdjustLow[ch]*100;
//	G_iSumPidTemp = cur;
//    G_iCurTemp = cur;
  	return cur;	
	
}

int CalAdjust(long cur_temp)//int low_adjust,int high_adjust)
{
		//6600/10*(0-100)/300+1000=22
	//int ret;
	#define ADJUST_TEMP	400
	//long l;//,m;
	int high,low,m;
	//int cur=cur_temp/10;
//	G_Para.AdjustHigh[ch]=141;
//	G_Para.AdjustLow[ch]=32;
	high = G_Para.AdjustHigh[0] * 10;
	low  = G_Para.AdjustLow[0] * 10;
	m    = high-low;
	
//	y = kx + delta
//  k = (2000 - (high + low)) / 2000
//  delta = low;
//  temp = cur_temp * k + low
//  temp = 

//	y = kx + delta
//  k = ((20000 - high) -(0 - low)) / 20000
//  delta = low;
//  temp = cur_temp * k + low
//  temp = 

    return ((long)cur_temp * (40000L + (high - low))  / 40000L + low)  - cur_temp + (int)G_Para.AdjustAll*10;

//	l=(long)(cur_temp+low)*m;
//	l=l/(long)((long)(ADJUST_TEMP*100)-m)+(long)(low);
	//l=(long)(cur_temp+low)*(long)m/(long)((long)(ADJUST_TEMP*100)-m)+(long)(low);
	
//	return ((long)(cur_temp+low)*(long)m)/((ADJUST_TEMP*100)-m)+(int)(low)+(int)G_Para.AdjustAll*10;
	


}

void CalAcResult(void)
{
	static int i =0;

	unsigned int sum;

	long cur;
	
	const char *pOSCCAL = (const char*)OSCCAL_ADDRESS;

	if( G_on_times >= STD_AD_VALUE[0])
	{
		G_iSumPidTemp = -600;
		DisplayTemp(&G_cLedBuf[0],-600);
		return ;
	}
	if( G_on_times <= STD_AD_VALUE[sizeof(STD_AD_VALUE) / sizeof(unsigned int) - 1])
	{
		G_iSumPidTemp = 5000;
		DisplayTemp(&G_cLedBuf[0],5000);
		return  ;
	}


	//sum=Filter(G_on_times);	
	cur = GetTemp(G_on_times);//sum);
	
	//DisplayTemp(&G_cLedBuf[0],(G_iSumPidTemp + 5)/ 10);
	if(*pOSCCAL == OSCCAL)
	{			
		cur+=CalAdjust(cur);
	}
	
	
	G_iCurTemp 		= cur;
	
	G_iSumPidTemp 	= G_iCurTemp;
	
	

	
	if(G_cMode != PID_AUTO_MODE && G_cMode != NORMAL_MODE)
		return;
	
	//For display,the tempeture has only one digital after the decimal point 
	//cur_temp=xxx.xx
	//cur/=10; 			
	//now cur_temp=xxx.x

	DisplayTemp(&G_cLedBuf[0],(G_iSumPidTemp + 5)/ 10);
	
}

void ChangSwitchStatus(char old_v1,char old_v2,char *new_v1, char *new_v2)
{
	char v1,v2;
	v1 = 0;
	v2 = 0;
	if(old_v1 & BIT(0))
		v1 += BIT(6);
	if(old_v1 & BIT(1))
		v1 += BIT(4);
	if(old_v1 & BIT(2))
		v1 += BIT(2);
	if(old_v1 & BIT(3))
		v1 += BIT(0);
	if(old_v1 & BIT(4))
		v2 += BIT(6);
	if(old_v1 & BIT(5))
		v2 += BIT(4);
	if(old_v1 & BIT(6))
		v2 += BIT(2);
	if(old_v1 & BIT(7))
		v2 += BIT(0);
	
	if(old_v2 & BIT(0))
		v1 += BIT(7);
	if(old_v2 & BIT(1))
		v1 += BIT(5);
	if(old_v2 & BIT(2))
		v1 += BIT(3);
	if(old_v2 & BIT(3))
		v1 += BIT(1);
	if(old_v2 & BIT(4))
		v2 += BIT(7);
	if(old_v2 & BIT(5))
		v2 += BIT(5);
	if(old_v2 & BIT(6))
		v2 += BIT(3);
	if(old_v2 & BIT(7))
		v2 += BIT(1);
	
	*new_v1 = v1;
	*new_v2 = v2;
}

void ReadSwitchStatus(void)
{
	static char ReadID = 0;
	static unsigned char cSwitchStatus[16];
	ADDR_DIR_ON();
	ReadID ++;
	if(ReadID > 15)
		ReadID = 0;
	switch(ReadID)
	{
		case 0:	
	//J18
	ADDR0_OFF();
	ADDR1_OFF();
	ADDR2_OFF();
	ADDR3_OFF();
	delay(100);
	//G_cSwitchStatus[0] = ~DATA;
	cSwitchStatus[1] = ~DATA;
		break;
	case 1:	
	//J16
	ADDR0_ON();
	ADDR1_OFF();
	ADDR2_OFF();
	ADDR3_OFF();
	delay(100);
	//G_cSwitchStatus[1] = ~DATA;
	cSwitchStatus[7] = ~DATA;
	break;
	case 2:
	//J12
	ADDR0_OFF();
	ADDR1_ON();
	ADDR2_OFF();
	ADDR3_OFF();
	delay(100);
	//G_cSwitchStatus[2] = ~DATA;
	cSwitchStatus[5] = ~DATA;
	break;
	case 3:
	//J19
	ADDR0_ON();
	ADDR1_ON();
	ADDR2_OFF();
	ADDR3_OFF();
	delay(100);
	//G_cSwitchStatus[3] = ~DATA;
	cSwitchStatus[2] = ~DATA;
	break;
	case 4:
	
	//J17
	ADDR0_OFF();
	ADDR1_OFF();
	ADDR2_ON();
	ADDR3_OFF();
	delay(100);
	//G_cSwitchStatus[4] = ~DATA;
	cSwitchStatus[0] = ~DATA;
	break;
	case 5:
	
	//J14
	ADDR0_ON();
	ADDR1_OFF();
	ADDR2_ON();
	ADDR3_OFF();
	delay(100);
	//G_cSwitchStatus[5] = ~DATA;
	cSwitchStatus[6] = ~DATA;
	break;
	
	case 6:
	//J10
	ADDR0_OFF();
	ADDR1_ON();
	ADDR2_ON();
	ADDR3_OFF();
	delay(100);
	//G_cSwitchStatus[6] = ~DATA;
	cSwitchStatus[4] = ~DATA;
	break;
	
	case 7:
	
	//J20
	ADDR0_ON();
	ADDR1_ON();
	ADDR2_ON();
	ADDR3_OFF();
	delay(100);
	//G_cSwitchStatus[7] = ~DATA;
	cSwitchStatus[3] = ~DATA;
	
	break;
	case 8:
	
	//J 11
	ADDR0_OFF();
	ADDR1_OFF();
	ADDR2_OFF();
	ADDR3_ON();
	delay(100);
	//G_cSwitchStatus[8] = ~DATA;
	cSwitchStatus[11] = ~DATA;
	break;
	case 9:
	
	//J 7
	ADDR0_ON();
	ADDR1_OFF();
	ADDR2_OFF();
	ADDR3_ON();
	delay(100);
	//G_cSwitchStatus[9] = ~DATA;
	cSwitchStatus[9] = ~DATA;

	break;
	case 10:
	
	//J 9
	ADDR0_OFF();
	ADDR1_ON();
	ADDR2_OFF();
	ADDR3_ON();
	delay(100);
	//G_cSwitchStatus[10] = ~DATA;
	cSwitchStatus[10] = ~DATA;

	break;
	case 11:
	
	//J5 
	ADDR0_ON();
	ADDR1_ON();
	ADDR2_OFF();
	ADDR3_ON();
	delay(100);
	//G_cSwitchStatus[11] = ~DATA;
	cSwitchStatus[8] = ~DATA;
	
	break;
	case 12:
	
	//J 6
	ADDR0_OFF();
	ADDR1_OFF();
	ADDR2_ON();
	ADDR3_ON();
	delay(100);
	//G_cSwitchStatus[12] = ~DATA;
	cSwitchStatus[14] = ~DATA;

	break;
	case 13:
	
	//J 8
	ADDR0_ON();
	ADDR1_OFF();
	ADDR2_ON();
	ADDR3_ON();
	delay(100);
	//G_cSwitchStatus[13] = ~DATA;
	cSwitchStatus[15] = ~DATA;
	
	break;
	case 14:
	
	//J 13
	ADDR0_OFF();
	ADDR1_ON();
	ADDR2_ON();
	ADDR3_ON();
	delay(100);
	//G_cSwitchStatus[14] = ~DATA;
	cSwitchStatus[12] = ~DATA;
	
	break;
	case 15:
	
	//J 15
	ADDR0_ON();
	ADDR1_ON();
	ADDR2_ON();
	ADDR3_ON();
	delay(100);
	//G_cSwitchStatus[15] = ~DATA;
	cSwitchStatus[13] = ~DATA;
	
	//(J17,J18)
	//J19,J20
	//J10,J12
	//J14,J16
	//J5,J7
	//J9,J11
	//J13,J15
	//J6,J8
	
	if(display_flag)
	{
		ChangSwitchStatus(cSwitchStatus[1],cSwitchStatus[0],&G_cSwitchStatus[0],&G_cSwitchStatus[1]);
		ChangSwitchStatus(cSwitchStatus[3],cSwitchStatus[2],&G_cSwitchStatus[2],&G_cSwitchStatus[3]);
		
		ChangSwitchStatus(cSwitchStatus[5],cSwitchStatus[4],&G_cSwitchStatus[4],&G_cSwitchStatus[5]);
		ChangSwitchStatus(cSwitchStatus[7],cSwitchStatus[6],&G_cSwitchStatus[6],&G_cSwitchStatus[7]);
		
		ChangSwitchStatus(cSwitchStatus[9],cSwitchStatus[8],&G_cSwitchStatus[8],&G_cSwitchStatus[9]);
		ChangSwitchStatus(cSwitchStatus[11],cSwitchStatus[10],&G_cSwitchStatus[10],&G_cSwitchStatus[11]);
		ChangSwitchStatus(cSwitchStatus[13],cSwitchStatus[12],&G_cSwitchStatus[12],&G_cSwitchStatus[13]);
		ChangSwitchStatus(cSwitchStatus[15],cSwitchStatus[14],&G_cSwitchStatus[14],&G_cSwitchStatus[15]);
	
		TM1638_Display(1);
		TM1638_Display(2);
		
		display_flag = 0;
	}
	break;
 }
}


unsigned int  ReadAdResult(void)
{
	long Count;
	unsigned char i;

	AD_DO_DDR  &=~BIT(AD_DO_BIT);
	AD_SCK_OFF();

	
	
	if(AD_DO_ON())
	{
		return 0;
	};
	
	Count = 0;

	CLI();
	delay(2);
	for (i=0;i<24;i++)
	{
		AD_SCK_ON();
		Count = Count * 2L;
		//delay(1);
		if(AD_DO_ON()) 
		{
			Count++;
		}
		AD_SCK_OFF();
		delay(2);
		
		
		
	}
	//Count += 5000;
	
	AD_SCK_ON();
//	Count=Count^0x800000;
	delay(2);
	AD_SCK_OFF();
	delay(2);

	delay(2);
	AD_SCK_OFF();
	delay(2);

	delay(2);
	AD_SCK_OFF();
	delay(2);
	
	SEI();
	
	//Count -= 0x800000; 
	
	return(Count >> 8);
}


