// TakeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "TakeDlg.h"
#include "comm.h"
#include "RecordViewDlg.h"
#include "ProductTempBar.h"
#include "RecordPrintPriew.h"
#include "switchdlg.h"
#include "shlwapi.h"

#pragma comment(lib,"shlwapi.lib")



SWITCH_TEST_RESULT G_TakeDlgSwitchTestResult[2][500];	//一个在线拔板，一个历史记录拔板
SWITCH_TEST_RESULT G_TakeDlgSwitchTestResult_Bak[2][500];


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern	TCHAR level_str[18][10];
extern  TCHAR level_str_4_csv[18][10];
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

TEST_RESULT_LEVEL CheckTestLevel(SWITCH_CONFIG_PARA SwitchConfigPara,
			SWITCH_TEST_RESULT SwitchTestResult,HEAT_BOX_TYPE HeatBoxType)
{
	//不合格的先选出来

//不用的
	
		if(SwitchTestResult.IsUsed == false)
		{
			return NOT_USED;
		}
//PTC		
		if(HeatBoxType == HEAT_BOX_TYPE_PTC)
		{
			if(SwitchTestResult.IsMoreThan1M)
			{
				return NO_PTC_LEVEL;
			}
		}
//动作闪烁		
		if (SwitchTestResult.IsOpenFlash)
		{
			return OPEN_FLASH_LEVEL;
		}

//复位闪烁		
		if (SwitchTestResult.IsCloseFlash)
		{
			return CLOSE_FLASH_LEVEL;
		}



//开门复位
		if(SwitchTestResult.CloseTemp + SwitchConfigPara.OpenDoorResetLevel>= SwitchTestResult.OpenTemp
			&& SwitchTestResult.CloseTemp > 0
			&& SwitchTestResult.OpenTemp > 0)
		{
			return OPEN_DOOR_RESET_LEVEL;
		}

		
			
//有用产品			
			
				
//主规格
			if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_RANGE)
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
			else if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP_AND_RANGE)
			{
				if((SwitchConfigPara.MainLevel != 0)
					&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus))
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus))
					&& (SwitchTestResult.CloseTemp  >= (SwitchConfigPara.CloseLevel - SwitchConfigPara.CloseLevelSpanMinus ))
					&& (SwitchTestResult.CloseTemp  <= (SwitchConfigPara.CloseLevel + SwitchConfigPara.CloseLevelSpanPlus))
					&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  >= (SwitchConfigPara.CloseLevelDeviationMinus))
					&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  <= (SwitchConfigPara.CloseLevelDeviationPlus)))
				{
					return MAIN_LEVEL;
				}
			}
			else if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP)
			{
				if((SwitchConfigPara.MainLevel != 0)
					&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus))
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus))
					&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp  >= (SwitchConfigPara.CloseLevelDeviationMinus))
					&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp  <= (SwitchConfigPara.CloseLevelDeviationPlus)))
				{
					return MAIN_LEVEL;
				}
			}
	
//高规格
		if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_RANGE)
		{
			if(SwitchConfigPara.HighLevel != 0
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.HighLevel - SwitchConfigPara.HighLevelSpanMinus))
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus))
				&& (SwitchTestResult.CloseTemp >=(SwitchConfigPara.CloseHighLevel - SwitchConfigPara.CloseHighLevelSpanMinus))
				&& (SwitchTestResult.CloseTemp <=(SwitchConfigPara.CloseHighLevel + SwitchConfigPara.CloseHighLevelSpanPlus)))
			{
				return HIGH_LEVEL;
			}

			if(SwitchConfigPara.CloseHighLevel == 0)
			{
				if((SwitchConfigPara.HighLevel != 0)
					&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.HighLevel - SwitchConfigPara.HighLevelSpanMinus))  
					&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus)))
				{//没有复位
					return HIGH_LEVEL;
				}
			}
		}
		else if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP_AND_RANGE)
		{
			if(SwitchConfigPara.HighLevel != 0
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.HighLevel - SwitchConfigPara.HighLevelSpanMinus))
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus))
				&& (SwitchTestResult.CloseTemp >=(SwitchConfigPara.CloseHighLevel - SwitchConfigPara.CloseHighLevelSpanMinus))
				&& (SwitchTestResult.CloseTemp <=(SwitchConfigPara.CloseHighLevel + SwitchConfigPara.CloseHighLevelSpanPlus))
				&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp >= SwitchConfigPara.CloseHighLevelDeviationMinus)
				&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp <= SwitchConfigPara.CloseHighLevelDeviationPlus))
			{
				return HIGH_LEVEL;
			}

		}
		else if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP)
		{
			if(SwitchConfigPara.HighLevel != 0
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.HighLevel - SwitchConfigPara.HighLevelSpanMinus))
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.HighLevel + SwitchConfigPara.HighLevelSpanPlus))
				&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp >= SwitchConfigPara.CloseHighLevelDeviationMinus)
				&& (SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp <= SwitchConfigPara.CloseHighLevelDeviationPlus))
			{
				return HIGH_LEVEL;
			}
		}
