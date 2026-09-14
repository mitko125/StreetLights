// JennicModule.cpp: implementation of the CJennicModule class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "tv_tracers.h"
#include "JennicModule.h"
#include "DynamicLib.h"
#include "HardwareTCP.h"

#include "..\Free_Util_Dll\Util_Dll.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CJennicModule::CJennicModule()
{

}

CJennicModule::~CJennicModule()
{

}

int CJennicModule::BroadcastLight(CString ip_address, unsigned char light)
{

	return GlobalSetUint8ByModuleID(ip_address,0xFFFFFE04, 0x02, light);
}

static struct in6_addr local_address =		{0xFE,0x80,0x00,0x00,0x00,0x00,0x00,0x00 ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
static struct in6_addr all_device_group =	{0xFF,0x15,0x00,0x00,0x00,0x00,0x00,0x00 ,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0x0F};
static struct in6_addr all_bulbs_group =	{0xFF,0x15,0x00,0x00,0x00,0x00,0x00,0x00 ,0x00,0x00,0x00,0x00,0x00,0x00,0xFE,0x04};
//static struct in6_addr hall_group =				{0xFF,0x15,0x00,0x00,0x00,0x00,0x00,0x00 ,0x00,0x00,0x00,0x00,0x00,0x00,0xA0,0x0A};
//static struct in6_addr lounge_group =			{0xFF,0x15,0x00,0x00,0x00,0x00,0x00,0x00 ,0x00,0x00,0x00,0x00,0x00,0x00,0xB0,0x0B};

static struct in6_addr dest={0xfd,0x04,0x0b,0xd3,0x80,0xe8,0x00,0x01 ,0x02,0x15,0x8D,0x00,0x00,0x52,0x65,0x11};	//lampa
static struct in6_addr sors={0xFD,0x04,0x0B,0xD3,0x80,0xE8,0xFF,0xFF ,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01};
static struct in6_addr usb_={0xfd,0x04,0x0b,0xd3,0x80,0xe8,0x00,0x01 ,0x02,0x15,0x8D,0x00,0x00,0x36,0x1C,0xF2};	//lampa
	//,dest;

int CJennicModule::GlobalSetUint8ByModuleID(CString ip_address,uint32_t ModuleID, uint8_t VariableIndex, uint8_t data)
{
	uint8_t buffer[20];

	buffer[0] = VERSION;
	buffer[1] = Set_by_ID_request;
	buffer[2] = rand() & 0x7F;

	buffer[3] = (ModuleID >> 24) & 0xFF;
	buffer[4] = (ModuleID >> 16) & 0xFF;
	buffer[5] = (ModuleID >> 8) & 0xFF;
	buffer[6] = (ModuleID ) & 0xFF;

	buffer[7] = VariableIndex;

	buffer[8] = 0x00;
	buffer[9] = UI8;
	buffer[10] = data;

	return eJennicModuleSendMessageIPv6(ip_address,&local_address,&all_device_group,11,buffer);



	buffer[1] = Get_by_ID_request;

	//Groups blob table
	ModuleID = 0xFFFFFF02;	
	VariableIndex = 0x00;

	buffer[3] = (ModuleID >> 24) & 0xFF;
	buffer[4] = (ModuleID >> 16) & 0xFF;
	buffer[5] = (ModuleID >> 8) & 0xFF;
	buffer[6] = (ModuleID ) & 0xFF;

	buffer[7] = VariableIndex;

	buffer[8] = 0;
	buffer[9] = 0;

	buffer[10] =0xFF;
	//return eJennicModuleSendMessageIPv6(ip_address,&sors,&dest,11,buffer);




	
	buffer[1] = Get_by_ID_request;

	//JenNet Module NetworkTable blob table
	ModuleID = 0xFFFFFF01;	
	VariableIndex = 0x04;

	buffer[3] = (ModuleID >> 24) & 0xFF;
	buffer[4] = (ModuleID >> 16) & 0xFF;
	buffer[5] = (ModuleID >> 8) & 0xFF;
	buffer[6] = (ModuleID ) & 0xFF;

	buffer[7] = VariableIndex;

	buffer[8] = 0;
	buffer[9] = 0;

	buffer[10] =0xFF;
	return eJennicModuleSendMessageIPv6(ip_address,&sors,&usb_,11,buffer);
}

int CJennicModule::eJennicModuleSendMessageIPv6(CString ip_address,in6_addr *source_addr, in6_addr *dest_addr, uint32_t u32Length, uint8_t *pu8Data, uint32_t timeout)
{
	uint8_t buffer[500];
	uint16_t checksum;

	buffer[0] = 0x60;
	buffer[1] = 0;
	buffer[2] = 0;
	buffer[3] = 0;

	buffer[4] = ((u32Length + 8) >> 8) & 0xFF;
	buffer[5] = (u32Length + 8) & 0xFF;
	buffer[6] = 0x11;
	buffer[7] = 0x40;

	memcpy(buffer + 8, source_addr, sizeof(struct in6_addr));
	memcpy(buffer + 24, dest_addr, sizeof(struct in6_addr));

	buffer[40] = (SOURCE_PORT >> 8 ) & 0xFF;
	buffer[41] = SOURCE_PORT & 0xFF;
	buffer[42] = (JENNIC_PORT >> 8) & 0xFF;
	buffer[43] = JENNIC_PORT & 0xFF;

	buffer[44] = ((u32Length+8) >> 8) & 0xFF;
	buffer[45] = (u32Length+8) & 0xFF;
	buffer[46] = 0;
	buffer[47] = 0;

	memcpy(buffer + 48, pu8Data, u32Length);

	u32Length += 40 + 8;
	checksum = CalculateChecsum(u32Length, pu8Data);

	//if (checksum == 0)
	//	checksum = 0xffff;
	buffer[46] = checksum >> 8;
	buffer[47] = checksum & 0xff;


	if( p_hardware != NULL ){
		uint8_t test[500];
		test[0] = IPv6_PACKET;

		memcpy(test+1,buffer,u32Length);
		return p_hardware->SendReciv(ip_address,test,1+u32Length,4, timeout);
	//	p_hardware->Close();
	}

	return 0;
}

uint16_t CJennicModule::CalculateChecsum(uint32_t u32Length, uint8_t *pu8Data)
{
	uint32_t checksum, lenght, data;
	uint8_t *p;

	if (u32Length & 1) {
		pu8Data[u32Length] = 0;
		u32Length++;
	}


	checksum = *(pu8Data + 46) << 8 | *(pu8Data + 47);
	lenght = *(pu8Data + 4) << 8 | *(pu8Data + 5);
	data = *(pu8Data + 6);

	checksum = lenght + data;
	for (p = pu8Data + 8; p < (pu8Data + u32Length); p += 2) {
		data = (*p) << 8 | *(p + 1);
		checksum += data;
		while (checksum >> 16)
			checksum = (checksum & 0xFFFF) + (checksum >> 16);
	}
	checksum = (uint16_t)~checksum;
	return checksum;
}

int CJennicModule::OnOffTime(CString ip_address,int OnHour,int OnMinute,int OffHour, int OffMinute)
{
	if( p_hardware != NULL ){
		BYTE test[200];
		tsTimerHourMinute tOn,tOff;

		test[0] =	COMMAND_TIME_ON_OFF;
		tOn.u8Hour = INT_BCD(OnHour);
		tOn.u8Minute = INT_BCD(OnMinute);
		tOff.u8Hour = INT_BCD(OffHour);
		tOff.u8Minute = INT_BCD(OffMinute);

		memcpy( test+1,&tOn,sizeof(tsTimerHourMinute));
		memcpy( test+1+sizeof(tsTimerHourMinute),&tOff,sizeof(tsTimerHourMinute));
		return p_hardware->SendReciv(ip_address,test,1+2*sizeof(tsTimerHourMinute),0);
	}
	return -1;
}

int CJennicModule::SimErrors(CString ip_address)
{
	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	GET_STATUS_ROUTER;
		return p_hardware->SendReciv(ip_address,test,1,5 + HEADER_SIZE );
	}
	return -1;
}

