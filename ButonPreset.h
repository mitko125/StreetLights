#if !defined(AFX_BUTONPRESET_H__FBAE2599_463E_4542_B3DA_DF14DAF7E1ED__INCLUDED_)
#define AFX_BUTONPRESET_H__FBAE2599_463E_4542_B3DA_DF14DAF7E1ED__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ButonPreset.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CButonPreset window

class CButonPreset : public CButton
{
// Construction
public:
	CButonPreset();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButonPreset)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CButonPreset();

	// Generated message map functions
protected:
	//{{AFX_MSG(CButonPreset)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BUTONPRESET_H__FBAE2599_463E_4542_B3DA_DF14DAF7E1ED__INCLUDED_)
