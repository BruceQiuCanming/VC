// TakeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "HeatBoxDlg.h"
#include "TakeDlg.h"
#include "comm.h"
#include "common.h"
#include "RecordViewDlg.h"
#include "ProductTempBar.h"
#include "RecordPrintPriew.h"
#include "switchdlg.h"
#include "shlwapi.h"
#include "atlimage.h"
#include "InputDlg.h"
#include "ListDlg.h"
#include "math.h"

#pragma comment(lib,"shlwapi.lib")


SWITCH_TEST_RESULT G_TakeDlgSwitchTestResult[2][192];	//一个在线拔板，一个历史记录拔板
SWITCH_TEST_RESULT G_TakeDlgSwitchTestResult_Bak[2][192];


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	TCHAR level_str[18][10];

int AdjustProductID[128] = 
{
  1,  2,  3,  4,  5,  6,  7,  8,105,106,
  9, 10, 11, 12, 13, 14, 15, 16,107,108,
 17, 18, 19, 20, 21, 22, 23, 24,109,110,
 25, 26, 27, 28, 29, 30, 31, 32,111,112,
 33, 34, 35, 36, 37, 38, 39, 40,113,114,
 41, 42, 43, 44, 45, 46, 47, 48,115,116,
 49, 50, 51, 52, 53, 54, 55, 56,117,118,
 57, 58, 59, 60, 61, 62, 63, 64,119,120,
 65, 66, 67, 68, 69, 70, 71, 72,121,122,
 73, 74, 75, 76, 77, 78, 79, 80,123,124,
 81, 82, 83, 84, 85, 86, 87, 88,125,126,
 89, 90, 91, 92, 93, 94, 95, 96,127,128,
 97, 98, 99,100,101,102,103,104,
};

/*
TEST_RESULT_LEVEL CheckTestLevel(SWITCH_CONFIG_PARA SwitchConfigPara,
			SWITCH_TEST_RESULT SwitchTestResult)
{
	//不合格的先选出来

//不用的
	
		if(SwitchTestResult.IsUsed == false)
		{
			return NOT_USED;
		}
//PTC		
		//if(BoxType == HEAT_BOX_TYPE_PTC)
		{
			if(SwitchTestResult.IsNoPTC)
			{
				return NO_PTC_LEVEL;
			}
		}
//动作闪烁	
#ifndef _CHENG_HONG_
		if (SwitchTestResult.IsOpenFlash)
		{
			return OPEN_FLASH_LEVEL;
		}
#endif

//复位闪烁	
#ifndef _CHENG_HONG_
		if (SwitchTestResult.IsCloseFlash)
		{
			return CLOSE_FLASH_LEVEL;
		}
#endif


//开门复位
#ifndef _CHENG_HONG_
		if(SwitchTestResult.CloseTemp + SwitchConfigPara.OpenDoorResetLevel>= SwitchTestResult.OpenTemp
			&& SwitchTestResult.CloseTemp > 0
			&& SwitchTestResult.OpenTemp > 0)
		{
			return OPEN_DOOR_RESET_LEVEL;
		}
#endif
		
			
//有用产品			
			
				
//主规格
			if(SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_RANGE)
			{
				if((SwitchConfigPara.MainLevel != 0)
					&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus))
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus))
					&& (SwitchTestResult.CloseTemp  >= (SwitchConfigPara.CloseLevel - SwitchConfigPara.CloseLevelSpanMinus ))
					&& (SwitchTestResult.CloseTemp  <= (SwitchConfigPara.CloseLevel + SwitchConfigPara.CloseLevelSpanPlus)))
				{
					return MAIN_LEVEL;
				}

				if((SwitchConfigPara.CloseLevelSpanMinus == 0) && (SwitchConfigPara.CloseLevelSpanPlus == 0) )
				{//没有复位
					if(((SwitchConfigPara.MainLevelSpanMinus != 0) || (SwitchConfigPara.MainLevelSpanPlus != 0))
						&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus))
						&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus))
						)
					{
						return MAIN_LEVEL;
					}
				
				}
			}
			else if(SwitchConfigPara.CloseTempType  == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP_AND_RANGE)
			{
				if((SwitchConfigPara.MainLevel != 0)
					&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus))
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus))
					&& (SwitchTestResult.CloseTemp  >= (SwitchConfigPara.CloseLevel - SwitchConfigPara.CloseLevelSpanMinus ))
					&& (SwitchTestResult.CloseTemp  <= (SwitchConfigPara.CloseLevel + SwitchConfigPara.CloseLevelSpanPlus))
					&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  >= (SwitchConfigPara.CloseLevelDiffMinus))
					&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  <= (SwitchConfigPara.CloseLevelDiffPlus)))
				{
					return MAIN_LEVEL;
				}
			}
			else if(SwitchConfigPara.CloseTempType  == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP)
			{
				if((SwitchConfigPara.MainLevel != 0)
					&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus))
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus))
					&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp  >= (SwitchConfigPara.CloseLevelDiffMinus))
					&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp  <= (SwitchConfigPara.CloseLevelDiffPlus)))
				{
					return MAIN_LEVEL;
				}
			}
	
//高规格
#ifndef _CHENG_HONG_
		if(SwitchConfigPara.CloseTempType  == CLOSE_TEMP_TYPE_RANGE)
		{
			if(SwitchConfigPara.HighLevel != 0
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.HighLevel - SwitchConfigPara.HighLevelSpanMinus))
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus))
				&& (SwitchTestResult.CloseTemp >=(SwitchConfigPara.CloseHighLevel - SwitchConfigPara.CloseHighLevelSpanMinus))
				&& (SwitchTestResult.CloseTemp <=(SwitchConfigPara.CloseHighLevel + SwitchConfigPara.CloseHighLevelSpanPlus)))
			{
				return TEST_RESULT_HIGH_LEVEL;
			}

			if(SwitchConfigPara.CloseHighLevel == 0)
			{
				if((SwitchConfigPara.HighLevel != 0)
					&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.HighLevel - SwitchConfigPara.HighLevelSpanMinus))  
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus)))
				{//没有复位
					return TEST_RESULT_HIGH_LEVEL;
				}
			}
		}
		else if(SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP_AND_RANGE)
		{
			if(SwitchConfigPara.HighLevel != 0
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.HighLevel - SwitchConfigPara.HighLevelSpanMinus))
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus))
				&& (SwitchTestResult.CloseTemp >=(SwitchConfigPara.CloseHighLevel - SwitchConfigPara.CloseHighLevelSpanMinus))
				&& (SwitchTestResult.CloseTemp <=(SwitchConfigPara.CloseHighLevel + SwitchConfigPara.CloseHighLevelSpanPlus))
				&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp >= SwitchConfigPara.CloseHighLevelDiffMinus)
				&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp <= SwitchConfigPara.CloseHighLevelDiffPlus))
			{
				return TEST_RESULT_HIGH_LEVEL;
			}

		}
		else if(SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP)
		{
			if(SwitchConfigPara.HighLevel != 0
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.HighLevel - SwitchConfigPara.HighLevelSpanMinus))
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus))
				&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp >= SwitchConfigPara.CloseHighLevelDiffMinus)
				&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp <= SwitchConfigPara.CloseHighLevelDiffPlus))
			{
				return TEST_RESULT_HIGH_LEVEL;
			}
		}
#endif
//低规格
#ifndef _CHENG_HONG_
		if(SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_RANGE)
		{
			if((SwitchConfigPara.LowLevel != 0)
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus))  
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.LowLevel + SwitchConfigPara.LowLevelSpanPlus))
				&& (SwitchTestResult.CloseTemp  >= (SwitchConfigPara.CloseLowLevel  - SwitchConfigPara.CloseLowLevelSpanMinus))
				&& (SwitchTestResult.CloseTemp  <= (SwitchConfigPara.CloseLowLevel  + SwitchConfigPara.CloseLowLevelSpanPlus ))
				)
			{
				return LOW_LEVEL;
			}

			if(SwitchConfigPara.CloseLowLevel == 0)
			{
				if((SwitchConfigPara.LowLevel != 0)
					&& (SwitchTestResult.OpenTemp >=  (SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus))  
					&& (SwitchTestResult.OpenTemp <=  (SwitchConfigPara.LowLevel + SwitchConfigPara.LowLevelSpanPlus)))
				{//没有复位
					return LOW_LEVEL;
				}
			
			}
		}
		else if(SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP_AND_RANGE)
		{
			if((SwitchConfigPara.LowLevel != 0)
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus))  
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.LowLevel + SwitchConfigPara.LowLevelSpanPlus))
				&& (SwitchTestResult.CloseTemp  >= (SwitchConfigPara.CloseLowLevel  - SwitchConfigPara.CloseLowLevelSpanMinus))
				&& (SwitchTestResult.CloseTemp  <= (SwitchConfigPara.CloseLowLevel  + SwitchConfigPara.CloseLowLevelSpanPlus ))
				&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  >= SwitchConfigPara.CloseLowLevelDiffMinus)
				&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  <= SwitchConfigPara.CloseLowLevelDiffPlus )
				)
			{
				return LOW_LEVEL;
			}
		}
		else if(SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP)
		{
			if((SwitchConfigPara.LowLevel != 0)
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus))  
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.LowLevel + SwitchConfigPara.LowLevelSpanPlus))
				&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  >= SwitchConfigPara.CloseLowLevelDiffMinus)
				&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  <= SwitchConfigPara.CloseLowLevelDiffPlus )
				)
			{
				return LOW_LEVEL;
			}

		}
#endif

			CONTROL_TEMP_RANGE temp_range;

			CheckHeatOrCoolMode(SwitchConfigPara, &temp_range);

#ifndef _CHENG_HONG_
		if((SwitchTestResult.IsOpenned == true)
			&& (SwitchTestResult.OpenTemp > temp_range.open_temp_max)
			&& (SwitchTestResult.OpenTemp <= temp_range.open_temp_max + G_iHighHighLevelDelta)) 
		{
			return HIGH_HIGH_LEVEL;
		}
#endif

//高温品

		if((SwitchTestResult.IsOpenned == false)
			|| (SwitchTestResult.OpenTemp > temp_range.open_temp_max + G_iHighHighLevelDelta)) //(SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus))) 
		{
			return OVER_HIGH_LEVEL;
		}

//低温品		
		if(SwitchTestResult.OpenTemp < temp_range.open_temp_min )//SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus)
		{
			return OVER_LOW_LEVEL;
		}

//不合格品
		if(SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_RANGE)
		{
			if((SwitchConfigPara.MainLevel != 0)
					&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus))
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus)))
			{
				if(SwitchConfigPara.CloseLevel != 0)
				{			
			//复小品
					if((SwitchTestResult.CloseTemp > SwitchConfigPara.CloseLevel - SwitchConfigPara.CloseLevelSpanMinus)
					&& (SwitchTestResult.CloseTemp > SwitchConfigPara.CloseLevel + SwitchConfigPara.CloseLevelSpanPlus))
					{
						return CLOSE_HIGH_LEVEL;
					}
				}
			
		//复大品
				if(SwitchTestResult.IsClosed == false
				|| ((SwitchConfigPara.CloseLevel != 0) && (SwitchTestResult.CloseTemp <= (SwitchConfigPara.CloseLevel - SwitchConfigPara.CloseLevelSpanMinus))))
				{
					return CLOSE_LOW_LEVEL;
				}

			}
		}
		else if(SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP_AND_RANGE)
		{
			if((SwitchConfigPara.MainLevel != 0)
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus))
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus)))
			{
				

				if(SwitchTestResult.CloseTemp  <= (SwitchConfigPara.CloseLowLevel  - SwitchConfigPara.CloseLowLevelSpanMinus))
				
				{
					return CLOSE_LOW_LEVEL ;
				}

				if (SwitchTestResult.CloseTemp  >= (SwitchConfigPara.CloseLowLevel  + SwitchConfigPara.CloseLowLevelSpanPlus ))
				
				{
					return CLOSE_HIGH_LEVEL;
				}

				if((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp) < SwitchConfigPara.CloseLevelDiffMinus)
				{
					return CLOSE_HIGH_LEVEL;
				}

				if(SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp > SwitchConfigPara.CloseLevelDiffPlus)
				{
					return CLOSE_LOW_LEVEL;
				}

			}
		}
		else if(SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP)
		{
			if((SwitchConfigPara.MainLevel != 0)
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus))
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus)))
			{
				if(SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp < SwitchConfigPara.CloseLevelDiffMinus)
				{
					return CLOSE_HIGH_LEVEL;
				}

				if(SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp > SwitchConfigPara.CloseLevelDiffPlus)
				{
					return CLOSE_LOW_LEVEL;
				}
			}
		}

#ifdef _CHANGHONG_
		if(SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_RANGE)
		{
			if((SwitchTestResult.OpenTemp >= (SwitchConfigPara.HighLevel - SwitchConfigPara.HighLevelSpanMinus))
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus)))
			{
			//高温复小品
					if(SwitchTestResult.CloseTemp > SwitchConfigPara.CloseHighLevel + SwitchConfigPara.CloseHighLevelSpanPlus)
					{
						return HIGH_LEVEL_CLOSE_HIGH_LEVEL;
					}
			//高温复大品
					if(SwitchTestResult.IsClosed == false
					|| ((SwitchConfigPara.CloseLevel != 0) && (SwitchTestResult.CloseTemp <= (SwitchConfigPara.CloseHighLevel - SwitchConfigPara.CloseHighLevelSpanMinus))))
					{
						return HIGH_LEVEL_CLOSE_LOW_LEVEL;
					}
			}
		}
		else
		{
			if((SwitchTestResult.OpenTemp >= (SwitchConfigPara.HighLevel - SwitchConfigPara.HighLevelSpanMinus))
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus)))
			{
			//高温复小品
					if( SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp < SwitchConfigPara.CloseHighLevelSpanMinus)
					{
						return HIGH_LEVEL_CLOSE_HIGH_LEVEL;
					}
			//高温复大品
					if(SwitchTestResult.IsClosed == false
					|| (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp > SwitchConfigPara.CloseHighLevelSpanPlus))
					{
						return HIGH_LEVEL_CLOSE_LOW_LEVEL;
					}
			}
		}

		if(SwitchConfigPara.CloseTempType == CLOSE_TEMP_TYPE_RANGE)
		{
			if((SwitchTestResult.OpenTemp >= (SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus))
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.LowLevel + SwitchConfigPara.LowLevelSpanPlus)))
			{
			//高温复小品
					if(SwitchTestResult.CloseTemp > SwitchConfigPara.CloseLowLevel + SwitchConfigPara.CloseLowLevelSpanPlus)
					{
						return LOW_LEVEL_CLOSE_HIGH_LEVEL;
					}
			//高温复大品
					if(SwitchTestResult.IsClosed == false
					|| (SwitchTestResult.CloseTemp <= (SwitchConfigPara.CloseHighLevel - SwitchConfigPara.CloseLowLevelSpanMinus)))
					{
						return LOW_LEVEL_CLOSE_LOW_LEVEL;
					}
			}
		}
		else
		{
			if((SwitchTestResult.OpenTemp >= (SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus))
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.LowLevel + SwitchConfigPara.LowLevelSpanPlus)))
			{
			//高温复小品
					if(SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp < SwitchConfigPara.CloseLowLevelSpanMinus)
					{
						return LOW_LEVEL_CLOSE_HIGH_LEVEL;
					}
			//高温复大品
					if(SwitchTestResult.IsClosed == false
					|| (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp <= SwitchConfigPara.CloseLowLevelSpanPlus))
					{
						return LOW_LEVEL_CLOSE_LOW_LEVEL;
					}
			}
		}
#endif

	return NO_LEVEL;
}

*/

/////////////////////////////////////////////////////////////////////////////
// CTakeoutDlg dialog

CTakeoutDlg::CTakeoutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTakeoutDlg::IDD, pParent)
	, m_iFailHighLevel(0)
	
	, m_iFailLowLevel(0)
	
	, m_iFailOtherLevel(0)
	
{
	//{{AFX_DATA_INIT(CTakeoutDlg)
	m_bMainLevel		= FALSE;
	m_bLowLevel			= FALSE;
	m_bHighLevel		= FALSE;
	m_bCloseHighLevel	= FALSE;
	m_bCloseLowLevel	= FALSE;
	m_bOverHighLevel	= FALSE;
	m_bOverLowLevel		= FALSE;
	m_bNotUsed			= FALSE;
	m_bOpenFlashLevel	= FALSE;
	m_bCloseFlashLevel	= FALSE;
	m_iCloseFlashLevel	= 0;
	m_iCloseHighLevel	= 0;
	m_iCloseLowLevel	= 0;
	m_iHighLevel		= 0;
	m_iLowLevel			= 0;
	m_iMainLevel		= 0;
	m_iNotUsed			= 0;
	m_iOpenFlashLevel	= 0;
	m_iOverHighLevel	= 0;
	m_iOverLowLevel		= 0;
	m_iNoPTCLevel		=	0;
	m_bNoPTCLevel		=	FALSE;
	m_Msg = _T("");
	m_iSelect = 0;
	m_bOpenDoorResetLevel = FALSE;
	m_iOpenDoorResetLevel = 0;
	m_bNoLevel = FALSE;
	m_iNoLevel = 0;
	m_bHighHighLevel = FALSE;
	m_iHighHighLevel = 0;
	m_bCloseFlashLevel = FALSE;
	m_iCloseFlashLevel = 0;
	m_bHighLevelCloseHighLevel = FALSE;
	m_bHighLevelCloseLowLevel = FALSE;
	m_bLowLevelCloseHighLevel = FALSE;
	m_bLowLevelCloseLowLevel = FALSE;
	m_iHighLevelCloseHighLevel = 0;
	m_iHighLevelCloseLowLevel = 0;
	m_iLowLevelCloseHighLevel = 0;
	m_iLowLevelCloseLowLevel = 0;
	//}}AFX_DATA_INIT

	m_UpDownID		=	0;
	m_fOpenTempShow = 999.99f;
//	m_iTakeBoardType = 0;

	for(int i = 0; i < MAX_HEAT_BOX; i++)
	{
		 m_iFailOtherLevel_Take[i] = 0;
		m_iFailLowLevel_Take[i] = 0;
		m_iFailHighLevel_Take[i] = 0;
	}
}


void CTakeoutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTakeoutDlg)
	DDX_Check(pDX, IDC_CHECK_MAIN_LEVEL, m_bMainLevel);
	DDX_Check(pDX, IDC_CHECK_LOW_LEVEL, m_bLowLevel);
	DDX_Check(pDX, IDC_CHECK_HIGH_LEVEL, m_bHighLevel);
	DDX_Check(pDX, IDC_CHECK_CLOSE_HIGH_LEVEL, m_bCloseHighLevel);
	DDX_Check(pDX, IDC_CHECK_CLOSE_LOW_LEVEL, m_bCloseLowLevel);
	DDX_Check(pDX, IDC_CHECK_OVER_HIGH_LEVEL, m_bOverHighLevel);
	DDX_Check(pDX, IDC_CHECK_OVER_LOW_LEVEL, m_bOverLowLevel);
	DDX_Check(pDX, IDC_CHECK_NO_ACTION, m_bNotUsed);
	DDX_Check(pDX, IDC_CHECK_OPEN_FLASH, m_bOpenFlashLevel);
	DDX_Check(pDX, IDC_CHECK_CLOSE_FLASH, m_bCloseFlashLevel);
	DDX_Check(pDX, IDC_CHECK_NO_PTC_LEVEL, m_bNoPTCLevel);
	DDX_Radio(pDX, IDC_RADIO_ONLY_ONE, m_iSelect);
	DDX_Check(pDX, IDC_CHECK_OPEN_DOOR_RESET, m_bOpenDoorResetLevel);
	DDX_Text(pDX, IDC_STATIC_MSG, m_Msg);
	DDX_Check(pDX, IDC_CHECK_NO_LEVEL, m_bNoLevel);
	DDX_Check(pDX, IDC_CHECK_HIGH_HIGH_LEVEL, m_bHighHighLevel);
	DDX_Check(pDX, IDC_CHECK_HIGH_LEVEL_CLOSE_HIGH_LEVEL, m_bHighLevelCloseHighLevel);
	DDX_Check(pDX, IDC_CHECK_HIGH_LEVEL_CLOSE_LOW_LEVEL, m_bHighLevelCloseLowLevel);
	DDX_Check(pDX, IDC_CHECK_LOW_LEVEL_CLOSE_HIGH_LEVEL, m_bLowLevelCloseHighLevel);
	DDX_Check(pDX, IDC_CHECK_LOW_LEVEL_CLOSE_LOW_LEVEL, m_bLowLevelCloseLowLevel);

	DDX_Text(pDX, IDC_EDIT_CLOSE_FLASH_LEVEL, m_iCloseFlashLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_HIGH_LEVEL, m_iCloseHighLevel);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LOW_LEVEL, m_iCloseLowLevel);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL, m_iHighLevel);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL, m_iLowLevel);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL, m_iMainLevel);
	DDX_Text(pDX, IDC_EDIT_NO_ACTION_LEVEL, m_iNotUsed);
	DDX_Text(pDX, IDC_EDIT_OPEN_FLASH_LEVEL, m_iOpenFlashLevel);
	DDX_Text(pDX, IDC_EDIT_OVER_HIGH_LEVEL, m_iOverHighLevel);
	DDX_Text(pDX, IDC_EDIT_OVER_LOW_LEVEL, m_iOverLowLevel);
	DDX_Text(pDX, IDC_EDIT_OPEN_DOOR_RESET, m_iOpenDoorResetLevel);
	DDX_Text(pDX, IDC_EDIT_NO_LEVEL, m_iNoLevel);
	DDX_Text(pDX, IDC_EDIT_HIGH_HIGH_LEVEL, m_iHighHighLevel);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_CLOSE_HIGH_LEVEL, m_iHighLevelCloseHighLevel);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_CLOSE_LOW_LEVEL, m_iHighLevelCloseLowLevel);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_CLOSE_HIGH_LEVEL, m_iLowLevelCloseHighLevel);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_CLOSE_LOW_LEVEL, m_iLowLevelCloseLowLevel);
	DDX_Text(pDX, IDC_EDIT_NO_PTC_LEVEL, m_iNoPTCLevel);

	DDX_Text(pDX, IDC_EDIT_CLOSE_FLASH_LEVEL2, m_iCloseFlashLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_CLOSE_HIGH_LEVEL2, m_iCloseHighLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_CLOSE_LOW_LEVEL2, m_iCloseLowLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL2, m_iHighLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL2, m_iLowLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_MAIN_LEVEL3, m_iMainLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_NO_ACTION_LEVEL2, m_iNotUsed_IsClosing);
	DDX_Text(pDX, IDC_EDIT_OPEN_FLASH_LEVEL2, m_iOpenFlashLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_OVER_HIGH_LEVEL2, m_iOverHighLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_OVER_LOW_LEVEL2, m_iOverLowLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_OPEN_DOOR_RESET2, m_iOpenDoorResetLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_NO_LEVEL2, m_iNoLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_HIGH_HIGH_LEVEL2, m_iHighHighLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_CLOSE_HIGH_LEVEL2, m_iHighLevelCloseHighLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_CLOSE_LOW_LEVEL2, m_iHighLevelCloseLowLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_CLOSE_HIGH_LEVEL2, m_iLowLevelCloseHighLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_CLOSE_LOW_LEVEL2, m_iLowLevelCloseLowLevel_IsClosing);
	DDX_Text(pDX, IDC_EDIT_NO_PTC_LEVEL2, m_iNoPTCLevel);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_GROUP_NR, m_ComboGroupNr);
	DDX_Text(pDX, IDC_EDIT_HIGH_FAIL_LEVEL, m_iFailHighLevel);
	
	DDX_Text(pDX, IDC_EDIT_LOW_FAIL_LEVEL, m_iFailLowLevel);
	
	DDX_Text(pDX, IDC_EDIT_OTHER_FAIL_LEVEL, m_iFailOtherLevel);
	//DDX_Text(pDX, IDC_EDIT_OTHER_FAIL_LEVEL_TAKE, m_iFailOtherLevel_Take);
	//DDX_Text(pDX, IDC_EDIT_HIGH_FAIL_LEVEL_TAKE, m_iFailHighLevel_Take);
	//DDX_Text(pDX, IDC_EDIT_LOW_FAIL_LEVEL_TAKE, m_iFailLowLevel_Take);
}


BEGIN_MESSAGE_MAP(CTakeoutDlg, CDialog)
	//{{AFX_MSG_MAP(CTakeoutDlg)
	ON_WM_SHOWWINDOW()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK_MAIN_LEVEL, OnCheckMainLevel)
	ON_BN_CLICKED(IDC_CHECK_OVER_HIGH_LEVEL, OnCheckOverHighLevel)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_HIGH_LEVEL, OnCheckCloseHighLevel)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_LOW_LEVEL, OnCheckCloseLowLevel)
	ON_BN_CLICKED(IDC_CHECK_HIGH_LEVEL, OnCheckHighLevel)
	ON_BN_CLICKED(IDC_CHECK_LOW_LEVEL, OnCheckLowLevel)
	ON_BN_CLICKED(IDC_CHECK_OVER_LOW_LEVEL, OnCheckOverLowLevel)
	ON_BN_CLICKED(IDC_CHECK_OPEN_FLASH, OnCheckOpenFlash)
	ON_BN_CLICKED(IDC_CHECK_NO_ACTION, OnCheckNoAction)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_OPEN_DOOR_RESET, OnCheckOpenDoorReset)
	ON_BN_CLICKED(IDC_RADIO_ONLY_ONE, OnRadioOnlyOne)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_RECORD_VIEW2, OnButtonRecordView2)
	ON_BN_CLICKED(IDC_CHECK_CLOSE_FLASH, OnCheckCloseFlash)
	ON_BN_CLICKED(IDC_RADIO_ALL_SELECT, OnRadioAllSelect)
	ON_WM_SETCURSOR()
	ON_BN_CLICKED(IDC_CHECK_NO_LEVEL, OnCheckNoLevel)
	ON_BN_CLICKED(IDC_CHECK_HIGH_HIGH_LEVEL, OnCheckHighHighLevel)
	ON_BN_CLICKED(IDC_CHECK_HIGH_LEVEL_CLOSE_HIGH_LEVEL, OnCheckHighLevelCloseHighLevel)
	ON_BN_CLICKED(IDC_CHECK_HIGH_LEVEL_CLOSE_LOW_LEVEL, OnCheckHighLevelCloseLowLevel)
	ON_BN_CLICKED(IDC_CHECK_LOW_LEVEL_CLOSE_HIGH_LEVEL, OnCheckLowLevelCloseHighLevel)
	ON_BN_CLICKED(IDC_CHECK_LOW_LEVEL_CLOSE_LOW_LEVEL, OnCheckLowLevelCloseLowLevel)
	//}}AFX_MSG_MAP
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_NO_PTC_LEVEL, &CTakeoutDlg::OnBnClickedCheckNoPtcLevel)
	ON_BN_CLICKED(IDC_BUTTON_TEMP_VIEW, &CTakeoutDlg::OnBnClickedButtonTempView)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_EXCEL, &CTakeoutDlg::OnBnClickedButtonSaveExcel)
	ON_BN_CLICKED(IDC_BUTTON_PREVIEW, &CTakeoutDlg::OnBnClickedButtonPreview)
	ON_BN_CLICKED(IDC_BUTTON_TODAY_CHANLIANG, &CTakeoutDlg::OnBnClickedButtonTodayChanliang)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, &CTakeoutDlg::OnBnClickedButtonPrint)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_GROUP, &CTakeoutDlg::OnBnClickedButtonGroup)
	ON_EN_SETFOCUS(IDC_EDIT_OPEN, &CTakeoutDlg::OnEnSetfocusEditOpen)
	ON_EN_SETFOCUS(IDC_EDIT_OPEN_PLUS, &CTakeoutDlg::OnEnSetfocusEditOpenPlus)
	ON_EN_SETFOCUS(IDC_EDIT_OPEN_MINUS, &CTakeoutDlg::OnEnSetfocusEditOpenMinus)
	ON_EN_SETFOCUS(IDC_EDIT_CLOSE, &CTakeoutDlg::OnEnSetfocusEditClose)
	ON_EN_SETFOCUS(IDC_EDIT_CLOSE_PLUS, &CTakeoutDlg::OnEnSetfocusEditClosePlus)
	ON_EN_SETFOCUS(IDC_EDIT_CLOSE_MINUS, &CTakeoutDlg::OnEnSetfocusEditCloseMinus)
	ON_EN_CHANGE(IDC_EDIT_HIGH_LEVEL_CLOSE_HIGH_LEVEL, &CTakeoutDlg::OnEnChangeEditHighLevelCloseHighLevel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTakeoutDlg message handlers
CDC MemDC;
CBitmap bitmap;
CBitmap* pOldBitmap;

void CTakeoutDlg::ShowTestResult(bool bDraw,bool level_text)
{
	static long last_ticket;
	
	if(::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_ONLY_OPENTEMP_TEST)
	{

		ShowTestResult_BOX_128_ONLY_OPENTEMP_TEST(bDraw,level_text);
		return;

	}

	CTime cur = CTime::GetCurrentTime();
	long t = ::GetTickCount();
	CString s;

	s.Format(_T("测试开始时间：%02d/%02d/%02d %02d:%02d:%02d  当前时间%d:%d:%d 通讯数量=%d 接收=%d  间隔:%d毫秒"), m_BeginTestTime.GetYear(),m_BeginTestTime.GetMonth(),m_BeginTestTime.GetDay(),  m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond(), cur.GetHour(),cur.GetMinute(),cur.GetSecond(),G_MainDlg->m_DebugDlg[m_BoxNr].m_SendCmdCount,::G_MainDlg->m_DebugDlg[m_BoxNr].m_ReceiveCmdCount ,  t - last_ticket);
	last_ticket = t;
//	this->SetWindowTextW(s);

	int id,ii;
	if(bDraw)
	{
		this->UpdateData();
	}

	m_HeatCoolType = ::CheckHeatOrCoolMode(m_BoxNr,this->m_SwitchConfigPara.para ,&m_TempRange,this->m_BoxType);
	
	level_text = m_IsVK;

	m_Msg = "";
	m_iNotUsed					= 0;
	m_iMainLevel				= 0;
	m_iLowLevel					= 0;
	m_iHighLevel				= 0;
	
	m_iHighHighLevel			= 0;
	m_iOverHighLevel			= 0;
	m_iOverLowLevel				= 0;
	
	m_iCloseHighLevel			= 0;
	m_iCloseLowLevel			= 0;
	
	m_iOpenFlashLevel			= 0;
	m_iCloseFlashLevel			= 0;
	
	m_iOpenDoorResetLevel		= 0;


	m_iNoLevel					= 0;

	m_iHighLevelCloseHighLevel	= 0;
	m_iLowLevelCloseHighLevel	= 0;
	m_iHighLevelCloseLowLevel	= 0;
	m_iLowLevelCloseLowLevel	= 0;
	
	m_iNoPTCLevel				= 0;

	//
	m_iNotUsed_IsClosing					= 0;
	m_iMainLevel_IsClosing				= 0;
	m_iLowLevel_IsClosing					= 0;
	m_iHighLevel_IsClosing				= 0;
	
	m_iHighHighLevel_IsClosing			= 0;
	m_iOverHighLevel_IsClosing			= 0;
	m_iOverLowLevel_IsClosing				= 0;
	
	m_iCloseHighLevel_IsClosing			= 0;
	m_iCloseLowLevel_IsClosing			= 0;
	
	m_iOpenFlashLevel_IsClosing			= 0;
	m_iCloseFlashLevel_IsClosing			= 0;
	
	m_iOpenDoorResetLevel_IsClosing		= 0;


	m_iNoLevel_IsClosing					= 0;

	m_iHighLevelCloseHighLevel_IsClosing	= 0;
	m_iLowLevelCloseHighLevel_IsClosing	= 0;
	m_iHighLevelCloseLowLevel_IsClosing	= 0;
	m_iLowLevelCloseLowLevel_IsClosing	= 0;
	
	m_iNoPTCLevel_IsClosing				= 0;

	
	if(!this->IsWindowVisible()) 
	{
		return;
	}

	
//
//	CheckFailCloseFlash(&G_TakeDlgSwitchTestResult[m_BoxNr],m_BoxNr);

	CRect w_rect;
	this->GetClientRect(w_rect); 
	#define PIC_GAP 5
	int SWITCH_PIC_HIGH		=	(w_rect.bottom - w_rect.top)		/ G_iLedLines[m_BoxNr] - PIC_GAP;
	int SWITCH_PIC_WIDTH	= 	(w_rect.right  - w_rect.left - 160) / G_iLedNrPerLine[m_BoxNr] - PIC_GAP;
	#define TEMP_STRING_HEIGHT	60

	this->GetWindowRect(w_rect); 
	#define PIC_GAP 5

	int nWidth = SWITCH_PIC_WIDTH;
    int nHeight = SWITCH_PIC_HIGH;

	CDC *dc = NULL;
	
	if(!this->IsWindowVisible()) 
	{
		return;
	}


	//CBitmap所需资源
//	CBitmap bitmap;

	    dc=this->GetDC();
		
		if(dc != NULL)
		{
			if(MemDC.GetSafeHdc() != NULL)
			{
				MemDC.DeleteDC();
			}
			
			if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
			{
				if(dc->GetSafeHdc() != NULL)
				{
					ReleaseDC(dc);
				}
				return;
			}
		}
		else
		{
			return;
		}
		CBitmap b;
		bitmap.DeleteObject();

		if(!bitmap.CreateCompatibleBitmap(dc,(nWidth + PIC_GAP)*G_iLedNrPerLine[m_BoxNr],(nHeight+PIC_GAP) * G_iLedLines[m_BoxNr]))	//建立与显示设备兼容的位图
		{
			if(dc->GetSafeHdc() != NULL)
			{
				ReleaseDC(dc);
			}
			return;
		}

		pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

		//RGB(0, 160, 128), RGB(255, 255, 60)
		CBrush bbrush(COLORREF(RGB(0,160,128)));

		MemDC.FillRect(CRect(0,0,(nWidth + PIC_GAP)*G_iLedNrPerLine[m_BoxNr],(nHeight+PIC_GAP) * G_iLedLines[m_BoxNr]),&bbrush);

//		dc->BitBlt(0,0,(nWidth + PIC_GAP)*G_iLedNrPerLine[m_BoxNr],(nHeight+PIC_GAP) * G_iLedLines[m_BoxNr],&MemDC,0,0,SRCCOPY); 	//显示原图形

		int count = 0;

		for(int i=0; i < G_iMaxLedNr[m_BoxNr] / 8 ; i++)
		{
			count = 0;
			for(int j = 0;j < 8; j++)
			{
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][i* 8 + j].IsCloseFlash)
				{
					count ++;
				}
			}
			/*
			if( count >= 4)
			{
				for(int j = 0;j < 8; j++)
				{
					G_TakeDlgSwitchTestResult[m_iTakeBoardType][i* 8 + j].IsCloseFlash = false;
				}
			}*/

			count = 0;
			for(int j = 0;j < 8; j++)
			{
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][i* 8 + j].IsOpenFlash)
				{
					count ++;
				}
			}
			/*
			if( count >= 4)
			{
				for(int j = 0;j < 8; j++)
				{
					G_TakeDlgSwitchTestResult[m_iTakeBoardType][i* 8 + j].IsOpenFlash = false;
				}
			}*/
		}



	for(ii=0; ii < G_iMaxLedNr[m_BoxNr] ; ii++)
	{
		if(::G_NormalConfigPara[m_BoxNr].BoxType == HEAT_BOX_TYPE_128_OLD_DISPLAY)
		{
			int old_id[128] = {
				 0, 1, 2, 3,  4,  5,  6,  7,104,105,
				 8, 9,10,11, 12, 13, 14, 15,106,107,
				16,17,18,19, 20, 21, 22, 23,108,109,
				24,25,26,27, 28, 29, 30, 31,110,111,
				32,33,34,35, 36, 37, 38, 39,112,113,
				40,41,42,43, 44, 45, 46, 47,114,115,
				48,49,50,51, 52, 53, 54, 55,116,117,
				56,57,58,59, 60, 61, 62, 63,118,119,
				64,65,66,67, 68, 69, 70, 71,120,121,
				72,73,74,75, 76, 77, 78, 79,122,123,
				80,81,82,83, 84, 85, 86, 87,124,125,
				88,89,90,91, 92, 93, 94, 95,126,127,
				96,97,98,99,100,101,102,103};
			id = old_id[ii];
			CString s;
			s.Format(_T("%d"),id); 
		//AfxMessageBox(s);
		}
		else
		{
			id = ii;
		}

		//clear 
//		if(bDraw)
//		{
//			DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
//		}
		//hasShowed = false;

		if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp > this->m_fOpenTempShow)
		{
			continue;
		}

