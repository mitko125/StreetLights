// EnergyMeters.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "EnergyMeters.h"
#include "DateTimeLimits.h"

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
#define C_STOP 4
#define MAX_COLUMS 5

/////////////////////////////////////////////////////////////////////////////
// CEnergyMeters property page

IMPLEMENT_DYNCREATE(CEnergyMeters, CPropertyPage)

CEnergyMeters::CEnergyMeters() : CPropertyPage(CEnergyMeters::IDD)
{
	m_iSelectItem=-1;
	fl_config=false;
	//{{AFX_DATA_INIT(CEnergyMeters)
	//}}AFX_DATA_INIT
}

CEnergyMeters::~CEnergyMeters()
{
}

void CEnergyMeters::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEnergyMeters)
	DDX_Control(pDX, IDC_BUTTON_TOTAL1, m_bTotal1);
	DDX_Control(pDX, IDC_BUTTON_HISTORY1, m_bHistory1);
	DDX_Control(pDX, IDC_BUTTON_CURRENT1, m_bCurrent1);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CEnergyMeters, CPropertyPage)
	//{{AFX_MSG_MAP(CEnergyMeters)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_PREVIEV, OnButtonPrintPreviev)
	ON_BN_CLICKED(IDC_BUTTON_CURRENT1, OnButtonCurrent1)
	ON_BN_CLICKED(IDC_BUTTON_CURRENT, OnButtonCurrent)
	ON_BN_CLICKED(IDC_BUTTON_TOTAL1, OnButtonTotal1)
	ON_BN_CLICKED(IDC_BUTTON_TOTAL, OnButtonTotal)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY1, OnButtonHistory1)
	ON_BN_CLICKED(IDC_BUTTON_GET_HISTORY, OnButtonHistory)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEnergyMeters message handlers

BOOL CEnergyMeters::ContinueModal()
{
	if (!IsWindowVisible())
		// send WM_IDLEUPDATECMDUI message to update toolbar state
		// This is normally called by OnIdle function in SDI or MSI applications.
		// Dialog based applications don't call OnIdle, so send the message from here instead
		AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
			(WPARAM)TRUE, 0, TRUE, TRUE);

	return CDialog::ContinueModal();
}

void CEnergyMeters::EnableKeys()
{
	BOOL bNew,bDel;
	bNew=bDel=false;
	if(m_iSelectItem>=1){
		bNew=bDel=true;
		if(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER))==0)
			bDel=false;
	}else
		bNew=true;
	m_bCurrent1.EnableWindow(bDel);
	m_bTotal1.EnableWindow(bDel);
	m_bHistory1.EnableWindow(bDel);
}

BOOL CEnergyMeters::OnInitDialog() 
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
	
	text.LoadString(IDS_STOP);
	m_Grid.SetColumnWidth(C_STOP,80);
	m_Grid.SetItemText(0,C_STOP,text);

	m_Grid.SetCellType(-1,C_STOP, RUNTIME_CLASS(CGridCellCombo));
	CGridCellCombo *pCell = (CGridCellCombo*) m_Grid.GetCell(-1,C_STOP);
  pCell->SetOptions(optStop);
  pCell->SetStyle(CBS_DROPDOWNLIST);

	m_Grid.SetFixedColumnSelection(TRUE);
	m_Grid.SetHeaderSort(true);
	
	return LoadData(true); 
}

BOOL CEnergyMeters::LoadData(bool fl_select)
{
	BOOL flag=true;
	CString text;
	_RecordsetPtr m_pRs=NULL;
	
  m_Grid.SetRowCount(1);
	  
	_bstr_t strCom("SELECT id_hardware,number_hardware,city_name,address_name,stop FROM hardwares ORDER BY id_hardware");
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

BOOL CEnergyMeters::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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

void CEnergyMeters::StoreConfigMessage()
{
	if(!fl_config){
		fl_config=true;
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		ASSERT_VALID(pApp);
		::StoreDataConfigMessage(pApp->Id_PC,pApp->Id_Operator,_CONFIG_ENERGY_METERS);
	}
}

void CEnergyMeters::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrint(&m_Grid,this);
}

void CEnergyMeters::OnButtonPrintPreviev() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrintPreviev(&m_Grid,this);		
}

UINT CEnergyMeters::GetLampCount(UINT id_router)
{
	UINT cou = 0;

	CString com;

	com.Format(_T("id_hardware=%u"),id_router);
	try{
		cou = ::GetSqlCount(_T("lamps"),com);
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}
	return cou;
}

