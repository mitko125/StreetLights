// LampInRouterSetup.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "LampInRouterSetup.h"


#include "DynamicLib.h"

#include "..\Free_Util_Dll\Util_Dll.h"
#include	"../../Grid1_0/NewCellTypes/GridCellCombo.h"
#include	"../../Grid1_0/NewCellTypes/GridCellNumeric.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



#define C_NUMBER 0
#define C_CITY 1
#define C_ADDRESS 2
#define C_SUCSES 3
#define C_SEQURITY 4
#define C_RADIUS 5
#define C_LAMPS 6
#define C_LAMPS_CONNECTED 7
#define C_STOP 8
#define MAX_COLUMS 9

/////////////////////////////////////////////////////////////////////////////
// CLampInRouterSetup property page

IMPLEMENT_DYNCREATE(CLampInRouterSetup, CPropertyPage)

CLampInRouterSetup::CLampInRouterSetup() : CPropertyPage(CLampInRouterSetup::IDD)
{
	m_iSelectItem=-1;
	fl_config=false;
	//{{AFX_DATA_INIT(CLampInRouterSetup)
	//}}AFX_DATA_INIT

}

CLampInRouterSetup::~CLampInRouterSetup()
{
}

void CLampInRouterSetup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLampInRouterSetup)
	DDX_Control(pDX, IDC_BUTTON_GET1, m_bGet1);
	DDX_Control(pDX, IDC_BUTTON_SET1, m_bSet1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_Grid); 
}

 
BEGIN_MESSAGE_MAP(CLampInRouterSetup, CPropertyPage)
	//{{AFX_MSG_MAP(CLampInRouterSetup)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_PREVIEV, OnButtonPrintPreviev)
	ON_BN_CLICKED(IDC_BUTTON_GET1, OnButtonGet1)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnButtonGet)
	ON_BN_CLICKED(IDC_BUTTON_GET_REJECT1, OnButtonGetReject1)
	ON_BN_CLICKED(IDC_BUTTON_LAMPS_SET1, OnButtonLampsSet1)
	ON_BN_CLICKED(IDC_BUTTON_GET_LANPS_STATUS1, OnButtonGetLanpsStatus1)
	ON_BN_CLICKED(IDC_BUTTON_GET_LANPS_STATUS, OnButtonGetLanpsStatus)
	ON_BN_CLICKED(IDC_BUTTON_LAMPS_SET, OnButtonLampsSet)
	ON_BN_CLICKED(IDC_BUTTON_GET_REJECT, OnButtonGetReject)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_ROUTERS_REJECT, OnButtonLoadRoutersReject)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_SET1, OnButtonSet1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLampInRouterSetup message handlers