//不合格的先选出来
		if(m_BoxType == ::BOX_DOUBLE_SWITCH)
		{
			int Ex_Nr[] = {10,11,12,13,14,15,16,17,18,19,
				0,1,2,3,4,5,6,7,8,9,
				30,31,32,33,34,35,36,37,38,39,
				20,21,22,23,24,25,26,27,28,29,

				50,51,52,53,54,55,56,57,58,59,
				40,41,42,43,44,45,46,47,48,49,

				70,71,72,73,74,75,76,77,78,79,
				60,61,62,63,64,65,66,67,68,69,

				90,91,92,93,94,95,96,97,98,99,
				80,81,82,83,84,85,86,87,88,89,

				110,111,112,113,114,115,116,117,118,119,
				100,101,102,103,104,105,106,107,108,109,

				121,120,123,122,125,124,127,126


			};
			m_SwitchTestResult_Ex = G_TakeDlgSwitchTestResult[m_iTakeBoardType][Ex_Nr[id]];
		}
		
	
//不用的
		switch(CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType))
		{
		case NOT_USED:
				if(m_bNotUsed && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,NOT_USED,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iNotUsed ++;

				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iNotUsed_IsClosing ++;
				}

		break;
		case NO_PTC_LEVEL:
				if(m_bNoPTCLevel && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,NO_PTC_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}				
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
			m_iNoPTCLevel ++;

				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iNoPTCLevel_IsClosing ++;
				}
			break;

//复位闪烁		
		case CLOSE_FLASH_LEVEL:
				if(m_bCloseFlashLevel && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,CLOSE_FLASH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
			m_iCloseFlashLevel ++;
			if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iCloseFlashLevel_IsClosing ++;
				}
		break;
//动作闪烁		
		case OPEN_FLASH_LEVEL:
				if(m_bOpenFlashLevel && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OPEN_FLASH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iOpenFlashLevel ++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iOpenFlashLevel_IsClosing ++;
				}
		break;
//高温品
		case HIGH_HIGH_LEVEL:
				if(m_bHighHighLevel  && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,HIGH_HIGH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iHighHighLevel++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iHighHighLevel_IsClosing ++;
				}
		break;

//不动作品
		case OVER_HIGH_LEVEL:
				if(m_bOverHighLevel  && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OVER_HIGH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iOverHighLevel++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iOverHighLevel_IsClosing ++;
				}
		break;
		
//复大品
		case CLOSE_HIGH_LEVEL:
				if(m_bCloseHighLevel && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,CLOSE_HIGH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iCloseHighLevel ++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iCloseHighLevel_IsClosing ++;
				}
		break;
		
		
//复小品		
		case CLOSE_LOW_LEVEL:
				if(m_bCloseLowLevel && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,CLOSE_LOW_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iCloseLowLevel ++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iCloseLowLevel_IsClosing ++;
				}
		break;
//开门复位
		case OPEN_DOOR_RESET_LEVEL:
				if(m_bOpenDoorResetLevel && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OPEN_DOOR_RESET_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iOpenDoorResetLevel ++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iOpenDoorResetLevel_IsClosing ++;
				}
		break;
			
//有用产品			
			
				
//主规格
		case MAIN_LEVEL:
				if(m_bMainLevel && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,MAIN_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iMainLevel++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iMainLevel_IsClosing ++;
				}
		break;	
//低规格
		case LOW_LEVEL:
				if(m_bLowLevel && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp, LOW_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iLowLevel++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iLowLevel_IsClosing ++;
				}
		break;
			
//高规格
		case TEST_RESULT_HIGH_LEVEL:
				if(m_bHighLevel  && bDraw && IsValidTemp(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp))
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,TEST_RESULT_HIGH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iHighLevel ++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iHighLevel_IsClosing ++;
				}
		break;
			
				
//低温品			
		case OVER_LOW_LEVEL:
				if(m_bOverLowLevel  && bDraw && IsValidTemp(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp))
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OVER_LOW_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iOverLowLevel ++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iOverLowLevel_IsClosing ++;
				}
			break;
		
//高低复大小
		case NO_LEVEL:
				if(m_bNoLevel  && bDraw )
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,NO_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iNoLevel ++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iNoLevel_IsClosing ++;
				}
		break;

		default:
		{
			CString s;
			s.Format(_T("%d#\r\n使用=%d\r\n状态=%d\r\n动作=%d\r\n复位=%d\r\n动作闪动=%d\r\n复位闪动=%d\r\n动作温度=%5.1f\r\n复位温度=%5.1f\r\n"),
			id,
			G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsUsed,		//产品是否插入
			G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing,	    //现在是否闭合
			G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsOpenned,		//是否曾经打开过
			G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosed,		//是否曾经复位过
			G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsOpenFlash,	//是否动作时闪动
			G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsCloseFlash,	//是否复位时闪动
			G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,		//动作温度
			G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp);	//复位温度	
	
			m_Msg += s;				
		}
		}
		
	}

		if(bDraw)
	{
		CRect w_rect;
		this->GetClientRect(w_rect); 
		#define PIC_GAP 5
		int SWITCH_PIC_HIGH	=	(w_rect.bottom - w_rect.top) / G_iLedLines[m_BoxNr] - PIC_GAP;
		int SWITCH_PIC_WIDTH = 	(w_rect.right  - w_rect.left - 160) / G_iLedNrPerLine[m_BoxNr] - PIC_GAP;
		#define TEMP_STRING_HEIGHT	60

		this->GetWindowRect(w_rect); 
		#define PIC_GAP 5

		int nWidth = SWITCH_PIC_WIDTH;
		int nHeight = SWITCH_PIC_HIGH;

		//dc->BitBlt((nWidth +PIC_GAP) * (switchNr % G_iLedNrPerLine[m_BoxNr]),(nHeight+PIC_GAP) * (switchNr / G_iLedNrPerLine[m_BoxNr]),nWidth,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
		CBrush tempBrush(RGB(255,255,0));

		if(G_iMaxLedNr[m_BoxNr] == 192)
		{
			MemDC.FillRect(CRect((nWidth +PIC_GAP) * (6 % G_iLedNrPerLine[m_BoxNr]),0,(nWidth +PIC_GAP) * (6 % G_iLedNrPerLine[m_BoxNr])-PIC_GAP,(nHeight+PIC_GAP)*16), &tempBrush);
			MemDC.FillRect(CRect(0,(nHeight+PIC_GAP) * 8, (nWidth +PIC_GAP) * 12-PIC_GAP,(nHeight+PIC_GAP) * 8-PIC_GAP), &tempBrush);
		}
		else
		{
			MemDC.FillRect(CRect((nWidth +PIC_GAP) * (5 % G_iLedNrPerLine[m_BoxNr]),0,(nWidth +PIC_GAP) * (5 % G_iLedNrPerLine[m_BoxNr])-PIC_GAP,(nHeight+PIC_GAP)*13), &tempBrush);
			if(::G_NormalConfigPara[0].BoxType == BOX_OIL_BATHS)
			{
			}
			else
			{
				MemDC.FillRect(CRect(0,(nHeight+PIC_GAP) * 6, (nWidth +PIC_GAP) * 10-PIC_GAP,(nHeight+PIC_GAP) * 6-PIC_GAP), &tempBrush);
			}
		}

		dc->BitBlt(0,0,(nWidth + PIC_GAP)*G_iLedNrPerLine[m_BoxNr],(nHeight+PIC_GAP) * G_iLedLines[m_BoxNr],&MemDC,0,0,SRCCOPY); 	//显示原图形


		this->UpdateData(false); 
	}
//return;
	CWnd *w,*w1;

	//m_iNotUsed = 1;
	w	= GetDlgItem(IDC_EDIT_NO_ACTION_LEVEL);
	w1	= GetDlgItem(IDC_CHECK_NO_ACTION);
	if(m_iNotUsed == 0 )
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iMainLevel = 1;
	w = GetDlgItem(IDC_EDIT_MAIN_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_MAIN_LEVEL);
	if(m_iMainLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iLowLevel = 1;
	w = GetDlgItem(IDC_EDIT_LOW_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_LOW_LEVEL);
	if(m_iLowLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iHighLevel =1;
	w = GetDlgItem(IDC_EDIT_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_HIGH_LEVEL);
	if(m_iHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iHighHighLevel = 1;
	w = GetDlgItem(IDC_EDIT_HIGH_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_HIGH_HIGH_LEVEL);
	if(m_iHighHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iOverHighLevel = 1;
	w = GetDlgItem(IDC_EDIT_OVER_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_OVER_HIGH_LEVEL);
	if(m_iOverHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iOverLowLevel = 1;
	w = GetDlgItem(IDC_EDIT_OVER_LOW_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_OVER_LOW_LEVEL);
	if(m_iOverLowLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}
	
	//m_iCloseHighLevel = 1;
	w = GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_CLOSE_HIGH_LEVEL);
	if(m_iCloseHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iCloseLowLevel = 1;
	w = GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_CLOSE_LOW_LEVEL);
	if(m_iCloseLowLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iOpenFlashLevel = 1;
	w = GetDlgItem(IDC_EDIT_OPEN_FLASH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_OPEN_FLASH);
	if(m_iOpenFlashLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iCloseFlashLevel = 1;
	w = GetDlgItem(IDC_EDIT_CLOSE_FLASH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_CLOSE_FLASH);
	if(m_iCloseFlashLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	
	//m_iOpenDoorResetLevel = 1;
	w = GetDlgItem(IDC_EDIT_OPEN_DOOR_RESET);
	w1 = GetDlgItem(IDC_CHECK_OPEN_DOOR_RESET);
	if(m_iOpenDoorResetLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);

	}

	
	//m_iNoLevel = 1;
	w = GetDlgItem(IDC_EDIT_NO_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_NO_LEVEL);
	if(m_iNoLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}


	w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_HIGH_LEVEL_CLOSE_HIGH_LEVEL);
	if(m_iHighLevelCloseHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_LOW_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_HIGH_LEVEL_CLOSE_LOW_LEVEL);
	if(m_iHighLevelCloseLowLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);

	}

	w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_LOW_LEVEL_CLOSE_HIGH_LEVEL);
	if(m_iLowLevelCloseHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_LOW_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_LOW_LEVEL_CLOSE_LOW_LEVEL);
	if(m_iLowLevelCloseLowLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	w = GetDlgItem(IDC_EDIT_NO_PTC_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_NO_PTC_LEVEL);
	if(m_iNoPTCLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	this->m_iFailHighLevel	= this->m_iOverHighLevel;
	this->m_iFailLowLevel	= this->m_iOverLowLevel;
	this->m_iFailOtherLevel = 	
	m_iCloseHighLevel +	m_iCloseLowLevel +	
	m_iOpenFlashLevel +	m_iCloseFlashLevel +	
	m_iOpenDoorResetLevel +	m_iNoLevel +
	m_iHighLevelCloseHighLevel +	m_iLowLevelCloseHighLevel +
	m_iHighLevelCloseLowLevel +	m_iLowLevelCloseLowLevel +	
	m_iNoPTCLevel;

	
	this->UpdateData(false);

	

		w = GetDlgItem(IDC_EDIT_OTHER_FAIL_LEVEL_TAKE);
		s.Format(_T("%d"), m_iFailOtherLevel_Take[m_BoxNr]);
		w->SetWindowTextW(s);

		w = GetDlgItem(IDC_EDIT_HIGH_FAIL_LEVEL_TAKE);
		s.Format(_T("%d"), G_TakeoutDlg.m_iFailHighLevel_Take[m_BoxNr]);
		w->SetWindowTextW(s);

		w = GetDlgItem(IDC_EDIT_LOW_FAIL_LEVEL_TAKE);
		s.Format(_T("%d"), G_TakeoutDlg.m_iFailLowLevel_Take[m_BoxNr]);
		w->SetWindowTextW(s);

//	return;
#ifndef _DEBUG
	if(dc->GetSafeHdc())
	{
		dc->DeleteDC(); 
	}
#endif
}

void CTakeoutDlg::ShowTestResult_BOX_128_ONLY_OPENTEMP_TEST(bool bDraw,bool level_text)
{
	static long last_ticket;

	CTime cur = CTime::GetCurrentTime();
	long t = ::GetTickCount();
	CString s;

	s.Format(_T("测试开始时间：%02d/%02d/%02d %02d:%02d:%02d  当前时间%d:%d:%d 通讯数量=%d 接收=%d  间隔:%d毫秒"), m_BeginTestTime.GetYear(),m_BeginTestTime.GetMonth(),m_BeginTestTime.GetDay(),  m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute(),m_BeginTestTime.GetSecond(), cur.GetHour(),cur.GetMinute(),cur.GetSecond(),G_MainDlg->m_DebugDlg[m_BoxNr].m_SendCmdCount,::G_MainDlg->m_DebugDlg[m_BoxNr].m_ReceiveCmdCount ,  t - last_ticket);
	last_ticket = t;
//	this->SetWindowTextW(s);

	int id,ii;
	if(bDraw)
	{
		this->UpdateData();
	}

	m_HeatCoolType = ::CheckHeatOrCoolMode(m_BoxNr,this->m_SwitchConfigPara.para ,&m_TempRange,this->m_BoxType);
	
	level_text = m_IsVK;

	m_Msg = "";
	m_iNotUsed					= 0;
	m_iMainLevel				= 0;
	m_iLowLevel					= 0;
	m_iHighLevel				= 0;
	
	m_iHighHighLevel			= 0;
	m_iOverHighLevel			= 0;
	m_iOverLowLevel				= 0;
	
	m_iCloseHighLevel			= 0;
	m_iCloseLowLevel			= 0;
	
	m_iOpenFlashLevel			= 0;
	m_iCloseFlashLevel			= 0;
	
	m_iOpenDoorResetLevel		= 0;


	m_iNoLevel					= 0;

	m_iHighLevelCloseHighLevel	= 0;
	m_iLowLevelCloseHighLevel	= 0;
	m_iHighLevelCloseLowLevel	= 0;
	m_iLowLevelCloseLowLevel	= 0;
	
	m_iNoPTCLevel				= 0;

	//
	m_iNotUsed_IsClosing					= 0;
	m_iMainLevel_IsClosing				= 0;
	m_iLowLevel_IsClosing					= 0;
	m_iHighLevel_IsClosing				= 0;
	
	m_iHighHighLevel_IsClosing			= 0;
	m_iOverHighLevel_IsClosing			= 0;
	m_iOverLowLevel_IsClosing				= 0;
	
	m_iCloseHighLevel_IsClosing			= 0;
	m_iCloseLowLevel_IsClosing			= 0;
	
	m_iOpenFlashLevel_IsClosing			= 0;
	m_iCloseFlashLevel_IsClosing			= 0;
	
	m_iOpenDoorResetLevel_IsClosing		= 0;


	m_iNoLevel_IsClosing					= 0;

	m_iHighLevelCloseHighLevel_IsClosing	= 0;
	m_iLowLevelCloseHighLevel_IsClosing	= 0;
	m_iHighLevelCloseLowLevel_IsClosing	= 0;
	m_iLowLevelCloseLowLevel_IsClosing	= 0;
	
	m_iNoPTCLevel_IsClosing				= 0;

	
	if(!this->IsWindowVisible()) 
	{
		return;
	}

	
//
//	CheckFailCloseFlash(&G_TakeDlgSwitchTestResult[m_BoxNr],m_BoxNr);

	CRect w_rect;
	this->GetClientRect(w_rect); 
	#define PIC_GAP 5
	int SWITCH_PIC_HIGH		=	(w_rect.bottom - w_rect.top)		/ G_iLedLines[m_BoxNr] - PIC_GAP;
	int SWITCH_PIC_WIDTH	= 	(w_rect.right  - w_rect.left - 160) / G_iLedNrPerLine[m_BoxNr] - PIC_GAP;
	#define TEMP_STRING_HEIGHT	60

	this->GetWindowRect(w_rect); 
	#define PIC_GAP 5

	int nWidth = SWITCH_PIC_WIDTH;
    int nHeight = SWITCH_PIC_HIGH;

	CDC *dc = NULL;
	
	if(!this->IsWindowVisible()) 
	{
		return;
	}


	//CBitmap所需资源
//	CBitmap bitmap;

	    dc=this->GetDC();
		
		if(dc != NULL)
		{
			if(MemDC.GetSafeHdc() != NULL)
			{
				MemDC.DeleteDC();
			}
			
			if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
			{
				if(dc->GetSafeHdc() != NULL)
				{
					ReleaseDC(dc);
				}
				return;
			}
		}
		else
		{
			return;
		}
		CBitmap b;
		bitmap.DeleteObject();

		if(!bitmap.CreateCompatibleBitmap(dc,(nWidth + PIC_GAP)*G_iLedNrPerLine[m_BoxNr],(nHeight+PIC_GAP) * G_iLedLines[m_BoxNr]))	//建立与显示设备兼容的位图
		{
			if(dc->GetSafeHdc() != NULL)
			{
				ReleaseDC(dc);
			}
			return;
		}

		pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

		//RGB(0, 160, 128), RGB(255, 255, 60)
		CBrush bbrush(COLORREF(RGB(0,160,128)));

		MemDC.FillRect(CRect(0,0,(nWidth + PIC_GAP)*G_iLedNrPerLine[m_BoxNr],(nHeight+PIC_GAP) * G_iLedLines[m_BoxNr]),&bbrush);

//		dc->BitBlt(0,0,(nWidth + PIC_GAP)*G_iLedNrPerLine[m_BoxNr],(nHeight+PIC_GAP) * G_iLedLines[m_BoxNr],&MemDC,0,0,SRCCOPY); 	//显示原图形

		int count = 0;

		for(int i=0; i < G_iMaxLedNr[m_BoxNr] / 8 ; i++)
		{
			count = 0;
			for(int j = 0;j < 8; j++)
			{
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][i* 8 + j].IsCloseFlash)
				{
					count ++;
				}
			}
			/*
			if( count >= 4)
			{
				for(int j = 0;j < 8; j++)
				{
					G_TakeDlgSwitchTestResult[m_iTakeBoardType][i* 8 + j].IsCloseFlash = false;
				}
			}*/

			count = 0;
			for(int j = 0;j < 8; j++)
			{
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][i* 8 + j].IsOpenFlash)
				{
					count ++;
				}
			}
			/*
			if( count >= 4)
			{
				for(int j = 0;j < 8; j++)
				{
					G_TakeDlgSwitchTestResult[m_iTakeBoardType][i* 8 + j].IsOpenFlash = false;
				}
			}*/
		}



	for(ii=0; ii < G_iMaxLedNr[m_BoxNr] ; ii++)
	{
		if(::G_NormalConfigPara[m_BoxNr].BoxType == HEAT_BOX_TYPE_128_OLD_DISPLAY)
		{
			int old_id[128] = {
				 0, 1, 2, 3,  4,  5,  6,  7,104,105,
				 8, 9,10,11, 12, 13, 14, 15,106,107,
				16,17,18,19, 20, 21, 22, 23,108,109,
				24,25,26,27, 28, 29, 30, 31,110,111,
				32,33,34,35, 36, 37, 38, 39,112,113,
				40,41,42,43, 44, 45, 46, 47,114,115,
				48,49,50,51, 52, 53, 54, 55,116,117,
				56,57,58,59, 60, 61, 62, 63,118,119,
				64,65,66,67, 68, 69, 70, 71,120,121,
				72,73,74,75, 76, 77, 78, 79,122,123,
				80,81,82,83, 84, 85, 86, 87,124,125,
				88,89,90,91, 92, 93, 94, 95,126,127,
				96,97,98,99,100,101,102,103};
			id = old_id[ii];
			CString s;
			s.Format(_T("%d"),id); 
		//AfxMessageBox(s);
		}
		else
		{
			id = ii;
		}

		//clear 
//		if(bDraw)
//		{
//			DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
//		}
		//hasShowed = false;

		if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp > this->m_fOpenTempShow)
		{
			continue;
		}

//不合格的先选出来
		if(m_BoxType == ::BOX_DOUBLE_SWITCH)
		{
			int Ex_Nr[] = {10,11,12,13,14,15,16,17,18,19,
				0,1,2,3,4,5,6,7,8,9,
				30,31,32,33,34,35,36,37,38,39,
				20,21,22,23,24,25,26,27,28,29,

				50,51,52,53,54,55,56,57,58,59,
				40,41,42,43,44,45,46,47,48,49,

				70,71,72,73,74,75,76,77,78,79,
				60,61,62,63,64,65,66,67,68,69,

				90,91,92,93,94,95,96,97,98,99,
				80,81,82,83,84,85,86,87,88,89,

				110,111,112,113,114,115,116,117,118,119,
				100,101,102,103,104,105,106,107,108,109,

				121,120,123,122,125,124,127,126


			};
			m_SwitchTestResult_Ex = G_TakeDlgSwitchTestResult[m_iTakeBoardType][Ex_Nr[id]];
		}
		
	/*
		NO_LEVEL						= 0,
	MAIN_LEVEL						= 1,
    TEST_RESULT_HIGH_LEVEL						= 2,
    LOW_LEVEL						= 3,
	OVER_HIGH_LEVEL					= 4,
    OVER_LOW_LEVEL					= 5,
	CLOSE_HIGH_LEVEL				= 6,
	CLOSE_LOW_LEVEL					= 7,
	*/
//不用的
		TEST_RESULT_LEVEL level = CheckTestLevel_BOX_128_ONLY_OPENTEMP_TEST(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType);
		
		switch(level)
		{
		default:
			{
				int i = level;
				CString s;
				s.Format(_T("%d"),i);
				AfxMessageBox(s);
			}
			break;
		case CLOSE_FLASH_LEVEL:
			if(m_bCloseFlashLevel  && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,CLOSE_FLASH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iCloseFlashLevel++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iCloseFlashLevel_IsClosing ++;
				}
			break;
		case OPEN_FLASH_LEVEL:
				if(m_bOpenFlashLevel  && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OPEN_FLASH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iOpenFlashLevel++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iOpenFlashLevel_IsClosing ++;
				}
	
			break;
		case OPEN_DOOR_RESET_LEVEL:
				if(m_bOpenDoorResetLevel  && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OPEN_DOOR_RESET_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iOpenDoorResetLevel++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iOpenDoorResetLevel_IsClosing ++;
				}
	
			break;
		case HIGH_HIGH_LEVEL:
				if(m_bHighHighLevel  && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,HIGH_HIGH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iHighHighLevel++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iHighHighLevel_IsClosing ++;
				}
	
			break;
		case NOT_USED: //12
				if(m_bNotUsed && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,NOT_USED,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iNotUsed ++;

				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iNotUsed_IsClosing ++;
				}

		break;
		




//不动作品
		case OVER_HIGH_LEVEL: //4
				if(m_bOverHighLevel  && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OVER_HIGH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iOverHighLevel++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iOverHighLevel_IsClosing ++;
				}
		break;
		
//复大品
		case CLOSE_HIGH_LEVEL: //6
				if(m_bCloseHighLevel && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,CLOSE_HIGH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iCloseHighLevel ++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iCloseHighLevel_IsClosing ++;
				}
		break;
		

//有用产品			
			
				
//主规格
		case MAIN_LEVEL: //1
				if(m_bMainLevel && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,MAIN_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iMainLevel++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iMainLevel_IsClosing ++;
				}
		break;	
//低规格
		case LOW_LEVEL: //3
				if(m_bLowLevel && bDraw)
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp, LOW_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iLowLevel++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iLowLevel_IsClosing ++;
				}
		break;
			
//高规格
		case TEST_RESULT_HIGH_LEVEL: //2
				if(m_bHighLevel  && bDraw && IsValidTemp(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp))
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,TEST_RESULT_HIGH_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iHighLevel ++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iHighLevel_IsClosing ++;
				}
		break;
			
				
