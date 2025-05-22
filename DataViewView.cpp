// DataViewView.cpp : implementation of the CDataViewView class
//

#include "stdafx.h"
#include "TV_Tracers.h"

#include "DataViewDoc.h"
#include "DataViewView.h"
#include "DataViewData.h"

#include "../../Grid1_0/GridCtrl.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataViewView

IMPLEMENT_DYNCREATE(CDataViewView, CScrollView)

BEGIN_MESSAGE_MAP(CDataViewView, CScrollView)
	//{{AFX_MSG_MAP(CDataViewView)
	ON_WM_CREATE()
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	//}}AFX_MSG_MAP
	// Standard printing commands
//	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataViewView construction/destruction

CDataViewView::CDataViewView()
{
	// TODO: add construction code here
	m_pOldFrame=NULL;
	m_pCDialog=NULL;
	m_uStartX=m_uEndY=2;
	m_nOldDepth=0;
	m_nHeaderHeight=m_nFooterHeight=2;
	m_nGap=1;
	m_iSelectGrid=0;
}

CDataViewView::~CDataViewView()
{
}

BOOL CDataViewView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CScrollView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CDataViewView drawing

void CDataViewView::OnDraw(CDC* pDC)
{
	CDataViewDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here

	int OldCDC=pDC->SaveDC();
	pDC->SelectObject (&m_screenFont);
	pDC->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
  pDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
	for(int i=0;i<pDoc->m_Data.GetSize();i++){
		CDataViewData *pData=(CDataViewData*)pDoc->m_Data[i];
		if(pData->pTL)
			pData->pTL->OnDraw(pData->rect,pDC);
	}
	pDC->RestoreDC(OldCDC);
}
/*
void CDataViewView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = sizeTotal.cy = 1000;
	SetScrollSizes(MM_TEXT, sizeTotal);
}
*/
/////////////////////////////////////////////////////////////////////////////
// CDataViewView printing

BOOL CDataViewView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	BOOL fl;
//	pInfo->m_pPD->m_pd.nCopies=2;
	fl=DoPreparePrinting(pInfo);
//	int num_copies=pInfo->m_pPD->m_pd.nCopies;//>GetCopies();
	return fl;
}

void CDataViewView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add extra initialization before printing
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	CDataViewDoc *pDoc=GetDocument();

	  ASSERT(pDC && pInfo);
    if (!pDC || !pInfo) return;

    // Get a DC for the current window (will be a screen DC for print previewing)
    CDC *pCurrentDC = GetDC();        // will have dimensions of the client area
    if (!pCurrentDC) return;

    CSize PaperPixelsPerInch(pDC->GetDeviceCaps(LOGPIXELSX), pDC->GetDeviceCaps(LOGPIXELSY));
    CSize ScreenPixelsPerInch(pCurrentDC->GetDeviceCaps(LOGPIXELSX), pCurrentDC->GetDeviceCaps(LOGPIXELSY));
		
		double hr_to_mm=(double)(pDC->GetDeviceCaps (HORZRES))/pDC->GetDeviceCaps (HORZSIZE);
		double vr_to_mm=(double)(pDC->GetDeviceCaps (VERTRES))/pDC->GetDeviceCaps (VERTSIZE);
		m_iLeftMargin=(int)(pApp->Margin.left*hr_to_mm/100);
		m_iRightMargin=(int)(pApp->Margin.right*hr_to_mm/100);
		m_iTopMargin=(int)(pApp->Margin.top*vr_to_mm/100);
		m_iBottomMargin=(int)(pApp->Margin.bottom*vr_to_mm/100);
    // Create the printer font
    int nFontSize = -10;
    CString strFontName = _T("MS Sans Serif");
    m_PrinterFont.CreateFont(nFontSize, 0,0,0, FW_NORMAL, 0,0,0, DEFAULT_CHARSET,
                             OUT_CHARACTER_PRECIS, CLIP_CHARACTER_PRECIS, DEFAULT_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, strFontName);

    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    // Get the average character width (in GridCtrl units) and hence the margins
		CSize m_CharSize = pDC->GetTextExtent(_T("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSATUVWXYZ"),52);
    m_PrCharHeight=m_CharSize.cy;
    int xMargins = m_iLeftMargin+m_iRightMargin;

    // Get the page sizes (physical and logical)
    m_PaperSize = CSize(pDC->GetDeviceCaps(HORZRES), pDC->GetDeviceCaps(VERTRES));

    if(! pApp->m_bFitPage)	{
			m_LogicalPageSize.cx = ScreenPixelsPerInch.cx * m_PaperSize.cx / PaperPixelsPerInch.cx;
      m_LogicalPageSize.cy = ScreenPixelsPerInch.cy * m_PaperSize.cy / PaperPixelsPerInch.cy;
    }else{
			int MaxWigth=100;
			for(int i=0;i<pDoc->m_Data.GetSize();i++){
				int m_y;
				CDataViewData *pData=(CDataViewData*)pDoc->m_Data[i];
				if(pData->pGrid){
					m_y=pData->pGrid->GetVirtualWidth();
				}else if(pData->pTL){
					m_y=pData->rect.left+pData->pTL->GetWidth();
				}
				if(m_y>MaxWigth)
					MaxWigth=m_y;
			}
      m_LogicalPageSize.cx = MaxWigth*m_PaperSize.cx/(m_PaperSize.cx-xMargins);
      m_LogicalPageSize.cy = MulDiv(m_LogicalPageSize.cx, m_PaperSize.cy, m_PaperSize.cx);
    }


		double koefy=(double)m_LogicalPageSize.cy/(double)m_PaperSize.cy;
		double koefx=(double)m_LogicalPageSize.cx/(double)m_PaperSize.cx;
		m_iLeftMargin=(int)(koefx*m_iLeftMargin);
		m_iRightMargin=(int)(koefx*m_iRightMargin);
		m_iTopMargin=(int)(koefy*m_iTopMargin);
		m_iBottomMargin=(int)(koefy*m_iBottomMargin);
