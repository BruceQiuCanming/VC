#ifndef _PID_H_
#define _PID_H_

#include <iom16v.h>
#include <macros.h>
#include <stdlib.h>


#define MAX_TEMP		300
#define MAX_TEMP_100L	30000
#define MIN_TEMP		0
#define MAX_P	300
#define MIN_P	0
#define MAX_I	999
#define MIN_I	0
#define MAX_D	(999/4)
#define MIN_D	1
#define MAX_T	600
#define MIN_T	1

typedef struct
{  
          long  SetPoint;           // Desired Value
          int  Proportion;         //  Proportional Const
          int  Integral;           //  Integral Const
          int  Derivative;         //  Derivative Const
          int  LastError;          //  Error[-1]
          int  PrevError;          //  Error[-2]
          //int  SumError;           //  Sums of Errors
          long  SumError;           //  Sums of Errors
          long  MaxSumError;		// P*100*I
          long  MinSumError;
}PID;
 
  
typedef struct
{
	
	signed int  Pid_T;	//以秒为单位
	signed int  BTA_T;	//双向可控硅的计数器
	signed int  MAX_BTA_T;	//双向可控硅的计数器最大值=Pid_T*50
}TIMER;


#define BTA_TIMRERS_PER_SECOND	100
#define PID_P_SHIFT_1	1
#define PID_I_SHIFT_100	100
#define PID_D_SHIFT_1	1  
 
enum {AT_CONTINUE,AT_FINISHED,AT_TIMEOUT};
 
#define MAX_AUTO_TEMP  10000
#define MIN_AUTO_TEMP -10000 
 

	extern int G_iPID_out;
	
	extern long		G_iSumPidTemp;	//PID_T周期内温度
	
	extern TIMER G_Timer;
	extern PID  G_HeatPid;  
	extern PID  G_CoolPid;
	
	void PIDInit (void);
	void PIDCal(int NextPoint );
	int PIDCal_Step(int NextPoint );
	int BangBangCtrol(int NextPoint);
	void PIDInit_auto (void);
	void BangBangInit(void);
	char Cal_P_I_D(void);

#endif