//低规格
		if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_RANGE)
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
		else if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP_AND_RANGE)
		{
			if((SwitchConfigPara.LowLevel != 0)
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus))  
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.LowLevel + SwitchConfigPara.LowLevelSpanPlus))
				&& (SwitchTestResult.CloseTemp  >= (SwitchConfigPara.CloseLowLevel  - SwitchConfigPara.CloseLowLevelSpanMinus))
				&& (SwitchTestResult.CloseTemp  <= (SwitchConfigPara.CloseLowLevel  + SwitchConfigPara.CloseLowLevelSpanPlus ))
				&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  >= SwitchConfigPara.CloseLowLevelDeviationMinus)
				&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  <= SwitchConfigPara.CloseLowLevelDeviationPlus )
				)
			{
				return LOW_LEVEL;
			}
		}
		else if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP)
		{
			if((SwitchConfigPara.LowLevel != 0)
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.LowLevel - SwitchConfigPara.LowLevelSpanMinus))  
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.LowLevel + SwitchConfigPara.LowLevelSpanPlus))
				&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  >= SwitchConfigPara.CloseLowLevelDeviationMinus)
				&& ((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp)  <= SwitchConfigPara.CloseLowLevelDeviationPlus )
				)
			{
				return LOW_LEVEL;
			}

		}
			
			CONTROL_TEMP_RANGE temp_range;

			CheckHeatOrCoolMode(SwitchConfigPara, &temp_range);

		if((SwitchTestResult.IsOpenned == true)
			&& (SwitchTestResult.OpenTemp > temp_range.open_temp_max)
			&& (SwitchTestResult.OpenTemp <= temp_range.open_temp_max + G_iHighHighLevelDelta)) 
		{
			return HIGH_HIGH_LEVEL;
		}

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
		if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_RANGE)
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
		else if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP_AND_RANGE)
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

				if((SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp) < SwitchConfigPara.CloseLevelDeviationMinus)
				{
					return CLOSE_HIGH_LEVEL;
				}

				if(SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp > SwitchConfigPara.CloseLevelDeviationPlus)
				{
					return CLOSE_LOW_LEVEL;
				}

			}
		}
		else if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_OPEN_CLOSE_GAP)
		{
			if((SwitchConfigPara.MainLevel != 0)
				&& (SwitchTestResult.OpenTemp >= (SwitchConfigPara.MainLevel - SwitchConfigPara.MainLevelSpanMinus))
				&& (SwitchTestResult.OpenTemp <= (SwitchConfigPara.MainLevel + SwitchConfigPara.MainLevelSpanPlus)))
			{
				if(SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp < SwitchConfigPara.CloseLevelDeviationMinus)
				{
					return CLOSE_HIGH_LEVEL;
				}

				if(SwitchTestResult.OpenTemp - SwitchTestResult.CloseTemp > SwitchConfigPara.CloseLevelDeviationPlus)
				{
					return CLOSE_LOW_LEVEL;
				}
			}
		}

#ifdef _CHANGHONG_
		if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_RANGE)
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

		if(SwitchConfigPara.ParaType == CLOSE_TEMP_TYPE_RANGE)
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


/////////////////////////////////////////////////////////////////////////////
// CTakeoutDlg dialog

CTakeoutDlg::CTakeoutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTakeoutDlg::IDD, pParent)
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

//	m_iTakeBoardType = 0;
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
	DDX_Text(pDX, IDC_STATIC_MSG, m_Msg);
	DDX_Radio(pDX, IDC_RADIO_ONLY_ONE, m_iSelect);
	DDX_Check(pDX, IDC_CHECK_OPEN_DOOR_RESET, m_bOpenDoorResetLevel);
	DDX_Text(pDX, IDC_EDIT_OPEN_DOOR_RESET, m_iOpenDoorResetLevel);
	DDX_Check(pDX, IDC_CHECK_NO_LEVEL, m_bNoLevel);
	DDX_Text(pDX, IDC_EDIT_NO_LEVEL, m_iNoLevel);
	DDX_Check(pDX, IDC_CHECK_HIGH_HIGH_LEVEL, m_bHighHighLevel);
	DDX_Text(pDX, IDC_EDIT_HIGH_HIGH_LEVEL, m_iHighHighLevel);
	DDX_Check(pDX, IDC_CHECK_HIGH_LEVEL_CLOSE_HIGH_LEVEL, m_bHighLevelCloseHighLevel);
	DDX_Check(pDX, IDC_CHECK_HIGH_LEVEL_CLOSE_LOW_LEVEL, m_bHighLevelCloseLowLevel);
	DDX_Check(pDX, IDC_CHECK_LOW_LEVEL_CLOSE_HIGH_LEVEL, m_bLowLevelCloseHighLevel);
	DDX_Check(pDX, IDC_CHECK_LOW_LEVEL_CLOSE_LOW_LEVEL, m_bLowLevelCloseLowLevel);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_CLOSE_HIGH_LEVEL, m_iHighLevelCloseHighLevel);
	DDX_Text(pDX, IDC_EDIT_HIGH_LEVEL_CLOSE_LOW_LEVEL, m_iHighLevelCloseLowLevel);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_CLOSE_HIGH_LEVEL, m_iLowLevelCloseHighLevel);
	DDX_Text(pDX, IDC_EDIT_LOW_LEVEL_CLOSE_LOW_LEVEL, m_iLowLevelCloseLowLevel);
	DDX_Text(pDX, IDC_EDIT_NO_PTC_LEVEL, m_iNoPTCLevel);
	//}}AFX_DATA_MAP
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
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
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
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTakeoutDlg message handlers
static CDC MemDC;
static CBitmap bitmap;
static CBitmap* pOldBitmap;

