#if !defined(AFX_NEWOPERATOR_H__1AA913A8_FECB_4047_AF65_C89CA5A57EBB__INCLUDED_)
#define AFX_NEWOPERATOR_H__1AA913A8_FECB_4047_AF65_C89CA5A57EBB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewOperator.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewOperator dialog

class CNewOperator : public CDialog
{
// Construction
public:
	CNewOperator(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CNewOperator)
	enum { IDD = IDD_NEW_OPERATOR };
	CString	m_Name;
	CString	m_Password;
	CString	m_ConfirmPassword;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewOperator)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewOperator)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWOPERATOR_H__1AA913A8_FECB_4047_AF65_C89CA5A57EBB__INCLUDED_)