float CEnergyMeters::GetCalculatedPawer(UINT id_router)
{
	float power = 0;

	_RecordsetPtr m_pRs=NULL;

	CString com;
	com.Format(_T("SELECT SUM(power_W) FROM lamps ")
		_T("LEFT JOIN lamp_types ON lamp_types.id_type_lamp=lamps.id_type_lamp ")
		_T("WHERE lamps.id_hardware = %u"),id_router);
	try{
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *)GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
				
		_variant_t index;
		index.vt = VT_I2;
		_variant_t data;
		if(!m_pRs->EndOfFile){		
			index.iVal = 0; data=m_pRs->Fields->GetItem(&index)->Value;
			power = (float)::GetDoubleFromVariant(data);
		}
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}
	return power/1000;
}

#include "DisabledLamps.h"

void CEnergyMeters::OnButtonCurrent1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString sucses;
		{
			BeginWaitCursor();
			
			CString to_edit,tmp;

			UINT cou_lamp = GetLampCount(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
			float calculated_power = GetCalculatedPawer(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));

			tmp.Format(IDS_CURRENT_ENERGY,m_Grid.GetItemText(m_iSelectItem,C_CITY),m_Grid.GetItemText(m_iSelectItem,C_ADDRESS),
				cou_lamp,calculated_power);
			to_edit += tmp;
			
			tmp.Format("enable_energy_meter != 0 AND id_hardware = %d",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
			if( ::GetSqlCount(_T("hardwares"),tmp) ){
				
				sucses.LoadString(IDS_ERROR);
				
				struct in_addr adr;
				adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);
				
				int recived=JennicModule.GetCurrentEnergy(inet_ntoa(adr));
				
				if( recived >= (int)( HEADER_SIZE + 1 + sizeof(tsCurrentEnergy) ) ){
					if( p_hardware->Bufer_Rx[HEADER_SIZE] == 	COMMAND_READ_CURRENT_ENERGY ){
						tsCurrentEnergy * psCurrentEnergy = (tsCurrentEnergy *)(p_hardware->Bufer_Rx + HEADER_SIZE + 1);
						
						tsDateTime * psDateTime = (tsDateTime *)(&psCurrentEnergy->sLastContact);
						COleDateTime time( BCD_INT(psDateTime->date_time[5])+2000, BCD_INT(psDateTime->date_time[4]),BCD_INT(psDateTime->date_time[3]),
							BCD_INT(psDateTime->date_time[2]),BCD_INT(psDateTime->date_time[1]),BCD_INT(psDateTime->date_time[0]));
						tmp.LoadString(IDS_LAST_CONTACT);
						tmp += ":\t";
						tmp += ::TimeToSQL(time);
						tmp += "\r\n";
						to_edit += tmp;
						
						tmp.Format(IDS_CURRENT_ENERGY1,psCurrentEnergy->Grid_frequency);
						to_edit += tmp;
						
						tmp.Format(IDS_CURRENT_ENERGY2,psCurrentEnergy->L1_Voltage,psCurrentEnergy->L2_Voltage,psCurrentEnergy->L3_Voltage);
						to_edit += tmp;
						
						tmp.Format(IDS_CURRENT_ENERGY3,psCurrentEnergy->L1_Current,psCurrentEnergy->L2_Current,psCurrentEnergy->L3_Current);
						to_edit += tmp;
						
						tmp.Format(IDS_CURRENT_ENERGY4,psCurrentEnergy->Active_power,psCurrentEnergy->L1_Active_power,
							psCurrentEnergy->L2_Active_power,psCurrentEnergy->L3_Active_power);
						to_edit += tmp;
						
						tmp.Format(IDS_CURRENT_ENERGY5,psCurrentEnergy->Reactive_power,psCurrentEnergy->L1_Reactive_power,
							psCurrentEnergy->L2_Reactive_power,psCurrentEnergy->L3_Reactive_power);
						to_edit += tmp;
						
						tmp.Format(IDS_CURRENT_ENERGY6,psCurrentEnergy->Apparent_power,psCurrentEnergy->L1_Apparent_power,
							psCurrentEnergy->L2_Apparent_power,psCurrentEnergy->L3_Apparent_power);
						to_edit += tmp;
						
						tmp.Format(IDS_CURRENT_ENERGY7,psCurrentEnergy->Power_factor,psCurrentEnergy->L1_Power_factor,
							psCurrentEnergy->L2_Power_factor,psCurrentEnergy->L3_Power_factor);
						to_edit += tmp;
						
						//	AfxMessageBox(text);
						sucses.LoadString(IDS_SUCSES);
					}
				}
			}
			m_Grid.SetItemText(m_iSelectItem,C_SUCSES,sucses);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);

			tmp.LoadString(IDS_ERROR);
			if( tmp == sucses ){
				tmp.Format("%s\r\n",sucses);
				to_edit += tmp;
			}
			CDisabledLamps DL(IDS_CURRENT_ENERGY_NAME,IDS_CURRENT_ENERGY_FILE);
			DL.text = to_edit;
			DL.DoModal();
		}
	}
	EndWaitCursor();
}

