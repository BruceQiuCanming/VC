// ParaDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HotLong_PCB.h"
#include "HotLong_PCBDlg.h"
#include "ParaDlg.h"


// CParaDlg 对话框

IMPLEMENT_DYNAMIC(CParaDlg, CDialog)

CParaDlg::CParaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CParaDlg::IDD, pParent)
	, m_SendTxt_PLC(_T(""))
	, m_RecTxt_PLC(_T(""))
{

}

CParaDlg::~CParaDlg()
{
}

void CParaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_COMM_AGING_1, m_Combo_COMM_AGING_1);
	DDX_Control(pDX, IDC_COMBO_COMM_AGING_2, m_Combo_COMM_AGING_2);
	DDX_Control(pDX, IDC_COMBO_COMM_AGING_3, m_Combo_COMM_AGING_3);
	DDX_Control(pDX, IDC_COMBO_COMM_AGING_4, m_Combo_COMM_AGING_4);
	DDX_Control(pDX, IDC_COMBO_COMM_AGING_5, m_Combo_COMM_AGING_5);
	DDX_Control(pDX, IDC_COMBO_COMM_AGING_6, m_Combo_COMM_AGING_6);
	DDX_Control(pDX, IDC_COMBO_COMM_AGING_7, m_Combo_COMM_AGING_7);
	DDX_Control(pDX, IDC_COMBO_COMM_AGING_8, m_Combo_COMM_AGING_8);
	DDX_Control(pDX, IDC_COMBO_PLC_COMM,	 m_Combo_PLC_COMM);
	DDX_Control(pDX, IDC_COMBO_PROGRAM_COMM,	 m_Combo_PROGRAM_COMM);

	DDX_Control(pDX, IDC_COMBO_CAMERA, m_Combo_Camera);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_AGING_1, m_Combo_Aging_Power_1);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_AGING_2, m_Combo_Aging_Power_2);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_AGING_3, m_Combo_Aging_Power_3);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_AGING_4, m_Combo_Aging_Power_4);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_AGING_5, m_Combo_Aging_Power_5);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_AGING_6, m_Combo_Aging_Power_6);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_AGING_7, m_Combo_Aging_Power_7);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_AGING_8, m_Combo_Aging_Power_8);
	DDX_Text(pDX, IDC_EDIT_PLC_SEND, m_SendTxt_PLC);
	DDX_Text(pDX, IDC_EDIT_PLC_REC, m_RecTxt_PLC);

	DDX_Text(pDX, IDC_EDIT_PROGRAM_SEND, m_SendTxt_HotLong[0]);
	DDX_Text(pDX, IDC_EDIT_AGING_SEND_1, m_SendTxt_HotLong[1]);
	DDX_Text(pDX, IDC_EDIT_AGING_SEND_2, m_SendTxt_HotLong[2]);
	DDX_Text(pDX, IDC_EDIT_AGING_SEND_3, m_SendTxt_HotLong[3]);
	DDX_Text(pDX, IDC_EDIT_AGING_SEND_4, m_SendTxt_HotLong[4]);
	DDX_Text(pDX, IDC_EDIT_AGING_SEND_5, m_SendTxt_HotLong[5]);
	DDX_Text(pDX, IDC_EDIT_AGING_SEND_6, m_SendTxt_HotLong[6]);
	DDX_Text(pDX, IDC_EDIT_AGING_SEND_7, m_SendTxt_HotLong[7]);
	DDX_Text(pDX, IDC_EDIT_AGING_SEND_8, m_SendTxt_HotLong[8]);

	DDX_Text(pDX, IDC_EDIT_PROGRAM_REC, m_RecTxt_HotLong[0]);
	DDX_Text(pDX, IDC_EDIT_AGING_REC_1, m_RecTxt_HotLong[1]);
	DDX_Text(pDX, IDC_EDIT_AGING_REC_2, m_RecTxt_HotLong[2]);
	DDX_Text(pDX, IDC_EDIT_AGING_REC_3, m_RecTxt_HotLong[3]);
	DDX_Text(pDX, IDC_EDIT_AGING_REC_4, m_RecTxt_HotLong[4]);
	DDX_Text(pDX, IDC_EDIT_AGING_REC_5, m_RecTxt_HotLong[5]);
	DDX_Text(pDX, IDC_EDIT_AGING_REC_6, m_RecTxt_HotLong[6]);
	DDX_Text(pDX, IDC_EDIT_AGING_REC_7, m_RecTxt_HotLong[7]);
	DDX_Text(pDX, IDC_EDIT_AGING_REC_8, m_RecTxt_HotLong[8]);



	DDX_Control(pDX, IDC_COMBO_PCB_TYPE, m_Combo_PCB_Type);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_TEST_NEEDLE, m_Combo_TEST_NEEDLE);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_TEST_START, m_Combo_TEST_START);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_SET_KEY, m_Combo_SET_KEY);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_PLUS_KEY, m_Combo_PLUS_KEY);
	DDX_Control(pDX, IDC_COMBO_PLC_Y_MINUS_KEY, m_Combo_MINUS_KEY);
}


