// Messages.h: 
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATE3_H__7842FE55_D4A0_4CEA_A328_022C4F439068__INCLUDED_)
#define AFX_DATE3_H__7842FE55_D4A0_4CEA_A328_022C4F439068__INCLUDED_

#include "MyCurrency.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//MainApp
#define MW_COM_UNK_ERR				(WM_USER+101)	// Unknown error occurred
#define MW_COM_NOT_AVAILABE		(WM_USER+102)	// Port is not present	по погрешка попадаше MW_INPUT_STR тук и и ги смених
//от WM_USER+2 на +100
#define MW_COM_IN_USE					(WM_USER+103)	// Port is in use
#define MW_TCP_BAD_CONNECT		(WM_USER+104)	// Socket error
#define MW_AUTO_UPDATE_TIMER	(WM_USER+105)

//CTestLinkView
#define MW_OUTPUT_STR					(WM_USER+1)
#define MW_INPUT_STR					(WM_USER+2)
#define MW_NOT_READ_INP_STR		(WM_USER+3)
#define MW_PROTOCOL_STR_OK		(WM_USER+4)
#define MW_PROTOCOL_STR_ERR		(WM_USER+5)
#define MW_PROTOCOL_DISCONECT	(WM_USER+6)
#define MW_MARGING_PTR				(WM_USER+7)
#define MW_PROTOCOL_ID				(WM_USER+8)

//CProtcols Dispensers
#define MW_TEST_VIEW_DISCONECT	(WM_USER+1)
#define MW_SET_COMMAND					(WM_USER+2)
#define MW_PROG_DEVICE					(WM_USER+3)
#define MW_EXTENDET_VIEW				(WM_USER+4)
#define MW_STOP_ALL							(WM_USER+5)
#define MW_NEW_DECORY						(WM_USER+6)

#define MW_PROTOCOL_IN_IDLE			(WM_USER+14)

//CDispenserViewBase
#define MW_HARDWARE_STOPED				(WM_USER+1)
#define MW_REDRAW_FULL_SATUS			(WM_USER+2)
#define MW_ERR_CONNECT						(WM_USER+3)
#define MW_END_PROGRAM						(WM_USER+4)
/*#define MW_OLD_POSITION_NAME			(WM_USER+2)
#define MW_REDRAW_CURENT_TRANSACTION	(WM_USER+10)
#define MW_REDRAW_NAME_FUELS			(WM_USER+11)
#define MW_REDRAW_RESPONDING			(WM_USER+12)
#define MW_REDRAW_STATUS					(WM_USER+13)
#define MW_TRANSACTION						(WM_USER+14)
#define MW_REDRAW_PRESET					(WM_USER+15)
#define MW_MESSAGE_FROM_PROTOCOL	(WM_USER+16)
#define MW_REDRAW_CLIENT_RELEASE	(WM_USER+17)*/

//Command Registers
#define OFFSET_TIMEOT_REGISTER 27
#define OFFSET_LIMIT_SOFT_UP 28
#define OFFSET_LIMIT_SOFT_DOWN 29
#define OFFSET_TARGET_POS 30
#define OFFSET_REMOTECOMANDS 31
#define BIT_REMOTE_MOVE_UP			0x0001
#define BIT_REMOTE_MOVE_DOWN		0x0002
#define BIT_REMOTE_MOVE_FAST		0x0004
#define BIT_REMOTE_REPER				0x0008
#define BIT_REMOTE_POSITION			0x0010

#define BIT_REMOTE_MOVE_LEFT		0x0020
#define BIT_REMOTE_MOVE_RIGHT		0x0040

typedef struct{
	int TargetPos;
	unsigned int RemoteCommands;
}COMMANDS;

//Status Registers
#define STATUS_OFFSET 32

#define CONTROLER_STATUS_OFFSET STATUS_OFFSET
#define BIT_LIMIT_UP		0x0001
#define BIT_LIMIT_DOWN	0x0002
#define BIT_MOVE_LEFT		0x0004	//#define BIT_LOCAL_UP 		0x0004
#define BIT_MOVE_RIGHT	0x0008	//#define BIT_LOCAL_DOWN	0x0008
#define BIT_AC_READY		0x0010
#define BIT_ACK					0x0020
#define BIT_LIMIT_SOFT	0x0040
#define BIT_ERR_VAGE		0x0080
#define BIT_MOVE_UP			0x0100
#define BIT_MOVE_DOWN		0x0200
#define BIT_LIMIT_LEFT	0x0400	//#define BIT_ERR_REMAK		0x0400

#define BIT_LIMIT_ERR		0x0800
#define BIT_Q10					0x1000
#define BIT_OVERLOAD		0x2000
#define BIT_LIMIT_RIGHT	0x4000

#define BIT_INIT				0x8000

#define CURRENT_POS_OFFSET CONTROLER_STATUS_OFFSET+1
#define VERSION_OFFSET CURRENT_POS_OFFSET+1
#define OFFSET_ERR_PULSES VERSION_OFFSET+1
#define OFFSET_LENGHT_PULSES OFFSET_ERR_PULSES+1
#define OFFSET_AC_MODBUS_READ_REGISTERS OFFSET_LENGHT_PULSES+2
#define AC_COU_REGISTERS 4
#define OFFSET_WRITE_DAC_TO_AC OFFSET_AC_MODBUS_READ_REGISTERS+AC_COU_REGISTERS
#define OFFSET_ADC_DC	OFFSET_WRITE_DAC_TO_AC+1
#define OFFSET_ADC_VDD	OFFSET_ADC_DC+1
#define OFFSET_50Hz	OFFSET_ADC_VDD+1

#define LENGHT_FULL_STATUS 13
#define LENGHT_SMAL_STATUS 3
typedef struct{
	CMyCurrency Position;
	unsigned int ControlerStatus;
	int CurrentPos;
	unsigned int Version;
	unsigned int Err_Pulses;
	long int Lenght_Pulses;
	unsigned int StatusAC;
	int RpmAC;
	int CurrentAC;
	int HercAC;
	unsigned int Dac_to_AC_Register;
	unsigned int Adc_DC_Volts;
	unsigned int Adc_VDD_Volts;
	unsigned int Time50Hz;
}FULL_STATUS;

typedef struct{
	CMyCurrency Offset;
	CMyCurrency Min;
	CMyCurrency Max;
}SETINGS;

#endif // !defined(AFX_DATE3_H__7842FE55_D4A0_4CEA_A328_022C4F439068__INCLUDED_)