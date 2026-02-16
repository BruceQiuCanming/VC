// RecordViewDlg.cpp : implementation file
//4821 6825

#include "stdafx.h"
#include "HeatBox.h"
#include "RecordViewDlg.h"
#include "ProductTempBar.h"
#include "RecordPrintPriew.h"
#include "ChanliangTongjiDlg.h"
#include "SwitchDlg.h"
#include "TakeDlg.h"
#include <stdio.h>

#include "shlwapi.h"

#pragma comment(lib,"shlwapi.lib")


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern TCHAR level_str_4_csv[18][10];
/////////////////////////////////////////////////////////////////////////////
// CRecordViewDlg dialog

CString strParaName[] =
{
		"产品规格",
		"产品类型",
	    "主规格",
		"主规负偏差",
		"主规正偏差",
		"高规格",
		"高规负偏差",
		"高规正偏差",
		"低规格",
		"低规负偏差",
		"低规正偏差",
		"主规复位温度",
		"主规复位负偏差",
		"主规复位正偏差",
		"高规复位温度",
		"高规复位负偏差",
		"高规复位正偏差",
		"低规复位温度",
		"低规复位负偏差",
		"低规复位正偏差",
		"开门复位"
};

extern TEST_RESULT_LEVEL CheckTestLevel(SWITCH_CONFIG_PARA SwitchConfigPara,
			SWITCH_TEST_RESULT SwitchTestResult,HEAT_BOX_TYPE HeatBoxType);

void TongJi(double *open_max_temp, double *open_min_temp, double *open_avr_temp, double *close_max_temp, double *close_min_temp, double *close_avr_temp, int *total_product, int *total_main_level,SWITCH_CONFIG_PARA ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,HEAT_BOX_TYPE m_HeatBoxType)
{
		CString str;
		(*open_max_temp) = 0.0f;
		(*open_min_temp) = 999.9f;
		(*open_avr_temp) = 0.0f;
		(*close_max_temp)= 0.0f;
		(*close_min_temp)= 999.9f;
		(*close_avr_temp)= 0.0f;
		(*total_product) = G_iMaxLedNr;
		(*total_main_level) = 0;
		int  open_count=0,close_count=0;  
		int j;
		float temp;

		for(j = 0; j < G_iMaxLedNr;j++)
		{
				switch(AfxGetApp()->GetProfileIntW(_T("SETTING"),_T("PRINT TYPE"),0))
				{
				case 1:
					switch(CheckTestLevel(ConfigPara,TestResult[j].result,m_HeatBoxType))
					{
					case MAIN_LEVEL:
					case HIGH_LEVEL:
					case LOW_LEVEL:
						break;
				
					default:
						(*total_product) --;
						continue;
					}
					break;
				case 2:
					switch(CheckTestLevel(ConfigPara,TestResult[j].result,m_HeatBoxType))
					{
					case MAIN_LEVEL:
						break;
					default:
						(*total_product) --;
						continue;					;
					}
					break;
				
				}

			

			switch(CheckTestLevel(ConfigPara,TestResult[j].result,m_HeatBoxType))
			{
			case MAIN_LEVEL:
				(*total_main_level) ++;
				break;
			case HIGH_LEVEL:
				(*total_main_level) ++;
				break;
			case LOW_LEVEL:
				(*total_main_level) ++;
				break;
			case NOT_USED:
				(*total_product) --;
				break;
			}
			temp = TestResult[j].result.OpenTemp ;
			if(temp > 0.0f && temp < 200.0f)
			{
				if((*open_max_temp) < temp)
				{
					(*open_max_temp) = temp;
				}
				if((*open_min_temp) > temp)
				{
					(*open_min_temp) = temp;
				}
				(*open_avr_temp) += temp;
				open_count ++;
			}
			
			temp = TestResult[j].result.CloseTemp ;
			if(temp > 0.0f && temp < 200.0f)
			{
				if((*close_max_temp) < temp)
				{
					(*close_max_temp) = temp;
				}
				if((*close_min_temp) > temp)
				{
					(*close_min_temp) = temp;
				}
				(*close_avr_temp) += temp;
				close_count ++;
			}
			
		}
		if(open_count > 0)
		{
			(*open_avr_temp) = (*open_avr_temp) / open_count;
		}
		if(close_count > 0)
		{
			(*close_avr_temp) = (*close_avr_temp) / close_count;
		}
}

