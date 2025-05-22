#include "stdafx.h"
#include "TV_Tracers.h"

#include "DataViewDoc.h"
#include "DataViewView.h"
#include "TextLine.h"

#include "OperatorManager.h"

#include "DynamicLib.h"

#include "../../Grid1_0/GridCtrl.h"
#include "..\Free_Util_Dll\Util_Dll.h"
#include	"../../Grid1_0/NewCellTypes/GridCellNumeric.h"

CDataViewDoc * CTV_TracersApp::GetNewDataViewDoc(BOOL bMakeVisible /*= TRUE */)
{
	POSITION pos=m_pDocManager->GetFirstDocTemplatePosition();
	CDocTemplate *m_pDocTemplate;
	for(int h = 0; h < ACOUTS_VIEW_POS; h ++)
		m_pDocTemplate=m_pDocManager->GetNextDocTemplate(pos);

	CDataViewDoc * pDVDoc=(CDataViewDoc*)m_pDocTemplate->OpenDocumentFile(NULL,bMakeVisible);
	ASSERT(pDVDoc!=NULL);
#ifndef _DEBUG
	pDVDoc->SetModifiedFlag();
#endif
	return pDVDoc;
}

void CTV_TracersApp::SetAccountTitle(CDataViewDoc *pSDoc, UINT ids)
{
	if(pSDoc){
		CTextLine *pTL=new CTextLine;
		CString text;
		text.LoadString(ids);
		pTL->AddText(text,400,TA_CENTER);
		pSDoc->AddTextLine(pTL);
	}
}

void CTV_TracersApp::SetDefaultGrid(CGridCtrl *pGrid)
{
	pGrid->SetEditable(false);
	pGrid->EnableSelection(true);
	pGrid->SetHandleTabKey(false);
	pGrid->EnableLitleCell();
	pGrid->SetFixedColumnSelection(true);
	pGrid->SetFixedRowSelection(true);
	pGrid->SetHeaderSort(true);
	pGrid->SetRowResize(true);
	pGrid->SetColumnResize(true);;
}

CGridCtrl * CTV_TracersApp::CreateDefaultGrid()
{
	CGridCtrl * pGrid=new CGridCtrl;
	SetDefaultGrid(pGrid);
	return pGrid;
}

CString CTV_TracersApp::NewAccountTitle(UINT id)
{
	CString name=_T("  _");

	name+=_T("_");
	name+=::TimeToSQL(COleDateTime::GetCurrentTime());

	int i;
	for( i=0;(i=name.FindOneOf(_T(" ")))!=-1;)
		name.SetAt(i,_T('_'));
	for(i=0;(i=name.FindOneOf(_T(":")))!=-1;)
		name.SetAt(i,_T('-'));
	CString name1;
	name1.LoadString(id);
	name1+=name.Mid(2);

	return name1;
}


