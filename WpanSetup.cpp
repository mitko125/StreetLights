// WpanSetup.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "WpanSetup.h"

#include "DynamicLib.h"

#include "..\Free_Util_Dll\Util_Dll.h"
#include	"../../Grid1_0/NewCellTypes/GridCellCombo.h"
#include	"../../Grid1_0/NewCellTypes/GridCellNumeric.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define R_HEADER 2

#define C_NUMBER 0
#define C_CITY 1
#define C_ADDRESS 2
#define C_SUCSES 3
#define C_CHANNEL 4
#define C_PAN_ID 5
#define C_MAC_ADDRESS 6
#define C_RESET_HOUR 7
#define C_RESET_MIN 8
#define C_ENABLE_ENERGY_METER 9
#define C_STOP 10
#define MAX_COLUMS 11

/////////////////////////////////////////////////////////////////////////////
// CWpanSetup property page

IMPLEMENT_DYNCREATE(CWpanSetup, CPropertyPage)

CWpanSetup::CWpanSetup() : CPropertyPage(CWpanSetup::IDD)
{
	m_iSelectItem=-1;
	fl_config=false;
	//{{AFX_DATA_INIT(CWpanSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CWpanSetup::~CWpanSetup()
{
}

void CWpanSetup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWpanSetup)
	DDX_Control(pDX, IDC_BUTTON_GET_MAC1, m_bGetMAC1);
	DDX_Control(pDX, IDC_BUTTON_GET1, m_bGet1);
	DDX_Control(pDX, IDC_BUTTON_SET1, m_bSet1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_Grid); 
}


BEGIN_MESSAGE_MAP(CWpanSetup, CPropertyPage)
	//{{AFX_MSG_MAP(CWpanSetup)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_PREVIEV, OnButtonPrintPreviev)
	ON_BN_CLICKED(IDC_BUTTON_SET1, OnButtonSet1)
	ON_BN_CLICKED(IDC_BUTTON_GET1, OnButtonGet1)
	ON_BN_CLICKED(IDC_BUTTON_GET_MAC1, OnButtonGetMac1)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnButtonGet)
	ON_BN_CLICKED(IDC_BUTTON_GET_MAC, OnButtonGetMac)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWpanSetup message handlers

