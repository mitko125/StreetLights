// OperatorManager.h: interface for the COperatorManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPERATORMANAGER_H__CC855640_C939_431C_969A_1AB127B580A4__INCLUDED_)
#define AFX_OPERATORMANAGER_H__CC855640_C939_431C_969A_1AB127B580A4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SERVIZ 1
#define CONFIG 2
class COperatorManager  
{
public:
	BOOL SetOperatorPrivilegies(UINT32 id_operator);
	BOOL OperatorSelect(BOOL first_time=false);
	COperatorManager();
	virtual ~COperatorManager();

};

#endif // !defined(AFX_OPERATORMANAGER_H__CC855640_C939_431C_969A_1AB127B580A4__INCLUDED_)
