// DebugDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox_CGP.h"
#include "DebugDlg.h"
#include "HeatBox_CGPDlg.h"
#include "NumInputDlg.h"

// CDebugDlg 对话框
extern CString theAppDirectory;

IMPLEMENT_DYNAMIC(CDebugDlg, CDialog)

CDebugDlg::CDebugDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDebugDlg::IDD, pParent)
	, m_FullOutTemp(0)
	, m_NormalPidTemp(0)
	, m_StepPidTopTemp(0)
	, m_StepPidTime(0)
	, m_FullCoolTemp(0)
	, m_StepCoolTemp(0)
	, m_PidCoolTemp(0)
	, m_Read_P(0)
	, m_Read_I(0)
	, m_Read_D(0)
	, m_Write_P(0)
	, m_Write_I(0)
	, m_Write_D(0)
	, m_Read_P_Cool(0)
	, m_Read_I_Cool(0)
	, m_Read_D_Cool(0)
	, m_Write_P_Cool(0)
	, m_Write_I_Cool(0)
	, m_Write_D_Cool(0)
	, m_ReadAdjustLow_1(0)
	, m_ReadAdjustHigh_1(0)
	, m_WriteAdjustLow_1(0)
	, m_WriteAdjustHigh_1(0)
	, m_CurTemp(0)
	, m_StepCoolMinutes(0)
	, m_AT_Temp(0)
	, m_StepPercent(0)
	, m_CoolStepPercent(0)
	, m_CommSpeed(0)
{
	m_AutoCalStep = 0;
	m_bShowed = false;
	m_bJiajuTesting = false;
	m_bSetFocusOnEditCurTemp = true;
}

CDebugDlg::~CDebugDlg()
{
	m_bShowed = false;
}

void CDebugDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FULL_COOL_OUT_TEMP, m_FullCoolTemp);
	DDX_Text(pDX, IDC_EDIT_STEP_COOL_OUT_TEMP, m_StepCoolTemp);
	DDX_Text(pDX, IDC_EDIT_PID_COOL_OUT_TEMP, m_PidCoolTemp);
	DDX_Text(pDX, IDC_EDIT_FULL_OUT_TEMP, m_FullOutTemp);
	DDX_Text(pDX, IDC_EDIT_PID_OUT, m_NormalPidTemp);
	DDX_Text(pDX, IDC_EDIT_STEP_PID_OUT_TEMP, m_StepPidTopTemp);
	DDX_Text(pDX, IDC_EDIT_STEP_PID_OUT_TIME, m_StepPidTime);
	DDX_Text(pDX, IDC_EDIT_READ_P, m_Read_P);
	DDX_Text(pDX, IDC_EDIT_READ_I, m_Read_I);
	DDX_Text(pDX, IDC_EDIT_READ_D, m_Read_D);
	DDX_Text(pDX, IDC_EDIT_WRITE_P, m_Write_P);
	DDX_Text(pDX, IDC_EDIT_WRITE_I, m_Write_I);
	DDX_Text(pDX, IDC_EDIT_WRITE_D, m_Write_D);
	DDX_Text(pDX, IDC_EDIT_READ_P2, m_Read_P_Cool);
	DDX_Text(pDX, IDC_EDIT_READ_I2, m_Read_I_Cool);
	DDX_Text(pDX, IDC_EDIT_READ_D2, m_Read_D_Cool);
	DDX_Text(pDX, IDC_EDIT_WRITE_P2, m_Write_P_Cool);
	DDX_Text(pDX, IDC_EDIT_WRITE_I2, m_Write_I_Cool);
	DDX_Text(pDX, IDC_EDIT_WRITE_D2, m_Write_D_Cool);
	DDX_Text(pDX, IDC_EDIT_READ_ADJUST_LOW, m_ReadAdjustLow_1);
	//	DDX_Text(pDX, IDC_EDIT_READ_ADJUST_LOW2, m_ReadAdjustLow_2);
	DDX_Text(pDX, IDC_EDIT_READ_ADJUST_HIGH, m_ReadAdjustHigh_1);
	//	DDX_Text(pDX, IDC_EDIT_READ_ADJUST_HIGH2, m_ReadAdjustHigh_2);
	DDX_Text(pDX, IDC_EDIT_WRITE_ADJUST_LOW_1, m_WriteAdjustLow_1);
	//	DDX_Text(pDX, IDC_EDIT_WRITE_ADJUST_LOW_2, m_WriteAdjustLow_2);
	DDX_Text(pDX, IDC_EDIT_WRITE_ADJUST_HIGH_1, m_WriteAdjustHigh_1);
	//	DDX_Text(pDX, IDC_EDIT_WRITE_ADJUST_HIGH_2, m_WriteAdjustHigh_2);
	DDX_Text(pDX, IDC_EDIT_CUR_TEMP, m_CurTemp);
	DDX_Text(pDX, IDC_EDIT_STEP_COOL_OUT_TIME, m_StepCoolMinutes);
	DDX_Text(pDX, IDC_EDIT_AT_TEMP, m_AT_Temp);
	DDX_Text(pDX, IDC_EDIT_STEP_PID_OUT_PERCENT, m_StepPercent);
	DDX_Text(pDX, IDC_EDIT_STEP_COOL_OUT_PERCENT, m_CoolStepPercent);
	DDX_Text(pDX, IDC_EDIT_COMM_SPEED, m_CommSpeed);
}


BEGIN_MESSAGE_MAP(CDebugDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_FULL_HEAT, &CDebugDlg::OnBnClickedButtonFullHeat)
	ON_BN_CLICKED(IDC_BUTTON_PID_HEAT, &CDebugDlg::OnBnClickedButtonPidHeat)
	ON_BN_CLICKED(IDC_BUTTON_STEP_PID_HEAT, &CDebugDlg::OnBnClickedButtonStepPidHeat)
	ON_BN_CLICKED(IDC_BUTTON_READ_PID, &CDebugDlg::OnBnClickedButtonReadPid)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_PID, &CDebugDlg::OnBnClickedButtonWritePid)
	ON_BN_CLICKED(IDC_BUTTON_READ_ADJUST_LOW, &CDebugDlg::OnBnClickedButtonReadAdjustLow)
	ON_BN_CLICKED(IDC_BUTTON_READ_ADJUST_LOW2, &CDebugDlg::OnBnClickedButtonReadAdjustLow2)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_ADJUST_LOW_1, &CDebugDlg::OnBnClickedButtonWriteAdjustLow1)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_ADJUST_HIGH, &CDebugDlg::OnBnClickedButtonWriteAdjustHigh)
	ON_BN_CLICKED(IDC_BUTTON_DIPSLAY_SWITCH_STATUS, &CDebugDlg::OnBnClickedButtonDipslaySwitchStatus)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY_ALL, &CDebugDlg::OnBnClickedButtonDisplayAll)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_DISPLAY_NO, &CDebugDlg::OnBnClickedButtonAutoDisplayNo)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_ADJUST_TEMP, &CDebugDlg::OnBnClickedButtonAutoAdjustTemp)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CDebugDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDebugDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_STOP_HEAT, &CDebugDlg::OnBnClickedButtonStopHeat)
	ON_BN_CLICKED(IDC_BUTTON_COOL, &CDebugDlg::OnBnClickedButtonCool)
	ON_BN_CLICKED(IDC_BUTTON_STEP_COOL, &CDebugDlg::OnBnClickedButtonStepCool)
	ON_BN_CLICKED(IDC_BUTTON_PID_COOL, &CDebugDlg::OnBnClickedButtonPidCool)
	ON_BN_CLICKED(IDC_BUTTON_RELAY_ON, &CDebugDlg::OnBnClickedButtonRelayOn)
	ON_BN_CLICKED(IDC_BUTTON_RELAY_OFF, &CDebugDlg::OnBnClickedButtonRelayOff)
	ON_BN_CLICKED(IDC_BUTTON_UPDATE_FIRMWARE, &CDebugDlg::OnBnClickedButtonUpdateFirmware)
	ON_BN_CLICKED(IDC_CHECK_JIAJU_TEST, &CDebugDlg::OnBnClickedCheckJiajuTest)
	ON_BN_CLICKED(IDC_BUTTON_READ_CURRENT_METER_TEMP_LOW, &CDebugDlg::OnBnClickedButtonReadCurrentMeterTempLow)
	ON_BN_CLICKED(IDC_BUTTON_READ_CURRENT_METER_TEMP_HIGH, &CDebugDlg::OnBnClickedButtonReadCurrentMeterTempHigh)
	ON_BN_CLICKED(IDC_BUTTON1, &CDebugDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON_INIT_ADJUST_TEMP, &CDebugDlg::OnBnClickedButtonInitAdjustTemp)
	ON_BN_CLICKED(IDC_BUTTON_PID_AUTOTUNE, &CDebugDlg::OnBnClickedButtonPidAutotune)
	ON_BN_CLICKED(IDC_BUTTON_COMM_SPEED, &CDebugDlg::OnBnClickedButtonCommSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_COMM_SPEED, &CDebugDlg::OnEnSetfocusEditCommSpeed)
	ON_EN_SETFOCUS(IDC_EDIT_CUR_TEMP, &CDebugDlg::OnEnSetfocusEditCurTemp)
	ON_EN_SETFOCUS(IDC_EDIT_FULL_OUT_TEMP, &CDebugDlg::OnEnSetfocusEditFullOutTemp)
	ON_EN_SETFOCUS(IDC_EDIT_PID_OUT, &CDebugDlg::OnEnSetfocusEditPidOut)
