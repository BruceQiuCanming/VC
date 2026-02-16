// HotLong_PCBDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HotLong_PCB.h"
#include "HotLong_PCBDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Dshow.h"

#include "strmif.h"
#pragma comment(lib, "Strmiids.lib")
#pragma comment(lib, "Quartz.lib")
#include <SetupAPI.h>
#include <InitGuid.h>
#include <tchar.h>

#pragma comment(lib, "Setupapi.lib")
#include <SetupAPI.h>
#include <InitGuid.h>
#include <tchar.h>

#pragma comment(lib, "Setupapi.lib")

#include "CaptureVideo.h"

#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "afxwin.h"
#include "modbus.h"
#include "InputDlg.h"
#include "BarcodeDlg.h"

CONFIG_DATA	m_ConfigData;




CString strINI1; //读取INI文件内容CString变量1
CString strINI2; //读取INI文件内容CString变量2
CString strINI3; //读取INI文件内容CString变量3
CString strINI4; //读取INI文件内容CString变量4
CString strExcel1; //读取Excel文件内容CString变量1
CString strExcel2; //读取Excel文件内容CString变量2
CString strINIFilePath;   //用于保存数据源INI文件路径
CString strExcleFilePath;  //用于保存数据源Excel文件路径
CString strOutputExcleFilePath;  //用于保存输出Excel文件路径
CString strWorkDir;     //用于保存exe所在路径



CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	
END_MESSAGE_MAP()


// CHotLong_PCBDlg 对话框




CHotLong_PCBDlg::CHotLong_PCBDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHotLong_PCBDlg::IDD, pParent)
	, m_Led_Cool(_T(""))
	, m_Led_Color(_T(""))
	, m_Led_Heat(_T(""))
	, m_sLED_NUM(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	ReadConfig();

	m_BarCodeID = -1;
	
	m_Beep_BeginTime	=	GetTickCount();
	m_Beep_mSecs		=	0;
	m_Beep_IsPass		=	false;

	for(int i = 0; i < 9; i++)
	{
		m_Pv[i] = 0;
		m_Sv[i] = 0;
		m_Pp[i] = 0;
	}
	//Excel_Test();
}



void CHotLong_PCBDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_SCREEN, m_staticScreen);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);

	DDX_Text(pDX, IDC_EDIT_LED_NUM_LEFT_1, m_ConfigData.m_LED_NUM_Left[0]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_RIGHT_1, m_ConfigData.m_LED_NUM_Right[0]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_TOP_1, m_ConfigData.m_LED_NUM_Top[0]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_BOTTOM_1, m_ConfigData.m_LED_NUM_Bottom[0]);

	DDX_Text(pDX, IDC_EDIT_LED_NUM_LEFT_2, m_ConfigData.m_LED_NUM_Left[1]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_RIGHT_2, m_ConfigData.m_LED_NUM_Right[1]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_TOP_2, m_ConfigData.m_LED_NUM_Top[1]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_BOTTOM_2, m_ConfigData.m_LED_NUM_Bottom[1]);

	DDX_Text(pDX, IDC_EDIT_LED_NUM_LEFT_3, m_ConfigData.m_LED_NUM_Left[2]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_RIGHT_3, m_ConfigData.m_LED_NUM_Right[2]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_TOP_3, m_ConfigData.m_LED_NUM_Top[2]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_BOTTOM_3, m_ConfigData.m_LED_NUM_Bottom[2]);

	DDX_Text(pDX, IDC_EDIT_LED_NUM_LEFT_4, m_ConfigData.m_LED_NUM_Left[3]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_RIGHT_4, m_ConfigData.m_LED_NUM_Right[3]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_TOP_4, m_ConfigData.m_LED_NUM_Top[3]);
	DDX_Text(pDX, IDC_EDIT_LED_NUM_BOTTOM_4, m_ConfigData.m_LED_NUM_Bottom[3]);


	DDX_Text(pDX, IDC_EDIT_LED_HEAT_LEFT, m_ConfigData.m_LED_HEAT_Left);
	DDX_Text(pDX, IDC_EDIT_LED_HEAT_RIGHT, m_ConfigData.m_LED_HEAT_Right);
	DDX_Text(pDX, IDC_EDIT_LED_HEAT_TOP, m_ConfigData.m_LED_HEAT_Top);
	DDX_Text(pDX, IDC_EDIT_LED_HEAT_BOTTOM, m_ConfigData.m_LED_HEAT_Bottom);

	DDX_Text(pDX, IDC_EDIT_LED_COOL_LEFT, m_ConfigData.m_LED_COOL_Left);
	DDX_Text(pDX, IDC_EDIT_LED_COOL_RIGHT, m_ConfigData.m_LED_COOL_Right);
	DDX_Text(pDX, IDC_EDIT_LED_COOL_TOP, m_ConfigData.m_LED_COOL_Top);
	DDX_Text(pDX, IDC_EDIT_LED_COOL_BOTTOM, m_ConfigData.m_LED_COOL_Bottom);

	DDX_Text(pDX, IDC_EDIT_LED_COLOR_LEFT, m_ConfigData.m_LED_COLOR_Left);
	DDX_Text(pDX, IDC_EDIT_LED_COLOR_RIGHT, m_ConfigData.m_LED_COLOR_Right);
	DDX_Text(pDX, IDC_EDIT_LED_COLOR_TOP, m_ConfigData.m_LED_COLOR_Top);
	DDX_Text(pDX, IDC_EDIT_LED_COLOR_BOTTOM, m_ConfigData.m_LED_COLOR_Bottom);

	DDX_Text(pDX, IDC_EDIT_LED_COOL, m_Led_Cool);
	DDX_Text(pDX, IDC_EDIT_LED_COLOR, m_Led_Color);
	DDX_Text(pDX, IDC_EDIT_LED_HEAT, m_Led_Heat);
	DDX_Text(pDX, IDC_EDIT_DEGREE, m_sLED_NUM);
	DDX_Text(pDX, IDC_EDIT_BARCODE,	  m_Device_HotLong_PCB[0].m_BarCode);
	DDX_Text(pDX, IDC_EDIT_BARCODE_1, m_Device_HotLong_PCB[1].m_BarCode);
	DDX_Text(pDX, IDC_EDIT_BARCODE_2, m_Device_HotLong_PCB[2].m_BarCode);
	DDX_Text(pDX, IDC_EDIT_BARCODE_3, m_Device_HotLong_PCB[3].m_BarCode);
	DDX_Text(pDX, IDC_EDIT_BARCODE_4, m_Device_HotLong_PCB[4].m_BarCode);
	DDX_Text(pDX, IDC_EDIT_BARCODE_5, m_Device_HotLong_PCB[5].m_BarCode);
	DDX_Text(pDX, IDC_EDIT_BARCODE_6, m_Device_HotLong_PCB[6].m_BarCode);
	DDX_Text(pDX, IDC_EDIT_BARCODE_7, m_Device_HotLong_PCB[7].m_BarCode);
	DDX_Text(pDX, IDC_EDIT_BARCODE_8, m_Device_HotLong_PCB[8].m_BarCode);

	DDX_Text(pDX, IDC_EDIT_PROGRAM_PASS,m_Program_Pass_Counts);
	DDX_Text(pDX, IDC_EDIT_PROGRAM_FAIL,m_Program_Fail_Counts);
	DDX_Text(pDX, IDC_EDIT_PROGRAM_CNT, m_Program_Total_Counts);
	DDX_Text(pDX, IDC_EDIT_PROGRAM_PERCENT, m_Program_Percent);

	DDX_Text(pDX, IDC_EDIT_AGING_PASS,	m_Aging_Pass_Counts);
	DDX_Text(pDX, IDC_EDIT_AGING_FAIL,	m_Aging_Fail_Counts);
	DDX_Text(pDX, IDC_EDIT_AGING_CNT,	m_Aging_Total_Counts);
	DDX_Text(pDX, IDC_EDIT_AGING_PERCENT, m_Aging_Percent);


	DDX_Control(pDX, IDC_CHECK_PCB_BUTTON_SET, m_CheckPCB_Button_Set);

	DDX_Control(pDX, IDC_CHECK_PCB_BUTTON_PLUS, m_CheckPCB_Button_Plus);
	DDX_Control(pDX, IDC_CHECK_PCB_BUTTON_MINUS, m_CheckPCB_Button_Minus);
	DDX_Control(pDX, IDC_CHECK_PCB_BUTTON_TEST_NEEDLE, m_CheckPCB_Button_Test_Needle);
	DDX_Control(pDX, IDC_CHECK_PCB_BUTTON_TEST_START, m_CheckPCB_Button_Test_Start);
	DDX_Control(pDX, IDC_CHECK_PCB_POWER_1, m_Device_HotLong_PCB[1].m_CheckPCB_Power);
	DDX_Control(pDX, IDC_CHECK_PCB_POWER_2, m_Device_HotLong_PCB[2].m_CheckPCB_Power);
	DDX_Control(pDX, IDC_CHECK_PCB_POWER_3, m_Device_HotLong_PCB[3].m_CheckPCB_Power);
	DDX_Control(pDX, IDC_CHECK_PCB_POWER_4, m_Device_HotLong_PCB[4].m_CheckPCB_Power);
	DDX_Control(pDX, IDC_CHECK_PCB_POWER_5, m_Device_HotLong_PCB[5].m_CheckPCB_Power);
	DDX_Control(pDX, IDC_CHECK_PCB_POWER_6, m_Device_HotLong_PCB[6].m_CheckPCB_Power);
	DDX_Control(pDX, IDC_CHECK_PCB_POWER_7, m_Device_HotLong_PCB[7].m_CheckPCB_Power);
	DDX_Control(pDX, IDC_CHECK_PCB_POWER_8, m_Device_HotLong_PCB[8].m_CheckPCB_Power);

	DDX_Control(pDX, IDC_EDIT_RESULT, m_Device_HotLong_PCB[0].m_Edit_Result);
	DDX_Control(pDX, IDC_EDIT_RESULT_1, m_Device_HotLong_PCB[1].m_Edit_Result);
	DDX_Control(pDX, IDC_EDIT_RESULT_2, m_Device_HotLong_PCB[2].m_Edit_Result);
	DDX_Control(pDX, IDC_EDIT_RESULT_3, m_Device_HotLong_PCB[3].m_Edit_Result);
	DDX_Control(pDX, IDC_EDIT_RESULT_4, m_Device_HotLong_PCB[4].m_Edit_Result);
	DDX_Control(pDX, IDC_EDIT_RESULT_5, m_Device_HotLong_PCB[5].m_Edit_Result);
	DDX_Control(pDX, IDC_EDIT_RESULT_6, m_Device_HotLong_PCB[6].m_Edit_Result);
	DDX_Control(pDX, IDC_EDIT_RESULT_7, m_Device_HotLong_PCB[7].m_Edit_Result);
	DDX_Control(pDX, IDC_EDIT_RESULT_8, m_Device_HotLong_PCB[8].m_Edit_Result);

	DDX_Control(pDX, IDC_LIST_PROGRAM, m_ListMsg[0]);
	DDX_Control(pDX, IDC_LIST_1, m_ListMsg[1]);
	DDX_Control(pDX, IDC_LIST_2, m_ListMsg[2]);
	DDX_Control(pDX, IDC_LIST_3, m_ListMsg[3]);
	DDX_Control(pDX, IDC_LIST_4, m_ListMsg[4]);
	DDX_Control(pDX, IDC_LIST_5, m_ListMsg[5]);
	DDX_Control(pDX, IDC_LIST_6, m_ListMsg[6]);
	DDX_Control(pDX, IDC_LIST_7, m_ListMsg[7]);
	DDX_Control(pDX, IDC_LIST_8, m_ListMsg[8]);

	DDX_Text(pDX, IDC_EDIT_Pv_1, m_Pv[1]);
	DDX_Text(pDX, IDC_EDIT_Pv_2, m_Pv[2]);
	DDX_Text(pDX, IDC_EDIT_Pv_3, m_Pv[3]);
	DDX_Text(pDX, IDC_EDIT_Pv_4, m_Pv[4]);
	DDX_Text(pDX, IDC_EDIT_Pv_5, m_Pv[5]);
	DDX_Text(pDX, IDC_EDIT_Pv_6, m_Pv[6]);
	DDX_Text(pDX, IDC_EDIT_Pv_7, m_Pv[7]);
	DDX_Text(pDX, IDC_EDIT_Pv_8, m_Pv[8]);

	DDX_Text(pDX, IDC_EDIT_PP_1, m_Pp[1]);
	DDX_Text(pDX, IDC_EDIT_PP_2, m_Pp[2]);
	DDX_Text(pDX, IDC_EDIT_PP_3, m_Pp[3]);
	DDX_Text(pDX, IDC_EDIT_PP_4, m_Pp[4]);
	DDX_Text(pDX, IDC_EDIT_PP_5, m_Pp[5]);
	DDX_Text(pDX, IDC_EDIT_PP_6, m_Pp[6]);
	DDX_Text(pDX, IDC_EDIT_PP_7, m_Pp[7]);
	DDX_Text(pDX, IDC_EDIT_PP_8, m_Pp[8]);


	DDX_Text(pDX, IDC_EDIT_Sv_1, m_Sv[1]);
	DDX_Text(pDX, IDC_EDIT_Sv_2, m_Sv[2]);
	DDX_Text(pDX, IDC_EDIT_Sv_3, m_Sv[3]);
	DDX_Text(pDX, IDC_EDIT_Sv_4, m_Sv[4]);
	DDX_Text(pDX, IDC_EDIT_Sv_5, m_Sv[5]);
	DDX_Text(pDX, IDC_EDIT_Sv_6, m_Sv[6]);
	DDX_Text(pDX, IDC_EDIT_Sv_7, m_Sv[7]);
	DDX_Text(pDX, IDC_EDIT_Sv_8, m_Sv[8]);

	DDX_Text(pDX, IDC_EDIT_MSG,   m_Device_HotLong_PCB[0].m_EditMsg);
	DDX_Text(pDX, IDC_EDIT_MSG_1, m_Device_HotLong_PCB[1].m_EditMsg);
	DDX_Text(pDX, IDC_EDIT_MSG_2, m_Device_HotLong_PCB[2].m_EditMsg);
	DDX_Text(pDX, IDC_EDIT_MSG_3, m_Device_HotLong_PCB[3].m_EditMsg);
	DDX_Text(pDX, IDC_EDIT_MSG_4, m_Device_HotLong_PCB[4].m_EditMsg);
	DDX_Text(pDX, IDC_EDIT_MSG_5, m_Device_HotLong_PCB[5].m_EditMsg);
	DDX_Text(pDX, IDC_EDIT_MSG_6, m_Device_HotLong_PCB[6].m_EditMsg);
	DDX_Text(pDX, IDC_EDIT_MSG_7, m_Device_HotLong_PCB[7].m_EditMsg);
	DDX_Text(pDX, IDC_EDIT_MSG_8, m_Device_HotLong_PCB[8].m_EditMsg);


	DDX_Control(pDX, IDC_CHECK_FOUCS, m_CheckFocus);
	DDX_Control(pDX, IDC_CHECK_BEEP, m_CheckPCB_Button_Beep);
	DDX_Control(pDX, IDC_STATIC_LED_NUM, m_LedNumGroup);
	DDX_Control(pDX, IDC_STATIC_DEGREE, m_STATIC_DEGREE);

	DDX_Control(pDX, IDC_STATIC_LEFT, m_STATIC_LEFT);
	DDX_Control(pDX, IDC_STATIC_RIGHT, m_STATIC_RIGHT);
	DDX_Control(pDX, IDC_STATIC_TOP, m_STATIC_TOP);
	DDX_Control(pDX, IDC_STATIC_BOTTOM, m_STATIC_BOTTOM);

	DDX_Control(pDX, IDC_EDIT_LED_NUM_LEFT_1, m_LED_NUM_Left[0]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_RIGHT_1, m_LED_NUM_Right[0]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_TOP_1, m_LED_NUM_Top[0]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_BOTTOM_1, m_LED_NUM_Bottom[0]);

	DDX_Control(pDX, IDC_EDIT_LED_NUM_LEFT_2, m_LED_NUM_Left[1]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_RIGHT_2, m_LED_NUM_Right[1]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_TOP_2, m_LED_NUM_Top[1]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_BOTTOM_2, m_LED_NUM_Bottom[1]);

	DDX_Control(pDX, IDC_EDIT_LED_NUM_LEFT_3, m_LED_NUM_Left[2]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_RIGHT_3, m_LED_NUM_Right[2]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_TOP_3, m_LED_NUM_Top[2]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_BOTTOM_3, m_LED_NUM_Bottom[2]);

	DDX_Control(pDX, IDC_EDIT_LED_NUM_LEFT_4, m_LED_NUM_Left[3]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_RIGHT_4, m_LED_NUM_Right[3]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_TOP_4, m_LED_NUM_Top[3]);
	DDX_Control(pDX, IDC_EDIT_LED_NUM_BOTTOM_4, m_LED_NUM_Bottom[3]);
	DDX_Control(pDX, IDC_PIC_LED_NUM_1, m_PIC_LED_NUM_1);
	DDX_Control(pDX, IDC_PIC_LED_NUM_2, m_PIC_LED_NUM_2);
	DDX_Control(pDX, IDC_PIC_LED_NUM_3, m_PIC_LED_NUM_3);
	DDX_Control(pDX, IDC_PIC_LED_NUM_4, m_PIC_LED_NUM_4);

	DDX_Control(pDX, IDC_STATIC_HEAT_LED_GROUP, m_STATIC_HEAT_LED_GROUP);
	DDX_Control(pDX, IDC_STATIC_COOL_LED_GROUP, m_STATIC_COOL_LED_GROUP);
	DDX_Control(pDX, IDC_STATIC_COLOR_LED_GROUP, m_STATIC_COLOR_LED_GROUP);
}

BEGIN_MESSAGE_MAP(CHotLong_PCBDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CHotLong_PCBDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CHotLong_PCBDlg::OnBnClickedButton1)

	ON_MESSAGE(Comm_Device_HuiKong_DIO_MSG_ID, OnComm_Device_HuiKong_DIO)
	ON_MESSAGE(Comm_PROGRAM_MSG_ID, OnComm_PROGRAM)

	ON_MESSAGE(Comm_AGING_MSG_ID_1, OnComm_PCB_1)
	ON_MESSAGE(Comm_AGING_MSG_ID_2, OnComm_PCB_2)
	ON_MESSAGE(Comm_AGING_MSG_ID_3, OnComm_PCB_3)
	ON_MESSAGE(Comm_AGING_MSG_ID_4, OnComm_PCB_4)
	ON_MESSAGE(Comm_AGING_MSG_ID_5, OnComm_PCB_5)
	ON_MESSAGE(Comm_AGING_MSG_ID_6, OnComm_PCB_6)
	ON_MESSAGE(Comm_AGING_MSG_ID_7, OnComm_PCB_7)
	ON_MESSAGE(Comm_AGING_MSG_ID_8, OnComm_PCB_8)


	ON_WM_TIMER()
