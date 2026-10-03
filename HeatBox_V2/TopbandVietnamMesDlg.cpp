// TopbandVietnamMesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox.h"
#include "TopbandVietnamMesDlg.h"
#include "KeyBoardDlg.h"
#include "HttpTools.h"

// CTopbandVietnamMesDlg 对话框

IMPLEMENT_DYNAMIC(CTopbandVietnamMesDlg, CDialog)

CTopbandVietnamMesDlg::CTopbandVietnamMesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTopbandVietnamMesDlg::IDD, pParent)
	, m_URL_1(_T(""))
	, m_URL_2(_T(""))
	, m_Site(_T(""))
	, m_Resrce(_T(""))
	, m_Barcode(_T(""))
	, m_Password(_T(""))
	, m_Operation(_T(""))
	, m_Userid(_T(""))
	, m_ShopOrder(_T(""))
	, m_Sfcs(_T(""))
{

}

CTopbandVietnamMesDlg::~CTopbandVietnamMesDlg()
{
}

void CTopbandVietnamMesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_URL_1, m_URL_1);
	DDX_Text(pDX, IDC_EDIT_URL_2, m_URL_2);
	DDX_Text(pDX, IDC_EDIT_site, m_Site);
	DDX_Text(pDX, IDC_EDIT_resrce, m_Resrce);
	DDX_Text(pDX, IDC_EDIT_barcode, m_Barcode);
	DDX_Text(pDX, IDC_EDIT_password, m_Password);
	DDX_Text(pDX, IDC_EDIT_operation, m_Operation);
	DDX_Text(pDX, IDC_EDIT_userid, m_Userid);
	DDX_Text(pDX, IDC_EDIT_shop_order, m_ShopOrder);
	DDX_Text(pDX, IDC_EDIT_sfcs, m_Sfcs);
}


