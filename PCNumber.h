#if !defined(AFX_PCNUMBER_H__D2F76A31_3050_47C5_9825_DEBC8703CD1A__INCLUDED_)
#define AFX_PCNUMBER_H__D2F76A31_3050_47C5_9825_DEBC8703CD1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PCNumber.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPCNumber dialog

class CPCNumber : public CDialog
{
// Construction
public:
	CPCNumber(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPCNumber)
	enum { IDD = IDD_PC_NUMBER };
	int		m_iNumber;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPCNumber)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPCNumber)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCNUMBER_H__D2F76A31_3050_47C5_9825_DEBC8703CD1A__INCLUDED_)