BOOL CLampInRouterSetup::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	BeginWaitCursor();
	CString text;

	CStringArray optStop;
	text.LoadString(IDS_WORK);
	optStop.Add(text);
	text.LoadString(IDS_NO_WORK);
	optStop.Add(text);
	
	m_Grid.SetEditable(true);
	m_Grid.EnableSelection(false);
	m_Grid.SetHandleTabKey(false);
	m_Grid.EnableLitleCell();
	TRY {
		m_Grid.SetFixedRowCount(1);
		m_Grid.SetFixedColumnCount(1);
	  m_Grid.SetRowCount(1);
	  m_Grid.SetColumnCount(MAX_COLUMS);
	}
	CATCH (CMemoryException, e) {
		e->ReportError();
  	return false;
	}END_CATCH

	GV_ITEM Item,Item1;
	Item1.mask=Item.mask = GVIF_FORMAT;
	Item.row=0; Item1.row=-1;
	Item1.nFormat=Item.nFormat=DT_RIGHT|DT_VCENTER|DT_SINGLELINE;

	m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_NUMBER);
	text.LoadString(IDS_NUMBER);
	m_Grid.SetColumnWidth(C_NUMBER,40);
	m_Grid.SetItemText(0,C_NUMBER,text);
	Item1.col=Item.col=C_NUMBER; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_ADDRESS1);
	m_Grid.SetColumnWidth(C_CITY,90);
	m_Grid.SetItemText(0,C_CITY,text);

	text.LoadString(IDS_ADDRESS2);
	m_Grid.SetColumnWidth(C_ADDRESS,100);
	m_Grid.SetItemText(0,C_ADDRESS,text);

	text.LoadString(IDS_SUCSES);
	m_Grid.SetColumnWidth(C_SUCSES,50);
	m_Grid.SetItemText(0,C_SUCSES,text);
	
	text.LoadString(IDS_SEQURITY);
	m_Grid.SetColumnWidth(C_SEQURITY,120);
	m_Grid.SetItemText(0,C_SEQURITY,text);

	text.LoadString(IDS_RADIUS);
	m_Grid.SetColumnWidth(C_RADIUS,80);
	m_Grid.SetItemText(0,C_RADIUS,text);

	m_Grid.SetCellType(-1,C_RADIUS, RUNTIME_CLASS(CGridCellCombo));
	CGridCellCombo *pCell = (CGridCellCombo*) m_Grid.GetCell(-1,C_RADIUS);
  pCell->SetOptions(optStop);
  pCell->SetStyle(CBS_DROPDOWNLIST);

	m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_LAMPS);
	text.LoadString(IDS_LAMPS);
	m_Grid.SetColumnWidth(C_LAMPS,60);
	m_Grid.SetItemText(0,C_LAMPS,text);
	Item1.col=Item.col=C_LAMPS; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_LAMPS_CONNECTED);
	text.LoadString(IDS_LAMPS_CONNECTED);
	m_Grid.SetColumnWidth(C_LAMPS_CONNECTED,80);
	m_Grid.SetItemText(0,C_LAMPS_CONNECTED,text);
	Item1.col=Item.col=C_LAMPS_CONNECTED; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_STOP);
	m_Grid.SetColumnWidth(C_STOP,80);
	m_Grid.SetItemText(0,C_STOP,text);

	m_Grid.SetCellType(-1,C_STOP, RUNTIME_CLASS(CGridCellCombo));
	pCell = (CGridCellCombo*) m_Grid.GetCell(-1,C_STOP);
  pCell->SetOptions(optStop);
  pCell->SetStyle(CBS_DROPDOWNLIST);

	m_Grid.SetFixedColumnSelection(TRUE);
	m_Grid.SetHeaderSort(true);
	
	return LoadData(true); 
}

BOOL CLampInRouterSetup::LoadData(bool fl_select)
{
	BOOL flag=true;
	CString text;
	_RecordsetPtr m_pRs=NULL;
	
  m_Grid.SetRowCount(1);
	  
	_bstr_t strCom("SELECT id_hardware,number_hardware,city_name,address_name,sequrity_key,radius_off,stop FROM hardwares ORDER BY id_hardware");
	try{
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open(strCom,_variant_t((IDispatch *)GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
				
		int i=1;
		_variant_t index;
		index.vt = VT_I2;
		_variant_t data;
		while(!m_pRs->EndOfFile){		
			TRY {
				CString text;
				m_Grid.InsertRow("");

				index.iVal = 0; data=m_pRs->Fields->GetItem(&index)->Value;
				
				text.Format("%u",::GetLongFromVariant(data));
				m_Grid.SetItemText(i,C_NUMBER,text);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemData(i,C_NUMBER,inet_addr(text));	

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_CITY,text);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_ADDRESS,text);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_SEQURITY,text);

				text.LoadString(IDS_WORK);
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				if(::GetLongFromVariant(data))
					text.LoadString(IDS_NO_WORK);
				m_Grid.SetItemText(i,C_RADIUS,text);

				text.LoadString(IDS_WORK);
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				if(::GetLongFromVariant(data))
					text.LoadString(IDS_NO_WORK);
				m_Grid.SetItemText(i,C_STOP,text);
			}
			m_pRs->MoveNext();
			i++;
			CATCH (CMemoryException, e){
				e->ReportError();
  			flag=false;
			}END_CATCH
		}		
		if(fl_select&&(i>1))
			m_Grid.SetFocusCell(m_iSelectItem=1,0);
		EnableKeys();
		m_Grid.RedrawWindow();
		m_Grid.SetFocus();
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
		flag=false;
	}
	if (m_pRs)
		if (m_pRs->State == adStateOpen)
			m_pRs->Close();
	return flag;
}

void CLampInRouterSetup::EnableKeys()
{
	BOOL bNew,bDel;
	bNew=bDel=false;
	if(m_iSelectItem>=1){
		bNew=bDel=true;
		if(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER))==0)
			bDel=false;
	}else
		bNew=true;
	//m_bNew.EnableWindow(bNew);
	//m_bDel.EnableWindow(bDel);
	m_bSet1.EnableWindow(bDel);
	m_bGet1.EnableWindow(bDel);
}