BEGIN_MESSAGE_MAP(CTopbandVietnamMesDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CTopbandVietnamMesDlg::OnBnClickedOk)
	ON_EN_SETFOCUS(IDC_EDIT_shop_order, &CTopbandVietnamMesDlg::OnEnSetfocusEditshoporder)
	ON_EN_SETFOCUS(IDC_EDIT_URL_1, &CTopbandVietnamMesDlg::OnEnSetfocusEditUrl1)
	ON_EN_SETFOCUS(IDC_EDIT_URL_2, &CTopbandVietnamMesDlg::OnEnSetfocusEditUrl2)
	ON_EN_SETFOCUS(IDC_EDIT_site, &CTopbandVietnamMesDlg::OnEnSetfocusEditsite)
	ON_EN_SETFOCUS(IDC_EDIT_operation, &CTopbandVietnamMesDlg::OnEnSetfocusEditoperation)
	ON_EN_SETFOCUS(IDC_EDIT_resrce, &CTopbandVietnamMesDlg::OnEnSetfocusEditresrce)
	ON_EN_SETFOCUS(IDC_EDIT_sfcs, &CTopbandVietnamMesDlg::OnEnSetfocusEditsfcs)
	ON_EN_SETFOCUS(IDC_EDIT_barcode, &CTopbandVietnamMesDlg::OnEnSetfocusEditbarcode)
	ON_EN_SETFOCUS(IDC_EDIT_userid, &CTopbandVietnamMesDlg::OnEnSetfocusEdituserid)
	ON_EN_SETFOCUS(IDC_EDIT_password, &CTopbandVietnamMesDlg::OnEnSetfocusEditpassword)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &CTopbandVietnamMesDlg::OnBnClickedButtonUpload)
	ON_BN_CLICKED(IDCANCEL, &CTopbandVietnamMesDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CTopbandVietnamMesDlg 消息处理程序
void CTopbandVietnamMesDlg::MES_Check(void)
{
	const CString strAgent;

	CString   response = L"";
	CString strMethod = L"POST";

	CString strUrl =  CString(::G_Topband_Vietnam_Config.URL_1);

	if(strUrl.GetLength() > 0)
	{
		CString  postData,s;

		postData = _T("{\r\n");

	
		s.Format(_T("    \"site\": \"%s\",\r\n"), CString(::G_Topband_Vietnam_Config.site));
		postData += s;
		s.Format(_T("    \"userid\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.userid));
		postData += s;

		s.Format(_T("    \"password\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.password));
		postData += s;

		s.Format(_T("    \"resrce\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.resrce));
		postData += s;


		s.Format(_T("    \"operation\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.operation));
		postData += s;

			
		if(CString(::G_Topband_Vietnam_Config.shop_order).GetLength() > 0)
		{
			s.Format(_T("    \"shop_order\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.shop_order));
			postData += s;
		}

		s.Format(_T("    \"sfcs\": %s\r\n"),_T("["));
		postData += s;

		postData += _T("    \"") + CString(::G_Topband_Vietnam_Config.sfcs);

		postData += _T("\"\r\n");
		postData += _T("    ]\r\n");

		postData += _T("}\r\n");


		AfxMessageBox(postData);
		/*
		"parametricCustomList": [
        {
            "field_description":"450012491_B0180_10_RE_ICTV02",
            "field_name":"450012491_B0180_10_RE_ICTV02",
            "field_value": "PASS"
        }
    ],
      "parametricMeasureInfoList": [
        {
            "measure_name":"1",
            "measure_status": "PASS",
            "unit_of_meas": "C",
            "description":"温度1",
            "high_limit": "100",
            "low_limit":"60",
            "actual":"80"
        },
        {
            "measure_name":"2",
            "measure_status": "PASS",
            "unit_of_meas": "C",
            "description":"温度1",
            "high_limit": "100",
            "low_limit":"60",
            "actual":"80.7"
        },
        {
            "measure_name":"3",
            "measure_status": "PASS",
            "unit_of_meas": "C",
            "description":"温度1",
            "high_limit": "100",
            "low_limit":"60",
            "actual":"80.9"
        }
    ]
	*/

		char buf[10000];
		memset(buf,0,sizeof(buf));
		for(int i = 0; i < postData.GetLength(); i++)
		{
			buf[i] = postData.GetAt(i);
		}
		HttpTools::HttpRequest(strMethod,strUrl,buf,response,IE_AGENT,true);

		response.Replace(_T(","),_T(",\r\n"));
		response.Replace(_T("{"),_T("{\r\n"));
		response.Replace(_T("}"),_T("\r\n}"));
		AfxMessageBox(response);
	}
	
}
void CTopbandVietnamMesDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	MES_Check();
		
}


BOOL CTopbandVietnamMesDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	this->m_URL_1 = CString(::G_Topband_Vietnam_Config.URL_1);
	this->m_URL_2 = CString(::G_Topband_Vietnam_Config.URL_2);

	m_Site		=	CString(G_Topband_Vietnam_Config.site);
	m_Resrce	=	CString(G_Topband_Vietnam_Config.resrce);
	m_Barcode	=	CString(G_Topband_Vietnam_Config.barcode);
	m_Password	=	CString(G_Topband_Vietnam_Config.password);
	m_Operation	=	CString(G_Topband_Vietnam_Config.operation);
	m_Userid	=	CString(G_Topband_Vietnam_Config.userid);
	m_ShopOrder	=	CString(G_Topband_Vietnam_Config.shop_order);
	m_Sfcs		=	CString(G_Topband_Vietnam_Config.sfcs);
	
	this->UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CTopbandVietnamMesDlg::OnEnSetfocusEditshoporder()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDOK);
	w->SetFocus();
	CKeyBoardDlg dlg;
	dlg.m_Input = this->m_ShopOrder;
	if(dlg.DoModal() == IDOK)
	{
		m_ShopOrder = dlg.m_Input;
		this->UpdateData(false);
		SaveConfig();
	}

}

void CTopbandVietnamMesDlg::SaveConfig(void)
{
	this->UpdateData(true);

	memset(&G_Topband_Vietnam_Config,0,sizeof(G_Topband_Vietnam_Config));

	memcpy(&G_Topband_Vietnam_Config.URL_1,m_URL_1.GetBuffer(),m_URL_1.GetLength() * sizeof(TCHAR));

	memcpy(&G_Topband_Vietnam_Config.URL_2,m_URL_2.GetBuffer(),m_URL_2.GetLength() * sizeof(TCHAR));

	memcpy(&G_Topband_Vietnam_Config.site,m_Site.GetBuffer(),m_Site.GetLength() * sizeof(TCHAR));
	memcpy(&G_Topband_Vietnam_Config.resrce,m_Resrce.GetBuffer(),m_Resrce.GetLength() * sizeof(TCHAR));
	memcpy(&G_Topband_Vietnam_Config.barcode,m_Barcode.GetBuffer(),m_Barcode.GetLength() * sizeof(TCHAR));
	memcpy(&G_Topband_Vietnam_Config.password,m_Password.GetBuffer(),m_Password.GetLength() * sizeof(TCHAR));
	memcpy(&G_Topband_Vietnam_Config.operation,m_Operation.GetBuffer(),m_Operation.GetLength() * sizeof(TCHAR));
	memcpy(&G_Topband_Vietnam_Config.userid,m_Userid.GetBuffer(),m_Userid.GetLength() * sizeof(TCHAR));
	memcpy(&G_Topband_Vietnam_Config.shop_order,m_ShopOrder.GetBuffer(),m_ShopOrder.GetLength() * sizeof(TCHAR));
	memcpy(&G_Topband_Vietnam_Config.sfcs,m_Sfcs.GetBuffer(),m_Sfcs.GetLength() * sizeof(TCHAR));


	::SaveNormalConfigPara();
}
void CTopbandVietnamMesDlg::OnEnSetfocusEditUrl1()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDOK);
	w->SetFocus();
	CKeyBoardDlg dlg;
	dlg.m_Input = this->m_URL_1;
	if(dlg.DoModal() == IDOK)
	{
		m_URL_1 = dlg.m_Input;
		this->UpdateData(false);
		SaveConfig();
	}
}

void CTopbandVietnamMesDlg::OnEnSetfocusEditUrl2()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDOK);
	w->SetFocus();
	CKeyBoardDlg dlg;
	dlg.m_Input = this->m_URL_2;
	if(dlg.DoModal() == IDOK)
	{
		m_URL_2 = dlg.m_Input;
		this->UpdateData(false);
		SaveConfig();
	}
}

void CTopbandVietnamMesDlg::OnEnSetfocusEditsite()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDOK);
	w->SetFocus();
	CKeyBoardDlg dlg;
	dlg.m_Input = this->m_Site;
	if(dlg.DoModal() == IDOK)
	{
		m_Site = dlg.m_Input;
		this->UpdateData(false);
		SaveConfig();
	}
}

void CTopbandVietnamMesDlg::OnEnSetfocusEditoperation()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDOK);
	w->SetFocus();
	CKeyBoardDlg dlg;
	dlg.m_Input = this->m_Operation;
	if(dlg.DoModal() == IDOK)
	{
		m_Operation = dlg.m_Input;
		this->UpdateData(false);
		SaveConfig();
	}
}