void CTakeoutDlg::ShowTestResult_HeYang(bool bDraw,bool level_text)
{
	CRect w_rect;
	if(!this->IsWindowVisible()) 
	{
		return;
	}
	
	this->GetClientRect(w_rect); 
	#define TOP_GAP 100
	#define LEFT_GAP 20

	int SWITCH_PIC_HIGH	=	(w_rect.bottom - w_rect.top  - TOP_GAP     ) / 30		- PIC_GAP;
	int SWITCH_PIC_WIDTH = 	(w_rect.right  - w_rect.left - LEFT_GAP * 2) / 9		- PIC_GAP;


	this->GetClientRect( w_rect);


	int nWidth	= SWITCH_PIC_WIDTH/3;
    int nHeight = SWITCH_PIC_HIGH ;

	CDC *dc = NULL;
	
	CFont font;
	CString msg;
	



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

		if(!bitmap.CreateCompatibleBitmap(dc,w_rect.Width() ,w_rect.Height()))	//建立与显示设备兼容的位图
		{
			if(dc->GetSafeHdc() != NULL)
			{
				ReleaseDC(dc);
			}
			return;
		}

		pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

		CBrush bbrush(COLORREF(RGB(255,255,255)));

		MemDC.FillRect(w_rect,&bbrush);

	int i,j,id;
	if(font.CreatePointFont(SWITCH_PIC_HIGH * 6 * 3,_T("黑体"),&MemDC))
	{
		CFont *oldfont = MemDC.SelectObject(&font); 
		MemDC.SetTextColor(RGB(0,0,0)); 

		CRect rect;
		rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * 0 + nWidth*0;
		rect.top  = TOP_GAP   + (SWITCH_PIC_HIGH + PIC_GAP)* (-4);
		rect.right  = rect.left + nWidth * 24;
		rect.bottom = rect.top + nHeight * 3;
		MemDC.DrawText(_T("JUC温控器自动温度特性机 III"), rect  ,DT_CENTER | DT_VCENTER);

		MemDC.SelectObject(oldfont); 
		font.DeleteObject(); 
	}
	if(font.CreatePointFont(SWITCH_PIC_HIGH * 6,_T("黑体"),&MemDC))
	{
		CFont *oldfont = MemDC.SelectObject(&font); 
		MemDC.SetTextColor(RGB(0,0,0)); 
		for(j = 0; j < 8; j++)
		{
			CRect rect;
			rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*0;
			rect.top  = TOP_GAP   + (SWITCH_PIC_HIGH + PIC_GAP)* (-1);
			rect.right  = rect.left + nWidth;
			rect.bottom = rect.top + nHeight;
			MemDC.DrawText(_T("工位"), rect  ,DT_RIGHT | DT_VCENTER);
			rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*1;
			rect.top  = TOP_GAP   + (SWITCH_PIC_HIGH + PIC_GAP)* (-1);
			rect.right  = rect.left + nWidth;
			rect.bottom = rect.top + nHeight;
			MemDC.DrawText(_T("动作"), rect  ,DT_CENTER | DT_VCENTER);
			rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*2;
			rect.top  = TOP_GAP   + (SWITCH_PIC_HIGH + PIC_GAP)* (-1);
			rect.right  = rect.left + nWidth;
			rect.bottom = rect.top + nHeight;
			MemDC.DrawText(_T("复位"), rect  ,DT_CENTER | DT_VCENTER);

		}

		for(j = 0; j < 8; j++)
		{
			for(i = 0; i < 30; i++)
			{
				MemDC.SetTextColor(RGB(0,0,0)); 
				id  = i  + j * 30;
				msg.Format(_T("%5d"), id + 1); 
				CRect rect;
				rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*0;
				rect.top  = TOP_GAP   + (SWITCH_PIC_HIGH + PIC_GAP)* i;
				rect.right  = rect.left + nWidth;
				rect.bottom = rect.top + nHeight;

				MemDC.DrawText(msg, rect  ,DT_LEFT | DT_VCENTER);

				MemDC.SetTextColor(RGB(255,0,0));
				rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*1;
				rect.top  = TOP_GAP + (SWITCH_PIC_HIGH + PIC_GAP)* i;
				rect.right  = rect.left + nWidth;
				rect.bottom = rect.top + nHeight;
				msg.Format(_T("%5.1f"), G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp); 
				MemDC.DrawText(msg, rect  ,DT_LEFT | DT_VCENTER);
				MemDC.MoveTo(rect.left,rect.top);
				MemDC.LineTo(rect.right,rect.top); 
				MemDC.MoveTo(rect.left,rect.top);
				MemDC.LineTo(rect.left,rect.bottom); 
				MemDC.MoveTo(rect.left,rect.bottom -1);
				MemDC.LineTo(rect.right ,rect.bottom- 1); 

				rect.left = LEFT_GAP  + (SWITCH_PIC_WIDTH + PIC_GAP) * j + nWidth*2;
				rect.top  = TOP_GAP + (SWITCH_PIC_HIGH + PIC_GAP)* i;
				rect.right  = rect.left + nWidth;
				rect.bottom = rect.top + nHeight;
				msg.Format(_T("%5.1f"), G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp); 
				MemDC.DrawText(msg, rect  ,DT_LEFT | DT_VCENTER);
				MemDC.MoveTo(rect.left,rect.top);
				MemDC.LineTo(rect.right,rect.top); 
				MemDC.MoveTo(rect.left,rect.top);
				MemDC.LineTo(rect.left,rect.bottom); 
				MemDC.MoveTo(rect.right,rect.top);
				MemDC.LineTo(rect.right,rect.bottom); 
				MemDC.MoveTo(rect.left,rect.bottom -1);
				MemDC.LineTo(rect.right ,rect.bottom-1); 


			}
		}
		
		MemDC.SelectObject(oldfont); 
		font.DeleteObject(); 
	}
	else
	{
		CSize size = MemDC.GetTextExtent(msg);
		MemDC.TextOut(0  , 0,msg);
	}
		

	if(bDraw)
	{
	
		dc->BitBlt(0,0,w_rect.Width(),w_rect.Height(),&MemDC,0,0,SRCCOPY); 	//显示原图形


	}

	


#ifndef _DEBUG
	if(dc->GetSafeHdc())
	{
		dc->DeleteDC(); 
	}
#endif


}
void CTakeoutDlg::ShowTestResult(bool bDraw,bool level_text)
{
	ShowTestResult_HeYang(bDraw,level_text);
	return;

	int id,i;
	if(bDraw)
	{
		this->UpdateData();
	}
//	bool hasShowed;
	
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



	if(!this->IsWindowVisible()) 
	{
		return;
	}
//
//	CheckFailCloseFlash(&G_TakeDlgSwitchTestResult[m_BoxNr],m_BoxNr);

	CRect w_rect;
	this->GetClientRect(w_rect); 

	int SWITCH_PIC_HIGH	=	(w_rect.bottom - w_rect.top) / G_iLedLines - PIC_GAP;
	int SWITCH_PIC_WIDTH = 	(w_rect.right  - w_rect.left - 160) / G_iLedNrPerLine - PIC_GAP;
	#define TEMP_STRING_HEIGHT	60

	this->GetWindowRect(w_rect); 

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

		if(!bitmap.CreateCompatibleBitmap(dc,(nWidth + PIC_GAP)*G_iLedNrPerLine,(nHeight+PIC_GAP) * G_iLedLines))	//建立与显示设备兼容的位图
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

		MemDC.FillRect(CRect(0,0,(nWidth + PIC_GAP)*G_iLedNrPerLine,(nHeight+PIC_GAP) * G_iLedLines),&bbrush);



#ifdef BOARD_100
	for(i=0; i < 100; i++)
#else
	for(i=0; i < G_iMaxLedNr ; i++)
#endif
	{
#ifdef MAX_LED_NR_256
		if(i % 16 >= 8)
		{
			id = 128 + (i / 16) * 8 + i % 8; 
		}
		else
		{
			id =       (i / 16) * 8 + i % 8;
		}
#else
		id = i;
#endif
		//clear 
		if(bDraw)
		{
			DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,ERROR_LEVEL,false,level_text);
		}
		//hasShowed = false;

	
//不合格的先选出来

//不用的
		switch(CheckTestLevel(m_SwitchConfigPara,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id],m_HeatBoxType))
		{
		case NOT_USED:
			if(m_bNotUsed && bDraw)
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,NOT_USED,true,level_text);
			}
			m_iNotUsed ++;
		break;
		case NO_PTC_LEVEL:
			if(m_bNoPTCLevel && bDraw)
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,NO_PTC_LEVEL,true,level_text);
			}
			m_iNoPTCLevel ++;
			break;

