// TimerSetup.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "TimerSetup.h"

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
#define LIGHT_SIZE 40

#define R_HEADER 2

#define C_NAME 0
#define C_ON 1
#define C_OFF 2
#define C_ON2 3
#define C_OFF2 4
#define C_GROUP 5
#define MAX_COLUMS (C_GROUP + ( MAX_GROUP_DATA_BASE * 3 ))


AFX_STATIC_DATA const TCHAR _afxSunriseSunset[] = _T("SunriseSunset");
AFX_STATIC_DATA const TCHAR _afxLat[] = _T("sLalitude");
AFX_STATIC_DATA const TCHAR _afxLon[] = _T("sLongitude");
AFX_STATIC_DATA const TCHAR _afxOffsetOnOff[] = _T("OffsetOnOff");
AFX_STATIC_DATA const TCHAR _afxAutoOnOff[] = _T("AutoOnOff");

/////////////////////////////////////////////////////////////////////////////
// CTimerSetup property page

IMPLEMENT_DYNCREATE(CTimerSetup, CPropertyPage)

CTimerSetup::CTimerSetup() : CPropertyPage(CTimerSetup::IDD)
{
	m_iSelectItem=-1;
	fl_config=false;
	//{{AFX_DATA_INIT(CTimerSetup)
	m_dLat = 0.0;
	m_dLon = 0.0;
	m_OffsetOnOff = 0;
	m_bAutoOnOff = FALSE;
	//}}AFX_DATA_INIT

	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);

	m_dLat = pApp->m_dLat;
	m_dLon = pApp->m_dLon;

	m_OffsetOnOff = pApp->m_OffsetOnOff;
	m_bAutoOnOff = pApp->m_bAutoOnOff;
}

CTimerSetup::~CTimerSetup()
{
}

void CTimerSetup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimerSetup)
	DDX_Control(pDX, IDC_SUNSET_OFFSET, m_cSunsetOffset);
	DDX_Control(pDX, IDC_SUNRISEOFFSET, m_cSunriseOffset);
	DDX_Control(pDX, IDC_SUNSET, m_cSunset);
	DDX_Control(pDX, IDC_SUNRISE, m_cSunrise);
	DDX_Control(pDX, IDC_LON, m_eLon);
	DDX_Control(pDX, IDC_LAT, m_eLat);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_bNew);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_bDel);
	DDX_Text(pDX, IDC_LAT, m_dLat);
	DDV_MinMaxDouble(pDX, m_dLat, -90., 90.);
	DDX_Text(pDX, IDC_LON, m_dLon);
	DDV_MinMaxDouble(pDX, m_dLon, -180., 180.);
	DDX_Text(pDX, IDC_OFFSET_ONOFF, m_OffsetOnOff);
	DDV_MinMaxInt(pDX, m_OffsetOnOff, -50, 50);
	DDX_Check(pDX, IDC_CHECK_AUTO_ONOFF, m_bAutoOnOff);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_Grid); 
}


BEGIN_MESSAGE_MAP(CTimerSetup, CPropertyPage)
	//{{AFX_MSG_MAP(CTimerSetup)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_PREVIEV, OnButtonPrintPreviev)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_CALCULATE, OnCalculate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimerSetup message handlers

BOOL CTimerSetup::OnInitDialog() 
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
		m_Grid.SetFixedColumnCount(0);
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

	Item1.nFormat=Item.nFormat=DT_VCENTER|DT_SINGLELINE;


	text.LoadString(IDS_NAME);
	m_Grid.SetColumnWidth(C_NAME,120);
	m_Grid.SetItemText(0,C_NAME,text);
	m_Grid.SetMerge(0,C_NAME,1,0,false);
	Item1.col=Item.col=C_NAME; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

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

	for( int i = 0 ; i < MAX_GROUP_DATA_BASE ; i++ ){
		m_Grid.SetCellType(-1,C_GROUP + 2 + i*3,RUNTIME_CLASS(CGridCellNumeric));
		m_Grid.SetCompareFunction(CGridCtrl::numeric_cmp,C_GROUP + 2 + i*3);

		Item1.col=Item.col=C_GROUP + i*3; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);
		Item1.col=Item.col=C_GROUP + 1 + i*3; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);
		Item1.col=Item.col=C_GROUP + 2 + i*3; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	
		text.Format(IDS_GROUP, i+1);	m_Grid.SetItemText(0,C_GROUP + i*3,text);

		m_Grid.SetMerge(0,C_GROUP + i*3 ,0,2,false);
		m_Grid.SetColumnWidth(C_GROUP + i*3,ON_OFF_SIZE);
		m_Grid.SetColumnWidth(C_GROUP + 1 + i*3,ON_OFF_SIZE);
		m_Grid.SetColumnWidth(C_GROUP + 2 + i*3,LIGHT_SIZE);

		text.LoadString(IDS_ON);	m_Grid.SetItemText(1,C_GROUP + i*3,text);
		text.LoadString(IDS_OFF);	m_Grid.SetItemText(1,C_GROUP + 1 + i*3,text);
		text.LoadString(IDS_LIGHT);	m_Grid.SetItemText(1,C_GROUP + 2 + i*3,text);
	}

	m_Grid.SetFixedColumnSelection(TRUE);
	m_Grid.SetHeaderSort(true);
	
	text.Format(_T("%10.6lf"),m_dLat);
	m_eLat.SetWindowText(text);

	text.Format(_T("%10.6lf"),m_dLon);
	m_eLon.SetWindowText(text);

	return LoadData(true); 

}

