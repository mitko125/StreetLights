// HardwareTCP.cpp: implementation of the CHardwareTCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tv_tracers.h"
#include "HardwareTCP.h"

#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MODBUS_ENCODING 0
#define PROGRAM_ENCODING 1

CHardwareTCP::CHardwareTCP()
{
	m_iTimeOut = TCP_RECIVE_TIMEOUT;

	tv_timeout_modbus.tv_sec = m_iTimeOut / 1000L;
	tv_timeout_modbus.tv_usec = (m_iTimeOut % 1000) * 1000;

	m_iTimeOutOpen = TCP_OPEN_TIME_OUT;//5000;//90000;

	tv_timeout_modbus_open.tv_sec = m_iTimeOutOpen / 1000L;
	tv_timeout_modbus_open.tv_usec = (m_iTimeOutOpen % 1000) * 1000;

	m_hSocket = NULL;

	bSendedSokedError = false;

	m_iSleepToNextSendTCP = 5000;

}

CHardwareTCP::~CHardwareTCP()
{
	Close();
}

void CHardwareTCP::Close()
{
	if(m_hSocket){
		CHardwareBase::Close();
		if(closesocket(m_hSocket) == SOCKET_ERROR) {
			TRACE("BAD closesocket socket\n");
			TestLinkMessage(_T("BAD closesocket socket\n"),false);
		}
		m_hSocket=NULL;
		m_sConnectServer = "";
	}
}