//	ON_BN_CLICKED(IDC_BUTTON_LED_RECT, &CHotLong_PCBDlg::OnBnClickedButtonLedRect)
	ON_BN_CLICKED(IDC_CHECK_PCB_BUTTON_SET, &CHotLong_PCBDlg::OnBnClickedButtonPCB_Set)
	ON_BN_CLICKED(IDC_CHECK_PCB_BUTTON_TEST_NEEDLE, &CHotLong_PCBDlg::OnBnClickedButtonPCB_Needle)
	ON_BN_CLICKED(IDC_CHECK_PCB_BUTTON_PLUS,        &CHotLong_PCBDlg::OnBnClickedButtonPCB_Plus)
	ON_BN_CLICKED(IDC_CHECK_PCB_BUTTON_MINUS,       &CHotLong_PCBDlg::OnBnClickedButtonPCB_Minus)
	ON_BN_CLICKED(IDC_CHECK_PCB_BUTTON_TEST_START, &CHotLong_PCBDlg::OnBnClickedButtonPCB_Write)

	ON_BN_CLICKED(IDC_BUTTON_PROGRAM_START,	  &CHotLong_PCBDlg::OnBnClickedButtonProgramStart)
	ON_BN_CLICKED(IDC_BUTTON_START_1, &CHotLong_PCBDlg::OnBnClickedButtonStart_1)
	ON_BN_CLICKED(IDC_BUTTON_START_2, &CHotLong_PCBDlg::OnBnClickedButtonStart_2)
	ON_BN_CLICKED(IDC_BUTTON_START_3, &CHotLong_PCBDlg::OnBnClickedButtonStart_3)
	ON_BN_CLICKED(IDC_BUTTON_START_4, &CHotLong_PCBDlg::OnBnClickedButtonStart_4)
	ON_BN_CLICKED(IDC_BUTTON_START_5, &CHotLong_PCBDlg::OnBnClickedButtonStart_5)
	ON_BN_CLICKED(IDC_BUTTON_START_6, &CHotLong_PCBDlg::OnBnClickedButtonStart_6)
	ON_BN_CLICKED(IDC_BUTTON_START_7, &CHotLong_PCBDlg::OnBnClickedButtonStart_7)
	ON_BN_CLICKED(IDC_BUTTON_START_8, &CHotLong_PCBDlg::OnBnClickedButtonStart_8)

	

	ON_BN_CLICKED(IDC_CHECK_PCB_POWER_1, &CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_1)
	ON_BN_CLICKED(IDC_CHECK_PCB_POWER_2, &CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_2)
	ON_BN_CLICKED(IDC_CHECK_PCB_POWER_3, &CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_3)
	ON_BN_CLICKED(IDC_CHECK_PCB_POWER_4, &CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_4)
	ON_BN_CLICKED(IDC_CHECK_PCB_POWER_5, &CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_5)
	ON_BN_CLICKED(IDC_CHECK_PCB_POWER_6, &CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_6)
	ON_BN_CLICKED(IDC_CHECK_PCB_POWER_7, &CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_7)
	ON_BN_CLICKED(IDC_CHECK_PCB_POWER_8, &CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_8)


	ON_BN_CLICKED(IDCANCEL, &CHotLong_PCBDlg::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_LEFT_1, &CHotLong_PCBDlg::OnEnChangeEditLedNumLeft_1)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_RIGHT_1, &CHotLong_PCBDlg::OnEnChangeEditLedNumRight_1)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_TOP_1, &CHotLong_PCBDlg::OnEnChangeEditLedNumTop_1)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_BOTTOM_1, &CHotLong_PCBDlg::OnEnChangeEditLedNumBottom_1)

	ON_EN_CHANGE(IDC_EDIT_LED_NUM_LEFT_2, &CHotLong_PCBDlg::OnEnChangeEditLedNumLeft_2)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_RIGHT_2, &CHotLong_PCBDlg::OnEnChangeEditLedNumRight_2)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_TOP_2, &CHotLong_PCBDlg::OnEnChangeEditLedNumTop_2)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_BOTTOM_2, &CHotLong_PCBDlg::OnEnChangeEditLedNumBottom_2)

	ON_EN_CHANGE(IDC_EDIT_LED_NUM_LEFT_3, &CHotLong_PCBDlg::OnEnChangeEditLedNumLeft_3)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_RIGHT_3, &CHotLong_PCBDlg::OnEnChangeEditLedNumRight_3)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_TOP_3, &CHotLong_PCBDlg::OnEnChangeEditLedNumTop_3)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_BOTTOM_3, &CHotLong_PCBDlg::OnEnChangeEditLedNumBottom_3)

	ON_EN_CHANGE(IDC_EDIT_LED_NUM_LEFT_4, &CHotLong_PCBDlg::OnEnChangeEditLedNumLeft_4)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_RIGHT_4, &CHotLong_PCBDlg::OnEnChangeEditLedNumRight_4)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_TOP_4, &CHotLong_PCBDlg::OnEnChangeEditLedNumTop_4)
	ON_EN_CHANGE(IDC_EDIT_LED_NUM_BOTTOM_4, &CHotLong_PCBDlg::OnEnChangeEditLedNumBottom_4)

	ON_EN_CHANGE(IDC_EDIT_LED_HEAT_LEFT, &CHotLong_PCBDlg::OnEnChangeEditLedHeatLeft)
	ON_EN_CHANGE(IDC_EDIT_LED_HEAT_RIGHT, &CHotLong_PCBDlg::OnEnChangeEditLedHeatRight)
	ON_EN_CHANGE(IDC_EDIT_LED_HEAT_TOP, &CHotLong_PCBDlg::OnEnChangeEditLedHeatTop)
	ON_EN_CHANGE(IDC_EDIT_LED_HEAT_BOTTOM, &CHotLong_PCBDlg::OnEnChangeEditLedHeatBottom)

	ON_EN_CHANGE(IDC_EDIT_LED_COOL_LEFT, &CHotLong_PCBDlg::OnEnChangeEditLedCoolLeft)
	ON_EN_CHANGE(IDC_EDIT_LED_COOL_RIGHT, &CHotLong_PCBDlg::OnEnChangeEditLedCoolRight)
	ON_EN_CHANGE(IDC_EDIT_LED_COOL_TOP, &CHotLong_PCBDlg::OnEnChangeEditLedCoolTop)
	ON_EN_CHANGE(IDC_EDIT_LED_COOL_BOTTOM, &CHotLong_PCBDlg::OnEnChangeEditLedCoolBottom)

	ON_EN_CHANGE(IDC_EDIT_LED_COLOR_LEFT, &CHotLong_PCBDlg::OnEnChangeEditLedColorLeft)
	ON_EN_CHANGE(IDC_EDIT_LED_COLOR_RIGHT, &CHotLong_PCBDlg::OnEnChangeEditLedColorRight)
	ON_EN_CHANGE(IDC_EDIT_LED_COLOR_TOP, &CHotLong_PCBDlg::OnEnChangeEditLedColorTop)
	ON_EN_CHANGE(IDC_EDIT_LED_COLOR_BOTTOM, &CHotLong_PCBDlg::OnEnChangeEditLedColorBottom)

	ON_EN_SETFOCUS(IDC_EDIT_BARCODE, &CHotLong_PCBDlg::OnEnSetFocusEditBarcode)
	ON_EN_SETFOCUS(IDC_EDIT_BARCODE_1, &CHotLong_PCBDlg::OnEnSetFocusEditBarcode_1)
	ON_EN_SETFOCUS(IDC_EDIT_BARCODE_2, &CHotLong_PCBDlg::OnEnSetFocusEditBarcode_2)
	ON_EN_SETFOCUS(IDC_EDIT_BARCODE_3, &CHotLong_PCBDlg::OnEnSetFocusEditBarcode_3)
	ON_EN_SETFOCUS(IDC_EDIT_BARCODE_4, &CHotLong_PCBDlg::OnEnSetFocusEditBarcode_4)
	ON_EN_SETFOCUS(IDC_EDIT_BARCODE_5, &CHotLong_PCBDlg::OnEnSetFocusEditBarcode_5)
	ON_EN_SETFOCUS(IDC_EDIT_BARCODE_6, &CHotLong_PCBDlg::OnEnSetFocusEditBarcode_6)
	ON_EN_SETFOCUS(IDC_EDIT_BARCODE_7, &CHotLong_PCBDlg::OnEnSetFocusEditBarcode_7)
	ON_EN_SETFOCUS(IDC_EDIT_BARCODE_8, &CHotLong_PCBDlg::OnEnSetFocusEditBarcode_8)

	ON_EN_SETFOCUS(IDC_EDIT_AGING_PASS, &CHotLong_PCBDlg::OnEnSetFocusEditAging_PASS)
	ON_EN_SETFOCUS(IDC_EDIT_AGING_FAIL, &CHotLong_PCBDlg::OnEnSetFocusEditAging_FAIL)
	ON_EN_SETFOCUS(IDC_EDIT_PROGRAM_PASS, &CHotLong_PCBDlg::OnEnSetFocusEditProgram_PASS)
	ON_EN_SETFOCUS(IDC_EDIT_PROGRAM_FAIL, &CHotLong_PCBDlg::OnEnSetFocusEditProgram_FAIL)


	ON_EN_CHANGE(IDC_EDIT_Sv_1, &CHotLong_PCBDlg::OnEnChangeEditSv_1)
	ON_STN_CLICKED(IDC_STATIC_LEFT5, &CHotLong_PCBDlg::OnStnClickedStaticLeft5)
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_LEFT_1, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumLeft1)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_RIGHT_1, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumRight1)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_TOP_1, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumTop1)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_BOTTOM_1, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumBottom1)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_LEFT_2, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumLeft2)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_RIGHT_2, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumRight2)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_TOP_2, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumTop2)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_BOTTOM_2, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumBottom2)

	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_LEFT_3, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumLeft3)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_RIGHT_3, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumRight3)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_TOP_3, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumTop3)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_BOTTOM_3, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumBottom3)

	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_LEFT_4, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumLeft4)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_RIGHT_4, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumRight4)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_TOP_4, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumTop4)
	ON_EN_SETFOCUS(IDC_EDIT_LED_NUM_BOTTOM_4, &CHotLong_PCBDlg::OnEnSetfocusEditLedNumBottom4)

	ON_EN_SETFOCUS(IDC_EDIT_LED_HEAT_LEFT, &CHotLong_PCBDlg::OnEnSetfocusEditLedHeatLeft)
	ON_EN_SETFOCUS(IDC_EDIT_LED_HEAT_RIGHT, &CHotLong_PCBDlg::OnEnSetfocusEditLedHeatRight)
	ON_EN_SETFOCUS(IDC_EDIT_LED_HEAT_TOP, &CHotLong_PCBDlg::OnEnSetfocusEditLedHeatTop)
	ON_EN_SETFOCUS(IDC_EDIT_LED_HEAT_BOTTOM, &CHotLong_PCBDlg::OnEnSetfocusEditLedHeatBottom)

	ON_EN_SETFOCUS(IDC_EDIT_LED_COOL_LEFT, &CHotLong_PCBDlg::OnEnSetfocusEditLedCoolLeft)
	ON_EN_SETFOCUS(IDC_EDIT_LED_COOL_RIGHT, &CHotLong_PCBDlg::OnEnSetfocusEditLedCoolRight)
	ON_EN_SETFOCUS(IDC_EDIT_LED_COOL_TOP, &CHotLong_PCBDlg::OnEnSetfocusEditLedCoolTop)
	ON_EN_SETFOCUS(IDC_EDIT_LED_COOL_BOTTOM, &CHotLong_PCBDlg::OnEnSetfocusEditLedCoolBottom)

	ON_EN_SETFOCUS(IDC_EDIT_LED_COLOR_LEFT, &CHotLong_PCBDlg::OnEnSetfocusEditLedColorLeft)
	ON_EN_SETFOCUS(IDC_EDIT_LED_COLOR_RIGHT, &CHotLong_PCBDlg::OnEnSetfocusEditLedColorRight)
	ON_EN_SETFOCUS(IDC_EDIT_LED_COLOR_TOP, &CHotLong_PCBDlg::OnEnSetfocusEditLedColorTop)
	ON_EN_SETFOCUS(IDC_EDIT_LED_COLOR_BOTTOM, &CHotLong_PCBDlg::OnEnSetfocusEditLedColorBottom)
	END_MESSAGE_MAP()


// CHotLong_PCBDlg 消息处理程序
void EnumSerialPortFriendlyNames(CStringArray& portList);

CBrush 	G_bkBrush;
COLORREF BK_COLOR(RGB(0,160,128)); //创建一把黄色的背景刷子
BOOL CHotLong_PCBDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	G_bkBrush.CreateSolidBrush(BK_COLOR); //创建一把黄色的背景刷子


	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
		
		CString strParaMenu;
		strParaMenu.LoadString(IDS_PARA_DLG);
		if (!strParaMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_PARABOX, strParaMenu);
		}
		
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	m_Edit_Degree.SubclassDlgItem(IDC_EDIT_DEGREE, this);
	m_Edit_Led_Heat.SubclassDlgItem(IDC_EDIT_LED_HEAT, this);
	m_Edit_Led_Cool.SubclassDlgItem(IDC_EDIT_LED_COOL, this);
	m_Edit_Led_Color.SubclassDlgItem(IDC_EDIT_LED_COLOR, this);

	m_Edit_Pv[0].SubclassDlgItem(IDC_EDIT_Pv_1, this);
	m_Edit_Pv[1].SubclassDlgItem(IDC_EDIT_Pv_2, this);
	m_Edit_Pv[2].SubclassDlgItem(IDC_EDIT_Pv_3, this);
	m_Edit_Pv[3].SubclassDlgItem(IDC_EDIT_Pv_4, this);
	m_Edit_Pv[4].SubclassDlgItem(IDC_EDIT_Pv_5, this);
	m_Edit_Pv[5].SubclassDlgItem(IDC_EDIT_Pv_6, this);
	m_Edit_Pv[6].SubclassDlgItem(IDC_EDIT_Pv_7, this);
	m_Edit_Pv[7].SubclassDlgItem(IDC_EDIT_Pv_8, this);

	m_Edit_Sv[0].SubclassDlgItem(IDC_EDIT_Sv_1, this);
	m_Edit_Sv[1].SubclassDlgItem(IDC_EDIT_Sv_2, this);
	m_Edit_Sv[2].SubclassDlgItem(IDC_EDIT_Sv_3, this);
	m_Edit_Sv[3].SubclassDlgItem(IDC_EDIT_Sv_4, this);
	m_Edit_Sv[4].SubclassDlgItem(IDC_EDIT_Sv_5, this);
	m_Edit_Sv[5].SubclassDlgItem(IDC_EDIT_Sv_6, this);
	m_Edit_Sv[6].SubclassDlgItem(IDC_EDIT_Sv_7, this);
	m_Edit_Sv[7].SubclassDlgItem(IDC_EDIT_Sv_8, this);

	m_Edit_Pv[0].SetTextColor(RGB(0xFF, 0, 0));
	m_Edit_Pv[0].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Pv[1].SetTextColor(RGB(0xFF, 0, 0));
	m_Edit_Pv[1].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Pv[2].SetTextColor(RGB(0xFF, 0, 0));
	m_Edit_Pv[2].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Pv[3].SetTextColor(RGB(0xFF, 0, 0));
	m_Edit_Pv[3].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Pv[4].SetTextColor(RGB(0xFF, 0, 0));
	m_Edit_Pv[4].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Pv[5].SetTextColor(RGB(0xFF, 0, 0));
	m_Edit_Pv[5].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Pv[6].SetTextColor(RGB(0xFF, 0, 0));
	m_Edit_Pv[6].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Pv[7].SetTextColor(RGB(0xFF, 0, 0));
	m_Edit_Pv[7].SetBackColor(RGB(0xFF, 0xFF, 0xFF));

	m_Edit_Sv[0].SetTextColor(RGB(0,0xFF, 0));
	m_Edit_Sv[0].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Sv[1].SetTextColor(RGB(0, 0xFF, 0));
	m_Edit_Sv[1].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Sv[2].SetTextColor(RGB(0, 0xFF, 0));
	m_Edit_Sv[2].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Sv[3].SetTextColor(RGB(0, 0xFF, 0));
	m_Edit_Sv[3].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Sv[4].SetTextColor(RGB(0, 0xFF, 0));
	m_Edit_Sv[4].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Sv[5].SetTextColor(RGB(0, 0xFF, 0));
	m_Edit_Sv[5].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Sv[6].SetTextColor(RGB(0, 0xFF, 0));
	m_Edit_Sv[6].SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	m_Edit_Sv[7].SetTextColor(RGB(0, 0xFF, 0));
	m_Edit_Sv[7].SetBackColor(RGB(0xFF, 0xFF, 0xFF));





	m_Edit_Degree.SetTextColor(RGB(0xFF, 0, 0));
	m_Edit_Degree.SetBackColor(RGB(0xFF, 0xFF, 0xFF));
	// 创建字体，设置字体大小和类型
	m_font.CreatePointFont(270, _T("楷体"));

	// 设置控件字体
	m_Edit_Degree.SetFont(&m_font);
	m_Edit_Led_Heat.SetFont(&m_font);
	m_Edit_Led_Cool.SetFont(&m_font);
	m_Edit_Led_Color.SetFont(&m_font);

	m_Device_HotLong_PCB[0].m_Edit_Result.SetFont(&m_font);
	m_Device_HotLong_PCB[1].m_Edit_Result.SetFont(&m_font);
	m_Device_HotLong_PCB[2].m_Edit_Result.SetFont(&m_font);
	m_Device_HotLong_PCB[3].m_Edit_Result.SetFont(&m_font);
	m_Device_HotLong_PCB[4].m_Edit_Result.SetFont(&m_font);
	m_Device_HotLong_PCB[5].m_Edit_Result.SetFont(&m_font);
	m_Device_HotLong_PCB[6].m_Edit_Result.SetFont(&m_font);
	m_Device_HotLong_PCB[7].m_Edit_Result.SetFont(&m_font);
	m_Device_HotLong_PCB[8].m_Edit_Result.SetFont(&m_font);

	


	CRect rect;
	GetClientRect(&rect); // 获取客户区的大小
	int wnd_width = rect.Width();  // 窗口宽度
	int wnd_height = rect.Height(); // 窗口高度

	this->m_staticScreen.MoveWindow(0,0,CAMERA_WIDTH,CAMERA_HEIGHT,true);

	//this->m_staticScreen.MoveWindow(0,0,wnd_width * 0.3,wnd_height * 0.3,true);
	

	m_ListCtrl.ResetContent();
	m_cap.EnumDevices (&m_ListCtrl,m_ListCtrl);
	m_ListCtrl.SetCurSel (m_ConfigData.m_Camera_Nr);
	if(m_ListCtrl.GetCount() > 0 
		&& m_ConfigData.m_Camera_Nr >= 0 
		&& m_ConfigData.m_Camera_Nr < m_ListCtrl.GetCount())
	{
		m_cap.Init(m_ConfigData.m_Camera_Nr,this->m_staticScreen);
	}
	else
	{
		AfxMessageBox(_T("无此编号的摄像头"));
	}



	CString s = _T("江阴辉龙线路板测试系统 编译时间:");
	s += __DATE__;
	s += _T(" ");
	s += __TIME__;

	this->SetWindowTextW( s );

	
	this->ShowWindow(SW_MAXIMIZE);

	this->SetTimer(1, 50, NULL);


	CString CommStr;

	

	CStringArray CommName;
	CommName.RemoveAll();
	EnumSerialPortFriendlyNames(CommName);

	m_Device_HuiKong_DIO.m_CommPara.comm		=&m_Device_HuiKong_DIO;
	m_Device_HuiKong_DIO.m_CommPara.MessageID	= Comm_Device_HuiKong_DIO_MSG_ID;
	m_Device_HuiKong_DIO.m_CommPara.m_hWnd		= this->GetSafeHwnd();
	m_Device_HuiKong_DIO.m_bXModem				= false;

	if(m_ConfigData.m_Comm_Device_HuiKong_DIO >= 0 && m_ConfigData.m_Comm_Device_HuiKong_DIO < CommName.GetSize())
	{
		CommStr = CommName.GetAt(m_ConfigData.m_Comm_Device_HuiKong_DIO);
		CommStr.Trim();
		int left = CommStr.Find(_T("("));
		CommStr = CommStr.Right(CommStr.GetLength() - left - 1);
		CommStr = CommStr.Left(CommStr.GetLength() - 1);
		if(m_Device_HuiKong_DIO.OpenComm(CommStr, &m_Device_HuiKong_DIO.m_CommPara) == 0)
		{
			m_Device_HuiKong_DIO.m_CommPara.ReadThread = AfxBeginThread(ReadCommThreadProc_PLC, (LPVOID)(&m_Device_HuiKong_DIO.m_CommPara), THREAD_PRIORITY_NORMAL);
		}

	}

	for (int i = 0; i < 9; i++)
	{

		
		m_Device_HotLong_PCB[i].m_CommPara.comm			= m_Device_HotLong_PCB[i].m_Comm;
		m_Device_HotLong_PCB[i].m_CommPara.MessageID	= Comm_PROGRAM_MSG_ID + i;
		m_Device_HotLong_PCB[i].m_CommPara.m_hWnd		= this->GetSafeHwnd();
		m_Device_HotLong_PCB[i].m_bXModem				= false;
		
		if(CommName.GetSize() > 0 
			&& m_ConfigData.m_Comm_Nr[i] >= 0 
			&& m_ConfigData.m_Comm_Nr[i] < CommName.GetSize())
		{
			CommStr = CommName.GetAt(m_ConfigData.m_Comm_Nr[i]);
			if(CommStr.GetLength() > 0)
			{
				CommStr = CommName.GetAt(m_ConfigData.m_Comm_Nr[i]);
				CommStr.Trim();
				int left = CommStr.Find(_T("("));
				CommStr = CommStr.Right(CommStr.GetLength() - left - 1);
				CommStr = CommStr.Left(CommStr.GetLength() - 1);
				if(m_Device_HotLong_PCB[i].OpenComm(CommStr, &m_Device_HotLong_PCB[i].m_CommPara) == 0)
				{
					m_Device_HotLong_PCB[i].m_CommPara.ReadThread = AfxBeginThread(ReadCommThreadProc_PCB, (LPVOID)(&m_Device_HotLong_PCB[i].m_CommPara), THREAD_PRIORITY_NORMAL);
				}
			}
		}
		else
		{
			CString ss;
			ss.Format(_T("没有对应的 COMM %d口"), i+1);
			AfxMessageBox(ss);
		}
	
		
	}

	
	m_Program_Pass_Counts	= m_ConfigData.m_Program_Pass_Counts;
	m_Program_Fail_Counts	= m_ConfigData.m_Program_Fail_Counts;
	m_Aging_Pass_Counts		= m_ConfigData.m_Aging_Pass_Counts;
	m_Aging_Fail_Counts		= m_ConfigData.m_Aging_Fail_Counts;

	
	SavePassFail();

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE

}

void CHotLong_PCBDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFFF) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	if ((nID & 0xFFFF) == IDM_PARABOX)
	{
		m_ParaDlg.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CHotLong_PCBDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);

	
	}
	else
	{
		switch(m_ConfigData.m_Pcb_Type)
		{
		case PCB_TYPE_BIG_AMP:
		case PCB_TYPE_BIG_AMP_NO_BARCODE:

			ReMoveWindows_BIG_AMP();
			break;
		case PCB_TYPE_X90:
	    case PCB_TYPE_X90_NO_BARCODE:
			ReMoveWindows_X90();
			break;
		}
		CDialog::OnPaint();
	}
}

