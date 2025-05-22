// OperatorSetup.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "OperatorSetup.h"

#include "OperatorManager.h"

#include "..\Free_Util_Dll\Util_Dll.h"
#include	"../../Grid1_0/NewCellTypes/GridCellCombo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define C_NAME 0
#define C_PRIVILEGI 1
#define MAX_COLUMS 2

/////////////////////////////////////////////////////////////////////////////
// COperatorSetup dialog


COperatorSetup::COperatorSetup(CWnd* pParent /*=NULL*/)
	: CDialog(COperatorSetup::IDD, pParent)
{
	//{{AFX_DATA_INIT(COperatorSetup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	fl_config=false;
	m_iSelectItem=-1;
}


void COperatorSetup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COperatorSetup)
	DDX_Control(pDX, IDC_BUTTON_NEW, m_bNew);
	DDX_Control(pDX, IDC_BUTTON_DEL, m_bDel);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_GRID, m_Grid); 
}


BEGIN_MESSAGE_MAP(COperatorSetup, CDialog)
	//{{AFX_MSG_MAP(COperatorSetup)
	ON_BN_CLICKED(IDC_BUTTON_PRINT_PREVIEV, OnButtonPrintPreviev)
	ON_BN_CLICKED(IDC_BUTTON_PRINT, OnButtonPrint)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COperatorSetup message handlers

void COperatorSetup::OnButtonPrintPreviev() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrintPreviev(&m_Grid,this);	
}

void COperatorSetup::OnButtonPrint() 
{
	// TODO: Add your control notification handler code here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	pApp->GridPrint(&m_Grid,this);
}

void COperatorSetup::StoreConfigMessage()
{
	if(!fl_config){
		fl_config=true;
		CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
		ASSERT_VALID(pApp);
		::StoreDataConfigMessage(pApp->Id_PC,pApp->Id_Operator,_CONFIG_OPERATORS);
	}
}

BOOL COperatorSetup::ContinueModal()
{
	if (!IsWindowVisible())
		// send WM_IDLEUPDATECMDUI message to update toolbar state
		// This is normally called by OnIdle function in SDI or MSI applications.
		// Dialog based applications don't call OnIdle, so send the message from here instead
		AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_IDLEUPDATECMDUI,
			(WPARAM)TRUE, 0, TRUE, TRUE);

	return CDialog::ContinueModal();
}

void COperatorSetup::EnableKeys()
{
	BOOL bNew,bDel;
	bNew=bDel=false;
	if(m_iSelectItem>=1){
		bNew=bDel=true;
		if(m_Grid.GetItemData(m_iSelectItem,C_NAME)==0)
			bDel=false;
	}else
		bNew=true;
	m_bNew.EnableWindow(bNew);
	m_bDel.EnableWindow(bDel);
}

