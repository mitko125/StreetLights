#if !defined(AFX_LAMPSETUP_H__05E7190D_0E3C_4175_933E_7CC998ABD9CB__INCLUDED_)
#define AFX_LAMPSETUP_H__05E7190D_0E3C_4175_933E_7CC998ABD9CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LampSetup.h : header file
//

#include "../../Grid1_0/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CLampSetup dialog

class CLampSetup : public CPropertyPage
{
	DECLARE_DYNCREATE(CLampSetup)

// Construction
public:
	CLampSetup();
	~CLampSetup();

// Dialog Data
	//{{AFX_DATA(CLampSetup)
	enum { IDD = IDD_LAMP_SETUP };
	CButton	m_bLight1;
	CEdit	m_bEditNumber;
	CButton	m_bNew;
	CButton	m_bDel;
	CButton	m_bLoadRouter;
	CButton	m_bLoadLamp;
	UINT	m_NewNumber;
	int		m_iLight;
	CString	m_sSucses;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CLampSetup)
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
	//{{AFX_MSG(CLampSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonPrintPreviev();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonLoadRouters();
	afx_msg void OnButtonLoadRoutersReject();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonLight1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAMPSETUP_H__05E7190D_0E3C_4175_933E_7CC998ABD9CB__INCLUDED_)