void CHotLong_PCBDlg::ReMoveWindows_BIG_AMP(void)
{
		if(this->IsWindowVisible())
		{
			CWnd *w;
			CRect rect;
			GetClientRect(&rect); // 获取客户区的大小
			int wnd_width = rect.Width();  // 窗口宽度
			int wnd_height = rect.Height(); // 窗口高度

			this->m_staticScreen.MoveWindow(0,0,CAMERA_WIDTH,CAMERA_HEIGHT,true);

			
			/*int ID = ::m_ConfigData.m_Camera_Nr;
			m_ListCtrl.ResetContent();
			m_cap.EnumDevices (&m_ListCtrl,m_ListCtrl);
			m_ListCtrl.SetCurSel (ID);
			m_cap.Init(ID,this->m_staticScreen);
			*/
			int left = CAMERA_WIDTH;
			int top  = 0;
			this->m_LedNumGroup.MoveWindow(left,0,(wnd_width - CAMERA_WIDTH) * 0.4 ,wnd_height * 0.3,true);
			this->m_LedNumGroup.GetClientRect(&rect);
			int width  = rect.Width() / 5;
			int height = rect.Height() / 8;

			this->m_STATIC_DEGREE.MoveWindow(left,height,width  ,height ,true);
			this->m_Edit_Degree.MoveWindow(left + width ,height,width  ,height ,true);

			this->m_STATIC_LEFT.MoveWindow(left,height * 2,width  ,height ,true);
			this->m_STATIC_RIGHT.MoveWindow(left,height * 3,width  ,height ,true);
			this->m_STATIC_TOP.MoveWindow(left,height * 4,width  ,height ,true);
			this->m_STATIC_BOTTOM.MoveWindow(left,height * 5,width  ,height ,true);
			for(int i = 0; i < 4; i++)
			{
				this->m_LED_NUM_Left[i].MoveWindow(left + width * (1 + i) ,height * 2,width  ,height ,true);
				this->m_LED_NUM_Right[i].MoveWindow(left + width * (1 + i) ,height * 3,width  ,height ,true);
				this->m_LED_NUM_Top[i].MoveWindow(left + width * (1 + i) ,height * 4,width  ,height ,true);
				this->m_LED_NUM_Bottom[i].MoveWindow(left + width * (1 + i) ,height * 5,width  ,height ,true);
			}
			this->m_CheckFocus.MoveWindow(left + width * 0 ,height * 6,width  ,height*2 ,true);
			 
			if(m_ConfigData.m_LED_NUM_Right[0] > m_ConfigData.m_LED_NUM_Left[0]
				&& m_ConfigData.m_LED_NUM_Bottom[0] > m_ConfigData.m_LED_NUM_Top[0])
				{
					m_PIC_LED_NUM_1.MoveWindow(left + width * 1 ,height * 6,
						m_ConfigData.m_LED_NUM_Right[0] - m_ConfigData.m_LED_NUM_Left[0]  ,
						m_ConfigData.m_LED_NUM_Bottom[0] - m_ConfigData.m_LED_NUM_Top[0]  ,true);
				}
			if(m_ConfigData.m_LED_NUM_Right[1] > m_ConfigData.m_LED_NUM_Left[1]
				&& m_ConfigData.m_LED_NUM_Bottom[1] > m_ConfigData.m_LED_NUM_Top[1])
				{
					m_PIC_LED_NUM_2.MoveWindow(left + width * 2 ,height * 6,
						m_ConfigData.m_LED_NUM_Right[1] - m_ConfigData.m_LED_NUM_Left[1]  ,
						m_ConfigData.m_LED_NUM_Bottom[1] - m_ConfigData.m_LED_NUM_Top[1]  ,true);
				}
			if(m_ConfigData.m_LED_NUM_Right[2] > m_ConfigData.m_LED_NUM_Left[2]
				&& m_ConfigData.m_LED_NUM_Bottom[2] > m_ConfigData.m_LED_NUM_Top[2])
				{
					m_PIC_LED_NUM_3.MoveWindow(left + width * 3 ,height * 6,
						m_ConfigData.m_LED_NUM_Right[2] - m_ConfigData.m_LED_NUM_Left[2]  ,
						m_ConfigData.m_LED_NUM_Bottom[2] - m_ConfigData.m_LED_NUM_Top[2]  ,true);
				}
			if(m_ConfigData.m_LED_NUM_Right[3] > m_ConfigData.m_LED_NUM_Left[3]
				&& m_ConfigData.m_LED_NUM_Bottom[3] > m_ConfigData.m_LED_NUM_Top[3])
				{
					m_PIC_LED_NUM_4.MoveWindow(left + width * 4 ,height * 6,
						m_ConfigData.m_LED_NUM_Right[3] - m_ConfigData.m_LED_NUM_Left[3]  ,
						m_ConfigData.m_LED_NUM_Bottom[3] - m_ConfigData.m_LED_NUM_Top[3]  ,true);
				}


			// 加热指示灯区域
			left = CAMERA_WIDTH + (wnd_width - CAMERA_WIDTH) * 0.4;
			this->m_STATIC_HEAT_LED_GROUP.MoveWindow(left ,0,(wnd_width - CAMERA_WIDTH) * 0.2 ,wnd_height * 0.3,true);
			this->m_STATIC_HEAT_LED_GROUP.GetClientRect(&rect);
			width  = rect.Width() / 2;
			height = rect.Height() / 8;

			w = GetDlgItem(IDC_STATIC_LED_HEAT);
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_HEAT_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_HEAT_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_HEAT_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_HEAT_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);

			w = GetDlgItem(IDC_EDIT_LED_HEAT);
			left += width;
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_HEAT_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_HEAT_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_HEAT_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_HEAT_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);

			w = GetDlgItem(IDC_PIC_LED_HEAT);
			w->MoveWindow(left ,height * 6,
				m_ConfigData.m_LED_HEAT_Right - m_ConfigData.m_LED_HEAT_Left  ,
				m_ConfigData.m_LED_HEAT_Bottom - m_ConfigData.m_LED_HEAT_Top  ,true);

			//制冷指示灯区域
			left = CAMERA_WIDTH + (wnd_width - CAMERA_WIDTH) * 0.6;
			this->m_STATIC_COOL_LED_GROUP.MoveWindow(left ,0,(wnd_width - CAMERA_WIDTH) * 0.2 ,wnd_height * 0.3,true);
			this->m_STATIC_COOL_LED_GROUP.GetClientRect(&rect);
			width  = rect.Width() / 2;
			height = rect.Height() / 8;

			w = GetDlgItem(IDC_STATIC_LED_COOL);
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COOL_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COOL_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COOL_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COOL_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);

			w = GetDlgItem(IDC_EDIT_LED_COOL);
			left += width;
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COOL_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COOL_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COOL_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COOL_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);
			
			w = GetDlgItem(IDC_PIC_LED_COOL);
			w->MoveWindow(left ,height * 6,
				m_ConfigData.m_LED_COOL_Right - m_ConfigData.m_LED_COOL_Left  ,
				m_ConfigData.m_LED_COOL_Bottom - m_ConfigData.m_LED_COOL_Top  ,true);



			//彩色指示灯区域
			left = CAMERA_WIDTH + (wnd_width - CAMERA_WIDTH) * 0.8;
			this->m_STATIC_COLOR_LED_GROUP.MoveWindow(left ,0,(wnd_width - CAMERA_WIDTH) * 0.2 ,wnd_height * 0.3,true);
			this->m_STATIC_COLOR_LED_GROUP.GetClientRect(&rect);
			width  = rect.Width() / 2;
			height = rect.Height() / 8;

			w = GetDlgItem(IDC_STATIC_LED_COLOR);
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COLOR_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COLOR_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COLOR_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COLOR_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);

			w = GetDlgItem(IDC_EDIT_LED_COLOR);
			left += width;
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COLOR_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COLOR_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COLOR_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COLOR_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);

			w = GetDlgItem(IDC_PIC_LED_COLOR);
			w->MoveWindow(left ,height * 6,
				m_ConfigData.m_LED_COLOR_Right - m_ConfigData.m_LED_COLOR_Left  ,
				m_ConfigData.m_LED_COLOR_Bottom - m_ConfigData.m_LED_COLOR_Top  ,true);


			//编程区
			left = CAMERA_WIDTH ;
			top =  wnd_height * 0.3;
			w = GetDlgItem(IDC_STATIC_PROGRAM_GROUP);
			w->MoveWindow(left ,top,+ (wnd_width - CAMERA_WIDTH) * 0.5 ,wnd_height * 0.2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 9;
			w = GetDlgItem(IDC_STATIC_PROGRAM_BARCODE);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 3,height ,true);
			w = GetDlgItem(IDC_EDIT_MSG);
			w->MoveWindow(left + width * 4,top + height * 1 ,width * 4,height ,true);

			

			w = GetDlgItem(IDC_CHECK_PCB_BUTTON_TEST_NEEDLE);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 2,height,true);
			w = GetDlgItem(IDC_CHECK_PCB_BUTTON_TEST_START);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 2,height,true);
			
			w = GetDlgItem(IDC_CHECK_PCB_BUTTON_SET);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 2,height,true);
			
			w = GetDlgItem(IDC_CHECK_PCB_BUTTON_PLUS);
			w->MoveWindow(left + width * 0,top + height * 5 ,width * 2,height,true);
			w = GetDlgItem(IDC_CHECK_PCB_BUTTON_MINUS);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 2,height,true);
			
			w = GetDlgItem(IDC_BUTTON_PROGRAM_START);
			w->MoveWindow(left + width * 0,top + height * 7 ,width * 4,height * 2,true);
			
			w = GetDlgItem(IDC_EDIT_RESULT);
			w->MoveWindow(left + width * 2,top + height * 2 ,width * 2,height * 5,true);
			
				
			w = GetDlgItem(IDC_LIST_PROGRAM);
			w->MoveWindow(left + width * 4,top + height * 2 ,width * 4,height * 7,true);
				
			//编程良率
			left = CAMERA_WIDTH + (wnd_width - CAMERA_WIDTH) * 0.5;
			top =  wnd_height * 0.3 ;
			w = GetDlgItem(IDC_STATIC_PROGRAM_PERCENT_GROUP);
			w->MoveWindow(left ,top,(wnd_width - CAMERA_WIDTH) * 0.25 ,wnd_height  * 0.2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 2;
			height = rect.Height() / 5;
			w = GetDlgItem(IDC_STATIC_PROGRAM_PASS);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_PROGRAM_FAIL);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_PROGRAM_CNT);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_PROGRAM_PERCENT);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 1,height,true);


			w = GetDlgItem(IDC_EDIT_PROGRAM_PASS);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PROGRAM_FAIL);
			w->MoveWindow(left + width * 1,top + height * 2 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PROGRAM_CNT);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PROGRAM_PERCENT);
			w->MoveWindow(left + width * 1,top + height * 4 ,width * 1,height,true);

			//老化良率
				
			left = CAMERA_WIDTH + (wnd_width - CAMERA_WIDTH) * 0.75;
			top =  wnd_height * 0.3 ;
			w = GetDlgItem(IDC_STATIC_AGING_PERCENT_GROUP);
			w->MoveWindow(left ,top,(wnd_width - CAMERA_WIDTH) * 0.25 ,wnd_height * 0.2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 2;
			height = rect.Height() / 5;
			w = GetDlgItem(IDC_STATIC_AGING_PASS);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_AGING_FAIL);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_AGING_CNT);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_AGING_PERCENT);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 1,height,true);


			w = GetDlgItem(IDC_EDIT_AGING_PASS);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_AGING_FAIL);
			w->MoveWindow(left + width * 1,top + height * 2 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_AGING_CNT);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_AGING_PERCENT);
			w->MoveWindow(left + width * 1,top + height * 4 ,width * 1,height,true);

			//POWER 1
			top = wnd_height * 0.5;

			left = wnd_width * 0 / 8;
			
			w = GetDlgItem(IDC_STATIC_POWER_1);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_1);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_1);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_1);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_1);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_1);
			w->MoveWindow(left + width * 2,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_STATIC_SV_1);
			w->MoveWindow(left + width * 4,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_1);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 2,height,true);

			w = GetDlgItem(IDC_EDIT_RESULT_1);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_1);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_1);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_1);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);

			//POWER 2
			left = wnd_width * 1 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_2);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_2);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_2);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_2);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_2);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_2);
			w->MoveWindow(left + width * 2,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_STATIC_SV_2);
			w->MoveWindow(left + width * 4,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_2);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 2,height,true);

			w = GetDlgItem(IDC_EDIT_RESULT_2);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_2);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_2);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_2);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);
			
			//POWER 3
			left = wnd_width * 2 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_3);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_3);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_3);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_3);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_3);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_3);
			w->MoveWindow(left + width * 2,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_STATIC_SV_3);
			w->MoveWindow(left + width * 4,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_3);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 2,height,true);

			w = GetDlgItem(IDC_EDIT_RESULT_3);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_3);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_3);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_3);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);

			//POWER 4
			left = wnd_width * 3 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_4);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_4);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_4);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_4);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_4);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_4);
			w->MoveWindow(left + width * 2,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_STATIC_SV_4);
			w->MoveWindow(left + width * 4,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_4);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 2,height,true);

			w = GetDlgItem(IDC_EDIT_RESULT_4);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_4);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_4);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_4);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);


			//POWER 5
			left = wnd_width * 4 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_5);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_5);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_5);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_5);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_5);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_5);
			w->MoveWindow(left + width * 2,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_STATIC_SV_5);
			w->MoveWindow(left + width * 4,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_5);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 2,height,true);

			w = GetDlgItem(IDC_EDIT_RESULT_5);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_5);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_5);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_5);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);

			//POWER 6
			left = wnd_width * 5 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_6);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_6);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_6);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_6);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_6);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_6);
			w->MoveWindow(left + width * 2,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_STATIC_SV_6);
			w->MoveWindow(left + width * 4,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_6);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 2,height,true);

			w = GetDlgItem(IDC_EDIT_RESULT_6);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_6);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_6);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_6);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);

			//POWER 7
			left = wnd_width * 6 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_7);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_7);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_7);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_7);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_7);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_7);
			w->MoveWindow(left + width * 2,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_STATIC_SV_7);
			w->MoveWindow(left + width * 4,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_7);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 2,height,true);

			w = GetDlgItem(IDC_EDIT_RESULT_7);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_7);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_7);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_7);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);


			//POWER 8
			left = wnd_width * 7 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_8);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_8);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_8);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_8);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_8);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_8);
			w->MoveWindow(left + width * 2,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_STATIC_SV_8);
			w->MoveWindow(left + width * 4,top + height * 3 ,width * 2,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_8);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 2,height,true);

			w = GetDlgItem(IDC_EDIT_RESULT_8);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_8);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_8);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_8);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);


		//	StartVideo();
		}
}		

