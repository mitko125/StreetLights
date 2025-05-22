#if !defined(AFX_OPERATORSELECT_H__0A0F2947_7C35_4690_A237_6D8E50C6E955__INCLUDED_)
#define AFX_OPERATORSELECT_H__0A0F2947_7C35_4690_A237_6D8E50C6E955__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OperatorSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COperatorSelect dialog

class COperatorSelect : public CDialog
{
// Construction
public:
	COperatorSelect(CWnd* pParent = NULL);   // standard constructor
	UINT32 id_operator;
// Dialog Data
	//{{AFX_DATA(COperatorSelect)
	enum { IDD = IDD_SELECT_OPERATOR };
	CComboBox	m_OL;
	CString	m_Password;
	CString	m_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COperatorSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COperatorSelect)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPERATORSELECT_H__0A0F2947_7C35_4690_A237_6D8E50C6E955__INCLUDED_)
