// LampSetup.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "LampSetup.h"

#include "DynamicLib.h"

#include "..\Free_Util_Dll\Util_Dll.h"
#include	"../../Grid1_0/NewCellTypes/GridCellCombo.h"
#include	"../../Grid1_0/NewCellTypes/GridCellNumeric.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define R_HEADER 1

#define C_NUMBER 0
#define C_MAC_ADDRESS 1
#define C_ROUTER 2
#define C_STREET 3
#define C_STREET_NUMBER 4
#define C_TYPE 5
#define C_LAT 6
#define C_LNG 7
#define C_BIT_GROUPS 8
#define C_TIME 9
#define C_WORK_HOURS 10
#define C_LAST_TEST 11
#define C_LABEL 12
#define MAX_COLUMS 13


/////////////////////////////////////////////////////////////////////////////
// CLampSetup property page

IMPLEMENT_DYNCREATE(CLampSetup, CPropertyPage)

CLampSetup::CLampSetup() : CPropertyPage(CLampSetup::IDD)
{
	m_iSelectItem=-1;
	fl_config=false;
	//{{AFX_DATA_INIT(CLampSetup)
	m_NewNumber = 1;
	m_iLight = 255;
	m_sSucses = _T("");
	//}}AFX_DATA_INIT
}

CLampSetup::~CLampSetup()
{
}

void CLampSetup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLampSetup)
	DDX_Control(pDX, IDC_BUTTON_LIGHT1, m_bLight1);
	DDX_Control(pDX, IDC_EDIT_NUMBER, m_bEditNumber);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_bNew);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_bDel);
	DDX_Control(pDX, IDC_BUTTON_LOAD_ROUTERS, m_bLoadRouter);
	DDX_Control(pDX, IDC_BUTTON_LOAD, m_bLoadLamp);
	DDX_Text(pDX, IDC_EDIT_NUMBER, m_NewNumber);
	DDV_MinMaxUInt(pDX, m_NewNumber, 1, 65535);
	DDX_Text(pDX, IDC_EDIT_LIGHTS, m_iLight);
	DDV_MinMaxInt(pDX, m_iLight, 1, 255);
	DDX_Text(pDX, IDC_STATIC_SUCSES, m_sSucses);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CLampSetup, CPropertyPage)
	//{{AFX_MSG_MAP(CLampSetup)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_PREVIEV, OnButtonPrintPreviev)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_ROUTERS, OnButtonLoadRouters)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_ROUTERS_REJECT, OnButtonLoadRoutersReject)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT1, OnButtonLight1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLampSetup message handlers

BOOL CLampSetup::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	BeginWaitCursor();
	CString text;

