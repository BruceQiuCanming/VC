// HeatBoxDlg.cpp : implementation file
//

#include "stdafx.h"
#include "HeatBox.h"
#include "HeatBoxDlg.h"
#include "comm.h"
#include "CopyRight.h"
//#include "TempAdjRst.h"
#include "shlwapi.h"
#include "math.h"
#include <stdio.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//#include "ParaRecordset.h"
//#include "AdjustPIDRecordset.h"
#include "afxwin.h"

#include "Setupapi.h"

CCommCmdArray_500	m_SendCmdArray500;


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
CString strModify =
_T(" \
2018/09/10 修改错误:程序运行时,没有显示温度修正值 \r\n \
2018/09/08 判断温度连续10次相同的话,发送命令,强行停止加热 \r\n \
2018/08/27 参数类型增加：复位温度范围和温度差同时起作用 \r\n \
2018/08/10 修改错误:参数为温度差时,复大品判断错误 \r\n \
2018/03/12 增加拔板界面的即时打印功能 \r\n \
2018/03/08 保存为Excel文件时,增加标题 \r\n \
2018/03/07 修改错误:保存为Excel文件时,产品序号没有排序 \r\n \
2017/12/23 优化了温度分布图,低于最低或高于最高测试温度的一个放到一个格子显示 \r\n \
2017-12-20 优化了控制命令的转发方式  \r\n \
2017-12-20 优化了通讯数据的读取和处理方式  \r\n \
2017-08-31 历史记录自动保存到 D:\\\\历史记录\\  目录下  \r\n \
2017-08-23 数据库全部改为文件方式 \r\n \
2017-08-06 修改查看历史记录时，拔板显示错误的问题 \r\n \
2017-08-06 拔板错误时，也显示产品编号 \r\n \
2017-08-03 修改在打开温度分布图的时候，通信中断的错误 \r\n \
2017-07-09 增加降温速度控制 \r\n \
2017-04-24 在选择不测试闪动后，屏蔽动闪和复闪  \r\n \
2017-04-24 增加高温品(常胜需要) \r\n \
2017-04-24 恢复离线拔板 \r\n \
2017-03-24 开始测试后，拔板按钮显示为黑色的\"在线查看\",测试结束后显示为红色的|\"在线拔板\" \r\n \
2017-03-20 开门复位的温差列表框是0-10度，程序里是0-5度，统一为0-10度 \r\n \
2017-03-01 打印时，可以选择只打印合格品或者主规格 \r\n \
2017-02-16 MODBUS和ASCII通信可以自由选择 \r\n \
2017-02-16 通信命令没有收到仪表的响应，会一直重发，保证联机 \r\n \
2017-02-05 历史记录也可以用拔板查看 \r\n \
2017-02-05 不用控件自绘温度曲线和温度分布图 \r\n \
2017-02-02 当复位温度最大最小值相同时，可以单测动作温度 \r\n \
2017-02-02 输入框改为软键盘操作 \r\n \
2017-01-13 128点，500点合并成一个版本 \r\n \
2017-01-11 修改错误：在只选一个测试规格的时候，低温品等全部归到了不动作里面 \r\n \
2016-12-23 增加功能：所有动作的产品都复位了才停止测试  \r\n \
2016-12-20 温度分布图数量上加入不动作和不复位的统计  \r\n \
2016-12-18 温度分布图上加上数量  \r\n \
2016-12-18 测试记录查询列表加上网格  \r\n \
2016-12-15 测试记录查询需要同时判断结束时间  \r\n \
2016-12-08 报表添加测试说明  \r\n \
2016-12-08 报表可以按照温度排序  \r\n \
2016-12-07 调试：PID恒温可以设置时间，时间单位为秒  \r\n \
2016-12-07 温度分布图添加百分比显示  \r\n \
2016-12-06 在每次开始测试时增加备注一项，方便记录查询  \r\n \
2016-12-06 测试记录里增加备注一项，方便记录查询  \r\n \
2016-12-06 输入数字对话框可以输入负数  \r\n \
2016-11-29 查询测试结果左列表里增加 测试参数 项  \r\n \
2016-10-29 对话框标题 可以在AboutDlg 下任意修改 \r\n \
2016-10-22 修改拔板时分隔线闪烁的问题 	\r\n \
2016-10-22 修改拔板时候温度的显示颜色 	\r\n \
2016-10-17 没有校温不能进行测试 	\r\n \
2016-10-08 一个窗口的参数修改立即在其他烘箱也生效 	\r\n \
2016-10-08 打印报表使用的\"公司名称\" 可以在AboutDlg 下任意修改 \r\n \
2016-10-03 增加修改说明 \r\n \
");


COMMAND_TEXT G_Cmd_Text[] =                                             
{                                                                       
	{CMD_NULL					,	_T("NULL					")},
	{CMD_STOP_OUT				,	_T("STOP_OUT				")},
	{CMD_FULL_OUT				,	_T("FULL_OUT				")},
	{CMD_NORMAL_PID_OUT			,	_T("NORMAL_PID_OUT			")},
	{CMD_STEP_PID_OUT			,	_T("STEP_PID_OUT			")},
	{CMD_READ_TEMP				,	_T("READ_TEMP				")},
	{CMD_READ_PID_T				,	_T("READ_PID_T				")},
	{CMD_SET_PID_T				,	_T("SET_PID_T				")},
	{CMD_SET_ADJUST_LOW			,	_T("SET_ADJUST_LOW			")},
	{CMD_SET_ADJUST_HIGH		,	_T("SET_ADJUST_HIGH			")},
	{CMD_READ_ADJUST_LOW		,	_T("READ_ADJUST_LOW			")},
	{CMD_READ_ADJUST_HIGH		,	_T("READ_ADJUST_HIGH		")},
                                                                    
	{CMD_READ_FLASH				,	_T("READ_FLASH				")},
	{CMD_SET_SWITCH_NORMAL_MODE	,	_T("SET_SWITCH_NORMAL_MODE	")},
	
	{CMD_READ_STATUS			,	_T("READ_STATUS				")},
	{CMD_SET_AIR_VALVE			,	_T("SET_AIR_VALVE		")},
	{CMD_SET_AIR_CONTROL		,	_T("SET_AIR_CONTROL		")},
	{CMD_SET_HEAT				,	_T("SET_HEAT			")},
	{CMD_SET_FAN				,	_T("SET_FAN			")},
	{CMD_SET_SSR_5				,	_T("SSR5			")},
                                                                    
	{CMD_SET_DISPLAY_TYPE		,	_T("SET_DISPLAY_TYPE		")},
	{CMD_READ_ADJUST_ALL		,	_T("READ_ADJUST_ALL			")},
	{CMD_WRITE_STEP_PID_COUNT	,	_T("WRITE_STEP_PID_COUNT	")},
	{CMD_READ_STEP_PID_COUNT	,	_T("READ_STEP_PID_COUNT		")},
	{CMD_SET_COOL_RELAY			,	_T("SET_COOL_RELAY			")},
	{CMD_STEP_DOWN_PID_OUT      ,	_T("STEP_DOWN_PID_OUT       ")},
                                                                    
	{CMD_PROGRAM_START			,	_T("PROGRAM_START			")},
	{CMD_PROGRAM_DATA			,	_T("PROGRAM_DATA			")},
	{CMD_PROGRAM_END			,	_T("PROGRAM_END				")},
                                                                        
};                                                                      

CString GetCmdString(unsigned char cmd)
{
	CString s = _T("");
	for(int i = 0; i < sizeof(G_Cmd_Text) / sizeof(COMMAND_TEXT); i++)
	{
		if(G_Cmd_Text[i].cmd == cmd)
		{
			s = G_Cmd_Text[i].text;
			s.Trim();
			return s;
		}
	}
	
	s.Format(_T("%X"), cmd);
	return s;
}

float Cal3Sigma(float *data,int data_len)
{
	if(data_len <= 1)
		return -1;

	/*
(1)先算样本均值m: 
m=(1/n)*(a[0]+a[1]+...+a[n-1]) 

(2)再算样本方差sigma^2: 
sigma^2=[1/n]*{(a[0]-m)^2+(a[2]-m)^2+...+(a[n-1]-m)^2} 

(3)把样本方差开根号就是样本标准差sigma 

(4)把sigma乘以3就行了
*/
	float avr = 0;
	float sigma = 0;
	for(int i = 0; i < data_len; i++)
	{
		avr += data[i];
	}
	avr /= data_len;


	for(int i = 0; i < data_len; i++)
	{
		sigma += (avr - data[i]) * (avr - data[i]); 
	}
	
	return  sqrt(sigma / (data_len -1)) * 3;
	
}


/*
Cp = (USL- LSL)/6Sigma
SC = (USL + LSL)/2
Ca = |平均X - SC| /((USL -LSL) /2 )
Cpk = Cp * (1 -Ca)
*/
float CalCp(float *data,int data_len,float USL,float LSL)
{
	return (USL - LSL) / Cal3Sigma(data,data_len) / 2;
}

float CalCpk(float *data,int data_len,float USL,float LSL)
{
	float avr = 0;
	for(int i = 0; i < data_len; i++)
	{
		avr += data[i];
	}
	avr /= data_len;

	float Ca = (avr - (USL + LSL)/2) / ((USL -LSL) / 2);

	if(Ca < 0)
	{
		Ca = 0 -Ca;
	}

	return CalCp(data,data_len,USL,LSL) * ( 1 - Ca);
}


class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CComboBox	m_CommList;
	CString	m_CompileTime;
	CString	m_Version;
	CString	m_strSoftTime;
	CString	m_strPcTime;
	DWORD	m_PcSN;
	DWORD	m_SoftSN;
	CString	m_PcTime2;
	CString	m_SoftTime2;
	//}}AFX_DATA

	CTime	m_PcTime;
	CTime	m_SoftTime;
	
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSelchangeCombo1();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnButtonSaveSn();
	afx_msg void OnButtonCrack();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_CopyrightDay;
	CString m_EditModify;
	afx_msg void OnEnChangeEditUser();
	afx_msg void OnEnChangeEditDialogTiltle();
	CComboBox m_MaxLedNrList;
	afx_msg void OnCbnSelchangeComboSwitchNr();
	CComboBox m_HighHighLevelDeltaList;
	afx_msg void OnCbnSelchangeComboHighHighLevelDelta();
	afx_msg void OnBnClickedButtonLastVersion();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnEnChangeEditCommTimer();
	UINT m_CommTimer;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_EditModify(_T(""))
, m_CommTimer(0)
{
	
	//{{AFX_DATA_INIT(CAboutDlg)
	m_CompileTime = _T("");
	m_Version = _T("");
//	m_BoxMsg = _T("");
	m_strSoftTime = _T("");
	m_strPcTime = _T("");
	m_PcSN = 0;
	m_SoftSN = 0;
	m_PcTime2 = _T("");
	m_SoftTime2 = _T("");
	//}}AFX_DATA_INIT

	m_CopyrightDay = COPYWRITE_DAYS; 

	
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)

	DDX_Control(pDX, IDC_COMBO1, m_CommList);
	DDX_Text(pDX, IDC_STATIC_COMPILE_TIME, m_CompileTime);
	DDX_Text(pDX, IDC_STATIC_VERSION, m_Version);
	//	DDX_Text(pDX, IDC_STATIC_BOX_MSG, m_BoxMsg);
	DDX_Text(pDX, IDC_EDIT_SOFT_TIME, m_strSoftTime);
	DDX_Text(pDX, IDC_EDIT_PC_TIME, m_strPcTime);
	DDX_Text(pDX, IDC_EDIT_PC_SN, m_PcSN);
	DDX_Text(pDX, IDC_EDIT_SOFT_SN, m_SoftSN);
	DDX_Text(pDX, IDC_EDIT_PC_TIME2, m_PcTime2);
	DDX_Text(pDX, IDC_EDIT_SOFT_TIME2, m_SoftTime2);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STATIC_COPYWRITE_DAYS, m_CopyrightDay);
	DDX_Text(pDX, IDC_EDIT_MODIFY_HISTORY, m_EditModify);
	DDX_Control(pDX, IDC_COMBO_SWITCH_NR, m_MaxLedNrList);
	DDX_Control(pDX, IDC_COMBO_HIGH_HIGH_LEVEL_DELTA, m_HighHighLevelDeltaList);

	DDX_Text(pDX, IDC_EDIT_COMM_TIMER, m_CommTimer);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	ON_WM_LBUTTONDBLCLK()
	ON_CBN_SELCHANGE(IDC_COMBO1, OnSelchangeCombo1)
	ON_WM_KEYDOWN()
	ON_BN_CLICKED(IDC_BUTTON_SAVE_SN, OnButtonSaveSn)
	ON_BN_CLICKED(IDC_BUTTON_CRACK, OnButtonCrack)
	//}}AFX_MSG_MAP
	ON_EN_CHANGE(IDC_EDIT_USER, &CAboutDlg::OnEnChangeEditUser)
	ON_EN_CHANGE(IDC_EDIT_DIALOG_TITLE, &CAboutDlg::OnEnChangeEditDialogTiltle)
	ON_CBN_SELCHANGE(IDC_COMBO_SWITCH_NR, &CAboutDlg::OnCbnSelchangeComboSwitchNr)
	ON_CBN_SELCHANGE(IDC_COMBO_HIGH_HIGH_LEVEL_DELTA, &CAboutDlg::OnCbnSelchangeComboHighHighLevelDelta)
	ON_BN_CLICKED(IDC_BUTTON_LAST_VERSION, &CAboutDlg::OnBnClickedButtonLastVersion)
	ON_WM_ACTIVATE()
	ON_EN_CHANGE(IDC_EDIT_COMM_TIMER, &CAboutDlg::OnEnChangeEditCommTimer)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeatBoxDlg dialog

CHeatBoxDlg::CHeatBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHeatBoxDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHeatBoxDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_CurRs485Nr = 0;

	m_MeterID = 0;

	m_CommTimeOut[0] = m_CommTimeOut[1] =m_CommTimeOut[2] =m_CommTimeOut[3] =0;
	for(int i = 0; i < MAX_HEAT_BOX; i++)
	{
		m_SN[i] = 8001 + i;
	}
	
	memset(m_SwitchStatus,0,sizeof(m_SwitchStatus));
}

void CHeatBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_TAB1, m_Tab[0]);
	DDX_Control(pDX, IDC_TAB2, m_Tab[1]);
	DDX_Control(pDX, IDC_TAB3, m_Tab[2]);
	DDX_Control(pDX, IDC_TAB4, m_Tab[3]);
	DDX_Control(pDX, IDC_TAB5, m_Tab[4]);
	DDX_Control(pDX, IDC_TAB6, m_Tab[5]);
	DDX_Control(pDX, IDC_TAB7, m_Tab[6]);
	DDX_Control(pDX, IDC_TAB8, m_Tab[7]);
	DDX_Control(pDX, IDC_TAB9, m_Tab[8]);
	DDX_Control(pDX, IDC_TAB10, m_Tab[9]);

	//{{AFX_DATA_MAP(CHeatBoxDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CHeatBoxDlg, CDialog)

	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, OnSelchangeTab1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB2, OnSelchangeTab2)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB3, OnSelchangeTab3)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB4, OnSelchangeTab4)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB5, OnSelchangeTab5)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB6, OnSelchangeTab6)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB7, OnSelchangeTab7)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB8, OnSelchangeTab8)

	//{{AFX_MSG_MAP(CHeatBoxDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_KEYDOWN()
	ON_WM_HELPINFO()
	ON_WM_VKEYTOITEM()
	//}}AFX_MSG_MAP
	ON_MESSAGE(COMM_MSG, OnComm)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHeatBoxDlg message handlers
unsigned char G_cSwitchStatus[16];
unsigned char TM1638_DisplayBuf_U2[8];
unsigned char TM1638_DisplayBuf_U3[8];
unsigned char TM1638_DisplayBuf_U4[8];
#define BIT(x)	(1<<x)
void DislaySwitchStatus(void)
{
		unsigned char i,j;
		unsigned char temp;
		
		G_cSwitchStatus[1] = 0xFF;
		G_cSwitchStatus[1] = 0x01;
		G_cSwitchStatus[2] = 0x03;
		G_cSwitchStatus[3] = 0x07;
		G_cSwitchStatus[4] = 0x0f;
		G_cSwitchStatus[5] = 0x1f;
		G_cSwitchStatus[6] = 0x3f;
		G_cSwitchStatus[7] = 0x7f;
		G_cSwitchStatus[8] = 0xff;
		
		
		for( i = 0; i < 8; i++)
		{
			{
				temp = 0;
				j = (i*12);
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(7);
				} 
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(6);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(5);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(4);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(3);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(2);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(1);
				}	
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(0);
				}
				
				TM1638_DisplayBuf_U2[i] = temp;
				/*
				//======333
				ReadRx();
				temp = 0;
				j = (i*12 + 96);
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(7);
				} 
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(6);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(5);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(4);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(3);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(2);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(1);
				}	
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(0);
				}
				TM1638_DisplayBuf_U3[i] = temp;
				*/
				/*
				//==44444===
				ReadRx();
				temp = 0;
				j = (i*24)+8;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(7);
				} 
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(6);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(5);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(4);
				}
				j = (i*24)+8+12;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(3);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(2);
				}
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(1);
				}	
				j++;
				if((G_cSwitchStatus[j/8] & BIT(j%8)) == 0)
				{
					temp |= BIT(0);
				}
				TM1638_DisplayBuf_U4[i] = temp;
				*/
			}
		}
		
		
}
long CalAdjust(long cur_temp,char ch)
{
	//6600/10*(0-100)/300+1000=22
	//int ret;
	#define ADJUST_TEMP	200L
	//long l;//,m;
	long high,low,m;
	//int cur=cur_temp/10;
//	G_Para.AdjustHigh[ch]=141;
//	G_Para.AdjustLow[ch]=32;
	high	=	12;//AdjustHigh;// + G_Para.AdjustHigh_Board[ch];
	low		=	12;//AdjustLow;// + G_Para.AdjustLow_Board[ch];
	low		=	low *1000L;
	high	=	high*1000L;
	m		=	(high-low) / 100L;
//	l=(long)(cur_temp+low)*m;
//	l=l/(long)((long)(ADJUST_TEMP*100)-m)+(long)(low);
	//l=(long)(cur_temp+low)*(long)m/(long)((long)(ADJUST_TEMP*100)-m)+(long)(low);
	return ((long)(cur_temp+low)*m)/((ADJUST_TEMP*100L)-m)+(low);
	
	//return (int)l;
	//high=((int)l)*10;
	//return high;

}

char G_cHeatPercent,G_cFixHeatPercent;
CString sHeatPercent;
void HeatPercentOutput(void)
{
	static char HEAT_T = 0;

	char cHeatPercent = G_cHeatPercent;
	
	
				
		if(HEAT_T >= 100)
		{
			HEAT_T = 0;
		}
		
		
		if((HEAT_T % 10) < (cHeatPercent / 10) )
		{
			sHeatPercent += _T("1,");
		}
		else if((HEAT_T % 10) == (cHeatPercent / 10) )
		{
			if((cHeatPercent % 10) > (HEAT_T / 10))
			{
				sHeatPercent += _T("1,");
			}
			else
			{
				sHeatPercent += _T("0,");
			}
		}	
		else
		{
			sHeatPercent += _T("0,");
		}

		HEAT_T++;

}

void test(void)
{
	unsigned char bits,i,data = 2,id = 0; 
	unsigned char G_cSwitchStatus[16];
	memset(G_cSwitchStatus,0xFF,sizeof(G_cSwitchStatus));

	for(i = 0; i < 5 ; i++)
	{
		
		if(data & BIT(i))
		{ 
			bits = (id * 5 + i);
			G_cSwitchStatus[bits/8] |= BIT( bits % 8);
		}
		else
		{
			bits = (id * 5 + i);
			G_cSwitchStatus[bits/8] &=~BIT( bits % 8);
		}
	}
}
BOOL CHeatBoxDlg::OnInitDialog()
{

	test();

	CDialog::OnInitDialog();
	
	G_cHeatPercent = 1;

	for(int i = 0; i < 101; i++)
	{
		HeatPercentOutput();
	}
//	AfxMessageBox(sHeatPercent);
	long ret =CalAdjust(1300,0);

	MODBUS_RS485_CMD_500 cmd;
	cmd.cmd = 0x10;
	cmd.RS485_Addr = 0x41;
	cmd.para_len = sizeof(cmd.para);
	memset(cmd.para,0,sizeof(cmd.para));  
	unsigned char buf[37] = {0x41,0x10,0x20,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xA0,0x0B};
		//{0x31,0x34,0x18,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xE4};
	//{0x31,0x36,0x10,0x06,0x00,0x32,0x00,0x06,0x00,0x01 ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x62,0xBE};
	CComm::CRC16((unsigned char*)&cmd,sizeof(cmd)-2,(unsigned char*)cmd.crc); 
	
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	if(!::SetPriorityClass(::GetCurrentProcess(),REALTIME_PRIORITY_CLASS))
	{
		AfxMessageBox(_T("设置最高优先级失败!"));
	}

	
	InitDatabase();

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
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here

	G_CopyRight.IsCopyRightOK();

    this->ShowWindow(SW_SHOWMAXIMIZED);
	CRect rect ;
	
	GetClientRect( &rect );


	int nWidth=rect.Width(); 
    int nHeight=rect.Height();

	int i = 0;
	for(i=0 ;i < 2*MAX_HEAT_BOX; i++)
	{
		m_Tab[i].ShowWindow(SW_HIDE); 
	}

	
		
	for(i=0 ;i < G_CurMaxHeatBox;i++)
	{
		//m_Tab[i+G_CurMaxHeatBox].ShowWindow(SW_SHOW);
		m_Tab[i].ShowWindow(SW_SHOW);

		//m_Tab[i+G_CurMaxHeatBox].MoveWindow(0 + i * nWidth / G_CurMaxHeatBox,0,nWidth / G_CurMaxHeatBox-5, 120); 
		//m_Tab[i+G_CurMaxHeatBox].InsertItem(0,_T("温度参数"));
		

		m_TempParaDlg.Create(IDD_DIALOG_TEMP_PARA,&m_Tab[i+G_CurMaxHeatBox]);
		//m_Tab[i+G_CurMaxHeatBox].GetClientRect(&rect);
		rect.top += 20;
		m_TempParaDlg.MoveWindow(&rect); 
		//m_Tab[i+G_CurMaxHeatBox].ShowWindow(SW_SHOW); 
		//m_Tab[i+G_CurMaxHeatBox].SetCurSel(0); 
		//SelectTab(i+G_CurMaxHeatBox);
		m_Tab[i].MoveWindow(0 + i * nWidth / G_CurMaxHeatBox,0,nWidth/ G_CurMaxHeatBox-5, nHeight-0); 
		m_Tab[i].InsertItem(0,_T("生产"));
		m_Tab[i].InsertItem(1,_T("参数"));
		m_Tab[i].InsertItem(2,_T("调试"));
		m_Tab[i].InsertItem(3,_T("曲线"));
		m_Tab[i].InsertItem(4,_T("离线拔板"));

		m_SwitchDlg.m_BoxNr = i;
		m_SwitchDlg.Create(IDD_DIALOG_SWITCH,&m_Tab[i]);
		m_CurveDlg.m_BoxNr = i;
		m_CurveDlg.Create (IDD_DIALOG_CURVE, &m_Tab[i]); 

		m_ParaDlg.m_BoxNr = i;
		m_ParaDlg.Create  (IDD_DIALOG_PARA,  &m_Tab[i]); 

		m_DebugDlg.m_BoxNr = i;
		m_DebugDlg.Create  (IDD_DIALOG_DEBUG,  &m_Tab[i]); 

		m_TakeoutPage.m_BoxNr = i;
		m_TakeoutPage.Create  (IDD_DIALOG_TAKEOUT_PAGE,  &m_Tab[i]); 

		m_Tab[i].GetClientRect(&rect);
		rect.bottom -= 20;

		m_SwitchDlg.MoveWindow(&rect); 
		m_CurveDlg.MoveWindow(&rect); 
		m_ParaDlg.MoveWindow(&rect);
		m_DebugDlg.MoveWindow(&rect); 
		m_TakeoutPage.MoveWindow(&rect);  
		m_CurveDlg.ShowWindow(SW_SHOW); 
		m_Tab[i].ShowWindow(SW_SHOW); 
		m_Tab[i].SetCurSel(0); 
		
		SelectTab(i);

	}

	ReadSwitchConfigPara();

	G_CommPara.comm = &m_Comm;
	G_CommPara.MessageID = COMM_MSG + 0;
	G_CommPara.m_hWnd = this->GetSafeHwnd(); 
	
	int comm_port = AfxGetApp()->GetProfileInt(_T("SETTING"),_T("COMM PORT"),0); 
	m_Comm.bXModem =	false;
	m_Comm.OpenComm(comm_port+1,&G_CommPara);


	for(i = 0; i < G_CurMaxHeatBox; i++)
	{
		WriteLogFile("RUN",i + 1);
	}

	this->SetWindowText(strMsgCaption);



#ifndef _DEBUG

/*	
	// TODO: Add extra initialization here
	
		for(i = 0; i < G_CurMaxHeatBox; i++)
		{
			if(m_SwitchDlg.m_HeatBoxType == HEAT_BOX_TYPE_HANJIE)
			{
				continue;
			}
			CString cmdStr;

			char para[100];
			memset(para,0,sizeof(para));

			para[0] = 6;
			para[2] = 50;
			para[4] = 6;
			para[6] = 1;

			SetCmdString( i + '1',CMD_SET_PID_T, para);

			memset(para,0,sizeof(para));
			SetCmdString( i + '1',CMD_READ_ADJUST_LOW, para);

			SetCmdString( i + '1',CMD_READ_ADJUST_HIGH, para);


		//	if(G_IsCmdAscii[i] == false)
			{
		//		SetCmdString(&cmdStr, i + '1',CMD_READ_ADJUST_ALL, para);
		//		m_SendCmdArray[i].Add(cmdStr);
			}
				SetCmdString( i + '1',CMD_NORMAL_PID_OUT, para);

				SetCmdString( i + '1',CMD_STOP_OUT, para);

				SetCmdString( i + '1',CMD_READ_PID_T, para);


		}
	*/
#endif

	strMsgCaption  += AfxGetApp()->GetProfileStringW(_T("SETTING"),_T("DIALOG TITLE"),_T("常州市润邦电子科技有限公司 TEL(微信): 13915838598 QQ:228154900")) ;

	
	
	this->SetTimer(1,AfxGetApp()->GetProfileIntW( _T("SETTING"),_T("COMM TIMER"), 300),NULL); 



	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CHeatBoxDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHeatBoxDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);


	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHeatBoxDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CHeatBoxDlg::OnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SelectTab(0);
	*pResult = 0;
}

void CHeatBoxDlg::OnSelchangeTab2(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SelectTab(1);
	*pResult = 0;
}

void CHeatBoxDlg::OnSelchangeTab3(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SelectTab(2);
	*pResult = 0;
}

void CHeatBoxDlg::OnSelchangeTab4(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SelectTab(3);
	*pResult = 0;
}
void CHeatBoxDlg::OnSelchangeTab5(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SelectTab(4);
	*pResult = 0;
}
void CHeatBoxDlg::OnSelchangeTab6(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SelectTab(5);
	*pResult = 0;
}
void CHeatBoxDlg::OnSelchangeTab7(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SelectTab(6);
	*pResult = 0;
}
void CHeatBoxDlg::OnSelchangeTab8(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	SelectTab(7);
	*pResult = 0;
}