void CHotLong_PCBDlg::ReMoveWindows_X90(void)
{
		if(this->IsWindowVisible())
		{
			CWnd *w;
			CRect rect;
			GetClientRect(&rect); // 获取客户区的大小
			int wnd_width = rect.Width();  // 窗口宽度
			int wnd_height = rect.Height(); // 窗口高度

			this->m_staticScreen.MoveWindow(0,0,CAMERA_WIDTH,CAMERA_HEIGHT,true);

			
			/*int ID = ::m_ConfigData.m_Camera_Nr;
			m_ListCtrl.ResetContent();
			m_cap.EnumDevices (&m_ListCtrl,m_ListCtrl);
			m_ListCtrl.SetCurSel (ID);
			m_cap.Init(ID,this->m_staticScreen);
			*/
			int left = CAMERA_WIDTH;
			int top  = 0;
			this->m_LedNumGroup.MoveWindow(left,0,(wnd_width - CAMERA_WIDTH) * 0.4 ,wnd_height * 0.3,true);
			this->m_LedNumGroup.GetClientRect(&rect);
			int width  = rect.Width() / 5;
			int height = rect.Height() / 8;

			this->m_STATIC_DEGREE.MoveWindow(left,height,width  ,height ,true);
			this->m_Edit_Degree.MoveWindow(left + width ,height,width  ,height ,true);

			this->m_LedNumGroup.ShowWindow(SW_HIDE);
			this->m_STATIC_DEGREE.ShowWindow(SW_HIDE);
			this->m_Edit_Degree.ShowWindow(SW_HIDE);
			for(int i = 0; i < 4; i++)
			{
				this->m_LED_NUM_Left[i].ShowWindow(SW_HIDE);
				this->m_LED_NUM_Right[i].ShowWindow(SW_HIDE);
				this->m_LED_NUM_Top[i].ShowWindow(SW_HIDE);
				this->m_LED_NUM_Bottom[i].ShowWindow(SW_HIDE);
			}
			this->m_STATIC_LEFT.ShowWindow(SW_HIDE);
			this->m_STATIC_RIGHT.ShowWindow(SW_HIDE);
			this->m_STATIC_TOP.ShowWindow(SW_HIDE);
			this->m_STATIC_BOTTOM.ShowWindow(SW_HIDE);
			m_PIC_LED_NUM_1.ShowWindow(SW_HIDE);
			m_PIC_LED_NUM_2.ShowWindow(SW_HIDE);
			m_PIC_LED_NUM_3.ShowWindow(SW_HIDE);
			m_PIC_LED_NUM_4.ShowWindow(SW_HIDE);
/*
			this->m_STATIC_LEFT.MoveWindow(left,height * 2,width  ,height ,true);
			this->m_STATIC_RIGHT.MoveWindow(left,height * 3,width  ,height ,true);
			this->m_STATIC_TOP.MoveWindow(left,height * 4,width  ,height ,true);
			this->m_STATIC_BOTTOM.MoveWindow(left,height * 5,width  ,height ,true);
			for(int i = 0; i < 4; i++)
			{
				this->m_LED_NUM_Left[i].MoveWindow(left + width * (1 + i) ,height * 2,width  ,height ,true);
				this->m_LED_NUM_Right[i].MoveWindow(left + width * (1 + i) ,height * 3,width  ,height ,true);
				this->m_LED_NUM_Top[i].MoveWindow(left + width * (1 + i) ,height * 4,width  ,height ,true);
				this->m_LED_NUM_Bottom[i].MoveWindow(left + width * (1 + i) ,height * 5,width  ,height ,true);
			}
			this->m_CheckFocus.MoveWindow(left + width * 0 ,height * 6,width  ,height*2 ,true);
			 
			if(m_ConfigData.m_LED_NUM_Right[0] > m_ConfigData.m_LED_NUM_Left[0]
				&& m_ConfigData.m_LED_NUM_Bottom[0] > m_ConfigData.m_LED_NUM_Top[0])
				{
					m_PIC_LED_NUM_1.MoveWindow(left + width * 1 ,height * 6,
						m_ConfigData.m_LED_NUM_Right[0] - m_ConfigData.m_LED_NUM_Left[0]  ,
						m_ConfigData.m_LED_NUM_Bottom[0] - m_ConfigData.m_LED_NUM_Top[0]  ,true);
				}
			if(m_ConfigData.m_LED_NUM_Right[1] > m_ConfigData.m_LED_NUM_Left[1]
				&& m_ConfigData.m_LED_NUM_Bottom[1] > m_ConfigData.m_LED_NUM_Top[1])
				{
					m_PIC_LED_NUM_2.MoveWindow(left + width * 2 ,height * 6,
						m_ConfigData.m_LED_NUM_Right[1] - m_ConfigData.m_LED_NUM_Left[1]  ,
						m_ConfigData.m_LED_NUM_Bottom[1] - m_ConfigData.m_LED_NUM_Top[1]  ,true);
				}
			if(m_ConfigData.m_LED_NUM_Right[2] > m_ConfigData.m_LED_NUM_Left[2]
				&& m_ConfigData.m_LED_NUM_Bottom[2] > m_ConfigData.m_LED_NUM_Top[2])
				{
					m_PIC_LED_NUM_3.MoveWindow(left + width * 3 ,height * 6,
						m_ConfigData.m_LED_NUM_Right[2] - m_ConfigData.m_LED_NUM_Left[2]  ,
						m_ConfigData.m_LED_NUM_Bottom[2] - m_ConfigData.m_LED_NUM_Top[2]  ,true);
				}
			if(m_ConfigData.m_LED_NUM_Right[3] > m_ConfigData.m_LED_NUM_Left[3]
				&& m_ConfigData.m_LED_NUM_Bottom[3] > m_ConfigData.m_LED_NUM_Top[3])
				{
					m_PIC_LED_NUM_4.MoveWindow(left + width * 4 ,height * 6,
						m_ConfigData.m_LED_NUM_Right[3] - m_ConfigData.m_LED_NUM_Left[3]  ,
						m_ConfigData.m_LED_NUM_Bottom[3] - m_ConfigData.m_LED_NUM_Top[3]  ,true);
				}
*/

			// 加热指示灯区域
			left = CAMERA_WIDTH + (wnd_width - CAMERA_WIDTH) * 0.4;
			this->m_STATIC_HEAT_LED_GROUP.MoveWindow(left ,0,(wnd_width - CAMERA_WIDTH) * 0.2 ,wnd_height * 0.3,true);
			this->m_STATIC_HEAT_LED_GROUP.GetClientRect(&rect);
			width  = rect.Width() / 2;
			height = rect.Height() / 8;

			w = GetDlgItem(IDC_STATIC_LED_HEAT);
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_HEAT_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_HEAT_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_HEAT_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_HEAT_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);

			w = GetDlgItem(IDC_EDIT_LED_HEAT);
			left += width;
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_HEAT_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_HEAT_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_HEAT_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_HEAT_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);

			w = GetDlgItem(IDC_PIC_LED_HEAT);
			w->MoveWindow(left ,height * 6,
				m_ConfigData.m_LED_HEAT_Right - m_ConfigData.m_LED_HEAT_Left  ,
				m_ConfigData.m_LED_HEAT_Bottom - m_ConfigData.m_LED_HEAT_Top  ,true);

			//制冷指示灯区域
			left = CAMERA_WIDTH + (wnd_width - CAMERA_WIDTH) * 0.6;
			this->m_STATIC_COOL_LED_GROUP.MoveWindow(left ,0,(wnd_width - CAMERA_WIDTH) * 0.2 ,wnd_height * 0.3,true);
			this->m_STATIC_COOL_LED_GROUP.GetClientRect(&rect);
			width  = rect.Width() / 2;
			height = rect.Height() / 8;

			w = GetDlgItem(IDC_STATIC_LED_COOL);
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COOL_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COOL_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COOL_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COOL_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);

			w = GetDlgItem(IDC_EDIT_LED_COOL);
			left += width;
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COOL_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COOL_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COOL_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COOL_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);
			
			w = GetDlgItem(IDC_PIC_LED_COOL);
			w->MoveWindow(left ,height * 6,
				m_ConfigData.m_LED_COOL_Right - m_ConfigData.m_LED_COOL_Left  ,
				m_ConfigData.m_LED_COOL_Bottom - m_ConfigData.m_LED_COOL_Top  ,true);



			//彩色指示灯区域
			left = CAMERA_WIDTH + (wnd_width - CAMERA_WIDTH) * 0.8;
			this->m_STATIC_COLOR_LED_GROUP.MoveWindow(left ,0,(wnd_width - CAMERA_WIDTH) * 0.2 ,wnd_height * 0.3,true);
			this->m_STATIC_COLOR_LED_GROUP.GetClientRect(&rect);
			width  = rect.Width() / 2;
			height = rect.Height() / 8;

			w = GetDlgItem(IDC_STATIC_LED_COLOR);
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COLOR_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COLOR_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COLOR_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_STATIC_COLOR_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);

			w = GetDlgItem(IDC_EDIT_LED_COLOR);
			left += width;
			w->MoveWindow(left ,height * 1,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COLOR_LEFT);
			w->MoveWindow(left ,height * 2,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COLOR_RIGHT);
			w->MoveWindow(left ,height * 3,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COLOR_TOP);
			w->MoveWindow(left ,height * 4,width  ,height ,true);
			w = GetDlgItem(IDC_EDIT_LED_COLOR_BOTTOM);
			w->MoveWindow(left ,height * 5,width  ,height ,true);

			w = GetDlgItem(IDC_PIC_LED_COLOR);
			w->MoveWindow(left ,height * 6,
				m_ConfigData.m_LED_COLOR_Right - m_ConfigData.m_LED_COLOR_Left  ,
				m_ConfigData.m_LED_COLOR_Bottom - m_ConfigData.m_LED_COLOR_Top  ,true);


			//编程区
			left = CAMERA_WIDTH ;
			top =  wnd_height * 0.3;
			w = GetDlgItem(IDC_STATIC_PROGRAM_GROUP);
			w->MoveWindow(left ,top,+ (wnd_width - CAMERA_WIDTH) * 0.5 ,wnd_height * 0.2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 9;
			w = GetDlgItem(IDC_STATIC_PROGRAM_BARCODE);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 3,height ,true);
			w = GetDlgItem(IDC_EDIT_MSG);
			w->MoveWindow(left + width * 4,top + height * 1 ,width * 4,height ,true);

			

			w = GetDlgItem(IDC_CHECK_PCB_BUTTON_TEST_NEEDLE);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 2,height,true);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_CHECK_PCB_BUTTON_TEST_START);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 2,height,true);
			
			w = GetDlgItem(IDC_CHECK_PCB_BUTTON_SET);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 2,height,true);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_CHECK_PCB_BUTTON_PLUS);
			w->MoveWindow(left + width * 0,top + height * 5 ,width * 2,height,true);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_CHECK_PCB_BUTTON_MINUS);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 2,height,true);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_BUTTON_PROGRAM_START);
			w->MoveWindow(left + width * 0,top + height * 7 ,width * 4,height * 2,true);
			w->ShowWindow(SW_HIDE);

			w = GetDlgItem(IDC_EDIT_RESULT);
			w->MoveWindow(left + width * 2,top + height * 2 ,width * 2,height * 5,true);
			
				
			w = GetDlgItem(IDC_LIST_PROGRAM);
			w->MoveWindow(left + width * 4,top + height * 2 ,width * 4,height * 7,true);


				
			//编程良率
			left = CAMERA_WIDTH + (wnd_width - CAMERA_WIDTH) * 0.5;
			top =  wnd_height * 0.3 ;
			w = GetDlgItem(IDC_STATIC_PROGRAM_PERCENT_GROUP);
			w->MoveWindow(left ,top,(wnd_width - CAMERA_WIDTH) * 0.25 ,wnd_height  * 0.2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 2;
			height = rect.Height() / 5;
			w = GetDlgItem(IDC_STATIC_PROGRAM_PASS);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_PROGRAM_FAIL);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_PROGRAM_CNT);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_PROGRAM_PERCENT);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 1,height,true);


			w = GetDlgItem(IDC_EDIT_PROGRAM_PASS);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PROGRAM_FAIL);
			w->MoveWindow(left + width * 1,top + height * 2 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PROGRAM_CNT);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PROGRAM_PERCENT);
			w->MoveWindow(left + width * 1,top + height * 4 ,width * 1,height,true);

			//老化良率
				
			left = CAMERA_WIDTH + (wnd_width - CAMERA_WIDTH) * 0.75;
			top =  wnd_height * 0.3 ;
			w = GetDlgItem(IDC_STATIC_AGING_PERCENT_GROUP);
			w->MoveWindow(left ,top,(wnd_width - CAMERA_WIDTH) * 0.25 ,wnd_height * 0.2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 2;
			height = rect.Height() / 5;
			w = GetDlgItem(IDC_STATIC_AGING_PASS);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_AGING_FAIL);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_AGING_CNT);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_STATIC_AGING_PERCENT);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 1,height,true);


			w = GetDlgItem(IDC_EDIT_AGING_PASS);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_AGING_FAIL);
			w->MoveWindow(left + width * 1,top + height * 2 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_AGING_CNT);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_AGING_PERCENT);
			w->MoveWindow(left + width * 1,top + height * 4 ,width * 1,height,true);

			//POWER 1
			top = wnd_height * 0.5;

			left = wnd_width * 0 / 8;
			
			w = GetDlgItem(IDC_STATIC_POWER_1);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_1);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_1);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_1);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_1);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_1);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_SV_1);
			w->MoveWindow(left + width * 2.5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_1);
			w->MoveWindow(left + width * 3.5,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_PP_1);
			w->MoveWindow(left + width * 5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PP_1);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 1.5,height,true);

			w = GetDlgItem(IDC_EDIT_RESULT_1);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_1);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_1);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_1);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);

			//POWER 2
			left = wnd_width * 1 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_2);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_2);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_2);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_2);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_2);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_2);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_SV_2);
			w->MoveWindow(left + width * 2.5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_2);
			w->MoveWindow(left + width * 3.5,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_PP_2);
			w->MoveWindow(left + width * 5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PP_2);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 1.5,height,true);

			w = GetDlgItem(IDC_EDIT_RESULT_2);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_2);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_2);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_2);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);
			
			//POWER 3
			left = wnd_width * 2 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_3);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_3);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_3);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_3);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_3);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_3);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_SV_3);
			w->MoveWindow(left + width * 2.5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_3);
			w->MoveWindow(left + width * 3.5,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_PP_3);
			w->MoveWindow(left + width * 5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PP_3);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 1.5,height,true);


			w = GetDlgItem(IDC_EDIT_RESULT_3);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_3);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_3);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_3);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);

			//POWER 4
			left = wnd_width * 3 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_4);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_4);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_4);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_4);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_4);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_4);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_SV_4);
			w->MoveWindow(left + width * 2.5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_4);
			w->MoveWindow(left + width * 3.5,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_PP_4);
			w->MoveWindow(left + width * 5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PP_4);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 1.5,height,true);


			w = GetDlgItem(IDC_EDIT_RESULT_4);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_4);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_4);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_4);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);


			//POWER 5
			left = wnd_width * 4 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_5);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_5);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_5);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_5);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_5);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_5);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_SV_5);
			w->MoveWindow(left + width * 2.5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_5);
			w->MoveWindow(left + width * 3.5,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_PP_5);
			w->MoveWindow(left + width * 5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PP_5);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 1.5,height,true);


			w = GetDlgItem(IDC_EDIT_RESULT_5);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_5);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_5);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_5);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);

			//POWER 6
			left = wnd_width * 5 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_6);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_6);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_6);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_6);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_6);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_6);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_SV_6);
			w->MoveWindow(left + width * 2.5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_6);
			w->MoveWindow(left + width * 3.5,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_PP_6);
			w->MoveWindow(left + width * 5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PP_6);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 1.5,height,true);


			w = GetDlgItem(IDC_EDIT_RESULT_6);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_6);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_6);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_6);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);

			//POWER 7
			left = wnd_width * 6 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_7);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_7);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_7);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_7);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_7);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_7);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_SV_7);
			w->MoveWindow(left + width * 2.5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_7);
			w->MoveWindow(left + width * 3.5,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_PP_7);
			w->MoveWindow(left + width * 5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PP_7);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 1.5,height,true);


			w = GetDlgItem(IDC_EDIT_RESULT_7);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_7);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_7);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_7);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);


			//POWER 8
			left = wnd_width * 7 / 8;
			top = wnd_height * 1 / 2;
			w = GetDlgItem(IDC_STATIC_POWER_8);
			w->MoveWindow(left ,top,wnd_width/8 ,wnd_height / 2,true);
			w->GetClientRect(&rect);
			width = rect.Width()  / 8;
			height = rect.Height() / 16;
			w = GetDlgItem(IDC_STATIC_BARCODE_8);
			w->MoveWindow(left + width * 0,top + height * 1 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_BARCODE_8);
			w->MoveWindow(left + width * 1,top + height * 1 ,width * 7,height ,true);

			w = GetDlgItem(IDC_CHECK_PCB_POWER_8);
			w->MoveWindow(left + width * 0,top + height * 2 ,width * 5,height,true);
		
			w = GetDlgItem(IDC_STATIC_PV_8);
			w->MoveWindow(left + width * 0,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Pv_8);
			w->MoveWindow(left + width * 1,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_SV_8);
			w->MoveWindow(left + width * 2.5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_Sv_8);
			w->MoveWindow(left + width * 3.5,top + height * 3 ,width * 1.5,height,true);
			w = GetDlgItem(IDC_STATIC_PP_8);
			w->MoveWindow(left + width * 5,top + height * 3 ,width * 1,height,true);
			w = GetDlgItem(IDC_EDIT_PP_8);
			w->MoveWindow(left + width * 6,top + height * 3 ,width * 1.5,height,true);


			w = GetDlgItem(IDC_EDIT_RESULT_8);
			w->MoveWindow(left + width * 0,top + height * 4 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_BUTTON_START_8);
			w->MoveWindow(left + width * 0,top + height * 6 ,width * 8,height * 2,true);
		
			w = GetDlgItem(IDC_EDIT_MSG_8);
			w->MoveWindow(left + width * 0,top + height * 8 ,width * 8,height * 1,true);
			
			
			w = GetDlgItem(IDC_LIST_8);
			w->MoveWindow(left + width * 0,top + height * 9 ,width * 8,height * 7,true);


		//	StartVideo();
		}
}		


//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHotLong_PCBDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHotLong_PCBDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//OnOK();

	this->m_ParaDlg.DoModal();
}

void CHotLong_PCBDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	StartVideo();
	m_cap.GrabOneFrame(true);

}


void ReadConfig(void)
{
	CString sDir = _T("C:\\data");

	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	

	CFile f;
	if (f.Open(_T("c:\\data\\Config.bin"), CFile::modeRead | CFile::modeCreate | CFile::modeNoTruncate))
	{
		f.SeekToBegin();
		
		int count = f.Read(&m_ConfigData, sizeof(m_ConfigData));
		if(count < sizeof(m_ConfigData))
		{
			memset(&m_ConfigData,0,sizeof(m_ConfigData));

	/*		for (int i = 0; i < 4; i++)
			{
				m_ConfigData.m_LED_NUM_Left[i] = 310;
				m_ConfigData.m_LED_NUM_Right[i] = 540;
				m_ConfigData.m_LED_NUM_Top[i] = 200;
				m_ConfigData.m_LED_NUM_Bottom[i] = 280;
			}

			m_ConfigData.m_LED_HEAT_Left	= 320;
			m_ConfigData.m_LED_HEAT_Right	= 370;
			m_ConfigData.m_LED_HEAT_Top		= 310;
			m_ConfigData.m_LED_HEAT_Bottom	= 340;

			m_ConfigData.m_LED_COOL_Left	= 390;
			m_ConfigData.m_LED_COOL_Right	= 440;
			m_ConfigData.m_LED_COOL_Top		= 310;
			m_ConfigData.m_LED_COOL_Bottom	= 340;


			m_ConfigData.m_LED_COLOR_Left	= 460;
			m_ConfigData.m_LED_COLOR_Right	= 510;
			m_ConfigData.m_LED_COLOR_Top	= 310;
			m_ConfigData.m_LED_COLOR_Bottom = 340;
*/

		}

		
	}
	else
	{
		AfxMessageBox(_T("Read f.Open(_T(\"c:\\Config.bin\") Fail"));
	}
}

void SaveConfig(void)
{
	CString sDir = _T("C:\\data");

	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	CFile f;
	if (f.Open(_T("c:\\data\\Config.bin"), CFile::modeReadWrite | CFile::modeCreate))
	{
		f.SeekToBegin();
		f.Write(&m_ConfigData, sizeof(m_ConfigData));
		f.Close();
	}
	else
	{
		AfxMessageBox(_T("Save f.Open(_T(\"c:\\Config.bin\") Fail"));
	}
}



void CHotLong_PCBDlg::OnEnChangeEditLedNumLeft_1()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumRight_1()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumTop_1()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumBottom_1()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumLeft_2()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumRight_2()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumTop_2()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumBottom_2()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumLeft_3()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumRight_3()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumTop_3()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumBottom_3()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumLeft_4()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumRight_4()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumTop_4()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedNumBottom_4()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedHeatLeft()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedHeatRight()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedHeatTop()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedHeatBottom()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedCoolLeft()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedCoolRight()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedCoolTop()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedCoolBottom()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedColorLeft()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedColorRight()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedColorTop()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditLedColorBottom()
{
	this->UpdateData();
	SaveConfig();
	this->RedrawWindow();
	this->StartVideo();
}

void CHotLong_PCBDlg::OnEnChangeEditSv_1()
{
	this->UpdateData(true);
	SetSv( 0, m_Sv[1]);
}

void CHotLong_PCBDlg::SetSv(int PCB_ID,int Sv)
{
	m_Device_HotLong_PCB[PCB_ID].SetBaudrate(m_Device_HotLong_PCB[PCB_ID].m_Comm);

	m_Device_HotLong_PCB[PCB_ID].SetSv(Sv);

}
LRESULT CHotLong_PCBDlg::OnComm_Device_HuiKong_DIO(WPARAM wParam, LPARAM lParam)
{
	this->m_ParaDlg.DisplayRec_PLC((unsigned char *)wParam,(int)lParam);

	m_Device_HuiKong_DIO.DealOnComm((unsigned char *)wParam,(int)lParam);
	
	CString s;
	s.Format(_T("%d"),(int)lParam);
	this->m_Device_HotLong_PCB[0].m_BarCode = s;
	this->UpdateData(false);
	
	this->m_CheckPCB_Button_Set.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_SET_ID]);
	this->m_CheckPCB_Button_Plus.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID]);
	this->m_CheckPCB_Button_Minus.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID]);
	this->m_CheckPCB_Button_Test_Needle.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_TEST_NEEDLE_ID]);
	this->m_CheckPCB_Button_Test_Start.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID]);
	this->m_CheckPCB_Button_Beep.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_BEEP_ID]);

	m_Device_HotLong_PCB[1].m_CheckPCB_Power.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_1]);
	m_Device_HotLong_PCB[2].m_CheckPCB_Power.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_2]);
	m_Device_HotLong_PCB[3].m_CheckPCB_Power.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_3]);
	m_Device_HotLong_PCB[4].m_CheckPCB_Power.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_4]);
	m_Device_HotLong_PCB[5].m_CheckPCB_Power.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_5]);
	m_Device_HotLong_PCB[6].m_CheckPCB_Power.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_6]);
	m_Device_HotLong_PCB[7].m_CheckPCB_Power.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_7]);
	m_Device_HotLong_PCB[8].m_CheckPCB_Power.SetCheck(m_Device_HuiKong_DIO.m_Y[m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_8]);
		
	return 0;//DealOnComm_Device_HuiKong_DIO(wParam, lParam, this);

}

LRESULT CHotLong_PCBDlg::OnComm_PROGRAM(WPARAM wParam, LPARAM lParam)
{
	int ID = 0;
	m_Device_HotLong_PCB[ID].DealOnComm((unsigned char *)wParam,(int)lParam);
	m_Pv[ID] = m_Device_HotLong_PCB[ID].m_Pv;
	m_Sv[ID] = m_Device_HotLong_PCB[ID].m_Sv;
	m_Pp[ID] = m_Device_HotLong_PCB[ID].m_Pp;
	this->UpdateData(false);

	return 0;//DealOnComm_Device_HuiKong_DIO(wParam, lParam, this);

}


LRESULT CHotLong_PCBDlg::OnComm_PCB_1(WPARAM wParam, LPARAM lParam)
{
	int ID = 1;
	m_Device_HotLong_PCB[ID].DealOnComm((unsigned char *)wParam,(int)lParam);
	m_Pv[ID] = m_Device_HotLong_PCB[ID].m_Pv;
	m_Sv[ID] = m_Device_HotLong_PCB[ID].m_Sv;
	m_Pp[ID] = m_Device_HotLong_PCB[ID].m_Pp;
	this->UpdateData(false);
	

	return 0;

}
LRESULT CHotLong_PCBDlg::OnComm_PCB_2(WPARAM wParam, LPARAM lParam)
{
	int ID = 2;
	m_Device_HotLong_PCB[ID].DealOnComm((unsigned char *)wParam,(int)lParam);
	m_Pv[ID] = m_Device_HotLong_PCB[ID].m_Pv;
	m_Sv[ID] = m_Device_HotLong_PCB[ID].m_Sv;
	m_Pp[ID] = m_Device_HotLong_PCB[ID].m_Pp;
	this->UpdateData(false);

	return 0;
}

LRESULT CHotLong_PCBDlg::OnComm_PCB_3(WPARAM wParam, LPARAM lParam)
{

	int ID = 3;
	m_Device_HotLong_PCB[ID].DealOnComm((unsigned char *)wParam,(int)lParam);
	m_Pv[ID] = m_Device_HotLong_PCB[ID].m_Pv;
	m_Sv[ID] = m_Device_HotLong_PCB[ID].m_Sv;
	m_Pp[ID] = m_Device_HotLong_PCB[ID].m_Pp;
	this->UpdateData(false);
	return 0;

}
LRESULT CHotLong_PCBDlg::OnComm_PCB_4(WPARAM wParam, LPARAM lParam)
{
	int ID = 4;
	m_Device_HotLong_PCB[ID].DealOnComm((unsigned char *)wParam,(int)lParam);
	m_Pv[ID] = m_Device_HotLong_PCB[ID].m_Pv;
	m_Sv[ID] = m_Device_HotLong_PCB[ID].m_Sv;
	m_Pp[ID] = m_Device_HotLong_PCB[ID].m_Pp;
	this->UpdateData(false);
	return 0;

}
LRESULT CHotLong_PCBDlg::OnComm_PCB_5(WPARAM wParam, LPARAM lParam)
{
	int ID = 5;
	m_Device_HotLong_PCB[ID].DealOnComm((unsigned char *)wParam,(int)lParam);
	m_Pv[ID] = m_Device_HotLong_PCB[ID].m_Pv;
	m_Sv[ID] = m_Device_HotLong_PCB[ID].m_Sv;
	m_Pp[ID] = m_Device_HotLong_PCB[ID].m_Pp;
	this->UpdateData(false);
	return 0;

}
LRESULT CHotLong_PCBDlg::OnComm_PCB_6(WPARAM wParam, LPARAM lParam)
{
	int ID = 6;
	m_Device_HotLong_PCB[ID].DealOnComm((unsigned char *)wParam,(int)lParam);
	m_Pv[ID] = m_Device_HotLong_PCB[ID].m_Pv;
	m_Sv[ID] = m_Device_HotLong_PCB[ID].m_Sv;
	m_Pp[ID] = m_Device_HotLong_PCB[ID].m_Pp;
	this->UpdateData(false);
	return 0;

}

LRESULT CHotLong_PCBDlg::OnComm_PCB_7(WPARAM wParam, LPARAM lParam)
{
	int ID = 7;
	m_Device_HotLong_PCB[ID].DealOnComm((unsigned char *)wParam,(int)lParam);
	m_Pv[ID] = m_Device_HotLong_PCB[ID].m_Pv;
	m_Sv[ID] = m_Device_HotLong_PCB[ID].m_Sv;
	m_Pp[ID] = m_Device_HotLong_PCB[ID].m_Pp;
	this->UpdateData(false);
	return 0;

}

LRESULT CHotLong_PCBDlg::OnComm_PCB_8(WPARAM wParam, LPARAM lParam)
{
	int ID = 8;
	m_Device_HotLong_PCB[ID].DealOnComm((unsigned char *)wParam,(int)lParam);
	m_Pv[ID] = m_Device_HotLong_PCB[ID].m_Pv;
	m_Sv[ID] = m_Device_HotLong_PCB[ID].m_Sv;
	m_Pp[ID] = m_Device_HotLong_PCB[ID].m_Pp;

	this->UpdateData(false);
	return 0;

}


void CHotLong_PCBDlg::OnBnClickedButtonPCB_Set()
{

	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_SET_ID, m_CheckPCB_Button_Set.GetCheck());

}
void CHotLong_PCBDlg::OnBnClickedButtonPCB_Needle()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_TEST_NEEDLE_ID, this->m_CheckPCB_Button_Test_Needle.GetCheck());
}
void CHotLong_PCBDlg::OnBnClickedButtonPCB_Plus()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID, this->m_CheckPCB_Button_Plus.GetCheck());
}
void CHotLong_PCBDlg::OnBnClickedButtonPCB_Minus()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID, this->m_CheckPCB_Button_Minus.GetCheck());
}
void CHotLong_PCBDlg::OnBnClickedButtonPCB_Write()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID, this->m_CheckPCB_Button_Test_Start.GetCheck());
}

void CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_1()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_1, m_Device_HotLong_PCB[1].m_CheckPCB_Power.GetCheck());
}
void CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_2()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_2, m_Device_HotLong_PCB[2].m_CheckPCB_Power.GetCheck());
}
void CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_3()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_3, m_Device_HotLong_PCB[3].m_CheckPCB_Power.GetCheck());
}
void CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_4()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_4, m_Device_HotLong_PCB[4].m_CheckPCB_Power.GetCheck());
}
void CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_5()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_5, m_Device_HotLong_PCB[5].m_CheckPCB_Power.GetCheck());
}
void CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_6()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_6, m_Device_HotLong_PCB[6].m_CheckPCB_Power.GetCheck());
}
void CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_7()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_7, m_Device_HotLong_PCB[7].m_CheckPCB_Power.GetCheck());
}
void CHotLong_PCBDlg::OnBnClickedButtonPCB_Power_8()
{
	SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_8, m_Device_HotLong_PCB[8].m_CheckPCB_Power.GetCheck());
}

CString CreateCurDateDirection(int BoxNr, CTime cur)
{
	


	CString sDir;
	CString s;

	sDir = _T("C:\\data");

	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	sDir = _T("c:\\data\\log");

	//sDir = theAppDirectory + sDir;

	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	s.Format(_T("\\box%d"), BoxNr + 1);

	sDir += s;

	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	s.Format(_T("\\%04d年"), cur.GetYear());
	sDir += s;



	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	s.Format(_T("\\%02d月"), cur.GetMonth());
	sDir += s;



	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	s.Format(_T("\\%02d日"), cur.GetDay());
	sDir += s;



	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	return sDir;
}

CString GetDateDirection(int m_BoxNr, CTime t)
{
	CTime cur = t;

	CString sDir;
	CString s;

	sDir = _T("log");

	sDir = theAppDirectory + sDir;

	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	s.Format(_T("\\box%d"), m_BoxNr + 1);

	sDir += s;

	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	s.Format(_T("\\%04d年"), cur.GetYear());
	sDir += s;



	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	s.Format(_T("\\%02d月"), cur.GetMonth());
	sDir += s;



	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	s.Format(_T("\\%02d日"), cur.GetDay());
	sDir += s;



	if (!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir, NULL);
	}

	return sDir;
}
void CHotLong_PCBDlg::WriteLog(int ID, CString Barcode, CTime BeginTime,int SubMode,int Pv,int Sv)
{
	CFile RecFile;
	CString file_name;
	CString s,msg;
	CreateCurDateDirection(ID, BeginTime);
	file_name = GetDateDirection(ID, BeginTime);
	file_name += BeginTime.Format(_T("\\%H_%M_%S.csv"));
	BOOL FLAG = PathFileExists(file_name);

	if (RecFile.Open(file_name, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
	{
		unsigned short							UNICODE_FILE_HEAR = 0xFEFF;
		unsigned short							CSV_COMMMA = 0x0009;
		RecFile.SeekToEnd();
		if (FLAG == false)
		{
			RecFile.Write(&UNICODE_FILE_HEAR, 2);
			msg = _T("时间");
			RecFile.Write(msg, msg.GetLength() * sizeof(TCHAR));
			RecFile.Write(&CSV_COMMMA, 2);
			msg = _T("工作步骤");
			RecFile.Write(msg, msg.GetLength() * sizeof(TCHAR));
			RecFile.Write(&CSV_COMMMA, 2);
			msg = _T("PV");
			RecFile.Write(msg, msg.GetLength() * sizeof(TCHAR));
			RecFile.Write(&CSV_COMMMA, 2);
			msg = _T("SV");
			RecFile.Write(msg, msg.GetLength() * sizeof(TCHAR));
			RecFile.Write(&CSV_COMMMA, 2);
			s = _T("\r\n");
			RecFile.Write(s, s.GetLength() * sizeof(TCHAR));
		}
		CTime cur = CTime::GetCurrentTime();
		msg = cur.Format(_T("%H:%M:%S"));
		RecFile.Write(msg, msg.GetLength() * sizeof(TCHAR));
		RecFile.Write(&CSV_COMMMA, 2);
		msg.Format(_T("%d"), SubMode);
		RecFile.Write(msg, msg.GetLength() * sizeof(TCHAR));
		RecFile.Write(&CSV_COMMMA, 2);
		msg.Format(_T("%d"), m_Pv[ID]);
		RecFile.Write(msg, msg.GetLength() * sizeof(TCHAR));
		RecFile.Write(&CSV_COMMMA, 2);
		msg.Format(_T("%d"), m_Sv[ID]);
		RecFile.Write(msg, msg.GetLength() * sizeof(TCHAR));
		RecFile.Write(&CSV_COMMMA, 2);
		s = _T("\r\n");
		RecFile.Write(s, s.GetLength() * sizeof(TCHAR));


	}
}

void CHotLong_PCBDlg::WriteLog(int ID, CString Barcode, CTime BeginTime,CString msg)
{
	CFile RecFile;
	CString file_name;
	CString s;
	CreateCurDateDirection(ID, BeginTime);
	file_name = GetDateDirection(ID, BeginTime);
	file_name += BeginTime.Format(_T("\\%H_%M_%S.csv"));
	BOOL FLAG = PathFileExists(file_name);

	if (RecFile.Open(file_name, CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
	{
		unsigned short							UNICODE_FILE_HEAR = 0xFEFF;
		unsigned short							CSV_COMMMA = 0x0009;
		RecFile.SeekToEnd();
		if (FLAG == false)
		{
			RecFile.Write(&UNICODE_FILE_HEAR, 2);
		}
		RecFile.Write(msg, msg.GetLength() * sizeof(TCHAR));
		s = _T("\r\n");
		RecFile.Write(s, s.GetLength() * sizeof(TCHAR));
	

	}
}
void CHotLong_PCBDlg::SetResult(int ID, bool Pass)
{
	CTime tm = CTime::GetCurrentTime(); // 获取当前时间 
	CString s = tm.Format(_T("%H:%M:%S "));
	m_ListMsg[ID].InsertString(0, s + _T("测试结束"));

	if (Pass)
	{
		m_Aging_Pass_Counts++;
		m_Beep_BeginTime = GetTickCount();
		m_Beep_IsPass = true;
		m_Beep_mSecs = 600;
		m_Device_HotLong_PCB[ID].m_Edit_Result.SetWindowTextW(_T("PASS"));
		m_Device_HotLong_PCB[ID].m_Edit_Result.SetTextColor(RGB(0xFF,0xFF,0xFF));
		m_Device_HotLong_PCB[ID].m_Edit_Result.SetBackColor(RGB(0,0xFF,0));
	}
	else
	{
		m_Aging_Fail_Counts++;
		m_Beep_BeginTime = GetTickCount();
		m_Beep_IsPass = false;
		m_Beep_mSecs = 1000;
		m_Device_HotLong_PCB[ID].m_Edit_Result.SetWindowTextW(_T("FAIL"));
		m_Device_HotLong_PCB[ID].m_Edit_Result.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
		m_Device_HotLong_PCB[ID].m_Edit_Result.SetBackColor(RGB(0xFF, 0, 0));
	}
	SavePassFail();
	if(m_ConfigData.m_Pcb_Type == PCB_TYPE_X90
			|| m_ConfigData.m_Pcb_Type == PCB_TYPE_X90_NO_BARCODE)
	{
		m_Device_HotLong_PCB[ID].SetSleepMode(true);
	}
	switch(ID)
	{
	case 1:
		SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_1, false);
		break;
	case 2:
		SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_2, false);
		break;
		case 3:
		SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_3, false);
		break;
		case 4:
		SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_4, false);
		break;
		case 5:
		SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_5, false);
		break;
		case 6:
		SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_6, false);
		break;
		case 7:
		SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_7, false);
		break;
		case 8:
		SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_8, false);
		break;
	}
	m_Device_HotLong_PCB[ID].m_SubMode = 0;
}

void CHotLong_PCBDlg::WorkMode_Program(void)
{
	CTime cur = CTime::GetCurrentTime();
	CTimeSpan span = cur - m_Device_HotLong_PCB[0].m_SubModeBeginTime;
	CString sCur,sSpan,msgLog;
	int iSpan = span.GetTotalSeconds();

	sCur = cur.Format(_T("%H:%M:%S "));
	sSpan.Format(_T("  %d"),iSpan);
	switch (m_Device_HotLong_PCB[0].m_SubMode)
	{
	case PROGRAM_KEY_MODE_IDLE:
		m_Device_HotLong_PCB[0].m_EditMsg = _T("停止中。。。");
		this->UpdateData(false);
		break;
	case PROGRAM_KEY_MODE_NEEDLE_DOWN:
	{
		m_Device_HotLong_PCB[0].m_EditMsg = _T("测试探针下压。。。");

		if (iSpan > 3)
		{
			msgLog = sCur + _T("烧录程序");
			this->m_ListMsg[0].InsertString(0, msgLog);
			WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
			m_Device_HotLong_PCB[0].m_SubMode++;
			m_Device_HotLong_PCB[0].m_SubModeBeginTime = CTime::GetCurrentTime();
			SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID, true);
		}
		else
		{
			this->UpdateData(false);
		}
		m_Device_HotLong_PCB[0].m_EditMsg += sSpan;
		this->UpdateData(false);
	}
	break;
	case PROGRAM_KEY_MODE_PROGRAM:
	{
		m_Device_HotLong_PCB[0].m_EditMsg = _T("程序烧录...");

		if (iSpan > 20)
		{
			 
			m_Device_HotLong_PCB[0].m_EditMsg = _T("程序烧录失败");
			msgLog = sCur + m_Device_HotLong_PCB[0].m_EditMsg;
			this->m_ListMsg[0].InsertString(0, msgLog);
			WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
			this->SetResult_Program(false);
			this->UpdateData(false);
			SetResult_Program(false);
		}
		else if (iSpan > 2)
		{
			
			CString msg;
			this->m_Edit_Degree.GetWindowTextW(msg);
			if (msg.Compare(_T("S no")) == 0)
			{
				m_Device_HotLong_PCB[0].m_EditMsg = _T("没有传感器");
				msgLog = sCur + m_Device_HotLong_PCB[0].m_EditMsg;
				this->m_ListMsg[0].InsertString(0, msgLog);
				WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
				this->SetResult_Program(false);
			}
			else
			{
				
				msg.Trim();
				int val = _ttoi(msg);
				if (val > 0 && val < 100)
				{
					m_Device_HotLong_PCB[0].m_SubMode;
					m_Device_HotLong_PCB[0].m_SubModeBeginTime = CTime::GetCurrentTime();
					SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID, false);
					SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_SET_ID, true);
					msgLog = sCur + _T("设置键下压");
					this->m_ListMsg[0].InsertString(0, msgLog);
					WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
				}
			}
		
		}
		m_Device_HotLong_PCB[0].m_EditMsg += sSpan;
		this->UpdateData(false);
		break;
	}
	case PROGRAM_KEY_MODE_SET_DOWN:
	{//设置键按下
		m_Device_HotLong_PCB[0].m_EditMsg = _T("设置按键。。。");

		if (span.GetTotalSeconds() > 30)
		{
			m_Device_HotLong_PCB[0].m_EditMsg = _T("设置按键失败。。。");
			this->SetResult_Program(false);
			this->UpdateData(false);
			this->SetResult_Program(false);
			msgLog = sCur + _T("设置按键失败");
			this->m_ListMsg[0].InsertString(0, msgLog);
			WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
		}
		else
		{
		
			CString msg;
			this->m_Edit_Degree.GetWindowTextW(msg);
			msg.Trim();
			if (msg.Compare(_T("STA")) == 0)
			{
				m_Device_HotLong_PCB[0].m_SubMode++;
				m_Device_HotLong_PCB[0].m_SubModeBeginTime = CTime::GetCurrentTime();
				SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_SET_ID, false);
				SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID, true);
				msgLog = sCur + _T("减少按键下压");
				this->m_ListMsg[0].InsertString(0, msgLog);
				WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
			}
			this->UpdateData(false);
		}
		m_Device_HotLong_PCB[0].m_EditMsg += sSpan;
		this->UpdateData(false);
	}
	break;
	case PROGRAM_KEY_MODE_MINUS_DOWN:
	{// 减少 按钮上升
		if (span.GetTotalSeconds() > 1)
		{
			CString msg;
			this->m_Edit_Degree.GetWindowTextW(msg);
			m_Device_HotLong_PCB[0].m_SubMode++;
			m_Device_HotLong_PCB[0].m_SubModeBeginTime = CTime::GetCurrentTime();
			SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID, false);
			msgLog = sCur + _T("减少按键抬起");
			this->m_ListMsg[0].InsertString(0, msgLog);
			WriteLog(0, _T(""),m_Device_HotLong_PCB[0]. m_BoxBeginTime, msgLog);
		}

	}
	break;
	case PROGRAM_KEY_MODE_MINUS_UP:
	{// 减少 按钮上升
		if (span.GetTotalSeconds() > 5)
		{
			msgLog = sCur + _T("减少按键抬起失败");
			this->m_ListMsg[0].InsertString(0, msgLog);
			WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
			SetResult_Program(false);
		}
		else
		{
			CString msg;
			this->m_Edit_Degree.GetWindowTextW(msg);
			msg.Trim();
			if (msg.Compare(_T("ID")) == 0)
			{
				m_Device_HotLong_PCB[0].m_SubMode++;
				m_Device_HotLong_PCB[0].m_SubModeBeginTime = CTime::GetCurrentTime();
				SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID, true);
				msgLog = sCur + _T("减少按键再次下压");
				this->m_ListMsg[0].InsertString(0, msgLog);
				WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
			}
			this->UpdateData(false);
		}

	}
	break;
	case PROGRAM_KEY_MODE_MINUS_DOWN_2:
	{// 减少 按钮上升
		if (span.GetTotalSeconds() > 1)
		{
			CString msg;
			this->m_Edit_Degree.GetWindowTextW(msg);
			m_Device_HotLong_PCB[0].m_SubMode++;
			m_Device_HotLong_PCB[0].m_SubModeBeginTime = CTime::GetCurrentTime();
			SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID, false);
			msgLog = sCur + _T("减少按键再次抬起");
			this->m_ListMsg[0].InsertString(0, msgLog);
			WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
		}

	}
	
	break;
	case PROGRAM_KEY_MODE_MINUS_UP_2:
	{// 减少 按钮上升
		if (span.GetTotalSeconds() > 5)
		{
			SetResult_Program(false);
		}
		else
		{
			CString msg;
			this->m_Edit_Degree.GetWindowTextW(msg);
			msg.Trim();
			if (msg.Compare(_T("HTA")) == 0)
			{
				m_Device_HotLong_PCB[0].m_SubMode++;
				m_Device_HotLong_PCB[0].m_SubModeBeginTime = CTime::GetCurrentTime();
				SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID, true);
				msgLog = sCur + _T("增加按键下压");
				this->m_ListMsg[0].InsertString(0, msgLog);
				WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
			}
			this->UpdateData(false);
		}

	}
	break;

	case PROGRAM_KEY_MODE_PLUS_DOWN:
	{
		if (span.GetTotalSeconds() > 1)
		{
			CString msg;
			this->m_Edit_Degree.GetWindowTextW(msg);
			m_Device_HotLong_PCB[0].m_SubMode++;
			m_Device_HotLong_PCB[0].m_SubModeBeginTime = CTime::GetCurrentTime();
			SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID, false);
			msgLog = sCur + _T("增加按键下压");
			this->m_ListMsg[0].InsertString(0, msgLog);
			WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
		}
	}
	break;
	case PROGRAM_KEY_MODE_PLUS_UP:
	{
		if (span.GetTotalSeconds() > 15)
		{
			SetResult_Program(false);
		}
		else
		{
			CString msg;
			this->m_Edit_Degree.GetWindowTextW(msg);
			msg.Trim();
			if (msg.Compare(_T("ID")) == 0)
			{
				m_Device_HotLong_PCB[0].m_SubMode++;
				m_Device_HotLong_PCB[0].m_SubModeBeginTime = CTime::GetCurrentTime();
				SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID, false);
				msgLog = sCur + _T("增加按键抬起");
				this->m_ListMsg[0].InsertString(0, msgLog);
				WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
				SetResult_Program(true);
				

			}
			this->UpdateData(false);
		}
	}
	break;
	
	}
}

void CHotLong_PCBDlg::BeepMode(void)
{
	if (!this->IsWindowVisible())
	{
		return;
	}
	DWORD mSecs = GetTickCount();
	mSecs -= m_Beep_BeginTime;
	if (mSecs > m_Beep_mSecs)
	{
		if (m_CheckPCB_Button_Beep.GetCheck())
		{
			m_CheckPCB_Button_Beep.SetCheck(false);
			SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_BEEP_ID, false);

		}
	}
	else
	{
		if (m_Beep_IsPass)
		{//200ms PULSE
			if (mSecs < 200)
			{
				if (!m_CheckPCB_Button_Beep.GetCheck())
				{
					m_CheckPCB_Button_Beep.SetCheck(true);
					SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_BEEP_ID, true);
				}
			}
			else if (mSecs < 400)
			{
				if (m_CheckPCB_Button_Beep.GetCheck())
				{
					m_CheckPCB_Button_Beep.SetCheck(false);
					SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_BEEP_ID, false);
				}
			}
			else if (mSecs < 600)
			{
				if (!m_CheckPCB_Button_Beep.GetCheck())
				{
					m_CheckPCB_Button_Beep.SetCheck(true);
					SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_BEEP_ID, true);
				}
			}
		}
		else
		{//1000ms
			if (!m_CheckPCB_Button_Beep.GetCheck())
			{
				m_CheckPCB_Button_Beep.SetCheck(true);
				SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_BEEP_ID, true);
			}
		}
	}
	
}
void CHotLong_PCBDlg::WorkMode_X90(int ID)
{
	CString s;
	CTime cur = CTime::GetCurrentTime();
	CTimeSpan span;
	CString sCur = cur.Format(_T("%H:%M:%S "));
	CString msg;
	if (ID == 0)
	{
		WorkMode_Program();
	}
	else if (ID >= 1 && ID <= 8)
	{
		if (m_Device_HotLong_PCB[ID].m_SubMode > 0)
		{
			WriteLog(ID, m_Device_HotLong_PCB[ID].m_BarCode, m_Device_HotLong_PCB[ID].m_BoxBeginTime, m_Device_HotLong_PCB[ID].m_SubMode, m_Pv[ID], m_Sv[ID]);
		}
		switch (m_Device_HotLong_PCB[ID].m_SubMode)
		{
		case 0:
			m_Device_HotLong_PCB[ID].m_EditMsg = _T("停止中。。。");
			this->UpdateData(false);
			break;
		case 1:
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			if (span.GetTotalSeconds() > 420)
			{
				SetResult(ID, false);
			}
			else
			{
				s.Format(_T("7分钟升温到95°,%02d:%02d"), span.GetMinutes(), span.GetSeconds());
				m_Device_HotLong_PCB[ID].m_EditMsg = s;
				this->UpdateData(false);
				if (m_Pv[ID] >= 95)
				{
					m_Device_HotLong_PCB[ID].m_SubModeBeginTime = cur;
					m_Device_HotLong_PCB[ID].m_SubMode ++;
					m_ListMsg[ID].InsertString(0, sCur + _T("等待2分钟,温度:80--120"));
				}
			}
			break;
		case 2:
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			s.Format(_T("等待2分钟，温度范围:80--120，%02d:%02d"),span.GetMinutes(),span.GetSeconds());
			m_Device_HotLong_PCB[ID].m_EditMsg = s;
			this->UpdateData(false);
			if (span.GetTotalSeconds() > 120)
			{
				m_Device_HotLong_PCB[ID].m_SubModeBeginTime = cur;
				m_Device_HotLong_PCB[ID].m_SubMode ++;
				m_ListMsg[ID].InsertString(0, sCur + _T("等待2分钟,温度:100--100"));
			}
			else
			{
				if (m_Pv[ID] < 80 || m_Pv[ID] > 120)
				{
					SetResult(ID, false);
				}
			}

			break;
		case 3:
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			s.Format(_T("恒温2分钟，温度范围:100--100，%02d:%02d"), span.GetMinutes(), span.GetSeconds());
			m_Device_HotLong_PCB[ID].m_EditMsg = s;
			this->UpdateData(false);
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			if (span.GetTotalSeconds() > 120)
			{
				SetSv(ID-1, 90);
				m_Device_HotLong_PCB[ID].m_SubModeBeginTime = cur;
				m_Device_HotLong_PCB[ID].m_SubMode ++;
				m_ListMsg[ID].InsertString(0, sCur + _T("2分钟降到92°"));
			}
			else
			{
				if (m_Pv[ID] < 100 || m_Pv[ID] > 100)
				{
					SetResult(ID, false);
				}
			}
			break;
		case 4:
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			s.Format(_T("2分钟降到92°，%02d:%02d"), span.GetMinutes(), span.GetSeconds());
			m_Device_HotLong_PCB[ID].m_EditMsg = s;
			this->UpdateData(false);
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			if (span.GetTotalSeconds() > 120)
			{
				    SetResult(ID, false);
			}
			else
			{
				if (m_Pv[ID] < 92)
				{
					SetResult(ID, true);
				}
			}
			break;
		}

	}
}
void CHotLong_PCBDlg::WorkMode_BIG_AMP(int ID)
{
	CString s;
	CTime cur = CTime::GetCurrentTime();
	CTimeSpan span;
	CString sCur = cur.Format(_T("%H:%M:%S "));
	CString msg;
	if (ID == 0)
	{
		WorkMode_Program();
	}
	else if (ID >= 1 && ID <= 8)
	{
		if (m_Device_HotLong_PCB[ID].m_SubMode > 0)
		{
			WriteLog(ID, m_Device_HotLong_PCB[ID].m_BarCode, m_Device_HotLong_PCB[ID].m_BoxBeginTime, m_Device_HotLong_PCB[ID].m_SubMode, m_Pv[ID], m_Sv[ID]);
		}
		switch (m_Device_HotLong_PCB[ID].m_SubMode)
		{
		case 0:
			m_Device_HotLong_PCB[ID].m_EditMsg = _T("停止中。。。");
			this->UpdateData(false);
			break;
		case 1:
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			if (span.GetTotalSeconds() > 420)
			{
				SetResult(ID, false);
			}
			else
			{
				s.Format(_T("7分钟升温到95°,%02d:%02d"), span.GetMinutes(), span.GetSeconds());
				m_Device_HotLong_PCB[ID].m_EditMsg = s;
				this->UpdateData(false);
				if (m_Pv[ID] >= 95)
				{
					m_Device_HotLong_PCB[ID].m_SubModeBeginTime = cur;
					m_Device_HotLong_PCB[ID].m_SubMode ++;
					m_ListMsg[ID].InsertString(0, sCur + _T("等待2分钟,温度:80--120"));
				}
			}
			break;
		case 2:
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			s.Format(_T("等待2分钟，温度范围:80--120，%02d:%02d"),span.GetMinutes(),span.GetSeconds());
			m_Device_HotLong_PCB[ID].m_EditMsg = s;
			this->UpdateData(false);
			if (span.GetTotalSeconds() > 120)
			{
				m_Device_HotLong_PCB[ID].m_SubModeBeginTime = cur;
				m_Device_HotLong_PCB[ID].m_SubMode ++;
				m_ListMsg[ID].InsertString(0, sCur + _T("等待2分钟,温度:99--101"));
			}
			else
			{
				if (m_Pv[ID] < 80 || m_Pv[ID] > 120)
				{
					SetResult(ID, false);
				}
			}

			break;
		case 3:
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			s.Format(_T("恒温2分钟，温度范围:99--101，%02d:%02d"), span.GetMinutes(), span.GetSeconds());
			m_Device_HotLong_PCB[ID].m_EditMsg = s;
			this->UpdateData(false);
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			if (span.GetTotalSeconds() > 120)
			{
				SetSv(ID-1, 90);
				m_Device_HotLong_PCB[ID].m_SubModeBeginTime = cur;
				m_Device_HotLong_PCB[ID].m_SubMode ++;
				m_ListMsg[ID].InsertString(0, sCur + _T("2分钟降到92°"));
			}
			else
			{
				if (m_Pv[ID] < 99 || m_Pv[ID] > 101)
				{
					SetResult(ID, false);
				}
			}
			break;
		case 4:
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			s.Format(_T("2分钟降到92°，%02d:%02d"), span.GetMinutes(), span.GetSeconds());
			m_Device_HotLong_PCB[ID].m_EditMsg = s;
			this->UpdateData(false);
			span = cur - m_Device_HotLong_PCB[ID].m_SubModeBeginTime;
			if (span.GetTotalSeconds() > 120)
			{
				    SetResult(ID, false);
			}
			else
			{
				if (m_Pv[ID] < 92)
				{
					SetResult(ID, true);
				}
			}
			break;
		}

	}
}
void CHotLong_PCBDlg::OnEnSetFocusEditBarcode()
{
	m_BarCodeID = 0;
}
void CHotLong_PCBDlg::OnEnSetFocusEditBarcode_1()
{
	m_BarCodeID = 1;
}
void CHotLong_PCBDlg::OnEnSetFocusEditBarcode_2()
{
	m_BarCodeID = 2;
}
void CHotLong_PCBDlg::OnEnSetFocusEditBarcode_3()
{
	m_BarCodeID = 3;
}
void CHotLong_PCBDlg::OnEnSetFocusEditBarcode_4()
{
	m_BarCodeID = 4;
}
void CHotLong_PCBDlg::OnEnSetFocusEditBarcode_5()
{
	m_BarCodeID = 5;
}
void CHotLong_PCBDlg::OnEnSetFocusEditBarcode_6()
{
	m_BarCodeID = 6;
}
void CHotLong_PCBDlg::OnEnSetFocusEditBarcode_7()
{
	m_BarCodeID = 7;
}
void CHotLong_PCBDlg::OnEnSetFocusEditBarcode_8()
{
	m_BarCodeID = 8;
}
void CHotLong_PCBDlg::OnBnClickedButtonProgramStart()
{
	//SetResult_Program(true);
	ClickedButtonStart(0);
	//StartVideo();
	
}