END_MESSAGE_MAP()


// CDebugDlg 消息处理程序

void CDebugDlg::OnBnClickedButtonFullHeat()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];

	this->UpdateData(true);

	
	CString cmd;
	
	memcpy(para,&m_FullOutTemp,sizeof(m_FullOutTemp));

	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_FULL_OUT,para)); 
}

void CDebugDlg::OnBnClickedButtonPidHeat()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];

	this->UpdateData(true);

	memset(para,0,sizeof(para));
	
	CString cmd;
	
	memcpy(para,&m_NormalPidTemp,sizeof(m_NormalPidTemp));


	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_NORMAL_PID_OUT,para)); 
}

void CDebugDlg::OnBnClickedButtonStepPidHeat()
{
	// TODO: 在此添加控件通知处理程序代码

		char para[16];

	this->UpdateData(true);

	
	CString cmd;
	
	memcpy(para,&m_StepPidTopTemp,sizeof(m_StepPidTopTemp));

	short seconds = m_StepPidTime * 60;

	memcpy(&para[sizeof(m_StepPidTopTemp)],&seconds,sizeof(seconds));

	memcpy(&para[sizeof(m_StepPidTopTemp)+sizeof(short)],&m_StepPercent,sizeof(m_StepPercent));
	

	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_STEP_PID_OUT,para)); 
}

void CDebugDlg::OnBnClickedButtonReadPid()
{
	// TODO: 在此添加控件通知处理程序代码
		char para[16];

	this->UpdateData(true);

		
	memset(para,0,sizeof(para));
	

	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_READ_PID_T,para));
}

void CDebugDlg::OnBnClickedButtonWritePid()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];

	this->UpdateData(true);

	memset(para,0,sizeof(para));
	
	memcpy(&para[0],&m_Write_P,sizeof(short));		
	memcpy(&para[2],&m_Write_I,sizeof(short));		
	memcpy(&para[4],&m_Write_D,sizeof(short));
	para[6] = 1;		
	para[7] = 0;
	memcpy(&para[8],&m_Write_P_Cool,sizeof(short));		
	memcpy(&para[10],&m_Write_I_Cool,sizeof(short));		
	memcpy(&para[12],&m_Write_D_Cool,sizeof(short));		
	

	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_PID_T,para));

	G_MainDlg->KillTimer(1);

	G_MainDlg->SetTimer(1,500,NULL); 
}

void CDebugDlg::OnBnClickedButtonReadAdjustLow()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];

	this->UpdateData(true);
	
	memset(para,0,sizeof(para));

	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_READ_ADJUST_LOW,para));

}

void CDebugDlg::OnBnClickedButtonReadAdjustLow2()
{
	// TODO: 在此添加控件通知处理程序代码
		char para[16];

	this->UpdateData(true);

	
	memset(para,0,sizeof(para));
	

	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_READ_ADJUST_HIGH,para));
}

void CDebugDlg::OnBnClickedButtonWriteAdjustLow1()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];

	this->UpdateData(true);

	
	memset(para,0,sizeof(para));

	memcpy(para,&m_WriteAdjustLow_1,sizeof(m_WriteAdjustLow_1));

	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_ADJUST_LOW,para));

	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_READ_ADJUST_LOW,para));

}

void CDebugDlg::OnBnClickedButtonWriteAdjustHigh()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];

	this->UpdateData(true);

	
	memset(para,0,sizeof(para));
	
	memcpy(para,&m_WriteAdjustHigh_1,sizeof(m_WriteAdjustHigh_1));
	
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_ADJUST_HIGH,para));
	
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_READ_ADJUST_HIGH,para));

}

void CDebugDlg::OnBnClickedButtonDipslaySwitchStatus()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];

	this->UpdateData(true);

	
	memset(para,0,sizeof(para));
	
	para[0] = 0;		
	
 
	
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_DISPLAY_TYPE,para));
}

void CDebugDlg::OnBnClickedButtonDisplayAll()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];

	this->UpdateData(true);

	
	memset(para,0,sizeof(para));
	
	para[0] = 1;		
	
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_DISPLAY_TYPE,para));
}

void CDebugDlg::OnBnClickedButtonAutoDisplayNo()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];

	this->UpdateData(true);

	memset(para,0,sizeof(para));
	
	para[0] = 2;		
	
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_DISPLAY_TYPE,para));
}

void CDebugDlg::OnBnClickedButtonAutoAdjustTemp()
{
	// TODO: 在此添加控件通知处理程序代码
/*

	if(m_AutoCalStep == 0)
	{
		CString cmdStr;
		char para[16];

		m_WriteAdjustLow_1 = 0;

		m_WriteAdjustHigh_1 = 0;


		this->UpdateData(false);

		CHeatBox_CGPDlg * wnd = (CHeatBox_CGPDlg *)this->GetParent(); 

		memset(para,0,sizeof(para));
		
		memcpy(para,&m_WriteAdjustLow_1,sizeof(m_WriteAdjustLow_1));

		
		wnd->m_SendCmdArray.Add(wnd->SetCmd('1', CMD_SET_ADJUST_LOW,para));

		memcpy(para,&m_WriteAdjustHigh_1,sizeof(m_WriteAdjustHigh_1));
		


		wnd->m_SendCmdArray.Add(wnd->SetCmd('1', CMD_SET_ADJUST_HIGH,para));
		m_AutoCalStep++;

//		wnd->m_AdjustTempCool = 0.0f;
//		wnd->m_AdjustTemp = 0.0f;

		AfxGetApp()->WriteProfileInt("SET","ADJUST_TEMP_HEAT",10000); 
	
		AfxGetApp()->WriteProfileInt("SET","ADJUST_TEMP_COOL",10000); 
	
		wnd->SendCmdReadAdjustLow(); 
		wnd->SendCmdReadAdjustHigh();
	}
	else
	{
		if(AfxMessageBox("正在校温中，需要终止吗？",MB_YESNO) == IDYES)
		{
			m_AutoCalStep = 0;
		}
	}

	*/
}

void CDebugDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	static bool IsTimering;
	static int  TimerCount = 0;
    static float input;

	char para[16];
	
	CDialog::OnTimer(nIDEvent);

	if(IsTimering)
	{
		return;
	}

	
	memset(para,0,sizeof(para));

	IsTimering = true;

	switch(m_AutoCalStep)
	{
	case 1:
		{
			CNumInputDlg dlg;
			dlg.m_Title  = "请输入低部校验温度";
			if(dlg.DoModal() == IDOK)
			{
				input =_wtof(dlg.m_Input[0]); 
				
				if(input > -50.0f && input < 400.0f)
				{
					G_MainDlg->SendCmdHeatNormalPid(input);
					m_AutoCalStep++;
				}
			}
			else
			{
				m_AutoCalStep = 0;
			}
		}
		break;
	case 2:
		
		if(m_CurTemp >= input - 0.1f)
		{
			TimerCount ++;
			if(TimerCount >= 60)
			{
				CNumInputDlg dlg;
				dlg.m_Title = "请输入水银温度计等的标准温度值";
				if(dlg.DoModal() == IDOK)
				{
					m_AutoCalGlassLow = _wtof(dlg.m_Input[0]); 
					m_AutoCalMeterLow = m_CurTemp;
					
					{
						CNumInputDlg dlg2;
						dlg2.m_Title = "请输入高端校验温度";
						if(dlg2.DoModal() == IDOK)
						{
							
							input =_wtof(dlg2.m_Input[0]); 
				
							if(input > -50.0f && input < 400.0f)
							{
								G_MainDlg->SendCmdHeatNormalPid(input);
								
							}
						}
					}
					m_AutoCalStep++;
					TimerCount = 0;
				}
				else
				{
					m_AutoCalStep = 0;
				}
			}
		}
		break;

	case 3:
		if(m_CurTemp >= input - 0.1f)
		{
			TimerCount ++;
			if(TimerCount >= 60)
			{
				CNumInputDlg dlg;
				dlg.m_Title = "请输入水银温度计等的标准温度值";
				if(dlg.DoModal() == IDOK)
				{
					m_AutoCalGlassHigh = _wtof(dlg.m_Input[0]); 
					m_AutoCalMeterHigh = m_CurTemp;
					CalAdjustTemp();

					G_MainDlg->SendCmdHeatNormalPid(0);

					//设置0度修正
					memcpy(para,&m_WriteAdjustLow_1,sizeof(m_WriteAdjustLow_1));
					/*
					*((short*)para) = (short)(m_WriteAdjustLow_1 * 10.0f);		

					*((short*)(para+2)) = (short)(m_WriteAdjustLow_2 * 10.0f);		
					*/
					

					G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_ADJUST_LOW,para));

					//设置400度修正
					memcpy(para,&m_WriteAdjustHigh_1,sizeof(m_WriteAdjustHigh_1));
					/*
					*((short*)para) = (short)(m_WriteAdjustHigh_1 * 10.0f);		

					*((short*)(para+2)) = (short)(m_WriteAdjustHigh_2 * 10.0f);	
					*/
					
					G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_ADJUST_HIGH,para));

					m_AutoCalStep = 0;
					TimerCount = 0;

					this->UpdateData(false); 

				}
				else
				{
					m_AutoCalStep = 0;
				}
				
			}
		}
		break;
	}

	IsTimering = false;
	
}