//		m_PrCharHeight=(int)(koefy*m_PrCharHeight);
		xMargins = m_iLeftMargin+m_iRightMargin;


    m_nPageHeight = m_LogicalPageSize.cy - m_iTopMargin - m_iBottomMargin -
                        (m_nHeaderHeight+m_nFooterHeight + 2*m_nGap)*m_PrCharHeight;
		m_nPageWidth=m_LogicalPageSize.cx - xMargins;
    // Get the number of pages. Assumes no row is bigger than the page size.
    int nTotalRowHeight = 0;
    int m_nNumPages = 1;
		int m_nMultiplePagesXY=0;
		int m_nOldMultiplePagesX=0;
		int m_nOldI=-1;
		m_Pages.RemoveAll();
		m_Pages.Add(0);
		int i;
		for( i=0;i<pDoc->m_Data.GetSize();i++){
			CDataViewData *pData=(CDataViewData*)pDoc->m_Data[i];
			pData->y=0;
			if(pData->pGrid){
				if(m_nOldI<0){
					m_nOldI=i;
				}
				BOOL fl_NewPage=false;
				nTotalRowHeight+=pData->pGrid->GetFixedRowHeight();
				bool fl_new_pg=false;
				ASSERT(pData->pGrid->GetRowCount()>pData->pGrid->GetFixedRowCount());//
				//не се печатат таблици само с глави т.е. без данни !!!!!!!!!!
				if(pData->pGrid->GetRowCount()>pData->pGrid->GetFixedRowCount())
					if((nTotalRowHeight+pData->pGrid->GetRowHeight(pData->pGrid->GetFixedRowCount()))>m_nPageHeight)
						fl_new_pg=true;
				if(fl_new_pg||pData->NewPage){
					m_nNumPages+=m_nMultiplePagesXY; 
					for(int j=0;j<m_nMultiplePagesXY;j++)
						m_Pages.Add(m_nOldI);
					m_nMultiplePagesXY=0;
					m_Pages.Add(i);
					m_nNumPages++;
					m_nOldI=i;
					m_nOldMultiplePagesX=0;
					nTotalRowHeight=pData->pGrid->GetFixedRowHeight();
				}else if(nTotalRowHeight>m_nPageHeight){
					m_nNumPages+=m_nMultiplePagesXY;
					for(int j=0;j<m_nMultiplePagesXY;j++)
						m_Pages.Add(m_nOldI);
					m_nMultiplePagesXY=0;
					m_Pages.Add(i);
					m_nNumPages++;
					m_nOldI=i;
					m_nOldMultiplePagesX=0;
					nTotalRowHeight=pData->pGrid->GetFixedRowHeight();
				}

				int iColumnOffset = 0;
				int i1;
				for( i1=0; i1 < pData->pGrid->GetFixedColumnCount(); i1++){
					iColumnOffset += pData->pGrid->GetColumnWidth( i1);
				}
				pData->pGrid->m_nPageWidth = m_nPageWidth - iColumnOffset;
				pData->pGrid->m_nPageMultiplier = 1;

				if( !pApp->m_bFitPage){
					int iTotalRowWidth = 0;
					for( i1 = pData->pGrid->GetFixedColumnCount(); i1 < pData->pGrid->GetColumnCount(); i1++){
            iTotalRowWidth += pData->pGrid->GetColumnWidth( i1);
            if( iTotalRowWidth > pData->pGrid->m_nPageWidth){
                pData->pGrid->m_nPageMultiplier++;
                iTotalRowWidth = pData->pGrid->GetColumnWidth( i1);
            }
					}
				}
				int m_nMultiplePages=pData->pGrid->m_nPageMultiplier;
				if(pData->pGrid->m_nPageMultiplier>m_nOldMultiplePagesX)
					m_nOldMultiplePagesX=pData->pGrid->m_nPageMultiplier;
				m_nMultiplePagesXY=m_nOldMultiplePagesX-1;
				pData->m_nCurPage=m_nNumPages;
				for (int row = pData->pGrid->GetFixedRowCount(); row < pData->pGrid->GetRowCount(); row++){
					nTotalRowHeight += pData->pGrid->GetRowHeight(row);
					if (nTotalRowHeight > m_nPageHeight){
						m_nNumPages+=m_nMultiplePagesXY;
						for(int j=0;j<m_nMultiplePagesXY;j++)
							m_Pages.Add(m_nOldI);
						m_nMultiplePagesXY=0;
						m_Pages.Add(i);
						m_nNumPages++;
						m_nOldI=i;
						m_nOldMultiplePagesX=pData->pGrid->m_nPageMultiplier;
						m_nMultiplePagesXY=m_nOldMultiplePagesX-1;
						
						nTotalRowHeight = pData->pGrid->GetFixedRowHeight()+pData->pGrid->GetRowHeight(row);
					}
				}
			}else if(pData->pTL){
				if(pData->NewPage){
					m_nNumPages+=m_nMultiplePagesXY;
					for(int j=0;j<m_nMultiplePagesXY;j++)
						m_Pages.Add(m_nOldI);
					m_nMultiplePagesXY=0;
					m_Pages.Add(i);
					m_nNumPages++;
					m_nOldMultiplePagesX=0;
					nTotalRowHeight=pData->pTL->height;
				}else{
					nTotalRowHeight+=pData->pTL->height;
					if (nTotalRowHeight > m_nPageHeight) {
						m_nNumPages+=m_nMultiplePagesXY;
						for(int j=0;j<m_nMultiplePagesXY;j++)
							m_Pages.Add(m_nOldI);
						m_nMultiplePagesXY=0;
						m_Pages.Add(i);
            m_nNumPages++;
						m_nOldMultiplePagesX=0;
						nTotalRowHeight=pData->pTL->height;
					}
				}
			}
		}

    // Set up the print info
		pInfo->SetMaxPage(m_nNumPages+m_nMultiplePagesXY);
		int j;
		for( j=0;j<m_nMultiplePagesXY;j++)
			m_Pages.Add(m_nOldI);
    pInfo->m_nCurPage = 1;                        // start printing at page# 1

		CUIntArray m_PagesMaxX;
		m_PagesMaxX.RemoveAll();
		int maxX=0,oldPage=-1;
		for(i=0;i<pDoc->m_Data.GetSize();i++){
			CDataViewData *pData=(CDataViewData*)pDoc->m_Data[i];
			if(pData->pGrid){
				if((pData->m_nCurPage!=oldPage)&&(oldPage!=-1)){
					m_PagesMaxX.Add(maxX);
					maxX=pData->pGrid->m_nPageMultiplier;
				}else{
					if(maxX<pData->pGrid->m_nPageMultiplier)
						maxX=pData->pGrid->m_nPageMultiplier;
				}
				oldPage=pData->m_nCurPage;
			}
		}
		{
			m_PagesMaxX.Add(maxX);
			j=0;
			oldPage=-1;
			for(i=0;i<pDoc->m_Data.GetSize();i++){
				CDataViewData *pData=(CDataViewData*)pDoc->m_Data[i];
				if(pData->pGrid){
					if((pData->m_nCurPage!=oldPage)&&(oldPage!=-1))
						j++;
					oldPage=pData->m_nCurPage;
					pData->m_nOldMultiplePagesX=m_PagesMaxX[j];
				}
			}
			m_PagesMaxX.RemoveAll();
		}
		ReleaseDC(pCurrentDC);
    pDC->SelectObject(pOldFont);
}

