#if !defined(AFX_PAGESETUP_H__6845D646_EA8E_45FC_ADF9_9DC2CCF2F2F9__INCLUDED_)
#define AFX_PAGESETUP_H__6845D646_EA8E_45FC_ADF9_9DC2CCF2F2F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PageSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPageSetup dialog

class CPageSetup : public CDialog
{
// Construction
public:
	CPageSetup(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPageSetup)
	enum { IDD = IDD_PAGE_SETUP };
	BOOL	m_bFitPage;
	UINT	m_uBottom;
	UINT	m_uLeft;
	UINT	m_uRight;
	UINT	m_uTop;
	BOOL	m_bPrintBlackWhite;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPageSetup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPageSetup)
	afx_msg void OnCheckFitPage();
	virtual BOOL OnInitDialog();
	afx_msg void OnBlackWite();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PAGESETUP_H__6845D646_EA8E_45FC_ADF9_9DC2CCF2F2F9__INCLUDED_)
