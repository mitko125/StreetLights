#if !defined(AFX_WPANSETUP_H__E9AFBE43_354C_4D36_B95F_4112375DFDB8__INCLUDED_)
#define AFX_WPANSETUP_H__E9AFBE43_354C_4D36_B95F_4112375DFDB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WpanSetup.h : header file
//

#include "../../Grid1_0/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CWpanSetup dialog

class CWpanSetup : public CPropertyPage
{
	DECLARE_DYNCREATE(CWpanSetup)

// Construction
public:
	CWpanSetup();
	~CWpanSetup();

// Dialog Data
	//{{AFX_DATA(CWpanSetup)
	enum { IDD = IDD_WPAN_SETUP };
	CButton	m_bGetMAC1;
	CButton	m_bGet1;
	CButton	m_bSet1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CWpanSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL ContinueModal();
	void StoreConfigMessage();
	void EnableKeys();
	BOOL LoadData(bool fl_select=false);
	CGridCtrl m_Grid;
	BOOL fl_config;
	int m_iSelectItem;
	// Generated message map functions
	//{{AFX_MSG(CWpanSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonPrintPreviev();
	afx_msg void OnButtonSet1();
	afx_msg void OnButtonGet1();
	afx_msg void OnButtonGetMac1();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonGet();
	afx_msg void OnButtonGetMac();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WPANSETUP_H__E9AFBE43_354C_4D36_B95F_4112375DFDB8__INCLUDED_)
