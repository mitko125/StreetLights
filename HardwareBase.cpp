// HardwareBase.cpp : implementation file
//

#include "stdafx.h"
#include "tv_tracers.h"
#include "HardwareBase.h"
#include "dynamiclib.h"

#include "Message.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHardwareBase


CHardwareBase::CHardwareBase()
{
	m_sTCP_Server = _T("");
	m_sConnectServer = _T("");
	m_pTestLinkView=p_TLView;

	m_Flags = FL_VIEW_DATA;//0;

	m_iTimeOut = 0;
	m_iRTU_TimeOut = 0;

	m_iSleepToNextSendTCP = 0;
}

CHardwareBase::~CHardwareBase()
{
	Close();
}

/////////////////////////////////////////////////////////////////////////////
// CHardwareBase message handlers

void CHardwareBase::Close()
{	
	if( ! m_sConnectServer.IsEmpty() ){
		CString text;
		text.Format(_T("Close %s"),m_sConnectServer);
		TestLinkMessage(text);
	}
}

void CHardwareBase::TestLinkMessage(LPCSTR str, BOOL ok)
{
	int length;
	if(m_pTestLinkView&&(length=strlen(str))){
		BYTE *p=new BYTE[length];
		::CopyMemory(p,str,length);
		if(ok)
			PostMessage(m_pTestLinkView->m_hWnd,MW_PROTOCOL_STR_OK,(WPARAM)p,length);
		else
			PostMessage(m_pTestLinkView->m_hWnd,MW_PROTOCOL_STR_ERR,(WPARAM)p,length);
	}
}

int CHardwareBase::MakeJennicCommand(BYTE * p_B , int RecivLenght, uint32_t timeout)
{
	DWORD dwStart = ::GetTickCount(); 
	int lenght = JennicTrxRtx( p_B , RecivLenght , timeout );
	DWORD dwEnd = ::GetTickCount(); 
	if( ( lenght < 0) || ( lenght < RecivLenght ) ){
		if( m_pTestLinkView ){
			char str[40];
			::sprintf(str,_T("%d Timeout %d"),*(Bufer_Tx + HEADER_SIZE),dwEnd - dwStart);
			TestLinkMessage(str,false);
		}
		return lenght;
	}else{
		if(lenght >0 ){
			if( m_pTestLinkView ){
				char str[40];
				::sprintf(str,_T("%d Recive lenght %d, time %d"),*(Bufer_Tx + HEADER_SIZE),lenght,dwEnd - dwStart);
				TestLinkMessage(str);
			}
			return lenght;
		}else	{
			if( m_pTestLinkView ){
				char str[40];
				::sprintf(str,_T("%d Send lenght %d, time %d"),*(Bufer_Tx + HEADER_SIZE),p_B-Bufer_Tx,dwEnd - dwStart);
				TestLinkMessage(str);
			}
		}
	}
	return 0;
}

int CHardwareBase::SendReciv(CString ip_address,BYTE *p_data, int SendLenght, int RecivLenght, uint32_t timeout,BYTE version)
{
	if( m_sTCP_Server != ip_address ){
		Close();
		m_sTCP_Server = ip_address;
	}

	Bufer_Tx[0]=HIBYTE(SendLenght);
	Bufer_Tx[1]=LOBYTE(SendLenght);
	Bufer_Tx[2]=version;

	BYTE * p_B = Bufer_Tx + HEADER_SIZE;
	while( SendLenght ){
		*p_B++ = *p_data++;
		SendLenght --;
	}

	int recived = MakeJennicCommand(p_B , RecivLenght, timeout );

	return recived;
}

