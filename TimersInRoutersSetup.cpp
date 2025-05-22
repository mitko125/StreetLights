// TimersInRoutersSetup.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "TimersInRoutersSetup.h"


#include "DynamicLib.h"

#include "..\Free_Util_Dll\Util_Dll.h"
#include	"../../Grid1_0/NewCellTypes/GridCellCombo.h"
#include	"../../Grid1_0/NewCellTypes/GridCellNumeric.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ON_OFF_SIZE 40

#define R_HEADER 2

#define C_NUMBER 0
#define C_CITY 1
#define C_ADDRESS 2
#define C_SUCSES 3
#define C_TIMERS 4
#define C_TIME 5
#define C_ON 6
#define C_OFF 7
#define C_ON2 8
#define C_OFF2 9
#define C_STOP 10
#define MAX_COLUMS 11
/////////////////////////////////////////////////////////////////////////////
// CTimersInRoutersSetup property page


IMPLEMENT_DYNCREATE(CTimersInRoutersSetup, CPropertyPage)

CTimersInRoutersSetup::CTimersInRoutersSetup() : CPropertyPage(CTimersInRoutersSetup::IDD)
{
	m_iSelectItem=-1;
	fl_config=false;
	//{{AFX_DATA_INIT(CTimersInRoutersSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTimersInRoutersSetup::~CTimersInRoutersSetup()
{
}

void CTimersInRoutersSetup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimersInRoutersSetup)
	DDX_Control(pDX, IDC_BUTTON_GET1, m_bGet1);
	DDX_Control(pDX, IDC_BUTTON_SET1, m_bSet1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_Grid); 
}


BEGIN_MESSAGE_MAP(CTimersInRoutersSetup, CPropertyPage)
	//{{AFX_MSG_MAP(CTimersInRoutersSetup)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_PREVIEV, OnButtonPrintPreviev)
	ON_BN_CLICKED(IDC_BUTTON_SET1, OnButtonSet1)
	ON_BN_CLICKED(IDC_BUTTON_GET1, OnButtonGet1)
	ON_BN_CLICKED(IDC_BUTTON_SET, OnButtonSet)
	ON_BN_CLICKED(IDC_BUTTON_GET, OnButtonGet)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimersInRoutersSetup message handlers

