// DataViewView.h : interface of the CDataViewView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAVIEWVIEW_H__6EDB3ADC_0550_40DF_BA02_791F98E4154E__INCLUDED_)
#define AFX_DATAVIEWVIEW_H__6EDB3ADC_0550_40DF_BA02_791F98E4154E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGridCtrl;
class CDataViewData;
class CDataViewDoc;

class CDataViewView : public CScrollView
{
protected: // create from serialization only
	CDataViewView();
	DECLARE_DYNCREATE(CDataViewView)

// Attributes
protected:
	int m_iSelectGrid;
	// Printing information
	int m_iLeftMargin,m_iRightMargin,m_iTopMargin,m_iBottomMargin;
	int m_PrCharHeight;
	CFont m_PrinterFont;
  int         m_nPageHeight;
  CSize       m_LogicalPageSize,      // Page size in gridctrl units.
              m_PaperSize;            // Page size in device units.
  // additional properties to support Wysiwyg printing
  int					m_nHeaderHeight,m_nFooterHeight,m_nGap;
	int  m_nPageWidth;
	CUIntArray m_Pages;
	
	CDataViewDoc* GetDocument();
	UINT m_uStartX,m_uEndY;
	CFont m_screenFont;
	UINT m_cyScreen;
	UINT m_cxScreen;
	LONG m_nOldDepth;

// Operations
public:
	CFrameWnd *m_pOldFrame;
	CDialog *m_pCDialog;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataViewView)
	protected:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddTextLine(CDataViewData * pData);
	void AddGridData(CDataViewData * pData);
	virtual void OnFilePrintPreview();
	virtual ~CDataViewView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	void PrintFooter(CDC *pDC, CPrintInfo *pInfo);
	void PrintHeader(CDC *pDC, CPrintInfo *pInfo);

// Generated message map functions
public:
	//{{AFX_MSG(CDataViewView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnFilePrint();
	afx_msg void OnEditCopy();
	afx_msg void OnEditCut();
	afx_msg void OnEditPaste();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in DataViewView.cpp
inline CDataViewDoc* CDataViewView::GetDocument()
   { return (CDataViewDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAVIEWVIEW_H__6EDB3ADC_0550_40DF_BA02_791F98E4154E__INCLUDED_)
