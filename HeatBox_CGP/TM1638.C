/*
 *版权信息：   深圳天微技术部
 *文件名：     TM1638
 *当前版本：   1.0
 *单片机型号： AT89S52
 *开发环境：   Keil uVision3
 *晶震频率：   12M
  功能：       驱动共阴LED数码屏显示数字0-7，按下面板按键显示不同的数值
 */  	

#include <iom16v.h>
#include <macros.h>
#include "typedefs.h"
#include <eeprom.h>
#include "tm1638.h"
#include "device.h"
#include "display.h"
#include "main.h"

#ifdef _TM1638_

const unsigned char LED09AZ[]={//           
	  LEDA+LEDB+LEDC+LEDD+LEDE+LEDF,//0   
	  LEDB+LEDC,					//1
	  LEDA+LEDB+LEDG+LEDE+LEDD,//2 fhbac  
	  LEDA+LEDB+LEDC+LEDD+LEDG,//3 fhbec  
	  LEDF+LEDG+LEDB+LEDC,//4 dbhe        
	  LEDA+LEDF+LEDG+LEDC+LEDD,//5 fdbec  
	  LEDA+LEDF+LEDE+LEDD+LEDC+LEDG,//6 fd
	  LEDA+LEDB+LEDC,//7 fhe              
	  LEDA+LEDB+LEDC+LEDD+LEDE+LEDF+LEDG,//
	  LEDA+LEDB+LEDC+LEDD+LEDF+LEDG,//9 bc
	  LEDA+LEDB+LEDC+LEDD+LEDE+LEDF+LEDH,//0.
	  LEDB+LEDC+LEDH,//1. eh              
	  LEDA+LEDB+LEDG+LEDE+LEDD+LEDH,//2. f
	  LEDA+LEDB+LEDC+LEDD+LEDG+LEDH,//3. f
	  LEDF+LEDG+LEDB+LEDC+LEDH,//4. dbhe  
	  LEDA+LEDF+LEDG+LEDC+LEDD+LEDH,//5. f
	  LEDA+LEDF+LEDE+LEDD+LEDC+LEDG+LEDH,//
	  LEDA+LEDB+LEDC+LEDH,//7. fhe        
	  LEDA+LEDB+LEDC+LEDD+LEDE+LEDF+LEDG+LEDH,
	  LEDA+LEDB+LEDC+LEDD+LEDF+LEDG+LEDH,//
	  LEDA+LEDB+LEDC+LEDE+LEDF+LEDG,//A fd
	  LEDF+LEDE+LEDD+LEDC+LEDG,     //B db
	  LEDA+LEDF+LEDE+LEDD,          //C fd
	  LEDG+LEDE+LEDD+LEDC+LEDB,     //D hb
	  LEDA+LEDF+LEDE+LEDD+LEDG,     //E fd
	  LEDA+LEDF+LEDE+LEDG,          //F fd
	  LEDA+LEDC+LEDD+LEDE+LEDF,//G        
	  LEDB+LEDC+LEDE+LEDF+LEDG,//H        
	  LEDE+LEDF,//I                       
	  LEDB+LEDC+LEDD,//J                  
	  LEDE+LEDF+LEDG,//K                  
	  LEDD+LEDE+LEDF,//L                  
	  0,//M                                 
	  LEDB+LEDC+LEDE+LEDF+LEDA,//N        
	  LEDC+LEDD+LEDE+LEDG,//O             
	  LEDA+LEDB+LEDE+LEDF+LEDG,//P        
	  LEDA+LEDB+LEDC+LEDF+LEDG,//Q        
	  LEDE+LEDG,//R                       
	  LEDA+LEDF+LEDG+LEDC+LEDD,//S        
	  LEDE+LEDF+LEDA,//T                  
	  LEDB+LEDC+LEDD+LEDE+LEDF,//U        
	  LEDC+LEDD+LEDE,//V                  
	  0,//W                                 
	  0,//X                                 
	  0,//Y                                 
	  0,//Z                                 
	  LEDB+LEDC+LEDG,//+                  
	  LEDG,//-	  	                    
	  };	                                


