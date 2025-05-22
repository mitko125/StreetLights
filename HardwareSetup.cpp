// HardwareSetup.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "HardwareSetup.h"

#include "DynamicLib.h"

#include "..\Free_Util_Dll\Util_Dll.h"
#include	"../../Grid1_0/NewCellTypes/GridCellCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define C_NUMBER 0
#define C_HARDWARE 1
#define C_CITY 2
#define C_ADDRESS 3
#define C_LAT	4
#define C_LNG 5
#define C_SUCSES 6
#define C_SIM_ERRORS 7
#define C_RESET_SIM 8
#define C_ON_COUNTER 9
#define C_CLEAR_RAM 10
#define C_TIME_ON 11
#define C_TIME_OFF 12
#define C_CSQ 13
#define C_RELAY 14
#define C_HOST_VERSION 15
#define C_STATE 16
#define C_JENNIC_VERSION 17
#define C_STOP 18
#define MAX_COLUMS 19

/////////////////////////////////////////////////////////////////////////////
// CHardwareSetup property page

IMPLEMENT_DYNCREATE(CHardwareSetup, CPropertyPage)

CHardwareSetup::CHardwareSetup() : CPropertyPage(CHardwareSetup::IDD)
{
	m_iSelectItem=-1;
	fl_config=false;
	//{{AFX_DATA_INIT(CHardwareSetup)
	m_iLights = 255;
	m_NewNumber = 1;
	//}}AFX_DATA_INIT
}

CHardwareSetup::~CHardwareSetup()
{
}

void CHardwareSetup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHardwareSetup)
	DDX_Control(pDX, IDC_BUTTON_CLEAR_RAM1, m_bClear1);
	DDX_Control(pDX, IDC_EDIT_NUMBER, m_bEditNumber);
	DDX_Control(pDX, IDC_BUTTON_TEST1, m_bTest1);
	DDX_Control(pDX, IDC_BUTTON_ON1, m_bOn1);
	DDX_Control(pDX, IDC_BUTTON_OFF1, m_bOff1);
	DDX_Control(pDX, IDC_BUTTON_LIGHT1, m_bLight1);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IPAddres);
	DDX_Control(pDX, IDC_BUTTON_NEW, m_bNew);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_bDel);
	DDX_Text(pDX, IDC_EDIT_LIGHTS, m_iLights);
	DDV_MinMaxInt(pDX, m_iLights, 1, 255);
	DDX_Text(pDX, IDC_EDIT_NUMBER, m_NewNumber);
	DDV_MinMaxUInt(pDX, m_NewNumber, 1, 65535);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_Grid); 
}


BEGIN_MESSAGE_MAP(CHardwareSetup, CPropertyPage)
	//{{AFX_MSG_MAP(CHardwareSetup)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_PREVIEV, OnButtonPrintPreviev)
	ON_BN_CLICKED(IDC_BUTTON_TEST, OnButtonTest)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_TEST1, OnButtonTest1)
	ON_BN_CLICKED(IDC_BUTTON_ON, OnButtonOn)
	ON_BN_CLICKED(IDC_BUTTON_ON1, OnButtonOn1)
	ON_BN_CLICKED(IDC_BUTTON_OFF, OnButtonOff)
	ON_BN_CLICKED(IDC_BUTTON_OFF1, OnButtonOff1)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT, OnButtonLight)
	ON_BN_CLICKED(IDC_BUTTON_LIGHT1, OnButtonLight1)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_RAM1, OnButtonClearRam1)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_RAM, OnButtonClearRam)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHardwareSetup message handlers

