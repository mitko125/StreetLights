// DeviceSetup.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "DeviceSetup.h"

#include "TimerSetup.h"
#include "WpanSetup.h"
#include "HardwareSetup.h"
#include "TimersInRoutersSetup.h"
#include "LampSetup.h"
#include "LampInRouterSetup.h"
#include "EnergyMeters.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDeviceSetup

IMPLEMENT_DYNAMIC(CDeviceSetup, CPropertySheet)

CDeviceSetup::CDeviceSetup()
	:CPropertySheetTabs(IDS_DEVICE_SETUP)
{
	privilege = 0;
}

CDeviceSetup::~CDeviceSetup()
{
}


BEGIN_MESSAGE_MAP(CDeviceSetup, CPropertySheetTabs)
	//{{AFX_MSG_MAP(CDeviceSetup)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(WM_USER, OnUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDeviceSetup message handlers


BOOL CDeviceSetup::ContinueModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	CPropertyPage* pPage=GetActivePage();
	if(pPage)
		pPage->ContinueModal();
	return CPropertySheetTabs::ContinueModal();
}

int CDeviceSetup::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_psh.dwFlags |= PSH_NOAPPLYNOW;


	CTimerSetup CTS;
	AddPage(&CTS);

	CTimersInRoutersSetup CTRS;
	AddPage(&CTRS);

	CLampInRouterSetup CLIRS;
	AddPage(&CLIRS);

	CWpanSetup CWS;
	AddPage(&CWS);

	CHardwareSetup CHS;
	AddPage(&CHS);

	CEnergyMeters CEM;
	AddPage(&CEM);

	if(GetPageCount()==0)
		return 0;
	return CPropertySheetTabs::DoModal();
}

BOOL CDeviceSetup::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertySheetTabs::OnChildNotify(message, wParam, lParam, pLResult);
}

BOOL CDeviceSetup::OnInitDialog() 
{
	BOOL bResult = CPropertySheetTabs::OnInitDialog();
	
	// TODO: Add your specialized code here
	GetDlgItem(IDOK)->EnableWindow(false); 
	GetDlgItem(IDOK)->ShowWindow(SW_HIDE);

	CRect rect,tabrect;


	GetDlgItem(IDHELP)->GetWindowRect(rect);
	ScreenToClient(rect);
	GetDlgItem(IDCANCEL)->GetWindowRect(tabrect);
	ScreenToClient(tabrect);
	
	int offset=rect.left-tabrect.left;
	int width = rect.Width();

	GetTabControl()->GetWindowRect(tabrect);
	ScreenToClient(tabrect);

	GetDlgItem(IDCANCEL)->MoveWindow(rect);

	GetDlgItem(IDHELP)->ShowWindow(SW_HIDE);
	CString text;
	text.LoadString(IDS_TEXT_EXIT);
	GetDlgItem(IDCANCEL)->SetWindowText(text);

	rect.left = tabrect.left; rect.right = tabrect.left + width;
	text.LoadString(IDS_BUT_PRINT_PREVIEF);
	m_bPrintPrevief.Create(text,
		BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, IDC_BUTTON_PRINT_PREVIEV);
	m_bPrintPrevief.SetFont(GetFont());

	rect.left += offset; rect.right += offset;
	text.LoadString(IDS_BUT_PRINT);
	m_bPrint.Create(text,
		BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, IDC_BUTTON_PRINT);
	m_bPrint.SetFont(GetFont());

	rect.left += offset; rect.right += offset;
	text.LoadString(IDS_BUT_UPDATE);
	m_bUpdate.Create(text,
		BS_PUSHBUTTON|WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, WM_USER);
	m_bUpdate.SetFont(GetFont());

	SetTabOrder(TRUE,this->m_hWnd,IDC_BUTTON_PRINT_PREVIEV,IDC_BUTTON_PRINT,
		WM_USER,IDCANCEL,0);
	return bResult;
}

void CDeviceSetup::OnUpdate()
{
	CDialog *pD=GetActivePage();
	if(pD)
		pD->OnInitDialog();
}