unsigned char U1_TM1638_DisplayBuf[16];
unsigned char U2_TM1638_DisplayBuf[16];
unsigned char U3_TM1638_DisplayBuf[16];
unsigned char G_cLedDisplayBuf[9];		//  = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
void ChangSwtichStatus2DisplayBuf(void)
{
	//U2
	//  06 04
	//  02 00
	//  14 12
	//  10 08
	
	U2_TM1638_DisplayBuf[6] = G_cSwitchStatus[2];
	U2_TM1638_DisplayBuf[4] = G_cSwitchStatus[3];	
	
	U2_TM1638_DisplayBuf[2] = G_cSwitchStatus[10];
	U2_TM1638_DisplayBuf[0] = G_cSwitchStatus[11];
	
	U2_TM1638_DisplayBuf[14] = G_cSwitchStatus[6];
	U2_TM1638_DisplayBuf[12] = G_cSwitchStatus[7];
	U2_TM1638_DisplayBuf[10] = G_cSwitchStatus[14];
	U2_TM1638_DisplayBuf[8] = G_cSwitchStatus[15];
	//U3
	// 02  00
	// 06  04
	// 10  08
	// 14  12
	U3_TM1638_DisplayBuf[2] = G_cSwitchStatus[0];
	U3_TM1638_DisplayBuf[0] = G_cSwitchStatus[1];	
	
	U3_TM1638_DisplayBuf[6] = G_cSwitchStatus[8];
	U3_TM1638_DisplayBuf[4] = G_cSwitchStatus[9];
	
	U3_TM1638_DisplayBuf[10] = G_cSwitchStatus[4];
	U3_TM1638_DisplayBuf[8] = G_cSwitchStatus[5];
	U3_TM1638_DisplayBuf[14] = G_cSwitchStatus[12];
	U3_TM1638_DisplayBuf[12] = G_cSwitchStatus[13];
	
}
void ChangeBuf2Data(void)
{
	
	char led_id;
	char temp;
	char i;
	
	ChangSwtichStatus2DisplayBuf();
	
	for(led_id = 0; led_id < 9; led_id++)
	{
		if(G_cMode != NORMAL_MODE && G_cMode != PID_AUTO_MODE)
		{
			if(LedFlash && (led_id == leftright))
			{	
				G_cLedDisplayBuf[led_id] = 0x00;
			}
			else
			{
				G_cLedDisplayBuf[led_id] = LED09AZ[G_cLedBuf[led_id]];
			}			
		}
		else
		{	
			G_cLedDisplayBuf[led_id] = LED09AZ[G_cLedBuf[led_id]];
		}	
	}
	
		


	for(led_id = 0; led_id < 8; led_id++)
	{
		temp = 0;
		//led_bit = BIT(led_id);
		for(i = 0; i < 8; i++)
		{
			if(G_cLedDisplayBuf[i] & BIT(led_id))
				temp |= BIT(i);
		}
		U1_TM1638_DisplayBuf[led_id *2] = temp;	
	}	
	

}


void TM1638_Port_Init(void)
{
	U1_STB_DDR |= BIT(U1_STB_BIT);
	U1_CLK_DDR |= BIT(U1_CLK_BIT);
	U1_DIO_DDR |= BIT(U1_DIO_BIT);
	U1_CLK_ON();                                                         
	U1_DIO_ON(); 
	U1_STB_ON();

	U2_STB_DDR |= BIT(U2_STB_BIT);
	U2_CLK_DDR |= BIT(U2_CLK_BIT);
	U2_DIO_DDR |= BIT(U2_DIO_BIT);
	U2_CLK_ON();                                                         
	U2_DIO_ON(); 
	U2_STB_ON();

	U3_STB_DDR |= BIT(U3_STB_BIT);
	U3_CLK_DDR |= BIT(U3_CLK_BIT);
	U3_DIO_DDR |= BIT(U3_DIO_BIT);
	U3_CLK_ON();                                                         
	U3_DIO_ON(); 
	U3_STB_ON();
	
}
                                                   
