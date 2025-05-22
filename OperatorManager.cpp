// OperatorManager.cpp: implementation of the COperatorManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tv_tracers.h"
#include "OperatorManager.h"

#include "..\Free_Util_Dll\Util_Dll.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "OperatorSelect.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COperatorManager::COperatorManager()
{

}

COperatorManager::~COperatorManager()
{

}

BOOL COperatorManager::OperatorSelect(BOOL first_time)
{
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	CString com;
	com.Format(_T("1=1"));
	BOOL flag=true;
	_RecordsetPtr m_pRs=NULL;
	try{
		if(first_time){
			int ver,sub_ver;
			ver=0;
			sub_ver=-1;
			TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
			while((sub_ver<DATABASE_SUB_VERSION)&&flag){
				com=_T("SELECT version,sub_version FROM version");
				m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *)::GetConnection(), true),adOpenForwardOnly,adLockOptimistic,adCmdUnknown);
				if(!m_pRs->EndOfFile){
					_variant_t index,data;
					index.vt = VT_I2;
					index.iVal = 0;
					data=m_pRs->Fields->GetItem(&index)->Value;
					ver=::GetLongFromVariant(data);
					index.iVal ++;
					data=m_pRs->Fields->GetItem(&index)->Value;
					sub_ver=::GetLongFromVariant(data);
					if(ver!=DATABASE_VERSION){
						com.LoadString(IDS_UNKNOV_DATA_BASE);
						CString text;
						text.Format(com,ver,sub_ver);
						AfxMessageBox(text,MB_ICONSTOP);
						flag=false;
					}else{
						_CommandPtr m_pCmd=NULL;
						TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
						m_pCmd->ActiveConnection=::GetConnection();
						switch(sub_ver){
						case 0:
								com=_T("CREATE TABLE IF NOT EXISTS lamp_types(")
									_T("id_type_lamp INT(10) UNSIGNED NOT NULL AUTO_INCREMENT UNIQUE,")
									_T("PRIMARY KEY (id_type_lamp),")
									_T("type_name CHAR(25) NOT NULL)");
								m_pCmd->CommandText=(LPCTSTR)com;
								m_pCmd->Execute(NULL, NULL, adCmdText);
								try{
									com=_T("SELECT COUNT(lat) FROM hardwares");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE hardwares ADD lat DECIMAl(10,6) NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								try{
									com=_T("SELECT COUNT(lng) FROM hardwares");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE hardwares ADD lng DECIMAl(10,6) NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}

								try{
									com=_T("SELECT COUNT(lat) FROM lamps");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE lamps ADD lat DECIMAl(10,6) NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								try{
									com=_T("SELECT COUNT(lng) FROM lamps");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE lamps ADD lng DECIMAl(10,6) NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}

								try{
									com=_T("SELECT COUNT(id_type_lamp) FROM lamps");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE lamps ADD id_type_lamp INT(10) UNSIGNED DEFAULT 0 NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}

								try{
									com=_T("SELECT COUNT(label) FROM lamps");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE lamps ADD label CHAR(30) NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}

								com=_T("UPDATE version SET sub_version= 1 WHERE version=1 AND sub_version=0");
								m_pCmd->CommandText=(LPCTSTR)com;
								m_pCmd->Execute(NULL, NULL, adCmdText);
								if (m_pRs->State == adStateOpen)
									m_pRs->Close();
							break;
						case 1:
								try{
									com=_T("SELECT COUNT(time_on2) FROM timers");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE timers ADD time_on2 INT(4) UNSIGNED DEFAULT 0 NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								try{
									com=_T("SELECT COUNT(time_off2) FROM timers");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE timers ADD time_off2 INT(4) UNSIGNED DEFAULT 0 NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								com=_T("UPDATE version SET sub_version= 2 WHERE version=1 AND sub_version=1");
								m_pCmd->CommandText=(LPCTSTR)com;
								m_pCmd->Execute(NULL, NULL, adCmdText);
								if (m_pRs->State == adStateOpen)
									m_pRs->Close();
							break;
						case 2:
								try{
									com=_T("SELECT COUNT(power_W) FROM lamp_types");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE lamp_types ADD power_W INT(5) UNSIGNED DEFAULT 0 NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								try{
									com=_T("SELECT COUNT(resetGPRS_hh) FROM hardwares");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE `hardwares` ADD `resetGPRS_hh` TINYINT UNSIGNED DEFAULT 0 NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								try{
									com=_T("SELECT COUNT(resetGPRS_mm) FROM hardwares");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE `hardwares` ADD `resetGPRS_mm` TINYINT UNSIGNED DEFAULT 0 NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								try{
									com=_T("SELECT COUNT(enable_energy_meter) FROM hardwares");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE `hardwares` ADD `enable_energy_meter` TINYINT UNSIGNED DEFAULT 0 NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								com=_T("UPDATE version SET sub_version= 3 WHERE version=1 AND sub_version=2");
								m_pCmd->CommandText=(LPCTSTR)com;
								m_pCmd->Execute(NULL, NULL, adCmdText);
								if (m_pRs->State == adStateOpen)
									m_pRs->Close();
							break;
						
				/*		case 1:	//временно попълваше празните таблици за Средец
							{
								int i=109;
								for( i = 1101 ; i<1171; i++){
									com.Format(_T("INSERT INTO lamps SET id_lamp=%u,id_type_lamp=4"),i);
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								if (m_pRs->State == adStateOpen)
									m_pRs->Close();

							}
							break;*/
/*							case 0:
								com=_T("ALTER TABLE `clients` CHANGE `dannum` `dannum` CHAR(20)  NOT NULL");
								m_pCmd->CommandText=(LPCTSTR)com;
								m_pCmd->Execute(NULL, NULL, adCmdText);
								com=_T("UPDATE version SET sub_version= 1 WHERE version=1 AND sub_version=0");
								m_pCmd->CommandText=(LPCTSTR)com;
								m_pCmd->Execute(NULL, NULL, adCmdText);
								if (m_pRs->State == adStateOpen)
									m_pRs->Close();
								break;
							case 1:
								try{
									com=_T("SELECT COUNT(old_liter) FROM tanks");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE tanks ADD old_liter DECIMAL(6,0)  NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}	
								try{
									com=_T("SELECT COUNT(old_kg) FROM tanks");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE tanks ADD old_kg DECIMAL(6,0)  NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								com=_T("CREATE TABLE IF NOT EXISTS accounts_tehnical ")
									_T("SELECT id_operator,id_pc,id_tank,id_dispenser,id_nozzle,")
									_T("date_time,time_transaction,")
									_T("round_transaction,price,")
									_T("volume,amount,round FROM accounts_dispenser WHERE 1 = 0");
								m_pCmd->CommandText=(LPCTSTR)com;
								m_pCmd->Execute(NULL, NULL, adCmdText);
								try{
									com=_T("SELECT COUNT(err_liter) FROM protocols_pbd");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE protocols_pbd ADD err_liter DOUBLE NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								try{
									com=_T("SELECT COUNT(err_percent) FROM protocols_pbd");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								catch(_com_error &e){
									e;
									com=_T("ALTER TABLE protocols_pbd ADD err_percent  DOUBLE NOT NULL");
									m_pCmd->CommandText=(LPCTSTR)com;
									m_pCmd->Execute(NULL, NULL, adCmdText);
								}
								com=_T("UPDATE version SET sub_version= 2 WHERE version=1 AND sub_version=1");
								m_pCmd->CommandText=(LPCTSTR)com;
								m_pCmd->Execute(NULL, NULL, adCmdText);
								if (m_pRs->State == adStateOpen)
									m_pRs->Close();
								break;*/
						}
					}
				}
			}
		}
		if(flag){
			flag=false;
			com.Format(_T("1=1"));
			if(::GetSqlCount(_T("operators"),com)){
					COperatorSelect OSD;
					if(OSD.DoModal()==IDOK){
						pApp->m_cNameOperator=OSD.m_Name;
						flag=SetOperatorPrivilegies(pApp->Id_Operator=OSD.id_operator);
					}
			}else{
				AfxMessageBox(IDS_NOT_OPERATORS_INF);
				pApp->privilege=CONFIG;
				pApp->Id_Operator=0;
				pApp->m_cNameOperator.LoadString(IDS_NOT_OPERATOR);
				flag=true;
			}
		}
	}
	catch(_com_error &e){
		::GenerateSqlError(e);//GenerateSqlError(e); 
		flag=false;
	}	
	if (m_pRs)
		if (m_pRs->State == adStateOpen)
				m_pRs->Close();
	return flag;
}

BOOL COperatorManager::SetOperatorPrivilegies(UINT32 id_operator)
{
	BOOL flag=true;
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
	CString com;
	_RecordsetPtr m_pRs=NULL;
	try{
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		com.Format(_T("SELECT privilege FROM operators WHERE id_operator=%u"),id_operator);
		m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *)::GetConnection(), true),adOpenForwardOnly,adLockOptimistic,adCmdUnknown);
		_variant_t index;
		index.vt = VT_I2;
		_variant_t data;
		if(!m_pRs->EndOfFile){
			index.iVal = 0;
			data=m_pRs->Fields->GetItem(&index)->Value;
			UINT32 privilege=::GetLongFromVariant(data);
			
			pApp->privilege=privilege;
			
		}else
			pApp->privilege=CONFIG;
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
		flag=false;
	}	
	if (m_pRs)
		if (m_pRs->State == adStateOpen)
				m_pRs->Close();
	if(flag&&((pApp->privilege&CONFIG)==0)){
		BOOL fl_none=false;
		com.Format(_T("privilege='%d'"),CONFIG);
		if(::GetSqlCount(_T("operators"),com))
			fl_none=true;
		if(!fl_none)
			pApp->privilege=CONFIG;
	}
	return flag;
}
