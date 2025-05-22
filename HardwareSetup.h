#if !defined(AFX_HARDWARESETUP_H__DA5EB571_3034_4E8B_B12D_B18F92C3092B__INCLUDED_)
#define AFX_HARDWARESETUP_H__DA5EB571_3034_4E8B_B12D_B18F92C3092B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HardwareSetup.h : header file
//

#include "../../Grid1_0/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CHardwareSetup dialog

class CHardwareSetup : public CPropertyPage
{
	DECLARE_DYNCREATE(CHardwareSetup)

// Construction
public:
	CHardwareSetup();
	~CHardwareSetup();
protected:
// Dialog Data
	//{{AFX_DATA(CHardwareSetup)
	enum { IDD = IDD_HARDWARE_SETUP };
	CButton	m_bClear1;
	CEdit	m_bEditNumber;
	CButton	m_bTest1;
	CButton	m_bOn1;
	CButton	m_bOff1;
	CButton	m_bLight1;
	CIPAddressCtrl	m_IPAddres;
	CButton	m_bNew;
	CButton	m_bDel;
	int		m_iLights;
	UINT	m_NewNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CHardwareSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual BOOL ContinueModal();
	void StoreConfigMessage();
	BOOL fl_config;
	void EnableKeys();
	int m_iSelectItem;
	BOOL LoadData(bool fl_select=false);
	CGridCtrl m_Grid;
	CString OldText;
	// Generated message map functions
	//{{AFX_MSG(CHardwareSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDel();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonPrintPreviev();
	afx_msg void OnButtonTest();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonTest1();
	afx_msg void OnButtonOn();
	afx_msg void OnButtonOn1();
	afx_msg void OnButtonOff();
	afx_msg void OnButtonOff1();
	afx_msg void OnButtonLight();
	afx_msg void OnButtonLight1();
	afx_msg void OnButtonClearRam1();
	afx_msg void OnButtonClearRam();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HARDWARESETUP_H__DA5EB571_3034_4E8B_B12D_B18F92C3092B__INCLUDED_)