void CHotLong_PCBDlg::OnEnSetFocusEditAging_PASS()
{
	CEdit* e = (CEdit*)this->GetDlgItem(IDC_EDIT_RESULT);
	e->SetFocus();

	CInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if(dlg.m_Input.Compare(_T("147258")) == 0)
		{
			m_Aging_Pass_Counts = 0;
			SavePassFail();
		}
		else
		{
			AfxMessageBox(_T("密码错误！"));
		}
	}
	
	
	e->SetFocus();
}
void CHotLong_PCBDlg::OnEnSetFocusEditAging_FAIL()
{
	CEdit* e = (CEdit*)this->GetDlgItem(IDC_EDIT_RESULT);
	e->SetFocus();

	CInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if(dlg.m_Input.Compare(_T("147258")) == 0)
		{
			m_Aging_Fail_Counts = 0;
			SavePassFail();
		}
		else
			{
				AfxMessageBox(_T("密码错误！"));
			}
	}
	
	
	e->SetFocus();
}
void CHotLong_PCBDlg::OnEnSetFocusEditProgram_PASS()
{
	CEdit* e = (CEdit*)this->GetDlgItem(IDC_EDIT_RESULT);
	e->SetFocus();

	CInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if(dlg.m_Input.Compare(_T("147258")) == 0)
		{
			m_Program_Pass_Counts = 0;
			SavePassFail();
		}
		else
		{
			AfxMessageBox(_T("密码错误！"));
		}
	}
	
	e->SetFocus();
}
void CHotLong_PCBDlg::OnEnSetFocusEditProgram_FAIL()
{
	CEdit* e = (CEdit*)this->GetDlgItem(IDC_EDIT_RESULT);
	e->SetFocus();

	CInputDlg dlg;
	if (dlg.DoModal() == IDOK)
	{
		if(dlg.m_Input.Compare(_T("147258")) == 0)
		{
			m_Program_Fail_Counts = 0;
			SavePassFail();
		}
		else
		{
			AfxMessageBox(_T("密码错误！"));
		}
	}
	
	e->SetFocus();
}
void CHotLong_PCBDlg::OnBnClickedButtonStart_1()
{

	ClickedButtonStart(1);
}
void CHotLong_PCBDlg::OnBnClickedButtonStart_2()
{
	ClickedButtonStart(2);
}
void CHotLong_PCBDlg::OnBnClickedButtonStart_3()
{
	ClickedButtonStart(3);
}
void CHotLong_PCBDlg::OnBnClickedButtonStart_4()
{
	ClickedButtonStart(4);
}
void CHotLong_PCBDlg::OnBnClickedButtonStart_5()
{
	ClickedButtonStart(5);
}
void CHotLong_PCBDlg::OnBnClickedButtonStart_6()
{
	ClickedButtonStart(6);
}
void CHotLong_PCBDlg::OnBnClickedButtonStart_7()
{
	ClickedButtonStart(7);
}
void CHotLong_PCBDlg::OnBnClickedButtonStart_8()
{
	ClickedButtonStart(8);
}

void CHotLong_PCBDlg::SavePassFail()
{
	m_ConfigData.m_Program_Pass_Counts = m_Program_Pass_Counts;
	m_ConfigData.m_Program_Fail_Counts = m_Program_Fail_Counts;
	m_Program_Total_Counts = m_Program_Pass_Counts + m_Program_Fail_Counts;
	if (m_Program_Total_Counts > 0)
	{
		m_Program_Percent = (m_Program_Pass_Counts * 100.0f) / m_Program_Total_Counts;
	}
	else
	{
		m_Program_Percent = 0;
	}

	m_ConfigData.m_Aging_Pass_Counts = m_Aging_Pass_Counts;
	m_ConfigData.m_Aging_Fail_Counts = m_Aging_Fail_Counts;
	m_Aging_Total_Counts = m_Aging_Pass_Counts + m_Aging_Fail_Counts;
	if (m_Aging_Total_Counts > 0)
	{
		m_Aging_Percent = (m_Aging_Pass_Counts * 100.0f) / m_Aging_Total_Counts;
	}
	else
	{
		m_Aging_Percent = 0;
	}

	SaveConfig();

	this->UpdateData(false);
}

void CHotLong_PCBDlg::SetResult_Program(bool pass)
{
	CTime tm = CTime::GetCurrentTime(); // 获取当前时间 
	CString s = tm.Format(_T("%H:%M:%S "));
	CString msgLog;
	this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_SET_ID, false);
	this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID, false);
	this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_TEST_NEEDLE_ID, false);
	this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID, false);
	this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID, false);
	msgLog = s + _T("测试结束");
	m_ListMsg[0].InsertString(0, msgLog);
	WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, msgLog);
	m_Device_HotLong_PCB[0].m_SubMode = 0;
	if (pass)
	{
		m_Program_Pass_Counts++;

		m_Device_HotLong_PCB[0].m_Edit_Result.SetWindowTextW( _T("PASS"));
		m_Device_HotLong_PCB[0].m_Edit_Result.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
		m_Device_HotLong_PCB[0].m_Edit_Result.SetBackColor(RGB(0, 0xFF, 0));
		WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, _T("PASS"));
	}
	else
	{
		m_Program_Fail_Counts++;

		m_Device_HotLong_PCB[0].m_Edit_Result.SetWindowTextW(_T("FAIL"));
		m_Device_HotLong_PCB[0].m_Edit_Result.SetTextColor(RGB(0xFF, 0xFF, 0xFF));
		m_Device_HotLong_PCB[0].m_Edit_Result.SetBackColor(RGB(0xFF, 0, 0));
		WriteLog(0, _T(""), m_Device_HotLong_PCB[0].m_BoxBeginTime, _T("FAIL"));
	}
	SavePassFail();
	this->UpdateData(false);
}
void CHotLong_PCBDlg::ClickedButtonStart(int ButtonID)
{
	
	CTime tm = CTime::GetCurrentTime(); // 获取当前时间 
	CString s = tm.Format(_T("%H:%M:%S "));
	CString msg;
#ifndef _DEBUG
	if(this->m_Device_HuiKong_DIO.m_hComm == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("没有找到 PLC 对应的通信口!"));
		return;
	}
	
	CTimeSpan ts = tm - m_Device_HuiKong_DIO.m_LastRecTime;
	if(ts.GetTotalSeconds() > 10)
	{
		AfxMessageBox(_T("PLC 没有通信!"));
		return;
	}


	if(this->m_Device_HotLong_PCB[ButtonID].m_hComm == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(_T("没有找到 对应的通信口!"));
		return;
	}
#endif
	if(m_ConfigData.m_Pcb_Type == PCB_TYPE_BIG_AMP
		|| m_ConfigData.m_Pcb_Type == PCB_TYPE_X90
		|| m_ConfigData.m_Pcb_Type == PCB_TYPE_X10)
	{
		CBarcodeDlg dlg;
		if(dlg.DoModal() == IDOK)
		{
			m_Device_HotLong_PCB[ButtonID].m_BarCode = dlg.m_Barcode;
			this->UpdateData(false);
		}
		else
		{
			return;
		}
	}
	if (ButtonID == 0)
	{
		switch (m_Device_HotLong_PCB[ButtonID].m_SubMode)
		{
		case 0:
			m_Device_HotLong_PCB[ButtonID].m_BoxBeginTime = tm;
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_SET_ID, false);
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID, false);
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID, false);
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID, false);
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_TEST_NEEDLE_ID, true);
			m_Device_HotLong_PCB[ButtonID].m_SubModeBeginTime = tm;
			m_Device_HotLong_PCB[ButtonID].m_Edit_Result.SetWindowTextW(_T(""));
			m_Device_HotLong_PCB[ButtonID].m_Edit_Result.SetBackColor(RGB(0xFF, 0xFF, 0));

			this->m_ListMsg[0].ResetContent();
			msg = s + _T("测试开始");
			m_ListMsg[ButtonID].InsertString(0, msg);
			WriteLog(ButtonID, _T(""), m_Device_HotLong_PCB[ButtonID].m_BoxBeginTime, msg);
			msg = s + _T("测试探针按键下压");
			this->m_ListMsg[0].InsertString(0,msg );
			WriteLog(ButtonID, _T(""), m_Device_HotLong_PCB[ButtonID].m_BoxBeginTime, msg);
			this->UpdateData(false);
			m_Device_HotLong_PCB[ButtonID].m_SubMode = 1;
			break;
		default:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_SET_ID, false);
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID, false);
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_TEST_NEEDLE_ID, false);
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID, false);
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID, false);
			m_ListMsg[ButtonID].InsertString(0, s + _T("断电"));
			m_ListMsg[ButtonID].InsertString(0, s + _T("测试结束"));
			m_Device_HotLong_PCB[ButtonID].m_SubMode = 0;
			break;
		}
		return ;
	}

	switch(m_Device_HotLong_PCB[ButtonID].m_SubMode)
	{
	case 0:
		m_Device_HotLong_PCB[ButtonID].m_BoxBeginTime = tm;
		m_ListMsg[ButtonID].ResetContent();
		m_ListMsg[ButtonID].InsertString(0, s + _T("测试开始"));
		switch(ButtonID)
		{
		case 1:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_1, true);
			break;
		case 2:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_2, true);
			break;
		case 3:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_3, true);
			break;
		case 4:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_4, true);
			break;
		case 5:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_5, true);
			break;
		case 6:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_6, true);
			break;
		case 7:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_7, true);
			break;
		case 8:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_8, true);
			break;

		}


		m_ListMsg[ButtonID].InsertString(0, s + _T("上电"));

		m_Device_HotLong_PCB[ButtonID].m_SubModeBeginTime = tm;
		m_Pv[ButtonID] = 0;
		m_Sv[ButtonID] = 0;
		m_Device_HotLong_PCB[ButtonID].m_Edit_Result.SetWindowTextW( _T(""));
		m_Device_HotLong_PCB[ButtonID].m_Edit_Result.SetBackColor(RGB(0xFF, 0xFF, 0));
		SetSv(ButtonID - 1, 100);
		if(m_ConfigData.m_Pcb_Type == PCB_TYPE_X90
			|| m_ConfigData.m_Pcb_Type == PCB_TYPE_X90_NO_BARCODE)
		{
			m_Device_HotLong_PCB[ButtonID].SetSleepMode(false);
		}
		m_ListMsg[ButtonID].InsertString(0, s + _T("7分钟升到95°"));
		this->UpdateData(false);
	    m_Device_HotLong_PCB[ButtonID].m_SubMode = 1;
		break;
	default:
		switch(ButtonID)
		{
		case 1:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_1, false);
			break;
		case 2:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_2, false);
			break;
		case 3:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_3, false);
			break;
		case 4:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_4, false);
			break;
		case 5:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_5, false);
			break;
		case 6:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_6, false);
			break;
		case 7:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_7, false);
			break;
		case 8:
			this->SetRelay(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_8, false);
			break;

		}
		SetResult(ButtonID, false);
		m_ListMsg[ButtonID].InsertString(0, s + _T("断电"));
		m_ListMsg[ButtonID].InsertString(0, s+_T("测试结束"));
		m_Device_HotLong_PCB[ButtonID].m_SubMode = 0;
		this->UpdateData(false);
		break;
	}

}


int CHotLong_PCBDlg::ReadExcelFile()
{
	// TODO: 在此处添加实现代码.
	strExcleFilePath = strWorkDir + _T("\\ExcelTest.xlsx");  //exe所在路径当前路径下的Excel文件
	CApplication app;
	CWorkbooks books;
	CWorkbook book;
	CWorksheets sheets;
	CWorksheet sheet;
	CRange range;
	LPDISPATCH lpDisp;
	COleVariant vResult;
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	if (!app.CreateDispatch(_T("Excel.Application")))
	{
		MessageBox(_T("Error!Creat Excel Application Server Faile!"));
		return -1;
	}
	books = app.get_Workbooks();   //获取工作薄集合

	lpDisp = books.Open(strExcleFilePath,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional, covOptional, covOptional,
		covOptional, covOptional);
	book.AttachDispatch(lpDisp);
	//book = books.Add(covOptional); //获取当前工作薄,若使用此语句，则为新建一个EXCEL表
	sheets = book.get_Worksheets(); //获取当前工作薄页的集合
	sheet = sheets.get_Item(COleVariant((short)1)); //获取当前活动页,第一页sheet1

	//1.获取数据1
	range = sheet.get_Range(COleVariant(_T("A1")), COleVariant(_T("A1"))); //获取单元格
	vResult = COleVariant(range.get_Value2());
	strExcel1 = CString(vResult.bstrVal);

	//1.获取数据2
	range = sheet.get_Range(COleVariant(_T("B1")), COleVariant(_T("B1"))); //获取单元格
	vResult = COleVariant(range.get_Value2());
	strExcel2 = CString(vResult.bstrVal);
	MessageBox(strExcel1);
	MessageBox(strExcel2);

	//释放各对象，注意其顺序，若不执行以下步骤，Excel进程无法退出，打开任务管理器将会看到残留进程
	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch(); //释放当前工作薄
	books.ReleaseDispatch(); //释放工作薄集
	app.Quit(); //退出EXCEL程序
	app.ReleaseDispatch(); //释放EXCEL程序
	return 0;
}



int CHotLong_PCBDlg::GetWorkDir()

{

	// TODO: 在此处添加实现代码.用于获取程序所在路径

	TCHAR pFileName[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, pFileName);
	CString dir(pFileName);
	strWorkDir = dir;


	return 0;

}

void CHotLong_PCBDlg::Excel_Test()

{
	GetWorkDir();

	// TODO: 在此添加控件通知处理程序代码
	strOutputExcleFilePath = strWorkDir + _T("\\Result.xlsx");  //指定要写入的文件名及其路径，若存在则覆盖

	CApplication app;
	CWorkbooks books;
	CWorkbook book;
	CWorksheets sheets;
	CWorksheet sheet;
	CRange range;
	COleVariant vResult;
	CString strTemp;
	COleVariant
		covTrue((short)TRUE),
		covFalse((short)FALSE),
		covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	if (!app.CreateDispatch(_T("Excel.Application")))
	{
		MessageBox(_T("Error!Creat Excel Application Server Faile!"));
		return;
	}

	books = app.get_Workbooks();   //获取工作薄集合

	book = books.Add(covOptional); //获取当前工作薄,若使用此语句，则为新建一个EXCEL表
	sheets = book.get_Worksheets(); //获取当前工作薄页的集合
	sheet = sheets.get_Item(COleVariant((short)1)); //获取当前活动页

	//写入从ini文件中读取的键值
	range = sheet.get_Range(COleVariant(_T("A1")), COleVariant(_T("A1"))); //获取单元格
	strTemp.Format(_T("%s"), strINI1);//从ini文件中读取到的数据，需要用CString::Format()再次将字符串格式化一下才能正确写入到Excel中
	range.put_Value2(COleVariant(_T("strTemp"))); //将内容填入单元格
	range = sheet.get_Range(COleVariant(_T("A2")), COleVariant(_T("A2"))); //获取单元格
	strTemp.Format(_T("%s"), strINI2);
	range.put_Value2(COleVariant(strTemp));
	range = sheet.get_Range(COleVariant(_T("A3")), COleVariant(_T("A3"))); //获取单元格
	strTemp.Format(_T("%s"), strINI3);
	range.put_Value2(COleVariant(strTemp));
	range = sheet.get_Range(COleVariant(_T("A4")), COleVariant(_T("A4"))); //获取单元格
	strTemp.Format(_T("%s"), strINI4);
	range.put_Value2(COleVariant(strTemp));

	//写入从Excle表中读取的数据
	range = sheet.get_Range(COleVariant(_T("A5")), COleVariant(_T("A5"))); //获取单元格
	range.put_Value2(COleVariant(strExcel1));
	range = sheet.get_Range(COleVariant(_T("A6")), COleVariant(_T("A6"))); //获取单元格
	range.put_Value2(COleVariant(strExcel2));

	book.SaveCopyAs(COleVariant(strOutputExcleFilePath));
	book.put_Saved(true);
	app.put_Visible(FALSE); //FALSE表示操作完，不打开工作表，若此处为FALSE，则一定需要调用app.Quit(),以避免程序结束后，还有EXCEL进程

	range.ReleaseDispatch();
	sheet.ReleaseDispatch();
	sheets.ReleaseDispatch();
	book.ReleaseDispatch(); //释放当前工作薄
	books.ReleaseDispatch(); //释放工作薄集
	app.Quit(); //退出EXCEL程序
	app.ReleaseDispatch(); //释放EXCEL程序

}

void CHotLong_PCBDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	

}

void CHotLong_PCBDlg::OnTimer(UINT_PTR nIDEvent)
{//50ms

	

	static int Timer;
	Timer++;
	//BeepMode();

	if (Timer == 4)
	{//200ms
		for (int i = 0; i < 9; i++)
		{
			switch(m_ConfigData.m_Pcb_Type)
			{
			case PCB_TYPE_BIG_AMP:
			case PCB_TYPE_BIG_AMP_NO_BARCODE:
				WorkMode_BIG_AMP(i);
				break;
			case PCB_TYPE_X90:
			case PCB_TYPE_X90_NO_BARCODE:
				WorkMode_X90(i);
				break;
			}

			TimerSend(i);

		}
		TimerSend_PLC();
		Timer = 0;
		StartVideo();
	}

	

	CDialog::OnTimer(nIDEvent);
}

