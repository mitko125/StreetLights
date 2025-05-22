#if !defined(AFX_LAMPINROUTERSETUP_H__8AA124BB_B7CF_4224_894B_886256446225__INCLUDED_)
#define AFX_LAMPINROUTERSETUP_H__8AA124BB_B7CF_4224_894B_886256446225__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LampInRouterSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLampInRouterSetup dialog

#include "../../Grid1_0/GridCtrl.h"

class CLampInRouterSetup : public CPropertyPage
{
	DECLARE_DYNCREATE(CLampInRouterSetup)

// Construction
public:
	CLampInRouterSetup();
	~CLampInRouterSetup();

// Dialog Data
	//{{AFX_DATA(CLampInRouterSetup)
	enum { IDD = IDD_LAMP_IN_ROUTER_SETUP };
	CButton	m_bGetMAC1;
	CButton	m_bGet1;
	CButton	m_bSet1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLampInRouterSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	unsigned int u16LampsInTable,u16LampsConnected;
	virtual BOOL ContinueModal();
	void StoreConfigMessage();
	void EnableKeys();
	BOOL LoadData(bool fl_select=false);
	CGridCtrl m_Grid;
	BOOL fl_config;
	int m_iSelectItem;
	// Generated message map functions
	//{{AFX_MSG(CLampInRouterSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonPrintPreviev();
	afx_msg void OnButtonGet1();
	afx_msg void OnButtonGet();
	afx_msg void OnButtonGetReject1();
	afx_msg void OnButtonLampsSet1();
	afx_msg void OnButtonGetLanpsStatus1();
	afx_msg void OnButtonGetLanpsStatus();
	afx_msg void OnButtonLampsSet();
	afx_msg void OnButtonGetReject();
	afx_msg void OnButtonLoadRoutersReject();
	afx_msg void OnButtonSet();
	afx_msg void OnButtonSet1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAMPINROUTERSETUP_H__8AA124BB_B7CF_4224_894B_886256446225__INCLUDED_)
