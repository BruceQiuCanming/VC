// HotLong_PCBDlg.h : 头文件
//

#pragma once

#include "resource.h"
#include "CaptureVideo.h"
#include "comm.h"
#include "waitcmd.h"
#include "CMyEdit.h"
#include <atlimage.h>
#include "afxwin.h"
#include "Device_HuiLong_PCB.h"
#include "HuiKong_DIO.h"
#include "ParaDlg.h"

#define CAMERA_WIDTH	(1920 / 3)
#define CAMERA_HEIGHT	(1028 / 3)


#define R_BITS	0x000000FF
#define G_BITS	0x0000FF00
#define B_BITS	0x00FF0000
#define R_BITS_MIN	0x000000F0
#define G_BITS_MIN	0x0000F000
#define B_BITS_MIN	0x00F00000
/*
#define RELAY_PCB_MINUS_ID			15
#define RELAY_PCB_PLUS_ID			14
#define RELAY_PCB_SET_ID			13
#define RELAY_PCB_TEST_NEEDLE_ID	12

#define RELAY_PCB_BEEP_ID			10

#define RELAY_PCB_PROGRAM_BEGIN_ID	9


#define RELAY_PCB_POWER_1_ID		7
#define RELAY_PCB_POWER_2_ID		6
#define RELAY_PCB_POWER_3_ID		5
#define RELAY_PCB_POWER_4_ID		4
#define RELAY_PCB_POWER_5_ID		3
#define RELAY_PCB_POWER_6_ID		2
#define RELAY_PCB_POWER_7_ID		1
#define RELAY_PCB_POWER_8_ID		0
*/

#define Comm_Device_HuiKong_DIO_MSG_ID	(COMM_MSG + 0)
#define Comm_PROGRAM_MSG_ID (COMM_MSG + 1)
#define Comm_AGING_MSG_ID_1	(Comm_PROGRAM_MSG_ID + 1)
#define Comm_AGING_MSG_ID_2	(Comm_PROGRAM_MSG_ID + 2)
#define Comm_AGING_MSG_ID_3	(Comm_PROGRAM_MSG_ID + 3)
#define Comm_AGING_MSG_ID_4	(Comm_PROGRAM_MSG_ID + 4)
#define Comm_AGING_MSG_ID_5	(Comm_PROGRAM_MSG_ID + 5)
#define Comm_AGING_MSG_ID_6	(Comm_PROGRAM_MSG_ID + 6)
#define Comm_AGING_MSG_ID_7	(Comm_PROGRAM_MSG_ID + 7)
#define Comm_AGING_MSG_ID_8	(Comm_PROGRAM_MSG_ID + 8)


typedef enum
{
	PROGRAM_KEY_MODE_IDLE = 0,
	PROGRAM_KEY_MODE_NEEDLE_DOWN,
	PROGRAM_KEY_MODE_PROGRAM,
	PROGRAM_KEY_MODE_SET_DOWN,
	PROGRAM_KEY_MODE_MINUS_DOWN,
	PROGRAM_KEY_MODE_MINUS_UP,
	PROGRAM_KEY_MODE_MINUS_DOWN_2,
	PROGRAM_KEY_MODE_MINUS_UP_2,
	PROGRAM_KEY_MODE_PLUS_DOWN,
	PROGRAM_KEY_MODE_PLUS_UP,


}PROGRAM_KEY_MODE;

typedef enum
{
	PCB_TYPE_BIG_AMP = 0,
	PCB_TYPE_BIG_AMP_NO_BARCODE,
	PCB_TYPE_X90  ,
	PCB_TYPE_X90_NO_BARCODE,
	PCB_TYPE_X10  ,
	PCB_TYPE_X10_NO_BARCODE,


}PCB_TYPE;