void CHotLong_PCBDlg::SetRelay(int ID,bool On)
{
	
	MODBUS_RS485_CMD_DATA_LEN_1 cmd;

	cmd.head.RS485_Addr = 1;
	cmd.head.cmd		= 6;
	cmd.head.Addr_H		= ID / 256;
	cmd.head.Addr_L		= ID % 256;
	cmd.data[0]			= 0;
	if (On)
	{
		cmd.data[1] = 1;
	}
	else
	{
		cmd.data[1] = 0;
	}

	cmd.CRC = CComm::CRC16_MODBUS((unsigned char*)&cmd, sizeof(cmd) - 2);

	MODBUS_RS485_CMD_ALL all;
	all.device_ID		= DEVICE_ID_HUIKONG_DIO;
	all.data_size	= sizeof(cmd);
	memcpy(&all.RS485_Addr, &cmd, sizeof(cmd));

	m_Device_HuiKong_DIO.AppendWaitCmd(all);

}

void CHotLong_PCBDlg::TimerSend_PLC()
{
	
	if(m_Device_HuiKong_DIO.m_hComm == INVALID_HANDLE_VALUE)
	{
		return;
	}
	m_Device_HuiKong_DIO.SetBaudrate(&m_Device_HuiKong_DIO);//.m_Comm);




	if(m_Device_HuiKong_DIO.SendWaitCmd(&m_Device_HuiKong_DIO)  == 0 )
	{
		m_Device_HuiKong_DIO.Read_Y(&m_Device_HuiKong_DIO,0,16);
	}
	this->m_ParaDlg.DisplaySend_PLC(m_Device_HuiKong_DIO.m_LastSendCmd);

	CTime cur = CTime::GetCurrentTime();
	CTimeSpan ts = cur - m_Device_HuiKong_DIO.m_LastRecTime;
	if(ts.GetTotalSeconds() >= 10)
	{
		CString s;
		s = cur.Format(_T("%H:%M:%S"));
		s += _T("  PLC 无响应");
		for(int i = 0; i < 9; i++)
		{
			m_ListMsg[i].AddString(s);
		}
		m_Device_HuiKong_DIO.m_LastRecTime = CTime::GetCurrentTime();
	}

}
void CHotLong_PCBDlg::TimerSend(int CommNr)
{

	if(m_Device_HotLong_PCB[CommNr].m_Comm->m_hComm == INVALID_HANDLE_VALUE)
	{
		return;
	}

	m_Device_HotLong_PCB[CommNr].SetBaudrate(m_Device_HotLong_PCB[CommNr].m_Comm);

	if(m_Device_HotLong_PCB[CommNr].SendWaitCmd(&m_Device_HuiKong_DIO)  == 0 )
	{
		m_Device_HotLong_PCB[CommNr].ReadData();
	}

	this->m_ParaDlg.DisplaySend_hotLong(CommNr,m_Device_HotLong_PCB[CommNr].m_Comm->m_LastSendCmd);

	CTime cur = CTime::GetCurrentTime();
	CTimeSpan ts = cur - m_Device_HotLong_PCB[CommNr].m_Comm->m_LastRecTime;
	if(ts.GetTotalSeconds() >= 10)
	{
		CString s;
		s = cur.Format(_T("%H:%M:%S"));
		s += _T("  线路板 无响应");
		
		m_ListMsg[CommNr].AddString(s);
		
		m_Device_HotLong_PCB[CommNr].m_Comm->m_LastRecTime = CTime::GetCurrentTime();
	}
	
}

void CHotLong_PCBDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Break = true; 
	CDialog::OnCancel();
}

void CHotLong_PCBDlg::StartVideo(void)
{
	// TODO: 在此添加控件通知处理程序代码
	static HWND hWnd = NULL;
	if (hWnd != NULL)
	{
		return;
	}
	
	if(!this->IsWindowVisible())
	{
		return;
	}

	CWnd* pWnd;
	pWnd = GetDlgItem(IDC_STATIC_SCREEN);    //获取控件指针，IDC_BUTTON1为控件ID号

//	while (1)
	{
		
		//		
				//HDC hdc = ::GetDC(hWnd);
				//CDC *picDC = CDC::FromHandle(hdc);//pWnd->GetDC();
		CDC* picDC = pWnd->GetDC();
		CPen whitePen, * pOldPen; //OldPen用来保存原指针
		whitePen.CreatePen(0, 1, RGB(0, 255, 0));//创建画笔，（样式，宽度，颜色）
		pOldPen = picDC->SelectObject(&whitePen);//选入上下文
		if (m_CheckFocus.GetCheck())
		{
			for (int i = 0; i < 4; i++)
			{
				picDC->MoveTo(m_ConfigData.m_LED_NUM_Left[i], m_ConfigData.m_LED_NUM_Top[i]);
				picDC->LineTo(m_ConfigData.m_LED_NUM_Right[i], m_ConfigData.m_LED_NUM_Top[i]);//当前位置保持在P1位置
				picDC->LineTo(m_ConfigData.m_LED_NUM_Right[i], m_ConfigData.m_LED_NUM_Bottom[i]);//当前位置保持在P1位置
				picDC->LineTo(m_ConfigData.m_LED_NUM_Left[i], m_ConfigData.m_LED_NUM_Bottom[i]);//当前位置保持在P1位置
				picDC->LineTo(m_ConfigData.m_LED_NUM_Left[i], m_ConfigData.m_LED_NUM_Top[i]);//当前位置保持在P1位置
			}

			this->UpdateData();

			picDC->MoveTo(m_ConfigData.m_LED_HEAT_Left, m_ConfigData.m_LED_HEAT_Top);
			picDC->LineTo(m_ConfigData.m_LED_HEAT_Right, m_ConfigData.m_LED_HEAT_Top);//当前位置保持在P1位置
			picDC->LineTo(m_ConfigData.m_LED_HEAT_Right, m_ConfigData.m_LED_HEAT_Bottom);//当前位置保持在P1位置
			picDC->LineTo(m_ConfigData.m_LED_HEAT_Left, m_ConfigData.m_LED_HEAT_Bottom);//当前位置保持在P1位置
			picDC->LineTo(m_ConfigData.m_LED_HEAT_Left, m_ConfigData.m_LED_HEAT_Top);//当前位置保持在P1位置

			picDC->MoveTo(m_ConfigData.m_LED_COOL_Left, m_ConfigData.m_LED_COOL_Top);
			picDC->LineTo(m_ConfigData.m_LED_COOL_Right, m_ConfigData.m_LED_COOL_Top);//当前位置保持在P1位置
			picDC->LineTo(m_ConfigData.m_LED_COOL_Right, m_ConfigData.m_LED_COOL_Bottom);//当前位置保持在P1位置
			picDC->LineTo(m_ConfigData.m_LED_COOL_Left, m_ConfigData.m_LED_COOL_Bottom);//当前位置保持在P1位置
			picDC->LineTo(m_ConfigData.m_LED_COOL_Left, m_ConfigData.m_LED_COOL_Top);//当前位置保持在P1位置

			picDC->MoveTo(m_ConfigData.m_LED_COLOR_Left, m_ConfigData.m_LED_COLOR_Top);
			picDC->LineTo(m_ConfigData.m_LED_COLOR_Right, m_ConfigData.m_LED_COLOR_Top);//当前位置保持在P1位置
			picDC->LineTo(m_ConfigData.m_LED_COLOR_Right, m_ConfigData.m_LED_COLOR_Bottom);//当前位置保持在P1位置
			picDC->LineTo(m_ConfigData.m_LED_COLOR_Left, m_ConfigData.m_LED_COLOR_Bottom);//当前位置保持在P1位置
			picDC->LineTo(m_ConfigData.m_LED_COLOR_Left, m_ConfigData.m_LED_COLOR_Top);//当前位置保持在P1位置



		}

		picDC->SelectObject(pOldPen);//恢复上下文
	
		SYSTEMTIME t1,t2;
		GetLocalTime(&t1);
		if(m_ConfigData.m_Pcb_Type == PCB_TYPE_BIG_AMP
			|| m_ConfigData.m_Pcb_Type == PCB_TYPE_BIG_AMP_NO_BARCODE)
		{
			Led_Num();
		}
		Led_Heat();
		Led_Cool();
		Led_Color();

			GetLocalTime(&t2);
	
	int t = t2.wMilliseconds-t1.wMilliseconds;
	if(t < 0)
	{
		t += 1000;
	}
	CString s;
	CString text = _T("江阴辉龙线路板测试系统 编译时间:");
	text += __DATE__;
	text += _T(" ");
	text += __TIME__;

	
	s.Format(_T(" 图像处理时间 %dms"),t);
	text += s;
	//AfxMessageBox(s);
	this->SetWindowTextW(text);
	}
	
	hWnd = NULL;
	
}

void CHotLong_PCBDlg::Take_Screenshot_To_File(CImage *source_image,int Left,int Top,int Right,int Bottom,CString fileName )
{
	
	int width = Right - Left;
	int height = Bottom - Top;
	
	
	source_image->Create(width,height , 24);
	CImageDC  imageDC(*source_image);
	CWnd* pic = this->GetDlgItem(IDC_STATIC_SCREEN);

	CClientDC pic_dc(pic);//只截对话框，用这句


	::BitBlt(imageDC, 0, 0, width, height, pic_dc, Left, Top, SRCCOPY);
	//source_image->Save(fileName);
	

}

void CHotLong_PCBDlg::ShowLedNumPicture(CImage *Image,int ID)
{
	
	
	CRect led_num_pic_rect;
	CWnd* pWnd;
	switch (ID)
	{
	case 0:
	default:
		pWnd = GetDlgItem(IDC_PIC_LED_NUM_1);
		break;
	case 1:
		pWnd = GetDlgItem(IDC_PIC_LED_NUM_2);
		break;
	case 2:
		pWnd = GetDlgItem(IDC_PIC_LED_NUM_3);
		break;
	case 3:
		pWnd = GetDlgItem(IDC_PIC_LED_NUM_4);
		break;
	}

	pWnd->GetClientRect(led_num_pic_rect);

	CDC* pDc = pWnd->GetDC();

	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

	//画图（以下两种方法都可）
	//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
	
	

	
	int h = Image->GetHeight();
	int w = Image->GetWidth();


	int result;

	bool a1, a2, a3, a4, a5, a6, a7, dot, space1，space2;



	//	 0-0.3 0.35--0.65 0.7--1.0w
	//		      a1				    0.2h
	//    a2    space_1      a3			0.2h
	//            a4				    0.2h
	//	  a5    space_2      a6			0.2h
	//            a7				    0,2h
	if(this->m_CheckFocus.GetCheck())
	{
		CRect Rect_a1(0.40 * w, 0.00 * h, 0.70 * w, 0.20 * h);
		CRect Rect_a2(0.10 * w, 0.20 * h, 0.40 * w, 0.40 * h);
		CRect Rect_a3(0.70 * w, 0.20 * h, 1.00 * w-1, 0.40 * h);
		CRect Rect_a4(0.35 * w, 0.40 * h, 0.65 * w, 0.60 * h);
		CRect Rect_a5(0.00 * w, 0.60 * h, 0.30 * w, 0.80 * h);
		CRect Rect_a6(0.60 * w, 0.60 * h, 0.90 * w-1, 0.80 * h);
		CRect Rect_a7(0.25 * w, 0.80 * h, 0.55 * w, 1.00 * h-1);
		CRect Rect_dot(0.90 * w, 0.80 * h, 1.00 * w, 1.00 * h-1);

		CRect Rect_space1(0.45 * w, 0.20 * h, 0.55 * w, 0.40 * h);
		CRect Rect_space2(0.45 * w, 0.60 * h, 0.55 * w, 0.80 * h);

		
		
		ImageDrawRect(Image, Rect_a1);
		ImageDrawRect(Image, Rect_a2);
		ImageDrawRect(Image, Rect_a3);
		ImageDrawRect(Image, Rect_a4);
		ImageDrawRect(Image, Rect_a5);
		ImageDrawRect(Image, Rect_a6);
		ImageDrawRect(Image, Rect_a7);
	}
	
	Image->Draw(pDc->m_hDC, led_num_pic_rect);                //将图片绘制到Picture控件表示的矩形区域

	
pWnd->ReleaseDC(pDc);
	//read_led_main();

}

void CHotLong_PCBDlg::SaveOnlyRedColor(CImage* image, unsigned int ColorBits, unsigned int ColorMinVal)
{
	int width = image->GetWidth();
	int height = image->GetHeight();
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			COLORREF c = image->GetPixel(x, y);

			int c_r, c_g, c_b;
			 c_r = R_BITS & c;
			 c_g = G_BITS & c;
			 c_b = B_BITS & c;
	
			if (c_r > R_BITS_MIN
				&& c_g > G_BITS_MIN
				&& c_b > B_BITS_MIN)
			{
				image->SetPixel(x, y, 0x00FFFFFF);
			}
			else
			{
			//	image->SetPixel(x, y, 0);
			}

		}
	}

}

void CHotLong_PCBDlg::ImageDrawRect(CImage *image,CRect rect)
{
	for (int x = rect.left; x < rect.right; x++)
	{
		image->SetPixel(x, rect.top, RGB(255, 0, 0));
		image->SetPixel(x, rect.bottom, RGB(255, 0, 0));
	}
	for (int x = rect.top; x < rect.bottom; x++)
	{
		image->SetPixel(rect.left, x, RGB(255, 0, 0));
		image->SetPixel(rect.right, x, RGB(255, 0, 0));
	}
}

bool CHotLong_PCBDlg::center(CImage * image, CRect rect)
{	

/*	for (int x = rect.left; x < rect.right; x++)
	{
		for (int y = rect.top; y < rect.bottom; y++)
		{
			COLORREF c = image->GetPixel(x, y);

			int c_r, c_g, c_b;
			 c_r = R_BITS & c;
			 c_g = G_BITS & c;
			 c_b = B_BITS & c;
	
			if (c_r > R_BITS_MIN
				&& c_g > G_BITS_MIN
				&& c_b > B_BITS_MIN)
			{
				image->SetPixel(x, y, 0x00FFFFFF);
			}
			else
			{
		//		image->SetPixel(x, y, 0);
			}

		}
	}
*/

	int x = rect.left + rect.Width() / 2;
	int y = rect.top + rect.Height() / 2;
	COLORREF c = image->GetPixel(x, y);
	
	

	int counts = 0;
	for (int i = 0; i < x; i++)
	{
		for (int j = 0; j < y; j++)
		{
			if (image->GetPixel(x, y) == RGB(255, 255, 255))
			{
				counts++;
			}
		}
	}

	if (counts * 2 > (x * y))
	{
		return true;
	}

	/*if (image->GetPixel(x, y) == RGB(255, 255, 255))
		return true;
	*/
	return false;
}
CString  CHotLong_PCBDlg::GetLedChar(int ID, CImage * image)
{
	int h = image->GetHeight();
	int w = image->GetWidth();


	int result;

	bool a1, a2, a3, a4, a5, a6, a7, dot, space1，space2;



	//	 0-0.3 0.35--0.65 0.7--1.0w
	//		      a1				    0.2h
	//    a2    space_1      a3			0.2h
	//            a4				    0.2h
	//	  a5    space_2      a6			0.2h
	//            a7				    0,2h
	CRect Rect_a1(0.40 * w, 0.00 * h, 0.70 * w, 0.20 * h);
	CRect Rect_a2(0.10 * w, 0.20 * h, 0.40 * w, 0.40 * h);
	CRect Rect_a3(0.70 * w, 0.20 * h, 1.00 * w-1, 0.40 * h);
	CRect Rect_a4(0.35 * w, 0.40 * h, 0.65 * w, 0.60 * h);
	CRect Rect_a5(0.00 * w, 0.60 * h, 0.30 * w, 0.80 * h);
	CRect Rect_a6(0.60 * w, 0.60 * h, 0.90 * w-1, 0.80 * h);
	CRect Rect_a7(0.25 * w, 0.80 * h, 0.55 * w, 1.00 * h-1);
	CRect Rect_dot(0.90 * w, 0.80 * h, 1.00 * w, 1.00 * h-1);

	CRect Rect_space1(0.45 * w, 0.20 * h, 0.55 * w, 0.40 * h);
	CRect Rect_space2(0.45 * w, 0.60 * h, 0.55 * w, 0.80 * h);
/*
	m_Rect_a1[ID] = Rect_a1;
	m_Rect_a2[ID] = Rect_a2;
	m_Rect_a3[ID] = Rect_a3;
	m_Rect_a4[ID] = Rect_a4;
	m_Rect_a5[ID] = Rect_a5;
	m_Rect_a6[ID] = Rect_a6;
	m_Rect_a7[ID] = Rect_a7;
*/	
	ImageDrawRect(image, Rect_a1);
	ImageDrawRect(image, Rect_a2);
	ImageDrawRect(image, Rect_a3);
	ImageDrawRect(image, Rect_a4);
	ImageDrawRect(image, Rect_a5);
	ImageDrawRect(image, Rect_a6);
	ImageDrawRect(image, Rect_a7);
	image->Save(_T("c:\\work\\led_rect.bmp"));
	

	CRect temp_rect;
	

	temp_rect = Rect_a1;
	if (center(image, temp_rect))
		a1 = true;
	else
		a1 = false;
	temp_rect = Rect_a2;
	if (center(image, temp_rect))
		a2 = true;
	else
		a2 = false;
	temp_rect = Rect_a3;
	if (center(image, temp_rect))
		a3 = true;
	else
		a3 = false;
	temp_rect = Rect_a4;
	if (center(image, temp_rect))
		a4 = true;
	else
		a4 = false;
	temp_rect = Rect_a5;
	if (center(image, temp_rect))
		a5 = true;
	else
		a5 = false;
	temp_rect = Rect_a6;
	if (center(image, temp_rect))
		a6 = true;
	else
		a6 = false;

	temp_rect = Rect_a7;
	if (center(image, temp_rect))
		a7 = true;
	else
		a7 = false;

	if (a1 == true && a2 == false && a3 == false && a4 == true && a5 == false && a6 == false && a7 == true)
	{//
		return _T("S");
	}
	else if (a1 == false && a2 == false && a3 == false && a4 == true && a5 == true && a6 == true && a7 == false)
	{
		return _T("n");
	}
	else if (a1 == false && a2 == false && a3 == false && a4 == true && a5 == true && a6 == true && a7 == true)
	{
		return _T("o");
	}
	
	else if (a1 == true && a2 == true && a3 == true && a4 == false && a5 == true && a6 == true && a7 == true)
	{
		return _T("0");
	}
	else if (a1 == false && a2 == false && a3 == true && a4 == false && a5 == false && a6 == true && a7 == false)
	{
		return _T("1");
	}
	else if (a1 == true && a2 == false && a3 == true && a4 == true && a5 == true && a6 == false && a7 == true)
	{
		return _T("2");
	}
	else if (a1 == true && a2 == false && a3 == true && a4 == true && a5 == false && a6 == true && a7 == true)
	{
		return _T("3");
	}
	else if (a1 == false && a2 == true && a3 == true && a4 == true && a5 == false && a6 == true && a7 == false)
	{
		return _T("4");
	}
	else if (a1 == true && a2 == true && a3 == false && a4 == true && a5 == false && a6 == true && a7 == true)
	{
		return _T("5");
	}
	else if (a1 == true && a2 == true && a3 == false && a4 == true && a5 == true && a6 == true && a7 == true)
	{
		return _T("6");
	}
	else if (a1 == true && a2 == false && a3 == true && a4 == false && a5 == false && a6 == true && a7 == false)
	{
		return _T("7");
	}
	else if (a1 == true && a2 == true && a3 == true && a4 == true && a5 == true && a6 == true && a7 == true)
	{
		return _T("8");
	}
	else if (a1 == true && a2 == true && a3 == true && a4 == true && a5 == false && a6 == true && a7 == true)
	{
		return _T("9");
	}
	else if (a1 == true && a2 == true && a3 == true && a4 == true && a5 == true && a6 == true && a7 == false)
	{
		return _T("A");
	}
	else if (a1 == false && a2 == true && a3 == false && a4 == true && a5 == true && a6 == false && a7 == true)
	{
		return _T("T");
	}
	else if (a1 == true && a2 == true && a3 == true && a4 == true && a5 == true && a6 == false && a7 == false)
	{
		return _T("P");
	}
	else if (a1 == true && a2 == false && a3 == true && a4 == false && a5 == false && a6 == true && a7 == true)
	{
		return _T("I");
	}
	else if (a1 == false && a2 == false && a3 == true && a4 == true && a5 == true && a6 == true && a7 == true)
	{
		return _T("D");
	}
	else if (a1 == false && a2 == true && a3 == true && a4 == true && a5 == true && a6 == true && a7 == false)
	{
		return _T("H");
	}
	return _T(" ");
}

int CHotLong_PCBDlg::getgrey(CImage image, int x, int y)
{

	return image.GetPixel(x,y);
	/*
	int w = image.GetWidth();

	Mat_<uchar>::iterator it = image.begin<uchar>();

	Mat_<uchar>::iterator itend = image.end<uchar>();
	int pixel = *(it + y * w + x);
	return pixel;
	*/
}

bool CHotLong_PCBDlg::cross(CImage image, int x1, int y1, int x2, int y2)

{
	if (getgrey(image, (x1+x2)/2, (y1+y2)/2 ) > 200)
		return true;
	return false;

	/*
	if (x1 == x2)
	{
		for (int y = y1; y < y2; y++)
		{
			if (getgrey(image, x1, y) > 200)
				return true;
		}
		return false;
	}
	else
	{
		for (int x = x1; x < x2; x++)
		{
			if (getgrey(image, x, y1) > 200)
				return true;
		}
		return false;

	}
	*/

}


