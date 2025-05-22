// HardwareCom.cpp: implementation of the CHardwareCom class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tv_tracers.h"
#include "HardwareCom.h"

#include <process.h>
#include <string.h>

#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
int ComThread(CHardwareCom *pHC){

//	AfxGetApp()->PostThreadMessage(MW_TCP_BAD_CONNECT,pHC->NumHardware,0);
//	return 0;

	unsigned char buffer[4000];	//V2 грешка от V1 кратък буфер

	typedef enum{
		AT_test,
		ATE0,
		CIFSR_test,
		PIN_test,
		PIN_set,
		CIPMUX,
		CREG,
		CGATT,
		CSQ,
		CSTT,
		CIICR,
		BAD_PIN,
		TO_Open,
		Opened,
	}sim900state;
	sim900state state;
	state = AT_test;

	while(pHC->fl_work){
		if( state < TO_Open ){
			pHC->fl_ready_to_open = false;
			pHC->fl_opened = false;
			pHC->fl_open = false;
			pHC->fl_close = false;
			pHC->fl_to_write = false;
			pHC->fl_write = false;
		}
		switch(state){
		case AT_test:
			{
				Sleep(1000);
				pHC->SimWrite((unsigned char*)"AT\r\n");
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result == 10 ){
					if( memcmp(buffer,"AT\r\n\r\nOK\r\n",result) == 0){
						state = ATE0;
						break;
					}
				}else if(result == 6){
					if( memcmp(buffer,"\r\nOK\r\n",result) == 0){
						state = CIFSR_test;
						break;
					}
				}else if (result == 9) {		//SIM800
					if (memcmp(buffer, "AT\r\r\nOK\r\n", result) == 0) {
						state = ATE0;
						break;
					}
				}	else {
					;//???
				}
				Sleep(1000);
			}
			break;
		case ATE0:
			{
				Sleep(1000);
				pHC->SimWrite((unsigned char*)"ATE0\r\n");
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result == 12 ){
					if( memcmp(buffer,"ATE0\r\n\r\nOK\r\n",result) == 0){
						state = CIFSR_test;
						break;
					}
				}else if( result == 0 ){
					state = AT_test;
				}else if (result == 11) {	//SIM800
					if (memcmp(buffer, "ATE0\r\r\nOK\r\n", result) == 0) {
						state = CIFSR_test;
						break;
					}
				}	else {	//SIM800
					;//???
				}
				break;
			}
		case CIFSR_test:
			{
				Sleep(1000);
				pHC->SimWrite((unsigned char*)"AT+CIFSR\r\n");
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result >10 ){
					state = TO_Open;
					//state = PIN_test;	//SIM800
				}else if( result == 0 ){
					state = AT_test;
				}else{
					state = PIN_test;
				}
				break;
			}
		case PIN_test:
			{
				Sleep(1000);
				pHC->SimWrite((unsigned char*)"AT+CPIN?\r\n");
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result == 24 ){
					if( memcmp(buffer,"\r\n+CPIN: SIM PIN\r\n\r\nOK\r\n",result) == 0){
						state = PIN_set;
						break;
					}
				}else if( result == 22 ){
					if( memcmp(buffer,"\r\n+CPIN: READY\r\n\r\nOK\r\n",result) == 0){
						state = CIPMUX;
						break;
					}
				}else if( result == 0 ){
					state = AT_test;
				}
				break;
			}
		case PIN_set:
			{
				Sleep(3000);
				char text[50];
				sprintf(text,"AT+CPIN=\"%s\"\r\n",PIN);
				pHC->SimWrite((unsigned char*)text);
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result == 6 ){
					if( memcmp(buffer,"\r\nOK\r\n",result) == 0){
						state = PIN_test;
						break;
					}
				}else if( result == 0 ){
					state = AT_test;
				}else
					state = BAD_PIN;
				break;
			}
		case CIPMUX:
			{
				Sleep(1000);
				pHC->SimWrite((unsigned char*)"AT+CIPMUX=1\r\n");
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result > 5 ){
					{
						state = CREG;
						break;
					}
				}else if( result == 0 ){
					state = AT_test;
				}
				break;
			}
		case CREG:
			{
				Sleep(1000);
				pHC->SimWrite((unsigned char*)"AT+CREG?\r\n");
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result == 20 ){
					if( memcmp(buffer,"\r\n+CREG: 0,1\r\n\r\nOK\r\n",result) == 0){
						state = CGATT;
						break;
					}
				}else if( result == 0 ){
					state = AT_test;
				}
				break;
			}
		case CGATT:
			{
				Sleep(1000);
				pHC->SimWrite((unsigned char*)"AT+CGATT?\r\n");
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result == 19 ){
					if( memcmp(buffer,"\r\n+CGATT: 1\r\n\r\nOK\r\n",result) == 0){
						state = CSQ;
						break;
					}
				}else if( result == 0 ){
					state = AT_test;
				}
				break;
			}
		case CSQ:
			{
				Sleep(1000);
				pHC->SimWrite((unsigned char*)"AT+CSQ\r\n");
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result > 10 ){
					{
						state = CSTT;
						break;
					}
				}else if( result == 0 ){
					state = AT_test;
				}
				break;
			}
		case CSTT:
			{
				Sleep(1000);
				char text[50];
				sprintf(text,"AT+CSTT=\"%s\"\r\n",APN);
				pHC->SimWrite((unsigned char*)text);
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result > 5 ){
					{
						state = CIICR;
						break;
					}
				}else if( result == 0 ){
					state = AT_test;
				}
				break;
			}
		case CIICR:
			{
				Sleep(1000);
				pHC->SimWrite((unsigned char*)"AT+CIICR\r\n");
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result > 5 ){
					{
						state = CIFSR_test;
						break;
					}
				}else if( result == 0 ){
					state = AT_test;
				}
				break;
			}
		case TO_Open:
			pHC->fl_ready_to_open = true;
			if( pHC->fl_open ){
				char text[50];

				{
					CString t_text;
					t_text.Format(_T("Open %s"),pHC->m_sTCP_Server);
					pHC->TestLinkMessage(t_text);
				}

				sprintf(text,"AT+CIPSTART=1,\"TCP\",\"%s\",\"%d\"\r\n",pHC->m_sTCP_Server,JENNIC_PORT);
				pHC->SimWrite((unsigned char*)text);
				unsigned int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result == 6 ){
					if( memcmp(buffer,"\r\nOK\r\n",result) == 0){
						pHC->fl_open = false;
						pHC->fl_to_write = false;
						pHC->fl_write = false;
						break;
					}else
						Sleep(10000);
				}else if( result == 31 ){
					if( memcmp(buffer,"\r\nERROR\r\n\r\n1, ALREADY CONNECT\r\n",result) == 0){
						pHC->fl_open = false;
						pHC->fl_to_write = false;
						pHC->fl_write = false;
						pHC->fl_fail = true;
						//close
						pHC->SimWrite((unsigned char*)"AT+CIPCLOSE=1\r\n");
						int result = pHC->SimRead(buffer,sizeof(buffer));
						pHC->recived = 0;
						break;
					}else
						Sleep(10000);
				}else
					Sleep(10000);
			}else {
				int result = pHC->SimRead(buffer,sizeof(buffer));
				if( result ){
					if( memcmp(buffer,"\r\n1, CONNECT OK\r\n",result) == 0){
						pHC->fl_open = false;
						pHC->fl_opened = true;
						pHC->fl_to_write = false;
						pHC->fl_write = false;
						state = Opened;
						pHC->recived = 0;
					}else if( memcmp(buffer,"\r\n1, CLOSE OK\r\n",result) == 0){
						pHC->fl_open = false;
						pHC->fl_opened = false;
						pHC->fl_close = false;
						pHC->fl_to_write = false;
						pHC->fl_write = false;
					}else if( memcmp(buffer,"\r\n1, CONNECT FAIL\r\n",result) == 0){
						pHC->fl_open = false;
						pHC->fl_opened = false;
						pHC->fl_close = false;
						pHC->fl_to_write = false;
						pHC->fl_write = false;
						pHC->fl_fail = true;
					}
				}
			}
			break;
		case Opened:		//само при 1, CONNECT OK
			{
				if( pHC->fl_close && pHC->fl_opened ){ 
					pHC->fl_opened = false;
					pHC->SimWrite((unsigned char*)"AT+CIPCLOSE=1\r\n");
					int result = pHC->SimRead(buffer,sizeof(buffer));
					pHC->recived = 0;
				}else	if( pHC->fl_to_write && !pHC->fl_write ){
					pHC->SimWrite((unsigned char*)"AT+CIPSEND=1\r\n");
					pHC->fl_write = true;
					pHC->recived = 0;
				}else{
					if( pHC->SimRead(buffer + pHC->recived,1) ){
						uint8_t u8Data = buffer[pHC->recived ++];
						
						if (pHC->recived && (pHC->recived == pHC->to_reciv)) {
							buffer[pHC->recived] = 0;
					
							int i;
							for (i = 0;i < (pHC->to_reciv >> 1); i++) {
								unsigned char data;
								if (buffer[i << 1] >= 'A')
									data = buffer[i << 1] - 'A' + 10;
								else
									data = buffer[i << 1] - '0';
								data <<= 4;
								if (buffer[(i << 1)+1] >= 'A')
									data |= (buffer[(i << 1)+1] - 'A' + 10)&0x0F;
								else
									data |= (buffer[(i << 1)+1] - '0')&0x0F;
								pHC->Bufer_Rx[i] = data;
							}
							int len = pHC->Bufer_Rx[0];
							len <<= 8;
							len |= pHC->Bufer_Rx[1];
							if ((len + HEADER_SIZE) == (pHC->to_reciv >> 1)) {
								pHC->RecivedLenght = len + HEADER_SIZE;
							}else{
								CString text;
								text.Format("BAD lenght from client:%d %d",len + HEADER_SIZE, pHC->to_reciv>>1);
								pHC->TestLinkMessage(text,false);
							}
							pHC->to_reciv = 0;
							pHC->recived = 0;
						}
						switch (u8Data) {
						case '>':
							pHC->SimWrite(pHC->SimBufer_Tx);
							break;
						case '+':
							buffer[0] = u8Data;
							pHC->recived = 1;
							break;
						case '\n':
							{
								buffer[pHC->recived] = 0;
								if ( (pHC->recived > 12) && (memcmp(buffer, "+RECEIVE,", 9) == 0 )) {
									int client = atoi((char*)(buffer + 9));
									if( client == 1 )
										pHC->to_reciv = atoi((char*)(buffer + 11));
								}/*else if( (recived > 11) && (memcmp(buffer, "+PDP: DEACT", 11) == 0 )){
									state = AT_test;
									On_off_SIM();
									sim_errors ++;
								}else if( (recived > 13) && (memcmp(buffer, "+CIPSERVER: 0", 13) == 0 )){
									state = AT_test;
									On_off_SIM();
									sim_errors ++;
								}else if( (recived > 13) && (memcmp(buffer, "+CIPSERVER: 1", 13) == 0 )){
									t_min_no_connect = 0;
									fl_test_connect = 0;
								}*/else	if( (pHC->recived >= 13) && (memcmp(buffer,"1, CLOSE OK\r\n",13) == 0 )){
									pHC->fl_open = false;
									pHC->fl_opened = false;
									pHC->fl_close = false;
									pHC->fl_to_write = false;
									pHC->fl_write = false;
									state = TO_Open;
								}else if( (pHC->recived >= 12) && (memcmp(buffer,"1, SEND OK\r\n",12) == 0 )){
									pHC->fl_to_write = false;
									pHC->fl_write = false;
								}
								pHC->recived = 0;
								break;
							}
						}
					}
				}
			}
			break;
		case BAD_PIN:
			break;
		}
	}
	pHC->fl_ended = false;
	return 0;
}