BOOL CDebugDlg::OnInitDialog()
{
	CWnd * w;
	CDialog::OnInitDialog();
	this->SetTimer(1,1000,NULL); 
	m_bShowed = true;
	
	this->m_CommSpeed = ::G_NormalConfigPara.CommSpeedTimer;   

	switch(G_NormalConfigPara.Language)
	{
	case 0:
		w = this->GetDlgItem(IDC_BUTTON_FULL_HEAT);
		w->SetWindowTextW(_T("全速加热")); 
		w = this->GetDlgItem(IDC_BUTTON_PID_HEAT);
		w->SetWindowTextW(_T("PID 恒温"));
		w = this->GetDlgItem(IDC_BUTTON_STEP_PID_HEAT);
		w->SetWindowTextW(_T("线性升温"));
		w = this->GetDlgItem(IDC_BUTTON_READ_PID);
		w->SetWindowTextW(_T("读PID"));
		w = this->GetDlgItem(IDC_BUTTON_WRITE_PID);
		w->SetWindowTextW(_T("写PID"));
		w = this->GetDlgItem(IDC_BUTTON_READ_ADJUST_LOW);
		w->SetWindowTextW(_T("读0度修正值"));
		w = this->GetDlgItem(IDC_BUTTON_READ_ADJUST_LOW2);
		w->SetWindowTextW(_T("读400度修正值"));
		w = this->GetDlgItem(IDC_BUTTON_WRITE_ADJUST_LOW_1);
		w->SetWindowTextW(_T("写0度修正值"));
		w = this->GetDlgItem(IDC_BUTTON_AUTO_DISPLAY_NO);
		w = this->GetDlgItem(IDC_BUTTON_WRITE_ADJUST_HIGH);
		w->SetWindowTextW(_T("写400度修正值"));
		w = this->GetDlgItem(IDC_BUTTON_DIPSLAY_SWITCH_STATUS);
		w->SetWindowTextW(_T("显示温度保护器状态"));
		w = this->GetDlgItem(IDC_BUTTON_DISPLAY_ALL);
		w->SetWindowTextW(_T("LED 全亮"));
		w = this->GetDlgItem(IDC_BUTTON_AUTO_DISPLAY_NO);
		w->SetWindowTextW(_T("LED 全暗"));
		
		w = this->GetDlgItem(IDC_BUTTON_AUTO_ADJUST_TEMP);
		w->SetWindowTextW(_T("进入自动校温程序"));
		w = this->GetDlgItem(IDOK);
		w->SetWindowTextW(_T("确认"));
		w = this->GetDlgItem(IDCANCEL);
		w->SetWindowTextW(_T("取消"));
		w = this->GetDlgItem(IDC_BUTTON_STOP_HEAT);
		w->SetWindowTextW(_T("停止加热/制冷"));
		w = this->GetDlgItem(IDC_BUTTON_COOL);
		w->SetWindowTextW(_T("全速制冷"));
		w = this->GetDlgItem(IDC_BUTTON_STEP_COOL);
		w->SetWindowTextW(_T("线性制冷"));
		w = this->GetDlgItem(IDC_BUTTON_PID_COOL);
		w->SetWindowTextW(_T("PID 恒温制冷"));
		w = this->GetDlgItem(IDC_BUTTON_RELAY_ON);
		w->SetWindowTextW(_T("继电器动作"));
		w = this->GetDlgItem(IDC_BUTTON_RELAY_OFF);
		w->SetWindowTextW(_T("继电器复位"));
			
		w = this->GetDlgItem(IDC_BUTTON_COMM_SPEED);
		w->SetWindowTextW(_T("通信速度"));

		w = this->GetDlgItem(IDC_STATIC_HEAT_Fixed_Ratio);
		w->SetWindowTextW(_T("固定比例"));
		
		w = this->GetDlgItem(IDC_STATIC_COOL_Fixed_Ratio);
		w->SetWindowTextW(_T("固定比例"));
		
		w = this->GetDlgItem(IDC_STATIC_Standard_Low_Temp);
		w->SetWindowTextW(_T("低温标准温度"));
		w = this->GetDlgItem(IDC_STATIC_Standard_High_Temp);
		w->SetWindowTextW(_T("高温标准温度"));
		

		w = this->GetDlgItem(IDC_BUTTON_READ_CURRENT_METER_TEMP_LOW);
		w->SetWindowTextW(_T("读取仪表低温"));
		w = this->GetDlgItem(IDC_BUTTON_READ_CURRENT_METER_TEMP_HIGH);
		w->SetWindowTextW(_T("读取仪表高温"));
		
		w = this->GetDlgItem(IDC_BUTTON_INIT_ADJUST_TEMP);
		w->SetWindowTextW(_T("清除修正温度"));
		

		w = this->GetDlgItem(IDC_BUTTON_CAL_AND_SAVE_ADJUST);
		w->SetWindowTextW(_T("计算并保存修正值"));
		

		

		break;
	case 1:
		w = this->GetDlgItem(IDC_BUTTON_FULL_HEAT);
		w->SetWindowTextW(_T("FULL HEAT")); 
		w = this->GetDlgItem(IDC_BUTTON_PID_HEAT);
		w->SetWindowTextW(_T("PID HEAT"));
		w = this->GetDlgItem(IDC_BUTTON_STEP_PID_HEAT);
		w->SetWindowTextW(_T("LINE HEAT"));
		w = this->GetDlgItem(IDC_BUTTON_READ_PID);
		w->SetWindowTextW(_T("READ PID"));
		w = this->GetDlgItem(IDC_BUTTON_WRITE_PID);
		w->SetWindowTextW(_T("WRITE PID"));
		w = this->GetDlgItem(IDC_BUTTON_READ_ADJUST_LOW);
		w->SetWindowTextW(_T("Read 0℃ Adjust"));
		w = this->GetDlgItem(IDC_BUTTON_READ_ADJUST_LOW2);
		w->SetWindowTextW(_T("Read 400℃ Adjust"));
		w = this->GetDlgItem(IDC_BUTTON_WRITE_ADJUST_LOW_1);
		w->SetWindowTextW(_T("Write 0℃ Adjust"));
		w = this->GetDlgItem(IDC_BUTTON_AUTO_DISPLAY_NO);
		w = this->GetDlgItem(IDC_BUTTON_WRITE_ADJUST_HIGH);
		w->SetWindowTextW(_T("Write 400℃ Adjust"));
		w = this->GetDlgItem(IDC_BUTTON_DIPSLAY_SWITCH_STATUS);
		w->SetWindowTextW(_T("DISPLAY PROTECTOR STATUS"));
		w = this->GetDlgItem(IDC_BUTTON_DISPLAY_ALL);
		w->SetWindowTextW(_T("LED All ON"));
		w = this->GetDlgItem(IDC_BUTTON_AUTO_DISPLAY_NO);
		w->SetWindowTextW(_T("LED All OFF"));
		
		w = this->GetDlgItem(IDC_BUTTON_AUTO_ADJUST_TEMP);
		w->SetWindowTextW(_T("TEMPERATURE ADJUSTING"));
		w = this->GetDlgItem(IDOK);
		w->SetWindowTextW(_T("OK"));
		w = this->GetDlgItem(IDCANCEL);
		w->SetWindowTextW(_T("CANCEL"));
		w = this->GetDlgItem(IDC_BUTTON_STOP_HEAT);
		w->SetWindowTextW(_T("STOP HEAT/COOL"));
		w = this->GetDlgItem(IDC_BUTTON_COOL);
		w->SetWindowTextW(_T("FULL COOL"));
		w = this->GetDlgItem(IDC_BUTTON_STEP_COOL);
		w->SetWindowTextW(_T("LINE COOL"));
		w = this->GetDlgItem(IDC_BUTTON_PID_COOL);
		w->SetWindowTextW(_T("PID COOL"));
		w = this->GetDlgItem(IDC_BUTTON_RELAY_ON);
		w->SetWindowTextW(_T("RELAY ON"));
		w = this->GetDlgItem(IDC_BUTTON_RELAY_OFF);
		w->SetWindowTextW(_T("RELAY OFF"));

		w = this->GetDlgItem(IDC_BUTTON_UPDATE_FIRMWARE);
		w->SetWindowTextW(_T("UPDATE FIRMWARE"));

		w = this->GetDlgItem(IDC_BUTTON_PID_AUTOTUNE);
		w->SetWindowTextW(_T("PID AUTOTUNE"));

		w = this->GetDlgItem(IDC_BUTTON_COMM_SPEED);
		w->SetWindowTextW(_T("COMM SPEED"));
		
		w = this->GetDlgItem(IDC_STATIC_HEAT_Fixed_Ratio);
		w->SetWindowTextW(_T("Fixed Ratio"));
		
		w = this->GetDlgItem(IDC_STATIC_COOL_Fixed_Ratio);
		w->SetWindowTextW(_T("Fixed Ratio"));
		
		w = this->GetDlgItem(IDC_STATIC_Standard_Low_Temp);
		w->SetWindowTextW(_T("Standard Low"));
		w = this->GetDlgItem(IDC_STATIC_Standard_High_Temp);
		w->SetWindowTextW(_T("Standard High"));

		w = this->GetDlgItem(IDC_BUTTON_READ_CURRENT_METER_TEMP_LOW);
		w->SetWindowTextW(_T("Meter Low"));
		w = this->GetDlgItem(IDC_BUTTON_READ_CURRENT_METER_TEMP_HIGH);
		w->SetWindowTextW(_T("Meter High"));
		
		w = this->GetDlgItem(IDC_BUTTON_INIT_ADJUST_TEMP);
		w->SetWindowTextW(_T("INIT ADJUST TEMP"));
		

		w = this->GetDlgItem(IDC_BUTTON_CAL_AND_SAVE_ADJUST);
		w->SetWindowTextW(_T("Cal & Save Correction"));
		

		break;
	}
//	this->ShowWindow(SW_MAXIMIZE);

	this->DrawLED();

	CString s;  
	GetModuleFileName(NULL,s.GetBufferSetLength(MAX_PATH+1),MAX_PATH+1);  
    s.ReleaseBuffer();

	CFileStatus fs;
	if(CFile::GetStatus(s,fs))
	{
		CString m_CompileTime;
		
		m_CompileTime  =_T("Modify:") + fs.m_mtime.Format(_T("%Y/%m/%d %H:%M:%S"));  
		m_CompileTime +=_T(" Create:") + fs.m_ctime.Format(_T("%Y/%m/%d %H:%M:%S")); 
		m_CompileTime += _T(" "); 
		m_CompileTime +=_T("编译:");
		m_CompileTime += __DATE__; 
		m_CompileTime += _T(" "); 
		m_CompileTime += __TIME__; 

		this->SetWindowTextW(_T("调试 ") +  m_CompileTime);
	}

	this->UpdateData(false); 

	return true;
}

void CDebugDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
		m_bShowed = false;
}

void CDebugDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
	m_bShowed = false;
}

void CDebugDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bShowed = false;
	CDialog::OnClose();
}

