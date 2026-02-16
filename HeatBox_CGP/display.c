#include <iom16v.h>
#include <macros.h>
#include "typedefs.h"
#include "main.h"
#include "device.h"
#include "display.h"

static unsigned char switch_id = 0;		//动态切换的开关状态
unsigned char G_cSwitchNormalOpen = 0;	// = 0 常闭, = 1常开
unsigned char leftright;
unsigned char MinusFlag;			//负号标字
unsigned char LedFlash;				//闪烁标志
unsigned char display_id;			//动态刷新LED的编号

unsigned char G_cLedBuf[16];		
#ifdef HEATBOX128_V1_0
	unsigned char G_cSwitchStatus[(MAX_LED_NR+7)/8];//=
		  //{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};
		//{0x00,0x0E,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x60,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};  		//实际输出的开关状态
	unsigned char G_cSwitchStatus_cur[(MAX_LED_NR+7)/8];    //上次输出的开关状态
	unsigned char G_cSwitchOpenFlash[(MAX_LED_NR+7)/8];    		//闪动标志
	unsigned char G_cSwitchCloseFlash[(MAX_LED_NR+7)/8];    		//闪动标志
#endif

#define FLASH_TEST

	
#ifdef FLASH_TEST
void  CheckSwitchOpenFlash(char switch_last_status,char switch_cur_status,char *switch_flash)
{
	char ret = *switch_flash;
	if(G_cAutoTest 
		&& G_cMode == NORMAL_MODE
		&& G_cSubMode == CONTROL_INCREASE_PID)		
	  {												  					
	  	if(G_cSwitchNormalOpen == 0) 												
	  	{//常闭，动作闪动是由开到闭
	   		ret |= (~switch_last_status) & (switch_cur_status);	
	   		//if(ret)
	   		//	G_cLedBuf[1][0] = 7;
	   	}
	   	else
	   	{//常开，动作闪动是由闭到开
	   		ret |= (switch_last_status) & (~switch_cur_status);	
	   		//if(ret)
	   		//	G_cLedBuf[1][0] = 3;															
	   }																	
	}
	*switch_flash = ret;
}
#endif

#ifdef FLASH_TEST  
	
#endif
		
#ifdef HEATBOX128_V1_0
	
#endif

void Display(void)
{}





void WriteOver(char flag)
{
 if(flag == OVER_HIGH)
 {
  G_cLedBuf[0] = 20+'H'-'A';
  G_cLedBuf[1] = 20+'H'-'A';
  G_cLedBuf[2] = 20+'H'-'A'; 
  G_cLedBuf[3] = 20+'H'-'A';
 }
 else
 {
  G_cLedBuf[0] = 20+'L'-'A';
  G_cLedBuf[1] = 20+'L'-'A';
  G_cLedBuf[2] = 20+'L'-'A';
  G_cLedBuf[3] = 20+'L'-'A';
 } 
  BTA_HEAT_OFF();

}

void DisplayInit(void)
{
	


}

void SelfDisplayCheck(void)
{
	
		char i = 0 ,j = 0 , k;
	
		unsigned char *pLed = &G_cSwitchStatus[0];
		char led_id=display_id;
    
}

unsigned char LedBuf[sizeof(G_cSwitchStatus)];

