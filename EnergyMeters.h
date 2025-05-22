#if !defined(AFX_ENERGYMETERS_H__88E9DA47_0314_4868_8257_3196B8FAA007__INCLUDED_)
#define AFX_ENERGYMETERS_H__88E9DA47_0314_4868_8257_3196B8FAA007__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EnergyMeters.h : header file
//

#include "../../Grid1_0/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CEnergyMeters dialog

class CEnergyMeters : public CPropertyPage
{
	DECLARE_DYNCREATE(CEnergyMeters)

// Construction
public:
	CEnergyMeters();
	~CEnergyMeters();

// Dialog Data
	//{{AFX_DATA(CEnergyMeters)
	enum { IDD = IDD_ENRGY_METERS };
	CButton	m_bTotal1;
	CButton	m_bHistory1;
	CButton	m_bCurrent1;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CEnergyMeters)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
protected:
	float GetCalculatedPawer(UINT id_router);
	UINT GetLampCount(UINT id_router);
	void StoreConfigMessage();
	BOOL LoadData(bool fl_select=false);
	void EnableKeys();
	virtual BOOL ContinueModal();
	CGridCtrl m_Grid;
	BOOL fl_config;
	int m_iSelectItem;
	// Generated message map functions
	//{{AFX_MSG(CEnergyMeters)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonPrintPreviev();
	afx_msg void OnButtonCurrent1();
	afx_msg void OnButtonCurrent();
	afx_msg void OnButtonTotal1();
	afx_msg void OnButtonTotal();
	afx_msg void OnButtonHistory1();
	afx_msg void OnButtonHistory();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENERGYMETERS_H__88E9DA47_0314_4868_8257_3196B8FAA007__INCLUDED_)