void CDebugDlg::CalAdjustTemp(void)
{
  float delta_low = m_AutoCalGlassLow - m_AutoCalMeterLow; 
  float delta_high= m_AutoCalGlassHigh - m_AutoCalMeterHigh;

  float k = (m_AutoCalGlassHigh - m_AutoCalGlassLow) / (m_AutoCalMeterHigh - m_AutoCalMeterLow);
 
  m_WriteAdjustLow_1 =  m_AutoCalGlassLow - k * m_AutoCalMeterLow;
  m_WriteAdjustHigh_1 = (400 * k + m_WriteAdjustLow_1) - 400.0f;



}

void CDebugDlg::OnBnClickedButtonStopHeat()
{
	// TODO: 在此添加控件通知处理程序代码

	char para[16];

	this->UpdateData(true);

	CString cmd;

	para[0] = 0;

	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_RELAY,para));

	para[0] = (int)((this->m_FullOutTemp * 10)) % 256;		
	para[1] = (int)((this->m_FullOutTemp * 10)) / 256;

	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_STOP_OUT,para));
}

void CDebugDlg::OnBnClickedButtonCool()
{
	// TODO: 在此添加控件通知处理程序代码

	char para[16];

	this->UpdateData(true);

	CString cmd;

	para[0] = 1;
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_RELAY,para));

	memcpy(para,&m_FullCoolTemp,sizeof(m_FullCoolTemp));

	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_COOL_FULL_OUT,para));
}

void CDebugDlg::OnBnClickedButtonStepCool()
{
	// TODO: 在此添加控件通知处理程序代码

	char para[16];

	this->UpdateData(true);

	CString cmd;

	para[0] = 1;
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_RELAY,para));

	memcpy(para,&m_StepCoolTemp,sizeof(m_StepCoolTemp));
	*((short *)&para[sizeof(m_StepCoolTemp)]) = (short)(this->m_StepCoolMinutes * 60);
	memcpy(&para[sizeof(m_StepCoolTemp) + sizeof(short)],&m_CoolStepPercent,sizeof(short));
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_COOL_STEP_PID_OUT,para));
}

void CDebugDlg::OnBnClickedButtonPidCool()
{
	// TODO: 在此添加控件通知处理程序代码

	char para[16];

	this->UpdateData(true);

	
	CString cmd;

	para[0] = 1;
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_RELAY,para));

	memcpy(para,&m_PidCoolTemp,sizeof(m_PidCoolTemp));
	*((short *)&para[sizeof(m_PidCoolTemp)]) = 10;
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_COOL_PID_OUT,para));

}

void CDebugDlg::OnBnClickedButtonRelayOn()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];
	CString cmd;

	para[0] = 1;
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_RELAY,para));
}

void CDebugDlg::OnBnClickedButtonRelayOff()
{
	// TODO: 在此添加控件通知处理程序代码
	char para[16];
	CString cmd;

	para[0] = 0;
	G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_SET_RELAY,para));
}

void CDebugDlg::OnBnClickedButtonUpdateFirmware()
{
	// TODO: 在此添加控件通知处理程序代码
	G_MainDlg->XModem_Start(0); 
}


void CDebugDlg::DrawLED(void)
{
//#define TEMP_STRING_HEIGHT	160	//非图标区
//return;
	int i,j,k;
	CFile log_file;
	


	if(m_bJiajuTesting)
	{
		DrawLED_JiaJu();
		return;
	}

	return;

	if(!this->IsWindowVisible()) 
	{
		return;
	}


	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 CRect rect(0,00,70,52);
	 
	 CBrush groundbrush(COLORREF(RGB(0x00,0xA0,0x80)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush greenbrush(COLORREF(RGB(0,0,0xFF)));
	 CBrush graybrush(COLORREF(RGB(0,0,0)));
	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	CString m_CalMsg;

	//绘制准备
	dc=this->GetWindowDC();
		

	if(dc == NULL)
	{
		m_CalMsg.Format(_T("dc == NULL %s %d\r\n"),__FILE__,__LINE__);
		//this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
			log_file.Close();
		}
		return;
	}
	
	CWnd *data_area = GetDlgItem(IDC_EDIT_READ_ADJUST_LOW2);
	CRect wndRect,data_area_Rect;
	data_area->GetClientRect(data_area_Rect);
	
	this->GetClientRect(wndRect);
	nWidth	= wndRect.right / 2;
	nHeight = wndRect.bottom -data_area_Rect.bottom;//wndRect.top -TEMP_STRING_HEIGHT;

	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		m_CalMsg.Format(_T("!MemDC.CreateCompatibleDC(dc)%s %d\r\n"),__FILE__,__LINE__);
		this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
			log_file.Close();
		}
		ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			m_CalMsg.Format(_T("!bitmap.DeleteObject()%s %d\r\n"),__FILE__,__LINE__);
			//this->UpdateData(false);
			if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
			{
				log_file.SeekToEnd();
				log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
				log_file.Close();
			}
			ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth,nHeight))	//建立与显示设备兼容的位图
	{
		m_CalMsg.Format(_T("!bitmap.CreateCompatibleBitmap%s %d\r\n"),__FILE__,__LINE__);
		//this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
			log_file.Close();
		}
		ReleaseDC(dc);
		bitmap.DeleteObject(); 
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	2	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0,0));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);
	//MemDC.SetBkColor(RGB(255,40,40));
	MemDC.SetTextColor(RGB(255,255,255));
	
	int Ellipse_W_Space	= 2;
	
	int G_iLedNrPerLine = 10;
	int G_iLedLines		= 13;
	int	G_iMaxLedNr		= 128;

	int Ellipse_W = (nWidth - PEN_WEIGHT *2) / G_iLedNrPerLine - Ellipse_W_Space;
	int Ellipse_H = (nHeight - PEN_WEIGHT *2) / G_iLedLines - Ellipse_W_Space;//Ellipse_W;
	
	int Ellipse_W_GAP = Ellipse_W;

	if(Ellipse_W < Ellipse_H)
	{
		Ellipse_H = Ellipse_W;
	}
	else
	{
		Ellipse_W = Ellipse_H;
	}

	int Ellipse_H_Space = nHeight / G_iLedLines - Ellipse_H - 1;
  
	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷

	//for(k=0;k<G_CurMaxHeatBox;k++)
	k = 0;
	{
		MemDC.RoundRect(PEN_WEIGHT + nWidth * k,PEN_WEIGHT,nWidth-PEN_WEIGHT*3 + nWidth * k + 4,nHeight-PEN_WEIGHT*5 ,10,10);
	}

	MemDC.SelectObject(&graybrush);				//选择画刷
	j = 0 ;