BOOL CHardwareTCP::Open()
{

	{
		WSADATA WSAData;
		
		// Initialize winsock dll
		if(WSAStartup(MAKEWORD(1, 0), &WSAData))
		{
			// Error handling
			int a =1;
		}
		
		// Get local host name
		char szHostName[128] = "";
		
		if(gethostname(szHostName, sizeof(szHostName)))
		{
			// Error handling -> call 'WSAGetLastError()'
			int a =1;
		}
		
		// Get local IP addresses
		struct sockaddr_in SocketAddress;
		struct hostent     *pHost        = 0;
		
		pHost = gethostbyname(szHostName);
		if(!pHost)
		{
			// Error handling -> call 'WSAGetLastError()'
			int a =1;
		}
		
		char aszIPAddresses[10][16]; // maximum of ten IP addresses
		
		for(int iCnt = 0; ((pHost->h_addr_list[iCnt]) && (iCnt < 10)); ++iCnt)
		{
			memcpy(&SocketAddress.sin_addr, pHost->h_addr_list[iCnt], pHost->h_length);
			strcpy(aszIPAddresses[iCnt], inet_ntoa(SocketAddress.sin_addr));
			int a =1;
		}
		
		// Cleanup
		WSACleanup();
	}

	{
		CString text;
		text.Format(_T("Open %s"),m_sTCP_Server);
		TestLinkMessage(text);
	}
	ASSERT(m_hSocket == NULL);
	if((m_hSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		TestLinkMessage(_T("BAD create socket\n"),false);
		m_hSocket = NULL;
	}

	sockaddr_in psa;
	psa.sin_family = AF_INET;
	psa.sin_addr.s_addr = inet_addr(m_sTCP_Server);
	psa.sin_port = htons(JENNIC_PORT);
			
	if(m_hSocket){		

		u_long ulEnableNonBlocking = 1;
		if( ioctlsocket (m_hSocket, FIONBIO, &ulEnableNonBlocking) !=0){
			TestLinkMessage(_T("BAD unblocing mode socket\n"),false);
		}

		if(connect(m_hSocket,(sockaddr *) &psa, sizeof(SOCKADDR)) == SOCKET_ERROR) {
			FD_SET fd = {1, m_hSocket};
			switch (select(0, NULL, &fd, NULL, &tv_timeout_modbus_open)) {
			case 0:
				TestLinkMessage(_T("BAD socket connect\n"),false);
				Close();
				break;
			case SOCKET_ERROR:
				TestLinkMessage(_T("BAD Set send timeout in connection\n"),false);
				Close();
				break;
			}
		}

		if(m_hSocket){
			ulEnableNonBlocking = 0;
			if( ioctlsocket (m_hSocket, FIONBIO, &ulEnableNonBlocking) !=0){
				TestLinkMessage(_T("BAD blocing mode socket\n"),false);
			}
		}
	}
	if( m_hSocket )
		bSendedSokedError = false;
	else{
		if( !bSendedSokedError ){
			bSendedSokedError = true;
			AfxGetApp()->PostThreadMessage(MW_TCP_BAD_CONNECT,psa.sin_addr.s_addr,0);
		}
		return 0;
	}
	return m_hSocket;
}

int CHardwareTCP::MyWrite(BYTE *str, DWORD length)
{
	DWORD nBytesSent = 0;
	int nBytesThisTime;
	const char* pch1 = (char *)str;
	do {
		if((nBytesThisTime = send(m_hSocket, pch1, length - nBytesSent, 0)) == SOCKET_ERROR) {
			TestLinkMessage(_T("Send error"),false);
			Close();
			return -1;
		}
		nBytesSent += nBytesThisTime;
		pch1 += nBytesThisTime;
	} while(nBytesSent < length);
	//flush(m_hSocket)
//	::FlushFileBuffers(m_hSocket);
	return nBytesSent;
}

int CHardwareTCP::JennicTrxRtx(BYTE *p_DataEnd, int RecivLenght, uint32_t timeout ,BOOL clearReciver)
{
	BYTE dump_buf[BUFERS_LENGHT*2];
	int result = NO_OPEN;
	int l_MBAP_recived = 0;
	int l_body_recived = 0;

	TIMEVAL recive_timeout = tv_timeout_modbus;
	if( timeout > 0 ){
		recive_timeout.tv_sec = timeout / 1000L;
		recive_timeout.tv_usec = (timeout % 1000) * 1000;
	}

	if( m_hSocket == NULL )
		Open();
	if( m_hSocket != NULL){
		FD_SET fd = {1, m_hSocket};
		if(select(0, NULL, &fd, NULL, &tv_timeout_modbus) == 0) {
			TestLinkMessage(_T("send timeout"),false);
			Close();
			return NO_SOCKET;
		}
	}
	if( m_hSocket != NULL )	{
		result = UNCNOW_ERROR;
		if(clearReciver){
			BYTE *TLVstr=NULL;				//Unreaded chars
			int nrLength=0;
			u_long ulByteToRead = 0;
			do{
				if( ioctlsocket (m_hSocket, FIONREAD, &ulByteToRead) !=0){
					TestLinkMessage(_T("BAD test unread in socket\n"),false);
					ulByteToRead = 0;
				}else if(ulByteToRead ){
					BYTE *p=TLVstr;
					TLVstr=new BYTE[nrLength+ulByteToRead];
					::CopyMemory(TLVstr,p,nrLength);
					if( recv(m_hSocket,(char *) TLVstr+nrLength, ulByteToRead, 0) == SOCKET_ERROR)
						TestLinkMessage(_T("BAD read unread in socket\n"),false);
					nrLength+=ulByteToRead;
					if(p)
						delete[] p;
				}
			}while(ulByteToRead);
			if( (nrLength &&m_pTestLinkView) && (m_Flags&FL_VIEW_DATA))
				PostMessage(m_pTestLinkView->m_hWnd,MW_NOT_READ_INP_STR,(WPARAM)TLVstr,nrLength);
			else if(TLVstr)
				delete[] TLVstr;

		}

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
	
		int sendet;
		if( (sendet = MyWrite(SimBufer_Tx,lenght<<1)) < 0)
			return NO_WRITE;

		if((m_pTestLinkView&&(sendet>0)) && (m_Flags&FL_VIEW_DATA)){
			BYTE *p=new BYTE[sendet];
			::CopyMemory(p,SimBufer_Tx,sendet);
			PostMessage(m_pTestLinkView->m_hWnd,MW_OUTPUT_STR,(WPARAM)p,sendet);
		}
		
		//Sleep(5000);

		result = 0;

		if( RecivLenght ){
			FD_SET fd = {1, m_hSocket};
			if(select(0, &fd, NULL, NULL, &recive_timeout) == 0) {
				TestLinkMessage(_T("reciv timeout"),false);
				Close();
				return RECIVE_TIMEOUT;
			}
			
			if( (l_MBAP_recived = recv(m_hSocket,(char *) Bufer_Rx, HEADER_SIZE<<1, 0)) == SOCKET_ERROR) {
				l_MBAP_recived = 0;
				TestLinkMessage(_T("HEADER reciv error"),false);
				Close();
			}else if(l_MBAP_recived == HEADER_SIZE<<1){
				memcpy(dump_buf,Bufer_Rx,HEADER_SIZE<<1);
				l_MBAP_recived >>=1;
				unsigned char data ,data1;
				for(int i = 0; i < HEADER_SIZE ; i++){
					data1 = Bufer_Rx[i<<1];
					if (data1 >= 'A')	data = data1 - 'A' + 10; else data = data1 - '0';
					data <<= 4;
					data1 = Bufer_Rx[(i<<1)+1];
					if (data1 >= 'A')	data |= data1 - 'A' + 10; else data |= data1 - '0';
					Bufer_Rx[i]= data;
				}
				int bytes = (Bufer_Rx[0]<<8) | (Bufer_Rx[1]);
				bytes <<=1;

				if( bytes > (BUFERS_LENGHT - HEADER_SIZE) ){
					TestLinkMessage(_T("Wery long body"),false);
					Close();
				}else{
					FD_SET fd = {1, m_hSocket};
					if(select(0, &fd, NULL, NULL, &recive_timeout) == 0) {
						TestLinkMessage(_T("reciv timeout"),false);
						Close();
					}else{
						if( (l_body_recived = recv(m_hSocket,(char *) (Bufer_Rx + HEADER_SIZE), bytes, 0)) == SOCKET_ERROR) {
							l_body_recived = 0;
							TestLinkMessage(_T("BAD Reciv"),false);
							Close();
						}else if(l_body_recived != bytes){
							TestLinkMessage(_T("BAD BODY Reciv"),false);
							Close();
						}else{
							unsigned char data ,data1;
							memcpy(dump_buf+(HEADER_SIZE<<1),Bufer_Rx+HEADER_SIZE,l_body_recived);
							l_body_recived>>=1;
							for(int i = 0; i < l_body_recived ; i++){
								data1 = Bufer_Rx[HEADER_SIZE+(i<<1)];
								if (data1 >= 'A')	data = data1 - 'A' + 10; else data = data1 - '0';
								data <<= 4;
								data1 = Bufer_Rx[HEADER_SIZE+(i<<1)+1];
								if (data1 >= 'A')	data |= data1 - 'A' + 10; else data |= data1 - '0';
								Bufer_Rx[HEADER_SIZE+i]= data;
							}
							result = HEADER_SIZE+l_body_recived;
						}
					}
				}
			}else {
				TestLinkMessage(_T("BAD lenght MBAP HEADER Reciv"),false);
				Close();
			}
		}
	}

	if(m_pTestLinkView && (l_body_recived + l_MBAP_recived) && (m_Flags&FL_VIEW_DATA) ){
		BYTE *p=new BYTE[(l_body_recived + l_MBAP_recived)*2];
		::CopyMemory(p,dump_buf,(l_body_recived + l_MBAP_recived)*2);
		PostMessage(m_pTestLinkView->m_hWnd,MW_INPUT_STR,(WPARAM)p,(l_body_recived + l_MBAP_recived)*2);
	}

	return result;
}