BOOL CTimerSetup::LoadData(bool fl_select)
{
	BOOL flag=true;
	CString text;
	_RecordsetPtr m_pRs=NULL;
	
  m_Grid.SetRowCount(R_HEADER);
	  
	_bstr_t strCom("SELECT * FROM timers ORDER BY id_timer");
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
				m_Grid.SetItemData(i,C_NAME,::GetLongFromVariant(data));	
				
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_NAME,text);

				unsigned int ui;

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				ui = ::GetLongFromVariant(data);
				text.Format("%u:%02u",ui/60,ui%60);
				m_Grid.SetItemText(i,C_ON,text);	

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				ui = ::GetLongFromVariant(data);
				text.Format("%u:%02u",ui/60,ui%60);
				m_Grid.SetItemText(i,C_OFF,text);

				for( int j = 0 ; j < MAX_GROUP_DATA_BASE ; j++){
					index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
					ui = ::GetLongFromVariant(data);
					text.Format("%u:%02u",ui/60,ui%60);
					m_Grid.SetItemText(i,C_GROUP + j*3,text);	

					index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
					ui = ::GetLongFromVariant(data);
					text.Format("%u:%02u",ui/60,ui%60);
					m_Grid.SetItemText(i,C_GROUP + 1 + j*3,text);

					index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
					text.Format("%u",::GetLongFromVariant(data));
					m_Grid.SetItemText(i,C_GROUP + 2 + j*3,text);
				}

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				ui = ::GetLongFromVariant(data);
				text.Format("%u:%02u",ui/60,ui%60);
				m_Grid.SetItemText(i,C_ON2,text);	

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				ui = ::GetLongFromVariant(data);
				text.Format("%u:%02u",ui/60,ui%60);
				m_Grid.SetItemText(i,C_OFF2,text);
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

void CTimerSetup::EnableKeys()
{
	BOOL bNew,bDel;
	bNew=bDel=false;
	if(m_iSelectItem>=R_HEADER){
		bNew=bDel=true;
		if(m_Grid.GetItemData(m_iSelectItem,C_NAME)==0)
			bDel=false;
	}else
		bNew=true;
	m_bNew.EnableWindow(bNew);
	m_bDel.EnableWindow(bDel);
}