//复位闪烁		
		case CLOSE_FLASH_LEVEL:
			if(m_bCloseFlashLevel && bDraw)
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,CLOSE_FLASH_LEVEL,true,level_text);
			}
			m_iCloseFlashLevel ++;
		break;
//动作闪烁		
		case OPEN_FLASH_LEVEL:
			if(m_bOpenFlashLevel && bDraw)
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OPEN_FLASH_LEVEL,true,level_text);
			}
			m_iOpenFlashLevel ++;
		break;
//高温品
		case HIGH_HIGH_LEVEL:
				if(m_bHighHighLevel  && bDraw)
				{
					DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,HIGH_HIGH_LEVEL,true,level_text);
				}
				m_iHighHighLevel++;
		break;

//不动作品
		case OVER_HIGH_LEVEL:
				if(m_bOverHighLevel  && bDraw)
				{
					DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OVER_HIGH_LEVEL,true,level_text);
				}
				m_iOverHighLevel++;
		break;
		
//复大品
		case CLOSE_HIGH_LEVEL:
			if(m_bCloseHighLevel && bDraw)
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,CLOSE_HIGH_LEVEL,true,level_text);
			}
			m_iCloseHighLevel ++;
		break;
		
		
//复小品		
		case CLOSE_LOW_LEVEL:
			if(m_bCloseLowLevel && bDraw)
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,CLOSE_LOW_LEVEL,true,level_text);
			}
			m_iCloseLowLevel ++;
		break;
//开门复位
		case OPEN_DOOR_RESET_LEVEL:
			if(m_bOpenDoorResetLevel && bDraw)
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OPEN_DOOR_RESET_LEVEL,true,level_text);
			}
			m_iOpenDoorResetLevel ++;
		break;
			
//有用产品			
			
				
//主规格
		case MAIN_LEVEL:
				if(m_bMainLevel && bDraw)
				{
					DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,MAIN_LEVEL,true,level_text);
				}
				m_iMainLevel++;
		break;	
//低规格
		case LOW_LEVEL:
				if(m_bLowLevel && bDraw)
				{
					DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp, LOW_LEVEL,true,level_text);
				}
				m_iLowLevel++;
		break;
			
//高规格
		case HIGH_LEVEL:
			if(m_bHighLevel  && bDraw && IsValidTemp(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp))
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,HIGH_LEVEL,true,level_text);
			}
			m_iHighLevel ++;
		break;
			
				
//低温品			
		case OVER_LOW_LEVEL:
			if(m_bOverLowLevel  && bDraw && IsValidTemp(G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp))
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,OVER_LOW_LEVEL,true,level_text);
			}
			m_iOverLowLevel ++;
			break;
		
//高低复大小
		case NO_LEVEL:
			if(m_bNoLevel  && bDraw )
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][id].CloseTemp,NO_LEVEL,true,level_text);
			}
			m_iNoLevel ++;
		break;
#ifdef _CHANGHONG_
		case HIGH_LEVEL_CLOSE_HIGH_LEVEL:
			if(m_bHighLevelCloseHighLevel  && bDraw )
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[id].OpenTemp,G_TakeDlgSwitchTestResult[id].CloseTemp,HIGH_LEVEL_CLOSE_HIGH_LEVEL,true,level_text);
			}
			m_iHighLevelCloseHighLevel ++;
		break;

		case HIGH_LEVEL_CLOSE_LOW_LEVEL:
			if(m_bHighLevelCloseLowLevel  && bDraw )
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[id].OpenTemp,G_TakeDlgSwitchTestResult[id].CloseTemp,HIGH_LEVEL_CLOSE_LOW_LEVEL,true,level_text);
			}
			m_iHighLevelCloseLowLevel ++;
		break;

		case LOW_LEVEL_CLOSE_HIGH_LEVEL:
			if(m_bLowLevelCloseHighLevel  && bDraw )
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[id].OpenTemp,G_TakeDlgSwitchTestResult[id].CloseTemp,LOW_LEVEL_CLOSE_HIGH_LEVEL,true,level_text);
			}
			m_iLowLevelCloseHighLevel ++;
		break;

		case LOW_LEVEL_CLOSE_LOW_LEVEL:
			if(m_bLowLevelCloseLowLevel  && bDraw )
			{
				DrawOneSwitch(i,G_TakeDlgSwitchTestResult[id].OpenTemp,G_TakeDlgSwitchTestResult[id].CloseTemp,LOW_LEVEL_CLOSE_LOW_LEVEL,true,level_text);
			}
			m_iLowLevelCloseLowLevel ++;
		break;