void CDataViewView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: add cleanup after printing
	m_Pages.RemoveAll();
	m_PrinterFont.DeleteObject ();
}

/////////////////////////////////////////////////////////////////////////////
// CDataViewView diagnostics

#ifdef _DEBUG
void CDataViewView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CDataViewView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

CDataViewDoc* CDataViewView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDataViewDoc)));
	return (CDataViewDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDataViewView message handlers

void CDataViewView::OnFilePrintPreview()
{
	CView::OnFilePrintPreview();
}

void CDataViewView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	// TODO: Add your specialized code here and/or call the base class
	  if (!pDC || !pInfo)
        return;
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		ASSERT_VALID(pApp);
		BOOL fl_PrLine=true;
		CDataViewDoc *pDoc=GetDocument();
    //CRect rcPage(pInfo->m_rectDraw);
    CFont *pOldFont = pDC->SelectObject(&m_PrinterFont);

    // Set the page map mode to use GridCtrl units, and setup margin
    pDC->SetMapMode(MM_ANISOTROPIC);
    pDC->SetWindowExt(m_LogicalPageSize);
    pDC->SetViewportExt(m_PaperSize);

    pDC->SetWindowOrg(-m_iLeftMargin,- m_iTopMargin);

    // Header
    pInfo->m_rectDraw.top    = 0;
    pInfo->m_rectDraw.left   = 0;
    pInfo->m_rectDraw.right  = m_nPageWidth;
	  pInfo->m_rectDraw.bottom = m_nHeaderHeight*m_PrCharHeight;
    PrintHeader(pDC, pInfo);
    pDC->OffsetWindowOrg(0, -m_nHeaderHeight * m_PrCharHeight);

		pInfo->m_rectDraw.bottom= m_nPageHeight;

    // Gap between header and column headings
    pDC->OffsetWindowOrg(0, -m_nGap * m_PrCharHeight);
    
		if(pInfo->m_nCurPage<=(unsigned int)m_Pages.GetSize()){
			for(int m_iData=m_Pages[pInfo->m_nCurPage-1];m_iData<pDoc->m_Data.GetSize();m_iData++){
				CDataViewData *pData=(CDataViewData*)pDoc->m_Data[m_iData];
				if(pData->NewPage&&pInfo->m_rectDraw.top)
					break;
				if(pData->pTL){
					CRect rect;
					rect.left=pData->rect.left;
					rect.right=min(pData->rect.right,m_nPageWidth);
					rect.top=pInfo->m_rectDraw.top;
					pInfo->m_rectDraw.top+=pData->rect.bottom-pData->rect.top;
					rect.bottom=pInfo->m_rectDraw.top;
					if(pInfo->m_rectDraw.top>pInfo->m_rectDraw.bottom)
						break;
					if(fl_PrLine)
						pData->pTL->OnDraw(rect,pDC);
				}else if(pData->pGrid){
					if(pData->m_nCurPage>(int)pInfo->m_nCurPage)
						break;
					if(pInfo->m_rectDraw.top&&(pData->y==0))
						pData->y=pInfo->m_rectDraw.top;
					pInfo->m_rectDraw.top=pData->y;
					
					pData->pGrid->m_bPrintBlackWhite=pApp->m_bPrintBlackWhite;
						
					// We need to find out which row to start printing for this page.
					int nTotalRowHeight = pInfo->m_rectDraw.top;
					UINT nNumPages = 1;
					pData->pGrid->m_nCurrPrintRow = pData->pGrid->GetFixedRowCount();
					
					
					// Not only the row, but we need to figure out column, too
					
					// Can print 4 pages, where page 1 and 2 represent the same rows but
					// with different WIDE columns.
					//
					// .......
					// .1 .2 .  If representing page 3  -->    iPageIfIgnoredWideCols = 2
					// .......                                 iWideColPageOffset = 0
					// .3 .4 .  If representing page 2  -->    iPageIfIgnoredWideCols = 1
					// .......                                 iWideColPageOffset = 1

					BOOL fl_noPrintGrid=false;
					int m_nCurPage=pData->m_nCurPage;
					if(pData->m_nOldMultiplePagesX>pData->pGrid->m_nPageMultiplier){
		/*				int a=(pInfo->m_nCurPage-m_nCurPage+1) / pData->pGrid->m_nPageMultiplier;
						int b = (pInfo->m_nCurPage-m_nCurPage+1) - ( a *pData->pGrid->m_nPageMultiplier);
						if( b > 0)
							a++;*/
						int a=(pInfo->m_nCurPage-m_nCurPage+1);
						if(a>pData->pGrid->m_nPageMultiplier){
							if(a<=pData->m_nOldMultiplePagesX){
								fl_noPrintGrid=true;
							}else{
								m_nCurPage+=(pData->m_nOldMultiplePagesX-pData->pGrid->m_nPageMultiplier);
							}
						}
					}
					if(!fl_noPrintGrid){
						if((pInfo->m_rectDraw.top+pData->pGrid->GetFixedRowHeight())>pInfo->m_rectDraw.bottom){
							pData->y=0;
							break;
						}
					}
					int iPageIfIgnoredWideCols = (pInfo->m_nCurPage-m_nCurPage+1) / pData->pGrid->m_nPageMultiplier;
					int iWideColPageOffset = (pInfo->m_nCurPage-m_nCurPage+1) - ( iPageIfIgnoredWideCols *pData->pGrid->m_nPageMultiplier);
					if( iWideColPageOffset > 0)
						iPageIfIgnoredWideCols++;
					
					if( iWideColPageOffset == 0)
						iWideColPageOffset = pData->pGrid->m_nPageMultiplier;
					iWideColPageOffset--;
					
					// calculate current print row based on iPageIfIgnoredWideCols
					while(  pData->pGrid->m_nCurrPrintRow < pData->pGrid->GetRowCount()
						&& (int)nNumPages < iPageIfIgnoredWideCols){
						nTotalRowHeight += pData->pGrid->GetRowHeight(pData->pGrid->m_nCurrPrintRow);
						if (nTotalRowHeight > (m_nPageHeight-pData->pGrid->GetFixedRowHeight())) {
							nNumPages++;
							if ((int)nNumPages == iPageIfIgnoredWideCols) 
								break;
							nTotalRowHeight = pData->pGrid->GetRowHeight(pData->pGrid->m_nCurrPrintRow);
						}
						pData->pGrid->m_nCurrPrintRow++;
					}
					
					pData->pGrid->m_nPrintColumn = pData->pGrid->GetFixedColumnCount();
					int iTotalRowWidth = 0;
					int i1, i2;
					
					
					// now, calculate which print column to start displaying
					for( i1 = 0; i1 < iWideColPageOffset; i1++){
						for( i2 = pData->pGrid->m_nPrintColumn; i2 < pData->pGrid->GetColumnCount(); i2++)						{
							iTotalRowWidth += pData->pGrid->GetColumnWidth( i2);
							if( iTotalRowWidth > pData->pGrid->m_nPageWidth){
                pData->pGrid->m_nPrintColumn = i2;
                iTotalRowWidth = 0;
                break;
							}
						}
					}
					if(pData->pGrid->m_nCurrPrintRow>=pData->pGrid->GetRowCount())
						fl_noPrintGrid=true;
					if(!fl_noPrintGrid&&pData->pGrid->m_nCurrPrintRow>pData->pGrid->GetFixedRowCount())
						pInfo->m_rectDraw.top=0;
					if(pData->pGrid->m_nPrintColumn>pData->pGrid->GetFixedColumnCount())
						fl_PrLine=false;
			/*		int x=pData->pGrid->m_nPrintColumn;
					int y=pData->pGrid->m_nCurrPrintRow;*/
					if(!fl_noPrintGrid){
						if(pData->pGrid->Print( pDC, pInfo)){   // print Grid
							break;
						}
					}else
						fl_PrLine=false;
					
					pInfo->m_rectDraw.left=0;
				}
			}
		}
    // Footer
		pInfo->m_rectDraw.left=0;
		pInfo->m_rectDraw.top=0;
    pInfo->m_rectDraw.bottom = m_nFooterHeight * m_PrCharHeight;
    pDC->SetWindowOrg( -m_iLeftMargin,
        -m_LogicalPageSize.cy + m_nFooterHeight * m_PrCharHeight+m_iBottomMargin);
    PrintFooter(pDC, pInfo);
    // SetWindowOrg back for next page
    pDC->SetWindowOrg(0,0);
    pDC->SelectObject(pOldFont);
}

