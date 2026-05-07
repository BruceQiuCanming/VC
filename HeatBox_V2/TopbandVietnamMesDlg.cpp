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
	MES_Collection_TEST();

}

void CTopbandVietnamMesDlg::MES_Collection_TEST(void)
{
	const CString strAgent;

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

		s.Format(_T("	\"parametricCustomList\": [\r\n"));
		postData += s;
		
		


				postData += _T("{\r\n");
				s.Format(_T("\"Nr\": \"%d\",\r\n"), 1);
				postData += s;
				
				postData += _T("    \"ACT. TEMP\":\"100.0\",\r\n");
			
			
				postData += _T("    \"RET. TEMP\":\"60.0\",\r\n");
			
				postData += _T("    \"Test Result\":\"OK\"\r\n");
			
			
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

void CTopbandVietnamMesDlg::MES_Collection(int BoxNr,RECORD *record,int start_Nr,int end_Nr)
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

		s.Format(_T("	\"parametricCustomList\": [\r\n"));
		postData += s;
		
		
		

		for(int i = start_Nr; i < end_Nr; i++)
		{
			if(record->TestResult[i].IsUsed)
			{
				used_counts --;

				postData += _T("{\r\n");
				s.Format(_T("\"Nr\": \"%d\",\r\n"), i+1);
				postData += s;
				if(record->TestResult[i].IsOpenned)
				{
					s.Format(_T("    \"ACT. TEMP\":\"%5.1f\",\r\n"),record->TestResult[i].OpenTemp);
					postData += s;
				}
				else
				{
					postData += _T("    \"ACT. TEMP\":\"-.-\",\r\n");
				}
				
				if(record->TestResult[i].IsClosed)
				{
					s.Format(_T("    \"RET. TEMP\":\"%5.1f\",\r\n"),record->TestResult[i].CloseTemp);
					postData += s;
				}
				else
				{
					postData += _T("    \"RET. TEMP\":\"-.-\",\r\n");
				}

				CONTROL_TEMP_RANGE temp_range;
				HEAT_COOL_ORDER HeatOrCool = CheckHeatOrCoolMode(BoxNr,record->ConfigPara ,&temp_range,record->BoxType);

				TEST_RESULT_LEVEL result = CheckTestLevel(record->ConfigPara,record->TestResult[i],HeatOrCool,temp_range,record->TestResult[i],record->BoxType);
				if(result == MAIN_LEVEL
					|| result == TEST_RESULT_HIGH_LEVEL	
					|| result == LOW_LEVEL)
				{
					postData += _T("    \"Test Result\":\"OK\"\r\n");
				}
				else
				{
					postData += _T("    \"Test Result\":\"NG\"\r\n");
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



//		AfxMessageBox(postData);

		char buf[10000];
		memset(buf,0,sizeof(buf));
		for(int i = 0; i < postData.GetLength(); i++)
		{
			buf[i] = postData.GetAt(i);
		}
		int answer = HttpTools::HttpRequest(strMethod,strUrl,buf,response,IE_AGENT,true);

		if(response.FindOneOf(_T("success")) > 0 )
		{
	//		AfxMessageBox(_T("success"));
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


/*[{
    "site": "1001",
    "userid": "SITE_ADMIN",
    "password": "MESXXXXX",
    "operation": "DICT1",
    "resrce": "D01ICT1",
    "sfc": "EO023621902567",
    "shop_order": "50560247",
    "parametricMeasureInfoList": [
        {
           "low_limit": "0.950nF",
            "actual": "0.9871830nF",
            "parametricMeasureCustomList": [
                {
                    "property_name": "上限",
                    "property_value": "20.0",
                    "unit_of_meas": ""
                },
                {
                    "property_name": "下限",
                    "property_value": "20.0",
                    "unit_of_meas": ""
                },
                {
                    "property_name": "形态",
                    "property_value": "C",
                    "unit_of_meas": ""
                },
                {
                    "property_name": "偏差",
                    "property_value": "-1.3  %",
                    "unit_of_meas": ""
                }
            ]
        }
        
        
    ]
}]
*/