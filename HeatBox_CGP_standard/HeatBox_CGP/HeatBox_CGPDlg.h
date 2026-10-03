// HeatBox_CGPDlg.h : 头文件
//

#pragma once

#include "comm.h"
#include "typedefs.h"
#include "DebugDlg.h"
#include "afxwin.h"

#define COOL_HEAT_READY_TEMP (m_TestPara.CloseTemp[m_CurTestPro] +  m_TestPara.CloseTempPlus[m_CurTestPro] + 5.0f) 
#define TEST_MODE_READY_GAP_TEMP	2.0f	
//#define HEAT_COOL_READY_TEMP (m_TestPara.OpenTemp[m_CurTestPro] -  m_TestPara.OpenTempMinus[m_CurTestPro] - 5.0f) 

enum  TEST_MODE
{
	TEST_MODE_IDLE		= 0,
	TEST_MODE_FULL_UP	= 1,
	TEST_MODE_READY		= 2,
	TEST_MODE_LINE_UP	= 3,
	TEST_MODE_FULL_DOWN = 4,
	TEST_MODE_LINE_DOWN	= 5
};
typedef struct
{
	int		CommPort;
	int		CommSpeedTimer;
	int		Language;
	float	AdjustTemp;
	int		SwitchCount;
	TCHAR	ParaName[256];


}NORMAL_CONFIG_PARA;


typedef struct
{
	TCHAR	ParaName_dummy[20];
	TCHAR	Productor[20];
	TCHAR	ProductorNo[20];

	char	TestPoint[4];
	char	Cycle[4];
	char    HeatCoolType[4];
	float	OpenTemp[4];
	float   OpenTempMinus[4];
	float   OpenTempPlus[4];
	float   CloseTemp[4];
	float   CloseTempMinus[4];
	float   CloseTempPlus[4];
	float   OpenCloseGapLow[4];
	float   OpenCloseGapHigh[4];

	char	TestSpeed;
	char    KeepWarmMode;
	char	IsDoubleSwitch;
	char	FlashGap;

	char	IsNormalOpen;

	short		HeatSpeed;
	short		CoolSpeed;
	unsigned char		HeatPercent;
	unsigned char		CoolPercent;

}TEST_PARA;

typedef struct 
{
	char NAME[100];
	int FILE_VER;
	int FILE_SUB_VER;
	int TEST_PARA_SIZE;
	char dummy[100];
}TEST_PARA_FILE_HEADER;

typedef struct 
{
	char NAME[100];
	int FILE_VER;
	int FILE_SUB_VER;
	int TEST_RECORD_SIZE;
	char dummy[100];
}TEST_RECORD_FILE_HEADER;

typedef struct 
{
	int					size;
	_int64				BeginTime;
	_int64				EndTime;
	TCHAR				ParaName[256];
	TEST_PARA			TestPara;
	SWITCH_TEST_RESULT	SwitchStatus[4][MAX_LED_NR];
	TCHAR				Barcode[MAX_LED_NR][100];		//2023/5/15增加二维条码

}TEST_RECORD;

typedef struct
{
	__time64_t	time;
	float		temp;
}TEMP_RECORD;
typedef CArray<TEMP_RECORD ,TEMP_RECORD> CTempRecordArray;

//typedef CArray<float ,float> CDoubleArray;
// CHeatBox_CGPDlg 对话框
class CHeatBox_CGPDlg : public CDialog
{
// 构造
public:
	CHeatBox_CGPDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HEATBOX_CGP_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

    CComm  m_Comm;
	CComm  m_CommScanner;
	float       m_CurTemp;
//	TEST_PARA	m_TestPara;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CCommCmdProgram	  m_SendmCmdProgram;
	TEST_MODE	m_TestMode;
	CString		m_TestMsg;
	CTime		m_BeginTime,m_EndTime;
	int			m_CommTimeOut;
	CModbus_RS485_Cmd_Array	m_SendCmdArray;

	CString      m_TestParaName;
	SWITCH_TEST_RESULT m_SwitchStatus[4][MAX_LED_NR];
	

	bool		m_Takeboard[4];
	TEST_PARA	m_TestPara;
	bool		m_bFlashShow;
	bool		m_ClickedButtonTestStart;
	CString		m_Productor;
	UINT		m_SwitchNr;
	CString		m_ProductNo;
	int			m_TestSpeed;
	UINT		m_FlashGap;
	CString		m_EnterTestMode;
	CString		m_KeepWarmMode;
	CDebugDlg	*m_DebugDlg;
	int			m_HeatOutPercent;	//read from pcb
	int			m_CoolOutPercent;	//read from pcb
	float		m_CurSv;
	int			m_CurScannerID;
	CString		m_NewBarcode;
	
