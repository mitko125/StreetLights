// DisabledLamps.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "DisabledLamps.h"

#include "..\Free_Util_Dll\Util_Dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDisabledLamps dialog


CDisabledLamps::CDisabledLamps(UINT ids1 /*= 0*/,UINT ids2 /*= 0*/,CWnd* pParent /*=NULL*/)
	: CDialog(CDisabledLamps::IDD, pParent)
{
	id_name1 = ids1;
	id_name2 = ids2;
	//{{AFX_DATA_INIT(CDisabledLamps)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
};

void CDisabledLamps::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDisabledLamps)
	DDX_Control(pDX, IDC_EDIT1, m_Edit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDisabledLamps, CDialog)
	//{{AFX_MSG_MAP(CDisabledLamps)
	ON_BN_CLICKED(ID_SAVE_DATA, OnSaveData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDisabledLamps message handlers

BOOL CDisabledLamps::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if(id_name1){
		CString text;
		text.LoadString(id_name1);
		SetWindowText(text);
	}
	m_Edit.SetWindowText(text);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDisabledLamps::OnSaveData() 
{
	// TODO: Add your control notification handler code here
	CString strFilter;
	strFilter.LoadString(IDS_FILE_SAVE_AS);
	CString newName="RejectedMAC_";
	if( id_name2 )
		newName.LoadString(id_name2);

	newName+=::TimeToSQL(COleDateTime::GetCurrentTime());

	int i;
	for( i=0;(i=newName.FindOneOf(_T(" ")))!=-1;)
		newName.SetAt(i,_T('_'));
	for(i=0;(i=newName.FindOneOf(_T(":")))!=-1;)
		newName.SetAt(i,_T('-'));

	CFileDialog aFileDialog( false, "txt", newName,
		    OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT |OFN_PATHMUSTEXIST,
							 strFilter);
	
	int nID = aFileDialog.DoModal();
	if(nID==IDOK){
		CStdioFile aFile;

		CFileException fx;
		if( !aFile.Open( aFileDialog.GetPathName(),CFile::modeCreate|CFile::modeWrite|CFile::typeText, &fx ) ){
			TCHAR buf[ 255 ];
			fx.GetErrorMessage( buf, 255 );
			CString strPrompt( buf );
			AfxMessageBox( strPrompt );
			return;
		}

		for(int i=0;i<m_Edit.GetLineCount();i++){
			TCHAR buf[ 256 ];
			int len=m_Edit.GetLine(i,buf,255);
			buf[len]=0;
			aFile.WriteString(buf);
			aFile.WriteString(_T("\n"));
		}
		aFile.Close();
		m_Edit.SetModify(false);
	}
}