BEGIN_MESSAGE_MAP(CParaDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CParaDlg::OnBnClickedOk)
	ON_CBN_SELENDOK(IDC_COMBO_CAMERA, &CParaDlg::OnCbnSelendokComboCamera)
	ON_CBN_SELENDOK(IDC_COMBO_COMM_AGING_1, &CParaDlg::OnCbnSelendokCombo_AGING_Comm1)
	ON_CBN_SELENDOK(IDC_COMBO_COMM_AGING_2, &CParaDlg::OnCbnSelendokCombo_AGING_Comm2)
	ON_CBN_SELENDOK(IDC_COMBO_COMM_AGING_3, &CParaDlg::OnCbnSelendokCombo_AGING_Comm3)
	ON_CBN_SELENDOK(IDC_COMBO_COMM_AGING_4, &CParaDlg::OnCbnSelendokCombo_AGING_Comm4)
	ON_CBN_SELENDOK(IDC_COMBO_COMM_AGING_5, &CParaDlg::OnCbnSelendokCombo_AGING_Comm5)
	ON_CBN_SELENDOK(IDC_COMBO_COMM_AGING_6, &CParaDlg::OnCbnSelendokCombo_AGING_Comm6)
	ON_CBN_SELENDOK(IDC_COMBO_COMM_AGING_7, &CParaDlg::OnCbnSelendokCombo_AGING_Comm7)
	ON_CBN_SELENDOK(IDC_COMBO_COMM_AGING_8, &CParaDlg::OnCbnSelendokCombo_AGING_Comm8)

	ON_CBN_SELENDOK(IDC_COMBO_PLC_COMM, &CParaDlg::OnCbnSelendokCombo_PLC_Comm)
	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_AGING_1, &CParaDlg::OnCbnSelendokComboPlcY_AGING_1)
	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_AGING_2, &CParaDlg::OnCbnSelendokComboPlcY_AGING_2)

	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_AGING_3, &CParaDlg::OnCbnSelendokComboPlcY_AGING_3)

	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_AGING_4, &CParaDlg::OnCbnSelendokComboPlcY_AGING_4)
	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_AGING_5, &CParaDlg::OnCbnSelendokComboPlcY_AGING_5)
	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_AGING_6, &CParaDlg::OnCbnSelendokComboPlcY_AGING_6)
	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_AGING_7, &CParaDlg::OnCbnSelendokComboPlcY_AGING_7)
	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_AGING_8, &CParaDlg::OnCbnSelendokComboPlcY_AGING_8)



	ON_CBN_SELENDOK(IDC_COMBO_PCB_TYPE, &CParaDlg::OnCbnSelendokComboPcbType)
	ON_CBN_SELENDOK(IDC_COMBO_PROGRAM_COMM, &CParaDlg::OnCbnSelendokComboProgramComm)
	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_TEST_NEEDLE, &CParaDlg::OnCbnSelendokComboPlcYTestNeedle)
	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_TEST_START, &CParaDlg::OnCbnSelendokComboPlcYTestStart)
	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_SET_KEY, &CParaDlg::OnCbnSelendokComboPlcYSetKey)
	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_PLUS_KEY, &CParaDlg::OnCbnSelendokComboPlcYPlusKey)
	ON_CBN_SELENDOK(IDC_COMBO_PLC_Y_MINUS_KEY, &CParaDlg::OnCbnSelendokComboPlcYMinusKey)
