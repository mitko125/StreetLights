// DateTimeLimits.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "DateTimeLimits.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDateTimeLimits dialog


CDateTimeLimits::CDateTimeLimits(CWnd* pParent /*=NULL*/)
	: CDialog(CDateTimeLimits::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDateTimeLimits)
	//}}AFX_DATA_INIT
}


void CDateTimeLimits::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDateTimeLimits)
	DDX_Control(pDX, IDC_TIME_MIN, m_TimeMin);
	DDX_Control(pDX, IDC_TIME_MAX, m_TimeMax);
	DDX_Control(pDX, IDC_DATE_MIN, m_DateMin);
	DDX_Control(pDX, IDC_DATE_MAX, m_DateMax);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDateTimeLimits, CDialog)
	//{{AFX_MSG_MAP(CDateTimeLimits)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDateTimeLimits message handlers

BOOL CDateTimeLimits::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_DateMax.SetFormat(_T("yyy-MM-dd"));
	m_DateMax.SetTime();
	m_DateMin.SetFormat(_T("yyy-MM-dd"));
	m_DateMin.SetTime();
	m_TimeMax.SetFormat(_T("HH:mm:ss"));
	m_TimeMax.SetTime();
	m_TimeMin.SetFormat(_T("HH:mm:ss"));
	m_TimeMin.SetTime();
/*	COleDateTime tt_max;
	m_DateMax.GetTime(tt_max);
	COleDateTime tt_day(0,0,1,0,0,0);
	tt_max = tt_max - tt_day;
	m_TimeMin.SetTime(tt_max);*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDateTimeLimits::OnOK() 
{
	// TODO: Add extra validation here
	m_DateMin.GetTime(d_min);
	if(d_min){
		d_min.SetDate(d_min.GetYear(),d_min.GetMonth(),d_min.GetDay());
		m_TimeMin.GetTime(t_min);
		if(t_min){
			t_min.SetTime(t_min.GetHour(),t_min.GetMinute(),t_min.GetSecond());
			d_min=d_min+t_min;
		}
	}

	m_DateMax.GetTime(d_max);
	if(d_max){
		d_max.SetDate(d_max.GetYear(),d_max.GetMonth(),d_max.GetDay());
		m_TimeMax.GetTime(t_max);
		if(t_max){
			t_max.SetTime(t_max.GetHour(),t_max.GetMinute(),t_max.GetSecond());
			d_max=d_max+t_max;
		}
	}
/*	if(d_min||d_max){
		CDialog::OnOK();
		m_pAccountLimits->not_clear=false;
		m_pAccountLimits->punkt=::GetRound();
		if(d_min)
			m_pAccountLimits->m_AccountTimeStart=::TimeToSQL(d_min);
		else
			m_pAccountLimits->m_AccountTimeStart=_T("");
		if(d_max)
			m_pAccountLimits->m_AccountTimeStop=::TimeToSQL(d_max);
		else
			m_pAccountLimits->m_AccountTimeStop=_T("");
		return;
	}
	int min=0,max=0;
	int cou=m_TimesList.GetCount();
	for(int i=0;i<cou;i++){
		if(m_TimesList.GetSel(i)>0){
			min=i;
			break;
		}
	}
	for(i=cou-1;i;i--){
		if(m_TimesList.GetSel(i)>0){
			max=i;
			break;
		}
	}	
	m_pAccountLimits->not_clear=false;
	if(max&&max<(cou-1))
		m_TimesList.GetText(max+1,m_pAccountLimits->m_AccountTimeStart);
	else
		m_pAccountLimits->m_AccountTimeStart=_T("");
	if(min){
		m_TimesList.GetText(min,m_pAccountLimits->m_AccountTimeStop);
		m_pAccountLimits->punkt=(BYTE)m_TimesList.GetItemData(min);
	}else{
		m_pAccountLimits->m_AccountTimeStop=_T("");
		m_pAccountLimits->punkt=::GetRound();
	}
	if((min==1)||(max==1)){
		m_TimesList.GetText(2,m_pAccountLimits->m_AccountTimeStart);
		m_pAccountLimits->m_AccountTimeStop=_T("");
		m_pAccountLimits->punkt=::GetRound();
		m_pAccountLimits->not_clear=true;
	}*/
	CDialog::OnOK();
}