BOOL CHardwareSetup::OnInitDialog() 
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

	text.LoadString(IDS_HARDWARE);
	m_Grid.SetColumnWidth(C_HARDWARE,90);
	m_Grid.SetItemText(0,C_HARDWARE,text);

	text.LoadString(IDS_ADDRESS1);
	m_Grid.SetColumnWidth(C_CITY,90);
	m_Grid.SetItemText(0,C_CITY,text);

	text.LoadString(IDS_ADDRESS2);
	m_Grid.SetColumnWidth(C_ADDRESS,100);
	m_Grid.SetItemText(0,C_ADDRESS,text);

	text.LoadString(IDS_LAT);
	m_Grid.SetColumnWidth(C_LAT,100);
	m_Grid.SetItemText(0,C_LAT,text);
	Item1.col=Item.col=C_LAT; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_LNG);
	m_Grid.SetColumnWidth(C_LNG,100);
	m_Grid.SetItemText(0,C_LNG,text);
	Item1.col=Item.col=C_LNG; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_SUCSES);
	m_Grid.SetColumnWidth(C_SUCSES,50);
	m_Grid.SetItemText(0,C_SUCSES,text);
	
	text.LoadString(IDS_GPRS_ERRORS);
	m_Grid.SetColumnWidth(C_SIM_ERRORS,60);
	m_Grid.SetItemText(0,C_SIM_ERRORS,text);
	Item1.col=Item.col=C_SIM_ERRORS; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_RESER_SIM);
	m_Grid.SetColumnWidth(C_RESET_SIM,115);
	m_Grid.SetItemText(0,C_RESET_SIM,text);

	text.LoadString(IDS_ON_OFF);
	m_Grid.SetColumnWidth(C_ON_COUNTER,50);
	m_Grid.SetItemText(0,C_ON_COUNTER,text);
	Item1.col=Item.col=C_ON_COUNTER; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_CLEAR_RAM);
	m_Grid.SetColumnWidth(C_CLEAR_RAM,115);
	m_Grid.SetItemText(0,C_CLEAR_RAM,text);

	text.LoadString(IDS_TIME_ON_ROUTER);
	m_Grid.SetColumnWidth(C_TIME_ON,115);
	m_Grid.SetItemText(0,C_TIME_ON,text);

	text.LoadString(IDS_TIME_OFF_ROUTER);
	m_Grid.SetColumnWidth(C_TIME_OFF,115);
	m_Grid.SetItemText(0,C_TIME_OFF,text);

	text.LoadString(IDS_CSQ);
	m_Grid.SetColumnWidth(C_CSQ,35);
	m_Grid.SetItemText(0,C_CSQ,text);
	Item1.col=Item.col=C_CSQ; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_STATE);
	m_Grid.SetColumnWidth(C_STATE,40);
	m_Grid.SetItemText(0,C_STATE,text);
	Item1.col=Item.col=C_STATE; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_RELAY);
	m_Grid.SetColumnWidth(C_RELAY,40);
	m_Grid.SetItemText(0,C_RELAY,text);
	Item1.col=Item.col=C_RELAY; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_HOST_VERSION);
	m_Grid.SetColumnWidth(C_HOST_VERSION,65);
	m_Grid.SetItemText(0,C_HOST_VERSION,text);
	Item1.col=Item.col=C_HOST_VERSION; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	text.LoadString(IDS_JENNIC_VERSION);
	m_Grid.SetColumnWidth(C_JENNIC_VERSION,65);
	m_Grid.SetItemText(0,C_JENNIC_VERSION,text);
	Item1.col=Item.col=C_JENNIC_VERSION; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

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

BOOL CHardwareSetup::LoadData(bool fl_select)
{
	BOOL flag=true;
	CString text;
	_RecordsetPtr m_pRs=NULL;
	
  m_Grid.SetRowCount(1);
	  
	_bstr_t strCom("SELECT id_hardware,number_hardware,city_name,address_name,lat,lng,stop FROM hardwares ORDER BY id_hardware");
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
				m_Grid.SetItemData(i,C_NUMBER,::GetLongFromVariant(data));	
				
				CString number;
				number.Format("%u",::GetLongFromVariant(data));
				m_Grid.SetItemText(i,C_NUMBER,number);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				number = ::GetStringFromVariant(data);
				
				m_Grid.SetItemText(i,C_HARDWARE,number);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_CITY,text);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_ADDRESS,text);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_LAT,text);

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text = ::GetStringFromVariant(data);
				m_Grid.SetItemText(i,C_LNG,text);

				number.LoadString(IDS_WORK);
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				if(::GetLongFromVariant(data))
					number.LoadString(IDS_NO_WORK);
				m_Grid.SetItemText(i,C_STOP,number);
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