BOOL CTimersInRoutersSetup::OnInitDialog() 
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
	
	CStringArray optTimers;
	_RecordsetPtr m_pRs=NULL;
	try{
		
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open("SELECT timer_name FROM timers ORDER BY id_timer",
			_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
		_variant_t index;
		index.vt = VT_I2;
		index.iVal = 0;
		optTimers.Add(_T(""));
		_variant_t data;
		while(!m_pRs->EndOfFile){
			data=m_pRs->Fields->GetItem(&index)->Value;
			optTimers.Add(::GetStringFromVariant(data));
			m_pRs->MoveNext();
		}
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}
	if (m_pRs)
		if (m_pRs->State == adStateOpen)
			m_pRs->Close();

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
	//Item1.nFormat=Item.nFormat=DT_CENTER|DT_VCENTER|DT_SINGLELINE;

	m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_NUMBER);
	text.LoadString(IDS_NUMBER);
	m_Grid.SetColumnWidth(C_NUMBER,40);
	m_Grid.SetItemText(0,C_NUMBER,text);
	m_Grid.SetMerge(0,C_NUMBER,1,0,false);
	Item1.col=Item.col=C_NUMBER; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	Item1.nFormat=Item.nFormat=DT_VCENTER|DT_SINGLELINE;

	text.LoadString(IDS_ADDRESS1);
	m_Grid.SetColumnWidth(C_CITY,90);
	m_Grid.SetItemText(0,C_CITY,text);
	m_Grid.SetMerge(0,C_CITY,1,0,false);
	Item1.col=Item.col=C_CITY; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_ADDRESS2);
	m_Grid.SetColumnWidth(C_ADDRESS,100);
	m_Grid.SetItemText(0,C_ADDRESS,text);
	m_Grid.SetMerge(0,C_ADDRESS,1,0,false);
	Item1.col=Item.col=C_ADDRESS; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_SUCSES);
	m_Grid.SetColumnWidth(C_SUCSES,50);
	m_Grid.SetItemText(0,C_SUCSES,text);
	m_Grid.SetMerge(0,C_SUCSES,1,0,false);
	Item1.col=Item.col=C_SUCSES; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);
	
	text.LoadString(IDS_TIMERS);
	m_Grid.SetColumnWidth(C_TIMERS,120);
	m_Grid.SetItemText(0,C_TIMERS,text);
	m_Grid.SetMerge(0,C_TIMERS,1,0,false);
	Item1.col=Item.col=C_TIMERS; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetCellType(-1,C_TIMERS, RUNTIME_CLASS(CGridCellCombo));
	CGridCellCombo *pCell = (CGridCellCombo*) m_Grid.GetCell(-1,C_TIMERS);
  pCell->SetOptions(optTimers);
  pCell->SetStyle(CBS_DROPDOWNLIST);


	text.LoadString(IDS_DATETIME);
	m_Grid.SetColumnWidth(C_TIME,140);
	m_Grid.SetItemText(0,C_TIME,text);
	m_Grid.SetMerge(0,C_TIME,1,0,false);
	Item1.col=Item.col=C_TIME; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	Item1.nFormat=Item.nFormat=DT_RIGHT|DT_VCENTER|DT_SINGLELINE;

	text.LoadString(IDS_ON_OFF_POWER);
	m_Grid.SetColumnWidth(C_ON,ON_OFF_SIZE+10);
	m_Grid.SetItemText(0,C_ON,text);
	Item1.col=Item.col=C_ON; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetColumnWidth(C_OFF,ON_OFF_SIZE+10);
	Item1.col=Item.col=C_OFF; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetMerge(0,C_ON,0,1,false);
	text.LoadString(IDS_ON);	m_Grid.SetItemText(1,C_ON,text);
	text.LoadString(IDS_OFF);	m_Grid.SetItemText(1,C_OFF,text);

	text.LoadString(IDS_ON_OFF_POWER2);
	m_Grid.SetColumnWidth(C_ON2,ON_OFF_SIZE+10);
	m_Grid.SetItemText(0,C_ON2,text);
	Item1.col=Item.col=C_ON2; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetColumnWidth(C_OFF2,ON_OFF_SIZE+10);
	Item1.col=Item.col=C_OFF2; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetMerge(0,C_ON2,0,1,false);
	text.LoadString(IDS_ON);	m_Grid.SetItemText(1,C_ON2,text);
	text.LoadString(IDS_OFF);	m_Grid.SetItemText(1,C_OFF2,text);

	Item1.nFormat=Item.nFormat=DT_VCENTER|DT_SINGLELINE;

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

BOOL CTimersInRoutersSetup::LoadData(bool fl_select)
{
	BOOL flag=true;
	CString text;
	_RecordsetPtr m_pRs=NULL;
	
  m_Grid.SetRowCount(R_HEADER);
	  

	_bstr_t strCom("SELECT id_hardware,number_hardware,city_name,address_name,timers.id_timer,timer_name,stop "
		"FROM hardwares LEFT JOIN timers ON hardwares.id_timer=timers.id_timer ORDER BY id_hardware");

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
				m_Grid.SetItemData(i,C_TIMERS,::GetLongFromVariant(data)); 

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				m_Grid.SetItemText(i,C_TIMERS,::GetStringFromVariant(data));
		
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

void CTimersInRoutersSetup::EnableKeys()
{
	BOOL bNew,bDel;
	bNew=bDel=false;
	if(m_iSelectItem>=R_HEADER){
		bNew=bDel=true;
		if(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER))==0)
			bDel=false;
	}else
		bNew=true;
//	m_bNew.EnableWindow(bNew);
//	m_bDel.EnableWindow(bDel);
	m_bSet1.EnableWindow(bDel);
	m_bGet1.EnableWindow(bDel);
}