extern CString G_sCompany;
void PrintReport_bmp(CDC *dc,CRect page_rect,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,int start_switch_nr,int end_switch_nr,HEAT_BOX_TYPE HeatBoxType)
{
	Title = _T("热保护器测试报告");
//#define MAX_LED_NR_128	
	CFont *font=dc->GetCurrentFont();  
	LOGFONT logfont; 
	font->GetLogFont(&logfont); 

	
	CString str;
	
	
	
	CBrush whitebrush(COLORREF(RGB(255,255,255)));
	dc->FillRect(page_rect,&whitebrush);

	int FontPoint = page_rect.right / 2;

	
	CFont newFont;
	CFont* oldFont;
	CSize chnSize;
	while(1)
	{
		newFont.CreatePointFont(FontPoint,_T("宋体"),dc);
		oldFont = dc->SelectObject(&newFont);
		chnSize = dc->GetTextExtent(_T("编号 动作 复位 测试结果 "));
		if(page_rect.right  >= (4 *  chnSize.cx))
		{
			break;
		}
		FontPoint -= 20;
		newFont.DeleteObject();
		dc->SelectObject(oldFont);
	}
	oldFont->DeleteObject();
	
	CPen pen(PS_SOLID,chnSize.cy / 8,RGB(0x00,0x00,0x00));
	CPen *oldPen = dc->SelectObject(&pen);
	oldPen->DeleteObject();
	
	chnSize = dc->GetTextExtent(_T("国"));
	
	chnSize.cy  = chnSize.cy *1.5;
	int cur_x,cur_y;
	cur_x = page_rect.right/3;
	cur_y = chnSize.cy;

	dc->TextOut(cur_x,cur_y, Title);
	
	G_sCompany = AfxGetApp()->GetProfileStringW(_T("SETTING"),_T("USER"),_T("常州市润邦电子科技有限公司")) ;

	CString sCompany = _T("公司名称:");
	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	
	sCompany += G_sCompany;

	dc->TextOut(cur_x,cur_y, sCompany);
	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	

	CString sVal;
	//产品规格
	sVal = _T("产品规格:") + TestParaName; 
	dc->TextOut(cur_x,cur_y, sVal);
	//产品类型
	sVal = _T("产品类型:")+ProduceType;
	dc->TextOut(cur_x+chnSize.cx * 16,cur_y,sVal);
	
	sVal = _T("测试说明:")+TestMemo;
	dc->TextOut(cur_x+chnSize.cx * 32,cur_y,sVal);

	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	str.Format(_T("烘箱编号: %d"), BoxNr + 1); 
	str += _T("  开始测试时间: ");
	sVal.Format(_T("%04d-%02d-%02d %02d:%02d"),BeginTime.GetYear(),BeginTime.GetMonth(),BeginTime.GetDay(),BeginTime.GetHour(),BeginTime.GetMinute());
	str += sVal;
	str += _T("   结束测试时间: ");
	sVal.Format(_T("%04d-%02d-%02d %02d:%02d"),EndTime.GetYear(),EndTime.GetMonth(),EndTime.GetDay(),EndTime.GetHour(),EndTime.GetMinute()); 
	str += sVal;
	dc->TextOut(cur_x,cur_y, str);

	CSize nr_Size = dc->GetTextExtent(_T("编号 "));
	CSize open_temp_Size = dc->GetTextExtent(_T("动作 "));
	CSize close_temp_Size = dc->GetTextExtent(_T("复位 "));
	CSize level_Size = dc->GetTextExtent(_T("测试结果 "));

	cur_x = 0;
	cur_y += (int)(chnSize.cy);
	str = _T("编号 动作 复位 测试结果 ");
	CSize title_Size = dc->GetTextExtent(str);
	int prn_col = page_rect.right / title_Size.cx;
	int i;

	for(i = 0; i < prn_col; i++)
	{
		dc->TextOut(cur_x,cur_y + chnSize.cy* 0.3, str);
		cur_x += page_rect.right / prn_col;
	}

	int table_start_y = cur_y;

	cur_y += (int)(chnSize.cy *1.5);
	

	int prn_row = (int)((page_rect.bottom - cur_y) / (chnSize.cy *1.0));

	if(prn_row > (128 / prn_col))
	{
		prn_row = (128 / prn_col);
	}

	double open_max_temp = 0.0f;
	double open_min_temp = 999.0f;
	double close_max_temp = 0.0f;
	double close_min_temp = 999.0f;
	double max_open_temp_level;
	double min_open_temp_level;
	double max_close_temp_level;
	double min_close_temp_level;
	
	
	max_open_temp_level = ConfigPara.HighLevel+ConfigPara.HighLevelSpanPlus;
	min_open_temp_level = ConfigPara.LowLevel -ConfigPara.LowLevelSpanMinus; 

	max_close_temp_level = ConfigPara.CloseLevel+ConfigPara.CloseLevelSpanPlus;
	min_close_temp_level = ConfigPara.CloseLevel-ConfigPara.CloseLevelSpanMinus; 

	int count = 0;

	for(i=0; i < G_iMaxLedNr / 8 ; i++)
	{
		count = 0;
		for(int j = 0;j < 8; j++)
		{
			if(TestResult[i* 8 + j].result.IsCloseFlash)
			{
				count ++;
			}
		}
		if( count >= 6)
		{
			for(int j = 0;j < 8; j++)
			{
				TestResult[i* 8 + j].result.IsCloseFlash = false;
			}
		}

		count = 0;
		for(int j = 0;j < 8; j++)
		{
			if(TestResult[i* 8 + j].result.IsOpenFlash)
			{
				count ++;
			}
		}
		if( count >= 4)
		{
			for(int j = 0;j < 8; j++)
			{
				TestResult[i* 8 + j].result.IsOpenFlash = false;
			}
		}
	}



	int   main_level = 0,total_switch = 0;

	for(i = 0; i < prn_row; i++)
	{
		for(int j = 0; j < prn_col; j++)
		{
			cur_x = j * page_rect.right / prn_col;
			if(i+prn_row*j <= end_switch_nr - start_switch_nr)

			{
				int index = i+prn_row * j + start_switch_nr;
				
				
				str.Format(_T("%3d"),TestResult[index].ID+1); 
				

				dc->TextOut(cur_x,cur_y, str);
				cur_x += nr_Size.cx;
				switch(AfxGetApp()->GetProfileIntW(_T("SETTING"),_T("PRINT TYPE"),0))
				{
				case 1:
					switch(CheckTestLevel(ConfigPara,TestResult[index].result,HeatBoxType))
					{
					case MAIN_LEVEL:
					case HIGH_LEVEL:
					case LOW_LEVEL:
						break;
				
					default:
						continue						;
					}
					break;
				case 2:
					switch(CheckTestLevel(ConfigPara,TestResult[index].result,HeatBoxType))
					{
					case MAIN_LEVEL:
						break;
					default:
						continue						;
					}
					break;
				
				default:
					
					break;
				}
				double open_temp = TestResult[index].result.OpenTemp;
				str.Format(_T("%5.1f"),open_temp);
				if(open_temp <= 0.0f || open_temp >= 300.0f)
				{
					str = "-";
				}
				else if(open_temp > max_open_temp_level || open_temp < min_open_temp_level)
				{
					str +="X";
				}
				
				if(TestResult[index].result.IsOpenFlash)
				{
					if(TestResult[index].result.IsUsed)
					{
						str += "V";
					}
				}
				dc->TextOut(cur_x,cur_y, str);
				cur_x += open_temp_Size.cx;
				
				
				double close_temp = TestResult[index].result.CloseTemp;;
				str.Format(_T("%5.1f"),close_temp);
				if(close_temp <= 0.0f || close_temp >= 300.0f)
				{
					str = _T("");
				}
				else if(close_temp > max_close_temp_level || close_temp < min_close_temp_level)
				{
					str +=_T("X");
				}

				if(TestResult[index].result.IsCloseFlash)
				{
					if(TestResult[index].result.IsUsed) 
						str += _T("V");
				}
				dc->TextOut(cur_x,cur_y, str);
				cur_x += close_temp_Size.cx;
				
				switch(CheckTestLevel(ConfigPara,TestResult[index].result,HeatBoxType))
				{
				case MAIN_LEVEL:
					main_level ++;
					str =_T( "");//"主规格";
					break;
				case OVER_HIGH_LEVEL:
					str = _T("高温品");
					break;
				case OVER_LOW_LEVEL:
					str = _T("低温品");
					break;
				case HIGH_LEVEL:
					str =_T("高规格G");
					break;
				case LOW_LEVEL:
					str =_T("低规格D");
					break;
				case CLOSE_LOW_LEVEL:
					str = _T("复大品X");
					break;
				case CLOSE_HIGH_LEVEL:
					str = _T("复小品X");
					break;
				case OPEN_FLASH_LEVEL:
					str = _T("动闪V");
					break;
				case CLOSE_FLASH_LEVEL:
					str = _T("复闪V");
					break;
				case OPEN_DOOR_RESET_LEVEL:
					str = _T("开门复位X");
					break;
				case NOT_USED:
					str = _T("无产品");
					break;
				default:
					str = _T("高低复大小");
					break;
				}
				
				dc->TextOut(cur_x,cur_y, str);
				cur_x += level_Size.cx;
				str = "";

				if(NOT_USED == CheckTestLevel(ConfigPara,TestResult[index].result,HeatBoxType))
				{}
				else
				{
					total_switch ++;
					if(open_temp > 0)
					{
						if(open_temp > open_max_temp)
						{
							open_max_temp = open_temp;
						}
						if(open_temp < open_min_temp)
						{
							open_min_temp = open_temp;
						}
					}
					if(close_temp > 0)
					{
						if(close_temp > close_max_temp)
						{
							close_max_temp = close_temp;
						}
						if(close_temp < close_min_temp)
						{
							close_min_temp = close_temp;
						}
					}
				}
				
			}
		}
		
		cur_y += (int)(chnSize.cy);
	}

	
	for(i = 0; i <= prn_row + 1; i++)
	{
		dc->MoveTo(0,table_start_y + i * chnSize.cy + chnSize.cy* 0.3);
		dc->LineTo(page_rect.right,table_start_y + i * chnSize.cy + chnSize.cy*0.3); 
	}
	
	
	for(i = 0; i < prn_col; i++)
	{
		cur_x = i*page_rect.right / prn_col;
		int start_y = table_start_y + chnSize.cy* 0.3;
		int end_y = table_start_y + (prn_row + 1) * chnSize.cy + chnSize.cy*0.3;
		dc->MoveTo(cur_x, start_y);
		dc->LineTo(cur_x, end_y);

		dc->MoveTo(cur_x+nr_Size.cx, start_y);
		dc->LineTo(cur_x+nr_Size.cx, end_y);

		dc->MoveTo(cur_x+nr_Size.cx+ open_temp_Size.cx, start_y);
		dc->LineTo(cur_x+nr_Size.cx+ open_temp_Size.cx, end_y);
		
		dc->MoveTo(cur_x+nr_Size.cx+ open_temp_Size.cx+ close_temp_Size.cx, start_y);
		dc->LineTo(cur_x+nr_Size.cx+ open_temp_Size.cx+ close_temp_Size.cx, end_y);

		dc->MoveTo((i+1)*page_rect.right / prn_col - 1, start_y);
		dc->LineTo((i+1)*page_rect.right / prn_col - 1, end_y);
		
	}
	
	
//	cur_y += (int)(chnSize.cy *1.0);

	double open_avr_temp,close_avr_temp;
	int total_product,total_main_level;

	TongJi(&open_max_temp, &open_min_temp, &open_avr_temp, &close_max_temp, &close_min_temp, &close_avr_temp, &total_product, &total_main_level,
		ConfigPara,TestResult,bTestCloseLevel,HeatBoxType);
	
	table_start_y = cur_y;
	
	cur_x = chnSize.cx ;
	dc->TextOut(cur_x,cur_y, _T("产品数"));
	cur_x += page_rect.right /6;
	str.Format(_T("%d"),total_product);
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("合格品"));
	cur_x += page_rect.right /6;
	str.Format(_T("%d"),total_main_level);
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("合格率"));
	cur_x += page_rect.right /6;
	if(total_product > 0)
	{
		str.Format(_T("%5.1f%%"),(float)(total_main_level*100.0f/total_product));
	}
	else
	{
		str = _T("   0.0%");
	}
	dc->TextOut(cur_x,cur_y, str);
	
	cur_y += (int)(chnSize.cy);

	cur_x = chnSize.cx;
	
	dc->TextOut(cur_x,cur_y, _T("动作最高温"));
	
	cur_x += page_rect.right /6;
	if(total_product > 0)
	{
		str.Format(_T("%5.1f"),open_max_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("动作最低温"));
	cur_x += page_rect.right /6;
	if(total_product > 0  && open_min_temp < 400)
	{
		str.Format(_T("%5.1f"),open_min_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("动作平均温"));
	cur_x += page_rect.right /6;
	if(total_product > 0)
	{
		str.Format(_T("%5.1f"),open_avr_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
	dc->TextOut(cur_x,cur_y, str);
	
	cur_y += (int)(chnSize.cy);

	cur_x = chnSize.cx;
	dc->TextOut(cur_x,cur_y, _T("复位最高温"));
	cur_x += page_rect.right /6;
	if(total_product > 0)
	{
		str.Format(_T("%5.1f"),close_max_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("复位最低温"));
	cur_x += page_rect.right /6;
	if(total_product > 0 && close_min_temp < 400)
	{
		str.Format(_T("%5.1f"),close_min_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
	dc->TextOut(cur_x,cur_y, str);

	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("复位平均温"));
	cur_x += page_rect.right /6;
	if(total_product > 0)
	{
		str.Format(_T("%5.1f"),close_avr_temp);
	}
	else
	{
		str = _T(" 0.0");
	}
	dc->TextOut(cur_x,cur_y, str);

	cur_y += (int)(chnSize.cy);

	//CPen pen2(PS_SOLID,chnSize.cy / 8,RGB(0x00,0x00,0x00));
	//oldPen = dc->SelectObject(&pen2);
	//oldPen->DeleteObject();

	for(i = 0; i <= 7; i++)
	{
		cur_x = i*page_rect.right / 6;
		int start_y = table_start_y;
		dc->MoveTo(cur_x, start_y);
		dc->LineTo(cur_x, cur_y);

		dc->MoveTo(cur_x+page_rect.right / 6 - 1, start_y);
		dc->LineTo(cur_x+page_rect.right / 6 - 1, cur_y);
	}

	for(i = 0; i <= 3; i++)
	{
		int start_y = table_start_y + i * chnSize.cy ;
		//int end_y = cur_y;
		dc->MoveTo(0, start_y);
		dc->LineTo(page_rect.right, start_y);
	}

	
	cur_y += (int)(chnSize.cy / 2);
	table_start_y = cur_y;

	cur_x = chnSize.cx;
	dc->TextOut(cur_x,cur_y, _T("主规格温度"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.MainLevel);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("主规负偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.MainLevelSpanMinus);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("主规正偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.MainLevelSpanPlus);
	dc->TextOut(cur_x,cur_y, str);
	cur_y += (int)(chnSize.cy);
//
	cur_x = chnSize.cx;
	dc->TextOut(cur_x,cur_y, _T("复位温度"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseLevel);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("复位负偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.CloseLevelSpanMinus);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("复位正偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseLevelSpanPlus);
	dc->TextOut(cur_x,cur_y, str);
	cur_y += (int)(chnSize.cy);
//
	cur_x = chnSize.cx;
	dc->TextOut(cur_x,cur_y, _T("高规格温度"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.HighLevel);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("高规负偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.HighLevelSpanMinus);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("高规正偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.HighLevelSpanPlus);
	dc->TextOut(cur_x,cur_y, str);
	cur_y += (int)(chnSize.cy);

	cur_x = chnSize.cx;
	dc->TextOut(cur_x,cur_y, _T("复位温度"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseHighLevel);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("复位负偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.CloseHighLevelSpanMinus);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("复位正偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseHighLevelSpanPlus);
	dc->TextOut(cur_x,cur_y, str);
	cur_y += (int)(chnSize.cy);

	//
		cur_x = chnSize.cx;
	dc->TextOut(cur_x,cur_y, _T("低规格温度"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.LowLevel);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("低规负偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.LowLevelSpanMinus);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("低规正偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.LowLevelSpanPlus);
	dc->TextOut(cur_x,cur_y, str);
	cur_y += (int)(chnSize.cy);

	cur_x = chnSize.cx;
	dc->TextOut(cur_x,cur_y, _T("复位温度"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseLowLevel);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("复位负偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),-ConfigPara.CloseLowLevelSpanMinus);
	dc->TextOut(cur_x,cur_y, str);
	cur_x += page_rect.right /6;
	dc->TextOut(cur_x,cur_y, _T("复位正偏差"));
	cur_x += page_rect.right /6;
	str.Format(_T("%5.1f"),ConfigPara.CloseLowLevelSpanPlus);
	dc->TextOut(cur_x,cur_y, str);
	cur_y += (int)(chnSize.cy);

//	CPen pen3(PS_SOLID,chnSize.cy / 8,RGB(0x00,0x00,0x00));
//	oldPen = dc->SelectObject(&pen3);
//	oldPen->DeleteObject();

	for(i = 0; i <= 7; i++)
	{
		cur_x = i*page_rect.right / 6;
		int start_y = table_start_y;
		dc->MoveTo(cur_x, start_y);
		dc->LineTo(cur_x, cur_y);

		dc->MoveTo(cur_x+page_rect.right / 6 - 1, start_y);
		dc->LineTo(cur_x+page_rect.right / 6 - 1, cur_y);
	}
	for(i = 0; i <= 6; i++)
	{
		int start_y = table_start_y + i * chnSize.cy ;
		//int end_y = cur_y;
		dc->MoveTo(0, start_y);
		dc->LineTo(page_rect.right, start_y);
	}

	
	
	//dc->TextOut(page_rect.right/2,page_rect.bottom - chnSize.cy, "制表： 常州市润邦电子科技有限公司 TEL:018052531601,013915838598");

}

void PrintReport(CDC *dc,CString Title ,int BoxNr, CTime BeginTime,CTime EndTime,
				 CString TestParaName,CString ProduceType,CString TestMemo,SWITCH_CONFIG_PARA ConfigPara,SWITCH_TEST_RESULT_EX* TestResult,BOOL bTestCloseLevel,int start_switch_nr,int end_switch_nr,HEAT_BOX_TYPE HeatBoxType)
{
	Title = _T("JUC温控器自动温度特性机测试结果2");
//#define MAX_LED_NR_128	
	CFont *font=dc->GetCurrentFont();  
	LOGFONT logfont; 
	font->GetLogFont(&logfont); 
#define LEFT_WIDTH	100
	CRect page_rect;
	CString str;
	
	page_rect.left = 0;
	page_rect.right = dc->GetDeviceCaps(HORZRES);
	page_rect.top = 0; 
	page_rect.bottom = dc->GetDeviceCaps(VERTRES);   

	page_rect.bottom = page_rect.right * 7008 / 4950 ;//A4

	
	CBrush whitebrush(COLORREF(RGB(255,255,255)));
	dc->FillRect(page_rect,&whitebrush);

	int FontPoint = page_rect.right / 2;

	
	CFont newFont,NumFont,newFont_OpenTemp;
	CFont* oldFont;
	CSize chnSize;
	while(1)
	{
		newFont.CreatePointFont(FontPoint,_T("宋体"),dc);
		oldFont = dc->SelectObject(&newFont);
		chnSize = dc->GetTextExtent(_T("编号动作复位差值结果"));
		if((page_rect.right - LEFT_WIDTH *2) >= (5 *  chnSize.cx))
		{
			newFont_OpenTemp.CreatePointFont(FontPoint * 1.20 ,_T("宋体"),dc);
			break;
		}
		FontPoint -= 10;
		newFont.DeleteObject();
		dc->SelectObject(oldFont);
	}
	oldFont->DeleteObject();
	
	CPen pen(PS_SOLID,1,RGB(0x00,0x00,0x00));
	CPen *oldPen = dc->SelectObject(&pen);
	oldPen->DeleteObject();
	
	chnSize = dc->GetTextExtent(_T("国"));
	
	chnSize.cy  = chnSize.cy *1.25;
	int cur_x,cur_y;
	cur_x = page_rect.right/3;
	cur_y = chnSize.cy;

	dc->TextOut(LEFT_WIDTH + cur_x,cur_y, Title);
	
		CString sVal;
	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	str = _T("测试时间: ");
	sVal.Format(_T("%04d-%02d-%02d %02d:%02d"),BeginTime.GetYear(),BeginTime.GetMonth(),BeginTime.GetDay(),BeginTime.GetHour(),BeginTime.GetMinute());
	str += sVal;
	dc->TextOut(LEFT_WIDTH + cur_x,cur_y, str);

	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	


	//产品规格
	sVal = _T("产品规格:") + TestParaName; 
	dc->TextOut(LEFT_WIDTH + cur_x,cur_y, sVal);
	//产品类型
	sVal = _T("产品形式:")+ProduceType;
	dc->TextOut(LEFT_WIDTH + cur_x+chnSize.cx * 13.5,cur_y,sVal);
	
	sVal = _T("生产批号:")+TestMemo;
	dc->TextOut(LEFT_WIDTH + cur_x+chnSize.cx * 24.5,cur_y,sVal);


	double open_avr_temp,close_avr_temp;
	int total_product,total_main_level;

	double open_max_temp = 0.0f;
	double open_min_temp = 999.0f;
	double close_max_temp = 0.0f;
	double close_min_temp = 999.0f;
	double max_open_temp_level;
	double min_open_temp_level;
	double max_close_temp_level;
	double min_close_temp_level;
	

	TongJi(&open_max_temp, &open_min_temp, &open_avr_temp, &close_max_temp, &close_min_temp, &close_avr_temp, &total_product, &total_main_level,
		ConfigPara,TestResult,bTestCloseLevel,HeatBoxType);


	
	cur_x = 0;
	cur_y += (int)(chnSize.cy *1.5);
	str.Format(_T("动作温度: %5.1f - %5.1f    动作平均:%5.1f        复位温度: %5.1f - %5.1f    动作平均:%5.1f"),ConfigPara.MainLevel - ConfigPara.MainLevelSpanMinus,ConfigPara.MainLevel + ConfigPara.MainLevelSpanPlus,open_avr_temp,
																		ConfigPara.CloseLevel - ConfigPara.CloseLevelSpanMinus ,ConfigPara.CloseLevel + ConfigPara.CloseLevelSpanPlus,close_avr_temp );
	dc->TextOut(LEFT_WIDTH + cur_x,cur_y, str);


	CSize nr_Size = dc->GetTextExtent(_T("编号"));
	CSize open_temp_Size = dc->GetTextExtent(_T("动作"));
	CSize close_temp_Size = dc->GetTextExtent(_T("复位"));
	CSize span_temp_Size = dc->GetTextExtent(_T("差值"));
	CSize level_Size = dc->GetTextExtent(_T("结果"));

	cur_x = 0;
	cur_y += (int)(chnSize.cy);
	str = _T("编号动作复位差值结果");
	CSize title_Size = dc->GetTextExtent(str);
	int prn_col = (page_rect.right - LEFT_WIDTH *2)/ title_Size.cx;
	int i;

	for(i = 0; i < prn_col; i++)
	{
		dc->TextOut(LEFT_WIDTH+cur_x,cur_y + chnSize.cy* 0.3, str);
		cur_x += (page_rect.right - LEFT_WIDTH *2) / prn_col;
	}

	int table_start_y = cur_y;

	cur_y += (int)(chnSize.cy *1.5);
	

	int prn_row = (int)((page_rect.bottom - cur_y) / (chnSize.cy *1.0));

	if(prn_row > (250 / prn_col))
	{
		prn_row = (250 / prn_col);
	}

	
	
	max_open_temp_level = ConfigPara.HighLevel+ConfigPara.HighLevelSpanPlus;
	min_open_temp_level = ConfigPara.LowLevel -ConfigPara.LowLevelSpanMinus; 

	max_close_temp_level = ConfigPara.CloseLevel+ConfigPara.CloseLevelSpanPlus;
	min_close_temp_level = ConfigPara.CloseLevel-ConfigPara.CloseLevelSpanMinus; 

	int count = 0;

	for(i=0; i < G_iMaxLedNr / 8 ; i++)
	{
		count = 0;
		for(int j = 0;j < 8; j++)
		{
			if(TestResult[i* 8 + j].result.IsCloseFlash)
			{
				count ++;
			}
		}
		if( count >= 6)
		{
			for(int j = 0;j < 8; j++)
			{
				TestResult[i* 8 + j].result.IsCloseFlash = false;
			}
		}

		count = 0;
		for(int j = 0;j < 8; j++)
		{
			if(TestResult[i* 8 + j].result.IsOpenFlash)
			{
				count ++;
			}
		}
		if( count >= 4)
		{
			for(int j = 0;j < 8; j++)
			{
				TestResult[i* 8 + j].result.IsOpenFlash = false;
			}
		}
	}



	int   main_level = 0,total_switch = 0;

	for(i = 0; i < prn_row; i++)
	{
		for(int j = 0; j < prn_col; j++)
		{
			cur_x = j * (page_rect.right - LEFT_WIDTH *2)/ prn_col;
			if(i+prn_row*j <= end_switch_nr - start_switch_nr)

			{
				//int index = i+prn_row * j + start_switch_nr;
				
				int index = i * prn_col + j + start_switch_nr;
				
				str.Format(_T("%d"),TestResult[index].ID+1); 
			
				
				dc->SelectObject(newFont); 
				dc->TextOut(LEFT_WIDTH + cur_x,cur_y, str);
				cur_x += nr_Size.cx;
				switch(AfxGetApp()->GetProfileIntW(_T("SETTING"),_T("PRINT TYPE"),0))
				{
				case 1:
					switch(CheckTestLevel(ConfigPara,TestResult[index].result,HeatBoxType))
					{
					case MAIN_LEVEL:
					case HIGH_LEVEL:
					case LOW_LEVEL:
						break;
				
					default:
						continue						;
					}
					break;
				case 2:
					switch(CheckTestLevel(ConfigPara,TestResult[index].result,HeatBoxType))
					{
					case MAIN_LEVEL:
						break;
					default:
						continue						;
					}
					break;
				
				default:
					
					break;
				}
				double open_temp = TestResult[index].result.OpenTemp;
				
				if(TestResult[index].result.IsOpenFlash)
				{
					str.Format(_T("X%d"),(int)(open_temp + 0.5f));
				}
				else
				{
					str.Format(_T("%d"),(int)(open_temp + 0.5f));
				}
				dc->SelectObject(newFont_OpenTemp); 
				dc->TextOut(LEFT_WIDTH + cur_x,cur_y - 8, str);
				cur_x += open_temp_Size.cx;
				
				
				double close_temp = TestResult[index].result.CloseTemp;;
				
				if(TestResult[index].result.IsCloseFlash)
				{
					str.Format(_T("%dX"),(int)(close_temp + 0.5f));
				}
				else
				{
					str.Format(_T("%d"),(int)(close_temp + 0.5f));
				}
				
				dc->SelectObject(newFont); 
				dc->TextOut(LEFT_WIDTH + cur_x,cur_y, str);
				cur_x += close_temp_Size.cx;
				
				dc->SelectObject(newFont_OpenTemp); 
				str.Format(_T("%d"), (int)(TestResult[index].result.OpenTemp - TestResult[index].result.CloseTemp + 0.5f)); 
				dc->TextOut(LEFT_WIDTH + cur_x,cur_y - 8, str);
				cur_x += level_Size.cx;
				str = "";

				if(NOT_USED == CheckTestLevel(ConfigPara,TestResult[index].result,HeatBoxType))
				{}
				else
				{
					total_switch ++;
					if(open_temp > 0)
					{
						if(open_temp > open_max_temp)
						{
							open_max_temp = open_temp;
						}
						if(open_temp < open_min_temp)
						{
							open_min_temp = open_temp;
						}
					}
					if(close_temp > 0)
					{
						if(close_temp > close_max_temp)
						{
							close_max_temp = close_temp;
						}
						if(close_temp < close_min_temp)
						{
							close_min_temp = close_temp;
						}
					}
				}
				
			}
		}
		
		cur_y += (int)(chnSize.cy);
	}

	dc->SelectObject(newFont); 
	
	for(i = 0; i <= prn_row + 1; i++)
	{
		dc->MoveTo(LEFT_WIDTH,table_start_y + i * chnSize.cy + chnSize.cy* 0.3);
		dc->LineTo(page_rect.right - LEFT_WIDTH ,table_start_y + i * chnSize.cy + chnSize.cy*0.3); 
	}
	
	
	for(i = 0; i <= prn_col; i++)
	{
		cur_x = i*(page_rect.right - LEFT_WIDTH * 2) / prn_col;
		int start_x = table_start_y + chnSize.cy* 0.3;
		int end_y = table_start_y + (prn_row + 1) * chnSize.cy + chnSize.cy*0.3;
		dc->MoveTo(LEFT_WIDTH + cur_x, start_x);
		dc->LineTo(LEFT_WIDTH + cur_x, end_y);

		 if(i < prn_col)
		 {
			dc->MoveTo(LEFT_WIDTH + cur_x+nr_Size.cx, start_x);
			dc->LineTo(LEFT_WIDTH + cur_x+nr_Size.cx, end_y);

			dc->MoveTo(LEFT_WIDTH + cur_x+nr_Size.cx+ open_temp_Size.cx, start_x);
			dc->LineTo(LEFT_WIDTH + cur_x+nr_Size.cx+ open_temp_Size.cx, end_y);
		
		
			dc->MoveTo(LEFT_WIDTH + cur_x+nr_Size.cx+ open_temp_Size.cx+ close_temp_Size.cx, start_x);
			dc->LineTo(LEFT_WIDTH + cur_x+nr_Size.cx+ open_temp_Size.cx+ close_temp_Size.cx, end_y);

			dc->MoveTo(LEFT_WIDTH + cur_x+nr_Size.cx+ open_temp_Size.cx+ close_temp_Size.cx + span_temp_Size.cx, start_x);
			dc->LineTo(LEFT_WIDTH + cur_x+nr_Size.cx+ open_temp_Size.cx+ close_temp_Size.cx + span_temp_Size.cx, end_y);
		}
		
	}
	
	if(newFont.GetSafeHandle())
	{
		newFont.DeleteObject(); 
	}

	if(newFont_OpenTemp.GetSafeHandle())
	{
		newFont_OpenTemp.DeleteObject(); 
	}


}





CRecordViewDlg::CRecordViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRecordViewDlg::IDD, pParent)
{
	m_BoxNr = 0;
	//{{AFX_DATA_INIT(CRecordViewDlg)
	m_Para = _T("");
	//}}AFX_DATA_INIT

	m_SelectTestTime = m_SelectTestTime.GetCurrentTime(); 
}


void CRecordViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRecordViewDlg)
	DDX_Control(pDX, IDC_LIST_RECORD_TIME, m_ListCtrlRecordTime);
	DDX_Control(pDX, IDC_LIST_RECORD, m_ListCtrlRecord);
	DDX_MonthCalCtrl(pDX, IDC_MONTHCALENDAR1, m_SelectTestTime);
	DDX_Text(pDX, IDC_STATIC_PARA, m_Para);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_SORT_TYPE, m_ComboBoxSortType);
	DDX_Control(pDX, IDC_COMBO_PRINT_TYPE, m_ComboPrintType);
}


BEGIN_MESSAGE_MAP(CRecordViewDlg, CDialog)
	//{{AFX_MSG_MAP(CRecordViewDlg)
	ON_WM_CLOSE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_RECORD_TIME, OnDblclkListRecordTime)
	ON_NOTIFY(MCN_SELCHANGE, IDC_MONTHCALENDAR1, OnSelchangeMonthcalendar1)
	ON_BN_CLICKED(IDC_BUTTON_PRN, OnButtonPrn)
	ON_BN_CLICKED(IDC_BUTTON_TEMP_BAR, OnButtonTempBar)
	ON_BN_CLICKED(IDC_BUTTON_PRN_PREVIEW, OnButtonPrnPreview)
	ON_BN_CLICKED(IDC_BUTTON_TONGJI, OnButtonTongji)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS, OnButtonSaveas)
	//}}AFX_MSG_MAP
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_RECORD_TIME, &CRecordViewDlg::OnLvnItemchangedListRecordTime)
	ON_BN_CLICKED(IDC_BUTTON_SAVEAS2, &CRecordViewDlg::OnBnClickedButtonSaveas2)
	ON_BN_CLICKED(IDC_BUTTON_TAKEDLG, &CRecordViewDlg::OnBnClickedButtonTakedlg)
	ON_CBN_SELCHANGE(IDC_COMBO_SORT_TYPE, &CRecordViewDlg::OnCbnSelchangeComboSortType)
	ON_CBN_SELCHANGE(IDC_COMBO_PRINT_TYPE, &CRecordViewDlg::OnCbnSelchangeComboPrintType)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRecordViewDlg message handlers

BOOL CRecordViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	CRect rect;

	m_ListCtrlRecordTime.GetClientRect(&rect); 
	m_ListCtrlRecordTime.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	this->m_ListCtrlRecordTime.InsertColumn(0,_T("开始时间"),LVCFMT_LEFT,rect.Width()*4/10);  
	this->m_ListCtrlRecordTime.InsertColumn(1,_T("测试参数"),LVCFMT_LEFT,rect.Width()*4/10); 
	this->m_ListCtrlRecordTime.InsertColumn(2,_T("结束时间"),LVCFMT_LEFT,rect.Width()*4/10);  
	this->m_ListCtrlRecordTime.InsertColumn(3,_T("测试说明"),LVCFMT_LEFT,rect.Width()*4/10);  
	
	m_ListCtrlRecord.GetClientRect(&rect); 
	m_ListCtrlRecord.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	this->m_ListCtrlRecord.InsertColumn(0,_T("编号"),    LVCFMT_LEFT,50);  
	this->m_ListCtrlRecord.InsertColumn(1,_T("有产品"),  LVCFMT_LEFT,70);  
	this->m_ListCtrlRecord.InsertColumn(2,_T("动作"),	LVCFMT_LEFT,50);  
	this->m_ListCtrlRecord.InsertColumn(3,_T("复位"),	LVCFMT_LEFT,50);  
	this->m_ListCtrlRecord.InsertColumn(4,_T("动闪"),	LVCFMT_LEFT,50);  
	this->m_ListCtrlRecord.InsertColumn(5,_T("复闪"),	LVCFMT_LEFT,50);  
	this->m_ListCtrlRecord.InsertColumn(6,_T("动作温度"),LVCFMT_LEFT,80);  
	this->m_ListCtrlRecord.InsertColumn(7,_T("复位温度"),LVCFMT_LEFT,80);  
 	this->m_ListCtrlRecord.InsertColumn(8,_T("产品规格"),LVCFMT_LEFT,80); 
	this->m_ListCtrlRecord.InsertColumn(9,_T("动闪温度"),LVCFMT_LEFT,80); 
	this->m_ListCtrlRecord.InsertColumn(10,_T("复闪温度"),LVCFMT_LEFT,80); 
	this->m_ListCtrlRecord.InsertColumn(11,_T("动作时间"),LVCFMT_LEFT,80);  
	this->m_ListCtrlRecord.InsertColumn(12,_T("复位时间"),LVCFMT_LEFT,80);  
 	
	m_ComboBoxSortType.AddString(_T("按测试序号")); 
	m_ComboBoxSortType.AddString(_T("按温度由高到低"));
	m_ComboBoxSortType.AddString(_T("按温度由低到高"));
	m_ComboBoxSortType.SetCurSel(0);

	memset(m_SwitchTestResult_Ex,0,sizeof(m_SwitchTestResult_Ex));

	UpdateListCtrlRecordTime(true);

	// TODO: Add extra initialization here
	
	this->m_ComboPrintType.SetCurSel(AfxGetApp()->GetProfileIntW(_T("SETTING"),_T("PRINT TYPE"),0)); 

	this->UpdateData(false); 
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRecordViewDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnClose();
}

void CRecordViewDlg::OnDblclkListRecordTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	int i =(int)(this->m_ListCtrlRecordTime.GetFirstSelectedItemPosition());
	if(i > 0)
	{
		CString s1 = m_ListCtrlRecordTime.GetItemText(i-1,0);  
		CString s2 = m_ListCtrlRecordTime.GetItemText(i-1,2);  
		
		this->UpdateRecordDetail(s1,s2); 
		
	}
	*pResult = 0;
}
 
extern void TraverseFolder(LPCTSTR lpPath,LPCTSTR file_ext,CStringArray *fileStr);

void CRecordViewDlg::UpdateListCtrlRecordTime(bool AllSelect)
{
	CFile RecFile;
	CFileFind ff;
	RECORD	record;
	CString fName;
	CStringArray sa;
	CString sDir;

	m_ListCtrlRecordTime.DeleteAllItems(); 

	sDir.Format(_T("log\\box%d\\"),m_BoxNr+1);

	sDir = theAppDirectory + sDir;

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	TraverseFolder(sDir,_T("*.record"),&sa);

	while(sa.GetCount() > 0)
	{
		CString s,s1,s2,s3,s4;
		s =  sa.GetAt(0);
		
		
		if(RecFile.Open(sDir+s,CFile::modeRead))
		{
			
			CString newName;
			if(s.GetAt(14) != '-')
			{
				newName = s;
				newName.Insert(8,'0');
				RecFile.Close(); 
				CFile::Rename(sDir + s,sDir + newName);  
				s = newName;
				RecFile.Open(sDir+s,CFile::modeRead);
			}

			if(s.GetLength() ==  35)
			{
				newName = s;
				newName.Insert(23,'0');
				RecFile.Close(); 
				CFile::Rename(sDir + s,sDir + newName);  
				s = newName;
				RecFile.Open(sDir+s,CFile::modeRead);
			}
			


			
			if(RecFile.Read(&record,sizeof(record)) <= sizeof(record)) 
			{
				s = s.Left(s.Find(_T(".")));

				s1.Format(_T("%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c"),s.GetAt(0),s.GetAt(1),s.GetAt(2),s.GetAt(3),s.GetAt(4),s.GetAt(5),s.GetAt(6),s.GetAt(7),s.GetAt(8),s.GetAt(9),s.GetAt(10),s.GetAt(11),s.GetAt(12),s.GetAt(13));  
				m_ListCtrlRecordTime.InsertItem(0,s1);
				sa.RemoveAt(0);


				s2 = record.ParaName; 
				m_ListCtrlRecordTime.SetItemText(0,3,s2);

				m_ListCtrlRecordTime.SetItemText(0,1,record.Memo);

				s1.Format(_T("%c%c%c%c-%c%c-%c%c %c%c:%c%c:%c%c"),s.GetAt(15+0),s.GetAt(15+1),s.GetAt(15+2),s.GetAt(15+3),s.GetAt(15+4),s.GetAt(15+5),s.GetAt(15+6),s.GetAt(15+7),s.GetAt(15+8),s.GetAt(15+9),s.GetAt(15+10),s.GetAt(15+11),s.GetAt(15+12),s.GetAt(15+13));  
				m_ListCtrlRecordTime.SetItemText(0,2,s1);

			}

			RecFile.Close(); 
		}
	}
	return;

	
	
	if(m_ListCtrlRecordTime.GetItemCount() > 0)
	{
		m_ListCtrlRecordTime.DeleteAllItems();
	}
	
	//ym_str.Format(_T("%04d-%02d"),m_SelectTestTime.GetYear(),m_SelectTestTime.GetMonth());    
	
	long FromMinutes,EndMinutes;
	
		CTime begin(m_SelectTestTime.GetYear(),m_SelectTestTime.GetMonth(),m_SelectTestTime.GetDay(),0,0,0);
		CTime end  (m_SelectTestTime.GetYear(),m_SelectTestTime.GetMonth(),m_SelectTestTime.GetDay(),23,59,59);
		
		CTimeSpan span = begin - STD_TIME;
		FromMinutes	= span.GetTotalMinutes();
		
		span = end - STD_TIME;
		EndMinutes	= span.GetTotalMinutes();

	fName.Format(_T("log\\RecFile%04d_%d.bin"),m_SelectTestTime.GetYear(),m_BoxNr);    
	
	if(RecFile.Open(theAppDirectory+fName,CFile::modeRead))
	{
		RecFile.SeekToBegin();
		while(1)
		{
			if(RecFile.Read(&record,sizeof(record)) == sizeof(record)) 
			{
				if(record.size == sizeof(record))
				{
					int item_count;
					item_count = m_ListCtrlRecordTime.GetItemCount(); 
					CString tempS;
					int days,hours,minutes;
					long total_minutes;
					total_minutes = record.BeginStart_From201001010000Minutes;

					if(total_minutes < FromMinutes || total_minutes > EndMinutes)
					{
						if( AllSelect == false)
						{
							continue;
						}
					}
					minutes = total_minutes % 60;
					total_minutes /= 60;

					hours = total_minutes % 24;
					total_minutes /= 24;

					days = total_minutes;

					CTimeSpan begin_span(days,hours,minutes,0);
					CTime BeginTime = STD_TIME;

					BeginTime	+= begin_span;
					tempS.Format(_T("%04d-%02d-%02d %02d:%02d"),BeginTime.GetYear(),BeginTime.GetMonth(),BeginTime.GetDay(),BeginTime.GetHour(), BeginTime.GetMinute());    
					m_ListCtrlRecordTime.InsertItem(item_count,tempS);
					

					tempS = record.ParaName;
					tempS.Trim();
					m_ListCtrlRecordTime.SetItemText(item_count,3,tempS);

					total_minutes = record.EndStart_From201001010000Minutes;
					minutes = total_minutes % 60;
					total_minutes /= 60;

					hours = total_minutes % 24;
					total_minutes /= 24;

					days = total_minutes;

					CTimeSpan end_span(days,hours,minutes,0);
					CTime EndTime(2010,1,1,0,0,0);
					EndTime	+= end_span;
					tempS.Format(_T("%04d-%02d-%02d %02d:%02d"),EndTime.GetYear(),EndTime.GetMonth(),EndTime.GetDay(),EndTime.GetHour(), EndTime.GetMinute());    
					
					m_ListCtrlRecordTime.SetItemText(item_count,2,tempS);

					tempS = record.Memo;
					tempS.Trim();
					m_ListCtrlRecordTime.SetItemText(item_count,1,tempS);
				}
				else
				{
					break;
				}
			}
			else
			{
				break;
			}
			/*
			CString txt,s;
			RecFile.ReadString(txt);

			if(txt.IsEmpty())
			{
				break;
			}
			else
			{
				if(txt.Find(ym_str,0)==0)
				{
					int i = 0;
					i = txt.Find( ',',0);
					int item_count;
					item_count = m_ListCtrlRecordTime.GetItemCount(); 
					CString tempS;
					tempS = txt.Left(i);
					m_ListCtrlRecordTime.InsertItem(item_count,tempS);
					txt = txt.Right(txt.GetLength() - i -1);  
					i = txt.Find( ',',0);
					tempS = txt.Left(i);
					m_ListCtrlRecordTime.SetItemText(item_count,1,tempS);
				}
			}*/
		}
		RecFile.Close();
	}	
}

void CRecordViewDlg::OnSelchangeMonthcalendar1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(); 
	this->UpdateListCtrlRecordTime();
	
	*pResult = 0;
}

extern TCHAR level_str[18][10];

void CRecordViewDlg::UpdateRecordDetail(CString begin, CString end)
{
		CFile RecFile;
		CString fName,ym_str;
		CFileFind ff;
		RECORD	record;
		CString header;
		CString s;
		CTimeSpan BeginSpan,EndSpan;
		int years,months,days,hours,minutes,seconds;
		
		years = (begin.GetAt(0) - '0')*1000
				+(begin.GetAt(1) - '0')*100
				+(begin.GetAt(2) - '0')*10
				+(begin.GetAt(3) - '0');

		months = (begin.GetAt(5) - '0')*10
				+(begin.GetAt(6) - '0');

		days	= (begin.GetAt(8) - '0')*10
				+(begin.GetAt(9) - '0');

		hours	= (begin.GetAt(11) - '0')*10
				+(begin.GetAt(12) - '0');

		minutes	= (begin.GetAt(14) - '0')*10
				+(begin.GetAt(15) - '0');

		seconds= (begin.GetAt(17) - '0')*10
				+(begin.GetAt(18) - '0');

		s.Format(_T("log\\box%d\\%04d%02d%02d%02d%02d%02d-"),m_BoxNr+1,years,months,days,hours,minutes,seconds); 
		fName = s;
		m_BeginTestTime = CTime(years,months,days,hours,minutes,0);
		
		BeginSpan = m_BeginTestTime - STD_TIME;

		years = (end.GetAt(0) - '0')*1000
				+(end.GetAt(1) - '0')*100
				+(end.GetAt(2) - '0')*10
				+(end.GetAt(3) - '0');

		months = (end.GetAt(5) - '0')*10
				+(end.GetAt(6) - '0');

		days	= (end.GetAt(8) - '0')*10
				+(end.GetAt(9) - '0');

		hours	= (end.GetAt(11) - '0')*10
				+(end.GetAt(12) - '0');

		minutes	= (end.GetAt(14) - '0')*10
				+(end.GetAt(15) - '0');

		seconds= (end.GetAt(17) - '0')*10
				+(end.GetAt(18) - '0');
		s.Format(_T("%04d%02d%02d%02d%02d%02d.record"),years,months,days,hours,minutes,seconds); 
		fName += s;

		m_EndTestTime = CTime(years,months,days,hours,minutes,0);
		EndSpan = m_EndTestTime - STD_TIME;

		header = begin;
		header +=",";
		header +=end;
		
		m_ListCtrlRecord.DeleteAllItems(); 
		fName = theAppDirectory + fName;
		if(RecFile.Open(fName,CFile::modeRead))
		{
			RecFile.SeekToBegin();
			RecFile.Read(&record,sizeof(record));
			RecFile.Close();
		}
		else
		{
			AfxMessageBox(_T("没有找到文件"));
			return;
		}

		int i = 0,j;

						CString strTemp;
						
						m_SwitchConfigPara = record.ConfigPara;
						
						if(record.SwitchType == 0)
						{
							m_TestProductType  = _T("常闭");
						}
						else
						{
							m_TestProductType  = _T("常开");
						}

						m_TestParaName.Format(_T("%s"),record.ParaName); 
						
						m_TestMemo.Format(_T("%s"),record.Memo);  
					 
						for(j = 0; j < G_iMaxLedNr;j++)
						{	
							//m_SwitchTestResult[j] = record.TestResult[j];
							m_SwitchTestResult_Ex[j].ID = j;
							m_SwitchTestResult_Ex[j].result = record.TestResult[j];
						}
						
						SortRecord();

						for(j = 0; j < G_iMaxLedNr;j++)
						{
							int item_count = m_ListCtrlRecord.GetItemCount(); 
							CString s;
							s.Format(_T("% 3d"),m_SwitchTestResult_Ex[j].ID+1); 
							m_ListCtrlRecord.InsertItem(item_count,s);
							if(m_SwitchTestResult_Ex[j].result.IsUsed)
							{
								s = "Y";
							
								m_ListCtrlRecord.SetItemText(item_count,1,s);	

								if(m_SwitchTestResult_Ex[j].result.IsOpenned)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,2,s);	

								if(m_SwitchTestResult_Ex[j].result.IsClosed )
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,3,s);	 	
								
								if(m_SwitchTestResult_Ex[j].result.IsOpenFlash)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,4,s);

								if(m_SwitchTestResult_Ex[j].result.IsCloseFlash)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,5,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.OpenTemp);
								m_ListCtrlRecord.SetItemText(item_count,6,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.CloseTemp);
								m_ListCtrlRecord.SetItemText(item_count,7,s);
								
								TEST_RESULT_LEVEL level =CheckTestLevel(record.ConfigPara,
												m_SwitchTestResult_Ex[j].result,HEAT_BOX_TYPE_NORMAL);
								m_ListCtrlRecord.SetItemText(item_count,8,level_str[level]);


								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.OpenFlashTemp);
								m_ListCtrlRecord.SetItemText(item_count,9,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.CloseFlashTemp);
								m_ListCtrlRecord.SetItemText(item_count,10,s);

								CTime t = GetRecordTime(m_SwitchTestResult_Ex[j].result.OpenTime);
								s.Format(_T("%02d:%02d:%02d"),t.GetHour(),t.GetMinute(),t.GetSecond());    
								m_ListCtrlRecord.SetItemText(item_count,11,s);
								
								t = GetRecordTime(m_SwitchTestResult_Ex[j].result.CloseTime);
								s.Format(_T("%02d:%02d:%02d"),t.GetHour(),t.GetMinute(),t.GetSecond());    
								m_ListCtrlRecord.SetItemText(item_count,12,s);
							}
							else
							{
								continue;
								s = "无产品";
								m_ListCtrlRecord.SetItemText(item_count,1,s);
								m_ListCtrlRecord.SetItemText(item_count,2,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,3,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,4,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,5,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,6,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,7,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,8,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,9,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,10,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,11,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,12,_T("-"));
							}
						}

						float open_max_temp, open_min_temp,open_avr_temp,close_max_temp,close_min_temp,close_avr_temp;
						int total_product,total_main_level;

						CString str;
						m_Para = "";
						str.Format(_T("主规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),record.ConfigPara.MainLevel,record.ConfigPara.MainLevelSpanMinus,record.ConfigPara.MainLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("主复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),record.ConfigPara.CloseLevel,record.ConfigPara.CloseLevelSpanMinus,record.ConfigPara.CloseLevelSpanPlus);      
						m_Para += str;

						str.Format(_T("高规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),record.ConfigPara.HighLevel,record.ConfigPara.HighLevelSpanMinus,record.ConfigPara.HighLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("高复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),record.ConfigPara.CloseHighLevel,record.ConfigPara.CloseHighLevelSpanMinus,record.ConfigPara.CloseHighLevelSpanPlus);      
						m_Para += str;

						str.Format(_T("低规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),record.ConfigPara.LowLevel,record.ConfigPara.LowLevelSpanMinus,record.ConfigPara.LowLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("低复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),record.ConfigPara.CloseLowLevel,record.ConfigPara.CloseLowLevelSpanMinus,record.ConfigPara.CloseLowLevelSpanPlus);      
						m_Para += str;
						this->TongJi(&open_max_temp, &open_min_temp, &open_avr_temp, &close_max_temp, &close_min_temp, &close_avr_temp, &total_product, &total_main_level);
						
						str.Format(_T("产品数:%d 合格品:%d 合格率:%5.1f%%\r\n动作最高温:%5.1f 动作最低温:%5.1f  动作平均温:%5.1f \r\n复位最高温:%5.1f 复位最低温:%5.1f  复位平均温:%5.1f"),total_product,total_main_level,(float)(total_main_level*100.0f/total_product),
							open_max_temp,open_min_temp,(open_avr_temp),close_max_temp,close_min_temp,(close_avr_temp));
						m_Para += str;

						this->UpdateData(false);
						
						return;
		




		//==============================================
