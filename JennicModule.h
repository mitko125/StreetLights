// JennicModule.h: interface for the CJennicModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_JENNICMODULE_H__CB7E9315_46C9_4502_924F_734398B5A7C6__INCLUDED_)
#define AFX_JENNICMODULE_H__CB7E9315_46C9_4502_924F_734398B5A7C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#define IP_HOST "127.0.0.1"
//#define IP_HOST	"172.16.0.15"//49"//49"

//Твърдица
#define PIN "9971" //Твърдица
#define APN "tvulosv" //Твърдица
//Средец , оказа се че в средец картите са без PIN, а APN е без значение всичко зависи от картите
//#define PIN "0000" //Средец
//#define APN "realp" //Средец
#define COM_NUMBER "COM22"

//#include <stdint.h>
typedef signed char        int8_t;
typedef short              int16_t;
typedef int                int32_t;
//typedef long long          int64_t;
typedef unsigned char      uint8_t;
typedef unsigned short     uint16_t;
typedef unsigned int       uint32_t;
//typedef unsigned long long uint64_t;

#define INT_BCD(A) ( ((A/10)<<4) | (A%10) )
#define BCD_INT(A) ( ((A>>4)*10) + (A&0x0F) )

#define SOURCE_PORT 0xD3ED

#define JENNIC_PORT 0x0751
#define HEADER_SIZE 3
#define VERSION 0

#define BUFERS_LENGHT (2048 + HEADER_SIZE)



#define Get_request 0x10 //Request to obtain the value of a variable
#define Get_by_ID_request 0x1C	//Request to obtain the value of a variable in the module with specified ID
#define Get_response 0x11 //Response to a previous ‘Get’ or ‘Get by ID’ request
//‘Set’ request 0x12 Request to set the value of a variable
#define Set_by_ID_request 0x1D //Request to set the value of a variable in the module with specified ID
#define Set_response 0x13 //Response to a previous ‘Set’ or ‘Set by ID’ request
//‘Query Modules’ request 0x14 Request to query the JenNet-IP database for a list of available modules
//‘Query Modules’ response 0x15 Response to a previous ‘Query Modules’ request
//‘Query Variables’ request 0x16 Request to query the JenNet-IP database for a list of available variables within a module
//‘Query Variables’ response 0x17 Response to a previous ‘Query Variables’ request
//‘Trap’ request 0x18 Request to enable the generation of a notification when a variable changes value
//‘Untrap’ request 0x19 Request to disable the generation of a notification when a variable changes value
//‘Trap’ response 0x1A Response to a previous ‘Trap’ request
//‘Trap’ notification 0x1B Notification of a change in a trapped variable


//0x00 8-bit signed integer
//0x01 16-bit signed integer
//0x02 32-bit signed integer
//0x03 64-bit signed integer
#define UI8 0x04 //8-bit unsigned integer
//0x05 16-bit unsigned integer
//0x06 32-bit unsigned integer
//0x07 64-bit unsigned integer
//0x08 32-bit IEEE 754 float
//0x09 64-bit IEEE 754 double
//0x0A Text string
#define BBLOB 0x0B //Binary blob
#define BLOB_TABLE 0x4B //Blob table

typedef enum
{
	COMMAND_SET_HOST_DATA	=	1,
	IPv6_PACKET						=	2,
	COMMAND_ON						= 3,
	COMMAND_OFF						= 4,
	COMMAND_TIME_ON_OFF		= 5,	//old
	GET_STATUS_ROUTER			= 6,
	SEND_STATUS_ROUTER		= 7,
	COMMAND_GET_HOST_DATA	=	8,
	COMMAND_MAC_ADDRESS		=	9,
	COMMAND_SET_TIMERS		=	10,	//& data time
	SEND_DATE_TIME				= 11,
	COMMAND_GET_TIMERS		= 12,
	COMMAND_GET_REJECT		= 13,
	SEND_REJECT_TABLE			= 14,
	SEND_LAMPS_MAC_TABLE	= 15,
	GET_LAMPS_STATUS			= 16,
	SEND_LAMPS_STATUS			= 17,
	SET_WORK_HOURS				= 18,
	COMMAND_CLEAR_RAM			= 19,
	ACK										= 20,
	COMMAND_READ_CURRENT_ENERGY	= 21,
	COMMAND_READ_TOTAL_ENERGY		= 22,
	//V7
	GET_MY_IP_FROM_SERVER = 23,
	SET_MY_IP_TO_CLIENT = 24,
	COOMAND_GET_CURRENT_ENERGY_ARRAY	= 32,		//V3 трябваше да е 23 но е сгрешено и е по обекти, да се заемат по-ниски номера
} teCommandsPC;

