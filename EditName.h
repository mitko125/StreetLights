#if !defined(AFX_EDITNAME_H__814FCE0D_8C0B_420B_9D10_50FF00E69F62__INCLUDED_)
#define AFX_EDITNAME_H__814FCE0D_8C0B_420B_9D10_50FF00E69F62__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditName dialog

class CEditName : public CDialog
{
// Construction
public:
	CEditName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditName)
	enum { IDD = IDD_NEW_NAME };
	CString	m_cName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditName)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITNAME_H__814FCE0D_8C0B_420B_9D10_50FF00E69F62__INCLUDED_)