BOOL CTimerSetup::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (wParam == (WPARAM)m_Grid.GetDlgCtrlID()){
    *pResult = 1;
    GV_DISPINFO *pDispInfo = (GV_DISPINFO*)lParam;
		switch(pDispInfo->hdr.code){
			case GVN_ENDLABELEDIT:
				{
					_CommandPtr m_pCmd=NULL;
					UINT32 ID=m_Grid.GetItemData(pDispInfo->item.row,C_NAME);
					CString com;
					CString text;
					try{
						switch(pDispInfo->item.col){
							case C_NAME:
								{
									text=m_Grid.GetItemText(pDispInfo->item.row,C_NAME);
									::CStringSize(text,30);
									com.Format(_T("timer_name='%s' AND id_timer != %u"),text,ID);
									if(::GetSqlCount(_T("timers"),com)==0){
										com.Format(_T("UPDATE timers SET timer_name='%s' WHERE id_timer=%u"),text,ID);
										TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
										m_pCmd->ActiveConnection=::GetConnection();
										m_pCmd->CommandText=(LPCTSTR)com;
										m_pCmd->Execute(NULL, NULL, adCmdText);	
										StoreConfigMessage();
										m_Grid.SetItemText(pDispInfo->item.row,C_NAME,text);
									}else{	
										com.Format(IDS_NAME_IS,text);
										AfxMessageBox(com);	
										m_Grid.SetItemText(pDispInfo->item.row,C_NAME,OldText);
									}
								}
								break;
							case C_ON:
								{
									unsigned int ui1=0,ui2=0;
									char c;

									text=m_Grid.GetItemText(pDispInfo->item.row,C_ON);
									sscanf(text,"%u%c%u",&ui1,&c,&ui2);
									ui1 = ui1%24; ui2 = ui2%60;
									text.Format("%u:%02u",ui1,ui2);
									m_Grid.SetItemText(pDispInfo->item.row,C_ON,text);
									ui1 *= 60;	ui1+=ui2;
									com.Format(_T("UPDATE timers SET time_on=%u WHERE id_timer=%u"),ui1,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_OFF:
								{
									unsigned int ui1=0,ui2=0;
									char c;

									text=m_Grid.GetItemText(pDispInfo->item.row,C_OFF);
									sscanf(text,"%u%c%u",&ui1,&c,&ui2);
									ui1 = ui1%24; ui2 = ui2%60;
									text.Format("%u:%02u",ui1,ui2);
									m_Grid.SetItemText(pDispInfo->item.row,C_OFF,text);
									ui1 *= 60;	ui1+=ui2;
									com.Format(_T("UPDATE timers SET time_off=%u WHERE id_timer=%u"),ui1,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_ON2:
								{
									unsigned int ui1=0,ui2=0;
									char c;

									text=m_Grid.GetItemText(pDispInfo->item.row,C_ON2);
									sscanf(text,"%u%c%u",&ui1,&c,&ui2);
									ui1 = ui1%24; ui2 = ui2%60;
									text.Format("%u:%02u",ui1,ui2);
									m_Grid.SetItemText(pDispInfo->item.row,C_ON2,text);
									ui1 *= 60;	ui1+=ui2;
									com.Format(_T("UPDATE timers SET time_on2=%u WHERE id_timer=%u"),ui1,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_OFF2:
								{
									unsigned int ui1=0,ui2=0;
									char c;

									text=m_Grid.GetItemText(pDispInfo->item.row,C_OFF2);
									sscanf(text,"%u%c%u",&ui1,&c,&ui2);
									ui1 = ui1%24; ui2 = ui2%60;
									text.Format("%u:%02u",ui1,ui2);
									m_Grid.SetItemText(pDispInfo->item.row,C_OFF2,text);
									ui1 *= 60;	ui1+=ui2;
									com.Format(_T("UPDATE timers SET time_off2=%u WHERE id_timer=%u"),ui1,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							default:
								if( pDispInfo->item.col < MAX_COLUMS ){
									int group = ( pDispInfo->item.col - C_GROUP )/3;
									int tim = ( pDispInfo->item.col - C_GROUP )%3;
									if( tim == 2 ){
										UINT32 data=::UI32Format(m_Grid.GetItemText(pDispInfo->item.row,pDispInfo->item.col),255);
										text.Format("%u",data);
										m_Grid.SetItemText(pDispInfo->item.row,pDispInfo->item.col,text);
										com.Format(_T("UPDATE timers SET light_g%d=%u WHERE id_timer=%u"),group+1,data,ID);
										TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
										m_pCmd->ActiveConnection=::GetConnection();
										m_pCmd->CommandText=(LPCTSTR)com;
										m_pCmd->Execute(NULL, NULL, adCmdText);	
										StoreConfigMessage();
									}else{
										unsigned int ui1=0,ui2=0;
										char c;
										
										text=m_Grid.GetItemText(pDispInfo->item.row,pDispInfo->item.col);
										sscanf(text,"%u%c%u",&ui1,&c,&ui2);
										ui1 = ui1%24; ui2 = ui2%60;
										text.Format("%u:%02u",ui1,ui2);
										m_Grid.SetItemText(pDispInfo->item.row,pDispInfo->item.col,text);
										ui1 *= 60;	ui1+=ui2;
										if( tim )
											com.Format(_T("UPDATE timers SET off_g%u=%u WHERE id_timer=%u"),group+1,ui1,ID);
										else
											com.Format(_T("UPDATE timers SET on_g%u=%u WHERE id_timer=%u"),group+1,ui1,ID);
										TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
										m_pCmd->ActiveConnection=::GetConnection();
										m_pCmd->CommandText=(LPCTSTR)com;
										m_pCmd->Execute(NULL, NULL, adCmdText);	
										StoreConfigMessage();
									}
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
				//if(pDispInfo->item.col!= C_STOP){
					//*pResult =-1;
				//}
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

void CTimerSetup::StoreConfigMessage()
{
	if(!fl_config){
		fl_config=true;
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		ASSERT_VALID(pApp);
		::StoreDataConfigMessage(pApp->Id_PC,pApp->Id_Operator,_CONFIG_ROUTERS);
	}
}

void CTimerSetup::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrint(&m_Grid,this);
}

void CTimerSetup::OnButtonPrintPreviev() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrintPreviev(&m_Grid,this);			
}

BOOL CTimerSetup::ContinueModal()
{
	if (!IsWindowVisible())
		// send WM_IDLEUPDATECMDUI message to update toolbar state
		// This is normally called by OnIdle function in SDI or MSI applications.
		// Dialog based applications don't call OnIdle, so send the message from here instead
		AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
			(WPARAM)TRUE, 0, TRUE, TRUE);

	return CDialog::ContinueModal();
}

#include "EditName.h"
void CTimerSetup::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	CEditName EN;
	if(EN.DoModal() == IDOK) {
		_CommandPtr m_pCmd=NULL;
		
		try{
			CString com;
			com.Format(_T("timer_name='%s'"),EN.m_cName);
			if(::GetSqlCount(_T("timers"),com)==0){
				com.Format(_T("INSERT INTO timers(timer_name) VALUES ('%s')"),EN.m_cName);
				TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
				m_pCmd->ActiveConnection=::GetConnection();
				m_pCmd->CommandText=(LPCTSTR)com;
				m_pCmd->Execute(NULL, NULL, adCmdText);
				
				StoreConfigMessage();
				LoadData();
				m_Grid.SetFocusCell(m_iSelectItem=m_Grid.GetRowCount()-1,0);
				m_Grid.RedrawWindow();
				m_Grid.SetFocus();
				m_Grid.EnsureVisible(m_iSelectItem,0);
				EnableKeys();
			}else{
				com.Format(IDS_NAME_IS,EN.m_cName);
				AfxMessageBox(com);
			}
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}
	}
}

void CTimerSetup::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	CString mes;
	mes.LoadString(IDS_DELETE);
	mes+=m_Grid.GetItemText(m_iSelectItem,C_NAME);
	if(AfxMessageBox(mes, MB_YESNO |	MB_DEFBUTTON2|MB_ICONQUESTION)==IDYES){
		_CommandPtr m_pCmd=NULL;
		try{
			UINT32 ID=m_Grid.GetItemData(m_iSelectItem,C_NAME);
			CString com;
			com.Format(_T("id_timer=%u"),ID);
			if(::GetSqlCount(_T("hardwares"),com)==0){
				com.Format(_T("DELETE FROM timers WHERE id_timer=%u"),ID);
				TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
				m_pCmd->ActiveConnection=::GetConnection();
				m_pCmd->CommandText=(LPCTSTR)com;
				m_pCmd->Execute(NULL, NULL, adCmdText);
				StoreConfigMessage();
				m_iSelectItem=-1;
				LoadData();
			}else{
				com.Format(IDS_BUSY_IN_RUTERS,m_Grid.GetItemText(m_iSelectItem,C_NAME));
				AfxMessageBox(com);
			}
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}
	}
}

#include "SunSet.h"

void CTimerSetup::OnCalculate() 
{
	// TODO: Add your control notification handler code here
	if( UpdateData() ){
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		ASSERT_VALID(pApp);

		CString text;
		text.Format(_T("%10.6lf"),pApp->m_dLat = m_dLat);
		m_eLat.SetWindowText(text);
		AfxGetApp()->WriteProfileString(_afxSunriseSunset,_afxLat,text);
		text.Format(_T("%10.6lf"),pApp->m_dLon = m_dLon);
		m_eLon.SetWindowText(text);
		AfxGetApp()->WriteProfileString(_afxSunriseSunset,_afxLon,text);

		AfxGetApp()->WriteProfileInt(_afxSunriseSunset,_afxOffsetOnOff,pApp->m_OffsetOnOff = m_OffsetOnOff);
		AfxGetApp()->WriteProfileInt(_afxSunriseSunset,_afxAutoOnOff,pApp->m_bAutoOnOff = m_bAutoOnOff);

		SYSTEMTIME lpSystemTime;
		GetSystemTime(&lpSystemTime);
		SYSTEMTIME lpLocalTime;
		GetLocalTime(&lpLocalTime);
		if( lpLocalTime.wMinute != lpSystemTime.wMinute )
			GetSystemTime(&lpSystemTime);
		int HourOffset = (lpLocalTime.wHour - lpSystemTime.wHour + 24 )%24;
	
		SunSet sun;
		sun.setPosition(m_dLat, m_dLon, HourOffset);
		sun.setCurrentDate(lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);

		long minute;
		minute = (long int)sun.calcSunrise();
		text.Format(_T("%2d:%02d"),(minute/60)%24,minute%60);
		m_cSunrise.SetWindowText(text);
		text.Format(_T("%2d:%02d"),((minute-m_OffsetOnOff)/60)%24,(minute-m_OffsetOnOff)%60);
		m_cSunriseOffset.SetWindowText(text);

		minute = (long int)sun.calcSunset();
		text.Format(_T("%2d:%02d"),(minute/60)%24,minute%60);
		m_cSunset.SetWindowText(text);
		text.Format(_T("%2d:%02d"),((minute+m_OffsetOnOff)/60)%24,(minute+m_OffsetOnOff)%60);
		m_cSunsetOffset.SetWindowText(text);

		UpdateData(true);
	}
}