//低温品			
		case OVER_LOW_LEVEL: //5
				if(m_bOverLowLevel  && bDraw && IsValidTemp(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp))
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OVER_LOW_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iOverLowLevel ++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iOverLowLevel_IsClosing ++;
				}
			break;
		
//高低复大小
		case NO_LEVEL: //0
				if(m_bNoLevel  && bDraw )
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,NO_LEVEL,true,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				else
				{
					DrawOneSwitch(ii,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTime,level_text);
				}
				m_iNoLevel ++;
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].IsClosing)
				{
					m_iNoLevel_IsClosing ++;
				}
		break;

		
		}
		
	}

		if(bDraw)
	{
		CRect w_rect;
		this->GetClientRect(w_rect); 
		#define PIC_GAP 5
		int SWITCH_PIC_HIGH	=	(w_rect.bottom - w_rect.top) / G_iLedLines[m_BoxNr] - PIC_GAP;
		int SWITCH_PIC_WIDTH = 	(w_rect.right  - w_rect.left - 160) / G_iLedNrPerLine[m_BoxNr] - PIC_GAP;
		#define TEMP_STRING_HEIGHT	60

		this->GetWindowRect(w_rect); 
		#define PIC_GAP 5

		int nWidth = SWITCH_PIC_WIDTH;
		int nHeight = SWITCH_PIC_HIGH;

		//dc->BitBlt((nWidth +PIC_GAP) * (switchNr % G_iLedNrPerLine[m_BoxNr]),(nHeight+PIC_GAP) * (switchNr / G_iLedNrPerLine[m_BoxNr]),nWidth,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
		CBrush tempBrush(RGB(255,255,0));

		if(G_iMaxLedNr[m_BoxNr] == 192)
		{
			MemDC.FillRect(CRect((nWidth +PIC_GAP) * (6 % G_iLedNrPerLine[m_BoxNr]),0,(nWidth +PIC_GAP) * (6 % G_iLedNrPerLine[m_BoxNr])-PIC_GAP,(nHeight+PIC_GAP)*16), &tempBrush);
			MemDC.FillRect(CRect(0,(nHeight+PIC_GAP) * 8, (nWidth +PIC_GAP) * 12-PIC_GAP,(nHeight+PIC_GAP) * 8-PIC_GAP), &tempBrush);
		}
		else
		{
			MemDC.FillRect(CRect((nWidth +PIC_GAP) * (5 % G_iLedNrPerLine[m_BoxNr]),0,(nWidth +PIC_GAP) * (5 % G_iLedNrPerLine[m_BoxNr])-PIC_GAP,(nHeight+PIC_GAP)*13), &tempBrush);
			if(::G_NormalConfigPara[0].BoxType == BOX_OIL_BATHS)
			{
			}
			else
			{
				MemDC.FillRect(CRect(0,(nHeight+PIC_GAP) * 6, (nWidth +PIC_GAP) * 10-PIC_GAP,(nHeight+PIC_GAP) * 6-PIC_GAP), &tempBrush);
			}
		}

		dc->BitBlt(0,0,(nWidth + PIC_GAP)*G_iLedNrPerLine[m_BoxNr],(nHeight+PIC_GAP) * G_iLedLines[m_BoxNr],&MemDC,0,0,SRCCOPY); 	//显示原图形


		this->UpdateData(false); 
	}