int CJennicModule::OnRelay(CString ip_address)
{
/*	// Тест на SET_MY_IP_TO_CLIENT
	int result = -1;

	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	SET_MY_IP_TO_CLIENT;
		
		result = p_hardware->SendReciv(ip_address,test,1,HEADER_SIZE + 1);

		if( result == (HEADER_SIZE + 1) ){
			if( p_hardware->Bufer_Rx[HEADER_SIZE] != 	ACK )
				result = -1;
		}else
			result = -1;
	}
	return result;*/
	// Оригинална COMMAND_ON
	int result = -1;

	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	COMMAND_ON;
		
		result = p_hardware->SendReciv(ip_address,test,1,HEADER_SIZE + 1);

		if( result == (HEADER_SIZE + 1) ){
			if( p_hardware->Bufer_Rx[HEADER_SIZE] != 	ACK )
				result = -1;
		}else
			result = -1;
	}
	return result;


	/*	//Тест на GET_MY_IP_FROM_SERVER
	int result = -1;

	if( p_hardware != NULL ){
		BYTE test[200];
		test[0] = GET_MY_IP_FROM_SERVER;
			
		result = p_hardware->SendReciv(ip_address,test,1,HEADER_SIZE + 1 + sizeof(sin_addr));

		if( result == (HEADER_SIZE + 1 + sizeof(sin_addr)) ){
			sin_addr my_ip;
			memcpy((unsigned char*)&my_ip,p_hardware->Bufer_Rx + HEADER_SIZE + 1,sizeof(sin_addr));
			my_ip.S_un.S_addr = ntohl(my_ip.S_un.S_addr);
			CString text;
			in_addr addres;
			addres.S_un.S_addr = my_ip.S_un.S_addr;
			text.Format(IDS_MY_IP,inet_ntoa(addres));
			AfxMessageBox(text);
			int a = 5;
		}else
			result = -1;
	}
	return result;*/


	/*	//Тест на SET_WORK_HOURS
		test[0] = SET_WORK_HOURS;
		tsSetWorkHours * psSetWorkHours = (tsSetWorkHours*)(test+1);
		uint8_t mac[]={0x00,0x15,0x8D,0x00,0x00,0x83,0xCD,0x1B};
		memcpy(psSetWorkHours->sMAC_Address.MAC,mac,sizeof(tsMAC_Address));
		psSetWorkHours->u32WorkHours = htonl(0x12345678);
		return p_hardware->SendReciv(ip_address,test,1+sizeof(tsSetWorkHours),0);*/
}

int CJennicModule::OffRelay(CString ip_address)
{
	int result = -1;
	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	COMMAND_OFF;
		result = p_hardware->SendReciv(ip_address,test,1,HEADER_SIZE + 1);

		if( result == (HEADER_SIZE + 1) ){
			if( p_hardware->Bufer_Rx[HEADER_SIZE] != 	ACK )
				result = -1;
		}else
			result = -1;
	}
	return result;
}

