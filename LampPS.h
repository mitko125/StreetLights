#if !defined(AFX_LAMPPS_H__6E8AD626_A11E_4573_AB27_FCDE78595F48__INCLUDED_)
#define AFX_LAMPPS_H__6E8AD626_A11E_4573_AB27_FCDE78595F48__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LampPS.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLampPS

#include "PropertySheetTabs.h"

class CLampPS : public CPropertySheetTabs
{
	DECLARE_DYNAMIC(CLampPS)

// Construction
public:
	CLampPS();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLampPS)
	public:
	virtual BOOL ContinueModal();
	virtual int DoModal();
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLampPS();

	// Generated message map functions
protected:
	afx_msg void OnUpdate();
	CButton m_bPrintPrevief,m_bPrint,m_bUpdate;
	//{{AFX_MSG(CLampPS)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAMPPS_H__6E8AD626_A11E_4573_AB27_FCDE78595F48__INCLUDED_)
