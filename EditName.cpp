// EditName.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "EditName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditName dialog


CEditName::CEditName(CWnd* pParent /*=NULL*/)
	: CDialog(CEditName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditName)
	m_cName = _T("");
	//}}AFX_DATA_INIT
}


void CEditName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditName)
	DDX_Text(pDX, IDC_EDIT_NAME, m_cName);
	DDV_MaxChars(pDX, m_cName, 29);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditName, CDialog)
	//{{AFX_MSG_MAP(CEditName)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditName message handlers
