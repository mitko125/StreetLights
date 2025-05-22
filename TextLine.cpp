// TextLine.cpp: implementation of the CTextLine class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "sapsan.h"
#include "TextLine.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CTextLine, CObject,VARIANT_DATA_ACOCOUTS|VERSIONABLE_SCHEMA)

CTextLine::CTextLine()
{
	height=0;
}

CTextLine::~CTextLine()
{

}

LONG CTextLine::GetWidth()
{
	LONG width=0;
	for(int i=0;i<strings.GetSize();i++){
		CStringInTextLine *pSITL=(CStringInTextLine*)strings[i];
		width+=pSITL->width;
	}
	return width;
}

void CTextLine::FreeData()
{
	for(int i=0;i<strings.GetSize();i++)
		delete strings[i];
	strings.RemoveAll();
}

void CTextLine::OnDraw(CRect rect,CDC *pDC)
{
	LOGPEN ps;
	LONG xPen,yPen;
	CRect rectExt,rectExtText;
	LONG x,y;

	pDC->GetCurrentPen()->GetLogPen(&ps);

	xPen=ps.lopnWidth.x+PEN_OFSET;
	yPen=ps.lopnWidth.y+PEN_OFSET;
	
	rectExt.bottom=height+(rectExt.top=rect.top);
	rectExt.left=rect.left;
	rectExt.right=rect.right;

	rectExtText.left=rect.left+xPen;
	rectExtText.right=rect.right;
	rectExtText.top=rect.top+yPen;
	rectExtText.bottom=rect.top+height-yPen;

	x=rect.left+xPen;
	y=rect.top+yPen;

	for(int i=0;i<strings.GetSize();i++){
		CStringInTextLine *pSITL=(CStringInTextLine*)strings[i];

		LONG m_x=rect.left=x;
		rect.top=y;
		rect.right=min(x+pSITL->width-2*xPen,rectExtText.right);
		rect.bottom=rectExtText.bottom;
		if((pSITL->align&TA_CENTER)==TA_CENTER){
			pDC->SetTextAlign(TA_CENTER);
			m_x+=(pSITL->width-2*xPen)/2;
		}else if((pSITL->align&TA_RIGHT)==TA_RIGHT){
			pDC->SetTextAlign(TA_RIGHT);
			m_x+=pSITL->width-2*xPen;
		}

		pDC->ExtTextOut(m_x,rect.top,ETO_CLIPPED,rect,pSITL->text,NULL);
		pDC->SetTextAlign(TA_LEFT);

		if(rectExt.left<=rectExt.right){
			pDC->MoveTo(rectExt.left,rectExt.top);
			LONG x=rectExt.left+pSITL->width;
			if(x>rectExt.right)
				x=rectExt.right;
			if(pSITL->line&L_T)
				pDC->LineTo(x,rectExt.top);
			else
				pDC->MoveTo(x,rectExt.top);
			if((rectExt.left+pSITL->width)>rectExt.right)
				pDC->MoveTo(x,rectExt.bottom);
			else
				if(pSITL->line&L_R)
					pDC->LineTo(x,rectExt.bottom);
				else
					pDC->MoveTo(x,rectExt.bottom);
			if(pSITL->line&L_B)
				pDC->LineTo(rectExt.left,rectExt.bottom);
			else
				pDC->MoveTo(rectExt.left,rectExt.bottom);
			if(pSITL->line&L_L)
				pDC->LineTo(rectExt.left,rectExt.top);
		}

		rectExt.left+=pSITL->width;
		rectExtText.left+=pSITL->width;
		x+=pSITL->width;
	}
}

void CTextLine::AddText(CString str, LONG width, UINT align,BYTE line)
{
	CStringInTextLine *pSITL=new CStringInTextLine;
	pSITL->text=str;
	pSITL->width=width;
	pSITL->align=align;
	pSITL->line=line;
	strings.Add(pSITL);
}

BOOL CTextLine::SaveInFile(CStdioFile *File, TCHAR chSeparator)
{
		CString strSeparator(chSeparator);

    TRY
    {
        for (int i = 0; i < strings.GetSize(); i++){
					CStringInTextLine *pSITL=(CStringInTextLine*)strings[i];
          File->WriteString(pSITL->text);
          File->WriteString(((i+1)==(strings.GetSize()))? _T("\n"): strSeparator);
        }

    }

    CATCH (CFileException, e)
    {
        AfxMessageBox(_T("Unable to save grid list"));
        return FALSE;
    }
    END_CATCH

		return TRUE;

}

void CTextLine::Serialize(CArchive &ar)
{
	CObject::Serialize( ar );
  // now do the stuff for our specific class
	strings.Serialize(ar);
  if( ar.IsStoring() ){
		ar<<height;
	}else{
		ar>>height;
	}
}