//#ifndef _HANJIE_192
	//for(j=0;j<MAX_HEAT_BOX;j++)
	{
		for(i=0; i < G_iMaxLedNr /* MAX_HEAT_BOX*/; i++)
		{//默认的全部是灰色
					rect.left =  nWidth * j + (i % G_iLedNrPerLine)*(Ellipse_W_GAP+Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
					rect.top = 5+(i / G_iLedNrPerLine)*(Ellipse_H + Ellipse_H_Space)+Ellipse_H_Space / 2;
					rect.right = rect.left + Ellipse_W;
					rect.bottom = rect.top + Ellipse_H;
					
					if(G_MainDlg->m_SwitchStatus[0][i].IsClosing == false)
					{
						MemDC.Ellipse(rect);		//填充背景
					}
		
		}
	}	
//#endif
	MemDC.SelectObject(&redbrush);				//选择画刷


	k = 0;
	{
		for(i=0; i< G_iLedNrPerLine; i++)
		{
			for(j=0; j < G_iLedLines; j++)
			{
				if((j * G_iLedNrPerLine + i) >= G_iMaxLedNr)
				{
				}
				else
				{
//#ifdef _HANJIE_192
//					if(m_SwitchTestResult[m_BoxNr][11 - i + j * G_iLedNrPerLine ].IsClosing)
//#else
					if(G_MainDlg->m_SwitchStatus[0][     i + j * G_iLedNrPerLine ].IsClosing)
//#endif

					{//闭合就是红色
						rect.left =  nWidth * k + i * (Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
						rect.top =  5+j*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2;
						rect.right = rect.left + Ellipse_W;
						rect.bottom = rect.top + Ellipse_H;
						MemDC.Ellipse(rect);		//填充背景
					}
				}
			}
		}
	}


	if(G_iMaxLedNr == 192)
	{
//#ifdef _HANJIE_192
//	CFont font;
//	if(font.CreatePointFont(250,_T("黑体"),&MemDC))
//	{
//		CFont *oldfont = MemDC.SelectObject(&font); 
//#endif				
				
		k = 0;
		{
			for(i=0; i < G_iMaxLedNr; i++)
			{
					CString nr;\
					if(((i + 1) % G_iLedNrPerLine) == 0)
					{
						nr.Format(_T("%dB"),(i + 1) / G_iLedNrPerLine );
					}
					else if(((i + 1) % G_iLedNrPerLine) < 10)
					{
						nr.Format(_T("%d%d"),(i+1)/G_iLedNrPerLine,(i+1)%G_iLedNrPerLine);
					}
					else if(((i + 1) % G_iLedNrPerLine) == 10)
					{
						nr.Format(_T("%d0"),(i+1)/G_iLedNrPerLine+1);
					}
					else if(((i + 1) % G_iLedNrPerLine) == 11)
					{
						nr.Format(_T("%dA"),(i + 1) / G_iLedNrPerLine + 1);
					}
					
					//nr.Format("%02d%c",((i+1) / G_iLedNrPerLine), nr_low[(i % G_iLedNrPerLine)]);
					CSize size = MemDC.GetTextExtent(nr);
					MemDC.TextOut(5 + nWidth * k + (i % G_iLedNrPerLine) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / G_iLedNrPerLine)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
			}
//#ifdef _HANJIE_192		
//		}

//		MemDC.SelectObject(oldfont); 
//		font.DeleteObject(); 
//#endif
	}
	}
	else
	{
		k = 0;
		{
			for(i=0; i < G_iMaxLedNr; i++)
			{
					CString nr;
					nr.Format(_T("%02d"),i+1);
					CSize size = MemDC.GetTextExtent(nr);
					MemDC.TextOut( 5 + nWidth * k + (i % G_iLedNrPerLine) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / G_iLedNrPerLine)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
					/*CRect rect;
					rect.left =5+(i % G_iLedNrPerLine) *(Ellipse_W + Ellipse_W_Space);
					rect.top  =5+(i / G_iLedNrPerLine)*((Ellipse_H + Ellipse_H_Space));
					rect.right = rect.left + Ellipse_W;
					rect.bottom= rect.top + Ellipse_H;
					
					MemDC.DrawText(nr,rect,DT_CENTER | DT_VCENTER);
					*/
					
			}
		}	
	}

	dc->BitBlt(nWidth,data_area_Rect.bottom + PEN_WEIGHT * 5 ,nWidth + 30 ,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
	//this->UpdateData(false); 


	MemDC.SelectObject(oldbrush);
	if(redbrush.GetSafeHandle() != NULL)
	{
		redbrush.DeleteObject();
	}
	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}
	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}
	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}
	
	ReleaseDC(dc); 
}

bool CDebugDlg::CheckJiaJuPass(unsigned char ID)
{
	if(ID < 16)
	{
		for(int i = 0; i < 64; i++)
		{
			if((i % 16) == ID)
			{
				if(::G_MainDlg->m_SwitchStatus[0][i].IsClosing == false)
				{
					return false;
				}
			}
			else
			{
				if(G_MainDlg->m_SwitchStatus[0][i].IsClosing == true)
				{
					return false;
				}
			}
		}
	}
	else
	{
		for(int i = 64; i < 128; i++)
		{
			if((i % 16) == (ID - 16))
			{
				if(G_MainDlg->m_SwitchStatus[0][i].IsClosing == false)
				{
					return false;
				}
			}
			else
			{
				if(G_MainDlg->m_SwitchStatus[0][i].IsClosing == true)
				{
					return false;
				}
			}
		}
	}
	m_JiaJuTested[ID] = 1;

	for(int i = 0; i < 32; i++)
	{
		if(m_JiaJuTested[i] != 1)
		{
			return false;
		}
	}
	AfxMessageBox(_T("夹具测试成功!!!"));
	m_bJiajuTesting = false;
	this->ShowWindow(SW_SHOWNORMAL);
	
	return true;
}

void CDebugDlg::DrawLED_JiaJu(void)
{
	int i,j,k;
	CFile log_file;
	

	
	if(!this->IsWindowVisible()) 
	{
		return;
	}

	for(i = 0; i < 32; i++)
	{
		CheckJiaJuPass(i);
	}


	 CBitmap bitmap;	
	 CBitmap* pOldBitmap;
	 CDC MemDC,*dc;
	 CRect rect(0,00,70,52);
	 
	 CBrush groundbrush(COLORREF(RGB(0x00,0xA0,0x80)));
	 CBrush redbrush(COLORREF(RGB(0xFF,0,0)));
	 CBrush greenbrush(COLORREF(RGB(0,0,0xFF)));
	 CBrush graybrush(COLORREF(RGB(0,0,0)));
	 CBrush* oldbrush;
	
	//绘制工具资源
	 CPen	pen;
	 CPen*	oldpen;
	int nWidth;
    int nHeight;

	//绘制准备
	dc=this->GetWindowDC();
		
	CString m_CalMsg;

	if(dc == NULL)
	{
		m_CalMsg.Format(_T("dc == NULL %s %d\r\n"),__FILE__,__LINE__);
		//this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
			log_file.Close();
		}
		return;
	}
	
	CWnd *data_area = GetDlgItem(IDC_EDIT_STD_TEMP_HIGH);
	CRect wndRect,data_area_Rect;
	data_area->GetClientRect(data_area_Rect);
	
	this->GetClientRect(wndRect);
	nWidth	= wndRect.right  / 3;
	nHeight = wndRect.bottom - data_area_Rect.bottom;//wndRect.top -TEMP_STRING_HEIGHT;

	if(!MemDC.CreateCompatibleDC(dc)) 						//建立与显示设备兼容的内存设备场境
	{
		m_CalMsg.Format(_T("!MemDC.CreateCompatibleDC(dc)%s %d\r\n"),__FILE__,__LINE__);
		//this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
			log_file.Close();
		}
		ReleaseDC(dc);
		return;
	}
	

	if(bitmap.GetSafeHandle() != NULL)
	{
		if(!bitmap.DeleteObject())
		{
			m_CalMsg.Format(_T("!bitmap.DeleteObject()%s %d\r\n"),__FILE__,__LINE__);
			//this->UpdateData(false);
			if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
			{
				log_file.SeekToEnd();
				log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
				log_file.Close();
			}
			ReleaseDC(dc);
			return;
		}
	}
	if(!bitmap.CreateCompatibleBitmap(dc,nWidth ,nHeight))	//建立与显示设备兼容的位图
	{
		m_CalMsg.Format(_T("!bitmap.CreateCompatibleBitmap%s %d\r\n"),__FILE__,__LINE__);
		//this->UpdateData(false);
		if(log_file.Open(theAppDirectory+_T("log.txt"),CFile::modeReadWrite | CFile::modeCreate | CFile::modeNoTruncate))
		{
			log_file.SeekToEnd();
			log_file.Write(m_CalMsg,m_CalMsg.GetLength()); 
			log_file.Close();
		}
		ReleaseDC(dc);
		bitmap.DeleteObject(); 
	}
	pOldBitmap=MemDC.SelectObject(&bitmap); 			//将位图选入内存场境

