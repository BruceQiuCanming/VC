//  /*
//  PID Function
//  
//  The PID function is used in mainly  control applications. 
//  PIDCalc performs one iteration of the PID    algorithm.
//  While the PID function works, main is just a dummy program showing
//  a typical usage.
//  */
  

 #include "typedefs.h"
 #include "eeprom.h" 
 #include "main.h" 
 #include "pid.h" 
 #include "device.h" 
 
 long		G_iSumPidTemp;	//PID_T周期内温度 
  
  TIMER G_Timer;
  char	cStepDownFlag;
  PID  G_HeatPid;  //用与升温
  PID  G_CoolPid;  //用与降温
  
  

  static int  G_Para_para_pid_p_100;
  
  static int Last_NextPoint;
  static char bBeginCalSum;

int abs( int val)
{
	if(val < 0)
		val =0-val;
	
	return val;	
}  
char err;           
void PIDCal(int NextPoint )
{    
      //PID *pp=&G_pid;
	  long v;         
      int  dError, Error;
      int step=0;
      
// 	G_pid.Proportion =(((MAX_TEMP))*PID_P_SHIFT_1)/G_Para.Pid_P; 
      //int p=G_pid.Proportion;
      int pid_p=G_Para.Pid_P*100;
      
      long setpoint	=G_HeatPid.SetPoint;
      long lasterror=G_HeatPid.LastError;
      long preverror=G_HeatPid.PrevError;
      long sumerror	=G_HeatPid.SumError;
      long iTopTemp	=G_iTopTemp;

	  int iPID_out; 
      
      Error = setpoint -  NextPoint; 
      err = G_Para_para_pid_p_100/10;
      if(G_cMode==NORMAL_MODE && G_cSubMode==CONTROL_INCREASE_PID && G_iTopTime>0)
      {//Step up to heat
		    	
		      	step=(iTopTemp-setpoint)/G_iTopTime;
		      	setpoint+=step;
		      	if(cStepDownFlag==0)
		      	{
			      	//pp->SetPoint+=step;
			      	if(setpoint>iTopTemp)
			      		setpoint=iTopTemp;
		      	}
		      	else
		      	{
			      	//pp->SetPoint+=step;
			      	if(setpoint<iTopTemp)
			      		setpoint=iTopTemp;
		      	}
		      	G_iTopTime--;
	
      }
      G_HeatPid.SetPoint = setpoint;
      //if(Error<G_Para_para_pid_p_10)
      
      
/*      RS485_RX_OFF();
	SendChar(Error);
		SendChar(Error/256);
		SendChar(G_Para_para_pid_p_100);
		SendChar(G_Para_para_pid_p_100/256);
		
	delay(500);
	Display();
	RS485_RX_ON();
*/	
      if(Error < G_Para_para_pid_p_100)
      {//在温度误差小于P的时候，进行全加热即100%输出
      	  /*if(Error>0)
      	  {
      	  		if(sumerror<G_pid.MaxSumError && sumerror>G_pid.MinSumError)
      	  			sumerror +=Error;
      		}
      		else
      		{
      			if(sumerror>G_pid.MinSumError)
      	  			sumerror +=Error;
      		}	
 	      G_pid.SumError=sumerror;
 	      if(lasterror!=0 && preverror!=0)
	      	dError = lasterror - preverror;
	      else	
	      	dError=0;	
	      G_pid.PrevError = lasterror;
	      G_pid.LastError = Error;
	      */
	    //if(lasterror!=0)
	    {
	      	dError = Error-lasterror;
	      	//if(dError<=0 || bBeginCalSum)
	      	{//只有在P不起作用的情况下，才计算SumError
	      		//但开始计算以后就一直要计算了，主要是防止溢出和过冲
	      		if(dError>-3 && (Error*10 < G_Para_para_pid_p_100*(10-G_Para.Pid_Exceed)))
	      			bBeginCalSum = 1;
	      		else if(dError < -6)
	      			bBeginCalSum = 0;
	      		if(bBeginCalSum)//2010-1-2 11:33上午
	      			sumerror += (long)Error;
	      		if(sumerror < 0)
	      			sumerror = 0;	
	      	//	bBeginCalSum=1;
	      	}
	    }
	    //else	
	     // 	dError=0;
	      G_HeatPid.LastError = Error;
	      G_HeatPid.SumError=sumerror;
      }
      else
      {
      	//在加温到pp->SetPoint-G_Para.pid_p前，PID的I,D作用应该无效
 	      //pp->SumError   = 0;
	      //pp->PrevError = 0;
	      //pp->LastError = 0;
	      //全速加热
	      //BTA_ON();
	      newBTA = G_Timer.MAX_BTA_T;
	      return;// MAX_TEMP_100L;
      }
      Error+=step;
//=======================================================
//
//	out=P*(Error+SumErr/I+dError*D)
//  out=P*Error+P*SumErr/I+P*dError*D
//
//=======================================================  
  
//   	//pp->Proportion=300*64/20=960
//      //l=(long)Error*(long)pp->Proportion;//=200*960=192000
//      //l=Error*pp->Proportion;//=200*960=192000
//      //v=(int)(l>>PID_P_SHIFT_0);//192000/64=3000
//      v=Error*p;//=200*960=192000
//	  
//  
//      //l=(long)pp->SumError;//200
//      //l=l*(long)pp->Integral;//200*256
////	  l=(long)pp->SumError*(long)pp->Integral;
////    v+=(long)(l/PID_I_SHIFT_100);//200*256/4096=200/16
//	  l=(long)pp->SumError*(long)p/(long)pp->Integral;
//      v+=l;
//
//  
//      //l=pp->Derivative *dError;
//      v+=(long)pp->Derivative *(long)dError*p;//(int)l;
//      //v+=pp->Derivative *dError;
     
//     v=(long)(Error*p)
//     	+(long)pp->SumError*(long)p/(long)pp->Integral
//     	+(long)(pp->Derivative*dError)*(long)p;
      
      
      //bta=iPID_out*G_Timer.Pid_T/((MAX_TEMP)/50);
      
      v=G_Timer.Pid_T*BTA_TIMRERS_PER_SECOND;
//      RS485_RX_OFF();
//      SendChar(v);
//      SendChar(v/256);
//      SendChar(dError);
//      SendChar(dError/256);
//      SendChar(Error);
//      SendChar(Error/256);
//      SendChar(sumerror);
//      SendChar(sumerror/256);
//      SendChar(G_pid.Integral);
//      SendChar(G_pid.Integral/256);
//      SendChar(pid_p);
//      SendChar(pid_p/256);
      
//      if(abs(Error) < 50)
//      	dError  = 0;
      		
      
      iPID_out=((long)v*(long)((long)G_HeatPid.Derivative*(long)dError+(long)Error)
     	+(long)v*(long)sumerror/(long)G_HeatPid.Integral)/(long)pid_p;
      
//		SendChar(iPID_out);
//		SendChar(iPID_out/256);
//	RS485_RX_ON();
      /*
      if((v<0))// 防止溢出
	  		iPID_out=0;
	  else if(v>=MAX_TEMP_100L)// 防止溢出
	  		iPID_out= MAX_TEMP_100L;	
      else 
      		iPID_out= ((int)v);
      */		
    
    if(iPID_out<=0)
    {
    	//newBTA=0;
    	v=0;
    }
    else
    {
		//ASSERT(iPID_out<=MAX_TEMP_10L);
		//bta=(long)iPID_out*(long)G_Timer.MAX_BTA_T/(long)(MAX_TEMP_10L);
		
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//为了防止溢出，  iPID_out被缩小了100倍,最大为(MAX_TEMP_100L)/100=300
		//Pid_T的值最大不能超过0x7FFF/300=100,否则也会溢出
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		/*
    	iPID_out/=100;
		//bta=iPID_out*G_Timer.Pid_T/((MAX_TEMP_100L)/50/100);
		bta=iPID_out*G_Timer.Pid_T/((MAX_TEMP)/BTA_TIMRERS_PER_SECOND);
		
		if(bta>=G_Timer.MAX_BTA_T)
		{
			bta=G_Timer.MAX_BTA_T;
		}
		newBTA=bta;
		*/
		if(iPID_out>=G_Timer.MAX_BTA_T)
			//newBTA=G_Timer.MAX_BTA_T;
			v=G_Timer.MAX_BTA_T;
		else
			//newBTA=iPID_out;
			v=iPID_out;
		
	}	
	newBTA = v;
	//G_iCurTemp[3]=v*10;
	//return 0;	
}
  