BOOL CWpanSetup::OnInitDialog() 
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
		m_Grid.SetFixedRowCount(R_HEADER);
		m_Grid.SetFixedColumnCount(1);
	  m_Grid.SetRowCount(R_HEADER);
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
	m_Grid.SetMerge(0,C_NUMBER,1,0,false);
	Item1.col=Item.col=C_NUMBER; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	Item1.nFormat=Item.nFormat=DT_VCENTER|DT_SINGLELINE;

	text.LoadString(IDS_ADDRESS1);
	m_Grid.SetColumnWidth(C_CITY,90);
	m_Grid.SetMerge(0,C_CITY,1,0,false);
	m_Grid.SetItemText(0,C_CITY,text);
	Item1.col=Item.col=C_CITY; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_ADDRESS2);
	m_Grid.SetColumnWidth(C_ADDRESS,100);
	m_Grid.SetMerge(0,C_ADDRESS,1,0,false);
	m_Grid.SetItemText(0,C_ADDRESS,text);
	Item1.col=Item.col=C_ADDRESS; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_SUCSES);
	m_Grid.SetColumnWidth(C_SUCSES,50);
	m_Grid.SetItemText(0,C_SUCSES,text);
	m_Grid.SetMerge(0,C_SUCSES,1,0,false);
	Item1.col=Item.col=C_SUCSES; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);
	
	Item1.nFormat=Item.nFormat=DT_RIGHT|DT_VCENTER|DT_SINGLELINE;

	text.LoadString(IDS_WPAN);
	m_Grid.SetItemText(0,C_CHANNEL,text);
	Item1.col=Item.col=C_CHANNEL; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);
	
	m_Grid.SetCellType(-1,C_CHANNEL,RUNTIME_CLASS(CGridCellNumeric));
	m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_CHANNEL);
	text.LoadString(IDS_CHANNEL);
	m_Grid.SetColumnWidth(C_CHANNEL,60);
	m_Grid.SetItemText(1,C_CHANNEL,text);
	Item1.col=Item.col=C_CHANNEL; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetCellType(-1,C_PAN_ID,RUNTIME_CLASS(CGridCellNumeric));
	m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_PAN_ID);
	text.LoadString(IDS_PAN_ID);
	m_Grid.SetColumnWidth(C_PAN_ID,60);
	m_Grid.SetItemText(1,C_PAN_ID,text);
	Item1.col=Item.col=C_PAN_ID; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_MAC_ADDRESS);
	m_Grid.SetColumnWidth(C_MAC_ADDRESS,120);
	m_Grid.SetItemText(1,C_MAC_ADDRESS,text);

	m_Grid.SetMerge(0,C_CHANNEL,0,2,false);

	text.LoadString(IDS_RESET_GPRS);
	m_Grid.SetItemText(0,C_RESET_HOUR,text);
	Item1.col=Item.col=C_RESET_HOUR; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);
	
	m_Grid.SetCellType(-1,C_RESET_HOUR,RUNTIME_CLASS(CGridCellNumeric));
	m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_RESET_HOUR);
	text.LoadString(IDS_HOUR);
	m_Grid.SetColumnWidth(C_RESET_HOUR,60);
	m_Grid.SetItemText(1,C_RESET_HOUR,text);
	Item1.col=Item.col=C_RESET_HOUR; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetCellType(-1,C_RESET_MIN,RUNTIME_CLASS(CGridCellNumeric));
	m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_RESET_MIN);
	text.LoadString(IDS_MINUTE);
	m_Grid.SetColumnWidth(C_RESET_MIN,60);
	m_Grid.SetItemText(1,C_RESET_MIN,text);
	Item1.col=Item.col=C_RESET_MIN; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetMerge(0,C_RESET_HOUR,0,1,false);
	
	Item1.nFormat=Item.nFormat=DT_VCENTER|DT_SINGLELINE;

	text.LoadString(IDS_ENERGY_METER);
	m_Grid.SetColumnWidth(C_ENABLE_ENERGY_METER,80);
	m_Grid.SetItemText(0,C_ENABLE_ENERGY_METER,text);
	text.LoadString(IDS_STOP);
	m_Grid.SetItemText(1,C_ENABLE_ENERGY_METER,text);
	Item1.col=Item.col=C_ENABLE_ENERGY_METER; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetCellType(-1,C_ENABLE_ENERGY_METER, RUNTIME_CLASS(CGridCellCombo));
	CGridCellCombo * pCell = (CGridCellCombo*) m_Grid.GetCell(-1,C_ENABLE_ENERGY_METER);
  pCell->SetOptions(optStop);
  pCell->SetStyle(CBS_DROPDOWNLIST);
	m_Grid.SetFixedColumnSelection(TRUE);

	text.LoadString(IDS_STOP);
	m_Grid.SetColumnWidth(C_STOP,80);
	m_Grid.SetItemText(0,C_STOP,text);
	m_Grid.SetMerge(0,C_STOP,1,0,false);
	Item1.col=Item.col=C_STOP; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetCellType(-1,C_STOP, RUNTIME_CLASS(CGridCellCombo));
	pCell = (CGridCellCombo*) m_Grid.GetCell(-1,C_STOP);
  pCell->SetOptions(optStop);
  pCell->SetStyle(CBS_DROPDOWNLIST);
	m_Grid.SetFixedColumnSelection(TRUE);

	m_Grid.SetHeaderSort(true);
	
	return LoadData(true); 
}