BOOL CTimersInRoutersSetup::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
							case C_TIMERS:
								{
									UINT32 id_timer = 0;
									text = m_Grid.GetItemText(pDispInfo->item.row,C_TIMERS);
									if(!text.IsEmpty()){
										_RecordsetPtr m_pRs=NULL;
										com.Format(_T("SELECT id_timer FROM timers WHERE timer_name='%s'"),text);
										TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
										m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockOptimistic,adCmdUnknown);
										_variant_t index;
										index.vt = VT_I2;
										index.iVal = 0;
										_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
										id_timer = ::GetLongFromVariant(data);
										m_pRs->Close();
									}
									com.Format(_T("UPDATE hardwares SET id_timer=%u WHERE id_hardware=%u"),id_timer,ID);
									
									m_Grid.SetItemData(pDispInfo->item.row,C_TIMERS,id_timer);

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
						m_Grid.SetItemText(pDispInfo->item.row,pDispInfo->item.col,OldText);
					}
					m_Grid.RedrawRow(pDispInfo->item.row);
				}
				return true;
			case GVN_BEGINLABELEDIT:
				OldText=m_Grid.GetItemText(pDispInfo->item.row,pDispInfo->item.col);
				switch(pDispInfo->item.col){
				case C_TIMERS:
				case C_STOP:
					break;
				default:
					*pResult =-1;
					break;
				}
				return true;
			case GVN_SELCHANGED:
				if(pDispInfo->item.row>=R_HEADER){
					m_iSelectItem=pDispInfo->item.row;
				}else
					m_iSelectItem=-1;	
				EnableKeys();
				return true;
		}
	}
	return CPropertyPage::OnNotify(wParam, lParam, pResult);
}

void CTimersInRoutersSetup::StoreConfigMessage()
{
	if(!fl_config){
		fl_config=true;
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		ASSERT_VALID(pApp);
		::StoreDataConfigMessage(pApp->Id_PC,pApp->Id_Operator,_CONFIG_ROUTERS);
	}
}

void CTimersInRoutersSetup::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrint(&m_Grid,this);
}

void CTimersInRoutersSetup::OnButtonPrintPreviev() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrintPreviev(&m_Grid,this);			
}

BOOL CTimersInRoutersSetup::ContinueModal()
{
	if (!IsWindowVisible())
		// send WM_IDLEUPDATECMDUI message to update toolbar state
		// This is normally called by OnIdle function in SDI or MSI applications.
		// Dialog based applications don't call OnIdle, so send the message from here instead
		AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
			(WPARAM)TRUE, 0, TRUE, TRUE);

	return CDialog::ContinueModal();
}