int CJennicModule::SetWPAN(UINT32 id_hardware)
{
	int result = -1;
	if( p_hardware != NULL ){
		BYTE test[200];
		
		_RecordsetPtr m_pRs=NULL;
		
		CString com;
		com.Format(_T("SELECT number_hardware,channel,pan_id,sequrity_key,radius_off,resetGPRS_hh,resetGPRS_mm,enable_energy_meter FROM hardwares WHERE id_hardware=%u"),id_hardware);
		try{
			TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
			m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
			
			if(!m_pRs->EndOfFile){		
				_variant_t index;
				index.vt = VT_I2;
				index.iVal = 0;
				
				_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
				CString ip_address = ::GetStringFromVariant(data);
				
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				int channel = ::GetLongFromVariant(data);
				
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				int pan_id = ::GetLongFromVariant(data);
				
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value;
				CString sequrity = ::GetStringFromVariant(data);
				::CStringSize(sequrity,16);
				
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				int radius_off = ::GetLongFromVariant(data);

				tsConfigBorderRuter sModuleSetConfig;

				sModuleSetConfig.sModuleConfigV11.u8Region = CONFIG_DEFAULT_REGION;
				sModuleSetConfig.sModuleConfigV11.u8Channel = channel;
				sModuleSetConfig.sModuleConfigV11.u16PanID = htons(pan_id);
				sModuleSetConfig.sModuleConfigV11.u32NetworkID = htonl(CONFIG_DEFAULT_NETWORK_ID);
				sModuleSetConfig.sModuleConfigV11.u64NetworkPrefixMSB = htonl(CONFIG_DEFAULT_PREFIX_MSB);
				sModuleSetConfig.sModuleConfigV11.u64NetworkPrefixLSB = htonl(CONFIG_DEFAULT_PREFIX_LSB | id_hardware);
				
				memset(&sModuleSetConfig.sSecurityConfig.uAuthSchemeData.sRadiusPAP.sAuthServerIP, 0xFF, sizeof(struct in6_addr));
				sModuleSetConfig.sSecurityConfig.uAuthSchemeData.sRadiusPAP.sAuthServerIP.u.Byte[0] = 0xfd;
				sModuleSetConfig.sSecurityConfig.uAuthSchemeData.sRadiusPAP.sAuthServerIP.u.Byte[1] = 0x04;
				sModuleSetConfig.sSecurityConfig.uAuthSchemeData.sRadiusPAP.sAuthServerIP.u.Byte[2] = 0x0b;
				sModuleSetConfig.sSecurityConfig.uAuthSchemeData.sRadiusPAP.sAuthServerIP.u.Byte[3] = 0xd3;
				sModuleSetConfig.sSecurityConfig.uAuthSchemeData.sRadiusPAP.sAuthServerIP.u.Byte[4] = 0x80;
				sModuleSetConfig.sSecurityConfig.uAuthSchemeData.sRadiusPAP.sAuthServerIP.u.Byte[5] = 0xe8;
				
				sModuleSetConfig.sSecurityConfig.eAuthScheme = (teAuthScheme)htonl(E_AUTH_SCHEME_RADIUS_PAP);
				memset(&sModuleSetConfig.sSecurityConfig.sKey, 0, sizeof(struct in6_addr));
				for(int i = 0 ; i < sequrity.GetLength() ; i ++ ){
					sModuleSetConfig.sSecurityConfig.sKey.u.Byte[i] = sequrity.GetAt(i);
				}
				
				sModuleSetConfig.u8JenNetProfile = CONFIG_DEFAULT_PROFILE;
				sModuleSetConfig.iAntennaDiversity = 0;
				sModuleSetConfig.eRadioFrontEnd = E_FRONTEND_STANDARD_POWER;
				sModuleSetConfig.u8RadiusOff = radius_off;

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				sModuleSetConfig.u8INT_resetGPRShours = (uint8_t)::GetLongFromVariant(data);
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				sModuleSetConfig.u8INT_resetGPRSminuts = (uint8_t)::GetLongFromVariant(data);
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				sModuleSetConfig.u8EnableEnergyMeter = (uint8_t)::GetLongFromVariant(data);
				
				
				test[0] =	COMMAND_SET_HOST_DATA;
				memcpy(test+1,(BYTE*)&sModuleSetConfig,sizeof(sModuleSetConfig));

				result = p_hardware->SendReciv(ip_address,test,1+sizeof(tsConfigBorderRuter),HEADER_SIZE + 1);

				if( result == (HEADER_SIZE + 1) ){
					if( p_hardware->Bufer_Rx[HEADER_SIZE] != 	ACK )
						result = -1;
				}else
					result = -1;
			}		
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}	
		if (m_pRs)
			if (m_pRs->State == adStateOpen)
				m_pRs->Close();
	}
	return result;
}

int CJennicModule::GetWPAN(CString ip_address)
{
	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	COMMAND_GET_HOST_DATA;
		return p_hardware->SendReciv(ip_address,test,1, HEADER_SIZE + 1 + 58);//// 58 old version sizeof(tsConfigBorderRuter));
	}
	return -1;
}

int CJennicModule::GetMAC(CString ip_address)
{
	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	COMMAND_MAC_ADDRESS;
		return p_hardware->SendReciv(ip_address,test,1, HEADER_SIZE + 1 + 8);
	}
	return -1;
}

