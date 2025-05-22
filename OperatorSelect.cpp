// OperatorSelect.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "OperatorSelect.h"

#include "..\Free_Util_Dll\Util_Dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COperatorSelect dialog


COperatorSelect::COperatorSelect(CWnd* pParent /*=NULL*/)
	: CDialog(COperatorSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(COperatorSelect)
	m_Password = _T("");
	m_Name = _T("");
	//}}AFX_DATA_INIT
}


void COperatorSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COperatorSelect)
	DDX_Control(pDX, IDC_COMBO_NAME, m_OL);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_Password);
	DDV_MaxChars(pDX, m_Password, 23);
	DDX_CBString(pDX, IDC_COMBO_NAME, m_Name);
	DDV_MaxChars(pDX, m_Name, 24);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COperatorSelect, CDialog)
	//{{AFX_MSG_MAP(COperatorSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COperatorSelect message handlers

BOOL COperatorSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	BOOL flag=true;
	_RecordsetPtr m_pRs=NULL;

	_bstr_t strCom("SELECT name_operator FROM operators ORDER BY id_operator");
	try{
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open(strCom,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);

		_variant_t index;
		index.vt = VT_I2;
		index.iVal = 0;
		int i=1;
		while(!m_pRs->EndOfFile){		
			_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
			int ind=m_OL.AddString(::GetStringFromVariant(data));
			i++;
			m_pRs->MoveNext();
		}		
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
		flag=false;
	}
  if (m_pRs)
		if (m_pRs->State == adStateOpen)
			m_pRs->Close();
	m_OL.SetCurSel(0);

	return flag;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COperatorSelect::OnOK() 
{
	// TODO: Add extra validation here
	CString text;
	UpdateData();
	
	_RecordsetPtr m_pRs=NULL;

	for(int i=0;i<m_OL.GetCount();i++){
		CString text;
		m_OL.GetLBText(i,text);
		if(text==m_Name){
			break;
		}
	}
	CString com;
	com.Format(_T("SELECT password_operator,id_operator FROM operators WHERE name_operator='%s'"),m_Name);
	try{
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);

		if(!m_pRs->EndOfFile){		
			_variant_t index;
			index.vt = VT_I2;
			index.iVal = 0;
			_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
			text=::DecodePassword((LPCSTR)((_bstr_t)data)/*::GetStringFromVariant(data)*/,24);
			if(text==m_Password){
				index.iVal = 1;
				_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
				id_operator=::GetLongFromVariant(data);
				m_pRs->Close();
				CDialog::OnOK();
				return;
			}else{
				text.LoadString(IDS_NOT_PASSWORD);
				AfxMessageBox(text);
				GetDlgItem(IDC_EDIT_PASSWORD)->SetFocus();
				m_pRs->Close();
				return;
			}
		}		
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}	
	if (m_pRs)
		if (m_pRs->State == adStateOpen)
			m_pRs->Close();
	text.LoadString(IDS_UNKNOW_OPERATOR_NAME);
	AfxMessageBox(text);
	m_OL.SetFocus();
}