CHardwareCom::CHardwareCom(CString com)
{
	m_sCOM = com;

	CString text;
	text = com.Mid(3);
	::sscanf(text,_T("%d"),&NumHardware);

	m_hFile=Open();

	ConfigCom();

	fl_ended = false;
	if(m_hFile){
		fl_work = fl_ended = true;
		_beginthread((void (__cdecl *)(void *))ComThread,0,this);
	}

	m_iTimeOut = 3000; //23;  //MB_RTU_ADU_MAX_LENGTH * 0.087
	m_iRTU_TimeOut = 50;
}

CHardwareCom::~CHardwareCom()
{
	if(m_hFile){
		if( fl_opened ){
			fl_open = false;
			fl_close = true;
			while(fl_close)
				Sleep(10);
		}
		m_sConnectServer = "";
		fl_work = false;
		while(fl_ended)
			Sleep(10);
		::CloseHandle(m_hFile);
		m_hFile=NULL;
	}
}

void CHardwareCom::ConfigCom()
{
	
	if(m_hFile){
		CString error;
		DCB dcb;
		dcb.DCBlength=sizeof(DCB);
		if (!::GetCommState(m_hFile,&dcb)){
			m_hFile=NULL;
			AfxGetApp()->PostThreadMessage(MW_COM_UNK_ERR,NumHardware,0);
			error.Format(IDS_ERR_HARWARE_UNKNOWN,NumHardware);
			TestLinkMessage(error,false);
		}else{
			dcb.BaudRate=CBR_115200;
			
			dcb.ByteSize = 8;
			dcb.Parity   = NOPARITY;
			dcb.StopBits = ONESTOPBIT;
			dcb.fParity  = false;
			dcb.fRtsControl=RTS_CONTROL_DISABLE;
			dcb.fDtrControl=DTR_CONTROL_ENABLE;
			
			dcb.fDsrSensitivity=false;
			dcb.fAbortOnError=false;
			dcb.fOutxCtsFlow=false;
			dcb.fOutX=false;
			dcb.fInX=false;
			if (!::SetCommState(m_hFile,&dcb))	{
				m_hFile=NULL;
				AfxGetApp()->PostThreadMessage(MW_COM_UNK_ERR,NumHardware,0);
				error.Format(IDS_ERR_HARWARE_UNKNOWN,NumHardware);
				TestLinkMessage(error,false);
			}else{
				COMMTIMEOUTS timeouts;
				{
					timeouts.ReadIntervalTimeout = m_iRTU_TimeOut;
					timeouts.ReadTotalTimeoutConstant = m_iTimeOut;
					timeouts.ReadTotalTimeoutMultiplier = 0;
					timeouts.WriteTotalTimeoutConstant = m_iTimeOut;
					timeouts.WriteTotalTimeoutMultiplier = 1;
				}
				if(!::SetCommTimeouts(m_hFile, &timeouts )){
					m_hFile=NULL;
					AfxGetApp()->PostThreadMessage(MW_COM_UNK_ERR,NumHardware,0);
					error.Format(IDS_ERR_HARWARE_UNKNOWN,NumHardware);
					TestLinkMessage(error,false);
				}
			}
		}
	}
}