void CDataViewView::OnEndPrintPreview(CDC* pDC, CPrintInfo* pInfo, POINT point, CPreviewView* pView) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	
	CView::OnEndPrintPreview(pDC, pInfo, point, pView);
		
	UINT h=LOWORD(GetCurrentMessage()->wParam);
	if(h!=AFX_ID_PREVIEW_PRINT){
		// Show the original frame
		if(m_pOldFrame){
			m_pOldFrame->ShowWindow(SW_SHOW);
			// Restore main frame pointer
			AfxGetApp()->m_pMainWnd=m_pOldFrame;
		}
		if(m_pCDialog)
			m_pCDialog->ShowWindow(SW_SHOW);
		// Kill parent frame and itself
		if(m_pOldFrame){
			GetDocument()->FreeData(false);
			GetParentFrame()->DestroyWindow();	
		}
	}
}

BOOL CDataViewView::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	CDataViewDoc *pDoc=GetDocument();
	if(wParam&&((int)wParam<=pDoc->m_Data.GetSize())){
		CDataViewData *pData=(CDataViewData*)pDoc->m_Data[wParam-1];
		if(pData->pGrid){
			*pResult = 1;
			GV_DISPINFO *pDispInfo = (GV_DISPINFO*)lParam;
			switch(pDispInfo->hdr.code){
				case GVN_SELCHANGED:
					m_iSelectGrid=wParam;
					break;
				case GVN_COLUMNCLICK:
					CRect rect;
					m_uEndY=pData->rect.top;
					int i;
					for( i=wParam-1;i<pDoc->m_Data.GetSize();i++){
						pData=(CDataViewData*)pDoc->m_Data[i];
						rect.left=m_uStartX;
						rect.top=m_uEndY;
						if(pData->pGrid){
							rect.right=rect.left+pData->pGrid->GetVirtualWidth()+2*GetSystemMetrics(SM_CXEDGE);
							m_uEndY=rect.bottom=rect.top+pData->pGrid->GetVirtualHeight()+2*GetSystemMetrics(SM_CYEDGE);
							CRect r=rect;
							r.OffsetRect(-GetScrollPos(SB_HORZ),-GetScrollPos(SB_VERT));
							pData->pGrid->MoveWindow(r,false);
						}else if(pData->pTL){
							rect.right=rect.left+pData->pTL->GetWidth();
							m_uEndY=rect.bottom=rect.top+pData->pTL->height;
						}
						pData->rect=rect;
					}
					for(i=m_nOldDepth=0;i<pDoc->m_Data.GetSize();i++){
						pData=(CDataViewData*)pDoc->m_Data[i];
						if(pData->rect.right>m_nOldDepth)
							m_nOldDepth=pData->rect.right;
					}
					SetScrollSizes (MM_TEXT, CSize (m_nOldDepth,m_uEndY),
						CSize (m_cxScreen * 50, m_cyScreen * 10), CSize (m_cxScreen*5, m_cyScreen));
					Invalidate();
					pDoc->UpdateAllViews(this);
					break;
			}
			return true;
		}
	}
	return CScrollView::OnNotify(wParam, lParam, pResult);
}

