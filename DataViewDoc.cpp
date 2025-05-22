// DataViewDoc.cpp : implementation of the CDataViewDoc class
//

#include "stdafx.h"
#include "TV_Tracers.h"

#include "DataViewView.h"
#include "DataViewDoc.h"
#include "DataViewData.h"


#include "../../Grid1_0/GridCtrl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataViewDoc

IMPLEMENT_DYNCREATE(CDataViewDoc, CDocument)

/////////////////////////////////////////////////////////////////////////////
// CDataViewDoc construction/destruction

BEGIN_MESSAGE_MAP(CDataViewDoc, CDocument)
	//{{AFX_MSG_MAP(CDataViewDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CDataViewDoc::CDataViewDoc()
{
	// TODO: add one-time construction code here
}

CDataViewDoc::~CDataViewDoc()
{
//	int i=5;
}




/////////////////////////////////////////////////////////////////////////////
// CDataViewDoc serialization

void CDataViewDoc::Serialize(CArchive& ar)
{

	m_Data.Serialize(ar);
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
		POSITION	pos = GetFirstViewPosition();
		CDataViewView * pView = (CDataViewView*)GetNextView(pos);
		ASSERT_VALID(pView);
		for(int i=0;i<m_Data.GetSize();i++){
			CDataViewData * pData = (CDataViewData*)m_Data[i];
			if(pData->pGrid)
				pView->AddGridData(pData);
			else if(pData->pTL)
				pView->AddTextLine(pData);
		}
		UpdateAllViews(NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDataViewDoc diagnostics

#ifdef _DEBUG
void CDataViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDataViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDataViewDoc commands

void CDataViewDoc::OnCloseDocument() 
{
	// TODO: Add your specialized code here and/or call the base class
	FreeData();
	CDocument::OnCloseDocument();
}

BOOL CDataViewDoc::OnOpenDocument(LPCTSTR lpszPathName) 
{
	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;
	
	// TODO: Add your specialized creation code here
	return TRUE;
}

void CDataViewDoc::FreeData(BOOL DeleteGrid)
{
	for(int i=0;i<m_Data.GetSize();i++){
		CDataViewData *pData=(CDataViewData*)m_Data[i];
		if(DeleteGrid&&pData->pGrid){
			pData->pGrid->DestroyWindow();
			delete pData->pGrid;
		}
		if(pData->pTL){
			pData->pTL->FreeData();
			delete pData->pTL;
		}
		delete m_Data[i];
	}
	m_Data.RemoveAll();
}

void CDataViewDoc::AddGrid(CGridCtrl *pGrid,BOOL NewPage)
{
	POSITION	pos = GetFirstViewPosition();
	CDataViewView * pView = (CDataViewView*)GetNextView(pos);
	ASSERT_VALID(pView);
	CDataViewData * pData = new CDataViewData;
	pData->pGrid=pGrid;
	pData->NewPage=NewPage;
	m_Data.Add(pData);
	pData->DlgId=m_Data.GetSize();
	pView->AddGridData(pData);
	UpdateAllViews(NULL);
}

void CDataViewDoc::AddTextLine(CTextLine *pTL,BOOL NewPage)
{
	POSITION	pos = GetFirstViewPosition();
	CDataViewView * pView = (CDataViewView*)GetNextView(pos);
	ASSERT_VALID(pView);
	CDataViewData * pData = new CDataViewData;
	pData->pTL=pTL;
	pData->NewPage=NewPage;
	m_Data.Add(pData);
	pData->DlgId=m_Data.GetSize();
	pView->AddTextLine(pData);
	UpdateAllViews(NULL);
}