HANDLE CHardwareCom::Open()
{
	CString name,error;

	HANDLE mHF;

	name.Format(_T("\\\\.\\COM%d"),NumHardware);
	mHF = ::CreateFile(name, 
						   GENERIC_READ|GENERIC_WRITE, 
						   0, 
						   0, 
						   OPEN_EXISTING, 
						   0,//FILE_FLAG_OVERLAPPED, 
						   0);
	
	// Check if we could open the device
	if (mHF == INVALID_HANDLE_VALUE)
	{
		// Display error
		switch (::GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
			// The specified COM-port does not exist
			AfxGetApp()->PostThreadMessage(MW_COM_NOT_AVAILABE,NumHardware,0);
			error.Format(IDS_ERR_HARWARE_NOT_PRESENT,NumHardware);
			TestLinkMessage(error,false);
			break;

		case ERROR_ACCESS_DENIED:
			// The specified COM-port is in use
			AfxGetApp()->PostThreadMessage(MW_COM_IN_USE,NumHardware,0);
			error.Format(IDS_ERR_HARWARE_IS_BUSY,NumHardware);
			TestLinkMessage(error,false);
			break;

		default:
			// Something else is wrong
			AfxGetApp()->PostThreadMessage(MW_COM_UNK_ERR,NumHardware,0);
			error.Format(IDS_ERR_HARWARE_UNKNOWN,NumHardware);
			TestLinkMessage(error,false);
			break;
		}
		mHF=NULL;
	}
	return mHF;
}