void CHeatBoxDlg::SelectTab(int tabnr)
{

	CHeatBoxApp * app=(CHeatBoxApp * )AfxGetApp();
	if(tabnr < G_CurMaxHeatBox)
	{
		app->m_TabActiveIndex[tabnr] = m_Tab[tabnr].GetCurSel();
		switch(app->m_TabActiveIndex[tabnr])
		{
		case 0:
			m_SwitchDlg.ShowWindow(SW_SHOW );
			m_CurveDlg.ShowWindow(SW_HIDE);
			m_ParaDlg.ShowWindow(SW_HIDE);
			m_DebugDlg.ShowWindow(SW_HIDE);
			m_SwitchDlg.UpdateData(false);
			m_TakeoutPage.ShowWindow(SW_HIDE);
			break;
		case 4:
			m_SwitchDlg.ShowWindow(SW_HIDE);
			m_CurveDlg.ShowWindow(SW_HIDE );
			m_ParaDlg.ShowWindow(SW_HIDE);
			m_DebugDlg.ShowWindow(SW_HIDE);
			m_TakeoutPage.ShowWindow(SW_SHOW);
			{
				m_TakeoutPage.m_SwitchConfigPara = m_SwitchDlg.m_SwitchConfigPara;
				//恢复生产记录
				CFile f;
				CString file_name;
				file_name.Format(_T("record%d.bin"),tabnr);
				if(f.Open(theAppDirectory+file_name, CFile::modeReadWrite))
				{
					f.SeekToBegin();
					if(f.Read(&m_TakeoutPage.m_SwitchTestResult,sizeof(m_TakeoutPage.m_SwitchTestResult)) != sizeof(m_TakeoutPage.m_SwitchTestResult))
					{
						memset(&m_TakeoutPage.m_SwitchTestResult,0,sizeof(m_TakeoutPage.m_SwitchTestResult));
					}
					m_TakeoutPage.ShowTestResult(); 
					f.Close(); 
				}

				m_TakeoutPage.SortTestResult();
			}
			break;

		case 3:
			m_SwitchDlg.ShowWindow(SW_HIDE);
			m_CurveDlg.ShowWindow(SW_SHOW );
			m_ParaDlg.ShowWindow(SW_HIDE);
			m_DebugDlg.ShowWindow(SW_HIDE);
			m_CurveDlg.UpdateData(false);
			m_TakeoutPage.ShowWindow(SW_HIDE);
			break;
		case 1:
			m_SwitchDlg.ShowWindow(SW_HIDE);
			m_CurveDlg.ShowWindow(SW_HIDE );
			m_ParaDlg.ShowWindow(SW_SHOW);
			m_DebugDlg.ShowWindow(SW_HIDE);
			m_ParaDlg.UpdateData(false);
			m_TakeoutPage.ShowWindow(SW_HIDE);
			break;
		case 2:
			m_SwitchDlg.ShowWindow(SW_HIDE);
			m_CurveDlg.ShowWindow(SW_HIDE );
			m_ParaDlg.ShowWindow(SW_HIDE);
			m_DebugDlg.ShowWindow(SW_SHOW);
			m_TakeoutPage.ShowWindow(SW_HIDE);
			break;
		}
	}
	else
	{
		app->m_TabActiveIndex[tabnr] = m_Tab[tabnr].GetCurSel();
		switch(app->m_TabActiveIndex[tabnr])
		{
			case 0:
				m_TempParaDlg.ShowWindow(SW_SHOW); 
				break;
		}
	}
}

void CHeatBoxDlg::SaveSwitchConfigPara(int BoxNr)
{
	CString str1,str2;

	
	str1= "";
	
	
	//更新其它各个窗口的参数
	ReadSwitchConfigPara();

}

void CHeatBoxDlg::ReadSwitchConfigPara()
{
	
	CFile ParaFile;
	CFileFind ff;
	SWITCH_CONFIG_PARA	para;
	CString fName;
	CString sDir;
	
	
	sDir = theAppDirectory +_T("para\\");

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 
	}

	//TraverseFolder(sDir,_T("*.para"),&sa);

	
	if(ParaFile.Open(sDir + _T("工位修正") + _T(".para"),CFile::modeCreate |  CFile::modeNoTruncate | CFile::modeReadWrite))
	{
		
		ParaFile.SeekToBegin();

		memset(G_SwitchTempAdjust,0,sizeof(G_SwitchTempAdjust));

		ParaFile.Read(G_SwitchTempAdjust, sizeof(G_SwitchTempAdjust));
	
		ParaFile.Close();
		
	}

	for(int i =0;i < G_CurMaxHeatBox; i++)
	{
		CString str1;

		str1.Format(_T("SWITCH_CONFIG_PARA_BOX_NR_%d"),i); 
		CString para_name = AfxGetApp()->GetProfileString(_T("SETTING"),
			str1,
			_T("测试参数"));

		m_SwitchDlg.m_ParaName = para_name;

		memset(&para,0,sizeof(SWITCH_CONFIG_PARA));

		if(ParaFile.Open(sDir + para_name + _T(".para"),/*CFile::mode::modeCreate |  CFile::modeNoTruncate |*/ CFile::modeRead))
		{
			memset(&para,0,sizeof(SWITCH_CONFIG_PARA));

			if(ParaFile.Read(&para,sizeof(SWITCH_CONFIG_PARA)) <= sizeof(SWITCH_CONFIG_PARA)) 
			{
				
				m_SwitchDlg.m_SwitchConfigPara.ParaType 						= para.ParaType 		;
				m_SwitchDlg.m_SwitchConfigPara.MainLevel						= para.MainLevel		;
				m_SwitchDlg.m_SwitchConfigPara.MainLevelSpanMinus			= para.MainLevelSpanMinus	;
				m_SwitchDlg.m_SwitchConfigPara.MainLevelSpanPlus				= para.MainLevelSpanPlus	;
				m_SwitchDlg.m_SwitchConfigPara.HighLevel 					= para.HighLevel 		;
				m_SwitchDlg.m_SwitchConfigPara.HighLevelSpanMinus			= para.HighLevelSpanMinus 	;
				m_SwitchDlg.m_SwitchConfigPara.HighLevelSpanPlus				= para.HighLevelSpanPlus 	;
				m_SwitchDlg.m_SwitchConfigPara.LowLevel 						= para.LowLevel 		;
				m_SwitchDlg.m_SwitchConfigPara.LowLevelSpanMinus				= para.LowLevelSpanMinus 	;
				m_SwitchDlg.m_SwitchConfigPara.LowLevelSpanPlus				= para.LowLevelSpanPlus 	;
				m_SwitchDlg.m_SwitchConfigPara.CloseLevel					= para.CloseLevel ;
				m_SwitchDlg.m_SwitchConfigPara.CloseLevelSpanPlus			= para.CloseLevelSpanPlus;
				m_SwitchDlg.m_SwitchConfigPara.CloseLevelSpanMinus			= para.CloseLevelSpanMinus;
				m_SwitchDlg.m_SwitchConfigPara.CloseLevelDeviationPlus		= para.CloseLevelDeviationPlus;
				m_SwitchDlg.m_SwitchConfigPara.CloseLevelDeviationMinus		= para.CloseLevelDeviationMinus;


				m_SwitchDlg.m_SwitchConfigPara.CloseHighLevel				= para.CloseHighLevel ;
				m_SwitchDlg.m_SwitchConfigPara.CloseHighLevelSpanPlus		= para.CloseHighLevelSpanPlus;
				m_SwitchDlg.m_SwitchConfigPara.CloseHighLevelSpanMinus		= para.CloseHighLevelSpanMinus;
				m_SwitchDlg.m_SwitchConfigPara.CloseHighLevelDeviationPlus	= para.CloseHighLevelDeviationPlus;
				m_SwitchDlg.m_SwitchConfigPara.CloseHighLevelDeviationMinus	= para.CloseHighLevelDeviationMinus;


				m_SwitchDlg.m_SwitchConfigPara.CloseLowLevel					= para.CloseLowLevel ;
				m_SwitchDlg.m_SwitchConfigPara.CloseLowLevelSpanPlus			= para.CloseLowLevelSpanPlus;
				m_SwitchDlg.m_SwitchConfigPara.CloseLowLevelSpanMinus		= para.CloseLowLevelSpanMinus;
				m_SwitchDlg.m_SwitchConfigPara.CloseLowLevelDeviationPlus	= para.CloseLowLevelDeviationPlus;
				m_SwitchDlg.m_SwitchConfigPara.CloseLowLevelDeviationMinus	= para.CloseLowLevelDeviationMinus;



				m_SwitchDlg.m_SwitchConfigPara.OpenDoorResetLevel			= para.OpenDoorResetLevel;
				
				m_ParaDlg.m_SwitchConfigPara									= m_SwitchDlg.m_SwitchConfigPara;
				
				
				m_ParaDlg.m_ComboBoxDisplayTempType.SetCurSel(para.ParaType);  

				m_ParaDlg.m_MainLevelList.SetWindowTextW(para_name);	// .SetCurSel(k); 
				m_ParaDlg.m_MainLevel							= para.MainLevel		;
				m_ParaDlg.m_MainLevelSpanMinus					= para.MainLevelSpanMinus	;
				m_ParaDlg.m_MainLevelSpanPlus					= para.MainLevelSpanPlus	;
				m_ParaDlg.m_HighLevel 							= para.HighLevel 		;
				m_ParaDlg.m_HighLevelSpanMinus 					= para.HighLevelSpanMinus 	;
				m_ParaDlg.m_HighLevelSpanPlus 					= para.HighLevelSpanPlus 	;
				m_ParaDlg.m_LowLevel 							= para.LowLevel 		;
				m_ParaDlg.m_LowLevelSpanMinus 					= para.LowLevelSpanMinus 	;
				m_ParaDlg.m_LowLevelSpanPlus 					= para.LowLevelSpanPlus 	;
				m_ParaDlg.m_CloseMainLevel						= para.CloseLevel ;
				m_ParaDlg.m_CloseMainLevelSpanPlus				= para.CloseLevelSpanPlus ;
				m_ParaDlg.m_CloseMainLevelSpanMinus				= para.CloseLevelSpanMinus ;
				m_ParaDlg.m_CloseMainLevelDeviationPlus			= para.CloseLevelDeviationPlus ;
				m_ParaDlg.m_CloseMainLevelDeviationMinus			= para.CloseLevelDeviationMinus ;


				m_ParaDlg.m_CloseHighLevel						= para.CloseHighLevel ;
				m_ParaDlg.m_CloseHighLevelSpanPlus				= para.CloseHighLevelSpanPlus ;
				m_ParaDlg.m_CloseHighLevelSpanMinus				= para.CloseHighLevelSpanMinus ;
				m_ParaDlg.m_CloseHighLevelDeviationPlus			= para.CloseHighLevelDeviationPlus ;
				m_ParaDlg.m_CloseHighLevelDeviationMinus			= para.CloseHighLevelDeviationMinus ;


				m_ParaDlg.m_CloseLowLevel						= para.CloseLowLevel ;
				m_ParaDlg.m_CloseLowLevelSpanPlus				= para.CloseLowLevelSpanPlus ;
				m_ParaDlg.m_CloseLowLevelSpanMinus				= para.CloseLowLevelSpanMinus ;
				m_ParaDlg.m_CloseLowLevelDeviationPlus			= para.CloseLowLevelDeviationPlus ;
				m_ParaDlg.m_CloseLowLevelDeviationMinus			= para.CloseLowLevelDeviationMinus ;



				m_ParaDlg.m_OpenDoorResetList.SetCurSel(para.OpenDoorResetLevel) 	;
				m_ParaDlg.m_MainLevelList.SetWindowText(para_name);  
				m_ParaDlg.UpdateData(false);
				m_ParaDlg.ShowCloseLevelEdit();


			}
			else
			{
				memset(&para,0,sizeof(SWITCH_CONFIG_PARA));

				para.ParaType					=   0;
				para.OpenDoorResetLevel			=   0; 
				para.MainLevel					=	90;
				para.MainLevelSpanMinus			=	5;
				para.MainLevelSpanPlus			=	5;
				para.HighLevel					=	95;
				para.HighLevelSpanMinus			=	5;
				para.HighLevelSpanPlus			=	5;
				para.LowLevel					=	85;
				para.LowLevelSpanMinus			=	5;
				para.LowLevelSpanPlus			=	5;
				para.CloseLevel					=	50;
				para.CloseLevelSpanPlus			=	15;
				para.CloseLevelSpanMinus		=	15;
				para.CloseHighLevel				=	55;
				para.CloseHighLevelSpanPlus		=	15;
				para.CloseHighLevelSpanMinus	=	15;
				para.CloseLowLevel				=	45;
				para.CloseLowLevelSpanPlus		=	15;
				para.CloseLowLevelSpanMinus		=	15;
				
				para.OpenDoorResetLevel			=   0;
				para.TakeoutTime				=	0;

				ParaFile.SeekToBegin();
				ParaFile.Write(&para,sizeof(para)); 
			}


			ParaFile.Close(); 
		}
	}



	int i;



	i = 0;
	for(i =0; i< G_CurMaxHeatBox ;i++)
	{
		CString s;
		
		s.Format(_T("TEST_COOL_SPEED_%d"),i);
		m_SwitchDlg.m_iStepDownPidSpeed = AfxGetApp()->GetProfileInt(_T("SETTING"),s,0);
		m_ParaDlg.m_ComboBoxCoolSpeed.SetCurSel(m_SwitchDlg.m_iStepDownPidSpeed);  
		//m_SwitchDlg.m_iStepDownPidSpeed;

		s.Format(_T("TEST_SPEED_%d"),i);
		m_SwitchDlg.m_iStepPidSpeed = AfxGetApp()->GetProfileInt(_T("SETTING"),s,20);
		m_ParaDlg.m_ComboBoxHeatSpeed.SetCurSel(m_SwitchDlg.m_iStepPidSpeed);  
		m_SwitchDlg.m_iStepPidSpeed++;

		s.Format(_T("NORMAL_PID_MINUTES_%d"),i);
		m_SwitchDlg.m_iNormalPidMinutes = AfxGetApp()->GetProfileInt(_T("SETTING"),s,3);
		
		m_ParaDlg.m_iNormalPidMinutes = m_SwitchDlg.m_iNormalPidMinutes;
		
		
		s.Format(_T("SwitchNormalOpen_%d"),i); 
		m_SwitchDlg.m_SwitchNormalOpen = AfxGetApp()->GetProfileInt(_T("SETTING"),s,0); 
	
		CWnd *wnd;
		
		s.Format(_T("TEST_MODE_%d"),i);
		
		//AfxGetApp()->WriteProfileInt(_T("SETTING"),s,1);
		
		int curSel = AfxGetApp()->GetProfileInt(_T("SETTING"),s,0);
		m_ParaDlg.m_TestModeList.SetCurSel(curSel);  

		double m_OpenDoorResetHeatDegree;
		double m_OpenDoorResetCoolDegree;

		s.Format(_T("OPEN_DOOR_RESET_HEAT_DEGREE_%d"),i);
		m_OpenDoorResetHeatDegree = AfxGetApp()->GetProfileInt(_T("SETTING"),s,10) / 10.0f;	
		m_SwitchDlg.m_OpenDoorResetHeatDegree = m_OpenDoorResetHeatDegree;
		m_ParaDlg.m_OpenDoorResetHeatDegree = m_OpenDoorResetHeatDegree;

		s.Format(_T("OPEN_DOOR_RESET_COOL_DEGREE_%d"),i);
		m_OpenDoorResetCoolDegree= AfxGetApp()->GetProfileInt(_T("SETTING"),s,20) / 10.0f;	
		m_SwitchDlg.m_OpenDoorResetCoolDegree = m_OpenDoorResetCoolDegree;
		m_ParaDlg.m_OpenDoorResetCoolDegree = m_OpenDoorResetCoolDegree;
	
		wnd = m_ParaDlg.GetDlgItem(IDC_EDIT_HEAT_DEGREE);
		s.Format(_T("%.1f"),m_ParaDlg.m_OpenDoorResetHeatDegree);
		wnd->SetWindowText(s);

		wnd = m_ParaDlg.GetDlgItem(IDC_EDIT_COOL_DEGREE);
		s.Format(_T("%.1f"),m_ParaDlg.m_OpenDoorResetCoolDegree);
		wnd->SetWindowText(s);


		if(curSel != TEST_MODE_NORMAL)
		{
			wnd = m_ParaDlg.GetDlgItem(IDC_STATIC_HEAT);
			wnd->ShowWindow(SW_SHOW);
			wnd = m_ParaDlg.GetDlgItem(IDC_STATIC_COOL);
			wnd->ShowWindow(SW_SHOW);
			wnd = m_ParaDlg.GetDlgItem(IDC_EDIT_HEAT_DEGREE);
			wnd->ShowWindow(SW_SHOW);
			wnd = m_ParaDlg.GetDlgItem(IDC_EDIT_COOL_DEGREE);
			wnd->ShowWindow(SW_SHOW);
		}
		else
		{
			wnd = m_ParaDlg.GetDlgItem(IDC_STATIC_HEAT);
			wnd->ShowWindow(SW_HIDE);
			wnd = m_ParaDlg.GetDlgItem(IDC_STATIC_COOL);
			wnd->ShowWindow(SW_HIDE);
			wnd = m_ParaDlg.GetDlgItem(IDC_EDIT_HEAT_DEGREE);
			wnd->ShowWindow(SW_HIDE);
			wnd = m_ParaDlg.GetDlgItem(IDC_EDIT_COOL_DEGREE);
			wnd->ShowWindow(SW_HIDE);
		}

	}

	i = 0;
	for(i=0 ;i < G_CurMaxHeatBox;i++)
	{
		
		m_SwitchDlg.UpdateData(false); 
		m_CurveDlg.UpdateData(false); 
		m_ParaDlg.UpdateData(false);
		//m_TempParaDlg.m_ParaName = m_SwitchDlg.m_ParaName
		m_TempParaDlg.m_MainLevelStr.Format(_T("%5.1f,-%4.1f,+%4.1f   %5.1f,-%4.1f,+%4.1f"),
			m_SwitchDlg.m_SwitchConfigPara.MainLevel,m_SwitchDlg.m_SwitchConfigPara.MainLevelSpanMinus,m_SwitchDlg.m_SwitchConfigPara.MainLevelSpanPlus,
			m_SwitchDlg.m_SwitchConfigPara.CloseLevel,m_SwitchDlg.m_SwitchConfigPara.CloseLevelSpanMinus,m_SwitchDlg.m_SwitchConfigPara.CloseLevelSpanPlus);
		m_TempParaDlg.m_HighLevelStr.Format(_T("%5.1f,-%4.1f,+%4.1f   %5.1f,-%4.1f,+%4.1f"),
			m_SwitchDlg.m_SwitchConfigPara.HighLevel,m_SwitchDlg.m_SwitchConfigPara.HighLevelSpanMinus,m_SwitchDlg.m_SwitchConfigPara.HighLevelSpanPlus,
			m_SwitchDlg.m_SwitchConfigPara.CloseHighLevel,m_SwitchDlg.m_SwitchConfigPara.CloseHighLevelSpanMinus,m_SwitchDlg.m_SwitchConfigPara.CloseHighLevelSpanPlus);
		m_TempParaDlg.m_LowLevelStr.Format(_T("%5.1f,-%4.1f,+%4.1f   %5.1f,-%4.1f,+%4.1f"),
			m_SwitchDlg.m_SwitchConfigPara.LowLevel,m_SwitchDlg.m_SwitchConfigPara.LowLevelSpanMinus,m_SwitchDlg.m_SwitchConfigPara.LowLevelSpanPlus,
			m_SwitchDlg.m_SwitchConfigPara.CloseLowLevel,m_SwitchDlg.m_SwitchConfigPara.CloseLowLevelSpanMinus,m_SwitchDlg.m_SwitchConfigPara.CloseLowLevelSpanPlus);
		m_TempParaDlg.UpdateData(false); 
	}

	for(i=0 ;i < G_CurMaxHeatBox;i++)
	{
		CString s;
		s.Format(_T("HEAT_BOX_TYPE_%d"),i); 
		m_ParaDlg.m_HeatBoxType = AfxGetApp()->GetProfileInt(_T("SETTING"),s,0);	
		m_SwitchDlg.m_HeatBoxType = m_ParaDlg.m_HeatBoxType;
		m_ParaDlg.UpdateData(false); 
	}
	/*
	for(i=0 ;i < G_CurMaxHeatBox;i++)
	{
		CString s;
		s.Format(_T("DISPLAY_TEMP_TYPE_%d"),i); 
		m_SwitchDlg.m_iDisplayTempType = AfxGetApp()->GetProfileIntW(_T("SETTING"),s,0);
		m_ParaDlg.m_ComboBoxDisplayTempType.SetCurSel(m_SwitchDlg.m_iDisplayTempType);
		
	}*/

	for(i=0 ;i < G_CurMaxHeatBox;i++)
	{
		CString s;
		s.Format(_T("CONTINUE_TEST_COUNT_%d"),i); 
		m_SwitchDlg.m_iContinueTestCount = AfxGetApp()->GetProfileIntW(_T("SETTING"),s,0);
		s.Format(_T("%d"),m_SwitchDlg.m_iContinueTestCount);
		m_ParaDlg.m_EditContinueTestCount.SetWindowTextW(s);
	}
}