END_MESSAGE_MAP()

void EnumSerialPortFriendlyNames(CStringArray& portList);
extern CONFIG_DATA	m_ConfigData;
// CParaDlg 消息处理程序
BOOL CParaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	this->m_Combo_PCB_Type.ResetContent();
	this->m_Combo_PCB_Type.AddString(_T("大电流"));
	this->m_Combo_PCB_Type.AddString(_T("大电流 无条码"));
	this->m_Combo_PCB_Type.AddString(_T("X90"));
	this->m_Combo_PCB_Type.AddString(_T("X90 无条码"));
	this->m_Combo_PCB_Type.AddString(_T("X10"));
	this->m_Combo_PCB_Type.AddString(_T("X10 无条码"));

	m_Combo_PCB_Type.SetCurSel(m_ConfigData.m_Pcb_Type);

	this->m_Combo_Aging_Power_1.ResetContent();
	this->m_Combo_Aging_Power_2.ResetContent();
	this->m_Combo_Aging_Power_3.ResetContent();
	this->m_Combo_Aging_Power_4.ResetContent();
	this->m_Combo_Aging_Power_5.ResetContent();
	this->m_Combo_Aging_Power_6.ResetContent();
	this->m_Combo_Aging_Power_7.ResetContent();
	this->m_Combo_Aging_Power_8.ResetContent();
	
	for(int i = 1; i <= 16; i++)
	{
		CString s;
		s.Format(_T("Y%d"),i);
		m_Combo_Aging_Power_1.AddString(s);
		m_Combo_Aging_Power_2.AddString(s);
		m_Combo_Aging_Power_3.AddString(s);
		m_Combo_Aging_Power_4.AddString(s);
		m_Combo_Aging_Power_5.AddString(s);
		m_Combo_Aging_Power_6.AddString(s);
		m_Combo_Aging_Power_7.AddString(s);
		m_Combo_Aging_Power_8.AddString(s);
		this->m_Combo_TEST_NEEDLE.AddString(s);
		this->m_Combo_TEST_START.AddString(s);
		this->m_Combo_SET_KEY.AddString(s);
		this->m_Combo_MINUS_KEY.AddString(s);
		this->m_Combo_PLUS_KEY.AddString(s);
	}
	if(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_1 >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_1 < 16)
	{
		m_Combo_Aging_Power_1.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_1);
	}
	if(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_2 >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_2 < 16)
	{
		m_Combo_Aging_Power_2.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_2);
	}
	if(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_3 >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_3 < 16)
	{
		m_Combo_Aging_Power_3.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_3);
	}

	if(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_4 >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_4 < 16)
	{
		m_Combo_Aging_Power_4.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_4);
	}

	if(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_5 >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_5 < 16)
	{
		m_Combo_Aging_Power_5.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_5);
	}

	if(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_6 >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_6 < 16)
	{
		m_Combo_Aging_Power_6.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_6);
	}
	if(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_7 >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_7 < 16)
	{
		m_Combo_Aging_Power_7.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_7);
	}

	if(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_8 >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_8 < 16)
	{
		m_Combo_Aging_Power_8.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_8);
	}

	if(m_ConfigData.m_Device_HuiKong_DIO_Y_TEST_NEEDLE_ID >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_TEST_NEEDLE_ID < 16)
	{
		m_Combo_TEST_NEEDLE.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_TEST_NEEDLE_ID);
	}

	if(m_ConfigData.m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID < 16)
	{
		m_Combo_TEST_START.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID);
	}

	if(m_ConfigData.m_Device_HuiKong_DIO_Y_SET_ID >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_SET_ID < 16)
	{
		m_Combo_SET_KEY.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_SET_ID);
	}
	
	if(m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID < 16)
	{
		m_Combo_MINUS_KEY.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID);
	}
	
	if(m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID >= 0 && m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID < 16)
	{
		m_Combo_PLUS_KEY.SetCurSel(m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID);
	}

		

	CStringArray sa;

	sa.RemoveAll();
	EnumSerialPortFriendlyNames(sa);


	this->m_Combo_COMM_AGING_1.ResetContent();
	this->m_Combo_COMM_AGING_2.ResetContent();
	this->m_Combo_COMM_AGING_3.ResetContent();
	this->m_Combo_COMM_AGING_4.ResetContent();
	this->m_Combo_COMM_AGING_5.ResetContent();
	this->m_Combo_COMM_AGING_6.ResetContent();
	this->m_Combo_COMM_AGING_7.ResetContent();
	this->m_Combo_COMM_AGING_8.ResetContent();
	this->m_Combo_PLC_COMM.ResetContent();
	this->m_Combo_PROGRAM_COMM.ResetContent();

	for(int i = 0; i < sa.GetSize(); i++)
	{
		m_Combo_COMM_AGING_1.AddString(sa.GetAt(i));
		m_Combo_COMM_AGING_2.AddString(sa.GetAt(i));
		m_Combo_COMM_AGING_3.AddString(sa.GetAt(i));
		m_Combo_COMM_AGING_4.AddString(sa.GetAt(i));
		m_Combo_COMM_AGING_5.AddString(sa.GetAt(i));
		m_Combo_COMM_AGING_6.AddString(sa.GetAt(i));
		m_Combo_COMM_AGING_7.AddString(sa.GetAt(i));
		m_Combo_COMM_AGING_8.AddString(sa.GetAt(i));
		m_Combo_PLC_COMM.AddString(sa.GetAt(i));
		m_Combo_PROGRAM_COMM.AddString(sa.GetAt(i));
	}
	if(sa.GetSize() > 0 && m_ConfigData.m_Comm_Nr[1] >= 0 && m_ConfigData.m_Comm_Nr[1] < sa.GetSize())
	{
		m_Combo_COMM_AGING_1.SetCurSel(m_ConfigData.m_Comm_Nr[1]);
	}
	if(sa.GetSize() > 0 && m_ConfigData.m_Comm_Nr[2] >= 0 && m_ConfigData.m_Comm_Nr[2] < sa.GetSize())
	{
		m_Combo_COMM_AGING_2.SetCurSel(m_ConfigData.m_Comm_Nr[2]);
	}
	if(sa.GetSize() > 0 && m_ConfigData.m_Comm_Nr[3] >= 0 && m_ConfigData.m_Comm_Nr[3] < sa.GetSize())
	{
		m_Combo_COMM_AGING_3.SetCurSel(m_ConfigData.m_Comm_Nr[3]);
	}
	if(sa.GetSize() > 0 && m_ConfigData.m_Comm_Nr[4] >= 0 && m_ConfigData.m_Comm_Nr[4] < sa.GetSize())
	{
		m_Combo_COMM_AGING_4.SetCurSel(m_ConfigData.m_Comm_Nr[4]);
	}
	if(sa.GetSize() > 0 && m_ConfigData.m_Comm_Nr[5] >= 0 && m_ConfigData.m_Comm_Nr[5] < sa.GetSize())
	{
		m_Combo_COMM_AGING_5.SetCurSel(m_ConfigData.m_Comm_Nr[5]);
	}
	if(sa.GetSize() > 0 && m_ConfigData.m_Comm_Nr[6] >= 0 && m_ConfigData.m_Comm_Nr[6] < sa.GetSize())
	{
		m_Combo_COMM_AGING_6.SetCurSel(m_ConfigData.m_Comm_Nr[6]);
	}

	if(sa.GetSize() > 0 && m_ConfigData.m_Comm_Nr[7] >= 0 && m_ConfigData.m_Comm_Nr[7] < sa.GetSize())
	{
		m_Combo_COMM_AGING_7.SetCurSel(m_ConfigData.m_Comm_Nr[7]);
	}

	if(sa.GetSize() > 0 && m_ConfigData.m_Comm_Nr[8] >= 0 && m_ConfigData.m_Comm_Nr[8] < sa.GetSize())
	{
		m_Combo_COMM_AGING_8.SetCurSel(m_ConfigData.m_Comm_Nr[8]);
	}

	if(sa.GetSize() > 0 && m_ConfigData.m_Comm_Device_HuiKong_DIO >= 0 && m_ConfigData.m_Comm_Device_HuiKong_DIO < sa.GetSize())
	{
		m_Combo_PLC_COMM.SetCurSel(m_ConfigData.m_Comm_Device_HuiKong_DIO);
	}
	

	if(sa.GetSize() > 0 && m_ConfigData.m_Comm_Nr[0] >= 0 && m_ConfigData.m_Comm_Nr[0] < sa.GetSize())
	{
		m_Combo_PROGRAM_COMM.SetCurSel(m_ConfigData.m_Comm_Nr[0]);
	}

	

	CStringArray  CameraName;
	CameraName.RemoveAll();
	CCaptureVideo::EnumDevices(&CameraName);
	for(int i = 0; i < CameraName.GetSize(); i++)
	{
		this->m_Combo_Camera.AddString(CameraName.GetAt(i));
	}
	if(CameraName.GetSize() > 0 && m_ConfigData.m_Camera_Nr >= 0 && m_ConfigData.m_Camera_Nr < CameraName.GetSize())
	{
		this->m_Combo_Camera.SetCurSel(m_ConfigData.m_Camera_Nr);
	}
	else
	{
		AfxMessageBox(_T("无此编号的摄像头"));
	}
	
	return true;
}