BOOL COperatorSetup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
		BeginWaitCursor();
	CString text;

	CStringArray optPrivilegi;
	text.LoadString(IDS_OPER);
	optPrivilegi.Add(text);
	text.LoadString(IDS_SERVIZ);
	optPrivilegi.Add(text);
	text.LoadString(IDS_ADMIN);
	optPrivilegi.Add(text);

	
	m_Grid.SetEditable(true);
	m_Grid.EnableSelection(false);
	m_Grid.SetHandleTabKey(false);
	m_Grid.EnableLitleCell();
	TRY {
		m_Grid.SetFixedRowCount(1);
		m_Grid.SetFixedColumnCount(0);
	  m_Grid.SetRowCount(1);
	  m_Grid.SetColumnCount(MAX_COLUMS);
	}
	CATCH (CMemoryException, e) {
		e->ReportError();
  	return false;
	}END_CATCH

	text.LoadString(IDS_NAME);
	m_Grid.SetColumnWidth(C_NAME,120);
	m_Grid.SetItemText(0,C_NAME,text);

	text.LoadString(IDS_PRIVILEGI);
	m_Grid.SetColumnWidth(C_PRIVILEGI,120);
	m_Grid.SetItemText(0,C_PRIVILEGI,text);

	m_Grid.SetCellType(-1,C_PRIVILEGI, RUNTIME_CLASS(CGridCellCombo));
	CGridCellCombo *pCell = (CGridCellCombo*) m_Grid.GetCell(-1,C_PRIVILEGI);
  pCell->SetOptions(optPrivilegi);
  pCell->SetStyle(CBS_DROPDOWNLIST);

	m_Grid.SetFixedColumnSelection(TRUE);
	m_Grid.SetHeaderSort(true);
	
	return LoadData(true);   // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL COperatorSetup::LoadData(bool fl_select)
{
	BOOL flag=true;
	CString text;
	_RecordsetPtr m_pRs=NULL;
	
  m_Grid.SetRowCount(1);
	  
	_bstr_t strCom("SELECT id_operator,name_operator,privilege FROM operators ORDER BY id_operator");
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
				m_Grid.SetItemData(i,C_NAME,::GetLongFromVariant(data));	

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				m_Grid.SetItemText(i,C_NAME,::GetStringFromVariant(data));

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				switch(::GetLongFromVariant(data)){
				case SERVIZ:
					text.LoadString(IDS_SERVIZ);
					break;
				case CONFIG:
					text.LoadString(IDS_ADMIN);
					break;
				default:
					text.LoadString(IDS_OPER);
					break;
				}
				m_Grid.SetItemText(i,C_PRIVILEGI,text);
				
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

BOOL COperatorSetup::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
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
									com.Format(_T("name_operator='%s' AND id_operator != %u"),text,ID);
									if(::GetSqlCount(_T("operators"),com)==0){
										com.Format(_T("UPDATE operators SET name_operator='%s' WHERE id_operator=%u"),text,ID);
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
							case C_PRIVILEGI:
								{
									int privilege = 0;
									text=m_Grid.GetItemText(pDispInfo->item.row,C_PRIVILEGI);
									com.LoadString(IDS_SERVIZ);
									if(com == text)
										privilege = 1;
									else{
										com.LoadString(IDS_ADMIN);
										if(com == text)
											privilege = 2;
									}

									com.Format(_T("UPDATE operators SET privilege=%d WHERE id_operator=%u"),privilege,ID);
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
			case GVN_SELCHANGED:
				if(pDispInfo->item.row>0){
					m_iSelectItem=pDispInfo->item.row;
				}else
					m_iSelectItem=-1;	
				EnableKeys();
				return true;
			case GVN_BEGINLABELEDIT:
				OldText=m_Grid.GetItemText(pDispInfo->item.row,pDispInfo->item.col);
				return true;
		}
	}	
	return CDialog::OnNotify(wParam, lParam, pResult);
}

void COperatorSetup::OnButtonDel() 
{
	// TODO: Add your control notification handler code here
	CString mes;
	mes.LoadString(IDS_DELETE);
	mes+=m_Grid.GetItemText(m_iSelectItem,C_NAME);
	if(AfxMessageBox(mes, MB_YESNO |	MB_DEFBUTTON2|MB_ICONQUESTION)==IDYES){
		_CommandPtr m_pCmd=NULL;
		try{
			CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
			UINT32 ID=m_Grid.GetItemData(m_iSelectItem,C_NAME);

			if(ID == pApp->Id_Operator){
				AfxMessageBox(IDS_CURRENT_OPERATOR);
				return;
			}
			CString com;
			com.Format(_T("id_operator=%u"),ID);
			if(::GetSqlCount(_T("accounts_device"),com)==0){
				if(::GetSqlCount(_T("accounts_system"),com)==0){
					com.Format(_T("DELETE FROM operators WHERE id_operator=%u"),ID);
					TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
					m_pCmd->ActiveConnection=::GetConnection();
					m_pCmd->CommandText=(LPCTSTR)com;
					m_pCmd->Execute(NULL, NULL, adCmdText);
					StoreConfigMessage();
					m_iSelectItem=-1;
					LoadData();
				}else{
					com.Format(IDS_BUSY_IN_ACCOUNT,m_Grid.GetItemText(m_iSelectItem,C_NAME));
					AfxMessageBox(com);
				}
			}else{
				com.Format(IDS_BUSY_IN_ACCOUNT,m_Grid.GetItemText(m_iSelectItem,C_NAME));
				AfxMessageBox(com);
			}
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}
	}
}

#include "NewOperator.h"
void COperatorSetup::OnButtonNew() 
{
	// TODO: Add your control notification handler code here
	CNewOperator NOD;
	if(NOD.DoModal()==IDOK){
		{
			_RecordsetPtr m_pRs=NULL;
			_CommandPtr m_pCmd=NULL;
		
			CString com;
			try{	
				com.Format(_T("name_operator='%s'"),NOD.m_Name);
				if(::GetSqlCount(_T("operators"),com)==0){
					com.Format(_T("INSERT INTO operators(id_operator,name_operator,password_operator) VALUES(NULL,'%s','%s');"),
						NOD.m_Name,::CodePassword(NOD.m_Password,24));
					TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
					m_pCmd->ActiveConnection=::GetConnection();
					m_pCmd->CommandText=(LPCTSTR)com;
					m_pCmd->Execute(NULL, NULL, adCmdText);
					StoreConfigMessage();
					m_iSelectItem=-1;
					LoadData();
				}else{
					com.Format(IDS_NAME_IS,NOD.m_Name);
					AfxMessageBox(com);
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
}
