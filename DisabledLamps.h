#if !defined(AFX_DISABLEDLAMPS_H__C24397D1_AD53_4AFD_8B63_F1277ACB22F3__INCLUDED_)
#define AFX_DISABLEDLAMPS_H__C24397D1_AD53_4AFD_8B63_F1277ACB22F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DisabledLamps.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDisabledLamps dialog

class CDisabledLamps : public CDialog
{
// Construction
public:
	CDisabledLamps(UINT ids1 = 0,UINT ids2 = 0,CWnd* pParent = NULL);   // standard constructor
	CString text;
// Dialog Data
	//{{AFX_DATA(CDisabledLamps)
	enum { IDD = IDD_DIALOG_DISBLED_LAMPS };
	CEdit	m_Edit;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDisabledLamps)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT id_name1;
	UINT id_name2;
	// Generated message map functions
	//{{AFX_MSG(CDisabledLamps)
	virtual BOOL OnInitDialog();
	afx_msg void OnSaveData();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DISABLEDLAMPS_H__C24397D1_AD53_4AFD_8B63_F1277ACB22F3__INCLUDED_)