void CTimersInRoutersSetup::OnButtonSet1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_TIME,"");
	m_Grid.SetItemText(m_iSelectItem,C_ON,"");
	m_Grid.SetItemText(m_iSelectItem,C_OFF,"");
	m_Grid.SetItemText(m_iSelectItem,C_ON2,"");
	m_Grid.SetItemText(m_iSelectItem,C_OFF2,"");
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString text;
		{
			BeginWaitCursor();

			struct in_addr adr;
			adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);
			
			int recived=JennicModule.SetTimers(inet_ntoa(adr),m_Grid.GetItemData(m_iSelectItem,C_TIMERS));
			text.LoadString(IDS_ERROR);
			if( recived >= (int)(HEADER_SIZE+1+6) ){
				if( p_hardware->Bufer_Rx[HEADER_SIZE] == SEND_DATE_TIME ){
					text.LoadString(IDS_SUCSES);
					COleDateTime time( BCD_INT(*(p_hardware->Bufer_Rx + HEADER_SIZE + 6))+2000, BCD_INT(*(p_hardware->Bufer_Rx + HEADER_SIZE +5)),BCD_INT(*(p_hardware->Bufer_Rx + HEADER_SIZE + 4)),
						BCD_INT(*(p_hardware->Bufer_Rx + HEADER_SIZE + 3)),BCD_INT(*(p_hardware->Bufer_Rx + HEADER_SIZE + 2)),BCD_INT(*(p_hardware->Bufer_Rx + HEADER_SIZE + 1)));
					m_Grid.SetItemText(m_iSelectItem,C_TIME,::TimeToSQL(time));
				}
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

void CTimersInRoutersSetup::OnButtonGet1() 
{
	// TODO: Add your control notification handler code here
	//index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
	//				pGrid->SetItemText(i,C_TIME,::GetStringFromVariant(data));

	m_Grid.SetItemText(m_iSelectItem,C_TIME,"");
	m_Grid.SetItemText(m_iSelectItem,C_ON,"");
	m_Grid.SetItemText(m_iSelectItem,C_OFF,"");
	m_Grid.SetItemText(m_iSelectItem,C_ON2,"");
	m_Grid.SetItemText(m_iSelectItem,C_OFF2,"");
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString sucses;
		{
			BeginWaitCursor();
			
			sucses.LoadString(IDS_ERROR);

			struct in_addr adr;
			adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);

			int recived=JennicModule.GetTimers(inet_ntoa(adr));
			
			if( recived >= (int)( HEADER_SIZE + 1 + sizeof(tsTimers) -(4 /*V2 понеже V1 беше по-къс*/)) ){
				if( p_hardware->Bufer_Rx[HEADER_SIZE] == 	COMMAND_SET_TIMERS ){
					sucses.LoadString(IDS_SUCSES);
					
					tsTimers sTimers;
					memset(&sTimers,0,sizeof(sTimers));

					memcpy(&sTimers,(tsTimers*)( p_hardware->Bufer_Rx + HEADER_SIZE + 1),recived - (HEADER_SIZE + 1) );

					COleDateTime time( BCD_INT(sTimers.sDateTime.date_time[5])+2000, BCD_INT(sTimers.sDateTime.date_time[4]),BCD_INT(sTimers.sDateTime.date_time[3]),
						BCD_INT(sTimers.sDateTime.date_time[2]),BCD_INT(sTimers.sDateTime.date_time[1]),BCD_INT(sTimers.sDateTime.date_time[0]));

					m_Grid.SetItemText(m_iSelectItem,C_TIME,::TimeToSQL(time));

					CString text;
					text.Format("%u:%02u",BCD_INT(sTimers.sTimerOn1.u8Hour),BCD_INT(sTimers.sTimerOn1.u8Minute));
					m_Grid.SetItemText(m_iSelectItem,C_ON,text);

					text.Format("%u:%02u",BCD_INT(sTimers.sTimerOff1.u8Hour),BCD_INT(sTimers.sTimerOff1.u8Minute));
					m_Grid.SetItemText(m_iSelectItem,C_OFF,text);

					text.Format("%u:%02u",BCD_INT(sTimers.sTimerOn2.u8Hour),BCD_INT(sTimers.sTimerOn2.u8Minute));
					m_Grid.SetItemText(m_iSelectItem,C_ON2,text);

					text.Format("%u:%02u",BCD_INT(sTimers.sTimerOff2.u8Hour),BCD_INT(sTimers.sTimerOff2.u8Minute));
					m_Grid.SetItemText(m_iSelectItem,C_OFF2,text);
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

void CTimersInRoutersSetup::OnButtonSet() 
{
	// TODO: Add your control notification handler code here
	for(int i = R_HEADER; i < m_Grid.GetRowCount() ; i++){
		m_Grid.SetItemText(i,C_TIME,"");
		m_Grid.SetItemText(i,C_ON,"");
		m_Grid.SetItemText(i,C_OFF,"");
		m_Grid.SetItemText(i,C_ON2,"");
		m_Grid.SetItemText(i,C_OFF2,"");
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

void CTimersInRoutersSetup::OnButtonGet() 
{
	// TODO: Add your control notification handler code here
	for(int i = R_HEADER; i < m_Grid.GetRowCount() ; i++){
		m_Grid.SetItemText(i,C_TIME,"");
		m_Grid.SetItemText(i,C_ON,"");
		m_Grid.SetItemText(i,C_OFF,"");
		m_Grid.SetItemText(i,C_ON2,"");
		m_Grid.SetItemText(i,C_OFF2,"");
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