void CParaDlg::OnCbnSelendokComboCamera()
{
	// TODO: 在此添加控件通知处理程序代码

	::m_ConfigData.m_Camera_Nr = this->m_Combo_Camera.GetCurSel();
	::SaveConfig();

}

void CParaDlg::OnCbnSelendokCombo_AGING_Comm1()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Comm_Nr[1] = this->m_Combo_COMM_AGING_1.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokCombo_AGING_Comm2()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Comm_Nr[2]= this->m_Combo_COMM_AGING_2.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokCombo_AGING_Comm3()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Comm_Nr[3]= this->m_Combo_COMM_AGING_3.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokCombo_AGING_Comm4()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Comm_Nr[4]= this->m_Combo_COMM_AGING_4.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokCombo_AGING_Comm5()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Comm_Nr[5]= this->m_Combo_COMM_AGING_5.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokCombo_AGING_Comm6()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Comm_Nr[6]= this->m_Combo_COMM_AGING_6.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokCombo_AGING_Comm7()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Comm_Nr[7]= this->m_Combo_COMM_AGING_7.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokCombo_AGING_Comm8()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Comm_Nr[8]= this->m_Combo_COMM_AGING_8.GetCurSel();
	::SaveConfig();
}
void CParaDlg::OnCbnSelendokCombo_PLC_Comm()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Comm_Device_HuiKong_DIO = this->m_Combo_PLC_COMM.GetCurSel();
	::SaveConfig();
}
void CParaDlg::OnBnClickedOk(void)
{
	this->OnCancel();

}
void CParaDlg::DisplayText(CString *text,unsigned int counts ,MODBUS_RS485_CMD_ALL all)
{
		CString s1;
		text->Format(_T("(%d)"),counts);
		unsigned char *data = (unsigned char *)&all.RS485_Addr;
		for(int i = 0; i < all.data_size; i++)
		{
			s1.Format(_T("%02X,"),data[i]);
			*text += s1;
		}
	
}
void CParaDlg::DisplayText(CString *text,unsigned int counts ,unsigned char *data,int date_len)
{
		CString s1;
		text->Format(_T("(%d)"),counts);
		for(int i = 0; i < date_len; i++)
		{
			s1.Format(_T("%02X,"),data[i]);
			*text += s1;
		}
	
}