#endif
		default:
		{
			CString s;
			s.Format(_T("%d#\r\n使用=%d\r\n状态=%d\r\n动作=%d\r\n复位=%d\r\n动作闪动=%d\r\n复位闪动=%d\r\n动作温度=%5.1f\r\n复位温度=%5.1f\r\n"),
			i,
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

		int SWITCH_PIC_HIGH	=	(w_rect.bottom - w_rect.top) / G_iLedLines - PIC_GAP;
		int SWITCH_PIC_WIDTH = 	(w_rect.right  - w_rect.left - 160) / G_iLedNrPerLine - PIC_GAP;
		#define TEMP_STRING_HEIGHT	60

		this->GetWindowRect(w_rect); 


		int nWidth = SWITCH_PIC_WIDTH;
		int nHeight = SWITCH_PIC_HIGH;

		//dc->BitBlt((nWidth +PIC_GAP) * (switchNr % G_iLedNrPerLine),(nHeight+PIC_GAP) * (switchNr / G_iLedNrPerLine),nWidth,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
		CBrush tempBrush(RGB(255,255,0));


		if(G_iMaxLedNr == 500)
		{
			MemDC.FillRect(CRect((nWidth +PIC_GAP) * (05),0,(nWidth +PIC_GAP) * (05)-PIC_GAP,(nHeight+PIC_GAP)*G_iLedLines), &tempBrush);
			MemDC.FillRect(CRect((nWidth +PIC_GAP) * (10),0,(nWidth +PIC_GAP) * (10)-PIC_GAP,(nHeight+PIC_GAP)*G_iLedLines), &tempBrush);
			MemDC.FillRect(CRect((nWidth +PIC_GAP) * (15),0,(nWidth +PIC_GAP) * (15)-PIC_GAP,(nHeight+PIC_GAP)*G_iLedLines), &tempBrush);
			MemDC.FillRect(CRect((nWidth +PIC_GAP) * (20),0,(nWidth +PIC_GAP) * (20)-PIC_GAP,(nHeight+PIC_GAP)*G_iLedLines), &tempBrush);
			MemDC.FillRect(CRect((nWidth +PIC_GAP) * (25),0,(nWidth +PIC_GAP) * (25)-PIC_GAP,(nHeight+PIC_GAP)*G_iLedLines), &tempBrush);
			MemDC.FillRect(CRect(0,(nHeight+PIC_GAP) * 10, (nWidth +PIC_GAP) * G_iLedNrPerLine-PIC_GAP,(nHeight+PIC_GAP) * 10-PIC_GAP), &tempBrush);
		}
		else
		{
			MemDC.FillRect(CRect((nWidth +PIC_GAP) * (5 % G_iLedNrPerLine),0,(nWidth +PIC_GAP) * (5 % G_iLedNrPerLine)-PIC_GAP,(nHeight+PIC_GAP)*G_iLedLines), &tempBrush);
			MemDC.FillRect(CRect(0,(nHeight+PIC_GAP) * 6, (nWidth +PIC_GAP) * 10-PIC_GAP,(nHeight+PIC_GAP) * 6-PIC_GAP), &tempBrush);
		}

		dc->BitBlt(0,0,(nWidth + PIC_GAP)*G_iLedNrPerLine,(nHeight+PIC_GAP) * G_iLedLines,&MemDC,0,0,SRCCOPY); 	//显示原图形


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


void CTakeoutDlg::DrawOneSwitch(int switchNr, float openTemp, float closeTemp,TEST_RESULT_LEVEL level,bool bDraw, bool level_text)
{



	CRect w_rect;
	this->GetClientRect(w_rect); 

	int SWITCH_PIC_HIGH	=	(w_rect.bottom - w_rect.top) / G_iLedLines - PIC_GAP;
	int SWITCH_PIC_WIDTH = 	(w_rect.right  - w_rect.left - 160) / G_iLedNrPerLine - PIC_GAP;
	#define TEMP_STRING_HEIGHT	60

	this->GetWindowRect(w_rect); 


	int nWidth	=	SWITCH_PIC_WIDTH;
    int nHeight =	SWITCH_PIC_HIGH;

	int nTopX	=	(nWidth +PIC_GAP) * (switchNr % G_iLedNrPerLine);
	int nTopY	=	(nHeight+PIC_GAP) * (switchNr / G_iLedNrPerLine);

	if(G_iMaxLedNr == 500)
	{
		nTopX	=	(nWidth +PIC_GAP) * ((int)(switchNr / (G_iLedLines * 5)) * 5 + ((int)(switchNr % (G_iLedLines * 5)) % 5));
		nTopY	=	(nHeight+PIC_GAP) *  (int)((switchNr % (100)) / 5);
	}



	

	CPen	pen;
	CPen*	oldpen;

	
	if(!this->IsWindowVisible()) 
	{
		return;
	}

	//CBitmap所需资源

	
	pen.DeleteObject();
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
	case HIGH_LEVEL:
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
    case HIGH_LEVEL_CLOSE_HIGH_LEVEL:
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

	

	bool same_to_selected_level = false;
	switch(level)
	{
	case MAIN_LEVEL:
		if(m_bMainLevel)
			same_to_selected_level = true;
		break;
	case HIGH_LEVEL:
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


	
		if(same_to_selected_level == true)
		{
			if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsUsed == true 
				&& G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosing == false)
			{
				MemDC.FillSolidRect(CRect(nTopX,nTopY, nTopX+nWidth,nTopY+nHeight),RGB(0x00,0xE0,0x00));
			}
		}
		else
		{
			if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosing == false 
				&& G_TakeDlgSwitchTestResult_Bak[m_iTakeBoardType][switchNr].IsClosing == true )
			{

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
				
				
			}
		}

	{
		CString msg;
		CSize size;
//open temp
		MemDC.SetBkMode(TRANSPARENT);
		MemDC.SetTextColor(RGB(0,0,0));
		msg.Format(_T("%.1f"),openTemp);
	
		if(level != NOT_USED && level != ERROR_LEVEL
			&& level != OVER_HIGH_LEVEL )
			
		{
			CFont font;
			if(font.CreatePointFont(nHeight*10/3,_T("黑体"),&MemDC))
			{
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosing == true) 
				{
					MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF)); 
				}
				else
				{
					MemDC.SetTextColor(RGB(0x00,0x00,0x00)); 
				}
				CFont *oldfont = MemDC.SelectObject(&font); 
				size = MemDC.GetTextExtent(msg);
				MemDC.TextOut(nTopX+nWidth-size.cx, nTopY,msg);
				MemDC.SelectObject(oldfont); 
				font.DeleteObject(); 
			}
			else
			{
				size = MemDC.GetTextExtent(msg);
				MemDC.TextOut(nTopX+(nWidth - size.cx)/2  , nTopY+(nHeight - size.cy)/2,msg);
			}
			/*
			if(m_SwitchTestResult[switchNr].IsClosing == false) 
			{
				MemDC.SetTextColor(RGB(0x00,0x00,0x00)); 
			}
			else
			{
				MemDC.SetTextColor(RGB(0x00,0xFF,0x00)); 
			}
			size = MemDC.GetTextExtent(msg);
			MemDC.TextOut(nTopX,nTopY, msg);
		*/
		}