void CHardwareCom::Close()
{
	if(m_hFile){
		if( fl_opened ){
			CHardwareBase::Close();
			fl_open = false;
			fl_close = true;
			while(fl_close)
				Sleep(10);
			m_sConnectServer = "";
		}
	}
}

void CHardwareCom::Flush()
{
	if(!m_hFile)
		return;

	::FlushFileBuffers(m_hFile);
	return;

	COMSTAT comstat;
  DWORD   err = 0;
	while(1){
		ClearCommError(m_hFile,&err,&comstat);
		if(comstat.cbOutQue)
			Sleep(0);
		else
			return;
	}
}

int CHardwareCom::JennicTrxRtx(BYTE *p_DataEnd, int RecivLenght, uint32_t timeout ,BOOL clearReciver)
{
	RecivedLenght = 0;

	uint32_t recive_timeout = SIM_RECIVE_TIMEOUT;
	
	if( timeout > 0 )
		recive_timeout = timeout;

	if(m_hFile == NULL)
		return NO_OPEN;

	if( !fl_ready_to_open )
		return NO_OPEN;

	fl_fail = false;
	if( !fl_opened ){
		DWORD dwStart = ::GetTickCount(); 
		fl_open = true;
		while( (!fl_opened) && (!fl_fail) && ( (dwStart + SIM_OPEN_TIME_OUT) > ::GetTickCount()))
			Sleep(100);
	
	}

	if( !fl_opened ){
		fl_open = false;
		TestLinkMessage(_T("Open Server timeout"),false);
		return NO_OPEN;
	}else{
		m_sConnectServer = m_sTCP_Server;
		int lenght = p_DataEnd - Bufer_Tx;
		int i;
		for( i = 0; i<lenght ; i++){
			unsigned char data,data1;
			data = data1 = Bufer_Tx[i];
			data >>= 4; data &=0x0f;
			data1 &= 0x0f;
			if( data > 9)	data+='A'-10;else	data+= '0';
			if( data1 > 9)	data1+='A'-10;else	data1+= '0';
			SimBufer_Tx[(i<<1)]=data;
			SimBufer_Tx[(i<<1)+1]=data1;
		}
		SimBufer_Tx[(i<<1)]=0x1A;
		SimBufer_Tx[(i<<1)+1]=0;
		
		DWORD dwStart = ::GetTickCount(); 
		fl_to_write = true;
		while( (fl_to_write) && ( (dwStart + SIM_SEND_TIMEOUT) > ::GetTickCount()))
			Sleep(100);
		if(fl_to_write){
			fl_to_write = false;
			fl_write = false;
			TestLinkMessage(_T("send timeout"),false);
			return NO_WRITE;
		}
		if( RecivLenght ){
			DWORD dwStart = ::GetTickCount(); 
			while( (RecivedLenght < RecivLenght) && ( (dwStart + recive_timeout) > ::GetTickCount())) //V2 грешка от V1 (RecivedLenght == 0)
				Sleep(100);
			if(RecivedLenght == 0) {
				TestLinkMessage(_T("reciv timeout"),false);
				return RECIVE_TIMEOUT;
			}
		}
	}
	
	return RecivedLenght;
}