int CJennicModule::SetTimers(CString ip_address, UINT32 id_timer)
{
	int result = -1;
	if( p_hardware != NULL ){
		BYTE test[500];
		
		tsTimers sTimers;
		memset(&sTimers,0,sizeof(sTimers));

		COleDateTime time = COleDateTime::GetCurrentTime();
		
		sTimers.sDateTime.date_time[5] = INT_BCD( time.GetYear() % 100 );
		sTimers.sDateTime.date_time[4] = INT_BCD( time.GetMonth() );
		sTimers.sDateTime.date_time[3] = INT_BCD( time.GetDay() );
		sTimers.sDateTime.date_time[2] = INT_BCD( time.GetHour() );
		sTimers.sDateTime.date_time[1] = INT_BCD( time.GetMinute() );
		sTimers.sDateTime.date_time[0] = INT_BCD( 0 );


		_RecordsetPtr m_pRs=NULL;
		
		CString com;
		com.Format(_T("SELECT * FROM timers WHERE id_timer=%u"),id_timer);
		try{
			TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
			m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
			
			if(!m_pRs->EndOfFile){		
				_variant_t index;
				index.vt = VT_I2;
				index.iVal = 0;
				_variant_t data;
				
				index.iVal = 0; data=m_pRs->Fields->GetItem(&index)->Value;
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
								
				unsigned int ui;

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				ui = ::GetLongFromVariant(data);
				sTimers.sTimerOn1.u8Hour = INT_BCD( ui/60 );
				sTimers.sTimerOn1.u8Minute = INT_BCD( ui%60 );

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				ui = ::GetLongFromVariant(data);
				sTimers.sTimerOff1.u8Hour = INT_BCD( ui/60 );
				sTimers.sTimerOff1.u8Minute = INT_BCD( ui%60 );

				for( int j = 0 ; j < MAX_GROUP_DATA_BASE ; j++){
					index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
					ui = ::GetLongFromVariant(data);
					sTimers.sGroupTimer[j].sTimerOn.u8Hour = INT_BCD( ui/60 );
					sTimers.sGroupTimer[j].sTimerOn.u8Minute = INT_BCD( ui%60 );

					index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
					ui = ::GetLongFromVariant(data);
					sTimers.sGroupTimer[j].sTimerOff.u8Hour = INT_BCD( ui/60 );
					sTimers.sGroupTimer[j].sTimerOff.u8Minute = INT_BCD( ui%60 );

					index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
					sTimers.sGroupTimer[j].u8Lights = (unsigned char)::GetLongFromVariant(data);
				}

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				ui = ::GetLongFromVariant(data);
				sTimers.sTimerOn2.u8Hour = INT_BCD( ui/60 );
				sTimers.sTimerOn2.u8Minute = INT_BCD( ui%60 );

				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				ui = ::GetLongFromVariant(data);
				sTimers.sTimerOff2.u8Hour = INT_BCD( ui/60 );
				sTimers.sTimerOff2.u8Minute = INT_BCD( ui%60 );
			}	

			test[0] =	COMMAND_SET_TIMERS;
			memcpy(test+1,(BYTE*)&sTimers,sizeof(sTimers));

			result = p_hardware->SendReciv(ip_address,test,1+sizeof(sTimers), HEADER_SIZE + 1 + 6);
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}	
		if (m_pRs)
			if (m_pRs->State == adStateOpen)
				m_pRs->Close();
	}
	return result;
}

int CJennicModule::GetTimers(CString ip_address)
{
	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	COMMAND_GET_TIMERS;
		return p_hardware->SendReciv(ip_address,test,1, HEADER_SIZE + 1 + sizeof(tsTimers) -(4 /*V2 понеже V1 беше по-къс*/));
	}
	return -1;
}

int CJennicModule::GetReject(CString ip_address)
{
	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	COMMAND_GET_REJECT;
		return p_hardware->SendReciv(ip_address,test,1, HEADER_SIZE + 1 + sizeof(tsMAC_Reject));
	}
	return -1;
}


uint8_t CJennicModule::hexa_to_byte(uint8_t *u8Data)
{
	uint8_t data = 0;
	if (*u8Data <= '9')
		data = *u8Data - '0';
	else
		data = *u8Data + 10 - 'A';
	data <<= 4;
	u8Data++;
	if (*u8Data <= '9')
		data |= *u8Data - '0';
	else
		data |= *u8Data + 10 - 'A';
	return data;
}

#define SEND_ACK

int CJennicModule::SetLampsMAC(CString ip_address,UINT32 id_hardware,int *CouLamps)
{
	int result = -1;

	uint16_t u16FirstTableEntry = 0;
	uint16_t u16EntryCount = 0;

	uint16_t ui16AllMAC = 0;

	uint8_t myStrMAC[20];
	CString MAC;

	if( p_hardware != NULL ){
		BYTE test[500];

		test[0] = SEND_LAMPS_MAC_TABLE;
		
		tsSendTable * psSendLamsMAC = (tsSendTable*)(test+1);
		tsMAC_Address * psMAC_Address = (tsMAC_Address*)(psSendLamsMAC+1);

		_RecordsetPtr m_pRs=NULL;
		
		CString com;
		com.Format(_T("SELECT MAC_address FROM lamps WHERE id_hardware=%u ORDER BY id_lamp"),id_hardware);
		try{
			TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
			m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
			
			while(!m_pRs->EndOfFile){		
				_variant_t index;
				index.vt = VT_I2;
				index.iVal = 0;
				
				_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
				MAC = ::GetStringFromVariant(data);
			
				if( ++ui16AllMAC > ROUTE_TABLE_ENTRIES)
					break;

				strcpy((char*)myStrMAC, (LPCTSTR)MAC);

				psMAC_Address->MAC[0]=hexa_to_byte(myStrMAC+0);
				psMAC_Address->MAC[1]=hexa_to_byte(myStrMAC+2);
				psMAC_Address->MAC[2]=hexa_to_byte(myStrMAC+4);
				psMAC_Address->MAC[3]=hexa_to_byte(myStrMAC+6);
				psMAC_Address->MAC[4]=hexa_to_byte(myStrMAC+8);
				psMAC_Address->MAC[5]=hexa_to_byte(myStrMAC+10);
				psMAC_Address->MAC[6]=hexa_to_byte(myStrMAC+12);
				psMAC_Address->MAC[7]=hexa_to_byte(myStrMAC+14);

				psMAC_Address ++;
				if( ++ u16EntryCount == MAX_SEND_MAC ){
					psSendLamsMAC->u16FirstTableEntry = htons(u16FirstTableEntry);
					psSendLamsMAC->u16EntryCount = htons(u16EntryCount);
					psSendLamsMAC->u8FlagEnd = 0;
					(*CouLamps)+=u16EntryCount;
#ifdef SEND_ACK
					result = p_hardware->SendReciv(ip_address,test,1+sizeof(tsSendTable)+(sizeof(tsMAC_Address)*u16EntryCount),HEADER_SIZE + 1);
					if( result == (HEADER_SIZE + 1) ){
						if( p_hardware->Bufer_Rx[HEADER_SIZE] != 	ACK )
							result = -1;
					}else
						result = -1;
#else
					result = p_hardware->SendReciv(ip_address,test,1+sizeof(tsSendTable)+(sizeof(tsMAC_Address)*u16EntryCount),0);// HEADER_SIZE + 1 + 6);

					if(p_hardware->m_iSleepToNextSendTCP)
						Sleep(p_hardware->m_iSleepToNextSendTCP);
#endif
					u16FirstTableEntry += u16EntryCount;
					u16EntryCount = 0;
					psSendLamsMAC = (tsSendTable*)(test+1);
					psMAC_Address = (tsMAC_Address*)(psSendLamsMAC+1);

					if( result < 0 ){
						if (m_pRs)
							if (m_pRs->State == adStateOpen)
								m_pRs->Close();
						return result;
					}
				}

				m_pRs->MoveNext();

			}		
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}	
		if (m_pRs)
			if (m_pRs->State == adStateOpen)
				m_pRs->Close();

		psSendLamsMAC->u16FirstTableEntry = htons(u16FirstTableEntry);
		psSendLamsMAC->u16EntryCount = htons(u16EntryCount);
		psSendLamsMAC->u8FlagEnd = 1;
		(*CouLamps)+=u16EntryCount;
#ifdef SEND_ACK
		result = p_hardware->SendReciv(ip_address,test,1+sizeof(tsSendTable)+(sizeof(tsMAC_Address)*u16EntryCount),HEADER_SIZE + 1);
		if( result == (HEADER_SIZE + 1) ){
			if( p_hardware->Bufer_Rx[HEADER_SIZE] != 	ACK )
				result = -1;
		}else
			result = -1;
#else
		result = p_hardware->SendReciv(ip_address,test,1+sizeof(tsSendTable)+(sizeof(tsMAC_Address)*u16EntryCount),0);// HEADER_SIZE + 1 + 6);
#endif
	}
	return result;
}

