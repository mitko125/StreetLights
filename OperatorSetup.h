#if !defined(AFX_OPERATORSETUP_H__77C611A4_00FB_4389_9F7F_1492B662D030__INCLUDED_)
#define AFX_OPERATORSETUP_H__77C611A4_00FB_4389_9F7F_1492B662D030__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OperatorSetup.h : header file
//
#include "../../Grid1_0/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// COperatorSetup dialog

class COperatorSetup : public CDialog
{
// Construction
public:
	COperatorSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(COperatorSetup)
	enum { IDD = IDD_OPERATORS_SETUP };
	CButton	m_bNew;
	CButton	m_bDel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COperatorSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL LoadData(bool fl_select=false);
	void EnableKeys();
	virtual BOOL ContinueModal();
	int m_iSelectItem;
	void StoreConfigMessage();
	BOOL fl_config;
	CString OldText;
	CGridCtrl m_Grid;
	// Generated message map functions
	//{{AFX_MSG(COperatorSetup)
	afx_msg void OnButtonPrintPreviev();
	afx_msg void OnButtonPrint();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonNew();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPERATORSETUP_H__77C611A4_00FB_4389_9F7F_1492B662D030__INCLUDED_)
