// LampPS.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "LampPS.h"

#include "TypeLampSetup.h"
#include "LampSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLampPS

IMPLEMENT_DYNAMIC(CLampPS, CPropertySheet)

CLampPS::CLampPS()
:CPropertySheetTabs(IDS_STREET_LAMPS)
{
}

CLampPS::~CLampPS()
{
}


BEGIN_MESSAGE_MAP(CLampPS, CPropertySheetTabs)
	//{{AFX_MSG_MAP(CLampPS)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(WM_USER, OnUpdate)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLampPS message handlers

BOOL CLampPS::ContinueModal() 
{
	// TODO: Add your specialized code here and/or call the base class
		// TODO: Add your specialized code here and/or call the base class
	CPropertyPage* pPage=GetActivePage();
	if(pPage)
		pPage->ContinueModal();
	return CPropertySheetTabs::ContinueModal();
}

int CLampPS::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
		m_psh.dwFlags |= PSH_NOAPPLYNOW;

	CLampSetup CLS;
	AddPage(&CLS);
	CTypeLampSetup CTLS;
	AddPage(&CTLS);


	if(GetPageCount()==0)
		return 0;
	return CPropertySheetTabs::DoModal();
}

BOOL CLampPS::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CPropertySheetTabs::OnChildNotify(message, wParam, lParam, pLResult);
}

BOOL CLampPS::OnInitDialog() 
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

void CLampPS::OnUpdate()
{
	CDialog *pD=GetActivePage();
	if(pD)
		pD->OnInitDialog();
}