/*
		ym_str.Format(_T("%04d%02d"),m_SelectTestTime.GetYear(),m_SelectTestTime.GetMonth());    
		fName.Format(_T("log\\RecFile%04d_%d.bin"),m_SelectTestTime.GetYear(),m_BoxNr);    
		if(RecFile.Open(theAppDirectory+fName,CFile::modeRead))
		{
			RecFile.SeekToBegin();

			while(1)
			{
				
				if(RecFile.Read(&record,sizeof(record)) != sizeof(record))
				{
					break;
				}
				else
				{
					if(record.size != sizeof(record))
					{
						break;
					}

					if(BeginSpan.GetTotalMinutes() != record.BeginStart_From201001010000Minutes)
					{
						continue;
					}

					if(EndSpan.GetTotalMinutes() != record.EndStart_From201001010000Minutes)
					{
						continue;
					}

						int i = 0,j;
						CString strTemp;
						
						m_SwitchConfigPara = record.ConfigPara;
						
						if(record.SwitchType == 0)
						{
							m_TestProductType  = _T("常闭");
						}
						else
						{
							m_TestProductType  = _T("常开");
						}

						m_TestParaName.Format(_T("%s"),record.ParaName); 
						
						m_TestMemo.Format(_T("%s"),record.Memo);  
					 
						for(j = 0; j < G_iMaxLedNr;j++)
						{	
							//m_SwitchTestResult[j] = record.TestResult[j];
							m_SwitchTestResult_Ex[j].ID = j;
							m_SwitchTestResult_Ex[j].result = record.TestResult[j];
						}
						
						SortRecord();

						for(j = 0; j < G_iMaxLedNr;j++)
						{
							int item_count = m_ListCtrlRecord.GetItemCount(); 
							CString s;
							s.Format(_T("% 3d"),m_SwitchTestResult_Ex[j].ID+1); 
							m_ListCtrlRecord.InsertItem(item_count,s);
							if(m_SwitchTestResult_Ex[j].result.IsUsed)
							{
								s = "Y";
							
								m_ListCtrlRecord.SetItemText(item_count,1,s);	

								if(m_SwitchTestResult_Ex[j].result.IsOpenned)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,2,s);	

								if(m_SwitchTestResult_Ex[j].result.IsClosed )
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,3,s);	 	
								
								if(m_SwitchTestResult_Ex[j].result.IsOpenFlash)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,4,s);

								if(m_SwitchTestResult_Ex[j].result.IsCloseFlash)
								{
									s = "Y";
								}
								else
								{
									s = "N";
								}
								m_ListCtrlRecord.SetItemText(item_count,5,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.OpenTemp);
								m_ListCtrlRecord.SetItemText(item_count,6,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.CloseTemp);
								m_ListCtrlRecord.SetItemText(item_count,7,s);
								
								TEST_RESULT_LEVEL level =CheckTestLevel(record.ConfigPara,
												m_SwitchTestResult_Ex[j].result,HEAT_BOX_TYPE_NORMAL);
								m_ListCtrlRecord.SetItemText(item_count,8,level_str[level]);


								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.OpenFlashTemp);
								m_ListCtrlRecord.SetItemText(item_count,9,s);

								s.Format(_T("%5.1f"),m_SwitchTestResult_Ex[j].result.CloseFlashTemp);
								m_ListCtrlRecord.SetItemText(item_count,10,s);

								CTime t = GetRecordTime(m_SwitchTestResult_Ex[j].result.OpenTime);
								s.Format(_T("%02d:%02d:%02d"),t.GetHour(),t.GetMinute(),t.GetSecond());    
								m_ListCtrlRecord.SetItemText(item_count,11,s);
								
								t = GetRecordTime(m_SwitchTestResult_Ex[j].result.CloseTime);
								s.Format(_T("%02d:%02d:%02d"),t.GetHour(),t.GetMinute(),t.GetSecond());    
								m_ListCtrlRecord.SetItemText(item_count,12,s);
							}
							else
							{
								s = "无产品";
								m_ListCtrlRecord.SetItemText(item_count,1,s);
								m_ListCtrlRecord.SetItemText(item_count,2,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,3,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,4,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,5,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,6,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,7,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,8,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,9,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,10,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,11,_T("-"));
								m_ListCtrlRecord.SetItemText(item_count,12,_T("-"));
							}
						}

						float open_max_temp, open_min_temp,open_avr_temp,close_max_temp,close_min_temp,close_avr_temp;
						int total_product,total_main_level;

						CString str;
						m_Para = "";
						str.Format(_T("主规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),record.ConfigPara.MainLevel,record.ConfigPara.MainLevelSpanMinus,record.ConfigPara.MainLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("主复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),record.ConfigPara.CloseLevel,record.ConfigPara.CloseLevelSpanMinus,record.ConfigPara.CloseLevelSpanPlus);      
						m_Para += str;

						str.Format(_T("高规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),record.ConfigPara.HighLevel,record.ConfigPara.HighLevelSpanMinus,record.ConfigPara.HighLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("高复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),record.ConfigPara.CloseHighLevel,record.ConfigPara.CloseHighLevelSpanMinus,record.ConfigPara.CloseHighLevelSpanPlus);      
						m_Para += str;

						str.Format(_T("低规格:%5.1f 负偏差:%5.1f 正偏差:%5.1f "),record.ConfigPara.LowLevel,record.ConfigPara.LowLevelSpanMinus,record.ConfigPara.LowLevelSpanPlus);      
						m_Para += str;
						str.Format(_T("低复位:%5.1f 负偏差:%5.1f 正偏差:%5.1f\r\n"),record.ConfigPara.CloseLowLevel,record.ConfigPara.CloseLowLevelSpanMinus,record.ConfigPara.CloseLowLevelSpanPlus);      
						m_Para += str;
						this->TongJi(&open_max_temp, &open_min_temp, &open_avr_temp, &close_max_temp, &close_min_temp, &close_avr_temp, &total_product, &total_main_level);
						
						str.Format(_T("产品数:%d 合格品:%d 合格率:%5.1f%%\r\n动作最高温:%5.1f 动作最低温:%5.1f  动作平均温:%5.1f \r\n复位最高温:%5.1f 复位最低温:%5.1f  复位平均温:%5.1f"),total_product,total_main_level,(float)(total_main_level*100.0f/total_product),
							open_max_temp,open_min_temp,(open_avr_temp),close_max_temp,close_min_temp,(close_avr_temp));
						m_Para += str;

						this->UpdateData(false);
						
						break;
					}
				
			}
			RecFile.Close();
		}
		*/
}

