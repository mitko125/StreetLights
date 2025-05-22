// PageSetup.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "PageSetup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPageSetup dialog


CPageSetup::CPageSetup(CWnd* pParent /*=NULL*/)
	: CDialog(CPageSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPageSetup)
	m_bFitPage = FALSE;
	m_uBottom = 0;
	m_uLeft = 0;
	m_uRight = 0;
	m_uTop = 0;
	m_bPrintBlackWhite = FALSE;
	//}}AFX_DATA_INIT
}


void CPageSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPageSetup)
	DDX_Check(pDX, IDC_CHECK_FIT_PAGE, m_bFitPage);
	DDX_Text(pDX, IDC_EDIT_BOTTOM, m_uBottom);
	DDV_MinMaxUInt(pDX, m_uBottom, 0, 100);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_uLeft);
	DDV_MinMaxUInt(pDX, m_uLeft, 0, 100);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_uRight);
	DDV_MinMaxUInt(pDX, m_uRight, 0, 100);
	DDX_Text(pDX, IDC_EDIT_TOP, m_uTop);
	DDV_MinMaxUInt(pDX, m_uTop, 0, 100);
	DDX_Check(pDX, IDC_BLACK_WITE, m_bPrintBlackWhite);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPageSetup, CDialog)
	//{{AFX_MSG_MAP(CPageSetup)
	ON_BN_CLICKED(IDC_CHECK_FIT_PAGE, OnCheckFitPage)
	ON_BN_CLICKED(IDC_BLACK_WITE, OnBlackWite)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPageSetup message handlers

void CPageSetup::OnCheckFitPage() 
{
	// TODO: Add your control notification handler code here
	CButton *pb=(CButton*) GetDlgItem(IDC_CHECK_FIT_PAGE);
	m_bFitPage=pb->GetCheck();
}

BOOL CPageSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton *pb=(CButton*) GetDlgItem(IDC_CHECK_FIT_PAGE);
	pb->SetCheck((m_bFitPage)?1:0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPageSetup::OnBlackWite() 
{
	// TODO: Add your control notification handler code here
	CButton *pb=(CButton*) GetDlgItem(IDC_BLACK_WITE);
	m_bPrintBlackWhite=pb->GetCheck();
}