typedef struct
{
	int     m_LED_NUM_Left_1;
	int     m_LED_NUM_Top_1;
	int     m_LED_NUM_Right_1;
	int     m_LED_NUM_Bottom_1;

	int     m_LED_HEAT_Left;
	int     m_LED_HEAT_Top;
	int     m_LED_HEAT_Right;
	int     m_LED_HEAT_Bottom;

	int     m_LED_COOL_Left;
	int     m_LED_COOL_Top;
	int     m_LED_COOL_Right;
	int     m_LED_COOL_Bottom;

	int     m_LED_COLOR_Left;
	int     m_LED_COLOR_Top;
	int     m_LED_COLOR_Right;
	int     m_LED_COLOR_Bottom;

	int     m_LED_NUM_Left[4];
	int     m_LED_NUM_Top[4];
	int     m_LED_NUM_Right[4];
	int     m_LED_NUM_Bottom[4];

	int		m_Program_Pass_Counts;
	int     m_Program_Fail_Counts;
	int		m_Aging_Pass_Counts;
	int     m_Aging_Fail_Counts;

	int     m_Comm_Nr[9];
	int     m_Comm_Device_HuiKong_DIO;
	int     m_Camera_Nr;

	int     m_Device_HuiKong_DIO_Y_Aging_Power_1;
	int     m_Device_HuiKong_DIO_Y_Aging_Power_2;
	int     m_Device_HuiKong_DIO_Y_Aging_Power_3;
	int     m_Device_HuiKong_DIO_Y_Aging_Power_4;
	int     m_Device_HuiKong_DIO_Y_Aging_Power_5;
	int     m_Device_HuiKong_DIO_Y_Aging_Power_6;
	int     m_Device_HuiKong_DIO_Y_Aging_Power_7;
	int     m_Device_HuiKong_DIO_Y_Aging_Power_8;

	int     m_Device_HuiKong_DIO_Y_MINUS_ID;
	int     m_Device_HuiKong_DIO_Y_PLUS_ID;
	int     m_Device_HuiKong_DIO_Y_SET_ID;
	int     m_Device_HuiKong_DIO_Y_TEST_NEEDLE_ID;
	int     m_Device_HuiKong_DIO_Y_BEEP_ID;
	int     m_Device_HuiKong_DIO_Y_PROGRAM_BEGIN_ID;



	PCB_TYPE	m_Pcb_Type;

}CONFIG_DATA;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:


	
};

// CHotLong_PCBDlg 对话框
class CHotLong_PCBDlg : public CDialog
{
// 构造
public:
	CHotLong_PCBDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_HOTLONG_PCB_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CParaDlg			m_ParaDlg;
	CStatic				m_staticScreen;
	CListBox			m_ListCtrl;
	CCaptureVideo		m_cap;

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();

	int							m_LED_NUM[4];

	CDevice_HuiKong_DIO			m_Device_HuiKong_DIO;
	CDevice_HuiLong_PCB		    m_Device_HotLong_PCB[9];

	DWORD   m_Beep_BeginTime;
	
	int		m_Beep_mSecs;
	bool    m_Beep_IsPass;

	int     m_Pv[9];
	int     m_Sv[9];
	int     m_Pp[9];

	int     m_BarCodeID;
	

	void  SavePassFail();
	void CHotLong_PCBDlg::ReMoveWindows_BIG_AMP(void);
	void CHotLong_PCBDlg::ReMoveWindows_X90(void);

	bool	m_Break;
	CFont	m_font;
	CMyEdit	m_Edit_Degree;
	CMyEdit	m_Edit_Led_Heat;
	CMyEdit	m_Edit_Led_Cool;
	CMyEdit	m_Edit_Led_Color;
	CMyEdit m_Edit_Pv[8];
	CMyEdit m_Edit_Sv[8];

	CListBox m_ListMsg[9];

	CRect m_Rect_a1[4];
	CRect m_Rect_a2[4];
	CRect m_Rect_a3[4];
	CRect m_Rect_a4[4];
	CRect m_Rect_a5[4];
	CRect m_Rect_a6[4];
	CRect m_Rect_a7[4];
	CRect m_Rect_dot[4];

	CRect m_Rect_space1[4];
	CRect m_Rect_space2[4];


	int		m_Program_Pass_Counts;
	int     m_Program_Fail_Counts;
	int     m_Program_Total_Counts;
	float   m_Program_Percent;

	int		m_Aging_Pass_Counts;
	int     m_Aging_Fail_Counts;
	int     m_Aging_Total_Counts;
	float   m_Aging_Percent;

	void StartVideo(void);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonLedRect();
	afx_msg void OnBnClickedButtonPCB_Set();
	afx_msg void OnBnClickedButtonPCB_Needle();
	afx_msg void OnBnClickedButtonPCB_Plus();
	afx_msg void OnBnClickedButtonPCB_Minus();
	afx_msg void OnBnClickedButtonPCB_Write();
	afx_msg void OnBnClickedButtonPCB_Power_1();
	afx_msg void OnBnClickedButtonPCB_Power_2();
	afx_msg void OnBnClickedButtonPCB_Power_3();
	afx_msg void OnBnClickedButtonPCB_Power_4();
	afx_msg void OnBnClickedButtonPCB_Power_5();
	afx_msg void OnBnClickedButtonPCB_Power_6();
	afx_msg void OnBnClickedButtonPCB_Power_7();
	afx_msg void OnBnClickedButtonPCB_Power_8();