void CEnergyMeters::OnButtonCurrent() 
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
						
			int old_iSelectItem = m_iSelectItem;
		
			UINT total_cou_lamp = 0;
			float total_calculated_power = 0;
			float active = 0;
			float reactive = 0;
			float apparent = 0;

			for( m_iSelectItem = 1; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
				CString text;
				text=m_Grid.GetItemText(m_iSelectItem,C_STOP);
				if(enabled == text){
					UINT cou_lamp = GetLampCount(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
					float calculated_power = GetCalculatedPawer(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
					total_cou_lamp += cou_lamp;
					total_calculated_power += calculated_power;

					tmp.Format(IDS_CURRENT_ENERGY,m_Grid.GetItemText(m_iSelectItem,C_CITY),m_Grid.GetItemText(m_iSelectItem,C_ADDRESS),
						cou_lamp,calculated_power);
					to_edit += tmp;

					sucses = "";
					tmp.Format("enable_energy_meter != 0 AND id_hardware = %d",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
					if( ::GetSqlCount(_T("hardwares"),tmp) ){
						sucses.LoadString(IDS_ERROR);
						
						struct in_addr adr;
						adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);
						
						int recived=JennicModule.GetCurrentEnergy(inet_ntoa(adr));
						
						if( recived >= (int)( HEADER_SIZE + 1 + sizeof(tsCurrentEnergy) ) ){
							if( p_hardware->Bufer_Rx[HEADER_SIZE] == 	COMMAND_READ_CURRENT_ENERGY ){
								tsCurrentEnergy * psCurrentEnergy = (tsCurrentEnergy *)(p_hardware->Bufer_Rx + HEADER_SIZE + 1);
								
								tsDateTime * psDateTime = (tsDateTime *)(&psCurrentEnergy->sLastContact);
								COleDateTime time( BCD_INT(psDateTime->date_time[5])+2000, BCD_INT(psDateTime->date_time[4]),BCD_INT(psDateTime->date_time[3]),
									BCD_INT(psDateTime->date_time[2]),BCD_INT(psDateTime->date_time[1]),BCD_INT(psDateTime->date_time[0]));
								tmp.LoadString(IDS_LAST_CONTACT);
								tmp += ":\t";
								tmp += ::TimeToSQL(time);
								tmp += "\r\n";
								to_edit += tmp;
								
								tmp.Format(IDS_CURRENT_ENERGY1,psCurrentEnergy->Grid_frequency);
								to_edit += tmp;
								
								tmp.Format(IDS_CURRENT_ENERGY2,psCurrentEnergy->L1_Voltage,psCurrentEnergy->L2_Voltage,psCurrentEnergy->L3_Voltage);
								to_edit += tmp;
								
								tmp.Format(IDS_CURRENT_ENERGY3,psCurrentEnergy->L1_Current,psCurrentEnergy->L2_Current,psCurrentEnergy->L3_Current);
								to_edit += tmp;
								
								tmp.Format(IDS_CURRENT_ENERGY4,psCurrentEnergy->Active_power,psCurrentEnergy->L1_Active_power,
									psCurrentEnergy->L2_Active_power,psCurrentEnergy->L3_Active_power);
								to_edit += tmp;
								
								active += psCurrentEnergy->Active_power;
								
								tmp.Format(IDS_CURRENT_ENERGY5,psCurrentEnergy->Reactive_power,psCurrentEnergy->L1_Reactive_power,
									psCurrentEnergy->L2_Reactive_power,psCurrentEnergy->L3_Reactive_power);
								to_edit += tmp;
								
								reactive += psCurrentEnergy->Reactive_power;
								
								tmp.Format(IDS_CURRENT_ENERGY6,psCurrentEnergy->Apparent_power,psCurrentEnergy->L1_Apparent_power,
									psCurrentEnergy->L2_Apparent_power,psCurrentEnergy->L3_Apparent_power);
								to_edit += tmp;
								
								apparent += psCurrentEnergy->Apparent_power;
								
								tmp.Format(IDS_CURRENT_ENERGY7,psCurrentEnergy->Power_factor,psCurrentEnergy->L1_Power_factor,
									psCurrentEnergy->L2_Power_factor,psCurrentEnergy->L3_Power_factor);
								to_edit += tmp;	
								
								//	AfxMessageBox(text);
								sucses.LoadString(IDS_SUCSES);
							}
						}
					}
					m_Grid.SetItemText(m_iSelectItem,C_SUCSES,sucses);
					m_Grid.SetFocusCell(m_iSelectItem,0);
					m_Grid.RedrawWindow();
					m_Grid.SetFocus();
					m_Grid.EnsureVisible(m_iSelectItem,0);
					
					tmp.LoadString(IDS_ERROR);
					if( tmp == sucses ){
						tmp.Format("%s\r\n",sucses);
						to_edit += tmp;
					}

					to_edit += "\r\n\r\n";		

				}
			}

			tmp.Format(IDS_TOTAL_CALCULATED,total_cou_lamp,total_calculated_power);
			to_edit += tmp;
			tmp.Format(IDS_CURRENT_ENERGY8,active);
			to_edit += tmp;
			tmp.Format(IDS_CURRENT_ENERGY9,reactive);
			to_edit += tmp;
			tmp.Format(IDS_CURRENT_ENERGY10,apparent);
			to_edit += tmp;

			m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);

			CDisabledLamps DL(IDS_CURRENT_ENERGY_NAME,IDS_CURRENT_ENERGY_FILE);
			DL.text = to_edit;
			DL.DoModal();
		}
	}
	EndWaitCursor();
}