void CTV_TracersApp::OnAccountsSystem() 
{
	// TODO: Add your command handler code here
	CDataViewDoc * pSDoc=GetNewDataViewDoc();

	SetAccountTitle(pSDoc,IDS_SYSTEM_ACCOUNT_NAME1);
	{
		CString com;
		_CommandPtr m_pCmd=NULL;
		_RecordsetPtr m_pRs=NULL;
		try{
			const BYTE C_PC=0;
			const BYTE C_NAME=1;
			const BYTE C_TIME=2;
			const BYTE C_TYPE=3;
			const BYTE MAX_COLS=4;
			CGridCtrl * pGrid=CreateDefaultGrid();
			TRY {
				pGrid->SetFixedRowCount(1);
				pGrid->SetFixedColumnCount(0);
				pGrid->SetRowCount(1);
				pGrid->SetColumnCount(MAX_COLS);
				
				GV_ITEM Item,Item1;
				Item1.mask=Item.mask = GVIF_FORMAT;
				Item.row=0; Item1.row=-1;
				Item1.nFormat=Item.nFormat=DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
				Item1.col=Item.col=C_PC; pGrid->SetItem(&Item); pGrid->SetItem(&Item1);
				
				com.LoadString(IDS_NUMBER_PC);
				pGrid->SetColumnWidth(C_PC,40);
				pGrid->SetItemText(0,C_PC,com);
				
				com.LoadString(IDS_OPERATOR);
				pGrid->SetColumnWidth(C_NAME,120);
				pGrid->SetItemText(0,C_NAME,com);
				
				com.LoadString(IDS_DATETIME);
				pGrid->SetColumnWidth(C_TIME,140);
				pGrid->SetItemText(0,C_TIME,com);
				
				com.LoadString(IDS_SYSTEM_TYPE);
				pGrid->SetColumnWidth(C_TYPE,300);
				pGrid->SetItemText(0,C_TYPE,com);
				
			}
			CATCH (CMemoryException, e) {
				e->ReportError();
  		}END_CATCH

			com.Format(_T("SELECT id_pc,name_operator,")
				_T("date_time,id_message ") 
				_T("FROM accounts_system ")
				_T("LEFT JOIN operators USING(id_operator)")
				_T("WHERE 1=1 ")
				_T("ORDER BY accounts_system.date_time"));
			TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
			m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *)GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
			int i=1;
			while(!m_pRs->EndOfFile){
				double a=0;
				_variant_t index;
				index.vt = VT_I2;

				TRY {
					_variant_t data;
					
					pGrid->InsertRow(_T(""));

					index.iVal = 0; data=m_pRs->Fields->GetItem(&index)->Value;
					pGrid->SetItemText(i,C_PC,::GetStringFromVariant(data));

					index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
					pGrid->SetItemText(i,C_NAME,::GetStringFromVariant(data));
					
					index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
					pGrid->SetItemText(i,C_TIME,::GetStringFromVariant(data));

					index.iVal++;	data=m_pRs->Fields->GetItem(&index)->Value;
					switch(::GetLongFromVariant(data)){
						case _CONFIG_LAMPS:	com.LoadString(IDS_CONFIG_LAMPS);	break;
						case _CONFIG_OPERATORS:	com.LoadString(IDS_CONFIG_OPERATORS);	break;
						case _CONFIG_ROUTERS:	com.LoadString(IDS_CONFIG_ROUTERS);	break;
						case _CONFIG_PC_NUMBER:	com.LoadString(IDS_CONFIG_PC_NUMBER);	break;
						case _CONFIG_CONNECTION_DB:	com.LoadString(IDS_CONFIG_CONNECT);	break;
						case _CLEAR_ACCOUNTS:	com.LoadString(IDS_CLEAR_ACCOUNTS);	break;
						case _CONFIG_ENERGY_METERS:	com.LoadString(IDS_CONFIG_ENERGY_METERS);	break;
						default:
							ASSERT(false);
							com.LoadString(IDS_SYSTEM_UNKNOW);
							break;
					}
					pGrid->SetItemText(i,C_TYPE,com);

				}
			
				i++;
				CATCH (CMemoryException, e){
					e->ReportError();
				}END_CATCH

				m_pRs->MoveNext();
			}
			if(i==1)
				pGrid->InsertRow(_T(""));

			pSDoc->AddGrid(pGrid);
		}
		catch(_com_error &e){
			GenerateSqlError(e);
		}
		if (m_pRs)
			if (m_pRs->State == adStateOpen)
				m_pRs->Close();
	}
	pSDoc->SetTitle(NewAccountTitle(IDS_SYSTEM_ACCOUNT_NAME));
//	SetStartEndAccount(pSDoc,m_pAccountLimits);		
}

void CTV_TracersApp::OnAccountsClear() 
{
	// TODO: Add your command handler code here
//	CString mes
//	mes.LoadString(IDS_DELETE_ACCOUTS);
	if(AfxMessageBox(IDS_DELETE_ACCOUTS, MB_YESNO |	MB_DEFBUTTON2|MB_ICONQUESTION)==IDYES){
		_CommandPtr m_pCmd=NULL;
		try{
			CString com;
			
			TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
			m_pCmd->ActiveConnection=::GetConnection();

			com.Format(_T("DELETE FROM accounts_system WHERE 1=1"));
			m_pCmd->CommandText=(LPCTSTR)com;
			m_pCmd->Execute(NULL, NULL, adCmdText);

			::StoreDataConfigMessage(Id_PC,Id_Operator,_CLEAR_ACCOUNTS);
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}
	}
}

void CTV_TracersApp::OnUpdateAccountsClear(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(privilege == CONFIG);
}

#define R_HEADER 2

#define C_NUMBER 0
#define C_NUMBER_P 1
#define C_MAC_ADDRESS_P 2
#define C_LQI_P 3
#define C_PER_P 4
#define C_NUMBER_C 5
#define C_MAC_ADDRESS_C 6
#define C_LQI_C 7
#define C_PER_C 8
#define C_DEPTH 9
#define C_STREET_C 10
#define C_STREET_NUMBER_C 11
#define C_TIME 12
#define C_WORK_HOURS 13
#define C_LAST_TEST 14
#define MAX_COLUMS 15