	void ClickedButtonStart(int ButtonID);
	afx_msg void OnBnClickedButtonProgramStart();
	afx_msg void OnBnClickedButtonStart_1();
	afx_msg void OnBnClickedButtonStart_2();
	afx_msg void OnBnClickedButtonStart_3();
	afx_msg void OnBnClickedButtonStart_4();
	afx_msg void OnBnClickedButtonStart_5();
	afx_msg void OnBnClickedButtonStart_6();
	afx_msg void OnBnClickedButtonStart_7();
	afx_msg void OnBnClickedButtonStart_8();
	void WriteLog(int ID, CString Barcode, CTime BeginTime, CString msg);
	void WriteLog(int ID, CString Barcode, CTime BeginTime, int SubMode, int Pv, int Sv);
	void SetResult_Program(bool pass);

	void SetSv(int PCB_ID, int Sv);

	void ShowLedNumPicture(CImage *Image,int ID);
	void Led_Heat(void);
	void ShowLedHeatPicture(CImage *Image);
	void Led_Cool(void);
	void ShowLedCoolPicture(CImage* Image);
	void Led_Num(void);

	void Led_Color(void);
	void ShowLedColorPicture(CImage* Image);
	unsigned int   CheckColor(CImage *image, unsigned int ColorBits, unsigned int ColorMinVal);
	CString  ShowColorText(CMyEdit* edit, float r, float g, float b, float pixels);
	void Take_Screenshot_To_File(CImage* source_image, int Left, int Top, int Right, int Bottom, CString fileName);
	void SaveOnlyRedColor(CImage* image, unsigned int ColorBits, unsigned int ColorMinVal);
	CString  GetLedChar(int ID,CImage* image);
	bool center(CImage* image, CRect rect);
	CRect Get_a1_Rect(CImage* image);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void ImageDrawRect(CImage* image, CRect rect);


	void WorkMode_X90(int ID);
	void WorkMode_BIG_AMP(int ID);
	void WorkMode_Program(void);
	void TimerSend(int SendID);
	void TimerSend_PLC(void);

	CString m_Led_Cool;
	CString m_Led_Color;
	CString m_Led_Heat;
	
	afx_msg void OnEnChangeEditLedNumLeft_1();
	afx_msg void OnEnChangeEditLedNumRight_1();
	afx_msg void OnEnChangeEditLedNumTop_1();
	afx_msg void OnEnChangeEditLedNumBottom_1();

	afx_msg void OnEnChangeEditLedNumLeft_2();
	afx_msg void OnEnChangeEditLedNumRight_2();
	afx_msg void OnEnChangeEditLedNumTop_2();
	afx_msg void OnEnChangeEditLedNumBottom_2();

	afx_msg void OnEnChangeEditLedNumLeft_3();
	afx_msg void OnEnChangeEditLedNumRight_3();
	afx_msg void OnEnChangeEditLedNumTop_3();
	afx_msg void OnEnChangeEditLedNumBottom_3();

	afx_msg void OnEnChangeEditLedNumLeft_4();
	afx_msg void OnEnChangeEditLedNumRight_4();
	afx_msg void OnEnChangeEditLedNumTop_4();
	afx_msg void OnEnChangeEditLedNumBottom_4();

	afx_msg void OnEnChangeEditLedHeatLeft();
	afx_msg void OnEnChangeEditLedHeatRight();
	afx_msg void OnEnChangeEditLedHeatTop();
	afx_msg void OnEnChangeEditLedHeatBottom();

	afx_msg void OnEnChangeEditLedColorLeft();
	afx_msg void OnEnChangeEditLedColorRight();
	afx_msg void OnEnChangeEditLedColorTop();
	afx_msg void OnEnChangeEditLedColorBottom();
	afx_msg void OnEnChangeEditSv_1();

	afx_msg void OnEnChangeEditLedCoolLeft();
	afx_msg void OnEnChangeEditLedCoolRight();
	afx_msg void OnEnChangeEditLedCoolTop();
	afx_msg void OnEnChangeEditLedCoolBottom();

	afx_msg void OnEnSetFocusEditBarcode();
	afx_msg void OnEnSetFocusEditBarcode_1();
	afx_msg void OnEnSetFocusEditBarcode_2();
	afx_msg void OnEnSetFocusEditBarcode_3();
	afx_msg void OnEnSetFocusEditBarcode_4();
	afx_msg void OnEnSetFocusEditBarcode_5();
	afx_msg void OnEnSetFocusEditBarcode_6();
	afx_msg void OnEnSetFocusEditBarcode_7();
	afx_msg void OnEnSetFocusEditBarcode_8();

	afx_msg void OnEnSetFocusEditAging_PASS();
	afx_msg void OnEnSetFocusEditAging_FAIL();
	afx_msg void OnEnSetFocusEditProgram_PASS();
	afx_msg void OnEnSetFocusEditProgram_FAIL();