/*	CStringArray optStop;
	text.LoadString(IDS_WORK);
	optStop.Add(text);
	text.LoadString(IDS_NO_WORK);
	optStop.Add(text);*/
	
	CStringArray optRouters;
	_RecordsetPtr m_pRs=NULL;
	try{
		
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open("SELECT id_hardware,city_name,address_name FROM hardwares ORDER BY id_hardware",
			_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
		_variant_t index;
		index.vt = VT_I2;
		index.iVal = 0;
		optRouters.Add(_T(""));
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
		
			optRouters.Add(text);

			m_pRs->MoveNext();
		}
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}
	if (m_pRs)
		if (m_pRs->State == adStateOpen)
			m_pRs->Close();
			
	CStringArray optTypes;
	try{
		
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open("SELECT type_name FROM lamp_types ORDER BY id_type_lamp",
			_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
		_variant_t index;
		index.vt = VT_I2;
		index.iVal = 0;
		optTypes.Add(_T(""));
		_variant_t data;
		while(!m_pRs->EndOfFile){
			index.iVal = 0; data=m_pRs->Fields->GetItem(&index)->Value;
			text = ::GetStringFromVariant(data);	
				
			optTypes.Add(text);

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

	m_Grid.SetCellType(-1,C_NUMBER,RUNTIME_CLASS(CGridCellNumeric));
	m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_NUMBER);
	text.LoadString(IDS_NUMBER);
	m_Grid.SetColumnWidth(C_NUMBER,40);
	m_Grid.SetItemText(0,C_NUMBER,text);
	Item1.col=Item.col=C_NUMBER; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);


	text.LoadString(IDS_MAC_ADDRESS);
	m_Grid.SetColumnWidth(C_MAC_ADDRESS,120);
	m_Grid.SetItemText(0,C_MAC_ADDRESS,text);

	text.LoadString(IDS_ROUTER);
	m_Grid.SetColumnWidth(C_ROUTER,180);
	m_Grid.SetItemText(0,C_ROUTER,text);

	m_Grid.SetCellType(-1,C_ROUTER, RUNTIME_CLASS(CGridCellCombo));
	CGridCellCombo *pCell = (CGridCellCombo*) m_Grid.GetCell(-1,C_ROUTER);
  pCell->SetOptions(optRouters);
  pCell->SetStyle(CBS_DROPDOWNLIST);

	text.LoadString(IDS_STREET);
	m_Grid.SetColumnWidth(C_STREET,110);
	m_Grid.SetItemText(0,C_STREET,text);

	text.LoadString(IDS_STREET_NUMBER);
	m_Grid.SetColumnWidth(C_STREET_NUMBER,70);
	m_Grid.SetItemText(0,C_STREET_NUMBER,text);

	text.LoadString(IDS_TYPE_LAMP);
	m_Grid.SetColumnWidth(C_TYPE,180);
	m_Grid.SetItemText(0,C_TYPE,text);

	m_Grid.SetCellType(-1,C_TYPE, RUNTIME_CLASS(CGridCellCombo));
	pCell = (CGridCellCombo*) m_Grid.GetCell(-1,C_TYPE);
  pCell->SetOptions(optTypes);
  pCell->SetStyle(CBS_DROPDOWNLIST);

	text.LoadString(IDS_LAT);
	m_Grid.SetColumnWidth(C_LAT,100);
	m_Grid.SetItemText(0,C_LAT,text);
	Item1.col=Item.col=C_LAT; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_LNG);
	m_Grid.SetColumnWidth(C_LNG,100);
	m_Grid.SetItemText(0,C_LNG,text);
	Item1.col=Item.col=C_LNG; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_BIT_GROUPS);
	text.LoadString(IDS_GROUPS);
	m_Grid.SetColumnWidth(C_BIT_GROUPS,110);
	m_Grid.SetItemText(0,C_BIT_GROUPS,text);
	Item1.col=Item.col=C_BIT_GROUPS; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);
	

	text.LoadString(IDS_LAST_CONTACT);
	m_Grid.SetColumnWidth(C_TIME,120);
	m_Grid.SetItemText(0,C_TIME,text);

	m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_WORK_HOURS);
	text.LoadString(IDS_WORK_HOURS);
	m_Grid.SetColumnWidth(C_WORK_HOURS,70);
	m_Grid.SetItemText(0,C_WORK_HOURS,text);
	Item1.col=Item.col=C_WORK_HOURS; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_LAST_TEST);
	m_Grid.SetColumnWidth(C_LAST_TEST,120);
	m_Grid.SetItemText(0,C_LAST_TEST,text);

	text.LoadString(IDS_LABEL);
	m_Grid.SetColumnWidth(C_LABEL,120);
	m_Grid.SetItemText(0,C_LABEL,text);

	m_Grid.SetHeaderSort(true);
	
#ifndef _DEBUG
//	m_bLoadLamp.EnableWindow(false);
//	m_bLoadRouter.EnableWindow(false);
#endif
	return LoadData(true); 
}