int CHotLong_PCBDlg::getnum_7(CImage image)
{


	int w = image.GetWidth();
	int h = image.GetHeight();



	int result;

	bool a1, a2, a3, a4, a5, a6, a7, dot, space1，space2;



	//	 0-0.3 0.35--0.65 0.7--1.0w
	//		      a1				    0.2h
	//    a2    space_1      a3			0.2h
	//            a4				    0.2h
	//	  a5    space_2      a6			0.2h
	//            a7				    0,2h
	CRect Rect_a1(0.10 * w, 0.00 * h, 0.90 * w, 0.20 * h);
	CRect Rect_a2(0.00 * w, 0.20 * h, 0.20 * w, 0.40 * h);
	CRect Rect_a3(0.70 * w, 0.20 * h, 1.00 * w, 0.40 * h);
	CRect Rect_a4(0.10 * w, 0.40 * h, 0.40 * w, 0.60 * h);
	CRect Rect_a5(0.00 * w, 0.60 * h, 0.20 * w, 0.80 * h);
	CRect Rect_a6(0.70 * w, 0.60 * h, 1.00 * w, 0.80 * h);
	CRect Rect_a7(0.00 * w, 0.80 * h, 0.30 * w, 1.00 * h);
	CRect Rect_dot(0.90 * w, 0.80 * h, 1.00 * w, 1.00 * h);

	CRect Rect_space1(0.40 * w, 0.20 * h, 0.50 * w, 0.40 * h);
	CRect Rect_space2(0.30 * w, 0.60 * h, 0.40 * w, 0.80 * h);




	CRect temp_rect;

	temp_rect = Rect_a1;
	if (cross(image, temp_rect.left, temp_rect.top, temp_rect.right, temp_rect.bottom))
		a1 = true;
	else
		a1 = false;
	temp_rect = Rect_a2;
	if (cross(image, temp_rect.left, temp_rect.top, temp_rect.right, temp_rect.bottom))
		a2 = true;
	else
		a2 = false;
	temp_rect = Rect_a3;
	if (cross(image, temp_rect.left, temp_rect.top, temp_rect.right, temp_rect.bottom))
		a3 = true;
	else
		a3 = false;
	temp_rect = Rect_a4;
	if (cross(image, temp_rect.left, temp_rect.top, temp_rect.right, temp_rect.bottom))
		a4 = true;
	else
		a4 = false;
	temp_rect = Rect_a5;
	if (cross(image, temp_rect.left, temp_rect.top, temp_rect.right, temp_rect.bottom))
		a5 = true;
	else
		a5 = false;
	temp_rect = Rect_a6;
	if (cross(image, temp_rect.left, temp_rect.top, temp_rect.right, temp_rect.bottom))
		a6 = true;
	else
		a6 = false;

	temp_rect = Rect_a7;
	if (cross(image, temp_rect.left, temp_rect.top, temp_rect.right, temp_rect.bottom))
		a7 = true;
	else
		a7 = false;

	int grey1 = getgrey(image, (Rect_space1.left + Rect_space1.right) / 2, (Rect_space1.top + Rect_space1.bottom) / 2);
	int grey2 = getgrey(image, (Rect_space2.left + Rect_space2.right) / 2, (Rect_space2.top + Rect_space2.bottom) / 2);


	if (a1 != 0 && a2 == 0 && a3 != 0 && a4 != 0 && a5 != 0 && a6 == 0 && a7 != 0
		&& (grey1 <= 150)
		&& (grey2 <= 150))
		result = 2;
	else if (a1 != 0 && a2 == 0 && a3 != 0 && a4 != 0 && a5 == 0 && a6 != 0 && a7 != 0
		&& (grey1 <= 150)
		&& (grey2 <= 150))
		result = 3;
	else if (a1 == 0 && a2 != 0 && a3 != 0 && a4 != 0 && a5 == 0 && a6 != 0 && a7 == 0
		&& (grey1 <= 150)
		&& (grey2 <= 150))
		result = 4;
	else if (a1 != 0 && a2 != 0 && a3 == 0 && a4 != 0 && a5 == 0 && a6 != 0 && a7 != 0
		&& (grey1 <= 150)
		&& (grey2 <= 150))
		result = 5;
	else if (a1 != 0 && a2 != 0 && a3 == 0 && a4 != 0 && a5 != 0 && a6 != 0 && a7 != 0
		&& (grey1 <= 150)
		&& (grey2 <= 150))
		result = 6;
	else if (a1 != 0 && a2 == 0 && a3 != 0 && a4 == 0 && a5 == 0 && a6 != 0 && a7 == 0
		&& (grey1 <= 150)
		&& (grey2 <= 150))
		result = 7;
	else if (a1 != 0 && a2 == 0 && a3 != 0 && a4 == 0 && a5 == 0 && a6 != 0 && a7 != 0
		&& (grey1 <= 150)
		&& (grey2 <= 150))
		result = 7;
	else if (a1 != 0 && a2 != 0 && a3 != 0 && a4 != 0 && a5 != 0 && a6 != 0 && a7 != 0
		&& (grey1 <= 150)
		&& (grey2 <= 150))
		result = 8;
	else if (a1 != 0 && a2 != 0 && a3 != 0 && a4 != 0 && a5 == 0 && a6 != 0 && a7 != 0
		&& (grey1 <= 150)
		&& (grey2 <= 150))
		result = 9;
	else if (a1 != 0 && a2 != 0 && a3 != 0 && a4 == 0 && a5 != 0 && a6 != 0 && a7 != 0
		&& (grey1 <= 150)
		&& (grey2 <= 150))
		result = 0;
	else
	{
		result = -1;
	}

	return result;

}

void CHotLong_PCBDlg::Led_Num(void)
{

	if(!this->IsWindowVisible())
	{
		return;
	}
	this->UpdateData(true);

	CImage Image[4];
	CString s;

	m_sLED_NUM = _T("");

	
	
	for (int i = 0; i < 4; i++)
	{
		s.Format(_T("C:\\work\\LED_NUM_%d.bmp"), i + 1);
		if(m_ConfigData.m_LED_NUM_Right[i] > m_ConfigData.m_LED_NUM_Left[i]
			&& m_ConfigData.m_LED_NUM_Bottom[i] > m_ConfigData.m_LED_NUM_Top[i])
			{
				Take_Screenshot_To_File(&Image[i],
					m_ConfigData.m_LED_NUM_Left[i], m_ConfigData.m_LED_NUM_Top[i],
					m_ConfigData.m_LED_NUM_Right[i], m_ConfigData.m_LED_NUM_Bottom[i],
					s);

				ShowLedNumPicture(&Image[i],i);

				this->SaveOnlyRedColor(&Image[i], R_BITS, R_BITS_MIN);
				s.Format(_T("C:\\work\\LED_NUM_White_%d.bmp"), i + 1);
			//	Image.Save(s);
			m_sLED_NUM += GetLedChar(i,&Image[i]);
				Image[i].Destroy();

			}
		
	}

	

	this->UpdateData(false);

	
	

}
void CHotLong_PCBDlg::OnStnClickedStaticLeft5()
{
	// TODO: 在此添加控件通知处理程序代码
}

void CHotLong_PCBDlg::Led_Heat(void)
{
	// TODO: 在此添加控件通知处理程序代码
	CImage Image;
	if(m_ConfigData.m_LED_HEAT_Right > m_ConfigData.m_LED_HEAT_Left
		&& m_ConfigData.m_LED_HEAT_Bottom > m_ConfigData.m_LED_HEAT_Top)
	{
		Take_Screenshot_To_File(&Image,
			m_ConfigData.m_LED_HEAT_Left,m_ConfigData.m_LED_HEAT_Top, 
			m_ConfigData.m_LED_HEAT_Right, m_ConfigData.m_LED_HEAT_Bottom, _T("C:\\work\\LED_HEAT.bmp"));

		ShowLedHeatPicture(&Image);
	}

}

void CHotLong_PCBDlg::Led_Color(void)
{
	// TODO: 在此添加控件通知处理程序代码
	CImage Image;
	if(m_ConfigData.m_LED_COLOR_Right > m_ConfigData.m_LED_COLOR_Left
		&& m_ConfigData.m_LED_COLOR_Bottom > m_ConfigData.m_LED_COLOR_Top)
	{

		Take_Screenshot_To_File(&Image,m_ConfigData.m_LED_COLOR_Left,
			m_ConfigData.m_LED_COLOR_Top, m_ConfigData.m_LED_COLOR_Right, 
			m_ConfigData.m_LED_COLOR_Bottom, _T("C:\\work\\LED_COLOR.bmp"));


		ShowLedColorPicture(&Image);
	}

}

void CHotLong_PCBDlg::Led_Cool(void)
{
	CImage Image;
	if(m_ConfigData.m_LED_COOL_Right > m_ConfigData.m_LED_COOL_Left
		&& m_ConfigData.m_LED_COOL_Bottom > m_ConfigData.m_LED_COOL_Top)
	{
		Take_Screenshot_To_File(&Image,m_ConfigData.m_LED_COOL_Left,
			m_ConfigData.m_LED_COOL_Top, m_ConfigData.m_LED_COOL_Right, m_ConfigData.m_LED_COOL_Bottom, _T("C:\\work\\LED_COOL.bmp"));

		ShowLedCoolPicture(&Image);
	}

}

void CHotLong_PCBDlg::ShowLedColorPicture(CImage *image)
{

	
	//image.Load(_T("c:\\work\\LED_COLOR.bmp"));
	
	


	float r = CheckColor(image, R_BITS, R_BITS_MIN);
	float g = CheckColor(image, G_BITS, G_BITS_MIN);
	float b = CheckColor(image, B_BITS, B_BITS_MIN);
	

	int width = image->GetWidth();
	int height = image->GetHeight();

	float pixels = width * height;


	this->m_Led_Color = ShowColorText(&m_Edit_Led_Color,r, g, b, pixels);


	this->UpdateData(false);

	//image.Save(_T("c:\\work\\LED_COLOR.png"));
	CRect led_num_pic_rect;
	CWnd* pWnd = GetDlgItem(IDC_PIC_LED_COLOR);

	pWnd->GetClientRect(led_num_pic_rect);

	CDC* pDc = pWnd->GetDC();

	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

	//画图（以下两种方法都可）
	//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
	image->Draw(pDc->m_hDC, led_num_pic_rect);                //将图片绘制到Picture控件表示的矩形区域

//	image.Destroy();

	pWnd->ReleaseDC(pDc);



}
CString  CHotLong_PCBDlg::ShowColorText(CMyEdit *edit,float r, float g, float b, float pixels)
{

	r /= pixels;
	g /= pixels;
	b /= pixels;

	if (r > 0.8 && g > 0.8 && b > 0.8)
	{
		edit->SetTextColor(RGB(0xFF, 0xFF, 0xFF));
		edit->SetBackColor(RGB(0, 0, 0));
		return _T("白色");
	}
	else if (r > 0.8)
	{
		edit->SetTextColor(RGB(0xFF, 0, 0));
		edit->SetBackColor(RGB(0xFF, 0xFF, 0xFF));
		return  _T("红色");
	}
	else if (g > 0.8)
	{
		edit->SetTextColor(RGB(0, 0xFF, 0));
		edit->SetBackColor(RGB(0xFF, 0xFF, 0xFF));
		return  _T("绿色");
	}
	else if (b > 0.8)
	{
		edit->SetTextColor(RGB(0, 0, 0xFF));
		edit->SetBackColor(RGB(0xFF, 0xFF, 0xFF));
		return  _T("蓝色");
	}
	
	edit->SetTextColor(RGB(0, 0, 0));
	edit->SetBackColor(RGB(0xFF, 0xFF, 0xFF));
		return _T("黑色");
	
}

void CHotLong_PCBDlg::ShowLedHeatPicture(CImage * image)
{

	
	//image.Load(_T("c:\\work\\LED_HEAT.bmp"));
	float r = CheckColor(image, R_BITS, R_BITS_MIN);
	float g = CheckColor(image, G_BITS, G_BITS_MIN);
	float b = CheckColor(image, B_BITS, B_BITS_MIN);
	
	int width = image->GetWidth();
	int height = image->GetHeight();

	float pixels = width * height;


	this->m_Led_Heat = ShowColorText(&m_Edit_Led_Heat,r, g, b, pixels);
	

	
	this->UpdateData(false);
	//image.Save(_T("c:\\work\\LED_HEAT.png"));
	CRect led_num_pic_rect;
	CWnd* pWnd = GetDlgItem(IDC_PIC_LED_HEAT);

	pWnd->GetClientRect(led_num_pic_rect);

	CDC* pDc = pWnd->GetDC();

	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

	//画图（以下两种方法都可）
	//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
	image->Draw(pDc->m_hDC, led_num_pic_rect);                //将图片绘制到Picture控件表示的矩形区域

	pWnd->ReleaseDC(pDc);



}

unsigned int  CHotLong_PCBDlg::CheckColor(CImage *image,unsigned int ColorBits, unsigned int ColorMinVal)
{
	int width = image->GetWidth();
	int height = image->GetHeight();
	int counts = 0;
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			COLORREF c = image->GetPixel(x, y);

			c &= ColorBits;
			if (c > ColorMinVal)
			{
				counts++;
			}
			else
			{
				
			}

		}
	}
	return counts;
}

void CHotLong_PCBDlg::ShowLedCoolPicture(CImage *image)
{

	
	

	float r = CheckColor(image, R_BITS, R_BITS_MIN);
	float g = CheckColor(image, G_BITS, G_BITS_MIN);
	float b = CheckColor(image, B_BITS, B_BITS_MIN);

	int width = image->GetWidth();
	int height = image->GetHeight();

	float pixels = width * height;


	this->m_Led_Cool = ShowColorText(&m_Edit_Led_Cool,r, g, b, pixels);
	
	this->UpdateData(false);

	//image.Save(_T("c:\\work\\LED_COOL.png"));
	CRect led_num_pic_rect;
	CWnd* pWnd = GetDlgItem(IDC_PIC_LED_COOL);

	pWnd->GetClientRect(led_num_pic_rect);

	CDC* pDc = pWnd->GetDC();

	SetStretchBltMode(pDc->m_hDC, STRETCH_HALFTONE);//绘图前必须调用此函数（设置缩放模式），否则失真严重

	//画图（以下两种方法都可）
	//image.StretchBlt(pDc->m_hDC, rectPicture, SRCCOPY); //将图片绘制到Picture控件表示的矩形区域
	image->Draw(pDc->m_hDC, led_num_pic_rect);                //将图片绘制到Picture控件表示的矩形区域

	
	pWnd->ReleaseDC(pDc);



}








// 端口类 GUID（COM 串口所在设备类）
#ifndef GUID_DEVCLASS_PORTS
DEFINE_GUID(GUID_DEVCLASS_PORTS,
0x4D36E978, 0xE325, 0x11CE, 0xBF, 0xC1, 0x08, 0x00, 0x2B, 0xE1, 0x03, 0x18);
#endif

void EnumSerialPortFriendlyNames(CStringArray& portList)
{
    portList.RemoveAll();

    // 1. 打开端口设备类
    HDEVINFO hDevInfo = SetupDiGetClassDevs(
        &GUID_DEVCLASS_PORTS,
        NULL,
        NULL,
        DIGCF_PRESENT | DIGCF_PROFILE
    );
    if (hDevInfo == INVALID_HANDLE_VALUE)
        return;

    SP_DEVINFO_DATA devInfoData = { 0 };
    devInfoData.cbSize = sizeof(SP_DEVINFO_DATA);

    // 2. 遍历所有端口设备
    for (DWORD i = 0; SetupDiEnumDeviceInfo(hDevInfo, i, &devInfoData); i++)
    {
        TCHAR szFriendlyName[256] = { 0 };
        TCHAR szPortName[32] = { 0 };

        // 3. 获取【友好名称】SPDRP_FRIENDLYNAME
        if (SetupDiGetDeviceRegistryProperty(
            hDevInfo,
            &devInfoData,
            SPDRP_FRIENDLYNAME,
            NULL,
            (PBYTE)szFriendlyName,
            sizeof(szFriendlyName),
            NULL))
        {
			// 4. 获取设备实例ID，用于映射到 COMx
            TCHAR szInstanceId[256] = { 0 };
            if (SetupDiGetDeviceInstanceId(
                hDevInfo,
                &devInfoData,
                szInstanceId,
                _countof(szInstanceId),
                NULL))
            {
                // 5. 从注册表 SERIALCOMM 映射到 COMx
                HKEY hKey = NULL;
                if (RegOpenKeyEx(
                    HKEY_LOCAL_MACHINE,
                    _T("HARDWARE\\DEVICEMAP\\SERIALCOMM"),
                    0,
                    KEY_READ,
                    &hKey) == ERROR_SUCCESS)
                {
                    TCHAR szDevicePath[256] = { 0 };
                    DWORD dwType = 0, dwSize = sizeof(szPortName);
					
					memset(szPortName,0,sizeof(szPortName));
                    // 遍历所有值，匹配设备路径
                    for (DWORD j = 0; ;	j++)
                    {
						DWORD counts ;

						counts = sizeof(szDevicePath) / sizeof(TCHAR);
						bool ret = RegEnumValue(
                        hKey,
                        j,
                        szDevicePath,
                        &counts,//_countof(szDevicePath),
                        NULL,
                        &dwType,
                        (LPBYTE)szPortName,
                        &dwSize);
						if( ret == ERROR_SUCCESS)
						{
							// 设备路径包含实例ID，说明是同一个串口
							//if (_tcsstr(szDevicePath, szInstanceId) != NULL)
							{
								// 组合：COMx + 友好名称
								CString strItem;
								//strItem.Format(_T("%s: %s"), szPortName, szFriendlyName);
								strItem.Format(_T("%s"), szFriendlyName);
								portList.Add(strItem);
								break;
							}
						}
						else
						{
							break;
						}
							dwSize = sizeof(szPortName);
                    }
                    RegCloseKey(hKey);
                }
            }
        }
    }

    SetupDiDestroyDeviceInfoList(hDevInfo);
}
HBRUSH CHotLong_PCBDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔

	if(CTLCOLOR_DLG == nCtlColor)
	{
		return G_bkBrush;
	}
	switch(pWnd->GetDlgCtrlID())
	{
		case IDC_EDIT_BARCODE:
		case IDC_EDIT_BARCODE_1:
		case IDC_EDIT_BARCODE_2:
		case IDC_EDIT_BARCODE_3:
		case IDC_EDIT_BARCODE_4:
		case IDC_EDIT_BARCODE_5:
		case IDC_EDIT_BARCODE_6:
		case IDC_EDIT_BARCODE_7:
		case IDC_EDIT_BARCODE_8:

			break;

		case IDC_EDIT_RESULT:
		case IDC_EDIT_RESULT_1:
		case IDC_EDIT_RESULT_2:
		case IDC_EDIT_RESULT_3:
		case IDC_EDIT_RESULT_4:
		case IDC_EDIT_RESULT_5:
		case IDC_EDIT_RESULT_6:
		case IDC_EDIT_RESULT_7:
		case IDC_EDIT_RESULT_8:
		break;

			
	default:
		return G_bkBrush;
	
	}

	return hbr;
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumLeft1()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_LEFT_1);
		w->SetWindowTextW(dlg.m_Input);
	}

}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumRight1()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_RIGHT_1);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumTop1()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_TOP_1);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumBottom1()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_BOTTOM_1);
		w->SetWindowTextW(dlg.m_Input);
	}
}

//
void CHotLong_PCBDlg::OnEnSetfocusEditLedNumLeft2()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_LEFT_2);
		w->SetWindowTextW(dlg.m_Input);
	}

}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumRight2()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_RIGHT_2);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumTop2()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_TOP_2);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumBottom2()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_BOTTOM_2);
		w->SetWindowTextW(dlg.m_Input);
	}
}
//3
void CHotLong_PCBDlg::OnEnSetfocusEditLedNumLeft3()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_LEFT_3);
		w->SetWindowTextW(dlg.m_Input);
	}

}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumRight3()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_RIGHT_3);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumTop3()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_TOP_3);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumBottom3()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_BOTTOM_3);
		w->SetWindowTextW(dlg.m_Input);
	}
}
//4
void CHotLong_PCBDlg::OnEnSetfocusEditLedNumLeft4()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_LEFT_4);
		w->SetWindowTextW(dlg.m_Input);
	}

}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumRight4()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_RIGHT_4);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumTop4()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_TOP_4);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedNumBottom4()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_NUM_BOTTOM_4);
		w->SetWindowTextW(dlg.m_Input);
	}
}

//Heat
void CHotLong_PCBDlg::OnEnSetfocusEditLedHeatLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_HEAT_LEFT);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedHeatRight()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_HEAT_RIGHT);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedHeatTop()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_HEAT_TOP);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedHeatBottom()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_HEAT_BOTTOM);
		w->SetWindowTextW(dlg.m_Input);
	}
}


//Cool
void CHotLong_PCBDlg::OnEnSetfocusEditLedCoolLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_COOL_LEFT);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedCoolRight()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_COOL_RIGHT);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedCoolTop()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_COOL_TOP);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedCoolBottom()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_COOL_BOTTOM);
		w->SetWindowTextW(dlg.m_Input);
	}
}


//Color
void CHotLong_PCBDlg::OnEnSetfocusEditLedColorLeft()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_COLOR_LEFT);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedColorRight()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_COLOR_RIGHT);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedColorTop()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_COLOR_TOP);
		w->SetWindowTextW(dlg.m_Input);
	}
}

void CHotLong_PCBDlg::OnEnSetfocusEditLedColorBottom()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDC_EDIT_DEGREE);
	w->SetFocus();
	CInputDlg dlg;

	if(dlg.DoModal() == IDOK)
	{
		w  = this->GetDlgItem(IDC_EDIT_LED_COLOR_BOTTOM);
		w->SetWindowTextW(dlg.m_Input);
	}
}