void PIDCal_Cool(int NextPoint )
{    
      //PID *pp=&G_pid;
	  long v;         
      int  dError, Error;
      int step=0;
      
// 	G_pid.Proportion =(((MAX_TEMP))*PID_P_SHIFT_1)/G_Para.Pid_P; 
      //int p=G_pid.Proportion;
      int pid_p=G_Para.Pid_P*100;
      
      long setpoint	=G_CoolPid.SetPoint;
      long lasterror=G_CoolPid.LastError;
      long preverror=G_CoolPid.PrevError;
      long sumerror	=G_CoolPid.SumError;
      long iTopTemp	=G_iTopTemp;

	  int iPID_out; 
      
      Error = setpoint -  NextPoint; 
      err = G_Para_para_pid_p_100/10;
      if(G_cMode==NORMAL_MODE && G_cSubMode==CONTROL_COOL_STEP_PID && G_iTopTime>0)
      {//Step up to heat
		      	step=(iTopTemp-setpoint)/G_iTopTime;
		      	setpoint += step;
		      	if(setpoint<iTopTemp)
			    	setpoint=iTopTemp;
		      	G_iTopTime--;
	
      }
      G_CoolPid.SetPoint = setpoint;

      if(Error < G_Para_para_pid_p_100)
      {//在温度误差小于P的时候，进行全加热即100%输出
	      	dError = Error-lasterror;
	      	
	      G_CoolPid.LastError = Error;
	      G_CoolPid.SumError=sumerror;
      }
      else
      {
      	//在加温到pp->SetPoint-G_Para.pid_p前，PID的I,D作用应该无效
 	      //pp->SumError   = 0;
	      //pp->PrevError = 0;
	      //pp->LastError = 0;
	      //全速加热
	      //BTA_ON();
	      newBTA = G_Timer.MAX_BTA_T;
	      return;// MAX_TEMP_100L;
      }
      Error+=step;
      
      v=G_Timer.Pid_T*BTA_TIMRERS_PER_SECOND;
      		
      
      iPID_out=((long)v*(long)((long)G_HeatPid.Derivative*(long)dError+(long)Error)
     	+(long)v*(long)sumerror/(long)G_HeatPid.Integral)/(long)pid_p;
      
    
    if(iPID_out<=0)
    {
    	v=0;
    }
    else
    {
		//ASSERT(iPID_out<=MAX_TEMP_10L);
		//bta=(long)iPID_out*(long)G_Timer.MAX_BTA_T/(long)(MAX_TEMP_10L);
		
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		//为了防止溢出，  iPID_out被缩小了100倍,最大为(MAX_TEMP_100L)/100=300
		//Pid_T的值最大不能超过0x7FFF/300=100,否则也会溢出
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		/*
    	iPID_out/=100;
		//bta=iPID_out*G_Timer.Pid_T/((MAX_TEMP_100L)/50/100);
		bta=iPID_out*G_Timer.Pid_T/((MAX_TEMP)/BTA_TIMRERS_PER_SECOND);
		
		if(bta>=G_Timer.MAX_BTA_T)
		{
			bta=G_Timer.MAX_BTA_T;
		}
		newBTA=bta;
		*/
		if(iPID_out>=G_Timer.MAX_BTA_T)
			//newBTA=G_Timer.MAX_BTA_T;
			v=G_Timer.MAX_BTA_T;
		else
			//newBTA=iPID_out;
			v=iPID_out;
		
	}	
	newBTA = v;
	//G_iCurTemp[3]=v*10;
	//return 0;	
  }
   