void CDataViewView::AddGridData(CDataViewData *pData)
{
	CRect rect;
	rect.left=m_uStartX;
	rect.top=m_uEndY;
	rect.right=rect.left+pData->pGrid->GetVirtualWidth()+2*GetSystemMetrics(SM_CXEDGE);
	m_uEndY=rect.bottom=rect.top+pData->pGrid->GetVirtualHeight()+2*GetSystemMetrics(SM_CYEDGE);
	if(m_pOldFrame==NULL)
		pData->pGrid->Create(rect, this,	pData->DlgId ,WS_CHILD | WS_TABSTOP | WS_VISIBLE,false);
	if(rect.right>m_nOldDepth)
		m_nOldDepth=rect.right;
	pData->rect=rect;
	SetScrollSizes (MM_TEXT, CSize (m_nOldDepth,m_uEndY),
		CSize (m_cxScreen * 50, m_cyScreen * 10), CSize (m_cxScreen*5, m_cyScreen));
	pData->pGrid->RedrawWindow();
}

int CDataViewView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CScrollView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	  CClientDC dc (this);
		dc.SetMapMode(MM_TEXT);
   
		NONCLIENTMETRICS ncm;
    ncm.cbSize = sizeof(NONCLIENTMETRICS);
    VERIFY(SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0));
    m_screenFont.CreateFontIndirect(&(ncm.lfMessageFont));

    TEXTMETRIC tm;
    dc.SelectObject (&m_screenFont);
    dc.GetTextMetrics (&tm);
		LOGPEN ps;
		dc.GetCurrentPen()->GetLogPen(&ps);
		LONG yPen=ps.lopnWidth.y+PEN_OFSET;
		
    m_cyScreen = tm.tmHeight + tm.tmExternalLeading+2*yPen;
		m_cxScreen = tm.tmMaxCharWidth+1;
    
	return 0;
}