	float		m_TopTemp;
	bool		m_bShowTempCurve;
	//CDoubleArray m_TempArray;
	int			m_CurTestPro;
	int			m_LastTestPro;
	int			m_CurTestCycle;
	MODBUS_RS485_READ_TEMP_ANSWER m_ReadTempAnswer;
	HBRUSH		m_background_hbr;
	WORK_STATUS m_WorkStatus;
	bool		TEST_MODE_READY_cmd_sended;
	UINT		m_UsedMem;
	afx_msg void OnBnClickedButtonPrint();
	afx_msg void OnBnClickedButtonDebug();

	afx_msg void OnBnClickedButtonTestPara();

	void CHeatBox_CGPDlg::DrawAllItem(int TotalCount);
	LRESULT OnComm(WPARAM wParam,LPARAM lParam);
	LRESULT CHeatBox_CGPDlg::OnCommScanner(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedButtonSysConfig();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	MODBUS_RS485_CMD CHeatBox_CGPDlg::SetCmd(int equip, COMMAND_ID cmdId, char *para);
	void CHeatBox_CGPDlg::ReadConfig(void);
	void CHeatBox_CGPDlg::SaveConfig(void);
	afx_msg void OnBnClickedOk();
	
	afx_msg void OnBnClickedButtonTestReady();
	afx_msg void OnBnClickedButtonTestStop();
	afx_msg void OnBnClickedButtonTestStart();
	afx_msg void OnBnClickedButtonPreTemp();


	void DrawTempControlInfo(void);
	
	void DrawTempCurve(void);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	void DealSwitchTest(void);
	void DealSwitchTest_HeatCool(void);
	void DealSwitchTest_CoolHeat(void);
	int SendCmdStepPid(float TopTemp);
	int SendCmdHeatFullOut(float TopTemp);
	int SendCmdCoolFullOut(float BottomTemp);
	int SendCmdCoolStepPid(float BottomTemp);
	int SendCmdHeatNormalPid(float TopTemp);
	int SendCmdCoolPidOut(float BottomTemp);
	int SendCmdStop(void);

	void SelectStartOrNextButton(void);
	void SaveTestResult(void);
	void SelectLanguage(void);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedButtonDataManage();
	afx_msg void OnBnClickedButtonAdjust();
	afx_msg void OnBnClickedButtonTestNext();
	void SendCmdReadAdjustLow(void);
	void SendCmdReadAdjustHigh(void);
	void SendCmdReadPidT(void);


	afx_msg void OnBnClickedButtonLanguage();
	afx_msg void OnBnClickedButtonMyLanguage();
	void DrawVoltAmp(int ID);
	float m_Amp[3];
	float m_Volt[3];
	void SaveOpenStatus(void);
	void SaveCloseStatus(void);
	void DrawUN(CDC * pDC,CRect rect,bool U_Flag);
	void CHeatBox_CGPDlg::CheckIsUsed(void);
	void CHeatBox_CGPDlg::ClearStatus(void);
	void CHeatBox_CGPDlg::EnterTestStart();
	void CHeatBox_CGPDlg::SaveExcel(void);
	bool CHeatBox_CGPDlg::IsAllOpend(void);
	void CHeatBox_CGPDlg::XModem_Start(int BoxNr);
	int CHeatBox_CGPDlg::Hex2Bin(CString HexFile,char *binData,int MaxLen);
	int CHeatBox_CGPDlg::Hex2Bin(unsigned char *Hex,int HexBufLen ,char *binData,int MaxLen);
	float CHeatBox_CGPDlg:: HEAT_COOL_READY_TEMP(void);
	int CHeatBox_CGPDlg::SendCmdReadTemp(void);
	int CHeatBox_CGPDlg::SendCmdAirValvePercent(char  percent);
	void	CHeatBox_CGPDlg::SaveTestRecord_Hex(void);
	CString CHeatBox_CGPDlg::CreateCurDateDirection(void);
	CString CHeatBox_CGPDlg::GetDateDirection(CTime t);
	int CHeatBox_CGPDlg::SendCmdReadAdjustAll(void);
	int CHeatBox_CGPDlg::SendCmdSetAdjustAll(float temp);
	UINT CHeatBox_CGPDlg::GetUsedMemory(void);
	void CHeatBox_CGPDlg::PrepareDraw(void);
	void ShowBarcode(void);

	CListBox m_ListBarcode;
	afx_msg void OnBnClickedButtonDelBarcode();
	afx_msg void OnLbnDblclkListBarcode();
	afx_msg void OnLbnSelchangeListBarcode();
};





extern CHeatBox_CGPDlg *G_MainDlg;
extern NORMAL_CONFIG_PARA G_NormalConfigPara;
extern CTempRecordArray	G_TempRecordArray;
extern CString theAppDirectory;