#define PEN_WEIGHT	2	
	pen.CreatePen(PS_SOLID,PEN_WEIGHT, RGB(0,0,0));  		//建立画笔
	oldpen=MemDC.SelectObject(&pen);					//选择画笔对象
	
	MemDC.SetBkMode(TRANSPARENT);
	//MemDC.SetBkColor(RGB(255,40,40));
	MemDC.SetTextColor(RGB(255,255,255));
	
	int Ellipse_W_Space	= 2;
	
	int Ellipse_W = (nWidth - PEN_WEIGHT *2) / 16 - Ellipse_W_Space;
	int Ellipse_H = (nHeight - PEN_WEIGHT *2) / 8 - Ellipse_W_Space;//Ellipse_W;
	
	int Ellipse_W_GAP = Ellipse_W;

	if(Ellipse_W < Ellipse_H)
	{
		Ellipse_H = Ellipse_W;
	}
	else
	{
		Ellipse_W = Ellipse_H;
	}

	int G_iLedNrPerLine = 10;
	int G_iLedLines		= 13;
	int	G_iMaxLedNr		= 128;

	int Ellipse_H_Space = nHeight / G_iLedLines - Ellipse_H - 1;
  
	//background
	oldbrush=MemDC.SelectObject(&groundbrush);				//选择画刷

	//for(k=0;k<G_CurMaxHeatBox;k++)
	k = 0;
	{
		MemDC.RoundRect(PEN_WEIGHT + nWidth * k,PEN_WEIGHT,nWidth-PEN_WEIGHT*3 + nWidth * k + 4,nHeight-PEN_WEIGHT*5 ,10,10);
	}

	MemDC.SelectObject(&graybrush);				//选择画刷
	j = 0 ;
//#ifndef _HANJIE_192
	//for(j=0;j<MAX_HEAT_BOX;j++)
	{
		for(i=0; i < G_iMaxLedNr ; i++)
		{//默认的全部是灰色
					rect.left =  nWidth * j + (i % 16)*(Ellipse_W_GAP+Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
					rect.top = 5+(i / 16)*(Ellipse_H + Ellipse_H_Space)+Ellipse_H_Space / 2;
					rect.right = rect.left + Ellipse_W;
					rect.bottom = rect.top + Ellipse_H;
					if(::G_MainDlg->m_SwitchStatus[0][i].IsClosing == false)
					{
						MemDC.Ellipse(rect);		//填充背景
					}
		
		}
	}	
//#endif
	MemDC.SelectObject(&redbrush);				//选择画刷


	k = 0;
	{
		for(i=0; i< 16; i++)
		{
			for(j=0; j < 8; j++)
			{
				if((j * 16 + i) >= G_iMaxLedNr)
				{
				}
				else
				{
					if(G_MainDlg->m_SwitchStatus[0][     i + j * 16 ].IsClosing)

					{//闭合就是红色
						rect.left =  nWidth * k + i * (Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space / 2 + 5;
						rect.top =  5+j*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2;
						rect.right = rect.left + Ellipse_W;
						rect.bottom = rect.top + Ellipse_H;
						MemDC.Ellipse(rect);		//填充背景
					}
				}
			}
		}
	}


	
	{
		k = 0;
		{
			for(i=0; i < G_iMaxLedNr; i++)
			{
				int id;
				if(i < 64)
				{
					id = (i % 16);
				}
				else
				{
					id = 16 + (i % 16);
				}
				if(m_JiaJuTested[id] ==  0)
				{
					CString nr;
					nr.Format(_T("%02d"),i+1);
					CSize size = MemDC.GetTextExtent(nr);
					MemDC.TextOut( 5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
				}
				else
				{
					CString nr = _T("√");
					CSize size = MemDC.GetTextExtent(nr);
					MemDC.TextOut( 5 + nWidth * k + (i % 16) *(Ellipse_W_GAP + Ellipse_W_Space) + Ellipse_W_Space/2 +(Ellipse_W-size.cx)/2,	5 + (i / 16)*(Ellipse_H + Ellipse_H_Space) + Ellipse_H_Space / 2 +(Ellipse_H-size.cy)/2,nr);
				
				}
					
			}
		}	
	}

	dc->BitBlt(nWidth,data_area_Rect.bottom + PEN_WEIGHT * 5 ,nWidth + 30 ,nHeight,&MemDC,0,0,SRCCOPY); 	//显示原图形
	//this->UpdateData(false); 


	MemDC.SelectObject(oldbrush);
	if(redbrush.GetSafeHandle() != NULL)
	{
		redbrush.DeleteObject();
	}
	if(graybrush.GetSafeHandle() != NULL)
	{
		graybrush.DeleteObject(); 
	}
	MemDC.SelectObject(pOldBitmap);
	if(bitmap.GetSafeHandle() != NULL)
	{
		bitmap.DeleteObject(); 
	}
	MemDC.SelectObject(oldpen);
	if(pen.GetSafeHandle() != NULL)
	{
		pen.DeleteObject();
	}
	if(MemDC.GetSafeHdc() != NULL)
	{
		MemDC.DeleteDC();
	}
	
	ReleaseDC(dc); 
		return;
  
}
void CDebugDlg::OnBnClickedCheckJiajuTest()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bJiajuTesting = true;

	this->ShowWindow(SW_SHOWMAXIMIZED);
	
	memset(this->m_JiaJuTested,0,sizeof( m_JiaJuTested));

}

void CDebugDlg::OnBnClickedButtonReadCurrentMeterTempLow()
{
	// TODO: 在此添加控件通知处理程序代码
	CString text;
	CWnd * wnd = this->GetDlgItem(IDC_EDIT_CUR_TEMP);
	wnd->GetWindowTextW(text);
	wnd = this->GetDlgItem(IDC_EDIT_METER_TEMP_LOW);
	wnd->SetWindowTextW(text);
}

void CDebugDlg::OnBnClickedButtonReadCurrentMeterTempHigh()
{
	// TODO: 在此添加控件通知处理程序代码
	CString text;
	CWnd * wnd = this->GetDlgItem(IDC_EDIT_CUR_TEMP);
	wnd->GetWindowTextW(text);
	wnd = this->GetDlgItem(IDC_EDIT_METER_TEMP_HIGH);
	wnd->SetWindowTextW(text);
}

void CDebugDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码


	double glass_low,glass_high;
	double meter_low,meter_high;
	double adjust_low,adjust_high;

	CString s;
	CWnd *w;

	w = this->GetDlgItem(IDC_EDIT_STD_TEMP_LOW);  
	w->GetWindowTextW(s);
	glass_low = _wtof(s);

	w = this->GetDlgItem(IDC_EDIT_STD_TEMP_HIGH);  
	w->GetWindowTextW(s);
	glass_high = _wtof(s);

	w = this->GetDlgItem(IDC_EDIT_METER_TEMP_LOW);  
	w->GetWindowTextW(s);
	meter_low = _wtof(s);

	w = this->GetDlgItem(IDC_EDIT_METER_TEMP_HIGH);  
	w->GetWindowTextW(s);
	meter_high = _wtof(s);


	//记录一个低温点，一个高温点，根据线性方程求0,200度的误差
	//低温点一般取常温，高温点最好在150度，要分别恒温 
	/*s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_InputGlassLow[0] ,m_InputGlassLow[1] ,m_InputGlassLow[2] ,m_InputGlassLow[3]);
	AfxMessageBox(s);

	s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_InputGlassHigh[0] ,m_InputGlassHigh[1] ,m_InputGlassHigh[2] ,m_InputGlassHigh[3]);
	AfxMessageBox(s);

		s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_MeterLow[0] ,m_MeterLow[1] ,m_MeterLow[2] ,m_MeterLow[3]);
	AfxMessageBox(s);

		s.Format(_T("%5.1f,%5.1f,%5.1f,%5.1f"),m_MeterHigh[0] ,m_MeterHigh[1] ,m_MeterHigh[2] ,m_MeterHigh[3]);
	AfxMessageBox(s);
*/
	

	/*
	m_MeterLow[0]  = 60;
	m_MeterHigh[0] = 140;
	glass_low_avr = 58;
	glass_high_avr = 130;
*/
	
	{
		// y = kx + delta
		double delta_low	= (glass_low	- meter_low); 
		double delta_high	= (glass_high	- meter_high);
		
		double k = (glass_high - glass_low)/(meter_high - meter_low);
		
		double delta = glass_low - k *meter_low;
		
		
		adjust_low = delta ;
		s.Format(_T("%6.2f"),adjust_low); 	
		w =GetDlgItem(IDC_EDIT_WRITE_ADJUST_LOW_1);
		w->SetWindowTextW(s); 

		//adjust_high = ((int)((400.0*k + delta) * 10.0f + 0.5))/10.0f - 200.0f;
		adjust_high = (400.0*k + delta) - 400.0f;
		s.Format(_T("%6.2f"),adjust_high); 	
		w =GetDlgItem(IDC_EDIT_WRITE_ADJUST_HIGH_1);
		w->SetWindowTextW(s); 

		OnBnClickedButtonWriteAdjustLow1();
		
		OnBnClickedButtonWriteAdjustHigh();
	}

}