void CTopbandVietnamMesDlg::OnEnSetfocusEditresrce()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDOK);
	w->SetFocus();
	CKeyBoardDlg dlg;
	dlg.m_Input = this->m_Resrce;
	if(dlg.DoModal() == IDOK)
	{
		m_Resrce = dlg.m_Input;
		this->UpdateData(false);
		SaveConfig();
	}
}

void CTopbandVietnamMesDlg::OnEnSetfocusEditsfcs()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDOK);
	w->SetFocus();
	CKeyBoardDlg dlg;
	dlg.m_Input = this->m_Sfcs;
	if(dlg.DoModal() == IDOK)
	{
		m_Sfcs = dlg.m_Input;
		this->UpdateData(false);
		SaveConfig();
	}
}

void CTopbandVietnamMesDlg::OnEnSetfocusEditbarcode()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDOK);
	w->SetFocus();
	CKeyBoardDlg dlg;
	dlg.m_Input = this->m_Barcode;
	if(dlg.DoModal() == IDOK)
	{
		m_Barcode = dlg.m_Input;
		this->UpdateData(false);
		SaveConfig();
	}
}

void CTopbandVietnamMesDlg::OnEnSetfocusEdituserid()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDOK);
	w->SetFocus();
	CKeyBoardDlg dlg;
	dlg.m_Input = this->m_Userid;
	if(dlg.DoModal() == IDOK)
	{
		m_Userid = dlg.m_Input;
		this->UpdateData(false);
		SaveConfig();
	}
}