void PIDInit (void)
{
	//PID *pp=&G_pid;
	long v;
	long l;
	//BangBangInit();
	//Cal_P_I_D();
  	  				   
      //memset ( pp,0,sizeof(PID));
//	pp->SetPoint   =G_Para.Sv*10;           // Desired Value
//	pp->Proportion =(((MAX_TEMP))<<PID_P_SHIFT_0)/G_Para.Pid_P; //=300*64/20=960        //  Proportional Const
//	if(G_Para.Pid_I==0)
//				pp->Integral=0;
//	else
//				pp->Integral   =(int)((((long)pp->Proportion)<<(PID_I_SHIFT_100-PID_P_SHIFT_0))/(long)G_Para.Pid_I);//960*64/240=256         // Integral Const
//	pp->Derivative =((pp->Proportion*G_Para.Pid_D)>>PID_P_SHIFT_0);         //  Derivative Const

	v  = G_Para.Sv * 10L; 
	/*if(G_cMode==PID_AUTO_MODE)
 	{}//	v  =G_Para.Sv*100;           // Desired Value
 	else*/ if(G_cMode==NORMAL_MODE)	
 	{
 		if(G_cSubMode==CONTROL_NORMAL_PID)
 		{//normal PID
#ifdef RS485 			
 			v   = G_iTopTemp;           // Desired Value
#else
			{}//v   =G_Para.Sv*100;
#endif			 			
 		}
 		else if(G_cSubMode==CONTROL_INCREASE_PID)
 		{//Step up PID
 			v   = G_iSumPidTemp;           // Desired Value
 			if(v>=G_iTopTemp)
 				cStepDownFlag=1;
 			else
 				cStepDownFlag=0;
 		}
 		//else
 		{
 			//v=G_Para.Sv*100;           // Desired Value
 		}
 	}
 	G_HeatPid.SetPoint = v;
	 

	//pp->Proportion值被放大了(1<<PID_P_SHIFT_0)倍
// 	G_pid.Proportion =(((MAX_TEMP))*PID_P_SHIFT_1)/G_Para.Pid_P; //=300*64/20=960        //  Proportional Const

 	
 	//pp->Integral值被放大了(1<<PID_I_SHIFT_100)倍
//	if(G_Para.Pid_I==0)
//			v=0;
//	else
//			v=pp->Proportion*PID_I_SHIFT_100/G_Para.Pid_I;// Integral Const
//	pp->Integral=v;		
	G_HeatPid.Integral = G_Para.Pid_I;
	
	
	//pp->Derivative值被缩小了(1<<PID_P_SHIFT_0)倍		
//	pp->Derivative =pp->Proportion*G_Para.Pid_D/PID_P_SHIFT_1;         //  Derivative Const
//	if(pp->Derivative<0)//溢出
//		pp->Derivative=30000;
	G_HeatPid.Derivative =G_Para.Pid_D;
	
	
	G_HeatPid.LastError  =9999;          //  Error[-1]
	G_HeatPid.PrevError  =9999;          //  Error[-2]
	G_HeatPid.SumError   =0;          //  Sums of Errors
	
	v=G_Para.Pid_P*100; 
	G_Para_para_pid_p_100 = v;
	
	//累计总误差，不能超过100%
	//Iout=(long)pp->SumError*(long)pp->Proportion/(long)pp->Integral;
	//Max_Iout=MAX_TEMP_100L/(G_Para.Pid_P*100)*MaxSumErr/pp->Integral;
	//因为Max_Iout=MAX_TEMP_100L;
	//MaxSumErr=(G_Para.Pid_P*100)*pp->Integral;
	//l=(long)(v/4)*(long)(pp->Integral/2);
	l=(long)(v)*(long)(G_HeatPid.Integral)/2;
	if(l>30000)
		v=30000;
	else
		v=l;	
	G_HeatPid.MaxSumError=v;           //  Sums of Errors
	G_HeatPid.MinSumError=0-v;           //  Sums of Errors
	
	
	bBeginCalSum=0;
	newBTA=0;
}

