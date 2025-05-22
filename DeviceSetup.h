#if !defined(AFX_DEVICESETUP_H__933E1D11_F2FD_4C2E_B90B_8997E6174599__INCLUDED_)
#define AFX_DEVICESETUP_H__933E1D11_F2FD_4C2E_B90B_8997E6174599__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DeviceSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDeviceSetup

#include "PropertySheetTabs.h"

class CDeviceSetup : public CPropertySheetTabs
{
	DECLARE_DYNAMIC(CDeviceSetup)

// Construction
public:
	CDeviceSetup();
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDeviceSetup)
	public:
	virtual BOOL ContinueModal();
	virtual int DoModal();
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	UINT32 privilege;
	virtual ~CDeviceSetup();

	// Generated message map functions
protected:
	afx_msg void OnUpdate();
	CButton m_bPrintPrevief,m_bPrint,m_bUpdate;
	//{{AFX_MSG(CDeviceSetup)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DEVICESETUP_H__933E1D11_F2FD_4C2E_B90B_8997E6174599__INCLUDED_)