void CDataViewView::AddTextLine(CDataViewData *pData)
{
	CRect rect;
	rect.left=m_uStartX;
	rect.top=m_uEndY;
	rect.right=rect.left+pData->pTL->GetWidth();
	m_uEndY=rect.bottom=rect.top+(pData->pTL->height=m_cyScreen);
	if(rect.right>m_nOldDepth)
		m_nOldDepth=rect.right;
	pData->rect=rect;
	SetScrollSizes (MM_TEXT, CSize (m_nOldDepth,m_uEndY),
		CSize (m_cxScreen * 50, m_cyScreen * 10), CSize (m_cxScreen*5, m_cyScreen));
}

void CDataViewView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	int wScrollNotify=-1;
	UINT uMessage;
	switch (nChar){
    case VK_UP:
      wScrollNotify = SB_LINEUP;
      uMessage = WM_VSCROLL;
      break;
    case VK_PRIOR:    //PAGEUP key
      wScrollNotify = SB_PAGEUP;
      uMessage = WM_VSCROLL;
      break;
    case VK_NEXT:     // PAGEDOWN key
      wScrollNotify = SB_PAGEDOWN;
      uMessage = WM_VSCROLL;
      break;
    case VK_DOWN:
      wScrollNotify = SB_LINEDOWN;
      uMessage = WM_VSCROLL;
      break;
   case VK_HOME:
      wScrollNotify = SB_TOP;//SB_PAGELEFT;
      uMessage = WM_VSCROLL;//WM_HSCROLL;
      break;
    case VK_END:
      wScrollNotify = SB_BOTTOM;//SB_PAGERIGHT;
      uMessage = WM_VSCROLL;//WM_HSCROLL;
			break;
    case VK_RIGHT:
      wScrollNotify = SB_LINEDOWN;
      uMessage = WM_HSCROLL;
      break;
    case VK_LEFT:
      wScrollNotify = SB_LINEUP;
      uMessage = WM_HSCROLL;
      break;
	}
  if (wScrollNotify != -1)
		SendMessage( uMessage, MAKELONG(wScrollNotify, 0), 0L);

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);	
}

