// TextLine.h: interface for the CTextLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTLINE_H__3E8B72A7_FFDD_4813_97F7_09E22F049BCD__INCLUDED_)
#define AFX_TEXTLINE_H__3E8B72A7_FFDD_4813_97F7_09E22F049BCD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StringInTextLine.h"

#define PEN_OFSET 1	//Pixel
#define L_L	0x01
#define L_R	0x02
#define L_T	0x04
#define L_B	0x08

class CTextLine : public CObject  
{
	DECLARE_SERIAL(CTextLine)
public:
	BOOL SaveInFile(CStdioFile *File, TCHAR chSeparator = _T(','));
	void AddText(CString str,LONG width,UINT align=TA_LEFT,BYTE line=0);
	void OnDraw(CRect rect,CDC* pDC);
	void FreeData(void);
	LONG GetWidth(void);
	CTextLine();
	virtual ~CTextLine();
	LONG height;
protected:
	virtual void Serialize(CArchive &ar);
	CObArray strings;
};

#endif // !defined(AFX_TEXTLINE_H__3E8B72A7_FFDD_4813_97F7_09E22F049BCD__INCLUDED_)