int CHardwareCom::SimWrite(BYTE *str, BOOL clearReciver)
{
	ConfigCom();
	if(clearReciver && m_hFile){
		BYTE *TLVstr=NULL;				//Unreaded chars
		int nrLength=0;
		COMSTAT comstat;
		DWORD   err = 0;
		do{
			ClearCommError(m_hFile,&err,&comstat);
			if(comstat.cbInQue){
				BYTE *p=TLVstr;
				TLVstr=new BYTE[nrLength+comstat.cbInQue];
				::CopyMemory(TLVstr,p,nrLength);
				::ReadFile(m_hFile,TLVstr+nrLength,comstat.cbInQue,&err,NULL);
				nrLength+=comstat.cbInQue;
				if(p)
					delete[] p;
			}
		}while(comstat.cbInQue);
		if((nrLength &&m_pTestLinkView) && (m_Flags&FL_VIEW_DATA) )
			PostMessage(m_pTestLinkView->m_hWnd,MW_NOT_READ_INP_STR,(WPARAM)TLVstr,nrLength);
		else if(TLVstr)
			delete[] TLVstr;
	}
	
	DWORD wLength;
	if(m_hFile){
		int length = strlen((const char *)str);
		::WriteFile(m_hFile,str,length,&wLength,NULL);
		if(m_pTestLinkView  && (m_Flags&FL_VIEW_DATA) ){
			BYTE *p=new BYTE[length];
			::CopyMemory(p,str,length);
			PostMessage(m_pTestLinkView->m_hWnd,MW_OUTPUT_STR,(WPARAM)p,length);
		}
	}
	Flush();
	return wLength;
}

int CHardwareCom::SimRead(BYTE *str, unsigned int bytetoread)
{
	DWORD   butes = 0;
	::ReadFile(m_hFile,str,bytetoread,&butes,NULL);
	if((m_pTestLinkView && butes) && (m_Flags&FL_VIEW_DATA) ){
		BYTE *p=new BYTE[butes];
		::CopyMemory(p,str,butes);
		PostMessage(m_pTestLinkView->m_hWnd,MW_INPUT_STR,(WPARAM)p,butes);
	}
	return butes;
}