void CEnergyMeters::OnButtonTotal1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString sucses;
		{
			BeginWaitCursor();
			
			CString to_edit,tmp;

			UINT cou_lamp = GetLampCount(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
			float calculated_power = GetCalculatedPawer(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));

			tmp.Format(IDS_TOTAL_ENERGY,m_Grid.GetItemText(m_iSelectItem,C_CITY),m_Grid.GetItemText(m_iSelectItem,C_ADDRESS),
				cou_lamp,calculated_power);
			to_edit += tmp;

			tmp.Format("enable_energy_meter != 0 AND id_hardware = %d",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
			if( ::GetSqlCount(_T("hardwares"),tmp) ){
				sucses.LoadString(IDS_ERROR);
				
				struct in_addr adr;
				adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);
				
				int recived=JennicModule.GetTotaltEnergy(inet_ntoa(adr));
				
				if( recived >= (int)( HEADER_SIZE + 1 + sizeof(tsTotalEnergy) ) ){
					if( p_hardware->Bufer_Rx[HEADER_SIZE] == 	COMMAND_READ_TOTAL_ENERGY ){
						tsTotalEnergy * psTotalEnergy = (tsTotalEnergy *)(p_hardware->Bufer_Rx + HEADER_SIZE + 1);
						
						tsDateTime * psDateTime = (tsDateTime *)(&psTotalEnergy->sLastContact);
						COleDateTime time( BCD_INT(psDateTime->date_time[5])+2000, BCD_INT(psDateTime->date_time[4]),BCD_INT(psDateTime->date_time[3]),
							BCD_INT(psDateTime->date_time[2]),BCD_INT(psDateTime->date_time[1]),BCD_INT(psDateTime->date_time[0]));
						tmp.LoadString(IDS_LAST_CONTACT);
						tmp += ":\t";
						tmp += ::TimeToSQL(time);
						tmp += "\r\n";
						to_edit += tmp;
						
						tmp.Format(IDS_TOTAL_ENERGY1,psTotalEnergy->Total_active_energy,psTotalEnergy->T1_Total_active_energy,
							psTotalEnergy->T2_Total_active_energy);
						to_edit += tmp;
						
						tmp.Format(IDS_TOTAL_ENERGY2,psTotalEnergy->L1_Total_active_energy,psTotalEnergy->L2_Total_active_energy,
							psTotalEnergy->L3_Total_active_energy);
						to_edit += tmp;
						
						tmp.Format(IDS_TOTAL_ENERGY3,psTotalEnergy->Total_reactive_energy,psTotalEnergy->T1_Total_reactive_energy,
							psTotalEnergy->T2_Total_reactive_energy);
						to_edit += tmp;
						
						tmp.Format(IDS_TOTAL_ENERGY4,psTotalEnergy->L1_Total_reactive_energy,psTotalEnergy->L2_Total_reactive_energy,
							psTotalEnergy->L3_Total_reactive_energy);
						to_edit += tmp;
						
						//	AfxMessageBox(text);
						sucses.LoadString(IDS_SUCSES);
					}
				}
			}
			m_Grid.SetItemText(m_iSelectItem,C_SUCSES,sucses);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);

			tmp.LoadString(IDS_ERROR);
			if( tmp == sucses ){
				tmp.Format("%s\r\n",sucses);
				to_edit += tmp;
			}
			CDisabledLamps DL(IDS_TOTAL_ENERGY_NAME,IDS_TOTAL_ENERGY_FILE);
			DL.text = to_edit;
			DL.DoModal();
		}
	}
	EndWaitCursor();
}

