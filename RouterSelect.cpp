// RouterSelect.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "RouterSelect.h"

#include "..\Free_Util_Dll\Util_Dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRouterSelect dialog


CRouterSelect::CRouterSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CRouterSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRouterSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_iNumberRouter = 0;
	RouterName = _T("");
}


void CRouterSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRouterSelect)
	DDX_Control(pDX, IDC_COMBO2, m_ComboCom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRouterSelect, CDialog)
	//{{AFX_MSG_MAP(CRouterSelect)
	ON_CBN_DBLCLK(IDC_COMBO2, OnDblclkCombo2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRouterSelect message handlers

BOOL CRouterSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CString text;
	_RecordsetPtr m_pRs=NULL;
	try{
		
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open("SELECT id_hardware,city_name,address_name FROM hardwares ORDER BY id_hardware",
			_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
		_variant_t index;
		index.vt = VT_I2;
		index.iVal = 0;
		_variant_t data;
		while(!m_pRs->EndOfFile){
			index.iVal = 0; data=m_pRs->Fields->GetItem(&index)->Value;
			text = ::GetStringFromVariant(data);	
			text += ' ';

			index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
			text += ::GetStringFromVariant(data);
			text += ' ';

			index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
			text += ::GetStringFromVariant(data);	
		
			m_ComboCom.AddString(text);

			m_pRs->MoveNext();
		}
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}
	if (m_pRs)
		if (m_pRs->State == adStateOpen)
			m_pRs->Close();

	m_ComboCom.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CRouterSelect::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();

	m_ComboCom.GetLBText(m_ComboCom.GetCurSel(),RouterName);
	m_iNumberRouter = atoi(RouterName);

	CDialog::OnOK();
}

void CRouterSelect::OnDblclkCombo2() 
{
	// TODO: Add your control notification handler code here
	OnOK();
}