void CDebugDlg::OnBnClickedButtonInitAdjustTemp()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w;
	w =GetDlgItem(IDC_EDIT_WRITE_ADJUST_LOW_1);
	w->SetWindowTextW(_T("0.0")); 

	w =GetDlgItem(IDC_EDIT_WRITE_ADJUST_HIGH_1);
	w->SetWindowTextW(_T("0.0")); 

	OnBnClickedButtonWriteAdjustLow1();
		
	OnBnClickedButtonWriteAdjustHigh();

	G_MainDlg->SendCmdSetAdjustAll(0.0f);

	G_MainDlg->SendCmdReadAdjustAll();

}

void CDebugDlg::OnBnClickedButtonPidAutotune()
{
	// TODO: 在此添加控件通知处理程序代码

	char para[16];

	this->UpdateData(true);

	CString cmd;
	
	memset(para,0,sizeof(para));
	memcpy(para,&m_AT_Temp,sizeof(m_AT_Temp));

	::G_MainDlg->m_SendCmdArray.Add(G_MainDlg->SetCmd('1', CMD_AT,para));
}

void CDebugDlg::OnBnClickedButtonCommSpeed()
{
	// TODO: 在此添加控件通知处理程序代码

	this->UpdateData(); 
	 ::G_NormalConfigPara.CommSpeedTimer = this->m_CommSpeed; 
	 ::G_MainDlg->SaveConfig();
	G_MainDlg->KillTimer(1);
	G_MainDlg->SetTimer(1,G_NormalConfigPara.CommSpeedTimer,NULL); 
}

void CDebugDlg::OnEnSetfocusEditCommSpeed()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_bSetFocusOnEditCurTemp == false)
	{
		return;
	}
	m_bSetFocusOnEditCurTemp = false;

	CNumInputDlg dlg;
	
	
	switch(::G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = _T("通讯间隔");
		dlg.m_Msg[0] =_T("通讯间隔"); 
		dlg.m_Msg[1] =_T("");
		dlg.m_Msg[2] =_T(""); 
		break;
	case 1:
		dlg.m_Title = _T("COMM TIMER");
		dlg.m_Msg[0] =_T("COMM TIMER"); 
		dlg.m_Msg[1] =_T(""); 
		dlg.m_Msg[2] =_T(""); 
		break;
	}

	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		::G_NormalConfigPara.CommSpeedTimer = _wtof(buf);
		
	    this->m_CommSpeed = ::G_NormalConfigPara.CommSpeedTimer; 
		
		CWnd *w = this->GetDlgItem(IDC_EDIT_COMM_SPEED);
		w->SetWindowTextW(buf); 

		::G_MainDlg->SaveConfig();

		G_MainDlg->KillTimer(1);
		G_MainDlg->SetTimer(1,G_NormalConfigPara.CommSpeedTimer,NULL); 
		
	}

	CWnd *w = this->GetDlgItem(IDC_EDIT_CUR_TEMP); 
	w->SetFocus(); 
}

void CDebugDlg::OnEnSetfocusEditCurTemp()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bSetFocusOnEditCurTemp = true;
}

void CDebugDlg::OnEnSetfocusEditFullOutTemp()
{
	// TODO: 在此添加控件通知处理程序代码

	if(m_bSetFocusOnEditCurTemp == false)
	{
		return;
	}
	m_bSetFocusOnEditCurTemp = false;

	CNumInputDlg dlg;
	
	
	switch(::G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = _T("全速加热");
		dlg.m_Msg[0] =_T("全速加热"); 
		dlg.m_Msg[1] =_T("");
		dlg.m_Msg[2] =_T(""); 
		break;
	case 1:
		dlg.m_Title = _T("FULL OUT");
		dlg.m_Msg[0] =_T("FULL OUT"); 
		dlg.m_Msg[1] =_T(""); 
		dlg.m_Msg[2] =_T(""); 
		break;
	}

	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		
		CWnd *w = this->GetDlgItem(IDC_EDIT_FULL_OUT_TEMP);
		w->SetWindowTextW(buf); 
		OnBnClickedButtonFullHeat();
	}

	CWnd *w = this->GetDlgItem(IDC_EDIT_CUR_TEMP); 
	w->SetFocus(); 
}

void CDebugDlg::OnEnSetfocusEditPidOut()
{
	// TODO: 在此添加控件通知处理程序代码
	


	if(m_bSetFocusOnEditCurTemp == false)
	{
		return;
	}
	m_bSetFocusOnEditCurTemp = false;

	CNumInputDlg dlg;
	
	
	switch(::G_NormalConfigPara.Language)
	{
	case 0:
		dlg.m_Title = _T("恒温温度");
		dlg.m_Msg[0] =_T("恒温温度"); 
		dlg.m_Msg[1] =_T("");
		dlg.m_Msg[2] =_T(""); 
		break;
	case 1:
		dlg.m_Title = _T("PID");
		dlg.m_Msg[0] =_T("PID"); 
		dlg.m_Msg[1] =_T(""); 
		dlg.m_Msg[2] =_T(""); 
		break;
	}

	if(dlg.DoModal() == IDOK)
	{
		TCHAR *buf;
		buf = dlg.m_Input[0].GetBuffer(dlg.m_Input[0].GetLength()); 
		
		CWnd *w = this->GetDlgItem(IDC_EDIT_PID_OUT);
		w->SetWindowTextW(buf); 
		OnBnClickedButtonFullHeat();
	}

	CWnd *w = this->GetDlgItem(IDC_EDIT_CUR_TEMP); 
	w->SetFocus(); 
}