void CRecordViewDlg::FindRecordDetail(CString begin, CString end)
{
		CFile RecFile;
		CString fName,ym_str;
		CFileFind ff;
		RECORD	record;
		CString header;
		CTimeSpan BeginSpan;
		int years,months,days,hours,minutes;
		
		years = (begin.GetAt(0) - '0')*1000
				+(begin.GetAt(1) - '0')*100
				+(begin.GetAt(2) - '0')*10
				+(begin.GetAt(3) - '0');

		months = (begin.GetAt(5) - '0')*10
				+(begin.GetAt(6) - '0');

		days	= (begin.GetAt(8) - '0')*10
				+(begin.GetAt(9) - '0');

		hours	= (begin.GetAt(11) - '0')*10
				+(begin.GetAt(12) - '0');

		minutes	= (begin.GetAt(14) - '0')*10
				+(begin.GetAt(15) - '0');
		
		m_BeginTestTime = CTime(years,months,days,hours,minutes,0);
		
		BeginSpan = m_BeginTestTime - CTime(2010,1,1,0,0,0);

		years = (end.GetAt(0) - '0')*1000
				+(end.GetAt(1) - '0')*100
				+(end.GetAt(2) - '0')*10
				+(end.GetAt(3) - '0');

		months = (end.GetAt(5) - '0')*10
				+(end.GetAt(6) - '0');

		days	= (end.GetAt(8) - '0')*10
				+(end.GetAt(9) - '0');

		hours	= (end.GetAt(11) - '0')*10
				+(end.GetAt(12) - '0');

		minutes	= (end.GetAt(14) - '0')*10
				+(end.GetAt(15) - '0');

		m_EndTestTime = CTime(years,months,days,hours,minutes,0);

		header = begin;
		header +=",";
		header +=end;
		
		m_ListCtrlRecord.DeleteAllItems(); 
		
		ym_str.Format(_T("%04d%02d"),m_SelectTestTime.GetYear(),m_SelectTestTime.GetMonth());    
		fName.Format(_T("log\\RecFile%04d_%d.bin"),m_SelectTestTime.GetYear(),m_BoxNr);    
		if(RecFile.Open(theAppDirectory+fName,CFile::modeRead))
		{
			RecFile.SeekToBegin();

			while(1)
			{
				
				if(RecFile.Read(&record,sizeof(record)) != sizeof(record))
				{
					break;
				}
				else
				{
					if(record.size != sizeof(record))
					{
						break;
					}

					if(BeginSpan.GetTotalMinutes() != record.BeginStart_From201001010000Minutes)
					{
						continue;
					}
						int i = 0,j;
						CString strTemp;
						
						
						for(j = 0; j < G_iMaxLedNr;j++)
						{	
							m_SwitchTestResult_Ex[j].result  = record.TestResult[j];
							m_SwitchTestResult_Ex[j].ID      = j;
							
						}

						break;
					}
				
			}
			RecFile.Close();
		}	
}