typedef enum
{
    E_REGION_EUROPE,
    E_REGION_USA,
    E_REGION_JAPAN,
    
    E_REGION_MAX
} teRegion;

/* Default network configuration */
#define CONFIG_DEFAULT_CHANNEL                          0
#define CONFIG_DEFAULT_PAN_ID                           0x1111
#define CONFIG_DEFAULT_NETWORK_ID                       0x11111111
#define CONFIG_DEFAULT_PREFIX_MSB                       0xfd040bd3L
#define CONFIG_DEFAULT_PREFIX_LSB												0x80e80000L
#define CONFIG_DEFAULT_REGION                           E_REGION_EUROPE
#define CONFIG_DEFAULT_PROFILE                          4	//0

#define MAX_TCP_IP4_CLIENTS 8
struct S_un_b {
	uint8_t s_b1;
	uint8_t s_b2;
	uint8_t s_b3;
	uint8_t s_b4;
};

struct S_un {
	union {
		struct S_un_b S_un_b;
		uint32_t S_addr;
	};
};


typedef struct
{
#pragma pack(push, 1)
	struct S_un S_un;
#pragma pack(pop)
}/* __attribute__((__packed__))*/ sin_addr;
//!!!	 Промяна при преминаване към ProjectsVS2015 тука структората я има в in6addr.h файла
/*
struct in6_addr {
	union {
		u_char  Byte[16];
		u_short Word[8];
	} u;
};
*/

typedef enum
{
    E_CHANNEL_AUTOMATIC     = 0,
    E_CHANNEL_MINIMUM       = 11,
    E_CHANNEL_MAXIMUM       = 26
} teChannel;

typedef enum
{
    E_FRONTEND_STANDARD_POWER,          /**< No frontend - just a standard power device */
    E_FRONTEND_HIGH_POWER,              /**< High power module - enable PA and LNA */
    E_FRONTEND_ETSI,                    /**< Enable ETSI compliant mode */
}teRadioFrontEnd;

/** Enumerated type of supported authorisation schemes */
typedef enum
{
    E_AUTH_SCHEME_NONE,
    E_AUTH_SCHEME_RADIUS_PAP,

		E_AUTH_SCHEME_DUMMY = 2147483647,	/**< Force this emumeration to be 4 bytes as sent by the host. */
} teAuthScheme;

typedef struct
{
#pragma pack(push, 1)
	uint8_t     u8Region;
	uint8_t     u8Channel;
	uint16_t    u16PanID;
	uint32_t    u32NetworkID;
	uint32_t    u64NetworkPrefixMSB;
	uint32_t    u64NetworkPrefixLSB;
#pragma pack(pop)
}/* __attribute__((__packed__))*/ tsModule_ConfigV11;

typedef union
{
    struct
    {
        struct in6_addr sAuthServerIP;
    } sRadiusPAP;
} tuAuthSchemeData;

typedef struct
{
#pragma pack(push, 1)
	struct in6_addr		sKey;   

	teAuthScheme			eAuthScheme;
	tuAuthSchemeData	uAuthSchemeData;
#pragma pack(pop)
} /*__attribute__((__packed__))*/ tsSecurityConfig;

typedef struct
{
#pragma pack(push, 1)
	tsModule_ConfigV11	sModuleConfigV11;
	tsSecurityConfig		sSecurityConfig;
	uint8_t							eRadioFrontEnd;
	uint8_t							u8JenNetProfile;
	uint8_t							iAntennaDiversity;
	uint8_t							u8RadiusOff;
	uint16_t						u16LampsInTable;
	uint16_t						u16LampsConnected;
	//V5
	uint8_t				u8INT_resetGPRShours;
	uint8_t				u8INT_resetGPRSminuts;
	uint8_t				u8EnableEnergyMeter;
#pragma pack(pop)
}/* __attribute__((__packed__))*/ tsConfigBorderRuter;	//58 + 3 V3, V5 Host

typedef struct
{
#pragma pack(push, 1)
	uint8_t     u8Hour;
	uint8_t     u8Minute;
#pragma pack(pop)
} /*__attribute__((__packed__))*/ tsTimerHourMinute;

typedef struct
{
#pragma pack(push, 1)
	tsTimerHourMinute sTimerOn;
	tsTimerHourMinute sTimerOff;
	uint8_t     u8Lights;
#pragma pack(pop)
} /*__attribute__((__packed__))*/ tsGrourTimer;