BOOL CLampSetup::LoadData(bool fl_select)
{
	BOOL flag=true;
	CString text;
	_RecordsetPtr m_pRs=NULL;
	
  m_Grid.SetRowCount(R_HEADER);
	  

	_bstr_t strCom("SELECT id_lamp,lamps.MAC_address,lamps.id_hardware,hardwares.city_name,hardwares.address_name,street_name,street_number,lamps.id_type_lamp,type_name,lamps.lat,lamps.lng,last_contact,work_hours,last_test,bit_groups,label "
	"FROM lamps LEFT JOIN hardwares ON hardwares.id_hardware=lamps.id_hardware "
	"LEFT JOIN lamp_types ON lamp_types.id_type_lamp=lamps.id_type_lamp ORDER BY id_lamp");

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
				m_Grid.SetItemData(i,C_NUMBER,::GetLongFromVariant(data));	
				
				text.Format("%u",::GetLongFromVariant(data));
				m_Grid.SetItemText(i,C_NUMBER,text);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_MAC_ADDRESS,text);

				UINT32 id_hardware;
				text = "";
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				m_Grid.SetItemData(i,C_ROUTER,id_hardware = ::GetLongFromVariant(data)); 
				
				if(id_hardware){
					text = ::GetStringFromVariant(data);
					text+=' ';
				}

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				if(id_hardware){
					text += ::GetStringFromVariant(data);
					text+=' ';
				}

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				if(id_hardware)
					text += ::GetStringFromVariant(data);
				
				m_Grid.SetItemText(i,C_ROUTER,text);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_STREET,text);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_STREET_NUMBER,text);

				UINT32 id_type_lamp;
				text = "";
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				m_Grid.SetItemData(i,C_TYPE,id_type_lamp = ::GetLongFromVariant(data)); 
				
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				if(id_type_lamp){
					m_Grid.SetItemText(i,C_TYPE,::GetStringFromVariant(data));
				}

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_LAT,text);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_LNG,text);

				index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
				m_Grid.SetItemText(i,C_TIME,::TimeToSQL(data));//::GetStringFromVariant(data));

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text.Format("%u",::GetLongFromVariant(data));
				m_Grid.SetItemText(i,C_WORK_HOURS,text);

				index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
				m_Grid.SetItemText(i,C_LAST_TEST,::TimeToSQL(data));//::GetStringFromVariant(data));

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				UINT32 groups = ::GetLongFromVariant(data);
				text="0000000000000000";
				for(int j = 0;j<16;j++)
					if(groups & (0x8000>>j))
						text.SetAt(j,'1');
				m_Grid.SetItemText(i,C_BIT_GROUPS,text);
				m_Grid.SetItemData(i,C_BIT_GROUPS,groups);
				
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_LABEL,text);
	
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

void CLampSetup::EnableKeys()
{
	BOOL bNew,bDel;
	bNew=bDel=false;
	if(m_iSelectItem>=R_HEADER){
		bNew=bDel=true;
		if(m_Grid.GetItemData(m_iSelectItem,C_NUMBER)==0)
			bDel=false;
	}else
		bNew=true;
	m_bNew.EnableWindow(bNew);
	m_bDel.EnableWindow(bDel);
	m_bLight1.EnableWindow(bDel);
}

#include "LampGroups.h"