void CHardwareSetup::EnableKeys()
{
	BOOL bNew,bDel;
	bNew=bDel=false;
	if(m_iSelectItem>=1){
		bNew=bDel=true;
		if(m_Grid.GetItemData(m_iSelectItem,C_NUMBER)==0)
			bDel=false;
	}else
		bNew=true;
	m_bNew.EnableWindow(bNew);
	m_bDel.EnableWindow(bDel);
	m_bTest1.EnableWindow(bDel);
	m_bOn1.EnableWindow(bDel);
	m_bOff1.EnableWindow(bDel);
	m_bLight1.EnableWindow(bDel);
	m_bClear1.EnableWindow(bDel);
}

BOOL CHardwareSetup::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
									
									com.Format(_T("id_hardware=%u"),ID);
									if(::GetSqlCount(_T("lamps"),com)==0){
										com.Format(_T("id_hardware=%u AND id_hardware != %u"),new_ID,ID);
										if(::GetSqlCount(_T("hardwares"),com)==0){
											com.Format(_T("UPDATE hardwares SET id_hardware=%u WHERE id_hardware=%u"),new_ID,ID);
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
									}else{
										com.Format(IDS_BUSY_IN_DEVICES,m_Grid.GetItemData(m_iSelectItem,C_NUMBER));
										AfxMessageBox(com);
										m_Grid.SetItemText(pDispInfo->item.row,C_NUMBER,OldText);
									}
								}
								break;
							case C_HARDWARE:
								{
									com.Format(_T("number_hardware='%s'"),m_Grid.GetItemText(pDispInfo->item.row,C_HARDWARE));
									if(::GetSqlCount(_T("hardwares"),com)==0){
										com.Format(_T("UPDATE hardwares SET number_hardware='%s' WHERE id_hardware=%u"),m_Grid.GetItemText(pDispInfo->item.row,C_HARDWARE),ID);
										TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
										m_pCmd->ActiveConnection=::GetConnection();
										m_pCmd->CommandText=(LPCTSTR)com;
										m_pCmd->Execute(NULL, NULL, adCmdText);
											
										StoreConfigMessage();
									}else{
										com.Format(IDS_HARDWARE_IS,m_Grid.GetItemText(pDispInfo->item.row,C_HARDWARE));
										AfxMessageBox(com);
										m_Grid.SetItemText(pDispInfo->item.row,C_HARDWARE,OldText);
									}
								}
							case C_CITY:
								{
									text=m_Grid.GetItemText(pDispInfo->item.row,C_CITY);
									::CStringSize(text,30);
									m_Grid.SetItemText(pDispInfo->item.row,C_CITY,text);
									com.Format(_T("UPDATE hardwares SET city_name='%s' WHERE id_hardware=%u"),text,ID);
									TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
									m_pCmd->ActiveConnection=::GetConnection();
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);	
									StoreConfigMessage();
								}
								break;
							case C_ADDRESS:
								{
									text=m_Grid.GetItemText(pDispInfo->item.row,C_ADDRESS);
									::CStringSize(text,30);
									m_Grid.SetItemText(pDispInfo->item.row,C_ADDRESS,text);
									com.Format(_T("UPDATE hardwares SET address_name='%s' WHERE id_hardware=%u"),text,ID);
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
									com.Format(_T("UPDATE hardwares SET lat='%s' WHERE id_hardware=%u"),text,ID);
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
									com.Format(_T("UPDATE hardwares SET lng='%s' WHERE id_hardware=%u"),text,ID);
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
				OldText=m_Grid.GetItemText(pDispInfo->item.row,pDispInfo->item.col);
				switch(pDispInfo->item.col){
				case C_NUMBER:
				case C_HARDWARE:
				case C_CITY:
				case C_ADDRESS:
				case C_LAT:
				case C_LNG:
				case C_STOP:
					break;
				default:
					*pResult =-1;
					break;
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

void CHardwareSetup::OnButtonNew() 
{
	// TODO: Add your control notification handler code here

	CString number;

	if(!UpdateData(true))
		return;

	{
		in_addr adr;
		m_IPAddres.GetAddress(adr.S_un.S_un_b.s_b1,adr.S_un.S_un_b.s_b2,adr.S_un.S_un_b.s_b3,adr.S_un.S_un_b.s_b4);
		number=inet_ntoa(adr);
	}
			
	_CommandPtr m_pCmd=NULL;

	try{
		CString com;
		com.Format(_T("number_hardware='%s'"),number);
		if(::GetSqlCount(_T("hardwares"),com)==0){
			com.Format(_T("id_hardware=%u"),m_NewNumber);
			if(::GetSqlCount(_T("hardwares"),com)==0){
				com.Format(_T("INSERT INTO hardwares(id_hardware,number_hardware) VALUES (%d,'%s')"),m_NewNumber,number);
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
		}else{
			com.Format(IDS_HARDWARE_IS,number);
			AfxMessageBox(com);
		}
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}
}

void CHardwareSetup::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	CString mes;
	CString com;
	mes.LoadString(IDS_DELETE);
	com.Format(" %u ",m_Grid.GetItemData(m_iSelectItem,C_NUMBER));
	mes+=com;
	if(AfxMessageBox(mes, MB_YESNO |	MB_DEFBUTTON2|MB_ICONQUESTION)==IDYES){
		_CommandPtr m_pCmd=NULL;
		try{
			UINT32 ID=m_Grid.GetItemData(m_iSelectItem,C_NUMBER);
			
			com.Format(_T("id_hardware=%u"),ID);
			if(::GetSqlCount(_T("lamps"),com)==0){
				com.Format(_T("DELETE FROM hardwares WHERE id_hardware=%u"),ID);
				TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
				m_pCmd->ActiveConnection=::GetConnection();
				m_pCmd->CommandText=(LPCTSTR)com;
				m_pCmd->Execute(NULL, NULL, adCmdText);
				StoreConfigMessage();
				m_iSelectItem=-1;
				LoadData();
			}else{
				com.Format(IDS_BUSY_IN_DEVICES,m_Grid.GetItemText(m_iSelectItem,C_NUMBER));
				AfxMessageBox(com);
			}
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}
	}
}

void CHardwareSetup::StoreConfigMessage()
{
	if(!fl_config){
		fl_config=true;
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		ASSERT_VALID(pApp);
		::StoreDataConfigMessage(pApp->Id_PC,pApp->Id_Operator,_CONFIG_ROUTERS);
	}
}

void CHardwareSetup::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrint(&m_Grid,this);
}

void CHardwareSetup::OnButtonPrintPreviev() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrintPreviev(&m_Grid,this);			
}