int CJennicModule::GetLampsStatus(CString ip_address, UINT32 id_hardware)
{
	int result = -1;

	if( p_hardware != NULL ){
		BYTE test[500];

		tsSendTable * psGetLampsStatus = (tsSendTable*)(test+1);
		psGetLampsStatus->u16FirstTableEntry = htons(0);

		tsSendTable * psSetLampsStatus = (tsSendTable*)(p_hardware->Bufer_Rx + HEADER_SIZE + 1);

		do{
			test[0] = GET_LAMPS_STATUS;
		
			psGetLampsStatus->u16EntryCount = htons(MAX_SEND_LAMP_STATUS);
			psGetLampsStatus->u8FlagEnd = 0;

			result = p_hardware->SendReciv(ip_address,test,1+sizeof(tsSendTable), HEADER_SIZE + 1 + sizeof(tsSendTable));
			if( result > 0 ){
				if( p_hardware->Bufer_Rx[HEADER_SIZE] == SEND_LAMPS_STATUS ){
					tsLampStatus * psLampStatus = (tsLampStatus*)(psSetLampsStatus +1);
					int i = 0;
					for( i = 0 ; i < ntohs(psSetLampsStatus->u16EntryCount) ; i++){
						psGetLampsStatus->u16FirstTableEntry = htons( ntohs(psGetLampsStatus->u16FirstTableEntry)+1);
						
						COleDateTime time( BCD_INT(psLampStatus->sLastContacts.date_time[5])+2000, BCD_INT(psLampStatus->sLastContacts.date_time[4]),BCD_INT(psLampStatus->sLastContacts.date_time[3]),
							BCD_INT(psLampStatus->sLastContacts.date_time[2]),BCD_INT(psLampStatus->sLastContacts.date_time[1]),BCD_INT(psLampStatus->sLastContacts.date_time[0]));
						
						uint32_t	u32WorkHours = ntohl(psLampStatus->u32WorkHours);

						CString MAC;
						MAC.Format("%02X%02X%02X%02X%02X%02X%02X%02X",
							psLampStatus->sMAC_Address.MAC[0],psLampStatus->sMAC_Address.MAC[1],psLampStatus->sMAC_Address.MAC[2],psLampStatus->sMAC_Address.MAC[3],
							psLampStatus->sMAC_Address.MAC[4],psLampStatus->sMAC_Address.MAC[5],psLampStatus->sMAC_Address.MAC[6],psLampStatus->sMAC_Address.MAC[7]);
						int a=5;
						psLampStatus ++;

						_CommandPtr m_pCmd=NULL;
						CString com;
						try{
							com.Format(_T("UPDATE lamps SET last_contact='%s',work_hours=%u,last_test='%s' WHERE MAC_address='%s' AND id_hardware=%u"),::TimeToSQL(time),u32WorkHours,TimeToSQL(COleDateTime::GetCurrentTime()),MAC,id_hardware);
							TESTHR(m_pCmd.CreateInstance(__uuidof(Command)));
							m_pCmd->ActiveConnection=::GetConnection();
							m_pCmd->CommandText=(LPCTSTR)com;
							m_pCmd->Execute(NULL, NULL, adCmdText);	
						}
						catch(_com_error &e){
							::GenerateSqlError(e);
						}
					}
				}
			}
		}while( (result > 0) && ( psSetLampsStatus->u8FlagEnd == 0) );
	}
	return result;
}

int CJennicModule::ClearRAM(CString ip_address)
{
	int result = -1;
	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	COMMAND_CLEAR_RAM;
		result = p_hardware->SendReciv(ip_address,test,1,HEADER_SIZE + 1);

		if( result == (HEADER_SIZE + 1) ){
			if( p_hardware->Bufer_Rx[HEADER_SIZE] != 	ACK )
				result = -1;
		}else
			result = -1;
	}
	return result;
}

