// NewOperator.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "NewOperator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewOperator dialog


CNewOperator::CNewOperator(CWnd* pParent /*=NULL*/)
	: CDialog(CNewOperator::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNewOperator)
	m_Name = _T("");
	m_Password = _T("");
	m_ConfirmPassword = _T("");
	//}}AFX_DATA_INIT
}


void CNewOperator::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewOperator)
	DDX_Text(pDX, IDC_EDIT_NAME, m_Name);
	DDV_MaxChars(pDX, m_Name, 24);
	DDX_Text(pDX, IDC_EDIT_PASSORD, m_Password);
	DDV_MaxChars(pDX, m_Password, 23);
	DDX_Text(pDX, IDC_EDIT_CONFIRM_PASSWORD, m_ConfirmPassword);
	DDV_MaxChars(pDX, m_ConfirmPassword, 23);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewOperator, CDialog)
	//{{AFX_MSG_MAP(CNewOperator)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewOperator message handlers

void CNewOperator::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	if(m_ConfirmPassword!=m_Password){
		AfxMessageBox(IDS_NOT_CONFIRM_PASSWORD);
		GetDlgItem(IDC_EDIT_CONFIRM_PASSWORD)->SetFocus();
		return;
	}
	if(m_Password.IsEmpty()){
		AfxMessageBox(IDS_NOT_PASSWORD_ADD);
		GetDlgItem(IDC_EDIT_PASSORD)->SetFocus();
		return;
	}
	if(m_Name.IsEmpty()){
		AfxMessageBox(IDS_NOT_NAME);
		GetDlgItem(IDC_EDIT_NAME)->SetFocus();
		return;
	}
	CDialog::OnOK();
}