BOOL CHardwareSetup::ContinueModal()
{
	if (!IsWindowVisible())
		// send WM_IDLEUPDATECMDUI message to update toolbar state
		// This is normally called by OnIdle function in SDI or MSI applications.
		// Dialog based applications don't call OnIdle, so send the message from here instead
		AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
			(WPARAM)TRUE, 0, TRUE, TRUE);

	return CDialog::ContinueModal();
}

void CHardwareSetup::OnButtonLoad() 
{

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
		
		CString str=_T(" ");
		do{
			aFile.ReadString(str);
			if(!str.IsEmpty()){
				CString id_hardware,number_hardware,city_name,address_name;
				int i;
				if(i=str.Find(_T('\t'))){
					id_hardware=str.Left(i);
					str = str.Mid(i+1);
					if(i=str.Find(_T('\t'))){
						number_hardware=str.Left(i);
						str = str.Mid(i+1);
						if(i=str.Find(_T('\t'))){
							city_name=str.Left(i);
							address_name = str.Mid(i+1);
						}
					}
				}
				_CommandPtr m_pCmd=NULL;
				
				try{
					CString com;
					com.Format(_T("number_hardware='%s'"),number_hardware);
					if(::GetSqlCount(_T("hardwares"),com)==0){
						com.Format(_T("INSERT INTO hardwares(id_hardware,number_hardware,city_name,address_name) VALUES (%s,'%s','%s','%s')"),
							id_hardware,number_hardware,city_name,address_name);
						TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
						m_pCmd->ActiveConnection=::GetConnection();
						m_pCmd->CommandText=(LPCTSTR)com;
						m_pCmd->Execute(NULL, NULL, adCmdText);
						
					//	StoreConfigMessage();
					//	LoadData();
					}else{
						com.Format(IDS_HARDWARE_IS,number_hardware);
						AfxMessageBox(com);
					}
				}
				catch(_com_error &e){
					::GenerateSqlError(e);
				}
			}
		}while(!str.IsEmpty());
		aFile.Close();		
	}
}

