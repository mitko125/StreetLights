#if !defined(AFX_TRACERVIEWSMAL_H__69FA2338_106A_4033_988B_42FFBF18F861__INCLUDED_)
#define AFX_TRACERVIEWSMAL_H__69FA2338_106A_4033_988B_42FFBF18F861__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TracerViewSmal.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTracerViewSmal form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

#include "TracerViewBase.h"
#include "ColorStatic.h"
#include "DynamicLED.h"

class CTracerViewSmal : public CTracerViewBase
{
protected:
	CTracerViewSmal();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CTracerViewSmal)

// Form Data
public:
	static CFont m_font;
	unsigned int OldControlerStatus;
	//{{AFX_DATA(CTracerViewSmal)
	enum { IDD = IDD_TV_TRACERS_FORM_SMAL };
	CDynamicLED	m_LedSelect;
	CColorStatic	m_ColorPosition;
	CColorStatic	m_ColorName;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTracerViewSmal)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual void OutGroup();
	virtual void InGroup();
	BOOL m_bErrConnect;
	CString	m_sTextStatus;
	BOOL OnToolTipNeedText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
	virtual void MoveInOldPositionAndName(CString name, WORD IdHardware, int ModbusAddrres,int x,int y);
	CToolTipCtrl m_ToolTip;
	BOOL m_bFocus;
	BOOL m_bFocus1;
	virtual void KillFocus();
	virtual void SetFocus();
	BOOL m_bFirstTime;
	CDynamicLED m_dynLED;
	virtual void ErrConnect();
	virtual void NewFullStatus(int cou_data);
	FULL_STATUS FullStatus;
	virtual ~CTracerViewSmal();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CTracerViewSmal)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRACERVIEWSMAL_H__69FA2338_106A_4033_988B_42FFBF18F861__INCLUDED_)
