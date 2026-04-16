// DongguanBetterMesMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HeatBox.h"
#include "DongguanBetterMesMsgDlg.h"
#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32.lib")

// CDongguanBetterMesMsgDlg 对话框

IMPLEMENT_DYNAMIC(CDongguanBetterMesMsgDlg, CDialog)

CDongguanBetterMesMsgDlg::CDongguanBetterMesMsgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDongguanBetterMesMsgDlg::IDD, pParent)

	, m_MES_TCP_Port(0)
	, m_EquipmentCode(_T(""))
	, m_WorkStation_Code(_T(""))
	, m_WorkStation_Name(_T(""))
	, m_WorkSheet_Code(_T(""))
	, m_Product_Code(_T(""))
	, m_Product_Name(_T(""))
{

}

CDongguanBetterMesMsgDlg::~CDongguanBetterMesMsgDlg()
{
}

void CDongguanBetterMesMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);


	DDX_Text(pDX, IDC_EDIT_TCP_PORT, m_MES_TCP_Port);
	DDX_Text(pDX, IDC_EDIT_EQUIPMENT_CODE, m_EquipmentCode);
	DDX_Text(pDX, IDC_EDIT_WORKSTATION_CODE, m_WorkStation_Code);
	DDX_Text(pDX, IDC_EDIT_WORKSTATION_NAME, m_WorkStation_Name);
	DDX_Text(pDX, IDC_EDIT_WORKSHEET_CODE, m_WorkSheet_Code);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_CODE, m_Product_Code);
	DDX_Text(pDX, IDC_EDIT_PRODUCT_NAME, m_Product_Name);
}


BEGIN_MESSAGE_MAP(CDongguanBetterMesMsgDlg, CDialog)
	ON_BN_CLICKED(IDOK, &CDongguanBetterMesMsgDlg::OnBnClickedOk)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IPADDRESS_MES, &CDongguanBetterMesMsgDlg::OnIpnFieldchangedIpaddressMes)

END_MESSAGE_MAP()


// CDongguanBetterMesMsgDlg 消息处理程序

void CDongguanBetterMesMsgDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	
	this->UpdateData();
	G_NormalConfigPara[0].MES_TCP_Port = this->m_MES_TCP_Port;

	TCHAR *buff = this->m_EquipmentCode.GetBuffer();
	memset(&G_NormalConfigPara[0].MES_Equipment_Code,0,sizeof(G_NormalConfigPara[0].MES_Equipment_Code));
	memcpy(&G_NormalConfigPara[0].MES_Equipment_Code,buff,m_EquipmentCode.GetLength() * sizeof(TCHAR));
	m_EquipmentCode.ReleaseBuffer();

	buff = this->m_WorkStation_Code.GetBuffer();
	memset(&G_NormalConfigPara[0].MES_WorkStation_Code,0,sizeof(G_NormalConfigPara[0].MES_WorkStation_Code));
	memcpy(&G_NormalConfigPara[0].MES_WorkStation_Code,buff,m_WorkStation_Code.GetLength() * sizeof(TCHAR));
	m_WorkStation_Code.ReleaseBuffer();

	buff = this->m_WorkStation_Name.GetBuffer();
	memset(&G_NormalConfigPara[0].MES_WorkStation_Name,0,sizeof(G_NormalConfigPara[0].MES_WorkStation_Name));
	memcpy(&G_NormalConfigPara[0].MES_WorkStation_Name,buff,m_WorkStation_Name.GetLength() * sizeof(TCHAR));
	m_WorkStation_Name.ReleaseBuffer();


	buff = this->m_WorkSheet_Code.GetBuffer();
	memset(&G_NormalConfigPara[0].MES_WorkSheet_Code,0,sizeof(G_NormalConfigPara[0].MES_WorkStation_Code));
	memcpy(&G_NormalConfigPara[0].MES_WorkSheet_Code,buff,m_WorkSheet_Code.GetLength() * sizeof(TCHAR));
	m_WorkSheet_Code.ReleaseBuffer();

	buff = this->m_Product_Code.GetBuffer();
	memset(&G_NormalConfigPara[0].MES_Product_Code,0,sizeof(G_NormalConfigPara[0].MES_Product_Code));
	memcpy(&G_NormalConfigPara[0].MES_Product_Code,buff,m_Product_Code.GetLength() * sizeof(TCHAR));
	m_Product_Code.ReleaseBuffer();

	buff = this->m_Product_Name.GetBuffer();
	memset(&G_NormalConfigPara[0].MES_Product_Name,0,sizeof(G_NormalConfigPara[0].MES_Product_Name));
	memcpy(&G_NormalConfigPara[0].MES_Product_Name,buff,m_Product_Name.GetLength() * sizeof(TCHAR));
	m_Product_Name.ReleaseBuffer();

	::SaveNormalConfigPara();

	this->OnOK();

}




