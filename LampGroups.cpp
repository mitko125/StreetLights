// LampGroups.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "LampGroups.h"

#include "DynamicLib.h"

#include "..\Free_Util_Dll\Util_Dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLampGroups dialog


CLampGroups::CLampGroups(CWnd* pParent /*=NULL*/)
	: CDialog(CLampGroups::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLampGroups)
	m_sSucses = _T("");
	//}}AFX_DATA_INIT
}


void CLampGroups::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLampGroups)
	DDX_Text(pDX, IDC_STATIC_SUCSES, m_sSucses);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLampGroups, CDialog)
	//{{AFX_MSG_MAP(CLampGroups)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_DB, OnButtonLoadDb)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_DB, OnButtonSaveDb)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_LAMP, OnButtonLoadLamp)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_LAMP, OnButtonSaveLamp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLampGroups message handlers

BOOL CLampGroups::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	SetCheckBoxs();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLampGroups::SetCheckBoxs()
{
	CButton *pB;
	for(int j = 0;j<16;j++){
		pB=(CButton*)GetDlgItem(IDC_DB_GROUP1+j);
		if(groups & (0x0001<<j))
			pB->SetCheck(1);
		else
			pB->SetCheck(0);
	}
}

void CLampGroups::OnButtonLoadDb() 
{
	// TODO: Add your control notification handler code here

	_RecordsetPtr m_pRs=NULL;

	CString com;
	com.Format(_T("SELECT bit_groups FROM lamps WHERE id_lamp=%u"),id_lamp);
	try{
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
								
		if(!m_pRs->EndOfFile){		
			_variant_t index;
			index.vt = VT_I2;
			index.iVal = 0;
			
			_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
			groups = ::GetLongFromVariant(data);
		}
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}	
	if (m_pRs)
		if (m_pRs->State == adStateOpen)
			m_pRs->Close();
	
	SetCheckBoxs();
	UpdateData(false);
}

void CLampGroups::OnButtonSaveDb() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(true))
		return;

	groups = 0;
	CButton *pB;

	for(int j = 0;j<16;j++){
		pB=(CButton*)GetDlgItem(IDC_DB_GROUP1+j);
		if( pB->GetCheck() )
			groups |= (0x0001<<j);
	}

	_CommandPtr m_pCmd=NULL;
	CString com;
				
	try{
		com.Format(_T("UPDATE lamps SET bit_groups=%u WHERE id_lamp=%u"),groups,id_lamp);
		TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
		m_pCmd->ActiveConnection=::GetConnection();
		m_pCmd->CommandText=(LPCTSTR)com;
		m_pCmd->Execute(NULL, NULL, adCmdText);	
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}
}

void CLampGroups::OnButtonLoadLamp() 
{
	// TODO: Add your control notification handler code here
	{
		
		{
			
			BeginWaitCursor();
			
			UINT32 new_groups = 0;

			int recived=JennicModule.GetLampGroups1(id_lamp,&new_groups);

			CString text;
			switch(recived){
			case UNCNOW_ERROR:
				text.LoadString(IDS_ERROR);
				break;
			case NO_ROUTER:
				text.LoadString(IDS_NO_RUTER);
				break;
			case NO_HARDWARE:
				text.LoadString(IDS_NO_HARDWARE);
				break;
			case NO_CONNECT:
				text.LoadString(IDS_NO_LAMP);
				break;
			default:
				if( recived < 0 )
					text.LoadString(IDS_ERROR);
				else{
					text.LoadString(IDS_SUCSES); 
					groups = new_groups;
					SetCheckBoxs();
				}
				break;
			}
			m_sSucses = text;
			UpdateData(false);
		}
	}
	EndWaitCursor();
}

void CLampGroups::OnButtonSaveLamp() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(true))
		return;

	groups = 0;
	CButton *pB;

	for(int j = 0;j<16;j++){
		pB=(CButton*)GetDlgItem(IDC_DB_GROUP1+j);
		if( pB->GetCheck() )
			groups |= (0x0001<<j);
	}

	{
		
		{
			
			BeginWaitCursor();
			
			int recived=JennicModule.SetLampGroups1(id_lamp,groups);

			CString text;
			switch(recived){
			case UNCNOW_ERROR:
				text.LoadString(IDS_ERROR);
				break;
			case NO_ROUTER:
				text.LoadString(IDS_NO_RUTER);
				break;
			case NO_HARDWARE:
				text.LoadString(IDS_NO_HARDWARE);
				break;
			case NO_CONNECT:
				text.LoadString(IDS_NO_LAMP);
				break;
			default:
				if( recived < 0 )
					text.LoadString(IDS_ERROR);
				else
					text.LoadString(IDS_SUCSES); 
				break;
			}
			m_sSucses = text;
			UpdateData(false);
		}
	}
	EndWaitCursor();
}