BOOL CLampInRouterSetup::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == (WPARAM)m_Grid.GetDlgCtrlID()){
    *pResult = 1;
    GV_DISPINFO *pDispInfo = (GV_DISPINFO*)lParam;
		switch(pDispInfo->hdr.code){
			case GVN_ENDLABELEDIT:
				{
					_CommandPtr m_pCmd=NULL;
					UINT32 ID=atoi(m_Grid.GetItemText(pDispInfo->item.row,C_NUMBER));
					CString com;
					CString text;
					try{
						switch(pDispInfo->item.col){
							case C_SEQURITY:
								{
									text=m_Grid.GetItemText(pDispInfo->item.row,C_SEQURITY);
									::CStringSize(text,16);
									m_Grid.SetItemText(pDispInfo->item.row,C_SEQURITY,text);
									_RecordsetPtr m_pRs=NULL;
									com.Format(_T("UPDATE hardwares SET sequrity_key='%s' WHERE id_hardware=%u"),text,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_RADIUS:
								{
									int radius_off = 0;
									text=m_Grid.GetItemText(pDispInfo->item.row,C_RADIUS);
									com.LoadString(IDS_WORK);
									if(com != text)
										radius_off = 1;
									_RecordsetPtr m_pRs=NULL;
									com.Format(_T("UPDATE hardwares SET radius_off=%d WHERE id_hardware=%u"),radius_off,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_STOP:
								{
									int stop = 0;
									text=m_Grid.GetItemText(pDispInfo->item.row,C_STOP);
									com.LoadString(IDS_WORK);
									if(com != text)
										stop = 1;
									_RecordsetPtr m_pRs=NULL;
									com.Format(_T("UPDATE hardwares SET stop=%d WHERE id_hardware=%u"),stop,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
						}							
					}
					catch(_com_error &e){
						::GenerateSqlError(e);
					//	m_Grid.SetItemText(pDispInfo->item.row,pDispInfo->item.col,OldText);
					}
					m_Grid.RedrawRow(pDispInfo->item.row);
				}
				return true;
			case GVN_BEGINLABELEDIT:
				if(pDispInfo->item.col!= C_STOP){
					switch(pDispInfo->item.col){
					case C_NUMBER:
					case C_CITY:
					case C_ADDRESS:
					case C_SUCSES:
					case C_LAMPS:
					case C_LAMPS_CONNECTED:
						*pResult =-1;
						break;
					}
				}
				return true;
			case GVN_SELCHANGED:
				if(pDispInfo->item.row>0){
					m_iSelectItem=pDispInfo->item.row;
				}else
					m_iSelectItem=-1;	
				EnableKeys();
				return true;
		}
	}	
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void CLampInRouterSetup::StoreConfigMessage()
{
	if(!fl_config){
		fl_config=true;
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		ASSERT_VALID(pApp);
		::StoreDataConfigMessage(pApp->Id_PC,pApp->Id_Operator,_CONFIG_ROUTERS);
	}
}

void CLampInRouterSetup::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrint(&m_Grid,this);
}

void CLampInRouterSetup::OnButtonPrintPreviev() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrintPreviev(&m_Grid,this);			
}

BOOL CLampInRouterSetup::ContinueModal()
{
	if (!IsWindowVisible())
		// send WM_IDLEUPDATECMDUI message to update toolbar state
		// This is normally called by OnIdle function in SDI or MSI applications.
		// Dialog based applications don't call OnIdle, so send the message from here instead
		AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
			(WPARAM)TRUE, 0, TRUE, TRUE);

	return CDialog::ContinueModal();
}

void CLampInRouterSetup::OnButtonSet1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString text;
		{
			BeginWaitCursor();
			
			int recived=JennicModule.SetWPAN(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
			text.LoadString(IDS_ERROR);
			if( recived >= 0 ){
				text.LoadString(IDS_SUCSES);
			}
			m_Grid.SetItemText(m_iSelectItem,C_SUCSES,text);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);
		}
	}
	EndWaitCursor();
}

void CLampInRouterSetup::OnButtonGet1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");
	m_Grid.SetItemText(m_iSelectItem,C_LAMPS,"");
	m_Grid.SetItemText(m_iSelectItem,C_LAMPS_CONNECTED,"");

	m_Grid.RedrawWindow();

	{
		CString sucses;
		{
			BeginWaitCursor();
			
			sucses.LoadString(IDS_ERROR);

			struct in_addr adr;
			adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);

			int recived=JennicModule.GetWPAN(inet_ntoa(adr));
			
			if( recived >= (int)(HEADER_SIZE+1+58) ){// 58 old version sizeof(tsConfigBorderRuter)) ){
				if( p_hardware->Bufer_Rx[HEADER_SIZE] == COMMAND_SET_HOST_DATA ){
					tsConfigBorderRuter sModuleGetConfig;
					memset(&sModuleGetConfig,0,sizeof(tsConfigBorderRuter));
					memcpy(&sModuleGetConfig,p_hardware->Bufer_Rx + HEADER_SIZE + 1,recived - (HEADER_SIZE + 1));
					{//if( sModuleGetConfig.sModuleConfigV11.u8Channel != 0 ){ V2
						CString text;
						
						sucses.LoadString(IDS_SUCSES);
						
						u16LampsInTable += htons(sModuleGetConfig.u16LampsInTable);
						text.Format("%u",htons(sModuleGetConfig.u16LampsInTable));
						m_Grid.SetItemText(m_iSelectItem,C_LAMPS,text);
						
						u16LampsConnected += htons(sModuleGetConfig.u16LampsConnected);
						text.Format("%u",htons(sModuleGetConfig.u16LampsConnected));
						m_Grid.SetItemText(m_iSelectItem,C_LAMPS_CONNECTED,text);
						
					}
				}
			}
			m_Grid.SetItemText(m_iSelectItem,C_SUCSES,sucses);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);
		}
	}
	EndWaitCursor();
}

