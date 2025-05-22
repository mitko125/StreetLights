#if !defined(AFX_TIMERSETUP_H__E062C122_8F6B_4939_814F_19643F2AE720__INCLUDED_)
#define AFX_TIMERSETUP_H__E062C122_8F6B_4939_814F_19643F2AE720__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TimerSetup.h : header file
//

#include "../../Grid1_0/GridCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CTimerSetup dialog

class CTimerSetup : public CPropertyPage
{
	DECLARE_DYNCREATE(CTimerSetup)

// Construction
public:
	CTimerSetup();
	~CTimerSetup();

// Dialog Data
	//{{AFX_DATA(CTimerSetup)
	enum { IDD = IDD_TIMERS_SETUP };
	CStatic	m_cSunsetOffset;
	CStatic	m_cSunriseOffset;
	CStatic	m_cSunset;
	CStatic	m_cSunrise;
	CEdit	m_eLon;
	CEdit	m_eLat;
	CButton	m_bNew;
	CButton	m_bDel;
	double	m_dLat;
	double	m_dLon;
	int		m_OffsetOnOff;
	BOOL	m_bAutoOnOff;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CTimerSetup)
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
	//{{AFX_MSG(CTimerSetup)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonPrint();
	afx_msg void OnButtonPrintPreviev();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDel();
	afx_msg void OnCalculate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIMERSETUP_H__E062C122_8F6B_4939_814F_19643F2AE720__INCLUDED_)