void CHardwareSetup::OnButtonTest1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SIM_ERRORS,"");
	m_Grid.SetItemText(m_iSelectItem,C_RESET_SIM,"");
	m_Grid.SetItemText(m_iSelectItem,C_ON_COUNTER,"");
	m_Grid.SetItemText(m_iSelectItem,C_CLEAR_RAM,"");
	m_Grid.SetItemText(m_iSelectItem,C_TIME_ON,"");
	m_Grid.SetItemText(m_iSelectItem,C_TIME_OFF,"");
	m_Grid.SetItemText(m_iSelectItem,C_CSQ,"");
	m_Grid.SetItemText(m_iSelectItem,C_STATE,"");
	m_Grid.SetItemText(m_iSelectItem,C_RELAY,"");
	m_Grid.SetItemText(m_iSelectItem,C_HOST_VERSION,"");
	m_Grid.SetItemText(m_iSelectItem,C_JENNIC_VERSION,"");
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString text;
		{
			
			text=m_Grid.GetItemText(m_iSelectItem,C_HARDWARE);
			
			BeginWaitCursor();
			
			int recived=JennicModule.SimErrors(text);
			text.LoadString(IDS_ERROR);
			if( recived >= 8 ){
				if (p_hardware->Bufer_Rx[HEADER_SIZE] == SEND_STATUS_ROUTER) {
					tsRouterStatus sRouterStatus;
					memset(&sRouterStatus,0,sizeof(tsRouterStatus));
					int len = p_hardware->Bufer_Rx[0] << 8;
					len |= p_hardware->Bufer_Rx[1];
					if( ( len ) > sizeof(tsRouterStatus) )
						len = sizeof(tsRouterStatus);
					memcpy(&sRouterStatus,(tsRouterStatus*)&p_hardware->Bufer_Rx[HEADER_SIZE+1],len);
					
					text.Format("%u",ntohs(sRouterStatus.u16SimErrors));
					m_Grid.SetItemText(m_iSelectItem,C_SIM_ERRORS,text);
					
					COleDateTime timeSIM( BCD_INT(sRouterStatus.sDateTimeResetGPRS.date_time[5])+2000, BCD_INT(sRouterStatus.sDateTimeResetGPRS.date_time[4]),BCD_INT(sRouterStatus.sDateTimeResetGPRS.date_time[3]),
					BCD_INT(sRouterStatus.sDateTimeResetGPRS.date_time[2]),BCD_INT(sRouterStatus.sDateTimeResetGPRS.date_time[1]),BCD_INT(sRouterStatus.sDateTimeResetGPRS.date_time[0]));

					m_Grid.SetItemText(m_iSelectItem,C_RESET_SIM,::TimeToSQL(timeSIM));
	

					text.Format("%u",ntohs(sRouterStatus.u16OnCounter));
					m_Grid.SetItemText(m_iSelectItem,C_ON_COUNTER,text);

					COleDateTime time( BCD_INT(sRouterStatus.sDateTimeClearRAM.date_time[5])+2000, BCD_INT(sRouterStatus.sDateTimeClearRAM.date_time[4]),BCD_INT(sRouterStatus.sDateTimeClearRAM.date_time[3]),
					BCD_INT(sRouterStatus.sDateTimeClearRAM.date_time[2]),BCD_INT(sRouterStatus.sDateTimeClearRAM.date_time[1]),BCD_INT(sRouterStatus.sDateTimeClearRAM.date_time[0]));

					m_Grid.SetItemText(m_iSelectItem,C_CLEAR_RAM,::TimeToSQL(time));

					COleDateTime time1( BCD_INT(sRouterStatus.sDateTimeOn.date_time[5])+2000, BCD_INT(sRouterStatus.sDateTimeOn.date_time[4]),BCD_INT(sRouterStatus.sDateTimeOn.date_time[3]),
					BCD_INT(sRouterStatus.sDateTimeOn.date_time[2]),BCD_INT(sRouterStatus.sDateTimeOn.date_time[1]),BCD_INT(sRouterStatus.sDateTimeOn.date_time[0]));

					m_Grid.SetItemText(m_iSelectItem,C_TIME_ON,::TimeToSQL(time1));

					COleDateTime time2( BCD_INT(sRouterStatus.sDateTimeOff.date_time[5])+2000, BCD_INT(sRouterStatus.sDateTimeOff.date_time[4]),BCD_INT(sRouterStatus.sDateTimeOff.date_time[3]),
					BCD_INT(sRouterStatus.sDateTimeOff.date_time[2]),BCD_INT(sRouterStatus.sDateTimeOff.date_time[1]),BCD_INT(sRouterStatus.sDateTimeOff.date_time[0]));

					m_Grid.SetItemText(m_iSelectItem,C_TIME_OFF,::TimeToSQL(time2));

					text.Format("%u",sRouterStatus.u8CSQ);
					m_Grid.SetItemText(m_iSelectItem,C_CSQ,text);

					text.Format("%u",sRouterStatus.u8JenniceModuleState);
					m_Grid.SetItemText(m_iSelectItem,C_STATE,text);

					text.Format("%u",sRouterStatus.u8Outputs);
					m_Grid.SetItemText(m_iSelectItem,C_RELAY,text);

					uint32_t ver = ntohl(sRouterStatus.u32HostVersion);
					text.Format("%4X.%X.%02X",ver>>16,(ver>>8)&0xFF,ver&0xFF);
					m_Grid.SetItemText(m_iSelectItem,C_HOST_VERSION,text);
					
					ver = ntohl(sRouterStatus.u32JennicDeviceVersion);
					text.Format("%4X.%X.%02X",ver>>16,(ver>>8)&0xFF,ver&0xFF);
					m_Grid.SetItemText(m_iSelectItem,C_JENNIC_VERSION,text);
					
					text.LoadString(IDS_SUCSES);
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

void CHardwareSetup::OnButtonTest() 
{
	// TODO: Add your control notification handler code here

	for(int i = 1; i < m_Grid.GetRowCount() ; i++){
		m_Grid.SetItemText(i,C_SIM_ERRORS,"");
		m_Grid.SetItemText(i,C_RESET_SIM,"");
		m_Grid.SetItemText(i,C_ON_COUNTER,"");
		m_Grid.SetItemText(i,C_CLEAR_RAM,"");
		m_Grid.SetItemText(i,C_TIME_ON,"");
		m_Grid.SetItemText(i,C_TIME_OFF,"");
		m_Grid.SetItemText(i,C_CSQ,"");
		m_Grid.SetItemText(i,C_STATE,"");
		m_Grid.SetItemText(i,C_RELAY,"");
		m_Grid.SetItemText(i,C_HOST_VERSION,"");
		m_Grid.SetItemText(i,C_JENNIC_VERSION,"");
		m_Grid.SetItemText(i,C_SUCSES,"");
	}

	CString enabled;
	enabled.LoadString(IDS_WORK);
	int old_iSelectItem = m_iSelectItem;
	for( m_iSelectItem = 1; m_iSelectItem < m_Grid.GetRowCount() ; m_iSelectItem++){
		CString text;
		text=m_Grid.GetItemText(m_iSelectItem,C_STOP);
		if(enabled == text){
			OnButtonTest1();
		}
	}
	m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
	m_Grid.SetFocusCell(m_iSelectItem,0);
	m_Grid.RedrawWindow();
	m_Grid.SetFocus();
	m_Grid.EnsureVisible(m_iSelectItem,0);
}

void CHardwareSetup::OnButtonOn1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString text;
		{
			
			text=m_Grid.GetItemText(m_iSelectItem,C_HARDWARE);
			
			BeginWaitCursor();
			
			int recived=JennicModule.OnRelay(text);
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

void CHardwareSetup::OnButtonOn() 
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
			OnButtonOn1();
		}
	}
	m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
	m_Grid.SetFocusCell(m_iSelectItem,0);
	m_Grid.RedrawWindow();
	m_Grid.SetFocus();
	m_Grid.EnsureVisible(m_iSelectItem,0);
}



void CHardwareSetup::OnButtonOff1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString text;
		{
			
			text=m_Grid.GetItemText(m_iSelectItem,C_HARDWARE);
			
			BeginWaitCursor();
			
			int recived=JennicModule.OffRelay(text);
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

void CHardwareSetup::OnButtonOff() 
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
			OnButtonOff1();
		}
	}
	m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
	m_Grid.SetFocusCell(m_iSelectItem,0);
	m_Grid.RedrawWindow();
	m_Grid.SetFocus();
	m_Grid.EnsureVisible(m_iSelectItem,0);
}