void CEnergyMeters::OnButtonTotal() 
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
						
			int old_iSelectItem = m_iSelectItem;
			
			UINT total_cou_lamp = 0;
			float total_calculated_power = 0;
			float Total_active_energy = 0;
			float T1_Total_active_energy = 0;
			float T2_Total_active_energy = 0;
			float Total_reactive_energy = 0;
			float T1_Total_reactive_energy = 0;
			float T2_Total_reactive_energy = 0;

			for( m_iSelectItem = 1; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
				CString text;
				text=m_Grid.GetItemText(m_iSelectItem,C_STOP);
				if(enabled == text){
					UINT cou_lamp = GetLampCount(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
					float calculated_power = GetCalculatedPawer(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
					total_cou_lamp += cou_lamp;
					total_calculated_power += calculated_power;

					tmp.Format(IDS_TOTAL_ENERGY,m_Grid.GetItemText(m_iSelectItem,C_CITY),m_Grid.GetItemText(m_iSelectItem,C_ADDRESS),
						cou_lamp,calculated_power);
					to_edit += tmp;
					
					sucses = "";
					tmp.Format("enable_energy_meter != 0 AND id_hardware = %d",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
					if( ::GetSqlCount(_T("hardwares"),tmp) ){
						sucses.LoadString(IDS_ERROR);
						
						struct in_addr adr;
						adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);
						
						int recived=JennicModule.GetTotaltEnergy(inet_ntoa(adr));
						
						if( recived >= (int)( HEADER_SIZE + 1 + sizeof(tsTotalEnergy) ) ){
							if( p_hardware->Bufer_Rx[HEADER_SIZE] == 	COMMAND_READ_TOTAL_ENERGY ){
								tsTotalEnergy * psTotalEnergy = (tsTotalEnergy *)(p_hardware->Bufer_Rx + HEADER_SIZE + 1);
								
								tsDateTime * psDateTime = (tsDateTime *)(&psTotalEnergy->sLastContact);
								COleDateTime time( BCD_INT(psDateTime->date_time[5])+2000, BCD_INT(psDateTime->date_time[4]),BCD_INT(psDateTime->date_time[3]),
									BCD_INT(psDateTime->date_time[2]),BCD_INT(psDateTime->date_time[1]),BCD_INT(psDateTime->date_time[0]));
								tmp.LoadString(IDS_LAST_CONTACT);
								tmp += ":\t";
								tmp += ::TimeToSQL(time);
								tmp += "\r\n";
								to_edit += tmp;
								
								tmp.Format(IDS_TOTAL_ENERGY1,psTotalEnergy->Total_active_energy,psTotalEnergy->T1_Total_active_energy,
									psTotalEnergy->T2_Total_active_energy);
								to_edit += tmp;
								Total_active_energy += psTotalEnergy->Total_active_energy;
								T1_Total_active_energy += psTotalEnergy->T1_Total_active_energy;
								T2_Total_active_energy += psTotalEnergy->T2_Total_active_energy;
								
								tmp.Format(IDS_TOTAL_ENERGY2,psTotalEnergy->L1_Total_active_energy,psTotalEnergy->L2_Total_active_energy,
									psTotalEnergy->L3_Total_active_energy);
								to_edit += tmp;
								
								tmp.Format(IDS_TOTAL_ENERGY3,psTotalEnergy->Total_reactive_energy,psTotalEnergy->T1_Total_reactive_energy,
									psTotalEnergy->T2_Total_reactive_energy);
								to_edit += tmp;
								Total_reactive_energy += psTotalEnergy->Total_reactive_energy;
								T1_Total_reactive_energy += psTotalEnergy->T1_Total_reactive_energy;
								T2_Total_reactive_energy += psTotalEnergy->T2_Total_reactive_energy;
								
								tmp.Format(IDS_TOTAL_ENERGY4,psTotalEnergy->L1_Total_reactive_energy,psTotalEnergy->L2_Total_reactive_energy,
									psTotalEnergy->L3_Total_reactive_energy);
								to_edit += tmp;
								
								//	AfxMessageBox(text);
								sucses.LoadString(IDS_SUCSES);
							}
						}
					}
					m_Grid.SetItemText(m_iSelectItem,C_SUCSES,sucses);
					m_Grid.SetFocusCell(m_iSelectItem,0);
					m_Grid.RedrawWindow();
					m_Grid.SetFocus();
					m_Grid.EnsureVisible(m_iSelectItem,0);
					
					tmp.LoadString(IDS_ERROR);
					if( tmp == sucses ){
						tmp.Format("%s\r\n",sucses);
						to_edit += tmp;
					}

					to_edit += "\r\n\r\n";		

				}
			}

			tmp.Format(IDS_TOTAL_CALCULATED,total_cou_lamp,total_calculated_power);
			to_edit += tmp;
			tmp.Format(IDS_TOTAL_ENERGY1,Total_active_energy,T1_Total_active_energy,T2_Total_active_energy);
			to_edit += tmp;
			tmp.Format(IDS_TOTAL_ENERGY3,Total_reactive_energy,T1_Total_reactive_energy,T2_Total_reactive_energy);
			to_edit += tmp;

			m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);

			CDisabledLamps DL(IDS_TOTAL_ENERGY_NAME,IDS_TOTAL_ENERGY_FILE);
			DL.text = to_edit;
			DL.DoModal();
		}
	}
	EndWaitCursor();
}

