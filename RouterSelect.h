#if !defined(AFX_ROUTERSELECT_H__256F865A_3A0E_4422_A920_5E780499BE5F__INCLUDED_)
#define AFX_ROUTERSELECT_H__256F865A_3A0E_4422_A920_5E780499BE5F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RouterSelect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CRouterSelect dialog

class CRouterSelect : public CDialog
{
// Construction
public:
	int m_iNumberRouter;
	CString RouterName;
	CRouterSelect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CRouterSelect)
	enum { IDD = IDD_ROUTER_SELECT };
	CComboBox	m_ComboCom;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRouterSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CRouterSelect)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnDblclkCombo2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ROUTERSELECT_H__256F865A_3A0E_4422_A920_5E780499BE5F__INCLUDED_)
