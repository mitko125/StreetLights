#if !defined(AFX_AUTOONOFF_H__3E81F22F_29D3_46F0_BB32_A48689CD418C__INCLUDED_)
#define AFX_AUTOONOFF_H__3E81F22F_29D3_46F0_BB32_A48689CD418C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutoOnOff.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAutoOnOff dialog

class CAutoOnOff : public CDialog
{
// Construction
public:
	CAutoOnOff(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAutoOnOff)
	enum { IDD = IDD_AUTO_ON_OFF };
	CStatic	m_cSunsetOffset;
	CStatic	m_cSunriseOffset;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutoOnOff)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	UINT m_iCloseTimer;
	UINT32 id_timer;
	// Generated message map functions
	//{{AFX_MSG(CAutoOnOff)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AUTOONOFF_H__3E81F22F_29D3_46F0_BB32_A48689CD418C__INCLUDED_)
