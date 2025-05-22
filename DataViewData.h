// DataViewData.h: interface for the CDataViewData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAVIEWDATA_H__03E0E0B0_1964_4182_A594_57C3137A2FDC__INCLUDED_)
#define AFX_DATAVIEWDATA_H__03E0E0B0_1964_4182_A594_57C3137A2FDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "TextLine.h"

class CGridCtrl;

class CDataViewData : public CObject  
{
	DECLARE_SERIAL(CDataViewData)
public:
	CDataViewData();
	virtual ~CDataViewData();

	CGridCtrl * pGrid;
	CTextLine * pTL;
	UINT DlgId;
	CRect rect;
	BOOL NewPage;
	long y;
	int					m_nCurPage;
	int m_nOldMultiplePagesX;
protected:
	virtual void Serialize(CArchive &ar);
};

#endif // !defined(AFX_DATAVIEWDATA_H__03E0E0B0_1964_4182_A594_57C3137A2FDC__INCLUDED_)
