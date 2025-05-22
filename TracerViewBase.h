#if !defined(AFX_TRACERVIEWBASE_H__A16E66F1_A6C1_4920_97E5_FF4B99000FB1__INCLUDED_)
#define AFX_TRACERVIEWBASE_H__A16E66F1_A6C1_4920_97E5_FF4B99000FB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TracerViewBase.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTracerViewBase form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
class CTV_TracersDoc;

class CTracerViewBase : public CFormView
{
protected:
	CTracerViewBase();           // protected constructor used by dynamic creation
	CTracerViewBase(UINT nIDTemplate);
	DECLARE_DYNCREATE(CTracerViewBase)

// Form Data
public:
	//{{AFX_DATA(CTracerViewBase)
	enum { IDD = IDD_TV_TRACERS_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	UINT32 id_device;

// Operations
public:
	virtual void OutGroup(){};
	virtual void InGroup(){};
	virtual void KillFocus(){};
	virtual void EndProgram(BOOL fl_ok){};
	virtual void ErrConnect(){};
	virtual void HardwareStoped(BOOL CloseWindows);
	virtual void NewFullStatus(int cou_data){};
	CTV_TracersDoc* GetDocument();
	virtual void MoveInOldPositionAndName(CString name, WORD IdHardware, int ModbusAddrres,int x,int y);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTracerViewBase)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void SetFocus(){};
	afx_msg LRESULT OnEndProgram(WPARAM wParam, LPARAM lParam);
	int old_y;
	int old_x;
	afx_msg LRESULT OnErrConnect(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRedrawFullStatus(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnHardwareStoped(WPARAM wParam, LPARAM lParam);
	virtual ~CTracerViewBase();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTracerViewBase)
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DataViewView.cpp
inline CTV_TracersDoc* CTracerViewBase::GetDocument()
   { return (CTV_TracersDoc*)m_pDocument; }
#endif
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACERVIEWBASE_H__A16E66F1_A6C1_4920_97E5_FF4B99000FB1__INCLUDED_)