void CLampInRouterSetup::OnButtonSet() 
{
	// TODO: Add your control notification handler code here
	for(int i = 1; i < m_Grid.GetRowCount() ; i++){
		m_Grid.SetItemText(i,C_SUCSES,"");
	}

	CString enabled;
	enabled.LoadString(IDS_WORK);
	int old_iSelectItem = m_iSelectItem;
	for( m_iSelectItem = 1; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
		CString text;
		text=m_Grid.GetItemText(m_iSelectItem,C_STOP);
		if(enabled == text){
			OnButtonSet1();
		}
	}
	m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
	m_Grid.SetFocusCell(m_iSelectItem,0);
	m_Grid.RedrawWindow();
	m_Grid.SetFocus();
	m_Grid.EnsureVisible(m_iSelectItem,0);
}

void CLampInRouterSetup::OnButtonGet() 
{
	// TODO: Add your control notification handler code here
	for(int i = 1; i < m_Grid.GetRowCount() ; i++){
		m_Grid.SetItemText(i,C_SUCSES,"");
	}

	u16LampsInTable = 0;
	u16LampsConnected = 0;

	CString enabled;
	enabled.LoadString(IDS_WORK);
	int old_iSelectItem = m_iSelectItem;
	for( m_iSelectItem = 1; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
		CString text;
		text=m_Grid.GetItemText(m_iSelectItem,C_STOP);
		if(enabled == text){
			OnButtonGet1();
		}
	}
	m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
	m_Grid.SetFocusCell(m_iSelectItem,0);
	m_Grid.RedrawWindow();
	m_Grid.SetFocus();
	m_Grid.EnsureVisible(m_iSelectItem,0);

	double precent=0;
	if(u16LampsInTable)
		precent=u16LampsConnected*100.0/u16LampsInTable;
	enabled.Format(IDS_PERCENT_CONNECTED,u16LampsInTable,u16LampsConnected,precent);
	AfxMessageBox(enabled);
}

void CLampInRouterSetup::OnButtonLampsSet1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString text;
		{
			BeginWaitCursor();
			int CouLamps=0;

			struct in_addr adr;
			adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);

			int recived=JennicModule.SetLampsMAC(inet_ntoa(adr),atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)),&CouLamps);
			text.LoadString(IDS_ERROR);
			if( recived >= 0 ){
				text.Format("%d",CouLamps);
				m_Grid.SetItemText(m_iSelectItem,C_LAMPS,text);
				m_Grid.SetItemText(m_iSelectItem,C_LAMPS_CONNECTED,"");
				text.LoadString(IDS_SUCSES);
			}
			m_Grid.SetItemText(m_iSelectItem,C_SUCSES,text);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);
		}
	}
	EndWaitCursor();
}


