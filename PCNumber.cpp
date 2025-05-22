// PCNumber.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "PCNumber.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPCNumber dialog


CPCNumber::CPCNumber(CWnd* pParent /*=NULL*/)
	: CDialog(CPCNumber::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPCNumber)
	m_iNumber = 0;
	//}}AFX_DATA_INIT
}


void CPCNumber::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPCNumber)
	DDX_Text(pDX, IDC_EDIT_NUMBER, m_iNumber);
	DDV_MinMaxInt(pDX, m_iNumber, 1, 3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPCNumber, CDialog)
	//{{AFX_MSG_MAP(CPCNumber)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPCNumber message handlers