extern CString WriteCommMsg;

COMMAND_STRING G_CommandString[] = 
{
	{CMD_NULL                   ,		_T("CMD_NULL")},
	{CMD_STOP_OUT               ,       _T("停止加热")},
	{CMD_FULL_OUT               ,       _T("全速加热")},
	{CMD_NORMAL_PID_OUT         ,       _T("恒温")},
	{CMD_STEP_PID_OUT           ,       _T("线性升温")},
	{CMD_READ_TEMP				,       _T("读当前温度")},
	{CMD_READ_PID_T				,       _T("读PID参数")},
	{CMD_SET_PID_T				,       _T("设置PID参数")},
	{CMD_SET_ADJUST_LOW			,       _T("设置0°修正值")},
	{CMD_SET_ADJUST_HIGH		,       _T("设置200°修正值")},
	{CMD_READ_ADJUST_LOW		,       _T("读0°修正值")},
	{CMD_READ_ADJUST_HIGH		,       _T("读200°修正值")},
  
  	{CMD_READ_FLASH				,       _T("读闪动")},
	{CMD_SET_SWITCH_NORMAL_MODE	,       _T("设置保护器类型")},

     
    {CMD_READ_STATUS			,       _T("读保护器状态")},
	{CMD_START_OPEN_FLASH_TEST	,       _T("开始动闪测试")},
	{CMD_START_CLOSE_FLASH_TEST	,       _T("开始复位测试")},
	{CMD_END_FLASH_TEST			,       _T("结束闪动测试")},
	
    {CMD_SET_DISPLAY_TYPE		,       _T("设置显示模式")},
	{CMD_READ_ADJUST_ALL		,       _T("读温度总调")},
	{CMD_WRITE_STEP_PID_COUNT	,       _T("CMD_WRITE_STEP_PID_COUNT")},
	{CMD_READ_STEP_PID_COUNT	,       _T("CMD_READ_STEP_PID_COUNT")},
	{CMD_SET_COOL_RELAY			,       _T("设置 制冷")},
	{CMD_STEP_DOWN_PID_OUT      ,       _T("设置 线性 降温 ")},
};
CString GetCommandString(COMMAND_ID id)
{
	CString s;
		for(int i = 0 ; i < sizeof(G_CommandString) / sizeof(COMMAND_STRING); i++)
		{
			if(G_CommandString[i].ID == id)
			{
				s.Format(_T(" %X"), id); 
				s = G_CommandString[i].str + s;
				return s;
			}
		}

		s.Format(_T("无效的命令 %X"),id);

		return  s;
}

void CHeatBoxDlg::TempCmd(COMMAND_ID cmd_id,char* para) 
{
	TempCmd_500(cmd_id,para);
	
}


void CHeatBoxDlg::TempCmd_500(COMMAND_ID cmd_id,char* para) 
{

	if(this->m_SendmCmdProgram.GetCount() > 0)
	{
		MODBUS_RS485_CMD_PROGRAM cmd = m_SendmCmdProgram.GetAt(0); 
		this->m_Comm.WriteComm((char*)&cmd,sizeof(cmd));
		this->m_DebugDlg.m_CalMsg.Format(_T("发: %s"),GetCmdString(cmd.cmd));
		CString s = _T("");
		for(int i = 0; i < sizeof(cmd); i++)
		{
			CString s1;
			s1.Format(_T("%02X "),((unsigned char*)&cmd)[i]);
			s+=s1;
			
		}
		m_DebugDlg.m_CommSendString = _T("发送:") + s;
		m_DebugDlg.UpdateData(false);
		return;
	}


	if(m_DebugDlg.IsWindowVisible())
	{
		char buf[100];
		sprintf(buf,"m_SendCmdArray500.GetSize() = %d",m_SendCmdArray500.GetSize());  
		WriteLogFile(buf,1);
	}
	
			switch(m_MeterID)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:


				m_CurCmtType = 1;
				
	

				if(m_SendCmdArray500.GetSize() == 0)
				{
					if(m_Comm.IsOpened())
					{
							MODBUS_RS485_CMD_500 cmd_500;

							cmd_500.RS485_Addr	= 'A' + m_MeterID ;
							cmd_500.cmd			= CMD_READ_STATUS;

							cmd_500.para_len = sizeof(cmd_500.para);

							memset(cmd_500.para,0,sizeof(cmd_500.para));

							CComm::CRC16_AFF2((unsigned char*)&cmd_500,sizeof(cmd_500) - 2,(unsigned char*)cmd_500.crc); 

							this->m_DebugDlg.m_SendCmdCount ++;
							CWnd * w = m_DebugDlg.GetDlgItem(IDC_EDIT_SEND_CMD_COUNT);
							CString s;
							s.Format(_T("%d"), m_DebugDlg.m_SendCmdCount);
							w->SetWindowTextW(s); 
							
							s = _T("");
							for(int i = 0; i < sizeof(cmd_500); i++)
							{
								CString s1;
								s1.Format(_T("%02X "),((unsigned char*)&cmd_500)[i]);
								s+=s1;
								
							}
							m_DebugDlg.m_CommSendString = _T("发送:") + s;
							m_DebugDlg.UpdateData(false);
							this->m_DebugDlg.m_CalMsg.Format(_T("发:%s%d"),GetCmdString(cmd_500.cmd),m_MeterID);
							m_DebugDlg.UpdateData(false);

							this->m_Comm.WriteComm((char*)&cmd_500,sizeof(cmd_500));
				
					}
				}
				else
				{
					if(m_Comm.IsOpened())
					{
							
							this->m_DebugDlg.m_SendCmdCount ++;

							CWnd * w = m_DebugDlg.GetDlgItem(IDC_EDIT_SEND_CMD_COUNT);
							CString s,s1;
							s.Format(_T("%d"), m_DebugDlg.m_SendCmdCount);
							w->SetWindowTextW(s); 
							
							MODBUS_RS485_CMD_500 cmd_500 = m_SendCmdArray500.GetAt(0); 

							this->m_Comm.WriteComm((char*)&cmd_500,sizeof(cmd_500));
							
							s = _T("");
							for(int i = 0; i < sizeof(cmd_500); i++)
							{
								s1.Format(_T("%02X "),((char*)&cmd_500)[i]);
								s+=s1;
							}
							m_DebugDlg.m_CommSendString = _T("发送:") + s;

							this->m_DebugDlg.m_CalMsg.Format(_T("发A:%s"),GetCmdString(cmd_500.cmd));
							this->m_DebugDlg.UpdateData(false); 

							WriteLogFile(("发A"),1);
		
					}
				}
				break;
			case 5:
			default:
				m_CurCmtType = 0;
				MODBUS_RS485_CMD_OMRON 	cmd_omron;

				cmd_omron.RS485_Addr 	= 1;
				cmd_omron.cmd			= CMD_OMRON_READ;

				memset(cmd_omron.para,0,sizeof(cmd_omron.para));
				*((short * )&cmd_omron.para[0]) = CDebugDlg::PcShort2OmronShort(OMRON_PV);

				*((short * )&cmd_omron.para[2]) = CDebugDlg::PcShort2OmronShort(0x0001);

				CComm::CRC16((unsigned char*)&cmd_omron,sizeof(cmd_omron) - 2,(unsigned char*)cmd_omron.crc); 
	

				if(this->m_SendCmdArray_OMRON.GetSize() == 0)
				{
					if(m_Comm.IsOpened())
					{
							this->m_DebugDlg.m_SendCmdCount ++;
							CWnd * w = m_DebugDlg.GetDlgItem(IDC_EDIT_SEND_CMD_COUNT);
							CString s;
							s.Format(_T("%d"), m_DebugDlg.m_SendCmdCount);
							w->SetWindowTextW(s); 
							
							s = _T("");
							for(int i = 0; i < sizeof(cmd_omron); i++)
							{
								CString s1;
								s1.Format(_T("%02X "),((unsigned char*)&cmd_omron)[i]);
								s+=s1;
								
							}
							m_DebugDlg.m_CommSendString = _T("发送:") + s;

							this->m_DebugDlg.m_CalMsg.Format(_T("发B:%s"),GetCmdString(cmd_omron.cmd));
							
							m_DebugDlg.UpdateData(false);
							
							m_SendCmdArray_OMRON.Add( cmd_omron );

							this->m_Comm.WriteComm((char*)&cmd_omron,sizeof(cmd_omron));
				
					}
				}
				else
				{
					if(m_Comm.IsOpened())
					{
							
							this->m_DebugDlg.m_SendCmdCount ++;
							CWnd * w = m_DebugDlg.GetDlgItem(IDC_EDIT_SEND_CMD_COUNT);
							CString s,s1;
							s.Format(_T("%d"), m_DebugDlg.m_SendCmdCount);
							w->SetWindowTextW(s); 
							
							cmd_omron = this->m_SendCmdArray_OMRON.GetAt(0);  
							this->m_Comm.WriteComm((char*)&cmd_omron,sizeof(cmd_omron));
							
							s = _T("");
							for(int i = 0; i < sizeof(cmd_omron); i++)
							{
								s1.Format(_T("%02X "),((unsigned char*)&cmd_omron)[i]);
								s+=s1;
							}
							m_DebugDlg.m_CommSendString = _T("发送:") + s;

							this->m_DebugDlg.m_CalMsg.Format(_T("发C:%s"),GetCmdString(cmd_omron.cmd));
							this->m_DebugDlg.UpdateData(false); 
		
					}
				}
				break;
			}

		m_MeterID ++;

	
	
		

		if(m_MeterID > 5)
		{

			m_MeterID = 0;

		}

}


void CHeatBoxDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	static char para[38];
	memset(para,0,sizeof(para));
	TempCmd(CMD_READ_TEMP,para);

	CTime cur;
	cur = cur.GetCurrentTime();
	CString s;
	TCHAR format[] = _T("  %04d-%02d-%02d %02d:%02d:%02d\r\n");
	s.Format(format,cur.GetYear(),cur.GetMonth(),cur.GetDay(),cur.GetHour(),cur.GetMinute(),cur.GetSecond());
	this->SetWindowText(strMsgCaption + s);
	
	CDialog::OnTimer(nIDEvent);
	

}

bool CHeatBoxDlg::CheckSameTemp(int BoxNr)
{
	
	if(m_LastCurTemp[BoxNr][0].GetSize() < 5)
	{
		return false;
	}

	for(int i = 0; i < (m_LastCurTemp[BoxNr][0].GetSize() -1); i++)
	{
		if(m_LastCurTemp[BoxNr][0].GetAt(i) !=  m_LastCurTemp[BoxNr][0].GetAt(i+1))
		{
			return false;
		}
	}

	for(int i = 0; i < (m_LastCurTemp[BoxNr][0].GetSize() -1); i++)
	{
		if(m_LastCurTemp[BoxNr][1].GetAt(i) !=  m_LastCurTemp[BoxNr][1].GetAt(i+1))
		{
			return false;
		}
	}

	for(int i = 0; i < (m_LastCurTemp[BoxNr][0].GetSize() -1); i++)
	{
		if(m_LastCurTemp[BoxNr][2].GetAt(i) !=  m_LastCurTemp[BoxNr][2].GetAt(i+1))
		{
			return false;
		}
	}

	for(int i = 0; i < (m_LastCurTemp[BoxNr][0].GetSize() -1); i++)
	{
		if(m_LastCurTemp[BoxNr][3].GetAt(i) !=  m_LastCurTemp[BoxNr][3].GetAt(i+1))
		{
			return false;
		}
	}

	return true;
}

LRESULT CHeatBoxDlg::OnComm(WPARAM wParam,LPARAM lParam)
{
	int i,source_id,data_len;
	CTime t;
	t=t.GetCurrentTime(); 
	char rec_cmd[1024];

	

	double temp[4];
	short para[4];
	short short_a;

	memset(rec_cmd,0,sizeof(rec_cmd));
	memcpy(rec_cmd,(char*)wParam,lParam);
	data_len = lParam;


	CString s,s1;

	
	

	MODBUS_RS485_CMD_128    *modbus_cmd_128 =(MODBUS_RS485_CMD_128*) rec_cmd;
	MODBUS_RS485_CMD_PROGRAM * cmd_program = (MODBUS_RS485_CMD_PROGRAM*) rec_cmd;

	MODBUS_RS485_ANSWER_OMRON_READ		*answer_omron_read  =(MODBUS_RS485_ANSWER_OMRON_READ*)  rec_cmd;
	MODBUS_RS485_ANSWER_OMRON_WRITE		*answer_omron_write =(MODBUS_RS485_ANSWER_OMRON_WRITE*) rec_cmd;
	MODBUS_RS485_ANSWER_500				*answer_500 = (MODBUS_RS485_ANSWER_500*) rec_cmd;

	unsigned char addr ;

	
	{
		addr = modbus_cmd_128->RS485_Addr;
		if((data_len != sizeof(MODBUS_RS485_CMD_128))
			&& (data_len != sizeof(MODBUS_RS485_CMD_500))
			&& (data_len != sizeof(MODBUS_RS485_CMD_PROGRAM))
			&& (data_len != sizeof(MODBUS_RS485_ANSWER_OMRON_READ))
			&& (data_len != sizeof(MODBUS_RS485_ANSWER_OMRON_WRITE))
			&& (data_len != sizeof(MODBUS_RS485_ANSWER_500))
			)
		{
			return 0;
		}
	}

	switch(addr)
	{
	case 1:
	case '1':
    case 'A':
		source_id = 0;
		break;

	case '2':
    case 'B':
		source_id = 1;
		break;

	case '3':
	case 'C':
		source_id = 2;
		break;

	case '4':
	case 'D':
		source_id = 3;
		break;
	case '5':
	case 'E':
		source_id = 4;
		break;
    
	default:
		/*{
			
			s.Format(_T("无效的设备编号 %d:"),addr);
			for(i = 0; i < data_len; i++)
			{
				s1.Format(_T("%02X "),(unsigned char)rec_cmd[i]);
				s+=s1;
			}
			AfxMessageBox(s);
		}*/
		return 0;
	}

	CWnd * w = m_DebugDlg.GetDlgItem(IDC_EDIT_RECEIVE_CMD_COUNT);
	m_DebugDlg.m_ReceiveCmdCount++;
	s.Format(_T("%d"), m_DebugDlg.m_ReceiveCmdCount);
	w->SetWindowTextW(s); 

	
	unsigned char cmd;
	
	cmd = modbus_cmd_128->cmd;
	

	
	{
		if(modbus_cmd_128->cmd  != CMD_READ_STATUS
			&&  modbus_cmd_128->cmd  != CMD_READ_TEMP)
		{
			s = _T("");
			for(int i = 0; i < data_len; i++)
			{
				s1.Format(_T("%02X "),(unsigned char)rec_cmd[i]);
				s+=s1;
			}
			m_DebugDlg.m_CommRecString = _T("接收") + s;
			m_DebugDlg.UpdateData(false);

			s1.Format(_T(" 接:%s(%d)"),::GetCmdString(cmd),source_id+1); 
			this->m_DebugDlg.m_CalMsg += s1;
			this->m_DebugDlg.UpdateData(false);
			if(G_iMaxLedNr == 500)
			{
				if(m_SendCmdArray500.GetSize() > 0)
				{
					MODBUS_RS485_CMD_500 cmd = m_SendCmdArray500.GetAt(0);
					s1.Format(_T("%d"), cmd.cmd );
				//	m_SendCmdArray500[source_id].RemoveAt(0); 
				}
			}
			
		}
		else
		{
			s = _T("");
			for(int i = 0; i < data_len; i++)
			{
				s1.Format(_T("%02X "),(unsigned char)rec_cmd[i]);
				s+=s1;
			}
			m_DebugDlg.m_CommRecString = _T("接收") + s;
			m_DebugDlg.UpdateData(false);

			s1.Format(_T(" 接:%s(%d)"),::GetCmdString(cmd),source_id+1);
			this->m_DebugDlg.m_CalMsg += s1;
			this->m_DebugDlg.UpdateData(false);
		}
	}


	


	switch(cmd)
	{

	case CMD_OMRON_WRITE:
			if(m_SendCmdArray_OMRON.GetSize() > 0)
			{
				m_SendCmdArray_OMRON.RemoveAt(0); 
			}
			break;
	case CMD_SET_AIR_VALVE:
		if(m_SendCmdArray500.GetSize() > 0)
		{
			m_SendCmdArray500.RemoveAt(0); 
		}
		//AfxMessageBox(_T("CMD_SET_AIR_VALVE"));
		if(modbus_cmd_128->para[0] == 0)
		{
			WriteLogFile(("CMD_SET_AIR_VALVE del 0"),1);
		}
		else
		{
			WriteLogFile(("CMD_SET_AIR_VALVE del 1"),1);
		}
		break;
	case CMD_SET_AIR_CONTROL:
		if(m_SendCmdArray500.GetSize() > 0)
		{
			m_SendCmdArray500.RemoveAt(0); 
		}
		//AfxMessageBox(_T("CMD_SET_AIR_CONTROL"));

		WriteLogFile(("CMD_SET_AIR_CONTROL"),1);
		break;
	case CMD_SET_HEAT:
		if(m_SendCmdArray500.GetSize() > 0)
		{
			m_SendCmdArray500.RemoveAt(0); 
		}
	//	AfxMessageBox(_T("CMD_SET_HEAT"));
		WriteLogFile(("CMD_SET_HEAT"),1);
		break;
	case CMD_SET_FAN:
		if(m_SendCmdArray500.GetSize() > 0)
		{
			m_SendCmdArray500.RemoveAt(0); 
		}
	//	AfxMessageBox(_T("CMD_SET_FAN"));
		WriteLogFile(("CMD_SET_FAN"),1);
		break;
	case CMD_SET_SSR_5:
		if(m_SendCmdArray500.GetSize() > 0)
		{
			m_SendCmdArray500.RemoveAt(0); 
		}
		WriteLogFile(("SSR5"),1);
		break;

	;
	case CMD_PROGRAM_START:
		if(this->m_SendmCmdProgram.GetSize() > 0) 
		{
			m_SendmCmdProgram.RemoveAt(0); 
		}
		//AfxMessageBox(_T("CMD_PROGRAM_START"));
		break;
	case CMD_PROGRAM_DATA:
		if(this->m_SendmCmdProgram.GetSize() > 0) 
		{
			m_SendmCmdProgram.RemoveAt(0); 
		}
		if(this->m_SendmCmdProgram.GetSize() == 0) 
		{
			AfxMessageBox(_T("程序更新结束"));
		}
		//AfxMessageBox(_T("CMD_PROGRAM_DATA"));
		break;
	case CMD_PROGRAM_END:
		if(this->m_SendmCmdProgram.GetSize() > 0) 
		{
			m_SendmCmdProgram.RemoveAt(0); 
		}
		//AfxMessageBox(_T("程序更新结束"));
		break;

	case CMD_STEP_DOWN_PID_OUT:
		WriteLogFile("CMD_STEP_DOWN_PID_OUT",source_id+1);
		break;
	case CMD_SET_COOL_RELAY:
		{
			WriteLogFile("CMD_SET_COOL_RELAY",source_id+1);
		}
		break;
	case CMD_SET_SWITCH_NORMAL_MODE:
		{
			WriteLogFile("CMD_SET_SWITCH_NORMAL_MODE",source_id+1);
		}
		break;
//#ifndef _ASCII_
	case CMD_WRITE_STEP_PID_COUNT:
		
		break;
//#endif
//#ifndef _ASCII_
	case CMD_READ_STEP_PID_COUNT:
		
		
		break;
//#endif
 
	case CMD_CPLD_RESET:
		WriteLogFile("CMD_CPLD_RESET",source_id+1);

		break;
//#endif


	
//#ifndef _ASCII_
	case CMD_READ_ADJUST_ALL:
		WriteLogFile("CMD_READ_ADJUST_ALL",source_id+1);

		m_DebugDlg.DealCmdAnswer( CMD_READ_ADJUST_ALL,rec_cmd,0,0);
		break;
//#endif

	case CMD_READ_STATUS:
		if(m_SendCmdArray500.GetSize() > 0)
		{
			m_SendCmdArray500.RemoveAt(0); 
		}

#ifdef _INTERFACE_5X20_
		if(data_len == (2 + 1 + 13 * 2 + 2))
		{
			for(i = 0; i < 13 ; i++)
			{
			//	m_SwitchStatus[0][i + 13 * source_id] = answer_500->para[i];
			}
			m_SwitchDlg.DealCmdAnswer(rec_cmd,0,0,source_id);
			m_DebugDlg.DealCmdAnswer( CMD_READ_STATUS,rec_cmd,0,0);
		}
		else
		{
			char s[100];
			sprintf_s(s,"CMD_READ_FPGA_STATUS datalen = %d\r\n", data_len);
			WriteLogFile(s,source_id+1);
		}
#else
		if(data_len == (2 + 1 + 13 * 2 + 2))
		{
			for(i = 0; i < sizeof(m_SwitchStatus[0]) ; i++)
			{
				m_SwitchStatus[0][i + 13 * source_id] = answer_500->para[i];
			}
			m_SwitchDlg.DealCmdAnswer(rec_cmd,0,0);
			m_DebugDlg[0].DealCmdAnswer( CMD_READ_STATUS,rec_cmd,0,0);
		}
		else
		{
			char s[100];
			sprintf_s(s,"CMD_READ_FPGA_STATUS datalen = %d\r\n", data_len);
			WriteLogFile(s,source_id+1);
		}
#endif		


		break;



	case CMD_OMRON_READ:

		{
			if(m_SendCmdArray_OMRON.GetSize() > 0)
			{
				MODBUS_RS485_CMD_OMRON cmd_omron = m_SendCmdArray_OMRON.GetAt(0); 
				m_SendCmdArray_OMRON.RemoveAt(0); 
				char para[100];
				memset(para,0,sizeof(para));
				short addr = CDebugDlg::OmronShort2PcShort(*((short*)cmd_omron.para));
				switch(addr)
				{
				case OMRON_PID_I:
				case OMRON_PID_P:
				case OMRON_PID_D:
				case OMRON_PID_T:
					para[0] = cmd_omron.para[0];
					para[1] = cmd_omron.para[1];
					para[2] = answer_omron_read->para[0];
					para[3] = answer_omron_read->para[1];
						m_DebugDlg.DealCmdAnswer( CMD_OMRON_READ,para,0,0);
						break;
					case OMRON_PV:
					for(i = 0; i < 4; i++)
					{
						SHORT_BUF sb;
						sb.buf[0] =  answer_omron_read->para[1];
						sb.buf[1] =  answer_omron_read->para[0];

						m_CurTemp[0][i] = sb.v  / 10.0f;
						
						m_SwitchDlg.m_CurTemp[i] = sb.v  / 10.0f;


					}
					
					

					m_SwitchDlg.m_PID_Temp = m_SwitchDlg.m_CurTemp[0];
					
					m_SwitchDlg.UpdateData(false);

					//更新有关窗口
					m_SwitchDlg.DealCmdAnswer((char*)answer_omron_read,0,0,source_id);
					m_CurveDlg.UpdateTemp(m_SwitchDlg.m_CurTemp);
					m_DebugDlg.DealCmdAnswer( CMD_OMRON_READ,rec_cmd,0,0);
					break;
				}
			}
		}
		break;


	case CMD_READ_PID_T:
		WriteLogFile("CMD_READ_PID_T",source_id+1);
		m_DebugDlg.DealCmdAnswer(CMD_READ_PID_T,rec_cmd,0,0);
		break;
	case CMD_SET_PID_T:
		WriteLogFile("CMD_SET_PID_T",source_id+1);
		break;
	case CMD_SET_ADJUST_LOW:
		WriteLogFile("CMD_SET_ADJUST_LOW",source_id+1);
		break;
	case CMD_SET_ADJUST_HIGH:
		WriteLogFile("CMD_SET_ADJUST_HIGH",source_id+1);
		break;
	case CMD_READ_ADJUST_LOW:
		WriteLogFile("CMD_READ_ADJUST_LOW",source_id+1);
		
		{//MODBUS
			for(i = 0; i< 4; i++)
			{
				char *buf= (char*)&short_a;
				if(G_iMaxLedNr == 500)
				{
					buf [0] = answer_500->para[2*i];
					buf [1] = answer_500->para[1 + 2*i];
				}
				else
				{
					buf [0] = modbus_cmd_128->para[2*i];
					buf [1] = modbus_cmd_128->para[1 + 2*i];
				}
				para[i] = short_a;
			}
		}
		m_DebugDlg.DealCmdAnswer( CMD_READ_ADJUST_LOW,rec_cmd,0,0);
		break;
	case CMD_READ_ADJUST_HIGH:
		WriteLogFile("CMD_READ_ADJUST_HIGH",source_id+1);
		
		{//MODBUS
			for(i = 0; i< 4; i++)
			{
				char *buf= (char*)&short_a;
				if(G_iMaxLedNr == 500)
				{
					buf [0] = answer_500->para[2*i];
					buf [1] = answer_500->para[1 + 2*i];
				}
				else
				{
					buf [0] = modbus_cmd_128->para[2*i];
					buf [1] = modbus_cmd_128->para[1 + 2*i];
				}
				para[i] = short_a;
			}
		}

		m_DebugDlg.DealCmdAnswer( CMD_READ_ADJUST_HIGH,rec_cmd,0,0);
		break;


	

	

	case CMD_NORMAL_PID_OUT:
//		m_SwitchDlg[source_id].m_TestMsg = "CMD_NORMAL_PID_OUT";
//		m_SwitchDlg[source_id].UpdateData(false);
		WriteLogFile("CMD_NORMAL_PID_OUT",source_id+1);
		m_DebugDlg.DealCmdAnswer( CMD_NORMAL_PID_OUT,rec_cmd,0,0);
		break;
    case CMD_STEP_PID_OUT:
//		m_SwitchDlg[source_id].m_TestMsg = "CMD_STEP_PID_OUT";
//		m_SwitchDlg[source_id].UpdateData(false);
		WriteLogFile("CMD_STEP_PID_OUT",source_id+1);
		m_DebugDlg.DealCmdAnswer( CMD_STEP_PID_OUT,rec_cmd,0,0);
		
		break;
    case 	CMD_STOP_OUT:
//		m_SwitchDlg[source_id].m_TestMsg = "CMD_STOP_OUT";
//		m_SwitchDlg[source_id].UpdateData(false);
		WriteLogFile("CMD_STOP_OUT",source_id+1);
		break;
	case 	CMD_FULL_OUT:
//		m_SwitchDlg[source_id].m_TestMsg = "CMD_FULL_OUT";
//		m_SwitchDlg[source_id].UpdateData(false);
		WriteLogFile("CMD_FULL_OUT",source_id+1);
		break;
    default:
		{
			s.Format(_T("错误命令 = %X:"),rec_cmd[1]);
			for(int i = 0; i < data_len; i++)
			{
				s1.Format(_T("%02X "),(unsigned char)rec_cmd[i]);
				s+=s1;
			}
			//AfxMessageBox(s);
			
			m_SwitchDlg.m_TestMsg = s;

			m_SwitchDlg.UpdateData(false);
		}
		break;
	}

	return 0;

}
void CHeatBoxDlg::SetCmdString_ASCII(ASCII_RS485_COMMAND *cmd,int equip, int cmdId, short para[4])
{
		cmd->header.start		=	CMD_STX;
		cmd->end				=	CMD_ETX;
		cmd->header.source_id	=	'0'+9;
		cmd->header.des_id		=	equip;
		cmd->header.command		=	cmdId;
		
		
		char p[16];
		sprintf_s(p,		"%04d", para[0]);
		memcpy(cmd->val+0,p,4); 
		sprintf_s(p,		"%04d", para[1]);
		memcpy(cmd->val+4,p,4); 
		sprintf_s(p,		"%04d", para[2]);
		memcpy(cmd->val+8,p,4); 
		sprintf_s(p,		"%04d",	para[3]);
		memcpy(cmd->val+12,p,4); 
		
		
		unsigned char *pCmd=(unsigned char*)cmd;
		unsigned char chksum=0;

		chksum = CalCheckSum((char*)&pCmd[1], sizeof(ASCII_RS485_COMMAND)-4);

		cmd->chk[0]= (chksum & 0x0F)    + '0';
		cmd->chk[1]=((chksum & 0xF0) >> 4)+ '0';
		
		//m_SendCmdArrayASC[equip].Add(*cmd);
}