typedef struct
{
#pragma pack(push, 1)
	unsigned char date_time[6];
#pragma pack(pop)
} /*__attribute__((__packed__))*/ tsDateTime;

#define MAX_GROUP_TIMERS 16
#define MAX_GROUP_DATA_BASE 16

typedef struct
{
#pragma pack(push, 1)
	tsDateTime sDateTime;
	tsTimerHourMinute sTimerOn1;
	tsTimerHourMinute sTimerOff1;
	tsGrourTimer sGroupTimer[MAX_GROUP_TIMERS];
	//V2
	tsTimerHourMinute sTimerOn2;
	tsTimerHourMinute sTimerOff2;
#pragma pack(pop)
} /*__attribute__((__packed__))*/ tsTimers;

typedef struct
{
	#pragma pack(push, 1)
	uint8_t MAC[8];
	#pragma pack(pop)
} /*__attribute__((__packed__))*/ tsMAC_Address;

#define MAX_ACCESS_REJECT_TABLE 16	//25

typedef struct
{
	#pragma pack(push, 1)
	tsMAC_Address sReject[MAX_ACCESS_REJECT_TABLE];
	#pragma pack(pop)
} /*__attribute__((__packed__))*/ tsMAC_Reject;

typedef struct
{
#pragma pack(push, 1)
	uint16_t u16FirstTableEntry;
	uint16_t u16EntryCount;
	uint8_t u8FlagEnd;
#pragma pack(pop)
} /*__attribute__((__packed__))*/ tsSendTable;

#define ROUTE_TABLE_ENTRIES		300	//Profile 4 е за  50-150 устройства но го оставяме на 300
#define MAX_SEND_MAC					20
#define MAX_SEND_LAMP_STATUS	10				

typedef struct
{
#pragma pack(push, 1)
	tsMAC_Address	sMAC_Address;
	tsDateTime	sLastContacts;
	uint32_t	u32WorkHours;
#pragma pack(pop)
} /*__attribute__((__packed__))*/ tsLampStatus;

typedef struct
{
#pragma pack(push, 1)
	tsMAC_Address	sMAC_Address;
	uint32_t	u32WorkHours;
#pragma pack(pop)
} /*__attribute__((__packed__))*/ tsSetWorkHours;

typedef struct
{
#pragma pack(push, 1)
	uint16_t u16SimErrors;
	uint16_t u16OnCounter;
	tsDateTime sDateTimeClearRAM;
	tsDateTime sDateTimeOn;
	tsDateTime sDateTimeOff;
	tsDateTime sLastDateTime;
	uint8_t u8CSQ;
	uint8_t u8JenniceModuleState;
	uint32_t u32HostVersion;
	uint32_t u32JennicDeviceVersion;
	uint8_t u8Inputs;
	uint8_t u8Outputs;
	tsDateTime sDateTimeResetGPRS;	// > V3.0.0
	tsDateTime sDateTimeLastClient;	// > V3.0.0	PC няма нужда да го чете,чете се само от Концентратора
#pragma pack(pop)
} /*__attribute__((__packed__))*/tsRouterStatus;


typedef struct
{
#pragma pack(push, 1)
	tsDateTime	sLastContact;
	
	float L1_Voltage;
	float L2_Voltage;
	float L3_Voltage;
	float Grid_frequency;
	float L1_Current;
	float L2_Current;
	float L3_Current;
	float Active_power;
	float L1_Active_power;
	float L2_Active_power;
	float L3_Active_power;
	float Reactive_power;
	float L1_Reactive_power;
	float L2_Reactive_power;
	float L3_Reactive_power;
	float Apparent_power;
	float L1_Apparent_power;
	float L2_Apparent_power;
	float L3_Apparent_power;
	float Power_factor;
	float L1_Power_factor;
	float L2_Power_factor;
	float L3_Power_factor;
#pragma pack(pop)
} /*__attribute__((__packed__))*/tsCurrentEnergy;


typedef struct
{
#pragma pack(push, 1)
	tsDateTime	sLastContact;
	
	float Total_active_energy;
	float T1_Total_active_energy;
	float T2_Total_active_energy;
	float L1_Total_active_energy;
	float L2_Total_active_energy;
	float L3_Total_active_energy;
	
	float Total_reactive_energy;
	float T1_Total_reactive_energy;
	float T2_Total_reactive_energy;
	float L1_Total_reactive_energy;
	float L2_Total_reactive_energy;
	float L3_Total_reactive_energy;
	
#pragma pack(pop)
} /*__attribute__((__packed__))*/tsTotalEnergy;


