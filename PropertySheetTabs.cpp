// PropertySheetTabs.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "PropertySheetTabs.h"

#include "DynamicLib.h"

#define ID_TIMER_CLOSE 1002

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetTabs

IMPLEMENT_DYNAMIC(CPropertySheetTabs, CPropertySheet)

CPropertySheetTabs::CPropertySheetTabs(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	m_sConnectServer = _T("");
	m_iCloseTimer = 0;
}

CPropertySheetTabs::CPropertySheetTabs(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_sConnectServer = _T("");
	m_iCloseTimer = 0;
}

CPropertySheetTabs::~CPropertySheetTabs()
{
	/*if( m_iCloseTimer )
		if( KillTimer(m_iCloseTimer) )
			m_iCloseTimer = 0;*/
	if( p_hardware )
		p_hardware->Close();
}


BEGIN_MESSAGE_MAP(CPropertySheetTabs, CPropertySheet)
	//{{AFX_MSG_MAP(CPropertySheetTabs)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetTabs message handlers
#include <STDARG.H>

BOOL CPropertySheetTabs::SetTabOrder(BOOL bShowWindow, HWND hWndMain, int Id,...)
{
	ASSERT(hWndMain != NULL); 
	BOOL bRtrn = FALSE; 
	UINT uiFlags = SWP_NOMOVE | SWP_NOSIZE; 
	
	// Validate that something was sent in 
	if (Id != 0){ 
		int mId;
		HWND curHwnd = ::GetDlgItem(hWndMain, mId=Id);
		HWND prevHwnd = hWndMain; 
		// Add the show windows if needed 
		if (bShowWindow == TRUE) 
			uiFlags |= SWP_SHOWWINDOW; 
		
		va_list curArg; 
		va_start(curArg, Id ); 
		// Initialize variable arguments 
		
		// Additional parameters can be optional 
		while ((curHwnd != NULL)&&(mId!=0)){ 
			// Now add any additional windows into the tab order 
			bRtrn = ::SetWindowPos(curHwnd, prevHwnd, 0, 0, 0, 0, uiFlags); 
			
			// Keep track of the last windows handle 
			prevHwnd = curHwnd; 
			
			// Get the next argument 
			if(mId = va_arg(curArg, int)) 
				curHwnd=::GetDlgItem(hWndMain, mId);
		} // while (lpTmp != NULL) 
		
		va_end(curArg); // Reset variable arguments 
	} // End of check whether handle is valid 
	
	return bRtrn; 
}

BOOL CPropertySheetTabs::ContinueModal() 
{
	// TODO: Add your specialized code here and/or call the base class

	if( p_hardware ){
		if( m_sConnectServer != p_hardware->m_sConnectServer ){
			m_sConnectServer = p_hardware->m_sConnectServer;
			if( m_iCloseTimer )
				if( KillTimer(m_iCloseTimer) )
					m_iCloseTimer = 0;
			if( !m_sConnectServer.IsEmpty() ){
				m_iCloseTimer = SetTimer(ID_TIMER_CLOSE,TIME_TO_CLOSE_CONNECTION,NULL);
			}
		}
	}
	
	return CPropertySheet::ContinueModal();
}

void CPropertySheetTabs::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_iCloseTimer )
		if( KillTimer(m_iCloseTimer) )
			m_iCloseTimer = 0;
	
	if( p_hardware )
		p_hardware->Close();
	
	CPropertySheet::OnTimer(nIDEvent);
}