int CJennicModule::SetLampLight1(UINT32 id_lamp, unsigned char light)
{
	CString ip_address=_T("");
			
	struct in6_addr lamp_address;
		
	int result = GetLampAddres(&ip_address,&lamp_address,id_lamp);

	if( result >=0){

		uint32_t ModuleID = 0xFFFFFE04;
		uint8_t VariableIndex = 0x02;

		uint8_t buffer[20];

		buffer[0] = VERSION;
		buffer[1] = Set_by_ID_request;
		buffer[2] = rand() & 0x7F;

		buffer[3] = (ModuleID >> 24) & 0xFF;
		buffer[4] = (ModuleID >> 16) & 0xFF;
		buffer[5] = (ModuleID >> 8) & 0xFF;
		buffer[6] = (ModuleID ) & 0xFF;

		buffer[7] = VariableIndex;

		buffer[8] = 0x00;
		buffer[9] = UI8;
		buffer[10] = light;

		result=eJennicModuleSendMessageIPv6(ip_address,&sors,&lamp_address,11,buffer);

		if( result > 0 ){
			int old_result = result;
			result = NO_CONNECT;
			if( p_hardware->Bufer_Rx[HEADER_SIZE] == IPv6_PACKET ){
				uint16_t checksum1, lenght, protocol;
				uint8_t * pu8Data = &p_hardware->Bufer_Rx[HEADER_SIZE+1];
				
				checksum1 = ((uint16_t)(*(pu8Data + 46))) << 8 | *(pu8Data + 47);
				lenght = ((uint16_t)(*(pu8Data + 4))) << 8 | *(pu8Data + 5);
				protocol = *(pu8Data + 6);
				if( lenght == 14 ){
					if( protocol = 17 ){
						if( *(pu8Data + 49) == Set_response ){
							if( *(pu8Data + 53 ) == 0 ){	//sucses
								result = old_result;
							}
						}
					}
				}
			}
		}
	}
	return result;

}

static uint8_t group_address[MAX_GROUP_TIMERS][3]={
	{ 0x15,0xF0,0x0F },
	{ 0x15,0xFE,0x04 },
	{ 0x15,0x1E,0x02 },
	{ 0x15,0x1E,0x03 },

	{ 0x15,0x1E,0x04 },
	{ 0x15,0x1E,0x05 },
	{ 0x15,0x1E,0x06 },
	{ 0x15,0x1E,0x07 },

	{ 0x15,0x1E,0x08 },
	{ 0x15,0x1E,0x09 },
	{ 0x15,0x1E,0x0A },
	{ 0x15,0x1E,0x0B },

	{ 0x15,0x1E,0x0C },
	{ 0x15,0x1E,0x0D },
	{ 0x15,0x1E,0x0E },
	{ 0x15,0x1E,0x0F },
};