typedef struct
{
#pragma pack(push, 1)
	tsDateTime	DateTime;
	
	/*float L1_Voltage;
	float L2_Voltage;
	float L3_Voltage;
	float Grid_frequency;
	float L1_Current;
	float L2_Current;
	float L3_Current;*/
	float Active_power;
	float L1_Active_power;
	float L2_Active_power;
	float L3_Active_power;
/*	float Reactive_power;
	float L1_Reactive_power;
	float L2_Reactive_power;
	float L3_Reactive_power;
	float Apparent_power;
	float L1_Apparent_power;
	float L2_Apparent_power;
	float L3_Apparent_power;
	float Power_factor;
	float L1_Power_factor;
	float L2_Power_factor;
	float L3_Power_factor;*/
	
#pragma pack(pop)
} /*__attribute__((__packed__))*/tsCurrentEnergySmall;

#define MAX_CURRENT_ENERGY 450
#define MAX_SEND_CURRENT_ENEGY_ARRAY 10

typedef struct
{
#pragma pack(push, 1)
	uint16_t	NextInArray;
	tsCurrentEnergySmall sCurrentEnergySmall[MAX_CURRENT_ENERGY];
#pragma pack(pop)
}tsCurrenEnegryArray;

typedef struct
{
#pragma pack(push, 1)
	tsDateTime	reversDateTimeStart;
	tsDateTime	reversDateTimeEnd;
	uint16_t u16FirstArrayEntry;
	uint16_t u16EntryArrayCount;
	uint8_t u8FlagEnd;
#pragma pack(pop)
} /*__attribute__((__packed__))*/tsSendEnergyArray;


typedef enum
{
	UNCNOW_ERROR = -1,
	NO_ROUTER = -5,
	NO_HARDWARE = -6,
	NO_CONNECT = -7,
	NO_OPEN = -8,
	NO_WRITE = -9,
	RECIVE_TIMEOUT = -10,
	NO_SOCKET = -11,
}teJennicSucses;

class CJennicModule  
{
public:
	int GetHistoryEnergy(CString ip_address,void * pDataToRead);
	int GetTotaltEnergy(CString ip_address);
	int GetCurrentEnergy(CString ip_address);
	int GetNeighbourTableBlobs(CString ip_address,CString MAC_address,UINT32 id_hardware,uint16_t u16FirstTableEntry,uint8_t u8EntryCount,uint8_t ** ppu8Data);
	int SetLampGroups1(UINT32 id_lamp, UINT32 bit_groups);
	int GetLampGroups1(UINT32 id_lamp, UINT32 *bit_groups);
	int SetLampLight1(UINT32 id_lamp, unsigned char light);
	int ClearRAM(CString ip_address);
	int GetLampsStatus(CString ip_address,UINT32 id_hardware);
	uint8_t hexa_to_byte(uint8_t  *u8Data);
	int SetLampsMAC(CString ip_address,UINT32 id_hardware,int *CouLamps);
	int GetReject(CString ip_address);
	int GetTimers(CString ip_address);
	int SetTimers(CString ip_address,UINT32 id_timer);
	int GetMAC(CString ip_address);
	int GetWPAN(CString ip_address);
	int SetWPAN(UINT32 id_hardware);
	int SimErrors(CString ip_address);
	int OnOffTime(CString ip_address,int OnHour,int OnMinute,int OffHour, int OffMinute);
	int OffRelay(CString ip_address);
	int OnRelay(CString ip_address);
	int BroadcastLight(CString ip_address,unsigned char light);
	CJennicModule();
	virtual ~CJennicModule();

protected:
	void CreateIPv6Address(in6_addr *lamp_addr,CString MAC_address,UINT32 id_hardware);
	int GetLampAddres(CString *ip_address,in6_addr *lamp_addr,UINT32 id_lamp);
	uint16_t CalculateChecsum(uint32_t u32Length, uint8_t *pu8Data);
	int eJennicModuleSendMessageIPv6(CString ip_address,struct in6_addr *source_addr,struct in6_addr *dest_addr,uint32_t u32Length, uint8_t *pu8Data, uint32_t timeout = 0);
	int GlobalSetUint8ByModuleID(CString ip_address,uint32_t ModuleID, uint8_t VariableIndex, uint8_t data);
};

#endif // !defined(AFX_JENNICMODULE_H__CB7E9315_46C9_4502_924F_734398B5A7C6__INCLUDED_)
