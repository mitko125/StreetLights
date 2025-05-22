// ServerTCP.cpp: implementation of the CServerTCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tv_tracers.h"
#include "ServerTCP.h"
#include "dynamiclib.h"

#include <process.h>
#include <string.h>
#include "JennicModule.h"

#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static CServerTCP *pSTCP;

// макрос для печати количества активных пользователей
#define PRINTNUSERS if (nclients) {	CString text;	text.Format("CServerTCP %d user on-line",nclients);	pSTCP->TestLinkMessage(text); } \
        else {pSTCP->TestLinkMessage("CServerTCP No User on line");}
// глобальная переменная - количество активных пользователей
volatile int nclients = 0;
volatile int last_clients = 0;

SOCKET my_sock[MAX_TCP_IP4_CLIENTS];

static volatile unsigned char thread_ok = 0;
// прототип функции, обслуживающий подключившихся пользователей
int SexToClient(int * client);

int ServerTCP_Thread(void){

	pSTCP->TestLinkMessage("CServerTCP Start ServerTCP_Thread");

	{	//Проверява IP тата на компютъра
		WSADATA WSAData;
		
		// Initialize winsock dll
		if(WSAStartup(MAKEWORD(1, 0), &WSAData))//if (::WSAStartup(MAKEWORD(1, 0), &WSAData))
		{
			// Error handling
			pSTCP->TestLinkMessage("CServerTCP Error WSAStartup",false);
		}
		
		// Get local host name
		char szHostName[128] = "";
		
		if(gethostname(szHostName, sizeof(szHostName))) //if(::gethostname(szHostName, sizeof(szHostName)))
		{
			// Error handling -> call 'WSAGetLastError()'
			pSTCP->TestLinkMessage("CServerTCP Error gethostname",false);
		}
		
		// Get local IP addresses
		struct sockaddr_in SocketAddress;
		struct hostent     *pHost        = 0;
		
		pHost = gethostbyname(szHostName);//::gethostbyname(szHostName);
		if(!pHost)
		{
			// Error handling -> call 'WSAGetLastError()'
			pSTCP->TestLinkMessage("CServerTCP Error gethostbyname",false);
		}
		
		char aszIPAddresses[16]; //
		
		int iCnt;
		for(iCnt = 0; ((pHost->h_addr_list[iCnt]) /* && (iCnt < 10)*/); ++iCnt)
		{
			memcpy(&SocketAddress.sin_addr, pHost->h_addr_list[iCnt], pHost->h_length);
			strcpy(aszIPAddresses, inet_ntoa(SocketAddress.sin_addr));
			CString text;
			text.Format(_T("CServerTCP My IP %d:"),iCnt);
			pSTCP->TestLinkMessage(text);
			text.Format(_T("\"%s\""),aszIPAddresses);
			//pSTCP->TestLinkMessage(text);
			if((pSTCP->m_pTestLinkView ) && (pSTCP->m_Flags&FL_VIEW_DATA) ){
				DWORD   butes = text.GetLength ();
				BYTE *p=new BYTE[butes];
				::CopyMemory(p,text,butes);
				PostMessage(pSTCP->m_pTestLinkView->m_hWnd,MW_INPUT_STR,(WPARAM)p,butes);
			}
		}
		
		// Cleanup
		WSACleanup();
	}





		char buff[1024]; // Буфер для различных нужд

	// Шаг 1 - Инициализация Библиотеки Сокетов
	// т.к. возвращенная функцией информация не используется
	// ей передается указатель на рабочий буфер, преобразуемый к указателю
	// на структуру WSADATA.
	// Такой прием позволяет сэкономить одну переменную, однако, буфер
	// должен быть не менее полкилобайта размером (структура WSADATA
	// занимает 400 байт)
	if (WSAStartup(0x0202, (WSADATA *)&buff[0]))
	{
		// Ошибка!
		CString text;
		text.Format("CServerTCP Error WSAStartup %d", WSAGetLastError());
		pSTCP->TestLinkMessage(text,false);
		
		return -1;
	}

	// Шаг 2 - создание сокета
	SOCKET mysocket;
	// AF_INET - сокет Интернета
	// SOCK_STREAM - потоковый сокет (с установкой соединения)
	// 0 - по умолчанию выбирается TCP протокол
	if ((mysocket = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		// Ошибка!
		CString text;
		text.Format("CServerTCP Error socke %d", WSAGetLastError());
		pSTCP->TestLinkMessage(text,false);

		WSACleanup(); // Деиницилизация библиотеки Winsock
		return -1;
	}

	// Шаг 3 - связывание сокета с локальным адресом
	struct sockaddr_in local_addr;
	local_addr.sin_family = AF_INET;
	local_addr.sin_port = htons(JENNIC_PORT);// 8050);// JENNIC_PORT); // не забываем о сетевом порядке!!!
	local_addr.sin_addr.s_addr = 0; // сервер принимает подключения
									// на все свои IP-адреса

									// вызываем bind для связывания
	if (bind(mysocket, (struct sockaddr *)&local_addr, sizeof(local_addr)))
	{
		// Ошибка
		CString text;
		text.Format("CServerTCP Error bind %d", WSAGetLastError());
		pSTCP->TestLinkMessage(text,false);

		closesocket(mysocket); // закрываем сокет!
		WSACleanup();
		return -1;
	}

	// Шаг 4 - ожидание подключений
	// размер очереди - MAX_TCP_IP4_CLIENTS
	if (listen(mysocket, MAX_TCP_IP4_CLIENTS))
	{
		// Ошибка
		CString text;
		text.Format("CServerTCP Error listen %d", WSAGetLastError());
		pSTCP->TestLinkMessage(text,false);

		closesocket(mysocket);
		WSACleanup();
		return -1;
	}

	pSTCP->TestLinkMessage("CServerTCP ServerTCP_Thread Wait clients");

	// Шаг 5 - извлекаем сообщение из очереди
	SOCKET client_socket; // сокет для клиента
	struct sockaddr_in client_addr; // адрес клиента (заполняется системой)

									// функции accept необходимо передать размер структуры
	int client_addr_size = sizeof(client_addr);

	thread_ok = 1;

	// цикл извлечения запросов на подключение из очереди
	while ((client_socket = accept(mysocket, (struct sockaddr *)&client_addr,&client_addr_size)) && pSTCP->fl_work )
	{
		if( nclients < MAX_TCP_IP4_CLIENTS ){
			my_sock[last_clients = nclients] = client_socket;
			nclients++; // увеличиваем счетчик подключившихся клиентов

					// пытаемся получить имя хоста
			HOSTENT *hst;
			hst = gethostbyaddr((char *)&client_addr.sin_addr.s_addr, 4, AF_INET);

			// вывод сведений о клиенте
			{
				CString text;
				text.Format("CServerTCP Client %d +%s [%s] new connect!",last_clients,
					(hst) ? hst->h_name : "", inet_ntoa(client_addr.sin_addr));
				pSTCP->TestLinkMessage(text);
			} 
			PRINTNUSERS;

			// Вызов нового потока для обслужвания клиента
			// Да, для этого рекомендуется использовать _beginthreadex
			// но, поскольку никаких вызовов функций стандартной Си библиотеки
			// поток не делает, можно обойтись и CreateThread

			_beginthread((void (__cdecl *)(void *))SexToClient,0,(void*)(&last_clients));
		}else{
			// закрываем сокет
			closesocket(client_socket);
			CString text;
			text.Format("CServerTCP Too much TCP clients > %d", MAX_TCP_IP4_CLIENTS);
			pSTCP->TestLinkMessage(text,false);
		}
	}

	for(int i = 0 ; i < MAX_TCP_IP4_CLIENTS ; i++){
		if( my_sock[i] ){
			closesocket(my_sock[i]);
			my_sock[i] = 0;
		}
	}
//	pSTCP->fl_ended = false;
	return 0;
}

// Эта функция создается в отдельном потоке
// и обсуживает очередного подключившегося клиента независимо от остальных
#define SIZE_ipv6_buf 2048
static unsigned char ipv6_buf[SIZE_ipv6_buf];

int SexToClient(int * client)
{
	int num_client = *client;

	// цикл эхо-сервера: прием строки от клиента и возвращение ее клиенту
	int bytes_r = 0;
	while ((bytes_r = recv(my_sock[num_client], (char*)(&ipv6_buf[0]), SIZE_ipv6_buf , 0)) && bytes_r != SOCKET_ERROR) {
		//send(my_sock, &buff[0], bytes_recv, 0);
		ipv6_buf[bytes_r] = 0;

		CString text;
		text.Format(_T("CServerTCP From client TCP/IP:%s"), ipv6_buf);
		//pSTCP->TestLinkMessage(text);
		if((pSTCP->m_pTestLinkView ) && (pSTCP->m_Flags&FL_VIEW_DATA) ){
			DWORD   butes = text.GetLength ();
			BYTE *p=new BYTE[butes];
			::CopyMemory(p,text,butes);
			PostMessage(pSTCP->m_pTestLinkView->m_hWnd,MW_INPUT_STR,(WPARAM)p,butes);
		}
		unsigned char data ,data1;
		bytes_r >>= 1;
		for(int i = 0; i < bytes_r ; i++){
			data1 = ipv6_buf[i<<1];
			if (data1 >= 'A')	data = data1 - 'A' + 10; else data = data1 - '0';
			data <<= 4;
			data1 = ipv6_buf[(i<<1)+1];
			if (data1 >= 'A')	data |= data1 - 'A' + 10; else data |= data1 - '0';
			ipv6_buf[i]= data;
			//text.Format(_T("%d"),data);
		//	pSTCP->TestLinkMessage(text,false);
		}

		switch(ipv6_buf[HEADER_SIZE]){
		case SET_MY_IP_TO_CLIENT:
			if( bytes_r == (HEADER_SIZE + 1 + sizeof(sin_addr))){
				sin_addr my_ip;
				memcpy((unsigned char*)&my_ip,ipv6_buf + HEADER_SIZE + 1,sizeof(sin_addr));
				my_ip.S_un.S_addr = ntohl(my_ip.S_un.S_addr);
				CString text;
				in_addr addres;
				addres.S_un.S_addr = my_ip.S_un.S_addr;
				text.Format(IDS_MY_IP,inet_ntoa(addres));
				AfxMessageBox(text);
				//AfxGetApp()->PostThreadMessage(MW_TCP_BAD_CONNECT,addres.S_un.S_addr,0);
			}else{
				text.Format(_T("Bad lengh=%d in message=%d from client PC server %d"),bytes_r,SET_MY_IP_TO_CLIENT);
				pSTCP->TestLinkMessage(text,false);
			}
			break;
		default:
			text.Format(_T("Error unknow message from client PC server %d"),ipv6_buf[HEADER_SIZE]);
			pSTCP->TestLinkMessage(text,false);
			break;
		}
	}

	// если мы здесь, то произошел выход из цикла по причине
	// возращения функцией recv ошибки - соединение с клиентом разорвано
 	nclients--; // уменьшаем счетчик активных клиентов

	pSTCP->TestLinkMessage("CServerTCP client -disconnect"); 
	PRINTNUSERS;

	// закрываем сокет
	closesocket(my_sock[num_client]);
	my_sock[num_client] = 0;
	return 0;
}

CServerTCP::CServerTCP(  )
{
	pSTCP = this;

	m_pTestLinkView=p_TLView;
	m_Flags = FL_VIEW_DATA;//0;

	fl_work = fl_ended = true;
	_beginthread((void (__cdecl *)(void *))ServerTCP_Thread,0,0);
	_beginthread((void (__cdecl *)(void *))AutoOnOff_Thread,0,0);

}

CServerTCP::~CServerTCP()
{
	fl_work = false;
	for(int i = 0 ; i < MAX_TCP_IP4_CLIENTS ; i++){
		if( my_sock[i] ){
			closesocket(my_sock[i]);
			my_sock[i] = 0;
		}
	}
	while(fl_ended)
		Sleep(10);
}

void CServerTCP::TestLinkMessage(LPCSTR str, BOOL ok)
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

int AutoOnOff_Thread(void){
	int posted = 0;
	CTV_TracersApp * pApp=dynamic_cast<CTV_TracersApp *> (AfxGetApp());
	//ASSERT_VALID(pApp);

//	Sleep(1000);
//	AfxGetApp()->PostThreadMessage(MW_AUTO_UPDATE_TIMER,0,0);
	while( pSTCP->fl_work ){
		Sleep(100);
		if( pApp->m_bAutoOnOff ){
			SYSTEMTIME lpLocalTime;
			GetLocalTime(&lpLocalTime);
			switch( lpLocalTime.wDayOfWeek ){
			case 1:
			case 3:
			case 5:
#define H_OBED 12
#define M_OBED 00
				if( ( lpLocalTime.wHour == H_OBED ) && ( lpLocalTime.wMinute == M_OBED ) && ( posted == 0 ) ){
					posted = 1;
					AfxGetApp()->PostThreadMessage(MW_AUTO_UPDATE_TIMER,0,0);
				}
				if( ( lpLocalTime.wHour != H_OBED ) || ( lpLocalTime.wMinute != M_OBED ) )
					posted = 0;
				break;
			}
		}
	}
	pSTCP->fl_ended = false;
	return 0;
}