#define MAX_NEIGHBOUR_BLOBS 10


int CTV_TracersApp::NewRown(int *num_line,int depth, CGridCtrl *pGrid, CString MAC_address, int lamp_number)
{
	CString text;
	int num_row = pGrid->GetRowCount();
	pGrid->InsertRow(_T(""));

	text.Format(_T("%d"),*num_line);
	(*num_line) ++;
	pGrid->SetItemText(num_row,C_NUMBER,text);

	text.Format(_T("%d"),depth);
	pGrid->SetItemText(num_row,C_DEPTH,text);

	if( lamp_number >= 0 ){
		text.Format(_T("%d"),lamp_number);
		pGrid->SetItemText(num_row,C_NUMBER_P,text);
	}

	pGrid->SetItemText(num_row,C_MAC_ADDRESS_P,MAC_address);

	return num_row;
}

void CTV_TracersApp::TestILQ(int * num_line,int depth,CGridCtrl *pGrid,CString ip_address,CString Old_MAC_address,int old_row,CString MAC_address,UINT32 id_hardware,int lamp_number)
{
	CString new_MAC_address[MAX_NEIGHBOUR_BLOBS];
	uint8_t u8LQI[MAX_NEIGHBOUR_BLOBS];
	uint8_t u8PER[MAX_NEIGHBOUR_BLOBS];

	uint16_t u16FirstTableEntry = 0;

	CString com;

	int num_row;

	int get_blobs;
	do{
		get_blobs = 0;
		uint8_t * pu8Data;
		int i16Lenght;
		int cou_error = 4;//2;//2;

		do{
			i16Lenght=JennicModule.GetNeighbourTableBlobs(ip_address, MAC_address, id_hardware, u16FirstTableEntry,MAX_NEIGHBOUR_BLOBS,&pu8Data);
			cou_error --;
		}while( ( i16Lenght == RECIVE_TIMEOUT ) && ( cou_error >= 0 ) );
		switch(i16Lenght){
		case NO_ROUTER:
			num_row = NewRown(num_line,depth-1,pGrid,MAC_address,lamp_number);
			com.LoadString(IDS_NO_RUTER);
			pGrid->SetItemText(num_row,C_MAC_ADDRESS_C,com);
			return;
		case NO_HARDWARE:
			num_row = NewRown(num_line,depth-1,pGrid,MAC_address,lamp_number);
			com.LoadString(IDS_NO_HARDWARE);
			pGrid->SetItemText(num_row,C_MAC_ADDRESS_C,com);
			return;
		case NO_CONNECT:
			num_row = NewRown(num_line,depth-1,pGrid,MAC_address,lamp_number);
			com.LoadString(IDS_NO_LAMP);
			pGrid->SetItemText(num_row,C_MAC_ADDRESS_C,com);
			return;
		default:
			if( i16Lenght < 0 ){
				num_row = NewRown(num_line,depth-1,pGrid,MAC_address,lamp_number);
				com.LoadString(IDS_ERROR);
				pGrid->SetItemText(num_row,C_MAC_ADDRESS_C,com);
				return;
			}else if( i16Lenght > 0 ){
				while( i16Lenght > 0 ){
					uint16_t EntryIndex = ((uint16_t)(*(pu8Data + 0))) << 8 | *(pu8Data + 1);
					uint8_t BlowLenght = *(pu8Data + 2);
										
					pu8Data += 3;
					if( BlowLenght == 10 ){
						unsigned char mac[17];
						unsigned char *p,*p1;
						p = mac;
						p1 = pu8Data;

						*p1 ^= 0x02;
						for( int i = 0 ; i < 8 ;i ++){
							unsigned char data = *p1;
							data >>= 4;
							if( data > 9 )
								*p++ = 'A' + data - 10;
							else
								*p++ = '0' + data;
							data = *p1++;
							data &= 0x0F;
							if( data > 9 )
								*p++ = 'A' + data - 10;
							else
								*p++ = '0' + data;
						}
						*p = 0 ;
						new_MAC_address[get_blobs] = mac;
						u8LQI[get_blobs] = *( pu8Data + 8 );
						u8PER[get_blobs] = *( pu8Data + 9 );
						
						u16FirstTableEntry ++;
						get_blobs ++;
					}
										
					pu8Data +=BlowLenght;
					i16Lenght -= 3+BlowLenght;
				}
				int i;
				for( i = 0 ; i < get_blobs ; i ++ ){
					if( new_MAC_address[i] == Old_MAC_address ){
						if( old_row > 0 ){
							com.Format(_T("%d"),u8LQI[i]);
							pGrid->SetItemText(old_row,C_LQI_P,com);
							com.Format(_T("%d"),u8PER[i]);
							pGrid->SetItemText(old_row,C_PER_P,com);
						}
						break;
					}
				}
				for( i = 0 ; i < get_blobs ; i ++ ){
					if( new_MAC_address[i] != Old_MAC_address ){
						_CommandPtr m_pCmd=NULL;
						_RecordsetPtr m_pRs=NULL;
						int new_lamp_number = -1;
						long work_hours;
						CString last_contact,last_test;
						CString street,street_number;

						try{					
							com.Format(_T("SELECT id_lamp,street_name,street_number,last_contact,work_hours,last_test ")
								_T("FROM lamps WHERE MAC_address = '%s'"),new_MAC_address[i]);
						
							TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
							m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *)GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
							
							if(!m_pRs->EndOfFile){
								_variant_t index;
								index.vt = VT_I2;
								_variant_t data;
								
								index.iVal = 0; data=m_pRs->Fields->GetItem(&index)->Value;
								new_lamp_number =::GetLongFromVariant(data);
								
								index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
								street = ::GetStringFromVariant(data);
								
								index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
								street_number = ::GetStringFromVariant(data);

								index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
								last_contact=::TimeToSQL(data);
								
								index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
								work_hours=::GetLongFromVariant(data);
									
								index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
								last_test=::TimeToSQL(data);
								
							}
						}
						catch(_com_error &e){
							GenerateSqlError(e);
						}
						if (m_pRs)
							if (m_pRs->State == adStateOpen)
								m_pRs->Close();	
							
						if( new_lamp_number >= 0){
							num_row = NewRown(num_line,depth,pGrid,MAC_address,lamp_number);

							com.Format(_T("%u"),new_lamp_number);
							pGrid->SetItemText(num_row,C_NUMBER_C,com);
							pGrid->SetItemText(num_row,C_MAC_ADDRESS_C,new_MAC_address[i]);

							com.Format(_T("%d"),u8LQI[i]);
							pGrid->SetItemText(num_row,C_LQI_C,com);
							com.Format(_T("%d"),u8PER[i]);
							pGrid->SetItemText(num_row,C_PER_C,com);

							pGrid->SetItemText(num_row,C_STREET_C,street);
							pGrid->SetItemText(num_row,C_STREET_NUMBER_C,street_number);

							pGrid->SetItemText(num_row,C_TIME,last_contact);
							com.Format(_T("%d"),work_hours);
							pGrid->SetItemText(num_row,C_WORK_HOURS,com);
							pGrid->SetItemText(num_row,C_LAST_TEST,last_test);

							TestILQ(num_line,depth+1,pGrid,ip_address,MAC_address,num_row,new_MAC_address[i],id_hardware,new_lamp_number);
						}
					}
				}
			}
			break;
		}
	}while( ( get_blobs != 0 ) && ( get_blobs == MAX_NEIGHBOUR_BLOBS ) );

	//NewRown(num_line,depth-1,pGrid,MAC_address,lamp_number);
}