typedef struct 
{
	int val;
	signed int time;
	char rise_up_flag;
}RECORD;
static RECORD G_Recs[30];
static char recs_id;
static int auto_pid_time = 0;  
//enum{NULL,UP,DOWN};
void BangBangInit(void)
{
	char i;
	char *p;
	p=(char*)&G_Recs[0];
	for(i=0;i<sizeof(G_Recs);i++)
	{
		p[i]=0;
	}
	
	recs_id=0;
	auto_pid_time=0;
	BTA_HEAT_ON();


}

char Cal_P_I_D(void)
{//自整定结束，计算新的P,I,D
	char i,j,k;
	int v=G_Para.Sv*100;	
	RECORD *pRecs=&G_Recs[0];
	
	for(i=1;i<10;i++)
	{
		if(pRecs[i].rise_up_flag==1 && (pRecs[i].val>v))
			break;
	}		
	for(j=i+1;j<10;j++)
	{
		if(pRecs[j].rise_up_flag==0 && (pRecs[j].val<v))
			break;
	}		
	
	for(k=j+1;k<10;k++)
	{
		if(pRecs[k].rise_up_flag==1 && (pRecs[k].val>v))
			break;
	}
	if(k<10 && pRecs[k+1].time!=0)
	{
		//G_Para.Pid_P=(recs[i].val-recs[j].val+50)/100;
		//G_Para.Pid_P=((recs[i].val-recs[j].val)*2+50)/100;
		G_Para.Pid_P=(pRecs[i].val-pRecs[j].val+25)/50;
		//2011-5-28 01:55下午
		//P加大不过冲
		G_Para.Pid_P ++;
		//G_Para.Pid_P=(recs[i].val-recs[j].val+60)/120;
		//if(G_Para.Pid_P<0)
		//	G_Para.Pid_P=0-G_Para.Pid_P;
		//G_Para.Pid_I=((recs[k].time-recs[i].time)/2);
		//G_Para.Pid_D=(G_Para.Pid_I/4);
		//2009-9-17 9:43:53
		//G_Para.Pid_I=((recs[k].time-recs[i].time));
		//G_Para.Pid_D=(G_Para.Pid_I/4);
		//2009-10-4 02:50下午
		G_Para.Pid_I=((pRecs[k].time-pRecs[i].time));
		G_Para.Pid_D=(G_Para.Pid_I/8);
//		backup();
		EnableEEPROMWrite();
		WritePara();
		return 1;
	}
	else
		return 0;
}