void CLampInRouterSetup::OnButtonLampsSet() 
{
	// TODO: Add your control notification handler code here
	for(int i = 1; i < m_Grid.GetRowCount() ; i++){
		m_Grid.SetItemText(i,C_SUCSES,"");
	}

	CString enabled;
	enabled.LoadString(IDS_WORK);
	int old_iSelectItem = m_iSelectItem;
	for( m_iSelectItem = 1; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
		CString text;
		text=m_Grid.GetItemText(m_iSelectItem,C_STOP);
		if(enabled == text){
			OnButtonLampsSet1();
		}
	}
	m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
	m_Grid.SetFocusCell(m_iSelectItem,0);
	m_Grid.RedrawWindow();
	m_Grid.SetFocus();
	m_Grid.EnsureVisible(m_iSelectItem,0);
}

void CLampInRouterSetup::OnButtonGetLanpsStatus1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString text;
		{
			BeginWaitCursor();
			
			struct in_addr adr;
			adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);

			int recived=JennicModule.GetLampsStatus(inet_ntoa(adr),atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
			text.LoadString(IDS_ERROR);
			if( recived >= 0 ){
				text.LoadString(IDS_SUCSES);
			}
			m_Grid.SetItemText(m_iSelectItem,C_SUCSES,text);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);
		}
	}
	EndWaitCursor();
}

void CLampInRouterSetup::OnButtonGetLanpsStatus() 
{
	// TODO: Add your control notification handler code here
	for(int i = 1; i < m_Grid.GetRowCount() ; i++){
		m_Grid.SetItemText(i,C_SUCSES,"");
	}

	CString enabled;
	enabled.LoadString(IDS_WORK);
	int old_iSelectItem = m_iSelectItem;
	for( m_iSelectItem = 1; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
		CString text;
		text=m_Grid.GetItemText(m_iSelectItem,C_STOP);
		if(enabled == text){
			OnButtonGetLanpsStatus1();
		}
	}
	m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
	m_Grid.SetFocusCell(m_iSelectItem,0);
	m_Grid.RedrawWindow();
	m_Grid.SetFocus();
	m_Grid.EnsureVisible(m_iSelectItem,0);
}

#include "DisabledLamps.h"

void CLampInRouterSetup::OnButtonGetReject1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString sucses;
		{
			BeginWaitCursor();
			
			CString to_edit,tmp;
			tmp.LoadString(IDS_DIABLED_LAMP_L1);
			to_edit += tmp;
			tmp.LoadString(IDS_DIABLED_LAMP_L2);
			to_edit += tmp;

			sucses.LoadString(IDS_ERROR);

			struct in_addr adr;
			adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);

			int recived=JennicModule.GetReject(inet_ntoa(adr));
			
			if( recived >= (int)( HEADER_SIZE + 1 + sizeof(tsMAC_Reject) ) ){
				if( p_hardware->Bufer_Rx[HEADER_SIZE] == 	SEND_REJECT_TABLE ){
					tsMAC_Address sMAC_zero = {0,0,0,0,0,0,0,0};
					tsMAC_Reject sMAC_Reject;
					memcpy(&sMAC_Reject,p_hardware->Bufer_Rx + HEADER_SIZE + 1,sizeof(tsMAC_Reject));
					
					_RecordsetPtr m_pRs=NULL;
					
					CString MACstr;

					for(int i = 0; i < MAX_ACCESS_REJECT_TABLE ; i++ ){
						if( memcmp(&(sMAC_Reject.sReject[i]),&sMAC_zero,sizeof(tsMAC_Address)) != 0){
							MACstr.Format("%02X%02X%02X%02X%02X%02X%02X%02X",sMAC_Reject.sReject[i].MAC[0],sMAC_Reject.sReject[i].MAC[1]
								,sMAC_Reject.sReject[i].MAC[2],sMAC_Reject.sReject[i].MAC[3],sMAC_Reject.sReject[i].MAC[4]
								,sMAC_Reject.sReject[i].MAC[5],sMAC_Reject.sReject[i].MAC[6],sMAC_Reject.sReject[i].MAC[7]);
							
							int id_hardware = 0;
							int id_lamp = 0;
							COleDateTime time(0,0,0,0,0,0);

							BOOL fl_data = false;

							CString com;
							com.Format(_T("SELECT id_hardware,id_lamp,work_hours,last_contact,last_test FROM lamps WHERE MAC_address='%s'"),MACstr);
							try{
								TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
								m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
								
								while(!m_pRs->EndOfFile){		
									_variant_t index;
									index.vt = VT_I2;
									index.iVal = 0;
									
									_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
									id_hardware = ::GetLongFromVariant(data);

									index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
									id_lamp = ::GetLongFromVariant(data);

									fl_data = true;

									index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
									unsigned int work_hours = ::GetLongFromVariant(data);

									index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
									CString last_contact=::TimeToSQL(data);

									index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
									CString last_test=::TimeToSQL(data);

									tmp.Format("%d\t%d\t%d\t%u\t%s\t%s\t%s\r\n",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)),id_hardware,id_lamp,work_hours,last_contact,last_test,MACstr);
									to_edit += tmp;

									m_pRs->MoveNext();
								}	
								if(!fl_data){
									tmp.Format("%d\t%d\t%d\t%s\r\n",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)),id_hardware,id_lamp,MACstr);
									to_edit += tmp;
								}
							}
							catch(_com_error &e){
								::GenerateSqlError(e);
							}	
							if (m_pRs)
								if (m_pRs->State == adStateOpen)
									m_pRs->Close();
						}
					}						
					//	AfxMessageBox(text);
					sucses.LoadString(IDS_SUCSES);
				}
			}
			m_Grid.SetItemText(m_iSelectItem,C_SUCSES,sucses);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);

			tmp.LoadString(IDS_ERROR);
			if( tmp == sucses ){
				tmp.Format("%d\t%s\r\n",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)),sucses);
				to_edit += tmp;
			}
			CDisabledLamps DL;
			DL.text = to_edit;
			DL.DoModal();
		}
	}
	EndWaitCursor();
}

