// StringInTextLine.h: interface for the CStringInTextLine class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGINTEXTLINE_H__D93C222C_63DF_4405_8483_01DBF72A840A__INCLUDED_)
#define AFX_STRINGINTEXTLINE_H__D93C222C_63DF_4405_8483_01DBF72A840A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VARIANT_DATA_ACOCOUTS 1

class CStringInTextLine : public CObject  
{
	DECLARE_SERIAL(CStringInTextLine)
public:
	CStringInTextLine();
	virtual ~CStringInTextLine();
	CString text;
	LONG width;
	UINT align;
	BYTE line;
protected:
	virtual void Serialize(CArchive& ar);
};

#endif // !defined(AFX_STRINGINTEXTLINE_H__D93C222C_63DF_4405_8483_01DBF72A840A__INCLUDED_)