//nr		
		//MemDC.SetTextColor(RGB(255,255,255));
		if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosing == true) 
		{
			MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF)); 
		}
		else
		{
			MemDC.SetTextColor(RGB(0x00,0x00,0x00)); 
		}
		
		msg.Format(_T("%02d"),(((switchNr ) +1) % 100));
		

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

			CFont font;
			if(font.CreatePointFont((nHeight*10+20)/3,_T("黑体"),&MemDC))
			{
				CFont *oldfont = MemDC.SelectObject(&font); 
				size = MemDC.GetTextExtent(msg);
				MemDC.TextOut(nTopX  , nTopY+(nHeight - size.cy)/3,msg);
				MemDC.SelectObject(oldfont); 
				font.DeleteObject(); 
			}
			else
			{
				size = MemDC.GetTextExtent(msg);
				MemDC.TextOut(nTopX  , nTopY+(nHeight - size.cy)/3,msg);
			}
			
		}
		else
		{
			
				CFont font;
				if(font.CreatePointFont((nHeight*10+20)/2,_T("黑体"),&MemDC))
				{
					CFont *oldfont = MemDC.SelectObject(&font); 
					size = MemDC.GetTextExtent(level_str[level]);
					MemDC.TextOut(nTopX/*+(nWidth - size.cx)/2 */ , nTopY+(nHeight - size.cy)/10,level_str[level]);
					MemDC.SelectObject(oldfont); 
					font.DeleteObject(); 
				}
				else
				{
					size = MemDC.GetTextExtent(level_str[level]);
					MemDC.TextOut(nTopX/*+(nWidth - size.cx)/2*/  , nTopY+(nHeight - size.cy)/10,level_str[level]);
				}
			
		}


		MemDC.SetTextColor(RGB(0,0x00,0xFF));
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
			msg.Format(_T("%.1f"),closeTemp);
		}

		if(level != NOT_USED && level != ERROR_LEVEL
			&& level != OVER_HIGH_LEVEL )
		{
			CFont font;
			if(font.CreatePointFont(nHeight*10/3,_T("黑体"),&MemDC))
			{
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][switchNr].IsClosing == true) 
				{
					MemDC.SetTextColor(RGB(0xFF,0xFF,0xFF)); 
				}
				else
				{
					MemDC.SetTextColor(RGB(0x00,0x00,0x00)); 
				}
				CFont *oldfont = MemDC.SelectObject(&font); 
				size = MemDC.GetTextExtent(msg);
				MemDC.TextOut(nTopX+nWidth - size.cx, nTopY+nHeight - size.cy,msg);
				MemDC.SelectObject(oldfont); 
				font.DeleteObject(); 
			}
			else
			{
				size = MemDC.GetTextExtent(msg);
				MemDC.TextOut(nTopX+nWidth - size.cx  , nTopY+nHeight - size.cy,msg);
			}
			//size = MemDC.GetTextExtent(msg); 
			//MemDC.TextOut(nTopX+nWidth - size.cx, nTopY+nHeight - size.cy ,msg);
		}
		
	}

	if(pen.GetSafeHandle())
	{
	//	pen.DeleteObject(); 
	}