#define MAX_COU_ERR 2

void CEnergyMeters::OnButtonHistory1() 
{
	// TODO: Add your control notification handler code here
	tsSendEnergyArray sSendEnergyArray;
	memset(&sSendEnergyArray,0,sizeof(tsSendEnergyArray));
	memset(&sSendEnergyArray.reversDateTimeEnd,0x99,sizeof(tsDateTime));

	CDateTimeLimits CDTL;
	if( CDTL.DoModal() != IDOK )
		return;
	else{
		if(CDTL.d_min){
			sSendEnergyArray.reversDateTimeStart.date_time[0] = INT_BCD(CDTL.d_min.GetYear()%100);
			sSendEnergyArray.reversDateTimeStart.date_time[1] = INT_BCD(CDTL.d_min.GetMonth());
			sSendEnergyArray.reversDateTimeStart.date_time[2] = INT_BCD(CDTL.d_min.GetDay());
			if(CDTL.t_min){
				sSendEnergyArray.reversDateTimeStart.date_time[3] = INT_BCD(CDTL.t_min.GetHour());
				sSendEnergyArray.reversDateTimeStart.date_time[4] = INT_BCD(CDTL.t_min.GetMinute());
				sSendEnergyArray.reversDateTimeStart.date_time[5] = INT_BCD(CDTL.t_min.GetSecond());
			}
		}
		if(CDTL.d_max){
			sSendEnergyArray.reversDateTimeEnd.date_time[0] = INT_BCD(CDTL.d_max.GetYear()%100);
			sSendEnergyArray.reversDateTimeEnd.date_time[1] = INT_BCD(CDTL.d_max.GetMonth());
			sSendEnergyArray.reversDateTimeEnd.date_time[2] = INT_BCD(CDTL.d_max.GetDay());
			if(CDTL.t_max){
				sSendEnergyArray.reversDateTimeEnd.date_time[3] = INT_BCD(CDTL.t_max.GetHour());
				sSendEnergyArray.reversDateTimeEnd.date_time[4] = INT_BCD(CDTL.t_max.GetMinute());
				sSendEnergyArray.reversDateTimeEnd.date_time[5] = INT_BCD(CDTL.t_max.GetSecond());
			}
		}
	}


	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString sucses;
		{
			BeginWaitCursor();
			
			CString to_edit,tmp;

			UINT cou_lamp = GetLampCount(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
			float calculated_power = GetCalculatedPawer(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));

			tmp.Format(IDS_HISTORY_ENERGY,m_Grid.GetItemText(m_iSelectItem,C_CITY),m_Grid.GetItemText(m_iSelectItem,C_ADDRESS),
				cou_lamp,calculated_power);
			to_edit += tmp;

			tmp.LoadString(IDS_HISTORY_ENERGY1);
			to_edit += tmp;

			tmp.Format("enable_energy_meter != 0 AND id_hardware = %d",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
			if( ::GetSqlCount(_T("hardwares"),tmp) ){
				sucses.LoadString(IDS_ERROR);
				
				int cou_report = 0;
				
				struct in_addr adr;
				adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);
				
				int cou_err = MAX_COU_ERR;
				
				while( sSendEnergyArray.u8FlagEnd == 0 ){
					
					sSendEnergyArray.u16EntryArrayCount = htons(MAX_SEND_CURRENT_ENEGY_ARRAY);
					int recived=JennicModule.GetHistoryEnergy(inet_ntoa(adr),&sSendEnergyArray);
					
					if( recived >= (int)( HEADER_SIZE + 1 + sizeof(tsSendEnergyArray) ) ){
						if( p_hardware->Bufer_Rx[HEADER_SIZE] == 	COOMAND_GET_CURRENT_ENERGY_ARRAY ){
							
							cou_err = 0;
							
							tsSendEnergyArray * psSendEnergyArray = (tsSendEnergyArray*)(p_hardware->Bufer_Rx + HEADER_SIZE + 1);
							tsCurrentEnergySmall * psCurrentEnergySmall = (tsCurrentEnergySmall*)(psSendEnergyArray +1);
							memcpy(&sSendEnergyArray,(tsSendEnergyArray *)(p_hardware->Bufer_Rx + HEADER_SIZE + 1),sizeof(tsSendEnergyArray));
							memcpy(&sSendEnergyArray,psSendEnergyArray,sizeof(tsSendEnergyArray));
							
							for( int i = 0 ; i < ntohs(psSendEnergyArray->u16EntryArrayCount) ; i++){
								tsDateTime * psDateTime = (tsDateTime *)(&psCurrentEnergySmall->DateTime);
								COleDateTime time( BCD_INT(psDateTime->date_time[5])+2000, BCD_INT(psDateTime->date_time[4]),BCD_INT(psDateTime->date_time[3]),
									BCD_INT(psDateTime->date_time[2]),BCD_INT(psDateTime->date_time[1]),BCD_INT(psDateTime->date_time[0]));
								tmp.Format(IDS_HISTORY_ENERGY2,::TimeToSQL(time),psCurrentEnergySmall->Active_power,psCurrentEnergySmall->L1_Active_power,
									psCurrentEnergySmall->L2_Active_power,psCurrentEnergySmall->L3_Active_power);
								to_edit += tmp;
								
								psCurrentEnergySmall ++; 
								
								cou_report++;
							}
						}else if( ++cou_err > MAX_COU_ERR )
							break;
					}else if( ++cou_err > MAX_COU_ERR )
						break;
					sSendEnergyArray.u16FirstArrayEntry = htons(1);
				}
				if( sSendEnergyArray.u8FlagEnd ){
					//AfxMessageBox(text);
					sucses.LoadString(IDS_SUCSES);
				}
				
				if( cou_report ){
					tmp.Format(IDS_TOTAL,cou_report);
					to_edit += tmp;
				}
			}
			m_Grid.SetItemText(m_iSelectItem,C_SUCSES,sucses);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);

			tmp.LoadString(IDS_ERROR);
			if( tmp == sucses ){
				tmp.Format("%s\r\n",sucses);
				to_edit += tmp;
			}
			CDisabledLamps DL(IDS_HISTORY_ENERGY_NAME,IDS_HISTORY_ENERGY_FILE);
			DL.text = to_edit;
			DL.DoModal();
		}
	}
	EndWaitCursor();
}