void CHeatBoxDlg::SetCmdString(int equip, COMMAND_ID cmdId, char para[4])
{

		int i,j;
		int box = 0;

		MODBUS_RS485_CMD_500 cmd;
		
		cmd.RS485_Addr = 'A';
		cmd.cmd  = cmdId;
		cmd.para_len = sizeof(cmd.para) ; 
		for(i = 0; i < sizeof(cmd.para); i++)
		{
			((char*)cmd.para)[i] = para[i];
		}
		

		CComm::CRC16_AFF2((unsigned char *)&cmd,sizeof(cmd)-2,(unsigned char*)cmd.crc);
		i = m_SendCmdArray500.GetCount(); 
		m_SendCmdArray500.Add(cmd); 
		m_SendCmdArray500.Add(cmd);
		m_SendCmdArray500.Add(cmd); 

		j = m_SendCmdArray500.GetCount(); 
		if(j == (i + 3))
		{
			WriteLogFile(("m_SendCmdArray500.Add Success"),1);
		}
		else
		{
			WriteLogFile(("m_SendCmdArray500.Add Fail"),1);
		}
		

}

void CHeatBoxDlg::SetCmdHeat(bool On)
{
	char para[16];

	
	memset(para,0,sizeof(para));
	if(On)
	{
		para[0] = 1;  
	}
	else
	{
		para[0] = 0;  
	}
	
	
	SetCmdString( 'A' ,CMD_SET_HEAT, para);
}

void CHeatBoxDlg::SetCmdFan(bool On)
{
	char para[16];

	
	memset(para,0,sizeof(para));
	if(On)
	{
		para[0] = 1;  
	}
	else
	{
		para[0] = 0;  
	}
	
	
	SetCmdString( 'A' ,CMD_SET_FAN, para);
	SetCmdString( 'A' ,CMD_SET_FAN, para);
	SetCmdString( 'A' ,CMD_SET_FAN, para);

}
void CHeatBoxDlg::SetCmdAirControl(bool On)
{
	char para[16];

	
	memset(para,0,sizeof(para));

	if(On)
	{
		para[0] = 1;  

	//	AfxMessageBox(_T("asdsa   1"));
	}
	else
	{
		para[0] = 0;  
	//	AfxMessageBox(_T("asdsa   0"));
	}
	
	
	SetCmdString( 'A' ,CMD_SET_AIR_CONTROL, para);
	
}
void CHeatBoxDlg::SetCmdAirValve(bool On)
{
	char para[16];

	
	memset(para,0,sizeof(para));
	if(On)
	{
		para[0] = 1;  //降温速度变慢
	}
	else
	{
		para[0] = 0;  
	}
	
	
	SetCmdString( 'A',CMD_SET_AIR_VALVE, para);

}

void CHeatBoxDlg::SetCmdString_OMRON(int equip, COMMAND_ID cmdId, char para[16])
{

		int i;
		int box = 0;
		
		MODBUS_RS485_CMD_OMRON cmd;
		
		cmd.RS485_Addr = 1;
		cmd.cmd  = cmdId;
		for(i = 0; i < sizeof(cmd.para); i++)
		{
			cmd.para[i] = para[i];
		}
		

		CComm::CRC16((unsigned char *)&cmd,sizeof(cmd)-2,(unsigned char*)cmd.crc);
		
		m_SendCmdArray_OMRON.Add(cmd);


}

void CHeatBoxDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	for(int i = 0; i < MAX_HEAT_BOX;i++)
	{
		if(m_SwitchDlg.m_WorkStatus.AutoTestFlag)
		{
			CString str;
			str.Format(_T("%d 号烘箱正在测试，请先停止!"),i+1);
			int ret = MessageBox(str,strMsgCaption,MB_OK | MB_ICONSTOP);
			return;
		}
	}
	KillTimer(NULL);
	if(m_Comm.IsOpened())
	{
		m_Comm.CloseComm();
		Sleep(100);
	}
	WriteLogFile("CLOSE",1);
	WriteLogFile("CLOSE",2);
	WriteLogFile("CLOSE",3);
	WriteLogFile("CLOSE",4);
	
	CDialog::OnClose();

}



void CHeatBoxDlg::InitDatabase()
{
	
	CFile ParaFile;
	CFileFind ff;
	SWITCH_CONFIG_PARA	para;
	CString fName;
	CString sDir;

	
	sDir = theAppDirectory +_T("para\\");

	if(!PathIsDirectory(sDir))
	{
		::CreateDirectory(sDir,NULL); 

		AfxMessageBox(_T("这是新程序第一次运行，请重新设置所有生产参数"),NULL,MB_ICONINFORMATION); 
	}
/*
	for(int i =0;i < G_CurMaxHeatBox; i++)
	{
		CString str1;

		str1.Format(_T("SWITCH_CONFIG_PARA_BOX_NR_%d"),i); 
		CString para_name = AfxGetApp()->GetProfileString(_T("SETTING"),
			str1,
			_T("测试参数"));

		para_name += _T(".para");

		if(ParaFile.Open(sDir + para_name,CFile::modeCreate |  CFile::modeNoTruncate | CFile::modeReadWrite))
		{
			memset(&para,0,sizeof(para));
			
			if(ParaFile.Read(&para,sizeof(SWITCH_CONFIG_PARA)) <= sizeof(SWITCH_CONFIG_PARA)) 
			{
				
		
			}
			else
			{
				//para.m_para_name				= _T("测试参数");
				para.ParaType					= 0;
				para.OpenDoorResetLevel			=   0; 
				para.MainLevel					=	90;
				para.MainLevelSpanMinus			=	5;
				para.MainLevelSpanPlus			=	5;
				para.HighLevel					=	95;
				para.HighLevelSpanMinus			=	5;
				para.HighLevelSpanPlus			=	5;
				para.LowLevel					=	85;
				para.LowLevelSpanMinus			=	5;
				para.LowLevelSpanPlus			=	5;
				para.CloseLevel					=	50;
				para.CloseLevelSpanPlus			=	5;
				para.CloseLevelSpanMinus		=	5;
				para.CloseHighLevel				=	55;
				para.CloseHighLevelSpanPlus		=	5;
				para.CloseHighLevelSpanMinus	=	5;
				para.CloseLowLevel				=	45;
				para.CloseLowLevelSpanPlus		=	5;
				para.CloseLowLevelSpanMinus		=	5;
				
				para.OpenDoorResetLevel			=   0;
				para.TakeoutTime				=	0;

				ParaFile.SeekToBegin();
				ParaFile.Write(&para,sizeof(para)); 
			}


			ParaFile.Close(); 
		}
	}
*/
	return;
/*
	TCHAR str[5000];
//	CFileFind ff;
	CString mdbFileName;
	mdbFileName.Format(_T("%s.mdb"),AfxGetApp()->m_pszExeName); 

	
	if(!ff.FindFile(mdbFileName))
	
	{
		try
		{
			m_DaoDatabase.Create(mdbFileName);

			str[0] = 0;
			_tcscat_s(str,_T("CREATE TABLE switch_test_para("));
			_tcscat_s(str,_T("para_name char(10),"));
			_tcscat_s(str,_T("ParaType int,"));
			_tcscat_s(str,_T("MainLevel double,MainLevelSpanMinus double,MainLevelSpanPlus double,"));
			_tcscat_s(str,_T("HighLevel double,HighLevelSpanMinus double,HighLevelSpanPlus double,"));
			_tcscat_s(str,_T("LowLevel double, LowLevelSpanMinus double, LowLevelSpanPlus double,"));
			_tcscat_s(str,_T("CloseMainLevel double, CloseMainLevelSpanPlus double, CloseMainLevelSpanMinus double,"));
			_tcscat_s(str,_T("CloseHighLevel double, CloseHighLevelSpanPlus double, CloseHighLevelSpanMinus double,"));
			_tcscat_s(str,_T("CloseLowLevel double, CloseLowLevelSpanPlus double, CloseLowLevelSpanMinus double,"));
			_tcscat_s(str,_T("OpenDoorResetLevel int,TakeoutTime int)"));
			m_DaoDatabase.Execute(str); 

			
			CParaRecordset para_rs;
			para_rs.Open();
			para_rs.AddNew();
			para_rs.m_para_name				= _T("测试参数");
			para_rs.m_ParaType				= 0;
			para_rs.m_OpenDoorResetLevel	=   0; 
			para_rs.m_MainLevel				=	90;
			para_rs.m_MainLevelSpanMinus	=	5;
			para_rs.m_MainLevelSpanPlus		=	5;
			para_rs.m_HighLevel				=	95;
			para_rs.m_HighLevelSpanMinus	=	5;
			para_rs.m_HighLevelSpanPlus		=	5;
			para_rs.m_LowLevel				=	85;
			para_rs.m_LowLevelSpanMinus		=	5;
			para_rs.m_LowLevelSpanPlus		=	5;
			para_rs.m_CloseMainLevel			=	50;
			para_rs.m_CloseMainLevelSpanPlus	=	5;
			para_rs.m_CloseMainLevelSpanMinus	=	5;
			para_rs.m_CloseHighLevel			=	55;
			para_rs.m_CloseHighLevelSpanPlus	=	5;
			para_rs.m_CloseHighLevelSpanMinus	=	5;
			para_rs.m_CloseLowLevel				=	45;
			para_rs.m_CloseLowLevelSpanPlus		=	5;
			para_rs.m_CloseLowLevelSpanMinus	=	5;
			
			para_rs.m_TakeoutTime			=	0;
			
			para_rs.Update(); 
			para_rs.Close(); 

			
			str[0] = 0;
			_tcscat_s(str,_T("CREATE TABLE ChanliangTongji("));
			_tcscat_s(str,_T("BeginTime date,"));
			_tcscat_s(str,_T("ProductTime date,"));
			_tcscat_s(str,_T("BoxNr int,"));
			_tcscat_s(str,_T("MainLevel int,"));
			_tcscat_s(str,_T("HighLevel int,"));
			_tcscat_s(str,_T("LowLevel int,"));
			_tcscat_s(str,_T("OverLowLevel int,"));
			_tcscat_s(str,_T("OverHighLevel int,"));
			_tcscat_s(str,_T("CloseHighLevel int,"));
			_tcscat_s(str,_T("CloseLowLevel int,"));
			_tcscat_s(str,_T("OpenFlashLevel int,"));
			_tcscat_s(str,_T("CloseFlashLevel int,"));
			_tcscat_s(str,_T("OpenDoorResetLevel int,"));
			_tcscat_s(str,_T("NotUsedLevel int)"));
			m_DaoDatabase.Execute(str); 

			
			str[0] = 0;
			_tcscat_s(str,_T("CREATE TABLE temp_cal_para("));
			_tcscat_s(str,_T("Box_Nr int,"));
			_tcscat_s(str,_T("AdjLow_0 float,AdjLow_1 float,AdjLow_2 float,AdjLow_3 float,"));
			_tcscat_s(str,_T("AdjHigh_0 float,AdjHigh_1 float,AdjHigh_2 float,AdjHigh_3 float)"));
			m_DaoDatabase.Execute(str); 
			
			
			CTempAdjRst adj_rst;
			adj_rst.Open();
			for(int i = 0; i < MAX_HEAT_BOX; i++)
			{
				adj_rst.AddNew();
				adj_rst.m_Box_Nr = i;
				adj_rst.m_AdjLow_0 = 0.0f;
				adj_rst.m_AdjLow_1 = 0.0f;
				adj_rst.m_AdjLow_2 = 0.0f;
				adj_rst.m_AdjLow_3 = 0.0f;
				adj_rst.m_AdjHigh_0 = 0.0f;
				adj_rst.m_AdjHigh_1 = 0.0f;
				adj_rst.m_AdjHigh_2 = 0.0f;
				adj_rst.m_AdjHigh_3 = 0.0f;
				adj_rst.Update();
			}

			m_DaoDatabase.Close();
			
		}
		catch(CDaoException e)
		{
			AfxMessageBox(e.m_pErrorInfo->m_strDescription);   
		}
		AfxMessageBox(_T("这是新程序第一次运行，请重新设置所有生产参数"),NULL,MB_ICONINFORMATION);   
	}
	else
	{
		
	}*/
	
}