//return;
	CWnd *w,*w1;

	//m_iNotUsed = 1;
	w	= GetDlgItem(IDC_EDIT_NO_ACTION_LEVEL);
	w1	= GetDlgItem(IDC_CHECK_NO_ACTION);
	if(m_iNotUsed == 0 )
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iMainLevel = 1;
	w = GetDlgItem(IDC_EDIT_MAIN_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_MAIN_LEVEL);
	if(m_iMainLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iLowLevel = 1;
	w = GetDlgItem(IDC_EDIT_LOW_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_LOW_LEVEL);
	if(m_iLowLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iHighLevel =1;
	w = GetDlgItem(IDC_EDIT_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_HIGH_LEVEL);
	if(m_iHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iHighHighLevel = 1;
	w = GetDlgItem(IDC_EDIT_HIGH_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_HIGH_HIGH_LEVEL);
	if(m_iHighHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iOverHighLevel = 1;
	w = GetDlgItem(IDC_EDIT_OVER_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_OVER_HIGH_LEVEL);
	if(m_iOverHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iOverLowLevel = 1;
	w = GetDlgItem(IDC_EDIT_OVER_LOW_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_OVER_LOW_LEVEL);
	if(m_iOverLowLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}
	
	//m_iCloseHighLevel = 1;
	w = GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_CLOSE_HIGH_LEVEL);
	if(m_iCloseHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iCloseLowLevel = 1;
	w = GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_CLOSE_LOW_LEVEL);
	if(m_iCloseLowLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iOpenFlashLevel = 1;
	w = GetDlgItem(IDC_EDIT_OPEN_FLASH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_OPEN_FLASH);
	if(m_iOpenFlashLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	//m_iCloseFlashLevel = 1;
	w = GetDlgItem(IDC_EDIT_CLOSE_FLASH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_CLOSE_FLASH);
	if(m_iCloseFlashLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	
	//m_iOpenDoorResetLevel = 1;
	w = GetDlgItem(IDC_EDIT_OPEN_DOOR_RESET);
	w1 = GetDlgItem(IDC_CHECK_OPEN_DOOR_RESET);
	if(m_iOpenDoorResetLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);

	}

	
	//m_iNoLevel = 1;
	w = GetDlgItem(IDC_EDIT_NO_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_NO_LEVEL);
	if(m_iNoLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}


	w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_HIGH_LEVEL_CLOSE_HIGH_LEVEL);
	if(m_iHighLevelCloseHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_LOW_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_HIGH_LEVEL_CLOSE_LOW_LEVEL);
	if(m_iHighLevelCloseLowLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);

	}

	w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_HIGH_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_LOW_LEVEL_CLOSE_HIGH_LEVEL);
	if(m_iLowLevelCloseHighLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_LOW_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_LOW_LEVEL_CLOSE_LOW_LEVEL);
	if(m_iLowLevelCloseLowLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	w = GetDlgItem(IDC_EDIT_NO_PTC_LEVEL);
	w1 = GetDlgItem(IDC_CHECK_NO_PTC_LEVEL);
	if(m_iNoPTCLevel == 0)
	{
		w->ShowWindow(SW_HIDE);
		w1->EnableWindow(false);
	}
	else
	{
		w->ShowWindow(SW_SHOW);
		w1->EnableWindow(true);
	}

	this->m_iFailHighLevel	= this->m_iOverHighLevel;
	this->m_iFailLowLevel	= this->m_iOverLowLevel;
	this->m_iFailOtherLevel = 	
	m_iCloseHighLevel +	m_iCloseLowLevel +	
	m_iOpenFlashLevel +	m_iCloseFlashLevel +	
	m_iOpenDoorResetLevel +	m_iNoLevel +
	m_iHighLevelCloseHighLevel +	m_iLowLevelCloseHighLevel +
	m_iHighLevelCloseLowLevel +	m_iLowLevelCloseLowLevel +	
	m_iNoPTCLevel;

	
	this->UpdateData(false);

	

		w = GetDlgItem(IDC_EDIT_OTHER_FAIL_LEVEL_TAKE);
		s.Format(_T("%d"), m_iFailOtherLevel_Take[m_BoxNr]);
		w->SetWindowTextW(s);

		w = GetDlgItem(IDC_EDIT_HIGH_FAIL_LEVEL_TAKE);
		s.Format(_T("%d"), G_TakeoutDlg.m_iFailHighLevel_Take[m_BoxNr]);
		w->SetWindowTextW(s);

		w = GetDlgItem(IDC_EDIT_LOW_FAIL_LEVEL_TAKE);
		s.Format(_T("%d"), G_TakeoutDlg.m_iFailLowLevel_Take[m_BoxNr]);
		w->SetWindowTextW(s);

//	return;
#ifndef _DEBUG
	if(dc->GetSafeHdc())
	{
		dc->DeleteDC(); 
	}
#endif
}

CBrush blackbrush(COLORREF(RGB(0,0,0)));
CBrush whitebrush(COLORREF(RGB(0xFF,0xFF,0xFF)));
//CBrush rbrush(COLORREF(RGB(0xFF,0,0)));
CBrush gbrush(COLORREF(RGB(0,0xFF,0)));
CBrush bbrush(COLORREF(RGB(0,0,0xFF)));
CFont  TempFont,NrFont;

void CTakeoutDlg::DrawOneSwitch(int switchNr, float openTemp, float closeTemp,TEST_RESULT_LEVEL level,bool bDraw,long OpenTime, bool level_text)
{

	if(switchNr == 20)
	{
		int nWidth	=	3;
	}

	CRect w_rect;
	this->GetClientRect(w_rect); 
	#define PIC_GAP 5
	int SWITCH_PIC_HIGH	=	(w_rect.bottom - w_rect.top) / G_iLedLines[m_BoxNr] - PIC_GAP;
	int SWITCH_PIC_WIDTH = 	(w_rect.right  - w_rect.left - 160) / G_iLedNrPerLine[m_BoxNr] - PIC_GAP;
	#define TEMP_STRING_HEIGHT	60

//	this->GetWindowRect(w_rect); 
	#define PIC_GAP 5

	int nWidth	=	SWITCH_PIC_WIDTH;
    int nHeight =	SWITCH_PIC_HIGH;
	int nTopX	=	(nWidth +PIC_GAP) * (switchNr % G_iLedNrPerLine[m_BoxNr]);
	int nTopY	=	(nHeight+PIC_GAP) * (switchNr / G_iLedNrPerLine[m_BoxNr]);


	if(::G_NormalConfigPara[this->m_BoxNr].BoxType == BOX_OIL_BATHS)
	{
		int x,y;
		int r = w_rect.Height() / 5;
		CPoint Circle_center((w_rect.Width() - 160)/2,w_rect.Height()/3);

		

		nHeight =	SWITCH_PIC_HIGH * 0.7 ;
		if(switchNr < 5)
		{
			
			r = w_rect.Height() / 5;
			int h = r - r * sin((-15.0f+(9-0)*210.0f/9)/180*3.1415926);
			int start_h = Circle_center.y + r * sin((-15.0f+(9-0)*210.0f/9)/180*3.1415926);

			
				x = Circle_center.x + r * cos((-15.0f+(9-switchNr)*210.0f/9)/180*3.1415926);
				y = start_h + h / 4  * switchNr;

				nTopX = x - (nWidth +PIC_GAP);
				nTopY =	y;
				
		
		}
		else if(switchNr < 10)
		{
		
			r = w_rect.Height() / 5;
			int h = r - r * sin((-15.0f+(9-0)*210.0f/9)/180*3.1415926);
			int start_h = Circle_center.y + r * sin((-15.0f+(9-0)*210.0f/9)/180*3.1415926);

			
			{
				x = Circle_center.x + r * cos((-15.0f+(9-switchNr)*210.0f/9)/180*3.1415926);
				y = start_h + h / 4  * (9 - switchNr);

				nTopX = x;
				nTopY =	y;
				
			}
		}
		else if(switchNr < 20)
		{
			
			r = w_rect.Height() / 5 * 2;
			int h = r - r * sin((-15.0f+(19-0)*210.0f/19)/180*3.1415926);
			int start_h = Circle_center.y + r * sin((-15.0f+(19-0)*210.0f/19)/180*3.1415926);

			
				x = Circle_center.x - r * cos((-15.0f+(switchNr - 10)*210.0f/19)/180*3.1415926);
				y = start_h + h / 10  * (switchNr - 10);

				nTopX = x - (nWidth +PIC_GAP);
				nTopY =	y;
				
		
		}
		else if(switchNr < 30)
		{
		
			r = w_rect.Height() / 5 * 2;
			int h = r - r * sin((-15.0f+(19-0)*210.0f/19)/180*3.1415926);
			int start_h = Circle_center.y + r * sin((-15.0f+(19-0)*210.0f/19)/180*3.1415926);

			
			{
				x = Circle_center.x + r * cos((-15.0f+(29-switchNr)*210.0f/19)/180*3.1415926);
				y = start_h + h / 10  * (29 - switchNr);

				nTopX = x;
				nTopY =	y;
				
			}
		}
		else if(switchNr < 45)
		{

			r = w_rect.Height() / 5 * 3;
			int h = r - r * sin((-15.0f+(29-0)*210.0f/29)/180*3.1415926);
			int start_h = Circle_center.y + r * sin((-15.0f+(29-0)*210.0f/29)/180*3.1415926);

			
				x = Circle_center.x - r * cos((-15.0f+(switchNr - 30)*210.0f/29)/180*3.1415926);
				y = start_h + h / 15  * (switchNr - 30);

				nTopX = x - (nWidth +PIC_GAP);
				nTopY =	y;
				
		
		}
		else if(switchNr < 60)
		{
		
			r = w_rect.Height() / 5 * 3;
			int h = r - r * sin((-15.0f+(29-0)*210.0f/29)/180*3.1415926);
			int start_h = Circle_center.y + r * sin((-15.0f+(29-0)*210.0f/29)/180*3.1415926);

			
			{
				x = Circle_center.x + r * cos((-15.0f+(59-switchNr)*210.0f/29)/180*3.1415926);
				y = start_h + h / 15  * (59 - switchNr);

				nTopX = x;
				nTopY =	y;
				
			}
		}
		else
		{
			return;
		}

	}

	

	

	CPen	pen;
	CPen*	oldpen;

	
	if(!this->IsWindowVisible()) 
	{
		return;
	}


	

	pen.CreatePen(PS_SOLID,3, RGB(0,0,0)); 			//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象

	
	CRect rect(nTopX,nTopY,nTopX+SWITCH_PIC_WIDTH,nTopY+SWITCH_PIC_HIGH);
	CRect center_rect(nTopX+SWITCH_PIC_WIDTH/5,nTopY+SWITCH_PIC_HIGH/4,nTopX+SWITCH_PIC_WIDTH*4/5,nTopY+SWITCH_PIC_HIGH*3/4);

	
	//绘制工具资源

	//背景,不同等级用不同的颜色
	//static int i = 0;
	switch(level)
	{
	case MAIN_LEVEL:
	case TEST_RESULT_HIGH_LEVEL:
	case LOW_LEVEL:
	case OVER_HIGH_LEVEL:
	case OVER_LOW_LEVEL:
	case CLOSE_HIGH_LEVEL:
	case CLOSE_LOW_LEVEL:
	case OPEN_FLASH_LEVEL:
	case CLOSE_FLASH_LEVEL:
	case OPEN_DOOR_RESET_LEVEL:
	case NOT_USED:
    case HIGH_HIGH_LEVEL:
	case NO_LEVEL:
//    case HIGH_LEVEL_CLOSE_HIGH_LEVEL:
    case HIGH_LEVEL_CLOSE_LOW_LEVEL:
    case LOW_LEVEL_CLOSE_HIGH_LEVEL:
    case LOW_LEVEL_CLOSE_LOW_LEVEL:
	case NO_PTC_LEVEL:
		MemDC.FillSolidRect(CRect(nTopX,nTopY, nTopX+nWidth,nTopY+nHeight),RGB(0xB0,0x00,0x00));
		break;
	default:
		MemDC.FillSolidRect(CRect(nTopX,nTopY, nTopX+nWidth,nTopY+nHeight),RGB(0,0,0));
		break;
	}

	MemDC.SelectObject(oldpen);
	pen.DeleteObject(); 
	


	bool same_to_selected_level = false;
	switch(level)
	{
	case MAIN_LEVEL:
		if(m_bMainLevel)
			same_to_selected_level = true;
		break;
	case TEST_RESULT_HIGH_LEVEL:
		if(m_bHighLevel)
			same_to_selected_level = true;
		break;
	case LOW_LEVEL:
		if(m_bLowLevel)
			same_to_selected_level = true;
		break;
	case OVER_HIGH_LEVEL:
		if(m_bOverHighLevel)
			same_to_selected_level = true;
		break;
	case OVER_LOW_LEVEL:
		if(m_bOverLowLevel)
			same_to_selected_level = true;
		break;
	case CLOSE_HIGH_LEVEL:
		if(m_bCloseHighLevel)
			same_to_selected_level = true;
		break;
	case CLOSE_LOW_LEVEL:
		if(m_bCloseLowLevel)
			same_to_selected_level = true;
		break;
	case OPEN_FLASH_LEVEL:
		if(m_bOpenFlashLevel)
			same_to_selected_level = true;
		break;
	case CLOSE_FLASH_LEVEL:
		if(m_bCloseFlashLevel)
			same_to_selected_level = true;
		break;
	case OPEN_DOOR_RESET_LEVEL:
		if(m_bOpenDoorResetLevel)
			same_to_selected_level = true;
		break;
	case NOT_USED:
		if(m_bNotUsed)
			same_to_selected_level = true;
		break;
	case NO_LEVEL:
		if(m_bNoLevel)
			same_to_selected_level = true;
		break;
	case HIGH_HIGH_LEVEL:
		if(m_bHighHighLevel)
			same_to_selected_level = true;
		break;
	case HIGH_LEVEL_CLOSE_HIGH_LEVEL:
		if(m_bHighLevelCloseHighLevel)
			same_to_selected_level = true;
		break;
	case HIGH_LEVEL_CLOSE_LOW_LEVEL:
		if(m_bHighLevelCloseLowLevel)
			same_to_selected_level = true;
		break;
	case LOW_LEVEL_CLOSE_HIGH_LEVEL:
		if(m_bLowLevelCloseHighLevel)
			same_to_selected_level = true;
		break;
	case LOW_LEVEL_CLOSE_LOW_LEVEL:
		if(m_bLowLevelCloseLowLevel)
			same_to_selected_level = true;
		break;
	case NO_PTC_LEVEL:
		if(m_bNoPTCLevel)
			same_to_selected_level = true;
		break;
	
	}

	CString wndTxt;
	this->GetWindowText(wndTxt);


//画 ×	
	
	if(same_to_selected_level == true)
	{
		if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsUsed == true 
			&& G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosing == false 
			)
		{
			MemDC.FillSolidRect(CRect(nTopX,nTopY, nTopX+nWidth,nTopY+nHeight),RGB(0x00,0xE0,0x00));
		}
	}
	else
	{
		if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosing == false 
			&& G_TakeDlgSwitchTestResult_Bak[m_iTakeBoardType][switchNr].IsClosing == true )
		{
			CString s;
			s.Format(_T("%d号错误"), switchNr + 1);
			if(G_SpeechArray.GetSize() < 10)
			{
				::G_SpeechArray.Add(s);
			}

			if(pen.GetSafeHandle())
			{
				pen.DeleteObject(); 
			}
			pen.CreatePen(NULL,5,RGB(0xFF,0x00,0x00)); 
			oldpen = MemDC.SelectObject(&pen); 
			MemDC.MoveTo(CPoint(nTopX,nTopY)); 
			MemDC.LineTo(CPoint(nTopX+nWidth,nTopY+nHeight)); 
			MemDC.MoveTo(CPoint(nTopX+nWidth,nTopY)); 
			MemDC.LineTo(CPoint(nTopX,nTopY+nHeight));
			MemDC.SelectObject(oldpen); 
			pen.DeleteObject(); 
			
		}
	}


	{
		CString msg;
		CSize size;


		//CFont font;
		CFont *oldfont = NULL;

		int fontHeight = nHeight*10;

//open temp
		if(TempFont.GetSafeHandle() == NULL)
		{
			while(1)
			{
				TempFont.CreatePointFont(fontHeight,_T("黑体"),&MemDC);
				oldfont = MemDC.SelectObject(&TempFont); 
				size = MemDC.GetTextExtent(_T("188.8"));
				if(size.cx > (nWidth/2))
				{
					fontHeight -= 10;
					TempFont.DeleteObject(); 
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&TempFont);
		}

		MemDC.SetBkMode(TRANSPARENT);
		MemDC.SetTextColor(RGB(0,0,0));
		
		
		
		msg.Format(_T("%.1f"),openTemp);
		
		
	
		if(level != NOT_USED && level != ERROR_LEVEL
			/*&& level != OVER_HIGH_LEVEL*/ )
			
		{
			if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosing == true) 
			{
				MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF)); 
			}
			else
			{
				MemDC.SetTextColor(RGB(0x00,0x00,0x00)); 
			}
			size = MemDC.GetTextExtent(msg);
			if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsOpenned)
			{
				MemDC.TextOut(nTopX+nWidth-size.cx, nTopY,msg);
			}
		}
		

	//复位温度
		if(m_iDisplayTempType == 1)
		{
			if(level == CLOSE_LOW_LEVEL 
				|| level == HIGH_LEVEL_CLOSE_LOW_LEVEL
				|| level == LOW_LEVEL_CLOSE_LOW_LEVEL)
			{
				if(::IsValidTemp( closeTemp))
				{
					msg.Format(_T("▲%.1f"),openTemp - closeTemp);
				}
				else
				{
					msg = "";
				}
			}
			else
			{
				msg.Format(_T("▲%.1f"),openTemp - closeTemp);
			}
		}
		else if(m_iDisplayTempType == 2)
		{
			if(level == CLOSE_LOW_LEVEL 
				|| level == HIGH_LEVEL_CLOSE_LOW_LEVEL
				|| level == LOW_LEVEL_CLOSE_LOW_LEVEL)
			{
				if(::IsValidTemp( closeTemp))
				{
					msg.Format(_T("%.1f▲%.1f"),closeTemp,openTemp - closeTemp);
				}
				else
				{
					msg = "";
				}
			}
			else
			{
				msg.Format(_T("%.1f▲%.1f"),closeTemp,openTemp - closeTemp);
			}
		}
		else
		{
			if(m_iTestMode == TEST_MODE_NORMAL_PID_CAL_OPEN_TIME)
			{
				msg.Format(_T("%d秒"),OpenTime);
			}
			else
			{
				msg.Format(_T("%.1f"),closeTemp);
			}
		}

		if(level != NOT_USED && level != ERROR_LEVEL
			/*&& level != OVER_HIGH_LEVEL*/ )
		{
			if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosing == true) 
			{
				MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF)); 
			}
			else
			{
				MemDC.SetTextColor(RGB(0x00,0x00,0x00)); 
			}
			
			size = MemDC.GetTextExtent(msg);
			if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosed)
			{
				MemDC.TextOut(nTopX+nWidth - size.cx, nTopY+nHeight - size.cy,msg);
			}
		}
		

//nr	
		
		if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosing == true) 
		{
			MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF)); 
		}
		else
		{
			MemDC.SetTextColor(RGB(0x00,0x00,0x00)); 
		}

		if(G_iMaxLedNr[m_BoxNr] == 192)
		{
			if(((switchNr + 1) % G_iLedNrPerLine[m_BoxNr]) == 0)
			{
				msg.Format(_T("%dB"),(switchNr + 1) / G_iLedNrPerLine[m_BoxNr]);
			}
			else if(((switchNr + 1) % G_iLedNrPerLine[m_BoxNr]) < 10)
			{
				msg.Format(_T("%d%d"),(switchNr+1)/G_iLedNrPerLine[m_BoxNr],(switchNr+1)%G_iLedNrPerLine[m_BoxNr]);
			}
			else if(((switchNr + 1) % G_iLedNrPerLine[m_BoxNr]) == 10)
			{
				msg.Format(_T("%d0"),(switchNr+1)/G_iLedNrPerLine[m_BoxNr]+1);
			}
			else if(((switchNr + 1) % G_iLedNrPerLine[m_BoxNr]) == 11)
			{
				msg.Format(_T("%dA"),(switchNr + 1) / G_iLedNrPerLine[m_BoxNr] + 1);
			}
		}
		else
		{
			msg.Format(_T("%02d"),((switchNr ) +1));
		}

		fontHeight = nHeight*10;
		if(NrFont.GetSafeHandle() == NULL)
		{
			while(1)
			{
				NrFont.CreatePointFont(fontHeight,_T("黑体"),&MemDC);
				oldfont = MemDC.SelectObject(&NrFont); 
				size = MemDC.GetTextExtent(_T("888"));
				if(size.cx > (nWidth/2 - 3))
				{
					fontHeight -= 10;
					NrFont.DeleteObject(); 
				}
				else
				{
					break;
				}
			}
		}
		else
		{
			oldfont = MemDC.SelectObject(&NrFont); 
		}
		if((level_text == false) || (level == ERROR_LEVEL))
		{
			if(same_to_selected_level == false)
			{
				
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosing == false 
				&& G_TakeDlgSwitchTestResult_Bak[m_iTakeBoardType][switchNr].IsClosing == true )
				{
					MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF));
				}
				else
				{
					msg = _T("");
				}
				
			}

			size = MemDC.GetTextExtent(msg);
			MemDC.TextOut(nTopX  , nTopY+(nHeight - size.cy)/2,msg);

			
		}
		else
		{
			size = MemDC.GetTextExtent(level_str[level]);
			MemDC.TextOut(nTopX, nTopY+(nHeight - size.cy)/10,level_str[level]);
		}

		if(oldfont != NULL)
		{
			MemDC.SelectObject(oldfont); 
		}
//		font.DeleteObject();
	}

	

	if(pen.GetSafeHandle())
	{
		pen.DeleteObject(); 
	}

}


void CTakeoutDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	

	// TODO: Add your message handler code here
	OnRadioAllSelect();

	this->ShowTestResult(true); 
	
}

BOOL CTakeoutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here