void CRecordViewDlg::OnButtonPrn() 
{
	// TODO: Add your control notification handler code here
	CPrintDialog prnDlg(true);//false);
	CString printer;
	CDC dc;
	int i;
	
	if(this->m_ListCtrlRecord.GetItemCount() < 1)
	{
		AfxMessageBox(_T("请选择具体的生产记录"));
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
	i=	dc.StartPage();

	CString BoxNr;


	BoxNr.Format(_T("%d"), m_BoxNr + 3-2);

	//AfxMessageBox("打印1");
	
	PrintReport(&dc,app_name, m_BoxNr,
				m_BeginTestTime,m_EndTestTime,
				m_TestParaName,m_TestProductType, m_TestMemo,
				m_SwitchConfigPara,m_SwitchTestResult_Ex,
				TRUE,0,249,(HEAT_BOX_TYPE)m_HeatBoxType);

	


	i =	dc.EndPage(); 
	
	if(G_iMaxLedNr > 250)
	{
		i = dc.StartPage();
	//	AfxMessageBox("打印2");
		PrintReport(&dc,app_name, m_BoxNr,
					m_BeginTestTime,m_EndTestTime,
					m_TestParaName,m_TestProductType,m_TestMemo, 
					m_SwitchConfigPara,m_SwitchTestResult_Ex,
					TRUE,250,G_iMaxLedNr - 1,(HEAT_BOX_TYPE)m_HeatBoxType);

		i =	dc.EndPage(); 
	}

	i =	dc.EndDoc(); 
	
	dc.DeleteDC();

}

void CRecordViewDlg::TongJi(float *open_max_temp, float *open_min_temp, float *open_avr_temp, float *close_max_temp, float *close_min_temp, float *close_avr_temp, int *total_product, int *total_main_level)
{
		CString str;
		(*open_max_temp) = 0.0f;
		(*open_min_temp) = 999.9f;
		(*open_avr_temp) = 0.0f;
		(*close_max_temp)= 0.0f;
		(*close_min_temp)= 999.9f;
		(*close_avr_temp)= 0.0f;
		(*total_product) = G_iMaxLedNr;
		(*total_main_level) = 0;
		int  open_count=0,close_count=0;  
		int j;
		float temp;

		for(j = 0; j < G_iMaxLedNr;j++)
		{
			str = m_ListCtrlRecord.GetItemText(j, 8);
			if(str.Find(_T("主规格")) >= 0)
			{
				(*total_main_level) ++;
			}
			else if(str.Find(_T("高规格")) >= 0)
			{
				(*total_main_level) ++;
			}
			else if(str.Find(_T("低规格")) >= 0)
			{
				(*total_main_level) ++;
			}
			
			//if(str.Find(_T("无产品")) >= 0)
			if(str.Find(_T("-")) >= 0)
			{
				(*total_product) --;
			}
			str = m_ListCtrlRecord.GetItemText(j, 6);
			//temp = atof(str);
			temp = _tcstod(str,NULL);
			if(temp > 0.0f && temp < 200.0f)
			{
				if((*open_max_temp) < temp)
				{
					(*open_max_temp) = temp;
				}
				if((*open_min_temp) > temp)
				{
					(*open_min_temp) = temp;
				}
				(*open_avr_temp) += temp;
				open_count ++;
			}
			str = m_ListCtrlRecord.GetItemText(j, 7);
			//temp = atof(str);
			temp = _tcstod(str,NULL);
			if(temp > 0.0f && temp < 200.0f)
			{
				if((*close_max_temp) < temp)
				{
					(*close_max_temp) = temp;
				}
				if((*close_min_temp) > temp)
				{
					(*close_min_temp) = temp;
				}
				(*close_avr_temp) += temp;
				close_count ++;
			}
			
		}
		if(open_count > 0)
		{
			(*open_avr_temp) = (*open_avr_temp) / open_count;
		}
		if(close_count > 0)
		{
			(*close_avr_temp) = (*close_avr_temp) / close_count;
		}
}


void CRecordViewDlg::OnButtonTempBar() 
{
	// TODO: Add your control notification handler code here

	if(this->m_ListCtrlRecord.GetItemCount() < 1)
	{
		AfxMessageBox(_T("请选择具体的生产记录"));
		return;
	}

	int j;
	CString str;
	CProductTempBar dlg;

	for(j = 0; j < G_iMaxLedNr;j++)
	{
		memcpy(&dlg.m_SwitchTestResult[j], &m_SwitchTestResult_Ex[j].result,sizeof(dlg.m_SwitchTestResult[j])) ;
	}
	dlg.m_BeginTestTime = m_BeginTestTime;
	dlg.m_SwitchConfigPara = m_SwitchConfigPara;
	dlg.DoModal(); 
	
}

void CRecordViewDlg::OnButtonPrnPreview() 
{
	// TODO: Add your control notification handler code here
	CPrnPreview dlg;
	if(this->m_ListCtrlRecord.GetItemCount() < 1)
	{
		AfxMessageBox(_T("请选择具体的生产记录"));
		return;
	}

	dlg.m_TestParaName		= m_TestParaName;
	dlg.m_EndTestTime		= m_EndTestTime;
	dlg.m_BeginTestTime		= m_BeginTestTime;
	dlg.m_BoxNr = m_BoxNr;
	dlg.m_SwitchConfigPara	= m_SwitchConfigPara;
	dlg.m_TestMemo			= m_TestMemo;


	memcpy(dlg.m_SwitchTestResult_Ex,m_SwitchTestResult_Ex,sizeof(m_SwitchTestResult_Ex));
	
	CString ProductType ;

	dlg.m_TestProductType= m_TestProductType;
	
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

void CRecordViewDlg::OnButtonTongji() 
{
	// TODO: Add your control notification handler code here
	//CChanliangTongjiDlg dlg;
	//dlg.DoModal(); 
}

void CRecordViewDlg::OnButtonSaveas() 
{
	// TODO: Add your control notification handler code here
	int j;
	TCHAR str[1000];
	TCHAR str1[1000];
	static TCHAR szFilter[] = _T("Excel Files (*.csv)|*.csv|");
	CString  file_name;
	CTime cur = cur.GetCurrentTime();


	if(this->m_ListCtrlRecord.GetItemCount() < 1)
	{
		AfxMessageBox(_T("请选择具体的生产记录"));
		return;
	}

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
			RecFile.Write(&UNICODE_FILE_HEAR,2);
			RecFile.Write(&CSV_COMMMA,2);
			RecFile.Write(&CSV_COMMMA,2);
			s = _T("热保护器测试报告\r\n");
			RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );


			s = _T("公司名称");
			RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
			s = AfxGetApp()->GetProfileStringW(_T("SETTING"),_T("USER"),_T("常州市润邦电子科技有限公司")) ;
			s += _T("\r\n");
			RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );
	

	//产品规格
	s = _T("产品规格");
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
	s = m_TestParaName + _T("\r\n"); 
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );
	
	//产品类型
	s = _T("产品类型");
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
	s = m_TestProductType + _T("\r\n");
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );
	
	
	s = _T("测试说明");
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
	s = m_TestMemo + _T("\r\n");
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );
	

	s = _T("烘箱编号");
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
	s.Format(_T("%d\r\n"), m_BoxNr + 1); 
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );
	s = _T("开始测试时间");
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
	s.Format(_T("%04d-%02d-%02d %02d:%02d\r\n"),m_BeginTestTime.GetYear(),m_BeginTestTime.GetMonth(),m_BeginTestTime.GetDay(),m_BeginTestTime.GetHour(),m_BeginTestTime.GetMinute());
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );

	s = _T("结束测试时间");
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );RecFile.Write(&CSV_COMMMA,2);
	s.Format(_T("%04d-%02d-%02d %02d:%02d\r\n"),m_EndTestTime.GetYear(),m_EndTestTime.GetMonth(),m_EndTestTime.GetDay(),m_EndTestTime.GetHour(),m_EndTestTime.GetMinute()); 
	RecFile.Write(s,s.GetLength() * sizeof(TCHAR) );

			_stprintf_s(str,_T("序号%c动作温度%c复位温度%c规格%c动作时间%c复位时间\r\n"),CSV_COMMMA,CSV_COMMMA,CSV_COMMMA,CSV_COMMMA,CSV_COMMMA);
			RecFile.Write(str,wcslen(str) * sizeof(TCHAR));
				
			for(j = 0; j < G_iMaxLedNr;j++)
			{
				if(m_SwitchTestResult_Ex[j].result.IsUsed)
				{
					if(G_iMaxLedNr == 500)
					{
						if((j % 12) < 9)
						{
							_stprintf_s(str,_T("%d%d%c"),m_SwitchTestResult_Ex[j].ID/12,(j%12)+1,CSV_COMMMA);				
						}
						else if((j % 12) == 9)
						{
							_stprintf_s(str,_T("%d0%c"),m_SwitchTestResult_Ex[j].ID/12 + 1,CSV_COMMMA);				
						}
						else if((j % 12) == 10)
						{
							_stprintf_s(str,_T("%dA%c"),m_SwitchTestResult_Ex[j].ID/12 + 1,CSV_COMMMA);				
						}
						else if((j % 12) == 11)
						{
							_stprintf_s(str,_T("%dB%c"),m_SwitchTestResult_Ex[j].ID/12 +1,CSV_COMMMA);				
						}
					}
					else
					{
						_stprintf_s(str,_T("%d%c"),m_SwitchTestResult_Ex[j].ID + 1,CSV_COMMMA);				
					}
					CTime t = GetRecordTime(m_SwitchTestResult_Ex[j].result.OpenTime); 
					CTime t1 = GetRecordTime(m_SwitchTestResult_Ex[j].result.CloseTime); 
					_stprintf_s(str1,_T("%5.1f%c%5.1f%c%s%c%04d/%02d/%02d %02d:%02d:%02d%c%04d/%02d/%02d %02d:%02d:%02d\r\n"),
						m_SwitchTestResult_Ex[j].result.OpenTemp,CSV_COMMMA,
						m_SwitchTestResult_Ex[j].result.CloseTemp,CSV_COMMMA,
						level_str_4_csv[CheckTestLevel(m_SwitchConfigPara,m_SwitchTestResult_Ex[j].result,HEAT_BOX_TYPE_NORMAL)],CSV_COMMMA,
						t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),CSV_COMMMA,
						t1.GetYear(),t1.GetMonth(),t1.GetDay(),t1.GetHour(),t1.GetMinute(),t1.GetSecond());   
					wcscat_s(str,str1);
					RecFile.Write(str,wcslen(str) * sizeof(TCHAR));
				}
				else
				{
					
				}
			}

			RecFile.Close();
		}
	}
	
	

}