void CParaDlg::DisplaySend_PLC(MODBUS_RS485_CMD_ALL all)
{
	static unsigned int SendCounts = 0;
	SendCounts ++;
	if(this->m_hWnd != 0)
	{
		DisplayText(&m_SendTxt_PLC,SendCounts,all);
		this->UpdateData(false);
	}

}

void CParaDlg::DisplaySend_hotLong(int ID,MODBUS_RS485_CMD_ALL all)
{
	static unsigned int SendCounts[9] = {0};
	SendCounts[ID] ++;
	if(this->m_hWnd != 0)
	{
	
		DisplayText(&m_SendTxt_HotLong[ID],SendCounts[ID],all);

		this->UpdateData(false);
	}

}

void CParaDlg::OnCbnSelendokComboPlcY_AGING_1()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_1 = this->m_Combo_Aging_Power_1.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokComboPlcY_AGING_2()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_2 = this->m_Combo_Aging_Power_2.GetCurSel();
	::SaveConfig();
}
void CParaDlg::OnCbnSelendokComboPlcY_AGING_3()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_3 = this->m_Combo_Aging_Power_3.GetCurSel();
	::SaveConfig();
}
void CParaDlg::OnCbnSelendokComboPlcY_AGING_4()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_4 = this->m_Combo_Aging_Power_4.GetCurSel();
	::SaveConfig();
}
void CParaDlg::OnCbnSelendokComboPlcY_AGING_5()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_5 = this->m_Combo_Aging_Power_5.GetCurSel();
	::SaveConfig();
}
void CParaDlg::OnCbnSelendokComboPlcY_AGING_6()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_6 = this->m_Combo_Aging_Power_6.GetCurSel();
	::SaveConfig();
}
void CParaDlg::OnCbnSelendokComboPlcY_AGING_7()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_7 = this->m_Combo_Aging_Power_7.GetCurSel();
	::SaveConfig();
}
void CParaDlg::OnCbnSelendokComboPlcY_AGING_8()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_Aging_Power_8 = this->m_Combo_Aging_Power_8.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokComboPcbType()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Pcb_Type = (PCB_TYPE)this->m_Combo_PCB_Type.GetCurSel();
	::SaveConfig();
}