void CDataViewView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pSender!=this){
		CDataViewDoc *pDoc=GetDocument();
		m_uStartX=m_uEndY=2;
		m_nOldDepth=0;
		for(int i=0;i<pDoc->m_Data.GetSize();i++){
			CDataViewData *pData=(CDataViewData*)pDoc->m_Data[i];

			CRect rect;
			rect.right=rect.left=m_uStartX;
			rect.top=m_uEndY;

			if(pData->pTL){
				rect.right=rect.left+pData->pTL->GetWidth();
				m_uEndY=rect.bottom=rect.top+(pData->pTL->height=m_cyScreen);
			}else if(pData->pGrid){
				rect.right=rect.left+pData->pGrid->GetVirtualWidth()+2*GetSystemMetrics(SM_CXEDGE);
				m_uEndY=rect.bottom=rect.top+pData->pGrid->GetVirtualHeight()+2*GetSystemMetrics(SM_CYEDGE);
			}else{
				;
				ASSERT(false);
			}
			pData->rect=rect;
			if(rect.right>m_nOldDepth)
					m_nOldDepth=rect.right;
		}
		SetScrollSizes (MM_TEXT, CSize (m_nOldDepth,m_uEndY),
			CSize (m_cxScreen * 50, m_cyScreen * 10), CSize (m_cxScreen*5, m_cyScreen));
	}
	Invalidate();
}
/*
void CDataViewView::OnFilePrint() 
{
	// TODO: Add your command handler code here

    CDC dc;

    
    CPrintDialog printDlg(FALSE);
    if (printDlg.DoModal() != IDOK)             // Get printer settings from user
      return;
		dc.Attach(printDlg.GetPrinterDC());         // attach a printer DC
    
    dc.m_bPrinting = TRUE;

    CString strTitle;
    strTitle.LoadString(AFX_IDS_APP_TITLE);

    if( strTitle.IsEmpty() )
    {
        CWnd *pParentWnd = GetParent();
        while (pParentWnd)
        {
            pParentWnd->GetWindowText(strTitle);
            if (strTitle.GetLength())  // can happen if it is a CView, CChildFrm has the title
                break;
            pParentWnd = pParentWnd->GetParent();
        }
    }

    DOCINFO di;                                 // Initialise print doc details
    memset(&di, 0, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;

    BOOL bPrintingOK = dc.StartDoc(&di);        // Begin a new print job

    CPrintInfo Info;
    Info.m_rectDraw.SetRect(0,0, dc.GetDeviceCaps(HORZRES), dc.GetDeviceCaps(VERTRES));

    OnBeginPrinting(&dc, &Info);                // Initialise printing
    for (UINT page = Info.GetMinPage(); page <= Info.GetMaxPage() && bPrintingOK; page++)
    {
        dc.StartPage();                         // begin new page
        Info.m_nCurPage = page;
        OnPrint(&dc, &Info);                    // Print page
        bPrintingOK = (dc.EndPage() > 0);       // end page
    }
    OnEndPrinting(&dc, &Info);                  // Clean up after printing

    if (bPrintingOK)
        dc.EndDoc();                            // end a print job
    else
        dc.AbortDoc();                          // abort job.

    dc.Detach();                                // detach the printer DC
}
*/

void CDataViewView::OnFilePrint() 
{
	// TODO: Add your command handler code here
	 CScrollView::OnFilePrint();

	 	// Show the original frame
		if(m_pOldFrame){
			m_pOldFrame->ShowWindow(SW_SHOW);
			// Restore main frame pointer
			AfxGetApp()->m_pMainWnd=m_pOldFrame;
		}
		if(m_pCDialog)
			m_pCDialog->ShowWindow(SW_SHOW);
		// Kill parent frame and itself
		if(m_pOldFrame){
			GetDocument()->FreeData(false);
			GetParentFrame()->DestroyWindow();	
		}
}

