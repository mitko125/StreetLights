#if !defined(AFX_PROPERTYSHEETTABS_H__DEFE4A0C_8E74_4D29_97D0_888DDF043AAF__INCLUDED_)
#define AFX_PROPERTYSHEETTABS_H__DEFE4A0C_8E74_4D29_97D0_888DDF043AAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropertySheetTabs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropertySheetTabs

class CPropertySheetTabs : public CPropertySheet
{
	DECLARE_DYNAMIC(CPropertySheetTabs)

// Construction
public:
	CPropertySheetTabs(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CPropertySheetTabs(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropertySheetTabs)
	public:
	virtual BOOL ContinueModal();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropertySheetTabs();

	// Generated message map functions
protected:
	UINT m_iCloseTimer;
	CString m_sConnectServer;
	BOOL SetTabOrder(BOOL bShowWindow,HWND hWndMain,int Id,...);
	//{{AFX_MSG(CPropertySheetTabs)
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPERTYSHEETTABS_H__DEFE4A0C_8E74_4D29_97D0_888DDF043AAF__INCLUDED_)