void CRecordViewDlg::OnLvnItemchangedListRecordTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

void CRecordViewDlg::OnBnClickedButtonSaveas2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFile	RecFile;
	CString file_name;
	char	str[1000],str1[1000];
	int		j;
	for(int i = 0; i < m_ListCtrlRecordTime.GetItemCount(); i++)
	{
		CString s1 = m_ListCtrlRecordTime.GetItemText(i,0);  
		CString s2 = m_ListCtrlRecordTime.GetItemText(i,1);  
		
		//this->UpdateRecordDetail(s1,s2); 
		FindRecordDetail(s1,s2);
		CString DirName;

		DirName.Format( _T("d:\\\\测试记录%d"),m_BoxNr);

		if (!::PathIsDirectory(DirName))
		{
			::CreateDirectory(DirName, NULL);
		}
		file_name = _T("测试记录");//201607132154.csv");

		file_name += s1;
		file_name +=_T(".csv");
		file_name.Replace(_T("-"),_T(""));
		file_name.Replace(_T(" "),_T(""));
		file_name.Replace(_T(":"),_T(""));
		DirName += "\\";
		file_name = DirName + file_name;
		
		if(RecFile.Open(file_name,CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate ))
		{
			sprintf_s(str,"序号,动作温度,复位温度,规格,动作时间,复位时间\r\n");
			RecFile.Write(str,strlen(str));
				
			for(j = 0; j < G_iMaxLedNr;j++)
			{
				if(m_SwitchTestResult_Ex[j].result.IsUsed)
				{
					
					sprintf_s(str,"%d,",m_SwitchTestResult_Ex[j].ID + 1);				
					
					CTime t = GetRecordTime(m_SwitchTestResult_Ex[j].result .OpenTime); 
					CTime t1 = GetRecordTime(m_SwitchTestResult_Ex[j].result.CloseTime); 
					sprintf_s(str1,"%5.1f,%5.1f,%s,%04d/%02d/%02d %02d:%02d:%02d,%04d/%02d/%02d %02d:%02d:%02d\r\n",m_SwitchTestResult_Ex[j].result.OpenTemp,m_SwitchTestResult_Ex[j].result.CloseTemp,level_str_4_csv[CheckTestLevel(m_SwitchConfigPara,m_SwitchTestResult_Ex[j].result,HEAT_BOX_TYPE_NORMAL)],
						t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute(),t.GetSecond(),
						t1.GetYear(),t1.GetMonth(),t1.GetDay(),t1.GetHour(),t1.GetMinute(),t1.GetSecond());   
					strcat_s(str,str1);
					RecFile.Write(str,strlen(str));
				}
				else
				{
					sprintf_s(str,"%d,",j + 1);
					strcat_s(str,"-,-,无产品,-,-,\r\n");
					RecFile.Write(str,strlen(str));
				}
			}

			RecFile.Close();
		}
	}
}