void  tm1638_delay(void)		  //延时函数
{
 	unsigned char  i;
	
 	ReadRx();

 	for(i = 0; i < 160;i++)
 	{
    	_NOP();
    	WDR();
    }
    
}
		
                                                      
void send_data(unsigned char dat,unsigned char unit)     //写入数据                                     
{                                                                  
 	unsigned char i;                                                       
  	for(i = 0; i < 8;i++)                                               
	{                                              
			if(unit == 0)
			{           
		 		U1_CLK_OFF(); 
		 	}
		 	else if(unit == 1)
			{           
		 		U2_CLK_OFF(); 
		 	}
		 	else
		 	{           
		 		U3_CLK_OFF(); 
		 	}
		 	tm1638_delay();                                                  
		 	if(dat & 0x01)
		 	{
		 		if(unit == 0)
		 		{
		 			U1_DIO_ON();
		 		}
		 		else if(unit == 1)
				{
					U2_DIO_ON();
				}		 		
				else
				{
					U3_DIO_ON();
				}		
			}
		 	else
		 	{
		 		if(unit == 0)
		 		{
		 			U1_DIO_OFF();
		 		}
		 		else if(unit == 1)
				{
					U2_DIO_OFF();
				}		 		
				else
				{
					U3_DIO_OFF();
				}
		 	}
		 	tm1638_delay();
		 	if(unit == 0)
			{           
		 		U1_CLK_ON(); 
		 	}
		 	else if(unit == 1)
			{           
		 		U2_CLK_ON(); 
		 	}
		 	else
		 	{           
		 		U3_CLK_ON(); 
		 	}
		 	tm1638_delay();                                                    											     
		 	dat >>= 1;//右移一位，低位先进，串口通讯模式 
		 	tm1638_delay();
	}                                                              
} 
		
void TM1638_Display(unsigned char unit)			 //显示函数,1-7位数码管显示数字0-6
{
		unsigned char i;
		ReadRx();
		
		ChangeBuf2Data();
		
		ReadRx();
		
		U1_DIO_DDR |= BIT(U1_DIO_BIT);
		U2_DIO_DDR |= BIT(U2_DIO_BIT);
		U3_DIO_DDR |= BIT(U3_DIO_BIT);
		
		if(unit == 0)
		{
			U1_STB_OFF();                                                         
		}
		else if(unit == 1)
		{
			U2_STB_OFF();
		}
		else
		{
			U3_STB_OFF();
		}
		
		send_data(0x40,unit);             //设置数据命令，采用地址自动加1模式              
		
		if(unit == 0)
		{
			U1_STB_ON();                                                         
		}
		else if(unit == 1)
		{
			U2_STB_ON();
		}
		else
		{
			U3_STB_ON();
		}
		
		if(unit == 0)
		{
			U1_STB_OFF();                                                         
		}
		else if(unit == 1)
		{
			U2_STB_OFF();
		}
		else
		{
			U3_STB_OFF();
		}						 
		send_data(0xc0,unit);			 //设置显示地址，从00H开始
		for(i = 0;i < 16;i++)			//发送显示数据
		{
			if(unit == 0)
			{	
				send_data(U1_TM1638_DisplayBuf[i],unit); 	            //如果要直接看显示效果，可以直接送0xff全亮
			}
			else if(unit == 1)
			{	
				if(G_cDisplayType == 0)
				{
					send_data(U2_TM1638_DisplayBuf[i],unit); 	            //如果要直接看显示效果，可以直接送0xff全亮
				}
				else if(G_cDisplayType == 1)
				{
					send_data(0xFF,unit); 	            //如果要直接看显示效果，可以直接送0xff全亮
				}
				else
				{
					send_data(0,unit); 	            //如果要直接看显示效果，可以直接送0xff全亮
				}
					
			}
			else
			{	
				if(G_cDisplayType == 0)
				{
					send_data(U3_TM1638_DisplayBuf[i],unit); 	            //如果要直接看显示效果，可以直接送0xff全亮
				}
				else if(G_cDisplayType == 1)
				{
					send_data(0xFF,unit); 	            //如果要直接看显示效果，可以直接送0xff全亮
				}
				else
				{
					send_data(0,unit); 	            //如果要直接看显示效果，可以直接送0xff全亮
				}
			}
		} 
		if(unit == 0)
		{
			U1_STB_ON();                                                         
		}
		else if(unit == 1)
		{
			U2_STB_ON();
		}
		else
		{
			U3_STB_ON();
		}
		 
		 tm1638_delay();
		 
		 if(unit == 0)
		{
			U1_STB_OFF();                                                         
		}
		else if(unit == 1)
		{
			U2_STB_OFF();
		}
		else
		{
			U3_STB_OFF();
		}                                                     
		 send_data(0x88 + 5,unit);         //显示控制命令，打开显示并设置为最亮 
		if(unit == 0)
		{
			U1_STB_ON();                                                         
		}
		else if(unit == 1)
		{
			U2_STB_ON();
		}
		else
		{
			U3_STB_ON();
		}
}  
		                                  

#endif
                                         