//	ReleaseDC(dc);
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

		int SWITCH_PIC_HIGH	=	(w_rect.bottom - w_rect.top) / G_iLedLines - PIC_GAP;
		int SWITCH_PIC_WIDTH = 	(w_rect.right  - w_rect.left - 160) / G_iLedNrPerLine - PIC_GAP;
		CWnd *w;
		int height;
		height = (w_rect.bottom - w_rect.top) / 25;

		item_rect.top = 10;

		w = GetDlgItem(IDC_CHECK_MAIN_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top          ,100,height);
		w->ShowWindow(SW_HIDE); 

		w = GetDlgItem(IDC_CHECK_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		w->ShowWindow(SW_HIDE); 

		w = GetDlgItem(IDC_CHECK_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		w->ShowWindow(SW_HIDE); 

		w = GetDlgItem(IDC_CHECK_HIGH_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		w->ShowWindow(SW_HIDE); 

		
		w = GetDlgItem(IDC_CHECK_OVER_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		w->ShowWindow(SW_HIDE); 
		
		w = GetDlgItem(IDC_CHECK_OVER_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		w->ShowWindow(SW_HIDE); 
		
		w = GetDlgItem(IDC_CHECK_OPEN_DOOR_RESET);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		w->ShowWindow(SW_HIDE); 
		
		w = GetDlgItem(IDC_CHECK_CLOSE_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		w->ShowWindow(SW_HIDE); 
		
		w = GetDlgItem(IDC_CHECK_CLOSE_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		w->ShowWindow(SW_HIDE); 
		
		w = GetDlgItem(IDC_CHECK_OPEN_FLASH);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		w->ShowWindow(SW_HIDE); 
		
		w = GetDlgItem(IDC_CHECK_CLOSE_FLASH);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,100,height);
		w->ShowWindow(SW_HIDE); 
		

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
		w->ShowWindow(SW_HIDE); 
		item_rect.top = 10;
		
		w = GetDlgItem(IDC_EDIT_MAIN_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top          ,60,height);
		w->ShowWindow(SW_HIDE); 

		w = GetDlgItem(IDC_EDIT_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE); 

		w = GetDlgItem(IDC_EDIT_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE); 

		w = GetDlgItem(IDC_EDIT_HIGH_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE);
		
		w = GetDlgItem(IDC_EDIT_OVER_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE); 
		
		w = GetDlgItem(IDC_EDIT_OVER_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE);
		
		w = GetDlgItem(IDC_EDIT_OPEN_DOOR_RESET);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE); 
		
		w = GetDlgItem(IDC_EDIT_CLOSE_HIGH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE); 
		
		w = GetDlgItem(IDC_EDIT_CLOSE_LOW_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE);
		
		w = GetDlgItem(IDC_EDIT_OPEN_FLASH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE); 
		
		w = GetDlgItem(IDC_EDIT_CLOSE_FLASH_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
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
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE); 

		w = GetDlgItem(IDC_EDIT_NO_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE); 
		
		w = GetDlgItem(IDC_EDIT_NO_ACTION_LEVEL);
		w->MoveWindow(w_rect.right - 60, item_rect.top += height,60,height);
		w->ShowWindow(SW_HIDE);

		w = GetDlgItem(IDC_RADIO_ONLY_ONE);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height,50,height);
		w->ShowWindow(SW_HIDE); 

		w = GetDlgItem(IDC_RADIO_MULTI_SELECT);
		w->MoveWindow(w_rect.right - 110, item_rect.top,50,height);
		w->ShowWindow(SW_HIDE); 

		w = GetDlgItem(IDC_RADIO_ALL_SELECT);
		w->MoveWindow(w_rect.right - 60, item_rect.top ,50,height);
		w->ShowWindow(SW_HIDE); 


		w = GetDlgItem(IDC_STATIC_MSG);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height*1.5);
		w->ShowWindow(SW_HIDE);

		w = GetDlgItem(IDC_BUTTON_RECORD_VIEW2);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height*1.5 ,150,height);
		w = GetDlgItem(IDC_BUTTON_TEMP_VIEW);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
		w = GetDlgItem(IDC_BUTTON_SAVE_EXCEL);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
		w = GetDlgItem(IDC_BUTTON_PREVIEW);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);

		w = GetDlgItem(IDC_BUTTON_TODAY_CHANLIANG);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);
		w = GetDlgItem(IDC_BUTTON_PRINT);
		w->MoveWindow(w_rect.right - 160, item_rect.top += height ,150,height);

	}


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTakeoutDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	ShowTestResult(true);



	// Do not call CDialog::OnPaint() for painting messages
}



void CTakeoutDlg::OnCheckMainLevel() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();

	int i;
	for(i = 0; i < G_iMaxLedNr; i++)
	{
		if((MAIN_LEVEL != CheckTestLevel(m_SwitchConfigPara,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatBoxType))
			&& (NOT_USED != CheckTestLevel(m_SwitchConfigPara,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatBoxType)))
		{
			if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][i].IsClosing)
			{
				//char level_str[18][10]
				CString s;
				s.Format(_T("还有 %s 的没拔板！"),level_str[CheckTestLevel(m_SwitchConfigPara,G_TakeDlgSwitchTestResult[m_iTakeBoardType][i],m_HeatBoxType)]);
				AfxMessageBox(s, MB_ICONQUESTION);
				break;
			}
		}

	}

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
	ShowTestResult(true);
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

	for(int i =0 ; i < G_iMaxLedNr; i++)
	{
		memcpy(&G_TakeDlgSwitchTestResult_Bak[m_iTakeBoardType][i], &G_TakeDlgSwitchTestResult[m_iTakeBoardType][i], sizeof(G_TakeDlgSwitchTestResult[this->m_iTakeBoardType][i]));
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

void CTakeoutDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	
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

BOOL CTakeoutDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN) //判断是否有按键按下
    {
       switch(pMsg->wParam)
       {
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
       }
	}

	return CDialog::PreTranslateMessage(pMsg);
}


HBRUSH CTakeoutDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	switch(nCtlColor)
	{
	case CTLCOLOR_DLG:
		hbr = G_WhiteBrush;
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
		hbr = G_bkBrush;
		pDC->SetBkColor(BK_COLOR); 
		pDC->SetTextColor(RGB(255, 255, 60));
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
	
	
	for(j = 0; j < G_iMaxLedNr;j++)
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

	m_ProductTempBarDlg.m_BeginTestTime		= m_BeginTestTime;
	m_ProductTempBarDlg.m_SwitchConfigPara	= m_SwitchConfigPara;

	m_ProductTempBarDlg.DoModal(); 
}