BOOL CLampSetup::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == (WPARAM)m_Grid.GetDlgCtrlID()){
    *pResult = 1;
    GV_DISPINFO *pDispInfo = (GV_DISPINFO*)lParam;
		switch(pDispInfo->hdr.code){
			case GVN_ENDLABELEDIT:
				{
					_CommandPtr m_pCmd=NULL;
					UINT32 ID=m_Grid.GetItemData(pDispInfo->item.row,C_NUMBER);
					CString com;
					CString text;
					try{
						switch(pDispInfo->item.col){
							case C_NUMBER:
								{
									UINT32 new_ID = atoi(m_Grid.GetItemText(pDispInfo->item.row,C_NUMBER));
									com.Format(_T("id_lamp=%u AND id_lamp != %u"),new_ID,ID);
									if(::GetSqlCount(_T("lamps"),com)==0){
										com.Format(_T("UPDATE lamps SET id_lamp=%u WHERE id_lamp=%u"),new_ID,ID);
										TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
										m_pCmd->ActiveConnection=::GetConnection();
										m_pCmd->CommandText=(LPCTSTR)com;
										m_pCmd->Execute(NULL, NULL, adCmdText);	
										StoreConfigMessage();
										m_Grid.SetItemData(pDispInfo->item.row,C_NUMBER,new_ID);
									}else{	
										com.Format(IDS_NUMBER_IS_BUSY,new_ID);
										AfxMessageBox(com);	
										m_Grid.SetItemText(pDispInfo->item.row,C_NUMBER,OldText);
									}
								}
								break;
							case C_MAC_ADDRESS:
								{
									text=m_Grid.GetItemText(pDispInfo->item.row,C_MAC_ADDRESS);
									::CStringSize(text,16);
									if(text.GetLength()!=16){
										AfxMessageBox(IDS_BAD_MAC);	
										m_Grid.SetItemText(pDispInfo->item.row,C_MAC_ADDRESS,OldText);
										break;
									}
									for( int i = 0; i<16 ;i++){
										if( ((text[i]>='0')&&(text[i]<='9')) || ((text[i]>='A')&&(text[i]<='F')) )
											;
										else{
											AfxMessageBox(IDS_BAD_MAC);	
											m_Grid.SetItemText(pDispInfo->item.row,C_MAC_ADDRESS,OldText);
											return true;
										}
									}

									com.Format(_T("MAC_address='%s' AND id_lamp != %u"),text,ID);
									if(::GetSqlCount(_T("lamps"),com)==0){
										com.Format(_T("UPDATE lamps SET MAC_address='%s' WHERE id_lamp=%u"),text,ID);
										TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
										m_pCmd->ActiveConnection=::GetConnection();
										m_pCmd->CommandText=(LPCTSTR)com;
										m_pCmd->Execute(NULL, NULL, adCmdText);	
										StoreConfigMessage();
										m_Grid.SetItemText(pDispInfo->item.row,C_MAC_ADDRESS,text);
									}else{	
										com.Format(IDS_NAME_IS,text);
										AfxMessageBox(com);	
										m_Grid.SetItemText(pDispInfo->item.row,C_MAC_ADDRESS,OldText);
									}
								}
								break;
							case C_ROUTER:
								{
									UINT32 id_hardware = 0;
									text = m_Grid.GetItemText(pDispInfo->item.row,C_ROUTER);
									sscanf(text,"%u",&id_hardware);

									com.Format(_T("UPDATE lamps SET id_hardware=%u WHERE id_lamp=%u"),id_hardware,ID);
									
									m_Grid.SetItemData(pDispInfo->item.row,C_ROUTER,id_hardware);

									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_STREET:
								{
									text=m_Grid.GetItemText(pDispInfo->item.row,C_STREET);
									::CStringSize(text,30);
									m_Grid.SetItemText(pDispInfo->item.row,C_STREET,text);
									com.Format(_T("UPDATE lamps SET street_name='%s' WHERE id_lamp=%u"),text,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_STREET_NUMBER:
								{
									text=m_Grid.GetItemText(pDispInfo->item.row,C_STREET_NUMBER);
									::CStringSize(text,30);
									m_Grid.SetItemText(pDispInfo->item.row,C_STREET_NUMBER,text);
									com.Format(_T("UPDATE lamps SET street_number='%s' WHERE id_lamp=%u"),text,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_TYPE:
								{
									text=m_Grid.GetItemText(pDispInfo->item.row,C_TYPE);
									if(text.IsEmpty())
										com.Format(_T("UPDATE lamps SET id_type_lamp=0 WHERE id_lamp=%u"),ID);
									else{
										_RecordsetPtr m_pRs=NULL;
										com.Format(_T("SELECT id_type_lamp FROM lamp_types WHERE type_name='%s'"),text);
										TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
										m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockOptimistic,adCmdUnknown);
										_variant_t index;
										index.vt = VT_I2;
										index.iVal = 0;
										_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
										com.Format(_T("UPDATE lamps SET id_type_lamp=%u WHERE id_lamp=%u"),
											::GetLongFromVariant(data),ID);
										m_pRs->Close();
									}
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_LAT:
								{
									double data=DoubleFormat(m_Grid.GetItemText(pDispInfo->item.row,C_LAT),-90,90);
									text.Format(_T("%10.6lf"),data);
									m_Grid.SetItemText(pDispInfo->item.row,C_LAT,text);
									com.Format(_T("UPDATE lamps SET lat='%s' WHERE id_lamp=%u"),text,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_LNG:
								{
									double data=DoubleFormat(m_Grid.GetItemText(pDispInfo->item.row,C_LNG),-180,180);
									text.Format(_T("%10.6lf"),data);
									m_Grid.SetItemText(pDispInfo->item.row,C_LNG,text);
									com.Format(_T("UPDATE lamps SET lng='%s' WHERE id_lamp=%u"),text,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_LABEL:
								{
									text=m_Grid.GetItemText(pDispInfo->item.row,C_LABEL);
									::CStringSize(text,30);
									m_Grid.SetItemText(pDispInfo->item.row,C_LABEL,text);
									com.Format(_T("UPDATE lamps SET label='%s' WHERE id_lamp=%u"),text,ID);
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
					case C_NUMBER:
					case C_MAC_ADDRESS:
					case C_ROUTER:
					case C_STREET:
					case C_STREET_NUMBER:
					case C_TYPE:
					case C_LAT:
					case C_LNG:
					case C_LABEL:
						break;
					case C_BIT_GROUPS:
						{
							CLampGroups CLG;
							CLG.id_lamp=m_Grid.GetItemData(pDispInfo->item.row,C_NUMBER);;
							CLG.groups=m_Grid.GetItemData(pDispInfo->item.row,C_BIT_GROUPS);;
							CLG.DoModal();
							StoreConfigMessage();
							_RecordsetPtr m_pRs=NULL;
							
							CString com;
							com.Format(_T("SELECT bit_groups FROM lamps WHERE id_lamp=%u"),m_Grid.GetItemData(pDispInfo->item.row,C_NUMBER));
							try{
								TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
								m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
								
								if(!m_pRs->EndOfFile){		
									_variant_t index;
									index.vt = VT_I2;
									index.iVal = 0;
									
									_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
									UINT32 groups = ::GetLongFromVariant(data);
									CString text="0000000000000000";
									for(int j = 0;j<16;j++)
										if(groups & (0x8000>>j))
											text.SetAt(j,'1');
									m_Grid.SetItemText(pDispInfo->item.row,C_BIT_GROUPS,text);
									m_Grid.SetItemData(pDispInfo->item.row,C_BIT_GROUPS,groups);	
								}		
							}
							catch(_com_error &e){
								::GenerateSqlError(e);
							}	
							if (m_pRs)
								if (m_pRs->State == adStateOpen)
									m_pRs->Close();
							m_Grid.RedrawWindow();
							*pResult =-1;
						}
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

void CLampSetup::StoreConfigMessage()
{
	if(!fl_config){
		fl_config=true;
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		ASSERT_VALID(pApp);
		::StoreDataConfigMessage(pApp->Id_PC,pApp->Id_Operator,_CONFIG_LAMPS);
	}
}

void CLampSetup::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrint(&m_Grid,this);
}

void CLampSetup::OnButtonPrintPreviev() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrintPreviev(&m_Grid,this);			
}

BOOL CLampSetup::ContinueModal()
{
	if (!IsWindowVisible())
		// send WM_IDLEUPDATECMDUI message to update toolbar state
		// This is normally called by OnIdle function in SDI or MSI applications.
		// Dialog based applications don't call OnIdle, so send the message from here instead
		AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
			(WPARAM)TRUE, 0, TRUE, TRUE);

	return CDialog::ContinueModal();
}

void CLampSetup::OnButtonLoad() 
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
		//AfxMessageBox("OK");
		//return;
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
		CString str=_T("");
		int opi = 0;
		int fl_first_line = 0;

		_CommandPtr m_pCmd=NULL;
		TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
		m_pCmd->ActiveConnection=::GetConnection();

		do{
			aFile.ReadString(str);
			if(!str.IsEmpty()){
//ново за прочитане на GPS координати Средец
				// N.B искат ODBC драйвер

				CString text = str;
				int counter = 0;
				int i;
				CString s_lamp_num,s_address,s_lat,s_lng;
				while(((i=text.Find(_T('\t')))>=0) && (counter < 6) ){
					CString num = text.Left(i);
					switch(counter){
					case 1:
						s_lamp_num = text.Left(i);
						break;
					case 3:
						s_address = text.Left(i);
						break;
					case 4:
						s_lat = text.Left(i);
						break;
					case 5:
						s_lng = text.Left(i);
						break;
					}

					text = text.Mid(i+1);
					counter ++;
				}
				if(fl_first_line == 0){
					fl_first_line = 1;
				}else{
					;
					try{
						if( !s_lamp_num.IsEmpty() ){
							CString com;
							if( ( s_lat.GetLength() > 5 ) && ( s_lng.GetLength() > 5 ) )
								com.Format(_T("UPDATE lamps SET street_name= '%s',lat = %s,lng = %s WHERE id_lamp=%s "),s_address,s_lat,s_lng,s_lamp_num);
							else
								com.Format(_T("UPDATE lamps SET street_name= '%s' WHERE id_lamp=%s "),s_address,s_lamp_num);
							
							m_pCmd->CommandText=(LPCTSTR)com;
							m_pCmd->Execute(NULL, NULL, adCmdText);	
						}
					}
					catch(_com_error &e){
						::GenerateSqlError(e);
					}
				}
			//	if( ++opi >10 )
			//		break;

/*	оригинално от 2015г
				CString id_lamp,MAC_address;
				int i;
				if((i=str.Find(_T('\t')))>0){
					id_lamp=str.Left(i);
					MAC_address = str.Mid(i+1);
					if((i=MAC_address.Find(_T('\t')))>0){
						MAC_address=MAC_address.Left(i);
					}
					//MAC_address = "00158D000083" + MAC_address;	//smal MAC
				}
				//::sscanf(id_lamp,"%d",&i); id_lamp.Format("%d",i+5000);	//101- 200 to 5101-5200
				_CommandPtr m_pCmd=NULL;
				
				try{
					CString com;
					com.Format(_T("id_lamp='%s'"),id_lamp);
					if(::GetSqlCount(_T("lamps"),com)==0){
						com.Format(_T("MAC_address='%s'"),MAC_address);
						if(::GetSqlCount(_T("lamps"),com)!=0){
							com.Format("MAC %s с номер %s е зает",MAC_address,id_lamp);
							AfxMessageBox(com);
							MAC_address = "0000000000000000";
						}
						com.Format(_T("INSERT INTO lamps(id_lamp,MAC_address) VALUES (%s,'%s')"),
							id_lamp,MAC_address);;
						TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
						m_pCmd->ActiveConnection=::GetConnection();
						m_pCmd->CommandText=(LPCTSTR)com;
						m_pCmd->Execute(NULL, NULL, adCmdText);	
					}else{
						com.Format("Номер %s е зает",id_lamp);
						AfxMessageBox(com);
					}
				}
				catch(_com_error &e){
					::GenerateSqlError(e);
				}*/
			}
		}while(!str.IsEmpty());
		aFile.Close();		
	}
	EndWaitCursor();
}

void CLampSetup::OnButtonLoadRouters() 
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
		BOOL fl_first_line = 0;
		int num_routers = 99;
		unsigned int routers[100];
		unsigned int lamps[100];
		int line_number = 0;
		do{
			aFile.ReadString(str);
			if(!str.IsEmpty()){
				CString text = str;
				int counter = 0;
				int i;
				line_number ++;
				int line_lenght = text.GetLength();
				int colun = 0;
				while(((i=text.Find(_T('\t')))>=0) && (counter < num_routers) ){
					CString num = text.Left(i);
					lamps[counter] = atoi(num);
					text = text.Mid(i+1);
					counter ++;
				}
				lamps[counter] = atoi(text);
				counter ++;
				if(fl_first_line == 0){
					fl_first_line = 1;
					num_routers = counter;
					memcpy(routers,lamps,sizeof(routers));
				}else{
					for(i = 0 ; i < num_routers; i++){
						if(lamps[i]){
							_CommandPtr m_pCmd=NULL;
							try{
								CString com;
								com.Format(_T("UPDATE lamps SET id_hardware=%d WHERE id_lamp=%d"),routers[i],lamps[i]);;
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
		}while(!str.IsEmpty());
		aFile.Close();		
	}
	EndWaitCursor();
}

void CLampSetup::OnButtonLoadRoutersReject() 
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

void CLampSetup::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(true))
		return;
			
	_CommandPtr m_pCmd=NULL;

	if( m_NewNumber == 0 ){
		m_bEditNumber.SetFocus();
		return;
	}

	try{
		CString com;
		com.Format(_T("id_lamp=%u"),m_NewNumber);
		if(::GetSqlCount(_T("lamps"),com)==0){
			com.Format(_T("INSERT INTO lamps(id_lamp) VALUES (%u)"),m_NewNumber);
			TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
			m_pCmd->ActiveConnection=::GetConnection();
			m_pCmd->CommandText=(LPCTSTR)com;
			m_pCmd->Execute(NULL, NULL, adCmdText);
			
			StoreConfigMessage();
 			LoadData();
		}else{
			com.Format(IDS_NUMBER_IS_BUSY,m_NewNumber);
			AfxMessageBox(com);
		}
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}
}

void CLampSetup::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	CString mes;
	mes.LoadString(IDS_DELETE);
	mes+=m_Grid.GetItemText(m_iSelectItem,C_NUMBER);
	if(AfxMessageBox(mes, MB_YESNO |	MB_DEFBUTTON2|MB_ICONQUESTION)==IDYES){
		_CommandPtr m_pCmd=NULL;
		try{
			UINT32 ID=m_Grid.GetItemData(m_iSelectItem,C_NUMBER);
			CString com;
			{
				com.Format(_T("DELETE FROM lamps WHERE id_lamp=%u"),ID);
				TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
				m_pCmd->ActiveConnection=::GetConnection();
				m_pCmd->CommandText=(LPCTSTR)com;
				m_pCmd->Execute(NULL, NULL, adCmdText);
				StoreConfigMessage();
				m_iSelectItem=-1;
				LoadData();
			}
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}
	}
}

void CLampSetup::OnButtonLight1() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(true))
		return;

	{
		
		{
			
			BeginWaitCursor();
			
			int recived=JennicModule.SetLampLight1(m_Grid.GetItemData(m_iSelectItem,C_NUMBER),m_iLight);

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