void CLampInRouterSetup::OnButtonGetReject() 
{
	// TODO: Add your control notification handler code here
	for(int i = 1; i < m_Grid.GetRowCount() ; i++){
		m_Grid.SetItemText(i,C_SUCSES,"");
	}
	m_Grid.RedrawWindow();

	CString enabled;
	enabled.LoadString(IDS_WORK);


	{
		CString sucses;
		{
			BeginWaitCursor();
			
			CString to_edit,tmp;
			tmp.LoadString(IDS_DIABLED_LAMP_L1);
			to_edit += tmp;
			tmp.LoadString(IDS_DIABLED_LAMP_L2);
			to_edit += tmp;
			
			int cou_lamp = 0;

			int old_iSelectItem = m_iSelectItem;

			for( m_iSelectItem = 1; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
				CString text;
				text=m_Grid.GetItemText(m_iSelectItem,C_STOP);
				if(enabled == text){
					sucses.LoadString(IDS_ERROR);
					
					struct in_addr adr;
				adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);

					int recived=JennicModule.GetReject(inet_ntoa(adr));
					
					if( recived >= (int)( HEADER_SIZE + 1 + sizeof(tsMAC_Reject) ) ){
						if( p_hardware->Bufer_Rx[HEADER_SIZE] == 	SEND_REJECT_TABLE ){
							tsMAC_Address sMAC_zero = {0,0,0,0,0,0,0,0};
							tsMAC_Reject sMAC_Reject;
							memcpy(&sMAC_Reject,p_hardware->Bufer_Rx + HEADER_SIZE + 1,sizeof(tsMAC_Reject));
							
							_RecordsetPtr m_pRs=NULL;
							
							CString MACstr;
							
							for(int i = 0; i < MAX_ACCESS_REJECT_TABLE ; i++ ){
								if( memcmp(&(sMAC_Reject.sReject[i]),&sMAC_zero,sizeof(tsMAC_Address)) != 0){
									MACstr.Format("%02X%02X%02X%02X%02X%02X%02X%02X",sMAC_Reject.sReject[i].MAC[0],sMAC_Reject.sReject[i].MAC[1]
										,sMAC_Reject.sReject[i].MAC[2],sMAC_Reject.sReject[i].MAC[3],sMAC_Reject.sReject[i].MAC[4]
										,sMAC_Reject.sReject[i].MAC[5],sMAC_Reject.sReject[i].MAC[6],sMAC_Reject.sReject[i].MAC[7]);
									
									int id_hardware = 0;
									int id_lamp = 0;
									BOOL fl_data = false;
									
									CString com;
									com.Format(_T("SELECT id_hardware,id_lamp,work_hours,last_contact,last_test FROM lamps WHERE MAC_address='%s'"),MACstr);
									try{
										TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
										m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
										
										while(!m_pRs->EndOfFile){		
											_variant_t index;
											index.vt = VT_I2;
											index.iVal = 0;
											
											_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
											id_hardware = ::GetLongFromVariant(data);
											
											index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
											id_lamp = ::GetLongFromVariant(data);
											
											fl_data = true;

											cou_lamp ++;

											index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
											unsigned int work_hours = ::GetLongFromVariant(data);

											index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
											CString last_contact=::TimeToSQL(data);

											index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
											CString last_test=::TimeToSQL(data);

											tmp.Format("%d\t%d\t%d\t%u\t%s\t%s\t%s\r\n",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)),id_hardware,id_lamp,work_hours,last_contact,last_test,MACstr);
											to_edit += tmp;
											
											m_pRs->MoveNext();
										}	
										if(!fl_data){
											tmp.Format("%d\t%d\t%d\t%s\r\n",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)),id_hardware,id_lamp,MACstr);
											to_edit += tmp;
											cou_lamp ++;
										}
									}
									catch(_com_error &e){
										::GenerateSqlError(e);
									}	
									if (m_pRs)
										if (m_pRs->State == adStateOpen)
											m_pRs->Close();
										
								}
							}						
							//	AfxMessageBox(text);
							sucses.LoadString(IDS_SUCSES);
						}
					}
					m_Grid.SetItemText(m_iSelectItem,C_SUCSES,sucses);
					m_Grid.SetFocusCell(m_iSelectItem,0);
					m_Grid.RedrawWindow();
					m_Grid.SetFocus();
					m_Grid.EnsureVisible(m_iSelectItem,0);
					
					tmp.LoadString(IDS_ERROR);
					if( tmp == sucses ){
						tmp.Format("%d\t%s\r\n",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)),sucses);
						to_edit += tmp;
					}
				}
			}
			if( cou_lamp ){
				tmp.Format(IDS_TOTAL,cou_lamp);
				to_edit += tmp;
			}
			m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);

			CDisabledLamps DL;
			DL.text = to_edit;
			DL.DoModal();
		}
	}
	EndWaitCursor();
}