void CDataViewView::PrintHeader(CDC *pDC, CPrintInfo *pInfo)
{
    // print App title on top right margin
    CString strRight;
    strRight.LoadString(AFX_IDS_APP_TITLE);

    // print parent window title in the centre (Gert Rijs)
    CString strLeft;
    CWnd *pParentWnd = GetParent();
    while (pParentWnd)
    {
        pParentWnd->GetWindowText(strLeft);
        if (strLeft.GetLength())  // can happen if it is a CView, CChildFrm has the title
            break;
        pParentWnd = pParentWnd->GetParent();
    }

    CFont   BoldFont;
    LOGFONT lf;

    //create bold font for header and footer
    VERIFY(m_PrinterFont.GetLogFont(&lf));
    lf.lfWeight = FW_BOLD;
    VERIFY(BoldFont.CreateFontIndirect(&lf));

    CFont *pNormalFont = pDC->SelectObject(&BoldFont);
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

    CRect   rc(pInfo->m_rectDraw);
    if( !strLeft.IsEmpty() )
        pDC->DrawText( strLeft, &rc, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
    if( !strRight.IsEmpty() )
        pDC->DrawText( strRight, &rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

    pDC->SetBkMode(nPrevBkMode);
    pDC->SelectObject(pNormalFont);
    BoldFont.DeleteObject();

    // draw ruled-line across top
    pDC->SelectStockObject(BLACK_PEN);
    pDC->MoveTo(rc.left, rc.bottom);
    pDC->LineTo(rc.right, rc.bottom);
}

void CDataViewView::PrintFooter(CDC *pDC, CPrintInfo *pInfo)
{
    // page numbering on left
    CString strLeft;
    strLeft.Format(IDS_PAGE_OF_PAGE, pInfo->m_nCurPage, pInfo->GetMaxPage() );

    // date and time on the right
    CString strRight;
    COleDateTime t = COleDateTime::GetCurrentTime();
    strRight = t.Format(_T("%c"));
    
    CRect rc(pInfo->m_rectDraw);

    // draw ruled line on bottom
    pDC->SelectStockObject(BLACK_PEN);
    pDC->MoveTo(rc.left, rc.top);
    pDC->LineTo(rc.right, rc.top);

    CFont BoldFont;
    LOGFONT lf;

    //create bold font for header and footer
    m_PrinterFont.GetLogFont(&lf);
    lf.lfWeight = FW_BOLD;
    BoldFont.CreateFontIndirect(&lf);

    CFont *pNormalFont = pDC->SelectObject(&BoldFont);
    int nPrevBkMode = pDC->SetBkMode(TRANSPARENT);

    // EFW - Bug fix - Force text color to black.  It doesn't always
    // get set to a printable color when it gets here.
    pDC->SetTextColor(RGB(0, 0, 0));

    if( !strLeft.IsEmpty() )
        pDC->DrawText( strLeft, &rc, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);
    if( !strRight.IsEmpty() )
        pDC->DrawText( strRight, &rc, DT_RIGHT | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER);

    pDC->SetBkMode(nPrevBkMode);
    pDC->SelectObject(pNormalFont);
    BoldFont.DeleteObject();
}

void CDataViewView::OnEditCopy() 
{
	// TODO: Add your command handler code here
	CDataViewDoc *pDoc=GetDocument();
	if(m_iSelectGrid&&((int)m_iSelectGrid<=pDoc->m_Data.GetSize())){
		CDataViewData *pData=(CDataViewData*)pDoc->m_Data[m_iSelectGrid-1];
		if(pData->pGrid)
			pData->pGrid->SendMessage(WM_COMMAND,ID_EDIT_COPY,0);
	}
}

void CDataViewView::OnEditCut() 
{
	// TODO: Add your command handler code here
	CDataViewDoc *pDoc=GetDocument();
	if(m_iSelectGrid&&((int)m_iSelectGrid<=pDoc->m_Data.GetSize())){
		CDataViewData *pData=(CDataViewData*)pDoc->m_Data[m_iSelectGrid-1];
		if(pData->pGrid)
			pData->pGrid->SendMessage(WM_COMMAND,ID_EDIT_CUT,0);
	}
}

void CDataViewView::OnEditPaste() 
{
	// TODO: Add your command handler code here
	CDataViewDoc *pDoc=GetDocument();
	if(m_iSelectGrid&&((int)m_iSelectGrid<=pDoc->m_Data.GetSize())){
		CDataViewData *pData=(CDataViewData*)pDoc->m_Data[m_iSelectGrid-1];
		if(pData->pGrid)
			pData->pGrid->SendMessage(WM_COMMAND,ID_EDIT_PASTE,0);
	}
}

void CDataViewView::OnFileSaveAs() 
{
	// TODO: Add your command handler code here
	CDataViewDoc *pDoc=GetDocument();
	CString strFilter;
	strFilter.LoadString(IDS_FILE_SAVE_AS_TEXT);
	CString newName=pDoc->GetTitle();
	int iBad = newName.FindOneOf(_T("."));
	if (iBad != -1)
		newName.ReleaseBuffer(iBad);
	newName+=_T(".txt");

	CFileDialog aFileDialog( false, NULL, newName,
		    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_PATHMUSTEXIST,
							 strFilter);
	
	int nID = aFileDialog.DoModal();
	if(nID==IDOK){
		CStdioFile aFile;

		CFileException fx;
		if( !aFile.Open( aFileDialog.GetPathName(),CFile::modeCreate|CFile::modeWrite|CFile::typeText, &fx ) )
		{
			TCHAR buf[ 255 ];
			fx.GetErrorMessage( buf, 255 );
			CString strPrompt( buf );
			AfxMessageBox( strPrompt );
			return;
		}
		for(int i=0;i<pDoc->m_Data.GetSize();i++){
			CDataViewData * pData = (CDataViewData*)pDoc->m_Data[i];
			if(pData->pGrid)
				pData->pGrid->SaveInFile(&aFile,_T('\t'));
			else if(pData->pTL)
				pData->pTL->SaveInFile(&aFile,_T('\t'));
		}
		aFile.Close();
	}
}
