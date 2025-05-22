// HardwareTCP.h: interface for the CHardwareTCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HARDWARETCP_H__3272EE52_F457_4AA3_8C46_F6C40CB6F6A2__INCLUDED_)
#define AFX_HARDWARETCP_H__3272EE52_F457_4AA3_8C46_F6C40CB6F6A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HardwareBase.h"

class CHardwareTCP : public CHardwareBase  
{
// Attributes
public:
	CHardwareTCP();


// Operations
protected:
	int m_iTimeOutOpen;
	virtual ~CHardwareTCP();
	int MyWrite(BYTE *str, DWORD length);
	BOOL bSendedSokedError;
	BOOL Open();
	virtual void Close();
	SOCKET m_hSocket;
	virtual int JennicTrxRtx(BYTE *p_DataEnd, int RecivLenght, uint32_t timeout = 0,BOOL clearReciver=true);

	TIMEVAL tv_timeout_modbus;
	TIMEVAL tv_timeout_modbus_open;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHardwareTCP)
	protected:

	//}}AFX_VIRTUAL

// Implementation

};

#endif // !defined(AFX_HARDWARETCP_H__3272EE52_F457_4AA3_8C46_F6C40CB6F6A2__INCLUDED_)
