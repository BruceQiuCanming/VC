//ICC-AVR application builder : 2011-3-31 下午 08:40:35
// Target : M16
// Crystal: 8.0000Mhz

#include <iom16v.h>
#include <macros.h>
#include <eeprom.h>
#include "typedefs.h"
#include "main.h"
#include "display.h"
#include "pid.h"
#include "device.h"
#include "TM1638.h"

static unsigned char WriteEEPromFlag = 0;
unsigned char G_cMode;
unsigned char G_cSubMode = CONTROL_STOP;
signed long  G_iTopTemp;
signed int  G_iTopTime;
signed long  G_iLastCoolTemp;
char G_cTestSpeed;
char G_cCoolSpeed;
stEEPROM G_Para;



void EnableEEPROMWrite(void)
{
	WriteEEPromFlag = 0xA5;
}

void DisableEEPROMWrite(void)
{
	WriteEEPromFlag = 0x00;
}
unsigned char IsEnableEEPROMWrite(void)
{
	if(WriteEEPromFlag == 0xA5)
		return 0xA5;
	else
		return 0x00;	
}
 
//
void main(void)
{
	static char id;
	char cMode,cSubMode;
	char i,j;
	long Sv_bak;
	long ad_result;
	
			
	init_devices();
	
	DisplayInit();
	//SelfDisplayCheck();		
	
	SystemParaInit();
	
	SEI();
  
	
	//if((MCUSR & BIT(WDRF)) == 0x00)
	{
		RS485_RX_OFF();
		SendChar('Q');
				delay(2000);
		SendChar('C');
				delay(2000);
		SendChar('M');
				delay(2000);
		SendChar(OSCCAL);
				delay(2000);
			
		SendChar(MCUSR);
				delay(2000);
		
		RS485_RX_ON();
//		ShowPowerOnInfo( 0 );	
	}
	//else
	{
	//	ShowPowerOnInfo( 2 );	
	}
	RS485_RX_ON();
    
   TM1638_Port_Init();

   TM1638_Display(0);
   TM1638_Display(1);
   TM1638_Display(2);

   NewPID();
   PIDInit();
 

	
	
	while(1)
	{
	 	WDR();
	 	
	 	
	 	ReadRx();
		deal_RS485();
		DealSend_Crc();
		if(CalAdFlag)
		{
			CalAcResult();
		}
		
		ReadSwitchStatus();
		
		
		
	 	if(one_second_flag)
		{
			
			StartAC();
			
			
			one_second_flag=0;
			//ad_result = ReadAdResult();
			//if(ad_result != 0)
			{
				//GetTemp(ad_result);
				//DisplayTemp(&G_cLedBuf[0],GetTemp(ad_result)/10);
			}
			TM1638_Display(0);
		}
		

/*		
		if(CheckParaValid(&G_Para) == 0)
		{
			ReadPara();
		
			init_devices();
			
			DisplayInit();
			
			SystemParaInit();
			
			SEI();
		}
		
*/	 	 
	 	
 		
 		
				 		
	 	cMode = G_cMode;
	 	
	 	cSubMode = G_cSubMode;


	 	
		
		if(cMode==NORMAL_MODE)
		{//正常测控状态 
			if(cSubMode == CONTROL_INCREASE_PID)
			{
				if(Sv_bak != G_HeatPid.SetPoint)
				{
					Sv_bak=G_HeatPid.SetPoint;
		 			DisplayTemp(&G_cLedBuf[4],Sv_bak/10);
	 			}
	 		}		
		 	else
		 	{
		 		if(Sv_bak != G_Para.Sv)
		 		{
		 			Sv_bak=G_Para.Sv;
		 			DisplayTemp(&G_cLedBuf[4],Sv_bak);
		 		}
	 		}
	 		
	 		 
 			
	 		if(cSubMode == CONTROL_STOP)
		 	{
		 		BTA_HEAT_OFF();
		 		BTA_COOL_OFF();
		 		newBTA	= 0;
		 		G_Para.Sv = 0;
		 		G_cCoolSpeed = 0;
			}
			else if(cSubMode == CONTROL_DIRECT_HEAT)
			{//switch control
				G_cCoolSpeed = 0;
				BTA_COOL_OFF();
			 	if(G_iSumPidTemp >= G_iTopTemp)
			 	{
			 		//G_Para.Sv = G_iTopTemp / 10; //G_iTopTemp以0.1度为单位
			 		G_cSubMode = CONTROL_NORMAL_PID;
			 		NewPID();
					BTA_HEAT_OFF();
					newBTA = 0;
				}
				else
				{
					newBTA = 100;	
					BTA_HEAT_ON();	
				}
		 	}
		 	else if(cSubMode == CONTROL_NORMAL_PID || cSubMode==CONTROL_INCREASE_PID)	
		 	{//2=normal PID control,3=Step up control
		 		G_cCoolSpeed = 0;
		 		BTA_COOL_OFF();		
		 		//if(G_Timer.BTA_T <newBTA)
		 		if((G_Timer.BTA_T % 10) < (newBTA / 10) )
		 		{
					BTA_HEAT_ON();
				}
				else if((G_Timer.BTA_T % 10) == (newBTA / 10) )
				{
					if((newBTA % 10) > (G_Timer.BTA_T / 10))
					{
						BTA_HEAT_ON();
					}
					else
					{
						BTA_HEAT_OFF();
					}
				}	
				else
				{
					BTA_HEAT_OFF();
				}
				
				//增加2009-12-27 09:33上午
				if(G_iSumPidTemp >= G_iTopTemp && cSubMode == CONTROL_INCREASE_PID)//防止过加热
						BTA_HEAT_OFF();
				//2011-5-30 11:32上午
				//由于存在烘箱加热速度不够的情况
				//必须改成判断温度是否到达为结束条件
				//if(cSubMode == CONTROL_INCREASE_PID && G_iTopTime <= 0)
				if((cSubMode == CONTROL_INCREASE_PID) && (G_iSumPidTemp >= G_iTopTemp))
				{
					G_cSubMode = CONTROL_NORMAL_PID;
					BTA_HEAT_OFF();
				}
					
				if(G_Timer.MAX_BTA_T <= G_Timer.BTA_T)
			  	{//一个新的PID_T周期到
			  		 

		  			PIDCal(G_iSumPidTemp);//iPidTemp));
		  			 
			  		NewPID();
			  	}
			}
			else if(cSubMode == CONTROL_DIRECT_COOL)
			{
				G_cCoolSpeed = 100;
				newBTA = 0;
				BTA_HEAT_OFF();
				
				BTA_COOL_ON();
			}
			else if(cSubMode == CONTROL_COOL_STEP_PID)
			{
				newBTA = 0;
				BTA_HEAT_OFF();
				BTA_COOL_OFF();
				if(G_Timer.BTA_T < (G_cCoolSpeed * 5))
		 		{
					BTA_COOL_ON();
				}
				else 
				{
					BTA_COOL_OFF();
				}	
				
				if(G_Timer.MAX_BTA_T * 5 <= G_Timer.BTA_T )
			  	{//一个新的PID_T周期到
			  		NewPID();
			  		
			  		if(((G_iLastCoolTemp - G_iSumPidTemp ) * (60 / 5)) > (G_cTestSpeed * 10 ))
			  		{
			  			if(G_cCoolSpeed > 0)
			  			{
			  				G_cCoolSpeed --;
			  			}
			  		}
			  		else
			  		{
			  			if(G_cCoolSpeed < 100)
			  			{
			  				G_cCoolSpeed ++;
			  			}
			  		}
			  		G_iLastCoolTemp = G_iSumPidTemp;
			  	}
			}
			else if(cMode == CONTROL_COOL_PID)
			{//
				BTA_HEAT_OFF();	
			}
				
		}
		else if(cMode == PID_AUTO_MODE)
		{//Bang-Bang Control
			 	
		 	if(G_Timer.MAX_BTA_T <= G_Timer.BTA_T)
		  	{//一个新的PID_T周期到
		  		//if(G_lSumTimer!=0)
		  		//iPidTemp=(G_iSumPidTemp+((int)(G_cSumTimer)>>1))/(int)G_cSumTimer;
		  		//iPidTemp=G_iSumPidTemp;///G_cSumTimer;
		  		//else
		  		//	iPidTemp=G_lSumPidTemp;
//		  		RS485_RX_OFF();
//		  		SendChar(G_cSumTimer);
//		  		SendChar(G_iSumPidTemp);
//		  		SendChar(G_iSumPidTemp>>8);
//		  		SendChar((char)iPidTemp);
//		  		SendChar((char)(iPidTemp>>8));
//		  		
//		  		RS485_RX_ON();
		  		i = BangBangCtrol(G_iSumPidTemp);//iPidTemp);
	 			if(i == AT_FINISHED)
	 			{
	 				//Cal_P_I_D();
	 				while(1);
	 			}
	 			else if( i== AT_TIMEOUT)
	 			{
	 				G_cMode = PID_AUTO_TIMEOUT_MODE;
	 			}	
	 			NewPID();
		  		
		  	}
		}
		else if(cMode == PID_AUTO_TIMEOUT_MODE)
		{//
			G_cLedBuf[0] = 20+'A'-'A';
			G_cLedBuf[1] = 20+'O'-'A';
			G_cLedBuf[2] = 20+'U'-'A';
			G_cLedBuf[3] = 20+'T'-'A';
			BTA_HEAT_OFF();
		}
		else
		{
				BTA_HEAT_OFF();	
		}
	 }
}