void AdjustLedBuf(void)
{
	int i;
	for(i=0; i < sizeof(LedBuf); i++)
	{
		LedBuf[i] = 0;
	}   
	LedBuf[0] =  G_cSwitchStatus[0];
	LedBuf[1] = (G_cSwitchStatus[1] >> 2) | (G_cSwitchStatus[2] << 6);
	LedBuf[2] = (G_cSwitchStatus[2] >> 4) | (G_cSwitchStatus[3] << 4);
	LedBuf[3] = (G_cSwitchStatus[3] >> 6) | (G_cSwitchStatus[4] << 2);
	
	LedBuf[4] = G_cSwitchStatus[5];
	LedBuf[5] = (G_cSwitchStatus[6] >> 2) | (G_cSwitchStatus[7] << 6);
	LedBuf[6] = (G_cSwitchStatus[7] >> 4) | (G_cSwitchStatus[8] << 4);
	LedBuf[7] = (G_cSwitchStatus[8] >> 6) | (G_cSwitchStatus[9] << 2);
	
	LedBuf[8]  = G_cSwitchStatus[10];
	LedBuf[9]  = (G_cSwitchStatus[11] >> 2) | (G_cSwitchStatus[12] << 6);
	LedBuf[10] = (G_cSwitchStatus[12] >> 4) | (G_cSwitchStatus[13] << 4);
	LedBuf[11] = (G_cSwitchStatus[13] >> 6) | (G_cSwitchStatus[14] << 2);
	
	LedBuf[12] = G_cSwitchStatus[15];
	
	
	LedBuf[13] = 0xFF;//(G_cSwitchStatus[16] >> 2) | (G_cSwitchStatus[17] << 6);
	LedBuf[14] = 0xFF;//(G_cSwitchStatus[17] >> 4) | (G_cSwitchStatus[18] << 4);
	LedBuf[18] = 0xFF;//(G_cSwitchStatus[18] >> 6) | (G_cSwitchStatus[19] << 2);
	
	//led 16
	if(G_cSwitchStatus[1] & BIT(0))
	{
		LedBuf[15] |= BIT(0); 
	}
	if(G_cSwitchStatus[1] & BIT(1))
	{
		LedBuf[15] |= BIT(1); 
	}
	
	if(G_cSwitchStatus[2] & BIT(2))
	{
		LedBuf[15] |= BIT(2); 
	}
	if(G_cSwitchStatus[2] & BIT(3))
	{
		LedBuf[15] |= BIT(3); 
	}

	if(G_cSwitchStatus[3] & BIT(4))
	{
		LedBuf[15] |= BIT(4); 
	}
	if(G_cSwitchStatus[3] & BIT(5))
	{
		LedBuf[15] |= BIT(5); 
	}
	
	if(G_cSwitchStatus[4] & BIT(6))
	{
		LedBuf[15] |= BIT(6); 
	}
	if(G_cSwitchStatus[4] & BIT(7))
	{
		LedBuf[15] |= BIT(7); 
	}
	
	
	//led 17
	if(G_cSwitchStatus[6] & BIT(0))
	{
		LedBuf[16] |= BIT(0); 
	}
	if(G_cSwitchStatus[6] & BIT(1))
	{
		LedBuf[16] |= BIT(1); 
	}
	
	if(G_cSwitchStatus[7] & BIT(2))
	{
		LedBuf[16] |= BIT(2); 
	}
	if(G_cSwitchStatus[7] & BIT(3))
	{
		LedBuf[16] |= BIT(3); 
	}

	if(G_cSwitchStatus[8] & BIT(4))
	{
		LedBuf[16] |= BIT(4); 
	}
	if(G_cSwitchStatus[8] & BIT(5))
	{
		LedBuf[16] |= BIT(5); 
	}
	
	if(G_cSwitchStatus[9] & BIT(6))
	{
		LedBuf[16] |= BIT(6); 
	}
	if(G_cSwitchStatus[9] & BIT(7))
	{
		LedBuf[16] |= BIT(7); 
	}
	
	//led 18
	if(G_cSwitchStatus[11] & BIT(0))
	{
		LedBuf[17] |= BIT(0); 
	}
	if(G_cSwitchStatus[11] & BIT(1))
	{
		LedBuf[17] |= BIT(1); 
	}
	
	if(G_cSwitchStatus[12] & BIT(2))
	{
		LedBuf[17] |= BIT(2); 
	}
	
	if(G_cSwitchStatus[12] & BIT(3))
	{
		LedBuf[17] |= BIT(3); 
	}

	if(G_cSwitchStatus[13] & BIT(4))
	{
		LedBuf[17] |= BIT(4); 
	}
	if(G_cSwitchStatus[13] & BIT(5))
	{
		LedBuf[17] |= BIT(5); 
	}
	
	if(G_cSwitchStatus[14] & BIT(6))
	{
		LedBuf[17] |= BIT(6); 
	}
	if(G_cSwitchStatus[14] & BIT(7))
	{
		LedBuf[17] |= BIT(7); 
	}
	
}