int CJennicModule::SetLampGroups1(UINT32 id_lamp, UINT32 bit_groups)
{

	CString ip_address=_T("");
			
	struct in6_addr lamp_address;
		
	int result = GetLampAddres(&ip_address,&lamp_address,id_lamp);

	if( result >=0){

		uint32_t ModuleID = 0xFFFFFF02;
		uint8_t VariableIndex = 0x03;

		uint8_t buffer[20];

		buffer[0] = VERSION;
		buffer[1] = Set_by_ID_request;
		buffer[2] = rand() & 0x7F;

		buffer[3] = (ModuleID >> 24) & 0xFF;
		buffer[4] = (ModuleID >> 16) & 0xFF;
		buffer[5] = (ModuleID >> 8) & 0xFF;
		buffer[6] = (ModuleID ) & 0xFF;

		buffer[7] = VariableIndex;

		buffer[8] = 0x00;
		buffer[9] = UI8;
		buffer[10] = 1;

		result=eJennicModuleSendMessageIPv6(ip_address,&sors,&lamp_address,11,buffer);

		if( result > 0 ){
			int old_result = result;
			result = NO_CONNECT;
			if( p_hardware->Bufer_Rx[HEADER_SIZE] == IPv6_PACKET ){
				uint16_t checksum1, lenght, protocol;
				uint8_t * pu8Data = &p_hardware->Bufer_Rx[HEADER_SIZE+1];
				
				checksum1 = ((uint16_t)(*(pu8Data + 46))) << 8 | *(pu8Data + 47);
				lenght = ((uint16_t)(*(pu8Data + 4))) << 8 | *(pu8Data + 5);
				protocol = *(pu8Data + 6);
				if( lenght == 14 ){
					if( protocol = 17 ){
						if( *(pu8Data + 49) == Set_response ){
							if( *(pu8Data + 53 ) == 0 ){	//sucses
								result = old_result;
								
								VariableIndex = 1;
								buffer[7] = VariableIndex;

								for( int i = 0 ; (i< MAX_GROUP_TIMERS) && (result>0); i++ ){
									if( bit_groups & (0x0001<<i) ){
										buffer[7] = VariableIndex;
										
										buffer[8] = 0x00;
										buffer[9] = BBLOB;
										buffer[10] = 3;
										
										buffer[11] = group_address[i][0];
										buffer[12] = group_address[i][1];
										buffer[13] = group_address[i][2];
										
										result=eJennicModuleSendMessageIPv6(ip_address,&sors,&lamp_address,14,buffer);
										if( result > 0 ){
											old_result = result;
											result = NO_CONNECT;
											if( p_hardware->Bufer_Rx[HEADER_SIZE] == IPv6_PACKET ){
												uint16_t checksum1, lenght, protocol;
												uint8_t * pu8Data = &p_hardware->Bufer_Rx[HEADER_SIZE+1];
												
												checksum1 = ((uint16_t)(*(pu8Data + 46))) << 8 | *(pu8Data + 47);
												lenght = ((uint16_t)(*(pu8Data + 4))) << 8 | *(pu8Data + 5);
												protocol = *(pu8Data + 6);
												if( lenght == 14 ){
													if( protocol = 17 ){
														if( *(pu8Data + 49) == Set_response ){
															if( *(pu8Data + 53 ) == 0 ){	//sucses
																result = old_result;
															}
														}
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return result;
}

void CJennicModule::CreateIPv6Address(in6_addr *lamp_addr, CString MAC_address, UINT32 id_hardware)
{
		lamp_addr->u.Byte[0]=CONFIG_DEFAULT_PREFIX_MSB>>24;
		lamp_addr->u.Byte[1]=(CONFIG_DEFAULT_PREFIX_MSB>>16)&0xFF;
		lamp_addr->u.Byte[2]=(CONFIG_DEFAULT_PREFIX_MSB>>8)&0xFF;
		lamp_addr->u.Byte[3]=CONFIG_DEFAULT_PREFIX_MSB&0xFF;
			
		lamp_addr->u.Byte[4]=CONFIG_DEFAULT_PREFIX_LSB>>24;
		lamp_addr->u.Byte[5]=(CONFIG_DEFAULT_PREFIX_LSB>>16)&0xFF;
		lamp_addr->u.Byte[6]=(id_hardware>>8)&0xFF;
		lamp_addr->u.Byte[7]=id_hardware&0xFF;
			
		unsigned char myStrMAC[20];
		strcpy((char*)myStrMAC, (LPCTSTR)MAC_address);
			
			
		lamp_addr->u.Byte[8]=hexa_to_byte(myStrMAC+0);
		lamp_addr->u.Byte[8] ^= 0x02;
		lamp_addr->u.Byte[9]=hexa_to_byte(myStrMAC+2);
		lamp_addr->u.Byte[10]=hexa_to_byte(myStrMAC+4);
		lamp_addr->u.Byte[11]=hexa_to_byte(myStrMAC+6);
		lamp_addr->u.Byte[12]=hexa_to_byte(myStrMAC+8);
		lamp_addr->u.Byte[13]=hexa_to_byte(myStrMAC+10);
		lamp_addr->u.Byte[14]=hexa_to_byte(myStrMAC+12);
		lamp_addr->u.Byte[15]=hexa_to_byte(myStrMAC+14);
}

int CJennicModule::GetLampAddres(CString *ip_address,in6_addr *lamp_addr, UINT32 id_lamp)
{
	int result = NO_HARDWARE;

	if( p_hardware != NULL ){
		
		UINT32 id_hardware=0;
		CString MAC_address=_T("");
		
		_RecordsetPtr m_pRs=NULL;
		
		CString com;
		com.Format(_T("SELECT lamps.MAC_address,lamps.id_hardware,number_hardware FROM lamps LEFT JOIN hardwares ON hardwares.id_hardware=lamps.id_hardware WHERE id_lamp=%u"),id_lamp);
		try{
			TESTHR(m_pRs.CreateInstance(__uuidof(Recordset)));
			m_pRs->Open((LPCTSTR)com,_variant_t((IDispatch *) ::GetConnection(), true),adOpenForwardOnly,adLockPessimistic,adCmdText);
			
			if(!m_pRs->EndOfFile){		
				_variant_t index;
				index.vt = VT_I2;
				index.iVal = 0;
				
				_variant_t data=m_pRs->Fields->GetItem(&index)->Value;
				MAC_address = ::GetStringFromVariant(data);
				
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				id_hardware = ::GetLongFromVariant(data);
				
				index.iVal ++; data=m_pRs->Fields->GetItem(&index)->Value; 
				*ip_address = ::GetStringFromVariant(data);
			}		
		}
		catch(_com_error &e){
			::GenerateSqlError(e);
		}	
		if (m_pRs)
			if (m_pRs->State == adStateOpen)
				m_pRs->Close();

		if(id_hardware == 0)
			return NO_ROUTER;

		CreateIPv6Address(lamp_addr, MAC_address, id_hardware);
		
		result = 0;
	}

	return result;
}

int CJennicModule::GetLampGroups1(UINT32 id_lamp, UINT32 *bit_groups)
{

	CString ip_address=_T("");
			
	struct in6_addr lamp_address;
		
	int result = GetLampAddres(&ip_address,&lamp_address,id_lamp);

	uint16_t u16FirstTableEntry = 0;
	uint16_t u16OldFirstTableEntry = 1000;
	if( result >= 0  ){//while( ( result >= 0 ) && ( u16OldFirstTableEntry != u16FirstTableEntry ) ){

		u16OldFirstTableEntry = u16FirstTableEntry;

		uint8_t buffer[20];
		uint8_t mouleIndex = 0x02;
		uint8_t VariableIndex = 0x00;
		
		uint8_t u8EntryCount = 255;	//1;//255;
	
		buffer[0] = VERSION;
		buffer[1] = Get_request;
		buffer[2] = rand() & 0x7F;

		buffer[3] = mouleIndex;

		buffer[4] = VariableIndex;

		buffer[5] = (u16FirstTableEntry >> 8) & 0xFF;
		buffer[6] = (u16FirstTableEntry) & 0xFF;;

		buffer[7] = u8EntryCount;

		result=eJennicModuleSendMessageIPv6(ip_address,&sors,&lamp_address,8,buffer);
	
		if( result > 0 ){
			int old_result = result;
			result = NO_CONNECT;
			if( p_hardware->Bufer_Rx[HEADER_SIZE] == IPv6_PACKET ){
				uint16_t checksum1, protocol;
				int16_t i16Lenght;
				uint8_t * pu8Data = &p_hardware->Bufer_Rx[HEADER_SIZE+1];
				
				checksum1 = ((uint16_t)(*(pu8Data + 46))) << 8 | *(pu8Data + 47);
				i16Lenght = ((uint16_t)(*(pu8Data + 4))) << 8 | *(pu8Data + 5);
				protocol = *(pu8Data + 6);
				if( i16Lenght >=19 ){
					if( protocol = 17 ){
						if( *(pu8Data + 49) == Get_response ){
							if( *(pu8Data + 53) == 0){	//sucses
								if( *(pu8Data + 54) == BLOB_TABLE ){	//Blob table
									uint8_t ModuleIndex = *(pu8Data + 51);
									uint8_t FirstVariableIndex = *(pu8Data + 52);
									
									uint16_t u16NumberOffRemainingEntries;
									uint16_t u16TableVersion;
									
									u16NumberOffRemainingEntries = *(pu8Data + 48 + 7); u16NumberOffRemainingEntries <<= 8;
									u16NumberOffRemainingEntries |= *(pu8Data + 48 + 8);
									
									u16TableVersion = *(pu8Data + 48 + 9); u16TableVersion <<= 8;
									u16TableVersion |= *(pu8Data + 48 + 10);
									
									i16Lenght -= 19;
									pu8Data += 40+19;
									
									//*bit_groups = 0;

									while( i16Lenght > 0 ){
										//uint8_t group[200];
										uint16_t EntryIndex = ((uint16_t)(*(pu8Data + 0))) << 8 | *(pu8Data + 1);
										uint8_t BlowLenght = *(pu8Data + 2);
										
										pu8Data += 3;
										if( BlowLenght ==3 ){
											for(int i = 0 ; i < MAX_GROUP_TIMERS ; i++){
												if(memcmp(group_address[i],pu8Data,BlowLenght) == 0){
													*bit_groups |= (0x0001<<i);
													break;
												}
											}
										}

										pu8Data +=BlowLenght;
										i16Lenght -= 3+BlowLenght;

										u16FirstTableEntry ++;
									}

									result = old_result;
								}
							}
						}
					}
				}
			}
		}
	}
	return result;
}

int CJennicModule::GetNeighbourTableBlobs(CString ip_address, CString MAC_address, UINT32 id_hardware, uint16_t u16FirstTableEntry, uint8_t u8EntryCount, uint8_t ** ppu8Data)
{
	struct in6_addr lamp_address;
	int result = 0;
		
	CreateIPv6Address(&lamp_address, MAC_address, id_hardware);

	{
		uint8_t buffer[20];
		uint8_t mouleIndex = 0x01;
		uint8_t VariableIndex = 0x06;
	
		buffer[0] = VERSION;
		buffer[1] = Get_request;
		buffer[2] = rand() & 0x7F;

		buffer[3] = mouleIndex;

		buffer[4] = VariableIndex;

		buffer[5] = (u16FirstTableEntry >> 8) & 0xFF;
		buffer[6] = (u16FirstTableEntry) & 0xFF;;

		buffer[7] = u8EntryCount;

		result=eJennicModuleSendMessageIPv6(ip_address,&sors,&lamp_address,8,buffer,RECIVE_TIMEOUT_GetNeighbourTableBlobs);
	
		if( result > 0 ){
			result = NO_CONNECT;
			if( p_hardware->Bufer_Rx[HEADER_SIZE] == IPv6_PACKET ){
				uint16_t checksum1, protocol;
				int16_t i16Lenght;
				uint8_t *pu8Data = &p_hardware->Bufer_Rx[HEADER_SIZE+1];
				
				checksum1 = ((uint16_t)(*(pu8Data + 46))) << 8 | *(pu8Data + 47);
				i16Lenght = ((uint16_t)(*(pu8Data + 4))) << 8 | *(pu8Data + 5);
				protocol = *(pu8Data + 6);
				if( i16Lenght >=19 ){
					if( protocol == 17 ){
						if( *(pu8Data + 49) == Get_response ){
							if( *(pu8Data + 53) == 0){	//sucses
								if( *(pu8Data + 54) == BLOB_TABLE ){	//Blob table
								/*	uint8_t ModuleIndex = *(pu8Data + 51);
									uint8_t FirstVariableIndex = *(pu8Data + 52);
									
									uint16_t u16NumberOffRemainingEntries;
									uint16_t u16TableVersion;
									
									u16NumberOffRemainingEntries = *(pu8Data + 48 + 7); u16NumberOffRemainingEntries <<= 8;
									u16NumberOffRemainingEntries |= *(pu8Data + 48 + 8);
									
									u16TableVersion = *(pu8Data + 48 + 9); u16TableVersion <<= 8;
									u16TableVersion |= *(pu8Data + 48 + 10);
									*/
									result = i16Lenght -= 19;
									pu8Data += 40+19;
									
									*ppu8Data = pu8Data ;
								}
							}
						}
					}
				}
			}
		}
	}
	return result;
}

int CJennicModule::GetCurrentEnergy(CString ip_address)
{
	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	COMMAND_READ_CURRENT_ENERGY;
		return p_hardware->SendReciv(ip_address,test,1, HEADER_SIZE + 1 + sizeof(tsCurrentEnergy));
	}
	return -1;
}

int CJennicModule::GetTotaltEnergy(CString ip_address)
{
	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	COMMAND_READ_TOTAL_ENERGY;
		return p_hardware->SendReciv(ip_address,test,1, HEADER_SIZE + 1 + sizeof(tsTotalEnergy));
	}
	return -1;
}

int CJennicModule::GetHistoryEnergy(CString ip_address, void *pDataToRead)
{
	tsSendEnergyArray * psSendEnergyArray = (tsSendEnergyArray*)pDataToRead;
	if( p_hardware != NULL ){
		BYTE test[200];

		test[0] =	COOMAND_GET_CURRENT_ENERGY_ARRAY;
		memcpy(test+1,psSendEnergyArray,sizeof(tsSendEnergyArray));
		return p_hardware->SendReciv(ip_address,test,1 + sizeof(tsSendEnergyArray), HEADER_SIZE + 1 + sizeof(tsSendEnergyArray));//,1000);
	}
	return -1;
}