int BangBangCtrol(int NextPoint)
{
    //整定周期模型如下：
	/*
	      _ max temp
	     / \               _   
	    /   \             / \ 
	   /     \           /   \
	  /       \         /     \
sv___/         \       /       \       _______________
	            \     /         \     /
		         \   /           \   /
		          \ /             \ / 
		           - min temp      -
	               |     Tk        |
	P=(max_temp-min_temp)/2/0.6=(max_temp-min_temp)/1.2;
	Ti=Tk*0.5;
	Td=Tk*0.125;			    
	//2009-9-17 9:42:11
	Ti=Tk;
	Td=Tk*0.5;
	//2009-10-4 03:05下午
	Ti=Tk;
	Td=Tk/6;
	*/

	static int LastPoint;
	int span;
	char temp_recs_id=recs_id;
	int temp_auto_pid_time=auto_pid_time;
	RECORD *pRecs=&G_Recs[0];
//	G_cLedBuf[3]=recs_id;
	//时间计数器
	temp_auto_pid_time++;
	auto_pid_time=temp_auto_pid_time;
	
	
	if(temp_auto_pid_time >= 60*60*5)//5 hours
	{
		
		BTA_HEAT_OFF();	
		return AT_TIMEOUT;
	}
	if(LastPoint==0)
	{
//		G_cLedBuf[4]=1;
		LastPoint=NextPoint;
		return AT_CONTINUE;
	}
		
	//判断当前温度是上升还是下降 
	//如果连续2次测量的值相差在+-5以内，
	//我们认为测量值没有变化，是测量波动
	//丢弃当前的值
	span=NextPoint-LastPoint;
	
	if((span<10) && (span>-10))
		return -1;
	if(NextPoint>=(int)G_Para.Sv*(int)100)
	{
		BTA_HEAT_OFF();
	}
	else
	{
		BTA_HEAT_ON();	
	}
	
	if(NextPoint>LastPoint)
	{//上升
//		G_cLedBuf[4]=2;
		if(pRecs[temp_recs_id].rise_up_flag)
		{//原来的就是在上升过程中,现在还是上升
//			G_cLedBuf[4]=3;
			if(pRecs[temp_recs_id].val<NextPoint)
			{
				pRecs[temp_recs_id].val=NextPoint;
				pRecs[temp_recs_id].time=temp_auto_pid_time;
				//backup();
			}
		}
		else
		{//原来的就是在下降过程中,现在是上升
			//if((NextPoint>G_Para.Sv*100) && (LastPoint<G_Para.Sv*100))
			{
					
					temp_recs_id++;
					recs_id=temp_recs_id;
					
			}
//			G_cLedBuf[4]=4;
			//if(recs[recs_id].val<NextPoint)
			{
				pRecs[temp_recs_id].val=NextPoint;
				pRecs[temp_recs_id].time=temp_auto_pid_time;
				pRecs[temp_recs_id].rise_up_flag=1;
			}
			//backup();
		}
	}
	else if(NextPoint<LastPoint)
	{//下降
//		G_cLedBuf[4]=5;
		if(pRecs[temp_recs_id].rise_up_flag)
		{//原来的就是在上升过程中,现在是下降
			//if((NextPoint<G_Para.Sv*100) && (LastPoint>G_Para.Sv*100))
			{
					temp_recs_id++;
					recs_id=temp_recs_id;
			}
//			G_cLedBuf[4]=6;
			//if(recs[recs_id].val>NextPoint)
			{
				pRecs[temp_recs_id].val=NextPoint;
				pRecs[temp_recs_id].time=temp_auto_pid_time;
				pRecs[temp_recs_id].rise_up_flag=0;
			}
			//backup();
		}
		else
		{//原来的就是在下降过程中,现在是下降
			if(pRecs[temp_recs_id].val>NextPoint)
			{
				pRecs[temp_recs_id].val=NextPoint;
				pRecs[temp_recs_id].time=temp_auto_pid_time;
				//backup();
			}
//			G_cLedBuf[4]=7;
		}
	}	
	LastPoint=NextPoint;
//	G_cLedBuf[5]= recs_id;
//G_cLedBuf[5]=(G_Para.Sv & 0x0F);
//G_cLedBuf[4]=(NextPoint>>12);
	//实际有效的值取recs_id=2,3,4,5
	//if(recs_id<6)
	if(Cal_P_I_D()==0)
		return AT_CONTINUE;
	else	
		return AT_FINISHED;	 
}