//	ShowWindow(SW_SHOWMAXIMIZED); 
	int cx = ::GetSystemMetrics(SM_CXSCREEN); 
	int cy = ::GetSystemMetrics(SM_CYSCREEN); 
	CString s;
	//s.Format(_T("%d,%d"),cx,cy);
	//AfxMessageBox(s);
	this->MoveWindow(5,30,cx-5,cy-60); 

	this->SetWindowText(m_WindowText); 
	


	{
		CRect w_rect,item_rect;
		this->GetClientRect(w_rect); 
		#define PIC_GAP 5
		int SWITCH_PIC_HIGH	=	(w_rect.bottom - w_rect.top) / G_iLedLines[m_BoxNr] - PIC_GAP;
		int SWITCH_PIC_WIDTH = 	(w_rect.right  - w_rect.left - 160) / G_iLedNrPerLine[m_BoxNr] - PIC_GAP;
		CWnd *w;
		int height;
		height = (w_rect.bottom - w_rect.top) / 36;

		item_rect.top = 10;
		
		if(::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_ONLY_OPENTEMP_TEST)
		{

			w = GetDlgItem(IDC_CHECK_CLOSE_FLASH);
			w->MoveWindow(w_rect.right - 160, item_rect.top,100,height);
			w->SetWindowTextW(_T("不动作"));

			w = GetDlgItem(IDC_CHECK_OPEN_DOOR_RESET);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			w->SetWindowTextW(_T("开门复位"));


			w = GetDlgItem(IDC_CHECK_OPEN_FLASH);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			w->SetWindowTextW(_T("闪动"));


			w = GetDlgItem(IDC_CHECK_MAIN_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,100,height);
			w->SetWindowTextW(_T("超低温"));
			
			w = GetDlgItem(IDC_CHECK_HIGH_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			w->SetWindowTextW(_T("低温"));

			

			w = GetDlgItem(IDC_CHECK_LOW_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			w->SetWindowTextW(_T("主规格"));
			
			w = GetDlgItem(IDC_CHECK_HIGH_HIGH_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			w->SetWindowTextW(_T("高规格1"));

			
			w = GetDlgItem(IDC_CHECK_OVER_HIGH_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			w->SetWindowTextW(_T("高规格2"));
			
			w = GetDlgItem(IDC_CHECK_OVER_LOW_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			w->SetWindowTextW(_T("高规格3"));
			
			
			
			
			w = GetDlgItem(IDC_CHECK_CLOSE_HIGH_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			w->SetWindowTextW(_T("高温"));
			//w->ShowWindow(SW_HIDE);
			
			w = GetDlgItem(IDC_CHECK_CLOSE_LOW_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			w->ShowWindow(SW_HIDE);
			
			
			//w->ShowWindow(SW_HIDE);
			
			
			//w->ShowWindow(SW_HIDE);
			


			w = GetDlgItem(IDC_CHECK_HIGH_LEVEL_CLOSE_HIGH_LEVEL);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_CHECK_HIGH_LEVEL_CLOSE_LOW_LEVEL);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_CHECK_LOW_LEVEL_CLOSE_HIGH_LEVEL);
			w->ShowWindow(SW_HIDE);
			
			w = GetDlgItem(IDC_CHECK_LOW_LEVEL_CLOSE_LOW_LEVEL);
			w->ShowWindow(SW_HIDE);



			w = GetDlgItem(IDC_CHECK_NO_PTC_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_CHECK_NO_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			w->ShowWindow(SW_HIDE);
			
			w = GetDlgItem(IDC_CHECK_NO_ACTION);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
			//w->ShowWindow(SW_HIDE);

			//
			item_rect.top = 10;
			w = GetDlgItem(IDC_EDIT_CLOSE_FLASH_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top ,30,height);
			//w->ShowWindow(SW_HIDE);
			w = GetDlgItem(IDC_EDIT_CLOSE_FLASH_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
			//w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_EDIT_OPEN_DOOR_RESET);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			w = GetDlgItem(IDC_EDIT_OPEN_DOOR_RESET2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);

			w = GetDlgItem(IDC_EDIT_OPEN_FLASH_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			//w->ShowWindow(SW_HIDE);
			w = GetDlgItem(IDC_EDIT_OPEN_FLASH_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
			//w->ShowWindow(SW_HIDE);


			w = GetDlgItem(IDC_EDIT_MAIN_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height  ,30,height);
			w = GetDlgItem(IDC_EDIT_MAIN_LEVEL3);
			w->MoveWindow(w_rect.right - 30, item_rect.top          ,30,height);

			w = GetDlgItem(IDC_EDIT_HIGH_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			w = GetDlgItem(IDC_EDIT_HIGH_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top,30,height);

			w = GetDlgItem(IDC_EDIT_LOW_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			w = GetDlgItem(IDC_EDIT_LOW_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);

			w = GetDlgItem(IDC_EDIT_HIGH_HIGH_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			w = GetDlgItem(IDC_EDIT_HIGH_HIGH_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
			
			w = GetDlgItem(IDC_EDIT_OVER_HIGH_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			w = GetDlgItem(IDC_EDIT_OVER_HIGH_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);

			
			w = GetDlgItem(IDC_EDIT_OVER_LOW_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			w = GetDlgItem(IDC_EDIT_OVER_LOW_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);

			
			
			
			w = GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			//w->ShowWindow(SW_HIDE);
			w = GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
			//w->ShowWindow(SW_HIDE);

			
			w = GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			w->ShowWindow(SW_HIDE);
			w = GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
			w->ShowWindow(SW_HIDE);
			
			
			
			
			
			
			
			

	
			w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_HIGH_LEVEL);
			w->ShowWindow(SW_HIDE);


			
			w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_LOW_LEVEL);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_HIGH_LEVEL);
			w->ShowWindow(SW_HIDE);
			
			w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_LOW_LEVEL);
			w->ShowWindow(SW_HIDE);

	
			w = GetDlgItem(IDC_EDIT_NO_PTC_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			w->ShowWindow(SW_HIDE);
			w = GetDlgItem(IDC_EDIT_NO_PTC_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
			w->ShowWindow(SW_HIDE);
			

			w = GetDlgItem(IDC_EDIT_NO_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			w->ShowWindow(SW_HIDE);
			w = GetDlgItem(IDC_EDIT_NO_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
			w->ShowWindow(SW_HIDE);
			
			
			w = GetDlgItem(IDC_EDIT_NO_ACTION_LEVEL);
			w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
			w = GetDlgItem(IDC_EDIT_NO_ACTION_LEVEL2);
			w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
			
			w = GetDlgItem(IDC_RADIO_ONLY_ONE);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height,50,height);


			w = GetDlgItem(IDC_RADIO_MULTI_SELECT);
			w->MoveWindow(w_rect.right - 110, item_rect.top,50,height);
			
			w = GetDlgItem(IDC_RADIO_ALL_SELECT);
			w->MoveWindow(w_rect.right - 60, item_rect.top ,50,height);

	#ifdef _CHANGHONG_		
			
			w = GetDlgItem(IDC_STATIC_MSG);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height*1.5);
			w = GetDlgItem(IDC_BUTTON_RECORD_VIEW2);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height*1.5 ,150,height);
			w = GetDlgItem(IDC_BUTTON_TEMP_VIEW);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
			w = GetDlgItem(IDC_BUTTON_SAVE_EXCEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
			w = GetDlgItem(IDC_BUTTON_PREVIEW);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);

	#else
			w = GetDlgItem(IDC_STATIC_MSG);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height*1.5);

			

			w = GetDlgItem(IDC_BUTTON_RECORD_VIEW2);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
			w = GetDlgItem(IDC_BUTTON_TEMP_VIEW);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
			w = GetDlgItem(IDC_BUTTON_SAVE_EXCEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
			w = GetDlgItem(IDC_BUTTON_PREVIEW);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
	#endif
			w = GetDlgItem(IDC_BUTTON_TODAY_CHANLIANG);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
			w = GetDlgItem(IDC_BUTTON_PRINT);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);

			CString s;
			w =GetDlgItem(IDC_STATIC_OPEN);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
			w =GetDlgItem(IDC_EDIT_OPEN);
			w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
			s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.MainLevel);
			w->SetWindowTextW(s);
			

			w =GetDlgItem(IDC_STATIC_OPEN_PLUS);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
			w =GetDlgItem(IDC_EDIT_OPEN_PLUS);
			w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
			s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.MainLevelSpanPlus);
			w->SetWindowTextW(s);

			w =GetDlgItem(IDC_STATIC_OPEN_MINUS);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
			w =GetDlgItem(IDC_EDIT_OPEN_MINUS);
			w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
			s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.MainLevelSpanMinus);
			w->SetWindowTextW(s);

			w =GetDlgItem(IDC_STATIC_CLOSE);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
			w =GetDlgItem(IDC_EDIT_CLOSE);
			w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
			s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.CloseLevel);
			w->SetWindowTextW(s);

			w =GetDlgItem(IDC_STATIC_CLOSE_PLUS);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
			w =GetDlgItem(IDC_EDIT_CLOSE_PLUS);
			w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
			s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.CloseLevelSpanPlus);
			w->SetWindowTextW(s);

			w =GetDlgItem(IDC_STATIC_CLOSE_MINUS);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
			w =GetDlgItem(IDC_EDIT_CLOSE_MINUS);
			w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
			s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.CloseLevelSpanMinus);
			w->SetWindowTextW(s);


			w = GetDlgItem(IDC_COMBO_GROUP_NR);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height*1.5 ,150,height*11);

			w = GetDlgItem(IDC_STATIC_HIGH_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height*1.5 ,150,height);
			w = GetDlgItem(IDC_EDIT_HIGH_FAIL_LEVEL);
			w->MoveWindow(w_rect.right - 80, item_rect.top ,40,height);
			w = GetDlgItem(IDC_EDIT_HIGH_FAIL_LEVEL_TAKE);
			w->MoveWindow(w_rect.right - 40, item_rect.top ,40,height);
			

			w = GetDlgItem(IDC_STATIC_LOW_LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height*1.5 ,150,height);
			w = GetDlgItem(IDC_EDIT_LOW_FAIL_LEVEL);
			w->MoveWindow(w_rect.right - 80, item_rect.top ,40,height);
			w = GetDlgItem(IDC_EDIT_LOW_FAIL_LEVEL_TAKE);
			w->MoveWindow(w_rect.right - 40, item_rect.top ,40,height);

			w = GetDlgItem(IDC_STATIC_OTHER_FAIL__LEVEL);
			w->MoveWindow(w_rect.right - 160, item_rect.top += height*1.5 ,150,height);
			w = GetDlgItem(IDC_EDIT_OTHER_FAIL_LEVEL);
			w->MoveWindow(w_rect.right - 80, item_rect.top ,40,height);
			w = GetDlgItem(IDC_EDIT_OTHER_FAIL_LEVEL_TAKE);
			w->MoveWindow(w_rect.right - 40, item_rect.top ,40,height);


			w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_HIGH_LEVEL2);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_LOW_LEVEL2);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_HIGH_LEVEL2);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_LOW_LEVEL2);
			w->ShowWindow(SW_HIDE);


		}
		else
		{

		w = GetDlgItem(IDC_CHECK_MAIN_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top          ,100,height);

		w = GetDlgItem(IDC_CHECK_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);

		w = GetDlgItem(IDC_CHECK_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);


		w = GetDlgItem(IDC_CHECK_HIGH_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		//w->ShowWindow(SW_HIDE);

		
		w = GetDlgItem(IDC_CHECK_OVER_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);

		
		w = GetDlgItem(IDC_CHECK_OVER_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);

		
		w = GetDlgItem(IDC_CHECK_OPEN_DOOR_RESET);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);

		
		w = GetDlgItem(IDC_CHECK_CLOSE_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);

		
		w = GetDlgItem(IDC_CHECK_CLOSE_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		
		
		w = GetDlgItem(IDC_CHECK_OPEN_FLASH);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		
		
		w = GetDlgItem(IDC_CHECK_CLOSE_FLASH);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		
		

#ifdef _CHANGHONG_
		w = GetDlgItem(IDC_CHECK_HIGH_LEVEL_CLOSE_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);

		w = GetDlgItem(IDC_CHECK_HIGH_LEVEL_CLOSE_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);

		w = GetDlgItem(IDC_CHECK_LOW_LEVEL_CLOSE_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		
		w = GetDlgItem(IDC_CHECK_LOW_LEVEL_CLOSE_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);

#else
		w = GetDlgItem(IDC_CHECK_HIGH_LEVEL_CLOSE_HIGH_LEVEL);
		w->ShowWindow(SW_HIDE);

		w = GetDlgItem(IDC_CHECK_HIGH_LEVEL_CLOSE_LOW_LEVEL);
		w->ShowWindow(SW_HIDE);

		w = GetDlgItem(IDC_CHECK_LOW_LEVEL_CLOSE_HIGH_LEVEL);
		w->ShowWindow(SW_HIDE);
		
		w = GetDlgItem(IDC_CHECK_LOW_LEVEL_CLOSE_LOW_LEVEL);
		w->ShowWindow(SW_HIDE);

#endif

		w = GetDlgItem(IDC_CHECK_NO_PTC_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		

		w = GetDlgItem(IDC_CHECK_NO_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		
		
		w = GetDlgItem(IDC_CHECK_NO_ACTION);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		
		item_rect.top = 10;
		
		w = GetDlgItem(IDC_EDIT_MAIN_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top          ,30,height);
		w = GetDlgItem(IDC_EDIT_MAIN_LEVEL3);
		w->MoveWindow(w_rect.right - 30, item_rect.top          ,30,height);

		w = GetDlgItem(IDC_EDIT_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_HIGH_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top,30,height);

		w = GetDlgItem(IDC_EDIT_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_LOW_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);

		w = GetDlgItem(IDC_EDIT_HIGH_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_HIGH_HIGH_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
		
		w = GetDlgItem(IDC_EDIT_OVER_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_OVER_HIGH_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);

		
		w = GetDlgItem(IDC_EDIT_OVER_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_OVER_LOW_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);

		
		w = GetDlgItem(IDC_EDIT_OPEN_DOOR_RESET);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_OPEN_DOOR_RESET2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);

		
		w = GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);

		
		w = GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
		
		
		w = GetDlgItem(IDC_EDIT_OPEN_FLASH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_OPEN_FLASH_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
		
		
		w = GetDlgItem(IDC_EDIT_CLOSE_FLASH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_CLOSE_FLASH_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
		
		

#ifdef _CHANGHONG_
		w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		
		w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);

		w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		
		w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
#else
		w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_HIGH_LEVEL);
		w->ShowWindow(SW_HIDE);

		
		w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_LOW_LEVEL);
		w->ShowWindow(SW_HIDE);

		w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_HIGH_LEVEL);
		w->ShowWindow(SW_HIDE);
		
		w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_LOW_LEVEL);
		w->ShowWindow(SW_HIDE);

#endif	

		w = GetDlgItem(IDC_EDIT_NO_PTC_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_NO_PTC_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
		

		w = GetDlgItem(IDC_EDIT_NO_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_NO_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
		
		
		w = GetDlgItem(IDC_EDIT_NO_ACTION_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,30,height);
		w = GetDlgItem(IDC_EDIT_NO_ACTION_LEVEL2);
		w->MoveWindow(w_rect.right - 30, item_rect.top ,30,height);
		
		w = GetDlgItem(IDC_RADIO_ONLY_ONE);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,50,height);


		w = GetDlgItem(IDC_RADIO_MULTI_SELECT);
		w->MoveWindow(w_rect.right - 110, item_rect.top,50,height);
		
		w = GetDlgItem(IDC_RADIO_ALL_SELECT);
		w->MoveWindow(w_rect.right - 60, item_rect.top ,50,height);

#ifdef _CHANGHONG_		
		
		w = GetDlgItem(IDC_STATIC_MSG);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height*1.5);
		w = GetDlgItem(IDC_BUTTON_RECORD_VIEW2);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height*1.5 ,150,height);
		w = GetDlgItem(IDC_BUTTON_TEMP_VIEW);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
		w = GetDlgItem(IDC_BUTTON_SAVE_EXCEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
		w = GetDlgItem(IDC_BUTTON_PREVIEW);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);

#else
		w = GetDlgItem(IDC_STATIC_MSG);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height*1.5);

		

		w = GetDlgItem(IDC_BUTTON_RECORD_VIEW2);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
		w = GetDlgItem(IDC_BUTTON_TEMP_VIEW);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
		w = GetDlgItem(IDC_BUTTON_SAVE_EXCEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
		w = GetDlgItem(IDC_BUTTON_PREVIEW);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
#endif
		w = GetDlgItem(IDC_BUTTON_TODAY_CHANLIANG);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
		w = GetDlgItem(IDC_BUTTON_PRINT);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);

		CString s;
		w =GetDlgItem(IDC_STATIC_OPEN);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
		w =GetDlgItem(IDC_EDIT_OPEN);
		w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
		s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.MainLevel);
		w->SetWindowTextW(s);
		

		w =GetDlgItem(IDC_STATIC_OPEN_PLUS);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
		w =GetDlgItem(IDC_EDIT_OPEN_PLUS);
		w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
		s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.MainLevelSpanPlus);
		w->SetWindowTextW(s);

		w =GetDlgItem(IDC_STATIC_OPEN_MINUS);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
		w =GetDlgItem(IDC_EDIT_OPEN_MINUS);
		w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
		s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.MainLevelSpanMinus);
		w->SetWindowTextW(s);

		w =GetDlgItem(IDC_STATIC_CLOSE);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
		w =GetDlgItem(IDC_EDIT_CLOSE);
		w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
		s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.CloseLevel);
		w->SetWindowTextW(s);

		w =GetDlgItem(IDC_STATIC_CLOSE_PLUS);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
		w =GetDlgItem(IDC_EDIT_CLOSE_PLUS);
		w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
		s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.CloseLevelSpanPlus);
		w->SetWindowTextW(s);

		w =GetDlgItem(IDC_STATIC_CLOSE_MINUS);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,80,height);
		w =GetDlgItem(IDC_EDIT_CLOSE_MINUS);
		w->MoveWindow(w_rect.right -  80, item_rect.top ,70,height);
		s.Format(_T("%5.1f"),this->m_SwitchConfigPara.para.CloseLevelSpanMinus);
		w->SetWindowTextW(s);


		w = GetDlgItem(IDC_COMBO_GROUP_NR);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height*1.5 ,150,height*11);

		w = GetDlgItem(IDC_STATIC_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height*1.5 ,150,height);
		w = GetDlgItem(IDC_EDIT_HIGH_FAIL_LEVEL);
		w->MoveWindow(w_rect.right - 80, item_rect.top ,40,height);
		w = GetDlgItem(IDC_EDIT_HIGH_FAIL_LEVEL_TAKE);
		w->MoveWindow(w_rect.right - 40, item_rect.top ,40,height);
		

		w = GetDlgItem(IDC_STATIC_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height*1.5 ,150,height);
		w = GetDlgItem(IDC_EDIT_LOW_FAIL_LEVEL);
		w->MoveWindow(w_rect.right - 80, item_rect.top ,40,height);
		w = GetDlgItem(IDC_EDIT_LOW_FAIL_LEVEL_TAKE);
		w->MoveWindow(w_rect.right - 40, item_rect.top ,40,height);

		w = GetDlgItem(IDC_STATIC_OTHER_FAIL__LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height*1.5 ,150,height);
		w = GetDlgItem(IDC_EDIT_OTHER_FAIL_LEVEL);
		w->MoveWindow(w_rect.right - 80, item_rect.top ,40,height);
		w = GetDlgItem(IDC_EDIT_OTHER_FAIL_LEVEL_TAKE);
		w->MoveWindow(w_rect.right - 40, item_rect.top ,40,height);


		w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_HIGH_LEVEL2);
		w->ShowWindow(SW_HIDE);

		w = GetDlgItem(IDC_EDIT_HIGH_LEVEL_CLOSE_LOW_LEVEL2);
		w->ShowWindow(SW_HIDE);

		w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_HIGH_LEVEL2);
		w->ShowWindow(SW_HIDE);

		w = GetDlgItem(IDC_EDIT_LOW_LEVEL_CLOSE_LOW_LEVEL2);
		w->ShowWindow(SW_HIDE);
		}


		

	}

	this->m_ComboGroupNr.AddString(_T("所有组"));
	this->m_ComboGroupNr.AddString(_T("01组"));
	this->m_ComboGroupNr.AddString(_T("02组"));
	this->m_ComboGroupNr.AddString(_T("03组"));
	this->m_ComboGroupNr.AddString(_T("04组"));
	this->m_ComboGroupNr.AddString(_T("05组"));
	this->m_ComboGroupNr.AddString(_T("06组"));
	this->m_ComboGroupNr.AddString(_T("07组"));
	this->m_ComboGroupNr.AddString(_T("08组"));
	this->m_ComboGroupNr.AddString(_T("09组"));
	this->m_ComboGroupNr.AddString(_T("10组"));
	
	m_ComboGroupNr.SetCurSel(0);

	this->SetTimer(1,1000,0); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTakeoutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	//ShowTestResult(true);



	// Do not call CDialog::OnPaint() for painting messages
}