CString CDongguanBetterMesMsgDlg::GetLocalIP()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return L"";

    char szHost[256] = {0};
    if (gethostname(szHost, 256) != 0)
    {
        WSACleanup();
        return L"";
    }

	CString s;
	s = CString(szHost);

    hostent* pHost = gethostbyname(szHost);
    if (!pHost)
    {
        WSACleanup();
        return L"";
    }

    in_addr addr;
    addr.S_un.S_addr = *(u_long*)pHost->h_addr_list[0];

    CString strIP;
	char *buf;
	buf = inet_ntoa(addr); // Unicode 下自动转
	strIP = CString(buf);



	m_LocalIP[0] = addr.S_un.S_un_b.s_b1;
	m_LocalIP[1] = addr.S_un.S_un_b.s_b2;
	m_LocalIP[2] = addr.S_un.S_un_b.s_b3;
	m_LocalIP[3] = addr.S_un.S_un_b.s_b4;
	
	
    WSACleanup();
    return strIP;
}

BOOL CDongguanBetterMesMsgDlg::OnInitDialog()
{

	
	CIPAddressCtrl * m_IPAddressCtrl_MES = (CIPAddressCtrl *) GetDlgItem(IDC_IPADDRESS_MES);
	
	m_IPAddressCtrl_MES->SetAddress(G_NormalConfigPara[0].MES_IP[0],
				G_NormalConfigPara[0].MES_IP[1],
				G_NormalConfigPara[0].MES_IP[2],
				G_NormalConfigPara[0].MES_IP[3]);

	CEdit * edit = (CEdit *) GetDlgItem(IDC_EDIT_TCP_PORT);
	CString s;
	s.Format(_T("%d"),G_NormalConfigPara[0].MES_TCP_Port);
	edit->SetWindowTextW(s);

	GetLocalIP();


	CIPAddressCtrl * m_IPAddressCtrl_Local = (CIPAddressCtrl *) GetDlgItem(IDC_IPADDRESS_LOCAL);
	
	m_IPAddressCtrl_Local->SetAddress(m_LocalIP[0],m_LocalIP[1],m_LocalIP[2],m_LocalIP[3]);


	
	this->m_MES_TCP_Port = ::G_NormalConfigPara[0].MES_TCP_Port;

	

	m_EquipmentCode  = CString(G_NormalConfigPara[0].MES_Equipment_Code);
	m_WorkStation_Code = CString(G_NormalConfigPara[0].MES_WorkStation_Code);
	m_WorkStation_Name = CString(G_NormalConfigPara[0].MES_WorkStation_Name);

	m_WorkSheet_Code = CString(G_NormalConfigPara[0].MES_WorkSheet_Code);

	m_Product_Code = CString(G_NormalConfigPara[0].MES_Product_Code);
	m_Product_Name = CString(G_NormalConfigPara[0].MES_Product_Name);

	this->UpdateData(false);
	

	return TRUE; 
}

CString CDongguanBetterMesMsgDlg::Get_MES_IP(void)
{
	CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_MES);
	BYTE nf1, nf2, nf3, nf4;
	pIP->GetAddress(nf1, nf2, nf3, nf4);
	CString str;
	str.Format(_T("%d.%d.%d.%d"), nf1, nf2, nf3, nf4);
	
	
	return str;
}
void CDongguanBetterMesMsgDlg::OnIpnFieldchangedIpaddressMes(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	
	CIPAddressCtrl* pIP = (CIPAddressCtrl*)GetDlgItem(IDC_IPADDRESS_MES);
	
	pIP->GetAddress(G_NormalConfigPara[0].MES_IP[0], 
		G_NormalConfigPara[0].MES_IP[1], 
		G_NormalConfigPara[0].MES_IP[2], 
		G_NormalConfigPara[0].MES_IP[3]);
	
	::SaveNormalConfigPara();

}




//void CDongguanBetterMesMsgDlg::OnEnChangeEditTcpPort()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
//	// 发送该通知，除非重写 CDialog::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//
//	this->UpdateData();
//	::G_NormalConfigPara[0].MES_TCP_Port = this->m_MES_TCP_Port;
//
//	::SaveNormalConfigPara();
//}

//void CDongguanBetterMesMsgDlg::OnEnChangeEditEquipmentCode()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
//	// 发送该通知，除非重写 CDialog::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//
//	this->UpdateData();
//
//	TCHAR *buff = this->m_EquipmentCode.GetBuffer();
//
//	memset(&G_NormalConfigPara[0].MES_Equipment_Code,0,sizeof(G_NormalConfigPara[0].MES_Equipment_Code));
//
//	memcpy(&G_NormalConfigPara[0].MES_Equipment_Code,buff,m_EquipmentCode.GetLength() * sizeof(TCHAR));
//
//	m_EquipmentCode.ReleaseBuffer();
//
//
//}

//void CDongguanBetterMesMsgDlg::OnEnChangeEditEquipmentAddr()
//{
//	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
//	// 发送该通知，除非重写 CDialog::OnInitDialog()
//	// 函数并调用 CRichEditCtrl().SetEventMask()，
//	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
//
//	// TODO:  在此添加控件通知处理程序代码
//	this->UpdateData();
//	TCHAR *buff = this->m_WorkStation_Code.GetBuffer();
//
//	memset(&G_NormalConfigPara[0].MES_WorkStation_Code,0,sizeof(G_NormalConfigPara[0].MES_WorkStation_Code));
//
//
//	memcpy(&G_NormalConfigPara[0].MES_WorkStation_Code,buff,m_WorkStation_Code.GetLength() * sizeof(TCHAR));
//
//	m_WorkStation_Code.ReleaseBuffer();
//
//	::SaveNormalConfigPara();
//
//}