BOOL CWpanSetup::LoadData(bool fl_select)
{
	BOOL flag=true;
	CString text;
	_RecordsetPtr m_pRs=NULL;
	
  m_Grid.SetRowCount(R_HEADER);
	  
	_bstr_t strCom("SELECT id_hardware,number_hardware,city_name,address_name,channel,pan_id,MAC_address,resetGPRS_hh,resetGPRS_mm,enable_energy_meter,stop FROM hardwares ORDER BY id_hardware");
	try{
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open(strCom,_variant_t((IDispatch *)GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
				
		int i=R_HEADER;
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
				text.Format("%u",::GetLongFromVariant(data));
				m_Grid.SetItemText(i,C_CHANNEL,text);	
			
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				text.Format("%u",::GetLongFromVariant(data));
				m_Grid.SetItemText(i,C_PAN_ID,text);	

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_MAC_ADDRESS,text);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				text.Format("%u",::GetLongFromVariant(data));
				m_Grid.SetItemText(i,C_RESET_HOUR,text);	

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				text.Format("%u",::GetLongFromVariant(data));
				m_Grid.SetItemText(i,C_RESET_MIN,text);	

				text.LoadString(IDS_NO_WORK);
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				if(::GetLongFromVariant(data))
					text.LoadString(IDS_WORK);
				m_Grid.SetItemText(i,C_ENABLE_ENERGY_METER,text);

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
		if(fl_select&&(i>R_HEADER))
			m_Grid.SetFocusCell(m_iSelectItem=R_HEADER,0);
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

void CWpanSetup::EnableKeys()
{
	BOOL bNew,bDel;
	bNew=bDel=false;
	if(m_iSelectItem>=R_HEADER){
		bNew=bDel=true;
		if(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER))==0)
			bDel=false;
	}else
		bNew=true;
	//m_bNew.EnableWindow(bNew);
	//m_bDel.EnableWindow(bDel);
	m_bSet1.EnableWindow(bDel);
	m_bGet1.EnableWindow(bDel);
	m_bGetMAC1.EnableWindow(bDel);
}

BOOL CWpanSetup::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
							case C_CHANNEL:
								{
									UINT32 data=::UI32Format(m_Grid.GetItemText(pDispInfo->item.row,C_CHANNEL),26);
									if(data < 11)
										data = 0;
									text.Format("%u",data);
									m_Grid.SetItemText(pDispInfo->item.row,C_CHANNEL,text);
									com.Format(_T("UPDATE hardwares SET channel=%u WHERE id_hardware=%u"),data,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_PAN_ID:
								{
									UINT32 data=::UI32Format(m_Grid.GetItemText(pDispInfo->item.row,C_PAN_ID),0xFFFF);
									if( data == 0 )
										data = 0xFFFF;
									text.Format("%u",data);
									m_Grid.SetItemText(pDispInfo->item.row,C_PAN_ID,text);
									com.Format(_T("UPDATE hardwares SET pan_id=%u WHERE id_hardware=%u"),data,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_RESET_HOUR:
								{
									UINT32 data=::UI32Format(m_Grid.GetItemText(pDispInfo->item.row,C_RESET_HOUR),23);
									text.Format("%u",data);
									m_Grid.SetItemText(pDispInfo->item.row,C_RESET_HOUR,text);
									com.Format(_T("UPDATE hardwares SET resetGPRS_hh=%u WHERE id_hardware=%u"),data,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_RESET_MIN:
								{
									UINT32 data=::UI32Format(m_Grid.GetItemText(pDispInfo->item.row,C_RESET_MIN),59);
									text.Format("%u",data);
									m_Grid.SetItemText(pDispInfo->item.row,C_RESET_MIN,text);
									com.Format(_T("UPDATE hardwares SET resetGPRS_mm=%u WHERE id_hardware=%u"),data,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_ENABLE_ENERGY_METER:
								{
									int enable = 0;
									text=m_Grid.GetItemText(pDispInfo->item.row,C_ENABLE_ENERGY_METER);
									com.LoadString(IDS_WORK);
									if(com == text)
										enable = 1;
									com.Format(_T("UPDATE hardwares SET enable_energy_meter=%d WHERE id_hardware=%u"),enable,ID);
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
					case C_MAC_ADDRESS:
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

void CWpanSetup::StoreConfigMessage()
{
	if(!fl_config){
		fl_config=true;
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		ASSERT_VALID(pApp);
		::StoreDataConfigMessage(pApp->Id_PC,pApp->Id_Operator,_CONFIG_ROUTERS);
	}
}

void CWpanSetup::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrint(&m_Grid,this);
}

void CWpanSetup::OnButtonPrintPreviev() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrintPreviev(&m_Grid,this);			
}

BOOL CWpanSetup::ContinueModal()
{
	if (!IsWindowVisible())
		// send WM_IDLEUPDATECMDUI message to update toolbar state
		// This is normally called by OnIdle function in SDI or MSI applications.
		// Dialog based applications don't call OnIdle, so send the message from here instead
		AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
			(WPARAM)TRUE, 0, TRUE, TRUE);

	return CDialog::ContinueModal();
}

void CWpanSetup::OnButtonSet1() 
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

void CWpanSetup::OnButtonGet1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

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
					{//if( sModuleGetConfig.sModuleConfigV11.u8Channel != 0 ){	//V2
						
						_CommandPtr m_pCmd=NULL;
						UINT32 ID=atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER));
						CString com;
						CString text;
						try{
							if( sModuleGetConfig.sModuleConfigV11.u8Channel == 0 ){	//V2
								sModuleGetConfig.sModuleConfigV11.u16PanID = 65535;
							}
														
							sucses.LoadString(IDS_SUCSES);

							text.Format("%u",sModuleGetConfig.sModuleConfigV11.u8Channel);
							m_Grid.SetItemText(m_iSelectItem,C_CHANNEL,text);

							text.Format("%u",htons(sModuleGetConfig.sModuleConfigV11.u16PanID));
							m_Grid.SetItemText(m_iSelectItem,C_PAN_ID,text);

							text.Format("%u",sModuleGetConfig.u8INT_resetGPRShours);
							m_Grid.SetItemText(m_iSelectItem,C_RESET_HOUR,text);

							text.Format("%u",sModuleGetConfig.u8INT_resetGPRSminuts);
							m_Grid.SetItemText(m_iSelectItem,C_RESET_MIN,text);

							text.LoadString(IDS_NO_WORK);
							if(sModuleGetConfig.u8EnableEnergyMeter)
								text.LoadString(IDS_WORK);
							m_Grid.SetItemText(m_iSelectItem,C_ENABLE_ENERGY_METER,text);

							com.Format(_T("UPDATE hardwares SET channel=%u,pan_id=%u,resetGPRS_hh=%u,resetGPRS_mm=%u,enable_energy_meter=%u WHERE id_hardware=%u"),
								sModuleGetConfig.sModuleConfigV11.u8Channel,htons(sModuleGetConfig.sModuleConfigV11.u16PanID),
								sModuleGetConfig.u8INT_resetGPRShours,sModuleGetConfig.u8INT_resetGPRSminuts,sModuleGetConfig.u8EnableEnergyMeter,ID);
							TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
							m_pCmd->ActiveConnection=::GetConnection();
							m_pCmd->CommandText=(LPCTSTR)com;
							m_pCmd->Execute(NULL, NULL, adCmdText);	
							StoreConfigMessage();
						}
						catch(_com_error &e){
							::GenerateSqlError(e);
						}
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

void CWpanSetup::OnButtonGetMac1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString sucses;
		{
			BeginWaitCursor();
			
			sucses.LoadString(IDS_ERROR);

			struct in_addr adr;
			adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);

			int recived=JennicModule.GetMAC(inet_ntoa(adr));
			
			if( recived >= (int)( HEADER_SIZE + 1 + 8 ) ){
				if( p_hardware->Bufer_Rx[HEADER_SIZE] == 	COMMAND_MAC_ADDRESS ){
					{
						
						_CommandPtr m_pCmd=NULL;
						UINT32 ID=atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER));
						CString com;
						CString text;
						text.Format("%02X%02X%02X%02X%02X%02X%02X%02X",p_hardware->Bufer_Rx[HEADER_SIZE +1],p_hardware->Bufer_Rx[HEADER_SIZE +2]
							,p_hardware->Bufer_Rx[HEADER_SIZE +3],p_hardware->Bufer_Rx[HEADER_SIZE +4],p_hardware->Bufer_Rx[HEADER_SIZE +5]
							,p_hardware->Bufer_Rx[HEADER_SIZE +6],p_hardware->Bufer_Rx[HEADER_SIZE +7],p_hardware->Bufer_Rx[HEADER_SIZE +8]);
						try{
							com.Format(_T("UPDATE hardwares SET MAC_address='%s' WHERE id_hardware=%u"),text,ID);
							TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
							m_pCmd->ActiveConnection=::GetConnection();
							m_pCmd->CommandText=(LPCTSTR)com;
							m_pCmd->Execute(NULL, NULL, adCmdText);	
							StoreConfigMessage();
							
							sucses.LoadString(IDS_SUCSES);

							m_Grid.SetItemText(m_iSelectItem,C_MAC_ADDRESS,text);
						}
						catch(_com_error &e){
							::GenerateSqlError(e);
						}
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

void CWpanSetup::OnButtonSet() 
{
	// TODO: Add your control notification handler code here
	for(int i = R_HEADER; i < m_Grid.GetRowCount() ; i++){
		m_Grid.SetItemText(i,C_SUCSES,"");
	}

	CString enabled;
	enabled.LoadString(IDS_WORK);
	int old_iSelectItem = m_iSelectItem;
	for( m_iSelectItem = R_HEADER; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
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

void CWpanSetup::OnButtonGet() 
{
	// TODO: Add your control notification handler code here
	for(int i = R_HEADER; i < m_Grid.GetRowCount() ; i++){
		m_Grid.SetItemText(i,C_SUCSES,"");
	}

	CString enabled;
	enabled.LoadString(IDS_WORK);
	int old_iSelectItem = m_iSelectItem;
	for( m_iSelectItem = R_HEADER; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
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
}

void CWpanSetup::OnButtonGetMac() 
{
	// TODO: Add your control notification handler code here
	for(int i = R_HEADER; i < m_Grid.GetRowCount() ; i++){
		m_Grid.SetItemText(i,C_SUCSES,"");
	}

	CString enabled;
	enabled.LoadString(IDS_WORK);
	int old_iSelectItem = m_iSelectItem;
	for( m_iSelectItem = R_HEADER; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
		CString text;
		text=m_Grid.GetItemText(m_iSelectItem,C_STOP);
		if(enabled == text){
			OnButtonGetMac1();
		}
	}
	m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
	m_Grid.SetFocusCell(m_iSelectItem,0);
	m_Grid.RedrawWindow();
	m_Grid.SetFocus();
	m_Grid.EnsureVisible(m_iSelectItem,0);
}