void CTakeoutDlg::OnCheckMainLevel() 
{
	// TODO: Add your control notification handler code here
	

	int i;
	if(m_BoxType == ::BOX_128_ONLY_OPENTEMP_TEST)
	{

	}
	else if(m_BoxType != ::BOX_DOUBLE_SWITCH)
	{
		if(G_NormalConfigPara[0].bONE_BY_ONE_TAKEBOARD)
		{
			for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
			{
				if((MAIN_LEVEL != CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType))
					&& (NOT_USED != CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType)))
				{
					if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][i].IsClosing)
					{
						m_bMainLevel = false;
						this->UpdateData(false);
						CString s;
						s.Format(_T("还有 %s 的没拔板！"),level_str[CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType)]);
						AfxMessageBox(s, MB_ICONQUESTION);
						return;
					}
				}

			}
		}
	}

	if(m_iFailHighLevel_Take[m_BoxNr] < this->m_iFailHighLevel && ::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_EXT_INPUT)
	{
		m_bMainLevel = false;
		this->UpdateData(false);
		AfxMessageBox(_T("高温品没有全部放入废品箱!"));
		return ;
	}
	if(m_iFailLowLevel_Take[m_BoxNr] < this->m_iFailLowLevel && ::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_EXT_INPUT)
	{
		m_bMainLevel = false;
		this->UpdateData(false);
		AfxMessageBox(_T("低温品没有全部放入废品箱!"));
		return ;
	}
	if(m_iFailOtherLevel_Take[m_BoxNr] < this->m_iFailOtherLevel && ::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_EXT_INPUT)
	{
		m_bMainLevel = false;
		this->UpdateData(false);
		AfxMessageBox(_T("其它不合格品没有全部放入废品箱!"));
		return ;
	}
	
	this->UpdateData();

	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bMainLevel = true;
	}
	
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);

	

}

void CTakeoutDlg::OnCheckOverHighLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bOverHighLevel = true;
	}
	
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnCheckCloseHighLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bCloseHighLevel = true;
	}
	
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnCheckCloseLowLevel() 
{
	// TODO: Add your control notification handler code here
	

	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bCloseLowLevel = true;
	}
	
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnCheckHighLevel() 
{
	// TODO: Add your control notification handler code here
		int i;
	if(m_BoxType == ::BOX_128_ONLY_OPENTEMP_TEST)
	{

	}
	else if(m_BoxType != ::BOX_DOUBLE_SWITCH)
	{
		if(G_NormalConfigPara[0].bONE_BY_ONE_TAKEBOARD)
		{
			for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
			{
				if((MAIN_LEVEL != CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType))
					&& (TEST_RESULT_HIGH_LEVEL != CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType))
					&& (NOT_USED != CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType)))
				{
					if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][i].IsClosing)
					{
						m_bHighLevel = false;
						this->UpdateData(false);
						CString s;
						s.Format(_T("还有 %s 的没拔板！"),level_str[CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType)]);
						AfxMessageBox(s, MB_ICONQUESTION);
						break;
					}
				}

			}
		}
	}

	if(m_iFailHighLevel_Take[m_BoxNr] < this->m_iFailHighLevel && ::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_EXT_INPUT)
	{
		m_bHighLevel = false;
		this->UpdateData(false);
		AfxMessageBox(_T("高温品没有全部放入废品箱!"));
		return ;
	}
	if(m_iFailLowLevel_Take[m_BoxNr] < this->m_iFailLowLevel && ::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_EXT_INPUT)
	{
		m_bHighLevel = false;
		this->UpdateData(false);
		AfxMessageBox(_T("低温品没有全部放入废品箱!"));
		return ;
	}
	if(m_iFailOtherLevel_Take[m_BoxNr] < this->m_iFailOtherLevel && ::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_EXT_INPUT)
	{
		m_bHighLevel = false;
		this->UpdateData(false);
		AfxMessageBox(_T("其它不合格品没有全部放入废品箱!"));
		return ;
	}

	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bHighLevel = true;
	}
	
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnCheckLowLevel() 
{
	// TODO: Add your control notification handler code here
	int i;
	if(m_BoxType == ::BOX_128_ONLY_OPENTEMP_TEST)
	{

	}
	else if(m_BoxType != ::BOX_DOUBLE_SWITCH)
	{
		if(G_NormalConfigPara[0].bONE_BY_ONE_TAKEBOARD)
		{
			for(i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
			{
				if((MAIN_LEVEL != CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType))
					&& (TEST_RESULT_HIGH_LEVEL != CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType))
					&& (LOW_LEVEL != CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType))
					&& (NOT_USED != CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType)))
				{
					if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][i].IsClosing)
					{
						m_bLowLevel = false;
						this->UpdateData(false);
						CString s;
						s.Format(_T("还有 %s 的没拔板！"),level_str[CheckTestLevel(m_SwitchConfigPara.para,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType)]);
						AfxMessageBox(s, MB_ICONQUESTION);
						break;
					}
				}

			}
		}
	}

	if(m_iFailHighLevel_Take[m_BoxNr] < this->m_iFailHighLevel && ::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_EXT_INPUT)
	{
		m_bLowLevel = false;
		this->UpdateData(false);
		AfxMessageBox(_T("高温品没有全部放入废品箱!"));
		return ;
	}
	if(m_iFailLowLevel_Take[m_BoxNr] < this->m_iFailLowLevel && ::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_EXT_INPUT)
	{
		m_bLowLevel = false;
		this->UpdateData(false);
		AfxMessageBox(_T("低温品没有全部放入废品箱!"));
		return ;
	}
	if(m_iFailOtherLevel_Take[m_BoxNr] < this->m_iFailOtherLevel && ::G_NormalConfigPara[m_BoxNr].BoxType == BOX_128_EXT_INPUT)
	{
		m_bLowLevel = false;
		this->UpdateData(false);
		AfxMessageBox(_T("其它不合格品没有全部放入废品箱!"));
		return ;
	}

	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bLowLevel = true;
	}
	
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnCheckOverLowLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bOverLowLevel = true;
	}
	
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);	
}



void CTakeoutDlg::OnCheckOpenFlash() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bOpenFlashLevel = true;
	}
	
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);		
}


void CTakeoutDlg::OnCheckNoAction() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bNotUsed = true;
	}
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnCheckCloseFlash() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		this->m_bCloseFlashLevel = true; 
	}
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if(this->m_fOpenTempShow < (this->m_SwitchConfigPara.para.MainLevel -  m_SwitchConfigPara.para.MainLevelSpanMinus))
	{
		this->m_fOpenTempShow = (this->m_SwitchConfigPara.para.MainLevel -  m_SwitchConfigPara.para.MainLevelSpanMinus);
	}
	else
	{
		m_fOpenTempShow += 0.1f;
	}

//	ShowTestResult(true);

	CDialog::OnTimer(nIDEvent);
}

void CTakeoutDlg::ClearAllSelected()
{
	
	m_bMainLevel			= FALSE;
	m_bLowLevel				= FALSE;
	m_bHighLevel			= FALSE;
	m_bCloseHighLevel		= FALSE;
	m_bCloseLowLevel		= FALSE;
	m_bOverHighLevel		= FALSE;
	m_bOverLowLevel			= FALSE;
	m_bNotUsed				= FALSE;
	m_bOpenFlashLevel		= FALSE;
	m_bCloseFlashLevel		= FALSE;
	m_bOpenDoorResetLevel	= FALSE;
	m_bNoLevel				=	FALSE;
	m_bHighHighLevel		=	FALSE;

	m_bHighLevelCloseHighLevel		= FALSE;
	m_bHighLevelCloseLowLevel		= FALSE;
	m_bLowLevelCloseHighLevel		= FALSE;
	m_bLowLevelCloseLowLevel		= FALSE;

	m_bNoPTCLevel					= FALSE;

	for(int i =0 ; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		memcpy(&G_TakeDlgSwitchTestResult_Bak[m_iTakeBoardType][i], &G_TakeDlgSwitchTestResult[m_iTakeBoardType][i], sizeof(G_TakeDlgSwitchTestResult[this->m_iTakeBoardType][i]));
	}
	if(G_SpeechArray.GetSize() > 0)
	{
		::G_SpeechArray.RemoveAll(); 
	}

}

void CTakeoutDlg::SetAllSelected()
{
	m_bMainLevel			= TRUE;
	m_bLowLevel				= TRUE;
	m_bHighLevel			= TRUE;
	m_bCloseHighLevel		= TRUE;
	m_bCloseLowLevel		= TRUE;
	m_bOverHighLevel		= TRUE;
	m_bOverLowLevel			= TRUE;
	m_bNotUsed				= TRUE;
	m_bOpenFlashLevel		= TRUE;
	m_bCloseFlashLevel		= TRUE;
	m_bOpenDoorResetLevel	= TRUE;
	m_bNoLevel				=	TRUE;
	m_bHighHighLevel		=	TRUE;
	m_bHighLevelCloseHighLevel		= TRUE;
	m_bHighLevelCloseLowLevel		= TRUE;
	m_bLowLevelCloseHighLevel		= TRUE;
	m_bLowLevelCloseLowLevel		= TRUE;
	m_bNoPTCLevel					= TRUE;
}
void CTakeoutDlg::SetAllSelected_NoNotUsed()
{
	m_bMainLevel			= TRUE;
	m_bLowLevel				= TRUE;
	m_bHighLevel			= TRUE;
	m_bCloseHighLevel		= TRUE;
	m_bCloseLowLevel		= TRUE;
	m_bOverHighLevel		= TRUE;
	m_bOverLowLevel			= TRUE;
	m_bNotUsed				= false;
	m_bOpenFlashLevel		= TRUE;
	m_bCloseFlashLevel		= TRUE;
	m_bOpenDoorResetLevel	= TRUE;
	m_bNoLevel				=	false;
	m_bHighHighLevel		=	TRUE;
	m_bHighLevelCloseHighLevel		= TRUE;
	m_bHighLevelCloseLowLevel		= TRUE;
	m_bLowLevelCloseHighLevel		= TRUE;
	m_bLowLevelCloseLowLevel		= TRUE;
	m_bNoPTCLevel					= TRUE;
}

void CTakeoutDlg::OnCheckOpenDoorReset() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bOpenDoorResetLevel = true;
	}
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnRadioOnlyOne() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
	}
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}


void CTakeoutDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	return;
	// TODO: Add your message handler code here
	if(this->IsWindowVisible())
	{
		ShowTestResult(true);
	}
}

void CTakeoutDlg::OnButtonRecordView2() 
{
	// TODO: Add your control notification handler code here
	CRecordViewDlg dlg;
	dlg.m_BoxNr  = m_BoxNr;
	dlg.DoModal(); 
}


void CTakeoutDlg::OnRadioAllSelect() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 2)
	{
		SetAllSelected();
	}
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

int CTakeoutDlg::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::DoModal();
}

BOOL CTakeoutDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CTakeoutDlg::OnCheckNoLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bNoLevel = true;
	}
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnCheckHighHighLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bHighHighLevel = true;
	}
	
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnCheckHighLevelCloseHighLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bHighLevelCloseHighLevel = true;
	}
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnCheckHighLevelCloseLowLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bHighLevelCloseLowLevel = true;
	}
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnCheckLowLevelCloseHighLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bLowLevelCloseHighLevel = true;
	}
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnCheckLowLevelCloseLowLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bLowLevelCloseLowLevel = true;
	}
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

