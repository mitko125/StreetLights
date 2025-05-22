#if !defined(AFX_DATETIMELIMITS_H__27B8C0FF_DA87_4D04_B1AE_A14A12DB7B21__INCLUDED_)
#define AFX_DATETIMELIMITS_H__27B8C0FF_DA87_4D04_B1AE_A14A12DB7B21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DateTimeLimits.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDateTimeLimits dialog

class CDateTimeLimits : public CDialog
{
// Construction
public:
	CDateTimeLimits(CWnd* pParent = NULL);   // standard constructor
 
	COleDateTime d_min;
	COleDateTime d_max;
	COleDateTime t_min;
	COleDateTime t_max;
// Dialog Data
	//{{AFX_DATA(CDateTimeLimits)
	enum { IDD = IDD_DATE_TIME_LIMIT };
	CDateTimeCtrl	m_TimeMin;
	CDateTimeCtrl	m_TimeMax;
	CDateTimeCtrl	m_DateMin;
	CDateTimeCtrl	m_DateMax;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDateTimeLimits)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDateTimeLimits)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATETIMELIMITS_H__27B8C0FF_DA87_4D04_B1AE_A14A12DB7B21__INCLUDED_)
