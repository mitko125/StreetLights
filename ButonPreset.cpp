// ButonPreset.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "ButonPreset.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CButonPreset

CButonPreset::CButonPreset()
{
}

CButonPreset::~CButonPreset()
{
}


BEGIN_MESSAGE_MAP(CButonPreset, CButton)
	//{{AFX_MSG_MAP(CButonPreset)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CButonPreset message handlers

void CButonPreset::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd* pOwner = GetOwner();
	NMHDR comand;
	comand.hwndFrom = GetSafeHwnd();
	comand.idFrom = GetDlgCtrlID();
	comand.code = NM_CLICK;
  if (pOwner)
    pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(),(LPARAM)&comand);
	CButton::OnLButtonDown(nFlags, point);
}

void CButonPreset::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CWnd* pOwner = GetOwner();
	NMHDR comand;
	comand.hwndFrom = GetSafeHwnd();
	comand.idFrom = GetDlgCtrlID();
	comand.code = 0;
  if (pOwner)
    pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(),(LPARAM)&comand);
	CButton::OnLButtonUp(nFlags, point);
}