void CEnergyMeters::OnButtonHistory() 
{
	// TODO: Add your control notification handler code here
	tsSendEnergyArray sSendEnergyArray;
	memset(&sSendEnergyArray,0,sizeof(tsSendEnergyArray));
	memset(&sSendEnergyArray.reversDateTimeEnd,0x99,sizeof(tsDateTime));

	CDateTimeLimits CDTL;
	if( CDTL.DoModal() != IDOK )
		return;
	else{
		if(CDTL.d_min){
			sSendEnergyArray.reversDateTimeStart.date_time[0] = INT_BCD(CDTL.d_min.GetYear()%100);
			sSendEnergyArray.reversDateTimeStart.date_time[1] = INT_BCD(CDTL.d_min.GetMonth());
			sSendEnergyArray.reversDateTimeStart.date_time[2] = INT_BCD(CDTL.d_min.GetDay());
			if(CDTL.t_min){
				sSendEnergyArray.reversDateTimeStart.date_time[3] = INT_BCD(CDTL.t_min.GetHour());
				sSendEnergyArray.reversDateTimeStart.date_time[4] = INT_BCD(CDTL.t_min.GetMinute());
				sSendEnergyArray.reversDateTimeStart.date_time[5] = INT_BCD(CDTL.t_min.GetSecond());
			}
		}
		if(CDTL.d_max){
			sSendEnergyArray.reversDateTimeEnd.date_time[0] = INT_BCD(CDTL.d_max.GetYear()%100);
			sSendEnergyArray.reversDateTimeEnd.date_time[1] = INT_BCD(CDTL.d_max.GetMonth());
			sSendEnergyArray.reversDateTimeEnd.date_time[2] = INT_BCD(CDTL.d_max.GetDay());
			if(CDTL.t_max){
				sSendEnergyArray.reversDateTimeEnd.date_time[3] = INT_BCD(CDTL.t_max.GetHour());
				sSendEnergyArray.reversDateTimeEnd.date_time[4] = INT_BCD(CDTL.t_max.GetMinute());
				sSendEnergyArray.reversDateTimeEnd.date_time[5] = INT_BCD(CDTL.t_max.GetSecond());
			}
		}
	}

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
						
			int old_iSelectItem = m_iSelectItem;

			UINT total_cou_lamp = 0;
			float total_calculated_power = 0;

			for( m_iSelectItem = 1; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
				CString text;
				text=m_Grid.GetItemText(m_iSelectItem,C_STOP);
				if(enabled == text){
					UINT cou_lamp = GetLampCount(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
					float calculated_power = GetCalculatedPawer(atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
					total_cou_lamp += cou_lamp;
					total_calculated_power += calculated_power;

					tmp.Format(IDS_HISTORY_ENERGY,m_Grid.GetItemText(m_iSelectItem,C_CITY),m_Grid.GetItemText(m_iSelectItem,C_ADDRESS),
						cou_lamp,calculated_power);
					to_edit += tmp;
					
					sucses = "";
					tmp.Format("enable_energy_meter != 0 AND id_hardware = %d",atoi(m_Grid.GetItemText(m_iSelectItem,C_NUMBER)));
					if( ::GetSqlCount(_T("hardwares"),tmp) ){
						tmp.LoadString(IDS_HISTORY_ENERGY1);
						to_edit += tmp;
						
						sucses.LoadString(IDS_ERROR);
						
						int cou_report = 0;
						
						struct in_addr adr;
						adr.S_un.S_addr = m_Grid.GetItemData(m_iSelectItem,C_NUMBER);
						
						int cou_err = MAX_COU_ERR;
						
						sSendEnergyArray.u8FlagEnd = 0;
						sSendEnergyArray.u16FirstArrayEntry = htons(0);
						
						while( sSendEnergyArray.u8FlagEnd == 0 ){
							
							sSendEnergyArray.u16EntryArrayCount = htons(MAX_SEND_CURRENT_ENEGY_ARRAY);
							int recived=JennicModule.GetHistoryEnergy(inet_ntoa(adr),&sSendEnergyArray);
							
							if( recived >= (int)( HEADER_SIZE + 1 + sizeof(tsSendEnergyArray) ) ){
								if( p_hardware->Bufer_Rx[HEADER_SIZE] == 	COOMAND_GET_CURRENT_ENERGY_ARRAY ){
									
									cou_err = 0;
									
									tsSendEnergyArray * psSendEnergyArray = (tsSendEnergyArray*)(p_hardware->Bufer_Rx + HEADER_SIZE + 1);
									tsCurrentEnergySmall * psCurrentEnergySmall = (tsCurrentEnergySmall*)(psSendEnergyArray +1);
									memcpy(&sSendEnergyArray,(tsSendEnergyArray *)(p_hardware->Bufer_Rx + HEADER_SIZE + 1),sizeof(tsSendEnergyArray));
									memcpy(&sSendEnergyArray,psSendEnergyArray,sizeof(tsSendEnergyArray));
									
									for( int i = 0 ; i < ntohs(psSendEnergyArray->u16EntryArrayCount) ; i++){
										tsDateTime * psDateTime = (tsDateTime *)(&psCurrentEnergySmall->DateTime);
										COleDateTime time( BCD_INT(psDateTime->date_time[5])+2000, BCD_INT(psDateTime->date_time[4]),BCD_INT(psDateTime->date_time[3]),
											BCD_INT(psDateTime->date_time[2]),BCD_INT(psDateTime->date_time[1]),BCD_INT(psDateTime->date_time[0]));
										tmp.Format(IDS_HISTORY_ENERGY2,::TimeToSQL(time),psCurrentEnergySmall->Active_power,psCurrentEnergySmall->L1_Active_power,
											psCurrentEnergySmall->L2_Active_power,psCurrentEnergySmall->L3_Active_power);
										to_edit += tmp;
										
										psCurrentEnergySmall ++; 
										
										cou_report++;
									}
								}else if( ++cou_err > MAX_COU_ERR )
									break;
							}else if( ++cou_err > MAX_COU_ERR )
								break;
							sSendEnergyArray.u16FirstArrayEntry = htons(1);
						}
						if( sSendEnergyArray.u8FlagEnd ){
							//AfxMessageBox(text);
							sucses.LoadString(IDS_SUCSES);
						}
						if( cou_report ){
							tmp.Format(IDS_TOTAL,cou_report);
							to_edit += tmp;
						}			
					}
					m_Grid.SetItemText(m_iSelectItem,C_SUCSES,sucses);
					m_Grid.SetFocusCell(m_iSelectItem,0);
					m_Grid.RedrawWindow();
					m_Grid.SetFocus();
					m_Grid.EnsureVisible(m_iSelectItem,0);
					
					tmp.LoadString(IDS_ERROR);
					if( tmp == sucses ){
						tmp.Format("%s\r\n",sucses);
						to_edit += tmp;
					}
							
					to_edit += "\r\n\r\n";
				}
			}

			tmp.Format(IDS_TOTAL_CALCULATED,total_cou_lamp,total_calculated_power);
			to_edit += tmp;

			m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
			m_Grid.SetFocusCell(m_iSelectItem,0);
			m_Grid.RedrawWindow();
			m_Grid.SetFocus();
			m_Grid.EnsureVisible(m_iSelectItem,0);

			CDisabledLamps DL(IDS_HISTORY_ENERGY_NAME,IDS_HISTORY_ENERGY_FILE);
			DL.text = to_edit;
			DL.DoModal();
		}
	}
	EndWaitCursor();
}