void CHardwareSetup::OnButtonLight1() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(true))
		return;

	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString text;
		{
			
			text=m_Grid.GetItemText(m_iSelectItem,C_HARDWARE);
			
			BeginWaitCursor();
			
			int recived=JennicModule.BroadcastLight(text,m_iLights);
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

void CHardwareSetup::OnButtonLight() 
{
	// TODO: Add your control notification handler code here
	if(!UpdateData(true))
		return;

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
			OnButtonLight1();
		}
	}
	m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
	m_Grid.SetFocusCell(m_iSelectItem,0);
	m_Grid.RedrawWindow();
	m_Grid.SetFocus();
	m_Grid.EnsureVisible(m_iSelectItem,0);
}



void CHardwareSetup::OnButtonClearRam1() 
{
	// TODO: Add your control notification handler code here
	m_Grid.SetItemText(m_iSelectItem,C_SUCSES,"");

	m_Grid.RedrawWindow();

	{
		CString text;
		{
			
			BeginWaitCursor();
			
			int recived=JennicModule.ClearRAM(m_Grid.GetItemText(m_iSelectItem,C_HARDWARE));
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

void CHardwareSetup::OnButtonClearRam() 
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
			OnButtonClearRam1();
		}
	}
	m_Grid.SetFocusCell(m_iSelectItem=old_iSelectItem,0);
	m_Grid.SetFocusCell(m_iSelectItem,0);
	m_Grid.RedrawWindow();
	m_Grid.SetFocus();
	m_Grid.EnsureVisible(m_iSelectItem,0);
}
