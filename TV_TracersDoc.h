// TV_TracersDoc.h : interface of the CTV_TracersDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TV_TRACERSDOC_H__FE2DD361_EFFA_497C_9869_74CC7DD78A95__INCLUDED_)
#define AFX_TV_TRACERSDOC_H__FE2DD361_EFFA_497C_9869_74CC7DD78A95__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Message.h"

class CTV_TracersDoc : public CDocument
{
protected: // create from serialization only
	CTV_TracersDoc();
	DECLARE_DYNCREATE(CTV_TracersDoc)

// Attributes
public:
	static CString m_strPathNameTracer;
	BOOL program_ok,program_err;
	FULL_STATUS FullStatus;
	SETINGS Setings;
// Operations
public:
	UINT32 id_device;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTV_TracersDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bInGroup;
	CMyCurrency StoredPosition;
	CString m_sStoredPosition;
	void OutGroup();
	void InGroup();
	CString GetName();
	COLORREF colorBk;
	COLORREF colorTx;
	BOOL m_bGoCommand;
	BOOL m_bToReperOrPos;
	BOOL m_bToDown;
	BOOL m_bToUp;
	DWORD dwStartToDwonOrUp; 
	BOOL m_bConnected;
	void GoToPos(CMyCurrency new_pos);
	CString m_sPosition;
	CString m_sOffset,m_sMin,m_sMax;
	void InitSetings();
	CString help1,help2;
	void KillFocus();
	void EndProgram(BOOL fl_ok);
	void SetExtendetView(BOOL fl_Extend);
	unsigned int RemoteCommands;
	BOOL SetHardwareCommand(COMMANDS * Command);
	void ErrConnect();
	void HardwareStoped(BOOL CloseWindows);
	void NewFullStatus(FULL_STATUS *pFS,int cou_data);
	int m_iModbusAddres;
	WORD IdHardware;
	virtual ~CTV_TracersDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	virtual BOOL SaveModified();
	BOOL OpenDocument(CString name);
	virtual afx_msg BOOL OnOpenRecentFile(UINT nID);
	virtual BOOL DoFileSave();
	virtual BOOL DoSave(LPCTSTR lpszPathName, BOOL bReplace = TRUE);
	void SetStatus(device_messages Satus);
	void StoreStatus(device_messages Satus);
	device_messages m_nStatus,m_nOldStatus;
	unsigned int OldControlerStatus;
	//{{AFX_MSG(CTV_TracersDoc)
	afx_msg void OnFileOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TV_TRACERSDOC_H__FE2DD361_EFFA_497C_9869_74CC7DD78A95__INCLUDED_)
