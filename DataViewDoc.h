// DataViewDoc.h : interface of the CDataViewDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAVIEWDOC_H__8B79C235_D844_401D_B065_54640F634FDD__INCLUDED_)
#define AFX_DATAVIEWDOC_H__8B79C235_D844_401D_B065_54640F634FDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTextLine;
class CGridCtrl;

class CDataViewDoc : public CDocument
{
protected: // create from serialization only
	CDataViewDoc();
	DECLARE_DYNCREATE(CDataViewDoc)

// Attributes
public:
	CObArray m_Data;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDataViewDoc)
	public:
	virtual void Serialize(CArchive& ar);
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
	virtual void OnCloseDocument();
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddTextLine(CTextLine * pTL,BOOL NewPage=false);
	void AddGrid(CGridCtrl * pGrid,BOOL NewPage=false);
	virtual ~CDataViewDoc();
	void FreeData(BOOL DeleteGrid=true);
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDataViewDoc)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DATAVIEWDOC_H__8B79C235_D844_401D_B065_54640F634FDD__INCLUDED_)
