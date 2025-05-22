#if !defined(AFX_LAMPGROUPS_H__016E0511_D5D2_40BB_8A90_200E31C11F0A__INCLUDED_)
#define AFX_LAMPGROUPS_H__016E0511_D5D2_40BB_8A90_200E31C11F0A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LampGroups.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLampGroups dialog

class CLampGroups : public CDialog
{
// Construction
public:
	UINT32 groups;
	UINT32 id_lamp;
	CLampGroups(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CLampGroups)
	enum { IDD = IDD_LAMP_GROUPS };
	CString	m_sSucses;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLampGroups)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void SetCheckBoxs();

	// Generated message map functions
	//{{AFX_MSG(CLampGroups)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonLoadDb();
	afx_msg void OnButtonSaveDb();
	afx_msg void OnButtonLoadLamp();
	afx_msg void OnButtonSaveLamp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LAMPGROUPS_H__016E0511_D5D2_40BB_8A90_200E31C11F0A__INCLUDED_)
