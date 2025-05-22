// HardwareCom.h: interface for the CHardwareCom class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HARDWARECOM_H__68A4CB1E_6682_4307_923A_376D85B145DB__INCLUDED_)
#define AFX_HARDWARECOM_H__68A4CB1E_6682_4307_923A_376D85B145DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HardwareBase.h"

class CHardwareCom : public CHardwareBase  
{
// Attributes
public:
	CHardwareCom(CString com);


// Operations
protected:
	virtual ~CHardwareCom();
	int SimRead(BYTE *str, unsigned int bytetoread);
	friend int ComThread(CHardwareCom *pHC);
	volatile BOOL fl_work;	//true за да работи таска
	volatile BOOL fl_ended;	// при false таска е приключил
	volatile BOOL fl_ready_to_open;	//при state TO_Open е true
	volatile BOOL fl_opened;	//връзката е отворена с CONNECT OK
	volatile BOOL fl_open;		//true за да отвори връзката
	volatile BOOL fl_close;		//true за да се затвори връзката
	volatile BOOL fl_write;	//true когато е изпратена командата AT+CIPSEND=1 след която може да се изпраща TCP
	volatile BOOL fl_to_write;	//true за да се тартира изпращане на TCP,fase при 1, CLOSE OK и 1, SEND OK\r\n
	volatile BOOL fl_fail;		//грешка CONNECT FAIL или 1, ALREADY CONNECT
	CString m_sCOM;
	int SimWrite(BYTE * str,BOOL clearReciver=true);
	void ConfigCom();
	virtual int JennicTrxRtx(BYTE *p_DataEnd, int RecivLenght, uint32_t timeout = 0,BOOL clearReciver=true);
	void Flush();
	HANDLE m_hFile;
	virtual void Close();
	HANDLE Open();
	int NumHardware;
	int recived;
	int to_reciv;
	volatile int RecivedLenght;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CHardwareCom)
	protected:

	//}}AFX_VIRTUAL

// Implementation

};

#endif // !defined(AFX_HARDWARECOM_H__68A4CB1E_6682_4307_923A_376D85B145DB__INCLUDED_)
