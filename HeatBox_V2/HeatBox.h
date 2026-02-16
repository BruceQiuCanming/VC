// HeatBox.h : main header file for the HEATBOX application
//
#include "stdAfx.h"

#if !defined(AFX_HEATBOX_H__C1EEF5A1_3BA6_458A_B100_F420A0E71A0A__INCLUDED_)
#define AFX_HEATBOX_H__C1EEF5A1_3BA6_458A_B100_F420A0E71A0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif


#include "resource.h"		// main symbols
#include "..\\..\\public_c\\typedefs.h"

//#define _HEATBOX_RESET_

#define LANG_CHINA		0x0803
//#define LANG_CHINESE		0x0804

extern LPCTSTR  G_pOldAppName;

#include "..\\..\\public_c\\comm.h"
#include "..\\..\\public_c\\common.h"

//d:\work\VC\HeatBox_V2\HeatBox.h
/////////////////////////////////////////////////////////////////////////////
// CHeatBoxApp:
// See HeatBox.cpp for the implementation of this class
//
//#define DIRECT_HEAT_DELTA	(7.0f)
//#define PID_HEAT_DELTA		(1.0f)
//#define STEP_COOL_DELTA		(5.0f)

//#define HIGH_HIGH_LEVEL_DELTA		(3.0f)

#define OVER_HEAT_DELTA		(0.9f)


//#define _HANJIE_192

extern int		G_iHighHighLevelDelta;
extern float	G_SwitchTempAdjust[6][192][2];


extern CString	G_sCompany;
extern int		G_CurMaxHeatBox;

#define BK_COLOR	RGB(0, 160, 128)
extern	CBrush		G_bkBrush;
//extern	CTime		STD_TIME;







extern unsigned short UNICODE_FILE_HEAR;
extern unsigned short CSV_COMMMA;
extern CString strMsgCaption[];
extern int		G_iLangID;
extern void GetLangID(void);







typedef struct
{
	short Temp[4];
	short Sv;
	DWORD TickCount;
}STRUCT_TEMP;



//typedef CArray<TEMP_RECORD ,TEMP_RECORD> CTempRecordArray;



class CHeatBoxApp : public CWinApp
{
public:
	int m_TabActiveIndex[2*MAX_HEAT_BOX];
	CHeatBoxApp();

	BOOL PreTranslateMessage(MSG *pMsg);
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHeatBoxApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CHeatBoxApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HEATBOX_H__C1EEF5A1_3BA6_458A_B100_F420A0E71A0A__INCLUDED_)


extern COMM_PROTOCOL_TYPE G_CommProtocolType;

