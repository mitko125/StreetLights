#if !defined(AFX_HARDWAREBASE_H__A392DFEC_71F0_40C3_BE46_66F5AC8398A6__INCLUDED_)
#define AFX_HARDWAREBASE_H__A392DFEC_71F0_40C3_BE46_66F5AC8398A6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// HardwareBase.h : header file
//

#include "JennicModule.h"
/////////////////////////////////////////////////////////////////////////////

// CHardwareBase thread
#define FL_TEST_LINK_VIEW			0x00000003UL
#define FL_VIEW_DATA					0x00000002UL

#define TCP_OPEN_TIME_OUT 20000 //90000
#define SIM_OPEN_TIME_OUT 90000 //90000
#define TCP_RECIVE_TIMEOUT 10000 //20000 //V3=20000,...olds = 30000;//1000;//30000
#define SIM_RECIVE_TIMEOUT 10000 //30000
#define SIM_SEND_TIMEOUT 10000 //30000
#define RECIVE_TIMEOUT_GetNeighbourTableBlobs	 10000
#define TIME_TO_CLOSE_CONNECTION 3*(60000)	//3min 

class CHardwareBase
{
protected:
	CHardwareBase();           // protected constructor used by dynamic creation

// Attributes
public:
	int m_iSleepToNextSendTCP;
// Operations
public:


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHardwareBase)
	protected:
	//}}AFX_VIRTUAL

// Implementation
protected:
	CWnd * m_pTestLinkView;
	void TestLinkMessage(LPCSTR str, BOOL ok=true);
	int m_iTimeOut;
	int m_iRTU_TimeOut;
	BYTE Bufer_Tx[BUFERS_LENGHT];
	BYTE SimBufer_Tx[BUFERS_LENGHT];
public:	
	CString m_sConnectServer;
	BYTE Bufer_Rx[BUFERS_LENGHT];
	virtual ~CHardwareBase();
	virtual void Close();
	int SendReciv(CString ip_address,BYTE *p_data, int SendLenght, int RecivLenght = 0, uint32_t timeout = 0,BYTE version = VERSION);
	UINT32 m_Flags;

protected:
	virtual void Flush(){};
	CString m_sTCP_Server;
	int MakeJennicCommand(BYTE * p_B , int RecivLenght, uint32_t timeout);
	virtual int JennicTrxRtx(BYTE *p_DataEnd, int RecivLenght, uint32_t timeout = 0,BOOL clearReciver=true) {return 0;};
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_HARDWAREBASE_H__A392DFEC_71F0_40C3_BE46_66F5AC8398A6__INCLUDED_)
