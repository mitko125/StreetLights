// TypeLampSetup.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "TypeLampSetup.h"

#include "DynamicLib.h"

#include "..\Free_Util_Dll\Util_Dll.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define R_HEADER 1

#define C_NAME 0
#define C_POWER 1
#define MAX_COLUMS 2

/////////////////////////////////////////////////////////////////////////////
// CTypeLampSetup property page

IMPLEMENT_DYNCREATE(CTypeLampSetup, CPropertyPage)

CTypeLampSetup::CTypeLampSetup() : CPropertyPage(CTypeLampSetup::IDD)
{
	m_iSelectItem=-1;
	fl_config=false;
	//{{AFX_DATA_INIT(CTypeLampSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CTypeLampSetup::~CTypeLampSetup()
{
}

void CTypeLampSetup::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTypeLampSetup)
	DDX_Control(pDX, IDC_BUTTON_NEW, m_bNew);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_bDel);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_Grid);
}


BEGIN_MESSAGE_MAP(CTypeLampSetup, CPropertyPage)
	//{{AFX_MSG_MAP(CTypeLampSetup)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_PREVIEV, OnButtonPrintPreviev)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTypeLampSetup message handlers

BOOL CTypeLampSetup::ContinueModal()
{
	if (!IsWindowVisible())
		// send WM_IDLEUPDATECMDUI message to update toolbar state
		// This is normally called by OnIdle function in SDI or MSI applications.
		// Dialog based applications don't call OnIdle, so send the message from here instead
		AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
			(WPARAM)TRUE, 0, TRUE, TRUE);

	return CDialog::ContinueModal();
}

void CTypeLampSetup::StoreConfigMessage()
{
	if(!fl_config){
		fl_config=true;
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		ASSERT_VALID(pApp);
		::StoreDataConfigMessage(pApp->Id_PC,pApp->Id_Operator,_CONFIG_LAMPS);
	}
}

#include "EditName.h"
void CTypeLampSetup::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	CEditName EN;
	if(EN.DoModal() == IDOK) {
		_CommandPtr m_pCmd=NULL;
		
		try{
			CString com;
			com.Format(_T("type_name='%s'"),EN.m_cName);
			if(::GetSqlCount(_T("lamp_types"),com)==0){
				com.Format(_T("INSERT INTO lamp_types(type_name) VALUES ('%s')"),EN.m_cName);
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

void CTypeLampSetup::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	CString mes;
	CString com;
	mes.LoadString(IDS_DELETE);
	mes+=m_Grid.GetItemText(m_iSelectItem,C_NAME);;
	if(AfxMessageBox(mes, MB_YESNO |	MB_DEFBUTTON2|MB_ICONQUESTION)==IDYES){
		_CommandPtr m_pCmd=NULL;
		try{
			UINT32 ID=m_Grid.GetItemData(m_iSelectItem,C_NAME);
			
			com.Format(_T("id_type_lamp=%u"),ID);
			if(::GetSqlCount(_T("lamps"),com)==0){
				com.Format(_T("DELETE FROM lamp_types WHERE id_type_lamp=%u"),ID);
				TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
				m_pCmd->ActiveConnection=::GetConnection();
				m_pCmd->CommandText=(LPCTSTR)com;
				m_pCmd->Execute(NULL, NULL, adCmdText);
				StoreConfigMessage();
				m_iSelectItem=-1;
				LoadData();
			}else{
				com.Format(IDS_BUSY_IN_DEVICES,m_Grid.GetItemText(m_iSelectItem,C_NAME));
				AfxMessageBox(com);
			}
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}
	}
}

BOOL CTypeLampSetup::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	BeginWaitCursor();
	CString text;
			

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
	Item1.nFormat=Item.nFormat=DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
	
	text.LoadString(IDS_TYPE_LAMP);
	m_Grid.SetColumnWidth(C_NAME,120);
	m_Grid.SetItemText(0,C_NAME,text);

	text.LoadString(IDS_POWER_W);
	m_Grid.SetColumnWidth(C_POWER,100);
	m_Grid.SetItemText(0,C_POWER,text);
	Item1.col=Item.col=C_POWER; m_Grid.SetItem(&Item); m_Grid.SetItem(&Item1);

	m_Grid.SetHeaderSort(true);
	
#ifndef _DEBUG
//	m_bLoadLamp.EnableWindow(false);
//	m_bLoadRouter.EnableWindow(false);
#endif
	return LoadData(true); 
}

BOOL CTypeLampSetup::LoadData(bool fl_select)
{
	BOOL flag=true;
	CString text;
	_RecordsetPtr m_pRs=NULL;
	
  m_Grid.SetRowCount(R_HEADER);
	  
	_bstr_t strCom("SELECT id_type_lamp,type_name,power_W FROM lamp_types ORDER BY id_type_lamp");
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

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				text.Format("%u",::GetLongFromVariant(data));
				m_Grid.SetItemText(i,C_POWER,text);

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

BOOL CTypeLampSetup::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
									::CStringSize(text,25);
									com.Format(_T("type_name='%s' AND id_type_lamp != %u"),text,ID);
									if(::GetSqlCount(_T("lamp_types"),com)==0){
										com.Format(_T("UPDATE lamp_types SET type_name='%s' WHERE id_type_lamp=%u"),text,ID);
										TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
										m_pCmd->ActiveConnection=::GetConnection();
										m_pCmd->CommandText=(LPCTSTR)com;
										m_pCmd->Execute(NULL, NULL, adCmdText);	
										StoreConfigMessage();
										m_Grid.SetItemText(pDispInfo->item.row,C_NAME,text);
									}	else{	
										com.Format(IDS_NAME_IS,text);
										AfxMessageBox(com);	
										m_Grid.SetItemText(pDispInfo->item.row,C_NAME,OldText);
									}
								}
								break;
							case C_POWER:
								{
									UINT32 data=::UI32Format(m_Grid.GetItemText(pDispInfo->item.row,C_POWER),99999);
									text.Format("%u",data);
									m_Grid.SetItemText(pDispInfo->item.row,C_POWER,text);
									com.Format(_T("UPDATE lamp_types SET power_W=%u WHERE id_type_lamp=%u"),data,ID);
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
					case C_NAME:
					case C_POWER:
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

void CTypeLampSetup::EnableKeys()
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

void CTypeLampSetup::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrint(&m_Grid,this);
}

void CTypeLampSetup::OnButtonPrintPreviev() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrintPreviev(&m_Grid,this);			
}