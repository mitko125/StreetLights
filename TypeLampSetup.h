#if !defined(AFX_TYPELAMPSETUP_H__833CEF85_BFBD_4200_9C86_4B0DA7ACD1BD__INCLUDED_)
#define AFX_TYPELAMPSETUP_H__833CEF85_BFBD_4200_9C86_4B0DA7ACD1BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TypeLampSetup.h : header file
//

#include "../../Grid1_0/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTypeLampSetup dialog

class CTypeLampSetup : public CPropertyPage
{
	DECLARE_DYNCREATE(CTypeLampSetup)

// Construction
public:
	CTypeLampSetup();
	~CTypeLampSetup();

// Dialog Data
	//{{AFX_DATA(CTypeLampSetup)
	enum { IDD = IDD_TYPE_LAMP_SETUP };
	CButton	m_bNew;
	CButton	m_bDel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTypeLampSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	void EnableKeys();
	BOOL LoadData(bool fl_select=false);
	void StoreConfigMessage();
	virtual BOOL ContinueModal();
	CGridCtrl m_Grid;
	BOOL fl_config;
	int m_iSelectItem;
	CString OldText;
	// Generated message map functions
	//{{AFX_MSG(CTypeLampSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonPrintPreviev();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TYPELAMPSETUP_H__833CEF85_BFBD_4200_9C86_4B0DA7ACD1BD__INCLUDED_)