void CTakeoutDlg::OnBnClickedButtonSaveExcel()
{
	// TODO: 在此添加控件通知处理程序代码
	int j;
	char str[1000];
	char str1[1000];
	static TCHAR szFilter[] = _T("Excel Files (*.csv)|*.csv|");
	CString  file_name;
	
	CString s ;
	s.Format(_T("生产记录%04d%02d%02d%02d%02d%02d"),
		m_BeginTestTime.GetYear(), 
		m_BeginTestTime.GetMonth(), 
		m_BeginTestTime.GetDay(), 
		m_BeginTestTime.GetHour(), 
		m_BeginTestTime.GetMinute(), 
		m_BeginTestTime.GetSecond()); 
		
	
	CFileDialog dlg(false,_T("*.csv"),s,NULL,szFilter);

	int ret = dlg.DoModal();
	
	CFile RecFile;

	if(ret == IDOK)
	{
		file_name = dlg.GetPathName();
		if(RecFile.Open(file_name,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate ))
		{
			sprintf_s(str,"序号,动作温度,复位温度,规格,动作时间,复位时间\r\n");
			RecFile.Write(str,strlen(str));
				
			for(j = 0; j < G_iMaxLedNr;j++)
			{
				if(G_iMaxLedNr == 500)
				{
					if((j % 12) < 9)
					{
						sprintf_s(str,"%d%d,",j/12,(j%12)+1 );				
					}
					else if((j % 12) == 9)
					{
						sprintf_s(str,"%d0,",j/12 + 1);				
					}
					else if((j % 12) == 10)
					{
						sprintf_s(str,"%dA,",j/12 + 1);				
					}
					else if((j % 12) == 11)
					{
						sprintf_s(str,"%dB,",j/12 +1);				
					}
				}
				else
				{
					sprintf_s(str,"%d,",j + 1);				
				}
				if(G_TakeDlgSwitchTestResult[m_iTakeBoardType][j].IsUsed) 
				{
					CTime t		= GetRecordTime(G_TakeDlgSwitchTestResult[m_iTakeBoardType][j].OpenTime); 
					CTime t1	= GetRecordTime(G_TakeDlgSwitchTestResult[m_iTakeBoardType][j].CloseTime); 
					
					sprintf_s(str1,"%5.1f,%5.1f,%s,%04d/%02d/%02d %02d:%02d:%02d,%04d/%02d/%02d %02d:%02d:%02d\r\n",G_TakeDlgSwitchTestResult[m_iTakeBoardType][j].OpenTemp,G_TakeDlgSwitchTestResult[m_iTakeBoardType][j].CloseTemp,level_str_4_csv[CheckTestLevel(m_SwitchConfigPara,G_TakeDlgSwitchTestResult[m_iTakeBoardType][j],HEAT_BOX_TYPE_NORMAL)],
						t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),
						t1.GetYear(),t1.GetMonth(),t1.GetDay(),t1.GetHour(),t1.GetMinute(),t1.GetSecond());   
					strcat_s(str,str1);
					RecFile.Write(str,strlen(str));
				}
				else
				{
					sprintf_s(str1,"%s", "-,-,无产品,-,-\r\n");
					strcat_s(str,str1);
					RecFile.Write(str,strlen(str));
				}
			}

			RecFile.Close();
		}
	}
	
	

}

void CTakeoutDlg::OnBnClickedButtonPreview()
{
	// TODO: 在此添加控件通知处理程序代码
	CPrnPreview dlg;
	
	SWITCH_TEST_RESULT_EX m_SwitchTestResult_Ex[500];

	dlg.m_TestParaName	= m_ParaName;
	dlg.m_EndTestTime	= m_EndTestTime;
	dlg.m_BeginTestTime = m_BeginTestTime;
	dlg.m_BoxNr			= m_BoxNr;
	dlg.m_SwitchConfigPara = m_SwitchConfigPara;

	for(int j = 0; j < G_iMaxLedNr;j++)
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

	
	sDir.Format(_T("log\\box%d\\"),m_BoxNr+1);

	sDir = theAppDirectory + sDir;

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

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

			if(m_FileTime.GetYear() == cur.GetYear() &&  m_FileTime.GetMonth() == cur.GetMonth() && m_FileTime.GetDay() == cur.GetDay())
			{
			
				if(RecFile.Read(&record,sizeof(record)) <= sizeof(record)) 
				{
					for(int i = 0; i < ::G_iMaxLedNr; i++)
					{
						switch(CheckTestLevel(record.ConfigPara,record.TestResult[i],(HEAT_BOX_TYPE)record.SwitchType))
							{
							case MAIN_LEVEL:
								m_iMainLevel++;
								break;
							case HIGH_LEVEL:
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
	m_iNotUsedLevel +
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
		CPrintDialog prnDlg(true);//false);
	CString printer;
	CDC dc;
	int i;
	
	

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
	i=	dc.StartPage();

	CString BoxNr;
	SWITCH_TEST_RESULT_EX SwitchTestResult_Ex[500];

	

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

	for(int i = 0; i <  G_iMaxLedNr; i++)
	{
		SwitchTestResult_Ex[i].ID = i;
		SwitchTestResult_Ex[i].result = G_SwitchTestResult[i];
	}

	PrintReport(&dc,app_name, m_BoxNr,
				m_BeginTestTime,m_EndTestTime,
				m_ParaName,ProductType, _T(""),
				m_SwitchConfigPara,SwitchTestResult_Ex,
				TRUE,0,249,(HEAT_BOX_TYPE)m_HeatBoxType);

	


	i =	dc.EndPage(); 
	
	if(G_iMaxLedNr > 250)
	{
		i = dc.StartPage();
		PrintReport(&dc,app_name, m_BoxNr,
					m_BeginTestTime,m_EndTestTime,
					m_ParaName,ProductType,_T(""), 
					m_SwitchConfigPara,SwitchTestResult_Ex,
					TRUE,250,G_iMaxLedNr - 1,(HEAT_BOX_TYPE)m_HeatBoxType);

		i =	dc.EndPage(); 
	}

	i =	dc.EndDoc(); 
	
	dc.DeleteDC();
}