void CLampInRouterSetup::OnButtonLoadRoutersReject() 
{
	// TODO: Add your control notification handler code here
	DWORD lFlags = OFN_HIDEREADONLY | OFN_FILEMUSTEXIST;
	CString fileName;


	CFileDialog dlgFile(true);

	CString title;
	VERIFY(title.LoadString(AFX_IDS_OPENFILE));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	
	// append the "*.txt" all files filter
	CString allFilter;
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.txt");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;

	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	if( dlgFile.DoModal() == IDOK){
		
		fileName.ReleaseBuffer();

		CStdioFile aFile;
	
		CFileException fx;
		if( !aFile.Open( fileName,CFile::modeRead|CFile::typeText, &fx ) )
		{
			TCHAR buf[ 255 ];
			fx.GetErrorMessage( buf, 255 );
			CString strPrompt( buf );
			AfxMessageBox( strPrompt );
			return ; 
		}
		
		BeginWaitCursor();
		CString str=_T(" ");

		int line_number = 0;
		do{
			aFile.ReadString(str);
			if(!str.IsEmpty()){
				line_number ++;
				if( line_number> 2){
					CString text = str;
					int id_hardware = 0;
					int id_lamp = 0;
					int i;

					int counter = 0;
					
					if((i=text.Find(_T('\t')))>=0){
						CString num = text.Left(i);
						id_hardware = atoi(num);
						text = text.Mid(i+1);
						if((i=text.Find(_T('\t')))>=0){
							text = text.Mid(i+1);
							if((i=text.Find(_T('\t')))>=0){
								num = text.Left(i);
								id_lamp = atoi(num);
							}else
								id_lamp = atoi(text);
						}
					}
				//	text.Format("%d     %d",id_hardware,id_lamp);
				//	AfxMessageBox(text);
				
					{
						{
							if(id_lamp){
								_CommandPtr m_pCmd=NULL;
								try{
									CString com;
									
									com.Format(_T("UPDATE lamps SET id_hardware=%d WHERE id_lamp=%d"),id_hardware,id_lamp);;
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									::GenerateSqlError(e);
								}
							}
						}
					}
				}
			}
		}while(!str.IsEmpty());
		aFile.Close();		
	}
	EndWaitCursor();	
}
