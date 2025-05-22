#if !defined(AFX_TIMERSINROUTERSSETUP_H__03BE5484_95A3_40C1_8AEA_65A28D190BD3__INCLUDED_)
#define AFX_TIMERSINROUTERSSETUP_H__03BE5484_95A3_40C1_8AEA_65A28D190BD3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimersInRoutersSetup.h : header file
//


#include "../../Grid1_0/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTimersInRoutersSetup dialog

class CTimersInRoutersSetup : public CPropertyPage
{
		DECLARE_DYNCREATE(CTimersInRoutersSetup)
// Construction
public:
	CTimersInRoutersSetup();
	~CTimersInRoutersSetup();

// Dialog Data
	//{{AFX_DATA(CTimersInRoutersSetup)
	enum { IDD = IDD_ROUTER_TIMERS_SETUP };
	CButton	m_bGet1;
	CButton	m_bSet1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTimersInRoutersSetup)
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
	CString OldText;
	// Generated message map functions
	//{{AFX_MSG(CTimersInRoutersSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonPrintPreviev();
	afx_msg void OnButtonSet1();
	afx_msg void OnButtonGet1();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonGet();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMERSINROUTERSSETUP_H__03BE5484_95A3_40C1_8AEA_65A28D190BD3__INCLUDED_)