	void SetResult(int ID, bool Pass);


	LRESULT OnComm_PROGRAM(WPARAM wParam, LPARAM lParam);
	LRESULT OnComm_Device_HuiKong_DIO(WPARAM wParam, LPARAM lParam);
	LRESULT OnComm_PCB_1(WPARAM wParam, LPARAM lParam);
	LRESULT OnComm_PCB_2(WPARAM wParam, LPARAM lParam);
	LRESULT OnComm_PCB_3(WPARAM wParam, LPARAM lParam);
	LRESULT OnComm_PCB_4(WPARAM wParam, LPARAM lParam);
	LRESULT OnComm_PCB_5(WPARAM wParam, LPARAM lParam);
	LRESULT OnComm_PCB_6(WPARAM wParam, LPARAM lParam);
	LRESULT OnComm_PCB_7(WPARAM wParam, LPARAM lParam);
	LRESULT OnComm_PCB_8(WPARAM wParam, LPARAM lParam);

	void SetRelay(int ID, bool OnOff);
	void BeepMode(void);
	
	CString m_sLED_NUM;
	CButton m_CheckPCB_Button_Set;
	CButton m_CheckPCB_Button_Plus;
	CButton m_CheckPCB_Button_Minus;
	CButton m_CheckPCB_Button_Test_Needle;
	CButton m_CheckPCB_Button_Test_Start;
	
	
	CButton m_CheckPCB_Button_Beep;
	
	CButton m_CheckFocus;

	int ReadExcelFile();
	int GetWorkDir();
	void Excel_Test();
	CStatic m_LedNumGroup;
	CStatic m_STATIC_DEGREE;

	CStatic m_STATIC_LEFT;
	CStatic m_STATIC_RIGHT;
	CStatic m_STATIC_TOP;
	CStatic m_STATIC_BOTTOM;

	CEdit m_LED_NUM_Left[4];
	CEdit m_LED_NUM_Right[4];
	CEdit m_LED_NUM_Top[4];
	CEdit m_LED_NUM_Bottom[4];
	CStatic m_PIC_LED_NUM_1;
	CStatic m_PIC_LED_NUM_2;
	CStatic m_PIC_LED_NUM_3;
	CStatic m_PIC_LED_NUM_4;


	bool cross(CImage image, int x1, int y1, int x2, int y2);
	int getnum(CImage image);
	int getnum_7(CImage image);
	int getnum_1(CImage image);
	int getgrey(CImage image, int x, int y);

	CStatic m_STATIC_HEAT_LED_GROUP;
	CStatic m_STATIC_COOL_LED_GROUP;
	CStatic m_STATIC_COLOR_LED_GROUP;
	afx_msg void OnStnClickedStaticLeft5();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnSetfocusEditLedNumLeft1();
	afx_msg void OnEnSetfocusEditLedNumRight1();
	afx_msg void OnEnSetfocusEditLedNumTop1();
	afx_msg void OnEnSetfocusEditLedNumBottom1();
	afx_msg void OnEnSetfocusEditLedNumLeft2();
	afx_msg void OnEnSetfocusEditLedNumRight2();
	afx_msg void OnEnSetfocusEditLedNumTop2();
	afx_msg void OnEnSetfocusEditLedNumBottom2();
	afx_msg void OnEnSetfocusEditLedNumLeft3();
	afx_msg void OnEnSetfocusEditLedNumRight3();
	afx_msg void OnEnSetfocusEditLedNumTop3();
	afx_msg void OnEnSetfocusEditLedNumBottom3();
	afx_msg void OnEnSetfocusEditLedNumLeft4();
	afx_msg void OnEnSetfocusEditLedNumRight4();
	afx_msg void OnEnSetfocusEditLedNumTop4();
	afx_msg void OnEnSetfocusEditLedNumBottom4();
	afx_msg void OnEnSetfocusEditLedHeatLeft();
	afx_msg void OnEnSetfocusEditLedHeatRight();
	afx_msg void OnEnSetfocusEditLedHeatTop();
	afx_msg void OnEnSetfocusEditLedHeatBottom();

	afx_msg void OnEnSetfocusEditLedCoolLeft();
	afx_msg void OnEnSetfocusEditLedCoolRight();
	afx_msg void OnEnSetfocusEditLedCoolTop();
	afx_msg void OnEnSetfocusEditLedCoolBottom();

	afx_msg void OnEnSetfocusEditLedColorLeft();
	afx_msg void OnEnSetfocusEditLedColorRight();
	afx_msg void OnEnSetfocusEditLedColorTop();
	afx_msg void OnEnSetfocusEditLedColorBottom();
};


extern 	void ReadConfig(void);
	
extern 	void SaveConfig(void);