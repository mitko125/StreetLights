// AutoOnOff.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "AutoOnOff.h"

#include "DynamicLib.h"
#include "..\Free_Util_Dll\Util_Dll.h"

#include "SunSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define ID_TIMER_AUTO_ON_OFF 1001

/////////////////////////////////////////////////////////////////////////////
// CAutoOnOff dialog


CAutoOnOff::CAutoOnOff(CWnd* pParent /*=NULL*/)
	: CDialog(CAutoOnOff::IDD, pParent)
{
	id_timer = -1;
	m_iCloseTimer = 0;
	//{{AFX_DATA_INIT(CAutoOnOff)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CAutoOnOff::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAutoOnOff)
	DDX_Control(pDX, IDC_SUNSET_OFFSET, m_cSunsetOffset);
	DDX_Control(pDX, IDC_SUNRISEOFFSET, m_cSunriseOffset);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAutoOnOff, CDialog)
	//{{AFX_MSG_MAP(CAutoOnOff)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAutoOnOff message handlers

BOOL CAutoOnOff::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	ASSERT_VALID(pApp);
		
			
	SYSTEMTIME lpSystemTime;
	GetSystemTime(&lpSystemTime);
	SYSTEMTIME lpLocalTime;
	GetLocalTime(&lpLocalTime);
	if( lpLocalTime.wMinute != lpSystemTime.wMinute )
		GetSystemTime(&lpSystemTime);
	int HourOffset = (lpLocalTime.wHour - lpSystemTime.wHour + 24 )%24;
		
	SunSet sun;
	sun.setPosition(pApp->m_dLat, pApp->m_dLon, HourOffset);
	sun.setCurrentDate(lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay);
		
	CString text_on;
	CString text_off;
	long minute;
	minute = (long int)sun.calcSunrise();
	text_off.Format(_T("%2d:%02d"),((minute-pApp->m_OffsetOnOff)/60)%24,(minute-pApp->m_OffsetOnOff)%60);
	m_cSunriseOffset.SetWindowText(text_off);
		
	minute = (long int)sun.calcSunset();
	text_on.Format(_T("%2d:%02d"),((minute+pApp->m_OffsetOnOff)/60)%24,(minute+pApp->m_OffsetOnOff)%60);
	m_cSunsetOffset.SetWindowText(text_on);
	


	_RecordsetPtr m_pRs=NULL;
	_bstr_t strCom("SELECT * FROM timers ORDER BY id_timer");
	try{
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open(strCom,_variant_t((IDispatch *)GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
				
		_variant_t index;
		index.vt = VT_I2;
		_variant_t data;
		if(!m_pRs->EndOfFile){		
			TRY {
				index.iVal = 0; data=m_pRs->Fields->GetItem(&index)->Value;
				id_timer = ::GetLongFromVariant(data);
			}
			m_pRs->MoveNext();
			CATCH (CMemoryException, e){
				e->ReportError();
			}END_CATCH
		}		
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}
	if (m_pRs)
		if (m_pRs->State == adStateOpen)
			m_pRs->Close();

	_CommandPtr m_pCmd=NULL;
	CString com;

	if( id_timer >= 0 ){
		try{
			{
				unsigned int ui1=0,ui2=0;
				char c;
				
				sscanf(text_on,"%u%c%u",&ui1,&c,&ui2);
				ui1 = ui1%24; ui2 = ui2%60;
				ui1 *= 60;	ui1+=ui2;
				com.Format(_T("UPDATE timers SET time_on=%u WHERE id_timer=%u"),ui1,id_timer);
				TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
				m_pCmd->ActiveConnection=::GetConnection();
				m_pCmd->CommandText=(LPCTSTR)com;
				m_pCmd->Execute(NULL, NULL, adCmdText);	
			}
			{
				unsigned int ui1=0,ui2=0;
				char c;
				
				sscanf(text_off,"%u%c%u",&ui1,&c,&ui2);
				ui1 = ui1%24; ui2 = ui2%60;
				ui1 *= 60;	ui1+=ui2;
				com.Format(_T("UPDATE timers SET time_off=%u WHERE id_timer=%u"),ui1,id_timer);
				TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
				m_pCmd->ActiveConnection=::GetConnection();
				m_pCmd->CommandText=(LPCTSTR)com;
				m_pCmd->Execute(NULL, NULL, adCmdText);	
			}
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}
		m_iCloseTimer = SetTimer(ID_TIMER_AUTO_ON_OFF,100,NULL);
	}else
		OnOK();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CAutoOnOff::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	BeginWaitCursor();

	_RecordsetPtr m_pRs=NULL;
	

	CString com;
	//com.Format(_T("SELECT bit_groups FROM lamps WHERE id_lamp=%u"),m_Grid.GetItemData(pDispInfo->item.row,C_NUMBER));
	com.Format(_T("SELECT number_hardware FROM hardwares WHERE id_timer=%u AND stop=0 ORDER BY id_hardware"),id_timer);

	try{
		TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
		m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *)::GetConnection(), true),adOpenForwardOnly,adLockOptimistic,adCmdUnknown);

				
		_variant_t index;
		index.vt = VT_I2;
		_variant_t data;
		while(!m_pRs->EndOfFile){		
			CString text;
				
			index.iVal = 0; data=m_pRs->Fields->GetItem(&index)->Value;
			text = ::GetStringFromVariant(data);

			JennicModule.SetTimers(text,id_timer);

			m_pRs->MoveNext();
		}
	}
	catch(_com_error &e){
		::GenerateSqlError(e);
	}
	if (m_pRs)
		if (m_pRs->State == adStateOpen)
			m_pRs->Close();

	CDialog::OnTimer(nIDEvent);

	if( m_iCloseTimer )
		KillTimer(m_iCloseTimer);
	EndWaitCursor();
	OnOK();
}