BOOL CAboutDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
//return true;

	CString s;  
	GetModuleFileName(NULL,s.GetBufferSetLength(MAX_PATH+1),MAX_PATH+1);  
    s.ReleaseBuffer();

	CFileStatus fs;
	if(CFile::GetStatus(s,fs))
	{
		m_CompileTime =_T("更新时间 ") + fs.m_mtime.Format(_T("%Y/%m/%d %H:%M:%S"));  
		m_CompileTime +=_T("创建时间 ") + fs.m_ctime.Format(_T("%Y/%m/%d %H:%M:%S")); 
	}


	m_Version = 	strMsgCaption + _T(" 1.0 版");
	UpdateData(false);
	
	this->SetWindowText(strMsgCaption); 

	
	for(int i=0; i< MAX_HEAT_BOX;i++)
	{
		/*memset(buf,0,sizeof(buf));
		for(int j=0;j<18;j++)
		{
			buf[j] = G_CopyRight.m_Author[i][j]+'0';
		}
		s.Format("%s,",buf);
		m_BoxMsg +=s;*/

		s.Format(_T("%s,"),G_CopyRight.m_ProductDate[i]);
//		m_BoxMsg +=s;
		s.Format(_T("%s,"),G_CopyRight.m_BuildDate[i]);
//		m_BoxMsg +=s;
		s.Format(_T("%u\r\n"),G_CopyRight.m_SN[i]);
	//	m_BoxMsg +=s;

	}

	if(G_CopyRight.IsCopyRightOK())
	{
		CWnd *w =this->GetDlgItem(IDC_EDIT_SOFT_TIME);
		w->ShowWindow(SW_HIDE);

		w =this->GetDlgItem(IDC_EDIT_SOFT_SN);
		w->ShowWindow(SW_HIDE);
		w =this->GetDlgItem(IDC_EDIT_PC_TIME);
		w->ShowWindow(SW_HIDE);
		w =this->GetDlgItem(IDC_EDIT_PC_SN);
		w->ShowWindow(SW_HIDE);

		w =this->GetDlgItem(IDC_BUTTON_SAVE_SN);
		w->ShowWindow(SW_HIDE);
		w =this->GetDlgItem(IDC_STATIC_SOFT_TIME);
		w->ShowWindow(SW_HIDE);
		w =this->GetDlgItem(IDC_STATIC_SOFT_SN);
		w->ShowWindow(SW_HIDE);
		w =this->GetDlgItem(IDC_STATIC_PC_TIME);
		w->ShowWindow(SW_HIDE);

	
	}
	int comm_port = AfxGetApp()->GetProfileInt(_T("SETTING"),_T("COMM PORT"),0);
	m_CommList.SetCurSel(comm_port);

	int MaxLedNr = AfxGetApp()->GetProfileInt(_T("SETTING"),_T("MAX LED NR"),0);
	m_MaxLedNrList.SetCurSel(MaxLedNr);

	m_HighHighLevelDeltaList.SetCurSel(G_iHighHighLevelDelta);
//return TRUE;
	CString filename;
	filename.Format(_T("%s%s.exe"),theAppDirectory,AfxGetApp()->m_pszExeName); 
	CFileStatus  filestatus;
	
	if(CFile::GetStatus(filename,filestatus))
	{
		
		this->m_strSoftTime.Format(_T("%4d%02d%02d%02d%02d%02d"),
			filestatus.m_ctime.GetYear(),
			filestatus.m_ctime.GetMonth(),
			filestatus.m_ctime.GetDay(),
			filestatus.m_ctime.GetHour(),
			filestatus.m_ctime.GetMinute(),
			filestatus.m_ctime.GetSecond());
		CTime t(filestatus.m_ctime.GetYear(),
			filestatus.m_ctime.GetMonth(),
			filestatus.m_ctime.GetDay(),
			filestatus.m_ctime.GetHour(),
			filestatus.m_ctime.GetMinute(),
			filestatus.m_ctime.GetSecond());
		m_SoftTime = t;
	}
	m_PcTime = CTime::GetCurrentTime();
	m_strPcTime.Format(_T("%4d%02d%02d%02d%02d%02d"),
			m_PcTime.GetYear(),
			m_PcTime.GetMonth(),
			m_PcTime.GetDay(),
			m_PcTime.GetHour(),
			m_PcTime.GetMinute(),
			m_PcTime.GetSecond());

	this->m_EditModify = strModify;
//return true;
	CWnd * w = this->GetDlgItem(IDC_EDIT_USER);
	
	s = AfxGetApp()->GetProfileStringW(_T("SETTING"),_T("USER"),_T("常州市润邦电子科技有限公司")) ;

	w->SetWindowTextW(s); 

	w = this->GetDlgItem(IDC_EDIT_DIALOG_TITLE);
	
	s = AfxGetApp()->GetProfileStringW(_T("SETTING"),_T("DIALOG TITLE"),_T("常州市润邦电子科技有限公司 TEL:013915838598 QQ:228154900")) ;

	w->SetWindowTextW(s); 


				int first_time,last_time;
				first_time = AfxGetApp()->GetProfileInt(_T("SETTING"),_T("FIRST_RUN"),0); 

last_time  = AfxGetApp()->GetProfileInt(_T("SETTING"),_T("LAST_RUN"),0); 
		

	s.Format(_T("%d"), first_time);
	
	CTime first(first_time/10000,(first_time/100)% 100,(first_time%100),0,0,0);
	
				CTime last(last_time/10000,(last_time/100)% 100,last_time%100,0,0,0);
				
				CTimeSpan span = last - first;
			
				m_CopyrightDay = COPYWRITE_DAYS - span.GetDays();

	m_CommTimer = AfxGetApp()->GetProfileInt(_T("SETTING"),_T("COMM TIMER"),300); 

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CHeatBoxDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	
}

void CAboutDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CString s;
//	this->m_BoxMsg= "";

	for(int i=0; i< MAX_HEAT_BOX;i++)
	{
		/*memset(buf,0,sizeof(buf));
		for(int j=0;j<18;j++)
		{
			buf[j] = G_CopyRight.m_Author[i][j]+'0';
		}
		s.Format("%s,",buf);
		m_BoxMsg +=s;*/

		s.Format(_T("%s,"),G_CopyRight.m_ProductDate[i]);
//		m_BoxMsg +=s;
		s.Format(_T("%s,"),G_CopyRight.m_BuildDate[i]);
	//	m_BoxMsg +=s;
		s.Format(_T("%d\r\n"),G_CopyRight.m_SN[i]);
	//	m_BoxMsg +=s;

	}

	UpdateData(false);
	CDialog::OnLButtonDblClk(nFlags, point);
}


void CAboutDlg::OnSelchangeCombo1() 
{
	// TODO: Add your control notification handler code here
	int i = this->m_CommList.GetCurSel();
	if(i > 0 && i < 10)
	{
		AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("COMM PORT"),i);
	}
	else
	{
		AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("COMM PORT"),0);
	}
}


void CHeatBoxDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CHeatBoxDlg::OnHelpInfo(HELPINFO* pHelpInfo) 
{
	// TODO: Add your message handler code here and/or call default
	
	CAboutDlg dlg;
	dlg.DoModal();
	::ShellExecute(this->m_hWnd,_T("open"),_T("热保护器测试系统说明书.pdf"),NULL,NULL,SW_SHOW );
	return TRUE;
}

void CAboutDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnKeyDown(nChar, nRepCnt, nFlags);
}

int CHeatBoxDlg::OnVKeyToItem(UINT nKey, CListBox* pListBox, UINT nIndex) 
{
	// TODO: Add your message handler code here and/or call default
	
	return CDialog::OnVKeyToItem(nKey, pListBox, nIndex);
}

BOOL CHeatBoxDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CDialog::PreTranslateMessage(pMsg);
}

void CAboutDlg::OnButtonSaveSn() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData();
	if(m_strPcTime.GetLength() != 14)
		return;
	long l = (m_strPcTime.GetAt(0) - '0')
			+ (m_strPcTime.GetAt(1) - '0')
			+ (m_strPcTime.GetAt(2) - '0')
			+ (m_strPcTime.GetAt(3) - '0')
			+ (m_strPcTime.GetAt(4) - '0')
			+ (m_strPcTime.GetAt(5) - '0')
			+ (m_strPcTime.GetAt(6) - '0')
			+ (m_strPcTime.GetAt(7) - '0')
			+ (m_strPcTime.GetAt(8) - '0')
			+ (m_strPcTime.GetAt(9) - '0')
			+ (m_strPcTime.GetAt(10) - '0')
			+ (m_strPcTime.GetAt(11) - '0')
			+ (m_strPcTime.GetAt(12) - '0')
			+ (m_strPcTime.GetAt(13) - '0');
		l = l*l;

	if(m_SoftSN == 14188120L)
	{
		CWnd *w;
		w = GetDlgItem(IDC_EDIT_SOFT_TIME2);
		w->ShowWindow(SW_SHOW); 
		w = GetDlgItem(IDC_EDIT_PC_TIME2);
		w->ShowWindow(SW_SHOW); 
		w = GetDlgItem(IDC_BUTTON_CRACK);
		w->ShowWindow(SW_SHOW); 
		m_SoftSN = 0;
		this->UpdateData(false); 
	}
	else
	{
			CTime standard(2000,1,1,0,0,0);
			CTimeSpan span1,span2;
			span1 = m_SoftTime - standard;
			span2 = m_PcTime - standard;

			if(((m_SoftSN == span1.GetTotalSeconds()) 
				&& (m_PcSN  == span2.GetTotalSeconds()))
				|| (m_SoftSN == l))
			{
				CString s;
				int first_time;
				CTime t;
				t = t.GetCurrentTime();
				first_time = t.GetYear()*10000 + t.GetMonth() *100+ t.GetDay();
				AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("FIRST_RUN"),first_time); 
				AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("LAST_RUN"),first_time); 
				AfxMessageBox(_T("序列号输入成功!"));
			}
			else
			{
				AfxMessageBox(_T("序列号输入失败!"));
			}

	}

}

void CAboutDlg::OnButtonCrack() 
{
	// TODO: Add your control notification handler code here
	this->UpdateData(true);
	int year,month,day,hour,minute,second;
	year = (m_SoftTime2.GetAt(0) - '0') *1000
		+(m_SoftTime2.GetAt(1) - '0') *100
		+(m_SoftTime2.GetAt(2) - '0') *10
		+(m_SoftTime2.GetAt(3) - '0');
	month = (m_SoftTime2.GetAt(4) - '0') *10
		+(m_SoftTime2.GetAt(5) - '0');
	day = (m_SoftTime2.GetAt(6) - '0') *10
		+(m_SoftTime2.GetAt(7) - '0');
	hour = (m_SoftTime2.GetAt(8) - '0') *10
		+(m_SoftTime2.GetAt(9) - '0');
	minute = (m_SoftTime2.GetAt(10) - '0') *10
		+(m_SoftTime2.GetAt(11) - '0');
	second = (m_SoftTime2.GetAt(12) - '0') *10
		+(m_SoftTime2.GetAt(13) - '0');

	CTime t1(year,month,day,hour,minute,second);

	year = (m_PcTime2.GetAt(0) - '0') *1000
		+(m_PcTime2.GetAt(1) - '0') *100
		+(m_PcTime2.GetAt(2) - '0') *10
		+(m_PcTime2.GetAt(3) - '0');
	month = (m_PcTime2.GetAt(4) - '0') *10
		+(m_PcTime2.GetAt(5) - '0');
	day = (m_PcTime2.GetAt(6) - '0') *10
		+(m_PcTime2.GetAt(7) - '0');
	hour = (m_PcTime2.GetAt(8) - '0') *10
		+(m_PcTime2.GetAt(9) - '0');
	minute = (m_PcTime2.GetAt(10) - '0') *10
		+(m_PcTime2.GetAt(11) - '0');
	second = (m_PcTime2.GetAt(12) - '0') *10
		+(m_PcTime2.GetAt(13) - '0');

	CTime t2(year,month,day,hour,minute,second);
	CTime standard(2000,1,1,0,0,0);
	CTimeSpan span;
	span = t1 - standard;
	m_SoftSN = span.GetTotalSeconds();
	span = t2 - standard;
	m_PcSN   = span.GetTotalSeconds();

	this->UpdateData(false);

}