bool CTakeoutDlg::SelectLevel(void)
{


//	m_iHighLevelCloseHighLevel	= 0;
//	m_iLowLevelCloseHighLevel	= 0;
//	m_iHighLevelCloseLowLevel	= 0;
//	m_iLowLevelCloseLowLevel	= 0;
	
	switch(m_UpDownID)
	{
	case 0:
		OnCheckMainLevel();
		if(m_iMainLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 1:
		OnCheckHighLevel();
		if(m_iHighLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 2:
		OnCheckLowLevel();
		if(m_iLowLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 3:
		OnCheckHighHighLevel();
		if(m_iHighHighLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 4:
		OnCheckOverHighLevel();
		if(m_iOverHighLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 5:
		OnCheckOverLowLevel();
		if(m_iOverLowLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 6:
		OnCheckOpenDoorReset();
		if(m_iOpenDoorResetLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 7:
		OnCheckCloseHighLevel();
		if(m_iCloseHighLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 8:
		OnCheckCloseLowLevel();
		if(m_iCloseLowLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 9:
		OnCheckOpenFlash();
		if(m_iOpenFlashLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 10:
		OnCheckCloseFlash();
		if(m_iCloseFlashLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 11:
		OnBnClickedCheckNoPtcLevel();
		if(m_iNoPTCLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 12:
		OnCheckNoLevel();
		if(m_iNoLevel == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	case 13:
		OnCheckNoAction();
		if( m_iNotUsed == 0)
		{
			return false;
		}
		else
		{
			return true;
		}
		break;
	}
	return false;
}

BOOL CTakeoutDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN) //判断是否有按键按下
    {
       switch(pMsg->wParam)
       {
	   case VK_DOWN:
		   
		   while(1)
		   {
			    m_UpDownID ++;
			   if(m_UpDownID > 13)
			   {
				   m_UpDownID = 0;
			   }
				if(SelectLevel() == true)
				{
					break;
				}
				
		   }
		   CDialog::PreTranslateMessage(pMsg);
		   return false;

		   break;
	   case VK_UP:
		   while(1)
		   {
			    m_UpDownID --;
			   if(m_UpDownID < 0)
			   {
				   m_UpDownID = 13;
			   }
				if(SelectLevel() == true)
				{
					break;
				}
				
		   }
		   CDialog::PreTranslateMessage(pMsg);
		   return false;
		   break;
	/*
          case VK_F5:
			ClearAllSelected();
			m_bMainLevel = true;
			UpdateData(false);
			m_IsVK	=	true;
			ShowTestResult(true,false);
			break;
          case VK_F6:
			ClearAllSelected();
			m_bHighLevel = true;
			UpdateData(false);
			m_IsVK	=	true;
			ShowTestResult(true,false);
			break;
          case VK_F7:
			ClearAllSelected();
			m_bLowLevel = true;
			UpdateData(false);
			m_IsVK	=	true;
			ShowTestResult(true,false);
			break;
          case VK_F8:
			UpdateData(true);
			m_bMainLevel			= FALSE;
			m_bLowLevel				= FALSE;
			m_bHighLevel			= FALSE;
			m_bCloseHighLevel		= TRUE;
			m_bCloseLowLevel		= TRUE;
			m_bOverHighLevel		= TRUE;
			m_bOverLowLevel			= TRUE;
			m_bNotUsed				= TRUE;
			m_bOpenFlashLevel		= TRUE;
			m_bCloseFlashLevel		= TRUE;
			m_bOpenDoorResetLevel	= TRUE;
			m_bNoLevel				=	TRUE;
			m_bHighHighLevel		=	TRUE;
			m_bHighLevelCloseHighLevel		= TRUE;
			m_bHighLevelCloseLowLevel		= TRUE;
			m_bLowLevelCloseHighLevel		= TRUE;
			m_bLowLevelCloseLowLevel		= TRUE;
			UpdateData(false);
			m_IsVK	=	true;
			ShowTestResult(true,true);
			break;
			*/
       }
	}

	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CTakeoutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	CWnd *w;
	// TODO:  在此更改 DC 的任何属性
	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		hbr = G_bkBrush;
		break;

	case CTLCOLOR_BTN:	//    Button control
		hbr = G_bkBrush;
		pDC->SetBkColor(BK_COLOR); 
		break;
	case CTLCOLOR_EDIT:	//    Edit control
		//hbr = m_bkBrush;
		 
		break;
	case CTLCOLOR_LISTBOX:	//    List-box control
		break;
	case CTLCOLOR_MSGBOX:	//    Message box
		break;
	case CTLCOLOR_SCROLLBAR:	//    Scroll-bar control
		break;
	case CTLCOLOR_STATIC:	//    Static control
		
		if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_STATIC_OPEN)->GetSafeHwnd())
		{
			hbr = G_bkBrush;
			pDC->SetBkColor(BK_COLOR); 
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		else if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_STATIC_CLOSE)->GetSafeHwnd())
		{
			hbr = G_bkBrush;
			pDC->SetBkColor(BK_COLOR); 
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		else if(pWnd->GetSafeHwnd() == GetDlgItem(IDC_STATIC_HIGH_LEVEL)->GetSafeHwnd()
			|| pWnd->GetSafeHwnd() == GetDlgItem(IDC_STATIC_LOW_LEVEL)->GetSafeHwnd()
			|| pWnd->GetSafeHwnd() == GetDlgItem(IDC_STATIC_OTHER_FAIL__LEVEL)->GetSafeHwnd())
		{
			hbr = G_bkBrush;
			pDC->SetBkColor(BK_COLOR); 
			pDC->SetTextColor(RGB(255, 0, 0));
		}
		else
		{
			hbr = G_bkBrush;
			pDC->SetBkColor(BK_COLOR); 
			pDC->SetTextColor(RGB(255, 255, 60));
		}
		break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CTakeoutDlg::OnBnClickedCheckNoPtcLevel()
{
	// TODO: 在此添加控件通知处理程序代码
	this->UpdateData();
	if(m_iSelect == 0)
	{
		ClearAllSelected();
		m_bNoPTCLevel = true;
	}
	this->UpdateData(false);
	m_IsVK	=	false;
	ShowTestResult(true);
}

void CTakeoutDlg::OnBnClickedButtonTempView()
{
	// TODO: 在此添加控件通知处理程序代码

	
	int j;
	
	
	for(j = 0; j < G_iMaxLedNr[m_BoxNr];j++)
	{
		m_ProductTempBarDlg.m_SwitchTestResult[j] = G_TakeDlgSwitchTestResult[this->m_iTakeBoardType][j];
		if(m_ProductTempBarDlg.m_SwitchTestResult[j].IsOpenned == false)
		{
			m_ProductTempBarDlg.m_SwitchTestResult[j].OpenTemp = 0.0f;
		}

		if(m_ProductTempBarDlg.m_SwitchTestResult[j].IsClosed == false)
		{
			m_ProductTempBarDlg.m_SwitchTestResult[j].CloseTemp = 0.0f;
		}

	}

	m_ProductTempBarDlg.m_BeginTestTime = m_BeginTestTime;
	m_ProductTempBarDlg.m_SwitchConfigPara = m_SwitchConfigPara.para;

	m_ProductTempBarDlg.DoModal(); 
}

void CTakeoutDlg::OnBnClickedButtonSaveExcel()
{
	RECORD	record;
	CString s;
	CString FileName;
	CString str;
	CTimeSpan span;

	memcpy(record.ParaName,::G_NormalConfigPara[m_BoxNr].ParaName,sizeof(record.ParaName));
	record.ConfigPara	=	this->m_SwitchConfigPara.para;
	s.Format(_T("TEST MEMO %d"),m_BoxNr);
	CString memo = AfxGetApp()->GetProfileStringW(_T("SETTING"),s,_T(""));
	TCHAR *b = memo.GetBuffer(sizeof(record.Memo));
	memcpy(record.Memo,b,sizeof( record.Memo));

	memcpy(&record.TestResult[0],&G_TakeDlgSwitchTestResult[0][0],sizeof(record.TestResult));

	span				= m_BeginTestTime - STD_TIME; 
	record.StartTime   	= span.GetTotalMinutes();

	span				= m_EndTestTime - STD_TIME; 
	record.EndTime  	= span.GetTotalMinutes();

	record.SwitchType	=	this->m_SwitchNormalOpen;


	CFileDialog m_cFileDlg(FALSE, 
		_T("csv "), 
		str, 
		OFN_OVERWRITEPROMPT, 
		_T("保存文件到Excel (*.csv)|*.csv|| "), this); 
		if(m_cFileDlg.DoModal()==IDOK) 
		{ 
			FileName = m_cFileDlg.GetPathName(); 
		}
		else
		{
			return;
		}

	SaveRecord2ExcelCSV(record,0,FileName);

	AfxMessageBox(_T("生产记录已经保存到 ") + FileName);
	
}

void CTakeoutDlg::OnBnClickedButtonPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	CPrnPreview dlg;
	
	dlg.m_CurGroupNr = this->m_ComboGroupNr.GetCurSel();

	SWITCH_TEST_RESULT_EX m_SwitchTestResult_Ex[192];

	dlg.m_TestParaName	= m_ParaName;
	dlg.m_EndTestTime	= m_EndTestTime;
	dlg.m_BeginTestTime = m_BeginTestTime;
	dlg.m_BoxNr			= m_BoxNr;
	dlg.m_SwitchConfigPara = m_SwitchConfigPara;

	for(int j = 0; j < G_iMaxLedNr[m_BoxNr];j++)
	{	
		
		//m_SwitchTestResult[j] = record.TestResult[j];
		m_SwitchTestResult_Ex[j].ID = j;
		m_SwitchTestResult_Ex[j].result = G_TakeDlgSwitchTestResult[this->m_iTakeBoardType][j];
	}

	memcpy(dlg.m_SwitchTestResult_Ex,m_SwitchTestResult_Ex,sizeof(m_SwitchTestResult_Ex));
	
	if(this->m_SwitchNormalOpen == 0)
	{
		dlg.m_TestProductType = _T("常闭");
	}
	else
	{
		dlg.m_TestProductType = _T("常开");
	}
	
	CString strTitle;
	this->GetWindowText(strTitle); 
	CString app_name;
	AfxGetApp()->GetMainWnd()->GetWindowText(app_name);
	app_name += _T("[");
	app_name += strTitle;
	app_name += _T("]");
	
	dlg.m_Title = app_name;

	dlg.DoModal(); 

}

extern void TraverseFolder(LPCTSTR lpPath,LPCTSTR file_ext,CStringArray *fileStr);

void CTakeoutDlg::OnBnClickedButtonTodayChanliang()
{
	// TODO: 在此添加控件通知处理程序代码
	long start_minutes,end_minutes;

	int m_iCloseFlashLevel = 0;
	int m_iCloseHighLevel = 0;
	int m_iCloseLowLevel = 0;
	int m_iHighLevel = 0;
	int m_iLowLevel = 0;
	int m_iMainLevel = 0;
	int m_iNotUsedLevel = 0;
	int m_iOpenDoorResetLevel = 0;
	int m_iOpenFlashLevel = 0;
	int m_iOverHighLevel = 0;
	int m_iOverLowLevel = 0;
	int m_iTotal = 0;

	CString s;

	CTime cur = CTime::GetCurrentTime();

	
	CTimeSpan span;
	CTime start_time(cur.GetYear(),cur.GetMonth(),cur.GetDay(),0,0,0);
	span = start_time - STD_TIME;
	start_minutes = span.GetTotalMinutes();
	
	CTime end_time(cur.GetYear(),cur.GetMonth(),cur.GetDay(),23,59,59);
	span = end_time - STD_TIME;
	end_minutes = span.GetTotalMinutes();

	
	CFile RecFile;
	RECORD	record;
	CString fName,ym_str;
	CFileFind ff;
	



	CStringArray sa;
	CString sDir;

	/*
	sDir.Format(_T("log\\box%d\\"),m_BoxNr+1);

	sDir = theAppDirectory + sDir;

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}
	*/

	sDir = CreateCurDateDirection(m_BoxNr);

	sDir += _T("\\");

	TraverseFolder(sDir,_T("*.record"),&sa);

	while(sa.GetCount() > 0)
	{
		CString s;
		s =  sa.GetAt(0);

		if(RecFile.Open(sDir+s,CFile::modeRead))
		{
			CTime m_FileTime;
			
			CFileStatus fs;
			if(CFile::GetStatus(sDir+s,fs))
			{
				m_FileTime =  fs.m_ctime;
			}

		//	if(m_FileTime.GetYear() == cur.GetYear() &&  m_FileTime.GetMonth() == cur.GetMonth() && m_FileTime.GetDay() == cur.GetDay())
			{
			
				if(RecFile.Read(&record,sizeof(record)) <= sizeof(record)) 
				{
					for(int i = 0; i < ::G_iMaxLedNr[m_BoxNr]; i++)
					{
						switch(CheckTestLevel(record.ConfigPara,record.TestResult[i],m_HeatCoolType,m_TempRange,m_SwitchTestResult_Ex,m_BoxType))
							{
							case MAIN_LEVEL:
								m_iMainLevel++;
								break;
							case TEST_RESULT_HIGH_LEVEL:
								m_iHighLevel++;
								break;
							case LOW_LEVEL:
								m_iLowLevel ++;
								break;
							
							case OVER_HIGH_LEVEL:
								m_iOverHighLevel ++;
								break;
							case OVER_LOW_LEVEL:
								m_iOverLowLevel ++;
								break;
							case CLOSE_HIGH_LEVEL:
								m_iCloseHighLevel ++;
								break;
							case CLOSE_LOW_LEVEL:
								m_iCloseLowLevel ++;
								break;
							
							case OPEN_FLASH_LEVEL:
								m_iOpenFlashLevel ++;
								break;
							
							case CLOSE_FLASH_LEVEL:
								m_iCloseFlashLevel++;
								break;

							case OPEN_DOOR_RESET_LEVEL:
								m_iOpenDoorResetLevel ++;
								break;

							case NOT_USED:
								//m_iNotUsedLevel ++;
								break;
							
							case NO_LEVEL:
							case HIGH_HIGH_LEVEL:
							case NO_PTC_LEVEL:
							case HIGH_LEVEL_CLOSE_HIGH_LEVEL:
							case HIGH_LEVEL_CLOSE_LOW_LEVEL:
							case LOW_LEVEL_CLOSE_HIGH_LEVEL:
							case LOW_LEVEL_CLOSE_LOW_LEVEL:
							default:
								m_iNotUsedLevel ++;
								break;
							}
					}

				}

			}
			RecFile.Close(); 
			sa.RemoveAt(0);
		}
	}
	

	m_iTotal = 
	m_iCloseFlashLevel +
	m_iCloseHighLevel +
	m_iCloseLowLevel +
	m_iHighLevel +
	m_iLowLevel +
	m_iMainLevel +
	//m_iNotUsedLevel +
	m_iOpenDoorResetLevel +
	m_iOpenFlashLevel +
	m_iOverHighLevel +
	m_iOverLowLevel;

	s.Format(_T("主规格:%8d\r\n高规格:%8d\r\n低规格:%8d\r\n高温品:%8d\r\n低温品:%8d\r\n复位大:%8d\r\n复位小:%8d\r\n动作闪:%8d\r\n复位闪:%8d\r\n总产量:%8d\r\n"),
				m_iMainLevel,
				m_iHighLevel,
				m_iLowLevel,
				m_iOverHighLevel,
				m_iOverLowLevel,
				m_iCloseLowLevel,
				m_iCloseHighLevel,
				m_iOpenFlashLevel,
				m_iCloseFlashLevel,
				m_iTotal);

	AfxMessageBox(s);






}

void CTakeoutDlg::OnBnClickedButtonPrint()
{
	// TODO: 在此添加控件通知处理程序代码

	

	RECORD	record;
	CTime cur;
	CTimeSpan span;
	cur = cur.GetCurrentTime();

	memset(&record,0,sizeof(record));

	span					= m_BeginTestTime - STD_TIME; 
	record.StartTime 		= span.GetTotalMinutes();

	span					= m_EndTestTime - STD_TIME; 
	record.EndTime 			= span.GetTotalMinutes();
	record.SwitchType		= G_NormalConfigPara[m_BoxNr].SwitchType;
	record.ConfigPara		= m_SwitchConfigPara.para;
	record.ConfigParaEx		= m_SwitchConfigPara.para_ex ;
	


	for(int i = 0; i < G_iMaxLedNr[m_BoxNr]; i++)
	{
		record.TestResult[i] = G_SwitchTestResult[m_BoxNr][i];  
	}
	memset(record.ParaName,0,sizeof(record.ParaName));
	for(int i = 0; i < sizeof(record.ParaName)/sizeof(TCHAR),i < m_ParaName.GetLength() ; i++)
	{
		record.ParaName[i] = m_ParaName.GetAt(i); 
	}
	record.size = sizeof(record);
	

	//G_MainDlg->m_LPM260_Printer.PrintTestResult(&record,0,m_ComboGroupNr.GetCurSel()); 
	

	CPrintDialog prnDlg(true);//false);
	CString printer;
	CDC dc;
	int i;
	
	CListDlg dlg_list;

	dlg_list.m_Tiltle = _T("选择 格式");
	
	dlg_list.m_ListItem.Add(_T("表格"));
	dlg_list.m_ListItem.Add(_T("收据"));
	dlg_list.m_ListItem.Add(_T("串口票据打印机"));

	
	if(dlg_list.DoModal() != IDOK)
	{//dlg_list.m_ListItem.Add(_T("串口票据打印机"));
		return;
	}

	if(dlg_list.m_iSelectItem == 2)
	{
		CONTROL_TEMP_RANGE	TempRange;
	
		HEAT_COOL_ORDER HeatCool = CheckHeatOrCoolMode(m_BoxNr,m_SwitchConfigPara.para  , &TempRange,G_NormalConfigPara[m_BoxNr].BoxType);



		G_MainDlg->m_LPM260_Printer.PrintTestResult(m_BoxNr,&record,0,0,TempRange,::G_NormalConfigPara[m_BoxNr].BoxType,0); 

		return;
	}

	if(prnDlg.DoModal() != IDOK)
	{
		AfxMessageBox(_T("打印取消!"));
		return;
	}
	printer  = prnDlg.GetDeviceName();
//		AfxMessageBox(printer);

	if(!dc.CreateDC(_T(""),printer,_T(""),NULL))
	{
		AfxMessageBox(_T("请设置打印机"));
		return;
	}	
	
	DOCINFO docInfo;
	memset(&docInfo, 0, sizeof(DOCINFO));
	docInfo.cbSize = sizeof(DOCINFO);
	CString strTitle;
	strTitle.Format(_T("%04d%02d%02d%02d%02d"),
		m_BeginTestTime.GetYear(),
		m_BeginTestTime.GetMonth(), 
		m_BeginTestTime.GetDay(), 
		m_BeginTestTime.GetHour(), 
		m_BeginTestTime.GetMinute()); 
	//this->GetWindowText(strTitle); 
	CString app_name = _T("热保护器测试记录");
	//AfxGetApp()->GetMainWnd()->GetWindowText(app_name);
	app_name += "[";
	app_name += strTitle;
	app_name += "]";
	docInfo.lpszDocName =   app_name;


	i=dc.StartDoc(&docInfo);  
	

	CString BoxNr;
	SWITCH_TEST_RESULT_EX SwitchTestResult_Ex[192];

	

	BoxNr.Format(_T("%d"), m_BoxNr + 1);

	//AfxMessageBox("打印1");
	CString ProductType;
	if(m_SwitchNormalOpen == 0)
	{
		ProductType = _T("常闭");
	}
	else
	{
		ProductType = _T("常开");
	}

	for(int i = 0; i <  G_iMaxLedNr[m_BoxNr]; i++)
	{
		SwitchTestResult_Ex[i].ID = i;
		SwitchTestResult_Ex[i].result = G_SwitchTestResult[m_BoxNr][i];
	}

	if(dlg_list.m_iSelectItem == 0)
	{

		i=	dc.StartPage();
		PrintReport_ByGroup(&dc,app_name, m_BoxNr,
					m_BeginTestTime,m_EndTestTime,
					m_ParaName,ProductType, _T(""),
					m_SwitchConfigPara,SwitchTestResult_Ex,
					TRUE,(BOX_TYPE)m_BoxType,m_ComboGroupNr.GetCurSel());////
		
		i =	dc.EndPage();
	}
	//SaveBmpFile(this,_T("C:\\temp.bmp"),strTitle ,m_BoxNr, m_BeginTestTime,m_EndTestTime,
	//			 m_ParaName,ProductType,_T(""),m_SwitchConfigPara.para,SwitchTestResult_Ex,TRUE,0,127,(BOX_TYPE)m_BoxType,m_ComboGroupNr.GetCurSel());


	if(dlg_list.m_iSelectItem == 1)
	{
	 
		i=	dc.StartPage();
		PrintReport_ByGroup_Ticket(&dc,app_name, m_BoxNr,
					m_BeginTestTime,m_EndTestTime,
					m_ParaName,ProductType, _T(""),
					m_SwitchConfigPara,SwitchTestResult_Ex,
					TRUE,(BOX_TYPE)m_BoxType,m_ComboGroupNr.GetCurSel());
		i =	dc.EndPage();
	}

	
	if(G_iMaxLedNr[m_BoxNr] > 128)
	{
		i = dc.StartPage();
		PrintReport_ByGroup(&dc,app_name, m_BoxNr,
					m_BeginTestTime,m_EndTestTime,
					m_ParaName,ProductType,_T(""), 
					m_SwitchConfigPara,SwitchTestResult_Ex,
					TRUE,(BOX_TYPE)m_BoxType,m_ComboGroupNr.GetCurSel());

		i =	dc.EndPage(); 
	}

	i =	dc.EndDoc(); 
	
	dc.DeleteDC();

}

void CTakeoutDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if(G_SpeechArray.GetSize() > 0)
	{
		::G_SpeechArray.RemoveAll();
	}

	CDialog::OnClose();
}



void CTakeoutDlg::OnBnClickedButtonGroup()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CTakeoutDlg::OnEnSetfocusEditOpen()
{
	// TODO: 在此添加控件通知处理程序代码

	CWnd *w = this->GetDlgItem(IDC_EDIT_MAIN_LEVEL); 
	w->SetFocus();

	CInputDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CEdit *edit=(CEdit*)this->GetDlgItem(IDC_EDIT_OPEN);
		edit->SetWindowTextW(dlg.m_Input);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		this->m_SwitchConfigPara.para.MainLevel = _tcstod(buf,NULL);
		this->ShowTestResult(true);
	}
}

void CTakeoutDlg::OnEnSetfocusEditOpenPlus()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MAIN_LEVEL); 
	w->SetFocus();

	CInputDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CEdit *edit=(CEdit*)this->GetDlgItem(IDC_EDIT_OPEN_PLUS);
		edit->SetWindowTextW(dlg.m_Input);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		this->m_SwitchConfigPara.para.MainLevelSpanPlus = _tcstod(buf,NULL);
		this->ShowTestResult(true);
	}
}

void CTakeoutDlg::OnEnSetfocusEditOpenMinus()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MAIN_LEVEL); 
	w->SetFocus();

	CInputDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CEdit *edit=(CEdit*)this->GetDlgItem(IDC_EDIT_OPEN_MINUS);
		edit->SetWindowTextW(dlg.m_Input);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		this->m_SwitchConfigPara.para.MainLevelSpanMinus = _tcstod(buf,NULL);
		this->ShowTestResult(true);
	}
}

void CTakeoutDlg::OnEnSetfocusEditClose()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MAIN_LEVEL); 
	w->SetFocus();

	CInputDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CEdit *edit=(CEdit*)this->GetDlgItem(IDC_EDIT_CLOSE);
		edit->SetWindowTextW(dlg.m_Input);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		this->m_SwitchConfigPara.para.CloseLevel = _tcstod(buf,NULL);
		this->ShowTestResult(true);
	}
}

void CTakeoutDlg::OnEnSetfocusEditClosePlus()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MAIN_LEVEL); 
	w->SetFocus();

	CInputDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CEdit *edit=(CEdit*)this->GetDlgItem(IDC_EDIT_CLOSE_PLUS);
		edit->SetWindowTextW(dlg.m_Input);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		this->m_SwitchConfigPara.para.CloseLevelSpanPlus = _tcstod(buf,NULL);
		this->ShowTestResult(true);
	}
}

void CTakeoutDlg::OnEnSetfocusEditCloseMinus()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_MAIN_LEVEL); 
	w->SetFocus();

	CInputDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		CEdit *edit=(CEdit*)this->GetDlgItem(IDC_EDIT_CLOSE_MINUS);
		edit->SetWindowTextW(dlg.m_Input);
		TCHAR buf[10];
		edit->GetWindowText(buf,10); 
		this->m_SwitchConfigPara.para.CloseLevelSpanMinus = _tcstod(buf,NULL);
		this->ShowTestResult(true);
	}
}

void CTakeoutDlg::OnEnChangeEditHighLevelCloseHighLevel()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}