void CTopbandVietnamMesDlg::OnEnSetfocusEditpassword()
{
	// TODO: 在此添加控件通知处理程序代码
	CWnd *w = this->GetDlgItem(IDOK);
	w->SetFocus();
	CKeyBoardDlg dlg;
	dlg.m_Input = this->m_Password;
	if(dlg.DoModal() == IDOK)
	{
		m_Password = dlg.m_Input;
		this->UpdateData(false);
		SaveConfig();
	}
}

void CTopbandVietnamMesDlg::OnBnClickedButtonUpload()
{
	// TODO: 在此添加控件通知处理程序代码
	RECORD	record;
	MES_Collection_TEST(record);

}

void CTopbandVietnamMesDlg::MES_Collection_TEST(RECORD	record)
{
	const CString strAgent;

	

	memset(&record,0,sizeof(record));

	for(int i = 0; i < 10; i++)
	{
		record.TestResult[0].IsUsed = true;
	}

	CString   response = L"";
	CString strMethod = L"POST";

	CString strUrl = CString(::G_Topband_Vietnam_Config.URL_2);

	if(strUrl.GetLength() > 0)
	{
		CString  postData,s;

		postData = _T("[{\r\n");

	
		s.Format(_T("	\"site\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.site));
		postData += s;
		s.Format(_T("	\"userid\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.userid));
		postData += s;

		

		s.Format(_T("	\"operation\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.operation));
		postData += s;

		s.Format(_T("	\"resrce\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.resrce));
		postData += s;


		s.Format(_T("	\"password\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.password));
		postData += s;
		if(CString(::G_Topband_Vietnam_Config.shop_order).GetLength() > 0)
		{
			s.Format(_T("	\"shop_order\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.shop_order));
			postData += s;
		}

		if(CString(::G_Topband_Vietnam_Config.barcode).GetLength() > 0)
		{
			s.Format(_T("	\"barcode\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.barcode));
			postData += s;
		}

		s.Format(_T("    \"sfc\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.sfcs));
		postData += s;

		/*
				"parametricCustomList": [
        {
            "field_description":"450012491_B0180_10_RE_ICTV02",
            "field_name":"450012491_B0180_10_RE_ICTV02",
            "field_value": "PASS"
        }
    ],
		*/
		s.Format(_T("	\"parametricCustomList\": [\r\n"));
		postData += s;
		s.Format(_T("	{\r\n"));
		postData += s;
		s.Format(_T("	\"field_description\":\"450012491_B0180_10_RE_ICTV02\", \r\n"));
		postData += s;
		s.Format(_T("	\"field_name\":\"450012491_B0180_10_RE_ICTV02\",\r\n"));
		postData += s;
		s.Format(_T("	\"field_value\": \"PASS\"\r\n"));
		postData += s;
		s.Format(_T("	}\r\n"));
		postData += s;
		s.Format(_T("	],\r\n"));
		postData += s;


				postData += _T("{\r\n");
				

				for(int i = 0; i < 128; i++)
				{
		/*			
      "parametricMeasureInfoList": [
        {
            "measure_name":"1",
            "measure_status": "PASS",
            "unit_of_meas": "C",
            "description":"温度1",
            "high_limit": "100",
            "low_limit":"60",
            "actual":"80"
        },
        {
            "measure_name":"2",
            "measure_status": "PASS",
            "unit_of_meas": "C",
            "description":"温度1",
            "high_limit": "100",
            "low_limit":"60",
            "actual":"80.7"
        },
        {
            "measure_name":"3",
            "measure_status": "PASS",
            "unit_of_meas": "C",
            "description":"温度1",
            "high_limit": "100",
            "low_limit":"60",
            "actual":"80.9"
        }
    ]
	*/
				}
			
				postData += _T("}\r\n");
				
		

		postData += _T("]\r\n");

		postData += _T("}]\r\n");



		AfxMessageBox(postData);

		char buf[10000];
		memset(buf,0,sizeof(buf));
		for(int i = 0; i < postData.GetLength(); i++)
		{
			buf[i] = postData.GetAt(i);
		}
		int answer = HttpTools::HttpRequest(strMethod,strUrl,buf,response,IE_AGENT,true);

	
			response.Replace(_T(","),_T(",\r\n"));
			response.Replace(_T("[{"),_T("[{\r\n"));
			response.Replace(_T("}]"),_T("\r\n}]\r\n"));
			AfxMessageBox(response);
	
	}

}

char temp_buf[1000000];
void CTopbandVietnamMesDlg::MES_Collection(int BoxNr,RECORD *record,int start_Nr,int end_Nr,CString barcode)
{


	const CString strAgent;

	CString   response = L"";
	CString strMethod = L"POST";

	CString strUrl = CString(::G_Topband_Vietnam_Config.URL_2);

	int used_counts = 0;

	for(int i = start_Nr; i < end_Nr; i++)
	{
		if(record->TestResult[i].IsUsed)
		{
			used_counts ++;
		}
	}

	if(used_counts == 0)
	{
		return;
	}

	if(strUrl.GetLength() > 0)
	{
		CString  postData,s;

		postData = _T("[{\r\n");

	
		s.Format(_T("	\"site\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.site));
		postData += s;
		s.Format(_T("	\"userid\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.userid));
		postData += s;

		

		s.Format(_T("	\"operation\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.operation));
		postData += s;

		s.Format(_T("	\"resrce\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.resrce));
		postData += s;

		s.Format(_T("   \"program_id\": \"YBV0001\",\r\n"));
		postData += s;
		
		s.Format(_T("   \"program_rev\":\"Changzhou Runbang Test Program Version V0001\",\r\n"));
		postData += s;
	
		s.Format(_T("   \"tester_hw_rev\":\"Changzhou Runbang PC-128 \",\r\n"));
		postData += s;

		
		s.Format(_T("   \"passStation\":\"N\",\r\n"));
		postData += s;

		s.Format(_T("	\"password\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.password));
		postData += s;
		if(CString(::G_Topband_Vietnam_Config.shop_order).GetLength() > 0)
		{
			s.Format(_T("	\"shop_order\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.shop_order));
			postData += s;
		}

		if(barcode.GetLength() > 0)
		{
			s.Format(_T("	\"barcode\": \"%s\",\r\n"),barcode);
			postData += s;
		}

		s.Format(_T("    \"sfc\": \"%s\",\r\n"),CString(::G_Topband_Vietnam_Config.sfcs));
		postData += s;

		
		/*
				"parametricCustomList": [
        {
            "field_description":"450012491_B0180_10_RE_ICTV02",
            "field_name":"450012491_B0180_10_RE_ICTV02",
            "field_value": "PASS"
        }
    ],
		*/
		s.Format(_T("	\"parametricCustomList\": [\r\n"));
		postData += s;
		s.Format(_T("	{\r\n"));
		postData += s;
		s.Format(_T("	\"field_description\":\"450012491_B0180_10_RE_ICTV02\", \r\n"));
		postData += s;
		s.Format(_T("	\"field_name\":\"450012491_B0180_10_RE_ICTV02\",\r\n"));
		postData += s;
		s.Format(_T("	\"field_value\": \"PASS\"\r\n"));
		postData += s;
		s.Format(_T("	}\r\n"));
		postData += s;
		s.Format(_T("	],\r\n"));
		postData += s;
	
		//AfxMessageBox(postData);

		//return;

		/*			
      "parametricMeasureInfoList": [
        {
            "measure_name":"1",
            "measure_status": "PASS",
            "unit_of_meas": "C",
            "description":"温度1",
            "high_limit": "100",
            "low_limit":"60",
            "actual":"80"
        },
        {
            "measure_name":"2",
            "measure_status": "PASS",
            "unit_of_meas": "C",
            "description":"温度1",
            "high_limit": "100",
            "low_limit":"60",
            "actual":"80.7"
        },
        {
            "measure_name":"3",
            "measure_status": "PASS",
            "unit_of_meas": "C",
            "description":"温度1",
            "high_limit": "100",
            "low_limit":"60",
            "actual":"80.9"
        }
    ]
	*/

	
		s.Format(_T("	\"parametricMeasureInfoList\": [\r\n"));
		postData += s;
		
		for(int i = start_Nr; i < end_Nr; i++)
		{
			if(record->TestResult[i].IsUsed)
			{
				used_counts --;

				postData += _T("{\r\n");
				s.Format(_T("\"measure_name\":\"%d-1\",\r\n"), i+1);
				postData += s;
				
				CONTROL_TEMP_RANGE temp_range;

				HEAT_COOL_ORDER HeatOrCool = ::CheckHeatOrCoolMode(BoxNr,record->ConfigPara,&temp_range,record->BoxType); 

				TEST_RESULT_LEVEL level = CheckTestLevel(record->ConfigPara,record->TestResult[i] ,HeatOrCool,temp_range,record->TestResult[i],record->BoxType);

				if(level == MAIN_LEVEL
					|| level == TEST_RESULT_HIGH_LEVEL	
					|| level == LOW_LEVEL)
				{
					s.Format(_T("\"measure_status\": \"PASS\",\r\n"));
				}
				else
				{
					s.Format(_T("\"measure_status\": \"FAIL\",\r\n"));
				}
				postData += s;
				
				s.Format(_T("\"unit_of_meas\": \"C\",\r\n"));
				postData += s;
				
				s.Format(_T("\"description\":\"Act.Temp%d-1\",\r\n"),i+1);
				postData += s;

				
				s.Format(_T("\"high_limit\": \"%.1f\",\r\n"),temp_range.open_temp_max);
				postData += s;

				s.Format(_T("\"low_limit\": \"%.1f\",\r\n"),temp_range.open_temp_min);
				postData += s;

				if(record->TestResult[i].IsOpenned)
				{
					s.Format(_T("    \"actual\":\"%5.1f\"\r\n"),record->TestResult[i].OpenTemp);
					postData += s;
				}
				else
				{
					postData += _T("    \"actual\":\"999.9\"\r\n");
				}
				postData += _T("},\r\n");

				//复位温度
				postData += _T("{\r\n");
				s.Format(_T("\"measure_name\":\"%d-2\",\r\n"), i+1);
				postData += s;
				
				
				if(level == MAIN_LEVEL
					|| level == TEST_RESULT_HIGH_LEVEL	
					|| level == LOW_LEVEL)
				{
					s.Format(_T("\"measure_status\": \"PASS\",\r\n"));
				}
				else
				{
					s.Format(_T("\"measure_status\": \"FAIL\",\r\n"));
				}
				postData += s;
				
				s.Format(_T("\"unit_of_meas\": \"C\",\r\n"));
				postData += s;
				
				s.Format(_T("\"description\":\"Ret.Temp%d-2\",\r\n"),i+1);
				postData += s;

				
				s.Format(_T("\"high_limit\": \"%.1f\",\r\n"),temp_range.close_temp_max);
				postData += s;

				s.Format(_T("\"low_limit\": \"%.1f\",\r\n"),temp_range.close_temp_min);
				postData += s;

				if(record->TestResult[i].IsOpenned)
				{
					s.Format(_T("    \"actual\":\"%5.1f\"\r\n"),record->TestResult[i].CloseTemp);
					postData += s;
				}
				else
				{
					postData += _T("    \"actual\":\"999.9\"\r\n");
				}
				
				if(used_counts == 0)
				{
					postData += _T("}\r\n");
				}
				else
				{
					postData += _T("},\r\n");
				}
				
			}
		}

		postData += _T("]\r\n");

		postData += _T("}]\r\n");

		TCHAR *buff = postData.GetBuffer();
		WriteLogFile(buff,BoxNr);
		postData.ReleaseBuffer();

	//	AfxMessageBox(postData);

		
		memset(temp_buf,0,sizeof(temp_buf));
		for(int i = 0; i < postData.GetLength(); i++)
		{
			temp_buf[i] = postData.GetAt(i);
		}

	
		int answer = HttpTools::HttpRequest(strMethod,strUrl,temp_buf,response,IE_AGENT,true);

		response += _T("\r\n");
		buff = response.GetBuffer();
		WriteLogFile(buff,BoxNr);
		postData.ReleaseBuffer();

		if(response.FindOneOf(_T("success")) > 0 )
		{
			AfxMessageBox(response);
		}
		else
		{
			response.Replace(_T(","),_T(",\r\n"));
			response.Replace(_T("[{"),_T("[{\r\n"));
			response.Replace(_T("}]"),_T("\r\n}]\r\n"));
			AfxMessageBox(response);
		}

	}

}


void CTopbandVietnamMesDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}