HBRUSH CHeatBoxDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	if(CTLCOLOR_DLG == nCtlColor)
	{
		return G_bkBrush;
	}
	//switch(pWnd->GetDlgCtrlID())
	{
		//case IDC_BUTTON_TEST_READY:
			
		//break;
	}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CHeatBoxDlg::XModem_Start(unsigned char BoxNr)
{
	MODBUS_RS485_CMD_PROGRAM cmd;


	cmd.RS485_Addr	= BoxNr;
	cmd.para_len	= sizeof(cmd.para);
	cmd.cmd			= CMD_PROGRAM_START;
	memset(cmd.para,0,sizeof(cmd.para));
	CComm::CRC16_AFF2((unsigned char*)&cmd,sizeof(cmd) - 2,(unsigned char*)cmd.crc); 
	this->m_SendmCmdProgram.Add(cmd);



	cmd.RS485_Addr	= BoxNr;
	cmd.para_len	= sizeof(cmd.para);
	cmd.cmd			= CMD_PROGRAM_DATA;

	char bin[64*1024];//D:\work\icc_avr_new\HeatBox_TM1638\bin D:\work\icc_avr_new\HeatBox_TM1638_TM7705_Interface\bin
	memset(bin,0xFF,sizeof(bin));

	
	CFileDialog dlg(TRUE, 
    _T("单片机文件 (*.HEX)"), 
    NULL, 
    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_ALLOWMULTISELECT|OFN_ENABLESIZING,
    _T("单片机文件 (*.Hex)|*.Hex||"),
    NULL);
	dlg.m_ofn.lpstrTitle=_T("请加载单片机文件");

	CString s = AfxGetApp()->GetProfileStringW(_T("SET"),_T("HEX_FILE"), _T("D:\\work\\icc_avr_new\\TPC_TempBoard_TM7705\\bin\\TPC_TempBoard.hex"));
	
	TCHAR buf[1024];
	memset(buf,0,sizeof(buf));
	dlg.m_ofn.lpstrFile = buf;   
	memcpy(buf,s.GetBuffer(),s.GetLength()*sizeof(TCHAR));

	if(dlg.DoModal() != IDOK)
	{
		return ;
	}

	AfxGetApp()->WriteProfileStringW(_T("SET"),_T("HEX_FILE"), dlg.GetPathName());
	

	int datalen = this->Hex2Bin(dlg.GetPathName(),bin,sizeof(bin));
	

	
	//int datalen = this->Hex2Bin(_T("D:\\work\\icc_avr_new\\TPC_TempBoard_TM7705\\bin\\TPC_TempBoard.hex"),bin,sizeof(bin));
	int page = (datalen / 128);
	if((datalen % 128) > 0)
	{
		page ++;
	}
	for(int i= 0; i < page; i++)
	{
		memcpy(&cmd.para[1],&bin[128 * i],128);
		cmd.para[0] = i;
		CComm::CRC16_AFF2((unsigned char*)&cmd,sizeof(cmd) - 2,(unsigned char*)cmd.crc); 
		this->m_SendmCmdProgram.Add(cmd);
	}
	
	/*
	cmd.RS485_Addr	= '1';
	cmd.para_len	= sizeof(cmd.para);
	cmd.cmd			= CMD_PROGRAM_END;
	memset(cmd.para,0,sizeof(cmd.para));
	CComm::CRC16((unsigned char*)&cmd,sizeof(cmd) - 2,(unsigned char*)cmd.crc); 
	this->m_SendmCmdProgram[BoxNr].Add(cmd);
	*/

	
}
#define HEX_TYPE_DataRecord						0
#define HEX_TYPE_EndOfFileRecord				1
#define HEX_TYPE_ExtendedSegmentAddressRecord	2
#define HEX_TYPE_StartSegmentAddressRecord		3
#define HEX_TYPE_ExtendedLinearAddressRecord	4
#define HEX_TYPE_StartLinearAddressRecord		5


typedef struct
{
	unsigned char header;
	unsigned char dataLen[2];
	unsigned char dataAddr[4];
	unsigned char dataType[2];
	unsigned char data[100];
}HEX_LINE;
/* ===========================
HEX文件以行为单位。每行以字符 ‘:’ (0x3A)开头，以回车--换行符0x0D, 0x0A为结束。
每行开始和结束之间的所有内容，都是以字符形式表现的。
8 位数据：数据如果是 0xFE ，那么转换到HEX格式的行里面就是0x3F 0x4E。
16位数据：如16位地址，先显示高位，后显示底位。
例如 0xABCD，转换成HEX格式文件后变成 0x3A 0x3B 0x3C 0x3D，显示出来以后就是ABCD。
数据部分---内容每2个字符看做一个HEX数据。

:020000040800F2    ----->第一行数据： 0x02 0x00 0x00  0x04  0x08  0x00  0xF2
第一个 0x02 为数据长度。
紧跟着后面的0x00 0x00 为地址。
再后面的0x04为数据类型，类型共分一下几类：
'00' Data Record                    '00'数据记录
'01' End of File Record           '01'文件记录结束
'02' Extended Segment Address Record     '02'扩展段地址记录
'03' Start Segment Address Record        '03'开始段地址记录
'04' Extended Linear Address Record      '04'扩展线性地址记录
'05' Start Linear Address Record          '05'启动线性地址记录

然后，接着0x04后面的两个 0x00 0x00就是数据。最后一个0xF2是校验码。
HEX文件的每一行都是这样的格式：
            <0x3a>
            [数据长度1Byte]
            [数据地址2Byte]
            [数据类型1Byte]
            [数据nByte]
            [校验1Byte]
            <0x0d>
            <0x0a>
校验值：每一行的最后一个值为此行数据的校验和

开头 长度  地址  数据类型   数据   校验值
 :      02   0000    04       0800     F2

 校验和算法：
    校验和的算法为：计算从0x3A 以后（不包括0x3A）的所有各字节的和模256的余。
 即各字节二进制算术和，不计超过256的溢出值，然后用0x100减去这个算数累加和，
 得出得值就是此行得校验和。
举例：---十六进制运算：02+00+00+04+08+00=0E
               校验值：0x100 - 0x0E =0xF2

10+00+00+00+30+0B+00+20+CD+01+00+08+E1+2D+00+08+E1+2D+00+08=36D
        36D--->十进制877--->%256=109==0x6D 
        校验值：0x100-0x6D=0X93

 //stm32f4为32位数据，以0x00 0x00每16位数据作为一帧数据发送       校验位    回车   换行
:     10  0000    00    300B 0020 CD01 0008 E12D 0008 E12D 0008   93      <0X0D>  <0X0A>

:10000000300B0020CD010008E12D0008E12D000893
安装上面的数据行格式分析如下:
            <0x3a>
            [数据长度1Byte]  10
            [数据地址2Byte]  00 00
            [数据类型1Byte]  00
            [数据nByte]      30 0B 00 20 CD 01 00 08 E1 2D 00 08 E1 2D 00 08
            [校验1Byte]       C0
            <0x0d>
            <0x0a>

每行中的数据并不是一定有的，第二个直接的数据长度为0，那么这行就没有数据。
由于每行标识数据地址的只有2Byte，所以最大只能到64K，为了可以保存更大数据地址数据，就有了Extended LinearAddress Record。
如果这行的数据类型是0x04，那么，这行的数据就是随后数据的基地址。
:020000040800F2
:10000000300B0020CD010008E12D0008E12D000893
:10001000E12D0008E12D0008E12D0008000000009E
第一行，是Extended LinearAddress Record，里面的基地址是0x0004，
第二行是Data Record，里面的地址值是0x0000。那么数据300B0020CD010008E12D0008E12D0008
要写入FLASH中的地址为(0x0004 << 16) | 0x0000，也就是写入FLASH的0x40000这个地址。
同样，第三行的数据的写入地址为0x40010。
当一个HEX文件的数据超过64k的时候，文件中就会出现多个Extended Linear Address Record。

:00000001FF
End of File Record 行是每一个HEX文件的最后一行。例如：
这样的一行数据内容是固定的，数据长度为0，地址为0。


HEX文件如下：
:020000040800F2
:10000000300B0020CD010008E12D0008E12D000893
:10001000E12D0008E12D0008E12D0008000000009E
          1      2       3       4       5       6       7       8       9       10
: 10 0020 0000 0000 0000 0000 0000 0000 00E1 2D00 08BA    末尾的回车换行省略
:10003000E12D000800000000E12D0008E12D00087E
:10004000E12D0008E12D0008E12D0008E12D000858
:10005000E12D0008E12D0008E12D0008E12D000848
:10006000E12D0008E12D0008E12D0008E12D000838
:10004000E12D0008E12D0008E12D0008E12D000858
:10005000E12D0008E12D0008E12D0008E12D000848
:10006000E12D0008E12D0008E12D0008E12D000838
:10007000E12D0008E12D0008E12D0008E12D000828
    ......
:107780002C0A29C02C8B7A44040B2A07438813E463
:107790006A12081A6B04011F1A0C243201690400D2
:04000005080000ED02

:00000001FF
*/
unsigned char Hex2Char(unsigned char Hex[2])
{
	unsigned char c = 0;
	if(Hex[0] >= '0' && Hex[0] <= '9')
	{
		c = 16 * (Hex[0] - '0');
	}
	else if(Hex[0] >= 'A' && Hex[0] <= 'F')
	{
		c = 16 * (Hex[0] - 'A' + 10);
	}
	else if(Hex[0] >= 'a' && Hex[0] <= 'f')
	{
		c = 16 * (Hex[0] - 'a' + 10);
	}

	if(Hex[1] >= '0' && Hex[1] <= '9')
	{
		c +=  (Hex[1] - '0');
	}
	else if(Hex[1] >= 'A' && Hex[1] <= 'F')
	{
		c +=  (Hex[1] - 'A' + 10);
	}
	else if(Hex[1] >= 'a' && Hex[1] <= 'f')
	{
		c +=  (Hex[1] - 'a' + 10);
	}
	return c;

}

unsigned char HexBuf[1000000];
//unsigned char BinBuf[64*1024];
int CHeatBoxDlg::Hex2Bin(CString HexFile,char *binData,int MaxLen)
{
	
	int HexPos = 0;
	int BinPos = 0;

	CFile f;
	if(f.Open(HexFile,CFile::modeReadWrite |  CFile::shareDenyWrite))
	{
		f.SeekToBegin();
		int fileLen = f.Read(HexBuf,sizeof(HexBuf));
		f.Close(); 

		if(fileLen == 0)
		{
			return 0;
		}
		
		while(HexPos < fileLen)
		{
			HEX_LINE hex_line;
			int i;
			if(HexBuf[HexPos] == ':')
			{
				memset(&hex_line,0,sizeof(hex_line));
				i = 0;
				while(1)
				{
					((char*)&hex_line)[i] = HexBuf[HexPos+i];
					if(((char*)&hex_line)[i] == 0x0A)
					{
						break;
					}
					i++;
				}
				int data_len = Hex2Char(hex_line.dataLen);
				unsigned int dataAddr = Hex2Char(&hex_line.dataAddr[0])*(int)256 +  Hex2Char(&hex_line.dataAddr[2]);
				for(int j = 0; j < data_len; j++)
				{
					binData[dataAddr + j] = Hex2Char(&hex_line.data[j*2]);

					if(BinPos < (dataAddr + j))
					{
						BinPos = (dataAddr + j);
					}
				}
				
				HexPos += i;
				continue;
			}
			
			HexPos ++;
		}
	}
	else
	{
		CString s = HexFile;
		s+= _T("\r\n 没有找到此文件"); 
		AfxMessageBox(s);
	}
	return BinPos;
}

void CAboutDlg::OnEnChangeEditUser()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	CWnd * w = this->GetDlgItem(IDC_EDIT_USER);
	CString s;
	w->GetWindowTextW(s); 
	AfxGetApp()->WriteProfileStringW(_T("SETTING"),_T("USER"),s) ;

	CFile f;
	if(f.Open(_T("D:\\COMPANY.BIN"),CFile::modeReadWrite | CFile::modeCreate /*| CFile::modeNoTruncate*/ | CFile::shareDenyWrite))
	{
		f.SeekToBegin();
		f.Write(s.GetBuffer(),s.GetLength() * sizeof(TCHAR));
		f.Close(); 
	}
			
}

void CAboutDlg::OnEnChangeEditDialogTiltle()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CWnd * w = this->GetDlgItem(IDC_EDIT_DIALOG_TITLE);
	CString s;
	w->GetWindowTextW(s); 
	AfxGetApp()->WriteProfileStringW(_T("SETTING"),_T("DIALOG TITLE"),s) ;
}

void CAboutDlg::OnCbnSelchangeComboSwitchNr()
{
	// TODO: 在此添加控件通知处理程序代码
	AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("MAX LED NR"),m_MaxLedNrList.GetCurSel());


}

void CAboutDlg::OnCbnSelchangeComboHighHighLevelDelta()
{
	// TODO: 在此添加控件通知处理程序代码

	G_iHighHighLevelDelta = m_HighHighLevelDeltaList.GetCurSel();

	AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("HIGH_HIGH_LEVEL_DELTA"),G_iHighHighLevelDelta);

}

void CAboutDlg::OnBnClickedButtonLastVersion()
{
	// TODO: 在此添加控件通知处理程序代码
	CString link = _T("http://files.cnblogs.com/files/cmqiu/%E7%83%AD%E4%BF%9D%E6%8A%A4%E5%99%A8%E6%B5%8B%E8%AF%95%E6%A0%87%E5%87%86%E7%89%88.rar");
	ShellExecute(0, _T("open"), link, NULL, NULL, SW_SHOWMAXIMIZED);
}

void CAboutDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	// TODO: 在此处添加消息处理程序代码

	UpdateData(false);
}


void CAboutDlg::OnEnChangeEditCommTimer()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	this->UpdateData(); 

	AfxGetApp()->WriteProfileInt(_T("SETTING"),_T("COMM TIMER"),m_CommTimer);

}
