// TV_TracersView.h : interface of the CTV_TracersView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TV_TRACERSVIEW_H__AD1B38DA_1ED0_4448_9D96_C1E873C7FB98__INCLUDED_)
#define AFX_TV_TRACERSVIEW_H__AD1B38DA_1ED0_4448_9D96_C1E873C7FB98__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Message.h"
#include "ButonPreset.h"

#include "TracerViewBase.h"

class CTV_TracersDoc;

class CTV_TracersView : public CTracerViewBase
{
protected: // create from serialization only
	CTV_TracersView();
	DECLARE_DYNCREATE(CTV_TracersView)

public:
	//{{AFX_DATA(CTV_TracersView)
	enum { IDD = IDD_TV_TRACERS_FORM };
	CString	m_sHarwareLimits;
	CString	m_sLoaclMovs;
	CString	m_sAC_Ready;
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTV_TracersView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	void StoreOldPosition();
	virtual ~CTV_TracersView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	FULL_STATUS FullStatus;
// Generated message map functions
protected:
	void EndProgram(BOOL fl_ok);
	virtual void ErrConnect();
	virtual void NewFullStatus(int cou_data);
	//{{AFX_MSG(CTV_TracersView)
	afx_msg void OnStop();
	afx_msg void OnButtonReper();
	afx_msg void OnButtonPosition();
	afx_msg void OnUpdateFilePrint(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFilePrintPreview(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSave(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileSaveAs(CCmdUI* pCmdUI);
	afx_msg void OnUpdateFileClose(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
/*
#ifndef _DEBUG  // debug version in TV_TracersView.cpp
inline CTV_TracersDoc* CTV_TracersView::GetDocument()
   { return (CTV_TracersDoc*)m_pDocument; }
#endif
*/
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TV_TRACERSVIEW_H__AD1B38DA_1ED0_4448_9D96_C1E873C7FB98__INCLUDED_)
