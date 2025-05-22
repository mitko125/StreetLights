// ServerTCP.h: interface for the CServerTCP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SERVERTCP_H__7571993C_28D1_4936_AC72_E377B3F8754A__INCLUDED_)
#define AFX_SERVERTCP_H__7571993C_28D1_4936_AC72_E377B3F8754A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CServerTCP  
{
public:
	CServerTCP();
	virtual ~CServerTCP();

protected:
	void TestLinkMessage(LPCSTR str, BOOL ok=true);
	CWnd * m_pTestLinkView;
	UINT32 m_Flags;
	friend int ServerTCP_Thread(void);
	friend int SexToClient(int * client);
	friend int AutoOnOff_Thread(void);
	volatile BOOL fl_work;	//true за да работи таска
	volatile BOOL fl_ended;	// при false таска е приключил

};

#endif // !defined(AFX_SERVERTCP_H__7571993C_28D1_4936_AC72_E377B3F8754A__INCLUDED_)