void CParaDlg::DisplayRec_PLC(unsigned char *Data,int Date_Len)
{
	static unsigned int RecCounts = 0;
	RecCounts ++;
	if(this->m_hWnd != 0)
	{
		DisplayText(&m_RecTxt_PLC,RecCounts,Data,Date_Len);
		this->UpdateData(false);
	}
}

void CParaDlg::DisplayRec_hotLong(int ID,unsigned int counts ,unsigned char *data,int data_len)
{
	static unsigned int RecCounts[9] = {0};
	RecCounts[ID] ++;
	if(this->m_hWnd != 0)
	{
		DisplayText(&m_RecTxt_HotLong[ID],RecCounts[ID],data,data_len);
	}
}
void CParaDlg::OnCbnSelendokComboProgramComm()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Comm_Nr[0] = this->m_Combo_PROGRAM_COMM.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokComboPlcYTestNeedle()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_TEST_NEEDLE_ID = this->m_Combo_TEST_NEEDLE.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokComboPlcYTestStart()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID = this->m_Combo_TEST_START.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokComboPlcYSetKey()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_SET_ID = this->m_Combo_SET_KEY.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokComboPlcYPlusKey()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_PLUS_ID = this->m_Combo_PLUS_KEY.GetCurSel();
	::SaveConfig();
}

void CParaDlg::OnCbnSelendokComboPlcYMinusKey()
{
	// TODO: 在此添加控件通知处理程序代码
	::m_ConfigData.m_Device_HuiKong_DIO_Y_MINUS_ID = this->m_Combo_MINUS_KEY.GetCurSel();
	::SaveConfig();
}