#include "RouterSelect.h"

void CTV_TracersApp::OnTestLqi() 
{
	// TODO: Add your command handler code here
	CRouterSelect RS;
	if(RS.DoModal()==IDOK){
		if( RS.m_iNumberRouter ){
			CDataViewDoc * pSDoc=GetNewDataViewDoc();
			
			//SetAccountTitle(pSDoc,IDS_TEST_LQI1);
			{
				CTextLine *pTL=new CTextLine;
				CString text;
				text.Format(IDS_TEST_LQI1,RS.RouterName);
				pTL->AddText(text,400,TA_CENTER);
				pSDoc->AddTextLine(pTL);
			}

			{
				CString com;
				_CommandPtr m_pCmd=NULL;
				_RecordsetPtr m_pRs=NULL;
				CString ip_address = _T("");
				CString MAC_address = _T("");
				CGridCtrl * pGrid=CreateDefaultGrid();
				pGrid->EnableSelection(false);

				try{
					
					TRY {
						pGrid->SetFixedRowCount(R_HEADER);
						pGrid->SetFixedColumnCount(0);
						pGrid->SetRowCount(R_HEADER);
						pGrid->SetColumnCount(MAX_COLUMS);
						
						GV_ITEM Item,Item1;
						Item1.mask=Item.mask = GVIF_FORMAT;
						Item.row=1; Item1.row=-1;
						Item1.nFormat=Item.nFormat=DT_RIGHT|DT_VCENTER|DT_SINGLELINE;
						
						pGrid->SetCellType(-1,C_NUMBER,RUNTIME_CLASS(CGridCellNumeric));
						pGrid->SetCompareFunction(CGridCtrl::numeric_cmp,C_NUMBER);
						com.LoadString(IDS_NUMBER );
						pGrid->SetColumnWidth(C_NUMBER,40);
						pGrid->SetItemText(0,C_NUMBER,com);
						Item1.col=Item.col=C_NUMBER; pGrid->SetItem(&Item); pGrid->SetItem(&Item1);
						pGrid->SetMerge(0,C_NUMBER,1,0,false);
						

						com.LoadString(IDS_PARENT);
						pGrid->SetItemText(0,C_NUMBER_P,com);
						pGrid->SetMerge(0,C_NUMBER_P,0,3,false);
						
						pGrid->SetCellType(-1,C_NUMBER_P,RUNTIME_CLASS(CGridCellNumeric));
						pGrid->SetCompareFunction(CGridCtrl::numeric_cmp,C_NUMBER_P);
						com.LoadString(IDS_NUMBER);
						pGrid->SetColumnWidth(C_NUMBER_P,40);
						pGrid->SetItemText(1,C_NUMBER_P,com);
						Item1.col=Item.col=C_NUMBER_P; pGrid->SetItem(&Item); pGrid->SetItem(&Item1);
						
						com.LoadString(IDS_MAC_ADDRESS);
						pGrid->SetColumnWidth(C_MAC_ADDRESS_P,120);
						pGrid->SetItemText(1,C_MAC_ADDRESS_P,com);
						
						pGrid->SetCellType(-1,C_LQI_P,RUNTIME_CLASS(CGridCellNumeric));
						pGrid->SetCompareFunction(CGridCtrl::numeric_cmp,C_LQI_P);
						com.LoadString(IDS_LQI);
						pGrid->SetColumnWidth(C_LQI_P,40);
						pGrid->SetItemText(1,C_LQI_P,com);
						Item1.col=Item.col=C_LQI_P; pGrid->SetItem(&Item); pGrid->SetItem(&Item1);
						
						pGrid->SetCellType(-1,C_PER_P,RUNTIME_CLASS(CGridCellNumeric));
						pGrid->SetCompareFunction(CGridCtrl::numeric_cmp,C_PER_P);
						com.LoadString(IDS_PER);
						pGrid->SetColumnWidth(C_PER_P,40);
						pGrid->SetItemText(1,C_PER_P,com);
						Item1.col=Item.col=C_PER_P; pGrid->SetItem(&Item); pGrid->SetItem(&Item1);
						
						
						
						com.LoadString(IDS_CHILDREN);
						pGrid->SetItemText(0,C_NUMBER_C,com);
						pGrid->SetMerge(0,C_NUMBER_C,0,9,false);
						
						pGrid->SetCellType(-1,C_NUMBER_C,RUNTIME_CLASS(CGridCellNumeric));
						pGrid->SetCompareFunction(CGridCtrl::numeric_cmp,C_NUMBER_C);
						com.LoadString(IDS_NUMBER);
						pGrid->SetColumnWidth(C_NUMBER_C,40);
						pGrid->SetItemText(1,C_NUMBER_C,com);
						Item1.col=Item.col=C_NUMBER_C; pGrid->SetItem(&Item); pGrid->SetItem(&Item1);
						
						com.LoadString(IDS_MAC_ADDRESS);
						pGrid->SetColumnWidth(C_MAC_ADDRESS_C,120);
						pGrid->SetItemText(1,C_MAC_ADDRESS_C,com);
						
						pGrid->SetCellType(-1,C_LQI_C,RUNTIME_CLASS(CGridCellNumeric));
						pGrid->SetCompareFunction(CGridCtrl::numeric_cmp,C_LQI_C);
						com.LoadString(IDS_LQI);
						pGrid->SetColumnWidth(C_LQI_C,40);
						pGrid->SetItemText(1,C_LQI_C,com);
						Item1.col=Item.col=C_LQI_C; pGrid->SetItem(&Item); pGrid->SetItem(&Item1);
						
						pGrid->SetCellType(-1,C_PER_C,RUNTIME_CLASS(CGridCellNumeric));
						pGrid->SetCompareFunction(CGridCtrl::numeric_cmp,C_PER_C);
						com.LoadString(IDS_PER);
						pGrid->SetColumnWidth(C_PER_C,40);
						pGrid->SetItemText(1,C_PER_C,com);
						Item1.col=Item.col=C_PER_C; pGrid->SetItem(&Item); pGrid->SetItem(&Item1);
						
						pGrid->SetCellType(-1,C_DEPTH,RUNTIME_CLASS(CGridCellNumeric));
						pGrid->SetCompareFunction(CGridCtrl::numeric_cmp,C_DEPTH);
						com.LoadString(IDS_DEPTH);
						pGrid->SetColumnWidth(C_DEPTH,40);
						pGrid->SetItemText(1,C_DEPTH,com);
						Item1.col=Item.col=C_DEPTH; pGrid->SetItem(&Item); pGrid->SetItem(&Item1);

						com.LoadString(IDS_STREET);
						pGrid->SetColumnWidth(C_STREET_C,110);
						pGrid->SetItemText(1,C_STREET_C,com);

						com.LoadString(IDS_STREET_NUMBER);
						pGrid->SetColumnWidth(C_STREET_NUMBER_C,110);
						pGrid->SetItemText(1,C_STREET_NUMBER_C,com);

						com.LoadString(IDS_LAST_CONTACT);
						pGrid->SetColumnWidth(C_TIME,120);
						pGrid->SetItemText(1,C_TIME,com);
						
						pGrid->SetCompareFunction(CGridCtrl::numeric_cmp,C_WORK_HOURS);
						com.LoadString(IDS_WORK_HOURS);
						pGrid->SetColumnWidth(C_WORK_HOURS,70);
						pGrid->SetItemText(1,C_WORK_HOURS,com);
						Item1.col=Item.col=C_WORK_HOURS; pGrid->SetItem(&Item); pGrid->SetItem(&Item1);
						
						com.LoadString(IDS_LAST_TEST);
						pGrid->SetColumnWidth(C_LAST_TEST,120);
						pGrid->SetItemText(1,C_LAST_TEST,com);
						
						pGrid->SetHeaderSort(true);
						
						
					}
					CATCH (CMemoryException, e) {
						e->ReportError();
					}END_CATCH
						
					

					com.Format(_T("SELECT number_hardware,MAC_address FROM hardwares ")
						_T("WHERE id_hardware=%u "),RS.m_iNumberRouter);
					TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
					m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *)GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);

					if(!m_pRs->EndOfFile){
						double a=0;
						_variant_t index;
						index.vt = VT_I2;
						_variant_t data;
						
						index.iVal = 0; data=m_pRs->Fields->GetItem(&index)->Value;
						ip_address=::GetStringFromVariant(data);
							
						index.iVal++; data=m_pRs->Fields->GetItem(&index)->Value;
						MAC_address=::GetStringFromVariant(data);
					}
				}
				catch(_com_error &e){
					GenerateSqlError(e);
				}
				if (m_pRs)
					if (m_pRs->State == adStateOpen)
						m_pRs->Close();		
					
				int num_line = 1;
				TestILQ(&num_line,1,pGrid,ip_address,_T(""),-1,MAC_address,RS.m_iNumberRouter,-1);
				if( p_hardware )
					p_hardware->Close();

				pSDoc->AddGrid(pGrid);
				
			}
			//pSDoc->SetTitle(NewAccountTitle(IDS_TEST_LQI));
			{
				CString name=_T("  _");
				
				name+=_T("_");
				name+=::TimeToSQL(COleDateTime::GetCurrentTime());
				
				int i;
				for( i=0;(i=name.FindOneOf(_T(" ")))!=-1;)
					name.SetAt(i,_T('_'));
				for(i=0;(i=name.FindOneOf(_T(":")))!=-1;)
					name.SetAt(i,_T('-'));
				CString name1;
				name1.Format(IDS_TEST_LQI,RS.m_iNumberRouter);
				name1+=name.Mid(2);
				pSDoc->SetTitle(name1);
			}
		}
	}
}