void SetDefaultPara(stEEPROM *pPara)
{
	char i;
		for(i=0;i<sizeof(stEEPROM);i++)
		{
			((char*)pPara)[i]=0;
		}
//#ifndef RS485	 
	 	pPara->Sv =  0;//delta temperature = 0.5 degree  
//#endif	 
		pPara->StepPidCount = 0;
	 	pPara->Pid_P = 2;//20 degree
	 	pPara->Pid_I = 30;
	 	pPara->Pid_D = 10;
	 	pPara->Pid_T = 1;//60;
	 	pPara->AlarmHigh = 400;
	 	pPara->RS485_id = '1';
	 	pPara->Pid_Exceed = 0;
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

void ReadPara(void)
{
 	int i,j,k;
 	char OK = 0;
 	stEEPROM *pPara = &G_Para;
	
	for(j=0;j<6;j++)
	{
		for(i=0;i<sizeof(stEEPROM);i++)
		{
	  		WDR();
	  		k = i+EEPROM_START_ADDRESS + 0x40*j;
	  		((char*)pPara)[i] = EEPROMread( k );
		}
		
		if((pPara->iChkSum == CheckSum((char*)pPara,sizeof(stEEPROM)-sizeof(G_Para.iChkSum)))
			&& (CheckParaValid(pPara) != 0))
	 	{
			OK = 1;
			
			if(pPara->Sv != 0)
			{
				pPara->Sv = 0;
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
			break;
		}
	}
	
	if(OK == 0)
	{
		SetDefaultPara(pPara);
	}
 	
}

char CheckParaValid(stEEPROM *pPara)
{

	if(((pPara->RS485_id >='1') && (pPara->RS485_id <='9'))
	&& (pPara->Pid_P	<= 100)
	&& (pPara->Pid_I	<= 300)
	&& (pPara->Pid_D	<= 100)
	&& (pPara->Pid_T	>= 1 && pPara->Pid_T	<= 10)
	&& (pPara->AdjustLow[0] > -200 && pPara->AdjustLow[0] < 200)
	&& (pPara->AdjustHigh[0] > -200 && pPara->AdjustHigh[0] < 200))
   {
		return 1;
	}
	
	return 0;
	 
}
void WritePara(void)
{
 	 int i,j=0;
 	 stEEPROM *pPara = &G_Para;
	 
	 if(CheckParaValid(pPara) == 0)
	 {
	 	return;
	 }
 	 
 	 pPara->iChkSum = CheckSum((char*)pPara,sizeof(stEEPROM)-sizeof(G_Para.iChkSum));
 	 
 	 for(i=0;i<sizeof(stEEPROM);i++)
	 {
  		WDR();
  		if((EEPROMread(i+EEPROM_START_ADDRESS+j*0x40)!=((char*)pPara)[i]) && IsEnableEEPROMWrite())
  		{
  			EEPROMwrite(i+EEPROM_START_ADDRESS+j*0x40, ((char*)pPara)[i]);
  		}
  		 	
	 }
	
	 DisableEEPROMWrite();
	 
	 ReadPara();
}


void WritePara_1(void)
{
 	 int i,j=1;
 	 stEEPROM *pPara = &G_Para;
	 
	 if(CheckParaValid(pPara) == 0)
	 {
	 	return;
	 }
	 
 	 pPara->iChkSum = CheckSum((char*)pPara,sizeof(stEEPROM)-sizeof(G_Para.iChkSum));
 	 
	 for(i=0;i<sizeof(stEEPROM);i++)
		 {
	  		WDR();
	  		if((EEPROMread(i+EEPROM_START_ADDRESS+j*0x40)!=((char*)pPara)[i]) && IsEnableEEPROMWrite())
	  			EEPROMwrite(i+EEPROM_START_ADDRESS+j*0x40, ((char*)pPara)[i]);
	  		 	
		 }
	
	 DisableEEPROMWrite();
	 ReadPara();
}
void WritePara_2(void)
{
 	 int i,j=2;
 	 stEEPROM *pPara = &G_Para;
	 
	 if(CheckParaValid(pPara) == 0)
	 {
	 	return;
	 }
	 
 	 pPara->iChkSum = CheckSum((char*)pPara,sizeof(stEEPROM)-sizeof(G_Para.iChkSum));
 	 
	 for(i=0;i<sizeof(stEEPROM);i++)
	 {
	  		WDR();
	  		if((EEPROMread(i+EEPROM_START_ADDRESS+j*0x40)!=((char*)pPara)[i]) && IsEnableEEPROMWrite())
	  			EEPROMwrite(i+EEPROM_START_ADDRESS+j*0x40, ((char*)pPara)[i]);
	  		 	
	}
	
	 DisableEEPROMWrite();
	 ReadPara();
}
void WritePara_3(void)
{
 	 int i,j=3;
 	 stEEPROM *pPara = &G_Para;
	 
	 if(CheckParaValid(pPara) == 0)
	 {
	 	return;
	 }
	 
 	 pPara->iChkSum=CheckSum((char*)pPara,sizeof(stEEPROM)-sizeof(G_Para.iChkSum));
	 for(i=0;i<sizeof(stEEPROM);i++)
		 {
	  		WDR();
	  		if((EEPROMread(i+EEPROM_START_ADDRESS+j*0x40)!=((char*)pPara)[i]) && IsEnableEEPROMWrite())
	  			EEPROMwrite(i+EEPROM_START_ADDRESS+j*0x40, ((char*)pPara)[i]);
	  		 	
		 }
	
	 DisableEEPROMWrite();
	 ReadPara();
}
void WritePara_4(void)
{
/* 	 int i,j=4;
 	 stEEPROM *pPara = &G_Para;
	 
	 if(CheckParaValid(pPara) == 0)
	 {
	 	return;
	 }
	 
 	 pPara->iChkSum=CheckSum((char*)pPara,sizeof(stEEPROM)-sizeof(G_Para.iChkSum));
	 for(i=0;i<sizeof(stEEPROM);i++)
		 {
	  		WDR();
	  		if((EEPROMread(i+EEPROM_START_ADDRESS+j*0x40)!=((char*)pPara)[i]) && IsEnableEEPROMWrite())
	  			EEPROMwrite(i+EEPROM_START_ADDRESS+j*0x40, ((char*)pPara)[i]);
	  		 	
		 }
	
	 DisableEEPROMWrite();
	 ReadPara();
*/	 
}
void WritePara_5(void)
{
/* 	 int i,j=5;
 	 stEEPROM *pPara = &G_Para;
	 
	 if(CheckParaValid(pPara) == 0)
	 {
	 	return;
	 }
	 
 	 pPara->iChkSum=CheckSum((char*)pPara,sizeof(stEEPROM)-sizeof(G_Para.iChkSum));
	 for(i=0;i<sizeof(stEEPROM);i++)
		 {
	  		WDR();
	  		if((EEPROMread(i+EEPROM_START_ADDRESS+j*0x40)!=((char*)pPara)[i]) && IsEnableEEPROMWrite())
	  			EEPROMwrite(i+EEPROM_START_ADDRESS+j*0x40, ((char*)pPara)[i]);
	  		 	
		 }
	
	 DisableEEPROMWrite();
	 ReadPara();
*/	 
}






void SystemParaInit(void)
{
	
	BTA_HEAT_OFF();	//不加热
	 
	ReadPara();

	G_Timer.Pid_T= G_Para.Pid_T;
	G_Timer.MAX_BTA_T = G_Timer.Pid_T*BTA_TIMRERS_PER_SECOND;
	G_Timer.BTA_T = 0;
	
	PIDInit();
	
}
void NewPID(void)
{
	CLI();
	G_Timer.BTA_T = 0;
	SEI();
}

void Int2Buf(char* buf,int val,char max_buf_id)
{
	signed char i;
	char minus=0;
	if(val<0)
	{
		minus=1;
		val=0-val;
	}
	for(i=max_buf_id;i>=0;i--)
	{
   		buf[i]=val%10;
	 	val/=10;
	}
	if(minus)
		buf[0]=20+'Z'-'A'+2;//-
}   
int FourBuf2Int(char* buf)
{
	unsigned char i;
	char minus;
	int v=0;
	if(buf[0] == '-')
	{
		minus=1;
		buf[0]='0';
	}
	else
		minus=0;
	for(i=0; i<4; i++)
	{
		v = v*10;
   		v += buf[i]-'0';
   		//v=v*10+buf[i]-'0';
	}
	if(minus)
		v = 0-v;
	return v;
}   

void DisplayTemp(char* led_buf,int cur)
{
//	int int_temp;	//温度的整数值
	char temp_buf[5];
	Int2Buf(temp_buf,cur,4);
	if(temp_buf[0] == 20+'Z'-'A'+2)//'-'
	{
		led_buf[0]=20+'Z'-'A'+2;
		led_buf[1]=temp_buf[2];
		led_buf[2]=temp_buf[3]+10;
		led_buf[3]=temp_buf[4];
	}
	else
	{
		led_buf[0]=temp_buf[1];
		led_buf[1]=temp_buf[2];
		led_buf[2]=temp_buf[3]+10;
		led_buf[3]=temp_buf[4];
		
	}

}


int CheckSum(char* source,unsigned char len)
{
	int ret=0;
	
	char i=0;
	while(i < len)
	{
		ret+=source[i];
		i++;
	}
	return ret;
}	  

void deal_RS485(void)
{
	char ret = AnlysizeRs485Data();
	if(ret==0)
	{
//		DealSend();
	}
	//G_cLedBuf[3]=ret;
	UCSRB = ( /*(1<<RXCIE) |*/ (1<<RXEN) | (1<<TXEN) );
}