void CRecordViewDlg::OnBnClickedButtonTakedlg()
{
	// TODO: 在此添加控件通知处理程序代码
 
	if(this->m_ListCtrlRecord.GetItemCount() < 1)
	{
		AfxMessageBox(_T("请选择具体的生产记录"));
		return;
	}

	CTakeoutDlg dlg;
	dlg.m_BoxNr = 5;
	dlg.m_WindowText	= _T("历史记录查看"); 

	for(int j = 0; j < G_iMaxLedNr;j++)
	{
		memcpy(&G_TakeDlgSwitchTestResult[1][j], &m_SwitchTestResult_Ex[j].result, sizeof(G_TakeDlgSwitchTestResult[1][j]));
		memcpy(&G_TakeDlgSwitchTestResult_Bak[1][j], &m_SwitchTestResult_Ex[j].result, sizeof(G_TakeDlgSwitchTestResult_Bak[1][j]));
	}

	dlg.m_iTakeBoardType = 1;

	dlg.m_BeginTestTime		= m_BeginTestTime;
	dlg.m_SwitchConfigPara	= m_SwitchConfigPara;
	dlg.DoModal(); 

}

void CRecordViewDlg::SortRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	SWITCH_TEST_RESULT_EX temp_result;

	switch(m_ComboBoxSortType.GetCurSel())
	{
	case 1:
		for(int i = 0; i < G_iMaxLedNr; i++)
		{
			for(int j = i+1; j < G_iMaxLedNr; j++)
			{
				if(m_SwitchTestResult_Ex[i].result.OpenTemp <  m_SwitchTestResult_Ex[j].result.OpenTemp)
				{
					temp_result = m_SwitchTestResult_Ex[i];
					m_SwitchTestResult_Ex[i] = m_SwitchTestResult_Ex[j];
					m_SwitchTestResult_Ex[j] = temp_result;
				}
			}
		}
		break;
	case 2:
		for(int i = 0; i < G_iMaxLedNr; i++)
		{
			for(int j = i+1; j < G_iMaxLedNr; j++)
			{
				if(m_SwitchTestResult_Ex[i].result.OpenTemp >  m_SwitchTestResult_Ex[j].result.OpenTemp)
				{
					temp_result = m_SwitchTestResult_Ex[i];
					m_SwitchTestResult_Ex[i] = m_SwitchTestResult_Ex[j];
					m_SwitchTestResult_Ex[j] = temp_result;
				}
			}
		}
		break;
	default:
		for(int i = 0; i < G_iMaxLedNr; i++)
		{
			for(int j = i+1; j < G_iMaxLedNr; j++)
			{
				if(m_SwitchTestResult_Ex[i].ID >  m_SwitchTestResult_Ex[j].ID)
				{
					temp_result = m_SwitchTestResult_Ex[i];
					m_SwitchTestResult_Ex[i] = m_SwitchTestResult_Ex[j];
					m_SwitchTestResult_Ex[j] = temp_result;
				}
			}
		}
		break;
	}
}
void CRecordViewDlg::OnCbnSelchangeComboSortType()
{
	// TODO: 在此添加控件通知处理程序代码

	int i =(int)(this->m_ListCtrlRecordTime.GetFirstSelectedItemPosition());
	if(i > 0)
	{
		CString s1 = m_ListCtrlRecordTime.GetItemText(i-1,0);  
		CString s2 = m_ListCtrlRecordTime.GetItemText(i-1,2);  
		
		this->UpdateRecordDetail(s1,s2); 
		
	}
}


void CRecordViewDlg::OnCbnSelchangeComboPrintType()
{
	// TODO: 在此添加控件通知处理程序代码
	switch(m_ComboPrintType.GetCurSel())
	{
	case 1:
		AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("PRINT TYPE"),1); 
		break;
	case 2:
		AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("PRINT TYPE"),2);
		break;
	default:
		AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("PRINT TYPE"),0);
		break;
	}
}
